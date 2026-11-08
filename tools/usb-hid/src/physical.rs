// SPDX-License-Identifier: GPL-2.0-only
use crate::layout::{Layout, invalid};
use std::{
    collections::BTreeMap,
    fs::{self, File, OpenOptions},
    io::{self, Write},
    os::{fd::AsRawFd, unix::fs::OpenOptionsExt},
    path::Path,
    sync::atomic::{AtomicU8, AtomicU64, Ordering, fence},
};

#[derive(Debug)]
pub(crate) struct Physical {
    pub(crate) file: File,
    pub(crate) descriptor: Vec<u8>,
    pub(crate) layout: Layout,
    latest: BTreeMap<u8, CachedInput>,
    planned: BTreeMap<u8, CachedInput>,
    completed: BTreeMap<u8, CachedInput>,
    pub(crate) name: String,
}

/// 一个采集线程写入，控制端点读取快照不阻塞数据线程。
#[derive(Debug)]
struct CachedInput {
    bytes: [AtomicU8; 64],
    version: AtomicU64,
    length: usize,
}

impl CachedInput {
    fn new(length: usize) -> Self {
        Self {
            bytes: std::array::from_fn(|_| AtomicU8::new(0)),
            version: AtomicU64::new(0),
            length,
        }
    }

    fn update(&self, data: &[u8]) {
        self.version.fetch_add(1, Ordering::AcqRel);
        // 读取到新字节的快照也必须观察到写入中的奇数版本；
        // 在弱内存顺序CPU上与快照的获取屏障配对。
        fence(Ordering::Release);
        for (byte, value) in self.bytes.iter().zip(data) {
            byte.store(*value, Ordering::Relaxed);
        }
        self.version.fetch_add(1, Ordering::Release);
    }

    fn snapshot(&self) -> Option<Vec<u8>> {
        let mut data = vec![0; self.length];
        // 与高速输入并发时限制快照重试次数。
        for _ in 0..8 {
            let first = self.version.load(Ordering::Acquire);
            if first == 0 {
                return None;
            }
            if first & 1 != 0 {
                std::hint::spin_loop();
                continue;
            }
            for (value, byte) in data.iter_mut().zip(&self.bytes) {
                *value = byte.load(Ordering::Relaxed);
            }
            fence(Ordering::Acquire);
            if self.version.load(Ordering::Relaxed) == first {
                return Some(data);
            }
        }
        None
    }
}

impl Physical {
    #[cfg(test)]
    pub(crate) fn fixture(descriptor: &[u8]) -> io::Result<Self> {
        let layout = Layout::parse(descriptor)?;
        let cache =
            || layout.bytes[0].iter().map(|(&id, &len)| (id, CachedInput::new(len))).collect();
        Ok(Self {
            file: OpenOptions::new().read(true).write(true).open("/dev/null")?,
            descriptor: descriptor.to_vec(),
            latest: cache(),
            planned: cache(),
            completed: cache(),
            layout,
            name: "fixture".into(),
        })
    }
    pub(crate) fn open(path: &Path) -> io::Result<Self> {
        let file =
            OpenOptions::new().read(true).write(true).custom_flags(libc::O_NONBLOCK).open(path)?;
        let mut size = 0_i32;
        // SAFETY：此ioctl向给定指针写入一个整数。
        if unsafe { libc::ioctl(file.as_raw_fd(), 0x80044801_u64 as libc::c_ulong, &raw mut size) }
            < 0
        {
            return Err(io::Error::last_os_error());
        }
        if !(1..=4096).contains(&size) {
            return Err(invalid("invalid physical HID descriptor size"));
        }
        #[repr(C)]
        struct RawDescriptor {
            size: u32,
            data: [u8; 4096],
        }
        let mut raw = RawDescriptor { size: size as u32, data: [0; 4096] };
        // SAFETY：该ABI由32位长度和4096字节数组组成。
        if unsafe { libc::ioctl(file.as_raw_fd(), 0x90044802_u64 as libc::c_ulong, &raw mut raw) }
            < 0
        {
            return Err(io::Error::last_os_error());
        }
        if raw.size > 4096 {
            return Err(invalid("invalid returned HID descriptor size"));
        }
        let descriptor = raw.data[..raw.size as usize].to_vec();
        let layout = Layout::parse(&descriptor)?;
        let name = path.display().to_string();
        let latest =
            layout.bytes[0].iter().map(|(&id, &len)| (id, CachedInput::new(len))).collect();
        let planned =
            layout.bytes[0].iter().map(|(&id, &len)| (id, CachedInput::new(len))).collect();
        let completed =
            layout.bytes[0].iter().map(|(&id, &len)| (id, CachedInput::new(len))).collect();
        Ok(Self { file, descriptor, layout, latest, planned, completed, name })
    }

    pub(crate) fn cache_input(&self, id: u8, data: &[u8]) {
        if let Some(cache) = self.latest.get(&id) {
            cache.update(data);
        }
    }

    pub(crate) fn latest_input(&self, id: u8) -> Option<Vec<u8>> {
        self.latest.get(&id).and_then(CachedInput::snapshot)
    }

    pub(crate) fn cache_planned(&self, id: u8, data: &[u8]) {
        if let Some(c) = self.planned.get(&id) {
            c.update(data);
        }
    }
    pub(crate) fn cache_output(&self, id: u8, data: &[u8]) {
        if let Some(c) = self.completed.get(&id) {
            c.update(data);
        }
    }
    pub(crate) fn latest_output(&self, id: u8) -> Option<Vec<u8>> {
        self.completed.get(&id).and_then(CachedInput::snapshot)
    }

    pub(crate) fn feature(
        &self,
        id: u8,
        length: usize,
        setting: Option<&[u8]>,
    ) -> io::Result<Vec<u8>> {
        if length > 4096 {
            return Err(invalid("feature report too large"));
        }
        let mut data = vec![0; length + usize::from(!self.layout.numbered)];
        if data.is_empty() {
            return Err(invalid("empty feature report"));
        }
        if let Some(payload) = setting {
            let offset = usize::from(!self.layout.numbered);
            if payload.len() != length {
                return Err(invalid("feature length mismatch"));
            }
            data[offset..].copy_from_slice(payload);
        }
        data[0] = id;
        let number = if setting.is_some() { 0x06 } else { 0x07 };
        let request = (3_u64 << 30) | ((data.len() as u64) << 16) | (0x48 << 8) | number;
        // SAFETY：功能请求使用编码后的缓冲长度；
        // 即使报告不带编号，第一个字节仍用于指定报告ID。
        let count = unsafe {
            libc::ioctl(self.file.as_raw_fd(), request as libc::c_ulong, data.as_mut_ptr())
        };
        if count < 0 {
            return Err(io::Error::last_os_error());
        }
        let count = usize::try_from(count).map_err(|_| invalid("feature result"))?.min(data.len());
        data.truncate(count);
        if !self.layout.numbered && !data.is_empty() {
            data.remove(0);
        }
        Ok(data)
    }

    pub(crate) fn output(&self, payload: &[u8]) -> io::Result<()> {
        let mut data = Vec::with_capacity(payload.len() + 1);
        if !self.layout.numbered {
            data.push(0);
        }
        data.extend_from_slice(payload);
        let count = (&self.file).write(&data)?;
        if count != data.len() {
            return Err(invalid("short physical output report write"));
        }
        Ok(())
    }
}

pub(crate) fn list() -> io::Result<()> {
    for entry in fs::read_dir("/sys/class/hidraw")? {
        let entry = entry?;
        println!(
            "/dev/{}\n{}",
            entry.file_name().to_string_lossy(),
            fs::read_to_string(entry.path().join("device/uevent"))?
        );
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::{Arc, atomic::AtomicBool};

    #[test]
    fn concurrent_input_snapshot_is_coherent() {
        let cache = Arc::new(CachedInput::new(64));
        assert!(cache.snapshot().is_none());
        let done = Arc::new(AtomicBool::new(false));
        std::thread::scope(|scope| {
            scope.spawn(|| {
                for generation in 1..=20_000_u32 {
                    cache.update(&[generation as u8; 64]);
                }
                done.store(true, Ordering::Release);
            });
            while !done.load(Ordering::Acquire) {
                if let Some(data) = cache.snapshot() {
                    assert_eq!(data.len(), 64);
                    assert!(data.iter().all(|value| *value == data[0]));
                }
            }
        });
        assert_eq!(cache.snapshot(), Some(vec![20_000_u32 as u8; 64]));
    }
}

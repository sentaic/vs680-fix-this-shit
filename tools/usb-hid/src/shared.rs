// SPDX-License-Identifier: GPL-2.0-only
//! 单控制者共享内存通道，直接使用Linux接口。
use crate::wire::{MAX_MESSAGE, Message, invalid};
use std::{
    io,
    mem::size_of,
    os::fd::{AsRawFd, OwnedFd},
    ptr::NonNull,
    sync::atomic::{AtomicU64, Ordering},
};
pub const CAPACITY: usize = 256;
#[repr(C, align(64))]
struct Cursor {
    v: AtomicU64,
}
#[repr(C)]
struct Ring {
    write: Cursor,
    read: Cursor,
    slots: [Slot; CAPACITY],
}
// 槽位长度属于共享队列ABI；内容是完整且可独立解码的CBOR项。
#[repr(C, align(64))]
struct Slot {
    len: AtomicU64,
    words: [AtomicU64; MAX_MESSAGE / 8],
}
#[repr(C)]
struct Area {
    magic: AtomicU64,
    epoch: AtomicU64,
    rings: [Ring; 3],
}
#[derive(Debug)]
pub struct Shared {
    fd: OwnedFd,
    p: NonNull<Area>,
}
// SAFETY：共享内容使用原子访问；映射随对象释放，文件已设置禁止缩小的 seal。
unsafe impl Send for Shared {}
unsafe impl Sync for Shared {}
impl Shared {
    pub fn create(epoch: u64) -> io::Result<Self> {
        let raw = unsafe {
            libc::memfd_create(c"hid-control".as_ptr(), libc::MFD_CLOEXEC | libc::MFD_ALLOW_SEALING)
        };
        let fd = crate::local::owned(raw)?;
        if unsafe { libc::ftruncate(raw, size_of::<Area>() as _) } != 0 {
            return Err(io::Error::last_os_error());
        }
        if unsafe {
            libc::fcntl(
                raw,
                libc::F_ADD_SEALS,
                libc::F_SEAL_SHRINK | libc::F_SEAL_GROW | libc::F_SEAL_SEAL,
            )
        } < 0
        {
            return Err(io::Error::last_os_error());
        }
        let m = Self::map(fd)?;
        m.area().epoch.store(epoch, Ordering::Relaxed);
        m.area().magic.store(0x32444948, Ordering::Release);
        unsafe {
            libc::mlock(m.p.as_ptr().cast(), size_of::<Area>());
        }
        Ok(m)
    }
    pub fn attach(fd: OwnedFd, epoch: u64) -> io::Result<Self> {
        let mut s = std::mem::MaybeUninit::<libc::stat>::uninit();
        if unsafe { libc::fstat(fd.as_raw_fd(), s.as_mut_ptr()) } != 0 {
            return Err(io::Error::last_os_error());
        }
        if unsafe { s.assume_init() }.st_size != size_of::<Area>() as i64 {
            return Err(invalid("mapping size"));
        }
        let seals = unsafe { libc::fcntl(fd.as_raw_fd(), libc::F_GET_SEALS) };
        let needed = libc::F_SEAL_SHRINK | libc::F_SEAL_GROW;
        if seals < 0 || seals & needed != needed {
            return Err(invalid("unsealed mapping"));
        }
        let m = Self::map(fd)?;
        if m.area().magic.load(Ordering::Acquire) != 0x32444948
            || m.area().epoch.load(Ordering::Relaxed) != epoch
        {
            return Err(invalid("mapping epoch"));
        }
        Ok(m)
    }
    fn map(fd: OwnedFd) -> io::Result<Self> {
        let p = unsafe {
            libc::mmap(
                std::ptr::null_mut(),
                size_of::<Area>(),
                libc::PROT_READ | libc::PROT_WRITE,
                libc::MAP_SHARED,
                fd.as_raw_fd(),
                0,
            )
        };
        if p == libc::MAP_FAILED {
            return Err(io::Error::last_os_error());
        }
        Ok(Self { fd, p: NonNull::new(p.cast()).ok_or_else(|| invalid("null mapping"))? })
    }
    fn area(&self) -> &Area {
        unsafe { self.p.as_ref() }
    }
    pub fn fd(&self) -> i32 {
        self.fd.as_raw_fd()
    }
    /// 每个环只有一个生产者。0为命令，1为结果，2为物理观察事件。
    pub fn push(&self, kind: usize, m: &Message) -> io::Result<()> {
        let r = self.area().rings.get(kind).ok_or_else(|| invalid("ring kind"))?;
        let w = r.write.v.load(Ordering::Relaxed);
        let p = r.read.v.load(Ordering::Acquire);
        let used = w.wrapping_sub(p);
        if used > CAPACITY as u64 {
            return Err(invalid("corrupt indices"));
        }
        if used == CAPACITY as u64 {
            return Err(io::ErrorKind::WouldBlock.into());
        }
        let b = m.encode()?;
        let slot = &r.slots[w as usize % CAPACITY];
        for (cell, part) in slot.words.iter().zip(b.chunks(8)) {
            let mut a = [0; 8];
            a[..part.len()].copy_from_slice(part);
            cell.store(u64::from_le_bytes(a), Ordering::Relaxed);
        }
        slot.len.store(b.len() as u64, Ordering::Relaxed);
        r.write.v.store(w.wrapping_add(1), Ordering::Release);
        Ok(())
    }
    /// 每个环只有一个消费者；先复制记录，再释放槽位。
    pub fn pop(&self, kind: usize) -> io::Result<Option<Message>> {
        let r = self.area().rings.get(kind).ok_or_else(|| invalid("ring kind"))?;
        let p = r.read.v.load(Ordering::Relaxed);
        let w = r.write.v.load(Ordering::Acquire);
        let used = w.wrapping_sub(p);
        if used > CAPACITY as u64 {
            return Err(invalid("corrupt indices"));
        }
        if used == 0 {
            return Ok(None);
        }
        let slot = &r.slots[p as usize % CAPACITY];
        let len = slot.len.load(Ordering::Relaxed) as usize;
        if len == 0 || len > MAX_MESSAGE {
            return Err(invalid("invalid CBOR slot length"));
        }
        let mut b = [0; MAX_MESSAGE];
        for (part, cell) in b.chunks_exact_mut(8).zip(&slot.words).take(len.div_ceil(8)) {
            part.copy_from_slice(&cell.load(Ordering::Relaxed).to_le_bytes());
        }
        r.read.v.store(p.wrapping_add(1), Ordering::Release);
        Message::decode(&b[..len]).map(Some)
    }
}
impl Drop for Shared {
    fn drop(&mut self) {
        unsafe {
            libc::munmap(self.p.as_ptr().cast(), size_of::<Area>());
        }
    }
}
#[derive(Debug)]
pub struct Wake(pub OwnedFd);
impl Wake {
    pub fn new() -> io::Result<Self> {
        crate::local::owned(unsafe { libc::eventfd(0, libc::EFD_CLOEXEC | libc::EFD_NONBLOCK) })
            .map(Self)
    }
    pub fn fd(&self) -> i32 {
        self.0.as_raw_fd()
    }
    pub fn notify(&self) {
        let v = 1_u64;
        unsafe {
            libc::write(self.fd(), (&raw const v).cast(), 8);
        }
    }
    pub fn drain(&self) {
        let mut v = 0_u64;
        unsafe {
            libc::read(self.fd(), (&raw mut v).cast(), 8);
        }
    }
}

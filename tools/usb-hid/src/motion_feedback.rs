// SPDX-License-Identifier: GPL-2.0-only
//! 记录 USB 写入完成后的累计位移、按钮和时间。
use std::sync::atomic::{AtomicI64, AtomicU64, Ordering};
use vs680_hid::wire::{Message, Status, now_ns};

#[derive(Default)]
pub(crate) struct Feedback {
    version: AtomicU64,
    x: AtomicI64,
    y: AtomicI64,
    path: AtomicU64,
    time: AtomicU64,
    buttons: AtomicU64,
    click_time: AtomicU64,
}
impl Feedback {
    pub(crate) fn complete_output(&self, x: i32, y: i32, buttons: u32) {
        let now = now_ns();
        self.version.fetch_add(1, Ordering::AcqRel);
        self.x.fetch_add(i64::from(x), Ordering::Relaxed);
        self.y.fetch_add(i64::from(y), Ordering::Relaxed);
        self.path.fetch_add((f64::from(x).hypot(f64::from(y)) * 256.).ceil() as u64, Ordering::Relaxed);
        self.time.store(now, Ordering::Relaxed);
        let previous = self.buttons.swap(u64::from(buttons), Ordering::Relaxed);
        if previous & 1 == 0 && buttons & 1 != 0 { self.click_time.store(now, Ordering::Relaxed); }
        self.version.fetch_add(1, Ordering::Release);
    }
    pub(crate) fn latest_completed(&self) -> Option<(i64, i64, u64, u64, u64, u64)> {
        for _ in 0..3 {
            let before = self.version.load(Ordering::Acquire);
            if before & 1 != 0 { continue; }
            let value = (self.x.load(Ordering::Relaxed), self.y.load(Ordering::Relaxed),
                self.time.load(Ordering::Relaxed), self.buttons.load(Ordering::Relaxed),
                self.click_time.load(Ordering::Relaxed), self.path.load(Ordering::Relaxed));
            if before == self.version.load(Ordering::Acquire) { return Some(value); }
        }
        None
    }
    pub(crate) fn response(&self, m: &Message) -> Message {
        let Some((x, y, at, buttons, click, path)) = self.latest_completed() else {
            return m.response(Status::Busy);
        };
        let mut r = m.response(Status::Applied);
        if m.args[0] == 1 {
            r.data[..4].copy_from_slice(b"CMP1");
            for (offset, value) in [(4, x as u64), (12, y as u64), (20, at), (32, click), (56, path)] {
                r.data[offset..offset + 8].copy_from_slice(&value.to_le_bytes());
            }
            r.data[28..32].copy_from_slice(&(buttons as u32).to_le_bytes());
            r.args[1] = 64;
        } else {
            let mut e = minicbor::Encoder::new(Vec::new());
            let result = e.array(9).and_then(|e| e.i64(x)).and_then(|e| e.i64(y))
                .and_then(|e| e.u64(at)).and_then(|e| e.u64(buttons)).and_then(|e| e.u64(click))
                .and_then(|e| e.u64(0)).and_then(|e| e.u64(0)).and_then(|e| e.u64(path)).and_then(|e| e.u64(0));
            if result.is_err() { return m.response(Status::Failed); }
            let bytes = e.into_writer();
            if bytes.len() > 64 { return m.response(Status::Failed); }
            r.data[..bytes.len()].copy_from_slice(&bytes);
            r.args[1] = bytes.len() as i32;
        }
        r
    }
}

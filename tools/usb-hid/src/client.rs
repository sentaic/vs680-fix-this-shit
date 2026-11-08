// SPDX-License-Identifier: GPL-2.0-only
//! Linux本地控制客户端。共享内存用于高频命令，套接字用于会话与紧急操作。
use crate::{
    local,
    shared::{Shared, Wake},
    wire::{MAX_BATCH, MAX_MESSAGE, Message, Op, SHARED, Status, invalid},
};
use std::{
    collections::VecDeque,
    io,
    os::fd::{AsRawFd, OwnedFd},
    path::Path,
    sync::{Arc, Condvar, Mutex},
    thread::JoinHandle,
    time::{Duration, Instant},
};
struct Heartbeat {
    stop: Arc<(Mutex<bool>, Condvar)>,
    thread: Option<JoinHandle<()>>,
}
impl Heartbeat {
    fn start(fd: OwnedFd, lease_ms: u64) -> io::Result<Self> {
        let stop = Arc::new((Mutex::new(false), Condvar::new()));
        let signal = stop.clone();
        let thread = std::thread::Builder::new().name("hid-heartbeat".into()).spawn(move || {
            let (lock, cv) = &*signal;
            let mut stopped = lock.lock().unwrap_or_else(std::sync::PoisonError::into_inner);
            loop {
                let result = cv
                    .wait_timeout(stopped, Duration::from_millis((lease_ms / 3).max(50)))
                    .unwrap_or_else(std::sync::PoisonError::into_inner);
                stopped = result.0;
                if *stopped {
                    break;
                }
                let mut m = Message::new(Op::Ping);
                m.flags = 0;
                if local::send_message(fd.as_raw_fd(), &m).is_err() {
                    break;
                }
            }
        })?;
        Ok(Self { stop, thread: Some(thread) })
    }
}
impl Drop for Heartbeat {
    fn drop(&mut self) {
        let (l, c) = &*self.stop;
        *l.lock().unwrap_or_else(std::sync::PoisonError::into_inner) = true;
        c.notify_one();
        if let Some(t) = self.thread.take() {
            let _ = t.join();
        }
    }
}
pub struct Client {
    fd: OwnedFd,
    map: Option<Shared>,
    send_wake: Option<Wake>,
    receive_wake: Option<Wake>,
    heartbeat: Option<Heartbeat>,
    sequence: u64,
    pending: VecDeque<Message>,
    pub epoch: u64,
}
impl std::fmt::Debug for Client {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Client")
            .field("epoch", &self.epoch)
            .field("shared", &self.map.is_some())
            .finish()
    }
}
impl Client {
    pub fn connect(path: impl AsRef<Path>, shared: bool) -> io::Result<Self> {
        let fd = local::connect(path.as_ref())?;
        let mut h = Message::new(Op::Hello);
        h.args[0] = 2000;
        h.flags = if shared { SHARED } else { 0 };
        local::send_message(fd.as_raw_fd(), &h)?;
        if !local::wait(fd.as_raw_fd(), 2000)? {
            return Err(io::ErrorKind::TimedOut.into());
        }
        let (bytes, mut fds) = local::receive_fds(fd.as_raw_fd())?;
        let h = Message::decode(&bytes)?;
        if h.op != Op::Response || h.args[0] != Status::Applied as i32 {
            return Err(io::Error::new(
                io::ErrorKind::PermissionDenied,
                format!("control handshake status {}", h.args[0]),
            ));
        }
        let (map, send_wake, receive_wake) = if shared {
            if fds.len() != 3 {
                return Err(invalid("handshake descriptors"));
            }
            let map = Shared::attach(fds.remove(0), h.id)?;
            let send = Wake(fds.remove(0));
            let receive = Wake(fds.remove(0));
            (Some(map), Some(send), Some(receive))
        } else {
            (None, None, None)
        };
        let heartbeat = Heartbeat::start(fd.try_clone()?, h.args[1] as u64)?;
        Ok(Self {
            fd,
            map,
            send_wake,
            receive_wake,
            heartbeat: Some(heartbeat),
            sequence: 0,
            pending: VecDeque::with_capacity(256),
            epoch: h.id,
        })
    }
    /// 返回入队请求的序号，USB 完成状态通过响应查询。
    pub fn submit(&mut self, mut m: Message) -> io::Result<u64> {
        self.sequence =
            self.sequence.checked_add(1).ok_or_else(|| invalid("sequence exhausted"))?;
        m.seq = self.sequence;
        let control = matches!(
            m.op,
            Op::Cancel
                | Op::CancelAll
                | Op::Release
                | Op::Capabilities
                | Op::Physical
                | Op::Output
                | Op::Subscribe
                | Op::Action
        );
        if let Some(map) = &self.map {
            if !control {
                map.push(0, &m)?;
                if let Some(w) = &self.send_wake {
                    w.notify();
                }
                return Ok(m.seq);
            }
        }
        local::send_message(self.fd.as_raw_fd(), &m)?;
        Ok(m.seq)
    }
    pub fn command(&mut self, m: Message) -> io::Result<Message> {
        let seq = self.submit(m)?;
        self.wait(seq, Duration::from_secs(2))
    }
    /// 整体接受一个宏；各步骤at为单调绝对时间，Queued回执表示宏已进入执行队列。
    pub fn batch(&mut self, group: u64, steps: &[Message]) -> io::Result<Message> {
        if steps.is_empty() || steps.len() > MAX_BATCH {
            return Err(invalid("batch size"));
        }
        self.sequence += 1;
        let mut h = Message::new(Op::Batch);
        h.seq = self.sequence;
        h.id = group;
        let b = crate::wire::encode_batch(&h, steps)?;
        local::send(self.fd.as_raw_fd(), &b)?;
        self.wait(h.seq, Duration::from_secs(2))
    }
    fn receive(&mut self, timeout: Duration) -> io::Result<Vec<Message>> {
        let mut out = Vec::new();
        let end = Instant::now() + timeout;
        loop {
            // 先清通知再检查队列，避免发布与入睡之间丢失唤醒。
            if let Some(w) = &self.receive_wake {
                w.drain();
            }
            if let Some(map) = &self.map {
                for kind in [1, 2] {
                    for _ in 0..256 {
                        match map.pop(kind)? {
                            Some(m) => out.push(m),
                            None => break,
                        }
                    }
                }
            }
            let mut b = [0; MAX_MESSAGE];
            loop {
                match local::receive(self.fd.as_raw_fd(), &mut b) {
                    Ok(0) => return Err(io::ErrorKind::NotConnected.into()),
                    Ok(n) => out.push(Message::decode(&b[..n])?),
                    Err(e) if e.kind() == io::ErrorKind::WouldBlock => break,
                    Err(e) => return Err(e),
                }
            }
            if !out.is_empty() || Instant::now() >= end {
                return Ok(out);
            }
            let ms = end.saturating_duration_since(Instant::now()).as_millis().min(20) as i32;
            if let Some(w) = &self.receive_wake {
                let mut p = [
                    libc::pollfd { fd: w.fd(), events: libc::POLLIN, revents: 0 },
                    libc::pollfd { fd: self.fd.as_raw_fd(), events: libc::POLLIN, revents: 0 },
                ];
                unsafe {
                    libc::poll(p.as_mut_ptr(), 2, ms);
                }
            } else {
                local::wait(self.fd.as_raw_fd(), ms)?;
            }
        }
    }
    pub fn poll(&mut self, timeout: Duration) -> io::Result<Vec<Message>> {
        if !self.pending.is_empty() {
            return Ok(self.pending.drain(..).collect());
        }
        self.receive(timeout)
    }
    pub fn wait(&mut self, seq: u64, timeout: Duration) -> io::Result<Message> {
        let end = Instant::now() + timeout;
        loop {
            if let Some(i) = self.pending.iter().position(|m| m.op == Op::Response && m.seq == seq)
            {
                return self.pending.remove(i).ok_or_else(|| invalid("response queue"));
            }
            if Instant::now() >= end {
                return Err(io::ErrorKind::TimedOut.into());
            }
            let messages = self.receive(end.saturating_duration_since(Instant::now()))?;
            for m in messages {
                if self.pending.len() >= 1024 {
                    return Err(io::Error::other("client observation queue full; drain with poll"));
                }
                self.pending.push_back(m);
            }
        }
    }
    pub fn subscribe(&mut self, enabled: bool) -> io::Result<Message> {
        let mut m = Message::new(Op::Subscribe);
        m.args[0] = i32::from(enabled);
        self.command(m)
    }
    pub fn takeover(&mut self, target: u32) -> io::Result<Message> {
        let mut m = Message::new(Op::Takeover);
        m.target = target;
        self.command(m)
    }
    pub fn release(&mut self, target: u32) -> io::Result<Message> {
        let mut m = Message::new(Op::Release);
        m.target = target;
        self.command(m)
    }
    pub fn move_by(&mut self, target: u32, x: i32, y: i32) -> io::Result<Message> {
        let mut m = Message::new(Op::Move);
        m.target = target;
        m.args = [x, y, 0, 0];
        self.command(m)
    }
    pub fn force_key(
        &mut self,
        target: u32,
        key: u16,
        down: bool,
        duration_ms: u32,
    ) -> io::Result<Message> {
        let mut m = Message::new(Op::ForceKey);
        m.target = target;
        m.args = [i32::from(key), i32::from(down), duration_ms as i32, 0];
        self.command(m)
    }
    pub fn cancel(&mut self, id: u64) -> io::Result<Message> {
        let mut m = Message::new(Op::Cancel);
        m.id = id;
        self.command(m)
    }
}
impl Drop for Client {
    fn drop(&mut self) {
        self.heartbeat.take();
        let mut m = Message::new(Op::Goodbye);
        m.flags = 0;
        let _ = local::send_message(self.fd.as_raw_fd(), &m);
    }
}

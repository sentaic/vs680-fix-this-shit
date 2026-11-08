// SPDX-License-Identifier: GPL-2.0-only
//! 本地单控制者、租约和独立取消通道。
use crate::{STOP, relay::State};
use std::{
    collections::VecDeque,
    fs, io,
    os::{
        fd::AsRawFd,
        unix::fs::{FileTypeExt, MetadataExt, PermissionsExt},
    },
    path::{Path, PathBuf},
    sync::{
        Arc, Mutex,
        atomic::{AtomicBool, AtomicU64, Ordering},
        mpsc,
    },
    thread::JoinHandle,
};
use vs680_hid::{
    local,
    shared::{Shared, Wake},
    wire::{
        MAX_MESSAGE, MAX_PACKET, Message, Op, REPLY, SHARED, Status, decode_packet, invalid, now_ns,
    },
};
#[derive(Debug)]
pub(crate) struct Session {
    pub(crate) id: u64,
    pub(crate) active: AtomicBool,
    pub(crate) expires: AtomicU64,
    pub(crate) observe: AtomicBool,
    pub(crate) shared: Shared,
    pub(crate) wake: Wake,
    replies: Mutex<()>,
    pub(crate) dropped: AtomicU64,
}
impl Session {
    #[cfg(test)]
    pub(crate) fn fixture() -> io::Result<Arc<Self>> {
        Ok(Arc::new(Self {
            id: 1,
            active: AtomicBool::new(true),
            expires: AtomicU64::new(u64::MAX),
            observe: AtomicBool::new(false),
            shared: Shared::create(1)?,
            wake: Wake::new()?,
            replies: Mutex::new(()),
            dropped: AtomicU64::new(0),
        }))
    }
    pub(crate) fn live(&self, now: u64) -> bool {
        self.active.load(Ordering::Acquire) && now < self.expires.load(Ordering::Acquire)
    }
    pub(crate) fn respond(&self, request: &Message, status: Status) {
        if request.flags & REPLY != 0 {
            self.reply(request.response(status));
        }
    }
    pub(crate) fn reply(&self, r: Message) {
        if !self.active.load(Ordering::Acquire) {
            return;
        }
        let _guard = self.replies.lock().unwrap_or_else(std::sync::PoisonError::into_inner);
        if self.shared.push(1, &r).is_err() {
            self.active.store(false, Ordering::Release);
        }
        self.wake.notify();
    }
    pub(crate) fn input(&self, m: Message) {
        if !self.observe.load(Ordering::Relaxed) {
            return;
        }
        let dropped = self.dropped.swap(0, Ordering::Relaxed);
        if dropped != 0 {
            let mut gap = Message::new(Op::Gap);
            gap.seq = m.seq;
            gap.id = dropped;
            gap.target = m.target;
            if self.shared.push(2, &gap).is_err() {
                self.dropped.fetch_add(dropped + 1, Ordering::Relaxed);
                return;
            }
        }
        if self.shared.push(2, &m).is_err() {
            self.dropped.fetch_add(1, Ordering::Relaxed);
        }
        self.wake.notify();
    }
}
#[derive(Debug)]
pub(crate) struct Envelope {
    pub(crate) session: Arc<Session>,
    pub(crate) header: Message,
    pub(crate) messages: Vec<Message>,
}
#[derive(Debug)]
pub(crate) struct Bus {
    pub(crate) wake: Arc<Wake>,
    pub(crate) current: Mutex<Option<Arc<Session>>>,
    pub(crate) revision: AtomicU64,
    pub(crate) sender: mpsc::SyncSender<Envelope>,
    pub(crate) urgent: Mutex<VecDeque<Envelope>>,
}
impl Bus {
    pub(crate) fn new() -> io::Result<(Arc<Self>, mpsc::Receiver<Envelope>)> {
        let (tx, rx) = mpsc::sync_channel(128);
        Ok((
            Arc::new(Self {
                wake: Arc::new(Wake::new()?),
                current: Mutex::new(None),
                revision: AtomicU64::new(0),
                sender: tx,
                urgent: Mutex::new(VecDeque::with_capacity(16)),
            }),
            rx,
        ))
    }
    pub(crate) fn current(&self) -> Option<Arc<Session>> {
        self.current.lock().unwrap_or_else(std::sync::PoisonError::into_inner).clone()
    }
    fn install(&self, s: Option<Arc<Session>>) {
        *self.current.lock().unwrap_or_else(std::sync::PoisonError::into_inner) = s;
        self.revision.fetch_add(1, Ordering::Release);
        self.wake.notify();
    }
}
struct SocketFile {
    path: PathBuf,
    dev: u64,
    ino: u64,
}
impl Drop for SocketFile {
    fn drop(&mut self) {
        if let Ok(m) = fs::symlink_metadata(&self.path) {
            if m.dev() == self.dev && m.ino() == self.ino {
                let _ = fs::remove_file(&self.path);
            }
        }
    }
}
pub(crate) fn start(path: &Path, bus: Arc<Bus>, state: Arc<State>) -> io::Result<JoinHandle<()>> {
    let parent = path.parent().ok_or_else(|| invalid("socket parent"))?;
    if !parent.exists() {
        fs::create_dir(parent)?;
        fs::set_permissions(parent, fs::Permissions::from_mode(0o700))?;
    }
    let metadata = fs::symlink_metadata(parent)?;
    if !metadata.is_dir()
        || metadata.uid() != unsafe { libc::geteuid() }
        || metadata.mode() & 0o022 != 0
    {
        return Err(invalid("control directory must be owned and not group/world writable"));
    }
    if let Ok(m) = fs::symlink_metadata(path) {
        if !m.file_type().is_socket() || m.uid() != unsafe { libc::geteuid() } {
            return Err(invalid("unsafe existing control path"));
        }
        if local::connect(path).is_ok() {
            return Err(io::Error::new(io::ErrorKind::AddrInUse, "controller already listening"));
        }
        fs::remove_file(path)?;
    }
    let listener = local::socket(true)?;
    let (addr, len) = local::address(path)?;
    if unsafe { libc::bind(listener.as_raw_fd(), (&raw const addr).cast(), len) } != 0 {
        return Err(io::Error::last_os_error());
    }
    fs::set_permissions(path, fs::Permissions::from_mode(0o600))?;
    let meta = fs::metadata(path)?;
    let guard = SocketFile { path: path.to_owned(), dev: meta.dev(), ino: meta.ino() };
    if unsafe { libc::listen(listener.as_raw_fd(), 4) } != 0 {
        return Err(io::Error::last_os_error());
    }
    std::thread::Builder::new().name("hid-control".into()).stack_size(256 * 1024).spawn(move || {
        let _guard = guard;
        if let Err(e) = serve(listener, bus.clone()) {
            *state.error.lock().unwrap_or_else(std::sync::PoisonError::into_inner) =
                Some(format!("control: {e}"));
            STOP.store(true, Ordering::Release);
            bus.wake.notify();
        }
    })
}
fn serve(listener: std::os::fd::OwnedFd, bus: Arc<Bus>) -> io::Result<()> {
    let mut peer: Option<(std::os::fd::OwnedFd, Arc<Session>, bool, u64)> = None;
    let mut epoch = now_ns();
    let mut pending = VecDeque::<Message>::with_capacity(64);
    while !STOP.load(Ordering::Acquire) {
        // 客户端关闭后，即便Goodbye还留在接收缓冲，也先撤销旧会话再接受重连。
        let closed = peer.as_ref().is_some_and(|(fd, _, _, _)| {
            let mut p = libc::pollfd { fd: fd.as_raw_fd(), events: libc::POLLIN, revents: 0 };
            unsafe {
                libc::poll(&raw mut p, 1, 0);
            }
            p.revents & (libc::POLLHUP | libc::POLLERR) != 0
        });
        if closed {
            if let Some((_, s, _, _)) = peer.take() {
                s.active.store(false, Ordering::Release);
            }
            pending.clear();
            bus.install(None);
        }
        if let Some((_, s, _, _)) = &peer {
            if !s.live(now_ns()) {
                s.active.store(false, Ordering::Release);
                peer = None;
                pending.clear();
                bus.install(None);
            }
        }
        let raw = unsafe {
            libc::accept4(
                listener.as_raw_fd(),
                std::ptr::null_mut(),
                std::ptr::null_mut(),
                libc::SOCK_CLOEXEC | libc::SOCK_NONBLOCK,
            )
        };
        if raw >= 0 {
            let fd = local::owned(raw)?;
            if peer.is_some() {
                // 先接收握手再返回Busy，避免客户端发送Hello时只得到断管错误。
                if local::wait(fd.as_raw_fd(), 20).unwrap_or(false) {
                    let mut hello = [0; MAX_MESSAGE];
                    let _ = local::receive(fd.as_raw_fd(), &mut hello);
                }
                let r = Message::new(Op::Hello).response(Status::Busy);
                let _ = local::send_message(fd.as_raw_fd(), &r);
            } else {
                // 仅允许文件所有者连接，并限制未完成握手的等待时间。
                let mut cred: libc::ucred = unsafe { std::mem::zeroed() };
                let mut n = std::mem::size_of_val(&cred) as libc::socklen_t;
                if unsafe {
                    libc::getsockopt(
                        fd.as_raw_fd(),
                        libc::SOL_SOCKET,
                        libc::SO_PEERCRED,
                        (&raw mut cred).cast(),
                        &raw mut n,
                    )
                } != 0
                    || cred.uid != unsafe { libc::geteuid() }
                {
                    continue;
                }
                if !local::wait(fd.as_raw_fd(), 200).unwrap_or(false) {
                    continue;
                }
                let mut b = [0; MAX_MESSAGE];
                let n = match local::receive(fd.as_raw_fd(), &mut b) {
                    Ok(n) => n,
                    Err(_) => continue,
                };
                let hello = match Message::decode(&b[..n]) {
                    Ok(m) if m.op == Op::Hello => m,
                    _ => continue,
                };
                let lease = hello.args[0].clamp(250, 5000) as u64 * 1_000_000;
                epoch = epoch.wrapping_add(1);
                let s = Arc::new(Session {
                    id: epoch,
                    active: AtomicBool::new(true),
                    expires: AtomicU64::new(now_ns() + lease),
                    observe: AtomicBool::new(false),
                    shared: Shared::create(epoch)?,
                    wake: Wake::new()?,
                    replies: Mutex::new(()),
                    dropped: AtomicU64::new(0),
                });
                let shared = hello.flags & SHARED != 0;
                let mut reply = hello.response(Status::Applied);
                reply.at = now_ns();
                reply.id = epoch;
                reply.args[1] = lease as i32 / 1_000_000;
                reply.flags = if shared { SHARED } else { 0 };
                if shared {
                    local::send_fds(
                        fd.as_raw_fd(),
                        &reply.encode()?,
                        &[s.shared.fd(), bus.wake.fd(), s.wake.fd()],
                    )?;
                } else {
                    local::send_message(fd.as_raw_fd(), &reply)?;
                }
                bus.install(Some(s.clone()));
                peer = Some((fd, s, shared, lease));
            }
        } else {
            let e = io::Error::last_os_error();
            if e.kind() != io::ErrorKind::WouldBlock && e.kind() != io::ErrorKind::Interrupted {
                return Err(e);
            }
        }
        if let Some((fd, s, shared, lease)) = &peer {
            for _ in 0..32 {
                let mut b = [0; MAX_PACKET];
                let n = match local::receive(fd.as_raw_fd(), &mut b) {
                    Ok(0) => {
                        s.active.store(false, Ordering::Release);
                        bus.wake.notify();
                        break;
                    }
                    Ok(n) => n,
                    Err(e) if e.kind() == io::ErrorKind::WouldBlock => break,
                    Err(_) => {
                        s.active.store(false, Ordering::Release);
                        bus.wake.notify();
                        break;
                    }
                };
                let (h, steps) = match decode_packet(&b[..n]) {
                    Ok(v) => v,
                    Err(_) => {
                        s.active.store(false, Ordering::Release);
                        bus.wake.notify();
                        break;
                    }
                };
                if h.op == Op::Ping {
                    s.expires.store(now_ns() + lease, Ordering::Release);
                    continue;
                }
                if h.op == Op::Goodbye {
                    s.active.store(false, Ordering::Release);
                    bus.wake.notify();
                    break;
                }
                let messages = if h.op == Op::Batch { steps } else { vec![h] };
                let urgent = matches!(h.op, Op::Cancel | Op::CancelAll | Op::Release);
                if *shared
                    && !urgent
                    && !matches!(
                        h.op,
                        Op::Batch
                            | Op::Capabilities
                            | Op::Physical
                            | Op::Output
                            | Op::Subscribe
                            | Op::Action
                    )
                {
                    let _ = local::send_message(fd.as_raw_fd(), &h.response(Status::Invalid));
                    continue;
                }
                let env = Envelope { session: s.clone(), header: h, messages };
                if urgent {
                    let mut q =
                        bus.urgent.lock().unwrap_or_else(std::sync::PoisonError::into_inner);
                    if q.len() == 16 {
                        s.active.store(false, Ordering::Release);
                    } else {
                        q.push_back(env);
                    }
                } else if bus.sender.try_send(env).is_err() {
                    let _ = local::send_message(fd.as_raw_fd(), &h.response(Status::Full));
                }
                bus.wake.notify();
            }
            if !shared {
                for _ in 0..32 {
                    if pending.len() >= 48 {
                        break;
                    }
                    match s.shared.pop(1)? {
                        Some(m) => pending.push_back(m),
                        None => break,
                    }
                }
                for _ in 0..32 {
                    if pending.len() >= 48 {
                        break;
                    }
                    match s.shared.pop(2)? {
                        Some(m) => pending.push_back(m),
                        None => break,
                    }
                }
                while let Some(m) = pending.front() {
                    match local::send_message(fd.as_raw_fd(), &m) {
                        Ok(()) => {
                            pending.pop_front();
                        }
                        Err(e) if e.kind() == io::ErrorKind::WouldBlock => break,
                        Err(_) => {
                            s.active.store(false, Ordering::Release);
                            bus.wake.notify();
                            break;
                        }
                    }
                }
            }
        }
        let mut p = libc::pollfd { fd: listener.as_raw_fd(), events: libc::POLLIN, revents: 0 };
        unsafe {
            libc::poll(&raw mut p, 1, 2);
        }
    }
    if let Some((_, s, _, _)) = peer {
        s.active.store(false, Ordering::Release);
    }
    bus.install(None);
    Ok(())
}

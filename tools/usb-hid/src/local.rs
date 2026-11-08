// SPDX-License-Identifier: GPL-2.0-only
//! Linux有序消息套接字及文件描述符传递。
use crate::wire::{MAX_MESSAGE, Message, invalid};
use std::{
    io,
    mem::{size_of, zeroed},
    os::fd::{AsRawFd, FromRawFd, OwnedFd},
    path::Path,
};
pub fn socket(nonblock: bool) -> io::Result<OwnedFd> {
    let fd = unsafe {
        libc::socket(
            libc::AF_UNIX,
            libc::SOCK_SEQPACKET
                | libc::SOCK_CLOEXEC
                | if nonblock { libc::SOCK_NONBLOCK } else { 0 },
            0,
        )
    };
    owned(fd)
}
pub fn owned(fd: i32) -> io::Result<OwnedFd> {
    if fd < 0 { Err(io::Error::last_os_error()) } else { Ok(unsafe { OwnedFd::from_raw_fd(fd) }) }
}
pub fn address(path: &Path) -> io::Result<(libc::sockaddr_un, libc::socklen_t)> {
    use std::os::unix::ffi::OsStrExt;
    let p = path.as_os_str().as_bytes();
    let mut a: libc::sockaddr_un = unsafe { zeroed() };
    if p.is_empty() || p.len() >= a.sun_path.len() || p.contains(&0) {
        return Err(invalid("socket path"));
    }
    a.sun_family = libc::AF_UNIX as _;
    for (dst, src) in a.sun_path.iter_mut().zip(p) {
        *dst = *src as _;
    }
    Ok((a, (size_of::<libc::sa_family_t>() + p.len() + 1) as _))
}
pub fn connect(path: &Path) -> io::Result<OwnedFd> {
    let fd = socket(false)?;
    let (a, n) = address(path)?;
    if unsafe { libc::connect(fd.as_raw_fd(), (&raw const a).cast(), n) } != 0 {
        return Err(io::Error::last_os_error());
    }
    Ok(fd)
}
pub fn send(fd: i32, b: &[u8]) -> io::Result<()> {
    let n = unsafe {
        libc::send(fd, b.as_ptr().cast(), b.len(), libc::MSG_NOSIGNAL | libc::MSG_DONTWAIT)
    };
    if n < 0 {
        return Err(io::Error::last_os_error());
    }
    if n as usize != b.len() {
        return Err(invalid("short packet send"));
    }
    Ok(())
}
pub fn send_message(fd: i32, m: &Message) -> io::Result<()> {
    send(fd, &m.encode()?)
}
pub fn receive(fd: i32, b: &mut [u8]) -> io::Result<usize> {
    let n = unsafe {
        libc::recv(fd, b.as_mut_ptr().cast(), b.len(), libc::MSG_DONTWAIT | libc::MSG_TRUNC)
    };
    if n < 0 {
        return Err(io::Error::last_os_error());
    }
    if n as usize > b.len() {
        return Err(invalid("oversized packet"));
    }
    Ok(n as usize)
}
pub fn send_fds(fd: i32, b: &[u8], fds: &[i32]) -> io::Result<()> {
    if fds.len() > 4 {
        return Err(invalid("fd count"));
    }
    let mut control = [0_usize; 16];
    let mut iov = libc::iovec { iov_base: b.as_ptr().cast_mut().cast(), iov_len: b.len() };
    let mut msg: libc::msghdr = unsafe { zeroed() };
    msg.msg_iov = &raw mut iov;
    msg.msg_iovlen = 1;
    msg.msg_control = control.as_mut_ptr().cast();
    msg.msg_controllen = unsafe { libc::CMSG_SPACE(std::mem::size_of_val(fds) as _) as usize };
    unsafe {
        let h = libc::CMSG_FIRSTHDR(&msg);
        (*h).cmsg_level = libc::SOL_SOCKET;
        (*h).cmsg_type = libc::SCM_RIGHTS;
        (*h).cmsg_len = libc::CMSG_LEN(std::mem::size_of_val(fds) as _) as usize;
        std::ptr::copy_nonoverlapping(
            fds.as_ptr().cast::<u8>(),
            libc::CMSG_DATA(h),
            std::mem::size_of_val(fds),
        );
    }
    let n = unsafe { libc::sendmsg(fd, &msg, libc::MSG_NOSIGNAL | libc::MSG_DONTWAIT) };
    if n < 0 {
        Err(io::Error::last_os_error())
    } else if n as usize != b.len() {
        Err(invalid("short fd send"))
    } else {
        Ok(())
    }
}
pub fn receive_fds(fd: i32) -> io::Result<(Vec<u8>, Vec<OwnedFd>)> {
    let mut b = [0; MAX_MESSAGE];
    let mut control = [0_usize; 16];
    let mut iov = libc::iovec { iov_base: b.as_mut_ptr().cast(), iov_len: b.len() };
    let mut msg: libc::msghdr = unsafe { zeroed() };
    msg.msg_iov = &raw mut iov;
    msg.msg_iovlen = 1;
    msg.msg_control = control.as_mut_ptr().cast();
    msg.msg_controllen = std::mem::size_of_val(&control);
    let n = unsafe { libc::recvmsg(fd, &raw mut msg, libc::MSG_CMSG_CLOEXEC) };
    if n < 0 {
        return Err(io::Error::last_os_error());
    }
    let mut fds = Vec::new();
    unsafe {
        let mut h = libc::CMSG_FIRSTHDR(&msg);
        while !h.is_null() {
            if (*h).cmsg_level == libc::SOL_SOCKET && (*h).cmsg_type == libc::SCM_RIGHTS {
                let base = libc::CMSG_LEN(0) as usize;
                if (*h).cmsg_len < base {
                    return Err(invalid("fd header"));
                }
                let count = ((*h).cmsg_len - base) / size_of::<i32>();
                let data = libc::CMSG_DATA(h).cast::<i32>();
                for i in 0..count {
                    fds.push(OwnedFd::from_raw_fd(std::ptr::read_unaligned(data.add(i))));
                }
            }
            h = libc::CMSG_NXTHDR(&msg, h);
        }
    }
    if n == 0
        || n as usize > MAX_MESSAGE
        || msg.msg_flags & (libc::MSG_CTRUNC | libc::MSG_TRUNC) != 0
    {
        return Err(invalid("truncated handshake"));
    }
    Ok((b[..n as usize].to_vec(), fds))
}
pub fn wait(fd: i32, ms: i32) -> io::Result<bool> {
    let mut p = libc::pollfd { fd, events: libc::POLLIN, revents: 0 };
    let n = unsafe { libc::poll(&raw mut p, 1, ms) };
    if n < 0 {
        return Err(io::Error::last_os_error());
    }
    if p.revents & libc::POLLIN != 0 {
        return Ok(true);
    }
    if p.revents & (libc::POLLHUP | libc::POLLERR | libc::POLLNVAL) != 0 {
        return Err(io::ErrorKind::NotConnected.into());
    }
    Ok(n > 0)
}

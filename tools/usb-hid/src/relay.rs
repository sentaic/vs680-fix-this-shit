// SPDX-License-Identifier: GPL-2.0-only
//! Linux采集和发送线程；处理器在两条有界队列之间工作。
use crate::{
    STOP,
    codec::{Codec, Report},
    gadget::{Output, Topology},
    layout::invalid,
    physical::Physical,
    processor::{Lane, Processor},
    scheduling,
    service::{Bus, Envelope, Session},
    stats::Stats,
};
use std::{
    io::{self, Read},
    os::fd::AsRawFd,
    path::Path,
    sync::{
        Arc, Mutex,
        atomic::{AtomicBool, AtomicU64, AtomicUsize, Ordering},
        mpsc,
    },
    thread::{self, JoinHandle},
    time::{Duration, Instant},
};
use vs680_hid::wire::{Message, Status};
#[derive(Debug, Default)]
pub(crate) struct State {
    pub(crate) enabled: AtomicBool,
    pub(crate) error: Mutex<Option<String>>,
}
pub(crate) struct Input {
    pub(crate) target: usize,
    pub(crate) id: u8,
    pub(crate) report: Report,
    pub(crate) at: Instant,
}
pub(crate) struct Packet {
    pub(crate) processing_started: Option<Instant>,
    pub(crate) enqueued: Instant,
    /// 合成结果过期时提交对应的物理输入或按键恢复报告。
    pub(crate) fallback: Option<Report>,
    pub(crate) mouse: bool,
    pub(crate) report: Report,
    pub(crate) id: u8,
    pub(crate) at: Instant,
    pub(crate) generation: u64,
    pub(crate) ticket: Option<(Arc<Session>, Message)>,
}
pub(crate) fn poll(fd: i32, events: i16, timeout: i32) -> io::Result<bool> {
    let mut p = libc::pollfd { fd, events, revents: 0 };
    // SAFETY：传入一个有效的pollfd，系统调用不会保留其指针。
    let count = unsafe { libc::poll(&raw mut p, 1, timeout) };
    if count < 0 {
        let e = io::Error::last_os_error();
        if e.kind() == io::ErrorKind::Interrupted {
            return Ok(false);
        }
        return Err(e);
    }
    if p.revents & (libc::POLLERR | libc::POLLHUP | libc::POLLNVAL) != 0 {
        return Err(io::Error::new(io::ErrorKind::NotConnected, "USB/HID descriptor disconnected"));
    }
    Ok(count > 0 && p.revents & events != 0)
}
fn spawn(
    state: Arc<State>,
    bus: Arc<Bus>,
    name: String,
    priority: i32,
    work: impl FnOnce() -> io::Result<()> + Send + 'static,
) -> io::Result<JoinHandle<()>> {
    thread::Builder::new().name(name.clone()).stack_size(512 * 1024).spawn(move || {
        scheduling::data_thread(priority);
        if let Err(e) = work() {
            if !STOP.swap(true, Ordering::SeqCst) {
                *state.error.lock().unwrap_or_else(std::sync::PoisonError::into_inner) =
                    Some(format!("{name}: {e}"));
            }
            bus.wake.notify();
        }
    })
}
pub(crate) fn start(
    devices: &[Arc<Physical>],
    topology: &Topology,
    ffs: &Path,
    state: &Arc<State>,
    stats: &[Arc<Stats>],
    rates: &[u32],
    priority: i32,
    bus: &Arc<Bus>,
    commands: mpsc::Receiver<Envelope>,
) -> io::Result<Vec<JoinHandle<()>>> {
    let mut input_receivers = Vec::new();
    let mut threads = Vec::new();
    let mut lanes = Vec::new();
    let mut outputs: Vec<Vec<Transmitter>> =
        (0..topology.descriptors.len()).map(|_| Vec::new()).collect();
    let wakes = (0..outputs.len())
        .map(|_| vs680_hid::shared::Wake::new().map(Arc::new))
        .collect::<io::Result<Vec<_>>>()?;
    // 先验证全部布局、打开端点，避免部分线程启动后才发现静态配置错误。
    let mut prepared = Vec::new();
    for (d, output) in devices.iter().zip(topology.outputs(ffs)?) {
        prepared.push((Codec::parse(&d.descriptor, &d.layout)?, output, d.file.try_clone()?));
    }
    for (index, (codec, output, mut input)) in prepared.into_iter().enumerate() {
        let rate = rates[index];
        let d = devices[index].clone();
        let (tx, rx) = mpsc::sync_channel::<Packet>(256);
        let generation = Arc::new(AtomicU64::new(0));
        let queued = Arc::new(AtomicUsize::new(0));
        let feedback = Arc::new(crate::motion_feedback::Feedback::default());
        let group = output.group();
        let output_codec = codec.clone();
        lanes.push(Lane {
            wake: wakes[group].clone(),
            feedback: feedback.clone(),
            physical: d.clone(),
            codec,
            stats: stats[index].clone(),
            sender: tx,
            generation: generation.clone(),
            queued: queued.clone(),
        });
        let (in_tx, input_rx) = mpsc::sync_channel(256);
        input_receivers.push(input_rx);
        let in_bus = bus.clone();
        let in_stats = stats[index].clone();
        let in_state = state.clone();
        threads.push(spawn(
            state.clone(),
            bus.clone(),
            format!("hid-rx{index}"),
            priority,
            move || {
                let mut b = [0u8; 65];
                let mut invalid_reports = 0_u64;
                while !STOP.load(Ordering::Acquire) {
                    if !poll(input.as_raw_fd(), libc::POLLIN, 100)? {
                        continue;
                    }
                    let n = match input.read(&mut b) {
                        Ok(0) => return Err(invalid("physical HID EOF")),
                        Ok(n) => n,
                        Err(e)
                            if matches!(
                                e.kind(),
                                io::ErrorKind::WouldBlock | io::ErrorKind::Interrupted
                            ) =>
                        {
                            continue;
                        }
                        Err(e) => return Err(e),
                    };
                    let at = Instant::now();
                    let id = match d.layout.input_id(&b[..n]) {
                        Ok(id) => id,
                        Err(_) => {
                            // 丢弃与主机描述符长度不匹配的输入报告，继续处理下一份报告。
                            let id = if d.layout.numbered { b[0] } else { 0 };
                            invalid_reports += 1;
                            if invalid_reports <= 8 || invalid_reports.is_power_of_two() {
                                eprintln!("interface={index} invalid_input count={invalid_reports} id={id} length={n} expected={:?}", d.layout.bytes[0].get(&id));
                            }
                            continue;
                        }
                    };
                    in_stats.received.fetch_add(1, Ordering::Relaxed);
                    if !in_state.enabled.load(Ordering::Acquire) {
                        d.cache_input(id, &b[..n]);
                        in_stats.before_enable.fetch_add(1, Ordering::Relaxed);
                        continue;
                    }
                    let mut pending = Input {
                            target: index,
                            id,
                            report: Report::from_slice(&b[..n])?,
                            at,
                        };
                    let mut pressured = false;
                    loop {
                        match in_tx.try_send(pending) {
                            Ok(()) => break,
                            Err(mpsc::TrySendError::Full(report)) => {
                                pending = report;
                                if !pressured {
                                    pressured = true;
                                    in_stats.queue_full.fetch_add(1, Ordering::Relaxed);
                                    eprintln!("interface={index} input_backpressure; independent endpoints remain active");
                                }
                                if STOP.load(Ordering::Acquire) { return Ok(()); }
                                thread::sleep(Duration::from_millis(1));
                            }
                            Err(mpsc::TrySendError::Disconnected(_)) => return Ok(()),
                        }
                    }
                    in_bus.wake.notify();
                }
                Ok(())
            },
        )?);
        outputs[group].push(Transmitter {
            codec: output_codec,
            output,
            device: devices[index].clone(),
            stats: stats[index].clone(),
            rx,
            generation,
            queued,
            rate,
            feedback,
        });
    }
    for (group, output_lanes) in outputs.into_iter().enumerate() {
        let out_state = state.clone();
        let out_bus = bus.clone();
        let wake = wakes[group].clone();
        threads.push(spawn(
            state.clone(),
            bus.clone(),
            format!("hid-ep{}", group + 1),
            priority,
            move || write_group(output_lanes, &out_state, &out_bus, &wake),
        )?);
    }
    let proc = Processor::new(lanes, bus.clone(), commands, input_receivers);
    threads.push(spawn(state.clone(), bus.clone(), "hid-process".into(), priority, move || {
        proc.run()
    })?);
    Ok(threads)
}
struct Transmitter {
    codec: Codec,
    output: Output,
    device: Arc<Physical>,
    stats: Arc<Stats>,
    rx: mpsc::Receiver<Packet>,
    generation: Arc<AtomicU64>,
    queued: Arc<AtomicUsize>,
    rate: u32,
    feedback: Arc<crate::motion_feedback::Feedback>,
}
fn write_group(
    lanes: Vec<Transmitter>,
    state: &State,
    bus: &Bus,
    wake: &vs680_hid::shared::Wake,
) -> io::Result<()> {
    while !STOP.load(Ordering::Acquire) {
        wake.drain();
        let mut active = false;
        let mut connected = false;
        // One owner per USB endpoint; round-robin among its physical interfaces.
        for lane in &lanes {
            match lane.rx.try_recv() {
                Ok(packet) => {
                    lane.write_packet(state, packet)?;
                    bus.wake.notify();
                    active = true;
                    connected = true;
                }
                Err(mpsc::TryRecvError::Empty) => connected = true,
                Err(mpsc::TryRecvError::Disconnected) => {}
            }
        }
        if !connected {
            break;
        }
        if !active {
            poll(wake.fd(), libc::POLLIN, 100)?;
        }
    }
    Ok(())
}
impl Transmitter {
    fn write_packet(&self, state: &State, packet: Packet) -> io::Result<()> {
        let Self { output, device, stats, generation, queued, rate, feedback, .. } = self;
        let cancelled = generation.load(Ordering::Acquire) != packet.generation
            || packet.ticket.as_ref().is_some_and(|(s, _)| !s.live(vs680_hid::wire::now_ns()));
        let stale = packet.at.elapsed() > Duration::from_millis(20);
        if stale { stats.stale_reports.fetch_add(1, Ordering::Relaxed); }
        let expired = stale && packet.ticket.is_some();
        if !state.enabled.load(Ordering::Acquire)
            || ((cancelled || expired) && packet.fallback.is_none()) {
            queued.fetch_sub(1, Ordering::AcqRel);
            if let Some((s, m)) = &packet.ticket {
                s.respond(m, if expired && !cancelled { Status::Expired } else { Status::Cancelled });
            }
            return Ok(());
        }
        // 过期的合成结果改用同一时刻的物理报告，保留按下和松开的顺序。
        let fallback = (stale || cancelled) && packet.fallback.is_some();
        let report = packet.submission(stale || cancelled);
        if fallback {
            stats.physical_fallbacks.fetch_add(1, Ordering::Relaxed);
        }
        let submitted = Instant::now();
        output.write(report.bytes())?;
        let finished = Instant::now();
        if packet.mouse {
            let actual = self.codec.decode(packet.id, report.bytes())?;
            feedback.complete_output(actual.x, actual.y, actual.buttons);
        }
        device.cache_output(packet.id, report.bytes());
        // 队列占用包含正在执行的 USB 写入。
        queued.fetch_sub(1, Ordering::AcqRel);
        let us = |d: Duration| d.as_micros().min(u64::MAX as u128) as u64;
        stats.stages(
            packet
                .processing_started
                .map(|started| [us(started - packet.at), us(packet.enqueued - started)]),
            us(submitted - packet.enqueued),
            us(finished - submitted),
        );
        stats.completed(
            us(finished - packet.at),
            us(submitted - packet.at),
            us(finished - submitted),
            *rate,
        );
        if let Some((s, m)) = &packet.ticket {
            s.respond(m, if cancelled { Status::Cancelled } else if expired { Status::Expired } else { Status::Completed });
        }
        Ok(())
    }
}
impl Packet {
    fn submission(&self, stale: bool) -> Report {
        if stale { self.fallback.unwrap_or(self.report) } else { self.report }
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    use std::os::{fd::OwnedFd, unix::net::UnixStream};

    #[test]
    fn session_change_preserves_physical_motion_and_completion() -> io::Result<()> {
        let desc = [5,1,9,2,0xa1,1,9,1,0xa1,0,5,9,0x19,1,0x29,8,0x15,0,0x25,1,0x75,1,0x95,8,0x81,2,
            5,1,9,0x30,9,0x31,0x15,0x81,0x25,0x7f,0x75,8,0x95,2,0x81,6,0xc0,0xc0];
        let device=Arc::new(Physical::fixture(&desc)?);
        let codec=Codec::parse(&desc,&device.layout)?;
        let state=State::default();state.enabled.store(true,Ordering::Release);
        let (_,rx)=mpsc::sync_channel(4);let (output,mut input)=UnixStream::pair()?;
        let feedback=Arc::new(crate::motion_feedback::Feedback::default());
        let lane=Transmitter{codec,device,stats:Arc::new(Stats::default()),rx,
            output:Output::fixture(std::fs::File::from(OwnedFd::from(output))),
            generation:Arc::new(AtomicU64::new(1)),queued:Arc::new(AtomicUsize::new(3)),rate:1000,feedback:feedback.clone()};
        let packets=[(10,1,Some([2,3,0])),(2,0,Some([0,2,0])),(7,1,None)];
        for (x,buttons,fallback) in packets {
            let at=Instant::now();
            lane.write_packet(&state,Packet{processing_started:None,enqueued:at,at,id:0,generation:0,ticket:None,
                report:Report::from_slice(&[buttons,x,0])?,fallback:fallback.map(|b|Report::from_slice(&b).unwrap()),
                mouse:true})?;
        }
        assert_eq!(feedback.latest_completed().map(|s|(s.0,s.1,s.3,s.5)),Some((5,0,0,1280)));
        assert_eq!(lane.queued.load(Ordering::Acquire),0);
        drop(lane);let mut bytes=Vec::new();input.read_to_end(&mut bytes)?;
        assert_eq!(bytes,[2,3,0,0,2,0]);
        Ok(())
    }

    #[test]
    fn session_change_preserves_short_physical_key_tap() -> io::Result<()> {
        let desc=[5,1,9,6,0xa1,1,0x75,8,0x95,8,0x81,2,0xc0];
        let device=Arc::new(Physical::fixture(&desc)?);let codec=Codec::parse(&desc,&device.layout)?;
        let state=State::default();state.enabled.store(true,Ordering::Release);
        let (_,rx)=mpsc::sync_channel(4);let(output,mut input)=UnixStream::pair()?;
        let lane=Transmitter{codec,device,stats:Arc::new(Stats::default()),rx,
            output:Output::fixture(std::fs::File::from(OwnedFd::from(output))),
            generation:Arc::new(AtomicU64::new(1)),queued:Arc::new(AtomicUsize::new(3)),rate:1000,
            feedback:Arc::new(crate::motion_feedback::Feedback::default())};
        let down=Report::from_slice(&[0,0,4,0,0,0,0,0])?;let up=Report::from_slice(&[0;8])?;
        for (report,fallback) in [(up,Some(down)),(down,Some(up)),(down,None)] {
            let at=Instant::now();lane.write_packet(&state,Packet{processing_started:None,enqueued:at,at,id:0,generation:0,ticket:None,
                report,fallback,mouse:false})?;
        }
        drop(lane);let mut bytes=Vec::new();input.read_to_end(&mut bytes)?;
        assert_eq!(bytes,[down.bytes(),up.bytes()].concat());Ok(())
    }

    #[test]
    fn expired_output_preserves_physical_key_releases() -> io::Result<()>
    {
        let desc = [5, 1, 9, 6, 0xa1, 1, 0x75, 8, 0x95, 8, 0x81, 2, 0xc0];
        let device = Arc::new(Physical::fixture(&desc)?);
        let state = State::default();
        state.enabled.store(true, Ordering::Release);
        let stats = Arc::new(Stats::default());
        let (tx, rx) = mpsc::sync_channel(4);
        let blank = Report::from_slice(&[0; 8])?;
        let held = Report::from_slice(&[1, 0, 4, 0, 0, 0, 0, 0])?;
        let at = Instant::now() - Duration::from_millis(30);
        // 过期的合成按住状态恢复为物理松开，随后按顺序发送实体按下和松开。
        for (report, fallback) in [(held, Some(blank)), (held, None), (blank, None)] {
            tx.send(Packet {
                processing_started: Some(at),
                enqueued: at,
                report,
                fallback,
                at,
                id: 0,
                generation: 0,
                mouse: false,
                ticket: None,
            })
            .unwrap();
        }
        drop(tx);
        let queued = Arc::new(AtomicUsize::new(3));
        let (output, mut input) = UnixStream::pair()?;
        let (bus, _) = Bus::new()?;
        write_group(
            vec![Transmitter {
                codec: Codec::parse(&desc, &device.layout)?,
                output: Output::fixture(std::fs::File::from(OwnedFd::from(output))),
                device: device.clone(),
                stats: stats.clone(),
                rx,
                generation: Arc::new(AtomicU64::new(0)),
                queued: queued.clone(),
                rate: 1000,
                feedback: Arc::new(crate::motion_feedback::Feedback::default()),
            }],
            &state,
            &bus,
            &vs680_hid::shared::Wake::new()?,
        )?;
        let mut bytes = Vec::new();
        input.read_to_end(&mut bytes)?;
        assert_eq!(bytes, [blank.bytes(), held.bytes(), blank.bytes()].concat());
        assert_eq!(device.latest_output(0).unwrap(), blank.bytes());
        assert_eq!(queued.load(Ordering::Acquire), 0);
        assert_eq!(stats.stale_reports.load(Ordering::Relaxed), 3);
        assert_eq!(stats.physical_fallbacks.load(Ordering::Relaxed), 1);
        assert!(state.error.lock().unwrap().is_none());
        Ok(())
    }
}

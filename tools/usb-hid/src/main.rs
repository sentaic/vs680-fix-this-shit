// SPDX-License-Identifier: GPL-2.0-only
//! 通过 FunctionFS 转发和合成 HID 输入。
mod actions;
mod codec;
mod control;
mod discovery;
mod gadget;
mod layout;
mod motion_feedback;
mod physical;
mod polling;
mod processor;
mod relay;
mod scheduling;
mod service;
mod stats;

use physical::Physical;
use relay::State;
use stats::Stats;
use std::{
    fs::{self, OpenOptions},
    io,
    os::fd::AsRawFd,
    path::PathBuf,
    sync::{
        Arc,
        atomic::{AtomicBool, Ordering},
    },
    time::{Duration, Instant},
};

static STOP: AtomicBool = AtomicBool::new(false);
const UDC_FILE: &str = "/sys/kernel/config/usb_gadget/vs680-kvm/UDC";
extern "C" fn stop_signal(_: libc::c_int) {
    STOP.store(true, Ordering::Relaxed);
}

#[derive(Debug)]
struct Options {
    paths: Vec<PathBuf>,
    selected: usize,
    ffs: PathBuf,
    udc: String,
    rate: u32,
    duration: Option<Duration>,
    rt_priority: i32,
    socket: PathBuf,
}

fn options() -> Result<Option<Options>, String> {
    let mut opts = Options {
        paths: Vec::new(),
        selected: 0,
        ffs: "/run/vs680-kvm/ffs".into(),
        udc: String::new(),
        rate: 0,
        duration: None,
        rt_priority: 20,
        socket: vs680_hid::wire::SOCKET.into(),
    };
    let mut args = std::env::args().skip(1);
    while let Some(arg) = args.next() {
        if arg == "--help" || arg == "-h" {
            println!(
                "vs680-hid --device /dev/hidrawN [--device /dev/hidrawM] --udc NAME [--rate auto|1000|4000|8000] [--rt-priority 0..30] [--duration SECONDS] [--ffs PATH]\nvs680-hid --list\nUse demos/hid-gadget.sh to prepare FunctionFS. Auto (default) uses each physical interrupt-IN interval for the virtual endpoint.\nNon-boot HID, original input reports up to 64 bytes. 4K/8K require High-Speed USB; rate is a requested polling interval.\nData workers request FIFO priority 20 by default; 0 disables this. Existing mappings are locked when permitted."
            );
            println!(
                "Sibling HID interfaces of each selected USB device are included automatically (up to 15, subject to controller capacity). Selected interfaces retain their target indices; auxiliary interfaces follow them."
            );
            return Ok(None);
        }
        if arg == "--list" {
            if args.next().is_some() || !opts.paths.is_empty() {
                return Err("--list must be used alone".into());
            }
            physical::list().map_err(|error| error.to_string())?;
            return Ok(None);
        }
        let value = args.next().ok_or_else(|| format!("{arg} requires a value"))?;
        match arg.as_str() {
            "--device" => opts.paths.push(value.into()),
            "--ffs" => opts.ffs = value.into(),
            "--socket" => opts.socket = value.into(),
            "--udc" => opts.udc = value,
            "--rate" => {
                opts.rate =
                    if value == "auto" { 0 } else { value.parse().map_err(|_| "invalid rate")? }
            }
            "--rt-priority" => {
                opts.rt_priority = value.parse().map_err(|_| "invalid RT priority")?
            }
            "--duration" => {
                let seconds: u64 = value.parse().map_err(|_| "invalid duration")?;
                if seconds == 0 {
                    return Err("duration must be positive".into());
                }
                opts.duration = Some(Duration::from_secs(seconds));
            }
            _ => return Err(format!("unknown argument: {arg}")),
        }
    }
    if opts.paths.is_empty()
        || opts.paths.len() > layout::MAX_INTERFACES
        || opts.udc.is_empty()
        || opts.udc.contains('/')
        || !matches!(opts.rate, 0 | 1000 | 4000 | 8000)
        || !(0..=30).contains(&opts.rt_priority)
    {
        return Err(
            "select 1..15 interfaces, a UDC name and rate auto/1000/4000/8000; see --help".into()
        );
    }
    opts.selected = opts.paths.len();
    opts.paths = discovery::expand(&opts.paths).map_err(|error| error.to_string())?;
    Ok(Some(opts))
}

fn install_stop() -> io::Result<()> {
    // SAFETY：处理函数只写原子标志，sigaction字段已完整初始化。
    unsafe {
        let mut action: libc::sigaction = std::mem::zeroed();
        action.sa_sigaction = stop_signal as *const () as usize;
        libc::sigemptyset(&raw mut action.sa_mask);
        for signal in [libc::SIGINT, libc::SIGTERM] {
            if libc::sigaction(signal, &raw const action, std::ptr::null_mut()) < 0 {
                return Err(io::Error::last_os_error());
            }
        }
    }
    Ok(())
}

fn run(opts: &Options) -> io::Result<()> {
    if !fs::read_to_string(UDC_FILE)?.trim().is_empty() {
        return Err(io::Error::other("the HID gadget is already bound"));
    }
    let devices = opts
        .paths
        .iter()
        .map(|path| Physical::open(path).map(Arc::new))
        .collect::<io::Result<Vec<_>>>()?;
    for device in &devices {
        println!("device={} reports={:?}", device.name, device.layout.bytes);
    }
    let topology =
        gadget::Topology::new(&devices, &discovery::groups(&opts.paths, opts.selected)?)?;
    let intervals = opts
        .paths
        .iter()
        .map(|p| {
            if opts.rate == 0 {
                polling::Polling::physical(p)
            } else {
                Ok(polling::Polling::fixed(opts.rate))
            }
        })
        .collect::<io::Result<Vec<_>>>()?;
    for (i, p) in intervals.iter().enumerate() {
        eprintln!(
            "interface={i} polling={} hs_period_us={} fs_interval_ms={}",
            if opts.rate == 0 { "auto" } else { "fixed" },
            p.period_us,
            p.fs
        );
    }
    let grouped_intervals = topology.intervals(&intervals);
    let intervals = topology.lane_intervals(&grouped_intervals);
    let rates: Vec<u32> = intervals.iter().map(|p| (1_000_000 / p.period_us).max(1)).collect();
    let descriptors = layout::descriptors_intervals(
        &topology.descriptors,
        &grouped_intervals.iter().map(|p| (p.fs, p.hs)).collect::<Vec<_>>(),
    )?;
    let mut ep0 = OpenOptions::new().read(true).write(true).open(opts.ffs.join("ep0"))?;
    control::write_packet(&ep0, &descriptors)?;
    control::write_packet(&ep0, &layout::strings())?;
    let state = Arc::new(State::default());
    let stats: Vec<_> = devices.iter().map(|_| Arc::new(Stats::default())).collect();
    let mut threads = Vec::new();
    let (bus, commands) = service::Bus::new()?;
    let result = (|| {
        threads.extend(relay::start(
            &devices,
            &topology,
            &opts.ffs,
            &state,
            &stats,
            &rates,
            opts.rt_priority,
            &bus,
            commands,
        )?);
        threads.push(service::start(&opts.socket, bus.clone(), state.clone())?);
        eprintln!("control_socket={}", opts.socket.display());
        scheduling::lock_memory();
        fs::write(UDC_FILE, &opts.udc)?;
        let rate_label =
            if opts.rate == 0 { "auto".to_owned() } else { format!("{}Hz", opts.rate) };
        eprintln!("state=waiting polling={rate_label}");
        let start = Instant::now();
        let mut last_stats = start;
        let mut previous = vec![(0, 0); stats.len()];
        while !STOP.load(Ordering::Relaxed)
            && !opts.duration.is_some_and(|duration| start.elapsed() >= duration)
        {
            if relay::poll(ep0.as_raw_fd(), libc::POLLIN, 20)? {
                let mut bytes = [0; 48];
                let count = control::events(&mut ep0, &mut bytes)?;
                for event in bytes[..count].chunks_exact(12) {
                    match event[8] {
                        2 => {
                            // ENABLE
                            let speed = fs::read_to_string(format!(
                                "/sys/class/udc/{}/current_speed",
                                opts.udc
                            ))?;
                            println!(
                                "state=enabled usb_speed={} polling={}",
                                speed.trim(),
                                rate_label
                            );
                            if opts.rate > 1000 && speed.trim() != "high-speed" {
                                return Err(io::Error::other(
                                    "4000/8000 Hz requires a High-Speed connection",
                                ));
                            }
                            for (s, interval) in stats.iter().zip(&intervals) {
                                s.poll_period_us.store(
                                    u64::from(if speed.trim() == "high-speed" {
                                        interval.period_us
                                    } else {
                                        u32::from(interval.fs) * 1000
                                    }),
                                    Ordering::Relaxed,
                                );
                            }
                            state.enabled.store(true, Ordering::Release);
                        }
                        1 | 3 => {
                            return Err(io::Error::new(
                                io::ErrorKind::NotConnected,
                                "USB disabled/unbound; rerun after reconnecting",
                            ));
                        }
                        4 => control::setup(&ep0, event, &devices, &topology)?,
                        5 => {
                            return Err(io::Error::new(
                                io::ErrorKind::NotConnected,
                                "USB suspended; stopping to avoid queued stale input",
                            ));
                        }
                        0 | 6 => {}
                        _ => return Err(layout::invalid("unknown FunctionFS event")),
                    }
                }
            }
            if last_stats.elapsed() >= Duration::from_secs(2) {
                let seconds = last_stats.elapsed().as_secs_f64();
                for (index, counters) in stats.iter().enumerate() {
                    let received = counters.received.load(Ordering::Relaxed);
                    let sent = counters.sent.load(Ordering::Relaxed);
                    let [p50, p95, p99] = counters.percentiles();
                    let [input_queue, processing, output_queue, usb_write] =
                        counters.take_stage_window();
                    println!(
                        "interface={index} read_reports={} sent_reports={} read_hz={:.1} sent_hz={:.1} max_relay_us={} over_period={} queue_full={} pre_enable={} window_max_us={} queue_max_us={} usb_max_us={} p50_le_us={} p95_le_us={} p99_le_us={} stale_reports={} physical_fallbacks={} input_queue_window_us={input_queue} processing_window_us={processing} output_queue_window_us={output_queue} usb_write_window_us={usb_write}",
                        received,
                        sent,
                        (received - previous[index].0) as f64 / seconds,
                        (sent - previous[index].1) as f64 / seconds,
                        counters.max_latency_us.load(Ordering::Relaxed),
                        counters.over_period.load(Ordering::Relaxed),
                        counters.queue_full.load(Ordering::Relaxed),
                        counters.before_enable.load(Ordering::Relaxed),
                        counters.window_max_us.swap(0, Ordering::Relaxed),
                        counters.max_queue_us.load(Ordering::Relaxed),
                        counters.max_usb_us.load(Ordering::Relaxed),
                        p50,
                        p95,
                        p99,
                        counters.stale_reports.load(Ordering::Relaxed),
                        counters.physical_fallbacks.load(Ordering::Relaxed)
                    );
                    previous[index] = (received, sent);
                }
                last_stats = Instant::now();
            }
        }
        Ok(())
    })();
    STOP.store(true, Ordering::Relaxed);
    bus.wake.notify();
    state.enabled.store(false, Ordering::Release);
    // 等待线程退出前先解绑，取消未完成的端点请求，
    // 同时移除虚拟设备，避免留下未释放的按键。
    let disconnected = fs::read_to_string(format!("/sys/class/udc/{}/state", opts.udc))
        .is_ok_and(|state| matches!(state.trim(), "not attached" | "suspended"));
    let cleanup = fs::write(UDC_FILE, "\n");
    if cleanup.is_err() {
        // 交由启动脚本强制结束并解绑，避免无限等待。
        return result.and(cleanup);
    }
    for thread in threads {
        if thread.join().is_err() {
            return Err(io::Error::other("HID worker panicked"));
        }
    }
    let worker_error = state.error.lock().unwrap_or_else(std::sync::PoisonError::into_inner).take();
    result?;
    if let Some(error) = worker_error {
        return Err(io::Error::new(
            if disconnected { io::ErrorKind::NotConnected } else { io::ErrorKind::Other },
            error,
        ));
    }
    Ok(())
}

fn main() -> std::process::ExitCode {
    let options = match options() {
        Ok(Some(options)) => options,
        Ok(None) => return std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            return std::process::ExitCode::from(2);
        }
    };
    match install_stop().and_then(|()| run(&options)) {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("state=failed error={error}");
            if error.kind() == io::ErrorKind::NotConnected {
                std::process::ExitCode::from(75)
            } else {
                std::process::ExitCode::FAILURE
            }
        }
    }
}

// SPDX-License-Identifier: GPL-2.0-only
//! 验证按键恢复、宏取消、条件触发和外部处理顺序。
use super::*;
const KEYBOARD: &[u8] = &[
    5, 1, 9, 6, 0xa1, 1, 5, 7, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0, 0x25, 1, 0x75, 1, 0x95, 8, 0x81, 2,
    0x95, 1, 0x75, 8, 0x81, 3, 0x95, 6, 0x75, 8, 0x15, 0, 0x26, 0xff, 0, 5, 7, 0x19, 0, 0x2a, 0xff,
    0, 0x81, 0, 0xc0,
];
const MOUSE: &[u8] = &[
    5, 1, 9, 2, 0xa1, 1, 0x85, 2, 9, 1, 0xa1, 0, 5, 9, 0x19, 1, 0x29, 16, 0x15, 0, 0x25, 1, 0x95,
    16, 0x75, 1, 0x81, 2, 5, 1, 9, 0x30, 9, 0x31, 0x16, 1, 0x80, 0x26, 0xff, 0x7f, 0x75, 16, 0x95,
    2, 0x81, 6, 9, 0x38, 0x15, 0x81, 0x25, 0x7f, 0x75, 8, 0x95, 1, 0x81, 6, 0xc0, 0xc0,
];
fn fixture() -> io::Result<(Processor, Vec<mpsc::Receiver<Packet>>)> {
    fixture_interfaces(&[KEYBOARD, MOUSE])
}

fn fixture_interfaces(
    descriptors: &[&[u8]],
) -> io::Result<(Processor, Vec<mpsc::Receiver<Packet>>)> {
    let (bus, commands) = Bus::new()?;
    let mut inputs = Vec::new();
    let mut lanes = Vec::new();
    let mut receivers = Vec::new();
    for &desc in descriptors {
        let physical = Arc::new(Physical::fixture(desc)?);
        let (tx, rx) = mpsc::sync_channel(256);
        let (_, input_rx) = mpsc::sync_channel(256);
        inputs.push(input_rx);
        lanes.push(Lane {
            wake: Arc::new(vs680_hid::shared::Wake::new()?),
            feedback: Arc::new(crate::motion_feedback::Feedback::default()),
            codec: Codec::parse(desc, &physical.layout)?,
            physical,
            stats: Arc::new(Stats::default()),
            sender: tx,
            generation: Arc::new(AtomicU64::new(0)),
            queued: Arc::new(AtomicUsize::new(0)),
        });
        receivers.push(rx);
    }
    let mut p = Processor::new(lanes, bus, commands, inputs);
    p.session = Some(Session::fixture()?);
    for d in &mut p.devices {
        for id in d.lane.codec.ids() {
            d.frames.insert(id, d.lane.codec.blank(id)?);
        }
    }
    Ok((p, receivers))
}

fn command(op: Op, target: u32, args: [i32; 4]) -> Message {
    let mut m = Message::new(op);
    m.flags = 0;
    m.target = target;
    m.args = args;
    m
}

#[test]
fn endpoint_backpressure_preserves_keyboard_releases_and_mouse_order() -> io::Result<()> {
    let (mut p, outputs) = fixture()?;
    let mut senders = Vec::new();
    p.inputs.clear();
    for _ in 0..2 {
        let (tx, rx) = mpsc::sync_channel(256);
        senders.push(tx);
        p.inputs.push(rx);
    }
    for (target, bytes) in [
        (1, &[2, 0, 0, 1, 0, 0, 0, 0][..]),
        (1, &[2, 0, 0, 2, 0, 0, 0, 0][..]),
        (0, &[0, 0, 0x59, 0, 0, 0, 0, 0][..]),
        (0, &[0; 8][..]),
    ] {
        senders[target]
            .send(Input {
                target,
                id: p.devices[target].lane.physical.layout.input_id(bytes)?,
                report: Report::from_slice(bytes)?,
                at: Instant::now(),
            })
            .unwrap();
    }
    p.devices[1].lane.queued.store(192, Ordering::Release);
    assert!(p.drain_inputs()?);
    let keyboard = drain(&p, &outputs[0], 0);
    assert_eq!(keyboard.len(), 2);
    assert_eq!(keyboard[0].data[2], 0x59);
    assert_eq!(keyboard[1].bytes(), &[0; 8]);
    assert!(outputs[1].try_recv().is_err());
    p.devices[1].lane.queued.store(0, Ordering::Release);
    assert!(p.drain_inputs()?);
    let mouse = drain(&p, &outputs[1], 1);
    assert_eq!(mouse.len(), 2);
    assert_eq!([mouse[0].data[3], mouse[1].data[3]], [1, 2]);
    Ok(())
}

fn input(p: &mut Processor, target: usize, bytes: &[u8]) -> io::Result<()> {
    p.input(Input {
        target,
        id: p.devices[target].lane.physical.layout.input_id(bytes)?,
        report: Report::from_slice(bytes)?,
        at: Instant::now(),
    })
}

fn drain(p: &Processor, rx: &mpsc::Receiver<Packet>, t: usize) -> Vec<Report> {
    let mut reports = Vec::new();
    while let Ok(packet) = rx.try_recv() {
        p.devices[t].lane.queued.fetch_sub(1, Ordering::Relaxed);
        p.devices[t].lane.physical.cache_output(packet.id, packet.report.bytes());
        reports.push(packet.report);
    }
    reports
}

fn last<'a>(reports: &'a [Report]) -> io::Result<&'a Report> {
    reports.last().ok_or_else(|| invalid("missing output"))
}

#[test]
fn disconnect_preserves_pending_external_physical_reports() -> io::Result<()> {
    let (mut p,rx)=fixture()?;
    p.apply(command(Op::Takeover,1,[3,100000,1,0]),now_ns(),0)?;
    drain(&p,&rx[1],1);
    for x in [5,7] {input(&mut p,1,&[2,3,0,x,0,0,0,255])?;}
    assert_eq!(p.waiting.len(),2);
    p.release_all()?;
    assert!(p.waiting.is_empty());
    let physical:Vec<_>=rx[1].try_iter().filter_map(|packet|packet.fallback).collect();
    assert_eq!(physical.len(),2);
    for (report,x) in physical.iter().zip([5,7]) {
        let v=p.devices[1].lane.codec.decode(2,report.bytes())?;
        assert_eq!((v.x,v.buttons,v.wheel),(x,3,-1));
    }
    Ok(())
}

#[test]
fn external_faults_drain_large_physical_fifo_under_backpressure() -> io::Result<()> {
    for disconnect in [false,true] {
        let(mut p,rx)=fixture()?;
        p.apply(command(Op::Takeover,1,[3,100000,0,0]),now_ns(),0)?;
        drain(&p,&rx[1],1);
        for n in 0..256u16 {
            let x=(n+1).to_le_bytes();
            input(&mut p,1,&[2,if n==255{0}else{3},0,x[0],x[1],0,0,255])?;
        }
        assert_eq!(p.waiting.len(),256);
        p.devices[1].lane.queued.store(192,Ordering::Release);
        if disconnect {p.release_all()?;} else {
            for w in &mut p.waiting {w.until=0;}
            p.drain_decisions(1)?;
        }
        assert_eq!(p.waiting.len(),256);
        p.devices[1].lane.queued.store(0,Ordering::Release);
        p.drain_decisions(1)?;
        assert_eq!(p.waiting.len(),64);
        assert!(p.devices[1].restore_pending);
        let mut reports=drain(&p,&rx[1],1);
        p.drain_decisions(1)?;
        reports.extend(drain(&p,&rx[1],1));
        assert!(p.waiting.is_empty());assert!(!p.devices[1].restore_pending);
        assert_eq!(reports.len(),257);
        for (index,r) in reports[..256].iter().enumerate() {
            let v=p.devices[1].lane.codec.decode(2,r.bytes())?;
            assert_eq!((v.x,v.y,v.buttons,v.wheel),(index as i32+1,0,if index==255{0}else{3},-1));
        }
        let restored=p.devices[1].lane.codec.decode(2,reports[256].bytes())?;
        assert_eq!((restored.x,restored.y,restored.buttons,restored.wheel),(0,0,0,0));
        assert!(!p.devices[1].external);
    }
    Ok(())
}

#[test]
fn reconnect_commands_keep_downgraded_physical_reports() -> io::Result<()> {
    for op in [Op::Release,Op::Takeover] {
        let(mut p,rx)=fixture()?;
        p.apply(command(Op::Takeover,1,[3,100000,0,0]),now_ns(),0)?;
        drain(&p,&rx[1],1);
        for _ in 0..256 {input(&mut p,1,&[2,0,0,1,0,0,0,0])?;}
        p.release_all()?;
        let mut reports=drain(&p,&rx[1],1);
        assert_eq!(p.waiting.len(),64);
        let mut old=command(Op::Decision,1,[3,99,0,0]);old.id=p.waiting[0].seq;
        p.apply(old,now_ns(),0)?;
        assert!(p.waiting.iter().all(|w|w.decision.is_none()));
        p.apply(command(op,1,[0;4]),now_ns(),0)?;
        assert_eq!(p.waiting.len(),64);
        p.drain_decisions(1)?;
        reports.extend(drain(&p,&rx[1],1));
        assert!(p.waiting.is_empty());assert!(!p.devices[1].restore_pending);
        let total=reports.iter().map(|r|p.devices[1].lane.codec.decode(2,r.bytes()).unwrap().x).sum::<i32>();
        assert_eq!(total,256);
        let last=p.devices[1].lane.codec.decode(2,last(&reports)?.bytes())?;
        assert_eq!((last.x,last.buttons),(0,0));
    }
    Ok(())
}

#[test]
fn full_external_waiting_applies_backpressure_only_to_external_input() -> io::Result<()> {
    let(mut p,rx)=fixture()?;
    p.apply(command(Op::Takeover,1,[3,100000,0,0]),now_ns(),0)?;
    drain(&p,&rx[1],1);
    for _ in 0..256 {input(&mut p,1,&[2,0,0,1,0,0,0,0])?;}
    p.inputs.clear();let mut senders=Vec::new();
    for _ in 0..2 {let(tx,receiver)=mpsc::sync_channel(256);senders.push(tx);p.inputs.push(receiver);}
    for (target,bytes) in [(0,[0,0,4,0,0,0,0,0]),(1,[2,0,0,2,0,0,0,0])] {
        senders[target].send(Input{target,id:p.devices[target].lane.physical.layout.input_id(&bytes)?,
            report:Report::from_slice(&bytes)?,at:Instant::now()}).unwrap();
    }
    assert!(p.drain_inputs()?);assert_eq!(p.waiting.len(),256);
    assert_eq!(last(&drain(&p,&rx[0],0))?.data[2],4);
    assert_eq!(p.inputs[1].try_recv().unwrap().report.data[3],2);
    Ok(())
}

#[test]
fn saturated_outputs_preserve_physical_transitions_and_merge_only_identical_snapshots() -> io::Result<()> {
    let(mut p,rx)=fixture()?;
    for t in 0..2 {p.devices[t].lane.queued.store(256,Ordering::Release);}
    let down=[0,0,4,0,0,0,0,0];let up=[0;8];
    input(&mut p,0,&down)?;input(&mut p,0,&up)?;
    input(&mut p,1,&[2,1,0,7,0,0,0,255])?;
    input(&mut p,1,&[2,0,0,3,0,0,0,1])?;
    for _ in 0..300 {p.restore(0,true,None)?;}
    assert_eq!(p.devices[0].output_pending.borrow().len(),3);
    assert_eq!(p.devices[1].output_pending.borrow().len(),2);
    p.release_all()?;
    for t in 0..2 {p.devices[t].lane.queued.store(0,Ordering::Release);p.drain_output(t)?;p.drain_decisions(t)?;}
    let key=drain(&p,&rx[0],0);
    assert_eq!(key[0].bytes(),down);assert_eq!(key[1].bytes(),up);
    let mouse=drain(&p,&rx[1],1);
    assert_eq!(mouse.len(),2);
    for (r,(x,buttons,wheel)) in mouse.iter().zip([(7,1,-1),(3,0,1)]) {
        let v=p.devices[1].lane.codec.decode(2,r.bytes())?;
        assert_eq!((v.x,v.buttons,v.wheel),(x,buttons,wheel));
    }
    assert!(p.devices.iter().all(|d|d.output_pending.borrow().is_empty()));
    input(&mut p,0,&down)?;
    assert_eq!(last(&drain(&p,&rx[0],0))?.bytes(),down);
    Ok(())
}

#[test]
fn composite_media_system_and_keypad_reports_preserve_press_release() -> io::Result<()> {
    let auxiliary = [
        5, 0x0c, 9, 1, 0xa1, 1, 0x85, 3, 0x15, 0, 0x26, 0xff, 3, 0x19, 0, 0x2a, 0xff, 3, 0x75, 16,
        0x95, 1, 0x81, 0, 0xc0, 5, 1, 9, 0x80, 0xa1, 1, 0x85, 4, 0x15, 0, 0x25, 1, 0x19, 0x81,
        0x29, 0x83, 0x75, 1, 0x95, 3, 0x81, 2, 0x75, 5, 0x95, 1, 0x81, 3, 0xc0,
    ];
    let (mut p, rx) = fixture_interfaces(&[KEYBOARD, MOUSE, &auxiliary, KEYBOARD])?;
    // Consumer and system report IDs share the third physical interface.
    for bytes in [&[3, 0xe9, 0][..], &[3, 0, 0], &[4, 2], &[4, 0]] {
        input(&mut p, 2, bytes)?;
        assert_eq!(last(&drain(&p, &rx[2], 2))?.bytes(), bytes);
    }
    // Num Lock and keypad digits must survive on both primary and auxiliary keyboards.
    for t in [0, 3] {
        for key in [0x53, 0x59, 0x62, 0x63] {
            let bytes = [0, 0, key, 0, 0, 0, 0, 0];
            input(&mut p, t, &bytes)?;
            assert_eq!(last(&drain(&p, &rx[t], t))?.bytes(), bytes);
            input(&mut p, t, &[0; 8])?;
            assert_eq!(last(&drain(&p, &rx[t], t))?.bytes(), &[0; 8]);
        }
    }
    // Exercise effect expiry and cancellation beyond the former two-lane limit.
    let now = now_ns();
    let mut effect = command(Op::ForceKey, 3, [0x59, 1, 10, 0]);
    effect.id = 900;
    p.apply(effect, now, 0)?;
    assert_eq!(last(&drain(&p, &rx[3], 3))?.data[2], 0x59);
    p.tick(now + 11_000_000)?;
    assert_eq!(last(&drain(&p, &rx[3], 3))?.bytes(), &[0; 8]);
    p.apply(effect, now_ns(), 0)?;
    drain(&p, &rx[3], 3);
    let mut cancel = command(Op::Cancel, 3, [0; 4]);
    cancel.id = 900;
    p.apply(cancel, now_ns(), 0)?;
    assert_eq!(last(&drain(&p, &rx[3], 3))?.bytes(), &[0; 8]);
    Ok(())
}

#[test]
fn integrated_state_and_queue_scenarios() -> io::Result<()> {
    let (mut p, rx) = fixture()?;
    let now = now_ns();
    input(&mut p, 0, &[0, 0, 4, 0, 0, 0, 0, 0])?;
    drain(&p, &rx[0], 0);
    p.apply(command(Op::ForceKey, 0, [4, 0, 10, 0]), now, 0)?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[2], 0);
    input(&mut p, 0, &[0, 0, 4, 0, 0, 0, 0, 0])?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[2], 0);
    p.tick(now + 11_000_000)?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[2], 4);
    p.apply(command(Op::Takeover, 0, [0; 4]), now, 0)?;
    drain(&p, &rx[0], 0);
    let mut press = command(Op::Key, 0, [0xe0, 1, 0, 0]);
    press.id = 99;
    p.apply(press, now, 0)?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[0], 1);
    let mut cancel = command(Op::Cancel, 0, [0; 4]);
    cancel.id = 99;
    p.apply(cancel, now, 0)?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[0], 0);
    p.apply(command(Op::Release, 0, [0; 4]), now, 0)?;
    drain(&p, &rx[0], 0);
    input(&mut p, 0, &[0; 8])?;
    drain(&p, &rx[0], 0);
    let mut rule = command(Op::Trigger, 0, [1, 0x90001, 3, Op::ForceKey as i32]);
    for (i, a) in [4_i32, 1, 0, 0].iter().enumerate() {
        rule.data[i * 4..i * 4 + 4].copy_from_slice(&a.to_le_bytes());
    }
    p.apply(rule, now, 0)?;
    input(&mut p, 1, &[2, 1, 0, 2, 0, 3, 0, 0])?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[2], 4);
    drain(&p, &rx[1], 1);
    input(&mut p, 1, &[2, 0, 0, 0, 0, 0, 0, 0])?;
    assert_eq!(last(&drain(&p, &rx[0], 0))?.data[2], 0);
    drain(&p, &rx[1], 1);
    p.apply(command(Op::Scale, 1, [32768, 32768, 0, 0]), now, 0)?;
    input(&mut p, 1, &[2, 0, 0, 1, 0, 0, 0, 0])?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[3], 0);
    input(&mut p, 1, &[2, 0, 0, 1, 0, 0, 0, 0])?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[3], 1);
    p.apply(command(Op::Takeover, 1, [2, 100000, 0, 0]), now_ns(), 0)?;
    drain(&p, &rx[1], 1);
    input(&mut p, 1, &[2, 0, 0, 5, 0, 0, 0, 0])?;
    let first = p.seq;
    input(&mut p, 1, &[2, 0, 0, 7, 0, 0, 0, 0])?;
    let second = p.seq;
    let mut decision = command(Op::Decision, 1, [0; 4]);
    decision.id = second;
    p.apply(decision, now_ns(), 0)?;
    assert!(drain(&p, &rx[1], 1).is_empty());
    decision.id = first;
    p.apply(decision, now_ns(), 0)?;
    let reports = drain(&p, &rx[1], 1);
    assert_eq!(reports.iter().map(|r| r.data[3]).collect::<Vec<_>>(), vec![5, 7]);
    p.release_all()?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[3], 0);
    Ok(())
}

#[test]
fn capture_age_and_expired_fifo_decisions() -> io::Result<()> {
    let (mut p, rx) = fixture()?;
    p.apply(command(Op::Takeover, 1, [2, 500, 1, 0]), now_ns(), 0)?;
    drain(&p, &rx[1], 1);
    p.input(Input {
        target: 1,
        id: 2,
        report: Report::from_slice(&[2, 0, 0, 5, 0, 0, 0, 0])?,
        at: Instant::now() - std::time::Duration::from_millis(2),
    })?;
    assert!(p.waiting[0].until < now_ns());
    p.tick(now_ns())?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[3], 5);
    input(&mut p, 1, &[2, 0, 0, 7, 0, 0, 0, 0])?;
    let mut m = command(Op::Decision, 1, [1, 8, 0, 0]);
    m.id = p.seq;
    let replacement = Report::from_slice(&[2, 0, 0, 99, 0, 0, 0, 0])?;
    p.waiting[0].decision = Some((Some(replacement), m));
    p.waiting[0].until = now_ns() - 1;
    p.drain_decisions(1)?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[3], 7);
    Ok(())
}

#[test]
fn external_xy_preserves_buttons_wheel_and_entry_state() -> io::Result<()> {
    let (mut p, rx) = fixture()?;
    input(&mut p, 1, &[2, 1, 0, 0, 0, 0, 0, 0])?;
    drain(&p, &rx[1], 1);
    p.apply(command(Op::Takeover, 1, [3, 100000, 1, 0]), now_ns(), 0)?;
    assert_eq!(last(&drain(&p, &rx[1], 1))?.data[1], 1);
    input(&mut p, 1, &[2, 1, 0, 5, 0, 6, 0, 3])?;
    let mut m = command(Op::Decision, 1, [3, 8, -4, 0]);
    m.id = p.seq;
    p.apply(m, now_ns(), 0)?;
    let reports = drain(&p, &rx[1], 1);
    let v = p.devices[1].lane.codec.decode(2, last(&reports)?.bytes())?;
    assert_eq!((v.x, v.y, v.buttons, v.wheel), (8, -4, 1, 3));
    Ok(())
}

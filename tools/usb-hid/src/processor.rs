// SPDX-License-Identifier: GPL-2.0-only
//! 串行物理队列、处理阶段和提交队列。
mod commands;
#[cfg(test)]
mod scenarios;
mod output;
mod variables;
use crate::{
    STOP,
    actions::{Effect, Rule},
    codec::{Codec, Report, Values},
    physical::Physical,
    relay::{Input, Packet},
    service::{Bus, Envelope, Session},
    stats::Stats,
};
use std::{
    cell::{Cell,RefCell},
    collections::{BTreeMap,VecDeque},
    io,
    sync::{
        Arc,
        atomic::{AtomicU64, AtomicUsize, Ordering},
        mpsc,
    },
    time::Instant,
};
use vs680_hid::{
    trajectory::{Remainder, Vector},
    wire::{Message, Op, Status, invalid, now_ns},
};

pub(crate) struct Lane {
    pub(crate) wake: Arc<vs680_hid::shared::Wake>,
    pub(crate) feedback: Arc<crate::motion_feedback::Feedback>,
    pub(crate) physical: Arc<Physical>,
    pub(crate) codec: Codec,
    pub(crate) stats: Arc<Stats>,
    pub(crate) sender: mpsc::SyncSender<Packet>,
    pub(crate) generation: Arc<AtomicU64>,
    pub(crate) queued: Arc<AtomicUsize>,
}
struct Device {
    lane: Lane,
    frames: BTreeMap<u8, Report>,
    full: bool,
    external: bool,
    external_xy: bool,
    owned: Values,
    remainder: Remainder,
    budget_ns: u64,
    fail_open: bool,
    direct: bool,
    // Restore current key/button state after the retained physical FIFO.
    restore_pending: bool,
    output_pending: RefCell<VecDeque<Packet>>,
    output_restore: Cell<bool>,
}
struct Waiting {
    timing: crate::stats::InputTiming,
    input: Input,
    seq: u64,
    until: u64,
    decision: Option<(Option<Report>, Message)>,
    // 外部处理结束后，按顺序发送原始物理报告。
    physical: bool,
}
struct Scheduled {
    message: Message,
    rule: u64,
}
pub(crate) struct Processor {
    input_timing: Option<crate::stats::InputTiming>,
    devices: Vec<Device>,
    bus: Arc<Bus>,
    commands: mpsc::Receiver<Envelope>,
    inputs: Vec<mpsc::Receiver<Input>>,
    session: Option<Arc<Session>>,
    revision: u64,
    seq: u64,
    last_command: u64,
    next_action: u64,
    effects: Vec<Effect>,
    rules: Vec<Rule>,
    scheduled: Vec<Scheduled>,
    waiting: Vec<Waiting>,
    holds: BTreeMap<(usize, u64), Values>,
    applying_scheduled: bool,
    variables: variables::Variables,
}
impl Processor {
    pub(crate) fn new(
        lanes: Vec<Lane>,
        bus: Arc<Bus>,
        commands: mpsc::Receiver<Envelope>,
        inputs: Vec<mpsc::Receiver<Input>>,
    ) -> Self {
        let device_count = lanes.len();
        Self {
            input_timing: None,
            devices: lanes
                .into_iter()
                .map(|lane| Device {
                    lane,
                    frames: BTreeMap::new(),
                    full: false,
                    external: false,
                    external_xy: false,
                    owned: Values::default(),
                    remainder: Remainder::default(),
                    budget_ns: 500_000,
                    fail_open: false,
                    direct: false,
                    restore_pending: false,
                    output_pending: RefCell::new(VecDeque::new()),
                    output_restore: Cell::new(false),
                })
                .collect(),
            bus,
            commands,
            inputs,
            session: None,
            revision: u64::MAX,
            seq: 0,
            last_command: 0,
            next_action: 1,
            effects: Vec::with_capacity(128),
            rules: Vec::with_capacity(64),
            scheduled: Vec::with_capacity(512),
            waiting: Vec::with_capacity(256),
            holds: BTreeMap::new(),
            applying_scheduled: false,
            variables: variables::Variables::new(device_count),
        }
    }
    pub(crate) fn run(mut self) -> io::Result<()> {
        for d in &mut self.devices {
            for id in d.lane.codec.ids() {
                let report = d
                    .lane
                    .physical
                    .latest_input(id)
                    .and_then(|b| Report::from_slice(&b).ok())
                    .unwrap_or(d.lane.codec.blank(id)?);
                d.frames.insert(id, report);
            }
        }
        while !STOP.load(Ordering::Acquire) {
            self.bus.wake.drain();
            let now = now_ns();
            let rev = self.bus.revision.load(Ordering::Acquire);
            if rev != self.revision {
                self.release_all()?;
                for d in &mut self.devices {
                    for (id, r) in &mut d.frames {
                        if let Some(b) = d.lane.physical.latest_input(*id) {
                            *r = Report::from_slice(&b)?;
                        }
                    }
                }
                self.session = self.bus.current();
                self.revision = rev;
                self.last_command = 0;
            }
            if self.session.as_ref().is_some_and(|s| !s.live(now)) {
                self.release_all()?;
                self.session = None;
            }
            let mut pending_work = false;
            for i in 0..16 {
                let e = self
                    .bus
                    .urgent
                    .lock()
                    .unwrap_or_else(std::sync::PoisonError::into_inner)
                    .pop_front();
                if let Some(e) = e {
                    self.envelope(e, now)?;
                    pending_work |= i == 15;
                } else {
                    break;
                }
            }
            for i in 0..32 {
                match self.commands.try_recv() {
                    Ok(e) => {
                        self.envelope(e, now)?;
                        pending_work |= i == 31;
                    }
                    Err(mpsc::TryRecvError::Empty) => break,
                    Err(_) => return Err(invalid("control channel closed")),
                }
            }
            if let Some(s) = self.session.clone() {
                for i in 0..64 {
                    match s.shared.pop(0) {
                        Ok(Some(m)) => {
                            self.command(m, now, 0)?;
                            pending_work |= i == 63;
                        }
                        Ok(None) => break,
                        Err(_) => {
                            s.active.store(false, Ordering::Release);
                            break;
                        }
                    }
                }
            }
            // 限制单轮批量，保证定时器与控制撤销能够得到处理。
            let active = self.drain_inputs()?;
            self.tick(now_ns())?;
            if active || pending_work {
                continue;
            }
            let now = now_ns();
            let mut next = now + 100_000_000;
            if self.rules.iter().any(|r| r.source == usize::MAX) {
                next = next.min(now + 1_000_000);
            }
            if let Some(s) = &self.session {
                next = next.min(s.expires.load(Ordering::Acquire));
            }
            for e in &self.effects {
                next = next.min(e.end);
                if matches!(e.command.op, Op::Velocity | Op::Offset) {
                    next = next.min(now + 1_000_000);
                }
            }
            for e in &self.scheduled {
                next = next.min(e.message.at);
            }
            for w in &self.waiting {
                if !w.physical && self.devices[w.input.target].lane.queued.load(Ordering::Acquire)<192 {
                    next = next.min(w.until);
                }
            }
            let delta = next.saturating_sub(now);
            let ts = libc::timespec {
                tv_sec: (delta / 1_000_000_000) as _,
                tv_nsec: (delta % 1_000_000_000) as _,
            };
            let mut p = libc::pollfd { fd: self.bus.wake.fd(), events: libc::POLLIN, revents: 0 };
            let n = unsafe { libc::ppoll(&raw mut p, 1, &raw const ts, std::ptr::null()) };
            if n < 0 && io::Error::last_os_error().kind() != io::ErrorKind::Interrupted {
                return Err(io::Error::last_os_error());
            }
        }
        Ok(())
    }
    fn drain_inputs(&mut self) -> io::Result<bool> {
        let mut active = false;
        for t in 0..self.inputs.len() {
            self.drain_output(t)?;
            if !self.devices[t].output_pending.borrow().is_empty() {continue;}
            self.drain_decisions(t)?;
            if self.waiting.iter().any(|w|w.input.target==t && w.physical) {continue;}
            for _ in 0..32 {
                // 按接口限制输入，给控制命令和释放按键的报告预留队列空间。
                if self.devices[t].lane.queued.load(Ordering::Acquire) >= 192 {
                    break;
                }
                if !self.devices[t].output_pending.borrow().is_empty() {break;}
                if self.devices[t].external && self.waiting.len()>=256 {break;}
                match self.inputs[t].try_recv() {
                    Ok(i) => {
                        self.input(i)?;
                        active = true;
                    }
                    Err(mpsc::TryRecvError::Empty) => break,
                    Err(_) => return Err(invalid("physical input channel closed")),
                }
            }
        }
        Ok(active)
    }
    fn physical_values(&self, t: usize) -> Values {
        let d = &self.devices[t];
        let mut v = Values::default();
        for (&id, b) in &d.frames {
            if let Ok(p) = d.lane.codec.decode(id, b.bytes()) {
                for i in 0..4 {
                    v.keys.0[i] |= p.keys.0[i];
                }
                v.buttons |= p.buttons;
            }
        }
        v
    }
    fn base_values(&self, t: usize, id: u8) -> io::Result<Values> {
        let d = &self.devices[t];
        let mut v = if d.full {
            d.owned.clone()
        } else {
            d.lane
                .codec
                .decode(id, d.frames.get(&id).ok_or_else(|| invalid("template"))?.bytes())?
        };
        v.x = 0;
        v.y = 0;
        v.wheel = 0;
        v.pan = 0;
        for (&(target, _), held) in &self.holds {
            if target == t {
                for i in 0..4 {
                    v.keys.0[i] |= held.keys.0[i];
                }
                v.buttons |= held.buttons;
            }
        }
        Ok(v)
    }
    fn encode(&self, t: usize, id: u8, v: &Values) -> io::Result<Report> {
        let d = &self.devices[t];
        d.lane.codec.encode(id, d.frames.get(&id).ok_or_else(|| invalid("template"))?.bytes(), v)
    }
    fn render(&mut self, t: usize, id: u8, motion: Vector) -> io::Result<Report> {
        self.render_motion(t, id, motion, true)
    }
    fn render_motion(&mut self, t: usize, id: u8, mut motion: Vector, transform: bool) -> io::Result<Report> {
        let mut v = self.base_values(t, id)?;
        for e in &self.effects {
            if e.command.target as usize == t {
                e.apply(&mut v);
                if let Some(m) = e.transform.filter(|_| transform) {
                    motion = m.apply(motion);
                }
            }
        }
        if Some(id) == self.devices[t].lane.codec.mouse {
            let (x, y) = self.devices[t].remainder.quantize(motion)
                .ok_or_else(|| invalid("motion overflow"))?;
            v.x = x;
            v.y = y;
        }
        self.encode(t, id, &v)
    }
    fn input(&mut self, i: Input) -> io::Result<()> {
        self.input_timing =
            Some(crate::stats::InputTiming { received: i.at, started: Instant::now() });
        let result = self.process_input(i);
        self.input_timing = None;
        result
    }
    fn process_input(&mut self, i: Input) -> io::Result<()> {
        self.seq = self.seq.wrapping_add(1);
        let seq = self.seq;
        let t = i.target;
        self.devices[t].frames.insert(i.id, i.report);
        self.devices[t].lane.physical.cache_input(i.id, i.report.bytes());
        if Some(i.id) == self.devices[t].lane.codec.mouse {
            let physical = self.devices[t].lane.codec.decode(i.id, i.report.bytes())?;
            self.variables.observe(t, physical.x, physical.y, physical.buttons, now_ns().saturating_sub(i.at.elapsed().as_nanos().min(u64::MAX as u128) as u64));
        }
        if let Some(s) = &self.session {
            let mut m = Message::new(Op::Input);
            m.seq = seq;
            m.at = now_ns().saturating_sub(i.at.elapsed().as_nanos().min(u64::MAX as u128) as u64);
            m.target = t as u32;
            m.args[0] = i.report.len as i32;
            m.args[1] = i.id as i32;
            m.data = i.report.data;
            if self.devices[t].external
                && self.devices[t].external_xy
                && Some(i.id) == self.devices[t].lane.codec.mouse
            {
                let v = self.devices[t].lane.codec.decode(i.id, i.report.bytes())?;
                m.op = Op::MotionInput;
                m.args = [v.x, v.y, v.wheel, v.pan];
                m.data = [0; 64];
            }
            s.input(m);
        }
        let values = if self.rules.iter().any(|r| r.source == t) {
            self.physical_values(t)
        } else {
            Values::default()
        };
        let mut triggered = Vec::new();
        let mut ended = Vec::new();
        for r in &mut self.rules {
            if r.source == t {
                let state = r.condition(&values);
                let (start, end) = r.evaluate(state);
                if start {
                    triggered.push((r.id, r.effect));
                }
                if end {
                    ended.push(r.id);
                }
            }
        }
        for id in ended {
            let targets: Vec<_> = self
                .effects
                .iter()
                .filter(|e| e.rule == id)
                .map(|e| e.command.target as usize)
                .collect();
            self.effects.retain(|e| e.rule != id);
            for target in targets {
                self.restore(target, true, None)?;
            }
        }
        for (id, e) in triggered {
            if !self.devices[e.target as usize].full && !self.effects.iter().any(|a| a.rule == id) {
                self.apply(e, now_ns(), id)?;
            }
        }
        if self.devices[t].full {
            return Ok(());
        }
        if self.devices[t].external {
            if self.waiting.len() >= 256 {
                return Err(invalid("external processing queue full"));
            }
            self.waiting.push(Waiting {
                timing: self.input_timing.expect("physical input timing"),
                until: now_ns()
                    .saturating_sub(i.at.elapsed().as_nanos().min(u64::MAX as u128) as u64)
                    .saturating_add(self.devices[t].budget_ns),
                input: i,
                seq,
                decision: None,
                physical: false,
            });
            return Ok(());
        }
        if (!self.effects.iter().any(|e| e.command.target as usize == t)
            && !self.holds.keys().any(|(target, _)| *target == t))
            || !self.devices[t].lane.codec.managed(i.id)
        {
            let raw=i.report;
            return self.emit_merged(t,raw,raw,i.at);
        }
        let v = self.devices[t].lane.codec.decode(i.id, i.report.bytes())?;
        let mut out = self.render(t, i.id, Vector { x: v.x as f64, y: v.y as f64 })?;
        // 轨迹变换只修改XY，保留同帧中的滚轮增量。
        let mut transformed = self.devices[t].lane.codec.decode(i.id, out.bytes())?;
        transformed.wheel = v.wheel;
        transformed.pan = v.pan;
        out = self.encode(t, i.id, &transformed)?;
        self.emit_merged(t,out,i.report,i.at)
    }
    fn tick(&mut self, now: u64) -> io::Result<()> {
        self.variable_rules(now)?;
        let mut steps = 0;
        while self.scheduled.first().is_some_and(|s| s.message.at <= now) && steps < 64 {
            let s = self.scheduled.remove(0);
            steps += 1;
            if s.message.expires != 0 && now > s.message.expires {
                self.effects.retain(|e| e.command.id != s.message.id);
                self.holds.retain(|(_, id), _| *id != s.message.id);
                self.scheduled.retain(|e| e.message.id != s.message.id);
                self.reply(&s.message, Status::Expired);
                self.restore(s.message.target as usize, true, None)?;
            } else {
                self.applying_scheduled = true;
                self.apply(s.message, now, s.rule)?;
                self.applying_scheduled = false;
            }
        }
        let mut delta = [Vector::default(); crate::layout::MAX_INTERFACES];
        let mut changed = [false; crate::layout::MAX_INTERFACES];
        for e in &mut self.effects {
            let t = e.command.target as usize;
            let v = e.advance(now);
            delta[t].x += v.x;
            delta[t].y += v.y;
            if e.end <= now {
                changed[t] = true;
            }
        }
        self.effects.retain(|e| e.end > now);
        for t in 0..self.devices.len() {
            if delta[t].x != 0. || delta[t].y != 0. {
                if let Some(id) = self.devices[t].lane.codec.mouse {
                    let r = self.render_motion(t, id, delta[t], false)?;
                    self.emit(t, r, Instant::now(), None, false)?;
                }
            }
            if changed[t] {
                self.restore(t, true, None)?;
            }
        }
        for t in 0..self.devices.len() {
            self.drain_decisions(t)?;
        }
        Ok(())
    }
    fn drain_decisions(&mut self, t: usize) -> io::Result<()> {
        while let Some(i) = self.waiting.iter().position(|w| w.input.target == t) {
            if self.devices[t].lane.queued.load(Ordering::Acquire)>=192 {break;}
            if self.waiting[i].physical {
                let w=self.waiting.remove(i);
                let raw=w.input.report;
                self.emit_waiting(&w,raw,None)?;
                continue;
            }
            if now_ns() >= self.waiting[i].until {
                if self.devices[t].fail_open {
                    let w = self.waiting.remove(i);
                    if let Some((_,m))=w.decision {self.reply(&m,Status::Expired);}
                    let raw=w.input.report;
                    self.emit_waiting(&w,raw,None)?;
                } else {
                    self.invalidate(t);
                    self.devices[t].external = false;
                    self.devices[t].external_xy = false;
                    self.physical_waiting(Some(t));
                    self.devices[t].restore_pending=true;
                }
                continue;
            }
            if self.waiting[i].decision.is_none() {
                break;
            }
            let mut w = self.waiting.remove(i);
            if let Some((report, message)) = w.decision.take() {
                if let Some(r) = report {
                    self.emit_waiting(&w, r, Some(message))?;
                } else {
                    self.reply(&message, Status::Applied);
                }
            }
        }
        if self.devices[t].restore_pending && !self.waiting.iter().any(|w|w.input.target==t)
            && self.devices[t].lane.queued.load(Ordering::Acquire)<192 {
            self.restore(t,true,None)?;
            self.devices[t].restore_pending=false;
        }
        Ok(())
    }
    fn physical_waiting(&mut self,target:Option<usize>) {
        let mut expired=Vec::new();
        for w in &mut self.waiting {
            if target.is_none_or(|t|w.input.target==t) {
                w.physical=true;
                if let Some((_,m))=w.decision.take(){expired.push(m);}
            }
        }
        for m in expired {self.reply(&m,Status::Expired);}
    }
    fn emit_waiting(&mut self, w: &Waiting, r: Report, ticket: Option<Message>) -> io::Result<()> {
        let raw=w.input.report;
        let previous = self.input_timing.replace(w.timing);
        let result = self.submit(w.input.target,r,w.input.at,ticket,false,Some(raw));
        self.input_timing = previous;
        result
    }
    fn invalidate(&self, t: usize) {
        self.devices[t].lane.generation.fetch_add(1, Ordering::AcqRel);
    }
    fn restore(&mut self, t: usize, physical: bool, ticket: Option<Message>) -> io::Result<()> {
        let ids: Vec<_> = self.devices[t]
            .lane
            .codec
            .ids()
            .filter(|id| self.devices[t].lane.codec.managed(*id))
            .collect();
        for (index, id) in ids.iter().enumerate() {
            let report = if physical {
                self.render(t, *id, Vector::default())?
            } else {
                self.encode(t, *id, &Values::default())?
            };
            let last = index + 1 == ids.len();
            self.emit(t, report, Instant::now(), if last { ticket } else { None }, true)?;
        }
        if ids.is_empty() {
            if let Some(t) = ticket {
                self.reply(&t, Status::Unsupported);
            }
        }
        Ok(())
    }
    fn release_all(&mut self) -> io::Result<()> {
        let had = self.session.is_some();
        let touched: Vec<_> = (0..self.devices.len())
            .map(|t| {
                self.devices[t].full
                    || self.devices[t].external
                    || self.devices[t].direct
                    || self.devices[t].restore_pending
                    || self.effects.iter().any(|e| e.command.target as usize == t)
                    || self.holds.keys().any(|(target, _)| *target == t)
            })
            .collect();
        self.effects.clear();
        self.rules.clear();
        self.scheduled.clear();
        self.variables.clear();
        self.physical_waiting(None);
        self.holds.clear();
        if had {
            for t in 0..self.devices.len() {
                if !touched[t] {
                    continue;
                }
                self.invalidate(t);
                self.devices[t].full = false;
                self.devices[t].external = false;
                self.devices[t].external_xy = false;
                self.devices[t].direct = false;
                self.devices[t].remainder = Remainder::default();
                self.devices[t].restore_pending=true;
                self.drain_decisions(t)?;
            }
        }
        Ok(())
    }
}

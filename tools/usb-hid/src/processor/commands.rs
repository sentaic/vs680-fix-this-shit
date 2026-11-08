// SPDX-License-Identifier: GPL-2.0-only
//! 控制命令验证、动作安装和规则变更。
use super::*;
impl Processor {
    pub(super) fn reply(&self, m: &Message, status: Status) {
        if let Some(s) = &self.session {
            s.respond(m, status);
        }
    }
    pub(super) fn valid(&self, m: &Message, now: u64) -> Result<(), Status> {
        let d = self.devices.get(m.target as usize).ok_or(Status::Invalid)?;
        let c = &d.lane.codec;
        if m.expires != 0 && (m.expires < now || m.at > m.expires) {
            return Err(Status::Expired);
        }
        if m.at > now + 3_600_000_000_000 {
            return Err(Status::Invalid);
        }
        match m.op {
            Op::Variable => {
                if !(0..64).contains(&m.args[0]) || m.args[2] < 0 { return Err(Status::Invalid); }
            }
            Op::Physical | Op::Output => {
                if !(0..=255).contains(&m.args[0]) {
                    return Err(Status::Invalid);
                }
            }
            Op::Capabilities => {
                if m.args[0] < 0 {
                    return Err(Status::Invalid);
                }
            }
            Op::Key | Op::ForceKey => {
                if m.args[0] < 4 || !c.supports_key(m.args[0] as u32) {
                    return Err(Status::Unsupported);
                }
                if !(0..=1).contains(&m.args[1]) {
                    return Err(Status::Invalid);
                }
            }
            Op::Button | Op::ForceButton => {
                if m.args[0] < 1 || !c.supports_button(m.args[0] as u32) {
                    return Err(Status::Unsupported);
                }
                if !(0..=1).contains(&m.args[1]) {
                    return Err(Status::Invalid);
                }
            }
            Op::Move
            | Op::Scroll
            | Op::Velocity
            | Op::Offset
            | Op::Rotate
            | Op::Scale
            | Op::Buttons => {
                if c.mouse.is_none() {
                    return Err(Status::Unsupported);
                }
            }
            Op::Keys => {
                if c.keyboard.is_none() {
                    return Err(Status::Unsupported);
                }
            }
            Op::Raw => {
                let len = usize::try_from(m.args[0]).map_err(|_| Status::Invalid)?;
                if len == 0 || len > 64 || d.lane.physical.layout.input_id(&m.data[..len]).is_err()
                {
                    return Err(Status::Invalid);
                }
            }
            Op::Hello
            | Op::Ping
            | Op::Goodbye
            | Op::Input
            | Op::MotionInput
            | Op::Gap
            | Op::Response
            | Op::Batch => {
                return Err(Status::Invalid);
            }
            _ => {}
        }
        if matches!(
            m.op,
            Op::ForceKey | Op::ForceButton | Op::Velocity | Op::Offset | Op::Scale | Op::Rotate
        ) && !(0..=3_600_000).contains(&m.args[2])
        {
            return Err(Status::Invalid);
        }
        if m.op == Op::Scale
            && (m.args[0].unsigned_abs() > 64 * 65536 || m.args[1].unsigned_abs() > 64 * 65536)
        {
            return Err(Status::Invalid);
        }
        if m.op == Op::Takeover
            && (!(0..=3).contains(&m.args[0])
                || m.args[1] < 0
                || m.args[1] > 100_000
                || !(0..=1).contains(&m.args[2]))
        {
            return Err(Status::Invalid);
        }
        Ok(())
    }
    pub(super) fn envelope(&mut self, e: Envelope, now: u64) -> io::Result<()> {
        if self.session.as_ref().is_none_or(|s| s.id != e.session.id) || !e.session.live(now) {
            e.session.respond(&e.header, Status::Stale);
            return Ok(());
        }
        // 普通套接字命令之前先消费已经发布的共享命令，保持同一客户端的顺序。
        if !matches!(e.header.op, Op::Cancel | Op::CancelAll | Op::Release) {
            for _ in 0..256 {
                match e.session.shared.pop(0) {
                    Ok(Some(m)) => self.command(m, now, 0)?,
                    Ok(None) => break,
                    Err(_) => {
                        e.session.active.store(false, Ordering::Release);
                        return Ok(());
                    }
                }
            }
        }
        if e.header.op != Op::Batch {
            return self.command(e.header, now, 0);
        }
        if e.header.expires != 0 && (e.header.expires < now || e.header.at > e.header.expires) {
            self.reply(&e.header, Status::Expired);
            return Ok(());
        }
        if e.messages.len() + self.scheduled.len() > 512 {
            self.reply(&e.header, Status::Full);
            return Ok(());
        }
        let allowed = |m: &Message| {
            matches!(
                m.op,
                Op::Key
                    | Op::Button
                    | Op::Move
                    | Op::Scroll
                    | Op::ForceKey
                    | Op::ForceButton
                    | Op::Scale
                    | Op::Rotate
                    | Op::Velocity
                    | Op::Offset
                    | Op::Raw
                    | Op::Keys
                    | Op::Buttons
            )
        };
        if e.messages.iter().any(|m| !allowed(m) || self.valid(m, now).is_err())
            || e.header.seq <= self.last_command
        {
            self.reply(&e.header, Status::Invalid);
            return Ok(());
        }
        self.last_command = e.header.seq;
        let group = if e.header.id == 0 { self.allocate_id() } else { e.header.id };
        for mut m in e.messages {
            m.at = m.at.max(now);
            m.id = group;
            m.seq = e.header.seq;
            m.flags = 0;
            self.scheduled.push(Scheduled { message: m, rule: 0 });
        }
        self.scheduled.sort_by_key(|s| (s.message.at, s.message.seq));
        let mut r = e.header.response(Status::Queued);
        r.id = group;
        if let Some(s) = &self.session {
            s.reply(r);
        }
        Ok(())
    }
    pub(super) fn allocate_id(&mut self) -> u64 {
        loop {
            let id = self.next_action;
            self.next_action = self.next_action.wrapping_add(1).max(1);
            if !self.effects.iter().any(|a| a.command.id == id)
                && !self.rules.iter().any(|r| r.id == id)
                && !self.scheduled.iter().any(|s| s.message.id == id)
                && !self.holds.keys().any(|(_, group)| *group == id)
            {
                return id;
            }
        }
    }
    pub(super) fn command(&mut self, mut m: Message, now: u64, rule: u64) -> io::Result<()> {
        if self.session.is_none() {
            return Ok(());
        }
        if rule == 0 && m.seq != 0 {
            if m.seq <= self.last_command {
                self.reply(&m, Status::Stale);
                return Ok(());
            }
            self.last_command = m.seq;
        }
        if let Err(s) = self.valid(&m, now) {
            self.reply(&m, s);
            return Ok(());
        }
        if m.at > now {
            if self.scheduled.len() >= 512 {
                self.reply(&m, Status::Full);
            } else {
                if m.id == 0 {
                    m.id = self.allocate_id();
                }
                self.reply(&m, Status::Queued);
                self.scheduled.push(Scheduled { message: m, rule });
                self.scheduled.sort_by_key(|s| (s.message.at, s.message.seq));
            }
            return Ok(());
        }
        self.apply(m, now, rule)
    }
    pub(super) fn apply(&mut self, mut m: Message, now: u64, rule: u64) -> io::Result<()> {
        let t = m.target as usize;
        match m.op {
            Op::Subscribe => {
                if let Some(s) = &self.session {
                    s.observe.store(m.args[0] != 0, Ordering::Release);
                }
                self.reply(&m, Status::Applied);
            }
            Op::Capabilities => {
                let d = &self.devices[t];
                let mut r = m.response(Status::Applied);
                r.args[1] = i32::from(d.lane.codec.keyboard.is_some())
                    | i32::from(d.lane.codec.mouse.is_some()) << 1;
                r.args[2] = 64;
                r.args[3] = self.devices.len() as i32;
                r.data[0] = d.lane.codec.keyboard.unwrap_or(0);
                r.data[1] = d.lane.codec.mouse.unwrap_or(0);
                let lengths = &d.lane.physical.layout.bytes[0];
                r.data[2..4].copy_from_slice(&(lengths.len() as u16).to_le_bytes());
                for (index, (&id, &len)) in
                    lengths.iter().skip(m.args[0] as usize).take(30).enumerate()
                {
                    r.data[4 + index * 2] = id;
                    r.data[5 + index * 2] = len as u8;
                }
                r.id = self.session.as_ref().map_or(0, |s| s.id);
                if let Some(s) = &self.session {
                    s.reply(r);
                }
            }
            Op::Physical | Op::Output => {
                let d = &self.devices[t];
                let id = m.args[0] as u8;
                let b = if m.op == Op::Physical {
                    d.lane.physical.latest_input(id)
                } else {
                    d.lane.physical.latest_output(id)
                };
                let mut r = m.response(if b.is_some() { Status::Applied } else { Status::Unknown });
                r.at = now;
                r.id = self.seq;
                if let Some(b) = b {
                    r.args[1] = b.len() as i32;
                    r.data[..b.len()].copy_from_slice(&b);
                }
                if let Some(s) = &self.session {
                    s.reply(r);
                }
            }
            Op::Takeover => {
                self.devices[t].restore_pending=self.waiting.iter().any(|w|w.input.target==t && w.physical);
                if self.devices[t].lane.codec.keyboard.is_none()
                    && self.devices[t].lane.codec.mouse.is_none()
                {
                    self.reply(&m, Status::Unsupported);
                    return Ok(());
                }
                self.invalidate(t);
                self.effects.retain(|e| e.command.target != m.target);
                self.scheduled.retain(|e| e.message.target != m.target);
                self.waiting.retain(|w| w.input.target != t || w.physical);
                self.holds.retain(|(target, _), _| *target != t);
                self.devices[t].full = m.args[0] < 2;
                self.devices[t].external = m.args[0] >= 2;
                self.devices[t].external_xy = m.args[0] == 3;
                self.devices[t].owned = Values::default();
                if m.args[0] == 1 {
                    self.devices[t].owned = self.physical_values(t);
                }
                self.devices[t].remainder = Remainder::default();
                self.devices[t].budget_ns =
                    if m.args[1] == 0 { 500_000 } else { m.args[1] as u64 * 1000 };
                self.devices[t].fail_open = m.args[2] == 1;
                if let Some(s) = &self.session {
                    if m.args[0] >= 2 {
                        s.observe.store(true, Ordering::Release);
                    }
                }
                self.restore(t, m.args[0] != 0, Some(m))?;
            }
            Op::Release => {
                self.devices[t].restore_pending=self.waiting.iter().any(|w|w.input.target==t && w.physical);
                self.invalidate(t);
                self.effects.retain(|e| e.command.target != m.target);
                self.rules.retain(|r| r.effect.target != m.target);
                self.scheduled.retain(|e| e.message.target != m.target);
                self.waiting.retain(|w| w.input.target != t || w.physical);
                self.holds.retain(|(target, _), _| *target != t);
                self.devices[t].full = false;
                self.devices[t].external = false;
                self.devices[t].external_xy = false;
                self.restore(t, true, Some(m))?;
            }
            Op::Raw => {
                if !self.devices[t].full {
                    self.reply(&m, Status::Busy);
                } else {
                    let report = Report::from_slice(&m.data[..m.args[0] as usize])?;
                    let id = self.devices[t].lane.physical.layout.input_id(report.bytes())?;
                    if !self.devices[t].lane.codec.managed(id) {
                        self.reply(&m, Status::Unsupported);
                        return Ok(());
                    }
                    let v = self.devices[t].lane.codec.decode(id, report.bytes())?;
                    if Some(id) == self.devices[t].lane.codec.keyboard {
                        self.devices[t].owned.keys = v.keys;
                    }
                    if Some(id) == self.devices[t].lane.codec.mouse {
                        self.devices[t].owned.buttons = v.buttons;
                    }
                    self.emit(t, report, Instant::now(), Some(m), true)?;
                }
            }
            Op::Decision => {
                let Some(index) =
                    self.waiting.iter().position(|w| w.seq == m.id && w.input.target == t)
                else {
                    self.reply(&m, Status::Stale);
                    return Ok(());
                };
                if self.waiting[index].physical {
                    self.reply(&m, Status::Stale);
                    return Ok(());
                }
                if now_ns() >= self.waiting[index].until {
                    self.reply(&m, Status::Expired);
                    return Ok(());
                }
                let replacement = match m.args[0] {
                    0 => Some(self.waiting[index].input.report),
                    1 => {
                        let len = m.args[1] as usize;
                        if len == 0
                            || len > 64
                            || self.devices[t]
                                .lane
                                .physical
                                .layout
                                .input_id(&m.data[..len])
                                .is_err()
                        {
                            self.reply(&m, Status::Invalid);
                            return Ok(());
                        }
                        Some(Report::from_slice(&m.data[..len])?)
                    }
                    2 => None,
                    3 if self.devices[t].external_xy => {
                        let original = &self.waiting[index].input;
                        let codec = &self.devices[t].lane.codec;
                        if Some(original.id) != codec.mouse {
                            self.reply(&m, Status::Unsupported);
                            return Ok(());
                        }
                        let mut values = codec.decode(original.id, original.report.bytes())?;
                        values.x = m.args[1];
                        values.y = m.args[2];
                        match codec.encode(original.id, original.report.bytes(), &values) {
                            Ok(r) => Some(r),
                            Err(_) => {
                                self.reply(&m, Status::Invalid);
                                return Ok(());
                            }
                        }
                    }
                    _ => {
                        self.reply(&m, Status::Invalid);
                        return Ok(());
                    }
                };
                if self.waiting[index].decision.is_some() {
                    self.reply(&m, Status::Stale);
                    return Ok(());
                }
                self.waiting[index].decision = Some((replacement, m));
                self.drain_decisions(t)?;
            }
            Op::Key | Op::Button | Op::Move | Op::Scroll | Op::Keys | Op::Buttons => {
                let id = if matches!(m.op, Op::Key | Op::Keys) {
                    self.devices[t].lane.codec.keyboard
                } else {
                    self.devices[t].lane.codec.mouse
                };
                let Some(id) = id else {
                    self.reply(&m, Status::Unsupported);
                    return Ok(());
                };
                let old_hold = self.holds.get(&(t, m.id)).cloned();
                let grouped = m.id != 0 && matches!(m.op, Op::Key | Op::Button);
                if grouped && old_hold.is_none() && self.holds.len() >= 128 {
                    self.reply(&m, Status::Full);
                    return Ok(());
                }
                let mut v = if grouped {
                    old_hold.clone().unwrap_or_default()
                } else if self.devices[t].full {
                    self.devices[t].owned.clone()
                } else {
                    self.base_values(t, id)?
                };
                v.x = 0;
                v.y = 0;
                v.wheel = 0;
                v.pan = 0;
                match m.op {
                    Op::Key => v.keys.set(m.args[0] as usize, m.args[1] != 0),
                    Op::Button => {
                        let mask = 1 << (m.args[0] - 1);
                        if m.args[1] != 0 { v.buttons |= mask } else { v.buttons &= !mask }
                    }
                    Op::Move => {
                        v.x = m.args[0];
                        v.y = m.args[1]
                    }
                    Op::Scroll => {
                        v.wheel = m.args[0];
                        v.pan = m.args[1]
                    }
                    Op::Keys => {
                        for i in 0..4 {
                            let mut a = [0; 8];
                            a.copy_from_slice(&m.data[i * 8..i * 8 + 8]);
                            v.keys.0[i] = u64::from_le_bytes(a);
                        }
                    }
                    Op::Buttons => v.buttons = m.args[0] as u32,
                    _ => {}
                }
                if grouped {
                    if v.keys.0 == [0; 4] && v.buttons == 0 {
                        self.holds.remove(&(t, m.id));
                    } else {
                        self.holds.insert((t, m.id), v);
                    }
                    v = self.base_values(t, id)?;
                }
                if m.op == Op::Keys
                    && (0..256).any(|k| {
                        v.keys.get(k) && !self.devices[t].lane.codec.supports_key(k as u32)
                    })
                {
                    self.reply(&m, Status::Unsupported);
                    return Ok(());
                }
                if m.op == Op::Buttons
                    && (0..32).any(|b| {
                        v.buttons & (1 << b) != 0
                            && !self.devices[t].lane.codec.supports_button(b + 1)
                    })
                {
                    self.reply(&m, Status::Unsupported);
                    return Ok(());
                }
                let mut actual = v.clone();
                if self.devices[t].full && !grouped {
                    for (&(target, _), h) in &self.holds {
                        if target == t {
                            for i in 0..4 {
                                actual.keys.0[i] |= h.keys.0[i];
                            }
                            actual.buttons |= h.buttons;
                        }
                    }
                }
                match self.encode(t, id, &actual) {
                    Ok(r) => {
                        if matches!(m.op, Op::Key | Op::Button | Op::Keys | Op::Buttons) {
                            self.devices[t].direct = true;
                        }
                        if self.devices[t].full && !grouped {
                            if matches!(m.op, Op::Key | Op::Keys) {
                                self.devices[t].owned.keys = v.keys.clone();
                            }
                            if matches!(m.op, Op::Button | Op::Buttons) {
                                self.devices[t].owned.buttons = v.buttons;
                            }
                        }
                        self.emit(t, r, Instant::now(), Some(m), true)?;
                    }
                    Err(_) => {
                        if grouped {
                            if let Some(old) = old_hold {
                                self.holds.insert((t, m.id), old);
                            } else {
                                self.holds.remove(&(t, m.id));
                            }
                        }
                        self.reply(&m, Status::Invalid)
                    }
                }
            }
            Op::ForceKey | Op::ForceButton | Op::Scale | Op::Rotate | Op::Velocity | Op::Offset => {
                if self.effects.len() >= 128 {
                    self.reply(&m, Status::Full);
                    return Ok(());
                }
                if m.id == 0 {
                    m.id = self.allocate_id();
                }
                if !self.applying_scheduled {
                    self.effects.retain(|e| e.command.id != m.id);
                }
                self.effects.push(Effect::new(m, now, rule));
                self.effects.sort_by_key(|e| (e.command.args[3], e.started, e.command.id));
                if matches!(m.op, Op::ForceKey | Op::ForceButton) {
                    let id = if m.op == Op::ForceKey {
                        self.devices[t].lane.codec.keyboard
                    } else {
                        self.devices[t].lane.codec.mouse
                    };
                    if let Some(id) = id {
                        match self.render(t, id, Vector::default()) {
                            Ok(r) => self.emit(t, r, Instant::now(), Some(m), true)?,
                            Err(_) => {
                                self.effects.retain(|e| e.command.id != m.id);
                                self.reply(&m, Status::Invalid);
                            }
                        }
                    }
                } else {
                    self.reply(&m, Status::Applied);
                }
            }
            Op::Cancel | Op::CancelAll => {
                let all = m.op == Op::CancelAll;
                let mut touched = [false; crate::layout::MAX_INTERFACES];
                if all {
                    for (t, d) in self.devices.iter_mut().enumerate() {
                        touched[t] = true;
                        d.owned = Values::default();
                    }
                }
                for (&(t, id), _) in &self.holds {
                    if all || id == m.id {
                        touched[t] = true;
                    }
                }
                self.holds.retain(|(_, id), _| !all && *id != m.id);
                for e in &self.scheduled {
                    if all || e.message.id == m.id {
                        touched[e.message.target as usize] = true;
                    }
                }
                for e in &self.effects {
                    if all || e.command.id == m.id {
                        touched[e.command.target as usize] = true;
                    }
                }
                self.effects.retain(|e| !all && e.command.id != m.id);
                self.scheduled.retain(|e| !all && e.message.id != m.id);
                if all {
                    self.rules.clear();
                    self.variables.clear();
                }
                for (t, touched) in touched.iter().enumerate().take(self.devices.len()) {
                    if *touched {
                        self.invalidate(t);
                        self.devices[t].remainder = Remainder::default();
                        self.restore(t, true, None)?;
                    }
                }
                self.reply(&m, Status::Applied);
            }
            Op::Variable => {
                self.variables.set(&m, now);
                self.variable_rules(now)?;
                self.reply(&m, Status::Applied);
            }
            Op::PhysicalState => {
                let values = self.physical_values(t);
                let response = self.variables.snapshot(t, values.buttons, values.keys.0, &m);
                if let Some(session) = &self.session { session.reply(response); }
            }
            Op::CompletedState => {
                let response = self.devices[t].lane.feedback.response(&m);
                if let Some(session) = &self.session { session.reply(response); }
            }
            Op::Trigger => self.install_rule(m, now)?,
            Op::RemoveTrigger => {
                let mut touched = [false; crate::layout::MAX_INTERFACES];
                for e in &self.effects {
                    if e.rule == m.id {
                        touched[e.command.target as usize] = true;
                    }
                }
                self.rules.retain(|r| r.id != m.id);
                self.effects.retain(|e| e.rule != m.id);
                self.scheduled.retain(|e| e.rule != m.id);
                for (t, b) in touched.iter().enumerate().take(self.devices.len()) {
                    if *b {
                        self.invalidate(t);
                        self.restore(t, true, None)?;
                    }
                }
                self.reply(&m, Status::Applied);
            }
            Op::Action => {
                let exists = self.effects.iter().any(|e| e.command.id == m.id)
                    || self.scheduled.iter().any(|e| e.message.id == m.id)
                    || self.rules.iter().any(|r| r.id == m.id);
                self.reply(&m, if exists { Status::Applied } else { Status::Unknown });
            }
            _ => self.reply(&m, Status::Unsupported),
        }
        Ok(())
    }
    pub(super) fn install_rule(&mut self, m: Message, now: u64) -> io::Result<()> {
        let source = m.args[0] as usize;
        let variable = m.args[0] == -1;
        let usage = m.args[1] as u32;
        let edge = m.args[2];
        if (!variable && source >= self.devices.len()) || !(1..=3).contains(&edge) || self.rules.len() >= 64 {
            self.reply(&m, Status::Invalid);
            return Ok(());
        }
        let valid = if variable { usage >> 16 == 0xff00 && (usage & 0xffff) < 64 } else { match usage >> 16 {
            7 => self.devices[source].lane.codec.supports_key(usage & 0xffff),
            9 => self.devices[source].lane.codec.supports_button(usage & 0xffff),
            _ => false,
        }};
        let Some(op) = u16::try_from(m.args[3]).ok().and_then(|v| Op::try_from(v).ok()) else {
            self.reply(&m, Status::Invalid);
            return Ok(());
        };
        let mut effect = Message::new(op);
        effect.target = m.target;
        effect.flags = 0;
        for i in 0..4 {
            let mut a = [0; 4];
            a.copy_from_slice(&m.data[i * 4..i * 4 + 4]);
            effect.args[i] = i32::from_le_bytes(a);
        }
        if !valid
            || !matches!(
                effect.op,
                Op::ForceKey | Op::ForceButton | Op::Scale | Op::Rotate | Op::Velocity | Op::Offset
            )
            || self.valid(&effect, now).is_err()
        {
            self.reply(&m, Status::Unsupported);
            return Ok(());
        }
        let id = if m.id == 0 { self.allocate_id() } else { m.id };
        effect.id = id;
        self.rules.retain(|r| r.id != id);
        self.effects.retain(|e| e.rule != id);
        let mut rule = Rule { id, source, usage, edge, effect, last: false };
        rule.last = if variable { self.variables.value((usage & 0xffff) as usize, now) != 0 }
            else { rule.condition(&self.physical_values(source)) };
        let active = edge == 3 && rule.last;
        self.rules.push(rule);
        if active {
            self.apply(effect, now, id)?;
        }
        let mut r = m.response(Status::Applied);
        r.id = id;
        if let Some(s) = &self.session {
            s.reply(r);
        }
        Ok(())
    }
}

// SPDX-License-Identifier: GPL-2.0-only
//! 单控制会话的有界规则与动作状态。
use crate::codec::Values;
use vs680_hid::{
    trajectory::{Transform, Vector},
    wire::{Message, Op},
};
#[derive(Debug, Clone)]
pub(crate) struct Effect {
    pub(crate) command: Message,
    pub(crate) started: u64,
    pub(crate) end: u64,
    pub(crate) rule: u64,
    pub(crate) transform: Option<Transform>,
    last: u64,
    offset: Vector,
}
impl Effect {
    pub(crate) fn new(c: Message, now: u64, rule: u64) -> Self {
        let transform = match c.op {
            Op::Scale => Transform::scale(c.args[0] as f64 / 65536., c.args[1] as f64 / 65536.),
            Op::Rotate => Transform::rotate((c.args[0] as f64 / 1000.).to_radians()),
            _ => None,
        };
        let end =
            if c.args[2] > 0 { now.saturating_add(c.args[2] as u64 * 1_000_000) } else { u64::MAX };
        Self {
            command: c,
            started: now,
            end,
            rule,
            transform,
            last: now,
            offset: Vector::default(),
        }
    }
    pub(crate) fn apply(&self, v: &mut Values) {
        match self.command.op {
            Op::ForceKey => v.keys.set(self.command.args[0] as usize, self.command.args[1] != 0),
            Op::ForceButton => {
                let bit = 1 << (self.command.args[0] - 1);
                if self.command.args[1] != 0 { v.buttons |= bit } else { v.buttons &= !bit }
            }
            _ => {}
        }
    }
    pub(crate) fn advance(&mut self, now: u64) -> Vector {
        let t = now.min(self.end);
        let dt = t.saturating_sub(self.last);
        self.last = t;
        if self.command.op == Op::Velocity {
            let dt = dt.min(20_000_000) as f64 / 1_000_000_000.;
            return Vector {
                x: self.command.args[0] as f64 / 1000. * dt,
                y: self.command.args[1] as f64 / 1000. * dt,
            };
        }
        if self.command.op == Op::Offset {
            let fraction = if self.command.args[2] == 0 {
                1.
            } else {
                (t.saturating_sub(self.started) as f64 / (self.end - self.started) as f64).min(1.)
            };
            let p = Vector {
                x: self.command.args[0] as f64 * fraction,
                y: self.command.args[1] as f64 * fraction,
            };
            let d = Vector { x: p.x - self.offset.x, y: p.y - self.offset.y };
            self.offset = p;
            if self.command.args[2] == 0 {
                self.end = now;
            }
            return d;
        }
        Vector::default()
    }
}
#[derive(Debug, Clone)]
pub(crate) struct Rule {
    pub(crate) id: u64,
    pub(crate) source: usize,
    pub(crate) usage: u32,
    pub(crate) edge: i32,
    pub(crate) effect: Message,
    pub(crate) last: bool,
}
impl Rule {
    pub(crate) fn condition(&self, v: &Values) -> bool {
        match self.usage >> 16 {
            7 => v.keys.get((self.usage & 0xffff) as usize),
            9 => {
                let k = self.usage & 0xffff;
                k > 0 && k <= 32 && v.buttons & (1 << (k - 1)) != 0
            }
            _ => false,
        }
    }
    pub(crate) fn evaluate(&mut self, now: bool) -> (bool, bool) {
        let start = match self.edge {
            1 => !self.last && now,
            2 => self.last && !now,
            3 => !self.last && now,
            _ => false,
        };
        let end = self.edge == 3 && self.last && !now;
        self.last = now;
        (start, end)
    }
}

// SPDX-License-Identifier: GPL-2.0-only
//! 宏条件变量和物理输入状态。
use super::*;

#[derive(Clone, Copy, Default)]
struct Variable { value: i32, expires: u64 }

pub(super) struct Variables {
    bank: [Variable; 64],
    physical: Vec<(i64, i64, i32, i32, u64)>,
}
impl Variables {
    pub(super) fn new(count: usize) -> Self {
        Self { bank: [Variable::default(); 64], physical: vec![(0, 0, 0, 0, 0); count] }
    }
    pub(super) fn value(&self, id: usize, now: u64) -> i32 {
        self.bank.get(id).filter(|v| v.expires > now).map_or(0, |v| v.value)
    }
    pub(super) fn set(&mut self, m: &Message, now: u64) {
        self.bank[m.args[0] as usize] = Variable { value: m.args[1],
            expires: if m.expires != 0 { m.expires } else if m.args[2] > 0 {
                now.saturating_add(m.args[2] as u64 * 1_000_000)
            } else { u64::MAX } };
    }
    pub(super) fn clear(&mut self) { self.bank.fill(Variable::default()); }
    pub(super) fn observe(&mut self, target: usize, x: i32, y: i32, _buttons: u32, _now: u64) {
        let p = &mut self.physical[target];
        p.0 += i64::from(x); p.1 += i64::from(y);
        p.2 = x; p.3 = y; p.4 = p.4.wrapping_add(1);
    }
    pub(super) fn snapshot(&self, target: usize, buttons: u32, keys: [u64; 4], m: &Message) -> Message {
        let p = self.physical[target];
        let mut out = m.response(Status::Applied);
        out.at = now_ns();
        let mut e = minicbor::Encoder::new(Vec::new());
        // 保留 CBOR v2 状态数组的字段位置，扩展字段填零。
        let result = (|| {
            e.array(11)?.i64(p.0)?.i64(p.1)?.i32(p.2)?.i32(p.3)?.u32(buttons)?.u64(p.4)?
                .u64(0)?.bool(false)?.array(4)?;
            for k in keys { e.u64(k)?; }
            e.bool(false)?.u32(0)?;
            Ok::<_, minicbor::encode::Error<std::convert::Infallible>>(())
        })();
        if result.is_err() { return m.response(Status::Failed); }
        let bytes = e.into_writer();
        if bytes.len() > 64 { return m.response(Status::Failed); }
        out.data[..bytes.len()].copy_from_slice(&bytes);
        out.args[1] = bytes.len() as i32;
        out
    }
}
impl Processor {
    pub(super) fn variable_rules(&mut self,now:u64)->io::Result<()> {
        let mut start=Vec::new();let mut end=Vec::new();
        for r in &mut self.rules {if r.source==usize::MAX {
            let active=self.variables.value((r.usage&0xffff) as usize,now)!=0;
            let (begin,finish)=r.evaluate(active);if begin {start.push((r.id,r.effect));}if finish{end.push(r.id);}
        }}
        for id in end {let targets:Vec<_>=self.effects.iter().filter(|e|e.rule==id).map(|e|e.command.target as usize).collect();
            self.effects.retain(|e|e.rule!=id);for t in targets {self.restore(t,true,None)?;}}
        for (id,e) in start {self.apply(e,now,id)?;}Ok(())
    }
}

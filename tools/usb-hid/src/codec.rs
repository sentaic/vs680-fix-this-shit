// SPDX-License-Identifier: GPL-2.0-only
//! 根据描述符定位键盘和相对鼠标字段。
//! 修改时保留原报告中无关的字段。
use crate::layout::{Layout, invalid};
use std::{collections::BTreeMap, io};
#[derive(Debug, Clone, Copy, Default)]
struct Global {
    page: u32,
    size: usize,
    count: usize,
    id: u8,
    min: i32,
    max: i32,
}
#[derive(Debug, Clone)]
struct Field {
    offset: usize,
    bits: usize,
    count: usize,
    page: u32,
    usages: Vec<u32>,
    array: bool,
    min: i32,
    max: i32,
    relative: bool,
}
#[derive(Debug, Clone)]
pub(crate) struct Shape {
    id: u8,
    len: usize,
    fields: Vec<Field>,
}
#[derive(Debug, Clone)]
pub(crate) struct Codec {
    reports: BTreeMap<u8, Shape>,
    pub(crate) keyboard: Option<u8>,
    pub(crate) mouse: Option<u8>,
}
#[derive(Debug, Clone, Copy)]
pub(crate) struct Report {
    pub(crate) data: [u8; 64],
    pub(crate) len: usize,
}
impl Report {
    pub(crate) fn from_slice(b: &[u8]) -> io::Result<Self> {
        if b.is_empty() || b.len() > 64 {
            return Err(invalid("report capacity"));
        }
        let mut r = Self { data: [0; 64], len: b.len() };
        r.data[..b.len()].copy_from_slice(b);
        Ok(r)
    }
    pub(crate) fn bytes(&self) -> &[u8] {
        &self.data[..self.len]
    }
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct Keys(pub(crate) [u64; 4]);
impl Default for Keys {
    fn default() -> Self {
        Self([0; 4])
    }
}
impl Keys {
    pub(crate) fn get(&self, k: usize) -> bool {
        k < 256 && self.0[k / 64] & (1 << (k % 64)) != 0
    }
    pub(crate) fn set(&mut self, k: usize, v: bool) {
        if k < 256 {
            if v {
                self.0[k / 64] |= 1 << (k % 64)
            } else {
                self.0[k / 64] &= !(1 << (k % 64));
            }
        }
    }
}
#[derive(Debug, Clone, Default)]
pub(crate) struct Values {
    pub(crate) keys: Keys,
    pub(crate) buttons: u32,
    pub(crate) x: i32,
    pub(crate) y: i32,
    pub(crate) wheel: i32,
    pub(crate) pan: i32,
}
impl Codec {
    pub(crate) fn parse(bytes: &[u8], layout: &Layout) -> io::Result<Self> {
        let mut reports: BTreeMap<u8, Shape> = layout.bytes[0]
            .iter()
            .map(|(&id, &len)| (id, Shape { id, len, fields: Vec::new() }))
            .collect();
        let mut offsets = BTreeMap::<u8, usize>::new();
        let mut g = Global::default();
        let mut stack = Vec::new();
        let mut usages = Vec::new();
        let (mut low, mut high) = (None, None);
        let mut p = 0;
        while p < bytes.len() {
            let prefix = bytes[p];
            p += 1;
            if prefix == 0xfe {
                return Err(invalid("long HID item"));
            }
            let n = match prefix & 3 {
                3 => 4,
                n => n as usize,
            };
            let d = bytes.get(p..p + n).ok_or_else(|| invalid("HID item"))?;
            p += n;
            let v = d.iter().enumerate().fold(0u32, |v, (i, b)| v | (u32::from(*b) << (8 * i)));
            let signed = if n > 0 && n < 4 && v & (1 << (n * 8 - 1)) != 0 {
                (v | (!0u32 << (n * 8))) as i32
            } else {
                v as i32
            };
            match ((prefix >> 2) & 3, prefix >> 4) {
                (1, 0) => g.page = v,
                (1, 1) => g.min = signed,
                (1, 2) => g.max = if g.min < 0 { signed } else { v as i32 },
                (1, 7) => g.size = v as usize,
                (1, 9) => g.count = v as usize,
                (1, 8) => g.id = v as u8,
                (1, 10) => stack.push(g),
                (1, 11) => g = stack.pop().ok_or_else(|| invalid("global stack"))?,
                (2, 0) => usages.push(v),
                (2, 1) => low = Some(v),
                (2, 2) => high = Some(v),
                (0, 8) => {
                    let offset = offsets.entry(g.id).or_insert(usize::from(layout.numbered) * 8);
                    if v & 1 == 0 && g.size > 0 && g.size <= 32 && g.count <= 512 {
                        if usages.is_empty() {
                            if let (Some(a), Some(b)) = (low, high) {
                                if b >= a && b - a < 512 {
                                    usages.extend(a..=b);
                                }
                            }
                        }
                        if let Some(r) = reports.get_mut(&g.id) {
                            r.fields.push(Field {
                                offset: *offset,
                                bits: g.size,
                                count: g.count,
                                page: g.page,
                                usages: usages.clone(),
                                array: v & 2 == 0,
                                min: g.min,
                                max: g.max,
                                relative: v & 4 != 0,
                            });
                        }
                    }
                    *offset = offset
                        .checked_add(
                            g.size.checked_mul(g.count).ok_or_else(|| invalid("field size"))?,
                        )
                        .ok_or_else(|| invalid("field offset"))?;
                    usages.clear();
                    low = None;
                    high = None;
                }
                (0, _) => {
                    usages.clear();
                    low = None;
                    high = None;
                }
                _ => {}
            }
        }
        let keyboard =
            reports.iter().find(|(_, r)| r.fields.iter().any(|f| f.page == 7)).map(|(&id, _)| id);
        let mouse = reports
            .iter()
            .find(|(_, r)| {
                r.fields.iter().any(|f| f.page == 1 && f.relative && f.usages.contains(&0x30))
            })
            .map(|(&id, _)| id);
        Ok(Self { reports, keyboard, mouse })
    }
    pub(crate) fn ids(&self) -> impl Iterator<Item = u8> + '_ {
        self.reports.keys().copied()
    }
    pub(crate) fn blank(&self, id: u8) -> io::Result<Report> {
        let r = self.reports.get(&id).ok_or_else(|| invalid("report id"))?;
        let mut b = Report { data: [0; 64], len: r.len };
        if id != 0 {
            b.data[0] = id;
        }
        Ok(b)
    }
    pub(crate) fn managed(&self, id: u8) -> bool {
        Some(id) == self.keyboard || Some(id) == self.mouse
    }
    pub(crate) fn supports_key(&self, k: u32) -> bool {
        self.keyboard
            .and_then(|id| self.reports.get(&id))
            .is_some_and(|r| r.fields.iter().any(|f| f.page == 7 && f.usages.contains(&k)))
    }
    pub(crate) fn supports_button(&self, k: u32) -> bool {
        self.mouse
            .and_then(|id| self.reports.get(&id))
            .is_some_and(|r| r.fields.iter().any(|f| f.page == 9 && f.usages.contains(&k)))
    }
    pub(crate) fn decode(&self, id: u8, b: &[u8]) -> io::Result<Values> {
        let r = self.reports.get(&id).ok_or_else(|| invalid("report"))?;
        if b.len() != r.len {
            return Err(invalid("report length"));
        }
        let mut out = Values::default();
        for f in &r.fields {
            for i in 0..f.count {
                let raw = read_bits(b, f.offset + i * f.bits, f.bits)?;
                let value = if f.min < 0 {
                    ((raw as i32) << (32 - f.bits)) >> (32 - f.bits)
                } else {
                    raw as i32
                };
                let usage = f.usages.get(i).copied().unwrap_or(0) & 0xffff;
                if f.page == 7 {
                    if f.array {
                        if (1..=3).contains(&raw) {
                            return Err(invalid("keyboard rollover state unknown"));
                        }
                        if raw < 256 && raw != 0 {
                            out.keys.set(raw as usize, true);
                        }
                    } else if usage < 256 && raw != 0 {
                        out.keys.set(usage as usize, true);
                    }
                } else if f.page == 9 && usage > 0 && usage <= 32 && raw != 0 {
                    out.buttons |= 1 << (usage - 1);
                } else if f.page == 1 && f.relative {
                    match usage {
                        0x30 => out.x = value,
                        0x31 => out.y = value,
                        0x38 => out.wheel = value,
                        _ => {}
                    }
                } else if f.page == 0x0c && usage == 0x238 && f.relative {
                    out.pan = value;
                }
            }
        }
        Ok(out)
    }
    pub(crate) fn encode(&self, id: u8, base: &[u8], v: &Values) -> io::Result<Report> {
        let r = self.reports.get(&id).ok_or_else(|| invalid("report"))?;
        if base.len() != r.len {
            return Err(invalid("base length"));
        }
        let mut report = Report::from_slice(base)?;
        let b = &mut report.data[..report.len];
        for f in &r.fields {
            if f.page == 7 && f.array {
                let mut keys = [0u32; 64];
                let mut count = 0;
                for k in 1..256 {
                    let variable = r
                        .fields
                        .iter()
                        .any(|v| v.page == 7 && !v.array && v.usages.contains(&(k as u32)));
                    if v.keys.get(k) && f.usages.contains(&(k as u32)) && !variable {
                        if count >= f.count || count >= keys.len() {
                            return Err(invalid("keyboard array capacity exceeded"));
                        }
                        keys[count] = k as u32;
                        count += 1;
                    }
                }
                for i in 0..f.count {
                    write_bits(
                        b,
                        f.offset + i * f.bits,
                        f.bits,
                        keys.get(i).copied().unwrap_or(0),
                    )?;
                }
                continue;
            }
            for i in 0..f.count {
                let u = f.usages.get(i).copied().unwrap_or(0) & 0xffff;
                let value = if f.page == 7 && u < 256 {
                    Some(i32::from(v.keys.get(u as usize)))
                } else if f.page == 9 && u > 0 && u <= 32 {
                    Some(i32::from(v.buttons & (1 << (u - 1)) != 0))
                } else if f.page == 1 && f.relative {
                    match u {
                        0x30 => Some(v.x),
                        0x31 => Some(v.y),
                        0x38 => Some(v.wheel),
                        _ => None,
                    }
                } else if f.page == 0x0c && u == 0x238 && f.relative {
                    Some(v.pan)
                } else {
                    None
                };
                if let Some(value) = value {
                    if value < f.min || value > f.max {
                        return Err(invalid("HID field range exceeded"));
                    }
                    write_bits(b, f.offset + i * f.bits, f.bits, value as u32)?;
                }
            }
        }
        if id != 0 {
            b[0] = r.id;
        }
        Ok(report)
    }
}
fn read_bits(b: &[u8], at: usize, n: usize) -> io::Result<u32> {
    if at + n > b.len() * 8 || n > 32 {
        return Err(invalid("field bounds"));
    }
    let mut v = 0;
    for i in 0..n {
        v |= u32::from((b[(at + i) / 8] >> ((at + i) % 8)) & 1) << i;
    }
    Ok(v)
}
fn write_bits(b: &mut [u8], at: usize, n: usize, v: u32) -> io::Result<()> {
    if at + n > b.len() * 8 || n > 32 {
        return Err(invalid("field bounds"));
    }
    for i in 0..n {
        let mask = 1 << ((at + i) % 8);
        let dst = &mut b[(at + i) / 8];
        *dst = (*dst & !mask) | ((((v >> i) & 1) as u8) << ((at + i) % 8));
    }
    Ok(())
}

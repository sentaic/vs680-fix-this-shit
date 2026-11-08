// SPDX-License-Identifier: GPL-2.0-only
//! HID报告长度校验与FunctionFS描述符编码。
use std::{collections::BTreeMap, io};

// One interrupt-IN endpoint per HID interface; endpoint zero is reserved.
pub(crate) const MAX_INTERFACES: usize = 15;

#[derive(Clone, Debug)]
pub(crate) struct Layout {
    /// 只有带编号的报告，其长度才包含报告ID字节。
    pub(crate) bytes: [BTreeMap<u8, usize>; 3],
    pub(crate) numbered: bool,
}

pub(crate) fn invalid(message: &str) -> io::Error {
    io::Error::new(io::ErrorKind::InvalidData, message)
}

impl Layout {
    pub(crate) fn parse(descriptor: &[u8]) -> io::Result<Self> {
        if descriptor.is_empty() || descriptor.len() > 4096 {
            return Err(invalid("invalid HID descriptor length"));
        }
        let mut bits: [BTreeMap<u8, u32>; 3] = std::array::from_fn(|_| BTreeMap::new());
        let mut globals = (0_u32, 0_u32, 0_u8); // report size, count, ID
        let mut stack = Vec::new();
        let mut offset = 0;
        let mut numbered = false;
        while offset < descriptor.len() {
            let prefix = descriptor[offset];
            offset += 1;
            if prefix == 0xfe {
                return Err(invalid("long HID items are not supported"));
            }
            let length = match prefix & 3 {
                3 => 4,
                value => usize::from(value),
            };
            let data = descriptor
                .get(offset..offset + length)
                .ok_or_else(|| invalid("truncated HID item"))?;
            offset += length;
            let value = data
                .iter()
                .enumerate()
                .fold(0_u32, |acc, (i, byte)| acc | (u32::from(*byte) << (8 * i)));
            let kind = (prefix >> 2) & 3;
            let tag = prefix >> 4;
            match (kind, tag) {
                (1, 7) => globals.0 = value,
                (1, 9) => globals.1 = value,
                (1, 8) => {
                    globals.2 = u8::try_from(value)
                        .ok()
                        .filter(|id| *id != 0)
                        .ok_or_else(|| invalid("invalid report ID"))?;
                    numbered = true;
                }
                (1, 10) => {
                    if stack.len() >= 16 {
                        return Err(invalid("HID global stack too deep"));
                    }
                    stack.push(globals);
                }
                (1, 11) => {
                    globals = stack.pop().ok_or_else(|| invalid("unbalanced HID global Pop"))?
                }
                (0, 8 | 9 | 11) => {
                    let index = match tag {
                        8 => 0,
                        9 => 1,
                        _ => 2,
                    };
                    let count = globals
                        .0
                        .checked_mul(globals.1)
                        .ok_or_else(|| invalid("HID field overflow"))?;
                    let total = bits[index].entry(globals.2).or_default();
                    *total = total
                        .checked_add(count)
                        .filter(|bits| *bits <= 32768)
                        .ok_or_else(|| invalid("HID report exceeds 4096 bytes"))?;
                }
                _ => {}
            }
        }
        if !stack.is_empty() || bits[0].is_empty() {
            return Err(invalid("unbalanced globals or no HID input report"));
        }
        if numbered && bits.iter().any(|reports| reports.contains_key(&0)) {
            return Err(invalid("mixed numbered and unnumbered reports"));
        }
        let bytes: [BTreeMap<u8, usize>; 3] = bits.map(|reports| {
            reports
                .into_iter()
                .map(|(id, bits)| (id, bits.div_ceil(8) as usize + usize::from(numbered)))
                .collect()
        });
        if bytes[0].values().any(|length| *length == 0 || *length > 64) {
            return Err(invalid("this relay supports input reports of 1..64 bytes"));
        }
        Ok(Self { bytes, numbered })
    }

    pub(crate) fn input_id(&self, data: &[u8]) -> io::Result<u8> {
        let id =
            if self.numbered { *data.first().ok_or_else(|| invalid("empty report"))? } else { 0 };
        if self.bytes[0].get(&id).copied() != Some(data.len()) {
            return Err(invalid("input report length/ID differs from descriptor"));
        }
        Ok(id)
    }
}

#[cfg(test)]
pub(crate) fn descriptors(reports: &[Vec<u8>], rate: u32) -> io::Result<Vec<u8>> {
    let interval = match rate {
        1000 => 4,
        4000 => 2,
        8000 => 1,
        _ => return Err(invalid("rate must be 1000, 4000 or 8000")),
    };
    descriptors_intervals(reports, &vec![(1, interval); reports.len()])
}
pub(crate) fn descriptors_intervals(
    reports: &[Vec<u8>],
    intervals: &[(u8, u8)],
) -> io::Result<Vec<u8>> {
    if reports.is_empty() || reports.len() > MAX_INTERFACES {
        return Err(invalid("select 1..15 HID interfaces"));
    }
    if intervals.len() != reports.len()
        || intervals.iter().any(|(fs, hs)| *fs == 0 || !(1..=16).contains(hs))
    {
        return Err(invalid("per-interface intervals"));
    }
    let mut result = Vec::new();
    for value in [3_u32, 0, 3, (reports.len() * 3) as u32, (reports.len() * 3) as u32] {
        result.extend(value.to_le_bytes());
    }
    for high_speed in [false, true] {
        for (index, report) in reports.iter().enumerate() {
            let speed_interval = if high_speed { intervals[index].1 } else { intervals[index].0 };
            // 使用非启动协议 HID，按原报告布局转发键鼠数据。
            result.extend([9, 4, index as u8, 0, 1, 3, 0, 0, 0]);
            result.extend(hid_descriptor(report.len()));
            result.extend([7, 5, 0x81 + index as u8, 3, 64, 0, speed_interval]);
        }
    }
    let length = result.len() as u32;
    result[4..8].copy_from_slice(&length.to_le_bytes());
    Ok(result)
}

pub(crate) fn hid_descriptor(length: usize) -> [u8; 9] {
    let size = (length as u16).to_le_bytes();
    [9, 0x21, 0x11, 0x01, 0, 1, 0x22, size[0], size[1]]
}

pub(crate) fn strings() -> Vec<u8> {
    // 接口字符串 ID 设为 0。
    [2_u32, 16, 0, 0].into_iter().flat_map(u32::to_le_bytes).collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn composite_interfaces_have_distinct_endpoints_at_both_speeds() -> io::Result<()> {
        let reports = vec![vec![0; 32]; 4];
        let bytes = descriptors_intervals(&reports, &[(1, 4); 4])?;
        assert_eq!(bytes.len(), 20 + 2 * 4 * 25);
        assert_eq!(u32::from_le_bytes(bytes[12..16].try_into().unwrap()), 12);
        assert_eq!(u32::from_le_bytes(bytes[16..20].try_into().unwrap()), 12);
        for speed in 0..2 {
            for interface in 0..4 {
                let start = 20 + (speed * 4 + interface) * 25;
                assert_eq!(bytes[start + 2], interface as u8);
                assert_eq!(bytes[start + 20], 0x81 + interface as u8);
                assert_eq!(bytes[start + 24], if speed == 0 { 1 } else { 4 });
            }
        }
        assert!(descriptors_intervals(&vec![vec![0; 32]; 16], &[(1, 4); 16]).is_err());
        Ok(())
    }
    #[test]
    fn report_ids_padding_and_global_stack() -> io::Result<()> {
        let layout = Layout::parse(&[
            0x85, 1, 0x75, 1, 0x95, 3, 0x81, 2, // three button bits
            0x95, 5, 0x81, 1, // five padding bits
            0xa4, 0x85, 2, 0x75, 16, 0x95, 2, 0x81, 6, 0xb4, 0x75, 8, 0x95, 1, 0x91, 2,
        ])?;
        assert_eq!(layout.bytes[0].get(&1), Some(&2));
        assert_eq!(layout.bytes[0].get(&2), Some(&5));
        assert_eq!(layout.bytes[1].get(&1), Some(&2));
        assert!(layout.input_id(&[2, 0, 0, 0]).is_err());
        Ok(())
    }
    #[test]
    fn malformed_descriptors_are_rejected() {
        for bytes in [&[0x75][..], &[0xb4], &[0x85, 0], &[0x75, 8, 0x95, 65, 0x81, 2]] {
            assert!(Layout::parse(bytes).is_err());
        }
    }
    #[test]
    fn speed_intervals_and_descriptor_lengths() -> io::Result<()> {
        for (rate, hs) in [(1000, 4), (4000, 2), (8000, 1)] {
            let bytes = descriptors(&[vec![0; 63], vec![0; 52]], rate)?;
            assert_eq!(bytes.len(), 120);
            assert_eq!(
                u32::from_le_bytes(bytes[4..8].try_into().map_err(|_| invalid("header"))?),
                120
            );
            assert_eq!([bytes[44], bytes[69], bytes[94], bytes[119]], [1, 1, hs, hs]);
        }
        Ok(())
    }
}

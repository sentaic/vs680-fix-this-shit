// SPDX-License-Identifier: GPL-2.0-only
//! 按实体设备的 interrupt-IN 间隔配置虚拟 USB 端点。
use crate::layout::invalid;
use std::{fs, io, path::Path};
#[derive(Debug, Clone, Copy)]
pub(crate) struct Polling {
    pub fs: u8,
    pub hs: u8,
    pub period_us: u32,
}
impl Polling {
    pub(crate) fn from_period(period_us: u32) -> Self {
        let mut hs = 1;
        while hs < 16 && (125_u32 << hs) <= period_us {
            hs += 1;
        }
        Self {
            fs: period_us.div_ceil(1000).clamp(1, 255) as u8,
            hs,
            period_us: 125_u32 << (hs - 1),
        }
    }
    pub(crate) fn fixed(rate: u32) -> Self {
        Self::from_period(1_000_000 / rate)
    }
    pub(crate) fn physical(path: &Path) -> io::Result<Self> {
        let name = path.file_name().ok_or_else(|| invalid("hidraw path"))?;
        let hid = fs::canonicalize(Path::new("/sys/class/hidraw").join(name).join("device"))?;
        let interface = hid.parent().ok_or_else(|| invalid("USB interface path"))?;
        let device = interface.parent().ok_or_else(|| invalid("USB device path"))?;
        let speed: f64 = fs::read_to_string(device.join("speed"))?
            .trim()
            .parse()
            .map_err(|_| invalid("USB speed"))?;
        let mut period: Option<u32> = None;
        for entry in fs::read_dir(interface)? {
            let entry = entry?;
            if !entry.file_name().to_string_lossy().starts_with("ep_") {
                continue;
            }
            let hex = |key: &str| -> io::Result<u32> {
                u32::from_str_radix(fs::read_to_string(entry.path().join(key))?.trim(), 16)
                    .map_err(|_| invalid("USB endpoint value"))
            };
            if hex("bEndpointAddress")? & 0x80 == 0 || hex("bmAttributes")? & 3 != 3 {
                continue;
            }
            let interval = hex("bInterval")?;
            let us = if speed >= 480. {
                if !(1..=16).contains(&interval) {
                    return Err(invalid("HS interrupt interval"));
                }
                125_u32 << (interval - 1)
            } else {
                if !(1..=255).contains(&interval) {
                    return Err(invalid("FS interrupt interval"));
                }
                interval * 1000
            };
            period = Some(period.map_or(us, |old| old.min(us)));
        }
        period
            .map(Self::from_period)
            .ok_or_else(|| invalid("physical interrupt-IN endpoint absent"))
    }
}

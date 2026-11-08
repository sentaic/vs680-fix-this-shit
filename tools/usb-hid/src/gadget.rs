// SPDX-License-Identifier: GPL-2.0-only
//! Combine receiver interfaces while preserving existing report IDs where possible.
//! Processing remains in physical report coordinates; only this boundary remaps IDs.
use crate::{
    layout::{Layout, invalid},
    physical::Physical,
    polling::Polling,
};
use std::{
    collections::{BTreeMap, BTreeSet},
    fs::{File, OpenOptions},
    io::{self, Write},
    path::Path,
    sync::Arc,
};

pub(crate) struct Topology {
    pub(crate) descriptors: Vec<Vec<u8>>,
    routes: Vec<Route>,
}

#[derive(Clone)]
struct Route {
    group: usize,
    numbered: bool,
    // Empty for a single unchanged interface. Otherwise original ID -> host ID.
    ids: BTreeMap<u8, u8>,
}

impl Route {
    fn host(&self, data: &[u8]) -> io::Result<Vec<u8>> {
        if self.ids.is_empty() {
            return Ok(data.to_vec());
        }
        let id = if self.numbered {
            *data.first().ok_or_else(|| invalid("empty HID report"))?
        } else {
            0
        };
        let mapped = self.ids.get(&id).ok_or_else(|| invalid("unmapped HID report"))?;
        let mut out = Vec::with_capacity(data.len() + usize::from(!self.numbered));
        out.push(*mapped);
        out.extend_from_slice(&data[usize::from(self.numbered)..]);
        Ok(out)
    }
}

impl Topology {
    pub(crate) fn new(devices: &[Arc<Physical>], groups: &[usize]) -> io::Result<Self> {
        if devices.is_empty() || groups.len() != devices.len() {
            return Err(invalid("HID grouping"));
        }
        let count = groups.iter().max().copied().unwrap_or(0) + 1;
        if count > devices.len() {
            return Err(invalid("HID group index"));
        }
        let mut routes: Vec<_> = devices
            .iter()
            .zip(groups)
            .map(|(d, &group)| Route { group, numbered: d.layout.numbered, ids: BTreeMap::new() })
            .collect();
        let mut descriptors = Vec::new();
        for group in 0..count {
            let members: Vec<_> =
                groups.iter().enumerate().filter_map(|(i, &g)| (g == group).then_some(i)).collect();
            if members.is_empty() {
                return Err(invalid("empty HID group"));
            }
            if members.len() == 1 {
                descriptors.push(devices[members[0]].descriptor.clone());
                continue;
            }
            let mut descriptor = Vec::new();
            let reserved: BTreeSet<_> = members
                .iter()
                .flat_map(|&i| devices[i].layout.bytes.iter().flat_map(|kind| kind.keys().copied()))
                .filter(|id| *id != 0)
                .collect();
            let mut assigned = BTreeSet::new();
            for i in members {
                let d = &devices[i];
                let ids: BTreeSet<_> =
                    d.layout.bytes.iter().flat_map(|kind| kind.keys().copied()).collect();
                for id in ids {
                    let host_id = if id != 0 && !assigned.contains(&id) {
                        id
                    } else {
                        (1..=255)
                            .find(|id| !reserved.contains(id) && !assigned.contains(id))
                            .ok_or_else(|| invalid("too many grouped report IDs"))?
                    };
                    routes[i].ids.insert(id, host_id);
                    assigned.insert(host_id);
                }
                // Isolate global state between independently authored descriptors.
                descriptor.push(0xa4);
                if !d.layout.numbered {
                    descriptor.extend([0x85, routes[i].ids[&0]]);
                }
                rewrite_descriptor(&d.descriptor, &routes[i].ids, &mut descriptor)?;
                descriptor.push(0xb4);
            }
            // Also enforce the 64-byte input limit after adding report IDs.
            Layout::parse(&descriptor)?;
            descriptors.push(descriptor);
        }
        Ok(Self { descriptors, routes })
    }

    pub(crate) fn intervals(&self, physical: &[Polling]) -> Vec<Polling> {
        (0..self.descriptors.len())
            .map(|group| {
                self.routes
                    .iter()
                    .zip(physical)
                    .filter(|(r, _)| r.group == group)
                    .map(|(_, p)| *p)
                    .min_by_key(|p| p.period_us)
                    .expect("nonempty HID group")
            })
            .collect()
    }

    pub(crate) fn lane_intervals(&self, grouped: &[Polling]) -> Vec<Polling> {
        self.routes.iter().map(|r| grouped[r.group]).collect()
    }

    pub(crate) fn resolve(&self, group: usize, host_id: u8) -> Option<(usize, u8)> {
        self.routes.iter().enumerate().find_map(|(index, r)| {
            if r.group != group {
                return None;
            }
            if r.ids.is_empty() {
                return Some((index, host_id));
            }
            r.ids.iter().find_map(|(&id, &mapped)| (mapped == host_id).then_some((index, id)))
        })
    }

    pub(crate) fn to_host(&self, physical: usize, data: &[u8]) -> io::Result<Vec<u8>> {
        self.routes[physical].host(data)
    }

    pub(crate) fn from_host(&self, physical: usize, id: u8, data: &[u8]) -> io::Result<Vec<u8>> {
        let r = &self.routes[physical];
        if r.ids.is_empty() {
            return Ok(data.to_vec());
        }
        if data.first() != r.ids.get(&id) {
            return Err(invalid("SET_REPORT ID mismatch"));
        }
        let mut out = Vec::new();
        if r.numbered {
            out.push(id);
        }
        out.extend_from_slice(&data[1..]);
        Ok(out)
    }

    pub(crate) fn host_length(&self, physical: usize, length: usize) -> usize {
        let r = &self.routes[physical];
        length + usize::from(!r.ids.is_empty() && !r.numbered)
    }

    pub(crate) fn outputs(&self, ffs: &Path) -> io::Result<Vec<Output>> {
        let files = (0..self.descriptors.len())
            .map(|group| {
                OpenOptions::new()
                    .write(true)
                    .open(ffs.join(format!("ep{}", group + 1)))
                    .map(Arc::new)
            })
            .collect::<io::Result<Vec<_>>>()?;
        Ok(self
            .routes
            .iter()
            .map(|route| Output { file: files[route.group].clone(), route: route.clone() })
            .collect())
    }
}

fn rewrite_descriptor(bytes: &[u8], ids: &BTreeMap<u8, u8>, out: &mut Vec<u8>) -> io::Result<()> {
    let mut offset = 0;
    while offset < bytes.len() {
        let prefix = bytes[offset];
        let size = match prefix & 3 {
            3 => 4,
            n => n as usize,
        };
        let item = bytes.get(offset..offset + size + 1).ok_or_else(|| invalid("HID item"))?;
        if prefix & 0xfc == 0x84 {
            let id = *item.get(1).ok_or_else(|| invalid("report ID item"))?;
            out.extend([0x85, *ids.get(&id).ok_or_else(|| invalid("unmapped descriptor ID"))?]);
        } else {
            out.extend_from_slice(item);
        }
        offset += item.len();
    }
    Ok(())
}

pub(crate) struct Output {
    file: Arc<File>,
    route: Route,
}
impl Output {
    pub(crate) fn group(&self) -> usize {
        self.route.group
    }
    pub(crate) fn write(&self, report: &[u8]) -> io::Result<()> {
        let mut mapped = [0_u8; 64];
        let bytes = if self.route.ids.is_empty() {
            report
        } else {
            let id = if self.route.numbered {
                *report.first().ok_or_else(|| invalid("empty HID report"))?
            } else {
                0
            };
            let payload = &report[usize::from(self.route.numbered)..];
            if payload.len() >= mapped.len() {
                return Err(invalid("grouped HID input too large"));
            }
            mapped[0] = *self.route.ids.get(&id).ok_or_else(|| invalid("unmapped HID input"))?;
            mapped[1..1 + payload.len()].copy_from_slice(payload);
            &mapped[..1 + payload.len()]
        };
        // All outputs for this endpoint belong to one transmitter thread.
        let mut file = &*self.file;
        if file.write(bytes)? != bytes.len() {
            return Err(invalid("short FunctionFS transfer"));
        }
        Ok(())
    }
    #[cfg(test)]
    pub(crate) fn fixture(file: File) -> Self {
        Self {
            file: Arc::new(file),
            route: Route { group: 0, numbered: false, ids: BTreeMap::new() },
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn composite_grouping_routes_input_leds_features_and_colliding_ids() -> io::Result<()> {
        // Unnumbered boot keyboard: eight input bytes and one LED output byte.
        let keyboard = [5, 1, 9, 6, 0xa1, 1, 0x75, 8, 0x95, 8, 0x81, 2, 0x95, 1, 0x91, 2, 0xc0];
        // Separate auxiliary interface uses ID 1 for media and ID 2 for NKRO/feature.
        let auxiliary = [
            5, 0x0c, 9, 1, 0xa1, 1, 0x85, 1, 0x75, 16, 0x95, 1, 0x81, 0, 0xc0, 5, 1, 9, 6, 0xa1, 1,
            0x85, 2, 0x75, 1, 0x95, 104, 0x81, 2, 0x75, 8, 0x95, 5, 0xb1, 2, 0xc0,
        ];
        let devices = [&keyboard[..], &auxiliary, &keyboard]
            .into_iter()
            .map(|d| Physical::fixture(d).map(Arc::new))
            .collect::<io::Result<Vec<_>>>()?;
        let t = Topology::new(&devices, &[0, 0, 1])?;
        assert_eq!(t.descriptors.len(), 2);
        assert_eq!(t.descriptors[1], keyboard);
        let combined = Layout::parse(&t.descriptors[0])?;
        assert_eq!(combined.bytes[0], BTreeMap::from([(1, 3), (2, 14), (3, 9)]));
        assert_eq!(combined.bytes[1], BTreeMap::from([(3, 2)]));
        assert_eq!(combined.bytes[2], BTreeMap::from([(2, 6)]));
        let keypad = [0, 0, 0x59, 0, 0, 0, 0, 0];
        assert_eq!(t.to_host(0, &keypad)?, [3, 0, 0, 0x59, 0, 0, 0, 0, 0]);
        assert_eq!(t.to_host(1, &[1, 0xe9, 0])?, [1, 0xe9, 0]);
        assert_eq!(t.resolve(0, 3), Some((0, 0)));
        assert_eq!(t.resolve(0, 2), Some((1, 2)));
        assert_eq!(t.resolve(0, 99), None);
        assert_eq!(t.from_host(0, 0, &[3, 2])?, [2]); // Num Lock LED
        assert_eq!(t.host_length(0, 1), 2);
        assert_eq!(t.from_host(1, 2, &[2, 1, 2, 3, 4, 5])?, [2, 1, 2, 3, 4, 5]);
        assert_eq!(t.to_host(1, &[2, 1, 2, 3, 4, 5])?, [2, 1, 2, 3, 4, 5]);
        assert!(t.from_host(1, 2, &[3, 0]).is_err());
        let duplicate = Topology::new(&[devices[1].clone(), devices[1].clone()], &[0, 0])?;
        assert_eq!(duplicate.to_host(0, &[1, 0xe9, 0])?, [1, 0xe9, 0]);
        assert_eq!(duplicate.to_host(1, &[1, 0xe9, 0])?, [3, 0xe9, 0]);
        assert_eq!(duplicate.resolve(0, 3), Some((1, 1)));
        let grouped =
            t.intervals(&[Polling::fixed(1000), Polling::fixed(4000), Polling::fixed(1000)]);
        assert_eq!(grouped.iter().map(|p| p.period_us).collect::<Vec<_>>(), [250, 1000]);
        Ok(())
    }
}

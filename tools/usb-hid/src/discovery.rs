// SPDX-License-Identifier: GPL-2.0-only
//! 扩展同一 USB 设备的辅助接口，保留用户指定接口的索引。
use crate::layout::{MAX_INTERFACES, invalid};
use std::{
    collections::BTreeSet,
    fs, io,
    path::{Path, PathBuf},
};

pub(crate) fn expand(selected: &[PathBuf]) -> io::Result<Vec<PathBuf>> {
    expand_at(selected, Path::new("/sys/class/hidraw"), Path::new("/dev"))
}

pub(crate) fn groups(paths: &[PathBuf], selected: usize) -> io::Result<Vec<usize>> {
    groups_at(paths, selected, Path::new("/sys/class/hidraw"))
}

fn groups_at(paths: &[PathBuf], selected: usize, sys: &Path) -> io::Result<Vec<usize>> {
    if selected == 0 || selected > paths.len() {
        return Err(invalid("selected HID interface count"));
    }
    // Selected interfaces stay independent. Only siblings of the same receiver
    // share an auxiliary endpoint, keeping keyboard media separate from mouse traffic.
    let mut parents = Vec::new();
    let mut groups: Vec<_> = (0..selected).collect();
    for path in &paths[selected..] {
        let name = path.file_name().ok_or_else(|| invalid("hidraw path"))?;
        let parent = usb_parent(&sys.join(name))?;
        let group = match parents.iter().position(|p| p == &parent) {
            Some(index) => index,
            None => {
                parents.push(parent);
                parents.len() - 1
            }
        };
        groups.push(selected + group);
    }
    Ok(groups)
}

fn usb_parent(node: &Path) -> io::Result<PathBuf> {
    let hid = fs::canonicalize(node.join("device"))?;
    let interface = hid.parent().ok_or_else(|| invalid("HID interface path"))?;
    // 通过 USB 设备路径区分接收器，同型号设备在不同端口上分别处理。
    if !interface.join("bInterfaceNumber").is_file() {
        return Err(invalid("selected HID must belong to a USB interface"));
    }
    interface.parent().map(Path::to_path_buf).ok_or_else(|| invalid("USB device path"))
}

fn expand_at(selected: &[PathBuf], sys: &Path, dev: &Path) -> io::Result<Vec<PathBuf>> {
    let mut parents = BTreeSet::new();
    let mut result = Vec::new();
    for path in selected {
        let path = fs::canonicalize(path)?;
        if result.contains(&path) {
            return Err(invalid("the same HID interface was selected twice"));
        }
        let name = path.file_name().ok_or_else(|| invalid("hidraw path"))?;
        parents.insert(usb_parent(&sys.join(name))?);
        result.push(path);
    }
    let mut siblings = Vec::new();
    for entry in fs::read_dir(sys)? {
        let entry = entry?;
        let Ok(parent) = usb_parent(&entry.path()) else { continue };
        if !parents.contains(&parent) {
            continue;
        }
        let path = fs::canonicalize(dev.join(entry.file_name()))?;
        if !result.contains(&path) {
            let hid = fs::canonicalize(entry.path().join("device"))?;
            siblings.push((hid, path));
        }
    }
    siblings.sort();
    result.extend(siblings.into_iter().map(|(_, path)| path));
    if result.is_empty() || result.len() > MAX_INTERFACES {
        return Err(invalid(
            "USB HID bridge supports 1..15 interfaces; controller endpoint limits also apply",
        ));
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::os::unix::fs::symlink;

    struct Fixture(PathBuf);
    impl Drop for Fixture {
        fn drop(&mut self) {
            let _ = fs::remove_dir_all(&self.0);
        }
    }

    #[test]
    fn composite_discovery_preserves_primary_order_and_excludes_other_devices() -> io::Result<()> {
        let path = std::env::temp_dir().join(format!("hid-discovery-{}", std::process::id()));
        fs::create_dir(&path)?;
        let root = Fixture(path);
        let sys = root.0.join("class");
        let dev = root.0.join("dev");
        fs::create_dir(&sys)?;
        fs::create_dir(&dev)?;
        // Same keyboard has a primary, a consumer interface and an NKRO interface.
        // A separate keyboard with the same model must remain outside the bridge.
        for (name, device, number) in [
            ("hidraw0", "keyboard", "00"),
            ("hidraw1", "keyboard", "01"),
            ("hidraw2", "keyboard", "02"),
            ("hidraw3", "mouse", "00"),
            ("hidraw4", "other-keyboard", "00"),
            ("hidraw5", "mouse", "01"),
        ] {
            let interface = root.0.join(device).join(number);
            let hid = interface.join("hid");
            fs::create_dir_all(&hid)?;
            fs::write(interface.join("bInterfaceNumber"), number)?;
            fs::create_dir(sys.join(name))?;
            symlink(&hid, sys.join(name).join("device"))?;
            fs::write(dev.join(name), [])?;
        }
        let selected = [dev.join("hidraw0"), dev.join("hidraw3")];
        assert_eq!(
            expand_at(&selected, &sys, &dev)?,
            [
                dev.join("hidraw0"),
                dev.join("hidraw3"),
                dev.join("hidraw1"),
                dev.join("hidraw2"),
                dev.join("hidraw5")
            ]
        );
        assert_eq!(groups_at(&expand_at(&selected, &sys, &dev)?, 2, &sys)?, [0, 1, 2, 2, 3]);
        assert_eq!(groups_at(&selected, 2, &sys)?, [0, 1]);
        // 显式选中的辅助接口只加入一次。
        let explicit = [selected[0].clone(), selected[1].clone(), dev.join("hidraw2")];
        assert_eq!(expand_at(&explicit, &sys, &dev)?.len(), 5);
        assert!(expand_at(&[selected[0].clone(), selected[0].clone()], &sys, &dev).is_err());
        Ok(())
    }
}

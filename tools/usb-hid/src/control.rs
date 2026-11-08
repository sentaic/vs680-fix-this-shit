// SPDX-License-Identifier: GPL-2.0-only
//! 按设备报告描述符处理 FunctionFS 控制端点请求。
use crate::{
    gadget::Topology,
    layout::{hid_descriptor, invalid},
    physical::Physical,
};
use std::{
    fs::File,
    io::{self, Read},
    os::fd::AsRawFd,
    sync::Arc,
};

pub(crate) fn write_packet(file: &File, data: &[u8]) -> io::Result<()> {
    // SAFETY：数据在调用期间可读；长度为零时仍发起系统调用。
    // 使用 write 提交零长度包，完成控制阶段确认。
    let count = unsafe { libc::write(file.as_raw_fd(), data.as_ptr().cast(), data.len()) };
    if count < 0 {
        return Err(io::Error::last_os_error());
    }
    if count as usize != data.len() {
        return Err(invalid("short USB control write"));
    }
    Ok(())
}

fn read_packet(file: &File, data: &mut [u8]) -> io::Result<usize> {
    // SAFETY：缓冲在调用期间可写，也允许零长度确认。
    let count = unsafe { libc::read(file.as_raw_fd(), data.as_mut_ptr().cast(), data.len()) };
    if count < 0 { Err(io::Error::last_os_error()) } else { Ok(count as usize) }
}

fn stall(ep0: &File, incoming: bool) {
    // FunctionFS通过反方向数据操作拒绝当前控制请求。
    if incoming {
        let _ = read_packet(ep0, &mut []);
    } else {
        let _ = write_packet(ep0, &[]);
    }
}

pub(crate) fn setup(
    ep0: &File,
    request: &[u8],
    devices: &[Arc<Physical>],
    topology: &Topology,
) -> io::Result<()> {
    let request_type = request[0];
    let incoming = request_type & 0x80 != 0;
    let operation = request[1];
    let value = u16::from_le_bytes([request[2], request[3]]);
    let index = usize::from(u16::from_le_bytes([request[4], request[5]]));
    let length = usize::from(u16::from_le_bytes([request[6], request[7]]));
    let Some(descriptor) = topology.descriptors.get(index) else {
        stall(ep0, incoming);
        return Ok(());
    };
    // Enumeration addresses a grouped USB interface; reports address its remapped ID.
    if request_type == 0x81 && operation == 6 {
        let response = match value {
            0x2200 => Some(descriptor.clone()),
            0x2100 => Some(hid_descriptor(descriptor.len()).to_vec()),
            _ => None,
        };
        if let Some(mut data) = response {
            data.truncate(length);
            return write_packet(ep0, &data);
        }
        stall(ep0, incoming);
        return Ok(());
    }
    if (request_type, operation) == (0xa1, 2) {
        return write_packet(ep0, &[0][..length.min(1)]);
    }
    if (request_type, operation) == (0x21, 10) && value >> 8 == 0 && length == 0 {
        read_packet(ep0, &mut [])?;
        return Ok(());
    }
    let Some((physical, id)) = topology.resolve(index, value as u8) else {
        stall(ep0, incoming);
        return Ok(());
    };
    let device = &devices[physical];
    let report_type = (value >> 8) as usize;
    let response = match (request_type, operation) {
        (0xa1, 1) if report_type == 1 => {
            // GET_REPORT(Input)
            device.latest_output(id).map(|data| topology.to_host(physical, &data)).transpose()?
        }
        (0xa1, 1) if report_type == 3 => match device.layout.bytes[2].get(&id) {
            Some(size) => match device.feature(id, *size, None) {
                Ok(bytes) => Some(topology.to_host(physical, &bytes)?),
                Err(error) => {
                    eprintln!("GET_FEATURE {}: {error}", device.name);
                    None
                }
            },
            None => None,
        },
        (0x21, 9) if matches!(report_type, 2 | 3) => {
            // SET_REPORT(Output/Feature)
            let physical_length = device.layout.bytes[report_type - 1].get(&id).copied();
            if physical_length.map(|n| topology.host_length(physical, n)) != Some(length)
                || length > 4096
            {
                stall(ep0, false);
                return Ok(());
            }
            let mut data = vec![0; length];
            if read_packet(ep0, &mut data)? != length {
                return Err(invalid("short SET_REPORT payload"));
            }
            let data = topology.from_host(physical, id, &data)?;
            if device.layout.numbered && data.first().copied() != Some(id) {
                return Err(invalid("SET_REPORT ID mismatch"));
            }
            // 控制数据阶段已完成；向实体设备转发失败时，将错误交给主循环结束会话。
            if report_type == 2 {
                device.output(&data)?;
            } else {
                device.feature(id, data.len(), Some(&data))?;
            }
            return Ok(());
        }
        _ => None,
    };
    if let Some(mut response) = response {
        response.truncate(length);
        write_packet(ep0, &response)
    } else {
        stall(ep0, incoming);
        Ok(())
    }
}

pub(crate) fn events(ep0: &mut File, buffer: &mut [u8; 48]) -> io::Result<usize> {
    let count = ep0.read(buffer)?;
    if count == 0 || count % 12 != 0 {
        return Err(invalid("invalid FunctionFS event record"));
    }
    Ok(count)
}

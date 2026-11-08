// SPDX-License-Identifier: GPL-2.0-only
//! HID 控制客户端、共享命令队列和二维位移变换。
pub mod client;
pub mod local;
pub mod shared;
pub mod trajectory;
pub mod wire;
mod sdk_ffi;
pub use sdk_ffi::{vhid_open, vhid_command, vhid_close};
#[cfg(test)]
mod regression;

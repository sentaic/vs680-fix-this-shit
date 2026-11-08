// SPDX-License-Identifier: GPL-2.0-only
//! 为阻塞式HID数据线程配置适度实时调度。
use std::io;

pub(crate) fn data_thread(priority: i32) {
    if priority == 0 {
        return;
    }
    let parameter = libc::sched_param { sched_priority: priority };
    // SAFETY：PID零表示调用线程，调度参数已初始化。
    if unsafe { libc::sched_setscheduler(0, libc::SCHED_FIFO, &raw const parameter) } != 0 {
        eprintln!("scheduling=fallback-normal error={}", io::Error::last_os_error());
    }
}

pub(crate) fn lock_memory() {
    // MCL_CURRENT 锁定调用时已有的内存映射。
    // SAFETY：操作只影响本进程，不涉及裸指针参数。
    if unsafe { libc::mlockall(libc::MCL_CURRENT) } == 0 {
        eprintln!("memory_lock=current-mappings");
    } else {
        eprintln!("memory_lock=unavailable error={}", io::Error::last_os_error());
    }
}

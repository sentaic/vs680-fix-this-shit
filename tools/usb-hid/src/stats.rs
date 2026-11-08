// SPDX-License-Identifier: GPL-2.0-only
//! 用固定直方图记录读取、处理、排队和 USB 写入耗时。
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::Instant;

#[derive(Clone, Copy)]
pub(crate) struct InputTiming {
    pub(crate) received: Instant,
    pub(crate) started: Instant,
}

const BOUNDS_US: [u64; 14] =
    [25, 50, 75, 100, 125, 250, 500, 750, 1000, 1500, 2000, 4000, 8000, u64::MAX];

#[derive(Debug, Default)]
pub(crate) struct Stats {
    pub(crate) received: AtomicU64,
    pub(crate) sent: AtomicU64,
    pub(crate) before_enable: AtomicU64,
    pub(crate) max_latency_us: AtomicU64,
    pub(crate) window_max_us: AtomicU64,
    pub(crate) max_queue_us: AtomicU64,
    pub(crate) max_usb_us: AtomicU64,
    pub(crate) over_period: AtomicU64,
    pub(crate) queue_full: AtomicU64,
    pub(crate) stale_reports: AtomicU64,
    pub(crate) physical_fallbacks: AtomicU64,
    pub(crate) poll_period_us: AtomicU64,
    histogram: [AtomicU64; 14],
    // Reset every reporting window; synthetic output has no input/processing stages.
    input_queue_window: AtomicU64,
    processing_window: AtomicU64,
    output_queue_window: AtomicU64,
    usb_window: AtomicU64,
}

impl Stats {
    pub(crate) fn stages(&self, input: Option<[u64; 2]>, output: u64, usb: u64) {
        if let Some([queued, processing]) = input {
            self.input_queue_window.fetch_max(queued, Ordering::Relaxed);
            self.processing_window.fetch_max(processing, Ordering::Relaxed);
        }
        self.output_queue_window.fetch_max(output, Ordering::Relaxed);
        self.usb_window.fetch_max(usb, Ordering::Relaxed);
    }

    pub(crate) fn take_stage_window(&self) -> [u64; 4] {
        [
            &self.input_queue_window,
            &self.processing_window,
            &self.output_queue_window,
            &self.usb_window,
        ]
        .map(|v| v.swap(0, Ordering::Relaxed))
    }

    pub(crate) fn completed(&self, total: u64, queue: u64, usb: u64, rate: u32) {
        self.max_latency_us.fetch_max(total, Ordering::Relaxed);
        self.window_max_us.fetch_max(total, Ordering::Relaxed);
        self.max_queue_us.fetch_max(queue, Ordering::Relaxed);
        self.max_usb_us.fetch_max(usb, Ordering::Relaxed);
        let configured = self.poll_period_us.load(Ordering::Relaxed);
        let period = if configured == 0 { 1_000_000 / u64::from(rate) } else { configured };
        if total > period {
            self.over_period.fetch_add(1, Ordering::Relaxed);
        }
        let bucket = BOUNDS_US.partition_point(|bound| *bound < total);
        self.histogram[bucket].fetch_add(1, Ordering::Relaxed);
        self.sent.fetch_add(1, Ordering::Relaxed);
    }

    /// 返回累计直方图中 P50、P95、P99 所在桶的上界。
    pub(crate) fn percentiles(&self) -> [u64; 3] {
        let counts = self.histogram.each_ref().map(|v| v.load(Ordering::Relaxed));
        let total: u64 = counts.iter().sum();
        [50, 95, 99].map(|percent| {
            if total == 0 {
                return 0;
            }
            let rank = (u128::from(total) * percent).div_ceil(100) as u64;
            let mut sum = 0;
            for (index, count) in counts.iter().enumerate() {
                sum += count;
                if sum >= rank {
                    return BOUNDS_US[index];
                }
            }
            u64::MAX
        })
    }
}

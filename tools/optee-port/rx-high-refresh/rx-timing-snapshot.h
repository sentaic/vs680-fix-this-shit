/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_TIMING_SNAPSHOT_H
#define VS680_RX_TIMING_SNAPSHOT_H
#include <stddef.h>
#include <stdint.h>
#define RX_TIMING_INPUT_PATH "/sys/kernel/vs680_rx_status/input"
#define RX_TIMING_MAX_TEXT 4096u
/* 输入像素时钟使用接收端的测量值。 */
enum rx_input_clock_source {
    RX_INPUT_CLOCK_UNKNOWN = 0,
    RX_INPUT_CLOCK_MEASURED_INPUT_PIXEL_CLOCK = 1
};
struct rx_input_clock_measurement {
    enum rx_input_clock_source source;
    uint64_t pixel_clock_hz;
};
struct rx_timing_snapshot {
    uint32_t lock_before, lock_after, deframer, video;
    uint32_t width, height, htotal, vtotal;
    uint32_t hfront, hsync_raw, hback, vfront, vsync_raw, vback;
    uint32_t vmon7, avi_header, avi_payload[4];
    uint32_t gcp_header, gcp_pb0, avmute;
    /* positive is a boolean; vendor HRX_MW_POSITIVE_POLARITY equals zero. */
    uint32_t hsync, vsync, hsync_positive, vsync_positive, progressive;
    uint32_t clock_known;
    uint64_t rgb24_bytes, pixel_clock_hz, refresh_millihz;
};
/* 解析稳定输入快照。短同步脉冲按公开驱动规则归一化；
 * 长脉冲返回 EOPNOTSUPP；时钟未知时频率字段为零。 */
int rx_timing_snapshot_parse(const char *text, size_t length,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out);
/* path 为 NULL 时读取 RX_TIMING_INPUT_PATH。 */
int rx_timing_snapshot_read(const char *path,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out);
/* 读取 RGB8 或 HDMI420 原始传输时序；420 的水平字段保留半宽传输单位。 */
int rx_timing_snapshot_read_transport(const char *path, struct rx_timing_snapshot *out);
/* 比较格式、信息包、同步和时钟字段；相同返回 1，变化返回 0，空参数返回 EINVAL。 */
int rx_timing_snapshot_same_input(const struct rx_timing_snapshot *a,
    const struct rx_timing_snapshot *b);
#endif

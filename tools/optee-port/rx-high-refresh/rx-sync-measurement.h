/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_SYNC_MEASUREMENT_H
#define VS680_RX_SYNC_MEASUREMENT_H
#include <stdint.h>
/* 极性取值来自 hrx-v4l2/hrx-drv.h:283-286 的枚举。 */
enum rx_sync_vendor_polarity {
    RX_SYNC_VENDOR_POSITIVE = 0,
    RX_SYNC_VENDOR_NEGATIVE = 1
};
enum rx_sync_measurement_result {
    RX_SYNC_MEASUREMENT_VALID = 0,
    RX_SYNC_MEASUREMENT_INVALID = -1,
    RX_SYNC_MEASUREMENT_UNNORMALIZED = -2
};
struct rx_sync_measurement {
    uint32_t pulse_width, positive;
    enum rx_sync_vendor_polarity vendor_polarity;
};
/* 按 hrx-v4l2/hrx-drv.c:2104-2113 分类原始间隔；输入为非零 16 位值。 */
static inline enum rx_sync_vendor_polarity rx_sync_raw_polarity(
    uint32_t active_pixels, uint32_t raw)
{
    return raw < active_pixels ? RX_SYNC_VENDOR_POSITIVE : RX_SYNC_VENDOR_NEGATIVE;
}
/* 接受短脉冲时序，长间隔返回 UNNORMALIZED；错误时保留原输出结构。 */
static inline int rx_sync_measurement_normalize(uint32_t active_pixels,
    uint32_t total, uint32_t front, uint32_t raw, uint32_t back,
    struct rx_sync_measurement *out)
{
    if (!out || !active_pixels || active_pixels > 65535u || !raw || raw > 65535u ||
        total > 65535u || total <= active_pixels ||
        front > 65535u || back > 65535u)
        return RX_SYNC_MEASUREMENT_INVALID;
    enum rx_sync_vendor_polarity polarity = rx_sync_raw_polarity(active_pixels, raw);
    if (polarity == RX_SYNC_VENDOR_NEGATIVE)
        return RX_SYNC_MEASUREMENT_UNNORMALIZED;
    if ((uint64_t)active_pixels + front + raw + back != total)
        return RX_SYNC_MEASUREMENT_INVALID;
    struct rx_sync_measurement measured = {raw,
        polarity == RX_SYNC_VENDOR_POSITIVE, polarity};
    *out = measured;
    return RX_SYNC_MEASUREMENT_VALID;
}
#endif

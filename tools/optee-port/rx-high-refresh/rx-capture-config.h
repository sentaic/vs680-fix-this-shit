/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_HIGH_REFRESH_CAPTURE_CONFIG_H
#define RX_HIGH_REFRESH_CAPTURE_CONFIG_H
#include <stdint.h>
#define RX_CAPTURE_CONFIG_COMMAND 0x7f32u
#define RX_CAPTURE_CONFIG_VERSION 1u
#define RX_CAPTURE_FORMAT_RGB888 1u
/* 原始 HDMI420 传输字：一个时钟内的三个 8 位分量，宽度使用传输时钟数。 */
#define RX_CAPTURE_FORMAT_HDMI420_8 2u
#ifndef RX_CAPTURE_CMA_BEGIN
#define RX_CAPTURE_CMA_BEGIN 0x75400000u
#endif
#ifndef RX_CAPTURE_CMA_END
#define RX_CAPTURE_CMA_END 0x76400000u
#endif
struct rx_capture_config {
    uint32_t version, size, width, height, stride, format;
    uint32_t video, vmon7, hfront, hsync_raw, hback, htotal;
    uint32_t vfront, vsync_raw, vback, vtotal;
    uint32_t avi_header, avi_payload[4], gcp_header, gcp_payload, avmute;
    uint64_t pixel_clock_hz;
    uint32_t reserved[4];
};
struct rx_capture_layout {
    uint32_t width, height, stride, bytes, allocation_bytes, pixels;
    uint32_t tg_size, otg_size, hb, tg_vb, otg_vb;
};
/* 校验全部输入后一次发布布局；失败保持原输出。 */
int rx_capture_layout_validate(const struct rx_capture_config *,
                               struct rx_capture_layout *);
int rx_capture_buffer_validate(const struct rx_capture_layout *,
                               uint64_t address, uint64_t length);
#endif

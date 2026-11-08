/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-capture-config.h"
#include "rx-sync-measurement.h"
int rx_capture_layout_validate(const struct rx_capture_config *c,
                               struct rx_capture_layout *out)
{
    if (!c || !out || c->version != RX_CAPTURE_CONFIG_VERSION ||
        c->size != sizeof(*c) || !c->width || !c->height ||
        c->width > 2560 || c->height > 1440 ||
        (c->format != RX_CAPTURE_FORMAT_RGB888 && c->format != RX_CAPTURE_FORMAT_HDMI420_8) ||
        c->stride != c->width * 3u || (c->stride & 15u) ||
        ((c->video & 15u) != 4u || (c->video & 0x70u) || (c->video & ~0xff7fu)) || c->vmon7 || (c->avmute & ~0x10u) ||
        ((c->avi_payload[1] >> 8) & 15u) ||
        ((c->avi_payload[0] >> 13) & 7u) !=
            (c->format == RX_CAPTURE_FORMAT_HDMI420_8 ? 3u : 0u))
        return -1;
    struct rx_sync_measurement hsync, vsync;
    if (rx_sync_measurement_normalize(c->width, c->htotal,
            c->hfront, c->hsync_raw, c->hback, &hsync) ||
        rx_sync_measurement_normalize(c->height, c->vtotal,
            c->vfront, c->vsync_raw, c->vback, &vsync))
        return -1;
    if ((c->avi_header & 0xffffu) != 0x0d02u) return -1;
    unsigned sum = 0x82u + 2u + 13u;
    for (unsigned i = 0; i < 14; i++)
        sum += (c->avi_payload[i / 4u] >> ((i % 4u) * 8u)) & 255u;
    if (sum & 255u) return -1;
    for (unsigned i = 0; i < 4; i++) if (c->reserved[i]) return -1;
    uint64_t bytes = (uint64_t)c->stride * c->height;
    if (bytes > RX_CAPTURE_CMA_END - RX_CAPTURE_CMA_BEGIN) return -1;
    struct rx_capture_layout p = {0};
    p.width = c->width; p.height = c->height; p.stride = c->stride;
    p.bytes = (uint32_t)bytes; p.pixels = c->width * c->height;
    p.allocation_bytes = (p.bytes + 4095u) & ~4095u;
    /* 官方 hrx-vip.c:309-334 的内部TG，和HDMI外部总数独立。 */
    p.tg_size = ((c->height + 3u) << 16) | (c->width + 20u);
    p.otg_size = ((c->height + 6u) << 16) | (c->width + 10u);
    p.hb = (7u << 16) | (c->width + 8u);
    p.tg_vb = (2u << 16) | (c->height + 3u);
    p.otg_vb = c->height + 1u;
    *out = p;
    return 0;
}
int rx_capture_buffer_validate(const struct rx_capture_layout *p,
                               uint64_t address, uint64_t length)
{
    if (!p || !p->bytes || address < RX_CAPTURE_CMA_BEGIN || (address & 63u) ||
        length < p->bytes || length > RX_CAPTURE_CMA_END - RX_CAPTURE_CMA_BEGIN ||
        address >= RX_CAPTURE_CMA_END || length > RX_CAPTURE_CMA_END - address)
        return -1;
    return 0;
}

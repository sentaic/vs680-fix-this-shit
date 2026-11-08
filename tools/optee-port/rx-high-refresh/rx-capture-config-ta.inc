/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-capture-config.c"
static struct rx_capture_config capture_geometry;
static struct rx_capture_layout capture_layout;
static int capture_geometry_configured;
/* 按官方 hdmirx.h 中的偏移读取寄存器。 */
static uint32_t capture_rx_read(uint32_t offset)
{
    return *(volatile uint32_t *)TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_AVIO_HDMIRX_REG_BASE + offset);
}
static int capture_geometry_matches_once(const struct rx_capture_config *c)
{
    uint32_t lock = capture_rx_read(0x0150);
    if (!(lock & 2u)) return 0;
    const uint32_t timing[7] = {
        c->hfront | (c->hsync_raw << 16), c->hback,
        c->width | (c->htotal << 16),
        c->vfront | (c->vsync_raw << 16), c->vback,
        c->height | (c->vtotal << 16), c->vmon7
    };
    for (unsigned i = 0; i < 7; i++) {
        uint32_t raw = capture_rx_read(0x1580 + i * 4u);
        uint32_t mask = (i == 1 || i == 4) ? 0xffffu : UINT32_MAX;
        if ((raw & mask) != timing[i]) return 0;
    }
    if (capture_rx_read(0x043c) != c->video ||
        (capture_rx_read(0x1200) & 0xffffu) != c->avi_header) return 0;
    for (unsigned i = 0; i < 4; i++)
        if ((capture_rx_read(0x1204 + i * 4u) & (i == 3 ? 0xffffu : UINT32_MAX)) != c->avi_payload[i]) return 0;
    if (capture_rx_read(0x1120) != c->gcp_header ||
        capture_rx_read(0x1124) != c->gcp_payload || capture_rx_read(0x1030) != c->avmute) return 0;
    return (capture_rx_read(0x0150) & 2u) != 0;
}
static int capture_geometry_matches(const struct rx_capture_config *c)
{
    return capture_geometry_matches_once(c) && capture_geometry_matches_once(c);
}
static TEE_Result capture_geometry_command(uint32_t types, TEE_Param p[4])
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
          TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) ||
        p[0].memref.size != sizeof(struct rx_capture_config) || !p[0].memref.buffer)
        return TEE_ERROR_BAD_PARAMETERS;
    struct rx_capture_config local;
    TEE_MemMove(&local, p[0].memref.buffer, sizeof(local));
    struct rx_capture_layout checked;
    if (rx_capture_layout_validate(&local, &checked)) return TEE_ERROR_BAD_PARAMETERS;
    prepare();
    if (active || busy()) return TEE_ERROR_BUSY;
    if (band_lines || !capture_geometry_matches(&local)) return TEE_ERROR_BAD_STATE;
    capture_geometry = local; capture_layout = checked;
    capture_geometry_configured = 1;
    local.reserved[0] = 0x52584331u;
    TEE_MemMove(p[0].memref.buffer, &local, sizeof(local));
    return TEE_SUCCESS;
}

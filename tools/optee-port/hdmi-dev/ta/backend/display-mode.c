/* SPDX-License-Identifier: GPL-2.0 */
/* Register/timing facts from public Synaptics/Synopsys GPL-2.0 sources.
 * Copyright (C) 2025 Synopsys, Inc.
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * Public source:
 * https://github.com/synaptics-astra/tee-optee_dev/tree/
 * e397232786bfee0599dc9d51eb3ca2d6db008eda/ta/display
 *   ta/vpp/g7/vppdrv/source/BE/hdmi/vendor/synopsys/api/src/
 *     edid/dtd.c; core/{fc_video,fc_avi,video_sampler,video_packetizer}.c;
 *     core/frame_composer/frame_composer_reg.h; phy/{phy,phy_316}.c
 *   .../BE/hdmi/vendor_interface/synopsys/vpp_be_hdmitx.c:BE_HDMITX_init
 * https://github.com/synaptics-astra/linux_5_15-overlay/blob/
 * 9d728a1ccc20762969b3a695c22ba41d296d606b/drivers/clk/berlin/dolphin-pll.c
 *
 * Linux design references:
 * https://github.com/torvalds/linux/blob/v6.12/drivers/gpu/drm/drm_modes.c
 * https://github.com/torvalds/linux/blob/v6.12/drivers/gpu/drm/drm_edid.c
 * https://github.com/torvalds/linux/blob/v6.12/drivers/gpu/drm/bridge/synopsys/dw-hdmi.c
 * Use the target sink's detailed timing for mode and link calculations.
 */
#include "display-mode.h"
#include "vs680_phy_range.h"

#define SNPS_BASE UINT32_C(0xf7440000)
#define PHY_CONF0 UINT32_C(0xc000)
#define PHY_STAT0 UINT32_C(0xc010)
#define MC_CLKDIS UINT32_C(0x10004)
#define MC_PHYRSTZ UINT32_C(0x10014)
#define FC_SCRAMBLER_CTRL UINT32_C(0x4384)
#define IH_I2CMPHY_STAT0 UINT32_C(0x420)
#define PHY_I2CM_ADDRESS UINT32_C(0xc084)
#define PHY_I2CM_DATAO_1 UINT32_C(0xc088)
#define PHY_I2CM_DATAO_0 UINT32_C(0xc08c)
#define PHY_I2CM_DATAI_1 UINT32_C(0xc090)
#define PHY_I2CM_DATAI_0 UINT32_C(0xc094)
#define PHY_I2CM_OPERATION UINT32_C(0xc098)
#define STEP(expr) do { enum hdmi_display_result result_ = (expr); \
    if (result_ != HDMI_DISPLAY_OK) return result_; } while (0)

static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

enum hdmi_display_result hdmi_display_get_preset(
    enum hdmi_display_preset preset, struct hdmi_display_timing *out)
{
    struct hdmi_display_timing value;
    if (out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    switch (preset) {
    case HDMI_DISPLAY_720P60:
        value = (struct hdmi_display_timing){74250000, 1280,1390,1430,1650,
            720,725,730,750, HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC, 4};
        break;
    case HDMI_DISPLAY_1080P60:
    case HDMI_DISPLAY_1080P120:
        value = (struct hdmi_display_timing){
            preset == HDMI_DISPLAY_1080P60 ? 148500000 : 297000000,
            1920,2008,2052,2200, 1080,1084,1089,1125,
            HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC,
            preset == HDMI_DISPLAY_1080P60 ? 16u : 63u};
        break;
    case HDMI_DISPLAY_1440P60:
        /* 厂商条目 238 使用 59.950550 Hz、正 H/V 极性；它是厂商内部编号，CTA VIC 字段保持 0。 */
        value = (struct hdmi_display_timing){241500000, 2560,2608,2640,2720,
            1440,1443,1448,1481, HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC, 0};
        break;
    case HDMI_DISPLAY_2160P60:
        value = (struct hdmi_display_timing){594000000, 3840,4016,4104,4400,
            2160,2168,2178,2250, HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC, 97};
        break;
    case HDMI_DISPLAY_1080P240:
    case HDMI_DISPLAY_1440P144:
        return HDMI_DISPLAY_TIMING_REQUIRED;
    default:
        return HDMI_DISPLAY_BAD_ARGUMENT;
    }
    *out = value;
    return HDMI_DISPLAY_OK;
}

static bool same_timing(const struct hdmi_display_timing *a,
                        const struct hdmi_display_timing *b)
{
    return a->pixel_clock_hz == b->pixel_clock_hz &&
        a->hdisplay == b->hdisplay && a->hsync_start == b->hsync_start &&
        a->hsync_end == b->hsync_end && a->htotal == b->htotal &&
        a->vdisplay == b->vdisplay && a->vsync_start == b->vsync_start &&
        a->vsync_end == b->vsync_end && a->vtotal == b->vtotal &&
        a->flags == b->flags;
}

enum hdmi_display_result hdmi_display_validate_timing(
    const struct hdmi_display_timing *t, struct hdmi_display_timing_info *out)
{
    struct hdmi_display_timing_info value;
    struct hdmi_display_timing known;
    enum hdmi_display_preset preset;
    uint64_t divisor;
    if (t == NULL || out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    if ((t->flags & ~(HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC |
                     HDMI_DISPLAY_INTERLACE | HDMI_DISPLAY_DBLSCAN)) != 0)
        return HDMI_DISPLAY_BAD_TIMING;
    if ((t->flags & (HDMI_DISPLAY_INTERLACE | HDMI_DISPLAY_DBLSCAN)) != 0)
        return HDMI_DISPLAY_UNSUPPORTED_SCAN;
    if (t->pixel_clock_hz == 0 || t->hdisplay == 0 || t->vdisplay == 0 ||
        t->hdisplay > t->hsync_start || t->hsync_start >= t->hsync_end ||
        t->hsync_end > t->htotal || t->vdisplay > t->vsync_start ||
        t->vsync_start >= t->vsync_end || t->vsync_end > t->vtotal)
        return HDMI_DISPLAY_BAD_TIMING;
    if (t->cta_vic != 0) {
        switch (t->cta_vic) {
        case 4: preset = HDMI_DISPLAY_720P60; break;
        case 16: preset = HDMI_DISPLAY_1080P60; break;
        case 63: preset = HDMI_DISPLAY_1080P120; break;
        case 97: preset = HDMI_DISPLAY_2160P60; break;
        default: return HDMI_DISPLAY_VIC_MISMATCH;
        }
        STEP(hdmi_display_get_preset(preset, &known));
        if (!same_timing(t, &known)) return HDMI_DISPLAY_VIC_MISMATCH;
    }
    value.hblank = t->htotal - t->hdisplay;
    value.hfront = t->hsync_start - t->hdisplay;
    value.hsync = t->hsync_end - t->hsync_start;
    value.hback = t->htotal - t->hsync_end;
    value.vblank = t->vtotal - t->vdisplay;
    value.vfront = t->vsync_start - t->vdisplay;
    value.vsync = t->vsync_end - t->vsync_start;
    value.vback = t->vtotal - t->vsync_end;
    value.refresh_num = t->pixel_clock_hz;
    value.refresh_den = (uint64_t)t->htotal * t->vtotal;
    divisor = gcd64(value.refresh_num, value.refresh_den);
    value.refresh_num /= divisor;
    value.refresh_den /= divisor;
    *out = value;
    return HDMI_DISPLAY_OK;
}

enum hdmi_display_result hdmi_display_plan_clock(
    uint64_t hz, struct hdmi_display_clock_plan *out)
{
    struct hdmi_display_clock_plan value = {0};
    uint64_t divider, feedback, divisor;
    if (out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    if (hz < UINT64_C(20000000) || hz > UINT64_C(3000000000))
        return HDMI_DISPLAY_CLOCK_RANGE;
    divider = UINT64_C(6000000000) / (2u * hz);
    if (divider > 32u) divider = 32u;
    value.vco_target_hz = hz * 2u * divider;
    if (divider == 0 || value.vco_target_hz < UINT64_C(1200000000))
        return HDMI_DISPLAY_CLOCK_RANGE;
    /* Fixed 25-MHz / 5 PFD; floor quantization follows the driver. All
     * intermediates are <= 6e9 * 5 * 2^24, safely within uint64_t. */
    feedback = (value.vco_target_hz * 5u * (UINT64_C(1) << 24)) /
               UINT64_C(100000000);
    value.divr = 4;
    value.divfi = (uint32_t)(feedback >> 24) - 1u;
    value.divff = (uint32_t)(feedback & UINT32_C(0xffffff));
    value.divq = (uint32_t)divider - 1u;
    value.range = 0;
    value.reference_hz = 25000000;
    value.target_hz = hz;
    value.actual_num = UINT64_C(100000000) * feedback;
    value.actual_den = 5u * (2u * divider) * (UINT64_C(1) << 24);
    divisor = gcd64(value.actual_num, value.actual_den);
    value.actual_num /= divisor;
    value.actual_den /= divisor;
    *out = value;
    return HDMI_DISPLAY_OK;
}

enum hdmi_display_result hdmi_display_plan_link_rgb8(
    const struct hdmi_display_timing *t,
    const struct hdmi_display_link_caps *caps,
    struct hdmi_display_link_plan *out)
{
    struct hdmi_display_timing_info info;
    struct hdmi_display_link_plan value;
    if (caps == NULL || out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    STEP(hdmi_display_validate_timing(t, &info));
    if (t->pixel_clock_hz > UINT64_MAX / 30u) return HDMI_DISPLAY_LINK_RATE;
    value.tmds_character_hz = t->pixel_clock_hz;
    value.lane_bit_rate_bps = t->pixel_clock_hz * 10u;
    value.total_bit_rate_bps = t->pixel_clock_hz * 30u;
    value.requires_scrambling = t->pixel_clock_hz > UINT64_C(340000000);
    value.requires_clock_ratio_40 = value.requires_scrambling;
    *out = value; /* diagnostic rate stays available on capability failure */
    if (caps->source_max_tmds_hz == 0 || caps->sink_max_tmds_hz == 0)
        return HDMI_DISPLAY_CAPABILITIES_REQUIRED;
    if (!caps->sink_is_hdmi) return HDMI_DISPLAY_LINK_UNSUPPORTED;
    if (!caps->sink_timing_listed) return HDMI_DISPLAY_SINK_MODE_UNLISTED;
    if (t->pixel_clock_hz > caps->source_max_tmds_hz ||
        t->pixel_clock_hz > caps->sink_max_tmds_hz ||
        t->pixel_clock_hz > UINT64_C(600000000))
        return HDMI_DISPLAY_LINK_RATE;
    if (value.requires_scrambling) {
        if (!caps->source_scdc || !caps->sink_scdc)
            return HDMI_DISPLAY_LINK_UNSUPPORTED;
        return HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION;
    }
    return HDMI_DISPLAY_OK;
}

enum hdmi_display_result hdmi_display_plan_core_rgb8(
    const struct hdmi_display_timing *t, struct hdmi_display_core_plan *out)
{
    struct hdmi_display_timing_info v;
    struct hdmi_display_core_plan p = {0};
    uint32_t polarity, aspect;
    if (out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    STEP(hdmi_display_validate_timing(t, &v));
    /* 检查公开驱动使用的字段宽度：H active/blank/front 13 位、V active 13 位、H sync 9 位、V blank/front 8 位、V sync 6 位。 */
    if (t->hdisplay > 8191u || v.hblank > 8191u || v.hfront > 8191u ||
        t->vdisplay > 8191u || v.hsync > 511u || v.vblank > 255u ||
        v.vfront > 255u || v.vsync > 63u)
        return HDMI_DISPLAY_REGISTER_RANGE;
    polarity = 0x18u | ((t->flags & HDMI_DISPLAY_PHSYNC) ? 0x20u : 0u) |
                         ((t->flags & HDMI_DISPLAY_PVSYNC) ? 0x40u : 0u);
    aspect = (uint64_t)t->hdisplay * 9u == (uint64_t)t->vdisplay * 16u ?
             0x20u : ((uint64_t)t->hdisplay * 3u == (uint64_t)t->vdisplay * 4u ?
             0x10u : 0u);
#define FIELD(off, m, val) do { \
    p.fields[p.count++] = (struct hdmi_display_reg_field){off, m, val}; \
} while (0)
#define SPLIT(off, val, hi_mask) do { \
    FIELD(off, 0xff, (val) & 0xffu); \
    FIELD((off) + 4u, hi_mask, (val) >> 8); \
} while (0)
    FIELD(0x4000, 0xfb, polarity);
    SPLIT(0x4004, t->hdisplay, 0x1f);
    SPLIT(0x400c, v.hblank, 0x1f);
    SPLIT(0x4014, t->vdisplay, 0x1f);
    FIELD(0x401c, 0xff, v.vblank);
    SPLIT(0x4020, v.hfront, 0x1f);
    SPLIT(0x4028, v.hsync, 1);
    FIELD(0x4030, 0xff, v.vfront);
    FIELD(0x4034, 0x3f, v.vsync);
    FIELD(0x4044, 0xff, 12); FIELD(0x4048, 0xff, 32);
    FIELD(0x404c, 0xff, 1); FIELD(0x4050, 0xff, 11);
    FIELD(0x4054, 0x3f, 22); FIELD(0x4058, 0x3f, 33);
    FIELD(0x4380, 0xff, 0x10); /* 像素重复次数为 0。 */
    FIELD(0x2004, 0xff, 0); FIELD(0x2008, 0x27, 7);
    FIELD(0x200c, 3, 0); FIELD(0x2010, 0x7f, 0x47);
    FIELD(0x800, 0x9f, 1); /* RGB8 sampler */
    FIELD(0x808, 0xff, 0); FIELD(0x80c, 0xff, 0);
    FIELD(0x810, 0xff, 0); FIELD(0x814, 0xff, 0);
    FIELD(0x818, 0xff, 0); FIELD(0x81c, 0xff, 0);
    FIELD(0x804, 7, 7);
    FIELD(0x4064, 0xff, 0x40); /* RGB; active aspect same as picture */
    FIELD(0x4068, 0xff, aspect | 8u);
    FIELD(0x406c, 0xff, 8); /* 全范围 RGB。 */
    FIELD(0x405c, 0x0f, 0); FIELD(0x4070, 0xff, t->cta_vic);
    FIELD(0x42cc, 0x1f, 0); FIELD(0x42d0, 0x0f, 0);
    FIELD(0x42d4, 0xff, 0x10); FIELD(0x42dc, 0x3f, 0x0c);
    FIELD(0x438c, 0xff, 6); FIELD(0x4060, 7, 2); /* AVMUTE */
    /* main_controller_reg.h 的 bit0=1 接入 CSC；RGB 直通使用旁路。 */
    FIELD(0x10010, 1, 0);
#undef SPLIT
#undef FIELD
    *out = p;
    return HDMI_DISPLAY_OK;
}

enum hdmi_display_result hdmi_display_plan_phy_rgb8(
    uint64_t hz, struct hdmi_display_phy_plan *out)
{
    struct hdmi_display_phy_plan p = {hz, hz, {{0x04,0}, {0x06,0}, {0x10,0},
        {0x11,0}, {0x19,0}, {0x0e,0}, {0x09,0}}};
    if (out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    switch (hz) {
    case UINT64_C(74250000):
        p.regs[1].value = 0x0002; p.regs[2].value = 0x1142;
        p.regs[3].value = 0x0214; break;
    case UINT64_C(148500000):
        p.regs[1].value = 0x0001; p.regs[2].value = 0x2080;
        p.regs[3].value = 0x020a; break;
    case UINT64_C(241500000):
        /* phy_get_freq 将 241.50 MHz 映射为 245.250 MHz，并选择对应 RGB8 参数。 */
        p.table_clock_hz = 245250000;
        p.regs[1].value = 0x0000; p.regs[2].value = 0x3040;
        p.regs[3].value = 0x0605; break;
    case UINT64_C(297000000):
        p.regs[1].value = 0x0000; p.regs[2].value = 0x3041;
        p.regs[3].value = 0x0205; break;
    case UINT64_C(594000000):
        p.regs[1].value = 0x0640; p.regs[2].value = 0x3080;
        p.regs[3].value = 0x0005; break;
    default: {
        const struct vs680_phy_request request = {hz, VS680_RGB, 8, 0};
        const struct vs680_phy_limits limits = {594000000, 594000000, 594000000};
        struct vs680_phy_selection selection;
        /* The high-band trial uses the real requested clock throughout.
         * Link planning separately checks the actual sink and SCDC state. */
        if (vs680_phy_select(&request, &limits, &vs680_phy316_high8_candidate,
                            1, 1, &selection) != VS680_SELECT_OK)
            return HDMI_DISPLAY_PHY_UNAVAILABLE;
        p.regs[1].value = selection.profile->opmode_pllcfg;
        p.regs[2].value = selection.profile->pllcurrctrl;
        p.regs[3].value = selection.profile->plldivctrl;
        break;
    }
    }
    /* 采用 BE_HDMITX_init 的模拟参数默认值。 */
    if (hz <= UINT64_C(165000000)) {
        p.regs[4].value = 7; p.regs[5].value = 0x0180;
        p.regs[6].value = 0x8188;
    } else if (hz < UINT64_C(371250000)) {
        p.regs[5].value = 0x0120; p.regs[6].value = 0x83d8;
    } else {
        p.regs[0].value = 0xb4db; p.regs[6].value = 0x8fd4;
    }
    *out = p;
    return HDMI_DISPLAY_OK;
}

static enum hdmi_display_result rd(const struct hdmi_display_io *io,
    struct hdmi_display_report *r, uint32_t offset, uint32_t *value)
{
    r->last_address = SNPS_BASE + offset;
    r->callback_error = io->read32(io->opaque, r->last_address, value);
    return r->callback_error ? HDMI_DISPLAY_IO_ERROR : HDMI_DISPLAY_OK;
}
static enum hdmi_display_result wr(const struct hdmi_display_io *io,
    struct hdmi_display_report *r, uint32_t offset, uint32_t value)
{
    r->last_address = SNPS_BASE + offset;
    r->callback_error = io->write32(io->opaque, r->last_address, value);
    if (r->callback_error) return HDMI_DISPLAY_IO_ERROR;
    ++r->writes_completed;
    return HDMI_DISPLAY_OK;
}
static enum hdmi_display_result delay(const struct hdmi_display_io *io,
    struct hdmi_display_report *r, uint32_t milliseconds)
{
    r->callback_error = io->wait_ms(io->opaque, milliseconds);
    return r->callback_error ? HDMI_DISPLAY_IO_ERROR : HDMI_DISPLAY_OK;
}
static enum hdmi_display_result field(const struct hdmi_display_io *io,
    struct hdmi_display_report *r, uint32_t offset, uint32_t mask, uint32_t value)
{
    uint32_t old, check;
    STEP(rd(io, r, offset, &old));
    STEP(wr(io, r, offset, (old & ~mask) | (value & mask)));
    STEP(rd(io, r, offset, &check));
    if ((check & mask) != (value & mask)) return HDMI_DISPLAY_READBACK_MISMATCH;
    return HDMI_DISPLAY_OK;
}
static enum hdmi_display_result phy_wait(const struct hdmi_display_io *io,
    struct hdmi_display_report *r)
{
    uint32_t status, i;
    for (i = 0; i < 400u; ++i) {
        STEP(delay(io, r, 2));
        STEP(rd(io, r, IH_I2CMPHY_STAT0, &status));
        status &= 3u;
        r->last_phy_i2c_status = status;
        if (status != 0) {
            STEP(wr(io, r, IH_I2CMPHY_STAT0, status)); /* 按 W1C 语义直接写入清除掩码。 */
            if ((status & 1u) != 0) return HDMI_DISPLAY_IO_ERROR;
            return HDMI_DISPLAY_OK;
        }
    }
    return HDMI_DISPLAY_TIMEOUT;
}
static enum hdmi_display_result phy_register(const struct hdmi_display_io *io,
    struct hdmi_display_report *r, const struct hdmi_display_phy_reg *reg)
{
    uint32_t high, low;
    STEP(wr(io, r, IH_I2CMPHY_STAT0, 3));
    STEP(wr(io, r, PHY_I2CM_ADDRESS, reg->reg));
    STEP(wr(io, r, PHY_I2CM_DATAO_1, reg->value >> 8));
    STEP(wr(io, r, PHY_I2CM_DATAO_0, reg->value & 0xffu));
    STEP(wr(io, r, PHY_I2CM_OPERATION, 0x10));
    STEP(phy_wait(io, r));
    STEP(wr(io, r, IH_I2CMPHY_STAT0, 3));
    STEP(wr(io, r, PHY_I2CM_ADDRESS, reg->reg));
    STEP(wr(io, r, PHY_I2CM_OPERATION, 1));
    STEP(phy_wait(io, r));
    STEP(rd(io, r, PHY_I2CM_DATAI_1, &high));
    STEP(rd(io, r, PHY_I2CM_DATAI_0, &low));
    if (((high & 0xffu) << 8 | (low & 0xffu)) != reg->value)
        return HDMI_DISPLAY_READBACK_MISMATCH;
    return HDMI_DISPLAY_OK;
}

static enum hdmi_display_result require_stopped(const struct hdmi_display_io *io,
    struct hdmi_display_report *r)
{
    uint32_t value;
    STEP(rd(io, r, PHY_CONF0, &value));
    if ((value & 0x18u) != 0x10u) return HDMI_DISPLAY_OUTPUT_ACTIVE;
    STEP(rd(io, r, MC_CLKDIS, &value));
    if ((value & 0x5fu) != 0x5fu) return HDMI_DISPLAY_OUTPUT_ACTIVE;
    STEP(rd(io, r, PHY_STAT0, &value));
    if ((value & 1u) != 0) return HDMI_DISPLAY_OUTPUT_ACTIVE;
    STEP(rd(io, r, FC_SCRAMBLER_CTRL, &value));
    if ((value & 1u) != 0) return HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION;
    return HDMI_DISPLAY_OK;
}

static enum hdmi_display_result prepare_stopped(
    const struct hdmi_display_timing *t,
    const struct hdmi_display_link_caps *caps,
    const struct hdmi_display_io *io, struct hdmi_display_report *r, bool allow_scdc)
{
    struct hdmi_display_core_plan core;
    struct hdmi_display_clock_plan clock;
    struct hdmi_display_phy_plan phy;
    struct hdmi_display_link_plan link;
    size_t i;
    if (r == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    *r = (struct hdmi_display_report){0};
    if (io == NULL || io->read32 == NULL || io->write32 == NULL ||
        io->wait_ms == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    STEP(hdmi_display_plan_core_rgb8(t, &core));
    STEP(hdmi_display_plan_clock(t->pixel_clock_hz, &clock));
    enum hdmi_display_result link_result=hdmi_display_plan_link_rgb8(t,caps,&link);
    if(link_result==HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION && allow_scdc)
        r->requires_scdc=true;
    else if(link_result!=HDMI_DISPLAY_OK)return link_result;
    STEP(hdmi_display_plan_phy_rgb8(t->pixel_clock_hz, &phy));
    if (io->configure_clock == NULL) return HDMI_DISPLAY_CLOCK_ADAPTER_REQUIRED;
    r->stage = HDMI_DISPLAY_STAGE_PREFLIGHT;
    STEP(require_stopped(io, r));
    r->stage = HDMI_DISPLAY_STAGE_CLOCK;
    r->callback_error = io->configure_clock(io->opaque, &clock);
    if (r->callback_error != 0) return HDMI_DISPLAY_IO_ERROR;
    r->clock_adapter_completed = true;
    STEP(require_stopped(io, r));
    r->stage = HDMI_DISPLAY_STAGE_CORE;
    for (i = 0; i < core.count; ++i)
        STEP(field(io, r, core.fields[i].offset, core.fields[i].mask,
                   core.fields[i].value));
    r->stage = HDMI_DISPLAY_STAGE_PHY;
    STEP(field(io, r, MC_PHYRSTZ, 1, 1));
    STEP(field(io, r, PHY_CONF0, 0x3b, 0x32)); /* off, PDDQ, SVSRET, DE+ */
    STEP(field(io, r, MC_PHYRSTZ, 1, 0));
    STEP(field(io, r, 0xc0d0, 0xff, 0x10));
    STEP(field(io, r, 0xc080, 0x7f, 0x69));
    STEP(field(io, r, 0xc09c, 4, 0));
    STEP(field(io, r, 0xc0a0, 0x44, 0));
    for (i = 0; i < HDMI_DISPLAY_PHY_REGS; ++i)
        STEP(phy_register(io, r, &phy.regs[i]));
    STEP(delay(io, r, 1));
    STEP(field(io, r, FC_SCRAMBLER_CTRL, 1, 0));
    /* 再次检查输出停止状态。 */
    STEP(require_stopped(io, r));
    r->stage = HDMI_DISPLAY_STAGE_PREPARED;
    return HDMI_DISPLAY_OK;
}

const char *hdmi_display_result_name(enum hdmi_display_result result)
{
    static const char *const names[] = {
        "ok (planning/preparation only)", "bad argument", "bad timing",
        "unsupported scan flags", "VIC/timing mismatch", "register field range",
        "VPLL numerical range", "target detailed timing required",
        "source/sink capabilities required", "timing not listed by sink",
        "unsupported HDMI link", "TMDS rate exceeds limit",
        "actual SCDC negotiation required", "PHY316 profile unavailable",
        "real pixel-clock adapter required", "output is not verified stopped",
        "I/O failure", "register readback mismatch", "PHY I2C timeout"
    };
    if ((unsigned)result >= sizeof(names) / sizeof(names[0])) return "unknown result";
    return names[result];
}

enum hdmi_display_result hdmi_display_program_stopped_rgb8(
    const struct hdmi_display_timing *t,const struct hdmi_display_link_caps *caps,
    const struct hdmi_display_io *io,struct hdmi_display_report *r)
{ return prepare_stopped(t,caps,io,r,false); }
enum hdmi_display_result hdmi_display_prepare_stopped_rgb8(
    const struct hdmi_display_timing *t,const struct hdmi_display_link_caps *caps,
    const struct hdmi_display_io *io,struct hdmi_display_report *r)
{ return prepare_stopped(t,caps,io,r,true); }

static enum hdmi_display_result phy_remaining(
    const struct hdmi_display_deadline_io *time,struct hdmi_display_report *r,
    uint64_t end,uint64_t *previous,uint32_t *left)
{
    uint64_t now;
    r->callback_error=time->now_us(time->opaque,&now);
    if(r->callback_error)return HDMI_DISPLAY_IO_ERROR;
    if(now<*previous)return HDMI_DISPLAY_TIMEOUT;
    *previous=now;
    if(now>=end)return HDMI_DISPLAY_TIMEOUT;
    *left=(uint32_t)(end-now);return HDMI_DISPLAY_OK;
}
enum hdmi_display_result hdmi_display_verify_phy_rgb8(uint64_t hz,
    const struct hdmi_display_io *io,const struct hdmi_display_deadline_io *time,
    uint32_t timeout,struct hdmi_display_report *r)
{
    struct hdmi_display_phy_plan p;
    uint64_t started,previous,end;
    uint32_t status,high,low,left;
    if(!r)return HDMI_DISPLAY_BAD_ARGUMENT;
    *r=(struct hdmi_display_report){0};
    if(!io || !io->read32 || !io->write32 || !time || !time->now_us ||
        !time->wait_us || !timeout || timeout>20000)return HDMI_DISPLAY_BAD_ARGUMENT;
    STEP(hdmi_display_plan_phy_rgb8(hz,&p));
    r->callback_error=time->now_us(time->opaque,&started);
    if(r->callback_error)return HDMI_DISPLAY_IO_ERROR;
    if(started>UINT64_MAX-timeout)return HDMI_DISPLAY_TIMEOUT;
    previous=started;end=started+timeout;r->stage=HDMI_DISPLAY_STAGE_PREFLIGHT;
    STEP(require_stopped(io,r));
    /* Verify bus selection/reset/interrupt masks from actual configuration. */
    const struct {uint32_t address,mask,value;} fields[]={
        {0xc080,0x7f,0x69},{0xc09c,4,0},{0xc0a0,0x44,0},{MC_PHYRSTZ,1,0}
    };
    for(size_t i=0;i<sizeof(fields)/sizeof(fields[0]);++i) {
        STEP(rd(io,r,fields[i].address,&status));
        if((status&fields[i].mask)!=fields[i].value)return HDMI_DISPLAY_READBACK_MISMATCH;
    }
    r->stage=HDMI_DISPLAY_STAGE_PHY;
    for(size_t i=0;i<HDMI_DISPLAY_PHY_REGS;++i) {
        STEP(phy_remaining(time,r,end,&previous,&left));
        STEP(wr(io,r,IH_I2CMPHY_STAT0,3));
        STEP(rd(io,r,IH_I2CMPHY_STAT0,&status));
        if(status&3u)return HDMI_DISPLAY_READBACK_MISMATCH;
        STEP(wr(io,r,PHY_I2CM_ADDRESS,p.regs[i].reg));
        STEP(wr(io,r,PHY_I2CM_OPERATION,1));
        bool completed=false;
        for(unsigned poll=0;poll<512;++poll) {
            STEP(phy_remaining(time,r,end,&previous,&left));
            STEP(rd(io,r,IH_I2CMPHY_STAT0,&status));
            r->last_phy_i2c_status=status&3u;
            if(status&1u)return HDMI_DISPLAY_IO_ERROR;
            if(status&2u) {completed=true;break;}
            uint32_t pause=left<50u ? left : 50u;
            uint64_t before=previous,after;
            r->callback_error=time->wait_us(time->opaque,pause,left);
            if(r->callback_error)return HDMI_DISPLAY_IO_ERROR;
            r->callback_error=time->now_us(time->opaque,&after);
            if(r->callback_error)return HDMI_DISPLAY_IO_ERROR;
            if(after<before || after-before<pause)return HDMI_DISPLAY_TIMEOUT;
            previous=after;
        }
        if(!completed)return HDMI_DISPLAY_TIMEOUT;
        STEP(rd(io,r,PHY_I2CM_DATAI_1,&high));STEP(rd(io,r,PHY_I2CM_DATAI_0,&low));
        if((((high&255u)<<8)|(low&255u))!=p.regs[i].value)
            return HDMI_DISPLAY_READBACK_MISMATCH;
        STEP(wr(io,r,IH_I2CMPHY_STAT0,2));STEP(rd(io,r,IH_I2CMPHY_STAT0,&status));
        if(status&3u)return HDMI_DISPLAY_READBACK_MISMATCH;
        STEP(phy_remaining(time,r,end,&previous,&left));
    }
    STEP(require_stopped(io,r));
    STEP(phy_remaining(time,r,end,&previous,&left));
    r->stage=HDMI_DISPLAY_STAGE_PREPARED;
    return HDMI_DISPLAY_OK;
}

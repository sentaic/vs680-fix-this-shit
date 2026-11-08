/* SPDX-License-Identifier: GPL-2.0 */
/* Register/timing facts from public Synaptics GPL-2.0 sources.
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * Audited source root, fixed to the dolphin register layout:
 * https://github.com/synaptics-astra/tee-optee_dev/tree/
 * e397232786bfee0599dc9d51eb3ca2d6db008eda/ta/display
 *   common/include/dolphin/{vpp,vpp_gfx_vop}.h
 *   ta/vpp/g7/vppdrv/source/FE/source/vpp_fe_dlr.c:
 *     FE_DLR_SetClientSize, SetPlaneSize, SetDummyTG_gfx1
 *   .../CPCB/source/vpp_cpcb{,_tg,_disptg,_ovl}.c:
 *     SetOutputResolution, SetHVTotal, SetParams, SetPlaneWindow
 *   .../BE/source/vpp_be_vop.c:BE_VOP_SetPixTot
 *   .../include/vpp_module.h:IS_RES_NEED_REDUCED_BLANKING
 *   .../vpp_cfg.c:RES_CVT_RB_2560X1440P60
 * The 720p geometry is also cross-checked against local mode-720p.c.
 */
#include "vpp-timing.h"

#define GFX_CTRL       UINT32_C(0xf7491000)
#define GFX_SCL        UINT32_C(0xf7491008)
#define GFX_WORDS      UINT32_C(0xf749100c)
#define GFX_PIXELS     UINT32_C(0xf7491010)
#define GFX_DUMMY_TG   UINT32_C(0xf7491800)
#define CPCB_CTRL2     UINT32_C(0xf7492a3c)
#define CPCB_PIXELS    UINT32_C(0xf7492a48)
#define CPCB_OVL       UINT32_C(0xf7492a5c)
#define CPCB_BETG      UINT32_C(0xf7492a90)
#define CPCB_TG        UINT32_C(0xf7492b20)
#define VOP_BETG       UINT32_C(0xf7496014)
#define VOP_TG         UINT32_C(0xf74960a4)
#define VOP_START     UINT32_C(0xf7480a94)
#define VOP_PIXELS     UINT32_C(0xf7480a98)
#define DISPLAY_CTRL1 UINT32_C(0xf7480bf0)
#define DISPLAY_BETG  UINT32_C(0xf7480bf4)
#define DISPLAY_TG    UINT32_C(0xf7480c84)

#define STEP(expr) do { enum hdmi_display_result result_ = (expr); \
    if (result_ != HDMI_DISPLAY_OK) return result_; } while (0)

_Static_assert(HDMI_VPP_TIMING_FIELDS >= 67u, "VPP timing recipe capacity");

static void field(struct hdmi_vpp_timing_plan *p, uint32_t address,
                  uint32_t mask, uint32_t value)
{
    /* 固定配置表包含 67 个字段。 */
    p->fields[p->count++] = (struct hdmi_vpp_reg_field){address, mask, value};
}

static void tg(struct hdmi_vpp_timing_plan *p, uint32_t base,
               uint32_t hlast, uint32_t vlast, uint32_t hs, uint32_t he,
               uint32_t vs, uint32_t ve, uint32_t frame_reset, bool display)
{
    field(p, base + 8u, 0x01ffffff, (hlast << 12) | vlast);
    field(p, base + 12u, 0x01ffffff, hlast > 8u ? hlast - 8u : 0u);
    field(p, base + 28u, 0xff, p->pixels & 0xffu);
    field(p, base, UINT32_MAX, (frame_reset << 10) | (15u << 2));
    field(p, base + 4u, 0x7ff, 0);
    field(p, base + 16u, 0x03ffffff, hs | (he << 13));
    field(p, base + 20u, 0x00ffffff, vs | (ve << 12));
    if (display) field(p, base + 24u, 0x03ffffff, hs | (hs << 13));
    field(p, base + 32u, 0x1fff, hs);
}

static void window(struct hdmi_vpp_timing_plan *p, uint32_t address,
                   uint32_t width, uint32_t height)
{
    field(p, address, 0x03ffffff, width << 13);
    field(p, address + 4u, 0x00ffffff, height << 12);
}

enum hdmi_display_result hdmi_vpp_plan_timing_rgb24(
    const struct hdmi_display_timing *t, enum hdmi_vpp_blanking blanking,
    struct hdmi_vpp_timing_plan *out)
{
    struct hdmi_display_timing_info info;
    struct hdmi_vpp_timing_plan p = {0};
    uint64_t ih, iv, hs, he, vs, ve, frst, pixels, words, line_bytes;
    uint32_t w, h, ctrl;
    size_t i;
    static const uint32_t bases[] = {CPCB_BETG, VOP_BETG};
    if (out == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    *out = p;
    STEP(hdmi_display_validate_timing(t, &info));
    if (t->flags != (HDMI_DISPLAY_PHSYNC | HDMI_DISPLAY_PVSYNC))
        return HDMI_DISPLAY_BAD_TIMING;
    w = t->hdisplay;
    h = t->vdisplay;
    ih = (uint64_t)t->htotal - 1u;
    iv = (uint64_t)t->vtotal - 1u;
    hs = t->hsync_start; he = t->hsync_end;
    vs = t->vsync_start; ve = t->vsync_end;
    switch (blanking) {
    case HDMI_VPP_BLANKING_FULL:
        break;
    case HDMI_VPP_BLANKING_FHD:
    case HDMI_VPP_BLANKING_PANEL:
        ih = (uint64_t)w + (blanking == HDMI_VPP_BLANKING_FHD ? 29u : 30u);
        iv = (uint64_t)h + (blanking == HDMI_VPP_BLANKING_FHD ? 5u : 6u);
        hs = (uint64_t)w + 10u; he = hs + 10u;
        vs = (uint64_t)h + 2u; ve = vs + 2u;
        break;
    case HDMI_VPP_BLANKING_UHD:
        ih = (uint64_t)w + 59u; iv = (uint64_t)h + 11u;
        hs = (uint64_t)w + 20u; he = hs + 20u;
        vs = (uint64_t)h + 4u; ve = vs + 4u;
        break;
    default:
        return HDMI_DISPLAY_BAD_ARGUMENT;
    }
    frst = (uint64_t)h + 7u;
    pixels = (uint64_t)w * h;
    line_bytes = (((uint64_t)w * 3u + 15u) / 16u) * 16u;
    words = (line_bytes / 16u) * h;
    /* Validate unpacked values before narrowing or shifting. The external
     * display TG always retains the exact requested totals and sync edges. */
    if (w > 8191u || h > 4095u || t->htotal > 8192u || t->vtotal > 4096u ||
        t->hsync_start > 8191u || t->hsync_end > 8191u ||
        t->vsync_start > 4095u || t->vsync_end > 4095u ||
        ih > 8191u || iv > 4095u || hs > 8191u || he > 8191u ||
        vs > 4095u || ve > 4095u || frst > 4095u ||
        (uint64_t)w + 20u > 65535u || (uint64_t)h + 6u > 65535u ||
        pixels > UINT32_MAX || words > UINT32_MAX || line_bytes > UINT32_MAX)
        return HDMI_DISPLAY_REGISTER_RANGE;
    p.pixels = (uint32_t)pixels;
    p.read_words = (uint32_t)words;
    p.read_line_bytes = (uint32_t)line_bytes;
    p.internal_htotal = (uint32_t)ih + 1u;
    p.internal_vtotal = (uint32_t)iv + 1u;

    /* RGB24 使用 pack=3，行刷新和 13 位行像素字段定义见 dolphin/vpp.h。 */
    ctrl = (5u << 17) | (w << 4) | ((w % 16u != 0) ? 8u : 0u) | 6u;
    field(&p, GFX_CTRL, 0x001ffffe, ctrl);
    field(&p, GFX_WORDS, UINT32_MAX, p.read_words);
    field(&p, GFX_PIXELS, UINT32_MAX, p.pixels);
    field(&p, GFX_DUMMY_TG + 4u, UINT32_MAX, ((w + 20u) << 16) | (h + 6u));
    field(&p, GFX_DUMMY_TG + 12u, UINT32_MAX, (7u << 16) | (w + 8u));
    field(&p, GFX_DUMMY_TG + 16u, UINT32_MAX, (7u << 16) | (w + 8u));
    field(&p, GFX_DUMMY_TG + 32u, UINT32_MAX, (2u << 16) | (h + 3u));
    field(&p, GFX_DUMMY_TG + 36u, UINT32_MAX, (2u << 16) | (h + 3u));
    tg(&p, CPCB_TG, (uint32_t)ih, (uint32_t)iv, (uint32_t)hs, (uint32_t)he,
       (uint32_t)vs, (uint32_t)ve, (uint32_t)frst, false);
    tg(&p, VOP_TG, (uint32_t)ih, (uint32_t)iv, (uint32_t)hs, (uint32_t)he,
       (uint32_t)vs, (uint32_t)ve, (uint32_t)frst, false);
    tg(&p, DISPLAY_TG, t->htotal - 1u, t->vtotal - 1u,
       t->hsync_start, t->hsync_end, t->vsync_start, t->vsync_end,
       (uint32_t)frst, true);
    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        /* 配置 GFX1 图层、裁剪和边框。 */
        window(&p, bases[i] + 0x18u, w, h);
        window(&p, bases[i] + 0x58u, w, h);
        window(&p, bases[i] + 0x38u, w, h);
    }
    window(&p, VOP_BETG, w, h);
    window(&p, VOP_BETG + 0x20u, w, h);
    window(&p, VOP_BETG + 0x40u, w, h);
    window(&p, CPCB_BETG, w, h);
    window(&p, CPCB_BETG + 0x20u, w, h);
    window(&p, CPCB_BETG + 0x40u, w, h);
    window(&p, DISPLAY_BETG, w, h);
    field(&p, DISPLAY_CTRL1, 2, 0); /* progressive; preserve SRAM/IRQ bits */
    field(&p, CPCB_PIXELS, UINT32_MAX, p.pixels);
    field(&p, VOP_PIXELS, UINT32_MAX, p.pixels);
    /* Opaque GFX1 overlay recipe from mode-720p, independent of resolution. */
    field(&p, CPCB_OVL + 0x2cu, 3, 2);
    field(&p, CPCB_OVL + 0x14u, 0xffff, 0x210);
    field(&p, CPCB_OVL + 4u, 0x0fffc000, 0x0fff0000);
    field(&p, CPCB_OVL, 4, 0);
    field(&p, CPCB_OVL + 0x30u, 4, 0);
    *out = p;
    return HDMI_DISPLAY_OK;
}

static enum hdmi_display_result read_field(
    const struct hdmi_vpp_timing_io *io, struct hdmi_vpp_timing_report *r,
    uint32_t address, uint32_t mask, uint32_t expected)
{
    r->last_address = address; r->last_mask = mask;
    r->expected = expected; r->observed = 0;
    r->callback_error = io->read32(io->opaque, address, &r->observed);
    if (r->callback_error != 0) return HDMI_DISPLAY_IO_ERROR;
    return HDMI_DISPLAY_OK;
}

enum hdmi_display_result hdmi_vpp_program_stopped_rgb24(
    const struct hdmi_display_timing *timing, enum hdmi_vpp_blanking blanking,
    const struct hdmi_vpp_timing_io *io, struct hdmi_vpp_timing_report *r)
{
    struct hdmi_vpp_timing_plan p;
    size_t i;
    static const struct { uint32_t address, mask; } stopped[] = {
        {CPCB_CTRL2, 0xbf}, {GFX_SCL, 0x0f}, {VOP_START, 3}
    };
    if (r == NULL) return HDMI_DISPLAY_BAD_ARGUMENT;
    *r = (struct hdmi_vpp_timing_report){0};
    if (io == NULL || io->read32 == NULL || io->write32 == NULL ||
        io->verify_quiesced == NULL)
        return HDMI_DISPLAY_BAD_ARGUMENT;
    STEP(hdmi_vpp_plan_timing_rgb24(timing, blanking, &p));
    r->stage = HDMI_VPP_STAGE_QUIESCENCE;
    r->callback_error = io->verify_quiesced(io->opaque);
    if (r->callback_error != 0) return HDMI_DISPLAY_IO_ERROR;
    r->quiescence_verified = true;
    r->stage = HDMI_VPP_STAGE_PREFLIGHT;
    for (i = 0; i < sizeof(stopped) / sizeof(stopped[0]); ++i) {
        STEP(read_field(io, r, stopped[i].address, stopped[i].mask, 0));
        if ((r->observed & r->last_mask) != 0) return HDMI_DISPLAY_OUTPUT_ACTIVE;
    }
    r->stage = HDMI_VPP_STAGE_PROGRAM;
    for (i = 0; i < p.count; ++i) {
        const struct hdmi_vpp_reg_field *f = &p.fields[i];
        uint32_t value;
        r->field_index = i;
        STEP(read_field(io, r, f->address, f->mask, f->value));
        value = (r->observed & ~f->mask) | f->value;
        ++r->writes_attempted;
        r->callback_error = io->write32(io->opaque, f->address, value);
        if (r->callback_error != 0) return HDMI_DISPLAY_IO_ERROR;
        ++r->writes_completed;
        STEP(read_field(io, r, f->address, f->mask, f->value));
        if ((r->observed & f->mask) != f->value)
            return HDMI_DISPLAY_READBACK_MISMATCH;
        ++r->writes_verified;
    }
    r->field_index = p.count;
    r->stage = HDMI_VPP_STAGE_PREPARED;
    return HDMI_DISPLAY_OK;
}

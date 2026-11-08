/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_VPP_TIMING_H
#define HDMI_DEV_VPP_TIMING_H

#include "display-mode.h"

/* CPCB/VOP 内部 TG 的消隐配置：720p 使用 FULL，1920×1080/1125 行使用 FHD，3840×2160/2250 行使用 UHD，2560×1440 CVT-RB 条目使用 PANEL。 */
enum hdmi_vpp_blanking {
    HDMI_VPP_BLANKING_FULL = 0,   /* totals and sync edges from timing */
    HDMI_VPP_BLANKING_FHD,        /* encoded totals active + 30/6 - 1 */
    HDMI_VPP_BLANKING_UHD,        /* encoded totals active + 60/12 - 1 */
    HDMI_VPP_BLANKING_PANEL       /* encoded totals active + 30/6 */
};

#define HDMI_VPP_TIMING_FIELDS 70u
struct hdmi_vpp_reg_field {
    uint32_t address; /* 物理字节地址。 */
    uint32_t mask, value;
};
struct hdmi_vpp_timing_plan {
    uint32_t pixels;
    uint32_t read_words;       /* total 128-bit words, rounded per row */
    uint32_t read_line_bytes;  /* ceil(width * 3 / 16) * 16 */
    uint32_t internal_htotal, internal_vtotal; /* decoded counts */
    size_t count;
    struct hdmi_vpp_reg_field fields[HDMI_VPP_TIMING_FIELDS];
};

/* 计算 GFX1→CPCB0→VOP→显示 TG 的 1:1 RGB24 配置，要求正 H/V 极性。按公开行读取规则生成 read_line_bytes；调用方据此准备 DMA 行数据和填充。TG_PRG_FT 编码像素数的低 8 位。 */
enum hdmi_display_result hdmi_vpp_plan_timing_rgb24(
    const struct hdmi_display_timing *timing, enum hdmi_vpp_blanking blanking,
    struct hdmi_vpp_timing_plan *plan);

struct hdmi_vpp_timing_io {
    void *opaque;
    int (*read32)(void *, uint32_t address, uint32_t *value);
    int (*write32)(void *, uint32_t address, uint32_t value);
    /* 回调检查当前硬件停止状态，成功返回 0；错误写入 callback_error 并返回 IO_ERROR。 */
    int (*verify_quiesced)(void *);
};
enum hdmi_vpp_timing_stage {
    HDMI_VPP_STAGE_VALIDATE = 0,
    HDMI_VPP_STAGE_QUIESCENCE,
    HDMI_VPP_STAGE_PREFLIGHT,
    HDMI_VPP_STAGE_PROGRAM,
    HDMI_VPP_STAGE_PREPARED
};
struct hdmi_vpp_timing_report {
    enum hdmi_vpp_timing_stage stage;
    int callback_error;
    size_t field_index;
    uint32_t last_address, last_mask, expected, observed;
    uint32_t writes_attempted, writes_completed, writes_verified;
    bool quiescence_verified;
};

/* 持有显示资源并完成复位、SRAM、时钟及路由准备后，配置像素几何。写入后读回，成功返回 PREPARED；首个错误即返回，并记录已尝试、完成和核对的写入数。 */
enum hdmi_display_result hdmi_vpp_program_stopped_rgb24(
    const struct hdmi_display_timing *timing, enum hdmi_vpp_blanking blanking,
    const struct hdmi_vpp_timing_io *io, struct hdmi_vpp_timing_report *report);

#endif

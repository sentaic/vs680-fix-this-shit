/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_DISPLAY_MODE_H
#define HDMI_DEV_DISPLAY_MODE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 显示模式和寄存器参数计算。 */
enum hdmi_display_result {
    HDMI_DISPLAY_OK = 0,
    HDMI_DISPLAY_BAD_ARGUMENT,
    HDMI_DISPLAY_BAD_TIMING,
    HDMI_DISPLAY_UNSUPPORTED_SCAN,
    HDMI_DISPLAY_VIC_MISMATCH,
    HDMI_DISPLAY_REGISTER_RANGE,
    HDMI_DISPLAY_CLOCK_RANGE,
    HDMI_DISPLAY_TIMING_REQUIRED,
    HDMI_DISPLAY_CAPABILITIES_REQUIRED,
    HDMI_DISPLAY_SINK_MODE_UNLISTED,
    HDMI_DISPLAY_LINK_UNSUPPORTED,
    HDMI_DISPLAY_LINK_RATE,
    HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION,
    HDMI_DISPLAY_PHY_UNAVAILABLE,
    HDMI_DISPLAY_CLOCK_ADAPTER_REQUIRED,
    HDMI_DISPLAY_OUTPUT_ACTIVE,
    HDMI_DISPLAY_IO_ERROR,
    HDMI_DISPLAY_READBACK_MISMATCH,
    HDMI_DISPLAY_TIMEOUT
};

#define HDMI_DISPLAY_PHSYNC    UINT32_C(1)
#define HDMI_DISPLAY_PVSYNC    UINT32_C(2)
#define HDMI_DISPLAY_INTERLACE UINT32_C(4)
#define HDMI_DISPLAY_DBLSCAN   UINT32_C(8)

/* 时序坐标满足 display <= sync_start < sync_end <= total；同步极性位清零表示负极性，时钟单位为 Hz。自定义时序使用 cta_vic=0，标准 VIC 按对应时序检查。 */
struct hdmi_display_timing {
    uint64_t pixel_clock_hz;
    uint32_t hdisplay, hsync_start, hsync_end, htotal;
    uint32_t vdisplay, vsync_start, vsync_end, vtotal;
    uint32_t flags;
    uint32_t cta_vic;
};

struct hdmi_display_timing_info {
    uint32_t hblank, hfront, hsync, hback;
    uint32_t vblank, vfront, vsync, vback;
    uint64_t refresh_num, refresh_den; /* exact Hz, reduced fraction */
};

enum hdmi_display_preset {
    HDMI_DISPLAY_720P60,
    HDMI_DISPLAY_1080P60,
    HDMI_DISPLAY_1080P120,
    HDMI_DISPLAY_1080P240,
    HDMI_DISPLAY_1440P60,
    HDMI_DISPLAY_1440P144,
    HDMI_DISPLAY_2160P60
};

/* 1080p240 和 1440p144 返回 TIMING_REQUIRED，调用方需提供接收端的详细时序。 */
enum hdmi_display_result hdmi_display_get_preset(
    enum hdmi_display_preset preset, struct hdmi_display_timing *out);
enum hdmi_display_result hdmi_display_validate_timing(
    const struct hdmi_display_timing *timing,
    struct hdmi_display_timing_info *out);

/* VS680 VPLL0 使用 25 MHz 参考时钟和 5 MHz PFD；VCO 范围 1.2..6 GHz，输出范围 20 MHz..3 GHz。返回编码后的分频字段及 actual_num/actual_den 表示的输出频率。 */
struct hdmi_display_clock_plan {
    uint64_t target_hz, actual_num, actual_den;
    uint64_t vco_target_hz;
    uint32_t reference_hz;
    uint32_t divr, divfi, divff, divq, range;
};
enum hdmi_display_result hdmi_display_plan_clock(
    uint64_t pixel_clock_hz, struct hdmi_display_clock_plan *out);

struct hdmi_display_link_caps {
    uint64_t source_max_tmds_hz;
    uint64_t sink_max_tmds_hz;
    bool sink_is_hdmi;
    bool sink_timing_listed; /* 接收端 EDID 或模式列表中的时序信息。 */
    bool source_scdc;
    bool sink_scdc;
};
struct hdmi_display_link_plan {
    uint64_t tmds_character_hz;
    uint64_t lane_bit_rate_bps; /* 10 bits per character, each of 3 lanes */
    uint64_t total_bit_rate_bps;
    bool requires_scrambling;
    bool requires_clock_ratio_40;
};
/* 计算 RGB 8 位、零像素重复模式的链路要求；超过 340 MHz 时返回 NEEDS_SCDC_NEGOTIATION。 */
enum hdmi_display_result hdmi_display_plan_link_rgb8(
    const struct hdmi_display_timing *timing,
    const struct hdmi_display_link_caps *caps,
    struct hdmi_display_link_plan *out);

#define HDMI_DISPLAY_CORE_FIELDS 48u
struct hdmi_display_reg_field {
    uint32_t offset; /* Synopsys 寄存器的字节偏移。 */
    uint32_t mask, value;
};
struct hdmi_display_core_plan {
    size_t count;
    struct hdmi_display_reg_field fields[HDMI_DISPLAY_CORE_FIELDS];
};
/* 生成 FC 时序、RGB8 打包器、采样器和 AVI 信息包配置。 */
enum hdmi_display_result hdmi_display_plan_core_rgb8(
    const struct hdmi_display_timing *timing,
    struct hdmi_display_core_plan *out);

#define HDMI_DISPLAY_PHY_REGS 7u
struct hdmi_display_phy_reg { uint16_t reg, value; };
struct hdmi_display_phy_plan {
    uint64_t target_pixel_clock_hz, table_clock_hz;
    struct hdmi_display_phy_reg regs[HDMI_DISPLAY_PHY_REGS];
};
/* 保留 RGB8 预设参数；475.2..594 MHz 选择高频段实验 PHY 配置，241.5 MHz 使用既有参数别名。接收端能力和 SCDC 由后续步骤检查。 */
enum hdmi_display_result hdmi_display_plan_phy_rgb8(
    uint64_t pixel_clock_hz, struct hdmi_display_phy_plan *out);

/* 回调按物理字节地址顺序访问 MMIO，成功返回 0；configure_clock 完成 VPLL、路由和门控设置，错误保存在 report。 */
struct hdmi_display_io {
    void *opaque;
    int (*read32)(void *, uint32_t address, uint32_t *value);
    int (*write32)(void *, uint32_t address, uint32_t value);
    int (*wait_ms)(void *, uint32_t milliseconds);
    int (*configure_clock)(void *, const struct hdmi_display_clock_plan *);
};
enum hdmi_display_stage {
    HDMI_DISPLAY_STAGE_VALIDATE = 0,
    HDMI_DISPLAY_STAGE_PREFLIGHT,
    HDMI_DISPLAY_STAGE_CLOCK,
    HDMI_DISPLAY_STAGE_CORE,
    HDMI_DISPLAY_STAGE_PHY,
    HDMI_DISPLAY_STAGE_PREPARED
};
struct hdmi_display_report {
    enum hdmi_display_stage stage;
    int callback_error;
    uint32_t last_address;
    uint32_t writes_completed;
    uint32_t last_phy_i2c_status;
    bool clock_adapter_completed;
    bool requires_scdc;
};

/* 在输出停止状态下配置 <=340 MHz 模式。先检查 PHY、时钟和加扰状态，再写入并读回核心及 PHY 配置；成功返回 PREPARED。配置中途失败时在 report 中保留进度，供调用方恢复。 */
enum hdmi_display_result hdmi_display_program_stopped_rgb8(
    const struct hdmi_display_timing *timing,
    const struct hdmi_display_link_caps *caps,
    const struct hdmi_display_io *io,
    struct hdmi_display_report *report);

const char *hdmi_display_result_name(enum hdmi_display_result result);

/* 七个 PHY 寄存器的读取共用一个截止时间，时间回调使用单调时钟。 */
struct hdmi_display_deadline_io {
    void *opaque;
    int (*now_us)(void *,uint64_t *);
    int (*wait_us)(void *,uint32_t,uint32_t);
};
enum hdmi_display_result hdmi_display_verify_phy_rgb8(uint64_t hz,
    const struct hdmi_display_io *,const struct hdmi_display_deadline_io *,
    uint32_t timeout_us,struct hdmi_display_report *);
/* 返回准备结果和 SCDC 要求，由调用方在启动前完成协商并检查启动时间窗口。 */
enum hdmi_display_result hdmi_display_prepare_stopped_rgb8(
    const struct hdmi_display_timing *,const struct hdmi_display_link_caps *,
    const struct hdmi_display_io *,struct hdmi_display_report *);

#endif

/* SPDX-License-Identifier: GPL-2.0 */
/* 寄存器和操作顺序来源：
 * Linux v6.12:
 * https://github.com/torvalds/linux/blob/v6.12/drivers/gpu/drm/display/drm_scdc_helper.c
 * https://github.com/torvalds/linux/blob/v6.12/drivers/gpu/drm/bridge/synopsys/dw-hdmi.c
 *   drm_scdc_set_* 的 0x20 读改写、1..100 ms 时间要求；
 *   hdmi_av_composer 的 keepout -> TMDS reset request -> scrambler 顺序。
 * Synaptics/Synopsys GPL-2.0，2025：
 * https://github.com/synaptics-astra/tee-optee_dev/tree/
 * e397232786bfee0599dc9d51eb3ca2d6db008eda/ta/display
 *   ta/vpp/g7/vppdrv/source/BE/hdmi/vendor/synopsys/api/src/
 *   scdc/{scdc,scrambling}.c; core 下的 frame_composer/main_controller 寄存器头；
 *   phy/phy_316.c: PHY 编程后保持断电，随后设置加扰，再 powerUp。
 *   vendor_interface/synopsys/vpp_be_hdmitx.c: 初始化设置 HDCP keepout。
 * 配置采用读改写保留其他位，并按 1..100 ms 规则计算启动窗口。
 */
#include "scdc-link.h"

#include <stddef.h>

#define BASE UINT32_C(0xf7440000)
#define FC_INVIDCONF UINT32_C(0x4000)
#define FC_SCRAMBLER_CTRL UINT32_C(0x4384)
#define PHY_CONF0 UINT32_C(0xc000)
#define PHY_STAT0 UINT32_C(0xc010)
#define MC_CLKDIS UINT32_C(0x10004)
#define MC_SWRSTZREQ UINT32_C(0x10008)
#define SINK_VERSION UINT8_C(0x01)
#define SOURCE_VERSION UINT8_C(0x02)
#define TMDS_CONFIG UINT8_C(0x20)
#define SCRAMBLING UINT8_C(0x01)
#define RATIO_40 UINT8_C(0x02)
#define TRY(expr) do { enum hdmi_scdc_result r_ = (expr); \
    if (r_ != HDMI_SCDC_OK) return r_; } while (0)

struct context {
    const struct hdmi_scdc_io *io;
    struct hdmi_scdc_report *report;
};

static enum hdmi_scdc_result callback(struct context *c, int error)
{
    if (error == 0) return HDMI_SCDC_OK;
    c->report->callback_error = error;
    return HDMI_SCDC_IO_ERROR;
}

static enum hdmi_scdc_result validate(const struct hdmi_scdc_caps *caps,
                                    const struct hdmi_scdc_io *io,
                                    uint64_t hz, bool recovery)
{
    enum hdmi_scdc_support support[6];
    size_t i;
    if (caps == NULL || io == NULL || io->read32 == NULL ||
        io->write32 == NULL || io->ddc_readb == NULL || io->ddc_writeb == NULL)
        return HDMI_SCDC_BAD_ARGUMENT;
    if (!recovery && (io->verify_source_clock == NULL || io->now_us == NULL ||
                      io->wait_us == NULL || hz == 0))
        return HDMI_SCDC_BAD_ARGUMENT;
    support[0] = caps->source_scdc_v1;
    support[1] = caps->source_scrambling;
    support[2] = caps->source_ratio_40;
    support[3] = caps->sink_hdmi;
    support[4] = caps->sink_scdc;
    support[5] = caps->sink_mode;
    for (i = 0; i < (recovery ? 5u : 6u); ++i) {
        if (support[i] == HDMI_SCDC_UNKNOWN)
            return HDMI_SCDC_CAPABILITIES_REQUIRED;
        if (support[i] == HDMI_SCDC_NOT_SUPPORTED)
            return HDMI_SCDC_UNSUPPORTED;
        if (support[i] != HDMI_SCDC_SUPPORTED)
            return HDMI_SCDC_BAD_ARGUMENT;
    }
    if (!recovery) {
        if (caps->source_max_tmds_hz == 0 || caps->sink_max_tmds_hz == 0)
            return HDMI_SCDC_CAPABILITIES_REQUIRED;
        if (hz > UINT64_C(600000000) || hz > caps->source_max_tmds_hz ||
            hz > caps->sink_max_tmds_hz)
            return HDMI_SCDC_RATE_UNSUPPORTED;
    }
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result read32(struct context *c, uint32_t offset,
                                   uint32_t *value)
{
    c->report->last_address = BASE + offset;
    return callback(c, c->io->read32(c->io->opaque, BASE + offset, value));
}

static enum hdmi_scdc_result write32(struct context *c, uint32_t offset,
                                    uint32_t value, uint32_t change)
{
    c->report->last_address = BASE + offset;
    c->report->changes_attempted |= change;
    ++c->report->writes_attempted;
    TRY(callback(c, c->io->write32(c->io->opaque, BASE + offset, value)));
    ++c->report->writes_completed;
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result update32(struct context *c, uint32_t offset,
                                     uint32_t mask, uint32_t value,
                                     uint32_t change)
{
    uint32_t before, after, desired;
    TRY(read32(c, offset, &before));
    desired = (before & ~mask) | (value & mask);
    TRY(write32(c, offset, desired, change));
    TRY(read32(c, offset, &after));
    return after == desired ? HDMI_SCDC_OK : HDMI_SCDC_READBACK_MISMATCH;
}

static enum hdmi_scdc_result readb(struct context *c, uint8_t offset,
                                  uint8_t *value)
{
    c->report->last_address = offset;
    return callback(c, c->io->ddc_readb(c->io->opaque, HDMI_SCDC_DDC_ADDRESS,
        offset, value, HDMI_SCDC_IO_TIMEOUT_US));
}

static enum hdmi_scdc_result writeb(struct context *c, uint8_t offset,
                                   uint8_t value)
{
    c->report->last_address = offset;
    if (offset == TMDS_CONFIG) c->report->sink_config_after_valid = false;
    c->report->changes_attempted |= offset == SOURCE_VERSION ?
        HDMI_SCDC_CHANGED_VERSION : HDMI_SCDC_CHANGED_SINK_CONFIG;
    ++c->report->writes_attempted;
    TRY(callback(c, c->io->ddc_writeb(c->io->opaque, HDMI_SCDC_DDC_ADDRESS,
        offset, value, HDMI_SCDC_IO_TIMEOUT_US)));
    ++c->report->writes_completed;
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result readback_config(struct context *c, uint8_t expected)
{
    uint8_t value;
    c->report->sink_config_after_valid = false;
    TRY(readb(c, TMDS_CONFIG, &value));
    c->report->sink_config_after = value;
    c->report->sink_config_after_valid = true;
    return value == expected ? HDMI_SCDC_OK : HDMI_SCDC_READBACK_MISMATCH;
}

static enum hdmi_scdc_result stopped(struct context *c)
{
    uint32_t config, status, clocks;
    TRY(read32(c, PHY_CONF0, &config));
    TRY(read32(c, PHY_STAT0, &status));
    TRY(read32(c, MC_CLKDIS, &clocks));
    if ((config & 0x18u) != 0x10u || (status & 1u) != 0 ||
        (clocks & 0x5fu) != 0x5fu)
        return HDMI_SCDC_OUTPUT_ACTIVE;
    if ((status & 2u) == 0) return HDMI_SCDC_DISCONNECTED;
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result sink_preflight(struct context *c)
{
    TRY(readb(c, SINK_VERSION, &c->report->sink_version));
    if (c->report->sink_version == 0) return HDMI_SCDC_VERSION_UNSUPPORTED;
    TRY(readb(c, TMDS_CONFIG, &c->report->sink_config_before));
    c->report->sink_config_before_valid = true;
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result source_reset(struct context *c)
{
    uint32_t value;
    TRY(read32(c, MC_SWRSTZREQ, &value));
    /* 复位请求低有效：TMDS 位写 0，其余复位位写 1，高位保留。 */
    return write32(c, MC_SWRSTZREQ, (value & ~UINT32_C(0xff)) | 0xfdu,
                   HDMI_SCDC_CHANGED_RESET);
}

static enum hdmi_scdc_result source_scrambling(struct context *c, bool enable)
{
    if (enable) {
        TRY(update32(c, FC_INVIDCONF, 0x80, 0x80,
                     HDMI_SCDC_CHANGED_KEEPOUT));
        TRY(source_reset(c));
        TRY(update32(c, FC_SCRAMBLER_CTRL, 1, 1,
                     HDMI_SCDC_CHANGED_SCRAMBLER));
    } else {
        TRY(update32(c, FC_SCRAMBLER_CTRL, 1, 0,
                     HDMI_SCDC_CHANGED_SCRAMBLER));
        TRY(source_reset(c));
    }
    return HDMI_SCDC_OK;
}

static enum hdmi_scdc_result now(struct context *c, uint64_t *value)
{
    return callback(c, c->io->now_us(c->io->opaque, value));
}

bool hdmi_scdc_resume_window_contains(const struct hdmi_scdc_report *r,
                                    uint64_t time_us, uint32_t launch_budget_us)
{
    return r != NULL && r->configuration_matches && r->resume_window_valid &&
        time_us >= r->resume_not_before_us && time_us < r->resume_before_us &&
        (uint64_t)launch_budget_us < r->resume_before_us - time_us;
}

enum hdmi_scdc_result hdmi_scdc_configure_stopped_rgb8(
    uint64_t hz, const struct hdmi_scdc_caps *caps,
    const struct hdmi_scdc_io *io, struct hdmi_scdc_report *r)
{
    struct context c = {io, r};
    uint8_t value, desired;
    uint32_t local;
    uint64_t time_us;
    bool high = hz > UINT64_C(340000000);
    if (r == NULL) return HDMI_SCDC_BAD_ARGUMENT;
    *r = (struct hdmi_scdc_report){0};
    TRY(validate(caps, io, hz, false));
    r->stage = HDMI_SCDC_STAGE_PREFLIGHT;
    TRY(stopped(&c));
    TRY(callback(&c, io->verify_source_clock(io->opaque, hz, high,
                                            HDMI_SCDC_IO_TIMEOUT_US)));
    r->source_clock_verified = true;
    TRY(sink_preflight(&c));
    /* 写入前检查执行环境的计时回调。 */
    TRY(now(&c, &time_us));
    if (time_us > UINT64_MAX - HDMI_SCDC_RESUME_MAX_US)
        return HDMI_SCDC_TIME_INVALID;

    r->stage = HDMI_SCDC_STAGE_SOURCE_VERSION;
    TRY(writeb(&c, SOURCE_VERSION, 1));
    TRY(readb(&c, SOURCE_VERSION, &value));
    if (value != 1) return HDMI_SCDC_READBACK_MISMATCH;

    /* 启用时先设置接收端；关闭时先关闭本地，遵循 dw-hdmi 顺序。 */
    if (!high) {
        r->stage = HDMI_SCDC_STAGE_SOURCE_SCRAMBLING;
        TRY(source_scrambling(&c, false));
    }
    r->stage = HDMI_SCDC_STAGE_SINK_SCRAMBLING;
    TRY(readb(&c, TMDS_CONFIG, &value));
    desired = (uint8_t)((value & (uint8_t)~SCRAMBLING) |
                       (high ? SCRAMBLING : 0));
    TRY(writeb(&c, TMDS_CONFIG, desired));
    TRY(readback_config(&c, desired));
    if (high) {
        r->stage = HDMI_SCDC_STAGE_SOURCE_SCRAMBLING;
        TRY(source_scrambling(&c, true));
    }
    TRY(stopped(&c));

    r->stage = HDMI_SCDC_STAGE_SINK_RATIO;
    TRY(readb(&c, TMDS_CONFIG, &value));
    if ((value & SCRAMBLING) != (high ? SCRAMBLING : 0))
        return HDMI_SCDC_READBACK_MISMATCH;
    desired = (uint8_t)((value & (uint8_t)~RATIO_40) | (high ? RATIO_40 : 0));
    TRY(now(&c, &r->ratio_write_started_us));
    if (r->ratio_write_started_us < time_us ||
        r->ratio_write_started_us > UINT64_MAX - HDMI_SCDC_RESUME_MAX_US)
        return HDMI_SCDC_TIME_INVALID;
    /* 即使比率位相同也写入，和 Linux helper 一致；每次调用重新计时。 */
    TRY(writeb(&c, TMDS_CONFIG, desired));
    TRY(now(&c, &r->ratio_write_finished_us));
    if (r->ratio_write_finished_us < r->ratio_write_started_us ||
        r->ratio_write_finished_us > UINT64_MAX - HDMI_SCDC_RESUME_MIN_US)
        return HDMI_SCDC_TIME_INVALID;
    r->resume_not_before_us = r->ratio_write_finished_us + HDMI_SCDC_RESUME_MIN_US;
    r->resume_before_us = r->ratio_write_started_us + HDMI_SCDC_RESUME_MAX_US;
    TRY(readback_config(&c, desired));
    r->stage = HDMI_SCDC_STAGE_SETTLE;
    TRY(callback(&c, io->wait_us(io->opaque, (uint32_t)HDMI_SCDC_RESUME_MIN_US,
                                 HDMI_SCDC_IO_TIMEOUT_US)));
    TRY(stopped(&c));
    TRY(read32(&c, FC_SCRAMBLER_CTRL, &local));
    if ((local & 1u) != (high ? 1u : 0u)) return HDMI_SCDC_READBACK_MISMATCH;
    TRY(now(&c, &time_us));
    if (time_us < r->resume_not_before_us) return HDMI_SCDC_TIME_INVALID;
    if (time_us >= r->resume_before_us) return HDMI_SCDC_RESUME_WINDOW_EXPIRED;
    r->configuration_matches = true;
    r->resume_window_valid = true;
    r->stage = HDMI_SCDC_STAGE_CONFIGURED;
    return HDMI_SCDC_OK;
}

enum hdmi_scdc_result hdmi_scdc_recover_stopped(
    const struct hdmi_scdc_caps *caps, const struct hdmi_scdc_io *io,
    struct hdmi_scdc_report *r)
{
    struct context c = {io, r};
    uint8_t value;
    if (r == NULL) return HDMI_SCDC_BAD_ARGUMENT;
    *r = (struct hdmi_scdc_report){0};
    TRY(validate(caps, io, 0, true));
    r->stage = HDMI_SCDC_STAGE_PREFLIGHT;
    TRY(stopped(&c));
    TRY(sink_preflight(&c));
    r->stage = HDMI_SCDC_STAGE_RECOVERY;
    TRY(source_scrambling(&c, false));
    TRY(readb(&c, TMDS_CONFIG, &value));
    value &= (uint8_t)~(SCRAMBLING | RATIO_40);
    TRY(writeb(&c, TMDS_CONFIG, value));
    TRY(readback_config(&c, value));
    TRY(stopped(&c));
    r->stage = HDMI_SCDC_STAGE_RECOVERED;
    return HDMI_SCDC_OK;
}

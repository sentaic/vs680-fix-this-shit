/* SPDX-License-Identifier: GPL-2.0 */
#include "tx-ddc-runtime.h"
#include <stddef.h>
#include <string.h>
#define SNPS UINT32_C(0xf7440000)
#define PHY_CONF UINT32_C(0xc000)
#define PHY_STAT UINT32_C(0xc010)
#define CLKDIS UINT32_C(0x10004)
#define STATUS UINT32_C(0x414)
#define INT_MASK UINT32_C(0x1f814)
#define CTLINT UINT32_C(0x1f818)
#define DIV UINT32_C(0x1f81c)
#define RESET UINT32_C(0x1f824)
#define SDA UINT32_C(0x1f84c)
#define AUTO UINT32_C(0x1f850)
struct field { uint32_t offset, mask, value; };
/* 25 MHz SFR 对应标准模式 SS 高/低计数 171/190、FS 2/4、SDA hold 22。参数来自公开 i2cm.c/i2cm.h；IH_I2CM_STAT0 的字节偏移为 0x414。 */
static const struct field config[] = {
    { INT_MASK, 0x04, 0x04 }, { CTLINT, 0x44, 0x44 },
    { DIV, 0x08, 0 }, { 0x1f82c, 0xff, 0 },
    { 0x1f830, 0xff, 171 }, { 0x1f834, 0xff, 0 },
    { 0x1f838, 0xff, 190 }, { 0x1f83c, 0xff, 0 },
    { 0x1f840, 0xff, 2 }, { 0x1f844, 0xff, 0 },
    { 0x1f848, 0xff, 4 }, { SDA, 0xff, 22 }, { AUTO, 0x31, 0 }
};
static int check_sfr(struct hdmi_tx_ddc_runtime *r)
{
    uint64_t hz = 0;
    if (!r || !r->query_sfr ||
        r->query_sfr(r->sfr_opaque, &hz) != 0 ||
        hz != UINT64_C(25000000) ||
        (r->sfr_hz && r->sfr_hz != hz))
        return HDMI_TX_SCDC_UNSUPPORTED;
    r->sfr_hz = hz;
    return 0;
}
static int io_read(void *opaque, uint32_t physical, uint32_t *value)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    TEE_Result e;
    if (!r || !value || !r->io.read32) return HDMI_TX_SCDC_EINVAL;
    e = r->io.read32(r->io.opaque, physical, value);
    r->last_result = e;
    return e == TEE_SUCCESS ? 0 : HDMI_TX_SCDC_IO;
}
/* 保留从硬件采样到的时序计数。 */
static int current_fields(struct hdmi_tx_ddc_runtime *r, uint32_t *capture)
{
    uint32_t values[13], value;
    size_t i;
    int e;
    if (!r->verify_owner || r->verify_owner(r->owner_opaque))
        return HDMI_TX_SCDC_BUSY;
    /* SDK 定义：关闭 HDCP 时钟、接收端检测和 ESM 的 DDC 驱动。 */
    e = io_read(r, SNPS + CLKDIS, &value);
    if (e) return e;
    if (!(value & 0x40u)) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + 0x14000u, &value);
    if (e) return e;
    if (value & 4u) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + 0x1e410u, &value);
    if (e) return e;
    if ((value & 6u) != 2u) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + 0x410u, &value);
    if (e) return e;
    if (value & 1u) return HDMI_TX_SCDC_BUSY;
    for (i = 0; i < 13; ++i) {
        e = io_read(r, SNPS + config[i].offset, &value);
        if (e) return e;
        values[i] = value & (i == 12 ? 0x31u : 0xffu);
        if (!capture && values[i] != r->current_config[i])
            return HDMI_TX_SCDC_IO;
    }
    if (!(values[3] | values[4]) || !(values[5] | values[6]) ||
        !(values[7] | values[8]) || !(values[9] | values[10]) ||
        !values[11] || values[12]) return HDMI_TX_SCDC_UNSUPPORTED;
    if (capture) memcpy(capture, values, sizeof(values));
    return 0;
}
static int check_access(struct hdmi_tx_ddc_runtime *r)
{
    return r->current_config_mode ? current_fields(r, NULL) : check_sfr(r);
}
static int current_write_allowed(uint32_t physical)
{
    size_t i;
    if (physical == SNPS + RESET) return 0;
    for (i = 0; i < 13; ++i)
        if (physical == SNPS + config[i].offset) return 0;
    return 1;
}
static int io_write(void *opaque, uint32_t physical, uint32_t value)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    TEE_Result e;
    if (!r || !r->io.write32) return HDMI_TX_SCDC_EINVAL;
    if (r->current_config_mode && !current_write_allowed(physical))
        return HDMI_TX_SCDC_UNSUPPORTED;
    {
        int access = check_access(r);
        if (access) return access;
    }
    e = r->io.write32(r->io.opaque, physical, value);
    r->last_result = e;
    return e == TEE_SUCCESS ? 0 : HDMI_TX_SCDC_IO;
}
static int now_us(void *opaque, uint64_t *out)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    TEE_Time t;
    uint64_t value;
    if (!r || !out) return HDMI_TX_SCDC_EINVAL;
    TEE_GetSystemTime(&t);
    if (t.millis >= 1000u) return HDMI_TX_SCDC_TIME;
    value = (uint64_t)t.seconds * UINT64_C(1000000) +
            (uint64_t)t.millis * UINT64_C(1000);
    if (r->time_valid && value < r->last_time_us)
        return HDMI_TX_SCDC_TIME;
    r->last_time_us = value;
    r->time_valid = 1;
    *out = value;
    return 0;
}
static int wait_us(void *opaque, uint32_t duration, uint32_t timeout)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    uint64_t before, after;
    uint32_t ms;
    int e;
    TEE_Result result;
    if (!r || !r->io.wait_ms || !duration || duration > timeout ||
        timeout > HDMI_SCDC_IO_TIMEOUT_US) return HDMI_TX_SCDC_EINVAL;
    ms = duration / 1000u + (duration % 1000u != 0u);
    if ((uint64_t)ms * 1000u > timeout) return HDMI_TX_SCDC_TIMEOUT;
    e = now_us(r, &before);
    if (e) return e;
    result = r->io.wait_ms(r->io.opaque, ms);
    r->last_result = result;
    if (result != TEE_SUCCESS) return HDMI_TX_SCDC_IO;
    e = now_us(r, &after);
    if (e) return e;
    if (after - before < duration) return HDMI_TX_SCDC_TIME;
    return after - before > timeout ? HDMI_TX_SCDC_TIMEOUT : 0;
}
static int stopped(struct hdmi_tx_ddc_runtime *r)
{
    uint32_t value;
    int e = io_read(r, SNPS + PHY_CONF, &value);
    if (e) return e;
    if ((value & 0x18u) != 0x10u) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + CLKDIS, &value);
    if (e) return e;
    /* TMDS clock disable bit1. Other clocks may be configured by INIT. */
    if (!(value & 2u)) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + PHY_STAT, &value);
    if (e) return e;
    if (!(value & 2u)) return HDMI_TX_SCDC_DISCONNECTED;
    if (value & 1u) return HDMI_TX_SCDC_BUSY;
    e = io_read(r, SNPS + AUTO, &value);
    if (e) return e;
    /* 自动轮询启用时返回错误，避免它在配置期间发起事务。 */
    return value & 0x31u ? HDMI_TX_SCDC_BUSY : 0;
}
static int check_fields(struct hdmi_tx_ddc_runtime *r)
{
    uint32_t value;
    size_t i;
    int e;
    if (r->current_config_mode) {
        e = current_fields(r, NULL);
        if (e) return e;
    }
    for (i = 0; !r->current_config_mode &&
         i < sizeof(config) / sizeof(config[0]); ++i) {
        e = io_read(r, SNPS + config[i].offset, &value);
        if (e) return e;
        if ((value & config[i].mask) != config[i].value)
            return HDMI_TX_SCDC_IO;
    }
    e = io_read(r, SNPS + RESET, &value);
    if (e) return e;
    if (!(value & 1u)) return HDMI_TX_SCDC_UNSUPPORTED;
    e = io_read(r, SNPS + STATUS, &value);
    if (e) return e;
    /* Preserve READREQ; automatic polling is separately forbidden. */
    return value & 3u ? HDMI_TX_SCDC_BUSY : 0;
}
static int ready(void *opaque, uint32_t timeout)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    uint64_t before, after;
    int e;
    if (!r || !timeout || timeout > HDMI_SCDC_IO_TIMEOUT_US)
        return HDMI_TX_SCDC_EINVAL;
    if (!r->initialized ||
        (!r->current_config_mode && (!r->reset_issued || !r->config_issued)) ||
        r->failed || r->adapter.active || r->adapter.faulted)
        return HDMI_TX_SCDC_BUSY;
    e = check_access(r);
    if (!e) e = now_us(r, &before);
    if (!e) e = stopped(r);
    if (!e) e = check_fields(r);
    if (!e) e = now_us(r, &after);
    if (!e && after - before >= timeout) e = HDMI_TX_SCDC_TIMEOUT;
    return e;
}
static int source_clock(void *opaque, uint64_t hz, bool ratio, uint32_t timeout)
{
    struct hdmi_tx_ddc_runtime *r = opaque;
    int access;
    if (!r || !r->verify_source_clock) return HDMI_TX_SCDC_UNSUPPORTED;
    access = check_access(r);
    if (access) return access;
    return r->verify_source_clock(r->source_opaque, hz, ratio, timeout);
}
static int configure(struct hdmi_tx_ddc_runtime *r, uint32_t timeout)
{
    uint64_t started, now;
    uint32_t value;
    unsigned poll;
    size_t i;
    int e;
    if (!timeout || timeout > HDMI_SCDC_IO_TIMEOUT_US)
        return HDMI_TX_SCDC_EINVAL;
    e = check_sfr(r);
    if (!e) e = now_us(r, &started);
    if (!e) e = stopped(r);
    if (e) return e;
    r->initialized = 0;
    e = io_read(r, SNPS + RESET, &value);
    if (e) return e;
    r->reset_issued = 1; /* Retain even when a write reports failure. */
    e = io_write(r, SNPS + RESET, value & ~1u);
    if (!e) e = io_write(r, SNPS + STATUS, 3u);
    if (e) return e;
    /* 轮询复位请求的硬件释放状态，使用设置阶段的截止时间。 */
    for (poll = 0; poll < 20u; ++poll) {
        e = io_read(r, SNPS + RESET, &value);
        if (e) return e;
        if (value & 1u) break;
        e = now_us(r, &now);
        if (e) return e;
        if (now - started >= timeout ||
            timeout - (now - started) < 1000u)
            return HDMI_TX_SCDC_UNSUPPORTED;
        e = wait_us(r, 1000u, (uint32_t)(timeout - (now - started)));
        if (e) return e;
    }
    if (!(value & 1u)) return HDMI_TX_SCDC_UNSUPPORTED;
    r->config_issued = 1;
    for (i = 0; i < sizeof(config) / sizeof(config[0]); ++i) {
        e = io_read(r, SNPS + config[i].offset, &value);
        if (e) return e;
        e = io_write(r, SNPS + config[i].offset,
                     (value & ~config[i].mask) | config[i].value);
        if (e) return e;
    }
    e = check_fields(r);
    if (!e) e = stopped(r);
    if (!e) e = now_us(r, &now);
    if (!e && now - started >= timeout) e = HDMI_TX_SCDC_TIMEOUT;
    if (!e) r->initialized = 1;
    return e;
}
static TEE_Result result(struct hdmi_tx_ddc_runtime *r, int e)
{
    if (!e) return TEE_SUCCESS;
    if (e == HDMI_TX_SCDC_UNSUPPORTED) return TEE_ERROR_NOT_SUPPORTED;
    if (e == HDMI_TX_SCDC_EINVAL) return TEE_ERROR_BAD_PARAMETERS;
    if (e == HDMI_TX_SCDC_BUSY) return TEE_ERROR_BUSY;
    if (r->last_result != TEE_SUCCESS) return r->last_result;
    return TEE_ERROR_GENERIC;
}
TEE_Result hdmi_tx_ddc_runtime_bind(struct hdmi_tx_ddc_runtime *r,
    const struct hdmi_mode_io *io,
    int (*verify)(void *, uint64_t, bool, uint32_t), void *source_opaque,
    struct hdmi_scdc_io *out)
{
    (void)r; (void)io; (void)verify; (void)source_opaque; (void)out;
    return TEE_ERROR_NOT_SUPPORTED;
}
TEE_Result hdmi_tx_ddc_runtime_bind_with_sfr(struct hdmi_tx_ddc_runtime *r,
    const struct hdmi_mode_io *io,
    int (*verify)(void *, uint64_t, bool, uint32_t), void *source_opaque,
    int (*query)(void *, uint64_t *), void *sfr_opaque,
    struct hdmi_scdc_io *out)
{
    struct hdmi_tx_scdc_platform platform;
    int e;
    if (!r || !io || !out || !io->read32 || !io->write32 || !io->wait_ms)
        return TEE_ERROR_BAD_PARAMETERS;
    if (!verify || !query) return TEE_ERROR_NOT_SUPPORTED;
    if (r->adapter.bound || r->adapter.active || r->adapter.faulted ||
        r->reset_issued || r->config_issued || r->failed)
        return TEE_ERROR_BUSY;
    memset(out, 0, sizeof(*out));
    r->io = *io;
    r->verify_source_clock = verify;
    r->source_opaque = source_opaque;
    r->query_sfr = query; r->sfr_opaque = sfr_opaque;
    e = check_sfr(r);
    if (e) return TEE_ERROR_NOT_SUPPORTED;
    e = configure(r, HDMI_SCDC_IO_TIMEOUT_US);
    if (e) { r->failed = 1; return result(r, e); }
    memset(&platform, 0, sizeof(platform));
    platform.opaque = r;
    platform.read32 = io_read; platform.write32 = io_write;
    platform.now_us = now_us; platform.wait_us = wait_us;
    platform.verify_source_clock = source_clock;
    platform.verify_ddc_ready = ready;
    /* Recovery is deliberately unsupported for poisoned/outstanding state. */
    platform.reset_ddc_verified = NULL;
    e = hdmi_tx_scdc_adapter_bind(&r->adapter, &platform, out);
    if (e) r->failed = 1;
    return result(r, e);
}
TEE_Result hdmi_tx_ddc_runtime_bind_current_config(struct hdmi_tx_ddc_runtime *r,
    const struct hdmi_mode_io *io,
    int (*verify)(void *, uint64_t, bool, uint32_t), void *source_opaque,
    int (*owner)(void *), void *owner_opaque, struct hdmi_scdc_io *out)
{
    struct hdmi_tx_scdc_platform platform;
    uint32_t first[13], second[13];
    uint64_t before, after;
    int e;
    if (!r || !io || !out || !io->read32 || !io->write32 || !io->wait_ms)
        return TEE_ERROR_BAD_PARAMETERS;
    if (!verify || !owner) return TEE_ERROR_NOT_SUPPORTED;
    if (r->adapter.bound || r->adapter.active || r->adapter.faulted ||
        r->reset_issued || r->config_issued || r->failed)
        return TEE_ERROR_BUSY;
    memset(out, 0, sizeof(*out));
    r->io = *io;
    r->verify_source_clock = verify; r->source_opaque = source_opaque;
    r->verify_owner = owner; r->owner_opaque = owner_opaque;
    r->current_config_mode = 1;
    e = now_us(r, &before);
    if (!e) e = stopped(r);
    if (!e) e = current_fields(r, first);
    if (!e) e = current_fields(r, second);
    if (!e && memcmp(first, second, sizeof(first))) e = HDMI_TX_SCDC_IO;
    if (!e) memcpy(r->current_config, first, sizeof(first));
    if (!e) e = check_fields(r);
    if (!e) e = stopped(r);
    if (!e) e = now_us(r, &after);
    if (!e && after - before >= HDMI_SCDC_IO_TIMEOUT_US)
        e = HDMI_TX_SCDC_TIMEOUT;
    if (e) { r->failed = 1; return result(r, e); }
    memset(&platform, 0, sizeof(platform));
    platform.opaque = r;
    platform.read32 = io_read; platform.write32 = io_write;
    platform.now_us = now_us; platform.wait_us = wait_us;
    platform.verify_source_clock = source_clock; platform.verify_ddc_ready = ready;
    e = hdmi_tx_scdc_adapter_bind(&r->adapter, &platform, out);
    if (e) r->failed = 1;
    else r->initialized = 1;
    return result(r, e);
}
TEE_Result hdmi_tx_ddc_runtime_reset(struct hdmi_tx_ddc_runtime *r,
                                    uint32_t timeout)
{
    int e;
    if (!r) return TEE_ERROR_BAD_PARAMETERS;
    if (r->current_config_mode) return TEE_ERROR_NOT_SUPPORTED;
    if (r->failed || !r->initialized || r->adapter.active ||
        r->adapter.faulted || !r->adapter.bound)
        return TEE_ERROR_NOT_SUPPORTED;
    e = configure(r, timeout);
    if (e) r->failed = 1;
    return result(r, e);
}

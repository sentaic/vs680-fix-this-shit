/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_PIXEL_CLOCK_PLAN_H
#define HDMI_DEV_PIXEL_CLOCK_PLAN_H
#include "mode-io.h"
#include "display-mode.h"
enum hdmi_vpll_stage {
    HDMI_VPLL_VALIDATE, HDMI_VPLL_PREFLIGHT, HDMI_VPLL_POWER_DOWN,
    HDMI_VPLL_RESET, HDMI_VPLL_DIVIDERS, HDMI_VPLL_RELEASE,
    HDMI_VPLL_LOCK, HDMI_VPLL_READBACK, HDMI_VPLL_VERIFIED
};
struct hdmi_vpll_report {
    enum hdmi_vpll_stage stage;
    TEE_Result error;
    uint32_t last_address, mask, expected, observed;
    uint32_t reads_attempted, writes_attempted, writes_completed;
    uint32_t waits_attempted, lock_polls;
    bool plan_valid, lock_observed, verified;
};
/* 按 display-mode 的输入计算 PLL 参数，参考时钟为 25 MHz；逐项检查分频字段范围。 */
TEE_Result hdmi_vpll_validate_plan(const struct hdmi_display_clock_plan *plan);
/* 调用方持有 VPLL0 及其输出资源。写入电源、复位、旁路和分频字段后，最多轮询 LOCK 100 次，每次间隔 1 ms；锁定且字段读回一致时设置 verified。失败进度保存在 report。 */
/* Fresh read-only verification of the actual plan, LOCK, route and gates. */
TEE_Result hdmi_vpll_verify_plan(const struct hdmi_mode_io *io,
    const struct hdmi_display_clock_plan *plan, struct hdmi_vpll_report *report);
TEE_Result hdmi_vpll_program_plan(const struct hdmi_mode_io *io,
    const struct hdmi_display_clock_plan *plan, struct hdmi_vpll_report *report);
#endif

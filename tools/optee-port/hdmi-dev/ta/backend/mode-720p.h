/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_MODE_720P_H
#define HDMI_DEV_MODE_720P_H

#include <stdint.h>
#include "mode-io.h"

/* mode-io.h统一定义32位MMIO回调；地址是字节物理地址，回调负责访问排序。 */
#define HDMI_MODE_720P_PIXEL_CLOCK_HZ UINT32_C(74250000)
#define HDMI_MODE_720P_WIDTH          UINT32_C(1280)
#define HDMI_MODE_720P_HEIGHT         UINT32_C(720)
#define HDMI_MODE_720P_HTOTAL         UINT32_C(1650)
#define HDMI_MODE_720P_VTOTAL         UINT32_C(750)
#define HDMI_MODE_720P_GFX_CHANNEL    UINT32_C(7)

/* 配置 720p60、RGB24、8 位全范围、正 H/V 极性。调用前完成 AVIO/VPP 初始化并确认 DMA 空闲。configure 失败时保留池供恢复；首帧命令排队后调用 enable，显示入口在 VBI 上启动。 */
TEE_Result hdmi_mode_720p_configure(const struct hdmi_mode_io *io);
TEE_Result hdmi_mode_720p_enable(const struct hdmi_mode_io *io);
struct hdmi_mode_resume_window {
    void *opaque;
    int (*now_us)(void *,uint64_t *);
    uint64_t not_before_us,before_us;
};
TEE_Result hdmi_mode_enable_prepared_window(const struct hdmi_mode_io *io,
    uint32_t vsync_width,const struct hdmi_mode_resume_window *window);
TEE_Result hdmi_mode_enable_prepared(const struct hdmi_mode_io *io, uint32_t vsync_width);
TEE_Result hdmi_mode_720p_disable(const struct hdmi_mode_io *io);

#endif

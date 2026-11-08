/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_FRAME_START_H
#define HDMI_DEV_FRAME_START_H
#include "avio-hardware.h"
#include "dma-pool.h"
#include "mode-io.h"
#include "gfx-dma.h"
#include "mixer.h"
struct hdmi_frame_start {
    struct hdmi_avio_registers *registers;
    struct hdmi_mode_io io;
    struct hdmi_dma_pool *pool;
    struct hdmi_mixer mixer;
    const struct hdmi_gfx_dma *overlay;
    uint32_t list_bytes, table_slot;
    bool configured, queued, prepared, repeating, switch_pending;
};
/* 两份物理表交替使用；旧表和像素在新表执行、旧DMA结束前保持不变。 */
TEE_Result hdmi_frame_start_init(struct hdmi_frame_start *start,
    struct hdmi_avio_registers *registers, const struct hdmi_mode_io *io,
    struct hdmi_dma_pool *pool);
TEE_Result hdmi_frame_start_set_mixer(struct hdmi_frame_start *start,
    uint32_t background_rgb, uint32_t alpha_q12);
TEE_Result hdmi_frame_start_ready(struct hdmi_frame_start *start);
TEE_Result hdmi_frame_start_queue(struct hdmi_frame_start *start,
    const struct hdmi_gfx_dma *gfx);
TEE_Result hdmi_frame_start_repeat(struct hdmi_frame_start *start, bool enabled);
/* 读取最后消费的 AIO 描述符及完成状态。 */
TEE_Result hdmi_frame_start_applied(struct hdmi_frame_start *start);
TEE_Result hdmi_frame_start_switch(struct hdmi_frame_start *start,
    const struct hdmi_gfx_dma *gfx, const struct hdmi_gfx_dma *overlay);
/* 调用者已经匹配两路旧DMA完成，并保留当前自动读取的像素槽。 */
TEE_Result hdmi_frame_start_release_old(struct hdmi_frame_start *start);
TEE_Result hdmi_frame_start_stop(struct hdmi_frame_start *);
#endif

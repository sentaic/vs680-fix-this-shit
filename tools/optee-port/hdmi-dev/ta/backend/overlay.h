/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_OVERLAY_H
#define HDMI_DEV_OVERLAY_H
#include "gfx-dma.h"
#include "frame-slots.h"
#include "mode-io.h"
#include "../../hdmi-dev.h"
struct hdmi_overlay {
    struct hdmi_gfx_dma dma;
    struct hdmi_frame_slots slots;
    struct hdmi_dev_overlay geometry;
    uint64_t submission;
    bool configured;
};
TEE_Result hdmi_overlay_configure(struct hdmi_overlay *overlay,
    struct hdmi_avio_registers *registers, const struct hdmi_mode_io *io,
    const struct hdmi_dev_overlay *geometry);
TEE_Result hdmi_overlay_configure_geometry(struct hdmi_overlay *,
    struct hdmi_avio_registers *,const struct hdmi_mode_io *,
    const struct hdmi_dev_overlay *,uint32_t,uint32_t);
TEE_Result hdmi_overlay_set(struct hdmi_overlay *overlay, struct hdmi_dma_pool *pool,
    const struct hdmi_dev_overlay *geometry, const void *pixels, size_t bytes);
TEE_Result hdmi_overlay_complete(struct hdmi_overlay *overlay);
TEE_Result hdmi_overlay_prepare(struct hdmi_overlay *overlay, struct hdmi_dma_pool *pool);
TEE_Result hdmi_overlay_started(struct hdmi_overlay *overlay);
#endif

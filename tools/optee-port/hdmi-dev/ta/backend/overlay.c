/* SPDX-License-Identifier: GPL-2.0 */
#include "overlay.h"
#include <string.h>
_Static_assert(HDMI_DEV_OVERLAY_OFFSET + 2u * HDMI_DEV_OVERLAY_SLOT_BYTES <=
    HDMI_DMA_COMMAND_BYTES, "overlay overlaps full-frame slots");
_Static_assert(HDMI_DEV_OVERLAY_OFFSET >= 3u * HDMI_DMA_POOL_ALIGN,
    "overlay overlaps metadata or BCM table");

static TEE_Result validate_pixel_regions(const struct hdmi_dma_pool *pool,
    uint64_t bytes)
{
    uint32_t offset;
    if (!bytes || bytes > UINT32_MAX) return TEE_ERROR_BAD_PARAMETERS;
    /* Validate both immutable slots before any CPU loan or pixel write. */
    for (unsigned slot=0;slot<2u;++slot) {
        TEE_Result rc=hdmi_dma_pool_pip_region(pool,slot,(uint32_t)bytes,&offset);
        if (rc) return rc;
    }
    return TEE_SUCCESS;
}

TEE_Result hdmi_overlay_set(struct hdmi_overlay *o, struct hdmi_dma_pool *pool,
    const struct hdmi_dev_overlay *geometry, const void *pixels, size_t bytes)
{
    unsigned slot;
    if (!o || !pool || !pixels || !hdmi_dev_overlay_valid(geometry,bytes))
        return TEE_ERROR_BAD_PARAMETERS;
    if (!o->configured || !pool->registered || !pool->mapped ||
        pool->bytes < HDMI_DMA_POOL_MIN_BYTES) return TEE_ERROR_BAD_STATE;
    if (memcmp(geometry,&o->geometry,sizeof(*geometry))) return TEE_ERROR_BAD_STATE;
    TEE_Result rc=validate_pixel_regions(pool,bytes);
    if (rc) return rc;
    rc=hdmi_frame_slots_begin(&o->slots,&slot);
    if (rc) return rc;
    uint32_t offset;
    rc=hdmi_dma_pool_pip_region(pool,slot,(uint32_t)bytes,&offset);
    if (rc) return rc;
    hdmi_payload_modified(&pool->payload_cache,offset,(uint32_t)bytes);
    TEE_MemMove((char *)pool->mapped+offset,pixels,bytes);
    rc=hdmi_payload_publish(&pool->payload_cache,pool->mapped,pool->bytes,
                            offset,(uint32_t)bytes);
    if (rc) return rc;
    /* 首图保留loan直到实际提交；后续图进入独立pending槽。 */
    return o->slots.current==HDMI_FRAME_NONE ? TEE_SUCCESS :
        hdmi_frame_slots_pending(&o->slots);
}
TEE_Result hdmi_overlay_complete(struct hdmi_overlay *o)
{
    uint64_t completed;
    if (!o || !o->configured) return TEE_ERROR_BAD_STATE;
    if (o->slots.active==HDMI_FRAME_NONE) return TEE_SUCCESS;
    unsigned slot=o->slots.active;
    TEE_Result rc=hdmi_gfx_dma_complete(&o->dma,&completed);
    if (rc) return rc;
    if (completed!=o->submission) return TEE_ERROR_BAD_STATE;
    return hdmi_frame_slots_read_completed(&o->slots,slot);
}
TEE_Result hdmi_overlay_prepare(struct hdmi_overlay *o, struct hdmi_dma_pool *pool)
{
    unsigned slot;
    TEE_Result rc;
    if (!o || !o->configured) return TEE_ERROR_BAD_STATE;
    rc=validate_pixel_regions(pool,(uint64_t)o->geometry.stride*o->geometry.height);
    if (rc) return rc;
    if (o->slots.current==HDMI_FRAME_NONE) {
        slot=o->slots.loan;
        if (slot>=2) return TEE_ERROR_BAD_STATE;
    } else {
        rc=hdmi_frame_slots_choose(&o->slots,&slot);
        if (rc) return rc;
    }
    return hdmi_gfx_dma_submit_region(&o->dma,pool,
        hdmi_dma_pool_pip_offset(pool,slot),
        o->geometry.stride,o->geometry.width*4u,o->geometry.height,&o->submission);
}
TEE_Result hdmi_overlay_started(struct hdmi_overlay *o)
{
    unsigned slot;
    if (!o || !o->configured || !o->dma.active) return TEE_ERROR_BAD_STATE;
    if (o->slots.current==HDMI_FRAME_NONE)
        return hdmi_frame_slots_first_started(&o->slots);
    TEE_Result rc=hdmi_frame_slots_choose(&o->slots,&slot);
    return rc ? rc : hdmi_frame_slots_started(&o->slots,slot);
}

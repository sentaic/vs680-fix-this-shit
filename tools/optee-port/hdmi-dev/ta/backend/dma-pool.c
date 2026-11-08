/* SPDX-License-Identifier: GPL-2.0 */
#include "dma-pool.h"
#include "../../hdmi-dev.h"
#include <tee_api_syna.h>
#include <tee_api.h>

_Static_assert(HDMI_DMA_IMAGE_BYTES <= HDMI_DMA_FRAME_SLOT_BYTES,
               "RGB24 frame exceeds its slot");
_Static_assert(HDMI_DMA_COMMAND_BYTES + 2u * HDMI_DMA_FRAME_SLOT_BYTES ==
               HDMI_DMA_POOL_MIN_BYTES, "invalid pool layout");

_Static_assert(HDMI_DMA_MAIN_COMMAND_BYTES+3u*HDMI_DMA_IMAGE_BYTES <=
    HDMI_DMA_POOL_MIN_BYTES,"three RGB24 frames exceed the retained pool");
_Static_assert(HDMI_DMA_IMAGE_BYTES % HDMI_DMA_POOL_ALIGN == 0,"frame alignment");
static bool valid_region(uint32_t address, uint32_t bytes)
{
    return hdmi_dev_pool_valid(address,bytes);
}

TEE_Result hdmi_dma_pool_plan_layout(uint32_t capacity, uint32_t width,
    uint32_t height, uint32_t stride, unsigned slots, bool pip_enabled,
    struct hdmi_dma_layout *layout)
{
    if (!layout || !width || width > 2560u || !height || height > 1440u ||
        slots < 2u || slots > 3u || stride < width * 3u ||
        (stride & 15u) || ((width * 3u) & 15u) || stride > 0x00ffffffu ||
        (capacity != HDMI_DMA_POOL_MIN_BYTES && capacity != HDMI_DEV_CACHED_POOL_BYTES))
        return TEE_ERROR_BAD_PARAMETERS;
    uint64_t image = (uint64_t)stride * height;
    uint64_t slot = (image + HDMI_DMA_POOL_ALIGN - 1u) &
        ~(uint64_t)(HDMI_DMA_POOL_ALIGN - 1u);
    /* Metadata occupies [0,4096); immutable BCM tables start at4096/8192
     * and each is <=384bytes. Keep the full64KiB command reservation. */
    uint32_t first = HDMI_DMA_MAIN_COMMAND_BYTES +
        (pip_enabled ? 2u * HDMI_DMA_PIP_COMPACT_SLOT_BYTES : 0u);
    uint64_t total = first + slot * slots;
    if (image > UINT32_MAX || slot > UINT32_MAX || total > capacity)
        return TEE_ERROR_OUT_OF_MEMORY;
    struct hdmi_dma_layout candidate = {
        .width = width, .height = height, .stride = stride,
        .image_bytes = (uint32_t)image, .frame_base = first,
        .slot_bytes = (uint32_t)slot, .slots = slots,
        .total_bytes = (uint32_t)total,
        .command_bytes = HDMI_DMA_MAIN_COMMAND_BYTES,
        .pip_offset = {pip_enabled ? HDMI_DMA_MAIN_COMMAND_BYTES : 0u,
            pip_enabled ? HDMI_DMA_MAIN_COMMAND_BYTES + HDMI_DMA_PIP_COMPACT_SLOT_BYTES : 0u},
        .pip_slot_bytes = pip_enabled ? HDMI_DMA_PIP_COMPACT_SLOT_BYTES : 0u,
        .pip_enabled = pip_enabled,
    };
    *layout = candidate;
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_configure_layout(struct hdmi_dma_pool *pool,
    uint32_t width, uint32_t height, uint32_t stride, unsigned slots,
    bool pip_enabled)
{
    struct hdmi_dma_layout layout;
    if (!pool) return TEE_ERROR_BAD_PARAMETERS;
    if (pool->dma_armed) return TEE_ERROR_BUSY;
    if (!pool->registered || !pool->mapped || !valid_region(pool->pa, pool->bytes))
        return TEE_ERROR_BAD_STATE;
    TEE_Result rc = hdmi_dma_pool_plan_layout(pool->bytes, width, height,
        stride, slots, pip_enabled, &layout);
    if (rc) return rc;
    pool->layout = layout;
    pool->layout_configured = true;
    pool->main_three_slots = slots == 3u;
    pool->payload_cache = (struct hdmi_payload_cache){0};
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_pip_region(const struct hdmi_dma_pool *pool,
    unsigned slot, uint32_t bytes, uint32_t *offset)
{
    if (!pool || !offset || slot >= 2u || !bytes)
        return TEE_ERROR_BAD_PARAMETERS;
    uint32_t capacity = hdmi_dma_pool_pip_slot_bytes(pool);
    uint32_t first = hdmi_dma_pool_pip_offset(pool,slot);
    if (!capacity || !first || bytes > capacity)
        return TEE_ERROR_BAD_PARAMETERS;
    if (!pool->registered || !pool->mapped || !valid_region(pool->pa,pool->bytes))
        return TEE_ERROR_BAD_STATE;
    uint32_t command = pool->layout_configured ? pool->layout.command_bytes : HDMI_DMA_MAIN_COMMAND_BYTES;
    uint32_t frame_base = hdmi_dma_pool_frame_offset(pool,0);
    uint32_t zero = hdmi_dma_pool_pip_offset(pool,0);
    uint32_t one = hdmi_dma_pool_pip_offset(pool,1);
    if (zero < command || (uint64_t)zero+capacity > one ||
        (uint64_t)one+capacity > frame_base) return TEE_ERROR_BAD_STATE;
    if (first < command || first >= pool->bytes || capacity > pool->bytes-first ||
        first >= frame_base || capacity > frame_base-first ||
        (first & 15u) || (uint64_t)pool->pa+first+capacity-1u > UINT32_MAX)
        return TEE_ERROR_BAD_STATE;
    *offset = first;
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_unmap(struct hdmi_dma_pool *pool)
{
    TEE_Result result;
    if (!pool) return TEE_ERROR_BAD_PARAMETERS;
    if (pool->dma_armed) return TEE_ERROR_BUSY;
    if (pool->mapped) {
        result = TEE_Unmap_Memory(pool->mem_id);
        if (result) return result;
        pool->mapped = NULL;
    }
    if (pool->registered) {
        result = TEE_Unregister_Memory(pool->mem_id);
        if (result) return result;
        pool->registered = false;
    }
    *pool = (struct hdmi_dma_pool){0};
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_map(struct hdmi_dma_pool *pool,
                             uint32_t address, uint32_t bytes)
{
    TEE_Result result, cleanup;
    paddr_t actual = 0;
    size_t mapped_bytes = 0;
    if (!pool || !valid_region(address, bytes)) return TEE_ERROR_BAD_PARAMETERS;
    if (pool->registered || pool->mapped || pool->dma_armed)
        return TEE_ERROR_BAD_STATE;
    result = TEE_Register_Memory(address, bytes, &pool->mem_id);
    if (result) return result;
    pool->registered = true;
    pool->pa = address;
    pool->bytes = bytes;
    result = TEE_GetPhys_By_Memid(pool->mem_id, &actual);
    if (!result && actual != address) result = TEE_ERROR_BAD_STATE;
    if (result) goto fail;
    result = TEE_Map_Memory(pool->mem_id, &pool->mapped, &mapped_bytes);
    if (!result && (!pool->mapped || mapped_bytes != bytes))
        result = TEE_ERROR_BAD_STATE;
    if (!result) return TEE_SUCCESS;
fail:
    cleanup = hdmi_dma_pool_unmap(pool);
    return cleanup ? cleanup : result;
}

TEE_Result hdmi_dma_pool_frame(struct hdmi_dma_pool *pool, unsigned slot,
                               void **mapped, uint32_t *pa)
{
    uint32_t offset;
    if (!pool || !mapped || !pa || slot >= hdmi_dma_pool_frame_count(pool)) return TEE_ERROR_BAD_PARAMETERS;
    *mapped = NULL;
    *pa = 0;
    if (!pool->registered || !pool->mapped || !valid_region(pool->pa, pool->bytes))
        return TEE_ERROR_BAD_STATE;
    offset = hdmi_dma_pool_frame_offset(pool,slot);
    if (offset >= pool->bytes || hdmi_dma_pool_frame_stride(pool) > pool->bytes - offset ||
        pool->pa > UINT32_MAX - offset) return TEE_ERROR_BAD_STATE;
    hdmi_payload_modified(&pool->payload_cache,offset,hdmi_dma_pool_frame_stride(pool));
    *mapped = (uint8_t *)pool->mapped + offset;
    *pa = pool->pa + offset;
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_clean_frame(struct hdmi_dma_pool *pool, unsigned slot)
{
    void *mapped;
    uint32_t pa;
    TEE_Result result = hdmi_dma_pool_frame(pool, slot, &mapped, &pa);
    if (result) return result;
    result = hdmi_payload_publish(&pool->payload_cache,pool->mapped,pool->bytes,
        hdmi_dma_pool_frame_offset(pool,slot),hdmi_dma_pool_image_bytes(pool));
    if (result) return result;
    __asm__ volatile("dsb sy" ::: "memory");
    return TEE_SUCCESS;
}

void hdmi_dma_pool_arm(struct hdmi_dma_pool *pool)
{
    if (pool) pool->dma_armed = true;
}

TEE_Result hdmi_dma_pool_reset_complete(struct hdmi_dma_pool *pool,
                                       const struct hdmi_avio_activity *activity)
{
    if (!pool || !activity) return TEE_ERROR_BAD_PARAMETERS;
    if (!(activity->reset_issued & 1u) || !hdmi_avio_idle(activity))
        return TEE_ERROR_BUSY;
    pool->dma_armed = false;
    return TEE_SUCCESS;
}

TEE_Result hdmi_dma_pool_stop_complete(struct hdmi_dma_pool *pool,
                                      const struct hdmi_avio_activity *activity)
{
    if(!pool||!activity)return TEE_ERROR_BAD_PARAMETERS;
    if(!hdmi_avio_idle(activity))return TEE_ERROR_BUSY;
    pool->dma_armed=false;return TEE_SUCCESS;
}

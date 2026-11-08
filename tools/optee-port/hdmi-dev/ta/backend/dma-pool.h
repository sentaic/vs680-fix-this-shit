/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_DMA_POOL_H
#define HDMI_DEV_DMA_POOL_H
#include <tee_internal_api.h>
#include "avio-hardware.h"
#include "payload-cache.h"
#define HDMI_DMA_POOL_FIRST UINT32_C(0x76400000)
#define HDMI_DMA_POOL_END UINT32_C(0x76e00000)
#define HDMI_DMA_POOL_ALIGN UINT32_C(4096)
#define HDMI_DMA_COMMAND_BYTES UINT32_C(0x200000)
#define HDMI_DMA_FRAME_SLOT_BYTES UINT32_C(0x300000)
#define HDMI_DMA_POOL_MIN_BYTES UINT32_C(0x800000)
#define HDMI_DMA_IMAGE_BYTES (1280u * 720u * 3u)
#define HDMI_DMA_MAIN_COMMAND_BYTES UINT32_C(0x10000)
#define HDMI_DMA_PIP_COMPACT_SLOT_BYTES UINT32_C(0x20000)
struct hdmi_dma_layout {
    uint32_t width, height, stride, image_bytes;
    uint32_t frame_base, slot_bytes, slots, total_bytes;
    uint32_t command_bytes, pip_offset[2], pip_slot_bytes;
    bool pip_enabled;
};
struct hdmi_dma_pool {
    void *mapped;
    uint32_t pa;
    uint32_t bytes;
    uint32_t mem_id;
    bool main_three_slots;
    bool registered;
    bool dma_armed;
    struct hdmi_payload_cache payload_cache;
    struct hdmi_dma_layout layout;
    bool layout_configured;
};
static inline unsigned hdmi_dma_pool_frame_count(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.slots :
    (pool && pool->main_three_slots ? 3u : 2u); }
static inline uint32_t hdmi_dma_pool_frame_stride(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.slot_bytes :
    (pool && pool->main_three_slots ? HDMI_DMA_IMAGE_BYTES : HDMI_DMA_FRAME_SLOT_BYTES); }
static inline uint32_t hdmi_dma_pool_frame_offset(const struct hdmi_dma_pool *pool,unsigned slot)
{
    return (pool && pool->layout_configured ? pool->layout.frame_base :
        (pool && pool->main_three_slots ? HDMI_DMA_MAIN_COMMAND_BYTES : HDMI_DMA_COMMAND_BYTES))+
        slot*hdmi_dma_pool_frame_stride(pool);
}
/* 按指定槽数计算内存布局，容量不足时返回错误。 */
TEE_Result hdmi_dma_pool_plan_layout(uint32_t capacity, uint32_t width,
    uint32_t height, uint32_t stride, unsigned slots, bool pip_enabled,
    struct hdmi_dma_layout *layout);
/* 物理池完整映射后绑定布局；从首次 DMA 提交到停止并解除映射期间，保持该布局。 */
TEE_Result hdmi_dma_pool_configure_layout(struct hdmi_dma_pool *pool,
    uint32_t width, uint32_t height, uint32_t stride, unsigned slots,
    bool pip_enabled);
static inline uint32_t hdmi_dma_pool_width(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.width : 1280u; }
static inline uint32_t hdmi_dma_pool_height(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.height : 720u; }
static inline uint32_t hdmi_dma_pool_row_stride(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.stride : 1280u * 3u; }
static inline uint32_t hdmi_dma_pool_image_bytes(const struct hdmi_dma_pool *pool)
{ return pool && pool->layout_configured ? pool->layout.image_bytes : HDMI_DMA_IMAGE_BYTES; }
/* 旧 INIT 使用固定 PIP 前缀；显式布局返回 PIP 偏移和容量，PIP 关闭时容量为 0。 */
static inline uint32_t hdmi_dma_pool_pip_slot_bytes(const struct hdmi_dma_pool *pool)
{ return !pool ? 0u : pool->layout_configured ?
    (pool->layout.pip_enabled ? pool->layout.pip_slot_bytes : 0u) : UINT32_C(0xe0000); }
static inline uint32_t hdmi_dma_pool_pip_offset(const struct hdmi_dma_pool *pool, unsigned slot)
{
    if (!pool || slot >= 2u) return 0u;
    return pool->layout_configured ?
        (pool->layout.pip_enabled ? pool->layout.pip_offset[slot] : 0u) :
        UINT32_C(0x10000) + slot * UINT32_C(0xe0000);
}
/* 检查映射边界、预留前缀、帧边界和物理地址范围；成功后更新 offset。 */
TEE_Result hdmi_dma_pool_pip_region(const struct hdmi_dma_pool *pool,
    unsigned slot, uint32_t bytes, uint32_t *offset);
TEE_Result hdmi_dma_pool_map(struct hdmi_dma_pool *pool,
                             uint32_t address, uint32_t bytes);
TEE_Result hdmi_dma_pool_frame(struct hdmi_dma_pool *pool, unsigned slot,
                               void **mapped, uint32_t *pa);
TEE_Result hdmi_dma_pool_clean_frame(struct hdmi_dma_pool *pool, unsigned slot);
/* 首次 DMA 提交前置位，停止确认后清除。 */
void hdmi_dma_pool_arm(struct hdmi_dma_pool *pool);
/* 调用者已停止Q0、AIO8、GFX1/PIP及TX输出，并取得专属通道静止快照。 */
TEE_Result hdmi_dma_pool_stop_complete(struct hdmi_dma_pool *,
                                      const struct hdmi_avio_activity *);
TEE_Result hdmi_dma_pool_reset_complete(struct hdmi_dma_pool *pool,
                                       const struct hdmi_avio_activity *activity);
/* 撤销 TA 的映射与注册，Linux holder 继续持有 DMA-BUF。 */
TEE_Result hdmi_dma_pool_unmap(struct hdmi_dma_pool *pool);
#endif

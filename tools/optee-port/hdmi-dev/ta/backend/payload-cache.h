/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_PAYLOAD_CACHE_H
#define HDMI_DEV_PAYLOAD_CACHE_H
#include <tee_internal_api.h>
#include <stdbool.h>
/* 主画面和PIP各两个CPU写入槽；表和元数据仍由各自提交者维护。 */
#define HDMI_PAYLOAD_CACHE_RANGES 4u
struct hdmi_payload_range { uint32_t offset, bytes; };
struct hdmi_payload_cache {
    struct hdmi_payload_range ranges[HDMI_PAYLOAD_CACHE_RANGES];
};
/* 每次向CPU交出可写缓冲之前使重叠的已发布范围失效。 */
void hdmi_payload_modified(struct hdmi_payload_cache *cache,
                           uint32_t offset, uint32_t bytes);
TEE_Result hdmi_payload_publish(struct hdmi_payload_cache *cache, void *mapped,
    uint32_t capacity, uint32_t offset, uint32_t bytes);
/* 发布记录覆盖整个 DMA 读取范围时复用，否则清理缓存。 */
TEE_Result hdmi_payload_ensure(struct hdmi_payload_cache *cache, void *mapped,
    uint32_t capacity, uint32_t offset, uint32_t bytes);
#endif

/* SPDX-License-Identifier: GPL-2.0 */
#include "payload-cache.h"
#include "../show-timing.h"
#include <tee_internal_api_extensions.h>

static bool valid(struct hdmi_payload_cache *cache, void *mapped,
                   uint32_t capacity, uint32_t offset, uint32_t bytes)
{
    return cache && mapped && bytes && offset < capacity &&
        bytes <= capacity-offset;
}
void hdmi_payload_modified(struct hdmi_payload_cache *cache,
                           uint32_t offset, uint32_t bytes)
{
    if (!cache || !bytes) return;
    for (unsigned i=0;i<HDMI_PAYLOAD_CACHE_RANGES;++i) {
        struct hdmi_payload_range *range=&cache->ranges[i];
        if (range->bytes && (uint64_t)offset+bytes > range->offset &&
            (uint64_t)range->offset+range->bytes > offset)
            range->bytes=0;
    }
}
TEE_Result hdmi_payload_publish(struct hdmi_payload_cache *cache, void *mapped,
    uint32_t capacity, uint32_t offset, uint32_t bytes)
{
    if (!valid(cache,mapped,capacity,offset,bytes)) return TEE_ERROR_BAD_PARAMETERS;
    /* 缓存清理失败时保留待发布状态，下一次提交重新清理。 */
    hdmi_payload_modified(cache,offset,bytes);
    hdmi_show_timing_mark(HDMI_SHOW_CACHE_BEGIN);
    TEE_Result rc=TEE_CacheClean((char *)mapped+offset,bytes);
    hdmi_show_timing_mark(HDMI_SHOW_CACHE_END);
    if (rc) return rc;
    for (unsigned i=0;i<HDMI_PAYLOAD_CACHE_RANGES;++i)
        if (!cache->ranges[i].bytes) {
            cache->ranges[i]=(struct hdmi_payload_range){offset,bytes};
            break;
        }
    /* 记录已满时仍完成本次清理，后续ensure保守地再次清理。 */
    return TEE_SUCCESS;
}
TEE_Result hdmi_payload_ensure(struct hdmi_payload_cache *cache, void *mapped,
    uint32_t capacity, uint32_t offset, uint32_t bytes)
{
    if (!valid(cache,mapped,capacity,offset,bytes)) return TEE_ERROR_BAD_PARAMETERS;
    for (unsigned i=0;i<HDMI_PAYLOAD_CACHE_RANGES;++i) {
        const struct hdmi_payload_range *range=&cache->ranges[i];
        if (range->bytes && offset>=range->offset &&
            (uint64_t)offset+bytes <= (uint64_t)range->offset+range->bytes)
            return TEE_SUCCESS;
    }
    return hdmi_payload_publish(cache,mapped,capacity,offset,bytes);
}

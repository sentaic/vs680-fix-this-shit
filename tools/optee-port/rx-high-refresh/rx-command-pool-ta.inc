/* SPDX-License-Identifier: GPL-2.0 */
#include <tee_api_syna.h>
#include <tee_api.h>
#include "rx-command-pool-command.h"
/* 底层NC DMA-BUF由外部holder固定；此对象保留本TA的注册与映射。 */
static struct {
    void *mapped;
    uint32_t pa, bytes, mem_id;
    uint64_t epoch;
    int registered, dma_armed;
} rx_command_pool;
static TEE_Result rx_command_pool_unmap(void)
{
    TEE_Result r;
    if (rx_command_pool.dma_armed) return TEE_ERROR_BUSY;
    if (rx_command_pool.mapped) {
        r = TEE_Unmap_Memory(rx_command_pool.mem_id);
        if (r) return r;
        rx_command_pool.mapped = NULL;
    }
    if (rx_command_pool.registered) {
        r = TEE_Unregister_Memory(rx_command_pool.mem_id);
        if (r) return r;
        rx_command_pool.registered = 0;
    }
    TEE_MemFill(&rx_command_pool, 0, sizeof(rx_command_pool));
    return TEE_SUCCESS;
}
static TEE_Result rx_command_pool_bind(uint64_t epoch, uint32_t pa, uint32_t bytes)
{
    if (!epoch || pa != RX_COMMAND_POOL_PA || bytes != RX_COMMAND_POOL_BYTES)
        return TEE_ERROR_BAD_PARAMETERS;
    if (rx_command_pool.registered || rx_command_pool.mapped || rx_command_pool.dma_armed)
        return TEE_ERROR_BAD_STATE;
    TEE_Result r = TEE_Register_Memory(pa, bytes, &rx_command_pool.mem_id);
    if (r) return r;
    rx_command_pool.registered = 1;
    rx_command_pool.epoch = epoch; rx_command_pool.pa = pa; rx_command_pool.bytes = bytes;
    paddr_t actual = 0; size_t mapped_bytes = 0;
    r = TEE_GetPhys_By_Memid(rx_command_pool.mem_id, &actual);
    if (!r && actual != pa) r = TEE_ERROR_BAD_STATE;
    if (!r) r = TEE_Map_Memory(rx_command_pool.mem_id, &rx_command_pool.mapped, &mapped_bytes);
    if (!r && (!rx_command_pool.mapped || mapped_bytes != bytes)) r = TEE_ERROR_BAD_STATE;
    if (!r) return TEE_SUCCESS;
    TEE_Result cleanup = rx_command_pool_unmap();
    return cleanup ? cleanup : r;
}
/* Q5 发布读取前置位 dma_armed；Q5 和通道 8 停止后清除。 */
static inline int rx_command_pool_bound(uint64_t epoch)
{
    return epoch && rx_command_pool.epoch == epoch && rx_command_pool.registered &&
        rx_command_pool.mapped && rx_command_pool.pa == RX_COMMAND_POOL_PA &&
        rx_command_pool.bytes == RX_COMMAND_POOL_BYTES;
}
static inline TEE_Result rx_command_pool_table(uint64_t epoch, uint32_t offset,
                                               uint32_t bytes, void **va, uint32_t *pa)
{
    if (!va || !pa || !bytes || (offset & 63u) || (bytes & 7u))
        return TEE_ERROR_BAD_PARAMETERS;
    *va = NULL; *pa = 0;
    if (!rx_command_pool_bound(epoch) || rx_command_pool.dma_armed)
        return TEE_ERROR_BAD_STATE;
    if (offset >= rx_command_pool.bytes || bytes > rx_command_pool.bytes - offset)
        return TEE_ERROR_BAD_PARAMETERS;
    *va = (uint8_t *)rx_command_pool.mapped + offset;
    *pa = rx_command_pool.pa + offset;
    return TEE_SUCCESS;
}
static TEE_Result rx_command_pool_command(uint32_t types, TEE_Param p[4], int owner_live)
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT, TEE_PARAM_TYPE_NONE,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) || !p[0].memref.buffer ||
        p[0].memref.size != sizeof(struct rx_command_pool_command))
        return TEE_ERROR_BAD_PARAMETERS;
    struct rx_command_pool_command c;
    TEE_MemMove(&c, p[0].memref.buffer, sizeof(c));
    if (c.version != RX_COMMAND_POOL_VERSION || c.size != sizeof(c) || !c.epoch ||
        c.reserved || c.mem_id || c.acknowledgement) return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result r;
    if (c.operation == RX_COMMAND_POOL_BIND) {
        if (owner_live) return TEE_ERROR_BUSY;
        r = rx_command_pool_bind(c.epoch, c.pa, c.bytes);
    } else {
        if (!rx_command_pool.registered || rx_command_pool.epoch != c.epoch)
            return TEE_ERROR_BAD_STATE;
        if (c.pa != rx_command_pool.pa || c.bytes != rx_command_pool.bytes)
            return TEE_ERROR_BAD_PARAMETERS;
        if (c.operation == RX_COMMAND_POOL_STATUS) r = TEE_SUCCESS;
        else if (c.operation == RX_COMMAND_POOL_UNBIND) {
            if (owner_live) return TEE_ERROR_BUSY;
            r = rx_command_pool_unmap();
        } else return TEE_ERROR_BAD_PARAMETERS;
    }
    if (r) return r;
    c.mem_id = rx_command_pool.mem_id; c.acknowledgement = RX_COMMAND_POOL_ACK;
    TEE_MemMove(p[0].memref.buffer, &c, sizeof(c));
    return TEE_SUCCESS;
}

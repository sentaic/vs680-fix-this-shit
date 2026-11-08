/* SPDX-License-Identifier: GPL-2.0 */
/* TA 内部接口：调用方持有显示和 RX 资源，TX 已停止，AIO8 已配置 128/384 字节 FIFO。 */
#include "avio.h"
#ifndef RX_STREAM_DISABLE_RECORD_DEFINED
#define RX_STREAM_DISABLE_RECORD_DEFINED
struct rx_stream_disable_record { uint32_t tg, wr, channel, second; };
#endif
#define RX_VDE_READ_CHANNEL 8u
#define RX_VDE_READ_MASK (1u << RX_VDE_READ_CHANNEL)
#define RX_VDE_QUEUE_MASK ((1u << 5) | (1u << 13))
#define RX_VDE_TABLE_CAPACITY 512u
#define RX_VDE_OUTER_OFFSET 512u
#define RX_VDE_POOL_REQUIRED 1024u
struct rx_vde_exclusive {
    HDL_dhub2d *aio;
    void *mapped;
    uint32_t pa, capacity, mem_id, inner_bytes, outer_bytes;
    uint32_t inner_command[2], outer_command[2], saved_q5;
    uint64_t epoch, token;
    uint32_t bound, prepared, armed, stopping;
    uint32_t aio_enable_issued, aio_disable_issued, aio_second_disable_issued, aio_fifo_disable_issued;
};
static inline uint32_t rx_vde_read32(uint32_t address)
{ return *(volatile uint32_t *)TEE_TA_RegPhyToVirt(address); }
static inline void rx_vde_barrier(void)
{
#ifndef RX_VDE_HOST_TEST
    __asm__ volatile("dsb sy" ::: "memory");
#endif
}
static inline uint32_t rx_vde_bcm(uint32_t offset)
{ return rx_vde_read32(MEMMAP_AVIO_BCM_REG_BASE + offset); }
static inline int rx_vde_aio_idle(struct rx_vde_exclusive *v, uint32_t *pointer)
{
    HDL_hbo *hbo = dhub_hbo(&v->aio->dhub);
    unsigned p = 0;
    if ((dhub_channel_busy(&v->aio->dhub) |
         dhub_channel_pending(&v->aio->dhub) |
         dhub2d_channel_busy(v->aio)) & RX_VDE_READ_MASK) return 0;
    if (hbo_queue_busy(hbo) & (3u << (2 * RX_VDE_READ_CHANNEL))) return 0;
    if (hbo_queue_query(hbo, 2 * RX_VDE_READ_CHANNEL,
                       SemaQueryMap_ADDR_master_consumer, &p)) return 0;
    if (hbo_queue_query(hbo, 2 * RX_VDE_READ_CHANNEL + 1,
                       SemaQueryMap_ADDR_master_consumer, pointer)) return 0;
    *pointer = p;
    return p < 16u;
}
static inline TEE_Result rx_vde_exclusive_bind(struct rx_vde_exclusive *v,
    HDL_dhub2d *aio, void *mapped, uint32_t pa, uint32_t bytes,
    uint32_t mem_id, uint64_t epoch, uint32_t enable_issued)
{
    if (!v || !aio || !mapped || ((uintptr_t)mapped & 7u) || (pa & 63u) ||
        bytes < RX_VDE_POOL_REQUIRED || (uint64_t)pa + bytes > UINT64_C(0x100000000) ||
        !epoch || !mem_id || enable_issued!=1) return TEE_ERROR_BAD_PARAMETERS;
    if (v->bound || v->armed) return TEE_ERROR_BAD_STATE;
    struct rx_vde_exclusive candidate = { .aio=aio, .mapped=mapped,
        .pa=pa, .capacity=bytes, .mem_id=mem_id, .epoch=epoch, .bound=1, .aio_enable_issued=1 };
    uint32_t pointer;
    /* 检查原 TX 使用的 Q0 状态。 */
    if (rx_vde_bcm(RA_AVIO_BCM_ERROR) ||
        (rx_vde_bcm(RA_AVIO_BCM_AUTOPUSH) & (1u | RX_VDE_QUEUE_MASK)) ||
        (rx_vde_bcm(RA_AVIO_BCM_EMP_STS) & (1u | RX_VDE_QUEUE_MASK)) != (1u | RX_VDE_QUEUE_MASK) ||
        !rx_vde_aio_idle(&candidate, &pointer)) return TEE_ERROR_BUSY;
    uint32_t again;
    rx_vde_barrier();
    if (!rx_vde_aio_idle(&candidate,&again) || again!=pointer) return TEE_ERROR_BUSY;
    candidate.saved_q5 = rx_vde_bcm(RA_AVIO_BCM_Q5);
    *v = candidate;
    return TEE_SUCCESS;
}
static inline void rx_vde_append(uint32_t *words, unsigned *pairs,
                                 uint32_t address, uint32_t value)
{ words[2 * *pairs] = value; words[2 * *pairs + 1] = address; ++*pairs; }
static inline TEE_Result rx_vde_exclusive_build(struct rx_vde_exclusive *v,
    HDL_dhub2d *rx, uint32_t frame_pa, const struct rx_dma_plan *dma, uint64_t token)
{
    T64b temporary[32] = {{0}};
    if (!v || !v->bound || v->prepared || v->armed || v->stopping || !rx || !dma || !token ||
        frame_pa < RX_STREAM_CMA_BEGIN || (frame_pa & 63u) ||
        (uint64_t)frame_pa + dma->transferred_bytes > RX_STREAM_CMA_END)
        return TEE_ERROR_BAD_PARAMETERS;
    /* The actual SDK emits (value,address), including 2ND START 0/1. */
    unsigned n = hbo_queue_enable(dhub_hbo(&rx->dhub), 2 * CHANNEL, 1, temporary);
    n += hbo_queue_enable(dhub_hbo(&rx->dhub), 2 * CHANNEL + 1, 1, temporary + n);
    n += dhub_channel_enable(&rx->dhub, CHANNEL, 1, temporary + n);
    n += rx_dma_plan_apply(rx, CHANNEL, frame_pa, dma, temporary + n);
    if (!n || n > 32u || (n + 9u) * 8u > RX_VDE_TABLE_CAPACITY)
        return TEE_ERROR_BAD_STATE;
    uint32_t *inner = v->mapped;
    TEE_MemMove(inner, temporary, n * 8u);
    const uint32_t offsets[3] = {
        RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr,
        RA_HDMIRX_PIPE_VIP_CTRL, RA_HDMIRX_PIPE_tg_ctrl
    };
    for (unsigned i=0; i<3; ++i) {
        rx_vde_append(inner, &n, PIPE + offsets[i], 2);
        rx_vde_append(inner, &n, PIPE + offsets[i], 0);
        rx_vde_append(inner, &n, PIPE + offsets[i], 1);
    }
    v->inner_bytes = n * 8u;
    dhub_channel_generate_cmd(&v->aio->dhub, RX_VDE_READ_CHANNEL,
        v->pa, (int)v->inner_bytes, 0, 0, 0, 1, (int *)v->inner_command);
    uint32_t *outer = (uint32_t *)v->mapped + RX_VDE_OUTER_OFFSET / sizeof(uint32_t);
    if (BCM_SCHED_PushCmd(BCM_SCHED_Q13, v->inner_command, outer) != 2)
        return TEE_ERROR_BAD_STATE;
    v->outer_bytes = 16u;
    dhub_channel_generate_cmd(&v->aio->dhub, RX_VDE_READ_CHANNEL,
        v->pa + RX_VDE_OUTER_OFFSET, (int)v->outer_bytes,
        0, 0, 0, 0, (int *)v->outer_command);
    TEE_Result r=TEE_CacheClean(v->mapped, RX_VDE_POOL_REQUIRED);
    if (r) return r;
    rx_vde_barrier();
    v->token=token; v->prepared=1;
    return TEE_SUCCESS;
}
static inline int rx_vde_rx_stopped_once(HDL_dhub2d *rx, const struct rx_stream_disable_record *issued)
{
    unsigned pointer;
    HDL_hbo *hbo=dhub_hbo(&rx->dhub);
    if (!issued || !issued->wr || !issued->channel || !issued->second ||
        (rx_vde_read32(PIPE + RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr) & 1u) ||
        ((dhub_channel_busy(&rx->dhub) | dhub_channel_pending(&rx->dhub) |
          dhub2d_channel_busy(rx)) & (1u << CHANNEL)) ||
        (hbo_queue_busy(hbo) & (3u << (2 * CHANNEL))) ||
        hbo_queue_query(hbo, 2 * CHANNEL, SemaQueryMap_ADDR_master_consumer, &pointer) ||
        hbo_queue_query(hbo, 2 * CHANNEL + 1, SemaQueryMap_ADDR_master_consumer, &pointer))
        return 0;
    return 1;
}
static inline int rx_vde_rx_stopped_for_rearm(HDL_dhub2d *rx,
    const struct rx_stream_disable_record *issued)
{
    if (!rx_vde_rx_stopped_once(rx,issued)) return 0;
    rx_vde_barrier();
    return rx_vde_rx_stopped_once(rx,issued);
}
static inline TEE_Result rx_vde_exclusive_publish(struct rx_vde_exclusive *v,
                                                  HDL_dhub2d *rx, int *pool_dma_armed,
                                                  struct rx_stream_disable_record *issued)
{
    uint32_t pointer;
    if (!v || !v->prepared || v->armed || v->stopping || !rx || !pool_dma_armed || !issued || *pool_dma_armed)
        return TEE_ERROR_BAD_STATE;
    if (!rx_vde_rx_stopped_for_rearm(rx,issued) || rx_vde_bcm(RA_AVIO_BCM_ERROR) ||
        (rx_vde_bcm(RA_AVIO_BCM_AUTOPUSH) & RX_VDE_QUEUE_MASK) ||
        (rx_vde_bcm(RA_AVIO_BCM_EMP_STS) & RX_VDE_QUEUE_MASK) != RX_VDE_QUEUE_MASK ||
        !rx_vde_aio_idle(v, &pointer) ||
        (semaphore_chk_full(dhub_semaphore(&v->aio->dhub), -1) & RX_VDE_READ_MASK) ||
        semaphore_query(dhub_semaphore(&v->aio->dhub), RX_VDE_READ_CHANNEL,
                        SemaQueryMap_ADDR_master_consumer, NULL) ||
        (semaphore_chk_full(dhub_semaphore(&rx->dhub), -1) & (1u << CHANNEL)) ||
        semaphore_query(dhub_semaphore(&rx->dhub), CHANNEL,
                        SemaQueryMap_ADDR_master_consumer, NULL))
        return TEE_ERROR_BUSY;
    /* Pin state becomes retained before the first externally visible write. */
    *issued=(struct rx_stream_disable_record){0};
    v->armed=1; *pool_dma_armed=1;
    BCM_SCHED_SetMux(BCM_SCHED_Q5, 13);
    rx_vde_barrier();
    if ((rx_vde_bcm(RA_AVIO_BCM_Q5) & 0x3fu) != 13u) return TEE_ERROR_BAD_STATE;
    return BCM_SCHED_PushCmd(BCM_SCHED_Q5, v->outer_command, NULL) == 2 ?
        TEE_SUCCESS : TEE_ERROR_BUSY;
}
static inline TEE_Result rx_vde_exclusive_complete(struct rx_vde_exclusive *v,
                                                    HDL_dhub2d *rx, int *pool_dma_armed)
{
    uint32_t pointer, again;
    if (!v || !v->armed || !rx || !pool_dma_armed || !*pool_dma_armed) return TEE_ERROR_BAD_STATE;
    if (rx_vde_bcm(RA_AVIO_BCM_ERROR)) return TEE_ERROR_BAD_STATE;
    if ((rx_vde_bcm(RA_AVIO_BCM_AUTOPUSH) & RX_VDE_QUEUE_MASK) ||
        (rx_vde_bcm(RA_AVIO_BCM_EMP_STS) & RX_VDE_QUEUE_MASK) != RX_VDE_QUEUE_MASK ||
        !rx_vde_aio_idle(v, &pointer)) return TEE_ERROR_BUSY;
    /* 发布前确认 RX9 的最终写入发生在信号量耗尽之后，用于核对嵌套 WR/TG 命令的执行。 */
    if (!(semaphore_chk_full(dhub_semaphore(&rx->dhub), -1) & (1u << CHANNEL)) ||
        semaphore_query(dhub_semaphore(&rx->dhub), CHANNEL,
                        SemaQueryMap_ADDR_master_consumer, NULL) != 1)
        return TEE_ERROR_BUSY;
    /* AIO8 的 FIFO 为 128 字节，共 16 条命令；核对最后消费的是内部表。 */
    uint32_t address = AG_HBO_SRAM_BASE + 0x4a00u + ((pointer + 15u) % 16u) * 8u;
    if (rx_vde_read32(address) != v->inner_command[0] ||
        rx_vde_read32(address + 4u) != v->inner_command[1]) return TEE_ERROR_BUSY;
    rx_vde_barrier();
    if (!rx_vde_aio_idle(v, &again) || again != pointer) return TEE_ERROR_BUSY;
    HDL_semaphore *sem=dhub_semaphore(&v->aio->dhub);
    if (!(semaphore_chk_full(sem, -1) & RX_VDE_READ_MASK) ||
        semaphore_query(sem, RX_VDE_READ_CHANNEL, SemaQueryMap_ADDR_master_consumer, NULL) != 1)
        return TEE_ERROR_BUSY;
    semaphore_pop(sem, RX_VDE_READ_CHANNEL, 1);
    semaphore_clr_full(sem, RX_VDE_READ_CHANNEL);
    rx_vde_barrier();
    /* 命令读取结束后允许复用命令内存；像素槽由流接口处理完成和出队。 */
    v->armed=0; v->prepared=0; *pool_dma_armed=0;
    return TEE_SUCCESS;
}
static inline TEE_Result rx_vde_exclusive_stop(struct rx_vde_exclusive *v,
                                               HDL_dhub2d *rx, int *pool_dma_armed)
{
    if (!v || !v->bound || !rx || !pool_dma_armed ||
        (uint32_t)(*pool_dma_armed != 0) != v->armed) return TEE_ERROR_BAD_STATE;
    v->stopping=1;
    /* 尚未触发的 Q5 保持占用，等待嵌套 DMA 读取结束。 */
    if (v->armed) {
        TEE_Result r=rx_vde_exclusive_complete(v, rx, pool_dma_armed);
        if (r) return r;
    }
    uint32_t pointer;
    if (!rx_vde_aio_idle(v, &pointer) ||
        (rx_vde_bcm(RA_AVIO_BCM_EMP_STS) & RX_VDE_QUEUE_MASK) != RX_VDE_QUEUE_MASK)
        return TEE_ERROR_BUSY;
    v->aio_disable_issued=v->aio_second_disable_issued=v->aio_fifo_disable_issued=0;
    dhub2nd_channel_enable(v->aio,RX_VDE_READ_CHANNEL,0,NULL);
    dhub_channel_enable(&v->aio->dhub, RX_VDE_READ_CHANNEL, 0, NULL);
    HDL_hbo *hbo=dhub_hbo(&v->aio->dhub);
    hbo_queue_enable(hbo,2*RX_VDE_READ_CHANNEL,0,NULL);
    hbo_queue_enable(hbo,2*RX_VDE_READ_CHANNEL+1,0,NULL);
    rx_vde_barrier();
    v->aio_disable_issued=v->aio_second_disable_issued=v->aio_fifo_disable_issued=1;
    v->aio_enable_issued=0;
    uint32_t again;
    if (!rx_vde_aio_idle(v,&pointer)) return TEE_ERROR_BUSY;
    rx_vde_barrier();
    if (!v->aio_disable_issued || !v->aio_second_disable_issued || !v->aio_fifo_disable_issued ||
        !rx_vde_aio_idle(v,&again) || again!=pointer ||
        (rx_vde_bcm(RA_AVIO_BCM_AUTOPUSH)&RX_VDE_QUEUE_MASK) ||
        rx_vde_bcm(RA_AVIO_BCM_ERROR) ||
        (rx_vde_bcm(RA_AVIO_BCM_EMP_STS)&RX_VDE_QUEUE_MASK)!=RX_VDE_QUEUE_MASK)
        return TEE_ERROR_BUSY;
    v->bound=0;
    return TEE_SUCCESS;
}

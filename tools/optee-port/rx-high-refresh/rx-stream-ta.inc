/* SPDX-License-Identifier: GPL-2.0 */
#define RX_STREAM_TEE_BUILD 1
#include "rx-stream.c"
#include "rx-stream-command.h"
static struct rx_stream capture_stream;
static int stream_first_frame_started;
#ifndef RX_STREAM_DISABLE_RECORD_DEFINED
#define RX_STREAM_DISABLE_RECORD_DEFINED
struct rx_stream_disable_record { uint32_t tg, wr, channel, second; };
#endif
static struct rx_stream_disable_record stream_disable_issued;
static void stream_disable_barrier(void)
{
#ifndef RX_STREAM_HOST_TEST
    __asm__ volatile("dsb sy" ::: "memory");
#endif
}
static void stream_issue_disable(int stop_tg)
{
    /* 初值 0 表示尚未发出关闭命令，写入完成后更新记录。 */
    stream_disable_issued.wr=stream_disable_issued.channel=stream_disable_issued.second=0;
    if (stop_tg) { stream_disable_issued.tg=0; reg_write(RA_HDMIRX_PIPE_tg_ctrl,0); }
    reg_write(RA_HDMIRX_PIPE_VIP_WR+RA_WriteClient_Wr,0);
    dhub2nd_channel_enable(&capture_dhub,CHANNEL,0,NULL);
    dhub_channel_enable(&capture_dhub.dhub,CHANNEL,0,NULL);
    stream_disable_barrier();
    stream_disable_issued.wr=stream_disable_issued.channel=stream_disable_issued.second=1;
    if (stop_tg) stream_disable_issued.tg=1;
}
#ifdef RX_AIO_EXCLUSIVE_VERSION
#include "rx-stream-exclusive-ta.inc"
#else
static int stream_exclusive_open_allowed(void) { return 1; }
static int stream_exclusive_next_supported(void) { return 0; }
static int stream_exclusive_enabled(void) { return 0; }
static TEE_Result stream_exclusive_prepare(uint64_t epoch)
{ (void)epoch;return TEE_ERROR_NOT_SUPPORTED; }
static TEE_Result stream_exclusive_start_next(const struct rx_stream_reservation *r)
{ (void)r;return TEE_ERROR_NOT_SUPPORTED; }
static TEE_Result stream_exclusive_read_finish(void) { return TEE_SUCCESS; }
static TEE_Result stream_exclusive_stop(void) { return TEE_SUCCESS; }
static TEE_Result stream_exclusive_release(void) { return TEE_SUCCESS; }
static int stream_exclusive_can_close(void) { return 1; }
#endif
static void stream_idle_sample(struct rx_stream_idle_proof *p)
{
    HDL_hbo *hbo = dhub_hbo(&capture_dhub.dhub);
    unsigned pointer;
    *p = (struct rx_stream_idle_proof){0};
    p->producer_stopped = stream_disable_issued.tg && stream_disable_issued.wr &&
        !(reg_read(RA_HDMIRX_PIPE_tg_ctrl)&1u) &&
        !(reg_read(RA_HDMIRX_PIPE_VIP_WR+RA_WriteClient_Wr)&1u);
    /* START 为只写寄存器，关闭状态由本次命令记录。 */
    p->channel_disabled = stream_disable_issued.channel;
    p->second_dma_disabled = stream_disable_issued.second;
    p->busy = dhub_channel_busy(&capture_dhub.dhub) & MASK;
    p->pending = dhub_channel_pending(&capture_dhub.dhub) & MASK;
    p->second_dma_busy = dhub2d_channel_busy(&capture_dhub) & MASK;
    p->command_fifo_used = hbo_queue_query(hbo, 2 * CHANNEL,
        SemaQueryMap_ADDR_master_consumer, &pointer);
    p->data_fifo_used = hbo_queue_query(hbo, 2 * CHANNEL + 1,
        SemaQueryMap_ADDR_master_consumer, &pointer);
    uint32_t fifo = hbo_queue_busy(hbo);
    p->command_fifo_busy = (fifo >> (2 * CHANNEL)) & 1u;
    p->data_fifo_busy = (fifo >> (2 * CHANNEL + 1)) & 1u;
}
static void stream_idle_pair(struct rx_stream_idle_proof *p)
{
    struct rx_stream_idle_proof first;
    stream_idle_sample(&first);
    __asm__ volatile("dsb sy" ::: "memory");
    stream_idle_sample(p);
    if (!TEE_MemCompare(&first, p, sizeof(first))) p->stable_samples = 2;
}
static TEE_Result stream_service(void)
{
    if (capture_stream.blocked || capture_stream.stop_requested) return TEE_ERROR_BAD_STATE;
    if (capture_stream.active_slot != RX_STREAM_NO_SLOT) {
        uint32_t slot = capture_stream.active_slot;
        HDL_semaphore *sem = dhub_semaphore(&capture_dhub.dhub);
        if (!(semaphore_chk_full(sem, -1) & MASK)) return TEE_ERROR_BUSY;
        TEE_Result table_result=stream_exclusive_read_finish();
        if (table_result) return table_result;
        /* VDE 需要 TG 继续运行；复用缓冲前先停止写入。 */
        stream_issue_disable(!stream_exclusive_enabled());
        struct rx_stream_completion_proof proof = {0};
        proof.epoch = capture_stream.epoch; proof.slot = slot;
        proof.token = capture_stream.slots[slot].token;
        proof.fresh_final_write_credit = 1;
        stream_idle_pair(&proof.idle);
        if (stream_exclusive_enabled()) {
            /* 保持 TG 产生下一次 VDE 边沿，同时停止 WR 和 RX DMA；STOP 随后关闭 TG。 */
            struct rx_stream_idle_proof first=proof.idle;first.stable_samples=0;
            first.producer_stopped=stream_disable_issued.wr && !(reg_read(RA_HDMIRX_PIPE_VIP_WR+RA_WriteClient_Wr)&1u);
            __asm__ volatile("dsb sy" ::: "memory");
            stream_idle_sample(&proof.idle);
            proof.idle.producer_stopped=stream_disable_issued.wr && !(reg_read(RA_HDMIRX_PIPE_VIP_WR+RA_WriteClient_Wr)&1u);
            if (!TEE_MemCompare(&first,&proof.idle,sizeof(first))) proof.idle.stable_samples=2;
        }
        if (rx_stream_complete(&capture_stream, &proof)) return TEE_ERROR_BUSY;
        semaphore_pop(sem, CHANNEL, 1); semaphore_clr_full(sem, CHANNEL);
        __asm__ volatile("dsb sy" ::: "memory");
        return TEE_SUCCESS;
    }
    /* 后续帧使用 BCM Q5/VDE 调度；默认路径在此返回 NOT_SUPPORTED。 */
    if (stream_first_frame_started) {
        if (!stream_exclusive_enabled()) return TEE_ERROR_NOT_SUPPORTED;
        struct rx_stream_reservation next;
        if (rx_stream_reserve(&capture_stream,capture_stream.epoch,&next)) return TEE_ERROR_BUSY;
        TEE_Result result=stream_exclusive_start_next(&next);
#ifdef RX_AIO_EXCLUSIVE_VERSION
        enum rx_stream_start_outcome outcome=result ?
            (stream_exclusive_vde.armed ? RX_STREAM_START_UNKNOWN : RX_STREAM_NOT_STARTED) : RX_STREAM_STARTED;
#else
        enum rx_stream_start_outcome outcome=RX_STREAM_NOT_STARTED;
#endif
        if (rx_stream_start_result(&capture_stream,next.epoch,next.slot,next.token,outcome))
            return TEE_ERROR_BAD_STATE;
        return result;
    }
    if (!capture_geometry_configured || !capture_sync_enabled || active || busy())
        return TEE_ERROR_BAD_STATE;
    if (semaphore_chk_full(dhub_semaphore(&capture_dhub.dhub), -1) & MASK)
        return TEE_ERROR_BAD_STATE;
    struct rx_stream_reservation reservation;
    if (rx_stream_reserve(&capture_stream, capture_stream.epoch, &reservation))
        return TEE_ERROR_BUSY;
    stream_disable_issued=(struct rx_stream_disable_record){0};
    TEE_Result result = start(reservation.address, reservation.length, 0);
    int recorded = rx_stream_start_result(&capture_stream, reservation.epoch,
        reservation.slot, reservation.token,
        result ? RX_STREAM_START_UNKNOWN : RX_STREAM_STARTED);
    if (recorded) return TEE_ERROR_BAD_STATE;
    if (!result) stream_first_frame_started = 1;
    return result;
}
static TEE_Result stream_command(uint32_t command, uint32_t types, TEE_Param p[4])
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT, TEE_PARAM_TYPE_NONE,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) ||
        p[0].memref.size != sizeof(struct rx_stream_command) || !p[0].memref.buffer)
        return TEE_ERROR_BAD_PARAMETERS;
    struct rx_stream_command c;
    TEE_MemMove(&c, p[0].memref.buffer, sizeof(c));
    if (c.version != RX_STREAM_VERSION || c.size != sizeof(c) || c.flags ||
        c.acknowledgement || c.software_result || c.can_unregister || c.next_frame_supported)
        return TEE_ERROR_BAD_PARAMETERS;
    if (command == RX_STREAM_SERVICE_COMMAND && c.operation == RX_STREAM_OP_EXCLUSIVE_CAPABILITY) {
        if (!c.epoch) return TEE_ERROR_BAD_PARAMETERS;
        c.acknowledgement=RX_STREAM_ACK;
        c.next_frame_supported=(uint32_t)stream_exclusive_next_supported();
        TEE_MemMove(p[0].memref.buffer,&c,sizeof(c));
        return TEE_SUCCESS;
    }
    int result = 0; TEE_Result hardware_result = TEE_SUCCESS;
    if (command == RX_STREAM_OPEN_COMMAND) {
        prepare();
        if (c.operation != RX_STREAM_OP_OPEN || !stream_exclusive_open_allowed() || !capture_geometry_configured ||
            active || busy() || c.frame_bytes != capture_layout.bytes)
            return TEE_ERROR_BAD_STATE;
        result = rx_stream_open(&capture_stream, c.epoch, c.frame_bytes, c.count, c.buffers);
        if (!result) stream_first_frame_started = 0;
    } else {
        if (!capture_stream.opened || c.epoch != capture_stream.epoch) return TEE_ERROR_BAD_STATE;
        struct rx_stream_reservation r;
        switch (c.operation) {
        case RX_STREAM_OP_QUEUE:
            result = rx_stream_queue(&capture_stream, c.epoch, c.slot, &c.token); break;
        case RX_STREAM_OP_SERVICE: hardware_result = stream_service(); break;
        case RX_STREAM_OP_PREPARE_EXCLUSIVE: hardware_result=stream_exclusive_prepare(c.epoch);break;
        case RX_STREAM_OP_DEQUEUE:
            result = rx_stream_dequeue(&capture_stream, c.epoch, &r);
            if (!result) { c.slot = r.slot; c.token = r.token; } break;
        case RX_STREAM_OP_RELEASE:
            result = rx_stream_release(&capture_stream, c.epoch, c.slot, c.token); break;
        case RX_STREAM_OP_STOP: {
            result = rx_stream_stop_begin(&capture_stream, c.epoch);
            if (result) break;
            hardware_result = stream_exclusive_stop();
            if (!hardware_result) {
                /* 显式关闭当前流的 TG、写客户端和 DMA。 */
                stream_issue_disable(1);
                hardware_result = stop();
            }
            if (!hardware_result) hardware_result = stream_exclusive_release();
            if (!hardware_result) {
                struct rx_stream_stop_proof proof = { .epoch = c.epoch };
                stream_idle_pair(&proof.idle);
                result = rx_stream_stop_finish(&capture_stream, &proof);
            }
            break;
        }
        case RX_STREAM_OP_STATUS: break;
        case RX_STREAM_OP_CLOSE:
            if (!stream_exclusive_can_close()) hardware_result=TEE_ERROR_BUSY;
            else result = rx_stream_close(&capture_stream, c.epoch);
            break;
        default: return TEE_ERROR_BAD_PARAMETERS;
        }
    }
    c.software_result = result; c.acknowledgement = RX_STREAM_ACK;
    c.can_unregister = stream_exclusive_can_close() && rx_stream_can_unregister(&capture_stream, c.epoch);
    if (c.operation == RX_STREAM_OP_CLOSE && !result && !hardware_result) c.can_unregister = 1;
    c.next_frame_supported = (uint32_t)stream_exclusive_next_supported();
    TEE_MemMove(p[0].memref.buffer, &c, sizeof(c));
    if (hardware_result) return hardware_result;
    return result ? TEE_ERROR_BAD_STATE : TEE_SUCCESS;
}

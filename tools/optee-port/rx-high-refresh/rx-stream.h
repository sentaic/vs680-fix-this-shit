/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_STREAM_H
#define VS680_RX_STREAM_H
#include <stdint.h>
#define RX_STREAM_MAX_SLOTS 3u
#define RX_STREAM_NO_SLOT UINT32_MAX
#define RX_STREAM_CMA_BEGIN UINT64_C(0x75400000)
#define RX_STREAM_CMA_END UINT64_C(0x76400000)
/* Fixed negative errno values keep the TA independent of Linux libc headers. */
enum rx_stream_error {
    RX_STREAM_EINVAL = -22, RX_STREAM_ENODEV = -19, RX_STREAM_ESTALE = -116,
    RX_STREAM_EBUSY = -16, RX_STREAM_ENOSPC = -28, RX_STREAM_EADDRINUSE = -98,
    RX_STREAM_EAGAIN = -11, RX_STREAM_EOVERFLOW = -75,
    RX_STREAM_EALREADY = -114, RX_STREAM_EPROTO = -71
};
enum rx_stream_slot_state {
    RX_STREAM_FREE = 0, RX_STREAM_QUEUED, RX_STREAM_ACTIVE,
    RX_STREAM_DONE, RX_STREAM_ACQUIRED, RX_STREAM_RETAINED
};
enum rx_stream_start_outcome {
    RX_STREAM_STARTED = 0,
    RX_STREAM_NOT_STARTED = 1,
    RX_STREAM_START_UNKNOWN = 2
};
struct rx_stream_buffer { uint64_t address, length; };
struct rx_stream_slot {
    struct rx_stream_buffer buffer;
    uint64_t epoch, token;
    enum rx_stream_slot_state state;
};
struct rx_stream_reservation {
    uint32_t slot;
    uint64_t epoch, token, address, length;
};
/* 硬件适配器提供关闭状态读回和稳定采样次数；stable_samples 从停止生产者后开始计数。 */
struct rx_stream_idle_proof {
    uint32_t producer_stopped, channel_disabled, second_dma_disabled;
    uint32_t busy, pending, second_dma_busy;
    uint32_t command_fifo_used, data_fifo_used;
    uint32_t command_fifo_busy, data_fifo_busy, stable_samples;
};
struct rx_stream_completion_proof {
    uint64_t epoch, token;
    uint32_t slot, fresh_final_write_credit;
    struct rx_stream_idle_proof idle;
};
struct rx_stream_stop_proof {
    uint64_t epoch;
    struct rx_stream_idle_proof idle;
};
/* 对象首次使用时清零。close 后保留 last_epoch 和 last_token，使后续会话继续使用新的编号。 */
struct rx_stream {
    uint64_t epoch, last_epoch, last_token, frame_bytes;
    uint32_t opened, slot_count, active_slot, start_confirmed;
    uint32_t blocked, stop_requested, stopped;
    struct rx_stream_slot slots[RX_STREAM_MAX_SLOTS];
};
int rx_stream_open(struct rx_stream *, uint64_t epoch, uint64_t frame_bytes,
                   uint32_t count, const struct rx_stream_buffer *);
int rx_stream_queue(struct rx_stream *, uint64_t epoch, uint32_t slot, uint64_t *token);
/* Select the oldest queued token and reserve ACTIVE before any MMIO. */
int rx_stream_reserve(struct rx_stream *, uint64_t epoch, struct rx_stream_reservation *);
int rx_stream_start_result(struct rx_stream *, uint64_t epoch, uint32_t slot,
                           uint64_t token, enum rx_stream_start_outcome);
int rx_stream_complete(struct rx_stream *, const struct rx_stream_completion_proof *);
int rx_stream_dequeue(struct rx_stream *, uint64_t epoch, struct rx_stream_reservation *);
int rx_stream_release(struct rx_stream *, uint64_t epoch, uint32_t slot, uint64_t token);
/* Call begin before touching stop controls. A failed/unknown stop retains DMA
 * slots and blocks Q/DQ. Readers may release, but unregister remains blocked. */
int rx_stream_stop_begin(struct rx_stream *, uint64_t epoch);
int rx_stream_stop_finish(struct rx_stream *, const struct rx_stream_stop_proof *);
/* 硬件停止且全部读取者释放后返回 1，否则返回 0。 */
int rx_stream_can_unregister(const struct rx_stream *, uint64_t epoch);
int rx_stream_close(struct rx_stream *, uint64_t epoch);
#endif

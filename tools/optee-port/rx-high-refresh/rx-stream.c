/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-stream.h"
#include <string.h>
static int rx_stream_session(const struct rx_stream *s, uint64_t epoch)
{
    if (!s) return RX_STREAM_EINVAL;
    if (!s->opened) return RX_STREAM_ENODEV;
    return epoch == s->epoch ? 0 : RX_STREAM_ESTALE;
}
static int rx_stream_idle(const struct rx_stream_idle_proof *p)
{
    return p->producer_stopped == 1 && p->channel_disabled == 1 &&
        p->second_dma_disabled == 1 && p->stable_samples >= 2 &&
        !p->busy && !p->pending && !p->second_dma_busy &&
        !p->command_fifo_used && !p->data_fifo_used &&
        !p->command_fifo_busy && !p->data_fifo_busy;
}
static void rx_stream_fill_reservation(const struct rx_stream *s, unsigned slot,
                        struct rx_stream_reservation *out)
{
    const struct rx_stream_slot *p = &s->slots[slot];
    *out = (struct rx_stream_reservation){ .slot = slot, .epoch = p->epoch,
        .token = p->token, .address = p->buffer.address, .length = p->buffer.length };
}
int rx_stream_open(struct rx_stream *s, uint64_t epoch, uint64_t bytes,
                   uint32_t count, const struct rx_stream_buffer *buffers)
{
    if (!s || !buffers || !epoch || !bytes || !count || count > RX_STREAM_MAX_SLOTS)
        return RX_STREAM_EINVAL;
    if (s->opened) return RX_STREAM_EBUSY;
    if (epoch <= s->last_epoch) return RX_STREAM_ESTALE;
    if (bytes > (RX_STREAM_CMA_END - RX_STREAM_CMA_BEGIN) / count) return RX_STREAM_ENOSPC;
    struct rx_stream next = { .epoch = epoch, .last_epoch = epoch,
        .last_token = s->last_token, .frame_bytes = bytes, .opened = 1,
        .slot_count = count, .active_slot = RX_STREAM_NO_SLOT };
    for (unsigned i = 0; i < count; i++) {
        uint64_t address = buffers[i].address, length = buffers[i].length;
        if ((address & 63u) || length < bytes) return RX_STREAM_EINVAL;
        if (address < RX_STREAM_CMA_BEGIN || address >= RX_STREAM_CMA_END ||
            length > RX_STREAM_CMA_END - address) return RX_STREAM_ENOSPC;
        for (unsigned j = 0; j < i; j++)
            if (address < buffers[j].address + buffers[j].length &&
                buffers[j].address < address + length) return RX_STREAM_EADDRINUSE;
        next.slots[i].buffer = buffers[i];
        next.slots[i].epoch = epoch;
        next.slots[i].state = RX_STREAM_FREE;
    }
    *s = next;
    return 0;
}
int rx_stream_queue(struct rx_stream *s, uint64_t epoch, uint32_t slot, uint64_t *token)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (!token || slot >= s->slot_count) return RX_STREAM_EINVAL;
    if (s->blocked) return RX_STREAM_EBUSY;
    if (s->slots[slot].state != RX_STREAM_FREE) return RX_STREAM_EBUSY;
    if (s->last_token == UINT64_MAX) return RX_STREAM_EOVERFLOW;
    struct rx_stream_slot *p = &s->slots[slot];
    p->token = ++s->last_token; p->epoch = epoch; p->state = RX_STREAM_QUEUED;
    *token = p->token;
    return 0;
}
int rx_stream_reserve(struct rx_stream *s, uint64_t epoch, struct rx_stream_reservation *out)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (!out) return RX_STREAM_EINVAL;
    if (s->blocked || s->active_slot != RX_STREAM_NO_SLOT) return RX_STREAM_EBUSY;
    unsigned selected = RX_STREAM_NO_SLOT;
    for (unsigned i = 0; i < s->slot_count; i++)
        if (s->slots[i].state == RX_STREAM_QUEUED &&
            (selected == RX_STREAM_NO_SLOT || s->slots[i].token < s->slots[selected].token))
            selected = i;
    if (selected == RX_STREAM_NO_SLOT) return RX_STREAM_EAGAIN;
    s->slots[selected].state = RX_STREAM_ACTIVE;
    s->active_slot = selected; s->start_confirmed = 0;
    rx_stream_fill_reservation(s, selected, out);
    return 0;
}
int rx_stream_start_result(struct rx_stream *s, uint64_t epoch, uint32_t slot,
                           uint64_t token, enum rx_stream_start_outcome outcome)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (slot >= s->slot_count || outcome < RX_STREAM_STARTED || outcome > RX_STREAM_START_UNKNOWN)
        return RX_STREAM_EINVAL;
    if (s->slots[slot].token != token || !token) return RX_STREAM_ESTALE;
    if (s->blocked || s->active_slot != slot || s->slots[slot].state != RX_STREAM_ACTIVE)
        return RX_STREAM_EBUSY;
    if (outcome == RX_STREAM_START_UNKNOWN) return rx_stream_stop_begin(s, epoch);
    if (s->start_confirmed) return RX_STREAM_EALREADY;
    if (outcome == RX_STREAM_STARTED) { s->start_confirmed = 1; return 0; }
    s->slots[slot].state = RX_STREAM_QUEUED;
    s->active_slot = RX_STREAM_NO_SLOT;
    return 0;
}
int rx_stream_complete(struct rx_stream *s, const struct rx_stream_completion_proof *proof)
{
    if (!proof) return RX_STREAM_EINVAL;
    int result = rx_stream_session(s, proof->epoch);
    if (result) return result;
    if (proof->slot >= s->slot_count) return RX_STREAM_EINVAL;
    if (!proof->token || proof->token != s->slots[proof->slot].token) return RX_STREAM_ESTALE;
    if (s->blocked) return RX_STREAM_EBUSY;
    if (s->active_slot != proof->slot || s->slots[proof->slot].state != RX_STREAM_ACTIVE)
        return RX_STREAM_ESTALE;
    if (!s->start_confirmed || proof->fresh_final_write_credit != 1 || !rx_stream_idle(&proof->idle))
        return RX_STREAM_EAGAIN;
    s->slots[proof->slot].state = RX_STREAM_DONE;
    s->active_slot = RX_STREAM_NO_SLOT; s->start_confirmed = 0;
    return 0;
}
int rx_stream_dequeue(struct rx_stream *s, uint64_t epoch, struct rx_stream_reservation *out)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (!out) return RX_STREAM_EINVAL;
    if (s->blocked) return RX_STREAM_EBUSY;
    unsigned selected = RX_STREAM_NO_SLOT;
    for (unsigned i = 0; i < s->slot_count; i++)
        if (s->slots[i].state == RX_STREAM_DONE &&
            (selected == RX_STREAM_NO_SLOT || s->slots[i].token < s->slots[selected].token))
            selected = i;
    if (selected == RX_STREAM_NO_SLOT) return RX_STREAM_EAGAIN;
    s->slots[selected].state = RX_STREAM_ACQUIRED;
    rx_stream_fill_reservation(s, selected, out);
    return 0;
}
int rx_stream_release(struct rx_stream *s, uint64_t epoch, uint32_t slot, uint64_t token)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (slot >= s->slot_count) return RX_STREAM_EINVAL;
    if (!token || s->slots[slot].token != token) return RX_STREAM_ESTALE;
    if (s->slots[slot].state != RX_STREAM_ACQUIRED) return RX_STREAM_EPROTO;
    s->slots[slot].state = s->blocked && !s->stopped ? RX_STREAM_RETAINED : RX_STREAM_FREE;
    return 0;
}
int rx_stream_stop_begin(struct rx_stream *s, uint64_t epoch)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (s->stopped || s->stop_requested) return 0;
    s->blocked = 1; s->stop_requested = 1;
    for (unsigned i = 0; i < s->slot_count; i++)
        if (s->slots[i].state != RX_STREAM_FREE && s->slots[i].state != RX_STREAM_ACQUIRED)
            s->slots[i].state = RX_STREAM_RETAINED;
    return 0;
}
int rx_stream_stop_finish(struct rx_stream *s, const struct rx_stream_stop_proof *proof)
{
    if (!proof) return RX_STREAM_EINVAL;
    int result = rx_stream_session(s, proof->epoch);
    if (result) return result;
    if (s->stopped) return 0;
    if (!s->stop_requested) return RX_STREAM_EPROTO;
    if (!rx_stream_idle(&proof->idle)) return RX_STREAM_EAGAIN;
    for (unsigned i = 0; i < s->slot_count; i++)
        if (s->slots[i].state != RX_STREAM_ACQUIRED) s->slots[i].state = RX_STREAM_FREE;
    s->active_slot = RX_STREAM_NO_SLOT; s->start_confirmed = 0;
    s->stopped = 1; s->stop_requested = 0;
    return 0;
}
int rx_stream_can_unregister(const struct rx_stream *s, uint64_t epoch)
{
    int result = rx_stream_session(s, epoch);
    if (result) return result;
    if (!s->stopped || s->active_slot != RX_STREAM_NO_SLOT) return 0;
    for (unsigned i = 0; i < s->slot_count; i++)
        if (s->slots[i].state != RX_STREAM_FREE) return 0;
    return 1;
}
int rx_stream_close(struct rx_stream *s, uint64_t epoch)
{
    int result = rx_stream_can_unregister(s, epoch);
    if (result < 0) return result;
    if (!result) return RX_STREAM_EBUSY;
    uint64_t last_epoch = s->last_epoch, last_token = s->last_token;
    memset(s, 0, sizeof(*s));
    s->last_epoch = last_epoch; s->last_token = last_token;
    return 0;
}

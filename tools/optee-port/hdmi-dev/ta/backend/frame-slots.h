/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_FRAME_SLOTS_H
#define HDMI_DEV_FRAME_SLOTS_H
#include <tee_internal_api.h>
#define HDMI_FRAME_NONE 3u
struct hdmi_frame_slots { unsigned current, pending, loan, active, queued, capacity; };
/* queued 保存已排队的源，pending 保存 CPU 预填的帧。 */
static inline void hdmi_frame_slots_init_count(struct hdmi_frame_slots *s,unsigned capacity)
{ *s=(struct hdmi_frame_slots){3,3,3,3,3,capacity}; }
static inline void hdmi_frame_slots_init(struct hdmi_frame_slots *s)
{ hdmi_frame_slots_init_count(s,2); }
static inline TEE_Result hdmi_frame_slots_begin(struct hdmi_frame_slots *s,unsigned *slot)
{
    if (!s || !slot || s->capacity<2 || s->capacity>3) return TEE_ERROR_BAD_PARAMETERS;
    if (s->pending!=HDMI_FRAME_NONE || s->loan!=HDMI_FRAME_NONE) return TEE_ERROR_BUSY;
    for (unsigned n=0;n<s->capacity;++n) if (n!=s->current && n!=s->active && n!=s->queued) {
        s->loan=*slot=n;return TEE_SUCCESS;
    }
    return TEE_ERROR_BUSY;
}
static inline TEE_Result hdmi_frame_slots_first_started(struct hdmi_frame_slots *s)
{
    if (!s || s->loan>=s->capacity || s->current!=HDMI_FRAME_NONE ||
        s->active!=HDMI_FRAME_NONE || s->queued!=HDMI_FRAME_NONE) return TEE_ERROR_BAD_STATE;
    s->current=s->active=s->loan;s->loan=HDMI_FRAME_NONE;return TEE_SUCCESS;
}
static inline TEE_Result hdmi_frame_slots_pending(struct hdmi_frame_slots *s)
{
    if (!s || s->loan>=s->capacity || s->current>=s->capacity) return TEE_ERROR_BAD_STATE;
    if (s->pending!=HDMI_FRAME_NONE || s->loan==s->current || s->loan==s->active ||
        s->loan==s->queued) return TEE_ERROR_BUSY;
    s->pending=s->loan;s->loan=HDMI_FRAME_NONE;return TEE_SUCCESS;
}
/* 实际新表排队成功后才转移pending；失败保留原槽。 */
static inline TEE_Result hdmi_frame_slots_queued(struct hdmi_frame_slots *s,unsigned slot)
{
    if (!s || slot>=s->capacity || s->queued!=HDMI_FRAME_NONE || s->loan!=HDMI_FRAME_NONE ||
        slot!=(s->pending<s->capacity ? s->pending : s->current)) return TEE_ERROR_BAD_STATE;
    s->queued=slot;
    if (s->pending==slot) s->pending=HDMI_FRAME_NONE;
    return TEE_SUCCESS;
}
/* 核对 DMA 完成、新命令表和源槽后释放旧引用。 */
static inline TEE_Result hdmi_frame_slots_read_completed(struct hdmi_frame_slots *s,unsigned slot)
{
    if (!s || slot>=s->capacity || s->active!=slot) return TEE_ERROR_BAD_STATE;
    s->active=HDMI_FRAME_NONE;return TEE_SUCCESS;
}
static inline TEE_Result hdmi_frame_slots_choose(const struct hdmi_frame_slots *s,unsigned *slot)
{
    if (!s || !slot) return TEE_ERROR_BAD_PARAMETERS;
    if (s->active!=HDMI_FRAME_NONE) return TEE_ERROR_BUSY;
    if (s->current>=s->capacity) return TEE_ERROR_BAD_STATE;
    *slot=s->queued<s->capacity ? s->queued : s->pending<s->capacity ? s->pending : s->current;
    return TEE_SUCCESS;
}
static inline TEE_Result hdmi_frame_slots_started(struct hdmi_frame_slots *s,unsigned slot)
{
    unsigned chosen;TEE_Result result=hdmi_frame_slots_choose(s,&chosen);
    if (result) return result;
    if (slot!=chosen) return TEE_ERROR_BAD_STATE;
    s->current=s->active=slot;
    if (s->pending==slot) s->pending=HDMI_FRAME_NONE;
    s->queued=HDMI_FRAME_NONE;
    return TEE_SUCCESS;
}
#endif

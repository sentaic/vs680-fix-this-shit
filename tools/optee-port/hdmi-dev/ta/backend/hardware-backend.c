/* SPDX-License-Identifier: GPL-2.0 */
#include "../backend.h"
#include "../show-timing.h"
#include "../switch-timing.h"
#include "avio-hardware.h"
#include "dma-pool.h"
#include "gfx-dma.h"
#include "mode-access.h"
#include "mode-720p.h"
#include "frame-start.h"
#include "frame-slots.h"
#include "overlay.h"
#include "tx-mode-adapter.h"
#include "ddc-diagnostic.h"
#include "tx-stop.h"
#include <tee_api.h>
#include <tee_api_syna.h>

#define RECORD_MAGIC UINT32_C(0x48444731)
#define NO_SLOT UINT32_MAX
struct backend_record {
    uint32_t magic, version, pa, bytes;
    uint32_t state, error, displayed_slot, flags;
    uint64_t read_completed, submitted;
};
static struct {
    struct hdmi_avio_registers registers;
    struct hdmi_tx_mode_adapter mode;
    bool parameterized;
    uint32_t requested_slots, requested_pip;
    struct hdmi_mode_access access;
    struct hdmi_mode_io io;
    struct hdmi_dma_pool pool;
    struct hdmi_gfx_dma gfx;
    struct hdmi_gfx_dma next_gfx, next_overlay;
    struct hdmi_overlay overlay;
    struct hdmi_frame_start first_start;
    struct backend_record *record;
    struct hdmi_frame_slots slots;
    uint64_t submission;
    unsigned next_slot, next_overlay_slot;
    bool first_applied, transition, mixer_dirty;
    bool owns_hardware, stop_verified, stopped;
    struct hdmi_ddc_owner ddc_owner;
} backend;

static bool valid_record(void)
{
    struct backend_record *record = backend.record;
    return record && record->magic == RECORD_MAGIC && record->version == 1u &&
        record->pa == backend.pool.pa && record->bytes == backend.pool.bytes &&
        record->state <= HDMI_DEV_FAULT &&
        !(record->flags & ~HDMI_DEV_DMA_QUIESCENT) &&
        (record->displayed_slot == NO_SLOT || record->displayed_slot < hdmi_dma_pool_frame_count(&backend.pool));
}
static TEE_Result publish(uint32_t state, TEE_Result error, uint32_t flags)
{
    if (!valid_record()) return TEE_ERROR_BAD_STATE;
    backend.record->state = state;
    backend.record->error = error;
    backend.record->flags = flags;
    return TEE_CacheClean(backend.pool.mapped, HDMI_DMA_POOL_ALIGN);
}
static TEE_Result failed(TEE_Result result)
{
    if (valid_record()) {
        TEE_Result saved = publish(HDMI_DEV_FAULT, result, 0);
        if (saved) return saved;
    }
    return result;
}

TEE_Result hdmi_dev_backend_ddc_snapshot(struct hdmi_dev_ddc_snapshot *out)
{
    if(!out)return TEE_ERROR_BAD_PARAMETERS;
    if(!backend.owns_hardware||!valid_record()||backend.stopped||
       (backend.record->state!=HDMI_DEV_READY&&
        backend.record->state!=HDMI_DEV_DISPLAYING))return TEE_ERROR_BAD_STATE;
    return hdmi_ddc_snapshot(&backend.io,out);
}

TEE_Result hdmi_dev_backend_ddc_read(struct hdmi_dev_ddc_read *out)
{
    if(!out)return TEE_ERROR_BAD_PARAMETERS;
    if(!backend.owns_hardware||!valid_record()||backend.stopped||
       (backend.record->state!=HDMI_DEV_READY&&
        backend.record->state!=HDMI_DEV_DISPLAYING))return TEE_ERROR_BAD_STATE;
    return hdmi_ddc_read_current(&backend.io,&backend.ddc_owner,out);
}

TEE_Result hdmi_dev_backend_ddc_prepare(struct hdmi_dev_ddc_prepare *out)
{
    if(!out)return TEE_ERROR_BAD_PARAMETERS;
    if(!backend.owns_hardware||!valid_record()||backend.stopped||
       (backend.record->state!=HDMI_DEV_READY&&
        backend.record->state!=HDMI_DEV_DISPLAYING))return TEE_ERROR_BAD_STATE;
    return hdmi_ddc_prepare_owner(&backend.io,&backend.ddc_owner,out);
}

TEE_Result hdmi_dev_backend_ddc_recover(struct hdmi_dev_ddc_recovery *out)
{
    if(!out)return TEE_ERROR_BAD_PARAMETERS;
    if(!backend.owns_hardware||!valid_record()||backend.stopped)
        return TEE_ERROR_BAD_STATE;
    return hdmi_ddc_recover_current(&backend.io,&backend.ddc_owner,out);
}

TEE_Result hdmi_dev_backend_preflight_mode(const struct hdmi_dev_mode_request *q,
    struct hdmi_dev_mode_preflight *out)
{
    struct hdmi_tx_mode_adapter selected={0};
    struct hdmi_dma_layout layout;
    if(!q || !out)return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result result=hdmi_tx_mode_validate(q,&selected);
    if(result)return result;
    /* 当前缺少 SFR 时钟查询适配；需要 SCDC 的模式返回 NOT_SUPPORTED。 */
    if(selected.requires_scdc)return TEE_ERROR_NOT_SUPPORTED;
    result=hdmi_dma_pool_plan_layout(q->pool_bytes,q->timing.hdisplay,
        q->timing.vdisplay,q->timing.hdisplay*3u,q->main_slots,q->pip_enabled,&layout);
    if(result)return result;
    _Static_assert(sizeof(*out)==184,"preflight reply ABI");
    struct hdmi_dev_mode_preflight reply={
        .version=HDMI_DEV_PREFLIGHT_VERSION,.size=sizeof(*out),.request=*q,
        .width=layout.width,.height=layout.height,.stride=layout.stride,
        .image_bytes=layout.image_bytes,.frame_base=layout.frame_base,
        .frame_slot_bytes=layout.slot_bytes,.main_slots=layout.slots,.total_bytes=layout.total_bytes,
        .pip_slot_bytes=layout.pip_slot_bytes,
        .pip_offset={layout.pip_offset[0],layout.pip_offset[1]},
        .command_bytes=layout.command_bytes,.flags=HDMI_DEV_PREFLIGHT_PURE,
    };
    *out=reply;return TEE_SUCCESS;
}

static TEE_Result backend_initialize(uint32_t pool_pa, uint32_t pool_bytes,
                                      const struct hdmi_dev_mode_request *request)
{
    struct hdmi_avio_activity activity;
    TEE_Result result;
    if (!hdmi_dev_pool_valid(pool_pa, pool_bytes)) return TEE_ERROR_BAD_PARAMETERS;
    struct hdmi_tx_mode_adapter selected={0};
    if (request) {
        result=hdmi_tx_mode_validate(request,&selected);
        if(result)return result;
        if(selected.requires_scdc)return TEE_ERROR_NOT_SUPPORTED;
        struct hdmi_dma_layout preflight;
        result=hdmi_dma_pool_plan_layout(pool_bytes,request->timing.hdisplay,
            request->timing.vdisplay,request->timing.hdisplay*3u,
            request->main_slots,request->pip_enabled,&preflight);
        if(result)return result;
    }
    if (backend.pool.registered || backend.pool.mapped) return TEE_ERROR_BAD_STATE;
    backend.stopped = false;
    backend.stop_verified = false;
    result = hdmi_avio_map(&backend.registers);
    if (result) return result;
    result = hdmi_mode_access_bind(&backend.access,&backend.io);
    if(result)return result;
    result = hdmi_tx_require_idle(&backend.io,&backend.registers,&activity);
    if(result)return result;
    /* Linux holder 通过 B0 固定 DMA-BUF，TA 接收对应物理地址并建立映射。 */
    result = hdmi_dma_pool_map(&backend.pool, pool_pa, pool_bytes);
    if (result) return result;
    result = TEE_CacheInvalidate(backend.pool.mapped, HDMI_DMA_POOL_ALIGN);
    if (result) return result;
    backend.parameterized=request!=NULL;
    backend.mode=selected;
    backend.requested_slots=request ? request->main_slots : 3;
    backend.requested_pip=request ? request->pip_enabled : 0;
    backend.pool.main_three_slots=true;
    if(request) {
        result=hdmi_dma_pool_configure_layout(&backend.pool,request->timing.hdisplay,
            request->timing.vdisplay,request->timing.hdisplay*3u,request->main_slots,request->pip_enabled);
        if(result)return result;
    }
    backend.record = backend.pool.mapped;
    if (valid_record()) {
        backend.owns_hardware = true;
        if (backend.record->state != HDMI_DEV_UNINITIALIZED ||
            !(backend.record->flags & HDMI_DEV_DMA_QUIESCENT)) return TEE_ERROR_BUSY;
    }
    /* Q0 和显示读通道空闲后，初始化当前显示实例。 */
    *backend.record = (struct backend_record){
        .magic = RECORD_MAGIC, .version = 1u, .pa = pool_pa, .bytes = pool_bytes,
        .state = HDMI_DEV_UNINITIALIZED, .displayed_slot = NO_SLOT,
        .flags = HDMI_DEV_DMA_QUIESCENT,
    };
    backend.owns_hardware = true;
    result = publish(HDMI_DEV_UNINITIALIZED, 0, HDMI_DEV_DMA_QUIESCENT);
    if (result) return result;
    result = hdmi_mode_access_bind(&backend.access, &backend.io);
    if (result) return failed(result);
    hdmi_dma_pool_arm(&backend.pool);
    result = publish(HDMI_DEV_UNINITIALIZED, 0, 0);
    if (result) return result;
    result = backend.parameterized ? hdmi_tx_mode_prepare(&backend.mode,&backend.io,&backend.registers) :
        hdmi_mode_720p_configure(&backend.io);
    if (result) return failed(result);
    result = hdmi_frame_start_init(&backend.first_start, &backend.registers,
                                  &backend.io, &backend.pool);
    if (result) return failed(result);
    result = hdmi_gfx_dma_init_geometry(&backend.gfx, &backend.registers,hdmi_dma_pool_width(&backend.pool)*3u);
    if (result) return failed(result);
    hdmi_frame_slots_init_count(&backend.slots,hdmi_dma_pool_frame_count(&backend.pool));
    backend.submission=0;
    backend.first_applied=false;
    backend.transition=false;
    backend.mixer_dirty=false;
    /* 初始化当前会话的 DDC 状态，STOP 完成后允许下一会话接管。 */
    backend.ddc_owner=(struct hdmi_ddc_owner){.initialized=true};
    return publish(HDMI_DEV_READY, 0, 0);
}

TEE_Result hdmi_dev_backend_init(uint32_t pa,uint32_t bytes)
{ return backend_initialize(pa,bytes,NULL); }
TEE_Result hdmi_dev_backend_init_mode(const struct hdmi_dev_mode_request *q)
{
    if(!q)return TEE_ERROR_BAD_PARAMETERS;
    return backend_initialize(q->pool_pa,q->pool_bytes,q);
}

TEE_Result hdmi_dev_backend_begin_frame(struct hdmi_dev_frame *frame)
{
    void *mapped;
    uint32_t pa;
    unsigned slot;
    TEE_Result result;
    if (!frame) return TEE_ERROR_BAD_PARAMETERS;
    *frame=(struct hdmi_dev_frame){0};
    if (!valid_record() || !backend.owns_hardware || !backend.gfx.configured ||
        (backend.record->state!=HDMI_DEV_READY && backend.record->state!=HDMI_DEV_DISPLAYING))
        return TEE_ERROR_BAD_STATE;
    result=hdmi_frame_slots_begin(&backend.slots,&slot);
    if (result) return result;
    result=hdmi_dma_pool_frame(&backend.pool,slot,&mapped,&pa);
    if (result) return failed(result);
    frame->mapped=mapped; frame->capacity=hdmi_dma_pool_frame_stride(&backend.pool);
    frame->width=hdmi_dma_pool_width(&backend.pool);frame->height=hdmi_dma_pool_height(&backend.pool);
    frame->stride=hdmi_dma_pool_row_stride(&backend.pool);
    return TEE_SUCCESS;
}
TEE_Result hdmi_dev_backend_present(void)
{
    TEE_Result result;
    if (!valid_record() || !backend.owns_hardware || backend.slots.loan>=backend.slots.capacity)
        return TEE_ERROR_BAD_STATE;
    /* 发布 pending 槽前清理帧缓存，后续 SERVICE 复用该发布记录。 */
    hdmi_show_timing_mark(HDMI_SHOW_CLEAN_BEGIN);
    result=hdmi_dma_pool_clean_frame(&backend.pool,backend.slots.loan);
    hdmi_show_timing_mark(HDMI_SHOW_CLEAN_END);
    if (result) return failed(result);
    if (backend.record->state==HDMI_DEV_DISPLAYING) {
        result=hdmi_frame_slots_pending(&backend.slots);
        if (result) return result;
        return publish(HDMI_DEV_DISPLAYING,0,0);
    }
    if (backend.record->state!=HDMI_DEV_READY) return TEE_ERROR_BAD_STATE;
    result=hdmi_gfx_dma_submit(&backend.gfx,&backend.pool,backend.slots.loan,
                             &backend.submission);
    if (result) return failed(result);
    if (backend.overlay.configured) {
        result=hdmi_overlay_prepare(&backend.overlay,&backend.pool);
        if (result) return failed(result);
    }
    result=backend.parameterized ? hdmi_tx_mode_enable(&backend.mode) : hdmi_mode_720p_enable(&backend.io);
    if (result) return failed(result);
    result=hdmi_frame_start_queue(&backend.first_start,&backend.gfx);
    if (result) return failed(result);
    result=hdmi_frame_start_repeat(&backend.first_start,true);
    if (result) return failed(result);
    if (backend.overlay.configured) {
        result=hdmi_overlay_started(&backend.overlay);
        if (result) return failed(result);
    }
    result=hdmi_frame_slots_first_started(&backend.slots);
    if (result) return failed(result);
    backend.record->displayed_slot=backend.slots.current;
    backend.record->submitted++;
    return publish(HDMI_DEV_DISPLAYING,0,0);
}
static TEE_Result prepare_replacement(void)
{
    TEE_Result result;
    uint64_t next_submission;
    backend.next_slot=backend.slots.pending<backend.slots.capacity ? backend.slots.pending : backend.slots.current;
    if (backend.next_slot>=backend.slots.capacity || backend.slots.loan<backend.slots.capacity) return TEE_ERROR_BAD_STATE;
    /* 为下一帧编码候选描述符，继续保留当前硬件表和源槽。 */
    backend.next_gfx=backend.gfx;
    backend.next_gfx.active=0;
    result=hdmi_gfx_dma_prepare_repeat(&backend.next_gfx,&backend.pool,
        hdmi_dma_pool_frame_offset(&backend.pool,backend.next_slot),
        hdmi_dma_pool_row_stride(&backend.pool),hdmi_dma_pool_width(&backend.pool)*3u,
        hdmi_dma_pool_height(&backend.pool),&next_submission);
    if (result) return result;
    if (backend.overlay.configured) {
        const struct hdmi_dev_overlay *g=&backend.overlay.geometry;
        backend.next_overlay_slot=backend.overlay.slots.pending<2 ?
            backend.overlay.slots.pending : backend.overlay.slots.current;
        if (backend.next_overlay_slot>=2 || backend.overlay.slots.loan<2)
            return TEE_ERROR_BAD_STATE;
        backend.next_overlay=backend.overlay.dma;
        backend.next_overlay.active=0;
        result=hdmi_gfx_dma_prepare_repeat(&backend.next_overlay,&backend.pool,
            hdmi_dma_pool_pip_offset(&backend.pool,backend.next_overlay_slot),
            g->stride,g->width*4u,g->height,&next_submission);
        if (result) return result;
    }
    result=hdmi_frame_start_switch(&backend.first_start,&backend.next_gfx,
        backend.overlay.configured ? &backend.next_overlay : NULL);
    if (result) return result;
    result=hdmi_frame_slots_queued(&backend.slots,backend.next_slot);
    if (result) return result;
    hdmi_switch_timing_begin(backend.slots.current,backend.next_slot);
    backend.transition=true;
    backend.record->submitted++;
    return publish(HDMI_DEV_DISPLAYING,0,0);
}

static TEE_Result complete_replacement(void)
{
    uint64_t completed;
    unsigned slot;
    TEE_Result result=hdmi_frame_start_applied(&backend.first_start);
    if (result) return result;
    hdmi_switch_timing_mark(HDMI_SWITCH_APPLIED);
    result=hdmi_gfx_dma_source_matches(&backend.next_gfx,&backend.io);
    if (!result && backend.overlay.configured)
        result=hdmi_gfx_dma_source_matches(&backend.next_overlay,&backend.io);
    if (result) return result;
    hdmi_switch_timing_mark(HDMI_SWITCH_SOURCE_MATCHED);
    if (backend.slots.active<backend.slots.capacity) {
        slot=backend.slots.active;
        result=hdmi_gfx_dma_complete(&backend.gfx,&completed);
        if (result) return result;
        if (completed!=backend.submission) return TEE_ERROR_BAD_STATE;
        result=hdmi_frame_slots_read_completed(&backend.slots,slot);
        if (result) return result;
        backend.record->read_completed++;
    }
    if (backend.overlay.configured) {
        result=hdmi_overlay_complete(&backend.overlay);
        if (result) return result;
    }
    hdmi_switch_timing_mark(HDMI_SWITCH_OLD_READ_DONE);
    bool another=backend.slots.pending<backend.slots.capacity;
    if (!another) {
        result=hdmi_frame_start_repeat(&backend.first_start,true);
        if (result) return result==TEE_ERROR_BUSY ? TEE_ERROR_BAD_STATE : result;
    }
    result=hdmi_frame_slots_choose(&backend.slots,&slot);
    if (result) return result;
    if (slot!=backend.next_slot) return TEE_ERROR_BAD_STATE;
    if (backend.overlay.configured) {
        unsigned pip_slot;
        result=hdmi_frame_slots_choose(&backend.overlay.slots,&pip_slot);
        if (result) return result;
        if (pip_slot!=backend.next_overlay_slot) return TEE_ERROR_BAD_STATE;
        result=hdmi_frame_slots_started(&backend.overlay.slots,pip_slot);
        if (result) return result;
        backend.overlay.dma=backend.next_overlay;
        backend.overlay.submission=backend.overlay.dma.active;
        backend.first_start.overlay=&backend.overlay.dma;
    }
    result=hdmi_frame_slots_started(&backend.slots,slot);
    if (result) return result;
    backend.gfx=backend.next_gfx;
    backend.submission=backend.gfx.active;
    result=hdmi_frame_start_release_old(&backend.first_start);
    if (result) return result;
    hdmi_switch_timing_mark(HDMI_SWITCH_RELEASED);
    backend.transition=false;
    backend.mixer_dirty=false;
    backend.record->displayed_slot=backend.slots.current;
    result=publish(HDMI_DEV_DISPLAYING,0,0);
    if (result) return result;
    /* 预填图已就绪时直接排下一表，避免先重投当前表占据下一次VBI。 */
    return another ? prepare_replacement() : TEE_SUCCESS;
}

TEE_Result hdmi_dev_backend_service(void)
{
    if (!valid_record() || !backend.owns_hardware) return TEE_ERROR_BAD_STATE;
    if (backend.record->state==HDMI_DEV_READY) return TEE_ERROR_BUSY;
    if (backend.record->state!=HDMI_DEV_DISPLAYING) return TEE_ERROR_BAD_STATE;
    TEE_Result result;
    if (!backend.first_applied) {
        result=hdmi_frame_start_applied(&backend.first_start);
        if (!result) backend.first_applied=true;
    } else if (backend.transition) {
        result=complete_replacement();
    } else if (backend.slots.pending<backend.slots.capacity || backend.mixer_dirty ||
               (backend.overlay.configured && backend.overlay.slots.pending<2)) {
        result=prepare_replacement();
    } else {
        /* 没有新图时，BCM 在每次 VBI 重复当前命令表。 */
        result=hdmi_frame_start_applied(&backend.first_start);
        if (!result) result=TEE_ERROR_BUSY;
    }
    return result && result!=TEE_ERROR_BUSY ? failed(result) : result;
}

TEE_Result hdmi_dev_backend_blend(uint32_t background_rgb, uint32_t alpha_q12)
{
    if (background_rgb>0xffffffu || alpha_q12>4095u) return TEE_ERROR_BAD_PARAMETERS;
    if (!valid_record() || !backend.owns_hardware ||
        (backend.record->state!=HDMI_DEV_READY && backend.record->state!=HDMI_DEV_DISPLAYING))
        return TEE_ERROR_BAD_STATE;
    if (backend.transition) return TEE_ERROR_BUSY;
    TEE_Result rc=hdmi_frame_start_set_mixer(&backend.first_start,background_rgb,alpha_q12);
    if (!rc && backend.record->state==HDMI_DEV_DISPLAYING) backend.mixer_dirty=true;
    return rc;
}

TEE_Result hdmi_dev_backend_overlay(const struct hdmi_dev_overlay *g,
    const void *pixels,size_t bytes)
{
    if (!pixels || !hdmi_dev_overlay_valid(g,bytes)) return TEE_ERROR_BAD_PARAMETERS;
    if (!valid_record() || !backend.owns_hardware ||
        (backend.record->state!=HDMI_DEV_READY && backend.record->state!=HDMI_DEV_DISPLAYING))
        return TEE_ERROR_BAD_STATE;
    if (backend.transition) return TEE_ERROR_BUSY;
    if(bytes>hdmi_dma_pool_pip_slot_bytes(&backend.pool))return TEE_ERROR_BAD_PARAMETERS;
    uint32_t canvas_w=hdmi_dma_pool_width(&backend.pool),canvas_h=hdmi_dma_pool_height(&backend.pool);
    if(g->width>canvas_w || g->height>canvas_h || g->x>canvas_w-g->width ||
        g->y>canvas_h-g->height)return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result rc;
    if (!backend.overlay.configured) {
        if (backend.record->state!=HDMI_DEV_READY || backend.slots.current!=HDMI_FRAME_NONE ||
            backend.slots.loan!=HDMI_FRAME_NONE) return TEE_ERROR_BAD_STATE;
        /* PIP首次配置发生在任何SHOW之前，恢复原2MiB命令/叠加及双槽布局。 */
        if (backend.parameterized && !backend.requested_pip) return TEE_ERROR_NOT_SUPPORTED;
        if (!backend.parameterized) backend.pool.main_three_slots=false;
        hdmi_frame_slots_init_count(&backend.slots,hdmi_dma_pool_frame_count(&backend.pool));
        rc=hdmi_overlay_configure_geometry(&backend.overlay,&backend.registers,&backend.io,g,canvas_w,canvas_h);
        if (rc) return failed(rc);
        backend.first_start.overlay=&backend.overlay.dma;
    }
    return hdmi_overlay_set(&backend.overlay,&backend.pool,g,pixels,bytes);
}

TEE_Result hdmi_dev_backend_status(struct hdmi_dev_status *status)
{
    if (!status) return TEE_ERROR_BAD_PARAMETERS;
    *status = (struct hdmi_dev_status){0};
    /* 池尚未绑定时，返回当前软件状态。 */
    if (backend.stopped) {
        status->state = HDMI_DEV_UNINITIALIZED;
        status->flags = HDMI_DEV_DMA_QUIESCENT;
        return TEE_SUCCESS;
    }
    if (!valid_record()) return TEE_ERROR_BAD_STATE;
    status->state = backend.record->state;
    status->last_error = backend.record->error;
    status->flags = backend.record->flags;
    if (status->state == HDMI_DEV_READY || status->state == HDMI_DEV_DISPLAYING) {
        status->width = hdmi_dma_pool_width(&backend.pool);
        status->height = hdmi_dma_pool_height(&backend.pool);
        status->refresh_millihz = backend.parameterized ? backend.mode.refresh_millihz : HDMI_DEV_REFRESH_MILLIHZ;
    }
    return TEE_SUCCESS;
}

TEE_Result hdmi_dev_backend_stop(void)
{
    struct hdmi_avio_activity activity;
    TEE_Result result;
    if (backend.stopped) return TEE_SUCCESS;
    if (backend.ddc_owner.quarantined) return TEE_ERROR_NOT_SUPPORTED;
    /* 硬件停止后若注销失败，保留注销状态供后续重试。 */
    if (backend.stop_verified) goto unmap;
    if (!backend.owns_hardware || !valid_record()) return TEE_ERROR_BAD_STATE;
    result=hdmi_tx_stop(&backend.io,&backend.registers,&backend.first_start,&activity);
    if(result)return failed(result);
    if(backend.parameterized && backend.mode.requires_scdc) {
        result=hdmi_tx_mode_stop_link(&backend.mode);
        if(result)return failed(result);
    }
    result=hdmi_dma_pool_stop_complete(&backend.pool,&activity);
    if(result)return failed(result);
    backend.gfx = (struct hdmi_gfx_dma){0};
    backend.overlay = (struct hdmi_overlay){0};
    backend.first_start = (struct hdmi_frame_start){0};
    hdmi_frame_slots_init(&backend.slots);
    backend.submission=0;
    backend.next_gfx=(struct hdmi_gfx_dma){0};
    backend.next_overlay=(struct hdmi_gfx_dma){0};
    backend.first_applied=backend.transition=backend.mixer_dirty=false;
    backend.record->displayed_slot = NO_SLOT;
    result = publish(HDMI_DEV_UNINITIALIZED, 0, HDMI_DEV_DMA_QUIESCENT);
    if (result) return result;
    backend.stop_verified = true;
    backend.record = NULL;
unmap:
    result = hdmi_dma_pool_unmap(&backend.pool);
    if (result) return result;
    backend.owns_hardware = false;
    backend.stopped = true;
    return TEE_SUCCESS;
}

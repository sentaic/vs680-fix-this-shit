/* SPDX-License-Identifier: GPL-2.0 */
#include "tx-mode-adapter.h"
#include "mode-720p.h"
#include "tx-stop.h"
#include <string.h>
#include "Galois_memmap.h"
#include "avio_memmap.h"
#include "avio.h"
static TEE_Result map_error(enum hdmi_display_result e, int callback)
{
    if(e==HDMI_DISPLAY_OK && !callback)return TEE_SUCCESS;
    if (callback) return (TEE_Result)(uint32_t)callback;
    if (e==HDMI_DISPLAY_BAD_ARGUMENT || e==HDMI_DISPLAY_BAD_TIMING ||
        e==HDMI_DISPLAY_REGISTER_RANGE) return TEE_ERROR_BAD_PARAMETERS;
    if (e==HDMI_DISPLAY_OUTPUT_ACTIVE) return TEE_ERROR_BUSY;
    if (e==HDMI_DISPLAY_READBACK_MISMATCH) return TEE_ERROR_BAD_STATE;
    return TEE_ERROR_NOT_SUPPORTED;
}
TEE_Result hdmi_tx_mode_validate(const struct hdmi_dev_mode_request *q,
                                 struct hdmi_tx_mode_adapter *out)
{
    if (!q || !out || q->version!=HDMI_DEV_MODE_VERSION || q->size!=sizeof(*q) ||
        !hdmi_dev_pool_valid(q->pool_pa,q->pool_bytes) ||
        q->main_slots<2 || q->main_slots>3 || q->pip_enabled>1 ||
        q->vpp_blanking>HDMI_VPP_BLANKING_PANEL || (q->caps_bits&~15u) ||
        !q->timing.hdisplay || q->timing.hdisplay>2560 ||
        !q->timing.vdisplay || q->timing.vdisplay>1440 ||
        ((q->timing.hdisplay*3u)&15u)) return TEE_ERROR_BAD_PARAMETERS;
    for (unsigned i=0;i<4;i++) if(q->reserved[i]) return TEE_ERROR_BAD_PARAMETERS;
    struct hdmi_tx_mode_adapter a={0};
    _Static_assert(sizeof(a.timing)==sizeof(q->timing),"detailed timing layout");
    memcpy(&a.timing,&q->timing,sizeof(a.timing));
    a.caps=(struct hdmi_display_link_caps){q->source_max_tmds_hz,q->sink_max_tmds_hz,
        (q->caps_bits&HDMI_DEV_MODE_SINK_HDMI)!=0,
        (q->caps_bits&HDMI_DEV_MODE_SINK_LISTED)!=0,
        (q->caps_bits&HDMI_DEV_MODE_SOURCE_SCDC)!=0,
        (q->caps_bits&HDMI_DEV_MODE_SINK_SCDC)!=0};
    a.blanking=(enum hdmi_vpp_blanking)q->vpp_blanking;
    struct hdmi_display_timing_info info;struct hdmi_display_clock_plan clock;
    struct hdmi_display_phy_plan phy;struct hdmi_display_link_plan link;
    enum hdmi_display_result e=hdmi_display_validate_timing(&a.timing,&info);
    if(e) return map_error(e,0);
    e=hdmi_display_plan_clock(a.timing.pixel_clock_hz,&clock);if(e)return map_error(e,0);
    TEE_Result result=hdmi_vpll_validate_plan(&clock);if(result)return result;
    e=hdmi_display_plan_phy_rgb8(a.timing.pixel_clock_hz,&phy);if(e)return map_error(e,0);
    e=hdmi_vpp_plan_timing_rgb24(&a.timing,a.blanking,&a.vpp_plan);if(e)return map_error(e,0);
    e=hdmi_display_plan_link_rgb8(&a.timing,&a.caps,&link);
    if(e!=HDMI_DISPLAY_OK && e!=HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION)return map_error(e,0);
    a.requires_scdc=e==HDMI_DISPLAY_NEEDS_SCDC_NEGOTIATION;
    uint64_t refresh=info.refresh_num*1000u/info.refresh_den;
    if(!refresh || refresh>UINT32_MAX) return TEE_ERROR_BAD_PARAMETERS;
    a.refresh_millihz=(uint32_t)refresh;
    *out=a;return TEE_SUCCESS;
}
static int read32(void *opaque,uint32_t address,uint32_t *value)
{ struct hdmi_tx_mode_adapter *a=opaque;return (int32_t)a->io.read32(a->io.opaque,address,value); }
static int write32(void *opaque,uint32_t address,uint32_t value)
{ struct hdmi_tx_mode_adapter *a=opaque;return (int32_t)a->io.write32(a->io.opaque,address,value); }
static int wait_ms(void *opaque,uint32_t value)
{ struct hdmi_tx_mode_adapter *a=opaque;return (int32_t)a->io.wait_ms(a->io.opaque,value); }
static int clock_configure(void *opaque,const struct hdmi_display_clock_plan *p)
{ struct hdmi_tx_mode_adapter *a=opaque;return (int32_t)hdmi_vpll_program_plan(&a->io,p,&a->clock); }
static int quiesced(void *opaque)
{
    struct hdmi_tx_mode_adapter *a=opaque;struct hdmi_avio_activity activity;
    TEE_Result result=hdmi_tx_require_idle(&a->io,a->registers,&activity);
    if(result)return (int32_t)result;
    if(!hdmi_avio_idle(&activity))return (int32_t)TEE_ERROR_BUSY;
    const struct {uint32_t address,mask,value;} fields[]={
        {0xf7491008,0xf,0},{0xf7480a94,3,0},{0xf7492a3c,0xbf,0},
        {MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_BCM_REG_BASE+RA_AVIO_BCM_EMP_STS,1u,1u},
        {MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_BCM_REG_BASE+RA_AVIO_BCM_AUTOPUSH,1u,0}
    };
    for(size_t i=0;i<sizeof(fields)/sizeof(fields[0]);++i) {
        uint32_t value;result=a->io.read32(a->io.opaque,fields[i].address,&value);
        if(result)return (int32_t)result;
        if((value&fields[i].mask)!=fields[i].value)return (int32_t)TEE_ERROR_BUSY;
    }
    struct hdmi_avio_activity second;
    result=hdmi_tx_require_idle(&a->io,a->registers,&second);
    if(result)return (int32_t)result;
    return hdmi_avio_idle(&second) ? 0 : (int32_t)TEE_ERROR_BUSY;
}
static int verify_source_clock(void *opaque,uint64_t hz,bool ratio40,uint32_t timeout)
{
    struct hdmi_tx_mode_adapter *a=opaque;
    struct hdmi_display_clock_plan p;
    if(!a || !a->configured || !timeout || timeout>HDMI_SCDC_IO_TIMEOUT_US ||
        hz!=a->timing.pixel_clock_hz || ratio40!=(hz>UINT64_C(340000000)))
        return (int32_t)TEE_ERROR_BAD_STATE;
    uint64_t started,now;
    int timer=a->scdc_io.now_us(a->scdc_io.opaque,&started);
    if(timer)return timer;
    int stopped=quiesced(a);if(stopped)return stopped;
    enum hdmi_display_result e=hdmi_display_plan_clock(hz,&p);
    if(e)return (int32_t)map_error(e,0);
    TEE_Result result=hdmi_vpll_verify_plan(&a->io,&p,&a->clock_verified);
    if(result)return (int32_t)result;
    struct hdmi_display_io io={a,read32,write32,wait_ms,NULL};
    struct hdmi_display_deadline_io time={a->scdc_io.opaque,a->scdc_io.now_us,a->scdc_io.wait_us};
    timer=a->scdc_io.now_us(a->scdc_io.opaque,&now);
    if(timer)return timer;
    if(now<started || now-started>=timeout)return (int32_t)TEE_ERROR_BUSY;
    uint32_t remaining=timeout-(uint32_t)(now-started);
    e=hdmi_display_verify_phy_rgb8(hz,&io,&time,remaining,&a->phy_verified);
    if(e==HDMI_DISPLAY_OK) {
        timer=a->scdc_io.now_us(a->scdc_io.opaque,&now);
        if(timer)return timer;
        if(now<started || now-started>=timeout)return (int32_t)TEE_ERROR_BUSY;
    }
    return (int32_t)map_error(e,a->phy_verified.callback_error);
}
static TEE_Result prepare(struct hdmi_tx_mode_adapter *a,
    const struct hdmi_mode_io *io,struct hdmi_avio_registers *registers,
    int (*owner)(void *), void *owner_opaque, bool current_config)
{
    if(!a || !io || !io->read32 || !io->write32 || !io->wait_ms || !registers)
        return TEE_ERROR_BAD_PARAMETERS;

    a->io=*io;a->registers=registers;a->configured=false;
    /* Reuse validated route/gates/bypass setup, then replace all geometry,
     * clock and PHY fields with the validated requested plan. */
    TEE_Result result=hdmi_mode_720p_configure(io);
    if(result)return result;
    result=hdmi_mode_720p_disable(io);if(result)return result;
    struct hdmi_display_io display={a,read32,write32,wait_ms,clock_configure};
    enum hdmi_display_result e=hdmi_display_prepare_stopped_rgb8(&a->timing,&a->caps,&display,&a->display);
    if(e)return map_error(e,a->display.callback_error);
    struct hdmi_vpp_timing_io vpp={a,read32,write32,quiesced};
    e=hdmi_vpp_program_stopped_rgb24(&a->timing,a->blanking,&vpp,&a->vpp_report);
    if(e)return map_error(e,a->vpp_report.callback_error);
    if(a->requires_scdc) {
        result=current_config ?
            hdmi_tx_ddc_runtime_bind_current_config(&a->ddc,io,
                verify_source_clock,a,owner,owner_opaque,&a->scdc_io) :
            hdmi_tx_ddc_runtime_bind(&a->ddc,io,verify_source_clock,a,&a->scdc_io);
        if(result)return result;
        a->scdc_caps=(struct hdmi_scdc_caps){
            a->caps.source_max_tmds_hz,a->caps.sink_max_tmds_hz,
            a->caps.source_scdc ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN,
            a->caps.source_scdc ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN,
            a->caps.source_scdc ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN,
            a->caps.sink_is_hdmi ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN,
            a->caps.sink_scdc ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN,
            a->caps.sink_timing_listed ? HDMI_SCDC_SUPPORTED : HDMI_SCDC_UNKNOWN};
    }
    a->configured=true;return TEE_SUCCESS;
}
TEE_Result hdmi_tx_mode_prepare(struct hdmi_tx_mode_adapter *a,
    const struct hdmi_mode_io *io,struct hdmi_avio_registers *registers)
{
    return prepare(a,io,registers,NULL,NULL,false);
}
TEE_Result hdmi_tx_mode_prepare_current_ddc(struct hdmi_tx_mode_adapter *a,
    const struct hdmi_mode_io *io,struct hdmi_avio_registers *registers,
    int (*owner)(void *), void *owner_opaque)
{
    if (!owner) return TEE_ERROR_NOT_SUPPORTED;
    if (owner(owner_opaque)) return TEE_ERROR_BUSY;
    return prepare(a,io,registers,owner,owner_opaque,true);
}
static TEE_Result scdc_error(enum hdmi_scdc_result e,int callback)
{
    if(e==HDMI_SCDC_OK && !callback)return TEE_SUCCESS;
    uint32_t tee=(uint32_t)callback;
    if(tee>=UINT32_C(0xffff0000) && tee<=UINT32_C(0xffff3fff))return tee;
    if(e==HDMI_SCDC_BAD_ARGUMENT || e==HDMI_SCDC_CAPABILITIES_REQUIRED)return TEE_ERROR_BAD_PARAMETERS;
    if(e==HDMI_SCDC_OUTPUT_ACTIVE || e==HDMI_SCDC_RESUME_WINDOW_EXPIRED ||
        callback==HDMI_TX_SCDC_TIMEOUT || callback==HDMI_TX_SCDC_BUSY)return TEE_ERROR_BUSY;
    if(e==HDMI_SCDC_READBACK_MISMATCH || e==HDMI_SCDC_TIME_INVALID)return TEE_ERROR_BAD_STATE;
    return TEE_ERROR_NOT_SUPPORTED;
}
TEE_Result hdmi_tx_mode_enable(struct hdmi_tx_mode_adapter *a)
{
    if(!a || !a->configured || a->link_failed)return TEE_ERROR_BAD_STATE;
    uint32_t vsync=a->timing.vsync_end-a->timing.vsync_start;
    if(!a->requires_scdc)return hdmi_mode_enable_prepared(&a->io,vsync);
    enum hdmi_scdc_result e=hdmi_scdc_configure_stopped_rgb8(
        a->timing.pixel_clock_hz,&a->scdc_caps,&a->scdc_io,&a->scdc);
    a->link_changed=a->scdc.changes_attempted!=0;
    TEE_Result result=scdc_error(e,a->scdc.callback_error);
    if(result) {a->link_failed=true;return result;}
    struct hdmi_mode_resume_window window={a->scdc_io.opaque,a->scdc_io.now_us,
        a->scdc.resume_not_before_us,a->scdc.resume_before_us};
    result=hdmi_mode_enable_prepared_window(&a->io,vsync,&window);
    if(result)a->link_failed=true;
    return result;
}
TEE_Result hdmi_tx_mode_stop_link(struct hdmi_tx_mode_adapter *a)
{
    if(!a)return TEE_ERROR_BAD_PARAMETERS;
    if(!a->requires_scdc)return TEE_SUCCESS;
    /* DDC 活动或故障时保留资源，后续由当前拥有者恢复并重试 STOP。 */
    if(a->ddc.adapter.active || a->ddc.adapter.faulted)return TEE_ERROR_NOT_SUPPORTED;
    if(!a->link_changed)return TEE_SUCCESS;
    struct hdmi_scdc_report recovery;
    enum hdmi_scdc_result e=hdmi_scdc_recover_stopped(&a->scdc_caps,&a->scdc_io,&recovery);
    TEE_Result result=scdc_error(e,recovery.callback_error);
    if(result)return result;
    a->link_changed=false;a->link_failed=false;return TEE_SUCCESS;
}

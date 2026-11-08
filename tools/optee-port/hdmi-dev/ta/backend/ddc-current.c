/* SPDX-License-Identifier: GPL-2.0 */
#include "ddc-diagnostic.h"
#include "vs680_ddc_diag.h"
#include "ddc-recovery.h"
#include <string.h>
#define SNPS UINT32_C(0xf7440000)
#define MC_CLKDIS UINT32_C(0x10004)
#define HPD_EVENT UINT32_C(0x410)
struct context {
    const struct hdmi_mode_io *io;
    struct hdmi_ddc_owner *owner;
    struct hdmi_dev_ddc_read *wire;
    bool recovering;
};
_Static_assert(sizeof(struct hdmi_dev_ddc_read)==540,"DDC read wire ABI");
static int read32(void *opaque,uint32_t off,uint32_t *value)
{
    struct context *c=opaque;
    return c->io->read32(c->io->opaque,SNPS+off,value)!=TEE_SUCCESS;
}
static int write32(void *opaque,uint32_t off,uint32_t value)
{
    struct context *c=opaque;
    /* Keep the portable module's write surface enforced at the TA boundary. */
    switch(off) {
    case VD_SLAVE:case VD_ADDRESS:case VD_SEGADDR:case VD_SEGPTR:break;
    case VD_OPERATION:if(value!=1u&&value!=2u&&value!=4u&&value!=8u)return -1;break;
    case VD_IH_STATUS:if(!value||(value&~3u))return -1;break;
    default:return -1;
    }
    return c->io->write32(c->io->opaque,SNPS+off,value)!=TEE_SUCCESS;
}
static int now_us(void *opaque,uint64_t *out)
{
    TEE_Time t;(void)opaque;
    TEE_GetSystemTime(&t);
    if(t.millis>=1000u)return -1;
    /* Synaptics platform forces secure CNTPCT time; exposed resolution is ms. */
    *out=(uint64_t)t.seconds*UINT64_C(1000000)+(uint64_t)t.millis*1000u;
    return 0;
}
static int wait_us(void *opaque,uint32_t us)
{
    struct context *c=opaque;
    if(!us||us>1000u)return -1;
    return c->io->wait_ms(c->io->opaque,(us+999u)/1000u)!=TEE_SUCCESS;
}
static int enter(void *opaque,uint64_t deadline)
{
    static const uint32_t regs[5]={MC_CLKDIS,0x14000,0x14004,0x1e410,HPD_EVENT};
    struct context *c=opaque;
    struct hdmi_dev_ddc_read *w=c->wire;
    uint64_t now;
    if(!c->owner->initialized||c->owner->entered)
        w->owner_blockers|=HDMI_DEV_DDC_OWNER_EPOCH;
    if(c->owner->quarantined&&!c->recovering)w->owner_blockers|=HDMI_DEV_DDC_OWNER_QUARANTINE;
    for(unsigned i=0;i<5;i++) {
        if(read32(c,regs[i],&w->owner_raw[i])) {
            w->owner_blockers|=HDMI_DEV_DDC_OWNER_IO;return -1;
        }
        w->owner_valid_mask|=1u<<i;
    }
    /* SDK mc_hdcp_clock_disable: bit6 disables HDCP1.4 clock.
     * hdcp_EnableRxDetect(false) prevents HDCP1.4 authentication requests.
     * vpp_be_hdmitx.c:hdcp_22_esm_ddc_reset explicitly documents that
     * HDCP22 override EN=1, VAL=0 stops ESM driving the DDC SCL line. */
    if(!(w->owner_raw[0]&0x40u))w->owner_blockers|=HDMI_DEV_DDC_OWNER_HDCP_CLOCK;
    if(w->owner_raw[1]&4u)w->owner_blockers|=HDMI_DEV_DDC_OWNER_HDCP14;
    if((w->owner_raw[3]&6u)!=2u)w->owner_blockers|=HDMI_DEV_DDC_OWNER_HDCP22;
    /* 保留待处理的 HPD 事件，交给连接状态处理流程。 */
    if(w->owner_raw[4]&1u)w->owner_blockers|=HDMI_DEV_DDC_OWNER_HPD_EVENT;
    if(c->recovering && (w->owner_raw[4]&1u)!=(c->owner->phy_event&1u))
        w->owner_blockers|=HDMI_DEV_DDC_OWNER_GENERATION;
    if(now_us(c,&now)||now>=deadline)return -1;
    if(w->owner_blockers)return -1;
    c->owner->phy_event=w->owner_raw[4];
    c->owner->entered=true;
    return 0;
}
static int finish(void *opaque,int recovery)
{
    struct context *c=opaque;
    uint32_t phy;
    if(read32(c,HPD_EVENT,&phy)) {
        recovery=1;c->wire->owner_blockers|=HDMI_DEV_DDC_OWNER_IO;
    } else if((phy&1u)!=(c->owner->phy_event&1u)) {
        recovery=1;c->wire->owner_blockers|=HDMI_DEV_DDC_OWNER_GENERATION;
    }
    if(recovery)c->owner->quarantined=true;
    c->owner->entered=false;
    return (c->wire->owner_blockers&
            (HDMI_DEV_DDC_OWNER_IO|HDMI_DEV_DDC_OWNER_GENERATION))?-1:0;
}
TEE_Result hdmi_ddc_read_current(const struct hdmi_mode_io *io,
    struct hdmi_ddc_owner *owner,struct hdmi_dev_ddc_read *out)
{
    struct vd_report r;
    struct hdmi_dev_ddc_read w={.version=HDMI_DEV_DDC_READ_VERSION,.size=sizeof(w)};
    struct context c={io,owner,&w,false};
    const struct vd_limits limits={UINT64_C(5000000),100000u,1000u,100u,2u};
    const struct vd_ops ops={&c,enter,finish,read32,write32,now_us,wait_us};
    if(!io||!io->read32||!io->write32||!io->wait_ms||!owner||!out)
        return TEE_ERROR_BAD_PARAMETERS;
    /* 当前 TX 会话持有显示锁。初始化后首次读取由该会话发起，后续读取等待前一事务结束。 */
    vd_run(&ops,&limits,w.edid,sizeof(w.edid),&r);
    if(r.readiness_result!=VD_NOT_ATTEMPTED) {
        owner->retained=r;
        owner->retained_valid=r.recovery_required&&r.operation_may_be_active;
    }
    w.result=r.result;w.readiness_result=r.readiness_result;
    w.edid_result=r.edid_result;w.cleanup_result=r.cleanup_result;
    w.before.version=HDMI_DEV_DDC_SNAPSHOT_VERSION;w.before.size=sizeof(w.before);
    memcpy(w.before.raw,r.before.raw,sizeof(w.before.raw));
    w.before.valid_mask=r.before.valid_mask;
    w.before.fast_mode=r.before.fast_mode;w.before.sda_hold=r.before.sda_hold;
    w.before.selected_high=r.before.fast_mode?r.before.fs_high:r.before.ss_high;
    w.before.selected_low=r.before.fast_mode?r.before.fs_low:r.before.ss_low;
    w.before.flags=HDMI_DEV_DDC_SFR_UNVERIFIED;
    w.edid_bytes=(uint32_t)r.edid_bytes;w.edid_blocks=r.edid_blocks_read;
    w.edid_expected=r.edid_expected_blocks;
    w.edid_flags=(r.edid_header_valid?1u:0u)|(r.edid_complete?2u:0u)|
                 (r.edid_valid?4u:0u)|(r.edid_limited?8u:0u);
    w.run_flags=(r.recovery_required||owner->quarantined?1u:0u)|
                (r.operation_may_be_active?2u:0u)|(r.selectors_restored?4u:0u);
    w.last_status=r.last_status;w.transactions=r.transactions_started;w.polls=r.polls;
    w.last_offset=r.last_mmio_offset;w.last_slave=r.last_slave;
    w.last_segment=r.last_segment;w.last_address=r.last_address;
    w.elapsed_us_low=(uint32_t)r.elapsed_us;w.elapsed_us_high=(uint32_t)(r.elapsed_us>>32);
    for(unsigned i=0;i<5;i++){w.scdc_result[i]=r.scdc[i].result;w.scdc_value[i]=r.scdc[i].value;}
    for(unsigned i=0;i<2;i++)w.checksum_sum[i]=r.edid_checksum_sum[i];
    /* 返回前检查读取结果和热插拔代次。 */
    if(w.owner_blockers&HDMI_DEV_DDC_OWNER_GENERATION) {
        w.edid_flags=0;w.edid_bytes=0;w.edid_blocks=0;
        for(unsigned i=0;i<5;i++){w.scdc_result[i]=VD_HPD_LOW;w.scdc_value[i]=0;}
    }
    if(w.edid_bytes<sizeof(w.edid))memset(w.edid+w.edid_bytes,0,sizeof(w.edid)-w.edid_bytes);
    *out=w;return TEE_SUCCESS;
}

TEE_Result hdmi_ddc_recover_current(const struct hdmi_mode_io *io,
    struct hdmi_ddc_owner *owner,struct hdmi_dev_ddc_recovery *out)
{
    struct hdmi_dev_ddc_read state={0};
    struct hdmi_dev_ddc_recovery reply={.version=HDMI_DEV_DDC_RECOVERY_VERSION,
        .size=sizeof(reply),.result=VD_OWNER_UNAVAILABLE};
    struct vd_recovery recovery={0};
    struct context c={io,owner,&state,true};
    const struct vd_ops ops={&c,enter,finish,read32,write32,now_us,wait_us};
    if(!io||!io->read32||!io->write32||!owner||!out)
        return TEE_ERROR_BAD_PARAMETERS;
    if(owner->quarantined&&owner->retained_valid) {
        vd_recover(&ops,&owner->retained,&recovery);
        reply.result=recovery.result;reply.original_result=recovery.original_result;
        reply.flags=recovery.flags;reply.status=recovery.status;
        memcpy(reply.selectors,recovery.selectors,sizeof(reply.selectors));
        if(!recovery.result&&(recovery.flags&VD_RECOVER_IDLE)&&!state.owner_blockers) {
            owner->quarantined=false;owner->retained_valid=false;
        }
    }
    reply.owner_blockers=state.owner_blockers;
    reply.owner_valid_mask=state.owner_valid_mask;
    *out=reply;return TEE_SUCCESS;
}

/* SPDX-License-Identifier: GPL-2.0 */
#include "ddc-recovery.h"
#include <string.h>
static const uint32_t selectors[4] = {VD_SLAVE, VD_ADDRESS, VD_SEGADDR, VD_SEGPTR};
static const unsigned indices[4] = {VD_S_SLAVE, VD_S_ADDRESS, VD_S_SEGADDR, VD_S_SEGPTR};
int vd_recover(const struct vd_ops *ops, const struct vd_report *saved,
               struct vd_recovery *out)
{
    struct vd_recovery reply = {.result=VD_BAD_ARGUMENT};
    uint64_t now;
    uint32_t value;
    int rc = VD_OK;
    if (!out) return VD_BAD_ARGUMENT;
    if (!ops || !saved || !ops->enter || !ops->finish || !ops->read32 ||
        !ops->write32 || !ops->now_us) goto done;
    reply.original_result=saved->result;
    if (!saved->recovery_required || !saved->operation_may_be_active ||
        !saved->transactions_started ||
        saved->before.valid_mask != ((1u<<VD_SNAPSHOT_COUNT)-1u)) {
        rc=VD_OWNER_UNAVAILABLE;goto done;
    }
    if (ops->now_us(ops->ctx,&now) || now>UINT64_MAX-100000u) {
        rc=VD_CLOCK_ERROR;goto done;
    }
    if (ops->enter(ops->ctx,now+100000u)) {rc=VD_OWNER_UNAVAILABLE;goto done;}
    reply.flags|=VD_RECOVER_ENTERED;
#define READ(reg,to) do {if(ops->read32(ops->ctx,(reg),&(to))) {rc=VD_MMIO_ERROR;goto finish;}} while(0)
#define WRITE(reg,v) do {if(ops->write32(ops->ctx,(reg),(v))) {rc=VD_MMIO_ERROR;goto finish;}} while(0)
    READ(VD_PHY_STATUS,value);
    if (!(value&2u)) {rc=VD_HPD_LOW;goto finish;}
    READ(VD_SCDC_UPDATE,value);
    if (value&0x31u) {rc=VD_AUTO_DDC_ACTIVE;goto finish;}
    for (unsigned i=0;i<4;i++) READ(selectors[i],reply.selectors[i]);
    if ((reply.selectors[0]&0x7fu)!=saved->last_slave ||
        (reply.selectors[1]&0xffu)!=saved->last_address ||
        (reply.selectors[2]&0xffu)!=(saved->last_slave==0x50u?0x30u:0u) ||
        (reply.selectors[3]&0xffu)!=saved->last_segment) {
        rc=VD_STALE_STATUS;goto finish;
    }
    /* 检查发起事务时的时序、屏蔽和自动模式配置。 */
    for (unsigned i=0;i<=VD_S_SOFTRSTZ;i++) {
        READ(vd_snapshot_offsets[i],value);
        if (value!=saved->before.raw[i]) {rc=VD_STALE_STATUS;goto finish;}
    }
    READ(VD_IH_STATUS,reply.status);
    if (reply.status&1u) {rc=VD_I2C_ERROR;goto finish;}
    if (!(reply.status&2u)) {rc=VD_TRANSACTION_TIMEOUT;goto finish;}
    reply.flags|=VD_RECOVER_DONE;
    WRITE(VD_IH_STATUS,2u);
    READ(VD_IH_STATUS,value);
    if (value&3u) {rc=VD_STALE_STATUS;goto finish;}
    reply.flags|=VD_RECOVER_ACKNOWLEDGED;
    READ(VD_PHY_STATUS,value);
    if (!(value&2u)) {rc=VD_HPD_LOW;goto finish;}
    for (unsigned i=0;i<4;i++) {
        WRITE(selectors[i],saved->before.raw[indices[i]]);
        READ(selectors[i],value);
        if (value!=saved->before.raw[indices[i]]) {rc=VD_RESTORE_ERROR;goto finish;}
    }
    reply.flags|=VD_RECOVER_RESTORED|VD_RECOVER_IDLE;
finish:
    if (ops->finish(ops->ctx,rc!=VD_OK)) {
        reply.flags&=~VD_RECOVER_IDLE;
        rc=VD_OWNER_FINISH_ERROR;
    }
#undef READ
#undef WRITE
done:
    if (rc==VD_OK && !reply.flags) rc=VD_BAD_ARGUMENT;
    reply.result=rc;*out=reply;return rc;
}

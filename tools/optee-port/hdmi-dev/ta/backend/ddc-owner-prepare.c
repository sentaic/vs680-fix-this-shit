/* SPDX-License-Identifier: GPL-2.0 */
#include "ddc-diagnostic.h"
#include <string.h>
#define BASE UINT32_C(0xf7440000)
static const uint32_t regs[5]={0x10004,0x14000,0x14004,0x1e410,0x410};
_Static_assert(sizeof(struct hdmi_dev_ddc_prepare)==80,"DDC prepare ABI");
static uint64_t time_us(void)
{
    TEE_Time t;
    TEE_GetSystemTime(&t);
    return (uint64_t)t.seconds*1000000u+(uint64_t)t.millis*1000u;
}
TEE_Result hdmi_ddc_prepare_owner(const struct hdmi_mode_io *io,
    struct hdmi_ddc_owner *owner,struct hdmi_dev_ddc_prepare *out)
{
    struct hdmi_dev_ddc_prepare w={.version=HDMI_DEV_DDC_PREPARE_VERSION,
        .size=sizeof(w),.result=-16};
    uint32_t mask,polarity,phy,status,automatic;
    uint64_t started=time_us(),ended;
    int failed=0;
    if(!io||!io->read32||!io->write32||!owner||!out)
        return TEE_ERROR_BAD_PARAMETERS;
    if(owner->entered||owner->quarantined){
        owner->quarantined=true;w.flags|=2u;*out=w;return TEE_SUCCESS;
    }
    if(!owner->initialized||owner->port_epoch==UINT32_MAX){
        *out=w;return TEE_SUCCESS;
    }
#define READ(off,to) do { if(io->read32(io->opaque,BASE+(off),&(to)))     {failed=1;goto done;} } while(0)
#define WRITE(off,value) do { ++w.writes;     if(io->write32(io->opaque,BASE+(off),(value)))     {failed=1;goto done;} } while(0)
    for(unsigned i=0;i<5;i++){READ(regs[i],w.before[i]);w.valid_mask|=1u<<i;}
    READ(0xc010,w.phy_before);
    READ(0x414,w.ddc_status);
    READ(0x1f850,automatic);
    /* 初始化会话取得 DDC 所有权后检查事务错误状态。 */
    if(!(w.before[0]&0x40u)||(w.before[1]&4u)||w.ddc_status&3u||
       automatic&0x31u||!(w.phy_before&2u))goto done;
    READ(0xc018,mask);READ(0xc01c,polarity);READ(0xc010,phy);
    if((phy&0xf2u)!=(w.phy_before&0xf2u))goto done;
    owner->entered=true;
    /* 按 hdcp_22_esm_ddc_reset 隔离 ESM 的 DDC SCL 驱动。 */
    if((w.before[3]&6u)!=2u){
        WRITE(0x1e410,(w.before[3]&~6u)|2u);w.flags|=4u;
        READ(0x1e410,status);
        if((status&6u)!=2u){failed=1;goto done;}
    }
    /* 按 phy_hot_plug_detected 屏蔽 HPD、设置反向极性、确认已处理的事件，再恢复屏蔽值。 */
    if(w.before[4]&1u){
        WRITE(0xc018,mask|2u);
        if(polarity&2u){WRITE(0xc01c,polarity&~2u);}
        READ(0xc01c,status);
        if(status&2u){failed=1;goto done;}
        READ(0xc010,phy);
        if((phy&0xf2u)!=(w.phy_before&0xf2u)){failed=1;goto done;}
        WRITE(0x410,1u);w.flags|=8u;
        READ(0x410,status);
        if(status&1u){failed=1;goto done;}
        WRITE(0xc018,mask);
        READ(0xc018,status);
        if(status!=mask){failed=1;goto done;}
    }
    for(unsigned i=0;i<5;i++){READ(regs[i],w.after[i]);w.valid_mask|=1u<<(i+5);}
    READ(0xc010,w.phy_after);READ(0x414,status);
    if(!(w.after[0]&0x40u)||(w.after[1]&4u)||(w.after[3]&6u)!=2u||
       w.after[4]&1u||status&3u||
       (w.phy_after&0xf2u)!=(w.phy_before&0xf2u)){failed=1;goto done;}
    ended=time_us();
    if(ended<started||ended-started>=20000u){failed=1;goto done;}
    w.result=0;w.flags|=1u;
    w.epoch=++owner->port_epoch;
    owner->phy_event=w.after[4];
done:
    if(failed){
        w.result=-5;
        if(w.writes){owner->quarantined=true;w.flags|=2u;}
    }
    owner->entered=false;*out=w;
    return TEE_SUCCESS;
#undef READ
#undef WRITE
}

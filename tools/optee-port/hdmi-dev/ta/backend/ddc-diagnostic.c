/* SPDX-License-Identifier: GPL-2.0 */
#include "ddc-diagnostic.h"
#define SNPS UINT32_C(0xf7440000)
/* Same order as the delivered vs680_ddc_diag vd_snapshot_offsets. */
static const uint32_t offsets[HDMI_DEV_DDC_RAW_COUNT] = {
    0x1f81c,0x1f82c,0x1f830,0x1f834,0x1f838,
    0x1f83c,0x1f840,0x1f844,0x1f848,0x1f84c,
    0x1f814,0x1f818,0x614,0x1f850,0x1f824,
    0x1f810,0x414,0xc010,0x1f800,0x1f804,0x1f820,0x1f828
};
_Static_assert(sizeof(struct hdmi_dev_ddc_snapshot)==128,"DDC snapshot ABI");
TEE_Result hdmi_ddc_snapshot(const struct hdmi_mode_io *io,
                            struct hdmi_dev_ddc_snapshot *out)
{
    struct hdmi_dev_ddc_snapshot p={
        .version=HDMI_DEV_DDC_SNAPSHOT_VERSION,.size=sizeof(p),
        .flags=HDMI_DEV_DDC_SNAPSHOT_ONLY|HDMI_DEV_DDC_OWNER_UNVERIFIED|
               HDMI_DEV_DDC_SFR_UNVERIFIED
    };
    if(!io||!io->read32||!out)return TEE_ERROR_BAD_PARAMETERS;
    for(unsigned i=0;i<HDMI_DEV_DDC_RAW_COUNT;i++) {
        TEE_Result e=io->read32(io->opaque,SNPS+offsets[i],&p.raw[i]);
        if(e) { p.io_result=e;p.readiness=HDMI_DEV_DDC_PARTIAL;*out=p;
            return TEE_SUCCESS; }
        p.valid_mask|=UINT32_C(1)<<i;
    }
    p.fast_mode=!!(p.raw[0]&8u);
    unsigned hi=p.fast_mode?5u:1u,lo=p.fast_mode?7u:3u;
    p.selected_high=((p.raw[hi]&255u)<<8)|(p.raw[hi+1]&255u);
    p.selected_low=((p.raw[lo]&255u)<<8)|(p.raw[lo+1]&255u);
    p.sda_hold=p.raw[9]&255u;
    if(!p.selected_high||!p.selected_low||!p.sda_hold)
        p.readiness|=HDMI_DEV_DDC_TIMING_UNREADY;
    if((p.raw[10]&4u)||(p.raw[11]&0x44u))p.readiness|=HDMI_DEV_DDC_IRQ_GATED;
    if(p.raw[13]&0x31u)p.readiness|=HDMI_DEV_DDC_AUTO_ACTIVE;
    if(!(p.raw[17]&2u))p.readiness|=HDMI_DEV_DDC_HPD_LOW;
    *out=p;return TEE_SUCCESS;
}

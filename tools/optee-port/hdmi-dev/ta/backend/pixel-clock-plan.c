/* SPDX-License-Identifier: GPL-2.0 */
#include "pixel-clock-plan.h"
#include "Galois_memmap.h"
#include "avio_memmap.h"
#include "avioGbl.h"
/* Linux drivers/clk/berlin/vs680-pll.c的update_setting及当前pixel-clock.c。 */
#define PLL (MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_GBL_BASE+RA_avioGbl_VPLL_WRAP0+RA_VPLL_WRAP_VPLL)
#define PD (MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_GBL_BASE+RA_avioGbl_AVPLL_CTRL0)
#define STEP(x) do { TEE_Result e=(x); if(e) { r->error=e;return e; } } while(0)
TEE_Result hdmi_vpll_validate_plan(const struct hdmi_display_clock_plan *p)
{
    struct hdmi_display_clock_plan expected;
    if (!p) return TEE_ERROR_BAD_PARAMETERS;
    if (p->divr>0x1ff || p->divfi>0x1ff || p->divff>0xffffff ||
        p->divq>0x1f || p->range>7 || !p->actual_den ||
        hdmi_display_plan_clock(p->target_hz,&expected)!=HDMI_DISPLAY_OK)
        return TEE_ERROR_BAD_PARAMETERS;
#define SAME(member) if (p->member!=expected.member) return TEE_ERROR_BAD_PARAMETERS
    SAME(target_hz);SAME(actual_num);SAME(actual_den);SAME(vco_target_hz);
    SAME(reference_hz);SAME(divr);SAME(divfi);SAME(divff);SAME(divq);SAME(range);
#undef SAME
    return TEE_SUCCESS;
}
static TEE_Result read_word(const struct hdmi_mode_io *io,struct hdmi_vpll_report *r,
    uint32_t address,uint32_t *value)
{
    r->last_address=address;++r->reads_attempted;
    return io->read32(io->opaque,address,value);
}
static TEE_Result update(const struct hdmi_mode_io *io,struct hdmi_vpll_report *r,
    uint32_t address,uint32_t mask,uint32_t value)
{
    uint32_t old;TEE_Result e;
    r->mask=mask;r->expected=value;
    e=read_word(io,r,address,&old);if(e)return e;
    r->observed=old;++r->writes_attempted;
    e=io->write32(io->opaque,address,(old&~mask)|(value&mask));
    if (!e) ++r->writes_completed;
    return e;
}
static TEE_Result expect(const struct hdmi_mode_io *io,struct hdmi_vpll_report *r,
    uint32_t address,uint32_t mask,uint32_t value)
{
    r->mask=mask;r->expected=value;
    TEE_Result e=read_word(io,r,address,&r->observed);if(e)return e;
    return (r->observed&mask)==value ? TEE_SUCCESS : TEE_ERROR_BAD_STATE;
}
TEE_Result hdmi_vpll_program_plan(const struct hdmi_mode_io *io,
    const struct hdmi_display_clock_plan *input,struct hdmi_vpll_report *r)
{
    if (!r) return TEE_ERROR_BAD_PARAMETERS;
    *r=(struct hdmi_vpll_report){0};
    if (!io || !io->read32 || !io->write32 || !io->wait_ms) {
        r->error=TEE_ERROR_BAD_PARAMETERS;return r->error;
    }
    STEP(hdmi_vpll_validate_plan(input));
    const struct hdmi_display_clock_plan p=*input;
    r->plan_valid=true;
    r->stage=HDMI_VPLL_POWER_DOWN;
    /* 每次RMW重新读共享寄存器，仅改已核对的VPLL0_PD bit0。 */
    STEP(update(io,r,PD,1,1));
    r->stage=HDMI_VPLL_RESET;
    STEP(update(io,r,PLL,3,3));STEP(update(io,r,PLL+4,1u<<9,0));
    r->stage=HDMI_VPLL_DIVIDERS;
    STEP(update(io,r,PLL+8,0x1ff,p.divr));
    STEP(update(io,r,PLL+12,0x1ff,p.divfi));
    STEP(update(io,r,PLL+16,0xffffff,p.divff));
    STEP(update(io,r,PLL,7u<<3,p.range<<3));
    STEP(update(io,r,PLL+20,0x1f,p.divq));
    r->stage=HDMI_VPLL_RELEASE;
    STEP(update(io,r,PLL,1,0));STEP(update(io,r,PLL,2,0));
    STEP(update(io,r,PD,1,0));
    r->stage=HDMI_VPLL_LOCK;r->mask=1;r->expected=1;
    for (unsigned i=0;i<100;++i) {
        ++r->lock_polls;STEP(read_word(io,r,PLL+28,&r->observed));
        if (r->observed&1u) { r->lock_observed=true;break; }
        ++r->waits_attempted;STEP(io->wait_ms(io->opaque,1));
    }
    if (!r->lock_observed) {r->error=TEE_ERROR_BUSY;return r->error;}
    r->stage=HDMI_VPLL_READBACK;
    STEP(expect(io,r,PLL,0x3b,p.range<<3));
    STEP(expect(io,r,PLL+4,1u<<9,0));
    STEP(expect(io,r,PLL+8,0x1ff,p.divr));
    STEP(expect(io,r,PLL+12,0x1ff,p.divfi));
    STEP(expect(io,r,PLL+16,0xffffff,p.divff));
    STEP(expect(io,r,PLL+20,0x1f,p.divq));
    STEP(expect(io,r,PD,1,0));STEP(expect(io,r,PLL+28,1,1));
    r->stage=HDMI_VPLL_VERIFIED;r->verified=true;
    return TEE_SUCCESS;
}

TEE_Result hdmi_vpll_verify_plan(const struct hdmi_mode_io *io,
    const struct hdmi_display_clock_plan *input,struct hdmi_vpll_report *r)
{
    if (!r) return TEE_ERROR_BAD_PARAMETERS;
    *r=(struct hdmi_vpll_report){0};
    if (!io || !io->read32) {r->error=TEE_ERROR_BAD_PARAMETERS;return r->error;}
    STEP(hdmi_vpll_validate_plan(input));
    const struct hdmi_display_clock_plan p=*input;
    r->plan_valid=true;r->stage=HDMI_VPLL_READBACK;
    STEP(expect(io,r,PLL,0x3b,p.range<<3));
    STEP(expect(io,r,PLL+4,1u<<9,0));
    STEP(expect(io,r,PLL+8,0x1ff,p.divr));
    STEP(expect(io,r,PLL+12,0x1ff,p.divfi));
    STEP(expect(io,r,PLL+16,0xffffff,p.divff));
    STEP(expect(io,r,PLL+20,0x1f,p.divq));
    STEP(expect(io,r,PD,1,0));STEP(expect(io,r,PLL+28,1,1));
    STEP(expect(io,r,UINT32_C(0xf749c000),0x1fff,0x820));
    STEP(expect(io,r,UINT32_C(0xf749c0ec),0xff,0x4d));
    r->lock_observed=true;r->verified=true;r->stage=HDMI_VPLL_VERIFIED;
    return TEE_SUCCESS;
}

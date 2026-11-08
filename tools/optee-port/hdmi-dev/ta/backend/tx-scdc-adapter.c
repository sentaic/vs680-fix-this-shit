/* SPDX-License-Identifier: GPL-2.0 */
#include "tx-scdc-adapter.h"
#include <stddef.h>
#define SNPS UINT32_C(0xf7440000)
/* 寄存器使用物理字节偏移；i2cm.c 的单字节读命令为 1，写命令为 0x10。 */
#define DDC_SLAVE UINT32_C(0x1f800)
#define DDC_ADDRESS UINT32_C(0x1f804)
#define DDC_DATAO UINT32_C(0x1f808)
#define DDC_DATAI UINT32_C(0x1f80c)
#define DDC_OPERATION UINT32_C(0x1f810)
#define DDC_SEGADDR UINT32_C(0x1f820)
#define DDC_SEGPTR UINT32_C(0x1f828)
#define DDC_AUTO UINT32_C(0x1f850)
#define DDC_STATUS UINT32_C(0x414)
#define PHY_STATUS UINT32_C(0xc010)
#define DDC_ERROR 1u
#define DDC_DONE 2u
#define DDC_POLL_LIMIT 512u
#define DDC_POLL_US 50u
static int read_mmio(void *context,uint32_t address,uint32_t *value)
{
    struct hdmi_tx_scdc_adapter *a=context;
    if (!a || !a->bound || !value) return HDMI_TX_SCDC_EINVAL;
    return a->platform.read32(a->platform.opaque,address,value);
}
static int write_mmio(void *context,uint32_t address,uint32_t value)
{
    struct hdmi_tx_scdc_adapter *a=context;
    if (!a || !a->bound) return HDMI_TX_SCDC_EINVAL;
    return a->platform.write32(a->platform.opaque,address,value);
}
static int now_bridge(void *context,uint64_t *value)
{
    struct hdmi_tx_scdc_adapter *a=context;
    if (!a || !a->bound || !value) return HDMI_TX_SCDC_EINVAL;
    return a->platform.now_us(a->platform.opaque,value);
}
static int wait_bridge(void *context,uint32_t us,uint32_t timeout)
{
    struct hdmi_tx_scdc_adapter *a=context;
    if (!a || !a->bound) return HDMI_TX_SCDC_EINVAL;
    return a->platform.wait_us(a->platform.opaque,us,timeout);
}
static int clock_bridge(void *context,uint64_t hz,bool ratio,uint32_t timeout)
{
    struct hdmi_tx_scdc_adapter *a=context;
    if (!a || !a->bound || a->faulted || a->active) return HDMI_TX_SCDC_BUSY;
    return a->platform.verify_source_clock(a->platform.opaque,hz,ratio,timeout);
}
static int remaining(struct hdmi_tx_scdc_adapter *a,uint32_t *us)
{
    uint64_t now;
    int r=now_bridge(a,&now);
    if (r) return r;
    if (now<a->last_us) return HDMI_TX_SCDC_TIME;
    a->last_us=now;
    if (now>=a->deadline_us) return HDMI_TX_SCDC_TIMEOUT;
    *us=(uint32_t)(a->deadline_us-now);
    return 0;
}
static int hpd(struct hdmi_tx_scdc_adapter *a)
{
    uint32_t value;
    int r=read_mmio(a,SNPS+PHY_STATUS,&value);
    if (r) return r;
    return value&2u ? 0 : HDMI_TX_SCDC_DISCONNECTED;
}
static int fail(struct hdmi_tx_scdc_adapter *a,int error)
{
    a->faulted=1;
    /* 事务完成或终止前保持 active。 */
    return error;
}
static int transfer(struct hdmi_tx_scdc_adapter *a,uint8_t slave,uint8_t offset,
                    uint8_t *value,bool write,uint32_t timeout)
{
    uint64_t started;
    uint32_t status, spare, left, observed=0;
    int r;
    if (!a || !a->bound || !value || slave!=HDMI_SCDC_DDC_ADDRESS ||
        !timeout || timeout>HDMI_SCDC_IO_TIMEOUT_US) return HDMI_TX_SCDC_EINVAL;
    if (a->faulted || a->active) return HDMI_TX_SCDC_BUSY;
    r=now_bridge(a,&started);
    if (r) return r;
    if (started>UINT64_MAX-timeout) return HDMI_TX_SCDC_TIME;
    a->deadline_us=started+timeout;a->last_us=started;
    r=a->platform.verify_ddc_ready(a->platform.opaque,timeout);
    if (r) return r;
    if ((r=remaining(a,&left)) || (r=hpd(a))) return r;
    r=read_mmio(a,SNPS+DDC_AUTO,&status);
    if (r) return r;
    if (status&0x31u) return HDMI_TX_SCDC_BUSY;
    /* 直接写 W1C 掩码清除 DDC ERROR/DONE，保留 SCDC_READREQ；读回后发起本次传输。 */
    if ((r=write_mmio(a,SNPS+DDC_STATUS,DDC_ERROR|DDC_DONE)))
        return fail(a,r);
    if ((r=read_mmio(a,SNPS+DDC_STATUS,&status))) return fail(a,r);
    if (status&(DDC_ERROR|DDC_DONE)) return fail(a,HDMI_TX_SCDC_BUSY);
    if ((r=read_mmio(a,SNPS+DDC_SLAVE,&spare))) return fail(a,r);
    if ((r=write_mmio(a,SNPS+DDC_SLAVE,(spare&~0x7fu)|slave)) ||
        (r=write_mmio(a,SNPS+DDC_ADDRESS,offset))) return fail(a,r);
    if (write) {
        r=write_mmio(a,SNPS+DDC_DATAO,*value);
    } else {
        r=write_mmio(a,SNPS+DDC_SEGADDR,0);
        if (!r) r=write_mmio(a,SNPS+DDC_SEGPTR,0);
    }
    if (r) return fail(a,r);
    if ((r=remaining(a,&left)) || (r=hpd(a))) return fail(a,r);
    a->active=1;
    r=write_mmio(a,SNPS+DDC_OPERATION,write?0x10u:1u);
    if (r) return fail(a,r);
    for (unsigned poll=0;poll<DDC_POLL_LIMIT;++poll) {
        if ((r=remaining(a,&left)) || (r=hpd(a))) return fail(a,r);
        if ((r=read_mmio(a,SNPS+DDC_STATUS,&status))) return fail(a,r);
        a->last_status=status&3u;
        if ((r=remaining(a,&left))) return fail(a,r);
        if (status&DDC_ERROR) return fail(a,HDMI_TX_SCDC_IO);
        if (status&DDC_DONE) {
            if (!write && (r=read_mmio(a,SNPS+DDC_DATAI,&observed))) return fail(a,r);
            if ((r=write_mmio(a,SNPS+DDC_STATUS,status&3u))) return fail(a,r);
            if ((r=read_mmio(a,SNPS+DDC_STATUS,&status))) return fail(a,r);
            if (status&3u) return fail(a,HDMI_TX_SCDC_BUSY);
            if ((r=remaining(a,&left)) || (r=hpd(a))) return fail(a,r);
            a->active=0;
            if (!write) *value=(uint8_t)observed;
            return 0;
        }
        uint32_t pause=left<DDC_POLL_US?left:DDC_POLL_US;
        uint64_t waited_from=a->last_us, waited_until;
        r=wait_bridge(a,pause,left);
        if (r) return fail(a,r);
        r=now_bridge(a,&waited_until);
        if (r) return fail(a,r);
        if (waited_until<waited_from || waited_until-waited_from<pause)
            return fail(a,HDMI_TX_SCDC_TIME);
        a->last_us=waited_until;
    }
    return fail(a,HDMI_TX_SCDC_TIMEOUT);
}
static int read_byte(void *context,uint8_t slave,uint8_t offset,uint8_t *value,uint32_t us)
{ return transfer(context,slave,offset,value,false,us); }
static int write_byte(void *context,uint8_t slave,uint8_t offset,uint8_t value,uint32_t us)
{ return transfer(context,slave,offset,&value,true,us); }
int hdmi_tx_scdc_adapter_bind(struct hdmi_tx_scdc_adapter *a,
    const struct hdmi_tx_scdc_platform *p,struct hdmi_scdc_io *out)
{
    if (!a || !p || !out) return HDMI_TX_SCDC_EINVAL;
    if (a->bound || a->active || a->faulted) return HDMI_TX_SCDC_BUSY;
    if (!p->read32 || !p->write32 || !p->now_us || !p->wait_us ||
        !p->verify_source_clock || !p->verify_ddc_ready) return HDMI_TX_SCDC_UNSUPPORTED;
    a->platform=*p;a->bound=1;
    *out=(struct hdmi_scdc_io){.opaque=a,.read32=read_mmio,.write32=write_mmio,
        .ddc_readb=read_byte,.ddc_writeb=write_byte,.verify_source_clock=clock_bridge,
        .now_us=now_bridge,.wait_us=wait_bridge};
    return 0;
}
int hdmi_tx_scdc_adapter_recover_ddc(struct hdmi_tx_scdc_adapter *a,uint32_t timeout)
{
    int r;
    if (!a || !a->bound || !timeout || timeout>HDMI_SCDC_IO_TIMEOUT_US)
        return HDMI_TX_SCDC_EINVAL;
    if (!a->platform.reset_ddc_verified) return HDMI_TX_SCDC_UNSUPPORTED;
    r=a->platform.reset_ddc_verified(a->platform.opaque,timeout);
    if (r) return r;
    r=a->platform.verify_ddc_ready(a->platform.opaque,timeout);
    if (r) return r;
    a->active=0;a->faulted=0;a->last_status=0;
    return 0;
}

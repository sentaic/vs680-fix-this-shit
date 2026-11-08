/* SPDX-License-Identifier: GPL-2.0 */
#include "avio.h"
#include "rx-aio-exclusive-command.h"
#define RX_AIO_CHANNEL avioDhubChMap_aio64b_BCM_R
#define RX_AIO_MASK (1u << RX_AIO_CHANNEL)
#define RX_AIO_BCM_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_BCM_REG_BASE)
static HDL_dhub2d rx_exclusive_aio;
static uint64_t rx_exclusive_aio_epoch;
static int rx_exclusive_aio_initialized, rx_exclusive_aio_owned;
static uint32_t rx_aio_issued_start,rx_aio_issued_cfg,rx_aio_issued_command,rx_aio_issued_data;
static uint32_t rx_aio_read(uint32_t address)
{ return *(volatile uint32_t *)TEE_TA_RegPhyToVirt(address); }
static void rx_aio_write(uint32_t address, uint32_t value)
{ *(volatile uint32_t *)TEE_TA_RegPhyToVirt(address) = value; }
static void rx_aio_barrier(void) { __asm__ volatile("dsb sy" ::: "memory"); }
static void rx_aio_handle(void)
{
    if (!rx_exclusive_aio_initialized) {
        dhub2d_hdl(AG_HBO_SRAM_BASE, AG_DHUB_BASE, &rx_exclusive_aio);
        rx_exclusive_aio_initialized = 1;
    }
}
static uint32_t rx_aio_channel_address(void)
{ return rx_exclusive_aio.dhub.ra + RA_dHubReg_ARR + RX_AIO_CHANNEL*sizeof(SIE_dHubChannel); }
static uint32_t rx_aio_fifo_address(unsigned id)
{ return rx_exclusive_aio.dhub.hbo.ra + RA_HBO_ARR + id*sizeof(SIE_FiFo); }
/* 全部BCM队列和自动重复均停、TX/RX DMA无活动，才允许取得共享通道。 */
static int rx_aio_quiet_once(uint32_t sample[14])
{
    unsigned pointer;
    sample[0]=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_EMP_STS);
    sample[1]=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_AUTOPUSH);
    sample[2]=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_ERROR);
    sample[3]=dhub_channel_busy(&capture_dhub.dhub);
    sample[4]=dhub_channel_pending(&capture_dhub.dhub);
    sample[5]=dhub2d_channel_busy(&capture_dhub);
    sample[6]=hbo_queue_busy(dhub_hbo(&capture_dhub.dhub));
    sample[7]=dhub_channel_busy(&rx_exclusive_aio.dhub)&RX_AIO_MASK;
    sample[8]=dhub_channel_pending(&rx_exclusive_aio.dhub)&RX_AIO_MASK;
    sample[9]=dhub2d_channel_busy(&rx_exclusive_aio)&RX_AIO_MASK;
    sample[10]=hbo_queue_busy(dhub_hbo(&rx_exclusive_aio.dhub))&(3u<<(2*RX_AIO_CHANNEL));
    sample[11]=hbo_queue_query(dhub_hbo(&rx_exclusive_aio.dhub),2*RX_AIO_CHANNEL,
        SemaQueryMap_ADDR_master_consumer,&pointer);
    sample[12]=hbo_queue_query(dhub_hbo(&rx_exclusive_aio.dhub),2*RX_AIO_CHANNEL+1,
        SemaQueryMap_ADDR_master_consumer,&pointer);
    sample[13]=semaphore_query(dhub_semaphore(&rx_exclusive_aio.dhub),RX_AIO_CHANNEL,
        SemaQueryMap_ADDR_master_consumer,&pointer);
    if ((sample[0]&0xfffffu)!=0xfffffu || sample[1] || sample[2] ||
        (semaphore_chk_full(dhub_semaphore(&rx_exclusive_aio.dhub),-1)&RX_AIO_MASK)) return 0;
    for (unsigned i=3;i<14;i++) if (sample[i]) return 0;
    return 1;
}
static int rx_aio_quiet(void)
{
    uint32_t first[14], second[14];
    if (!rx_aio_quiet_once(first)) return 0;
    rx_aio_barrier();
    return rx_aio_quiet_once(second) && !TEE_MemCompare(first,second,sizeof(first));
}
static void rx_aio_fifo(unsigned id, uint32_t base, unsigned bytes)
{
    T32FiFo_CFG cfg={.u32=0}; T32Semaphore_CFG depth={.u32=0};
    cfg.uCFG_BASE=base; depth.uCFG_DEPTH=bytes/8u;
    rx_aio_write(rx_aio_fifo_address(id)+RA_FiFo_CFG,cfg.u32);
    rx_aio_write(rx_exclusive_aio.dhub.hbo.ra+RA_HBO_FiFoCtl+RA_SemaHub_ARR+
        id*sizeof(SIE_Semaphore)+RA_Semaphore_CFG,depth.u32);
    rx_aio_write(rx_aio_fifo_address(id)+RA_FiFo_START,1);
}
static TEE_Result rx_aio_exclusive_prepare(uint64_t epoch)
{
    rx_aio_handle();
    if (!rx_command_pool_bound(epoch) || rx_command_pool.dma_armed)
        return TEE_ERROR_BAD_STATE;
    if (rx_exclusive_aio_owned) return TEE_ERROR_BAD_STATE;
    if (!rx_aio_quiet())
        return TEE_ERROR_BUSY;
    /* 首次写入前持有配置；清理失败时阻止命令池解除映射。 */
    rx_exclusive_aio_owned=1; rx_exclusive_aio_epoch=epoch;
    dhub_channel_enable(&rx_exclusive_aio.dhub,RX_AIO_CHANNEL,0,NULL);
    rx_aio_issued_start=0;
    for (unsigned id=2*RX_AIO_CHANNEL;id<=2*RX_AIO_CHANNEL+1;id++) {
        rx_aio_write(rx_aio_fifo_address(id)+RA_FiFo_START,0);
        rx_aio_write(rx_aio_fifo_address(id)+RA_FiFo_CLEAR,1);
    }
    rx_aio_barrier();
    unsigned i;
    for (i=0;i<200;i++) {
        if (!(hbo_queue_busy(dhub_hbo(&rx_exclusive_aio.dhub))&(3u<<(2*RX_AIO_CHANNEL)))) break;
        TEE_Result r=TEE_Wait(1); if(r) return r;
    }
    if(i==200) return TEE_ERROR_BUSY;
    T32dHubChannel_CFG cfg={.u32=0}; T32dHubChannel_ARPARAMS ar={.u32=0};
    cfg.uCFG_MTU=dHubChannel_CFG_MTU_256byte; cfg.uCFG_QoS=1; ar.uARPARAMS_PROT=2;
    rx_aio_write(rx_aio_channel_address()+RA_dHubChannel_CFG,cfg.u32);
    rx_aio_write(rx_aio_channel_address()+RA_dHubChannel_ARPARAMS,ar.u32);
    rx_aio_fifo(2*RX_AIO_CHANNEL,0x4a00,128);
    rx_aio_fifo(2*RX_AIO_CHANNEL+1,0x4a80,384);
    rx_aio_issued_cfg=cfg.u32;rx_aio_issued_command=0x4a00;rx_aio_issued_data=0x4a80;
    HDL_semaphore *sem=dhub_semaphore(&rx_exclusive_aio.dhub);
    semaphore_cfg(sem,RX_AIO_CHANNEL,1,NULL);
    semaphore_intr_enable(sem,RX_AIO_CHANNEL,0,0,0,0,0);
    rx_aio_write(sem->ra+RA_SemaHub_ARR+RX_AIO_CHANNEL*sizeof(SIE_Semaphore)+RA_Semaphore_mask,0);
    semaphore_clr_full(sem,RX_AIO_CHANNEL);
    rx_exclusive_aio.dhub.MTUb[RX_AIO_CHANNEL]=8;
    dhub_channel_enable(&rx_exclusive_aio.dhub,RX_AIO_CHANNEL,1,NULL);
    rx_aio_issued_start=1;
    rx_aio_barrier();
    if (!rx_aio_quiet())
        return TEE_ERROR_BAD_STATE;
    return TEE_SUCCESS;
}
static inline HDL_dhub2d *rx_aio_exclusive_handle(uint64_t epoch)
{
    return rx_exclusive_aio_owned && epoch==rx_exclusive_aio_epoch &&
        rx_command_pool_bound(epoch) ? &rx_exclusive_aio : NULL;
}
static TEE_Result rx_aio_exclusive_release(uint64_t epoch)
{
    rx_aio_handle();
    if (!rx_exclusive_aio_owned || epoch!=rx_exclusive_aio_epoch ||
        rx_command_pool.dma_armed) return TEE_ERROR_BAD_STATE;
    if (!rx_aio_quiet()) return TEE_ERROR_BUSY;
    dhub_channel_enable(&rx_exclusive_aio.dhub,RX_AIO_CHANNEL,0,NULL);
    for (unsigned id=2*RX_AIO_CHANNEL;id<=2*RX_AIO_CHANNEL+1;id++)
        rx_aio_write(rx_aio_fifo_address(id)+RA_FiFo_START,0);
    rx_aio_barrier();
    rx_aio_issued_start=0;
    if (!rx_aio_quiet())
        return TEE_ERROR_BUSY;
    rx_exclusive_aio_owned=0; rx_exclusive_aio_epoch=0;
    return TEE_SUCCESS;
}
static TEE_Result rx_aio_exclusive_command(uint32_t types, TEE_Param p[4], int owner_live)
{
    if (types!=TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE) || !p[0].memref.buffer ||
        p[0].memref.size!=sizeof(struct rx_aio_exclusive_command)) return TEE_ERROR_BAD_PARAMETERS;
    struct rx_aio_exclusive_command c;
    TEE_MemMove(&c,p[0].memref.buffer,sizeof(c));
    if(c.version!=RX_AIO_EXCLUSIVE_VERSION || c.size!=sizeof(c) || c.acknowledgement ||
        c.issued_channel_start || c.issued_channel_cfg || c.issued_command_fifo_cfg || c.issued_data_fifo_cfg ||
        c.bcm_empty || c.autopush || c.error || c.owned ||
        c.dhub_base || c.hbo_base || c.declared_base || c.declared_hbo) return TEE_ERROR_BAD_PARAMETERS;
    rx_aio_handle(); TEE_Result r;
    if(c.operation==RX_AIO_STATUS) r=TEE_SUCCESS;
    else {
        if(owner_live) return TEE_ERROR_BUSY;
        if(c.operation==RX_AIO_PREPARE) r=rx_aio_exclusive_prepare(c.epoch);
        else if(c.operation==RX_AIO_RELEASE) r=rx_aio_exclusive_release(c.epoch);
        else return TEE_ERROR_BAD_PARAMETERS;
    }
    if(r) return r;
    c.acknowledgement=RX_AIO_EXCLUSIVE_ACK;
    c.issued_channel_start=rx_aio_issued_start;
    c.issued_channel_cfg=rx_aio_issued_cfg;
    c.issued_command_fifo_cfg=rx_aio_issued_command;
    c.issued_data_fifo_cfg=rx_aio_issued_data;
    c.bcm_empty=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_EMP_STS);
    c.autopush=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_AUTOPUSH);
    c.error=rx_aio_read(RX_AIO_BCM_BASE+RA_AVIO_BCM_ERROR); c.owned=rx_exclusive_aio_owned;
    c.dhub_base=rx_exclusive_aio.dhub.ra; c.hbo_base=rx_exclusive_aio.dhub.hbo.ra;
    c.declared_base=AG_DHUB_BASE; c.declared_hbo=AG_HBO_SRAM_BASE;
    TEE_MemMove(p[0].memref.buffer,&c,sizeof(c)); return TEE_SUCCESS;
}

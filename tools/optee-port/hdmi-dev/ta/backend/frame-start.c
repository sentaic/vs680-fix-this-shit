/* SPDX-License-Identifier: GPL-2.0 */
/* Dolphin BCM Q0 在 CPCB0 VBI 上触发 AIO 读取。 */
#include "frame-start.h"
#include "api_avio_dhub.h"
#include "avio.h"
#include "vpp.h"
#include <tee_api_syna.h>

/* SDK结构布局变化时停止编译，避免再次交换MEM与HDR。 */
_Static_assert(sizeof(SIE_dHubCmd)==8,"dHub command must contain two words");
_Static_assert(__builtin_offsetof(SIE_dHubCmd,u32dHubCmd_MEM)==0,"dHub MEM must be first");
_Static_assert(__builtin_offsetof(SIE_dHubCmd,ie_HDR)==4,"dHub HDR must be second");

#define HUB RA_dHubReg2D_dHub
#define HBO (HUB + RA_dHubReg_HBO)
#define SEMA (HUB + RA_dHubReg_SemaHub)
#define CHANNEL (HUB + RA_dHubReg_ARR + avioDhubChMap_aio64b_BCM_R * sizeof(SIE_dHubChannel))
#define BCM_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_BCM_REG_BASE)
#define LIST_OFFSET HDMI_DMA_POOL_ALIGN
#define VBI_ID 16u

static void frame_barrier(void) { __asm__ volatile("dsb sy" ::: "memory"); }
static void aio_write(struct hdmi_frame_start *s, uint32_t offset, uint32_t value)
{ s->registers->aio[offset / 4u] = value; }
static void vpp_write(struct hdmi_frame_start *s, uint32_t offset, uint32_t value)
{ s->registers->vpp[offset / 4u] = value; }
static TEE_Result update(struct hdmi_frame_start *s, uint32_t address,
                         uint32_t mask, uint32_t value)
{
    uint32_t old;
    TEE_Result result=s->io.read32(s->io.opaque,address,&old);
    if (result) return result;
    return s->io.write32(s->io.opaque,address,(old & ~mask) | (value & mask));
}
static void fifo(struct hdmi_frame_start *s, unsigned id, uint32_t base,
                  uint32_t bytes)
{
    uint32_t offset=HBO + RA_HBO_ARR + id * sizeof(SIE_FiFo);
    T32FiFo_CFG cfg={.u32=0};
    T32Semaphore_CFG depth={.u32=0};
    cfg.uCFG_BASE=base; depth.uCFG_DEPTH=bytes / 8u;
    aio_write(s,offset + RA_FiFo_START,0);
    aio_write(s,offset + RA_FiFo_CFG,cfg.u32);
    aio_write(s,HBO + RA_HBO_FiFoCtl + RA_SemaHub_ARR +
        id*sizeof(SIE_Semaphore) + RA_Semaphore_CFG,depth.u32);
    aio_write(s,offset + RA_FiFo_START,1);
}
static TEE_Result clear_aio_fifos(struct hdmi_frame_start *s)
{
    unsigned command=2u*avioDhubChMap_aio64b_BCM_R;
    for (unsigned id=command;id<=command+1u;++id) {
        uint32_t offset=HBO + RA_HBO_ARR + id*sizeof(SIE_FiFo);
        aio_write(s,offset + RA_FiFo_START,0);
        aio_write(s,offset + RA_FiFo_CLEAR,1);
    }
    frame_barrier();
    for (unsigned i=0;i<200;++i) {
        if (!(s->registers->aio[(HBO+RA_HBO_BUSY)/4u] & (3u<<command)))
            return TEE_SUCCESS;
        TEE_Result result=TEE_Wait(1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
}
TEE_Result hdmi_frame_start_init(struct hdmi_frame_start *s,
    struct hdmi_avio_registers *registers, const struct hdmi_mode_io *io,
    struct hdmi_dma_pool *pool)
{
    struct hdmi_avio_activity activity;
    uint32_t empty, autopush;
    T32dHubChannel_CFG cfg={.u32=0};
    T32dHubChannel_ARPARAMS read_params={.u32=0};
    T32Semaphore_CFG depth={.u32=0};
    TEE_Result result;
    if (!s || !registers || !io || !io->read32 || !io->write32 ||
        !pool || !pool->mapped || !pool->registered ||
        pool->bytes < HDMI_DMA_POOL_MIN_BYTES) return TEE_ERROR_BAD_PARAMETERS;
    if (s->configured || s->queued) return TEE_ERROR_BAD_STATE;
    result=hdmi_avio_tx_snapshot(registers,&activity);
    if (result) return result;
    if (!hdmi_avio_idle(&activity)) return TEE_ERROR_BUSY;
    *s=(struct hdmi_frame_start){.registers=registers,.io=*io,.pool=pool,.mixer={.alpha_q12=4095}};
    result=io->read32(io->opaque,BCM_BASE + RA_AVIO_BCM_EMP_STS,&empty);
    if (!result) result=io->read32(io->opaque,BCM_BASE + RA_AVIO_BCM_AUTOPUSH,&autopush);
    if (result) return result;
    if (!(empty & 1u) || (autopush & 1u)) return TEE_ERROR_BUSY;
    /* avio_dhub_cfg.c：BCM_R命令128B、数据384B，SRAM 0x4a00/0x4a80。 */
    cfg.uCFG_MTU=dHubChannel_CFG_MTU_256byte;
    cfg.uCFG_QoS=1;
    aio_write(s,CHANNEL + RA_dHubChannel_START,0);
    /* 深度配置重置计数，channel_cfg 另行清除 FIFO 读写指针。 */
    result=clear_aio_fifos(s);
    if (result) return result;
    aio_write(s,CHANNEL + RA_dHubChannel_CFG,cfg.u32);
    /* 池来自NonSecure ION；AXI ARPROT[1]=1指定非安全数据读取。 */
    read_params.uARPARAMS_PROT=2;
    aio_write(s,CHANNEL + RA_dHubChannel_ARPARAMS,read_params.u32);
    fifo(s,2u*avioDhubChMap_aio64b_BCM_R,0x4a00,128);
    fifo(s,2u*avioDhubChMap_aio64b_BCM_R+1u,0x4a80,384);
    depth.uCFG_DEPTH=1;
    aio_write(s,SEMA + RA_SemaHub_ARR + avioDhubChMap_aio64b_BCM_R *
        sizeof(SIE_Semaphore) + RA_Semaphore_CFG,depth.u32);
    aio_write(s,SEMA + RA_SemaHub_full,1u << avioDhubChMap_aio64b_BCM_R);
    aio_write(s,CHANNEL + RA_dHubChannel_START,1);
    vpp_write(s,SEMA + RA_SemaHub_ARR + VBI_ID*sizeof(SIE_Semaphore) +
        RA_Semaphore_CFG,depth.u32);
    vpp_write(s,SEMA + RA_SemaHub_full,1u << VBI_ID);
    frame_barrier();
    result=update(s,BCM_BASE + RA_AVIO_BCM_Q0,0x3f,BCM_SCHED_TRIG_CPCB0_VBI);
    if (!result) result=update(s,UINT32_C(0xf7490008),1,1);
    if (result) return result;
    s->configured=true;
    return TEE_SUCCESS;
}
TEE_Result hdmi_frame_start_set_mixer(struct hdmi_frame_start *s,
    uint32_t background_rgb, uint32_t alpha_q12)
{
    if (!s || background_rgb>0xffffffu || alpha_q12>4095u)
        return TEE_ERROR_BAD_PARAMETERS;
    if (!s->configured) return TEE_ERROR_BAD_STATE;
    s->mixer=(struct hdmi_mixer){background_rgb,alpha_q12};
    /* 当前已提交的物理表保持不变，新参数在下一次VBI表中使用。 */
    s->prepared=false;
    return TEE_SUCCESS;
}
static uint32_t cell_count(volatile uint32_t *registers, uint32_t base, unsigned id)
{
    uint32_t query=(id<<LSb32SemaQueryMap_ADDR_ID) |
        (SemaQueryMap_ADDR_master_consumer<<LSb32SemaQueryMap_ADDR_master);
    return registers[(base + RA_SemaHub_Query + query)/4u] & MSK32SemaQuery_RESP_CNT;
}
static TEE_Result pop_cell(volatile uint32_t *registers, unsigned id)
{
    uint32_t count=cell_count(registers,SEMA,id);
    if (count>1u) return TEE_ERROR_BAD_STATE;
    if (count) {
        T32SemaHub_POP pop={.u32=0};
        pop.uPOP_ID=id; pop.uPOP_delta=1;
        registers[(SEMA + RA_SemaHub_POP)/4u]=pop.u32;
        frame_barrier();
    }
    registers[(SEMA + RA_SemaHub_full)/4u]=1u<<id;
    frame_barrier();
    return !cell_count(registers,SEMA,id) &&
        !(registers[(SEMA + RA_SemaHub_full)/4u] & (1u<<id)) ?
        TEE_SUCCESS : TEE_ERROR_BUSY;
}
static TEE_Result acknowledge_vbi(struct hdmi_frame_start *s)
{
    uint32_t count=cell_count(s->registers->vpp,SEMA,VBI_ID);
    if (count>1u) return TEE_ERROR_BAD_STATE;
    if (count) {
        T32SemaHub_POP pop={.u32=0};
        pop.uPOP_ID=VBI_ID; pop.uPOP_delta=1;
        vpp_write(s,SEMA+RA_SemaHub_POP,pop.u32);
        frame_barrier();
    }
    vpp_write(s,SEMA+RA_SemaHub_full,1u<<VBI_ID);
    frame_barrier();
    /* VBI 持续产生事件，保留新事件增加的信号量计数。 */
    return TEE_SUCCESS;
}
TEE_Result hdmi_frame_start_ready(struct hdmi_frame_start *s)
{
    uint32_t error, empty, channel=avioDhubChMap_aio64b_BCM_R;
    TEE_Result result;
    if (!s || !s->configured) return TEE_ERROR_BAD_STATE;
    if (s->repeating || s->switch_pending) return TEE_ERROR_BUSY;
    result=s->io.read32(s->io.opaque,BCM_BASE + RA_AVIO_BCM_ERROR,&error);
    if (result) return result;
    if (error) return TEE_ERROR_BAD_STATE;
    if (!s->queued) return TEE_SUCCESS;
    result=s->io.read32(s->io.opaque,BCM_BASE + RA_AVIO_BCM_EMP_STS,&empty);
    if (result) return result;
    uint32_t busy=s->registers->aio[(HUB+RA_dHubReg_BUSY)/4u] |
        s->registers->aio[(HUB+RA_dHubReg_PENDING)/4u] |
        s->registers->aio[RA_dHubReg2D_BUSY/4u];
    if (!(empty&1u) || (busy&(1u<<channel)) ||
        cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,2*channel) ||
        cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,2*channel+1)) return TEE_ERROR_BUSY;
    if (!(s->registers->aio[(SEMA+RA_SemaHub_full)/4u] & (1u<<channel)))
        return TEE_ERROR_BUSY;
    if (cell_count(s->registers->aio,SEMA,channel)!=1u) return TEE_ERROR_BAD_STATE;
    result=pop_cell(s->registers->aio,channel);
    if (!result) result=acknowledge_vbi(s);
    if (result) return result;
    /* AIO已完成且命令/数据FIFO空；下一次表可重建，提交期间保持不变。 */
    s->queued=false; s->prepared=false;
    return TEE_SUCCESS;
}
static TEE_Result prepare_start_table(struct hdmi_frame_start *s,
    const struct hdmi_gfx_dma *dma)
{
    uint32_t gfx, vop, cpcb, gcp, error, dtg_status, dtg_control, pip=0;
    TEE_Result result;
    result=s->io.read32(s->io.opaque,UINT32_C(0xf7491008),&gfx);
    if (!result) result=s->io.read32(s->io.opaque,UINT32_C(0xf7480a94),&vop);
    if (!result) result=s->io.read32(s->io.opaque,UINT32_C(0xf7492a3c),&cpcb);
    if (!result) result=s->io.read32(s->io.opaque,UINT32_C(0xf7444060),&gcp);
    if (!result) result=s->io.read32(s->io.opaque,UINT32_C(0xf7480bec),&dtg_status);
    if (!result) result=s->io.read32(s->io.opaque,UINT32_C(0xf7480be0),&dtg_control);
    if (!result) result=s->io.write32(s->io.opaque,BCM_BASE + RA_AVIO_BCM_ERROR,1);
    if (!result) result=s->io.read32(s->io.opaque,BCM_BASE + RA_AVIO_BCM_ERROR,&error);
    if (!result && s->overlay)
        result=s->io.read32(s->io.opaque,UINT32_C(0xf7490410),&pip);
    if (result) return result;
    if (error) return TEE_ERROR_BAD_STATE;
    if (s->overlay && (!s->overlay->configured || !s->overlay->active))
        return TEE_ERROR_BAD_STATE;
    pip &= ~UINT32_C(0xf);
    if (s->overlay) cpcb |= 4u;
    gfx &= ~UINT32_C(0xf); vop &= ~UINT32_C(3); cpcb &= ~UINT32_C(0x80);
    const struct hdmi_register_write writes[]={
        {gfx|0xa,0xf7491008},{gfx,0xf7491008},
        {vop|2,0xf7480a94},{vop,0xf7480a94},
        {gfx|5,0xf7491008},{gfx,0xf7491008},
        {vop|1,0xf7480a94},{vop,0xf7480a94},
        {cpcb|0x80,0xf7492a3c},{cpcb,0xf7492a3c},
        {(gcp & ~UINT32_C(3))|1,0xf7444060}
    };
    struct hdmi_register_write sequence[22 + 2u*HDMI_GFX_PROGRAM_WRITES + HDMI_MIXER_WRITES];
    _Static_assert(sizeof(sequence)<=384,"BCM table exceeds configured AIO data FIFO");
    size_t count=4;
    TEE_MemMove(sequence,writes,4*sizeof(writes[0]));
    if (s->overlay) {
        sequence[count++]=(struct hdmi_register_write){pip|0xa,0xf7490410};
        sequence[count++]=(struct hdmi_register_write){pip,0xf7490410};
        sequence[count++]=(struct hdmi_register_write){2,0xf749051c};
        sequence[count++]=(struct hdmi_register_write){0,0xf749051c};
    }
    result=hdmi_mixer_program(&s->mixer,&s->io,sequence+count);
    if (result) return result;
    count+=HDMI_MIXER_WRITES;
    if (dtg_status & (UINT32_C(1)<<14)) {
        /* VPP_DTGUF_Handler：保持bit2及其它控制位，在VBI清内部FIFO。 */
        sequence[count++]=(struct hdmi_register_write){dtg_control&~3u,0xf7480be0};
        sequence[count++]=(struct hdmi_register_write){dtg_control|3u,0xf7480be0};
        sequence[count++]=(struct hdmi_register_write){dtg_control&~3u,0xf7480be0};
    }
    /* 先启动全部 FE，再启动 VOP/CPCB，最后启动输出 TG。 */
    TEE_MemMove(sequence+count,writes+4,2*sizeof(writes[0]));
    count+=2;
    if (s->overlay) {
        sequence[count++]=(struct hdmi_register_write){pip|5,0xf7490410};
        sequence[count++]=(struct hdmi_register_write){pip,0xf7490410};
        sequence[count++]=(struct hdmi_register_write){1,0xf749051c};
        sequence[count++]=(struct hdmi_register_write){0,0xf749051c};
    }
    TEE_MemMove(sequence+count,writes+6,5*sizeof(writes[0]));
    count+=5;
    TEE_MemMove(sequence+count,dma->program,sizeof(dma->program));
    count+=HDMI_GFX_PROGRAM_WRITES;
    if (s->overlay) {
        TEE_MemMove(sequence+count,s->overlay->program,sizeof(s->overlay->program));
        count+=HDMI_GFX_PROGRAM_WRITES;
    }
    void *list=(char *)s->pool->mapped+LIST_OFFSET+s->table_slot*HDMI_DMA_POOL_ALIGN;
    TEE_MemMove(list,sequence,count*sizeof(sequence[0]));
    result=TEE_CacheClean(list,count*sizeof(sequence[0]));
    if (result) return result;
    s->list_bytes=count*sizeof(sequence[0]); s->prepared=true;
    return TEE_SUCCESS;
}
TEE_Result hdmi_frame_start_queue(struct hdmi_frame_start *s,
    const struct hdmi_gfx_dma *dma)
{
    uint32_t full;
    SIE_dHubCmd command={0};
    TEE_Result result;
    if (!s || !dma || !dma->configured || !dma->active ||
        !s->configured || s->queued || s->table_slot>1u || !s->pool->dma_armed)
        return TEE_ERROR_BAD_STATE;
    result=s->io.read32(s->io.opaque,BCM_BASE + RA_AVIO_BCM_FULL_STS,&full);
    if (result) return result;
    if (full&1u) return TEE_ERROR_BUSY;
    if (!s->prepared) {
        result=prepare_start_table(s,dma);
        if (result) return result;
    }
    command.ie_HDR.uDESC_size=s->list_bytes;
    command.ie_HDR.uDESC_ovrdQos=1;
    command.ie_HDR.uDESC_qosSel=1;
    command.ie_HDR.uDESC_interrupt=1;
    command.uMEM_addr=s->pool->pa + LIST_OFFSET + s->table_slot*HDMI_DMA_POOL_ALIGN;
    result=s->io.write32(s->io.opaque,BCM_BASE,command.uMEM_addr);
    if (result) return result;
    s->queued=true;
    return s->io.write32(s->io.opaque,BCM_BASE+4,command.ie_HDR.u32dHubCmdHDR_DESC);
}

TEE_Result hdmi_frame_start_repeat(struct hdmi_frame_start *s, bool enabled)
{
    uint32_t actual;
    if (!s || !s->configured || (enabled && !s->queued)) return TEE_ERROR_BAD_STATE;
    TEE_Result rc=update(s,BCM_BASE+RA_AVIO_BCM_AUTOPUSH,1u,enabled?1u:0u);
    if (rc) return rc;
    frame_barrier();
    rc=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_AUTOPUSH,&actual);
    if (rc) return rc;
    if ((actual&1u)!=(enabled?1u:0u)) return TEE_ERROR_BUSY;
    s->repeating=enabled;
    return TEE_SUCCESS;
}

TEE_Result hdmi_frame_start_applied(struct hdmi_frame_start *s)
{
    uint32_t error, empty, response, again;
    const unsigned channel=avioDhubChMap_aio64b_BCM_R;
    const uint32_t query=(2u*channel<<LSb32SemaQueryMap_ADDR_ID) |
        (SemaQueryMap_ADDR_master_consumer<<LSb32SemaQueryMap_ADDR_master);
    const uint32_t query_offset=HBO+RA_HBO_FiFoCtl+RA_SemaHub_Query+query;
    SIE_dHubCmd consumed={0};
    if (!s || !s->configured || !s->queued || s->table_slot>1u)
        return TEE_ERROR_BAD_STATE;
    TEE_Result rc=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_ERROR,&error);
    if (!rc) rc=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_EMP_STS,&empty);
    if (rc) return rc;
    if (error) return TEE_ERROR_BAD_STATE;
    uint32_t busy=s->registers->aio[(HUB+RA_dHubReg_BUSY)/4u] |
        s->registers->aio[(HUB+RA_dHubReg_PENDING)/4u] |
        s->registers->aio[RA_dHubReg2D_BUSY/4u];
    response=s->registers->aio[query_offset/4u];
    if (!(empty&1u) || (busy&(1u<<channel)) ||
        (response&MSK32SemaQuery_RESP_CNT) ||
        cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,2u*channel+1u))
        return TEE_ERROR_BUSY;
    unsigned pointer=(response&MSK32SemaQuery_RESP_PTR)>>LSb32SemaQuery_RESP_PTR;
    if (pointer>=16u) return TEE_ERROR_BAD_STATE;
    /* 配置的命令FIFO为128B、16个64位项；消费者位置指向下一项。 */
    uint32_t address=AG_HBO_SRAM_BASE+0x4a00u+((pointer+15u)%16u)*sizeof(consumed);
    rc=s->io.read32(s->io.opaque,address,&consumed.u32dHubCmd_MEM);
    if (!rc) rc=s->io.read32(s->io.opaque,address+4u,&consumed.ie_HDR.u32dHubCmdHDR_DESC);
    if (rc) return rc;
    frame_barrier();
    again=s->registers->aio[query_offset/4u];
    if (again!=response ||
        consumed.uMEM_addr!=s->pool->pa+LIST_OFFSET+s->table_slot*HDMI_DMA_POOL_ALIGN ||
        consumed.ie_HDR.uDESC_size!=s->list_bytes) return TEE_ERROR_BUSY;
    /* 描述符读取期间DMA可开始执行，结束时重新核对全部AIO活动状态。 */
    busy=s->registers->aio[(HUB+RA_dHubReg_BUSY)/4u] |
        s->registers->aio[(HUB+RA_dHubReg_PENDING)/4u] |
        s->registers->aio[RA_dHubReg2D_BUSY/4u];
    if ((busy&(1u<<channel)) ||
        cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,2u*channel) ||
        cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,2u*channel+1u))
        return TEE_ERROR_BUSY;
    /* 重复扫描会合并完成事件；实际消费的表地址负责区分新旧表。 */
    if (!(s->registers->aio[(SEMA+RA_SemaHub_full)/4u]&(1u<<channel)))
        return TEE_ERROR_BUSY;
    return cell_count(s->registers->aio,SEMA,channel)==1u ?
        TEE_SUCCESS : TEE_ERROR_BAD_STATE;
}

TEE_Result hdmi_frame_start_switch(struct hdmi_frame_start *s,
    const struct hdmi_gfx_dma *gfx, const struct hdmi_gfx_dma *overlay)
{
    uint32_t full;
    if (!s || !gfx || !s->queued || s->switch_pending)
        return TEE_ERROR_BAD_STATE;
    bool was_repeating=s->repeating;
    TEE_Result rc=hdmi_frame_start_repeat(s,false);
    if (rc) return rc;
    rc=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_FULL_STS,&full);
    if (rc) return rc;
    if (full&1u) {
        if (was_repeating) rc=hdmi_frame_start_repeat(s,true);
        return rc ? rc : TEE_ERROR_BUSY;
    }
    /* 自动重投停用后提交两字描述符；当前表及其源始终保持不变。 */
    s->table_slot^=1u;
    s->overlay=overlay;
    s->queued=false;
    s->prepared=false;
    s->switch_pending=true;
    rc=hdmi_frame_start_queue(s,gfx);
    /* 新表先执行一次，给旧DMA留下可核对的静止状态；回收后恢复自动重投。 */
    /* 描述符开始更新后若返回 BUSY，保留引用并进入故障恢复。 */
    return rc==TEE_ERROR_BUSY ? TEE_ERROR_BAD_STATE : rc;
}

TEE_Result hdmi_frame_start_release_old(struct hdmi_frame_start *s)
{
    if (!s || !s->switch_pending || !s->queued)
        return TEE_ERROR_BAD_STATE;
    s->switch_pending=false;
    return TEE_SUCCESS;
}

TEE_Result hdmi_frame_start_stop(struct hdmi_frame_start *s)
{
    uint32_t empty,automatic,busy;
    if(!s||!s->configured)return TEE_ERROR_BAD_STATE;
    TEE_Result result=hdmi_frame_start_repeat(s,false);
    if(result)return result;
    /* 保留VBI直到已排队的Q0表执行完；空队列之后才关调度入口。 */
    for(unsigned poll=0;poll<200;poll++) {
        result=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_EMP_STS,&empty);
        if(!result)result=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_AUTOPUSH,&automatic);
        if(result)return result;
        frame_barrier();
        busy=s->registers->aio[(HUB+RA_dHubReg_BUSY)/4u]|
             s->registers->aio[(HUB+RA_dHubReg_PENDING)/4u]|
             s->registers->aio[RA_dHubReg2D_BUSY/4u];
        if((empty&1u)&&!(automatic&1u)&&!(busy&(1u<<8))&&
           !cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,16u)&&
           !cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,17u)) {
            /* 核对最后消费的表地址和完成事件后，关闭 Q0。 */
            if(!s->queued)goto drained;
            result=hdmi_frame_start_applied(s);
            if(!result)goto drained;
            if(result!=TEE_ERROR_BUSY)return result;
        }
        result=s->io.wait_ms(s->io.opaque,1);if(result)return result;
    }
    return TEE_ERROR_BUSY;
drained:
    result=update(s,BCM_BASE+RA_AVIO_BCM_Q0,0x3fu,BCM_SCHED_TRIG_NONE);
    if(result)return result;
    aio_write(s,CHANNEL+RA_dHubChannel_START,0);
    aio_write(s,CHANNEL+RA_dHubChannel_CLEAR,1);
    frame_barrier();
    for(unsigned poll=0;poll<200;poll++) {
        busy=s->registers->aio[(HUB+RA_dHubReg_BUSY)/4u]|
             s->registers->aio[(HUB+RA_dHubReg_PENDING)/4u]|
             s->registers->aio[RA_dHubReg2D_BUSY/4u];
        if(!(busy&(1u<<8)))goto clear_fifo;
        result=s->io.wait_ms(s->io.opaque,1);if(result)return result;
    }
    return TEE_ERROR_BUSY;
clear_fifo:
    result=clear_aio_fifos(s);if(result)return result;
    if(cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,16u)||
       cell_count(s->registers->aio,HBO+RA_HBO_FiFoCtl,17u))return TEE_ERROR_BUSY;
    result=pop_cell(s->registers->aio,8u);if(result)return result;
    result=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_EMP_STS,&empty);
    if(!result)result=s->io.read32(s->io.opaque,BCM_BASE+RA_AVIO_BCM_AUTOPUSH,&automatic);
    if(result)return result;
    if(!(empty&1u)||(automatic&1u))return TEE_ERROR_BUSY;
    s->configured=s->queued=s->prepared=s->repeating=s->switch_pending=false;
    return TEE_SUCCESS;
}

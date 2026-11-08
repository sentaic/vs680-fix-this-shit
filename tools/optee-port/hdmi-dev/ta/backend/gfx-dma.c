/* SPDX-License-Identifier: GPL-2.0 */
#include "gfx-dma.h"
#include "api_avio_dhub.h"
#include <tee_internal_api_extensions.h>

#define GFX (dma->channel)
#define GFX_BIT (UINT32_C(1) << GFX)
#define HUB RA_dHubReg2D_dHub
#define HBO (HUB + RA_dHubReg_HBO)
#define SEMA (HUB + RA_dHubReg_SemaHub)
#define FIFO(id) (HBO + RA_HBO_ARR + (id) * sizeof(SIE_FiFo))
#define CHANNEL (HUB + RA_dHubReg_ARR + GFX * sizeof(SIE_dHubChannel))
#define CMD_2D (RA_dHubReg2D_ARR + GFX * sizeof(SIE_dHubCmd2D))
#define CMD_2ND (RA_dHubReg2D_ARR_2ND + GFX * sizeof(SIE_dHubCmd2ND))
#define FIFO_WORD_BYTES 16u
#define POLLS 200u
/* Dolphin avio_dhub_cfg.c 的 GFX_R1 SRAM 分区，保持其他通道的空间。 */
static const DHUB_channel_config gfx_profile = {
    .chanId = avioDhubChMap_vpp128b_GFX_R1,
    .chanCmdBase = 0x77a0,
    .chanDataBase = 0x77e0,
    .chanCmdSize = 64,
    .chanDataSize = 23040 - 64,
    .chanQos = 1,
};

static const DHUB_channel_config pip_profile = {
    .chanId = avioDhubChMap_vpp128b_GFX_PIP_R0,
    .chanCmdBase = 0x4140, .chanDataBase = 0x4180,
    .chanCmdSize = 64, .chanDataSize = 11520 - 64, .chanQos = 1,
};

static uint32_t read_word(struct hdmi_gfx_dma *dma, uint32_t offset)
{
    return dma->registers->vpp[offset / 4u];
}
static void write_word(struct hdmi_gfx_dma *dma, uint32_t offset, uint32_t value)
{
    dma->registers->vpp[offset / 4u] = value;
}
static void device_barrier(void)
{
    __asm__ volatile("dsb sy" ::: "memory");
}
static uint32_t count(struct hdmi_gfx_dma *dma, uint32_t sema, unsigned id)
{
    const uint32_t query = (id << LSb32SemaQueryMap_ADDR_ID) |
        (SemaQueryMap_ADDR_master_consumer << LSb32SemaQueryMap_ADDR_master);
    return read_word(dma, sema + RA_SemaHub_Query + query) & MSK32SemaQuery_RESP_CNT;
}
static bool dma_busy(struct hdmi_gfx_dma *dma)
{
    return (read_word(dma, HUB + RA_dHubReg_BUSY) |
        read_word(dma, HUB + RA_dHubReg_PENDING) |
        read_word(dma, RA_dHubReg2D_BUSY)) & GFX_BIT;
}
static TEE_Result consume_completion(struct hdmi_gfx_dma *dma)
{
    T32SemaHub_POP pop = {.u32 = 0};
    uint32_t credits = count(dma, SEMA, GFX);
    if (credits > 1u) return TEE_ERROR_BAD_STATE;
    if (credits) {
        pop.uPOP_ID = GFX;
        pop.uPOP_delta = 1;
        write_word(dma, SEMA + RA_SemaHub_POP, pop.u32);
        device_barrier();
    }
    write_word(dma, SEMA + RA_SemaHub_full, GFX_BIT);
    device_barrier();
    return !count(dma, SEMA, GFX) &&
        !(read_word(dma, SEMA + RA_SemaHub_full) & GFX_BIT) ?
        TEE_SUCCESS : TEE_ERROR_BUSY;
}
static void configure_fifo(struct hdmi_gfx_dma *dma, unsigned id,
                           uint32_t base, uint32_t bytes)
{
    T32FiFo_CFG cfg = {.u32 = 0};
    T32Semaphore_CFG sem = {.u32 = 0};
    cfg.uCFG_BASE = base;
    sem.uCFG_DEPTH = bytes / FIFO_WORD_BYTES;
    write_word(dma, FIFO(id) + RA_FiFo_START, 0);
    write_word(dma, FIFO(id) + RA_FiFo_CFG, cfg.u32);
    write_word(dma, HBO + RA_HBO_FiFoCtl + RA_SemaHub_ARR +
               id * sizeof(SIE_Semaphore) + RA_Semaphore_CFG, sem.u32);
    write_word(dma, FIFO(id) + RA_FiFo_START, 1);
}

static TEE_Result clear_initial_fifos(struct hdmi_gfx_dma *dma)
{
    for (unsigned id=2u*GFX;id<=2u*GFX+1u;++id) {
        write_word(dma,FIFO(id) + RA_FiFo_START,0);
        write_word(dma,FIFO(id) + RA_FiFo_CLEAR,1);
    }
    device_barrier();
    for (unsigned i=0;i<POLLS;++i) {
        if (!(read_word(dma,HBO+RA_HBO_BUSY) & (3u<<(2u*GFX))))
            return TEE_SUCCESS;
        TEE_Result result=TEE_Wait(1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
}
static TEE_Result init_channel(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers, const DHUB_channel_config *profile,
    uint32_t line_bytes)
{
    struct hdmi_avio_activity activity;
    T32dHubChannel_CFG cfg = {.u32 = 0};
    T32dHubChannel_ARPARAMS read_params = {.u32 = 0};
    T32Semaphore_CFG completion = {.u32 = 0};
    TEE_Result result;
    if (!dma || !registers || !line_bytes || line_bytes > 8192u ||
        (line_bytes & 15u)) return TEE_ERROR_BAD_PARAMETERS;
    if (dma->configured || dma->active) return TEE_ERROR_BAD_STATE;
    result = hdmi_avio_tx_snapshot(registers, &activity);
    if (result) return result;
    if (!hdmi_avio_idle(&activity)) return TEE_ERROR_BUSY;
    if (profile->chanId != avioDhubChMap_vpp128b_GFX_R1 &&
        profile->chanId != avioDhubChMap_vpp128b_GFX_PIP_R0) return TEE_ERROR_BAD_PARAMETERS;
    dma->channel = profile->chanId;
    if (profile->chanCmdSize != 64 ||
        profile->chanDataSize <= 0 ||
        profile->chanCmdBase > UINT32_C(0x20000) - 64u ||
        profile->chanDataBase != profile->chanCmdBase + profile->chanCmdSize ||
        (unsigned)profile->chanDataSize > UINT32_C(0x20000) ||
        (unsigned)profile->chanDataBase > UINT32_C(0x20000) -
                                           (unsigned)profile->chanDataSize ||
        (profile->chanCmdBase % FIFO_WORD_BYTES) ||
        (profile->chanDataSize % FIFO_WORD_BYTES)) return TEE_ERROR_BAD_STATE;
    dma->registers = registers;
    write_word(dma, CHANNEL + RA_dHubChannel_START, 0);
    result = clear_initial_fifos(dma);
    if (result) return result;
    /* 按 vppDhubCfg2NDDMA 的规则，整行长度无法使用较大 MTU 时选 MTU128；1080p RGB24 每行为 5760 字节。 */
    dma->mtu_bytes = (line_bytes % 256u) ? 128u : 256u;
    cfg.uCFG_MTU = dma->mtu_bytes == 256u ?
        dHubChannel_CFG_MTU_256byte : dHubChannel_CFG_MTU_128byte;
    cfg.uCFG_QoS = profile->chanQos;
    cfg.uCFG_selfLoop = 0;
    write_word(dma, CHANNEL + RA_dHubChannel_CFG, cfg.u32);
    /* RGB24帧来自NonSecure ION；ARPROT[1]=1指定非安全数据读取。 */
    read_params.uARPARAMS_PROT = 2;
    write_word(dma, CHANNEL + RA_dHubChannel_ARPARAMS, read_params.u32);
    configure_fifo(dma, 2u * GFX, profile->chanCmdBase, profile->chanCmdSize);
    configure_fifo(dma, 2u * GFX + 1u, profile->chanDataBase, profile->chanDataSize);
    completion.uCFG_DEPTH = 1;
    write_word(dma, SEMA + RA_SemaHub_ARR + GFX * sizeof(SIE_Semaphore) +
               RA_Semaphore_CFG, completion.u32);
    write_word(dma, CHANNEL + RA_dHubChannel_START, 1);
    device_barrier();
    dma->configured = true;
    return consume_completion(dma);
}

TEE_Result hdmi_gfx_dma_init(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers)
{ return init_channel(dma, registers, &gfx_profile, 1280u * 3u); }
TEE_Result hdmi_gfx_dma_init_pip(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers)
{ return init_channel(dma, registers, &pip_profile, 1024u); }
TEE_Result hdmi_gfx_dma_init_geometry(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers, uint32_t line_bytes)
{ return init_channel(dma, registers, &gfx_profile, line_bytes); }
TEE_Result hdmi_gfx_dma_init_pip_geometry(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers, uint32_t line_bytes)
{ return init_channel(dma, registers, &pip_profile, line_bytes); }
TEE_Result hdmi_gfx_dma_submit(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, unsigned slot, uint64_t *submission)
{
    if (!submission) return TEE_ERROR_BAD_PARAMETERS;
    *submission = 0;
    if (!pool || slot >= hdmi_dma_pool_frame_count(pool)) return TEE_ERROR_BAD_PARAMETERS;
    return hdmi_gfx_dma_submit_region(dma, pool,
        hdmi_dma_pool_frame_offset(pool,slot),
        hdmi_dma_pool_row_stride(pool), hdmi_dma_pool_width(pool) * 3u,
        hdmi_dma_pool_height(pool), submission);
}

static TEE_Result prepare_region(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, uint32_t offset, uint32_t stride,
    uint32_t line_bytes, uint32_t height, uint64_t *submission, bool require_idle)
{
    SIE_dHubCmd2ND cmd = {0};
    T32dHubChannel_ROB_MAP rob = {.u32 = 0};
    uint32_t pa;
    TEE_Result result;
    if (!dma || !pool || !submission || !height || height > 1440u ||
        height > MSK32dHubCmd2ND_DESC_2D_SZ_size ||
        !line_bytes || line_bytes > 8192u ||
        line_bytes > MSK32dHubCmd2ND_DESC_burst || stride < line_bytes ||
        stride > MSK32dHubCmd2ND_DESC_2D_ST_step ||
        (stride % 16u) || (line_bytes % 16u) || (offset % 16u))
        return TEE_ERROR_BAD_PARAMETERS;
    *submission = 0;
    if (!dma->configured || dma->active) return TEE_ERROR_BAD_STATE;
    uint32_t mtu = dma->mtu_bytes ? dma->mtu_bytes : 256u;
    if ((mtu != 128u && mtu != 256u) || (mtu == 256u && line_bytes % 256u))
        return TEE_ERROR_BAD_PARAMETERS;
    if (dma->sequence == UINT64_MAX) return TEE_ERROR_OVERFLOW;
    if (require_idle && (dma_busy(dma) || count(dma, HBO + RA_HBO_FiFoCtl, 2u * GFX) ||
        count(dma, SEMA, GFX) || (read_word(dma, SEMA + RA_SemaHub_full) & GFX_BIT)))
        return TEE_ERROR_BUSY;
    uint64_t bytes = (uint64_t)stride * (height - 1u) + line_bytes;
    if (!pool->registered || !pool->mapped || offset >= pool->bytes ||
        bytes > pool->bytes - offset || pool->pa > UINT32_MAX - offset ||
        (uint64_t)pool->pa + offset + bytes - 1u > UINT32_MAX)
        return TEE_ERROR_BAD_PARAMETERS;
    pa = pool->pa + offset;
    result = hdmi_payload_ensure(&pool->payload_cache,pool->mapped,pool->bytes,
                                offset,(uint32_t)bytes);
    if (result) return result;
    device_barrier();
    /* vppDhubCfg2NDDMA：一行一个burst，支持行跨度，最后一行产生完成事件。 */
    cmd.uMEM_addr = pa;
    cmd.uDESC_burst = line_bytes;
    cmd.uDESC_interrupt = 1;
    cmd.uDESC_ovrdQos = 1;
    cmd.uDESC_qosSel = 1;
    cmd.uDESC_1D_ST_step = line_bytes;
    cmd.uDESC_1D_SZ_size = 1;
    cmd.uDESC_2D_ST_step = stride;
    cmd.uDESC_2D_SZ_size = height;
    /* vpp_isr.c：CLEAR表、SET表后执行DMA配置，统一由同一次VBI触发。 */
    const struct hdmi_register_write program[HDMI_GFX_PROGRAM_WRITES] = {
        {0, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_START},
        {rob.u32, VPP_DHUB_BASE + CHANNEL + RA_dHubChannel_ROB_MAP},
        {cmd.u32dHubCmd2ND_MEM, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_MEM},
        {cmd.u32dHubCmd2ND_DESC, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_DESC},
        {cmd.u32dHubCmd2ND_DESC_1D_ST, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_DESC_1D_ST},
        {cmd.u32dHubCmd2ND_DESC_1D_SZ, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_DESC_1D_SZ},
        {cmd.u32dHubCmd2ND_DESC_2D_ST, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_DESC_2D_ST},
        {cmd.u32dHubCmd2ND_DESC_2D_SZ, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_DESC_2D_SZ},
        {1, VPP_DHUB_BASE + CMD_2ND + RA_dHubCmd2ND_START},
    };
    for (unsigned i=0;i<HDMI_GFX_PROGRAM_WRITES;++i) dma->program[i]=program[i];
    dma->active = ++dma->sequence;
    hdmi_dma_pool_arm(pool);
    *submission = dma->active;
    return TEE_SUCCESS;
}

TEE_Result hdmi_gfx_dma_submit_region(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, uint32_t offset, uint32_t stride,
    uint32_t line_bytes, uint32_t height, uint64_t *submission)
{
    return prepare_region(dma,pool,offset,stride,line_bytes,height,submission,true);
}
TEE_Result hdmi_gfx_dma_prepare_repeat(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, uint32_t offset, uint32_t stride,
    uint32_t line_bytes, uint32_t height, uint64_t *submission)
{
    return prepare_region(dma,pool,offset,stride,line_bytes,height,submission,false);
}

TEE_Result hdmi_gfx_dma_source_matches(struct hdmi_gfx_dma *dma,
    const struct hdmi_mode_io *io)
{
    if (!dma || !dma->configured || !dma->active || !io || !io->read32 ||
        (dma->channel!=avioDhubChMap_vpp128b_GFX_R1 &&
         dma->channel!=avioDhubChMap_vpp128b_GFX_PIP_R0))
        return TEE_ERROR_BAD_STATE;
    /* 2ND MEM 为只写配置；从命令 SRAM 核对最后消费的行地址。 */
    const unsigned id=2u*GFX;
    const uint32_t query=(id<<LSb32SemaQueryMap_ADDR_ID) |
        (SemaQueryMap_ADDR_master_consumer<<LSb32SemaQueryMap_ADDR_master);
    const uint32_t offset=HBO+RA_HBO_FiFoCtl+RA_SemaHub_Query+query;
    uint32_t before=read_word(dma,offset);
    if (dma_busy(dma) || (before&MSK32SemaQuery_RESP_CNT)) return TEE_ERROR_BUSY;
    unsigned pointer=(before&MSK32SemaQuery_RESP_PTR)>>LSb32SemaQuery_RESP_PTR;
    if (pointer>=4u) return TEE_ERROR_BAD_STATE;
    uint32_t base=GFX==avioDhubChMap_vpp128b_GFX_R1 ?
        gfx_profile.chanCmdBase : pip_profile.chanCmdBase;
    SIE_dHubCmd consumed={0};
    uint32_t address=VPP_HBO_SRAM_BASE+base+((pointer+3u)%4u)*FIFO_WORD_BYTES;
    TEE_Result rc=io->read32(io->opaque,address,&consumed.u32dHubCmd_MEM);
    if (!rc) rc=io->read32(io->opaque,address+4,&consumed.ie_HDR.u32dHubCmdHDR_DESC);
    if (rc) return rc;
    device_barrier();
    if (read_word(dma,offset)!=before || dma_busy(dma)) return TEE_ERROR_BUSY;
    uint32_t rows=dma->program[7].value&MSK32dHubCmd2ND_DESC_2D_SZ_size;
    uint32_t stride=dma->program[6].value&MSK32dHubCmd2ND_DESC_2D_ST_step;
    uint32_t bytes=dma->program[3].value&MSK32dHubCmd2ND_DESC_burst;
    if (!rows) return TEE_ERROR_BAD_STATE;
    uint64_t last=(uint64_t)dma->program[2].value+(uint64_t)(rows-1u)*stride;
    if (last>UINT32_MAX) return TEE_ERROR_BAD_STATE;
    uint32_t mtu = dma->mtu_bytes ? dma->mtu_bytes : 256u;
    if (mtu != 128u && mtu != 256u) return TEE_ERROR_BAD_STATE;
    return consumed.uMEM_addr==last && consumed.ie_HDR.uDESC_interrupt &&
        consumed.ie_HDR.uDESC_size*(consumed.ie_HDR.uDESC_sizeMTU ? mtu : 1u)==bytes ? TEE_SUCCESS : TEE_ERROR_BUSY;
}

TEE_Result hdmi_gfx_dma_complete(struct hdmi_gfx_dma *dma, uint64_t *submission)
{
    TEE_Result result;
    if (!dma || !submission) return TEE_ERROR_BAD_PARAMETERS;
    *submission = 0;
    if (!dma->configured || !dma->active) return TEE_ERROR_BAD_STATE;
    if (!(read_word(dma, SEMA + RA_SemaHub_full) & GFX_BIT)) return TEE_ERROR_BUSY;
    if (count(dma, SEMA, GFX) != 1u) return TEE_ERROR_BAD_STATE;
    if (dma_busy(dma) || count(dma, HBO + RA_HBO_FiFoCtl, 2u * GFX))
        return TEE_ERROR_BUSY;
    result = consume_completion(dma);
    if (result) return result;
    *submission = dma->active;
    dma->active = 0;
    return TEE_SUCCESS;
}

TEE_Result hdmi_gfx_dma_stop(struct hdmi_gfx_dma *dma)
{
    TEE_Result result;
    if (!dma || !dma->registers) return TEE_ERROR_BAD_PARAMETERS;
    write_word(dma, CMD_2ND + RA_dHubCmd2ND_START, 0);
    write_word(dma, CMD_2ND + RA_dHubCmd2ND_CLEAR, 1);
    write_word(dma, CMD_2D + RA_dHubCmd2D_START, 0);
    write_word(dma, CMD_2D + RA_dHubCmd2D_CLEAR, 1);
    write_word(dma, CHANNEL + RA_dHubChannel_START, 0);
    write_word(dma, CHANNEL + RA_dHubChannel_CLEAR, 1);
    device_barrier();
    for (unsigned poll = 0; poll < POLLS; ++poll) {
        if (!dma_busy(dma)) goto clear_fifos;
        result = TEE_Wait(1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
clear_fifos:
    write_word(dma, FIFO(2u * GFX) + RA_FiFo_START, 0);
    write_word(dma, FIFO(2u * GFX + 1u) + RA_FiFo_START, 0);
    write_word(dma, FIFO(2u * GFX) + RA_FiFo_CLEAR, 1);
    write_word(dma, FIFO(2u * GFX + 1u) + RA_FiFo_CLEAR, 1);
    device_barrier();
    for (unsigned poll = 0; poll < POLLS; ++poll) {
        if (!(read_word(dma, HBO + RA_HBO_BUSY) & (3u << (2u * GFX))) &&
            !dma_busy(dma) && !count(dma, HBO + RA_HBO_FiFoCtl, 2u * GFX) &&
            !count(dma, HBO + RA_HBO_FiFoCtl, 2u * GFX + 1u)) {
            result = consume_completion(dma);
            if (result) return result;
            dma->active = 0;
            dma->configured = false;
            return TEE_SUCCESS;
        }
        result = TEE_Wait(1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
}

/* SPDX-License-Identifier: GPL-2.0 */
/* 在用户进程中比较两个命令编码器的输出。 */
#include "gfx-dma.h"
#include "api_avio_dhub.h"
#include <tee_api_syna.h>
extern int printf(const char *format, ...);

static uint32_t registers[8192 / 4];
static uint8_t memory[HDMI_DMA_POOL_MIN_BYTES];
static unsigned clean_calls;
static uint32_t consumed_words[2];
static uint32_t consumed_address;
static uint32_t busy_during_read;
static TEE_Result observe_word(void *opaque,uint32_t address,uint32_t *value)
{
    (void)opaque;
    if (address<consumed_address || address>consumed_address+4u || address%4u)
        return TEE_ERROR_BAD_PARAMETERS;
    *value=consumed_words[(address-consumed_address)/4u];
    if (busy_during_read) registers[(RA_dHubReg2D_dHub+RA_dHubReg_BUSY)/4u]=busy_during_read;
    return TEE_SUCCESS;
}
static uint32_t frame_offset, line_bytes, stride, rows;
TEE_Result TEE_CacheClean(char *buffer, size_t bytes)
{
    if ((void *)buffer != memory + frame_offset ||
        bytes != (uint64_t)stride*(rows-1u)+line_bytes) return TEE_ERROR_BAD_PARAMETERS;
    ++clean_calls;
    return TEE_SUCCESS;
}
void *TEE_TA_RegPhyToVirt(uint32_t address)
{
    (void)address;
    return NULL;
}
static int compare_channel(unsigned channel,uint32_t offset,
    uint32_t row_bytes,uint32_t row_stride,uint32_t height)
{
    frame_offset=offset;line_bytes=row_bytes;stride=row_stride;rows=height;
    clean_calls=0;
    struct hdmi_avio_registers views = {.vpp = registers};
    struct hdmi_gfx_dma dma = {.registers = &views, .configured = true,
        .channel = channel};
    struct hdmi_dma_pool pool = {.mapped = memory,
        .pa = HDMI_DMA_POOL_FIRST, .bytes = HDMI_DMA_POOL_MIN_BYTES,
        .registered = true};
    HDL_dhub2d sdk = {0};
    T64b expected[16] = {{0}};
    uint64_t submission = 0;
    unsigned sdk_reserved = 0;
    TEE_Result result = hdmi_gfx_dma_submit_region(&dma,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result || submission != 1 || dma.active != 1 || !pool.dma_armed ||
        clean_calls != 1) {
        printf("submit result=%#x id=%llu clean=%u\n", result,
                (unsigned long long)submission, clean_calls);
        return 1;
    }
    /* 比较完整命令序列，包括两个 START 写入。 */
    for (unsigned i=0;i<sizeof(registers)/sizeof(registers[0]);++i)
        if (registers[i]) return 1;
    unsigned writes = dhub2nd_channel_cfg(&sdk, channel,
        HDMI_DMA_POOL_FIRST+offset,row_bytes,row_bytes,1,row_stride,height,
        0, 0, 1, 1, expected);
    if (writes != HDMI_GFX_PROGRAM_WRITES) return 1;
    for (unsigned i = 0; i < writes; ++i) {
        uint32_t offset = expected[i][1];
        const uint32_t base = RA_dHubReg2D_ARR_2ND +
            channel * sizeof(SIE_dHubCmd2ND);
        uint32_t mask = UINT32_MAX;
        if (offset == base + RA_dHubCmd2ND_DESC_1D_ST)
            mask = MSK32dHubCmd2ND_DESC_1D_ST_step;
        else if (offset == base + RA_dHubCmd2ND_DESC_1D_SZ)
            mask = MSK32dHubCmd2ND_DESC_1D_SZ_size;
        else if (offset == base + RA_dHubCmd2ND_DESC_2D_ST)
            mask = MSK32dHubCmd2ND_DESC_2D_ST_step;
        else if (offset == base + RA_dHubCmd2ND_DESC_2D_SZ)
            mask = MSK32dHubCmd2ND_DESC_2D_SZ_size;
        if (expected[i][0] & ~mask) ++sdk_reserved;
        /* 比较有效寄存器字段，检查新编码器将保留位清零。 */
        if (offset >= sizeof(registers) || offset % 4 ||
            (dma.program[i].value & ~mask) ||
            dma.program[i].address != VPP_DHUB_BASE + offset ||
            dma.program[i].value != (expected[i][0] & mask)) {
            printf("encoding offset=%#x actual=%#x sdk=%#x\n",
                offset, dma.program[i].value,
                expected[i][0]);
            return 1;
        }
    }
    result = hdmi_gfx_dma_submit_region(&dma,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result != TEE_ERROR_BAD_STATE || submission || clean_calls != 1)
        return 1;
    /* 复用已发布载荷的缓存记录；重写载荷后再次清理。 */
    struct hdmi_gfx_dma repeated = {.registers=&views,.configured=true,.channel=channel};
    result=hdmi_gfx_dma_submit_region(&repeated,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result || clean_calls!=1) return 1;
    hdmi_payload_modified(&pool.payload_cache,offset,1);
    memory[offset]^=1;
    repeated=(struct hdmi_gfx_dma){.registers=&views,.configured=true,.channel=channel};
    result=hdmi_gfx_dma_submit_region(&repeated,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result || clean_calls!=2) return 1;
    /* DMA 忙时允许生成候选描述符，当前实例保持原状态。 */
    const uint32_t busy_offset=RA_dHubReg2D_dHub+RA_dHubReg_BUSY;
    registers[busy_offset/4]=1u<<channel;
    struct hdmi_gfx_dma candidate={.registers=&views,.configured=true,.channel=channel,
        .sequence=dma.sequence};
    result=hdmi_gfx_dma_submit_region(&candidate,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result!=TEE_ERROR_BUSY || submission || candidate.active || clean_calls!=2) return 1;
    result=hdmi_gfx_dma_prepare_repeat(&candidate,&pool,offset,row_stride,row_bytes,height,&submission);
    if (result || submission!=2 || candidate.active!=2 || dma.active!=1 || clean_calls!=2) return 1;
    const struct hdmi_mode_io io={.read32=observe_word};
    consumed_address=VPP_HBO_SRAM_BASE+
        (channel==avioDhubChMap_vpp128b_GFX_R1 ? 0x77a0u : 0x4140u)+48u;
    SIE_dHubCmd consumed={0};
    consumed.uMEM_addr=candidate.program[2].value+(height-1u)*row_stride;
    consumed.ie_HDR.uDESC_size=row_bytes;
    consumed.ie_HDR.uDESC_interrupt=1;
    consumed_words[0]=consumed.u32dHubCmd_MEM;
    consumed_words[1]=consumed.ie_HDR.u32dHubCmdHDR_DESC;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_ERROR_BUSY) return 1;
    registers[busy_offset/4]=0;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_SUCCESS) return 1;
    busy_during_read=1u<<channel;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_ERROR_BUSY) return 1;
    busy_during_read=0;registers[busy_offset/4]=0;
    consumed.ie_HDR.uDESC_size=row_bytes/256u;
    consumed.ie_HDR.uDESC_sizeMTU=1;
    consumed_words[1]=consumed.ie_HDR.u32dHubCmdHDR_DESC;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_SUCCESS) return 1;
    consumed_words[0]-=row_stride;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_ERROR_BUSY) return 1;
    consumed_words[0]+=row_stride;
    consumed_words[1]=0;
    if (hdmi_gfx_dma_source_matches(&candidate,&io)!=TEE_ERROR_BUSY) return 1;
    registers[busy_offset/4]=1u<<channel;
    for (unsigned i=0;i<HDMI_GFX_PROGRAM_WRITES;++i)
        if (candidate.program[i].address!=dma.program[i].address ||
            candidate.program[i].value!=dma.program[i].value) return 1;
    for (unsigned i=0;i<sizeof(registers)/sizeof(registers[0]);++i)
        if (registers[i]!=(i==busy_offset/4 ? 1u<<channel : 0)) return 1;
    registers[busy_offset/4]=0;
    printf("DMA encoding matches SDK: channel=%u writes=%u stride=%u line=%u rows=%u interrupt=1; active resubmit rejected; SDK reserved registers=%u\n",channel,writes,row_stride,row_bytes,height,sdk_reserved);
    return 0;
}

int main(void)
{
    if (compare_channel(avioDhubChMap_vpp128b_GFX_R1,
        HDMI_DMA_COMMAND_BYTES,3840,3840,720)) return 1;
    return compare_channel(avioDhubChMap_vpp128b_GFX_PIP_R0,
        0x10000,1024,1088,128);
}

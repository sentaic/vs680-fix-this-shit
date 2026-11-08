/* SPDX-License-Identifier: GPL-2.0 */
/* 全部显示DMA停止后，经AIO通道读取holder保留的最多256B。 */
#include "tee_internal_api.h"
#include "tee_ta_mmap.h"
#include "api_avio_dhub.h"
#include "rx-dma-credit.h"
#define AIO_PAYLOAD_CHANNEL avioDhubChMap_aio64b_GFX2_R
#include "aio-payload.h"
static uint32_t aio_payload_busy(HDL_dhub2d *hub)
{
    return dhub_channel_busy(&hub->dhub) | dhub_channel_pending(&hub->dhub) |
        dhub2d_channel_busy(hub);
}
static bool aio_payload_has_data(HDL_dhub2d *hub, unsigned channels)
{
    unsigned pointer;
    for (unsigned channel=0;channel<channels;++channel)
        if (hbo_queue_query(dhub_hbo(&hub->dhub),2*channel,1,&pointer) ||
            hbo_queue_query(dhub_hbo(&hub->dhub),2*channel+1,1,&pointer)) return true;
    return false;
}
static TEE_Result aio_payload_clear(HDL_dhub2d *hub, unsigned channel)
{
    unsigned command=2*channel, data=command+1;
    HDL_hbo *hbo=dhub_hbo(&hub->dhub);
    dhub2nd_channel_enable(hub,channel,0,NULL);
    dhub2nd_channel_clear(hub,channel);
    dhub2d_channel_enable(hub,channel,0,NULL);
    dhub2d_channel_clear(hub,channel);
    dhub_channel_enable(&hub->dhub,channel,0,NULL);
    dhub_channel_clear(&hub->dhub,channel);
    for (unsigned i=0;i<300;++i) {
        if (!(aio_payload_busy(hub)&(1u<<channel))) goto fifos;
        TEE_Wait(1);
    }
    return TEE_ERROR_BUSY;
fifos:
    hbo_queue_enable(hbo,command,0,NULL); hbo_queue_enable(hbo,data,0,NULL);
    hbo_queue_clear(hbo,command); hbo_queue_clear(hbo,data);
    for (unsigned i=0;i<300;++i) {
        unsigned pointer;
        if (!(hbo_queue_busy(hbo)&(3u<<command)) &&
            !hbo_queue_query(hbo,command,1,&pointer) && !hbo_queue_query(hbo,data,1,&pointer))
            return TEE_SUCCESS;
        TEE_Wait(1);
    }
    return TEE_ERROR_BUSY;
}
TEE_Result loop_aio_payload_check(uint32_t types, TEE_Param params[4], int capture_active)
{
    if (types!=TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_MEMREF_OUTPUT,
            TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE)) return TEE_ERROR_BAD_PARAMETERS;
    if (params[1].memref.size<sizeof(struct rx_dma_credit_result)) {
        params[1].memref.size=sizeof(struct rx_dma_credit_result);
        return TEE_ERROR_SHORT_BUFFER;
    }
    uint32_t pa=params[0].value.a, length=params[0].value.b & 0xffffu;
    bool bcm_sram=params[0].value.b & UINT32_C(0x10000);
    bool direct_bcm=params[0].value.b & UINT32_C(0x20000);
    unsigned channel=direct_bcm ? avioDhubChMap_aio64b_BCM_R : AIO_PAYLOAD_CHANNEL;
    if (!params[1].memref.buffer || (params[0].value.b & ~UINT32_C(0x3ffff)) || !length || length>256 || (length&7u) || (pa&255u) ||
        pa<UINT32_C(0x76400000) || pa>UINT32_C(0x76bfff00)) return TEE_ERROR_BAD_PARAMETERS;
    struct rx_dma_credit_result result={.abi=RX_DMA_CREDIT_ABI};
    HDL_dhub2d commands={0},pixels={0};
    dhub2d_hdl(AG_HBO_SRAM_BASE,AG_DHUB_BASE,&commands);
    dhub2d_hdl(VPP_HBO_SRAM_BASE,VPP_DHUB_BASE,&pixels);
    HDL_hbo *hbo=dhub_hbo(&commands.dhub);
    HDL_semaphore *sem=dhub_semaphore(&commands.dhub);
    unsigned profile_id=(bcm_sram || direct_bcm) ? avioDhubChMap_aio64b_BCM_R : AIO_PAYLOAD_CHANNEL;
    DHUB_channel_config *profile=&AG_config[profile_id];
    uint32_t bcm=MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_BCM_REG_BASE;
    volatile uint32_t *empty=TEE_TA_RegPhyToVirt(bcm+RA_AVIO_BCM_EMP_STS);
    volatile uint32_t *autopush=TEE_TA_RegPhyToVirt(bcm+RA_AVIO_BCM_AUTOPUSH);
    if (!empty || !autopush || profile->chanId!=(int)profile_id ||
        profile->chanCmdSize<8 || profile->chanDataSize<256) return TEE_ERROR_BAD_STATE;
    uint32_t queues=(MSK32AVIO_BCM_EMP_STS_Q18<<1)-1;
    if (capture_active || aio_payload_busy(&commands) || aio_payload_busy(&pixels) ||
        aio_payload_has_data(&commands,AG_NUM_OF_CHANNELS) ||
        aio_payload_has_data(&pixels,VPP_NUM_OF_CHANNELS) ||
        ((*empty&queues)!=queues) || (*autopush&queues)) return TEE_ERROR_BUSY;
    unsigned command=2*channel, data=command+1;
    dhub_channel_cfg(&commands.dhub,channel,profile->chanCmdBase,
        profile->chanDataBase,profile->chanCmdSize/8,profile->chanDataSize/8,
        dHubChannel_CFG_MTU_256byte,profile->chanQos,0,1,NULL,1);
    semaphore_cfg(sem,channel,1,NULL);
    semaphore_clr_full(sem,channel);
    result.armed=1;
    unsigned slot=0,initial_pointer=0;
    if (hbo_queue_query(hbo,command,0,&slot) || slot>=profile->chanCmdSize/8u ||
        hbo_queue_query(hbo,data,1,&initial_pointer) || initial_pointer>=profile->chanDataSize/8u) {
        result.result=TEE_ERROR_BAD_STATE; goto cleanup;
    }
    SIGN32 words[2];
    dhub_channel_generate_cmd(&commands.dhub,channel,pa,length,0,0,0,1,words);
    volatile uint32_t *entry=TEE_TA_RegPhyToVirt(hbo->mem+profile->chanCmdBase+slot*8);
    if (!entry || !TEE_TA_RegPhyToVirt(hbo->mem+profile->chanCmdBase+slot*8+4)) {
        result.result=TEE_ERROR_BAD_STATE; goto cleanup;
    }
    entry[0]=words[0]; entry[1]=words[1];
    __asm__ volatile("dsb sy" ::: "memory");
    hbo_queue_push(hbo,command,1);
    for (unsigned i=0;i<100;++i) {
        result.released.busy=aio_payload_busy(&commands)&(1u<<channel);
        result.released.completion=semaphore_chk_full(sem,-1)&(1u<<channel);
        if (!result.released.busy && result.released.completion) break;
        TEE_Wait(1);
    }
    if (result.released.busy || !result.released.completion) {
        result.result=TEE_ERROR_TIMEOUT; goto cleanup;
    }
    unsigned depth=profile->chanDataSize/8;
    for (unsigned i=0;i<length/4u;++i) {
        unsigned index=(initial_pointer+i/2)%depth;
        volatile uint32_t *view=TEE_TA_RegPhyToVirt(hbo->mem+profile->chanDataBase+index*8+(i%2)*4);
        if (!view) { result.result=TEE_ERROR_BAD_STATE; goto cleanup; }
        result.words[i]=*view;
    }
    result.words_valid=length/4u;
    result.observations=RX_DMA_CREDIT_COMPLETED|RX_DMA_CREDIT_FIFO_PRESENT;
cleanup:
    result.cleanup_result=aio_payload_clear(&commands,channel);
    if (!result.cleanup_result) {
        semaphore_cfg(sem,channel,1,NULL);
        semaphore_clr_full(sem,channel);
        dhub_channel_cfg(&commands.dhub,channel,profile->chanCmdBase,
            profile->chanDataBase,profile->chanCmdSize/8,profile->chanDataSize/8,
            profile->chanMtuSize,profile->chanQos,profile->chanSelfLoop,0,NULL,1);
        __asm__ volatile("dsb sy" ::: "memory");
        result.restored=!(aio_payload_busy(&commands)&(1u<<channel));
    }
    *(struct rx_dma_credit_result *)params[1].memref.buffer=result;
    params[1].memref.size=sizeof(result);
    return TEE_SUCCESS;
}

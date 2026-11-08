/* SPDX-License-Identifier: GPL-2.0 */
#include "avio-hardware.h"
#include <tee_api_syna.h>
#include "Galois_memmap.h"
#include "avio_memmap.h"
#include "avioDhub.h"
#include "global.h"

#define CHIP_BYTES UINT32_C(4096)
#define DHUB_BYTES UINT32_C(8192)
#define DHUB_CHANNELS 16u
#define RESET_POLLS 200u

static void device_barrier(void)
{
#ifdef HDMI_DEV_MMIO_TEST
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
#else
    __asm__ volatile("dsb sy" ::: "memory");
#endif
}

static TEE_Result map_once(volatile uint32_t **view, uint32_t address,
                           uint32_t bytes)
{
    void *mapping = NULL;
    TEE_Result result;
    if (*view) return TEE_SUCCESS;
    result = TEE_Map_Register(address, bytes, 0, 0, &mapping);
    if (result) return result;
    if (!mapping) return TEE_ERROR_BAD_STATE;
    *view = mapping;
    return TEE_SUCCESS;
}

TEE_Result hdmi_avio_map(struct hdmi_avio_registers *registers)
{
    TEE_Result result;
    if (!registers) return TEE_ERROR_BAD_PARAMETERS;
    result = map_once(&registers->chip, MEMMAP_CHIP_CTRL_REG_BASE, CHIP_BYTES);
    if (result) return result;
    result = map_once(&registers->vpp, MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_VPP128B_DHUB_REG_BASE + RA_vpp128bDhub_dHub0, DHUB_BYTES);
    if (result) return result;
    return map_once(&registers->aio, MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_AIO64B_DHUB_REG_BASE + RA_aio64bDhub_dHub0, DHUB_BYTES);
}

static uint32_t read_word(const volatile uint32_t *registers, uint32_t offset)
{
    return registers[offset / sizeof(uint32_t)];
}

static bool has_queued_data(const volatile uint32_t *registers, uint32_t channels)
{
    /* 按 semaphore_query 的消费者地址编码读取计数。 */
    const uint32_t fifo = RA_dHubReg2D_dHub + RA_dHubReg_HBO +
                          RA_HBO_FiFoCtl + RA_SemaHub_Query;
    for (unsigned id = 0; id < 2u * DHUB_CHANNELS; ++id) {
        if (!(channels & (1u << (id / 2u)))) continue;
        const uint32_t query = (id << LSb32SemaQueryMap_ADDR_ID) |
            (SemaQueryMap_ADDR_master_consumer << LSb32SemaQueryMap_ADDR_master);
        if (read_word(registers, fifo + query) & MSK32SemaQuery_RESP_CNT)
            return true;
    }
    return false;
}

TEE_Result hdmi_avio_snapshot(const struct hdmi_avio_registers *registers,
                             struct hdmi_avio_activity *activity)
{
    if (!registers || !activity) return TEE_ERROR_BAD_PARAMETERS;
    if (!registers->chip || !registers->vpp || !registers->aio)
        return TEE_ERROR_BAD_STATE;
    device_barrier();
    *activity = (struct hdmi_avio_activity){
        .reset_issued = read_word(registers->chip, RA_Gbl_avioResetStatus),
        .vpp_busy = read_word(registers->vpp, RA_dHubReg2D_dHub + RA_dHubReg_BUSY),
        .vpp_pending = read_word(registers->vpp, RA_dHubReg2D_dHub + RA_dHubReg_PENDING),
        .vpp_2d_busy = read_word(registers->vpp, RA_dHubReg2D_BUSY),
        .aio_busy = read_word(registers->aio, RA_dHubReg2D_dHub + RA_dHubReg_BUSY),
        .aio_pending = read_word(registers->aio, RA_dHubReg2D_dHub + RA_dHubReg_PENDING),
        .aio_2d_busy = read_word(registers->aio, RA_dHubReg2D_BUSY),
        .queued_domains = (has_queued_data(registers->vpp,UINT32_MAX) ? 1u : 0u) |
                          (has_queued_data(registers->aio,UINT32_MAX) ? 2u : 0u),
    };
    device_barrier();
    return TEE_SUCCESS;
}

TEE_Result hdmi_avio_tx_snapshot(const struct hdmi_avio_registers *registers,
                                struct hdmi_avio_activity *activity)
{
    const uint32_t vpp_mask=(1u<<5)|(1u<<7),aio_mask=1u<<8;
    if(!registers||!activity)return TEE_ERROR_BAD_PARAMETERS;
    if(!registers->vpp||!registers->aio)return TEE_ERROR_BAD_STATE;
    device_barrier();
    *activity=(struct hdmi_avio_activity){
        .vpp_busy=read_word(registers->vpp,RA_dHubReg2D_dHub+RA_dHubReg_BUSY)&vpp_mask,
        .vpp_pending=read_word(registers->vpp,RA_dHubReg2D_dHub+RA_dHubReg_PENDING)&vpp_mask,
        .vpp_2d_busy=read_word(registers->vpp,RA_dHubReg2D_BUSY)&vpp_mask,
        .aio_busy=read_word(registers->aio,RA_dHubReg2D_dHub+RA_dHubReg_BUSY)&aio_mask,
        .aio_pending=read_word(registers->aio,RA_dHubReg2D_dHub+RA_dHubReg_PENDING)&aio_mask,
        .aio_2d_busy=read_word(registers->aio,RA_dHubReg2D_BUSY)&aio_mask,
        .queued_domains=(has_queued_data(registers->vpp,vpp_mask)?1u:0u)|
            (has_queued_data(registers->aio,aio_mask)?2u:0u),
    };
    const uint32_t hbo=RA_dHubReg2D_dHub+RA_dHubReg_HBO+RA_HBO_BUSY;
    if(read_word(registers->vpp,hbo)&((3u<<10)|(3u<<14)))activity->queued_domains|=1u;
    if(read_word(registers->aio,hbo)&(3u<<16))activity->queued_domains|=2u;
    device_barrier();return TEE_SUCCESS;
}

bool hdmi_avio_idle(const struct hdmi_avio_activity *activity)
{
    return activity && !activity->vpp_busy && !activity->vpp_pending &&
        !activity->vpp_2d_busy && !activity->aio_busy && !activity->aio_pending &&
        !activity->aio_2d_busy && !activity->queued_domains;
}

TEE_Result hdmi_avio_reset(struct hdmi_avio_registers *registers,
                          struct hdmi_avio_activity *activity)
{
    TEE_Result result;
    if (!activity) return TEE_ERROR_BAD_PARAMETERS;
    *activity = (struct hdmi_avio_activity){0};
    result = hdmi_avio_map(registers);
    if (result) return result;
    /* 厂商 AVIO_Reset 的完整硬件复位顺序；状态位记录复位已发出。 */
    registers->chip[RA_Gbl_avioResetStatus / 4] = 0;
    device_barrier();
    registers->chip[RA_Gbl_avioReset / 4] = 1;
    device_barrier();
    result = TEE_Wait(1);
    registers->chip[RA_Gbl_avioReset / 4] = 0;
    device_barrier();
    if (result) return result;
    for (unsigned poll = 0; poll < RESET_POLLS; ++poll) {
        result = hdmi_avio_snapshot(registers, activity);
        if (result) return result;
        if ((activity->reset_issued & 1u) && hdmi_avio_idle(activity))
            return TEE_SUCCESS;
        result = TEE_Wait(1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
}

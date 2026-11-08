/* SPDX-License-Identifier: GPL-2.0 */
#ifdef TEE_Wait
#undef TEE_Wait
#endif
#include "tee_internal_api.h"
#include "api_avio_dhub.h"
#include "galois_io.h"
#include "tee_ta_mmap.h"
#include "avio.h"
#include "global.h"
#include "tee_api_syna.h"
#define MEMMAP_AVIO_BCM_REG_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_BCM_REG_BASE)
static HDL_dhub2d display_pixels, display_commands;
static int display_handles_ready;
static volatile uint32_t *display_chip_registers;
static void display_prepare(void)
{
    if (display_handles_ready) return;
    dhub2d_hdl(VPP_HBO_SRAM_BASE, VPP_DHUB_BASE, &display_pixels);
    dhub2d_hdl(AG_HBO_SRAM_BASE, AG_DHUB_BASE, &display_commands);
    display_handles_ready = 1;
}
static uint32_t channel_busy(HDL_dhub2d *handle)
{
    return dhub_channel_busy(&handle->dhub) | dhub_channel_pending(&handle->dhub) |
           dhub2d_channel_busy(handle);
}
static uint32_t queued(HDL_dhub2d *handle, unsigned channel)
{
    unsigned pointer;
    unsigned commands = hbo_queue_query(dhub_hbo(&handle->dhub), 2 * channel,
        SemaQueryMap_ADDR_master_consumer, &pointer);
    unsigned data = hbo_queue_query(dhub_hbo(&handle->dhub), 2 * channel + 1,
        SemaQueryMap_ADDR_master_consumer, &pointer);
    return (commands & 0xffff) | ((data & 0xffff) << 16);
}
static TEE_Result clear_channel(HDL_dhub2d *handle, unsigned channel)
{
    HDL_hbo *hbo = dhub_hbo(&handle->dhub);
    unsigned i;
    dhub2nd_channel_enable(handle, channel, 0, 0);
    dhub2nd_channel_clear(handle, channel);
    dhub2d_channel_enable(handle, channel, 0, 0);
    dhub2d_channel_clear(handle, channel);
    dhub_channel_enable(&handle->dhub, channel, 0, 0);
    dhub_channel_clear(&handle->dhub, channel);
    for (i = 0; i < 500 && (channel_busy(handle) & (1u << channel)); i++) TEE_Wait(1);
    if (i == 500) return TEE_ERROR_BUSY;
    hbo_queue_enable(hbo, 2 * channel, 0, 0);
    hbo_queue_enable(hbo, 2 * channel + 1, 0, 0);
    hbo_queue_clear(hbo, 2 * channel);
    hbo_queue_clear(hbo, 2 * channel + 1);
    for (i = 0; i < 500 && (hbo_queue_busy(hbo) & (3u << (2 * channel))); i++) TEE_Wait(1);
    if (i == 500 || queued(handle, channel)) return TEE_ERROR_BUSY;
    return TEE_SUCCESS;
}
TEE_Result loop_display_dma_command(uint32_t command, TEE_Param params[4])
{
    TEE_Result result = TEE_SUCCESS;
    display_prepare();
    if (command == 0x7f27) {
        uint32_t base, group = params[0].value.a, words[4];
        if (group>=28 && group<=30) {
            /* 通过内核区域 API 读取内存布局。 */
            TEE_MemRegion regions[16];
            uint32_t count=TEE_GetMemRegionCount(0,0), index=params[0].value.b;
            if (!count || count>16) return TEE_ERROR_BAD_STATE;
            if (index>=count) return TEE_ERROR_ITEM_NOT_FOUND;
            TEE_Result rc=TEE_GetMemRegionList(regions,16,0,0);
            if (rc) return rc;
            if (group==28) {
                words[0]=regions[index].base;words[1]=regions[index].size;
                words[2]=regions[index].attr;words[3]=regions[index].userdata[0];
            } else TEE_MemMove(words,regions[index].name+(group-29)*16,16);
            params[1].value.a=words[0];params[1].value.b=words[1];
            params[2].value.a=words[2];params[2].value.b=words[3];
            return TEE_SUCCESS;
        }
        const uint32_t offsets[3][4] = {
            {RA_AVIO_Q0, RA_AVIO_Q1, RA_AVIO_Q3, RA_AVIO_BCM_AUTOPUSH_CNT},
            {RA_AVIO_BCM_Q0, RA_AVIO_BCM_Q1, RA_AVIO_BCM_Q3, RA_AVIO_BCM_AUTOPUSH},
            {RA_AVIO_BCM_ERROR, RA_AVIO_BCM_LOG_ADDR, RA_AVIO_BCM_ERROR_DATA, RA_AVIO_BCM_FULL_STS_STICKY}
        };
        if (group >= 22 && group <= 25) {
            /* 读取 CPCB 流控、PIP 格式及尺寸。 */
            static const uint32_t address[4][4]={
                {0xf7492a38,0xf7492a4c,0xf7492a50,0xf7492a58},
                {0xf7490400,0xf7490404,0xf7490430,0xf7490000},
                {0xf7490414,0xf7490424,0xf749051c,0xf7490520},
                {0xf749044c,0xf7490454,0xf7490458,0xf7490468}
            };
            uint32_t value[4];
            for (unsigned i=0;i<4;++i) {
                volatile uint32_t *view=TEE_TA_RegPhyToVirt(address[group-22][i]);
                if (!view) return TEE_ERROR_BAD_STATE;
                value[i]=*view;
            }
            params[1].value.a=value[0];params[1].value.b=value[1];
            params[2].value.a=value[2];params[2].value.b=value[3];
            return TEE_SUCCESS;
        }
        if (group == 26) {
            unsigned command_pointer,data_pointer;
            HDL_hbo *hbo=dhub_hbo(&display_pixels.dhub);
            unsigned channel=avioDhubChMap_vpp128b_GFX_PIP_R0;
            params[1].value.a=hbo_queue_query(hbo,2*channel,
                SemaQueryMap_ADDR_master_consumer,&command_pointer);
            params[1].value.b=command_pointer;
            params[2].value.a=hbo_queue_query(hbo,2*channel+1,
                SemaQueryMap_ADDR_master_consumer,&data_pointer);
            params[2].value.b=data_pointer;
            return TEE_SUCCESS;
        }
        if (group == 21) {
            /* BCM_R实际命令FIFO的首个HDR/MEM，保持消费者位置不变。 */
            volatile uint32_t *view=TEE_TA_RegPhyToVirt(AG_HBO_SRAM_BASE + 0x4a00);
            if (!view || !TEE_TA_RegPhyToVirt(AG_HBO_SRAM_BASE + 0x4a0c))
                return TEE_ERROR_BAD_STATE;
            params[1].value.a=view[0]; params[1].value.b=view[1];
            params[2].value.a=view[2]; params[2].value.b=view[3];
            return TEE_SUCCESS;
        }
        if (group == 20) {
            /* 已消费的AIO命令表仍保留在SRAM；固定读取最初两个64位字。 */
            volatile uint32_t *view=TEE_TA_RegPhyToVirt(AG_HBO_SRAM_BASE + 0x4a80);
            if (!view || !TEE_TA_RegPhyToVirt(AG_HBO_SRAM_BASE + 0x4a8c))
                return TEE_ERROR_BAD_STATE;
            params[1].value.a=view[0]; params[1].value.b=view[1];
            params[2].value.a=view[2]; params[2].value.b=view[3];
            return TEE_SUCCESS;
        }
        if (group == 19) {
            /* Dolphin SOC protOR：CPU、AVIO及BCM主设备的只读访问属性。 */
            void *mapping=NULL;
            TEE_Result rc=TEE_Map_Register(UINT32_C(0xf7ca1000),4096,0,0,&mapping);
            if (rc) return rc;
            if (!mapping) return TEE_ERROR_BAD_STATE;
            volatile uint32_t *view=mapping;
            params[1].value.a=view[0x200/4];
            params[1].value.b=view[0x204/4];
            params[2].value.a=view[0x25c/4];
            params[2].value.b=view[0x260/4];
            return TEE_SUCCESS;
        }
        if (group == 18) {
            /* 官方DTGUF_Handler的状态/控制及Q0实际执行尾部；安全态定点读取。 */
            const uint32_t address[] = {0xf7480bec,0xf7480be0,0xf7444060,0xf7490008};
            uint32_t value[4];
            for (unsigned i=0; i<4; ++i) {
                volatile uint32_t *view=TEE_TA_RegPhyToVirt(address[i]);
                if (!view) return TEE_ERROR_BAD_STATE;
                value[i]=*view;
            }
            params[1].value.a=value[0]; params[1].value.b=value[1];
            params[2].value.a=value[2]; params[2].value.b=value[3];
            return TEE_SUCCESS;
        }
        if (group == 16) {
            const uint32_t address[] = {0xf7491000,0xf7491018,0xf7491024,0xf7491800};
            uint32_t value[4];
            for (unsigned i=0; i<4; ++i) {
                volatile uint32_t *view=TEE_TA_RegPhyToVirt(address[i]);
                if (!view) return TEE_ERROR_BAD_STATE;
                value[i]=*view;
            }
            params[1].value.a=value[0]; params[1].value.b=value[1];
            params[2].value.a=value[2]; params[2].value.b=value[3];
            return TEE_SUCCESS;
        }
        if (group == 17) {
            unsigned command_pointer, data_pointer;
            HDL_hbo *hbo=dhub_hbo(&display_pixels.dhub);
            unsigned channel=avioDhubChMap_vpp128b_GFX_R1;
            params[1].value.a=hbo_queue_query(hbo,2*channel,
                SemaQueryMap_ADDR_master_consumer,&command_pointer);
            params[1].value.b=command_pointer;
            params[2].value.a=hbo_queue_query(hbo,2*channel+1,
                SemaQueryMap_ADDR_master_consumer,&data_pointer);
            params[2].value.b=data_pointer;
            return TEE_SUCCESS;
        }
        if (group >= 13 && group <= 15) {
            /* 读取时钟门、TG 控制和 PHY 状态。 */
            static const uint32_t address[3][4] = {
                {0xf749c0ec,0xf749c0f0,0xf749c10c,0xf74960ec},
                {0xf7492a3c,0xf7491008,0xf7480a94,0xf7480c8c},
                {0xf744c000,0xf744c010,0xf7450004,0xf7444000}
            };
            uint32_t value[4];
            for (unsigned i=0; i<4; ++i) {
                volatile uint32_t *view=TEE_TA_RegPhyToVirt(address[group-13][i]);
                if (!view) return TEE_ERROR_BAD_STATE;
                value[i]=*view;
            }
            params[1].value.a=value[0]; params[1].value.b=value[1];
            params[2].value.a=value[2]; params[2].value.b=value[3];
            return TEE_SUCCESS;
        }
        if (group == 12) {
            const uint32_t address[] = {UINT32_C(0xf749c004), UINT32_C(0xf749c020),
                UINT32_C(0xf749c138), UINT32_C(0xf749c000)};
            uint32_t value[4];
            for (unsigned i = 0; i < 4; ++i) {
                volatile uint32_t *view = TEE_TA_RegPhyToVirt(address[i]);
                if (!view) return TEE_ERROR_BAD_STATE;
                value[i] = *view;
            }
            params[1].value.a = value[0]; params[1].value.b = value[1];
            params[2].value.a = value[2]; params[2].value.b = value[3];
            return TEE_SUCCESS;
        }
        if (group == 11) {
            uint32_t address = params[0].value.b, id = 0, refs = 0;
            paddr_t physical = 0;
            if (address < UINT32_C(0x76400000) || address >= UINT32_C(0x76e00000) ||
                address % 4096u) return TEE_ERROR_BAD_PARAMETERS;
            TEE_Result query = TEE_GetMemid_By_Phys(&id, address, 4096);
            params[1].value.a = query;
            if (!query) {
                TEE_Result rc = TEE_Mem_GetRefCnt(id, &refs);
                if (rc) return rc;
                rc = TEE_GetPhys_By_Memid(id, &physical);
                if (rc) return rc;
            }
            params[1].value.b = refs;
            params[2].value.a = id;
            params[2].value.b = physical;
            return TEE_SUCCESS;
        }
        if (group == 9 || group == 10) {
            /* Development recovery: caller has frozen every AVIO user and retains buffers. */
            if (params[0].value.b != UINT32_C(0x52455354))
                return TEE_ERROR_BAD_PARAMETERS;
            void *mapping = 0;
            uint32_t page = MEMMAP_CHIP_CTRL_REG_BASE & ~UINT32_C(4095);
            if (!display_chip_registers) {
                TEE_Result rc = TEE_Map_Register(page, 4096, 0, 0, &mapping);
                if (rc) return rc;
                display_chip_registers = mapping;
            }
            unsigned reset = (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avioReset - page) / 4;
            unsigned status = (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avioResetStatus - page) / 4;
            /* Group 10 verifies a prior reset while the owners remain frozen. */
            if (group == 9) {
            /* Clear the issued-reset history, then use the vendor's 1 -> 0 sequence. */
            display_chip_registers[status] = 0;
            __asm__ volatile("dsb sy" ::: "memory");
            display_chip_registers[reset] = 1;
            __asm__ volatile("dsb sy" ::: "memory");
            TEE_Wait(1);
            display_chip_registers[reset] = 0;
            __asm__ volatile("dsb sy" ::: "memory");
            }
            for (unsigned tries = 0; tries < 200; tries++) {
                uint32_t outstanding = 0;
                for (unsigned i = 0; i < VPP_NUM_OF_CHANNELS; i++)
                    if (queued(&display_pixels, i)) outstanding |= 1;
                for (unsigned i = 0; i < AG_NUM_OF_CHANNELS; i++)
                    if (queued(&display_commands, i)) outstanding |= 2;
                params[1].value.a = display_chip_registers[status];
                params[1].value.b = channel_busy(&display_pixels);
                params[2].value.a = channel_busy(&display_commands);
                params[2].value.b = outstanding;
                if ((params[1].value.a & 1) && !params[1].value.b &&
                    !params[2].value.a && !outstanding) return TEE_SUCCESS;
                TEE_Wait(1);
            }
            return TEE_ERROR_BUSY;
        }
        if (group == 8) {
            void *mapping = 0;
            /* ResetStatus 记录复位请求的发出状态。 */
            uint32_t address = MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avioResetStatus;
            if (!display_chip_registers) {
                TEE_Result rc = TEE_Map_Register(address & ~UINT32_C(4095),
                    4096, 0, 0, &mapping);
                if (rc) return rc;
                display_chip_registers = mapping;
            }
            params[1].value.a = display_chip_registers[(address & 4095) / sizeof(uint32_t)];
            params[1].value.b = MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avioReset;
            params[2].value.a = channel_busy(&display_pixels);
            params[2].value.b = channel_busy(&display_commands);
            /* Register mappings are permanent for this TA instance. */
            return TEE_SUCCESS;
        }
        if (group >= 4 && group <= 7) {
            unsigned channel = avioDhubChMap_aio64b_BCM_R;
            unsigned commands = group >= 6;
            unsigned id = 2 * channel + !commands, pointer, count;
            HDL_hbo *hbo = dhub_hbo(&display_commands.dhub);
            DHUB_channel_config *config = &AG_config[channel];
            if (config->chanId != (int)channel || config->chanDataSize < 16)
                return TEE_ERROR_BAD_STATE;
            unsigned fifo_base = commands ? config->chanCmdBase : config->chanDataBase;
            unsigned size = commands ? (unsigned)config->chanCmdSize : (unsigned)config->chanDataSize;
            unsigned depth = size / 8;
            count = hbo_queue_query(hbo, id, SemaQueryMap_ADDR_master_consumer, &pointer);
            if (group == 4 || group == 6) {
                params[1].value.a = fifo_base;
                params[1].value.b = size;
                params[2].value.a = pointer;
                params[2].value.b = count;
                return TEE_SUCCESS;
            }
            /* AIO FIFO 每项为 64 位，命令项由 HDR 和 ADDR 组成。 */
            if (pointer >= depth) return TEE_ERROR_BAD_STATE;
            if (count < 2 || pointer + 1 >= depth)
                return TEE_ERROR_BUSY;
            uint32_t address = hbo->mem + fifo_base + pointer * 8;
            volatile uint32_t *view = TEE_TA_RegPhyToVirt(address);
            if (!view || !TEE_TA_RegPhyToVirt(address + 12)) return TEE_ERROR_BAD_STATE;
            params[1].value.a = view[0]; params[1].value.b = view[1];
            params[2].value.a = view[2]; params[2].value.b = view[3];
            return TEE_SUCCESS;
        }
        if (group == 2) {
            /* Observe completion credits without consuming or acknowledging them. */
            HDL_semaphore *commands = dhub_semaphore(&display_commands.dhub);
            HDL_semaphore *pixels = dhub_semaphore(&display_pixels.dhub);
            params[1].value.a = semaphore_chk_full(commands, -1);
            params[1].value.b = semaphore_query(commands,
                avioDhubChMap_aio64b_BCM_R, SemaQueryMap_ADDR_master_consumer, 0);
            params[2].value.a = semaphore_chk_full(pixels, -1);
            params[2].value.b = semaphore_query(pixels,
                avioDhubChMap_vpp128b_GFX_R1, SemaQueryMap_ADDR_master_consumer, 0);
            return TEE_SUCCESS;
        }
        if (group > 3) return TEE_ERROR_BAD_PARAMETERS;
        if (group == 3) group = 2;
        base = MEMMAP_AVIO_BCM_REG_BASE;
        for (unsigned i = 0; i < 4; i++) {
            volatile uint32_t *view = TEE_TA_RegPhyToVirt(base + offsets[group][i]);
            if (!view) return TEE_ERROR_BAD_STATE;
            words[i] = *view;
        }
        params[1].value.a = words[0]; params[1].value.b = words[1];
        params[2].value.a = words[2]; params[2].value.b = words[3];
        return TEE_SUCCESS;
    }
    if (command == 0x7f1b) {
        /* Pause the old owner before this command; retain its buffers on any failure. */
        const unsigned queues[] = {BCM_SCHED_Q0, BCM_SCHED_Q1, BCM_SCHED_Q3};
        unsigned i, attempt;
        BCM_SCHED_Open();
        for (i = 0; i < 3; i++) BCM_SCHED_AutoPushCmd(queues[i], 0);
        result = clear_channel(&display_commands, avioDhubChMap_aio64b_BCM_R);
        if (!result) {
            /* FLUSH 清理当前显示实例使用的队列。 */
            volatile uint32_t *flush = (volatile uint32_t *)TEE_TA_RegPhyToVirt(
                MEMMAP_AVIO_BCM_REG_BASE + RA_AVIO_BCM_FLUSH);
            uint32_t previous = *flush;
            *flush = previous | 0xbu;
            for (attempt = 0; attempt < 500; attempt++) {
                unsigned all_empty = 1;
                for (i = 0; i < 3; i++) {
                    unsigned empty;
                    BCM_SCHED_GetEmptySts(queues[i], &empty);
                    all_empty &= empty;
                }
                if (all_empty) break;
                TEE_Wait(1);
            }
            *flush = previous;
            if (attempt == 500) result = TEE_ERROR_BUSY;
        }
        BCM_SCHED_Close();
        if (!result) result = clear_channel(&display_pixels, avioDhubChMap_vpp128b_GFX_R1);
    }
    if (command == 0x7f1c) {
        uint32_t base = MEMMAP_AVIO_BCM_REG_BASE;
        /* 暂停显示进程并固定命令表和源槽后，修改 Q0 自动重复位；结束时由原显示实例执行 STOP。 */
        if (params[0].value.a || params[0].value.b) {
            uint32_t request=params[0].value.b;
            volatile SIE_dHubCmd *source=TEE_TA_RegPhyToVirt(AG_HBO_SRAM_BASE+0x4a00);
            volatile uint32_t *control=TEE_TA_RegPhyToVirt(base+RA_AVIO_BCM_AUTOPUSH);
            volatile uint32_t *trigger=TEE_TA_RegPhyToVirt(base+RA_AVIO_BCM_Q0);
            if (params[0].value.a!=0x76401000 ||
                (request!=0xa5700000 && request!=0xa5700001))
                return TEE_ERROR_BAD_PARAMETERS;
            if (!source || !control || !trigger ||
                source->uMEM_addr!=params[0].value.a ||
                !source->ie_HDR.uDESC_size || source->ie_HDR.uDESC_size>384 ||
                (*trigger&0x3f)!=BCM_SCHED_TRIG_CPCB0_VBI)
                return TEE_ERROR_BAD_STATE;
            *control=(*control&~1u)|(request&1u);
            __asm__ volatile("dsb sy" ::: "memory");
            if ((*control&1u)!=(request&1u)) return TEE_ERROR_BUSY;
        }
        params[1].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_AVIO_BCM_EMP_STS);
        params[1].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_AVIO_BCM_AUTOPUSH);
        params[2].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_AVIO_BCM_FULL_STS);
        params[2].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_AVIO_BCM_FLUSH);
        return TEE_SUCCESS;
    }
    __asm__ volatile("dsb sy" ::: "memory");
    params[1].value.a = channel_busy(&display_pixels);
    params[1].value.b = channel_busy(&display_commands);
    params[2].value.a = queued(&display_pixels, avioDhubChMap_vpp128b_GFX_R1);
    params[2].value.b = queued(&display_commands, avioDhubChMap_aio64b_BCM_R);
    if (command == 0x7f1b && result == TEE_SUCCESS) {
        volatile uint32_t *autopush = TEE_TA_RegPhyToVirt(
            MEMMAP_AVIO_BCM_REG_BASE + RA_AVIO_BCM_AUTOPUSH);
        unsigned empty;
        if (!autopush) return TEE_ERROR_BAD_STATE;
        /* 清理后检查自动重复位、忙状态和队列计数。 */
        if ((*autopush & 0xbu) ||
            (params[1].value.a & (1u << avioDhubChMap_vpp128b_GFX_R1)) ||
            (params[1].value.b & (1u << avioDhubChMap_aio64b_BCM_R)) ||
            params[2].value.a || params[2].value.b)
            return TEE_ERROR_BUSY;
        const unsigned queues[] = {BCM_SCHED_Q0, BCM_SCHED_Q1, BCM_SCHED_Q3};
        for (unsigned i = 0; i < 3; i++) {
            BCM_SCHED_GetEmptySts(queues[i], &empty);
            if (!empty) return TEE_ERROR_BUSY;
        }
        /* 此路径保留命令池及 TX 映射，并返回 NOT_SUPPORTED。 */
        return TEE_ERROR_NOT_SUPPORTED;
    }
    return result;
}

/* Read queued pixels without changing the FIFO consumer pointer. */
static TEE_Result loop_display_fifo_command(TEE_Param params[4])
{
    unsigned channel = avioDhubChMap_vpp128b_GFX_R1;
    unsigned id = 2 * channel + 1, pointer, count;
    unsigned offset = params[0].value.a;
    HDL_hbo *hbo;
    DHUB_channel_config *config = &VPP_config[channel];
    /* Dolphin VPP initialization uses 128-bit FIFO entries. */
    unsigned depth = config->chanDataSize / 16;
    uint32_t address, words[4];
    volatile uint32_t *view;
    if (offset > 16 || params[0].value.b > 1 || config->chanId != (int)channel || depth < 2)
        return TEE_ERROR_BAD_PARAMETERS;
    display_prepare();
    hbo = dhub_hbo(&display_pixels.dhub);
    /* FiFo.CFG 为只写寄存器，SRAM 基址取初始化配置。 */
    if (params[0].value.b == 1) {
        count = hbo_queue_query(hbo, id, SemaQueryMap_ADDR_master_consumer, &pointer);
        params[1].value.a = config->chanDataBase;
        params[1].value.b = (unsigned)config->chanDataSize;
        params[2].value.a = pointer;
        params[2].value.b = count;
        return TEE_SUCCESS;
    }
    /* Use the same source configuration as the active FASTLOGO initializer. */
    count = hbo_queue_query(hbo, id, SemaQueryMap_ADDR_master_consumer, &pointer);
    if (pointer >= depth || count < offset + 2) return TEE_ERROR_BUSY;
    pointer = (pointer + offset) % depth;
    address = hbo->mem + config->chanDataBase + pointer * 16;
    view = TEE_TA_RegPhyToVirt(address);
    if (!view || !TEE_TA_RegPhyToVirt(address + 12)) return TEE_ERROR_BAD_STATE;
    for (unsigned i = 0; i < 4; i++) words[i] = view[i];
    params[1].value.a = words[0]; params[1].value.b = words[1];
    params[2].value.a = words[2]; params[2].value.b = words[3];
    return TEE_SUCCESS;
}

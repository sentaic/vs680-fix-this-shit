/* SPDX-License-Identifier: GPL-2.0 */
/* Finite GFX1 read-DMA credit and 256-byte SRAM payload check. */
#include "tee_internal_api.h"
#include "tee_ta_mmap.h"
#include "api_avio_dhub.h"
#include "vpp.h"
#include "avio.h"
#include "rx-dma-credit.h"
#define CREDIT_READ_CHANNEL avioDhubChMap_vpp128b_GFX_R1
#define CREDIT_READY_CHANNEL avioDhubChMap_vpp128b_1DSCL_W0
#define CREDIT_READ_MASK (1u << CREDIT_READ_CHANNEL)
static uint32_t credit_dma_busy(HDL_dhub2d *hub)
{
    return dhub_channel_busy(&hub->dhub) | dhub_channel_pending(&hub->dhub) |
        dhub2d_channel_busy(hub);
}
static uint32_t credit_hub_queued(HDL_dhub2d *hub, unsigned channels)
{
    HDL_hbo *hbo = dhub_hbo(&hub->dhub);
    uint32_t found = 0;
    for (unsigned channel = 0; channel < channels; channel++) {
        unsigned pointer;
        if (hbo_queue_query(hbo, 2 * channel, 1, &pointer) ||
            hbo_queue_query(hbo, 2 * channel + 1, 1, &pointer))
            found |= 1u << channel;
    }
    return found;
}
static void credit_dma_snapshot_channel(HDL_dhub2d *hub, struct rx_dma_credit_state *state,
                                        unsigned channel)
{
    HDL_semaphore *sem = dhub_semaphore(&hub->dhub);
    HDL_hbo *hbo = dhub_hbo(&hub->dhub);
    unsigned pointer;
    __asm__ volatile("dsb sy" ::: "memory");
    state->busy = dhub_channel_busy(&hub->dhub) & (1u << channel);
    state->pending = dhub_channel_pending(&hub->dhub) & (1u << channel);
    state->two_d = dhub2d_channel_busy(hub) & (1u << channel);
    state->completion = semaphore_chk_full(sem, -1) & (1u << channel);
    state->commands = hbo_queue_query(hbo, 2 * channel, 1, &pointer);
    state->data = hbo_queue_query(hbo, 2 * channel + 1, 1, &pointer);
    state->data_pointer = pointer;
    uint32_t offset = CREDIT_READY_CHANNEL << LSb32SemaQueryMap_ADDR_ID;
    state->ready_producer = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(
        sem->ra + RA_SemaHub_Query + offset);
    state->ready_consumer = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(
        sem->ra + RA_SemaHub_Query + offset + (1u << LSb32SemaQueryMap_ADDR_master));
}
static void credit_dma_snapshot(HDL_dhub2d *hub, struct rx_dma_credit_state *state)
{
    credit_dma_snapshot_channel(hub, state, CREDIT_READ_CHANNEL);
}
static void credit_cell_normal(HDL_semaphore *sem, unsigned channel)
{
    volatile uint32_t *mask = TEE_TA_RegPhyToVirt(sem->ra + RA_SemaHub_ARR +
        channel * sizeof(SIE_Semaphore) + RA_Semaphore_mask);
    semaphore_cfg(sem, channel, 1, 0);
    *mask = 0;
    for (unsigned cpu = 0; cpu < 3; cpu++)
        semaphore_intr_enable(sem, channel, 0, 0, 0, 0, cpu);
    semaphore_clr_full(sem, channel);
    __asm__ volatile("dsb sy" ::: "memory");
}
static TEE_Result credit_clear_channel(HDL_dhub2d *hub, unsigned channel)
{
    HDL_hbo *hbo = dhub_hbo(&hub->dhub);
    unsigned command = 2 * channel, data = command + 1;
    dhub2nd_channel_enable(hub, channel, 0, 0);
    dhub2nd_channel_clear(hub, channel);
    dhub2d_channel_enable(hub, channel, 0, 0);
    dhub2d_channel_clear(hub, channel);
    dhub_channel_enable(&hub->dhub, channel, 0, 0);
    dhub_channel_clear(&hub->dhub, channel);
    unsigned attempt;
    for (attempt = 0; attempt < 300; attempt++) {
        if (!(credit_dma_busy(hub) & (1u << channel))) break;
        TEE_Wait(1);
    }
    if (attempt == 300) return TEE_ERROR_BUSY;
    hbo_queue_enable(hbo, command, 0, 0);
    hbo_queue_enable(hbo, data, 0, 0);
    hbo_queue_clear(hbo, command);
    hbo_queue_clear(hbo, data);
    for (attempt = 0; attempt < 300; attempt++) {
        if (!(hbo_queue_busy(hbo) & (3u << command))) break;
        TEE_Wait(1);
    }
    unsigned pointer;
    if (attempt == 300 || hbo_queue_query(hbo, command, 1, &pointer) ||
        hbo_queue_query(hbo, data, 1, &pointer)) return TEE_ERROR_BUSY;
    return TEE_SUCCESS;
}
static TEE_Result credit_dma_check(uint32_t types, TEE_Param params[4],
                               int capture_active, int hardware_write)
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_OUTPUT,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE)) return TEE_ERROR_BAD_PARAMETERS;
    if (params[1].memref.size < sizeof(struct rx_dma_credit_result)) {
        params[1].memref.size = sizeof(struct rx_dma_credit_result);
        return TEE_ERROR_SHORT_BUFFER;
    }
    if (!params[1].memref.buffer) return TEE_ERROR_BAD_PARAMETERS;
    uint32_t address = params[0].value.a, length = params[0].value.b;
    uint64_t end = (uint64_t)address + length;
    bool capture_pool = address >= UINT32_C(0x75400000) && end <= UINT32_C(0x76400000);
    /* 仅有限读取支持当前holder保留的8MiB显示池；写入诊断仍限原采集池。 */
    bool display_pool = !hardware_write && address >= UINT32_C(0x76400000) &&
        end <= UINT32_C(0x76c00000);
    if (length < 256 || (address & 255u) || (!capture_pool && !display_pool))
        return TEE_ERROR_BAD_PARAMETERS;
    struct rx_dma_credit_result result = { .abi = RX_DMA_CREDIT_ABI };
    HDL_dhub2d pixels = {0}, commands = {0};
    dhub2d_hdl(VPP_HBO_SRAM_BASE, VPP_DHUB_BASE, &pixels);
    dhub2d_hdl(AG_HBO_SRAM_BASE, AG_DHUB_BASE, &commands);
    HDL_hbo *hbo = dhub_hbo(&pixels.dhub);
    HDL_semaphore *sem = dhub_semaphore(&pixels.dhub);
    DHUB_channel_config *config = &VPP_config[CREDIT_READ_CHANNEL];
    DHUB_channel_config *writer = &VPP_config[CREDIT_READY_CHANNEL];
    uint32_t bcm = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_BCM_REG_BASE;
    volatile uint32_t *empty = TEE_TA_RegPhyToVirt(bcm + RA_AVIO_BCM_EMP_STS);
    volatile uint32_t *autopush = TEE_TA_RegPhyToVirt(bcm + RA_AVIO_BCM_AUTOPUSH);
    volatile uint32_t *client = TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_VPP_REG_BASE + RA_Vpp_GFX1 + RA_GFX1_gfxScl);
    const uint32_t bcm_queues = (MSK32AVIO_BCM_EMP_STS_Q18 << 1) - 1;
    if (!empty || !autopush || !client || config->chanId != CREDIT_READ_CHANNEL ||
        config->chanCmdSize < 16 || config->chanDataSize < 256 ||
        (hardware_write && (writer->chanId != CREDIT_READY_CHANNEL ||
          writer->chanCmdSize < 16 || writer->chanDataSize < 256))) {
        result.result = TEE_ERROR_BAD_STATE;
        goto report;
    }
    /* 检查 DMA 通道和 BCM 队列是否空闲。 */
    result.vpp_initial = credit_dma_busy(&pixels);
    result.aio_initial = credit_dma_busy(&commands);
    result.vpp_queued = credit_hub_queued(&pixels, VPP_NUM_OF_CHANNELS);
    result.aio_queued = credit_hub_queued(&commands, AG_NUM_OF_CHANNELS);
    result.bcm_empty = *empty;
    result.bcm_autopush = *autopush;
    if (capture_active || result.vpp_initial || result.aio_initial ||
        result.vpp_queued || result.aio_queued ||
        (result.bcm_empty & bcm_queues) != bcm_queues ||
        (result.bcm_autopush & bcm_queues)) {
        result.result = TEE_ERROR_BUSY;
        goto report;
    }
    /* 显示实例停止后，将 GFX1 客户端和缩放器保持在清零状态。 */
    *client = MSK32GFX1_gfxScl_client_clear | MSK32GFX1_gfxScl_scl_clear;
    __asm__ volatile("dsb sy" ::: "memory");
    result.armed = 1;
    /* CFG and ARQOS are W- registers. Keep QoS unchanged during the check. */
    T32dHubChannel_CFG normal = { .u32 = 0 };
    normal.uCFG_MTU = config->chanMtuSize;
    normal.uCFG_QoS = config->chanQos;
    normal.uCFG_selfLoop = config->chanSelfLoop;
    result.normal_cfg = normal.u32;
    credit_cell_normal(sem, CREDIT_READY_CHANNEL);
    credit_cell_normal(sem, CREDIT_READ_CHANNEL);
    dhub2nd_channel_enable(&pixels, CREDIT_READ_CHANNEL, 0, 0);
    dhub2d_channel_enable(&pixels, CREDIT_READ_CHANNEL, 0, 0);
    dhub_channel_cfg(&pixels.dhub, CREDIT_READ_CHANNEL, config->chanCmdBase,
        config->chanDataBase, config->chanCmdSize / 16, config->chanDataSize / 16,
        dHubChannel_CFG_MTU_256byte, config->chanQos, 0, 1, 0, 1);
    if (hardware_write) {
        dhub2nd_channel_enable(&pixels, CREDIT_READY_CHANNEL, 0, 0);
        dhub2d_channel_enable(&pixels, CREDIT_READY_CHANNEL, 0, 0);
        dhub_channel_cfg(&pixels.dhub, CREDIT_READY_CHANNEL, writer->chanCmdBase,
            writer->chanDataBase, writer->chanCmdSize / 16, writer->chanDataSize / 16,
            dHubChannel_CFG_MTU_256byte, writer->chanQos, 0, 1, 0, 1);
        unsigned writer_slot;
        if (hbo_queue_query(hbo, 2 * CREDIT_READY_CHANNEL, 0, &writer_slot) ||
            writer_slot >= (unsigned)writer->chanCmdSize / 16) {
            result.result = TEE_ERROR_BUSY;
            goto cleanup;
        }
        SIGN32 writer_words[2];
        /* 用通道 9 的完成信号量控制读通道启动。 */
        dhub_channel_generate_cmd(&pixels.dhub, CREDIT_READY_CHANNEL, address, 256,
            0, 0, 0, 1, writer_words);
        volatile uint32_t *writer_command = TEE_TA_RegPhyToVirt(hbo->mem +
            writer->chanCmdBase + writer_slot * 16);
        writer_command[0] = writer_words[0]; writer_command[1] = writer_words[1];
        writer_command[2] = 0; writer_command[3] = 0;
        __asm__ volatile("dsb sy" ::: "memory");
        hbo_queue_push(hbo, 2 * CREDIT_READY_CHANNEL, 1);
    }
    credit_dma_snapshot(&pixels, &result.initial);
    unsigned slot;
    SIGN32 command_depth = hbo->fifoCtl.depth[2 * CREDIT_READ_CHANNEL];
    if (command_depth <= 0 ||
        hbo_queue_query(hbo, 2 * CREDIT_READ_CHANNEL, 0, &slot) >=
            (unsigned)command_depth || slot >= (unsigned)command_depth) {
        result.result = TEE_ERROR_BUSY;
        goto cleanup;
    }
    SIGN32 words[2];
    dhub_channel_generate_cmd(&pixels.dhub, CREDIT_READ_CHANNEL, address, 256,
        0, CREDIT_READY_CHANNEL, 0, 1, words);
    volatile uint32_t *command = TEE_TA_RegPhyToVirt(hbo->mem +
        config->chanCmdBase + slot * 16);
    command[0] = words[0]; command[1] = words[1];
    command[2] = 0; command[3] = 0;
    __asm__ volatile("dmb sy" ::: "memory");
    hbo_queue_push(hbo, 2 * CREDIT_READ_CHANNEL, 1);
    for (unsigned i = 0; i < 20; i++) TEE_Wait(1);
    credit_dma_snapshot(&pixels, &result.empty_ready);
    if (!result.empty_ready.completion && !result.empty_ready.data &&
        (result.empty_ready.busy || result.empty_ready.pending || result.empty_ready.commands))
        result.observations |= RX_DMA_CREDIT_WAITED;
    if (hardware_write) {
        unsigned write_slot;
        if (hbo_queue_query(hbo, 2 * CREDIT_READY_CHANNEL + 1, 0, &write_slot) ||
            write_slot >= (unsigned)writer->chanDataSize / 16) {
            result.result = TEE_ERROR_BUSY;
            goto cleanup;
        }
        for (unsigned i = 0; i < 64; i++) {
            unsigned entry = (write_slot + i / 4) % (writer->chanDataSize / 16);
            volatile uint32_t *data = TEE_TA_RegPhyToVirt(hbo->mem +
                writer->chanDataBase + entry * 16 + (i % 4) * 4);
            *data = RX_DMA_WRITE_READ_PATTERN | (i * 0x010101u);
        }
        /* This publishes FIFO input data. Hardware writes DDR and signals cell 9. */
        __asm__ volatile("dsb sy" ::: "memory");
        hbo_queue_push(hbo, 2 * CREDIT_READY_CHANNEL + 1, 16);
    } else {
        semaphore_push(sem, CREDIT_READY_CHANNEL, 1);
    }
    for (unsigned i = 0; i < 100; i++) {
        credit_dma_snapshot(&pixels, &result.released);
        if (result.released.completion && !result.released.busy &&
            !result.released.pending && !result.released.two_d) break;
        TEE_Wait(1);
    }
    if (result.released.completion && !result.released.busy &&
        !result.released.pending && !result.released.two_d)
        result.observations |= RX_DMA_CREDIT_COMPLETED;
    else result.result = TEE_ERROR_TIMEOUT;
    unsigned depth = config->chanDataSize / 16;
    /* 消费者推进指针后，SRAM 中仍保留命令内容；读取本次命令起始槽的全部 256 字节。 */
    if ((result.observations & RX_DMA_CREDIT_COMPLETED) &&
        !result.initial.data && result.initial.data_pointer < depth && depth >= 16) {
        __asm__ volatile("dsb sy" ::: "memory");
        for (unsigned i = 0; i < RX_DMA_CREDIT_FIFO_WORDS; i++) {
            unsigned entry = (result.initial.data_pointer + i / 4) % depth;
            volatile uint32_t *data = TEE_TA_RegPhyToVirt(hbo->mem +
                config->chanDataBase + entry * 16 + (i % 4) * 4);
            result.words[i] = *data;
        }
        result.words_valid = RX_DMA_CREDIT_FIFO_WORDS;
        result.observations |= RX_DMA_CREDIT_FIFO_PRESENT;
    }
    if (hardware_write) {
        result.write_busy = credit_dma_busy(&pixels) & (1u << CREDIT_READY_CHANNEL);
        unsigned pointer;
        result.write_completed = !result.write_busy &&
            !hbo_queue_query(hbo, 2 * CREDIT_READY_CHANNEL, 1, &pointer) &&
            !hbo_queue_query(hbo, 2 * CREDIT_READY_CHANNEL + 1, 1, &pointer) &&
            (result.observations & RX_DMA_CREDIT_COMPLETED);
        if (result.write_completed) result.observations |= RX_DMA_CREDIT_WRITE_COMPLETED;
    }
cleanup:
    result.cleanup_result = credit_clear_channel(&pixels, CREDIT_READ_CHANNEL);
    if (hardware_write) {
        TEE_Result cleared = credit_clear_channel(&pixels, CREDIT_READY_CHANNEL);
        if (!result.cleanup_result) result.cleanup_result = cleared;
        if (!cleared) result.writer_config_writes = dhub_channel_cfg(&pixels.dhub, CREDIT_READY_CHANNEL,
            writer->chanCmdBase, writer->chanDataBase, writer->chanCmdSize / 16,
            writer->chanDataSize / 16, writer->chanMtuSize, writer->chanQos,
            writer->chanSelfLoop, 0, 0, 1);
    }
    if (!result.cleanup_result) {
        /* Restore normal source configuration with owner still stopped. */
        result.config_writes = dhub_channel_cfg(&pixels.dhub, CREDIT_READ_CHANNEL,
            config->chanCmdBase, config->chanDataBase, config->chanCmdSize / 16,
            config->chanDataSize / 16, config->chanMtuSize, config->chanQos,
            config->chanSelfLoop, 0, 0, 1);
        __asm__ volatile("dsb sy" ::: "memory");
        credit_cell_normal(sem, CREDIT_READ_CHANNEL);
        credit_cell_normal(sem, CREDIT_READY_CHANNEL);
        *client = 0; /* 保持客户端停止。 */
        __asm__ volatile("dsb sy" ::: "memory");
    }
    credit_dma_snapshot(&pixels, &result.final);
    credit_dma_snapshot_channel(&pixels, &result.writer_final, CREDIT_READY_CHANNEL);
    result.ready_empty = semaphore_chk_empty(sem, CREDIT_READY_CHANNEL);
    result.ready_full = semaphore_chk_full(sem, CREDIT_READY_CHANNEL);
    result.ready_mask = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(sem->ra +
        RA_SemaHub_ARR + CREDIT_READY_CHANNEL * sizeof(SIE_Semaphore) + RA_Semaphore_mask);
    /* 检查可读的空闲状态；CFG/ARQOS 是只写寄存器。 */
    result.rx_restored = !result.cleanup_result &&
        (!hardware_write || result.writer_config_writes) &&
        !result.writer_final.busy && !result.writer_final.pending &&
        !result.writer_final.two_d && !result.writer_final.commands &&
        !result.writer_final.data && !result.final.ready_producer &&
        !result.final.ready_consumer && result.ready_empty &&
        !result.ready_full && !result.ready_mask;
    result.restored = result.rx_restored && result.config_writes &&
        !result.final.busy && !result.final.pending && !result.final.two_d &&
        !result.final.commands && !result.final.data;
    if (!result.restored && !result.cleanup_result)
        result.cleanup_result = TEE_ERROR_BAD_STATE;
report:
    *(struct rx_dma_credit_result *)params[1].memref.buffer = result;
    params[1].memref.size = sizeof(result);
    return TEE_SUCCESS;
}

TEE_Result rx_dma_credit_check(uint32_t types, TEE_Param params[4], int capture_active)
{
    return credit_dma_check(types, params, capture_active, 0);
}
TEE_Result rx_dma_write_read_credit_check(uint32_t types, TEE_Param params[4], int capture_active)
{
    return credit_dma_check(types, params, capture_active, 1);
}

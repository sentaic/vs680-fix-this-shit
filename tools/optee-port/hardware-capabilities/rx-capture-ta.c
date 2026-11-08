/* SPDX-License-Identifier: GPL-2.0 */
#include "tee_internal_api.h"
#include "galois_io.h"
#include "tee_ta_mmap.h"
TEE_Result loop_capture_command(uint32_t command, uint32_t types, TEE_Param p[4]);
#include "aio-payload.h"
TEE_Result rx_dma_credit_check(uint32_t types, TEE_Param p[4], int capture_active);
TEE_Result rx_dma_write_read_credit_check(uint32_t types, TEE_Param p[4], int capture_active);
#include "rx-dma-credit.h"
#include "api_avio_dhub.h"
#include "avioGbl.h"
#include "hdmi_tx_wrap.h"
#include "../../../sources/astra-current/tee-optee_dev/ta/display/ta/vpp/g7/vppdrv/source/BE/hdmi/vendor/synopsys/api/src/core/main_controller/main_controller_reg.h"
#include "../../../sources/astra-current/linux_5_15-modules/drivers/hrx-v4l2/hdmirxpipe.h"

#define PIPE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMIRXPIPE_REG_BASE)
#define CHANNEL avioDhubChMap_vpp128b_1DSCL_W0
#define MASK (1u << CHANNEL)
static HDL_dhub2d capture_dhub;
static int prepared, active;
static int input_pending;
static uint32_t pattern_seed = 0x804020;
static unsigned packed_input;
static unsigned strict_flow;
static uint32_t saved_tx[2];
static int tx_color_active;
static uint32_t reg_read(uint32_t offset) { return *(volatile uint32_t *)TEE_TA_RegPhyToVirt(PIPE + offset); }
static void reg_write(uint32_t offset, uint32_t value) { *(volatile uint32_t *)TEE_TA_RegPhyToVirt(PIPE + offset) = value; }
#include "rx-capture-state.c"
static void prepare(void)
{
    if (!prepared) {
        dhub2d_hdl(VPP_HBO_SRAM_BASE, VPP_DHUB_BASE, &capture_dhub);
        prepared = 1;
    }
}
static uint32_t busy(void)
{
    return (dhub_channel_busy(&capture_dhub.dhub) |
            dhub_channel_pending(&capture_dhub.dhub) |
            dhub2d_channel_busy(&capture_dhub)) & MASK;
}
#include "rx-capture-notify.h"
#include "rx-semaphore-credit.h"
static int capture_sync_enabled;
#include "rx-capture-bands.c"
#include "rx-capture-input-sync.c"
static struct rx_capture_input_sync_status capture_sync_status;
static TEE_Result stop(void)
{
    HDL_hbo *hbo = dhub_hbo(&capture_dhub.dhub);
    unsigned i;
    if (!active) return TEE_SUCCESS;
    reg_write(RA_HDMIRX_PIPE_tg_ctrl, 0);
    reg_write(RA_HDMIRX_PIPE_tg_ctrl, 2);
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_Wr, 0);
    dhub2nd_channel_enable(&capture_dhub, CHANNEL, 0, 0);
    dhub2nd_channel_clear(&capture_dhub, CHANNEL);
    dhub_channel_enable(&capture_dhub.dhub, CHANNEL, 0, 0);
    dhub_channel_clear(&capture_dhub.dhub, CHANNEL);
    for (i = 0; i < 500 && busy(); i++) TEE_Wait(1);
    if (busy()) return TEE_ERROR_BUSY;
    hbo_queue_enable(hbo, 2 * CHANNEL, 0, 0);
    hbo_queue_enable(hbo, 2 * CHANNEL + 1, 0, 0);
    hbo_queue_clear(hbo, 2 * CHANNEL);
    hbo_queue_clear(hbo, 2 * CHANNEL + 1);
    for (i = 0; i < 500; i++) {
        if (!(hbo_queue_busy(hbo) & (3u << (2 * CHANNEL)))) break;
        TEE_Wait(1);
    }
    if (i == 500) return TEE_ERROR_BUSY;
    unsigned pointer;
    __asm__ volatile("dsb sy" ::: "memory");
    /* 清除操作结束还须队列为空，才能恢复配置及释放采集缓冲。 */
    if (busy() ||
        hbo_queue_query(hbo, 2 * CHANNEL, SemaQueryMap_ADDR_master_consumer, &pointer) ||
        hbo_queue_query(hbo, 2 * CHANNEL + 1, SemaQueryMap_ADDR_master_consumer, &pointer))
        return TEE_ERROR_BUSY;
    HDL_semaphore *completion = dhub_semaphore(&capture_dhub.dhub);
    if (semaphore_chk_full(completion, -1) & MASK)
        semaphore_pop(completion, CHANNEL, 1);
    semaphore_clr_full(completion, CHANNEL);
    if (!capture_config_restore()) return TEE_ERROR_BAD_STATE;
    active = 0;
    input_pending = 0;
    return TEE_SUCCESS;
}
#include "../rx-high-refresh/rx-capture-config-ta.inc"
#include "../rx-high-refresh/rx-dma-plan-ta.inc"
#include "rx-capture-start.c"
#include "../rx-high-refresh/rx-command-pool-ta.inc"
#include "../rx-high-refresh/rx-aio-exclusive-ta.inc"
#include "../rx-high-refresh/rx-stream-ta.inc"
/* Source-defined fixed-color control from MV_VPPOBJ_EnableAutoPush.
 * Access exactly two wrapper registers; keep TX timing and DMA ownership intact.
 */
static TEE_Result tx_color(int enable)
{
    uint32_t base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMITX_WRAP_REG_BASE;
    volatile uint32_t *reg0 = TEE_TA_RegPhyToVirt(base + RA_HDMI_TX_WRAP_VID_CTRL);
    volatile uint32_t *reg1 = TEE_TA_RegPhyToVirt(base + RA_HDMI_TX_WRAP_VID_CTRL1);
    if (enable) {
        T32HDMI_TX_WRAP_VID_CTRL control = { .u32 = *reg0 };
        T32HDMI_TX_WRAP_VID_CTRL1 control1 = { .u32 = *reg1 };
        if (tx_color_active) return TEE_ERROR_BUSY;
        saved_tx[0] = control.u32; saved_tx[1] = control1.u32;
        control.uVID_CTRL_fix_clr_ch0 = 0xf00;
        control.uVID_CTRL_fix_clr_ch1 = 0x700;
        control1.uVID_CTRL_fix_clr_ch2 = 0x300;
        control.uVID_CTRL_fix_clr_sel = 1;
        *reg1 = control1.u32; *reg0 = control.u32;
        tx_color_active = 1;
    } else if (tx_color_active) {
        *reg1 = saved_tx[1]; *reg0 = saved_tx[0];
        tx_color_active = 0;
    }
    return TEE_SUCCESS;
}
static void credit_snapshot(HDL_semaphore *sem, struct rx_semaphore_credit_sample *sample)
{
    /* Query address: ID in bits 6:2, producer/consumer in bit 7. */
    uint32_t offset = CHANNEL << LSb32SemaQueryMap_ADDR_ID;
    volatile uint32_t *producer = TEE_TA_RegPhyToVirt(sem->ra +
        RA_SemaHub_Query + offset);
    volatile uint32_t *consumer = TEE_TA_RegPhyToVirt(sem->ra +
        RA_SemaHub_Query + offset + (1u << LSb32SemaQueryMap_ADDR_master));
    __asm__ volatile("dsb sy" ::: "memory");
    sample->producer = *producer;
    sample->consumer = *consumer;
}
static TEE_Result credit_check(uint32_t types, TEE_Param p[4])
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;
    if (p[0].memref.size < sizeof(struct rx_semaphore_credit)) {
        p[0].memref.size = sizeof(struct rx_semaphore_credit);
        return TEE_ERROR_SHORT_BUFFER;
    }
    if (!p[0].memref.buffer) return TEE_ERROR_BAD_PARAMETERS;
    prepare();
    /* 此 TA 使用采集通道 9 的信号量。 */
    if (active || busy()) return TEE_ERROR_BUSY;
    HDL_semaphore *sem = dhub_semaphore(&capture_dhub.dhub);
    volatile uint32_t *mask = TEE_TA_RegPhyToVirt(sem->ra + RA_SemaHub_ARR +
        CHANNEL * sizeof(SIE_Semaphore) + RA_Semaphore_mask);
    struct rx_semaphore_credit snapshot = {0};
    semaphore_intr_enable(sem, CHANNEL, 0, 0, 0, 0, 0);
    *mask = 0; /* Both producer-full and consumer-empty bypasses disabled. */
    semaphore_cfg(sem, CHANNEL, 4, 0);
    semaphore_clr_full(sem, CHANNEL);
    credit_snapshot(sem, &snapshot.steps[0]);
    semaphore_push(sem, CHANNEL, 1);
    credit_snapshot(sem, &snapshot.steps[1]);
    semaphore_push(sem, CHANNEL, 1);
    credit_snapshot(sem, &snapshot.steps[2]);
    semaphore_pop(sem, CHANNEL, 1);
    credit_snapshot(sem, &snapshot.steps[3]);
    /* Restore this capture channel's normal completion configuration. */
    semaphore_cfg(sem, CHANNEL, 1, 0);
    *mask = 0;
    semaphore_intr_enable(sem, CHANNEL, 0, 0, 0, 0, 0);
    semaphore_clr_full(sem, CHANNEL);
    __asm__ volatile("dsb sy" ::: "memory");
    *(struct rx_semaphore_credit *)p[0].memref.buffer = snapshot;
    p[0].memref.size = sizeof(snapshot);
    return TEE_SUCCESS;
}
TEE_Result loop_capture_command(uint32_t command, uint32_t types, TEE_Param p[4])
{
    TEE_Result result = TEE_SUCCESS;
    if (command == RX_COMMAND_POOL_COMMAND) {
        prepare();
        return rx_command_pool_command(types, p, capture_stream.opened || active || busy() || rx_exclusive_aio_owned);
    }
    if (command == RX_AIO_EXCLUSIVE_COMMAND) {
        prepare();
        return rx_aio_exclusive_command(types,p,capture_stream.opened || active || busy());
    }
    if (command == RX_STREAM_OPEN_COMMAND || command == RX_STREAM_SERVICE_COMMAND)
        return stream_command(command, types, p);
    /* 流打开期间由流接口修改硬件，旧接口提供状态读取。 */
    if (capture_stream.opened && command != 0x7f10 && command != 0x7f16 &&
        command != 0x7f17 && command != 0x7f18 && command != 0x7f24 && command != 0x7f2e)
        return TEE_ERROR_BUSY;
    if (command == RX_CAPTURE_CONFIG_COMMAND) return capture_geometry_command(types, p);
    if (command == 0x7f31) return loop_aio_payload_check(types, p, active);
    if (command == RX_DMA_CREDIT_COMMAND) return rx_dma_credit_check(types, p, active);
    if (command == RX_DMA_WRITE_READ_CREDIT_COMMAND)
        return rx_dma_write_read_credit_check(types, p, active);
    if (command == RX_SEMAPHORE_CREDIT_COMMAND) return credit_check(types, p);
    if (command == 0x7f2b) return band_collect(types, p);
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_VALUE_OUTPUT,
                                TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;
    prepare();
    if (command == 0x7f2f) {
        if (active || busy()) return TEE_ERROR_BUSY;
        if (p[0].value.a > 1) return TEE_ERROR_BAD_PARAMETERS;
        capture_sync_enabled = p[0].value.a;
        return TEE_SUCCESS;
    }
    if (command == 0x7f2e) {
        p[1].value.a = capture_sync_status.polls;
        p[1].value.b = capture_sync_status.input_status;
        p[2].value.a = capture_sync_status.event_status;
        p[2].value.b = capture_sync_status.elapsed_ms;
        return TEE_SUCCESS;
    }
    if (command == 0x7f28) {
        if (active || busy()) return TEE_ERROR_BUSY;
        unsigned rows = p[0].value.a;
        if (rows && (rows < 60 || rows > 720 || 720 % rows))
            return TEE_ERROR_BAD_PARAMETERS;
        band_lines = rows;
        return TEE_SUCCESS;
    }
    if (command == 0x7f2a) {
        if (active || busy()) return TEE_ERROR_BUSY;
        HDL_semaphore *sem = dhub_semaphore(&capture_dhub.dhub);
        semaphore_cfg(sem, CHANNEL, 16, 0);
        __asm__ volatile("dsb sy" ::: "memory");
        unsigned ptr = 0;
        p[1].value.a = semaphore_query(sem, CHANNEL, 1, &ptr);
        p[1].value.b = ptr;
        semaphore_push(sem, CHANNEL, 3);
        __asm__ volatile("dsb sy" ::: "memory");
        p[2].value.a = semaphore_query(sem, CHANNEL, 1, &ptr);
        p[2].value.b = ptr;
        semaphore_cfg(sem, CHANNEL, 1, 0);
        semaphore_clr_full(sem, CHANNEL);
        return TEE_SUCCESS;
    }
    if (command == 0x7f29) return band_progress(p);
    if (command == 0x7f26) {
        if (!active || !input_pending) return TEE_ERROR_BAD_STATE;
        input_pending = 0;
        if (!capture_sync_enabled) reg_write(RA_HDMIRX_PIPE_tg_ctrl, 1);
        p[1].value.a = busy();
        return TEE_SUCCESS;
    }
    if (command == 0x7f22) {
        if (active || busy()) return TEE_ERROR_BUSY;
        if (p[0].value.a > 1) return TEE_ERROR_BAD_PARAMETERS;
        strict_flow = p[0].value.a;
        return TEE_SUCCESS;
    }
    if (command == 0x7f20) return loop_display_mixer_command(p);
    if (command == 0x7f1f) return loop_display_fifo_command(p);
    if (command == 0x7f1d) return loop_display_graphics_command(p);
    if (command == 0x7f1e) {
        if (active || busy()) return TEE_ERROR_BUSY;
        if (p[0].value.a > 1) return TEE_ERROR_BAD_PARAMETERS;
        packed_input = p[0].value.a;
        return TEE_SUCCESS;
    }
    if (command == 0x7f27 || (command >= 0x7f1a && command <= 0x7f1c))
        return loop_display_dma_command(command, p);
    if (command == 0x7f19) {
        if (active || busy()) return TEE_ERROR_BUSY;
        if (p[0].value.a > 0xffffff) return TEE_ERROR_BAD_PARAMETERS;
        pattern_seed = p[0].value.a;
        return TEE_SUCCESS;
    }
    if (command == 0x7f18) {
        uint32_t base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMITX_WRAP_REG_BASE;
        p[1].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_HDMI_TX_WRAP_VID_CTRL);
        p[1].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + RA_HDMI_TX_WRAP_VID_CTRL1);
        p[2].value.a = reg_read(RA_HDMIRX_PIPE_CFG0);
        p[2].value.b = reg_read(RA_HDMIRX_PIPE_CFG4);
        return TEE_SUCCESS;
    }
    if (command == 0x7f24 || command == 0x7f25) {
        uint32_t global = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE;
        volatile uint32_t *clock = TEE_TA_RegPhyToVirt(
            global + RA_avioGbl_HDMIRX_VCLK_CTRL);
        if (command == 0x7f25) {
            if (p[0].value.a > 1) return TEE_ERROR_BAD_PARAMETERS;
            if (active || busy()) return TEE_ERROR_BUSY;
            uint32_t value = *clock;
            value &= ~MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn;
            if (p[0].value.a) value |= MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn;
            *clock = value;
        }
        p[1].value.a = *clock;
        p[1].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(
            global + RA_avioGbl_BLANK_CG);
        p[2].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(
            global + RA_avioGbl_SWRST_CTRL);
        uint32_t tx = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_SNPS_HDMITX_REG_BASE;
        volatile uint32_t *flow = TEE_TA_RegPhyToVirt(tx + MC_FLOWCTRL);
        volatile uint32_t *gates = TEE_TA_RegPhyToVirt(tx + MC_CLKDIS);
        if (!flow || !gates) return TEE_ERROR_BAD_STATE;
        p[2].value.b = (*flow & 255u) | ((*gates & 255u) << 8);
        return TEE_SUCCESS;
    }
    if (command == 0x7f17) {
        p[1].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE + RA_avioGbl_HDMI_CLK_EN);
        p[1].value.b = reg_read(RA_HDMIRX_PIPE_SRAMPWR_Y);
        p[2].value.a = reg_read(RA_HDMIRX_PIPE_SRAMPWR_UFP_FIFO);
        p[2].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMIRX_WRAP_REG_BASE + 0x30);
        return TEE_SUCCESS;
    }
    if (command == 0x7f16) {
        p[1].value.a = reg_read(RA_HDMIRX_PIPE_STATUS);
        p[1].value.b = reg_read(RA_HDMIRX_PIPE_STATUS1);
        p[2].value.a = reg_read(RA_HDMIRX_PIPE_STATUS2);
        p[2].value.b = reg_read(RA_HDMIRX_PIPE_VIPSTS + RA_VIPSTS_STS0);
        return TEE_SUCCESS;
    }
    if (command == 0x7f21) result = start(p[0].value.a, p[0].value.b, 2);
    else if (command == 0x7f11) result = start(p[0].value.a, p[0].value.b, 0);
    else if (command == 0x7f13) result = start(p[0].value.a, p[0].value.b, 1);
    else if (command == 0x7f12) result = stop();
    else if (command == 0x7f14) result = tx_color(1);
    else if (command == 0x7f15) result = tx_color(0);
    else if (command != 0x7f10) return TEE_ERROR_NOT_SUPPORTED;
    p[1].value.a = busy();
    /* 读取当前采集通道最后一条命令的完成位。 */
    p[1].value.b = semaphore_chk_full(dhub_semaphore(&capture_dhub.dhub), -1) & MASK;
    p[2].value.a = reg_read(RA_HDMIRX_PIPE_CTRL);
    p[2].value.b = reg_read(RA_HDMIRX_PIPE_BYPASS_CTRL);
    return result;
}

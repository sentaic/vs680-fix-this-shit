/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-capture-input-sync.h"
#include "Galois_memmap.h"
#include "avio_memmap.h"
#include "hdmiRxPipe.h"
#include "tee_ta_mmap.h"

/* hrx-v4l2/hdmirx.h:55,73,75,299,301,486,538-539,1126. */
#define RX_MAIN_STATUS 0x0150u
#define RX_DEFRAMER_CONFIG 0x0270u
#define RX_VSYNC_COUNT_CLEAR 0x0278u
#define RX_EVENT_STATUS 0x5050u
#define RX_EVENT_CLEAR 0x5058u
#define RX_TMDS_STABLE (1u << 1)
#define RX_VSYNC_EVENT 1u
#define RX_VSYNC_THRESHOLD_MASK (0xffu << 20)
#define RX_SYNC_TIMEOUT_MS 250u

static uint32_t input_sync_read(uint32_t base, uint32_t offset)
{
    return *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + offset);
}
static void input_sync_write(uint32_t base, uint32_t offset, uint32_t value)
{
    *(volatile uint32_t *)TEE_TA_RegPhyToVirt(base + offset) = value;
}
static uint64_t input_sync_millis(const TEE_Time *time)
{
    return (uint64_t)time->seconds * 1000u + time->millis;
}

TEE_Result rx_capture_input_sync_arm(struct rx_capture_input_sync_status *status)
{
    const uint32_t pipe = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMIRXPIPE_REG_BASE;
    const uint32_t input = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_HDMIRX_REG_BASE;
    struct rx_capture_input_sync_status local = {0};
    T32HDMIRX_PIPE_CFG0 cfg = { .u32 = input_sync_read(pipe, RA_HDMIRX_PIPE_CFG0) };
    T32HDMIRX_PIPE_CTRL ctrl = { .u32 = input_sync_read(pipe, RA_HDMIRX_PIPE_CTRL) };
    uint32_t saved_deframer = input_sync_read(input, RX_DEFRAMER_CONFIG);
    TEE_Time begin, now;
    TEE_Result result;

    /* 在调用方持有的管线复位区间内完成配置。 */
    if (!cfg.uCFG0_start_mask_reset) return TEE_ERROR_BAD_STATE;
    input_sync_write(pipe, RA_HDMIRX_PIPE_tg_ctrl, 2);
    input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_CTRL, 2);
    input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr, 0);
    cfg.uCFG0_vsync_cnt_val = 2;
    cfg.uCFG0_patvop_fifoCLR = 1;
    input_sync_write(pipe, RA_HDMIRX_PIPE_CFG0, cfg.u32);

    /* Preserve the vendor bootstrap before switching to actual FIFO clken.
     * hdmiRxPipe.h CTRL specifies two initial HDEs by reset default; disabling
     * them stalls the initial blank lines before FIFO flow can become active.
     */
    ctrl.uCTRL_SCL_CLKEN_CTRL = 0;
    ctrl.uCTRL_clken_ctrl = 1;
    ctrl.uCTRL_clken_cnt = 2;
    ctrl.uCTRL_oclken_ctrl = 1;
    ctrl.uCTRL_oclken_ocnt = 2;
    ctrl.uCTRL_tgEof_en = 0;
    input_sync_write(pipe, RA_HDMIRX_PIPE_CTRL, ctrl.u32);

    input_sync_write(input, RX_DEFRAMER_CONFIG,
        (saved_deframer & ~RX_VSYNC_THRESHOLD_MASK) | (1u << 20));
    input_sync_write(input, RX_EVENT_CLEAR, RX_VSYNC_EVENT);
    input_sync_write(input, RX_VSYNC_COUNT_CLEAR, 1);
    __asm__ volatile("dsb sy" ::: "memory");
    TEE_GetSystemTime(&begin);
    for (;;) {
        local.polls++;
        local.input_status = input_sync_read(input, RX_MAIN_STATUS);
        if (!(local.input_status & RX_TMDS_STABLE)) {
            result = TEE_ERROR_BAD_STATE;
            break;
        }
        local.event_status = input_sync_read(input, RX_EVENT_STATUS);
        TEE_GetSystemTime(&now);
        uint64_t elapsed = input_sync_millis(&now) - input_sync_millis(&begin);
        local.elapsed_ms = elapsed > UINT32_MAX ? UINT32_MAX : (uint32_t)elapsed;
        if (local.event_status & RX_VSYNC_EVENT) {
            result = TEE_SUCCESS;
            break;
        }
        if (elapsed >= RX_SYNC_TIMEOUT_MS) {
            result = TEE_ERROR_TIMEOUT;
            break;
        }
    }
    input_sync_write(input, RX_EVENT_CLEAR, RX_VSYNC_EVENT);
    input_sync_write(input, RX_DEFRAMER_CONFIG, saved_deframer);

    if (result == TEE_SUCCESS) {
        /* The fresh boundary replaces a guessed delay. Reset the hardware
         * two-VSYNC validity mask just as hrx-vip-isr.c does for its first ISR.
         * DMA was prepared by the caller while TG and FIFO were held clear.
         */
        cfg.uCFG0_start_mask_reset = 0;
        input_sync_write(pipe, RA_HDMIRX_PIPE_CFG0, cfg.u32);
        cfg.uCFG0_start_mask_reset = 1;
        input_sync_write(pipe, RA_HDMIRX_PIPE_CFG0, cfg.u32);
        cfg.uCFG0_start_mask_reset = 0;
        cfg.uCFG0_patvop_fifoCLR = 0;
        input_sync_write(pipe, RA_HDMIRX_PIPE_CFG0, cfg.u32);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr, 2);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr, 0);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_Wr, 1);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_CTRL, 2);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_CTRL, 0);
        input_sync_write(pipe, RA_HDMIRX_PIPE_VIP_CTRL, 1);
        input_sync_write(pipe, RA_HDMIRX_PIPE_tg_ctrl, 2);
        input_sync_write(pipe, RA_HDMIRX_PIPE_tg_ctrl, 0);
        __asm__ volatile("dsb sy" ::: "memory");
        input_sync_write(pipe, RA_HDMIRX_PIPE_tg_ctrl, 1);
        __asm__ volatile("dsb sy" ::: "memory");
    }
    if (status) *status = local;
    return result;
}

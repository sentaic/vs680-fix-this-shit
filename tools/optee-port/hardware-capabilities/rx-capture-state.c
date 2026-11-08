/* SPDX-License-Identifier: GPL-2.0 */
/* 放在 reg_read/reg_write 之后包含；保存可恢复的配置寄存器。 */
static const uint32_t capture_config_offsets[] = {
    RA_HDMIRX_PIPE_CTRL, RA_HDMIRX_PIPE_CTRL0,
    RA_HDMIRX_PIPE_BYPASS_CTRL, RA_HDMIRX_PIPE_CFG0, RA_HDMIRX_PIPE_CFG4,
    RA_HDMIRX_PIPE_VIPSCLTOP + RA_VIPSCLTOP_CTRL5,
    RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_NonStdRes,
    RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_pix,
    RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_pack,
    RA_HDMIRX_PIPE_hdmirxpipe_Inpix, RA_HDMIRX_PIPE_scl1d_Inpix,
    RA_HDMIRX_PIPE_scl1d_Outpix, RA_HDMIRX_PIPE_sclOFIFO_YInpix,
    RA_HDMIRX_PIPE_sclOFIFO_UVInpix,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_INIT,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_SIZE,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_HB,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_HB_CR,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_HB_CR2,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_VB0,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_VB0_CR,
    RA_HDMIRX_PIPE_VIP_TG + RA_TG_VB0_CR2,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_INIT,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_SIZE,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_HB,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_HB_CR,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_HB_CR2,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_VB0,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_VB0_CR,
    RA_HDMIRX_PIPE_VIP_OTG + RA_TG_VB0_CR2,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG0,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG1,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG2,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG3,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG4,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG5,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG6,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG7,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG8,
    RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG9,
};
static uint32_t saved_capture_config[sizeof(capture_config_offsets) / sizeof(uint32_t)];
static uint32_t saved_rx_vclk_enable;
static volatile uint32_t *rx_video_clock(void)
{
    return TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_AVIO_GBL_BASE + RA_avioGbl_HDMIRX_VCLK_CTRL);
}

static void capture_config_save(void)
{
    saved_rx_vclk_enable = *rx_video_clock() & MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn;
    for (unsigned i = 0; i < sizeof(capture_config_offsets) / sizeof(uint32_t); i++)
        saved_capture_config[i] = reg_read(capture_config_offsets[i]);
}

static int capture_config_restore(void)
{
    /* DMA and both queues have stopped before restoring configuration. */
    for (unsigned i = 0; i < sizeof(capture_config_offsets) / sizeof(uint32_t); i++)
        reg_write(capture_config_offsets[i], saved_capture_config[i]);
    volatile uint32_t *clock = rx_video_clock();
    *clock = (*clock & ~MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn) | saved_rx_vclk_enable;
    __asm__ volatile("dsb sy" ::: "memory");
    return (*clock & MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn) == saved_rx_vclk_enable;
}

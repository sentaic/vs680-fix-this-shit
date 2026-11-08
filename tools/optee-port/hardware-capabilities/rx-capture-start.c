/* SPDX-License-Identifier: GPL-2.0 */
/* Capture source, geometry and startup configuration. Included by the TA. */
static void geometry(uint32_t base, unsigned extra_x, unsigned extra_y,
                     unsigned vb_end, unsigned vb_start)
{
    T32TG_INIT initial = { .u32 = 0 };
    T32TG_SIZE size = { .u32 = 0 };
    T32TG_HB hb = { .u32 = 0 };
    T32TG_VB0 vb = { .u32 = 0 };
    size.uSIZE_X = (capture_geometry_configured ? capture_layout.width : 1280u) + extra_x; size.uSIZE_Y = (capture_geometry_configured ? capture_layout.height : 720u) + extra_y;
    hb.uHB_FE = (capture_geometry_configured ? capture_layout.width : 1280u) + 8u; hb.uHB_BE = 7;
    vb.uVB0_FE = (capture_geometry_configured ? capture_layout.height : 720u) + vb_end; vb.uVB0_BE = vb_start;
    if (capture_sync_enabled) {
        initial.uINIT_X = 1; initial.uINIT_Y = 1;
        reg_write(base + RA_TG_INIT, initial.u32);
    }
    reg_write(base + RA_TG_SIZE, size.u32);
    reg_write(base + RA_TG_HB, hb.u32);
    reg_write(base + RA_TG_HB_CR, hb.u32);
    reg_write(base + RA_TG_HB_CR2, hb.u32);
    reg_write(base + RA_TG_VB0, vb.u32);
    reg_write(base + RA_TG_VB0_CR, vb.u32);
    reg_write(base + RA_TG_VB0_CR2, vb.u32);
}
static TEE_Result start(uint32_t address, uint32_t length, int source)
{
    struct rx_dma_plan geometry_dma={0};
    if (capture_geometry_configured) {
        if (source != 0 || band_lines) return TEE_ERROR_NOT_SUPPORTED;
        if (rx_capture_buffer_validate(&capture_layout, address, length))
            return TEE_ERROR_BAD_PARAMETERS;
        if (!capture_geometry_matches(&capture_geometry)) return TEE_ERROR_BAD_STATE;
        if (rx_dma_plan_make(capture_layout.stride,capture_layout.stride,
                             capture_layout.height,&geometry_dma)) return TEE_ERROR_BAD_PARAMETERS;
    }
    int pattern = source == 1;
    int vop = source == 2;
    T32HDMIRX_PIPE_CFG0 cfg = { .u32 = reg_read(RA_HDMIRX_PIPE_CFG0) };
    T32HDMIRX_PIPE_CTRL ctrl = { .u32 = reg_read(RA_HDMIRX_PIPE_CTRL) };
    T32HDMIRX_PIPE_BYPASS_CTRL bypass = { .u32 = reg_read(RA_HDMIRX_PIPE_BYPASS_CTRL) };
    DHUB_channel_config *config = &VPP_config[CHANNEL];
    if (active || busy()) return TEE_ERROR_BUSY;
    /* Buffer comes from the nonsecure CMA window selected for this build. */
    if (address < RX_CAPTURE_CMA_BEGIN || length < (capture_geometry_configured ? capture_layout.bytes : 1280u * 720u * 3u) ||
        (uint64_t)address + length > RX_CAPTURE_CMA_END || (address & 63))
        return TEE_ERROR_BAD_PARAMETERS;
    if (config->chanId != CHANNEL) return TEE_ERROR_BAD_STATE;
    capture_config_save();
    active = 1;
    /* The capture owns the RX lease and saves this dedicated clock bit.
     * Enable the video clock before preparing FIFO and timing generators. */
    volatile uint32_t *video_clock = rx_video_clock();
    *video_clock |= MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn;
    __asm__ volatile("dsb sy" ::: "memory");
    if (!(*video_clock & MSK32avioGbl_HDMIRX_VCLK_CTRL_clkEn))
        return TEE_ERROR_BAD_STATE;
    /* The vendor receiver's VIP configuration pulses only the pipe sync reset. */
    volatile uint32_t *reset_reg = TEE_TA_RegPhyToVirt(MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_AVIO_GBL_BASE + RA_avioGbl_SWRST_CTRL);
    T32avioGbl_SWRST_CTRL reset = { .u32 = *reset_reg };
    reset.uSWRST_CTRL_vipPipeSyncRstn = 0; *reset_reg = reset.u32;
    reset.uSWRST_CTRL_vipPipeSyncRstn = 1; *reset_reg = reset.u32;
    if (pattern) {
        /* Constant seed and 720p timing, from the PATGEN register specification. */
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG0, 0);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG1, 0);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG2, 0);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG3, pattern_seed);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG4, (720u << 16) | 1280);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG5, (750u << 16) | 1650);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG6, (5u << 16) | 40);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG7, (25u << 16) | 260);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG8, 0);
        reg_write(RA_HDMIRX_PIPE_VIP_PATGEN + RA_PATGEN_CFG9, pattern_seed);
    }
    dhub_channel_cfg(&capture_dhub.dhub, CHANNEL, config->chanCmdBase,
        config->chanDataBase, config->chanCmdSize / 16, config->chanDataSize / 16,
        capture_geometry_configured ? geometry_dma.channel_mtu :
        band_lines ? dHubChannel_CFG_MTU_256byte : dHubChannel_CFG_MTU_4096byte,
        config->chanQos, 0, 1, 0, 1);
    semaphore_cfg(dhub_semaphore(&capture_dhub.dhub), CHANNEL, 1, 0);
    semaphore_clr_full(dhub_semaphore(&capture_dhub.dhub), CHANNEL);
    semaphore_intr_enable(dhub_semaphore(&capture_dhub.dhub), CHANNEL, 0, 0, 0, 0, 0);
    ctrl.uCTRL_read_sel = pattern || vop ? 1 : 0; ctrl.uCTRL_scl1d_in = 0;
    cfg.uCFG0_pat_vopFifo_ctrl = pattern;
    ctrl.uCTRL_hdmi420sp_wrbk = 0;
    /* Register specification requires continuous scaler clock for CPCB input. */
    if (vop) ctrl.uCTRL_SCL_CLKEN_CTRL = 1;
    if (strict_flow) {
        /* Wait for real FIFO status from the first line (register specification). */
        ctrl.uCTRL_clken_ctrl = 0;
        ctrl.uCTRL_oclken_ctrl = 0;
    }
    /* Bypass path must wait for input FIFO data and write-client readiness. */
    ctrl.uCTRL_fifo_sts_ctrl = 1;
    T32HDMIRX_PIPE_CTRL0 flow = { .u32 = reg_read(RA_HDMIRX_PIPE_CTRL0) };
    flow.uCTRL0_rdy0_sts_ctrl = 1;
    reg_write(RA_HDMIRX_PIPE_CTRL0, flow.u32);
    /* RGB444 selection and FIFO-empty gates from vip_prog_scl. */
    T32HDMIRX_PIPE_CFG4 layout = { .u32 = reg_read(RA_HDMIRX_PIPE_CFG4) };
    layout.uCFG4_scl_OFifoY_aEmpCtrl = 1;
    layout.uCFG4_scl_OFifoUV_aEmpCtrl = 1;
    layout.uCFG4_uv422_444_sel = 1;
    layout.uCFG4_uvrd_en_mask = 1;
    layout.uCFG4_UV_swap_regs = 0;
    layout.uCFG4_mode_yc42x = 0;
    reg_write(RA_HDMIRX_PIPE_CFG4, layout.u32);
    cfg.uCFG0_write_sel_8b = 0; cfg.uCFG0_CfgSelAlgn = 0x19;
    cfg.uCFG0_CfgSel_12_10_8 = !pattern && !vop && packed_input ? 1 : 0;
    cfg.uCFG0_fifo_rd_sel = !pattern && !vop && packed_input ? 1 : 0;
    cfg.uCFG0_hdmi_pk_8b_10b = 0; cfg.uCFG0_hdmipk_420_sel = 0;
    bypass.uBYPASS_CTRL_scl1d_bypass = 1;
    bypass.uBYPASS_CTRL_ups420_bypass = 1;
    bypass.uBYPASS_CTRL_csc_bypass = 1;
    bypass.uBYPASS_CTRL_dns444_422_bypass = 1;
    bypass.uBYPASS_CTRL_dither_bypass = 1;
    /* Generated specification: this bit enables PATGEN despite its name. */
    bypass.uBYPASS_CTRL_patgen_bypass = pattern;
    bypass.uBYPASS_CTRL_hdmiRxFifo_bypass = 1;
    T32VIPSCLTOP_CTRL5 output = { .u32 = reg_read(RA_HDMIRX_PIPE_VIPSCLTOP + RA_VIPSCLTOP_CTRL5) };
    output.uCTRL5_sclOutrdy_sts0_en = 0; output.uCTRL5_sclOutrdy_sts1_en = 0;
    output.uCTRL5_hde_msk_en = 1; output.uCTRL5_chroma_data_sel = 0;
    output.uCTRL5_luma_data_sel = 0; output.uCTRL5_hde_initval0 = 0;
    output.uCTRL5_sclOfifo_Y_stsCtrl = 1; output.uCTRL5_sclOifo_UV_stsCtrl = 1;
    reg_write(RA_HDMIRX_PIPE_VIPSCLTOP + RA_VIPSCLTOP_CTRL5, output.u32);
    reg_write(RA_HDMIRX_PIPE_CTRL, ctrl.u32);
    /* Receiver ISR masks invalid FIFO writes until two input VSyncs. */
    cfg.uCFG0_vsync_cnt_val = 2;
    cfg.uCFG0_start_mask_reset = 0;
    cfg.uCFG0_patvop_fifoCLR = 0;
    reg_write(RA_HDMIRX_PIPE_CFG0, cfg.u32);
    cfg.uCFG0_start_mask_reset = 1;
    cfg.uCFG0_patvop_fifoCLR = 1;
    reg_write(RA_HDMIRX_PIPE_CFG0, cfg.u32);
    if (!capture_sync_enabled) {
        cfg.uCFG0_start_mask_reset = 0;
        cfg.uCFG0_patvop_fifoCLR = 0;
        reg_write(RA_HDMIRX_PIPE_CFG0, cfg.u32);
    }
    reg_write(RA_HDMIRX_PIPE_BYPASS_CTRL, bypass.u32);
    T32VIP_WriteClient_NonStdRes nonstd = { .u32 = 0 };
    nonstd.uNonStdRes_pixlineTot = 0x64;
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_NonStdRes, nonstd.u32);
    /* VPP_VIP_TG_SetTGParams uses a separate inline VOP writeback timing. */
    geometry(RA_HDMIRX_PIPE_VIP_TG, vop ? 50 : 20, vop ? 6 : 3, 3, 2);
    geometry(RA_HDMIRX_PIPE_VIP_OTG, 10, 6, 1, 0);
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_pix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_WriteClient_pack, 5);
    reg_write(RA_HDMIRX_PIPE_hdmirxpipe_Inpix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_scl1d_Inpix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_scl1d_Outpix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_sclOFIFO_YInpix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_sclOFIFO_UVInpix, (capture_geometry_configured ? capture_layout.pixels : 1280u * 720u));
    reg_write(RA_HDMIRX_PIPE_tg_ctrl, 2);
    reg_write(RA_HDMIRX_PIPE_tg_ctrl, 0);
    dhub2nd_channel_start_seq(&capture_dhub, CHANNEL);
    if (band_lines) {
        band_address = address;
        band_queued = 0;
        band_completed = 0;
        dhub2nd_channel_enable(&capture_dhub, CHANNEL, 0, 0);
        band_enqueue();
    } else {
        if (capture_geometry_configured)
            rx_dma_plan_apply(&capture_dhub,CHANNEL,address,&geometry_dma,NULL);
        else
            dhub2nd_channel_cfg(&capture_dhub, CHANNEL, address, 256, 256, 15,
                               3840, 720, 0, 0, 1, 1, 0);
    }
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_Wr, 2);
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_Wr, 0);
    T32HDMIRX_PIPE_CFG2 status = { .u32 = reg_read(RA_HDMIRX_PIPE_CFG2) };
    status.uCFG2_patgenFifo_stsClr = 1;
    status.uCFG2_hdmiRxFifo_stsClr = 1;
    reg_write(RA_HDMIRX_PIPE_CFG2, status.u32);
    __asm__ volatile("dsb sy" ::: "memory");
    status.uCFG2_patgenFifo_stsClr = 0;
    status.uCFG2_hdmiRxFifo_stsClr = 0;
    reg_write(RA_HDMIRX_PIPE_CFG2, status.u32);
    if (capture_sync_enabled && source == 0) {
        input_pending = 1;
        return rx_capture_input_sync_arm(&capture_sync_status);
    }
    reg_write(RA_HDMIRX_PIPE_VIP_WR + RA_VIP_WriteClient_Wr, 1);
    reg_write(RA_HDMIRX_PIPE_VIP_CTRL, 2);
    reg_write(RA_HDMIRX_PIPE_VIP_CTRL, 0);
    reg_write(RA_HDMIRX_PIPE_VIP_CTRL, 1);
    input_pending = source == 0;
    if (!input_pending) reg_write(RA_HDMIRX_PIPE_tg_ctrl, 1);
    return TEE_SUCCESS;
}

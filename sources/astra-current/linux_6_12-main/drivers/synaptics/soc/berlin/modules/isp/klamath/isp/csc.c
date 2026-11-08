// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "camera_isp_driver.h"
#include "isp_ctrl.h"
#include "csipipe_pvt.h"
#include "csiPipe.h"
#include "csc_common.h"
#include "csc_icsccfg.h"



static uint32_t ConfigureDNS444To422(CSI_PL_CTX_t *ctx, CSC_CONFIG_t *csc_cfg)
{
	uint32_t val;
	(void)csc_cfg;
	val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_init_val_444to422,
		bHOST2DHUB_CTRL1_init_val_444to422);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);
	return 0;
}

static uint32_t ConfigureDNS422To420(CSI_PL_CTX_t *ctx, CSC_CONFIG_t *csc_cfg)
{
	uint32_t base_addr =
		ctx->pipe_base_addr + RA_HOST2DHUB_CSCDNS + RA_CSCDNS_DNS422_420;
	uint32_t val;

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_DNS422_420_CFG0);
	SET_BIT(val, 1, LSb32DNS422_420_CFG0_dns422_420_en, bDNS422_420_CFG0_dns422_420_en);
	SET_BIT(val, 1, LSb32DNS422_420_CFG0_sp_en, bDNS422_420_CFG0_sp_en);
	SET_BIT(val, 0, LSb32DNS422_420_CFG0_dns422_420_auto_pixcnt,
		bDNS422_420_CFG0_dns422_420_auto_pixcnt);
	SET_BIT(val, ctx->op_wt, LSb32DNS422_420_CFG0_dns422_420_hres,
		bDNS422_420_CFG0_dns422_420_hres);
	SET_BIT(val, (ctx->op_wt + DUMMY_TG_SIZE_H_BLANK),
		LSb32DNS422_420_CFG0_dns422_420_htot, bDNS422_420_CFG0_dns422_420_htot);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_DNS422_420_CFG0, val);
	val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
	if (csc_cfg->zero_line_delay) {
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_init_val_422to420,
			bHOST2DHUB_CTRL1_init_val_422to420);
	} else {
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_init_val_422to420,
			bHOST2DHUB_CTRL1_init_val_422to420);
	}
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_line_toggle_en_422to420,
		bHOST2DHUB_CTRL1_line_toggle_en_422to420);
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_HS_sel_422to420, bHOST2DHUB_CTRL1_HS_sel_422to420);
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_VS_sel_422to420, bHOST2DHUB_CTRL1_VS_sel_422to420);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);

	return 0;
}

static uint32_t CSC_ICSC_SetColorSpaceConversion(CSI_PL_CTX_t *ctx, CSC_CONFIG_t *csc_cfg)
{
	int ret = 0;
	uint32_t (*pOffset)[CSC_MAX_ICSC_OFF];
	CSC_MODE CscMode = csc_cfg->mode;
	uint32_t base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_CSCDNS + RA_CSCDNS_CSC;

	T32CSC_C17O24_CFG0 csc_cfg0;
	T32CSC_C17O24_CFG1 csc_cfg1;
	T32CSC_C17O24_CFG2 csc_cfg2;
	T32CSC_C17O24_CFG3 csc_cfg3;
	T32CSC_C17O24_CFG4 csc_cfg4;
	T32CSC_C17O24_CFG5 csc_cfg5;
	T32CSC_C17O24_CFG6 csc_cfg6;
	T32CSC_C17O24_CFG7 csc_cfg7;
	T32CSC_C17O24_CFG8 csc_cfg8;
	T32CSC_C17O24_CFG9 csc_cfg9;
	T32CSC_C17O24_CFG10 csc_cfg10;
	T32CSC_C17O24_CFG11 csc_cfg11;

	pOffset = gICscWindowOffset_8bits;
	csc_cfg0.mCFG0_C0 = gICscWindowCoeff[CscMode][2][0];
	csc_cfg1.mCFG1_C1 = gICscWindowCoeff[CscMode][2][1];
	csc_cfg2.mCFG2_C2 = gICscWindowCoeff[CscMode][2][2];
	csc_cfg3.mCFG3_C3 = gICscWindowCoeff[CscMode][1][0];
	csc_cfg4.mCFG4_C4 = gICscWindowCoeff[CscMode][1][1];
	csc_cfg5.mCFG5_C5 = gICscWindowCoeff[CscMode][1][2];
	csc_cfg6.mCFG6_C6 = gICscWindowCoeff[CscMode][0][0];
	csc_cfg7.mCFG7_C7 = gICscWindowCoeff[CscMode][0][1];
	csc_cfg8.mCFG8_C8 = gICscWindowCoeff[CscMode][0][2];
	csc_cfg9.mCFG9_OFF1 = pOffset[CscMode][0];
	csc_cfg10.mCFG10_OFF2 = pOffset[CscMode][1];
	csc_cfg11.mCFG11_OFF3 = pOffset[CscMode][2];

	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG0, csc_cfg0.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG1, csc_cfg1.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG2, csc_cfg2.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG3, csc_cfg3.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG4, csc_cfg4.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG5, csc_cfg5.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG6, csc_cfg6.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG7, csc_cfg7.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG8, csc_cfg8.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG9, csc_cfg9.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG10, csc_cfg10.u32);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_CSC_C17O24_CFG11, csc_cfg11.u32);

	return ret;
}

static int ConfigureCSCModule(void *handle, void *config)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	CSC_CONFIG_t *csc_cfg = (CSC_CONFIG_t*)config;
	uint32_t pix_tot;
	int val;

	val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
	SET_BIT(val, csc_cfg->input_sel, LSb32HOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl,
		bHOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);
	CSC_ICSC_SetColorSpaceConversion(ctx, csc_cfg);
	pix_tot = ctx->op_wt * ctx->op_ht;
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL3, pix_tot);

	return 0;
}

static int EnableCSCModule(void* handle, int enable)
{
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;

	/* Enabling CSC would automatically set DNS input as CSC */
	if (enable == 1) {
		val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_enable_csc, bHOST2DHUB_CTRL1_enable_csc);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);
	} else {
		val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_enable_csc, bHOST2DHUB_CTRL1_enable_csc);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);
	}

	return 0;
}

int csc_s_ctrl(struct isp_ctrl *ctrl)
{
	switch (ctrl->id)
	{
		case CID_CSC_ENABLE:
			EnableCSCModule(ctrl->handler, 1);
			break;
		case CID_CSC_DISABLE:
			EnableCSCModule(ctrl->handler, 0);
			break;
		case CID_CSC_CONFIG:
			ConfigureCSCModule(ctrl->handler, ctrl->cfg);
			break;
		case CID_CSC_DNS_444_TO_422:
			ConfigureDNS444To422(ctrl->handler, ctrl->cfg);
			break;
		case CID_CSC_DNS_422_TO_420:
			ConfigureDNS422To420(ctrl->handler, ctrl->cfg);
			break;
		default:
			pr_err("unknown ctrl id %d\n", ctrl->id);
			return -1;
	}
	return 0;
}

// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "csiPipe.h"
#include "camera_isp_driver.h"
#include "isp_ctrl.h"
#include "csipipe_api.h"
#include "csipipe_pvt.h"
#include "fvf.h"
#include "klamath_memmap.h"

typedef struct FVF_CTRL_s {
	FVF_CONFIG_t fc_fvf_config;
} FVF_CTRL_t;

static int ConfigureFvfModule(void *handle, FVF_CONFIG_t *fc_fvf_cfg)
{
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_FVF;

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_FVF_FVF_CFG0);
	SET_BIT(val, 0, LSb32FVF_FVF_CFG0_FVF_ENABLE,  bFVF_FVF_CFG0_FVF_ENABLE);
	SET_BIT(val, (fc_fvf_cfg->frame_width), LSb32FVF_FVF_CFG0_FVF_FRAME_WIDTH, bFVF_FVF_CFG0_FVF_FRAME_WIDTH);
	SET_BIT(val, fc_fvf_cfg->frame_height, LSb32FVF_FVF_CFG0_FVF_FRAME_HEIGHT, bFVF_FVF_CFG0_FVF_FRAME_HEIGHT);
	SET_BIT(val, 1, LSb32FVF_FVF_CFG0_IGNORE_LINE_CTL, bFVF_FVF_CFG0_IGNORE_LINE_CTL);
	SET_BIT(val, 1, LSb32FVF_FVF_CFG0_FRAME_COUNTERS_EN, bFVF_FVF_CFG0_FRAME_COUNTERS_EN);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_FVF_FVF_CFG0, val);

//TODO FVF CFG1 control
	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_FVF_FVF_CFG1);
	SET_BIT(val, fc_fvf_cfg->min_frame_gap, LSb32FVF_FVF_CFG1_MIN_FRAME_GAP, bFVF_FVF_CFG1_MIN_FRAME_GAP);
	SET_BIT(val, fc_fvf_cfg->min_line_gap, LSb32FVF_FVF_CFG1_MIN_LINE_GAP , bFVF_FVF_CFG1_MIN_LINE_GAP);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_FVF_FVF_CFG1, val);

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_FVF_FVF_MAX_FRM_CFG);
	SET_BIT(val, fc_fvf_cfg->max_frame_duration_enable, LSb32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN, bFVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN);
	SET_BIT(val, fc_fvf_cfg->max_frame_duration_value, LSb32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE, bFVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_FVF_FVF_MAX_FRM_CFG, val);

	return 0;
}

static int EnableFvfModule(void* handle, int enable)
{
	CSI_PL_CTX_t *cam = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = cam->pipe_base_addr + RA_HOST2DHUB_FVF;
	int val;

	val =  CAM_HAL_ReadReg(cam->dev, base_addr + RA_FVF_FVF_CFG0);
	SET_BIT(val, enable, LSb32FVF_FVF_CFG0_FVF_ENABLE,	bFVF_FVF_CFG0_FVF_ENABLE);
	CAM_HAL_WriteReg(cam->dev, NULL, base_addr + RA_FVF_FVF_CFG0, val);

	return 0;
}

static int EnableFvfErrors(void* handle, int error_mask)
{
	CSI_PL_CTX_t *cam = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = cam->pipe_base_addr + RA_HOST2DHUB_FVF;
	int val;

	val = error_mask & 0x1ff;
	CAM_HAL_WriteReg(cam->dev, NULL, base_addr + RA_FVF_FVF_ERROR_EN_CFG, val);

	return 0;
}

static int PrintFvfStatus(void* handle)
{
	uint32_t count, status;
	uint32_t frm_cnt, prc_frm_cnt;
	CSI_PL_CTX_t *cam = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = cam->pipe_base_addr + RA_HOST2DHUB_FVF;

	count = CAM_HAL_ReadReg(cam->dev, base_addr + RA_FVF_FVF_COUNT);
	status = CAM_HAL_ReadReg(cam->dev, base_addr + RA_FVF_FVF_STATUS1);
	frm_cnt = count & MSK32FVF_FVF_COUNT_FRAME_COUNT;
	prc_frm_cnt = (count & MSK32FVF_FVF_COUNT_PROCESSED_FRAME_COUNT) >> 16;
	prc_frm_cnt &= 0xffff;
	pr_info("Frame Count = %d Processed Frame Count = %d\n", frm_cnt, prc_frm_cnt);
	pr_err("FVF ERR: 0x%x\n", status);
	return 0;
}

int fvf_s_ctrl(struct isp_ctrl *ctrl)
{
	FVF_CONFIG_t *cfg = (FVF_CONFIG_t*)ctrl->cfg;
	switch (ctrl->id)
	{
		case CID_FVF_ENABLE:
			EnableFvfModule(ctrl->handler, cfg->enable);
			break;
		case CID_FVF_CONFIG:
			ConfigureFvfModule(ctrl->handler, cfg);
			break;
		case CID_FVF_EN_ERRS:
			EnableFvfErrors(ctrl->handler, cfg->error_mask);
			break;
		case CID_FVF_G_STATUS:
			PrintFvfStatus(ctrl->handler);
			break;
		default:
			pr_err("unknown ctrl id %d\n", ctrl->id);
			return -1;
	}
	return 0;
}

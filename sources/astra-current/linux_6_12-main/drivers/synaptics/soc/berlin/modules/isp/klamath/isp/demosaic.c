// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "camera_isp_driver.h"
#include "csiPipe.h"
#include "csipipe_api.h"
#include "csipipe_pvt.h"
#include "demosaic.h"
#include "klamath_memmap.h"

typedef struct DEMOSAIC_CTRL_s {
	DEMOSAIC_CONFIG_t fc_demosaic_config;
}DEMOSAIC_CTRL_t;

static int ConfigureDemosaicModule(void * handle, void *config)
{
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_DEMOSAIC;

	DEMOSAIC_CONFIG_t *fc_demosaic_cfg = (DEMOSAIC_CONFIG_t*)config;

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_DEMOSAIC_CTRL);
	SET_BIT(val, 0, LSb32DEMOSAIC_CTRL_enable, bDEMOSAIC_CTRL_enable);
	SET_BIT(val, fc_demosaic_cfg->width, LSb32DEMOSAIC_CTRL_image_width,
		bDEMOSAIC_CTRL_image_width);
	SET_BIT(val, fc_demosaic_cfg->height, LSb32DEMOSAIC_CTRL_image_height,
		bDEMOSAIC_CTRL_image_height);
	SET_BIT(val, fc_demosaic_cfg->demosaic_mode, LSb32DEMOSAIC_CTRL_mode,
		bDEMOSAIC_CTRL_mode);
	SET_BIT(val, fc_demosaic_cfg->input_sel, LSb32DEMOSAIC_CTRL_input_sel,
		bDEMOSAIC_CTRL_input_sel);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_DEMOSAIC_CTRL, val);

	val = CAM_HAL_ReadReg(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1);
	SET_BIT(val, fc_demosaic_cfg->swizzle_ctrl, LSb32HOST2DHUB_CTRL1_DEMOSAIC_SWIZZLE,
		bHOST2DHUB_CTRL1_DEMOSAIC_SWIZZLE);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CTRL1, val);

	return 0;
}

static int EnableDemosaicModule(void * handle, int enable)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_DEMOSAIC;
	int val;

	if (enable == 1) {
		val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_DEMOSAIC_CTRL);
		SET_BIT(val, enable, LSb32DEMOSAIC_CTRL_enable, bDEMOSAIC_CTRL_enable);
		CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_DEMOSAIC_CTRL, val);
	}

	return 0;
}

int demosaic_s_ctrl(struct isp_ctrl *ctrl)
{
	switch (ctrl->id)
	{
		case CID_DEMOSAIC_ENABLE:
			EnableDemosaicModule(ctrl->handler, 1);
			break;
		case CID_DEMOSAIC_CONFIG:
			ConfigureDemosaicModule(ctrl->handler, ctrl->cfg);
			break;
		default:
			pr_err("unknown ctrl id %d\n", ctrl->id);
			return -1;
	}
	return 0;
}

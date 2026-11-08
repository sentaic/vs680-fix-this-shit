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
#include "wb.h"
#include "klamath_memmap.h"

typedef struct WB_CTRL_s {
	WB_CONFIG_t fc_wb_config;
} WB_CTRL_t;

static int ConfigureWhiteBalanceModule(void *handle, void *config)
{
	WB_CONFIG_t *fc_wb_cfg = (WB_CONFIG_t*)config;
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_WB;

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_WB_CTRL);
	SET_BIT(val, fc_wb_cfg->wb_en, LSb32WB_CTRL_enable, bWB_CTRL_enable);
	SET_BIT(val, fc_wb_cfg->wb_mode, LSb32WB_CTRL_mode, bWB_CTRL_mode);
	SET_BIT(val, fc_wb_cfg->wb_p00_mantissa, LSb32WB_CTRL_p00_man, bWB_CTRL_p00_man);
	SET_BIT(val, fc_wb_cfg->wb_p00_exponent, LSb32WB_CTRL_p00_exp, bWB_CTRL_p00_exp);
	SET_BIT(val, fc_wb_cfg->wb_p01_mantissa, LSb32WB_CTRL_p01_man, bWB_CTRL_p01_man);
	SET_BIT(val, fc_wb_cfg->wb_p01_exponent, LSb32WB_CTRL_p01_exp, bWB_CTRL_p01_exp);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_WB_CTRL, val);

	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_WB_CTRL1);
	SET_BIT(val, fc_wb_cfg->wb_p10_mantissa, LSb32WB_CTRL1_p10_man, bWB_CTRL1_p10_man);
	SET_BIT(val, fc_wb_cfg->wb_p10_exponent, LSb32WB_CTRL1_p10_exp, bWB_CTRL1_p10_exp);
	SET_BIT(val, fc_wb_cfg->wb_p11_mantissa, LSb32WB_CTRL1_p11_man, bWB_CTRL1_p11_man);
	SET_BIT(val, fc_wb_cfg->wb_p11_exponent, LSb32WB_CTRL1_p11_exp, bWB_CTRL1_p11_exp);
	SET_BIT(val, fc_wb_cfg->input_sel, LSb32WB_CTRL1_input_sel, bWB_CTRL1_input_sel);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_WB_CTRL1, val);

	return 0;
}

static int EnableWhiteBalanceModule(void* handle, int enable)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) handle;
	unsigned int base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_WB;
	int val;

	pr_debug("[WB] EnableWhiteBalanceModule: enable=%d, pipe_base=0x%x\n",
		   enable, ctx->pipe_base_addr);

	/* Read current WB control register */
	val = CAM_HAL_ReadReg(ctx->dev, base_addr + RA_WB_CTRL);

	/* Set or clear the enable bit */
	SET_BIT(val, enable ? 1 : 0, LSb32WB_CTRL_enable, bWB_CTRL_enable);

	/* Write back to register */
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_WB_CTRL, val);

	pr_debug("[WB] WB module %s (reg_val=0x%x)\n",
		   enable ? "enabled" : "disabled", val);

	return 0;
}

int wb_s_ctrl(struct isp_ctrl *ctrl)
{
	switch (ctrl->id) {
	case CID_WB_ENABLE:
		if (ctrl->cfg) {
			WB_CONFIG_t *wb_cfg = (WB_CONFIG_t *)ctrl->cfg;
			EnableWhiteBalanceModule(ctrl->handler, wb_cfg->wb_en);
		} else {
			EnableWhiteBalanceModule(ctrl->handler, 0);
		}
		break;
	case CID_WB_CONFIG:
		ConfigureWhiteBalanceModule(ctrl->handler, ctrl->cfg);
		break;
	default:
		pr_err("unknown ctrl id %d\n", ctrl->id);
		return -1;
	}
	return 0;
}

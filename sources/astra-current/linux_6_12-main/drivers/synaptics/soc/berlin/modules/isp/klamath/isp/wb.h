// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __WB_H__
#define __WB_H__
#include <linux/types.h>
#include "isp_ctrl.h"

enum {
	CID_WB_ENABLE = 0,
	CID_WB_CONFIG,
	CID_WB_MAX,
};

/**
 * White balance configuration
 */
typedef struct WB_CONFIG_s {
//	  MODULE_STATE_t state;
	/* 0 white balance disabled
	   1 white balance enabled */
	int wb_en;
	/* 0 white balance mode
	   1 perform white balancing */
	int wb_mode;
	//Upper left pixel configuration
	int wb_p00_mantissa;
	int wb_p00_exponent;
	//Upper right pixel configuration
	int wb_p01_mantissa;
	int wb_p01_exponent;
	//Lower left pixel configuration
	int wb_p10_mantissa;
	int wb_p10_exponent;
	//Loer right pixel configuration
	int wb_p11_mantissa;
	int wb_p11_exponent;
	int input_sel;
} WB_CONFIG_t;

int wb_s_ctrl(struct isp_ctrl *ctrl);
#endif

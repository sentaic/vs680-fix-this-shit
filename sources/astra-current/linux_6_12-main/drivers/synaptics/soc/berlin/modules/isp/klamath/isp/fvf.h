// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __FVF_H__
#define __FVF_H__
#include <linux/types.h>
#include "isp_ctrl.h"

enum {
	CID_FVF_ENABLE = 0,
	CID_FVF_CONFIG,
	CID_FVF_EN_ERRS,
	CID_FVF_G_STATUS,
	CID_FVF_MAX,
};

/**
 * Structure for holding FVF parameters
 */
typedef struct FVF_CONFIG_s {
//	  LPS_MODULE_STATE_t state;
	int enable;
	int frame_width;
	int frame_height;
	int ignore_line_control;
	int framecounter_enable;

	int min_frame_gap;
	int min_line_gap;
	int halt_enable_eol;
	int halt_period;

	int max_frame_duration_enable;
	int max_frame_duration_value;
	int error_mask;
} FVF_CONFIG_t;

int fvf_s_ctrl(struct isp_ctrl *ctrl);
#endif

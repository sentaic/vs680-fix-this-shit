// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __DEMOSAIC_H__
#define __DEMOSAIC_H__
#include <linux/types.h>
#include "isp_ctrl.h"

enum {
	CID_DEMOSAIC_ENABLE = 0,
	CID_DEMOSAIC_CONFIG = 1,
	CID_DEMOSAIC_MAX,
};

/**
 * Demosaic modes
 */
typedef enum DEMOSAIC_MODE_s {
	DEMOSAIC_BYPASS,
	DEMOSAIC_BGbGrR,
	DEMOSAIC_GbBRGr,
	DEMOSAIC_GrRBGb,
	DEMOSAIC_RGrGbB,
	DEMOSAIC_MAX = DEMOSAIC_RGrGbB
} DEMOSAIC_MODE;

/**
 * Demosaic configuration
 */
typedef struct DEMOSAIC_CONFIG_s {
	int enable;
//	  MODULE_STATE_t state;
	DEMOSAIC_MODE demosaic_mode;
	int width;
	int height;
	int input_sel;
	int swizzle_ctrl;
} DEMOSAIC_CONFIG_t;

int demosaic_s_ctrl(struct isp_ctrl *ctrl);
#endif

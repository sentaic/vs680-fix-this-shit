// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __CSC_COMMON__
#define __CSC_COMMON__

#include <linux/types.h>
#include "isp_ctrl.h"

// Type of Channel
typedef enum tagCSC_TYPE
{
	CSC_MAIN = 0,
	CSC_PIP,
	CSC_VOP2,
	CSC_VPTOP,
	MAX_CSC
 } CSC_TYPE, *PCSC_TYPE;

/* Enumeration of different O/P types of CSC */

typedef enum tagCSC_MODE
{
	CSC_BYPASS_MODE = 0,
	CSC_YUV_601_TO_709,
	CSC_YUV_709_TO_601,
	CSC_RGB_TO_YUV_2020,
	CSC_YUV_2020_TO_RGB,
	CSC_RGB_TO_YUV_709,
	CSC_YUV_709_TO_RGB,
	CSC_RGB_TO_YUV_601,
	CSC_YUV_601_TO_RGB,
	CSC_sRGB_TO_YUV_2020,
	CSC_YUV_2020_TO_sRGB,
	CSC_sRGB_TO_YUV_709,
	CSC_YUV_709_TO_sRGB,
	CSC_sRGB_TO_YUV_601,
	CSC_YUV_601_TO_sRGB,
	CSC_YUV_601_TO_YUV_2020,
	CSC_YUV_2020_TO_YUV_601,
	CSC_YUV_709_TO_YUV_2020,
	CSC_YUV_2020_TO_YUV_709,
	CSC_UV_SWAP,
	CSC_MAX_CONV_TYPES
} CSC_MODE;

/*
 * CSC common types
 */

/* Enumeration of different I/P types to CSC */
typedef enum tagCSC_INPUT_SIGNAL_TYPE
{
	CSC_YUV_601 = 0,
	CSC_YUV_709,
	CSC_RGB_444
} CSC_INPUT_SIGNAL_TYPE,*PCMU_INPUT_SIGNAL_TYPE;

enum {
	CID_CSC_DISABLE = 0,
	CID_CSC_ENABLE = 1,
	CID_CSC_CONFIG = 2,
	CID_CSC_DNS_444_TO_422 = 3,
	CID_CSC_DNS_422_TO_420 = 4,
};

typedef struct CSC_CONFIG_s
{
	uint32_t input_sel;
	uint32_t enable;
	CSC_MODE mode;
	uint8_t zero_line_delay;
} CSC_CONFIG_t;

int csc_s_ctrl(struct isp_ctrl *ctrl);
#endif //__CSC_COMMON__

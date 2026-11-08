// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CSIPIPE_H__
#define __CSIPIPE_H__
#include <linux/types.h>

typedef enum CAM_HAL_CAPTURE_TYPE_e
{
	CONTINUOUS_CAPTURE_SW,
	SINGLE_CAPTURE_HW,
	SINGLE_CAPTURE_SW,
	NTH_CAPTURE_HW,
	NTH_CAPTURE_SW,
} CAM_HAL_CAPTURE_TYPE_t;

typedef enum MODULE_ID_e {
	MODULE_IIF = 0,
	MODULE_IPI0,
	MODULE_IPI1,
	MODULE_IMGRES,
	MODULE_FVF,
	MODULE_WB,
	MODULE_DEMOSAIC,
	MODULE_CSC,
	MODULE_DNS444_422,
	MODULE_DNS422_420,
	MODULE_DHUB,
	MODULE_MAX
} MODULE_ID;

typedef enum CAM_PIXFMT_s {
	CAM_PIXFMT_RAW8		= 0,
	CAM_PIXFMT_RAW16	= 1,
	CAM_PIXFMT_RGB888	= 2,
	CAM_PIXFMT_RGB565	= 3,
	CAM_PIXFMT_YUV444	= 4,
	CAM_PIXFMT_YUV422SP = 5,
	CAM_PIXFMT_YUV422P	= 6,
	CAM_PIXFMT_YUV420SP = 7,
} CAM_PIXFMT;

/**
 * Structure for holding CAM parameters
 */
typedef struct CAM_CONFIG_s {
	int skip_frame_num;
	int capture_frame_interval; /// for Nth frame capture mode - value of N
	CAM_HAL_CAPTURE_TYPE_t capture_frame_mode;
	u8 mute;
	// Parameters related to ImageRes config
	int scale;
	u8 imgres_oprn; // subsampling / binning
	int crop_x_st;
	int crop_x_end;
	int crop_y_st;
	int crop_y_end;
	u8 src;
	u8 wb_en;
	u8 fvf_en;
	u8 op_through_ipi;
	u8 imgres_of;
	u32 src_fmt;
	u8 inp_comp_order; // grayscale, bggr, gbrg, grbg, rggb
} CAM_CONFIG_t;

typedef struct CSIPIPE_CFG_s {
	MODULE_ID id;
	void *mod_cfg;
} CSIPIPE_CFG_t;

int CSI_PIPE_Init(struct camera_isp_dev *isp_dev);
void CSI_PIPE_Exit(struct camera_isp_dev *isp_dev);
CSIPIPE_HANDLE CSI_PIPE_Create(struct camera_isp_dev *isp_dev, int pipe);
void CSI_PIPE_Destroy(CSIPIPE_HANDLE);

void CSI_PIPE_Set_Fmt(CSIPIPE_HANDLE handle, struct v4l2_mbus_framefmt *format);
void CSI_PIPE_Set_Input_Fmt(CSIPIPE_HANDLE handle, uint32_t width, uint32_t height);
int CSI_PIPE_Config(CSIPIPE_HANDLE handle, uint32_t mbus_code, uint32_t scale_factor);
void CSI_PIPE_Start(CSIPIPE_HANDLE handle);
void CSI_PIPE_Stop(CSIPIPE_HANDLE handle);
void CSI_PIPE_Mute(CSIPIPE_HANDLE handle, int mute);
void CSI_PIPE_Capture(CSIPIPE_HANDLE handle);
void CSI_PIPE_Status(CSIPIPE_HANDLE handle);
#endif

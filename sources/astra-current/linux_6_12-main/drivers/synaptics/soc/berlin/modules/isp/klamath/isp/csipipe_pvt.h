// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CSIPIPE_PVT_H__
#define __CSIPIPE_PVT_H__
#include <linux/types.h>

#include "camera_v4l2_common.h"
#include "cam_bcmbuf.h"

#define BCM_BUF_RING_NUM	3
#define MAX_INTR			32

/* TG Configurations */
#define DUMMY_TG_SIZE_H_BLANK	   12
#define DUMMY_TG_HB_BE			   7
#define DUMMY_TG_HB_FE_OFF		   (DUMMY_TG_HB_BE) + 1
#define DUMMY_TG_HS_FE			   3
#define DUMMY_TG_HS_BE_OFF		   DUMMY_TG_SIZE_H_BLANK
#define DUMMY_TG_SIZE_V_BLANK	   3
#define DUMMY_TG_VB_FE_OFF		   2
#define DUMMY_TG_VB_BE			   1
#define DUMMY_TG_VS_FE			   2
#define DUMMY_TG_VS_BE_OFF		   DUMMY_TG_SIZE_V_BLANK

enum SRC_INTF {
	SRC_INTF_IIF = 0,
	SRC_INTF_IPI0,
	SRC_INTF_IPI1,
};

typedef struct _CAM_HAL_BCMBUF_ITEM_ {
	struct BCMBUF bcmBuf;
	struct BCMBUF dhub_bcmBuf;

	struct DHUB_CFGQ dhub_cfgQ;
	struct DHUB_CFGQ final_cfgQ;
} CAM_HAL_BCMBUF_ITEM;

typedef struct CSI_PL_CROP_WINDOW_s
{
	u32 x_st;
	u32 x_end;
	u32 y_st;
	u32 y_end;
	u32 scale;
	u32 imgres_oprn;
	u8 imgres_paths;
} CSI_PL_CROP_WINDOW_t;

typedef struct CSI_PL_CTX_s {
	void *parent;
	struct camera_isp_dev *dev;
	u32 id;
	atomic_t active;
	CAM_HAL_BCMBUF_ITEM p_bcmq[BCM_BUF_RING_NUM];
	CAM_HAL_BCMBUF_ITEM *p_curr_bcmq;
	u8 curr_bcm_index;
	u32 pipe_base_addr;
	u8 src;
	u32 hres;
	u32 vres;
	u32 src_fmt;
	u32 op_fmt;
	u8 inp_comp_order;
	u32 swizzle_ctrl;
	u8 capture_mode;
	u32 capture_frame_interval;
	u32 skip_frame_num;
	CSI_PL_CROP_WINDOW_t crop;
	u8 wb_en;
	u8 fvf_en;
	u8 op_through_ipi;
	u8 yuv420_dir_op;
	u32 frame_cnt;
	u32 op_wt;
	u32 op_ht;
	u32 op_bpp;
	u32 pack_sel;
	u32 y_wr_ip;
	u32 c_wr_ip;
	u32 enabled_modules;
	u32 intr_cnt[MAX_INTR];
	u8 bcm_enable;
	u8 tg_en;
	struct {
		spinlock_t lock;
		struct list_head queue;
		struct camera_vb2_buffer *curr;
		struct camera_vb2_buffer *next;
	} buf;
} CSI_PL_CTX_t;

#endif

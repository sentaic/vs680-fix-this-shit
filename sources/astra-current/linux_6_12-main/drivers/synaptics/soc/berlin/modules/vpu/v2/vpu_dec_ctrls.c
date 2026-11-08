// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022 - 2023 Synaptics Incorporated.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/errno.h>
#include <linux/videodev2.h>
#include <media/v4l2-common.h>

#include "vpu_common.h"
#include "vpu_dec_ctrls.h"
#include "vpu_dec_drv.h"
#include "vpu_fw.h"
#include "v4l2_syna_externtion.h"

extern int vdpu_debug;
#define vdpu_dbg(level, fmt, arg...)                                           \
	v4l2_dbg(level, vdpu_debug, &vpu->v4l2_dev, fmt, ##arg)
#define vdpu_info(vpu, fmt, arg...) v4l2_info(&vpu->v4l2_dev, fmt, ##arg)
#define vdpu_err(vpu, fmt, arg...) v4l2_err(&vpu->v4l2_dev, fmt, ##arg)

static int vidioc_vdec_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct syna_vcodec_ctx *ctx;
	struct syna_vdec_config *p;
	struct syna_vpu_dev *vpu;
	struct syna_vpu_ctrl *vpu_ctrl;
	int ret = 0;

	ctx = container_of(ctrl->handler, struct syna_vcodec_ctx, ctrl_handler);
	vpu_ctrl = syna_vpu_get_ctrl_shm_buffer(ctx);
	vpu = ctx->vpu;
	p = ctx->dec_params;

	switch (ctrl->id) {
	case V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY_ENABLE:
		ctx->enable_user_dpb = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY:
		ctx->n_user_dpb = ctrl->val;
		if (ctx->enable_user_dpb) {
			p->user_dpb_size = ctx->n_user_dpb;
		} else {
			switch (ctx->src_fmt.pixelformat) {
			case V4L2_PIX_FMT_VP8:
			case V4L2_PIX_FMT_VP9:
			case V4L2_PIX_FMT_AV1:
				p->user_dpb_size = 8;
				break;
			default:
				p->user_dpb_size = 0;
				break;
			}
		}
		/* Firmware can't update this value during the decoding */
		break;
	case SYNA_V4L2_CID_DEC_OPT_DOVI:
		vpu_ctrl->v4g_ext_cfg.opt.dvEn = ctrl->val;
		break;
	case SYNA_V4L2_CID_DEC_OPT_DOVI_VES_MODE:
		vpu_ctrl->v4g_ext_cfg.opt.dvMode = ctrl->val;
		break;
	case SYNA_V4L2_CID_DEC_OPT_HDR10_PLUS:
		vpu_ctrl->v4g_ext_cfg.opt.hdr10PlusEn = ctrl->val;
		break;
	case SYNA_V4L2_CID_DEC_OPT_NO_REORDER:
		vpu_ctrl->v4g_ext_cfg.opt.noReorder = ctrl->val;
		break;
	case SYNA_V4L2_CID_DEC_OPT_LOW_LATENCY:
		vpu_ctrl->v4g_ext_cfg.opt.lowLatencyEn = ctrl->val;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	vdpu_dbg(2, "[%s():%d]: id(0x%x),name(%s): val(%d), ret(%d)",
			 __func__, __LINE__, ctrl->id, ctrl->name, ctrl->val, ret);

	return ret;
}

static int vidioc_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
	struct syna_vcodec_ctx *ctx;
	struct syna_vpu_dev *vpu;
	struct syna_vpu_ctrl *vpu_ctrl;
	int ret = 0;

	ctx = container_of(ctrl->handler, struct syna_vcodec_ctx, ctrl_handler);
	vpu_ctrl = syna_vpu_get_ctrl_shm_buffer(ctx);
	vpu = ctx->vpu;

	switch (ctrl->id) {
	case V4L2_CID_MIN_BUFFERS_FOR_CAPTURE:
		ctrl->val = ctx->req_dpb_size;
		break;
	case SYNA_V4L2_CID_DEC_OPT_DOVI:
		ctrl->val = vpu_ctrl->v4g_ext_cfg.opt.dvEn;
		break;
	case SYNA_V4L2_CID_DEC_OPT_DOVI_VES_MODE:
		ctrl->val = vpu_ctrl->v4g_ext_cfg.opt.dvMode;
		break;
	case SYNA_V4L2_CID_DEC_OPT_HDR10_PLUS:
		ctrl->val = vpu_ctrl->v4g_ext_cfg.opt.hdr10PlusEn;
		break;
	case SYNA_V4L2_CID_DEC_OPT_NO_REORDER:
		ctrl->val = vpu_ctrl->v4g_ext_cfg.opt.noReorder;
		break;
	case SYNA_V4L2_CID_DEC_OPT_LOW_LATENCY:
		ctrl->val = vpu_ctrl->v4g_ext_cfg.opt.lowLatencyEn;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	vdpu_dbg(2, "[%s():%d]: id(0x%x),name(%s): val(%d), ret(%d)",
			 __func__, __LINE__, ctrl->id, ctrl->name, ctrl->val, ret);

	return ret;
}

static const struct v4l2_ctrl_ops syna_vpu_dec_ctrl_ops = {
	.s_ctrl = vidioc_vdec_s_ctrl,
	.g_volatile_ctrl = vidioc_g_volatile_ctrl,
};

static const struct v4l2_ctrl_config opt_dv_cfg = {
	.ops = &syna_vpu_dec_ctrl_ops,
	.id = SYNA_V4L2_CID_DEC_OPT_DOVI,
	.name = "SYNA dec option: dolby vision",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = 0,
	.max = 1,
	.step = 1,
	.def = 0,
};

static const struct v4l2_ctrl_config opt_dvMode_cfg = {
	.ops = &syna_vpu_dec_ctrl_ops,
	.id = SYNA_V4L2_CID_DEC_OPT_DOVI_VES_MODE,
	.name = "SYNA dec option: dolby vision mode",
	.type = V4L2_CTRL_TYPE_INTEGER,
	.min = 0,
	.max = 15,
	.step = 1,
	.def = 0,
};

static const struct v4l2_ctrl_config opt_hdr10Plus_cfg = {
	.ops = &syna_vpu_dec_ctrl_ops,
	.id = SYNA_V4L2_CID_DEC_OPT_HDR10_PLUS,
	.name = "SYNA dec option: hdr10 plus",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = 0,
	.max = 1,
	.step = 1,
	.def = 0,
};

static const struct v4l2_ctrl_config opt_noReorder_cfg = {
	.ops = &syna_vpu_dec_ctrl_ops,
	.id = SYNA_V4L2_CID_DEC_OPT_NO_REORDER,
	.name = "SYNA dec option: no reorder",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = 0,
	.max = 1,
	.step = 1,
	.def = 0,
};

static const struct v4l2_ctrl_config opt_lowLatency_cfg = {
	.ops = &syna_vpu_dec_ctrl_ops,
	.id = SYNA_V4L2_CID_DEC_OPT_LOW_LATENCY,
	.name = "SYNA dec option: low latency",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = 0,
	.max = 1,
	.step = 1,
	.def = 0,
};

int vpu_dec_ctrls_init(struct v4l2_ctrl_handler *handler)
{
	const struct v4l2_ctrl_ops *ops = &syna_vpu_dec_ctrl_ops;

	v4l2_ctrl_handler_init(handler, SYNA_DEC_MAX_CTRLS_HINT);

	v4l2_ctrl_new_std_menu(handler, ops, V4L2_CID_MPEG_VIDEO_H264_PROFILE,
			       V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10,
			       ~((1 << V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE) |
				 (1 << V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE) |
				 (1 << V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) |
				 (1 << V4L2_MPEG_VIDEO_H264_PROFILE_HIGH) |
				 (1 << V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10)),
			       V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE);

	v4l2_ctrl_new_std_menu(handler, ops, V4L2_CID_MPEG_VIDEO_HEVC_PROFILE,
			       V4L2_MPEG_VIDEO_HEVC_PROFILE_MAIN_10,
			       0, V4L2_MPEG_VIDEO_HEVC_PROFILE_MAIN);

	v4l2_ctrl_new_std_menu(handler, ops, V4L2_CID_MPEG_VIDEO_VP8_PROFILE,
			       V4L2_MPEG_VIDEO_VP8_PROFILE_3,
			       0, V4L2_MPEG_VIDEO_VP8_PROFILE_0);

	v4l2_ctrl_new_std_menu(handler, ops, V4L2_CID_MPEG_VIDEO_VP9_PROFILE,
			       V4L2_MPEG_VIDEO_VP9_PROFILE_2,
			       ~((1 << V4L2_MPEG_VIDEO_VP9_PROFILE_0) |
				 (1 << V4L2_MPEG_VIDEO_VP9_PROFILE_2)),
			       V4L2_MPEG_VIDEO_VP9_PROFILE_0);

	v4l2_ctrl_new_std(handler, NULL,
			  V4L2_CID_MIN_BUFFERS_FOR_CAPTURE, 1, 32, 1, 2);

	v4l2_ctrl_new_std(handler, NULL, V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY,
			  0, 16, 1, 0);
	v4l2_ctrl_new_std(handler, NULL,
			  V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY_ENABLE,
			  0, 1, 1, 0);

	v4l2_ctrl_new_custom(handler, &opt_dv_cfg, NULL);
	v4l2_ctrl_new_custom(handler, &opt_dvMode_cfg, NULL);
	v4l2_ctrl_new_custom(handler, &opt_hdr10Plus_cfg, NULL);
	v4l2_ctrl_new_custom(handler, &opt_noReorder_cfg, NULL);
	v4l2_ctrl_new_custom(handler, &opt_lowLatency_cfg, NULL);

	if (handler->error)
		return handler->error;

	v4l2_ctrl_handler_setup(handler);

	return 0;
}

void vpu_dec_ctrls_deinit(struct v4l2_ctrl_handler *handler)
{
	v4l2_ctrl_handler_free(handler);
}
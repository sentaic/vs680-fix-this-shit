// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/of_reserved_mem.h>
#include <linux/of_graph.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/moduleparam.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/delay.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mc.h>
#include <media/videobuf2-dma-contig.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-mediabus.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>
#include <linux/limits.h>

#include "camera_isp_driver.h"
#include <media/v4l2-ctrls.h>
#include "camera_v4l2_common.h"
#include "csipipe_api.h"
#include "isp_shm.h"
#include "csipipe_pvt.h"
#include "camera_isp_wb_sysfs.h"

#define MAX_SENSOR_WIDTH  1920
#define MAX_SENSOR_HEIGHT 1080

static const char *isp_clock_list[] = {
	"aviopclk",
};

/* Selection method enumeration */
enum selection_method {
	ISP_SCALABLE = 1,   /* integral downscale is possible from a larger sensor mode */
	EXACT_MATCH = 2,    /* found matching sensor mode; no scaling needed */
	NEAREST_MATCH = 3   /* fallback when exact/scale not available; choose closest sensor mode */
};

/* Helper: Free dynamically allocated sensor mode cache */
static void free_sensor_modes(struct camera_isp_dev *isp_dev)
{
	int i;

	for (i = 0; i < isp_dev->num_cached_codes; i++)
		kfree(isp_dev->cached_modes[i]);
	kfree(isp_dev->cached_modes);
	kfree(isp_dev->num_resolutions);
	isp_dev->cached_modes = NULL;
	isp_dev->num_resolutions = NULL;
	isp_dev->num_cached_codes = 0;
}

/* Helper: Enumerate all sensor modes into a 2D array [code_idx][res_idx] */
static int enumerate_sensor_modes(struct camera_isp_dev *isp_dev,
			struct v4l2_subdev *subdev,
			struct v4l2_subdev_state *sd_state)
{
	struct v4l2_subdev_mbus_code_enum code_enum = { .which = V4L2_SUBDEV_FORMAT_ACTIVE };
	struct v4l2_subdev_frame_size_enum fse = { .which = V4L2_SUBDEV_FORMAT_ACTIVE, .pad = 0 };
	struct sensor_mode **modes = NULL, **tmp_codes;
	int *num_res = NULL, *tmp_res;
	int n_codes = 0, i, ret;

	free_sensor_modes(isp_dev);

	for (code_enum.index = 0; ; code_enum.index++) {
		struct sensor_mode *code_res = NULL, *tmp_mode;
		int n_res = 0;

		ret = v4l2_subdev_call(subdev, pad, enum_mbus_code, sd_state, &code_enum);
		if (ret)
			break;

		fse.code = code_enum.code;
		for (fse.index = 0; ; fse.index++) {
			ret = v4l2_subdev_call(subdev, pad, enum_frame_size, sd_state, &fse);
			if (ret)
				break;
			if (fse.max_width > MAX_SENSOR_WIDTH || fse.max_height > MAX_SENSOR_HEIGHT)
				continue;
			if ((isp_dev->max_sensor_width &&
			     fse.max_width > isp_dev->max_sensor_width) ||
			    (isp_dev->max_sensor_height &&
			     fse.max_height > isp_dev->max_sensor_height))
				continue;
			tmp_mode = krealloc(code_res, (n_res + 1) * sizeof(*code_res), GFP_KERNEL);
			if (!tmp_mode) {
				kfree(code_res);
				goto err;
			}
			code_res = tmp_mode;
			code_res[n_res++] = (struct sensor_mode){
				.code   = code_enum.code,
				.width  = fse.max_width,
				.height = fse.max_height
			};
		}

		if (!n_res) {
			kfree(code_res);
			continue;
		}

		tmp_codes = krealloc(modes, (n_codes + 1) * sizeof(*modes), GFP_KERNEL);
		if (!tmp_codes) {
			kfree(code_res);
			goto err;
		}
		modes = tmp_codes;
		tmp_res = krealloc(num_res, (n_codes + 1) * sizeof(*num_res), GFP_KERNEL);
		if (!tmp_res) {
			kfree(code_res);
			goto err;
		}
		num_res = tmp_res;
		modes[n_codes]   = code_res;
		num_res[n_codes] = n_res;
		n_codes++;
	}

	isp_dev->cached_modes    = modes;
	isp_dev->num_resolutions = num_res;
	isp_dev->num_cached_codes = n_codes;
	return n_codes > 0 ? 0 : -ENODEV;
err:
	for (i = 0; i < n_codes; i++)
		kfree(modes[i]);
	kfree(modes);
	kfree(num_res);
	return -ENOMEM;
}

/* METHOD 2 - Check for exact code+resolution match */
static const struct sensor_mode *find_exact_match(struct camera_isp_dev *isp_dev,
			u32 requested_width, u32 requested_height,
			u32 requested_code)
{
	int code_idx, res_idx;

	for (code_idx = 0; code_idx < isp_dev->num_cached_codes; code_idx++) {
		u32 first_code =
			isp_dev->cached_modes[code_idx][CAMERA_ISP_FIRST_RES_IDX].code;
		if (first_code != requested_code)
			continue;
		for (res_idx = 0; res_idx < isp_dev->num_resolutions[code_idx]; res_idx++) {
			const struct sensor_mode *m = &isp_dev->cached_modes[code_idx][res_idx];

			if (m->width == requested_width && m->height == requested_height)
				return m;
		}
	}
	return NULL;
}

static int calculate_scaling_factor(u32 in_w, u32 in_h, u32 out_w, u32 out_h)
{
	u32 scale_x;
	u32 scale_y;

	if (in_w >= out_w && in_h >= out_h) {
		scale_x = in_w / out_w;
		scale_y = in_h / out_h;

		/* Check for perfect integral scaling */
		if (scale_x == scale_y &&
				(in_w % out_w) == 0 && (in_h % out_h) == 0)
			return scale_x;

	}

	return -1;
}

/* METHOD 1 - Find ISP scalable mode, preferring requested_code on tie */
static const struct sensor_mode *find_scalable_mode(struct camera_isp_dev *isp_dev,
		u32 requested_width, u32 requested_height,
		u32 requested_code,
		u32 *out_scale_factor)
{
	const struct sensor_mode *best = NULL;
	u32 best_scale_factor = UINT_MAX;
	int code_idx, res_idx, sf;

	/* Skip scaling when bypass_isp_enabled = 1 */
	if (isp_dev->bypass_isp_enabled)
		return NULL;

	for (code_idx = 0; code_idx < isp_dev->num_cached_codes; code_idx++) {
		for (res_idx = 0; res_idx < isp_dev->num_resolutions[code_idx]; res_idx++) {
			const struct sensor_mode *m = &isp_dev->cached_modes[code_idx][res_idx];

			if (m->width < requested_width || m->height < requested_height)
				continue;

			sf = calculate_scaling_factor(m->width, m->height,
					requested_width, requested_height);

			if (sf > 0 && ((u32)sf < best_scale_factor ||
			    ((u32)sf == best_scale_factor && m->code == requested_code))) {
				best_scale_factor = (u32)sf;
				best = m;
			}
		}
	}

	if (best)
		*out_scale_factor = best_scale_factor;

	return best;
}

static void camera_isp_reset_controls(struct camera_isp_dev *isp_dev)
{
	struct v4l2_ctrl *ctrl;

	ctrl = v4l2_ctrl_find(&isp_dev->ctrl_handler, V4L2_CID_USER_WB_ENABLE);
	if (ctrl)
		v4l2_ctrl_s_ctrl(ctrl, ctrl->default_value);

	ctrl = v4l2_ctrl_find(&isp_dev->ctrl_handler, V4L2_CID_USER_DISABLE_SCALE_CROP);
	if (ctrl)
		v4l2_ctrl_s_ctrl(ctrl, ctrl->default_value);
}

/**
 * camera_isp_select_optimal_sensor_mode - Select optimal sensor mode
 * @requested_code: Preferred media bus format code (e.g. RGB888_1X24 or YUYV8_2X8)
 * @out_method: Returns which method was used for selection
 */
static int camera_isp_select_optimal_sensor_mode(struct camera_isp_dev *isp_dev,
			struct v4l2_subdev *subdev,
			struct v4l2_subdev_state *sd_state,
			u32 requested_width,
			u32 requested_height,
			u32 requested_code,
			struct v4l2_subdev_format *selected_fmt,
			enum selection_method *out_method)
{
	const struct sensor_mode *selected = NULL;
	u32 scale_factor = 0;
	enum selection_method method = NEAREST_MATCH;
	int ret;

	/* Enumerate sensor modes on first call */
	if (isp_dev->num_cached_codes == 0) {
		ret = enumerate_sensor_modes(isp_dev, subdev, sd_state);
		if (ret)
			return ret;
	}

	/* METHOD 1: ISP scalable */
	selected = find_scalable_mode(isp_dev,
			requested_width, requested_height, requested_code, &scale_factor);
	if (selected) {
		method = ISP_SCALABLE;
		isp_dev->scale_factor = scale_factor;
		goto mode_selected;
	}

	/* METHOD 2: Exact match */
	selected = find_exact_match(isp_dev,
			requested_width, requested_height, requested_code);
	if (selected) {
		method = EXACT_MATCH;
		isp_dev->scale_factor = 1; /* No scaling for exact match */
		goto mode_selected;
	}

	/* METHOD 3: bypass mode — resolution-only match.
	 * Prefer UYVY8_1X16 for YUV outputs, RGB888_1X24 for RGB outputs.
	 */
	if (isp_dev->bypass_isp_enabled) {
		bool want_yuv = (requested_code != MEDIA_BUS_FMT_RGB888_1X24);
		bool done = false;
		int code_idx, res_idx;

		for (code_idx = 0; code_idx < isp_dev->num_cached_codes && !done; code_idx++) {
			for (res_idx = 0; res_idx < isp_dev->num_resolutions[code_idx]; res_idx++) {
				const struct sensor_mode *m =
					&isp_dev->cached_modes[code_idx][res_idx];

				if (m->width != requested_width || m->height != requested_height)
					continue;
				if (!selected)
					selected = m;
				if (want_yuv && m->code == MEDIA_BUS_FMT_UYVY8_1X16) {
					selected = m;
					done = true;
					break;
				}
				if (!want_yuv && m->code == MEDIA_BUS_FMT_RGB888_1X24) {
					selected = m;
					done = true;
					break;
				}
			}
		}
		if (!selected) {
			camera_isp_reset_controls(isp_dev);
			dev_err(isp_dev->dev,
				"bypass_isp_enabled: no sensor mode for %ux%u\n",
				requested_width, requested_height);
			return -EINVAL;
		}
		method = NEAREST_MATCH;
		isp_dev->scale_factor = 1;
		goto mode_selected;
	}

mode_selected:
	if (!selected)
		return -EINVAL;

	/* Configure and apply selected format */
	selected_fmt->which = V4L2_SUBDEV_FORMAT_ACTIVE;
	selected_fmt->pad = 0;
	selected_fmt->format.width = selected->width;
	selected_fmt->format.height = selected->height;
	selected_fmt->format.code = selected->code;
	selected_fmt->format.field = V4L2_FIELD_NONE;
	selected_fmt->format.colorspace = V4L2_COLORSPACE_SRGB;

	/* Return which method was selected */
	if (out_method)
		*out_method = method;

	return 0;
}

/**
 * camera_isp_get_sensor_resolution_and_program - Get resolution from sensor and program CSI
 * @isp_dev: ISP device pointer
 * @subdev: CSI subdevice pointer
 * @sd_state: subdevice state
 * @sd_fmt: Format structure (input: requested, output: actual)
 * Returns: 0 on success, negative error code on failure
 */
static int camera_isp_get_sensor_resolution_and_program(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *subdev,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_format *sd_fmt)
{
	struct media_pad *sensor_pad;
	struct v4l2_subdev *sensor_subdev;
	struct v4l2_subdev_format sensor_fmt = {0};
	enum selection_method method;
	u32 requested_width = sd_fmt->format.width;
	u32 requested_height = sd_fmt->format.height;
	int ret;

	/* Find the sensor subdevice connected to CSI */
	sensor_pad = media_pad_remote_pad_first(&subdev->entity.pads[0]);
	if (!sensor_pad || !is_media_entity_v4l2_subdev(sensor_pad->entity))
		return -ENODEV;

	sensor_subdev = media_entity_to_v4l2_subdev(sensor_pad->entity);
	if (!sensor_subdev)
		return -ENODEV;

	/* Resolution + format-code selection with method information */
	ret = camera_isp_select_optimal_sensor_mode(isp_dev, sensor_subdev, sd_state,
				requested_width, requested_height,
				sd_fmt->format.code,
				&sensor_fmt, &method);
	if (ret)
		return ret;

	/* Always update sd_fmt with selected sensor resolution */
	/* This is what goes to CSI and sensor */
	sd_fmt->format.width = sensor_fmt.format.width;
	sd_fmt->format.height = sensor_fmt.format.height;
	sd_fmt->format.code = sensor_fmt.format.code;

	return 0;
}

static int camera_isp_ctrl_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct camera_isp_dev *isp_dev = container_of(ctrl->handler,
			struct camera_isp_dev, ctrl_handler);
	struct isp_ctrl wb_ctrl;
	int ret = 0;


	switch (ctrl->id) {
	case V4L2_CID_USER_WB_ENABLE:
		/* Update ISP device configuration */
		isp_dev->wb_config.wb_en = ctrl->val;

		/* Apply to active pipe only */
		if (isp_dev->active_pipe_id >= 0 && isp_dev->active_pipe_id < MAX_PL) {
			if (isp_dev->pipe[isp_dev->active_pipe_id]) {
				CSI_PL_CTX_t *ctx =
					(CSI_PL_CTX_t *)isp_dev->pipe[isp_dev->active_pipe_id];
				ctx->wb_en = ctrl->val;

				/* Configure white balance module */
				wb_ctrl.id = CID_WB_CONFIG;
				wb_ctrl.handler = ctx;
				wb_ctrl.cfg = &isp_dev->wb_config;
				ret = wb_s_ctrl(&wb_ctrl);
				if (ret) {
					dev_err(isp_dev->dev, "Failed to configure WB module: %d\n",
							ret);
					break;
				}

				/* Enable/disable white balance module */
				wb_ctrl.id = CID_WB_ENABLE;
				wb_ctrl.handler = ctx;
				wb_ctrl.cfg = &isp_dev->wb_config;
				ret = wb_s_ctrl(&wb_ctrl);
				if (ret) {
					dev_err(isp_dev->dev, "Failed to %s WB module: %d\n",
						   ctrl->val ? "enable" : "disable", ret);
					break;
				}

				dev_info(isp_dev->dev, "WB module %s for pipe %d\n",
						ctrl->val ? "enabled" : "disabled",
						isp_dev->active_pipe_id);
			}
		}
		break;
		/* Store the bypass_isp_enabled flag in isp_dev
		 * select_sensor_mode() will check this before allowing scaling
		 */
	case V4L2_CID_USER_DISABLE_SCALE_CROP:
		isp_dev->bypass_isp_enabled = !!ctrl->val;
		dev_info(isp_dev->dev, "bypass_isp_enabled=%d\n", isp_dev->bypass_isp_enabled);
		break;
	default:
		dev_err(isp_dev->dev, "Unsupported control ID: 0x%x\n", ctrl->id);
		return -EINVAL;
	}
	return ret;
}

static int camera_isp_ctrl_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
	struct camera_isp_dev *isp_dev = container_of(ctrl->handler,
			struct camera_isp_dev, ctrl_handler);
	switch (ctrl->id) {
	case V4L2_CID_USER_WB_ENABLE:
		ctrl->val = isp_dev->wb_config.wb_en;
		break;
	/* Return current value of bypass_isp_enabled flag */
	case V4L2_CID_USER_DISABLE_SCALE_CROP:
		ctrl->val = isp_dev->bypass_isp_enabled ? 1 : 0;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static const struct v4l2_ctrl_ops camera_isp_ctrl_ops = {
	.s_ctrl          = camera_isp_ctrl_s_ctrl,
	.g_volatile_ctrl = camera_isp_ctrl_g_volatile_ctrl,
};

static const struct v4l2_ctrl_config camera_isp_ctrls[] = {
	/* WB enable: 0=disable white balance  1=enable white balance */
	{
		.ops   = &camera_isp_ctrl_ops,
		.id    = V4L2_CID_USER_WB_ENABLE,
		.type  = V4L2_CTRL_TYPE_BOOLEAN,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.name  = "wb_enable",
		.min   = 0,
		.max   = 1,
		.step  = 1,
		.def   = 0,
	},
	/* Disable scale+crop: 0=allow scaling  1=only exact sensor resolution */
	{
		.ops   = &camera_isp_ctrl_ops,
		.id    = V4L2_CID_USER_DISABLE_SCALE_CROP,
		.type  = V4L2_CTRL_TYPE_BOOLEAN,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.name  = "bypass_isp_enabled",
		.min   = 0,
		.max   = 1,
		.step  = 1,
		.def   = 0,
	},
};

static int camera_isp_link_setup(struct media_entity *entity,
		const struct media_pad *local,
		const struct media_pad *remote,
		u32 flags)
{
	pr_info("[ENTITY_OPS] link_setup called for %s pad %u -> %s pad %u\n",
			entity->name, local->index, remote->entity->name, remote->index);
	return 0;
}

static const struct media_entity_operations camera_isp_entity_ops = {
	.link_setup = camera_isp_link_setup,
	.link_validate = v4l2_subdev_link_validate,
	.get_fwnode_pad = v4l2_subdev_get_fwnode_pad_1_to_1,
};

static int camera_isp_notifier_bound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd, struct v4l2_async_connection *asc)
{
	struct camera_isp_dev *isp_dev = container_of(notifier, struct camera_isp_dev, notifier);
	struct device *dev = isp_dev->dev;
	struct fwnode_handle *ep = NULL;
	struct v4l2_fwnode_link link;
	struct media_entity *source, *sink;
	unsigned int source_pad, sink_pad;
	int ret = 0;

	while (1) {
		ep = fwnode_graph_get_next_endpoint(sd->fwnode, ep);
		if (!ep)
			break;
		ret = v4l2_fwnode_parse_link(ep, &link);
		if (ret < 0) {
			dev_err(dev, "failed to parse link for %pOF: %d\n", to_of_node(ep), ret);
			continue;
		}
		if (sd->entity.pads[link.local_port].flags == MEDIA_PAD_FL_SINK)
			continue;
		source	   = &sd->entity;
		sink	   = &isp_dev->sd.entity;
		source_pad = link.local_port;
		sink_pad   = link.remote_port;
		v4l2_fwnode_put_link(&link);
		if (!source || !sink) {
			dev_err(dev, "media_create_pad_link: source or sink entity is NULL!\n");
			ret = -EINVAL;
			break;
		}
		if (!source->pads || !sink->pads) {
			dev_err(dev, "media_create_pad_link: source or sink pads are NULL!\n");
			ret = -EINVAL;
			break;
		}
		dev_info(sd->dev,
				"%s: linking %s source_pad %d flags %ld and %s sink_pad %d flags %ld\n",
				__func__, source->name, source_pad, source->pads[source_pad].flags,
				sink->name, sink_pad, sink->pads[sink_pad].flags);
		ret = media_create_pad_link(source, source_pad, sink, sink_pad,
				MEDIA_LNK_FL_ENABLED);
		if (ret) {
			dev_err(dev, "failed to create %s:%u -> %s:%u link\n",
				source->name, source_pad, sink->name, sink_pad);
			break;
		}
	}
	fwnode_handle_put(ep);
	return ret;
}

static void camera_isp_notifier_unbound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd, struct v4l2_async_connection *asc)
{
	return;
}

static const struct v4l2_async_notifier_operations camera_isp_notify_ops = {
	.bound = camera_isp_notifier_bound,
	.unbind = camera_isp_notifier_unbound,
};

/* Supported media bus formats for main path */
struct camera_isp_mbus_fmt camera_isp_mp_fmts[] = {
	{ .code = MEDIA_BUS_FMT_YUYV8_2X8 },   /* NV16 */
	{ .code = MEDIA_BUS_FMT_YUYV8_1_5X8 }, /* NV12 */
	{ .code = MEDIA_BUS_FMT_YUYV8_1X16 },  /* YUYV */
	{ .code = MEDIA_BUS_FMT_UYVY8_1X16 },  /* UYVY */
	{ .code = MEDIA_BUS_FMT_YUV8_1X24 },   /* YUV444 */
	{ .code = MEDIA_BUS_FMT_RGB888_1X24 },  /* RGB24 */
	{ .code = MEDIA_BUS_FMT_BGR888_1X24 },  /* BGR24 */
	{ .code = MEDIA_BUS_FMT_SBGGR8_1X8 },  /*  BGGR 8*/
	{ .code = MEDIA_BUS_FMT_SGRBG8_1X8 },  /*  GRBG 8*/
	{ .code = MEDIA_BUS_FMT_SGRBG10_1X10},	/* GRBG 10 */
	{ .code = MEDIA_BUS_FMT_SGBRG10_1X10},	/* GBRG 10 */
	{ .code = MEDIA_BUS_FMT_SRGGB10_1X10},	/* RGGB 10 */
	{ .code = MEDIA_BUS_FMT_SBGGR10_1X10},	/* BGGR 10 */
};

/* Supported media bus formats for self path */
struct camera_isp_mbus_fmt camera_isp_sp_fmts[] = {
	{ .code = MEDIA_BUS_FMT_YUYV8_2X8 },   /* NV16 */
	{ .code = MEDIA_BUS_FMT_YUYV8_1_5X8 }, /* NV12 */
	{ .code = MEDIA_BUS_FMT_YUYV8_1X16 },  /* YUYV */
	{ .code = MEDIA_BUS_FMT_UYVY8_1X16 },  /* UYVY */
	{ .code = MEDIA_BUS_FMT_YUV8_1X24 },   /* YUV444 */
	{ .code = MEDIA_BUS_FMT_RGB888_1X24 },  /* RGB24 */
	{ .code = MEDIA_BUS_FMT_BGR888_1X24 },  /* BGR24 */
	{ .code = MEDIA_BUS_FMT_SBGGR8_1X8 },  /*  BGGR 8*/
	{ .code = MEDIA_BUS_FMT_SGRBG8_1X8 },  /*  GRBG 8*/
	{ .code = MEDIA_BUS_FMT_SGRBG10_1X10},	/* GRBG 10 */
	{ .code = MEDIA_BUS_FMT_SGBRG10_1X10},	/* GBRG 10 */
	{ .code = MEDIA_BUS_FMT_SRGGB10_1X10},	/* RGGB 10 */
	{ .code = MEDIA_BUS_FMT_SBGGR10_1X10},	/* RGGR 10 */
};

static int camera_isp_subscribe_event(struct v4l2_subdev *sd,
				struct v4l2_fh *fh,
				struct v4l2_event_subscription *sub)
{
	switch (sub->type) {
	case V4L2_EVENT_FRAME_SYNC:
		return v4l2_event_subscribe(fh, sub, 0, NULL);
	default:
		return -EINVAL;
	}
}

static int camera_isp_buf_queue(struct v4l2_subdev *sd, void *arg)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	struct camera_pad_buf *pad_buf = (struct camera_pad_buf *)arg;
	unsigned long flags;
	CSI_PL_CTX_t *ctx;

	if (!pad_buf || pad_buf->pad == CAMERA_ISP_PAD_SINK ||
		pad_buf->pad >= CAMERA_ISP_PAD_NR) {
		dev_err(isp_dev->dev, "%s: invalid pad %u for buf_queue\n",
			__func__, pad_buf ? pad_buf->pad : (u32)-1);
		return -EINVAL;
	}

	if (!isp_dev->pipe[pad_buf->pad - 1]) {
		dev_err(isp_dev->dev, "%s: pipeline not initialized for pad %u\n",
			__func__, pad_buf->pad);
		return -EINVAL;
	}

	if (!pad_buf->buf) {
		dev_err(isp_dev->dev, "%s: NULL buffer for pad %u\n", __func__, pad_buf->pad);
		return -EINVAL;
	}

	/* If first plane dma address looks invalid */
	if (!pad_buf->buf->planes[0].dma_addr)
		dev_warn(isp_dev->dev, "%s: pad %u plane0 dma_addr is 0\n", __func__, pad_buf->pad);

	ctx = (CSI_PL_CTX_t *)isp_dev->pipe[pad_buf->pad - 1];

	spin_lock_irqsave(&ctx->buf.lock, flags);
	list_add_tail(&pad_buf->buf->list, &ctx->buf.queue);
	pr_debug("Add addr: 0x%x ctx: %d\n", pad_buf->buf->planes[0].dma_addr, ctx->id);
	spin_unlock_irqrestore(&ctx->buf.lock, flags);

	return 0;
}

static void camera_isp_csi_power(struct camera_isp_dev *isp_dev, int on)
{
	struct media_pad *pad;
	struct v4l2_subdev *subdev;
	int ret;

	pad = media_pad_remote_pad_first(&isp_dev->pads[CAMERA_ISP_PAD_SINK]);
	if (pad && is_media_entity_v4l2_subdev(pad->entity)) {
		subdev = media_entity_to_v4l2_subdev(pad->entity);
		ret = v4l2_subdev_call(subdev, core, s_power, on);
		if (ret)
			dev_err(isp_dev->dev, "CSI s_power(%d) failed: %d\n", on, ret);
	}
}

static void camera_isp_csi_try_power_on(struct camera_isp_dev *isp_dev, int on)
{
	unsigned long flags;
	bool call_csi_power;

	spin_lock_irqsave(&isp_dev->isr_lock, flags);
	call_csi_power = (isp_dev->streaming == 0);
	spin_unlock_irqrestore(&isp_dev->isr_lock, flags);

	if (call_csi_power)
		camera_isp_csi_power(isp_dev, on);
}

static int camera_isp_s_stream(struct v4l2_subdev *sd, void *arg)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	struct camera_pad_stream_status *pad_stream = (struct camera_pad_stream_status *)arg;
	int ret = 0;
	int req_pad = pad_stream->pad - 1;

	if (!pad_stream || pad_stream->pad == CAMERA_ISP_PAD_SINK ||
	    pad_stream->pad >= CAMERA_ISP_PAD_NR) {
		dev_err(isp_dev->dev, "%s: invalid pad %u for s_stream\n",
			__func__, pad_stream ? pad_stream->pad : (u32)-1);
		return -EINVAL;
	}

	if (!isp_dev->pipe[pad_stream->pad - 1]) {
		dev_err(isp_dev->dev,
			"%s: pipeline not initialized for pad %u\n", __func__,
			pad_stream->pad);
		return -EINVAL;
	}

	//TODO pipe index calculation
	if (pad_stream->status) {
		isp_dev->active_pipe_id = req_pad;

		ret = v4l2_ctrl_handler_setup(sd->ctrl_handler);
		if (ret)
			return ret;

		if (isp_dev->bypass_isp_enabled) {
			if (isp_dev->scale_factor > 1) {
				dev_err(isp_dev->dev,
					"bypass_isp_enabled=1 but scale_factor=%u, use exact sensor resolution\n",
					isp_dev->scale_factor);
				return -EINVAL;
			}
			if (isp_dev->pad_data[req_pad + 1].is_cropping_enable) {
				dev_err(isp_dev->dev,
					"bypass_isp_enabled=1 but crop is active, remove crop or clear flag\n");
				return -EINVAL;
			}
		}

		camera_isp_csi_try_power_on(isp_dev, 1);
		CSI_PIPE_Start(isp_dev->pipe[req_pad]);
	} else {
		isp_dev->pad_data[req_pad + 1].is_cropping_enable = false;
		CSI_PIPE_Stop(isp_dev->pipe[req_pad]);
		camera_isp_csi_try_power_on(isp_dev, 0);

		if (isp_dev->pipe[req_pad] != NULL) {
			CSI_PIPE_Destroy(isp_dev->pipe[req_pad]);
			isp_dev->pipe[req_pad] = NULL;
			isp_dev->pipeline_ready[req_pad] = false;
		}

		camera_isp_reset_controls(isp_dev);
	}

	return ret;
}

static int camera_isp_ioctl_g_ctrl(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *sd, void *arg)
{
	struct camera_pad_control *pad_ctrl = (struct camera_pad_control *)arg;
	struct v4l2_ctrl *ctrl;

	if (!pad_ctrl || !pad_ctrl->control) {
		dev_err(isp_dev->dev, "Invalid G_CTRL parameters\n");
		return -EINVAL;
	}

	ctrl = v4l2_ctrl_find(&isp_dev->ctrl_handler, pad_ctrl->control->id);
	if (!ctrl) {
		dev_err(isp_dev->dev, "Control 0x%x not found in ISP handler\n",
				pad_ctrl->control->id);
		return -EINVAL;
	}
	pad_ctrl->control->value = ctrl->cur.val;
	return 0;
}
static int camera_isp_ioctl_s_ctrl(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *sd, void *arg)
{
	struct camera_pad_control *pad_ctrl = (struct camera_pad_control *)arg;

	if (!pad_ctrl || !pad_ctrl->control) {
		dev_err(isp_dev->dev, "Invalid S_CTRL parameters\n");
		return -EINVAL;
	}

	struct v4l2_ctrl *ctrl = v4l2_ctrl_find(&isp_dev->ctrl_handler,
			pad_ctrl->control->id);
	if (!ctrl) {
		dev_err(isp_dev->dev, "Control 0x%x not found in ISP handler\n",
				pad_ctrl->control->id);
		return -EINVAL;
	}
	return v4l2_ctrl_s_ctrl(ctrl, pad_ctrl->control->value);
}
static int camera_isp_ioctl_g_ext_ctrls(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *sd, void *arg)
{
	struct camera_pad_ext_controls *pad_ext_ctrls = arg;

	return v4l2_g_ext_ctrls(&isp_dev->ctrl_handler, sd->devnode,
			sd->v4l2_dev->mdev, pad_ext_ctrls->ext_controls);
}
static int camera_isp_ioctl_s_ext_ctrls(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *sd, void *arg)
{
	struct camera_pad_ext_controls *pad_ext_ctrls = arg;

	return v4l2_s_ext_ctrls(NULL, &isp_dev->ctrl_handler, sd->devnode,
			sd->v4l2_dev->mdev, pad_ext_ctrls->ext_controls);
}
static int camera_isp_ioctl_try_ext_ctrls(struct camera_isp_dev *isp_dev,
		struct v4l2_subdev *sd, void *arg)
{
	struct camera_pad_ext_controls *pad_ext_ctrls = arg;

	return v4l2_try_ext_ctrls(&isp_dev->ctrl_handler, sd->devnode,
			sd->v4l2_dev->mdev, pad_ext_ctrls->ext_controls);
}

static int camera_isp_ioctl_queryctrl(struct camera_isp_dev *isp_dev, void *arg)
{
	struct camera_pad_queryctrl *pad_query_ctrl = arg;

	return v4l2_queryctrl(&isp_dev->ctrl_handler, pad_query_ctrl->query_ctrl);
}

static int camera_isp_ioctl_query_ext_ctrl(struct camera_isp_dev *isp_dev, void *arg)
{
	struct camera_pad_query_ext_ctrl *pad_query_ext_ctrl = arg;

	return v4l2_query_ext_ctrl(&isp_dev->ctrl_handler, pad_query_ext_ctrl->query_ext_ctrl);
}

static int camera_isp_ioctl_querymenu(struct camera_isp_dev *isp_dev, void *arg)
{
	struct camera_pad_querymenu *pad_querymenu = arg;

	return v4l2_querymenu(&isp_dev->ctrl_handler, pad_querymenu->querymenu);
}

static int camera_isp_querycap(struct v4l2_subdev *sd, void *arg)
{
	struct v4l2_capability *cap = (struct v4l2_capability *)arg;

	strncpy(cap->driver, sd->name, sizeof(cap->driver));
	strncpy(cap->card, sd->name, sizeof(cap->card));
	snprintf(cap->bus_info, sizeof(cap->bus_info),
			"platform:%s", sd->name);

	return 0;
}

static long camera_isp_priv_ioctl(struct v4l2_subdev *sd,
		unsigned int cmd, void *arg)
{
	int ret = -EINVAL;
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);

	if (!isp_dev)
		return -ENODEV;

	switch (cmd) {
	case CAMERA_ISP_IOC_QUERYCAP:
		return camera_isp_querycap(sd, arg);
	case CAMERA_PAD_G_CTRL:
		ret = camera_isp_ioctl_g_ctrl(isp_dev, sd, arg);
		break;
	case CAMERA_PAD_S_CTRL:
		ret = camera_isp_ioctl_s_ctrl(isp_dev, sd, arg);
		break;
	case CAMERA_PAD_G_EXT_CTRLS:
		ret = camera_isp_ioctl_g_ext_ctrls(isp_dev, sd, arg);
		break;
	case CAMERA_PAD_S_EXT_CTRLS:
		ret = camera_isp_ioctl_s_ext_ctrls(isp_dev, sd, arg);
		break;
	case CAMERA_PAD_TRY_EXT_CTRLS:
		ret = camera_isp_ioctl_try_ext_ctrls(isp_dev, sd, arg);
		break;
	case CAMERA_PAD_QUERYCTRL:
		ret = camera_isp_ioctl_queryctrl(isp_dev, arg);
		break;
	case CAMERA_PAD_QUERY_EXT_CTRL:
		ret = camera_isp_ioctl_query_ext_ctrl(isp_dev, arg);
		break;
	case CAMERA_PAD_QUERYMENU:
		ret = camera_isp_ioctl_querymenu(isp_dev, arg);
		break;
	case CAMERA_PAD_S_STREAM:
		ret = camera_isp_s_stream(sd, arg);
		break;
	case CAMERA_PAD_BUF_QUEUE:
		ret = camera_isp_buf_queue(sd, arg);
		break;
	default:
		dev_err(isp_dev->dev, "Unsupported ioctl: 0x%x\n", cmd);
		ret = -ENOTTY;
	}

	return ret;
}

static const struct v4l2_subdev_core_ops camera_isp_core_ops = {
	.ioctl = camera_isp_priv_ioctl,
	.subscribe_event = camera_isp_subscribe_event,
	.unsubscribe_event = v4l2_event_subdev_unsubscribe,
};

/* V4L2 subdev selection operations */
static int camera_isp_get_selection(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_selection *sel)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	struct camera_isp_pad_data *pad_data;
	u32 in_w, in_h;

	if (sel->pad >= CAMERA_ISP_PAD_NR) {
		dev_err(isp_dev->dev, "Invalid pad %d for get_selection\n", sel->pad);
		return -EINVAL;
	}

	/* Only support crop on source pads */
	if (sel->pad == CAMERA_ISP_PAD_SINK) {
		dev_err(isp_dev->dev, "Selection not supported on sink pad\n");
		return -EINVAL;
	}
	pad_data = &isp_dev->pad_data[sel->pad];

	/* Use sink format as input bounds when ctx is not ready */
	in_w = isp_dev->formats[CAMERA_ISP_PAD_SINK].width;
	in_h = isp_dev->formats[CAMERA_ISP_PAD_SINK].height;
	if (!in_w || !in_h) {
		in_w = CAMERA_ISP_DEFAULT_WIDTH;
		in_h = CAMERA_ISP_DEFAULT_HEIGHT;
	}

	switch (sel->target) {
	case V4L2_SEL_TGT_CROP_DEFAULT:
	case V4L2_SEL_TGT_CROP_BOUNDS:
		/* Full input frame bounds */
		sel->r.left = 0;
		sel->r.top = 0;
		sel->r.width = in_w;
		sel->r.height = in_h;
		break;

	case V4L2_SEL_TGT_CROP:
		/* Current rectangle */
		if (pad_data->is_cropping_enable)
			sel->r = pad_data->r;
		break;

	default:
		dev_dbg(isp_dev->dev, "Unsupported selection target %d LINE: %d\n",
			sel->target, __LINE__);
		return -EINVAL;
	}

	dev_dbg(isp_dev->dev, "get_selection pad %d target %d: (%d,%d)/%dx%d\n",
			sel->pad, sel->target, sel->r.left, sel->r.top,
			sel->r.width, sel->r.height);

	return 0;
}

static int camera_isp_check_cropping_enable(struct v4l2_subdev_selection *sel)
{
	int x = sel->r.left;
	int y = sel->r.top;
	int width = sel->r.width;
	int height = sel->r.height;

	if (x == 0 && width == CAMERA_ISP_DEFAULT_WIDTH &&
			y == 0 && height == CAMERA_ISP_DEFAULT_HEIGHT)
		return 1;

	return 0;
}

static int camera_isp_set_selection(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_selection *sel)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	int ret = 0;

	if (sel->pad >= CAMERA_ISP_PAD_NR) {
		dev_err(isp_dev->dev, "Invalid pad %d for set_selection\n", sel->pad);
		return -EINVAL;
	}

	/* Only support crop on source pads */
	if (sel->pad == CAMERA_ISP_PAD_SINK) {
		dev_err(isp_dev->dev, "Selection not supported on sink pad\n");
		return -EINVAL;
	}

	if (sel->target != V4L2_SEL_TGT_CROP) {
		dev_err(isp_dev->dev, "Unsupported selection target %d\n",
			sel->target);
		return -EINVAL;
	}

	ret = camera_isp_check_cropping_enable(sel);

	if (isp_dev->bypass_isp_enabled && !ret) {
		dev_err(isp_dev->dev,
			"bypass_isp_enabled=1 does not allow crop selection\n");
		return -EINVAL;
	}

	if (!ret) {
		dev_dbg(isp_dev->dev, "cropping\n");
		isp_dev->pad_data[sel->pad].is_cropping_enable = true;
		isp_dev->pad_data[sel->pad].r = sel->r;
		isp_dev->pad_data[sel->pad].target = sel->target;
	} else {
		dev_dbg(isp_dev->dev, "scaling\n");
		isp_dev->pad_data[sel->pad].is_cropping_enable = false;
	}

	return 0;
}

/* Get supported formats for a pad */
static void camera_isp_supported_fmts_for_pad(u32 pad,
					  struct camera_isp_mbus_fmt **fmts,
					  int *num_fmts)
{
	if (pad == CAMERA_ISP_PAD_SOURCE_PATH0 ||
	    pad == CAMERA_ISP_PAD_SOURCE_PATH1) {
		*fmts = camera_isp_mp_fmts;
		*num_fmts = ARRAY_SIZE(camera_isp_mp_fmts);
	} else {
		*fmts = camera_isp_sp_fmts;
		*num_fmts = ARRAY_SIZE(camera_isp_sp_fmts);
	}
}

/* Check input/output formats */
static int camera_isp_check_formats(struct device *dev,
					     u32 in_code, u32 out_code)
{
	bool in_is_raw = false;
	bool in_is_yuv422 = false;
	bool in_is_yuv420 = false;
	bool in_is_yuv444 = false;
	bool in_is_rgb = false;
	bool out_is_raw = false;
	bool out_is_yuv420 = false;
	bool out_is_yuv422 = false;
	bool out_is_yuv444 = false;
	bool out_is_rgb = false;

	switch (in_code) {
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SBGGR10_1X10:
		in_is_raw = true;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
		in_is_yuv422 = true;
		break;
	case MEDIA_BUS_FMT_YUYV8_1_5X8:
		in_is_yuv420 = true;
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
		in_is_yuv444 = true;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_BGR888_1X24:
		in_is_rgb = true;
		break;
	default:
		break;
	}

	switch (out_code) {
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SBGGR10_1X10:
		out_is_raw = true;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
		out_is_yuv422 = true;
		break;
	case MEDIA_BUS_FMT_YUYV8_1_5X8:
		out_is_yuv420 = true;
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
		out_is_yuv444 = true;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_BGR888_1X24:
		out_is_rgb = true;
		break;
	default:
		break;
	}

	if (in_is_raw) {
		if (!(out_is_yuv422 || out_is_yuv420 || out_is_rgb || out_is_raw)) {
			dev_err(dev, "%s: RAW-in allows YUV422, YUV420, RGB888, or RAW out (req=0x%x)\n",
				__func__, out_code);
			return -EINVAL;
		}
	} else if (in_is_yuv422) {
		if (!(out_is_yuv422 || out_is_yuv420)) {
			dev_err(dev, "%s: YUV422-in allows YUV422 or YUV420 out (in=0x%x out=0x%x)\n",
				__func__, in_code, out_code);
			return -EINVAL;
		}
	} else if (in_is_yuv444) {
		if (!(out_is_yuv422 || out_is_yuv420)) {
			dev_err(dev, "%s: YUV444-in allows YUV422 or YUV420 out (in=0x%x out=0x%x)\n",
				__func__, in_code, out_code);
			return -EINVAL;
		}
	} else if (in_is_yuv420) {
		if (out_code != in_code) {
			dev_err(dev, "%s: YUV420-in must pass-through as-is (in=0x%x out=0x%x)\n",
				__func__, in_code, out_code);
			return -EINVAL;
		}
	} else if (in_is_rgb) {
		if (!(out_is_rgb || out_is_yuv422 || out_is_yuv420)) {
			dev_err(dev, "%s: RGB-in allows RGB, YUV422, or YUV420 out (in=0x%x out=0x%x)\n",
				__func__, in_code, out_code);
			return -EINVAL;
		}
	} else {
		dev_err(dev, "%s: unsupported input code 0x%x\n", __func__, in_code);
		return -EINVAL;
	}

	return 0;
}

static int camera_isp_validate_cropping(struct camera_isp_dev *isp_dev,
		struct v4l2_mbus_framefmt *out_fmt,
		struct v4l2_rect *r, u32 *scale_factor,
		CSI_PL_CTX_t *ctx)
{
	/* Validate and clamp crop rectangle */
	r->left = clamp_t(u32, r->left, 0, CAMERA_ISP_DEFAULT_WIDTH - 1);
	r->top = clamp_t(u32, r->top, 0, CAMERA_ISP_DEFAULT_HEIGHT - 1);
	r->width = clamp_t(u32, r->width, CAMERA_ISP_WIDTH_MIN,
			CAMERA_ISP_DEFAULT_WIDTH - r->left);
	r->height = clamp_t(u32, r->height, CAMERA_ISP_HEIGHT_MIN,
			CAMERA_ISP_DEFAULT_HEIGHT - r->top);

	/* Ensure crop rectangle doesn't exceed bounds after alignment */
	if (r->left + r->width > CAMERA_ISP_DEFAULT_WIDTH) {
		r->width = CAMERA_ISP_DEFAULT_WIDTH - r->left;
		r->width = ALIGN_DOWN(r->width, CAMERA_ISP_WIDTH_ALIGN);
	}
	if (r->top + r->height > CAMERA_ISP_DEFAULT_HEIGHT) {
		r->height = CAMERA_ISP_DEFAULT_HEIGHT - r->top;
		r->height = ALIGN_DOWN(r->height, CAMERA_ISP_HEIGHT_ALIGN);
	}

	/* Update crop window in pipeline context */
	ctx->crop.x_st = r->left;
	ctx->crop.y_st = r->top;
	ctx->crop.x_end = r->left + r->width - 1;
	ctx->crop.y_end = r->top + r->height - 1;

	/* Output size should be <= crop size */
	if (out_fmt->width > r->width || out_fmt->height > r->height)
		return -EINVAL;

	/* If output size != crop size, compute integral scaling factor */
	if (out_fmt->width != r->width || out_fmt->height != r->height) {
		int sf = calculate_scaling_factor(r->width, r->height,
				out_fmt->width, out_fmt->height);
		if (sf > 0) {
			*scale_factor = sf;
		} else {
			dev_err(isp_dev->dev, "non-integral crop->o/p: (%dx%d -> %dx%d)\n",
					r->width, r->height, out_fmt->width, out_fmt->height);
			return -EINVAL;
		}
	}

	return 0;
}

/* Program pipeline */
static int camera_isp_program_pipeline(struct camera_isp_dev *isp_dev,
						 u32 pad,
						 struct v4l2_mbus_framefmt *in_fmt,
						 struct v4l2_mbus_framefmt *out_fmt,
						 u32 scale_factor)
{
	CSI_PL_CTX_t *ctx;
	struct v4l2_rect *r = &isp_dev->pad_data[pad].r;
	int ret;

	if (!isp_dev->pipeline_ready[pad - 1]) {
		isp_dev->pipe[pad - 1] = CSI_PIPE_Create(isp_dev, pad - 1);
		isp_dev->pipeline_ready[pad - 1] = true;
	}

	ctx = (CSI_PL_CTX_t *)isp_dev->pipe[pad - 1];
	if (!ctx)
		return -ENOMEM;

	CSI_PIPE_Set_Input_Fmt(isp_dev->pipe[pad - 1],
				  in_fmt->width, in_fmt->height);

	if (scale_factor == 0) {
		ret = camera_isp_validate_cropping(isp_dev, out_fmt, r,
					&scale_factor, ctx);
		if (ret)
			return ret;
	} else {
		ctx->crop.x_st = 0;
		ctx->crop.y_st = 0;
		ctx->crop.x_end = ctx->hres - 1;
		ctx->crop.y_end = ctx->vres - 1;
	}

	dev_dbg(isp_dev->dev, "%s crop (%d,%d)/%dx%d -> (%d,%d)-(%d,%d) scale=%d oprn=%d\n",
			 __func__, r->left, r->top, r->width, r->height,
			 ctx->crop.x_st, ctx->crop.y_st, ctx->crop.x_end, ctx->crop.y_end,
			 ctx->crop.scale, ctx->crop.imgres_oprn);


	CSI_PIPE_Set_Fmt(isp_dev->pipe[pad - 1], out_fmt);
	CSI_PIPE_Config(isp_dev->pipe[pad - 1], in_fmt->code, scale_factor);

	return 0;
}

/* V4L2 subdev pad operations */
static int camera_isp_set_fmt(struct v4l2_subdev *sd,
				struct v4l2_subdev_state *sd_state,
				struct v4l2_subdev_format *format)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	struct camera_isp_mbus_fmt *supported_fmts;
	struct v4l2_subdev *subdev;
	struct media_pad *pad;
	int num_fmts;
	int i;
	int ret;
	bool cropping = isp_dev->pad_data[format->pad].is_cropping_enable;
	struct v4l2_subdev_format sd_fmt = {
		.which = format->which,
		.pad = 0,
		.format = {
			.width = format->format.width,
			.height = format->format.height,
		}
	};

	if (format->pad >= CAMERA_ISP_PAD_NR)
		return -EINVAL;

	/* Get supported formats based on pad */
	camera_isp_supported_fmts_for_pad(format->pad, &supported_fmts, &num_fmts);

	/* Validate format */
	for (i = 0; i < num_fmts; i++) {
		if (supported_fmts[i].code == format->format.code)
			break;
	}

	if (i >= num_fmts) {
		dev_dbg(isp_dev->dev, "%s: unsupported code 0x%x on pad %u\n",
			__func__, format->format.code, format->pad);
		return -EINVAL;
	}

	if (isp_dev->bypass_isp_enabled && cropping) {
		dev_err(isp_dev->dev,
			"bypass_isp_enabled=1 does not allow crop+scale format\n");
		return -EINVAL;
	}

	/* Clamp dimensions */
	format->format.width = clamp_t(u32, format->format.width,
			CAMERA_ISP_WIDTH_MIN, CAMERA_ISP_WIDTH_MAX);
	format->format.height = clamp_t(u32, format->format.height,
			CAMERA_ISP_HEIGHT_MIN, CAMERA_ISP_HEIGHT_MAX);

	pad = media_pad_remote_pad_first(&isp_dev->pads[CAMERA_ISP_PAD_SINK]);

	if (pad && is_media_entity_v4l2_subdev(pad->entity)) {
		sd_fmt.pad = pad->index;
		sd_fmt.format = format->format;

		subdev = media_entity_to_v4l2_subdev(pad->entity);
		/* Cropping-only */
		if (cropping) {
			sd_fmt.format.width = CAMERA_ISP_DEFAULT_WIDTH;
			sd_fmt.format.height = CAMERA_ISP_DEFAULT_HEIGHT;
		}

		/* Apply selected sensor format to CSI */
		if (format->which == V4L2_SUBDEV_FORMAT_ACTIVE) {
			ret = camera_isp_get_sensor_resolution_and_program(isp_dev, subdev,
					sd_state, &sd_fmt);
			if (ret)
				return ret;

			ret = v4l2_subdev_call(subdev, pad, set_fmt, NULL, &sd_fmt);
			if (ret)
				return ret;
		}
	}

	/* Enforce input->output format policy */
	ret = camera_isp_check_formats(isp_dev->dev,
			sd_fmt.format.code,
			format->format.code);
	if (ret)
		return ret;

	/* Program pipeline */
	if (format->which == V4L2_SUBDEV_FORMAT_ACTIVE) {
		/* Disable scaling for cropping-only */
		ret = camera_isp_program_pipeline(isp_dev, format->pad,
				&sd_fmt.format,
				&format->format,
				(cropping ? 0 : isp_dev->scale_factor));
		if (ret)
			return ret;
	}

	return 0;
}

static int camera_isp_get_fmt(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_format *format)
{
	struct v4l2_mbus_framefmt *fmt;
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);

	if (format->pad >= CAMERA_ISP_PAD_NR)
		return -EINVAL;

	if (format->which == V4L2_SUBDEV_FORMAT_TRY)
		fmt = v4l2_subdev_state_get_format(sd_state, format->pad);
	else
		fmt = &isp_dev->formats[format->pad];

	format->format = *fmt;

	return 0;
}

static int camera_isp_enum_mbus_code(struct v4l2_subdev *sd,
				struct v4l2_subdev_state *sd_state,
				struct v4l2_subdev_mbus_code_enum *code)
{
	struct camera_isp_mbus_fmt *supported_fmts;
	int num_fmts;

	if (code->pad >= CAMERA_ISP_PAD_NR)
		return -EINVAL;

	/* Get supported formats based on pad */
	if (code->pad == CAMERA_ISP_PAD_SOURCE_PATH0 ||
		code->pad == CAMERA_ISP_PAD_SOURCE_PATH1) {
		supported_fmts = camera_isp_mp_fmts;
		num_fmts = ARRAY_SIZE(camera_isp_mp_fmts);
	} else {
		supported_fmts = camera_isp_sp_fmts;
		num_fmts = ARRAY_SIZE(camera_isp_sp_fmts);
	}

	if (code->index >= num_fmts)
		return -EINVAL;

	code->code = supported_fmts[code->index].code;

	return 0;
}


static const struct v4l2_subdev_pad_ops camera_isp_pad_ops = {
	.set_fmt = camera_isp_set_fmt,
	.get_fmt = camera_isp_get_fmt,
	.enum_mbus_code = camera_isp_enum_mbus_code,
	.get_selection = camera_isp_get_selection,
	.set_selection = camera_isp_set_selection,
};

static const struct v4l2_subdev_ops camera_isp_subdev_ops = {
	.core = &camera_isp_core_ops,
	/*.video = &camera_isp_video_ops,*/
	.pad = &camera_isp_pad_ops,
};

/* V4L2 subdev internal operations */
static int camera_isp_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);
	struct v4l2_mbus_framefmt *format;
	int i;

	/* Initialize default formats for all pads */
	for (i = 0; i < CAMERA_ISP_PAD_NR; i++) {
		//format = v4l2_subdev_get_try_format(sd, fh->state, i);
		format = v4l2_subdev_state_get_format(fh->state, i);
		format->width = CAMERA_ISP_DEFAULT_WIDTH;
		format->height = CAMERA_ISP_DEFAULT_HEIGHT;
		format->field = V4L2_FIELD_NONE;
		format->colorspace = V4L2_COLORSPACE_DEFAULT;

		if (i == CAMERA_ISP_PAD_SOURCE_PATH0 || i == CAMERA_ISP_PAD_SOURCE_PATH1)
			format->code = camera_isp_mp_fmts[0].code;
		else
			format->code = camera_isp_sp_fmts[0].code;

		/* Initialize default crop rectangles for source pads */
		if (i != CAMERA_ISP_PAD_SINK) {
			struct v4l2_rect *crop;

			crop = v4l2_subdev_state_get_crop(fh->state, i);
			crop->left = 0;
			crop->top = 0;
			crop->width = CAMERA_ISP_DEFAULT_WIDTH;
			crop->height = CAMERA_ISP_DEFAULT_HEIGHT;
		}
	}

	dev_dbg(isp_dev->dev, "ISP subdev opened\n");
	return 0;
}

static int camera_isp_close(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct camera_isp_dev *isp_dev = v4l2_get_subdevdata(sd);

	dev_dbg(isp_dev->dev, "ISP subdev closed\n");
	return 0;
}

static const struct v4l2_subdev_internal_ops camera_isp_internal_ops = {
	.open = camera_isp_open,
	.close = camera_isp_close,
};

static int camera_isp_register_async_notifier(struct camera_isp_dev *isp_dev)
{
	int ret;
	int pad;
	struct fwnode_handle *ep, *remote_ep;

	v4l2_async_subdev_nf_init(&isp_dev->notifier, &isp_dev->sd);

	isp_dev->notifier.ops = &camera_isp_notify_ops;
	if (dev_fwnode(isp_dev->dev) != NULL) {
		for (pad = 0; pad < CAMERA_ISP_PAD_NR; pad++) {

			if (isp_dev->pads[pad].flags != MEDIA_PAD_FL_SINK)
				continue;

			ep = fwnode_graph_get_endpoint_by_id(
					dev_fwnode(isp_dev->dev),
					pad, 0, FWNODE_GRAPH_ENDPOINT_NEXT);
			if (!ep)
				continue;

			remote_ep = fwnode_graph_get_remote_endpoint(ep);

			if (!remote_ep) {
				dev_info(isp_dev->dev, "No remote endpoint, sink pad %d\n", pad);
				fwnode_handle_put(ep);
				continue;
			}

			if (ep && remote_ep) {
				v4l2_async_nf_add_fwnode_remote(&isp_dev->notifier, ep,
						struct v4l2_async_connection);
				dev_dbg(isp_dev->dev, "Registered async notifier\n");
			} else {
				dev_err(isp_dev->dev,
						"Skipping async notifier, sink pad %d\n", pad);
			}

			fwnode_handle_put(remote_ep);
			fwnode_handle_put(ep);
		}
	}
	// Register notifier
	ret = v4l2_async_nf_register(&isp_dev->notifier);

	if (ret) {
		dev_err(isp_dev->dev, "Async notifier register error\n");
		v4l2_async_nf_cleanup(&isp_dev->notifier);
	}

	return ret;
}

static void camera_isp_unregister_async_notifier(struct camera_isp_dev *isp_dev)
{
	v4l2_async_nf_unregister(&isp_dev->notifier);
	v4l2_async_nf_cleanup(&isp_dev->notifier);
}

/* Platform driver operations */
static void parse_wb_config_from_dt(struct device_node *node, WB_CONFIG_t *cfg)
{
	struct device_node *wb_np = of_get_child_by_name(node, "white-balance-config");

	if (wb_np) {
		of_property_read_u32(wb_np, "wb-mode", &cfg->wb_mode);
		of_property_read_u32(wb_np, "wb-p00-mantissa", &cfg->wb_p00_mantissa);
		of_property_read_u32(wb_np, "wb-p00-exponent", &cfg->wb_p00_exponent);
		of_property_read_u32(wb_np, "wb-p01-mantissa", &cfg->wb_p01_mantissa);
		of_property_read_u32(wb_np, "wb-p01-exponent", &cfg->wb_p01_exponent);
		of_property_read_u32(wb_np, "wb-p10-mantissa", &cfg->wb_p10_mantissa);
		of_property_read_u32(wb_np, "wb-p10-exponent", &cfg->wb_p10_exponent);
		of_property_read_u32(wb_np, "wb-p11-mantissa", &cfg->wb_p11_mantissa);
		of_property_read_u32(wb_np, "wb-p11-exponent", &cfg->wb_p11_exponent);
		of_property_read_u32(wb_np, "input-sel", &cfg->input_sel);
		of_node_put(wb_np);

		/* Enable WB by default */
		cfg->wb_en = 1;
	} else {
		memset(cfg, 0, sizeof(*cfg));
	}
}

static int isp_fetch_clocks(struct device *dev, struct clk **isp_clks)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(isp_clock_list); i++) {
		isp_clks[i] = devm_clk_get(dev, isp_clock_list[i]);
		if (IS_ERR(isp_clks[i])) {
			dev_err(dev, "failed to get %s ...!\n", isp_clock_list[i]);
			return PTR_ERR(isp_clks[i]);
		}
	}

	return 0;
}

static int isp_enable_clocks(struct device *dev, struct clk **isp_clks)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(isp_clock_list); i++) {
		ret = clk_prepare_enable(isp_clks[i]);
		if (ret < 0) {
			dev_err(dev, "%s prepare failed..!\n", isp_clock_list[i]);
			goto prepare_failure;
		}
	}
	return 0;
prepare_failure:
	while (--i >= 0)
		clk_disable_unprepare(isp_clks[i]);

	return ret;
}

static int camera_isp_parse_dt(struct camera_isp_dev *isp_dev,
		struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct resource *res;
	struct device *dev = &pdev->dev;

	// Parse WB config from DT
	parse_wb_config_from_dt(node, &isp_dev->wb_config);

	if (!node) {
		dev_err(dev, "No device tree node found\n");
		return -EINVAL;
	}

	/* Parse device ID */
	if (of_property_read_u32(node, "id", &isp_dev->id))
		isp_dev->id = 0;

	if (of_property_read_u32(node, "max-sensor-width",
				   &isp_dev->max_sensor_width))
		isp_dev->max_sensor_width = MAX_SENSOR_WIDTH;
	if (of_property_read_u32(node, "max-sensor-height",
				   &isp_dev->max_sensor_height))
		isp_dev->max_sensor_height = MAX_SENSOR_HEIGHT;

	/* Fetch IORESOURCE_MEM for core_base_addr */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "No core IORESOURCE_MEM found\n");
		return -ENODEV;
	}
	isp_dev->core_base_addr = devm_ioremap_resource(dev, res);
	if (IS_ERR(isp_dev->core_base_addr)) {
		dev_err(dev, "Failed to ioremap core resource\n");
		return PTR_ERR(isp_dev->core_base_addr);
	}

	/* Fetch second IORESOURCE_MEM for dhub_base_addr */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (res) {
		isp_dev->dhub_base_addr = devm_ioremap_resource(dev, res);
		if (IS_ERR(isp_dev->dhub_base_addr)) {
			dev_err(dev, "Failed to ioremap dhub resource\n");
			return PTR_ERR(isp_dev->dhub_base_addr);
		}
	} else {
		isp_dev->dhub_base_addr = NULL;
	}

	/* Fetch IRQ */
	isp_dev->irq_num = platform_get_irq(pdev, 0);
	if (isp_dev->irq_num < 0) {
		dev_err(dev, "Failed to get IRQ\n");
		return isp_dev->irq_num;
	}

	return 0;
}

static void camera_isp_reset(struct reset_control *rst)
{
	reset_control_assert(rst);
	udelay(1);
	reset_control_deassert(rst);
}

static int camera_isp_probe(struct platform_device *pdev)
{
	struct camera_isp_dev *isp_dev;
	struct v4l2_subdev *sd;
	struct media_pad *pads;
	struct reset_control *rst;
	int ret;
	int i;
	struct device *dev = &pdev->dev;

	dev_info(dev, "Camera ISP subdevice probe started\n");

	isp_dev = devm_kzalloc(dev, sizeof(*isp_dev), GFP_KERNEL);
	if (!isp_dev) {
		dev_err(dev, "failed to allocate memory for isp_dev...!\n");
		return -ENOMEM;
	}

	isp_dev->dev = dev;
	isp_dev->active_pipe_id = -1;
	platform_set_drvdata(pdev, isp_dev);

	isp_dev->isp_clks = devm_kzalloc(dev,
		sizeof(struct clk *) * ARRAY_SIZE(isp_clock_list), GFP_KERNEL);
	if (IS_ERR(isp_dev->isp_clks)) {
		dev_err(dev, "failed to allocate memory for isp_clocks...!\n");
		return PTR_ERR(isp_dev->isp_clks);
	}

	rst = devm_reset_control_get_optional(&pdev->dev, "isprst");
	if (IS_ERR(rst) && PTR_ERR(rst) == -EPROBE_DEFER) {
		dev_err(dev, "isprst reset failed...!\n");
		return -EPROBE_DEFER;
	}

	camera_isp_reset(rst);

	/* Fetch Clk */
	ret = isp_fetch_clocks(dev, isp_dev->isp_clks);
	if (ret) {
		dev_err(dev, "isp clock fetch failed...!\n");
		return ret;
	}

	/* Parse device tree */
	ret = camera_isp_parse_dt(isp_dev, pdev);
	if (ret)
		return ret;

	ret = isp_enable_clocks(dev, isp_dev->isp_clks);
	if (ret) {
		dev_err(dev, "isp clock enable failed...!\n");
		return ret;
	}

	/* Initialize V4L2 subdevice */
	sd = &isp_dev->sd;
	v4l2_subdev_init(sd, &camera_isp_subdev_ops);

	snprintf(sd->name, sizeof(sd->name), "%s.%d", CAMERA_ISP_NAME, isp_dev->id);
	sd->internal_ops = &camera_isp_internal_ops;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE | V4L2_SUBDEV_FL_HAS_EVENTS;
	sd->dev =  dev;
	sd->owner = THIS_MODULE;
	sd->entity.function = MEDIA_ENT_F_IO_V4L;
	sd->entity.obj_type = MEDIA_ENTITY_TYPE_V4L2_SUBDEV;
	sd->entity.name = sd->name;

	v4l2_set_subdevdata(sd, isp_dev);

	/* Initialize media entity pads */
	pads = isp_dev->pads;
	pads[CAMERA_ISP_PAD_SINK].flags = MEDIA_PAD_FL_SINK;
	pads[CAMERA_ISP_PAD_SOURCE_PATH0].flags = MEDIA_PAD_FL_SOURCE;
	pads[CAMERA_ISP_PAD_SOURCE_PATH1].flags = MEDIA_PAD_FL_SOURCE;

	for (i = 0; i < CAMERA_ISP_PAD_NR; i++) {
		isp_dev->formats[i].code = camera_isp_mp_fmts[0].code;
		/* Initialize to default resolution from header (supports 1080p by default).
		 * 640x480 and other sizes can be selected later via set_fmt.
		 */
		isp_dev->formats[i].width = CAMERA_ISP_DEFAULT_WIDTH;
		isp_dev->formats[i].height = CAMERA_ISP_DEFAULT_HEIGHT;
	}

	ret = media_entity_pads_init(&sd->entity, CAMERA_ISP_PAD_NR, pads);
	if (ret) {
		dev_err(dev, "Failed to initialize media entity pads: %d\n", ret);
		goto err_cleanup_clk;
	}

	sd->entity.function = MEDIA_ENT_F_PROC_VIDEO_ISP;
	sd->entity.ops = &camera_isp_entity_ops;

	ret = camera_isp_register_async_notifier(isp_dev);
	if (ret) {
		dev_err(dev, "Failed to register async notifier: %d\n", ret);
		goto err_cleanup_entity;
	}

	/* Register V4L2 async subdevice */
	ret = v4l2_async_register_subdev(sd);
	if (ret) {
		dev_err(dev, "Failed to register async subdev: %d\n", ret);
		goto err_cleanup_notifier;
	}

	/* Initialize mutex */
	mutex_init(&isp_dev->lock);

	isp_dev->cached_modes = NULL;
	isp_dev->num_resolutions = NULL;
	isp_dev->num_cached_codes = 0;

	ret = v4l2_ctrl_handler_init(&isp_dev->ctrl_handler, ARRAY_SIZE(camera_isp_ctrls));
	if (ret) {
		dev_err(dev, "Failed to init ctrl handler: %d\n", ret);
		goto err_cleanup_async;
	}

	for (int i = 0; i < ARRAY_SIZE(camera_isp_ctrls); i++) {
		v4l2_ctrl_new_custom(&isp_dev->ctrl_handler, &camera_isp_ctrls[i], NULL);
		if (isp_dev->ctrl_handler.error) {
			dev_err(dev, "Failed to register ctrl '%s': %d\n",
				camera_isp_ctrls[i].name, isp_dev->ctrl_handler.error);
			ret = isp_dev->ctrl_handler.error;
			goto err_cleanup_ctrl;
		}
	}

	isp_dev->sd.ctrl_handler = &isp_dev->ctrl_handler;

	ret = camera_isp_create_wb_sysfs(isp_dev);
	if (ret)
		dev_warn(dev, "Failed to create WB sysfs group\n");


	ret = isp_shm_init(dev);
	if (ret) {
		dev_err(dev, "Failed isp_shm_init: %d\n", ret);
		goto err_cleanup_ctrl;
	}

	ret = CSI_PIPE_Init(isp_dev);
	if (ret) {
		dev_err(dev, "Failed CSI_PIPE_Init: %d\n", ret);
		goto err_cleanup_shm;
	}
	/* Enable runtime PM */
	pm_runtime_enable(dev);

	dev_info(dev, "Camera ISP subdevice registered successfully\n");
	return 0;

err_cleanup_shm:
	isp_shm_deinit(dev);
err_cleanup_ctrl:
	v4l2_ctrl_handler_free(&isp_dev->ctrl_handler);
err_cleanup_async:
	v4l2_async_unregister_subdev(sd);
err_cleanup_notifier:
	camera_isp_unregister_async_notifier(isp_dev);
err_cleanup_entity:
	media_entity_cleanup(&sd->entity);
err_cleanup_clk:
	for (i = 0; i < ARRAY_SIZE(isp_clock_list); i++)
		clk_disable_unprepare(isp_dev->isp_clks[i]);
	return ret;
}

static void camera_isp_remove(struct platform_device *pdev)
{
	struct camera_isp_dev *isp_dev = platform_get_drvdata(pdev);
	int i;

	if (!isp_dev)
		return;

	pm_runtime_disable(&pdev->dev);
	CSI_PIPE_Exit(isp_dev);
	isp_shm_deinit(&pdev->dev);
	camera_isp_remove_wb_sysfs(isp_dev);
	v4l2_ctrl_handler_free(&isp_dev->ctrl_handler);
	free_sensor_modes(isp_dev);
	mutex_destroy(&isp_dev->lock);
	v4l2_async_unregister_subdev(&isp_dev->sd);
	camera_isp_unregister_async_notifier(isp_dev);
	media_entity_cleanup(&isp_dev->sd.entity);
	for (i = 0; i < ARRAY_SIZE(isp_clock_list); i++)
		clk_disable_unprepare(isp_dev->isp_clks[i]);
	dev_info(&pdev->dev, "Camera ISP subdevice removed\n");
}

/* Device tree matching */
static const struct of_device_id camera_isp_of_match[] = {
	{ .compatible = "syna,camera-isp" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, camera_isp_of_match);

/* Platform driver */
static struct platform_driver camera_isp_driver = {
	.probe = camera_isp_probe,
	.remove = camera_isp_remove,
	.driver = {
		.name = CAMERA_ISP_NAME,
		.owner = THIS_MODULE,
		.of_match_table = camera_isp_of_match,
		//.pm = &camera_isp_pm_ops,
	},
};

static int __init camera_isp_init_module(void)
{
	int ret;

	ret = platform_driver_register(&camera_isp_driver);
	if (ret) {
		pr_err("Failed to register camera ISP driver: %d\n", ret);
		return ret;
	}

	pr_info("Camera ISP driver initialized\n");
	return 0;
}

static void __exit camera_isp_exit_module(void)
{
	platform_driver_unregister(&camera_isp_driver);
	pr_info("Camera ISP driver exited\n");
}

module_init(camera_isp_init_module);
module_exit(camera_isp_exit_module);

MODULE_DESCRIPTION("Camera ISP V4L2 subdevice driver");
MODULE_AUTHOR("Synaptics Camera Team");
MODULE_LICENSE("GPL");

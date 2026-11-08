/* SPDX-License-Identifier: GPL-2.0 */

/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CAMERA_V4L2_COMMON_H__
#define __CAMERA_V4L2_COMMON_H__
#include <linux/videodev2.h>

struct camera_video_plane {
	uint32_t dma_addr;
	uint32_t size;
};

struct camera_vb2_buffer {
	struct vb2_v4l2_buffer vb;
	unsigned int num_planes;
	struct camera_video_plane planes[VIDEO_MAX_PLANES];
	struct list_head list;
	uint32_t sequence;
};

struct camera_pad_reqbufs {
	int pad;
	uint32_t num_buffers;
};

struct camera_pad_buf {
	uint32_t pad;
	struct camera_vb2_buffer *buf;
};

struct camera_pad_stream_status {
	uint32_t pad;
	uint32_t status;
};

struct camera_pad_queryctrl {
	uint32_t pad;
	struct v4l2_queryctrl *query_ctrl;
};

struct camera_pad_query_ext_ctrl {
	uint32_t pad;
	struct v4l2_query_ext_ctrl *query_ext_ctrl;
};

struct camera_pad_control {
	uint32_t pad;
	struct v4l2_control *control;
};

struct camera_pad_ext_controls {
	uint32_t pad;
	struct v4l2_ext_controls *ext_controls;
};

struct camera_pad_querymenu {
	uint32_t pad;
	struct v4l2_querymenu *querymenu;
};

#define V4L2_CID_USER_WB_ENABLE (V4L2_CID_USER_BASE + 0x10)
#define V4L2_CID_USER_DISABLE_SCALE_CROP (V4L2_CID_USER_BASE + 0x11)

#define CAMERA_PAD_REQUBUFS       _IOWR('V',  BASE_VIDIOC_PRIVATE + 0, struct camera_pad_reqbufs)
#define CAMERA_PAD_BUF_DONE       _IOWR('V',  BASE_VIDIOC_PRIVATE + 1, struct camera_pad_buf)
#define CAMERA_PAD_BUF_QUEUE      _IOWR('V',  BASE_VIDIOC_PRIVATE + 2, struct camera_pad_buf)
#define CAMERA_PAD_S_STREAM \
	_IOWR('V',  BASE_VIDIOC_PRIVATE + 3, struct camera_pad_stream_status)

#define CAMERA_PAD_QUERYCTRL      _IOWR('V',  BASE_VIDIOC_PRIVATE + 4, struct camera_pad_queryctrl)
#define CAMERA_PAD_QUERY_EXT_CTRL _IOWR('V',  BASE_VIDIOC_PRIVATE + 5, struct camera_pad_query_ext_ctrl)
#define CAMERA_PAD_G_CTRL         _IOWR('V',  BASE_VIDIOC_PRIVATE + 6, struct camera_pad_control)
#define CAMERA_PAD_S_CTRL         _IOWR('V',  BASE_VIDIOC_PRIVATE + 7, struct camera_pad_control)
#define CAMERA_PAD_G_EXT_CTRLS    _IOWR('V',  BASE_VIDIOC_PRIVATE + 8, struct camera_pad_ext_controls)
#define CAMERA_PAD_S_EXT_CTRLS    _IOWR('V',  BASE_VIDIOC_PRIVATE + 9, struct camera_pad_ext_controls)
#define CAMERA_PAD_TRY_EXT_CTRLS \
	_IOWR('V',  BASE_VIDIOC_PRIVATE + 10, struct camera_pad_ext_controls)
#define CAMERA_PAD_QUERYMENU      _IOWR('V',  BASE_VIDIOC_PRIVATE + 11, struct camera_pad_querymenu)

#endif

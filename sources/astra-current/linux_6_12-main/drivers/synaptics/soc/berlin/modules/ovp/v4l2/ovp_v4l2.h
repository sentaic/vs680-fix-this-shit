/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2026 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __M2M_OVPD_H__
#define __M2M_OVPD_H__

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/videobuf2-v4l2.h>
#include "vpp_mem.h"

#define OVPD_NAME			"ovpd"
#define OVPD_MAX_CONTEXTS		4

/* Shared Mem to allocate OVP_CMD_OBJ structure in the OVP TA (ovp_module.h)
 * This buffer needs physical address to program the OVP HW
 * Each context allocate 1024 bytes
 */
#define OVPD_HW_CMD_SHM_SIZE		(1024 * OVPD_MAX_CONTEXTS)

#define MAX_PENDING_BUFS			10
/* V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED => Number of output buffers required to start streaming
 * V4L2_OVPD_MIN_DST_BUFFER_REQUIRED => Number of capture buffers required to startstreaming
 * We need at least 1 output buffer and 2 capture
 * OVPD takes one interlaced frame and generates two de-interlaced frames
 */
#define V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED	1
#define V4L2_OVPD_MIN_DST_BUFFER_REQUIRED	2

static inline unsigned int min_buf_reqd(struct vb2_queue *vq)
{
	if (vq->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE)
		return V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED;

	return V4L2_OVPD_MIN_DST_BUFFER_REQUIRED;
}

/* AXI error because of invalid address (using virt instead of phy address)
 */
#define OVP_INTR_STATUS_AXI_ERROR 0x20

/* Invalid command in the command buffer
 */
#define OVP_INTR_STATUS_CMD_ERROR 0x10

#define OVP_INTR_STATUS_ERROR_MASK_ALL (OVP_INTR_STATUS_AXI_ERROR | OVP_INTR_STATUS_CMD_ERROR)

struct deint_param_s {
	bool b_is_whole_frame;
	bool b_is_one_field;
	u32 u_priority;
};

/**
 * struct ovpd_fmt - driver's internal color format data
 * @pixelformat:fourcc code for this format
 * @nb_planes:  number of planes  (ex: [0]=RGB/Y - [1]=Cb/Cr, ...)
 * @bpp:        bits per pixel (general)
 */
struct ovpd_fmt {
	u32 pixelformat;
	u8  nb_planes;
	u8  bpp;
};

struct ovpd_frame_info {
	u32 u_width;
	u32 u_height;
	struct ovpd_fmt fmt;
};

/**
 * struct m2m_ovpd_ctx - device context data
 *
 * @state: flags to keep track of user configuration
 * @m2m_ovpd_dev: the device this context applies to
 * @output_frame_info: V4L2 output buffer frame info
 * @sink_fmt:
 * @capture_frame_info: V4L2 capture buffer frame info
 * @src_fmt:
 * @fh: v4l2 file handle
 * @ctrl_handler: v4l2 control handler
 * @cur_src: current V4L2 src (output) buffer array
 * @cur_dst1: current V4L2 dst (capture) buffer array
 * @cur_dst2: current V4L2 dst (capture) buffer array
 * @cur_desc: current vbuf descriptor array
 * @buf_id: Unique ID to match HW m_uch_buff_id
 * @lock: used for queue management
 * @work_done:
 * @ctrls_rdy: whether v4l2 controls are ready to use
 * @ovpd_delayed_work: work to be run on timeout
 * @list: list_head for further contexts
 * @deint_param:
 * @timeperframe:
 * @cur_tail_index:
 * @cur_head_index:
 * @firstPushDone: boolean to track first push (ovpd needs two push to return)
 */
struct m2m_ovpd_ctx {
	struct m2m_ovpd_dev		*m2m_ovpd_dev;
	struct ovpd_frame_info		output_frame_info;
	struct v4l2_pix_format_mplane	sink_fmt;
	struct ovpd_frame_info		capture_frame_info;
	struct v4l2_pix_format_mplane	src_fmt;
	struct v4l2_fh			fh;
	struct vb2_v4l2_buffer		*cur_src[MAX_PENDING_BUFS];
	struct vb2_v4l2_buffer		*cur_dst1[MAX_PENDING_BUFS];
	struct vb2_v4l2_buffer		*cur_dst2[MAX_PENDING_BUFS];
	struct ovp_vid_buf_desc_t	*cur_desc[MAX_PENDING_BUFS];
	u8				buf_id;
	/* mutex lock for queue management */
	struct mutex			lock;
	struct completion		work_done;
	struct delayed_work		ovpd_delayed_work;
	/* 1 when a mem2mem job is in flight; cleared by ISR or timeout path */
	atomic_t			job_in_flight;
	/* 1 when context is being torn down; ISR must skip processing */
	atomic_t			torn_down;
	struct list_head		list;
	struct deint_param_s		deint_param;
	int				cur_tail_index;
	int				cur_head_index;
	u8				first_push_done;
};

/**
 * struct ovpd_m2m_device - v4l2 memory-to-memory device data
 *
 * @vdev:       video device node for v4l2 m2m mode
 * @m2m_dev:    v4l2 m2m device data
 * @refcnt:     reference counter
 */
struct ovpd_m2m_device {
	struct video_device	*vdev;
	struct v4l2_m2m_dev	*m2m_dev;
};

/**
 * struct m2m_ovpd_dev - abstraction for ovpd entity
 *
 * @v4l2_dev:       v4l2 device
 * @vdev:           video device
 * @pdev:           platform device
 * @dev:            device for DMA coherent memory
 * @alloc_dev:      device for buffer memory allocation
 * @lock:           mutex protecting this data structure
 * @id:             device index
 * @m2m:            memory-to-memory V4L2 device information
 * @ref_count:      Reference count to track open instances
 * @mem_list:       memory list to contain VBUF allocations
 * @ctx_list_head:  List of open m2m contexts for this device
 */
struct m2m_ovpd_dev {
	struct v4l2_device	v4l2_dev;
	struct video_device	vdev;
	struct platform_device	*pdev;
	struct device		*dev;
	struct device		*alloc_dev;
	/* mutex lock for device open/close management */
	struct mutex		lock;
	u16			id;
	struct ovpd_m2m_device	m2m;
	atomic_t		ref_count;
	VPP_MEM_LIST		*mem_list;
	struct list_head	ctx_list_head;
};

static inline void m2m_src_buf_next(struct m2m_ovpd_ctx *ctx, int idx,
				    struct vb2_v4l2_buffer **arr)
{
	if (!arr[idx])
		arr[idx] = v4l2_m2m_next_src_buf(ctx->fh.m2m_ctx);
}

static inline void m2m_dst_buf_next(struct m2m_ovpd_ctx *ctx, int idx,
				    struct vb2_v4l2_buffer **arr)
{
	if (!arr[idx])
		arr[idx] = v4l2_m2m_next_dst_buf(ctx->fh.m2m_ctx);
}

static inline void m2m_src_buf_remove(struct m2m_ovpd_ctx *ctx, int idx,
				      struct vb2_v4l2_buffer **arr)
{
	if (!arr[idx])
		arr[idx] = v4l2_m2m_src_buf_remove(ctx->fh.m2m_ctx);
}

static inline void m2m_dst_buf_remove(struct m2m_ovpd_ctx *ctx, int idx,
				      struct vb2_v4l2_buffer **arr)
{
	if (!arr[idx])
		arr[idx] = v4l2_m2m_dst_buf_remove(ctx->fh.m2m_ctx);
}

#endif //__M2M_OVPD_H__

// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/sched/task.h>

#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-vmalloc.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-common.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-core.h>
#include <linux/dma-mapping.h>
#include "avio_type.h"
#include "vpp_vbuf.h"
#include "ovp_v4l2.h"
#include "drv_ovp_wrap.h"
#include "vbuf.h"
#include "vpp_mem.h"
#include "ovp_debug.h"
#include "tee_ca_ovp.h"

#define OVP_MODULE_NAME			"ovpd"
#define OVP_VERSION			"1.0"
#define SUPPORTED_NUM_PLANES            2
#define MAX_VBUF_INFO			(OVPD_MAX_CONTEXTS * MAX_PENDING_BUFS)

#define OVPD_DELAYED_WORK_DELAY       (3 * 1000)
#define SYNA_OVPD_MAX_BUF_SLOT        (32U)

#define SYNA_DRIVER_NAME		"syna-" OVP_MODULE_NAME
#define SYNA_CARD_TYPE			OVP_MODULE_NAME
#define SYNA_OVPD_NAME			"syna," OVP_MODULE_NAME

#define fh_to_ctx(__fh) container_of(__fh, struct m2m_ovpd_ctx, fh)

static struct task_struct *ovp_isr_task;

/* OVPD device specific variables */
static struct m2m_ovpd_dev *ovpd;

/* VBUF buffer variables */
static VPP_MEM vpp_disp_info_shm_handle[MAX_VBUF_INFO];
static VBUF_INFO vpp_disp_desc_array[MAX_VBUF_INFO];

static VPP_MEM ovp_shm_handle;

static const struct ovpd_fmt m2m_ovpd_format = {
	.pixelformat    = V4L2_PIX_FMT_NV12M,
	.nb_planes      = 2,
	.bpp            = 12,
};

static const struct ovpd_fmt *m2m_ovpd_find_fmt(u32 pixelformat)
{
	if (pixelformat == V4L2_PIX_FMT_NV12M)
		return &m2m_ovpd_format;

	return NULL;
}

static void m2m_ovpd_vb2_finish(struct vb2_v4l2_buffer *cur_vb,
				struct vb2_v4l2_buffer *ref_vb, int vb_state)
{
	if (cur_vb) {
		if (ref_vb) {
			cur_vb->vb2_buf.timestamp = ref_vb->vb2_buf.timestamp;
			cur_vb->timecode = ref_vb->timecode;
			cur_vb->flags &= ~V4L2_BUF_FLAG_TSTAMP_SRC_MASK;
			cur_vb->flags |= ref_vb->flags & V4L2_BUF_FLAG_TSTAMP_SRC_MASK;
		}

		if (cur_vb->vb2_buf.state == VB2_BUF_STATE_ACTIVE)
			v4l2_m2m_buf_done(cur_vb, vb_state);
	}
}

static void m2m_ovpd_job_finish(struct m2m_ovpd_ctx *ctx, int vb_state,
				bool first_push)
{
	int slot = ctx->cur_head_index;

	if (first_push) {
		/* Drop dummy slot (peeked buffers) and release its descriptor. */
		ctx->cur_src[slot] = NULL;
		ctx->cur_dst1[slot] = NULL;
		ctx->cur_dst2[slot] = NULL;
		kfree(ctx->cur_desc[slot]);
		ctx->cur_desc[slot] = NULL;

		slot = (slot + 1) % MAX_PENDING_BUFS;
		ovp_debug("%s: First push, complete slot %d\n", __func__, slot);
	}

	m2m_ovpd_vb2_finish(ctx->cur_src[slot],
			    NULL, vb_state);
	m2m_ovpd_vb2_finish(ctx->cur_dst1[slot],
			    ctx->cur_src[slot], vb_state);
	m2m_ovpd_vb2_finish(ctx->cur_dst2[slot],
			    ctx->cur_src[slot], vb_state);
	ctx->cur_src[slot] = NULL;
	ctx->cur_dst1[slot] = NULL;
	ctx->cur_dst2[slot] = NULL;

	v4l2_m2m_job_finish(ctx->m2m_ovpd_dev->m2m.m2m_dev, ctx->fh.m2m_ctx);

	kfree(ctx->cur_desc[slot]);
	ctx->cur_desc[slot] = NULL;
	ctx->cur_head_index = (slot + 1) % MAX_PENDING_BUFS;

	complete(&ctx->work_done);
}

static void delayed_m2m_ovpd_work_function(struct work_struct *work)
{
	struct delayed_work *pdelay = container_of(work,
			struct delayed_work, work);
	struct m2m_ovpd_ctx *ctx = container_of(pdelay,
			struct m2m_ovpd_ctx,  ovpd_delayed_work);

	/* ISR may have already completed this job; avoid double-finish. */
	if (!atomic_xchg(&ctx->job_in_flight, 0))
		return;

	ovp_error("%s: timedout events (first_push_done: %d)\n", __func__, ctx->first_push_done);
	m2m_ovpd_job_finish(ctx, VB2_BUF_STATE_ERROR, ctx->first_push_done ? false : true);
}

static int prepare_vbuf_desc(struct m2m_ovpd_ctx *ctx,
			     struct ovp_vid_buf_desc_t *ptr_desc,
			     int slot)
{
	unsigned int vbuf_index;
	VBUF_INFO *ptr_vbuf_info;
	VPP_VBUF *ptr_vpp_buf;

	if (ctx->buf_id == 0 || ctx->buf_id > OVPD_MAX_CONTEXTS ||
	    slot < 0 || slot >= MAX_PENDING_BUFS) {
		ovp_error("%s: invalid buf_id %u slot %d\n", __func__, ctx->buf_id, slot);
		return -EINVAL;
	}

	/* Reserve a dedicated VBUF_INFO per active context/slot pair. */
	vbuf_index = ((ctx->buf_id - 1) * MAX_PENDING_BUFS) + slot;
	ptr_vbuf_info = &vpp_disp_desc_array[vbuf_index];

	ptr_desc->user_data = ptr_vbuf_info;
	ptr_vpp_buf = ptr_vbuf_info->pVppVbufInfo_virt;
	if (!ptr_vpp_buf) {
		ovp_error("%s: ptr_vpp_buf NULL\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static void syna_ovp_convert_frame_info(struct ovp_vid_buf_desc_t *ptr_desc, u32 fmt, int x,
					int y, int width, int height)
{
	VBUF_INFO *ptr_vbuf_info;
	VPP_VBUF *ptr_vpp_buf;

	ptr_vbuf_info = (VBUF_INFO *)ptr_desc->user_data;
	ptr_vpp_buf = ptr_vbuf_info->pVppVbufInfo_virt;

	if (fmt == V4L2_PIX_FMT_NV12M) {
		ptr_vpp_buf->m_bytes_per_pixel = 2;
		ptr_vpp_buf->m_buf_stride = width;
		ptr_vpp_buf->m_buf_stride_UV = width;
		ptr_vpp_buf->m_buf_size = (height * ptr_vpp_buf->m_buf_stride * 3) / 2;
		ptr_vpp_buf->m_bits_per_pixel = 8;
	} else {
		ovp_error("%s: fmt %x not supported\n", __func__, fmt);
		return;
	}

	ptr_vpp_buf->m_active_width = width;
	ptr_vpp_buf->m_active_height = height;
	ptr_vpp_buf->m_is_frame_seq = 1;
	ptr_vpp_buf->m_flags = 1;
}

static int build_vbuf_desc(struct ovp_vid_buf_desc_t *ptr_desc, u32 ui_out_buf_alloc_align,
			   struct vb2_v4l2_buffer *src_vb,
			   struct vb2_v4l2_buffer *dst1_vb,
			   struct vb2_v4l2_buffer *dst2_vb)
{
	VBUF_INFO *ptr_vbuf_info;
	VPP_VBUF *ptr_vpp_buf;

	if (!src_vb || !dst1_vb || !dst2_vb) {
		ovp_error("%s: NULL args (src_vb: %p, dst1_vb: %p, dst2_vb:%p)\n",
			  __func__, src_vb, dst1_vb, dst2_vb);
		return -EINVAL;
	}

	ptr_desc->base_addr = (u32)vb2_dma_contig_plane_dma_addr(&src_vb->vb2_buf, 0);

	ptr_vbuf_info = (VBUF_INFO *)ptr_desc->user_data;
	ptr_vpp_buf = ptr_vbuf_info->pVppVbufInfo_virt;

	/* src buffers copy to m_pbuf_start */
	ptr_vpp_buf->m_pbuf_start =
		(ARCH_PTR_TYPE)vb2_dma_contig_plane_dma_addr(&src_vb->vb2_buf, 0);
	ptr_vpp_buf->m_buf_pbuf_start_UV =
		(u32)vb2_dma_contig_plane_dma_addr(&src_vb->vb2_buf, 1);

	/* src buffers copy to m_pbuf_start */
	ptr_vpp_buf->m_pbuf_Out_start_0 =
		(u32)vb2_dma_contig_plane_dma_addr(&dst1_vb->vb2_buf, 0);
	ptr_vpp_buf->m_pbuf_Out_start_UV_0 =
		(u32)vb2_dma_contig_plane_dma_addr(&dst1_vb->vb2_buf, 1);

	/* Second capture must be available, as OVP HW returns 2 output frames! */
	ptr_vpp_buf->m_pbuf_Out_start_1 =
		(u32)vb2_dma_contig_plane_dma_addr(&dst2_vb->vb2_buf, 0);
	ptr_vpp_buf->m_pbuf_Out_start_UV_1 =
		(u32)vb2_dma_contig_plane_dma_addr(&dst2_vb->vb2_buf, 1);

	return 0;
}

static int m2m_ovpd_push_buf(struct m2m_ovpd_ctx *ctx, bool first_push)
{
	int ret;
	struct ovp_msg_header ovp_msg;
	struct ovp_vid_buf_desc_t *ptr_buf_desc;

	if (first_push) {
		m2m_src_buf_next(ctx, ctx->cur_tail_index, ctx->cur_src);
		m2m_dst_buf_next(ctx, ctx->cur_tail_index, ctx->cur_dst1);
		m2m_dst_buf_next(ctx, ctx->cur_tail_index, ctx->cur_dst2);
	} else {
		m2m_src_buf_remove(ctx, ctx->cur_tail_index, ctx->cur_src);
		m2m_dst_buf_remove(ctx, ctx->cur_tail_index, ctx->cur_dst1);
		m2m_dst_buf_remove(ctx, ctx->cur_tail_index, ctx->cur_dst2);
	}

	ptr_buf_desc = kzalloc(sizeof(*ptr_buf_desc), GFP_KERNEL);
	if (!ptr_buf_desc) {
		ret = -ENOMEM;
		ovp_error("%s: Malloc bufdesc failed!\n", __func__);
		goto buf_error;
	}

	ctx->cur_desc[ctx->cur_tail_index] = ptr_buf_desc;

	ret = prepare_vbuf_desc(ctx, ptr_buf_desc, ctx->cur_tail_index);
	if (ret < 0) {
		ovp_error("%s: prepare_vbuf_desc failed (%d)\n", __func__, ret);
		goto malloc_error;
	}

	syna_ovp_convert_frame_info(ptr_buf_desc, ctx->output_frame_info.fmt.pixelformat,
				    0, 0, ctx->output_frame_info.u_width,
				    ctx->output_frame_info.u_height);

	ret = build_vbuf_desc(ptr_buf_desc, 0,
			      ctx->cur_src[ctx->cur_tail_index],
			      ctx->cur_dst1[ctx->cur_tail_index],
			      ctx->cur_dst2[ctx->cur_tail_index]);
	if (ret < 0) {
		ovp_error("%s: build_vbuf_desc failed (%d)\n", __func__, ret);
		goto malloc_error;
	}

	/* Populate the OVP MSG header */
	ovp_msg.m_uch_buff_id = ctx->buf_id;
	ovp_msg.m_uch_output_whole_frame = ctx->deint_param.b_is_whole_frame;
	ovp_msg.m_uch_output_one_field = ctx->deint_param.b_is_one_field;
	ovp_msg.m_uch_in_bit_depth = ctx->output_frame_info.fmt.bpp;
	ovp_msg.m_uch_out_bit_depth = ctx->capture_frame_info.fmt.bpp;
	ovp_msg.m_uch_priority = ctx->deint_param.u_priority;
	ovp_msg.pst_frame_desc = ptr_buf_desc;

	/* Push frame to OVPD module*/
	ret = syna_ovpd_ca_push_frame(&ovp_msg);
	if (ret < 0) {
		ovp_error("%s: syna_ovpd_ca_push_frame failed (%d)\n", __func__, ret);
		goto malloc_error;
	}
	ctx->cur_tail_index = (ctx->cur_tail_index + 1) % MAX_PENDING_BUFS;

	return 0;

malloc_error:
	ctx->cur_desc[ctx->cur_tail_index] = NULL;
	kfree(ptr_buf_desc);
buf_error:
	m2m_ovpd_job_finish(ctx, VB2_BUF_STATE_ERROR, first_push);

	return ret;
}

static struct m2m_ovpd_ctx *find_ovpd_ctx_from_msg_id(struct m2m_ovpd_dev *ptr_ovp_dev,
						      struct ovp_msg_header *ptr_msg)
{
	struct m2m_ovpd_ctx *ctx;

	list_for_each_entry(ctx, &ptr_ovp_dev->ctx_list_head, list) {
		if (ptr_msg->m_uch_buff_id == ctx->buf_id)
			return ctx;
	}

	return NULL;
}

static u8 ovpd_alloc_buf_id(struct m2m_ovpd_dev *ovpd)
{
	u8 buf_id;
	struct m2m_ovpd_ctx *ctx;
	bool in_use;

	for (buf_id = 1; buf_id <= OVPD_MAX_CONTEXTS; buf_id++) {
		in_use = false;
		list_for_each_entry(ctx, &ovpd->ctx_list_head, list) {
			if (ctx->buf_id == buf_id) {
				in_use = true;
				break;
			}
		}

		if (!in_use)
			return buf_id;
	}

	return 0;
}

static int OVP_ISR_Handler(void *param)
{
	CC_MSG_t msg;
	struct m2m_ovpd_dev *ptr_ovp_dev = (struct m2m_ovpd_dev *)param;
	struct m2m_ovpd_ctx *ctx;
	int ret;
	u32 ui_intr_sts;

	while (!kthread_should_stop()) {
		struct ovp_msg_header ovp_msg;

		/* Wait for completed frame from OVPD HW */
		ret = wrap_ovp_drv_get_isr_msg(&msg);
		if (ret < 0) {
			ovp_error("[%s]: ovp_drv_get_isr_msg failed (%d)\n", __func__, ret);
			continue;
		}

		ui_intr_sts = msg.m_Param1;
		if ((ui_intr_sts & OVP_INTR_STATUS_ERROR_MASK_ALL) != 0)
			ovp_error("ISR ret error code %d\n", ui_intr_sts);

		memset(&ovp_msg, 0, sizeof(struct ovp_msg_header));

		syna_ovpd_ca_process_frame(&ovp_msg, ui_intr_sts);

		/* Serialize context list walk/use with open/release list updates */
		mutex_lock(&ptr_ovp_dev->lock);

		/* OVP HW msg received, find the corresponding ovpd ctx*/
		ctx = find_ovpd_ctx_from_msg_id(ptr_ovp_dev, &ovp_msg);
		if (!ctx) {
			mutex_unlock(&ptr_ovp_dev->lock);
			ovp_error("No context found for buff_id %d\n", ovp_msg.m_uch_buff_id);
			continue;
		}

		/* CRITICAL: Check for teardown BEFORE using ctx to prevent use-after-free */
		if (atomic_read(&ctx->torn_down)) {
			mutex_unlock(&ptr_ovp_dev->lock);
			ovp_debug("Skipping ISR for torn-down context buff_id %d\n",
				  ovp_msg.m_uch_buff_id);
			continue;
		}

		/* ISR atomically claims job ownership;
		 * timeout handler will skip if already claimed
		 */
		if (!atomic_xchg(&ctx->job_in_flight, 0)) {
			mutex_unlock(&ptr_ovp_dev->lock);
			ovp_debug("Skipping ISR: job already claimed by timeout handler\n");
			continue;
		}

		if (!ctx->first_push_done) {
			ovp_debug("%s: dummy buf processed!\n", __func__);
			ctx->first_push_done = 1;
			m2m_ovpd_job_finish(ctx, VB2_BUF_STATE_DONE, true);
		} else {
			/* Deinterlaced frame available, read and update V4L2 M2M buf */
			m2m_ovpd_job_finish(ctx, VB2_BUF_STATE_DONE, false);
		}

		mutex_unlock(&ptr_ovp_dev->lock);
	}

	return 0;
}

static void m2m_ovpd_device_run(void *priv)
{
	struct m2m_ovpd_ctx *ctx = priv;
	struct m2m_ovpd_dev *ovpd;
	int ret;

	ovpd = ctx->m2m_ovpd_dev;

	if (!ctx->first_push_done) {
		/* Dummy buffer push */
		ovp_debug("%s: first buf was pushed, use initial dummy buffer!\n", __func__);
		ret = m2m_ovpd_push_buf(ctx, true);
		if (ret < 0)
			return;
	}

	/* Arm ownership before submitting the real frame so ISR cannot beat us. */
	atomic_set(&ctx->job_in_flight, 1);
	ret = m2m_ovpd_push_buf(ctx, false);
	if (ret < 0) {
		atomic_set(&ctx->job_in_flight, 0);
		return;
	}

	/* Start timeout tracking for this in-flight job. */
	schedule_delayed_work(&ctx->ovpd_delayed_work,
			      msecs_to_jiffies(OVPD_DELAYED_WORK_DELAY));
}

static int m2m_ovpd_job_ready(void *priv)
{
	struct m2m_ovpd_ctx *ctx = priv;
	unsigned int n_src, n_dst;

	n_src = v4l2_m2m_num_src_bufs_ready(ctx->fh.m2m_ctx);
	n_dst = v4l2_m2m_num_dst_bufs_ready(ctx->fh.m2m_ctx);

	if (n_src < V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED) {
		ovp_debug("%s: Required %d, but only %d src buffers available\n",
			  __func__, V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED, n_src);
		return 0;
	}

	if (n_dst < V4L2_OVPD_MIN_DST_BUFFER_REQUIRED) {
		ovp_debug("%s: Required %d, but only %d dst buffers available\n",
			  __func__, V4L2_OVPD_MIN_DST_BUFFER_REQUIRED, n_dst);
		return 0;
	}

	return 1;  // ok to run
}

static void m2m_ovpd_abort_func(void *priv)
{
	struct m2m_ovpd_ctx *ctx = priv;

	wait_for_completion_killable(&ctx->work_done);
}

static const struct v4l2_m2m_ops ovpd_m2m_ops = {
	.device_run     = m2m_ovpd_device_run,
	.job_ready	= m2m_ovpd_job_ready,
	.job_abort	= m2m_ovpd_abort_func
};

static int m2m_ovpd_queue_setup(struct vb2_queue *vq,
				unsigned int *nb_buf, unsigned int *nb_planes,
				unsigned int sizes[], struct device *alloc_devs[])
{
	struct m2m_ovpd_ctx *ctx = vb2_get_drv_priv(vq);
	int i;
	unsigned int min = min_buf_reqd(vq);

	if (*nb_planes) {
		ovp_debug("%s: nb_planes: %d, sizes[0]: %d\n",
			  __func__, *nb_planes, sizes[0]);
		//validate number of planes
		if (*nb_planes != SUPPORTED_NUM_PLANES)
			return  -EINVAL;
	} else {
		struct ovpd_frame_info *ptr_frame_info = NULL;

		if (vq->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE) {
			ptr_frame_info = &ctx->output_frame_info;
		} else if (vq->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			ptr_frame_info = &ctx->capture_frame_info;
		} else {
			ovp_error("vq->type (%d) INVALID!!\n", vq->type);
			return -EINVAL;
		}

		if (ptr_frame_info->fmt.pixelformat != V4L2_PIX_FMT_NV12M) {
			ovp_error("fmt.pixelformat INVALID!!\n");
			return -EINVAL;
		}
		*nb_planes = 2;
	}

	if (vq->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
		if (*nb_buf < min) {
			*nb_buf = V4L2_OVPD_MIN_DST_BUFFER_REQUIRED;
			ovp_debug("%s: setting min dst(capture) bufs to %d\n", __func__, *nb_buf);
		}
	} else if (vq->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE) {
		if (*nb_buf < min) {
			*nb_buf = V4L2_OVPD_MIN_SRC_BUFFER_REQUIRED;
			ovp_debug("%s: setting min src(output) bufs to %d\n", __func__, *nb_buf);
		}
	}

	switch (vq->type) {
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		sizes[0] = ctx->output_frame_info.u_width * ctx->output_frame_info.u_height;
		sizes[1] = (ctx->output_frame_info.u_width * ctx->output_frame_info.u_height) / 2;
		break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
		sizes[0] = ctx->capture_frame_info.u_width * ctx->capture_frame_info.u_height;
		sizes[1] = (ctx->capture_frame_info.u_width * ctx->capture_frame_info.u_height) / 2;
		break;
	default:
		ovp_error("invalid queue type: %d\n", vq->type);
		return -EINVAL;
	}

	for (i = 0; i < *nb_planes; ++i)
		alloc_devs[i] = ctx->m2m_ovpd_dev->alloc_dev;

	return 0;
}

static int m2m_ovpd_buf_prepare(struct vb2_buffer *vb)
{
	struct m2m_ovpd_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	struct ovpd_frame_info *fi;

	if (V4L2_TYPE_IS_OUTPUT(vb->type))
		fi = &ctx->output_frame_info;
	else
		fi = &ctx->capture_frame_info;

	vb2_set_plane_payload(vb, 0, fi->u_width * fi->u_height);
	vb2_set_plane_payload(vb, 1, (fi->u_width * fi->u_height) / 2);

	return 0;
}

static void m2m_ovpd_buf_finish(struct vb2_buffer *vb)
{
}

static void m2m_ovpd_buf_queue(struct vb2_buffer *vb)
{
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct m2m_ovpd_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	int i;

	if (V4L2_TYPE_IS_OUTPUT(vb->type)) {
		for (i = 0; i < vb->num_planes; ++i) {
			/* return to V4L2 any 0-size buffer so it can be dequeued by user */
			if (!vb2_get_plane_payload(vb, i)) {
				ovp_error("0 data buffer, skip it\n");
				vb2_buffer_done(vb, VB2_BUF_STATE_DONE);
				return;
			}
		}
	}

	if (ctx->fh.m2m_ctx) {
		v4l2_m2m_buf_queue(ctx->fh.m2m_ctx, vbuf);
		v4l2_m2m_try_schedule(ctx->fh.m2m_ctx);
	}
}

static int m2m_ovpd_start_streaming(struct vb2_queue *q, unsigned int count)
{
	struct m2m_ovpd_ctx *ctx = q->drv_priv;

	v4l2_m2m_update_start_streaming_state(ctx->fh.m2m_ctx, q);

	return 0;
}

static void m2m_ovpd_stop_streaming(struct vb2_queue *q)
{
	struct m2m_ovpd_ctx *ctx = q->drv_priv;
	struct vb2_v4l2_buffer *src_vb, *dst_vb;
	int i;

	v4l2_m2m_update_stop_streaming_state(ctx->fh.m2m_ctx, q);

	/*
	 * Do NOT call m2m_ovpd_job_finish / v4l2_m2m_job_finish here.
	 * stop_streaming is called once per queue (twice total); each spurious
	 * v4l2_m2m_job_finish call triggers device_run on a tearing-down context,
	 * causing new TA pushes and heap corruption.
	 *
	 * Instead, explicitly complete any ACTIVE (already-dequeued) vb2 buffers
	 * tracked in our per-slot arrays — m2m_ovpd_vb2_finish checks state before
	 * calling v4l2_m2m_buf_done, so NULL / QUEUED slots are safely skipped.
	 * The second queue-stop call is a no-op here since all slots are NULL'd.
	 */
	for (i = 0; i < MAX_PENDING_BUFS; i++) {
		m2m_ovpd_vb2_finish(ctx->cur_src[i], NULL, VB2_BUF_STATE_ERROR);
		ctx->cur_src[i] = NULL;
		m2m_ovpd_vb2_finish(ctx->cur_dst1[i], NULL, VB2_BUF_STATE_ERROR);
		ctx->cur_dst1[i] = NULL;
		m2m_ovpd_vb2_finish(ctx->cur_dst2[i], NULL, VB2_BUF_STATE_ERROR);
		ctx->cur_dst2[i] = NULL;
	}

	if (V4L2_TYPE_IS_OUTPUT(q->type))
		while ((src_vb = v4l2_m2m_src_buf_remove(ctx->fh.m2m_ctx)))
			v4l2_m2m_buf_done(src_vb, VB2_BUF_STATE_ERROR);
	else
		while ((dst_vb = v4l2_m2m_dst_buf_remove(ctx->fh.m2m_ctx))) {
			vb2_set_plane_payload(&dst_vb->vb2_buf, 0, 0);
			v4l2_m2m_buf_done(dst_vb, VB2_BUF_STATE_ERROR);
		}
}

static const struct vb2_ops ovpd_qops = {
	.queue_setup     = m2m_ovpd_queue_setup,
	.buf_prepare     = m2m_ovpd_buf_prepare,
	.buf_finish      = m2m_ovpd_buf_finish,
	.buf_queue       = m2m_ovpd_buf_queue,
	.wait_prepare    = vb2_ops_wait_prepare,
	.wait_finish     = vb2_ops_wait_finish,
	.stop_streaming  = m2m_ovpd_stop_streaming,
	.start_streaming = m2m_ovpd_start_streaming,
};

static int m2m_vb2_queue_init(struct vb2_queue *vq, struct m2m_ovpd_ctx *ctx,
			      bool b_src_vq)
{
	int ret;

	if (!vq || !ctx)
		return -EINVAL;

	memset(vq, 0, sizeof(*vq));
	if (b_src_vq)
		vq->type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	else
		vq->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	vq->io_modes = VB2_MMAP | VB2_DMABUF;
	vq->drv_priv = ctx;
	vq->ops = &ovpd_qops;
	vq->mem_ops = &vb2_dma_contig_memops;
	vq->buf_struct_size = sizeof(struct v4l2_m2m_buffer);
	vq->timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_COPY;
	vq->lock = &ctx->lock;
	vq->dev = ctx->m2m_ovpd_dev->v4l2_dev.dev;
	vq->dma_attrs = DMA_ATTR_ALLOC_SINGLE_PAGES |
			    DMA_ATTR_NO_KERNEL_MAPPING;

	ret = vb2_queue_init(vq);
	if (ret)
		ovp_error("vb2_queue_init error (%d) for %s\n", ret,
			  b_src_vq ? "src_vq" : "dst_vq");

	return ret;
}

static int m2m_queue_init(void *priv,
			  struct vb2_queue *src_vq, struct vb2_queue *dst_vq)
{
	struct m2m_ovpd_ctx *ctx = priv;
	int ret;

	/* init src_q first
	 */
	ret = m2m_vb2_queue_init(src_vq, ctx, true);

	/* init dst_q if src_q init success
	 */
	if (!ret)
		ret = m2m_vb2_queue_init(dst_vq, ctx, false);

	return ret;
}

static int m2m_ovpd_querycap(struct file *file, void *fh,
			     struct v4l2_capability *cap)
{
	strscpy(cap->driver, SYNA_DRIVER_NAME, sizeof(cap->driver));
	strscpy(cap->card, SYNA_CARD_TYPE, sizeof(cap->card));
	snprintf(cap->bus_info, sizeof(cap->bus_info),
		 "platform: %s", SYNA_OVPD_NAME);
	return 0;
}

static int m2m_ovpd_enum_fmt(struct file *file, void *fh, struct v4l2_fmtdesc *f)
{
	const struct ovpd_fmt *fmt;

	if (f->index >= 1)
		return -EINVAL;

	fmt = &m2m_ovpd_format;
	f->pixelformat = fmt->pixelformat;

	return 0;
}

static int m2m_ovpd_g_fmt_out_mp(struct file *file, void *fh, struct v4l2_format *fmt)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(fh);
	struct v4l2_pix_format_mplane *pix_mp = &fmt->fmt.pix_mp;

	*pix_mp = ctx->sink_fmt;

	return 0;
}

static int m2m_ovpd_g_fmt_cap_mp(struct file *file, void *fh, struct v4l2_format *fmt)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(fh);
	struct v4l2_pix_format_mplane *pix_mp = &fmt->fmt.pix_mp;

	*pix_mp = ctx->src_fmt;

	return 0;
}

static int m2m_ovpd_try_fmt_out_mp(struct file *file, void *fh, struct v4l2_format *fmt)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(fh);
	struct v4l2_pix_format_mplane *pix_mp = &fmt->fmt.pix_mp;
	const struct ovpd_fmt *format;

	/* Check if the hardware supports the requested format
	 */
	format = m2m_ovpd_find_fmt(pix_mp->pixelformat);
	if (!format) {
		ovp_error("Unknown format 0x%x\n", pix_mp->pixelformat);
		m2m_ovpd_g_fmt_out_mp(file, fh, fmt);
		return -EINVAL;
	}

	if (format->pixelformat != V4L2_PIX_FMT_NV12M) {
		ovp_error("%s: format not V4L2_PIX_FMT_NV12M\n", __func__);
		return -EINVAL;
	}

	switch (pix_mp->field) {
	case V4L2_FIELD_NONE:
	case V4L2_FIELD_INTERLACED:
	case V4L2_FIELD_ALTERNATE:
	case V4L2_FIELD_INTERLACED_TB:
	case V4L2_FIELD_INTERLACED_BT:
		break;
	case V4L2_FIELD_ANY:
		pix_mp->field = V4L2_FIELD_INTERLACED;
		break;
	default:
		ovp_error("%s: Unsupported field 0x%x\n", __func__, pix_mp->field);
		return -EINVAL;
	}

	v4l2_fill_pixfmt_mp(pix_mp,
			    pix_mp->pixelformat,
			    pix_mp->width,
			    pix_mp->height);

	pix_mp->plane_fmt[0].sizeimage = pix_mp->width * pix_mp->height;
	pix_mp->plane_fmt[0].bytesperline = pix_mp->width;
	pix_mp->plane_fmt[1].sizeimage = (pix_mp->width * pix_mp->height) / 2;
	pix_mp->plane_fmt[1].bytesperline = pix_mp->width;

	ctx->output_frame_info.fmt = *format;
	ctx->output_frame_info.u_width = pix_mp->width;
	ctx->output_frame_info.u_height = pix_mp->height;
	ctx->sink_fmt = *pix_mp;

	return 0;
}

static int m2m_ovpd_try_fmt_cap_mp(struct file *file, void *fh, struct v4l2_format *fmt)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(fh);
	struct v4l2_pix_format_mplane *pix_mp = &fmt->fmt.pix_mp;
	const struct ovpd_fmt *format;

	/* Check if the hardware supports the requested format
	 */
	format = m2m_ovpd_find_fmt(pix_mp->pixelformat);
	if (!format) {
		ovp_debug("Unknown format 0x%x\n", pix_mp->pixelformat);
		m2m_ovpd_g_fmt_cap_mp(file, fh, fmt);
		return -EINVAL;
	}
	if (format->pixelformat != V4L2_PIX_FMT_NV12M) {
		ovp_error("%s: format not V4L2_PIX_FMT_NV12M\n", __func__);
		return -EINVAL;
	}

	switch (pix_mp->field) {
	case V4L2_FIELD_NONE:
		break;
	case V4L2_FIELD_ANY:
		pix_mp->field = V4L2_FIELD_NONE;
		break;
	default:
		ovp_error("%s: Unsupported field 0x%x\n", __func__, pix_mp->field);
		return -EINVAL;
	}

	v4l2_fill_pixfmt_mp(pix_mp,
			    pix_mp->pixelformat,
			    pix_mp->width,
			    pix_mp->height);

	pix_mp->plane_fmt[0].sizeimage = pix_mp->width * pix_mp->height;
	pix_mp->plane_fmt[0].bytesperline = pix_mp->width;
	pix_mp->plane_fmt[1].sizeimage = (pix_mp->width * pix_mp->height) / 2;
	pix_mp->plane_fmt[1].bytesperline = pix_mp->width;

	ctx->capture_frame_info.fmt = *format;
	ctx->capture_frame_info.u_width = pix_mp->width;
	ctx->capture_frame_info.u_height = pix_mp->height;
	ctx->src_fmt = *pix_mp;

	return 0;
}

static int m2m_ovpd_s_fmt_mp(struct file *file, void *fh, struct v4l2_format *fmt)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(fh);
	struct vb2_queue *vq;
	int ret;

	/* Try setting the format and validate based on buffer type */
	if (fmt->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE)
		ret = m2m_ovpd_try_fmt_out_mp(file, fh, fmt);
	else if (fmt->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		ret = m2m_ovpd_try_fmt_cap_mp(file, fh, fmt);
	else
		ret = -EINVAL;

	if (ret < 0) {
		ovp_error("Cannot set format (queue type %d)\n",
			  fmt->type);
		return ret;
	}

	/* Get the V4L2 queue based on the buffer type */
	vq = v4l2_m2m_get_vq(ctx->fh.m2m_ctx, fmt->type);
	if (vb2_is_streaming(vq)) {
		ovp_error("queue (%d) busy\n", fmt->type);
		return -EBUSY;
	}

	return ret;
}

static int m2m_ovpd_ioctl_qbuf(struct file *file, void *priv,
			       struct v4l2_buffer *buf)
{
	struct v4l2_fh *fh = file->private_data;

	return v4l2_m2m_qbuf(file, fh->m2m_ctx, buf);
}

static int m2m_ovpd_streamon(struct file *file, void *priv,
			     enum v4l2_buf_type type)
{
	struct v4l2_fh *fh = file->private_data;

	return v4l2_m2m_streamon(file, fh->m2m_ctx, type);
}

static const struct v4l2_ioctl_ops ovpd_ioctl_ops = {
	.vidioc_querycap                = m2m_ovpd_querycap,
	.vidioc_enum_fmt_vid_cap        = m2m_ovpd_enum_fmt,
	.vidioc_enum_fmt_vid_out        = m2m_ovpd_enum_fmt,
	.vidioc_g_fmt_vid_out_mplane    = m2m_ovpd_g_fmt_out_mp,
	.vidioc_g_fmt_vid_cap_mplane    = m2m_ovpd_g_fmt_cap_mp,
	.vidioc_try_fmt_vid_out_mplane	= m2m_ovpd_try_fmt_out_mp,
	.vidioc_try_fmt_vid_cap_mplane	= m2m_ovpd_try_fmt_cap_mp,
	.vidioc_s_fmt_vid_out_mplane	= m2m_ovpd_s_fmt_mp,
	.vidioc_s_fmt_vid_cap_mplane    = m2m_ovpd_s_fmt_mp,
	.vidioc_reqbufs                 = v4l2_m2m_ioctl_reqbufs,
	.vidioc_create_bufs             = v4l2_m2m_ioctl_create_bufs,
	.vidioc_expbuf                  = v4l2_m2m_ioctl_expbuf,
	.vidioc_querybuf                = v4l2_m2m_ioctl_querybuf,
	.vidioc_qbuf                    = m2m_ovpd_ioctl_qbuf,
	.vidioc_dqbuf                   = v4l2_m2m_ioctl_dqbuf,
	.vidioc_prepare_buf             = v4l2_m2m_ioctl_prepare_buf,
	.vidioc_streamon                = m2m_ovpd_streamon,
	.vidioc_streamoff               = v4l2_m2m_ioctl_streamoff,
};

/*
 * m2m_ovpd_open - Open function for the ovpd V4L2 driver
 *
 * This function is called when a user-space application opens the
 * ovpd device.
 *
 * Parameters:
 * @file: Pointer to the file structure representing the opened device file
 *
 * Returns:
 * 0 on success, negative error code on failure
 */
static int m2m_ovpd_open(struct file *file)
{
	struct m2m_ovpd_dev *ovpd = video_drvdata(file);
	struct m2m_ovpd_ctx *ctx;
	int ret;

	/* Acquire the ovpd device lock */
	if (mutex_lock_interruptible(&ovpd->lock))
		return -ERESTARTSYS;

	/* Check if device is already opened */
	if (atomic_inc_return(&ovpd->ref_count) > OVPD_MAX_CONTEXTS) {
		ovp_error("Device is already opened %d times.\n", OVPD_MAX_CONTEXTS);
		atomic_dec(&ovpd->ref_count);
		ret = -EBUSY;
		goto unlock;
	}

	/* Allocate memory for both context and node */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		ret = -ENOMEM;
		atomic_dec(&ovpd->ref_count);
		goto unlock;
	}
	ctx->m2m_ovpd_dev = ovpd;

	ctx->deint_param.b_is_whole_frame = 1;
	ctx->deint_param.b_is_one_field = 0;
	ctx->deint_param.u_priority = 1;

	/* Initialize the V4L2 file handle */
	v4l2_fh_init(&ctx->fh, ovpd->m2m.vdev);

	/* Use separate control handler per file handle */
	file->private_data = &ctx->fh;
	v4l2_fh_add(&ctx->fh);

	/* Setup the device context for mem2mem mode */
	ctx->fh.m2m_ctx = v4l2_m2m_ctx_init(ovpd->m2m.m2m_dev, ctx, m2m_queue_init);
	if (IS_ERR(ctx->fh.m2m_ctx)) {
		ovp_error("Failed to initialize m2m context\n");
		ret = PTR_ERR(ctx->fh.m2m_ctx);
		goto error_ctx_fail;
	}

	/* Assign a buf_id unique among active contexts. */
	ctx->buf_id = ovpd_alloc_buf_id(ovpd);
	if (!ctx->buf_id) {
		ovp_error("Failed to allocate unique buf_id\n");
		ret = -EBUSY;
		goto error_ctx_fail;
	}

	/* Initialize timeout work item for this context. */
	INIT_DELAYED_WORK(&ctx->ovpd_delayed_work, delayed_m2m_ovpd_work_function);
	atomic_set(&ctx->job_in_flight, 0);
	atomic_set(&ctx->torn_down, 0);

	/* Each context has been assigned with each own lock */
	mutex_init(&ctx->lock);

	init_completion(&ctx->work_done);

	ovp_debug("driver opened, ctx = 0x%p\n", ctx);

	INIT_LIST_HEAD(&ctx->list);
	/* Add the context into the list for ovpd */
	list_add_tail(&ctx->list, &ovpd->ctx_list_head);

	/* Release the ovpd device lock */
	mutex_unlock(&ovpd->lock);

	/* Return success status */
	return 0;

error_ctx_fail:
	if (!IS_ERR_OR_NULL(ctx->fh.m2m_ctx))
		v4l2_m2m_ctx_release(ctx->fh.m2m_ctx);
	v4l2_fh_del(&ctx->fh);
	v4l2_fh_exit(&ctx->fh);
	kfree(ctx);
	atomic_dec(&ovpd->ref_count);
unlock:
	/* Release the ovpd device lock */
	mutex_unlock(&ovpd->lock);

	/* Return the error code */
	return ret;
}

/*
 * m2m_ovpd_release - Release function for the ovpd V4L2 driver
 *
 * This function is called when a user-space application releases its
 * access to the ovpd device.
 *
 * Parameters:
 * @file: Pointer to the file structure representing the opened device file
 *
 * Returns:
 * 0 on success, negative error code on failure
 */
static int m2m_ovpd_release(struct file *file)
{
	struct m2m_ovpd_ctx *ctx = fh_to_ctx(file->private_data);
	struct m2m_ovpd_dev *ovpd = ctx->m2m_ovpd_dev;
	int ret, index;
	u32 frames_waiting;
	int drain_iter;

	/* Acquire the ovpd device lock */
	mutex_lock(&ovpd->lock);

	/* Decrement the reference count */
	atomic_dec_return(&ovpd->ref_count);

	/* CRITICAL: Mark context as torn down to prevent ISR from using freed memory */
	atomic_set(&ctx->torn_down, 1);

	/* Remove from context list FIRST to atomically prevent ISR from finding this ctx */
	if (!list_empty(&ctx->list))
		list_del(&ctx->list);

	/*
	 * Release global device lock early: remaining teardown may block in
	 * V4L2/TEE paths and does not require ctx list serialization anymore.
	 */
	mutex_unlock(&ovpd->lock);

	/* Now safe to teardown: ISR can no longer access this context */
	/* Prevent delayed work from running and wait for any in-flight work to finish */
	atomic_set(&ctx->job_in_flight, 0);
	cancel_delayed_work_sync(&ctx->ovpd_delayed_work);

	/*
	 * Signal work_done so that v4l2_m2m_cancel_job -> job_abort does not
	 * block indefinitely: ISR and timeout are already disabled by this point
	 * and will never fire a completion.
	 */
	complete(&ctx->work_done);

	/* Release the V4L2 mem2mem (m2m) context associated with the context file handle.
	 * This internally calls stop_streaming (which uses ctx->lock via vb2 queues),
	 * so mutex_destroy must happen AFTER this call.
	 */
	v4l2_m2m_ctx_release(ctx->fh.m2m_ctx);

	/* Remove the context file handle from the V4L2 framework */
	v4l2_fh_del(&ctx->fh);

	/* Clean up the context file handle resources */
	v4l2_fh_exit(&ctx->fh);

	/* Destroy context specific lock only after all V4L2/vb2 cleanup is done */
	mutex_destroy(&ctx->lock);

	/* Drain all queued TA frames for this ctx to avoid stale state reuse. */
	if (ctx->first_push_done) {
		struct ovp_msg_header ovp_msg;

		drain_iter = 0;
		do {
			ret = syna_ovpd_ca_get_no_of_frames_waiting(ctx->buf_id, &frames_waiting);
			if (ret < 0) {
				ovp_error("%s: get_no_of_frames_waiting failed (%d)\n",
					  __func__, ret);
				break;
			}

			if (!frames_waiting)
				break;

			memset(&ovp_msg, 0, sizeof(struct ovp_msg_header));
			ret = syna_ovpd_ca_release_buffer(ctx->buf_id, &ovp_msg);
			if (ret < 0) {
				ovp_error("%s: syna_ovpd_ca_release_buffer failed (%d)\n",
					  __func__, ret);
				break;
			}
		} while (++drain_iter < (MAX_PENDING_BUFS + 4));

		if (drain_iter >= (MAX_PENDING_BUFS + 4))
			ovp_error("%s: timed out draining pending TA frames for buf_id %u\n",
				  __func__, ctx->buf_id);
	}

	for (index = 0; index < MAX_PENDING_BUFS; index++) {
		kfree(ctx->cur_desc[index]);
		ctx->cur_desc[index] = NULL;
	}

	/* Free the memory allocated for the context */
	kfree(ctx);

	/* Return success status */
	return 0;
}

static const struct v4l2_file_operations ovpd_fops = {
	.owner          = THIS_MODULE,
	.open           = m2m_ovpd_open,
	.release        = m2m_ovpd_release,
	.poll           = v4l2_m2m_fop_poll,
	.unlocked_ioctl = video_ioctl2,
	.mmap           = v4l2_m2m_fop_mmap,
};

/*
 * m2m_ovpd_register_device - Register the ovpd device with V4L2 and video-dev
 *
 * This function initializes the ovpd device, sets up its parameters,
 * and registers it with both the V4L2 and video-dev frameworks.
 *
 * Parameters:
 * @ovpd: Pointer to the ovpd device structure
 *
 * Returns:
 * 0 on success, negative error code on failure
 */
static int m2m_ovpd_register_device(struct m2m_ovpd_dev *ovpd)
{
	int ret;

	/* Initialize video device structure parameters */
	ovpd->vdev.fops        = &ovpd_fops;
	ovpd->vdev.ioctl_ops   = &ovpd_ioctl_ops;
	ovpd->vdev.release     = video_device_release_empty;
	ovpd->vdev.lock        = NULL;
	ovpd->vdev.vfl_dir     = VFL_DIR_M2M;
	ovpd->vdev.v4l2_dev    = &ovpd->v4l2_dev;
	ovpd->vdev.device_caps = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_M2M_MPLANE;
	snprintf(ovpd->vdev.name, sizeof(ovpd->vdev.name), "%s.%d",
		 OVPD_NAME, ovpd->id);

	ovpd->m2m.vdev = &ovpd->vdev;
	ovpd->m2m.m2m_dev = v4l2_m2m_init(&ovpd_m2m_ops);
	if (IS_ERR(ovpd->m2m.m2m_dev)) {
		ovp_error("failed to initialize v4l2-m2m device\n");
		return PTR_ERR(ovpd->m2m.m2m_dev);
	}

	/* Register the video device with V4L2 and video-dev frameworks */
	ret = video_register_device(&ovpd->vdev, VFL_TYPE_VIDEO, -1);
	if (ret) {
		ovp_error("%s(): failed to register video device\n", __func__);
		v4l2_m2m_release(ovpd->m2m.m2m_dev);
		return ret;
	}

	video_set_drvdata(&ovpd->vdev, ovpd);

	/* Return success status */
	return 0;
}

/*
 * m2m_ovpd_unregister_device - Unregister the ovpd device from V4L2 and video-dev
 *
 * This function unregisters the ovpd device from both the V4L2 and video-dev frameworks
 * and releases associated resources.
 *
 * Parameters:
 * @ovpd: Pointer to the ovpd device structure
 */
static void m2m_ovpd_unregister_device(struct m2m_ovpd_dev *ovpd)
{
	/* Release ovpd mem2mem (m2m) context */
	if (ovpd->m2m.m2m_dev)
		v4l2_m2m_release(ovpd->m2m.m2m_dev);

	/* Unregister the video device from V4L2 and video-dev frameworks */
	video_unregister_device(ovpd->m2m.vdev);
}

/*
 * ovpd_v4l2_remove - Remove function for the ovpd platform driver
 *
 * This function is called when the ovpd platform driver is being removed.
 *
 * Parameters:
 * @pdev: Pointer to the platform device structure
 *
 * Returns:
 * 0 on success, negative error code on failure
 */
static void ovpd_v4l2_remove(struct platform_device *pdev)
{
	int i;
	INTR_MSG intr_msg;

	/* Disable driver interrupt */
	intr_msg.Enable = 0;
	wrap_ovp_drv_set_intr(&intr_msg);

	/* Stop kthread */
	kthread_stop(ovp_isr_task);

	/* clear mem at TA side */
	syna_ovpd_ca_destroy();
	/* free registered ISR */
	wrap_ovp_drv_free_isr();
	syna_ovpd_ca_deinitialize();

	/* Unregister the ovpd device from the V4L2 framework */
	m2m_ovpd_unregister_device(ovpd);

	/* Unregister the V4L2 device */
	v4l2_device_unregister(&ovpd->v4l2_dev);

	/* Destroy the ovpd device lock */
	mutex_destroy(&ovpd->lock);

	VPP_MEM_FreeMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA, &ovp_shm_handle);
	/* Free up the VBUF memory */
	for (i = 0; i < MAX_VBUF_INFO; i++) {
		VPP_MEM_FreeMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA,
				   &vpp_disp_info_shm_handle[i]);
	}
	/* Free up the device memory list used for VBUF*/
	if (ovpd->mem_list != 0) {
		VPP_MEM_DeInitMemory(ovpd->mem_list);
		kfree(ovpd->mem_list);
	}

	/* Log driver unload information */
	ovp_debug("ovpd driver unloaded\n");
}

/*
 * ovpd_v4l2_probe - Probe function for the ovpd platform driver
 *
 * This function is called when a ovpd platform device is detected
 * and being probed.
 *
 * Parameters:
 * @pdev: Pointer to the platform device structure
 *
 * Returns:
 * 0 on success, negative error code on failure
 */
static int ovpd_v4l2_probe(struct platform_device *pdev)
{
	int ret;
	int i;
	VBUF_INFO *vbufinfo;
	VPP_MEM *shm_handle;
	INTR_MSG intr_msg;
	unsigned int ui_shm_size, ui_shm_PA;

	/* Allocate memory for the ovpd device structure */
	ovpd = kzalloc(sizeof(*ovpd), GFP_KERNEL);
	if (!ovpd)
		return -ENOMEM;

	/* Initialize the ovpd device lock */
	mutex_init(&ovpd->lock);

	snprintf(ovpd->v4l2_dev.name, sizeof(ovpd->v4l2_dev.name),
		 "%s", OVP_MODULE_NAME);

	dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	vb2_dma_contig_set_max_seg_size(&pdev->dev, DMA_BIT_MASK(32));

	/* Store platform device pointer for later use */
	ovpd->pdev = pdev;
	ovpd->dev = &pdev->dev;
	ovpd->alloc_dev = &pdev->dev;

	/* Register the ovpd device with the V4L2 framework */
	ret = v4l2_device_register(&pdev->dev, &ovpd->v4l2_dev);
	if (ret) {
		ovp_error("failed to register v4l2 device!\n");
		goto err_ovpd_fail;
	}

	/* Init the m2m ctx list head */
	INIT_LIST_HEAD(&ovpd->ctx_list_head);

	/* Register the ovpd device with the video-dev framework */
	ret = m2m_ovpd_register_device(ovpd);
	if (ret) {
		ovp_error("failed to register m2m device!\n");
		goto err_reg_fail;
	}

	/* Register ISR now with the OVP driver
	 * use wrap_ovp_drv_get_isr_msg() in ISR task to wait for HW message
	 */
	ret = wrap_ovp_drv_register_isr();
	if (ret) {
		ovp_error("failed to register driver ISR!\n");
		goto err_drv_open_fail;
	}

	ret = syna_ovpd_ca_initialize();
	if (ret < 0) {
		ovp_error("ovp driver CA init failed %d!\n", ret);
		goto err_vpp_ca_fail;
	}

	/* Enable driver interrupt */
	intr_msg.Enable = 1;
	wrap_ovp_drv_set_intr(&intr_msg);

	/* Allocate the memory list for VBUF management*/
	ovpd->mem_list = kzalloc(sizeof(VPP_MEM_LIST), GFP_KERNEL);

	if (!ovpd->mem_list) {
		ovp_error("%s alloc mem for mem_list failed\n", __func__);
		goto err_vpp_mem_fail;
	}

	ovpd->mem_list->dev = &pdev->dev;

	ret = VPP_MEM_InitMemory(ovpd->mem_list);
	if (ret != 0) {
		ovp_error("Can't Initialize mem_list\n");
		goto err_vpp_mem2_fail;
	}

	/* Allocate memory for TA command buffer HW */
	ovp_shm_handle.size = OVPD_HW_CMD_SHM_SIZE;
	ret = VPP_MEM_AllocateMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA,
				     &ovp_shm_handle, GFP_KERNEL | __GFP_NOWARN);
	if (ret != 0) {
		ovp_error("VPP internal memory allocation: Not enough memory!!!!!!!!\n");
		goto err_vpp_mem3_fail;
	}

	ui_shm_size = ovp_shm_handle.size;
	ui_shm_PA = (uintptr_t)ovp_shm_handle.p_addr;
	ret = syna_ovpd_ca_create(ui_shm_PA, ui_shm_size);
	if (ret) {
		ovp_error("failed to create shm memory !\n");
		goto err_vpp_mem4_fail;
	}

	/* Allocate the VBUFs inside the mem list */
	for (i = 0; i < MAX_VBUF_INFO; i++) {
		vbufinfo = &vpp_disp_desc_array[i];
		shm_handle = &vpp_disp_info_shm_handle[i];

		shm_handle->size = OVP_SHM_4K_ALIGN_ROUNDUP(sizeof(VPP_VBUF));
		ret = VPP_MEM_AllocateMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA, shm_handle, 0);
		if (ret != 0) {
			ovp_error("%s %d  VPP Mem alloc failed!\n", __func__, __LINE__);
			goto err_vpp_mem5_fail;
		}

		vbufinfo->hShm_vbuf = shm_handle;
		vbufinfo->pVppVbufInfo_virt = shm_handle->k_addr;
		vbufinfo->pVppVbufInfo_phy = (phys_addr_t)shm_handle->p_addr;

		ovp_debug("Init vpp_disp_info_phys_addr[%d]=[phy:%p, virt:%p], hShm_vbuf: %p\n",
			  i, (void *)vbufinfo->pVppVbufInfo_phy,
			  (void *)vbufinfo->pVppVbufInfo_virt, vbufinfo->hShm_vbuf);
	}

	/* Start kthread to handle ISR from OVP HW */
	ovp_isr_task = kthread_run(OVP_ISR_Handler, ovpd, "OVP ISR Thread");
	if (IS_ERR(ovp_isr_task)) {
		ret = PTR_ERR(ovp_isr_task);
		ovp_error("%s OVP ISR Thread creation failed (%d)!!\n", __func__, ret);
		goto err_vpp_mem5_fail;
	}

	/* Log successful registration information */
	ovp_debug("%s%d registered as /dev/video%d\n", OVPD_NAME,
		  ovpd->id, ovpd->vdev.num);

	/* Return success status */
	return 0;

err_vpp_mem5_fail:
	syna_ovpd_ca_destroy();
	for (i = 0; i < MAX_VBUF_INFO; i++) {
		if (vpp_disp_desc_array[i].hShm_vbuf) {
			VPP_MEM_FreeMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA,
					   &vpp_disp_info_shm_handle[i]);
		}
	}
err_vpp_mem4_fail:
	VPP_MEM_FreeMemory(ovpd->mem_list, VPP_MEM_TYPE_DMA, &ovp_shm_handle);
err_vpp_mem3_fail:
	VPP_MEM_DeInitMemory(ovpd->mem_list);
err_vpp_mem2_fail:
	kfree(ovpd->mem_list);
err_vpp_mem_fail:
	syna_ovpd_ca_deinitialize();
err_vpp_ca_fail:
	wrap_ovp_drv_free_isr();
err_drv_open_fail:
	m2m_ovpd_unregister_device(ovpd);
err_reg_fail:
	/* Unregister the V4L2 device */
	v4l2_device_unregister(&ovpd->v4l2_dev);
err_ovpd_fail:
	/* Free allocated memory for the ovpd device structure */
	mutex_destroy(&ovpd->lock);
	kfree(ovpd);

	/* Transient failure: defer probe */
	if (ret == -EAGAIN)
		ret = -EPROBE_DEFER;

	/* Return the error code */
	return ret;
}

static int ovpd_v4l2_suspend(struct device *dev)
{
	return 0;
}

static int ovpd_v4l2_resume(struct device *dev)
{
	return 0;
}

/* Compatible string for device tree match */
static const struct of_device_id ovpd_match_types[] = {
	{.compatible = "syna,ovp-deint"},
	{}
};

MODULE_DEVICE_TABLE(of, ovpd_match_types);

static SIMPLE_DEV_PM_OPS(ovpd_v4l2_pmops, ovpd_v4l2_suspend, ovpd_v4l2_resume);

static struct platform_driver ovpd_driver = {
	.probe		= ovpd_v4l2_probe,
	.remove         = ovpd_v4l2_remove,
	.driver         = {
		.name           = OVPD_NAME,
		.owner		= THIS_MODULE,
		.of_match_table = ovpd_match_types,
		.pm		= &ovpd_v4l2_pmops,
	},
};

static int __init ovpd_v4l2_init(void)
{
	return platform_driver_register(&ovpd_driver);
}

static void __exit ovpd_v4l2_exit(void)
{
	platform_driver_unregister(&ovpd_driver);
}
module_init(ovpd_v4l2_init);
module_exit(ovpd_v4l2_exit);

MODULE_AUTHOR("Synaptics");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("V4L2 OVPD Driver");

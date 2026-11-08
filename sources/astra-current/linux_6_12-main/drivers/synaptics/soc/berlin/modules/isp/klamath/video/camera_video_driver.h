/* SPDX-License-Identifier: GPL-2.0 */

/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CAMERA_VIDEO_DRIVER_H__
#define __CAMERA_VIDEO_DRIVER_H__

#include <linux/list.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mc.h>
#include <media/v4l2-ctrls.h>
#include <media/videobuf2-dma-contig.h>

/* Driver identification and configuration */
#define CAMERA_VIDEO_NAME "camera-video"		/**< Platform driver name */
#define CAMERA_VIDEO_PORT_MAX 2			/**< Maximum number of video ports */

/* Video format alignment requirements */
#define CAMERA_VIDEO_WIDTH_ALIGN	16		/**< Width alignment in pixels */
#define CAMERA_VIDEO_HEIGHT_ALIGN	8		/**< Height alignment in pixels */

/* Minimum video resolution constraints */
#define CAMERA_VIDEO_MIN_WIDTH	32		/**< Minimum width in pixels */
#define CAMERA_VIDEO_MIN_HEIGHT	16		/**< Minimum height in pixels */

/**
 * struct camera_video_dev - Individual video device instance
 * @camera_mdev: Pointer to parent camera media device
 * @video: V4L2 video device instance
 * @pad: Media controller pad for this video device
 * @queue: VB2 queue for buffer management
 * @video_lock: Mutex for video device operations
 * @format: Current video format configuration
 * @pipeline: Pipeline identifier for media controller
 *
 * This structure represents a single video device node (e.g., /dev/video8).
 * Each camera port creates one instance of this structure for video capture.
 * The driver currently supports up to 2 video devices as per user requirements.
 */
struct camera_video_dev {
	struct camera_media_dev *camera_mdev;	/**< Parent media device */
	struct video_device *video;		/**< V4L2 video device */
	struct media_pad pad;			/**< Media controller pad */
	struct vb2_queue queue;			/**< Buffer queue */
	struct mutex video_lock;		/**< Video operations mutex */
	struct v4l2_format format;		/**< Current video format */
	uint32_t pipeline;			/**< Pipeline identifier */
	struct v4l2_subdev *isp_sd;		/**< ISP subdevice */
};

/**
 * struct camera_media_dev - Main camera media device structure
 * @id: Device identifier from device tree
 * @dev: Pointer to platform device
 * @mdev: Media controller device instance
 * @v4l2_dev: V4L2 device instance
 * @notifier: Async notifier for subdevice binding
 * @ports: Number of active video ports (limited to 2)
 * @video_devs: Array of video device instances
 *
 * This is the main structure that manages the entire camera video driver.
 * It coordinates multiple video devices, media controller integration,
 * and subdevice management through the async notifier framework.
 */
struct camera_media_dev {
	int id;					/**< Device ID from DT */
	struct device *dev;			/**< Platform device */
	struct media_device mdev;		/**< Media controller */
	struct v4l2_device v4l2_dev;		/**< V4L2 device */
	struct v4l2_async_notifier notifier;	/**< Async notifier */
	int ports;				/**< Active port count */
	struct camera_video_dev *video_devs[CAMERA_VIDEO_PORT_MAX]; /**< Video devices */
};

/**
 * struct camera_video_fmt_info - Format conversion information
 * @fourcc: V4L2 pixel format (FOURCC code)
 * @mbus: Media bus format code
 *
 * This structure provides mapping between V4L2 pixel formats and
 * media bus formats for format conversion operations.
 */
struct camera_video_fmt_info {
	uint32_t fourcc;	/**< V4L2 pixel format */
	uint32_t mbus;		/**< Media bus format */
};

struct remote_node_handle {
	struct list_head link;
	struct fwnode_handle *remote;
};

#endif

/* SPDX-License-Identifier: GPL-2.0 */

/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CAMERA_VIDEO_REGISTER_H__
#define __CAMERA_VIDEO_REGISTER_H__

#include "camera_video_driver.h"

/**
 * camera_video_register - Register a video device for specified port
 * @camera_mdev: Camera media device instance
 * @port: Port number to register (0-based index)
 *
 * This function creates and registers a V4L2 video device for the specified
 * port. It initializes the video device structure, VB2 queue, media pad,
 * and all necessary V4L2 operations for video capture functionality.
 *
 * The function performs:
 * - Video device allocation and initialization
 * - VB2 queue setup for buffer management
 * - Media pad registration for media controller integration
 * - V4L2 IOCTL operations setup
 * - Default format configuration
 *
 * Return: 0 on success, negative error code on failure
 */
int camera_video_register(struct camera_media_dev *camera_mdev, int port);

/**
 * camera_video_unregister - Unregister video device for specified port
 * @camera_mdev: Camera media device instance
 * @port: Port number to unregister (0-based index)
 *
 * This function unregisters and cleans up the V4L2 video device for the
 * specified port. It performs proper cleanup of all resources including
 * video device, VB2 queue, and media pad.
 *
 * The function performs:
 * - Video device unregistration from V4L2 subsystem
 * - VB2 queue cleanup and buffer release
 * - Media pad cleanup
 * - Memory deallocation
 *
 * Return: 0 on success, negative error code on failure
 */
int camera_video_unregister(struct camera_media_dev *camera_mdev, int port);

#endif /* __CAMERA_VIDEO_REGISTER_H__ */

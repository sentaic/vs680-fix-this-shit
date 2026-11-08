// SPDX-License-Identifier: GPL-2.0
/*
* Copyright (C) 2021 - 2023 Synaptics Incorporated
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/

#ifndef __CAMERA_ISP_WB_SYSFS_H__
#define __CAMERA_ISP_WB_SYSFS_H__

struct camera_isp_dev;

int camera_isp_create_wb_sysfs(struct camera_isp_dev *isp_dev);
void camera_isp_remove_wb_sysfs(struct camera_isp_dev *isp_dev);

#endif /* __CAMERA_ISP_WB_SYSFS_H__ */

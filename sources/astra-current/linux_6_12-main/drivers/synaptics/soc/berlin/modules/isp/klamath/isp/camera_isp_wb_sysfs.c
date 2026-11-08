// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/*
 * White balance sysfs support for Synaptics ISP
 */

#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include "camera_isp_driver.h"
#include "camera_isp_wb_sysfs.h"

#define WB_SYSFS_SHOW(name, expr) \
static ssize_t name##_show(struct device *dev, \
		struct device_attribute *attr, char *buf) \
{ \
	struct camera_isp_dev *isp_dev = dev_get_drvdata(dev); \
	return sprintf(buf, "%d\n", (expr)); \
}

#define WB_SYSFS_STORE(name, expr) \
static ssize_t name##_store(struct device *dev, \
		struct device_attribute *attr, const char *buf, size_t count) \
{ \
	struct camera_isp_dev *isp_dev = dev_get_drvdata(dev); \
	int val; \
	int pipe_id = isp_dev->active_pipe_id; \
	if (kstrtoint(buf, 10, &val) == 0) { \
		(expr) = val; \
		if (isp_dev->wb_config.wb_en && \
			pipe_id >= 0 && pipe_id < MAX_PL && \
			isp_dev->pipe[pipe_id]) { \
			struct isp_ctrl ctrl = { \
				.id = CID_WB_CONFIG, \
				.handler = isp_dev->pipe[pipe_id], \
				.cfg = &isp_dev->wb_config \
			}; \
			wb_s_ctrl(&ctrl); \
		} \
	} \
	return count; \
}

#define WB_SYSFS_ATTR(name, expr) \
	WB_SYSFS_SHOW(name, expr) \
	WB_SYSFS_STORE(name, expr) \
	static DEVICE_ATTR_RW(name)

static struct class *wb_class;
static struct device *wb_class_dev;

WB_SYSFS_ATTR(wb_mode, isp_dev->wb_config.wb_mode);
WB_SYSFS_ATTR(wb_p00_mantissa, isp_dev->wb_config.wb_p00_mantissa);
WB_SYSFS_ATTR(wb_p00_exponent, isp_dev->wb_config.wb_p00_exponent);
WB_SYSFS_ATTR(wb_p01_mantissa, isp_dev->wb_config.wb_p01_mantissa);
WB_SYSFS_ATTR(wb_p01_exponent, isp_dev->wb_config.wb_p01_exponent);
WB_SYSFS_ATTR(wb_p10_mantissa, isp_dev->wb_config.wb_p10_mantissa);
WB_SYSFS_ATTR(wb_p10_exponent, isp_dev->wb_config.wb_p10_exponent);
WB_SYSFS_ATTR(wb_p11_mantissa, isp_dev->wb_config.wb_p11_mantissa);
WB_SYSFS_ATTR(wb_p11_exponent, isp_dev->wb_config.wb_p11_exponent);
WB_SYSFS_ATTR(input_sel, isp_dev->wb_config.input_sel);

static struct attribute *wb_attrs[] = {
	&dev_attr_wb_mode.attr,
	&dev_attr_wb_p00_mantissa.attr,
	&dev_attr_wb_p00_exponent.attr,
	&dev_attr_wb_p01_mantissa.attr,
	&dev_attr_wb_p01_exponent.attr,
	&dev_attr_wb_p10_mantissa.attr,
	&dev_attr_wb_p10_exponent.attr,
	&dev_attr_wb_p11_mantissa.attr,
	&dev_attr_wb_p11_exponent.attr,
	&dev_attr_input_sel.attr,
	NULL,
};

static const struct attribute_group wb_attr_group = {
	.attrs = wb_attrs,
};

int camera_isp_create_wb_sysfs(struct camera_isp_dev *isp_dev) {
	int ret;
	wb_class = class_create("camera_isp_wb");
	if (IS_ERR(wb_class)) {
		pr_err("Failed to create camera_isp_wb class\n");
		return PTR_ERR(wb_class);
	}
	wb_class_dev = device_create(wb_class, NULL, MKDEV(0, 0), isp_dev, "wb_config");
	if (IS_ERR(wb_class_dev)) {
		pr_err("Failed to create camera_isp_wb class device\n");
		class_destroy(wb_class);
		return PTR_ERR(wb_class_dev);
	}
	ret = sysfs_create_group(&wb_class_dev->kobj, &wb_attr_group);
	if (ret) {
		pr_err("Failed to create wb sysfs group\n");
		device_destroy(wb_class, MKDEV(0, 0));
		class_destroy(wb_class);
		return ret;
	}
	return 0;
}

void camera_isp_remove_wb_sysfs(struct camera_isp_dev *isp_dev) {
	sysfs_remove_group(&wb_class_dev->kobj, &wb_attr_group);
	device_destroy(wb_class, MKDEV(0, 0));
	class_destroy(wb_class);
}

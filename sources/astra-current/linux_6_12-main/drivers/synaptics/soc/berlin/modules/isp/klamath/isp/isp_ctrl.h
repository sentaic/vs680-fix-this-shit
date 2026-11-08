// SPDX-License-Identifier: GPL-2.0
/*
* Copyright (C) 2025 Synaptics Incorporated
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/

#ifndef __ISP_CTRL_H__
#define __ISP_CTRL_H__

#include <linux/types.h>

/**
 * struct isp_ctrl - Custom control structure for ISP module
 * @id: Control ID
 * @handler: Pointer to handler context
 * @cfg: Pointer to configuration data
 *
 * This structure is used for ISP module control operations
 * that are separate from the kernel's V4L2 API
 */
struct isp_ctrl {
	int id;
	void *handler;
	void *cfg;
};

#endif /* __ISP_CTRL_H__ */

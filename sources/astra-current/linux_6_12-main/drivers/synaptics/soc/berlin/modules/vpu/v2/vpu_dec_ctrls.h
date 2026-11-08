/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2022 - 2023 Synaptics Incorporated.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __VPU_DEC_CTRLS_H__
#define __VPU_DEC_CTRLS_H__

#include <linux/types.h>
#include <media/v4l2-ctrls.h>

#define SYNA_DEC_MAX_CTRLS_HINT		(12)

int vpu_dec_ctrls_init(struct v4l2_ctrl_handler *handler);
void vpu_dec_ctrls_deinit(struct v4l2_ctrl_handler *handler);

#endif // __VPU_DEC_CTRLS_H__

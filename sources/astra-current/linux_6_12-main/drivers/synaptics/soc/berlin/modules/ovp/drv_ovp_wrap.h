/* SPDX-License-Identifier: GPL-2.0
 * Copyright (C) 2026 Synaptics Incorporated
 */

#ifndef _OVP_DRIVER_WRAP_H_
#define _OVP_DRIVER_WRAP_H_

#include <linux/interrupt.h>
#include <linux/types.h>

#include "drv_msg.h"
#include "ovp_ioctl.h"

/* driver control API's */
int wrap_ovp_drv_register_isr(void);
void wrap_ovp_drv_free_isr(void);
int wrap_ovp_drv_get_isr_msg(CC_MSG_t *ptr_msg);
int wrap_ovp_drv_set_intr(INTR_MSG *ptr_ovp_intr_info);
int wrap_ovp_drv_clk_ctrl(bool b_ovp_clock_control);

#endif /* _OVP_DRIVER_WRAP_H_ */

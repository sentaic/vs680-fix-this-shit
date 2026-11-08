/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RF_H__
#define __RF_H__

#define RF_M_STAGE			(0xff000000)
#define RF_M_PERM			(0x000000ff)
#define RF_S_STAGE			(24)
#define RF_GET_STAGE(attr)		((attr) >> RF_S_STAGE)
#define RF_GET_PERM(attr)		((attr) & RF_M_PERM)
#define RF_ATTR(stage, perm)		(((stage) << RF_S_STAGE) | \
					((perm) & RF_M_PERM))

enum {
	TZ_BOOT_STAGE_ROMCODE,
	TZ_BOOT_STAGE_SYSINIT,
	TZ_BOOT_STAGE_TRUSTZONE,
	TZ_BOOT_STAGE_BOOTLOADER,
	TZ_BOOT_STAGE_LINUX,
	TZ_BOOT_STAGE_ANDROID
};

/*
 * register firewall data structure
 */
struct rf_window {
	/*
	 * @id		- unique id of register firewall window
	 */
	uint32_t id;
	/*
	 * @base	- register base address
	 */
	uint32_t base;
	/*
	 * @mask	- address mask, used along with base to determine the
	 * 		  firewall window area
	 */
	uint32_t mask;
	/*
	 * @flag	- permission flag, control if non-secure can have read/write
	 *		  permissions, secure can always access
	 */
	uint32_t attr;
};

int rf_init(unsigned long rf_base, const struct rf_window *rf_params, int nr);
#endif /* __RF_H__ */

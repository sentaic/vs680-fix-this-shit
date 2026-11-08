/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <arch_helpers.h>
#include <plat/common/platform.h>
#include <common/debug.h>
#include "bcm.h"

#define RANDOM_CANARY_VALUE ((u_register_t) 3288484550995823360ULL)

u_register_t plat_get_stack_protector_canary(void)
{
	u_register_t random_val = 0;
#ifdef CFG_SYNA_BCM
	unsigned int status;
	status = bcm_hwrand(&random_val, sizeof(random_val));
	if (status) {
		panic();
	}
	return random_val;
#endif

#ifdef CFG_SYNA_APPCRYPTO
	extern void appcrypto_get_random(uint8_t *data, uint32_t size);

	appcrypto_get_random((uint8_t *)&random_val, sizeof(random_val));
	return random_val;
#endif
	/*
	 * Ideally, a random number should be returned instead of the
	 * combination of a timer's value and a compile-time constant.
	 */
	 random_val = (u_register_t)read_cntpct_el0();
	return RANDOM_CANARY_VALUE ^ random_val;

}

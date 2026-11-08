// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */


#include <kernel/tee_time.h>

__weak void syna_get_secure_time(TEE_Time *time)
{
	/*
	 * Dummy function. The implementation shall be in the driver
	 * of the module that generates the secure clock.
	 */
}

TEE_Result tee_time_get_sys_time(TEE_Time *time)
{
	syna_get_secure_time(time);

	return TEE_SUCCESS;
}

uint32_t tee_time_get_sys_time_protection_level(void)
{
	return 1000;
}

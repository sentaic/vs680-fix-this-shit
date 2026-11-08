// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <string.h>
#include <stdint.h>
#include <mm/core_memprot.h>
#include <tee/cache.h>
#include "tee_api_types.h"
#include "tz_errcode.h"
#include "bcm_mailbox.h"
#include "bcm_cmd.h"
#include "bcm_execute_cmd.h"
#include "bcm_suspend.h"

#define BCM_KERNEL_PI_STORE_TEE_DATA 0x0034

void bcm_suspend_init(void)
{
}

void bcm_suspend(void)
{
}

void bcm_resume(void)
{
}

int bcm_store_tee_data(void* buff, uint32_t offset, uint32_t size)
{
	BCM_MAILBOX_COMMAND cmd = {0};
	BCM_MAILBOX_STATUS status = {0};

	cache_operation(TEE_CACHEFLUSH, (void *)buff, size);

	cmd.arg0 = (uintptr_t)virt_to_phys(buff);
	cmd.arg1 = offset;
	cmd.arg2 = size;

	cmd.command = BCM_KERNEL_PI_STORE_TEE_DATA;

	return execute_bcm_command(&cmd, &status, 0);
}

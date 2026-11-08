// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <initcall.h>
#include <kernel/linker.h>
#include "mr_setup.h"
#include "tz_boot_cmd.h"
#include "syna_setup.h"
#include "hal_soc.h"

static unsigned long tz_boot_stage = TZ_BOOT_STAGE_BOOTLOADER;
static unsigned long tz_boot_mode = TZ_BOOT_MODE_NORMAL;


unsigned long tz_get_boot_stage(void)
{
	return tz_boot_stage;
}

unsigned long tz_sw_get_boot_mode(void)
{
	return tz_boot_mode;
}

void tz_set_boot_stage(unsigned long state)
{
	tz_boot_stage = state;
}

void tz_sw_set_boot_mode(unsigned long mode)
{
	tz_boot_mode = mode;
}

static TEE_Result syna_setup(void)
{
	mr_setup();
	otp_init();

	return TEE_SUCCESS;
}

preinit_late(syna_setup);

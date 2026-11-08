// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <tee/entry_fast.h>
#include <trace.h>
#include <util.h>
#include "tz_errcode.h"
#include "tz_boot_cmd.h"
#include "syna_setup.h"
#include "syna_fastcall.h"

void fastcall_enter_boot_stage(struct thread_smc_args *args)
{
	unsigned long stage = args->a1;
	unsigned long mode = args->a2;

	/* currently, only support set boot stage in bootloader */
	if (tz_get_boot_stage() != TZ_BOOT_STAGE_BOOTLOADER &&
			stage != TZ_BOOT_STAGE_LINUX)
		return;

	tz_set_boot_stage(stage);
	tz_sw_set_boot_mode(mode);

	//FIXME
	//prot_set_by_stage(stage);
	//rf_set_by_stage(stage);

	if (stage == TZ_BOOT_STAGE_LINUX) {
		//FIXME
		//rf_buffer_registers();
		//prot_buffer_registers();
		//bcm_enable_bg_checking();
	}

	return;
}

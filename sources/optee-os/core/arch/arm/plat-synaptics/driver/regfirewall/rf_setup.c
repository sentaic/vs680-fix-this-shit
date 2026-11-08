// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <trace.h>
#include "regfirewall.h"
#include "rf_setup.h"
#include "tz_boot_param.h"
#include "tz_boot_param_value.h"
#include "tz_boot_cmd.h"
#include <initcall.h>


__attribute__((weak)) void rf_set_by_stage(uint32_t __unused stage)
{
	//do nothing
	//remove the fastcall in bootloader, linux etc.
}


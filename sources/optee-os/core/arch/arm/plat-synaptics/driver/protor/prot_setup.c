// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include "protor.h"
#include "types_ext.h"
#include "prot_setup.h"
#include "tz_boot_param.h"
#include "tz_boot_param_value.h"
#include "tz_boot_cmd.h"
#include <trace.h>
#include <initcall.h>
#include <kernel/pm.h>

static const struct master_setting *master_value;
static int master_num;

static void prot_set_i(const struct master_setting *master)
{
	prot_set(master->id, MST_GET_PERM(master->attr));
}

void prot_set_by_stage(uint32_t stage)
{
	int i;

	for (i = 0; i < master_num; i++) {
		if (MST_GET_STAGE(master_value[i].attr) == stage)
			prot_set_i(&master_value[i]);
	}
}

static TEE_Result prot_configure_setup(void)
{
	int i;

	master_value = master_setting_param;
	master_num = master_setting_num;
	DMSG("master_num = %d\n", master_num);

	for (i = 0; i < master_num; i++) {
		if (MST_GET_STAGE(master_value[i].attr) <= TZ_BOOT_STAGE_TRUSTZONE)
			prot_set_i(&master_value[i]);
	}
	return TEE_SUCCESS;
}

static TEE_Result
pm_enter_resume(enum pm_op op, uint32_t pm_hint __unused,
		const struct pm_callback_handle *pm_handle __unused)
{
	DMSG("op = 0x%x.\n", op);
	if (op == PM_OP_RESUME)
		return prot_configure_setup();

	return TEE_SUCCESS;
}

static TEE_Result prot_setup(void)
{
	register_pm_driver_cb(pm_enter_resume, NULL, "syna_prot");

	prot_configure_setup();

	return TEE_SUCCESS;
}

driver_init(prot_setup);

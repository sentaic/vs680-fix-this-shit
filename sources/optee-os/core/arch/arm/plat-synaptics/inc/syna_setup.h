// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef SYNA_SETUP_H
#define SYNA_SETUP_H
#include <mm/mobj.h>

extern struct mobj *syna_pt_mobj;
extern struct mobj *syna_vpp_mobj;
extern struct mobj *syna_nsec_nc_mobj;

unsigned long tz_get_boot_stage(void);
void tz_set_boot_stage(unsigned long state);
unsigned long tz_sw_get_boot_mode(void);
void tz_sw_set_boot_mode(unsigned long mode);

TEE_Result syna_get_hw_unique_key(size_t key_len, uint8_t *key);

TEE_Result syna_exec_cmd(uint32_t cmd_id, const void *param,
	uint32_t param_len, const void *result, uint32_t result_len);

TEE_Result syna_exec_cmd_ex(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result,
		uint32_t result_len, uint32_t delay);

#endif

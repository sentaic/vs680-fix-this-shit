/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TEE_TA_PERM_CTRL_H_
#define _TEE_TA_PERM_CTRL_H_

#include "tee_ta_perm_rule_def.h"

/*
 * Definations for group item.
 * embeded in TA certificate
 */
#define TEE_PC_GROUP_HEAD_MAGIC    (0x84658070)

typedef struct _ta_group_header_t {
	uint32_t magic;
	uint32_t version;
	uint32_t total_size;                 // include header size
	uint32_t ta_vendor_gid;              // GID for the TA vendor
	uint32_t group_count;
	struct {
		char     name[TEE_PC_MAX_NAME_LENGTH];
		uint32_t length;
	} group_names[0];
} ta_group_header_t;

TEE_Result ta_perm_set_group_info(ta_group_header_t *root_groups,
		ta_group_header_t *groups,
		const TEE_UUID *uuid);

TEE_Result ta_perm_check_mmap(const TEE_UUID *uuid,
		paddr_t address, uint32_t size, bool *allow);

TEE_Result ta_perm_check_bcm_pi(const TEE_UUID *uuid, uint32_t pi, bool *allow);

#endif //_TEE_TA_PERM_CTRL_H_

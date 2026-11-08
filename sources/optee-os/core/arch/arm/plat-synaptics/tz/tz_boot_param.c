// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <string.h>
#include <kernel/panic.h>
#include "tz_boot_param.h"

#ifndef CFG_SYNA_BOOT_PARAM_MAX_SIZE
#define CFG_SYNA_BOOT_PARAM_MAX_SIZE 0x20000
#endif

static struct tz_boot_param_header *root = NULL;

void tz_boot_param_init(void *addr, bool reset)
{
	if (!addr)
		return;

	root = addr;
	if (!reset) {
		uint32_t offset = sizeof(*root);

		if (root->magic_num != TZ_BOOT_PARAM_MAGIC_NUM ||
		    root->version != TZ_BOOT_PARAM_VERSION ||
		    root->size < sizeof(*root) ||
		    root->size > CFG_SYNA_BOOT_PARAM_MAX_SIZE ||
		    (root->size & 3))
			panic("Invalid Synaptics boot parameter header");
		while (offset < root->size) {
			const struct tz_boot_param *param;

			if (root->size - offset < sizeof(*param))
				panic("Truncated Synaptics boot parameter");
			param = (const void *)((uintptr_t)root + offset);
			if (param->size < sizeof(*param) || (param->size & 3) ||
			    param->size > root->size - offset)
				panic("Invalid Synaptics boot parameter length");
			offset += param->size;
		}
	}
	if (reset) {
		root->magic_num = TZ_BOOT_PARAM_MAGIC_NUM;
		root->size = sizeof(*root);
		root->version = TZ_BOOT_PARAM_VERSION;
		root->reserved = 0;
	}
}

struct tz_boot_param *tz_boot_param_add(uint32_t id,
		void *data, uint32_t size)
{
	struct tz_boot_param *curr = NULL;

	if (!root)
		return NULL;

	curr = (struct tz_boot_param *)((uintptr_t)root + root->size);
	/* make size 4B aligned */
	size = (size + 3) & (~3);

	/* fill current parameter */
	curr->size = size + sizeof(struct tz_boot_param);
	curr->id = id;
	memcpy(curr->data, data, size);

	/* increase the whole boot param size */
	root->size += curr->size;

	return curr;
}

const struct tz_boot_param *tz_boot_param_get(uint32_t id)
{
	uint32_t size = 0;
	struct tz_boot_param *curr = NULL;

	if (!root)
		return NULL;

	size = sizeof(struct tz_boot_param_header);

	while (size < root->size) {
		curr = (struct tz_boot_param *)((uintptr_t)root + size);
		if (curr->id == id)
			return curr;
		size += curr->size;
	}

	return NULL;
}

/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tz_boot_param.h"
#include "assert.h"

#ifndef CONFIG_BOOT_PARAM_MAX_SIZE
#define CONFIG_BOOT_PARAM_MAX_SIZE	(100 << 10)
#endif /* CONFIG_BOOT_PARAM_MAX_SIZE */

static struct tz_boot_param_header *root = 0;

int tz_boot_param_init(void *addr)
{
	if (!addr)
		return -1;

	root = addr;

	if (root->magic_num != TZ_BOOT_PARAM_MAGIC_NUM ||
			root->version != TZ_BOOT_PARAM_VERSION)

		return -1;

	mr_setup();

	return 0;
}

struct tz_boot_param *tz_boot_param_get(uint32_t id)
{
	if (!root)
		return 0;

	uint32_t size = sizeof(struct tz_boot_param_header);

	while (size < root->size) {
		struct tz_boot_param *curr = (void*)((unsigned long)root + size);
		if (curr->id == id)
			return curr;
		size += curr->size;
	}

	return 0;
}

#define MR_MAX		16
struct syna_mem_region g_mr[MR_MAX];
static int mr_num = 0;

int mr_setup(void)
{
	const struct tz_boot_param *param =
				tz_boot_param_get(TZ_BOOT_PARAM_MEMREGION);
	int i;

	if (param) {
		mr_num = (param->size - sizeof(struct tz_boot_param))
			/ sizeof(struct syna_mem_region);

		memcpy(g_mr, (void *)(param->data), mr_num * sizeof(struct syna_mem_region));
		INFO("num: %d\n", mr_num);
	} else {
		return -1;
	}

	for (i = 0; i < mr_num; i++) {
		INFO("@@@(%d/%d) %s, %lx, %lx, %x, %x\n", i, mr_num, g_mr[i].name,
			(uint64_t)g_mr[i].base, (uint64_t)g_mr[i].size,
			g_mr[i].attr, g_mr[i].userdata[0]);
	}

	return 0;
}

const struct syna_mem_region *mr_find_region_by_name(const char *name)
{
	const struct syna_mem_region *r = g_mr;
	int i;

	if (!name) {
		ERROR("name == NULL\n");
		return NULL;
	}

	for (i = 0; i < mr_num; i++, r++) {
		if (0 == strcmp(name, r->name))
			return r;
	}
	ERROR("can't find region: %s\n", name);

	return NULL;
}


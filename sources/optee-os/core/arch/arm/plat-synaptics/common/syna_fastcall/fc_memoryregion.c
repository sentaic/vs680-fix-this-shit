// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <tee/entry_fast.h>
#include <trace.h>
#include <util.h>
#include "syna_setup.h"
#include "syna_fastcall.h"
#include "tz_boot_cmd.h"
#include "mr_setup.h"
#include "tz_errcode.h"
#include "tz_perm.h"
#include "mem_region.h"
#include "mem_region_userdata.h"
#include "tz_boot_cmd.h"
#include <tee/cache.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <mm/mobj.h>

void fastcall_get_mem_region_count(struct thread_smc_args *args)
{
	unsigned long attr_mask = args->a1;
	unsigned long attr_val = args->a2;
	unsigned long result = 0;
	/* this command can only be executed in bootloader */
	if (tz_get_boot_stage() != TZ_BOOT_STAGE_BOOTLOADER) {
		DMSG("get_mem_region_count can only be called in bootloader\n");
		return;
	}

	result = mr_filter(NULL, 0, attr_mask, attr_val);
	args->a0 = result;
}

void fastcall_get_mem_region_list(struct thread_smc_args *args)
{
	paddr_t pa = args->a1;
	unsigned long max_count = args->a2;
	unsigned long attr_mask = args->a3;
	unsigned long attr_val = args->a4;
	struct mem_region *region;
	struct mobj *mobj;
	void *va;
	int result = 0;
	unsigned long total_param_len;
	const struct mem_region *mr = NULL;
	int i;

	/* this command can only be executed in bootloader */
	if (tz_get_boot_stage() != TZ_BOOT_STAGE_BOOTLOADER) {
		DMSG("get_mem_region_list can only be called in bootloader\n");
		return;
	}


	if (max_count > MR_MAX) {
		EMSG("max_count is too large\n");
		return;
	}

	total_param_len = max_count * sizeof(struct mem_region);
	DMSG("pa = 0x%08" PRIxPA " total_param_len = 0x%ld\n", pa, total_param_len);

	/* Check if this region is in static shared space */
	if (core_pbuf_is(CORE_MEM_NSEC_SHM, pa, total_param_len)) {
		va = phys_to_virt(pa, MEM_AREA_NSEC_SHM, 1);
		if (!va) {
			EMSG("Fail to transfer va to pa address.\n");
			return;
		}
		mobj = mobj_shm_alloc(pa, total_param_len, 0);
	} else {
		if (pa & SMALL_PAGE_MASK) {
			EMSG("Command buffer spans across page boundary");
			return;
		}
		mobj = mobj_mapped_shm_alloc(&pa, 1, 0, 0);
	}

	va = mobj_get_va(mobj, 0, 1);
	if (!va) {
		EMSG("Fail to get va address.\n");
		mobj_put(mobj);
		return;
	}
	region = va;

	/* check region, must be non secure memory */
	mr = mr_find_region(pa, sizeof(struct mem_region) * max_count);
	if (!mr) {
		EMSG("mr_find_region error\n");
		return;
	}
	if (TZ_IS_NNA(MR_PERM(mr))) {
		EMSG("region 0x%p  is not non secure memory\n", region);
		return;
	}
	result = mr_filter(region, max_count, attr_mask, attr_val);

	/* flush dcache before verification */
	cache_operation(TEE_CACHEFLUSH, region, total_param_len);
	DMSG("region @ 0x%p region count = %d.\n", region, result);

	for (i = 0; i < result; i++) {
		DMSG("%s: base 0x%x, size 0x%x.\n", region[i].name, region[i].base, region[i].size);
	}

	args->a0 = result;
}

// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <assert.h>
#include <string.h>
#include <kernel/panic.h>
#include <mm/core_memprot.h>
#include <mm/mobj.h>
#include "mr_setup.h"
#include "tz_boot_param.h"
#include <trace.h>
#include <util.h>
#include <stdlib.h>
#include "mem_region.h"
#include "mem_region_userdata.h"
#include "tz_boot_arg.h"

#define  MAX_REGION_COUNT 16

struct mem_region mr[MAX_REGION_COUNT];
static int mr_num = 0;

struct mobj *syna_sec_mem_mobj;
struct mobj *syna_nsec_mobj;
struct mobj *syna_pt_mobj;
struct mobj *syna_nsec_nc_mobj;
struct mobj *syna_vpp_mobj;

/*
 * boot arguments
 */
struct tz_boot_arg tzb_arg = { 0xdeadbeef };
extern uint64_t __save_boot_regs[8];


/* fill the memory region info to mem_map_section */
void mr_setup(void)
{
	const struct tz_boot_param *param = NULL;
	void *boot_param;
	int i = 0;
	unsigned long pt_paddr;
	unsigned long pt_size;
	unsigned long nsec_nc_paddr;
	unsigned long nsec_nc_size;
	unsigned long vpp_paddr;
	unsigned long vpp_size;
	unsigned int mr_type;

	tzb_arg.boot_param = (void *)(paddr_t)__save_boot_regs[0];
	if (tzb_arg.boot_param == NULL) {
		tzb_arg.boot_param = (void *)0x101000;
	}

	boot_param = phys_to_virt((unsigned long)tzb_arg.boot_param, MEM_AREA_RAM_SEC, 1);
	DMSG("boot_param = 0x%p, arg.boot_param = 0x%p.\n", boot_param, tzb_arg.boot_param);


	tz_boot_param_init(boot_param, false);

	mr_type = __save_boot_regs[1];
	switch(mr_type){
	case TZ_BOOT_PARAM_MEMREGION_512M ... TZ_BOOT_PARAM_MEMREGION_4G:
		break;
	default:
		mr_type = TZ_BOOT_PARAM_MEMREGION;
	}
	DMSG("mr_type = 0x%x", mr_type);

	//get memory region and fill it in setion "phys_mem_map_section"
	param = tz_boot_param_get(mr_type);

	if (!param) {
		panic("No memory region in boot parameter!\n");
		return;
	}

	mr_num = (param->size - sizeof(struct tz_boot_param)) / sizeof(struct mem_region);

	if (mr_num > MAX_REGION_COUNT) {
		EMSG("memory region number (%d)in boot parameter is too big!\n", mr_num);
		return;
	}
	DMSG("num: %d\n", mr_num);

	memcpy(mr, (void *)(param->data), mr_num * sizeof(struct mem_region));

	for(i = 0; i < mr_num; i++) {
		DMSG("@@@(%d/%d) %s, %lx, %lx, %x, %x\n", i,
			mr_num, mr[i].name, (uint64_t)mr[i].base,
			(uint64_t)mr[i].size, mr[i].attr, mr[i].userdata[0]);
#ifdef CFG_SECURE_DATA_PATH
		if (TEE_MR_USER_IS_ION(mr + i) && MR_IS_SECURE(mr + i)) {
			core_mmu_set_spd_memory(mr[i].base, mr[i].size);
		}
#endif
		if (TEE_MR_USER_IS_ION(mr + i) && MR_IS_NONSECURE(mr + i) && MR_IS_CACHEABLE(mr + i)) {
			default_nsec_paddr = mr[i].base;
			default_nsec_size = mr[i].size;
		}
		if (TEE_MR_USER_IS_PT(mr + i)) {
			pt_paddr = mr[i].base;
			pt_size = mr[i].size;
			syna_pt_mobj = mobj_phys_alloc(pt_paddr, pt_size,
							TEE_MATTR_MEM_TYPE_CACHED, CORE_MEM_SDP_MEM);
			if (!syna_pt_mobj)
				panic("Failed to register pt memory");
		}
		if (TEE_MR_USER_IS_VPP(mr + i)) {
			vpp_paddr = mr[i].base;
			vpp_size = mr[i].size;
			syna_vpp_mobj = mobj_phys_alloc(vpp_paddr, vpp_size,
							TEE_MATTR_MEM_TYPE_CACHED, CORE_MEM_SDP_MEM);
			if (!syna_vpp_mobj)
				panic("Failed to register pt memory");
		}

		if (TEE_MR_USER_IS_ION(mr + i) && MR_IS_NONSECURE(mr + i) && MR_IS_UNCACHEABLE(mr + i)) {
			nsec_nc_paddr = mr[i].base;
			nsec_nc_size = mr[i].size;
			syna_nsec_nc_mobj =  mobj_phys_alloc(nsec_nc_paddr, nsec_nc_size,
									TEE_MATTR_MEM_TYPE_DEV, CORE_MEM_NON_SEC);
		}

	}

}

uint32_t mr_get_region(const struct mem_region **region)
{
	if (region)
		*region = mr;
	return mr_num;
}

const struct mem_region *mr_find_region(paddr_t buffer, uint32_t size)
{
	const struct mem_region *r = mr;
	paddr_t start = buffer;
	int i;

	/* temporally workaround the VPP CEC issue */
#if 0
	assert(buffer);
	assert(size);
#else
	if (!buffer || size <= 0)
		EMSG("buffer=0x%08" PRIxPA ", size=0x%08x\n", buffer, size);
#endif

	for (i = 0; i < mr_num; i++, r++) {
		/* do boundary check */
		if ((start >= r->base) && (size <= r->size)
			&& (start - r->base <= r->size - size))
			return r;
	}
	DMSG("can't find region: start 0x%08" PRIxPA ", size 0x%08x\n", start, size);

	return NULL;
}

const struct mem_region *mr_find_region_by_name(const char *name)
{
	const struct mem_region *r = mr;
	int i;

	if (!name) {
		EMSG("name == NULL\n");
		return NULL;
	}

	for (i = 0; i < mr_num; i++, r++) {
		if (0 == strcmp(name, r->name))
			return r;
	}
	DMSG("can't find region: %s\n", name);

	return NULL;
}

uint32_t mr_filter(struct mem_region *region, int32_t max_num,
		uint32_t attr_mask, uint32_t attr_val)
{
	int count = 0, i;

	assert((!region && !max_num) || (region && max_num));
	assert((!region && !attr_mask) || region);

	if (max_num > MR_MAX) {
		EMSG("max_count is too large\n");
		return 0;
	}

	if (attr_mask == 0) {
		if (region)
			memcpy(region, mr, MIN(max_num, mr_num) * sizeof(*mr));
		return mr_num;
	}

	for (i = 0; i < mr_num; i++) {
		if ((mr[i].attr & attr_mask) == (attr_val & attr_mask)) {
			if ((count < max_num) && region)
				memcpy(&region[count], &mr[i], sizeof(*mr));

			count++;
		}
	}

	return count;
}

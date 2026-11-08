// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _MR_SETUP_H_
#define _MR_SETUP_H_

#include "mem_region.h"

#define MR_MAX		16

void mr_setup(void);

uint32_t mr_get_region(const struct mem_region **region);

const struct mem_region *mr_find_region(paddr_t buffer, uint32_t size);

const struct mem_region *mr_find_region_by_name(const char *name);

/** Filter memory region list.
 *
 * @param region	buffer to store the retrieved regions.
 *			NULL to return region number.
 * @param max_num	max number of regions to be retrieved.
 * @param attr_mask	attribute mask to filter memory regions.
 * 			can be the flags in MR_M_*. 0 to get all.
 * 			only (region.attr & attr_mask == attr_val) is filtered.
 * @param attr_val	attribute value to filter memory regions.
 * 			use MR_ATTR() to generate it. ignored if attr_mask=0.
 *
 * @return	total matched region number. if max_num < total matched region
 * 		number, then only max_num would be copied.
 */
uint32_t mr_filter(struct mem_region *region, int32_t max_num,
		uint32_t attr_mask, uint32_t attr_val);

#endif	/* _MR_SETUP_H_ */

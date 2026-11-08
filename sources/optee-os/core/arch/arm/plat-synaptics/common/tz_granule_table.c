// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include <string.h>
#include <trace.h>
#include <sys/types.h>
#include "scatter_list.h"
#include <kernel/spinlock.h>
#include <mm/mobj.h>
#include <tz_granule_table.h>

/* each bit indicates a 4KB page. it is used to do overlap check */

#define BITS_U32 (32) /* 32 bits per uint32_t */
#define MAX_INDEX (0xFFFFFFFF)
#define POWER_PAGE (12)
#define POWER_32 (5)

/* 128KB table for 4GB memory */
static uint32_t gtable[0x8000];
static unsigned int lock = SPINLOCK_UNLOCK;

static uint32_t get_granule_index(uint32_t addr)
{
	uint32_t index = 0;

	/* must align with granularity */
	if((addr & (SMALL_PAGE_SIZE - 1)) != 0)
		return -1;

	index = addr >> POWER_PAGE;
	index = index >> POWER_32;

	return index;
}

static uint32_t get_granule_startbit(uint32_t addr)
{
	uint32_t start = 0;

	/* must align with granularity */
	if((addr & (SMALL_PAGE_SIZE - 1)) != 0)
		return -1;

	start = addr >> POWER_PAGE;

	start &= (BITS_U32 - 1);

	return start;
}

static uint32_t set_granule_bits(int value, uint32_t index,
					uint32_t start, uint32_t bitnum, uint32_t end_index)
{
	uint32_t curindex = index;
	uint32_t leftbits = bitnum, startbit = start, curbits = 0;
	uint32_t bitmask = 0;

	while ((leftbits > 0) && (curindex != end_index)) {
		curbits = (leftbits + startbit) > BITS_U32 ?
			(BITS_U32 - startbit) : leftbits;

		if (curbits == BITS_U32)
			bitmask = 0xFFFFFFFF;
		else
			bitmask = ((1 << curbits) - 1) << startbit;

		//DMSG("%d:%08x, %x\n", curindex, gtable[curindex], bitmask);

		if (value) {
			if (gtable[curindex] & bitmask)
				return curindex; /* already set, overlap */

			gtable[curindex] |= bitmask;
		} else {
			if ((~gtable[curindex]) & bitmask)
				return curindex; /* already cleared, something wrong */

			gtable[curindex] &= (~bitmask);
		}
		//DMSG("%d:%08x\n", curindex, gtable[curindex]);

		leftbits -= curbits;
		startbit = 0;
		curindex++;
	}

	return 0;
}

/*
 * scatters registerred are marked to 1 in granule table with
 * one bit one page
 * if the corresponding bit of a page is already marked,
 * it means it is already in reservation store and is
 * overlapped. tzk have to deny the registration and restore
 * the granule table.
 * by this way, the time complexity decreases to N and is only
 * related with the scale of scatler list itself. So that we can
 * get better performance
 */
static uint32_t set_pages_in_granuletable(scatter_list_t *s_list,
									uint32_t value, uint32_t end_index)
{
	unsigned int i = 0;
	uint32_t index = 0, start = 0, bitnum = 0;
	uint32_t err_index = 0;

	for (i = 0; i < s_list->num; i++) {
		index = get_granule_index(s_list->s_item[i].addr);
		if (index == (uint32_t)-1)
			return -1;

		start = get_granule_startbit(s_list->s_item[i].addr);
		if (start == (uint32_t)-1)
			return -2;

		bitnum = s_list->s_item[i].npage;

		if (index == end_index)
			return 0;

		err_index = set_granule_bits(value, index, start, bitnum, end_index);
		if (err_index != 0) {
			if (value)
				EMSG("[address@0x%x, npage=0x%x]overlapped inside itself or with existing reservation\n",
						s_list->s_item[i].addr, s_list->s_item[i].npage);
			else
				EMSG("try to clear a non-existing reservation\n");
			return err_index; /* overlap find */
		}
	}

	return 0;
}

/* mark corresponding bit if success, otherwise restore */
int mark_pages_in_granuletable(scatter_list_t *s_list)
{
	uint32_t err_index = 0;
	unsigned long flag;

	flag = cpu_spin_lock_xsave(&lock);

	err_index =  set_pages_in_granuletable(s_list, 1, MAX_INDEX);

	if (err_index != 0) {
		/* roll back the change */
		set_pages_in_granuletable(s_list, 0, err_index);
		cpu_spin_unlock_xrestore(&lock, flag);
		return -1;
	}

	cpu_spin_unlock_xrestore(&lock, flag);
	return 0;
}

/* clear corresponding bit if success */
int clear_pages_in_granuletable(scatter_list_t *s_list)
{
	uint32_t err_index = 0;
	unsigned long flag;

	flag = cpu_spin_lock_xsave(&lock);

	err_index = set_pages_in_granuletable(s_list, 0, MAX_INDEX);

	if (err_index != 0) {
		/* roll back the change */
		set_pages_in_granuletable(s_list, 1, err_index);
		assert(err_index == 0); /* should not come here */
	}

	cpu_spin_unlock_xrestore(&lock, flag);
	return 0;
}

/* try to mark and check if overlap. restore anyway at the end */
int try_mark_pages_in_granuletable(scatter_list_t *s_list)
{
	uint32_t err_index = 0;
	unsigned long flag;

	flag = cpu_spin_lock_xsave(&lock);

	err_index = set_pages_in_granuletable(s_list, 1, MAX_INDEX);

	/* roll back the change */
	if (err_index != 0) {
		set_pages_in_granuletable(s_list, 0, err_index);
		cpu_spin_unlock_xrestore(&lock, flag);
		return -1;
	} else {
		set_pages_in_granuletable(s_list, 0, MAX_INDEX);
		cpu_spin_unlock_xrestore(&lock, flag);
		return 0;
	}
}

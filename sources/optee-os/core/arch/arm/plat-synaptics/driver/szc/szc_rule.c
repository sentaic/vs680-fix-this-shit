// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <types_ext.h>
#include <io.h>
#include <trace.h>
#include <assert.h>
#include <platform_config.h>
#include <initcall.h>
#include <mm/core_memprot.h>
#include "SZC.h"
#include "Galois_memmap.h"
#include "szc.h"
#include "tz_boot_param_value.h"
#include "config.h"

#define readl io_read32
#define writel(v, a) do {               \
		io_write32(a, v);                    \
	} while(0)

#ifndef SZC_BASE
#define SZC_BASE                    (vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, MEMMAP_MPT_REG_SIZE)
#endif
#define SZC_RULE_BASE               (SZC_BASE + RA_SZC_Rule)
#define SZC_RULE(r)                 (SZC_RULE_BASE + (r) * 128)
#define SZC_RULE_MASTER(r, m)       (SZC_RULE(r) + (m) * 4)

void szc_set_rule_master(unsigned int rule)
{
	unsigned int rule_index;
	unsigned int master_index;
	unsigned perm;

	rule_index = RULE_GET_ID(rule);
	assert(rule_index < SZC_RULE_NUM_RULES_MAX);
	master_index = RULE_GET_MST(rule);
	assert(master_index < SZC_RULE_NUM_MASTERS_MAX);

	perm = RULE_GET_PERM(rule);

	DMSG("rule_index = 0x%x, master_index = 0x%x, perm = 0x%x.\n",
			rule_index, master_index, perm);

	writel(perm, SZC_RULE_MASTER(rule_index, master_index));
	assert(perm == readl(SZC_RULE_MASTER(rule_index, master_index)));
}

void szc_set_rule_master_attr(unsigned int rule_index, unsigned int master_index, unsigned int s_rd, unsigned int s_wr, unsigned int ns_rd, unsigned int ns_wr)
{
	unsigned int val = s_rd | (s_wr << 1) | (ns_rd << 2) | (ns_wr << 3);

	writel(val, SZC_RULE_MASTER(rule_index, master_index));
	assert(val == readl(SZC_RULE_MASTER(rule_index, master_index)));
}

void szc_set_rule_lock(unsigned int lock_mask)
{
	writel(lock_mask, SZC_RULE_BASE + RA_Rule_Rule_lock);
}

void szc_set_rule_sel(unsigned int sel)
{
	writel(sel, SZC_RULE_BASE + RA_Rule_Rule_sel);
}

void szc_set_rule_enable(unsigned int enable)
{
	writel(enable, SZC_RULE_BASE + RA_Rule_Rule_enable);
}

void szc_set_rule_enable_per_master(unsigned int master_index, unsigned int enable)
{
	unsigned int val;

	val = readl(SZC_RULE_BASE + RA_Rule_Rule_enable);
	if (enable)
	{
		val |= 1 << master_index;
	}
	else
	{
		val &= ~(1 << master_index);
	}
	writel(val, SZC_RULE_BASE + RA_Rule_Rule_enable);
}

void szc_set_rule_index_sel(unsigned int index)
{
	writel(index, SZC_RULE_BASE + RA_Rule_Rule_index_sel);
}

int szc_get_rule_num(void)
{
	assert(szc_rule_num <= SZC_RULE_NUM_RULES_MAX);

	return szc_rule_num;
}

static TEE_Result szc_rule_init(void)
{
	int i = 0, j = 0;

	//disable all masters rule check
	szc_set_rule_enable(0);

	/* since the all the rule masters' access permissions
	 * are enabled by default, so disable it first
	 */
	for (i = 0; i < SZC_RULE_NUM_RULES_MAX; i++) {
		for(j = 0; j < SZC_RULE_NUM_MASTERS_MAX; j++)
			szc_set_rule_master_attr(i, j, 0, 0, 0 ,0);
	}

	//set predefined rules
	for (i = 0; i < szc_rule_num; i++) {
		szc_set_rule_master(szc_rule_param[i]);
	}

	// lock all rules
	szc_set_rule_lock(0xffffffff);

	// select generic rule as default
	szc_set_rule_sel(0);

	//enable rule check for all masters
	szc_set_rule_enable(0xffffffff);

	return TEE_SUCCESS;
}
driver_init(szc_rule_init);


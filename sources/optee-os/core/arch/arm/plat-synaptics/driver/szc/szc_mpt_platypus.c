// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "SZC.h"
#include <types_ext.h>
#include "io.h"
#include <trace.h>
#include <string.h>
#include <config.h>
#include <assert.h>
#include "Galois_memmap.h"
#include "szc.h"
#include "szc_mpt.h"
#include <platform_config.h>
#include <mm/core_memprot.h>
#include <initcall.h>

#if 0
#include "mm.h"
#include "mmu.h"
#include "cache.h"
#include "tz_errcode.h"
#endif
#define readl io_read32
#define writel(v, a) do {               \
		io_write32(a, v);                    \
	} while(0)


static uint32_t granularity = 0x1000;
static uint32_t gpower = 12;
static uint32_t num_ctrlbit = 1;

#define MPT_BASE        (vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, MEMMAP_MPT_REG_SIZE)
#define MPT_SRAM_SZIE 0x1C000
static unsigned char mpt_sram_backup[MPT_SRAM_SZIE];

#define SIZE_PERREG (0x20000) /* the memory size controled by a dword mpt reg */
#define POWER_PERREG (17) /* 2^17 = 0x20000 */

static uint32_t rule_benchmark[6] = {
	0x00000000, /* dummy */
	0x00000000, /* dummy */
	0xFFFFFFFF, /* 1 ctrl bit: rule 1 */
	0x55555555, /* 2 ctrl bit: rule 1 */
	0xAAAAAAAA, /* 2 ctrl bit: rule 2 */
	0xFFFFFFFF, /* 2 ctrl bit: rule 3 */
};

static int mpt_attribute_change(unsigned int mpt_start_addr,
			unsigned int bitloc,
			unsigned int size_in_bits,
			unsigned int rule)
{
	unsigned int val = 0;
	uint32_t bitmask = 0;
	uint32_t benchmark = 0x00000000;

	if (rule)
		benchmark = rule_benchmark[num_ctrlbit + rule];

	val = readl(mpt_start_addr);

	if (size_in_bits == 32) {
		val = benchmark;
	} else {
		bitmask = ((1 << size_in_bits) - 1) << bitloc;

		val &= (~bitmask);
		if (rule)
			val |= (bitmask & benchmark);
	}

	writel(val, mpt_start_addr);

	return 0;
}


static unsigned int mpt_attribute_get(unsigned int start_addr)
{
	volatile unsigned int mpt_start_dword = 0;
	volatile unsigned int mpt32_bit_loc = 0;
	volatile unsigned int mpt_start_addr = 0;
	unsigned int val = 0, rule = 0;

	assert(!(start_addr & (mpt_get_granularity() - 1)));

	/* 128KB super page per dword in MPT */
	mpt_start_dword = (start_addr >> POWER_PERREG);
	mpt_start_addr = (MPT_BASE + mpt_start_dword * 4);
	mpt32_bit_loc = ((start_addr & (SIZE_PERREG - 1)) >> gpower) * mpt_get_num_ctrlbit();

	val = readl(mpt_start_addr);

	rule = (val >> mpt32_bit_loc) & ((1 << mpt_get_num_ctrlbit()) - 1);

	return rule;
}

static int mpt_address_configure(unsigned int start_addr, unsigned int size, unsigned int rule)
{
	volatile unsigned int mpt_start_dword = 0;
	volatile unsigned int mpt32_bit_loc = 0;
	volatile unsigned int mpt_start_addr = 0;
	int bits_left = 0;
	int bits_temp = 0;

	assert(!(start_addr & (mpt_get_granularity() - 1)));
	assert(!(size & (mpt_get_granularity() - 1)));

	mpt_start_dword = (start_addr >> POWER_PERREG);
	mpt_start_addr = (MPT_BASE + mpt_start_dword * 4);
	mpt32_bit_loc = ((start_addr & (SIZE_PERREG - 1)) >> gpower) * mpt_get_num_ctrlbit();

	bits_left = (size >> gpower) * mpt_get_num_ctrlbit();

	while (bits_left > 0) {
		if (mpt32_bit_loc != 0)
			bits_temp = MIN(bits_left, (32 - mpt32_bit_loc));
		else
			bits_temp = MIN(bits_left, 32);
#if 0
		DMSG("mpt overwrite address is: 0x%08x\n", mpt_start_addr);
		DMSG("bits left is: %d ", bits_left);
		DMSG("bits temp is: %d \n", bits_temp);
#endif
		mpt_attribute_change(mpt_start_addr, mpt32_bit_loc, bits_temp, rule);

		mpt32_bit_loc = 0; // set to 0 after the first time
		bits_left -= bits_temp;
		mpt_start_addr += 4;
	};

	return 0;
}

void mpt_cache_flush(void)
{
	writel(0xF, (MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_flush));

	if (readl(MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_flush) == 0xF) {
		writel(0x0, (MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_flush));
	}
}

void mpt_cache_enable(void)
{
	writel(0xF, (MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_enable));
}

void mpt_cache_disable(void)
{
	unsigned int en;
	en = readl(MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_enable);
	if (en == 0xF) {
		mpt_cache_flush();
		writel(0x0, (MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_cache_enable));
	}
}

const int mpt_page_size[] = {0x1000, 0x2000, 0x4000};
const int page_size_power[] = {12, 13, 14};

static unsigned int mpt_set_granularity(unsigned int size)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(mpt_page_size); i++) {
		if (size == mpt_page_size[i]) {
			gpower = page_size_power[i];
			break;
		}
	}
	if (i == ARRAY_SIZE(mpt_page_size)) {
		EMSG("mpt ctrl page size 0x%x is not supported.\n", size);
		return TEE_ERROR_NOT_SUPPORTED;
	}

	writel(i, MPT_BASE + RA_SZC_MPT_CTRL + RA_MPT_CTRL_MPT_Page);

	granularity = size;
	num_ctrlbit = (1 << i);

	return TEE_SUCCESS;
}



/* select the master_id rule for the memory region
 * mpt_id: rule index to be selected.
 * mpt_id 0: SR = 1, SW=1, NR=1, NW=1
 * mpt_id 1: SR = 1, SW=1, NR=0, NW=0
 * start_address: the memory start address must be aligned with mpt_get_granularity()
 * size: memory size, must be mpt_get_granularity() aligned
 */
int mpt_configure_region(unsigned int start_addr,
					unsigned int size, unsigned int rule)
{
	//DMSG("start_addr = 0x%x, size = 0x%x, rule = %d\n",
		//	start_addr, size, rule);

	assert(rule < CONFIG_MPT_RULE_NUM);
	assert(!(start_addr & (mpt_get_granularity() - 1)));
	assert(!(size & (mpt_get_granularity() - 1)));

	mpt_address_configure(start_addr, size, rule);

	return 0;
}

int mpt_get_rule(unsigned int start_addr, unsigned int size, unsigned int *rule)
{
	unsigned int i = 0, ng;
	unsigned int rule1 = 0;

	assert(!(start_addr & (mpt_get_granularity() - 1)));
	assert(!(size & (mpt_get_granularity() - 1)));

	ng = size >> gpower;

	rule1 = mpt_attribute_get(start_addr);
	for (i = 1; i < ng; i++) {
		if (rule1 != mpt_attribute_get(start_addr + i * mpt_get_granularity()))
			return -1;
	}
	*rule = rule1;

	return 0;
}

/*
 * FIXME: the following functions hard code the configuration on VS680 Z1.
 * should get them from configuration registers from VS680 A0
 */
unsigned int mpt_get_granularity(void)
{
	return granularity;
}

unsigned int mpt_get_num_ctrlbit(void)
{
	return num_ctrlbit;
}

unsigned int mpt_get_num_rule(void)
{
	return szc_get_rule_num();
}

void mpt_suspend(void)
{
	memcpy(mpt_sram_backup, (void *)MPT_BASE, MPT_SRAM_SZIE);
}

void mpt_resume(void)
{
	memcpy((void *)MPT_BASE, mpt_sram_backup, MPT_SRAM_SZIE);
}

/* we set the range of MPT protection size to 3.5G given that we have
 * memory layout of 2GB, 3GB, 4GB(the rest 0.5G can't be accessed by ACPU
 * and should be protected by secure zone).
 * the last 16KB of MPT will be used by Linux of OOPS feature.
 */
#define CONFIG_MEM_PROTECT_SIZE 0xE0000000

static TEE_Result mpt_init(void)
{
	int i;
	// disable rule check for all masters
	for (i = 0; i < SZC_RULE_NUM_MASTERS_MAX; i++)
		szc_set_rule_enable_per_master(i, 0);

	/* select MPT rule for all masters on VS640
	 * fixme: need a configuration to define which rule is used for different masters
	 */
	szc_set_rule_sel(0xffffffff);

	mpt_set_granularity(0x1000);

	memset((void *)MPT_BASE, 0x00, CONFIG_MEM_PROTECT_SIZE/(mpt_get_granularity()/mpt_get_num_ctrlbit())/8);

	// enable rule check for all masters
	for (i = 0; i < SZC_RULE_NUM_MASTERS_MAX; i++)
		szc_set_rule_enable_per_master(i, 1);

	return TEE_SUCCESS;
}

driver_init(mpt_init);

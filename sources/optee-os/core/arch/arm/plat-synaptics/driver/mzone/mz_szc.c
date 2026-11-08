// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include "types.h"
#include "mz.h"
#include "mz_szc.h"
#include "io.h"
#include "log.h"
#include "mem_region_userdata.h"
#include "mem_region.h"
#include "mr_setup.h"

static int mz_lock(int chip_id)
{
	uint32_t i;
	uintptr_t reg_base = MZ_REGION_REG_BASE;

	for (i = 0; i < MZ_REGION_NUM; i++) {
		uint32_t val = readl(reg_base + RA_Secure_Zone_REGION_CTRL);
		writel(MSK32Secure_Zone_REGION_CTRL_Lock | val, reg_base + RA_Secure_Zone_REGION_CTRL);
		reg_base += MZ_REGION_REG_SIZE;
	}

	return 0;
}

static uint32_t _mz_convert_perm(uint32_t perm)
{
	uint32_t perm_val = 0;

	if ((perm & TZ_SNW) == 0) {
		perm_val |= (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_S_Write_En);
	}
	if ((perm & TZ_SNR) == 0) {
		perm_val |= (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_S_Read_En);
	}

	if ((perm & TZ_NNW) == 0) {
		perm_val |= (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_NS_Write_En);
	}
	if ((perm & TZ_NNR) == 0) {
		perm_val |= (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_NS_Read_En);
	}

	return perm_val;
}

static int mz_set_zone(int chip_id, int zone_id, uint32_t start_addr,
			uint32_t size, int perm)
{
	uintptr_t reg_base = 0;
	uint32_t val = 0;

	trace("zone=%d, start_addr=0x%08x, size=0x%08x, perm=0x%08x\n",
			zone_id, start_addr, size, perm);
	assert(zone_id >= 0 && zone_id < MZ_REGION_NUM);
	assert((start_addr & 0x3) == 0);
	assert((size & 0x3) == 0);
	assert(start_addr + size > start_addr);

	perm &= (TZ_NNA | TZ_SNA);
	if (perm == 0) {
		/* Skip RW regions */
		return 0;
	}
	reg_base = MZ_REGION_REG_BASE + zone_id * MZ_REGION_REG_SIZE;

	val = readl(reg_base + RA_Secure_Zone_REGION_CTRL);
	val &= (~MSK32Secure_Zone_REGION_CTRL_Enable);
	writel(val, reg_base + RA_Secure_Zone_REGION_CTRL);

	writel(start_addr, reg_base + RA_Secure_Zone_REGION_SADDR);
	writel(start_addr + size - 4, reg_base + RA_Secure_Zone_REGION_EADDR);

	val = readl(reg_base + RA_Secure_Zone_REGION_MST_CFG);
	val &= (~MZ_ACPU_ACCESS_MASK);
	val |= _mz_convert_perm(perm);
	writel(val, reg_base + RA_Secure_Zone_REGION_MST_CFG);

	return 0;
}

static int mz_enable_zone(int chip_id, int zone_id, int perm)
{
	uintptr_t reg_base = 0;
	uint32_t val = 0;

	assert(zone_id >= 0 && zone_id < MZ_REGION_NUM);

	perm &= (TZ_NNA | TZ_SNA);
	if (perm == 0) {
		/* Skip RW regions */
		return 0;
	}
	reg_base = MZ_REGION_REG_BASE + zone_id * MZ_REGION_REG_SIZE;

	val = readl(reg_base + RA_Secure_Zone_REGION_MST_CFG);
	val &= MZ_ACPU_ACCESS_MASK;
	assert (val == _mz_convert_perm(perm));

	val = readl(reg_base + RA_Secure_Zone_REGION_CTRL);
	val |= (MSK32Secure_Zone_REGION_CTRL_Enable | MSK32Secure_Zone_REGION_CTRL_NS);
	writel(val, reg_base + RA_Secure_Zone_REGION_CTRL);

	return 0;
}

static mz_ops_t szc_ops = {
	.lock = mz_lock,
	.enable_zone= mz_enable_zone,
	.enable_chip = NULL,
	.set_zone = mz_set_zone
};

void platform_setup_mz_ops(mz_ops_t **ops)
{
	if (ops)
		*ops = &szc_ops;
}

int generate_securezone_by_mr(void)
{
	const struct mem_region * mr = NULL;
	uint32_t mr_num = 0;
	int i = 0, zone_num = 0;

	mr_num = mr_get_region(&mr);

	//check the num and point before generate zone
	if((mr_num == 0) || (mr_num > MZ_REGION_NUM) || (mr == NULL)) {
		error("failed to generated zone for Secure memory region, %d !\n", zone_num);
		return -1;
	}

	for (i = 0; i < mr_num; i++) {
		if (TZ_IS_NNA(mr[i].attr) && !TEE_MR_USER_IS_REGISTER(&mr[i])) {
			secure_memzone_info[zone_num].start = mr[i].base;
			secure_memzone_info[zone_num].size = mr[i].size;
			zone_num++;
		}
	}

	secure_memzone_num = zone_num;

	for (i=0; i < zone_num; i++)
		trace("zone start =%x, size=%x\n", secure_memzone_info[i].start, secure_memzone_info[i].size);

	return 0;
}

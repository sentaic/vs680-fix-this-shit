// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <platform_config.h>
#include <stdint.h>
#include <trace.h>
#include <assert.h>
#include "io.h"
#include "regfirewall.h"
#include "rf_setup.h"
#include "Galois_memmap.h"
#include "soc.h"
#include "tz_boot_param.h"
#include "tz_boot_param_value.h"
#include "tz_boot_cmd.h"
#include "szc_mst.h"
#include <mm/core_memprot.h>
#include <initcall.h>
#include <kernel/pm.h>

#define MEMMAP_SOC_REG_VA_BASE (vaddr_t)phys_to_virt_io(MEMMAP_SOC_REG_BASE, MEMMAP_SOC_REG_SIZE)
#define CONFIG_RF_WINDOW_NUM 32

#define RF_WINDOW_NUM		CONFIG_RF_WINDOW_NUM
#define RF_WINDOW_REG_SIZE		(RA_seRegionX_CTRL1-RA_seRegionX_CTRL0)

static uint32_t _g_rf_region_ctrl_offsets[] = {
	RA_SOC_SecureRegionCtrl0,
	RA_SOC_SecureRegionCtrl1,
	RA_SOC_SecureRegionCtrl2,
	RA_SOC_SecureRegionCtrl3,
};

#define RF_WINDOW_START(id)	(_g_rf_region_ctrl_offsets[id / 8])
#define RF_REG_BASE(id)		(MEMMAP_SOC_REG_VA_BASE + RF_WINDOW_START(id) + (id % 8) * RF_WINDOW_REG_SIZE)

#define RFW_MST_BASE                    (MEMMAP_SOC_REG_VA_BASE + RA_SOC_Mstgroup)
#define RFW_MST_SIZE                    (4)
#define RFW_MST_GROUP_NUM               (16)

#define rf_writel(v, a) do {               \
		io_write32(a, v);                    \
	} while(0)


#define readl io_read32

void rf_buffer_registers(void)
{
	DMSG("rf_buffer_registers ##########.\n");
}


static int rfw_master_group_cfg(int master_id, int group)
{
	vaddr_t rfw_mst_base;

	assert(0 <= master_id && master_id < 32);
	assert(0 <= group && group < RFW_MST_GROUP_NUM);

	rfw_mst_base = RFW_MST_BASE + master_id * RFW_MST_SIZE;

	rf_writel(group, rfw_mst_base);
	assert((uint32_t)group == readl(rfw_mst_base));

	return 0;
}

static int rfw_zone_enable(int zone_id, int enable, int secure_only, int lock)
{
	uint32_t val;
	vaddr_t addr;

	assert(0 <= zone_id && zone_id < RF_WINDOW_NUM);
	addr = RF_REG_BASE(zone_id) + RA_SecureZoneX_REGION_CTRL;
	val = readl(addr);

	if (enable >= 0){
		val &= ~(1 << LSb32SecureZoneX_REGION_CTRL_Enable);
		val |= (enable ? 1:0) << LSb32SecureZoneX_REGION_CTRL_Enable;
	}

	if (secure_only >= 0){
		val &= ~(1 << LSb32SecureZoneX_REGION_CTRL_NS);
		val |= (secure_only ? 1:0) << LSb32SecureZoneX_REGION_CTRL_NS;
	}

	if (lock >= 0) {
		val &= ~(1 << LSb32SecureZoneX_REGION_CTRL_Lock);
		val |= (lock ? 1:0) << LSb32SecureZoneX_REGION_CTRL_Lock;
	}

	rf_writel(val, addr);
	if(val != readl(addr)) {
		EMSG("RF window %d enable %d, secure_only %d, lock %d, val = 0x%x, reg_val = 0x%x\n",
			zone_id, enable, secure_only, lock, val, readl(addr));
		assert(0);
	}

	return 0;
}

static int rfw_zone_cfg_set_default(int zone_id, uint32_t base, uint32_t end, int perm, uint32_t mst_group_ids)
{
	uint32_t val;
	vaddr_t reg_base;

	assert(0 <= zone_id && zone_id < RF_WINDOW_NUM);
	assert(mst_group_ids <= 0xff);

	reg_base = RF_REG_BASE(zone_id);

	/* set start & end */
	rf_writel(base, reg_base + RA_SecureZoneX_REGION_SADDR);
	if(base != readl(reg_base + RA_SecureZoneX_REGION_SADDR)) {
		EMSG("RF window %d start address mismatch, start address 0x%x is already set, can not set it to  0x%x\n",
			zone_id, readl(reg_base + RA_SecureZoneX_REGION_SADDR), base);
		assert(0);
	}


	rf_writel(end, reg_base + RA_SecureZoneX_REGION_EADDR);
	if (end != readl(reg_base + RA_SecureZoneX_REGION_EADDR)) {
		EMSG("RF window %d end address mismatch, end address 0x%x is already set, can not set it to  0x%x\n",
			zone_id, readl(reg_base + RA_SecureZoneX_REGION_EADDR), end);
		assert(0);
	}

	mst_group_ids &= 0xff;

	//secure read and write enable
	val = 0;
	if (TZ_IS_SR(perm))
		val |= mst_group_ids;

	if (TZ_IS_SW(perm))
		val |= (mst_group_ids <<  16);

	rf_writel(val, reg_base + RA_SecureZoneX_REGION_MST_CFG);
	if (val != readl(reg_base + RA_SecureZoneX_REGION_MST_CFG)) {
		EMSG("RF window %d mst cfg mismatch, mst cfg 0x%x is set, can not set it to  0x%x\n",
			zone_id, readl(reg_base + RA_SecureZoneX_REGION_MST_CFG), val);
		assert(0);
	}

	//non secure read and write enable
	val = 0;
	if (TZ_IS_NR(perm))
		val |= mst_group_ids;

	if (TZ_IS_NW(perm))
		val |= (mst_group_ids <<  16);

	rf_writel(val,  reg_base + RA_SecureZoneX_REGION_MST_CFG1);
	if (val != readl(reg_base + RA_SecureZoneX_REGION_MST_CFG1)) {
		EMSG("RF window %d mst cfg mismatch, mst cfg1 0x%x is set, can not set it to  0x%x\n",
			zone_id, readl(reg_base + RA_SecureZoneX_REGION_MST_CFG1), val);
		assert(0);
	}

	return 0;
}

static int rfw_zone_cfg_set(int zone_id, int group, int perm)
{
	uint32_t val;
	vaddr_t addr;

	assert(0 <= group && group < RFW_MST_GROUP_NUM);
	assert(0 <= zone_id && zone_id < RF_WINDOW_NUM);

	addr = RF_REG_BASE(zone_id) + RA_SecureZoneX_REGION_MST_CFG;
	//secure
	val = readl(addr);

	if (perm & TZ_SRW || perm & TZ_SRO)
		val |= 1 << group;
	else
		val |= ~(1 << group);

	if (perm & TZ_SRW || perm & TZ_SWO)
		val |= (1 << (group + 16));
	else
		val |= ~(1 << (group + 16));

	rf_writel(val, addr);

	addr = RF_REG_BASE(zone_id) + RA_SecureZoneX_REGION_MST_CFG1;
	//non secure
	val = readl(addr);

	if (perm & TZ_NRW || perm & TZ_NRO)
		val |= 1 << group;
	else
		val |= ~(1 << group);

	if (perm & TZ_NRW || perm & TZ_NWO)
		val |= (1 << (group + 16));
	else
		val |= ~(1 << (group + 16));

	rf_writel(val,  addr);

	return 0;
}

static int syna_rf_set_window(int id, uint32_t base, uint32_t size, int perm, uint32_t mst_group_ids, int lock)
{
	DMSG("set rf %x size %x: %x\n", base, size, perm);
	assert(0 <= id && id < RF_WINDOW_NUM);
	assert(base < base + size);

	/* set permission */
	rfw_zone_cfg_set_default(id, base, base + size, perm, mst_group_ids);

	if (!TZ_IS_NNA(perm))
		rfw_zone_enable(id, 1, 1, lock); // zone enable, non secure
	else
		rfw_zone_enable(id, 1, 0, lock); // zone enable, secure

	return 0;
}

int rf_lock(void)
{
	int id = 0;
	for(id = 0; id < RF_WINDOW_NUM; id++) {
		rfw_zone_enable(id, -1, -1, 1);
	}

	return 0;
}

static TEE_Result rf_configure_setup(void)
{
	int i;
	const struct rf_window *rf_value;
	int rf_num;

	rf_value = rf_window_param;
	rf_num = rf_window_num;

	DMSG("rf_num = %d\n", rf_num);

	for (i = 0; i < rf_num; i++) {
		DMSG("rf=%d: base=0x%08x, mask=0x%08x, attr=0x%08x\n",
				rf_value[i].id, rf_value[i].base,
				rf_value[i].mask, rf_value[i].attr);
		// set RF window and lock it
		syna_rf_set_window(rf_value[i].id, rf_value[i].base,
				rf_value[i].mask, RF_GET_PERM(rf_value[i].attr), rf_value[i].mst_group_ids, 1);
	}

	return TEE_SUCCESS;
}

static TEE_Result
pm_enter_resume(enum pm_op op, uint32_t pm_hint __unused,
		const struct pm_callback_handle *pm_handle __unused)
{
	DMSG("op = 0x%x.\n", op);
	if (op == PM_OP_RESUME)
		return rf_configure_setup();

	return TEE_SUCCESS;
}

static TEE_Result rf_setup(void)
{
	register_pm_driver_cb(pm_enter_resume, NULL, "syna_rf");

	rf_configure_setup();

	return TEE_SUCCESS;
}

driver_init(rf_setup);

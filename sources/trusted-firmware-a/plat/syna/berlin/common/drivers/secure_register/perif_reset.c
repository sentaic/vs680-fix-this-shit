/*
 * Copyright (c) 2026, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <assert.h>
#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>

#include "tz_boot_param.h"
#include "perif_reset.h"
#include "perif_reset_id.h"

enum reset_type {
	PERIF_RESET,
	PERIF_ASSERT,
	PERIF_DEASSERT
};

#ifdef PCIE_RESET

#define PCIE_REGION_NAME "PCIE"

#define MAX_SYSTEM_NUM 3

const char *systems[MAX_SYSTEM_NUM] = {"bootloader", "system", "system_1"};

static void set_pcie_axi_limit(void)
{
	const struct syna_mem_region *mr_pcie = mr_find_region_by_name(PCIE_REGION_NAME);
	const struct syna_mem_region *mr = NULL;
	uint32_t lower_addr = 0;
	uint32_t upper_addr = 0;
	int i;

	for (i = 0; i < MAX_SYSTEM_NUM; i++) {
		mr = mr_find_region_by_name(systems[i]);
		if (mr){
			if (!lower_addr && !upper_addr) {
				lower_addr = mr->base;
				upper_addr = mr->base + mr->size;
				continue;
			}
			if (mr->base < lower_addr)
				lower_addr = mr->base;

			if (mr->base + mr->size > upper_addr)
				upper_addr = mr->base + mr->size;
		}

	}

	if (mr_pcie != NULL) {
		if((mr_pcie->base > lower_addr) &&
			((mr_pcie->base + mr_pcie->size) < upper_addr)) {
			lower_addr = mr_pcie->base;
			upper_addr = mr_pcie->base + mr_pcie->size;
		}
	}

	if (!lower_addr || !upper_addr) {
		assert(0);
	}

	mmio_write_32(PCIE_LOWER_BOUNDARY_ADDR , lower_addr);
	mmio_write_32(PCIE_UPPER_BOUNDARY_ADDR, upper_addr);

	INFO("PCIE 0x%x:0x%x, 0x%x:0x%x\n",
				PCIE_LOWER_BOUNDARY_ADDR, mmio_read_32(PCIE_LOWER_BOUNDARY_ADDR),
				PCIE_UPPER_BOUNDARY_ADDR, mmio_read_32(PCIE_UPPER_BOUNDARY_ADDR));
}

static void set_pcie_addr_filter_flag(void)
{
	uint32_t val;

	val = mmio_read_32(PCIE_MAC_CTRL);
	val |= 	(1 << LSb32PCIE_MAC_CTRL_ADDR_FILTER_FLAG);
	mmio_write_32(PCIE_MAC_CTRL, val);

	INFO("PCIE MAC CTR 0x%x\n", mmio_read_32(PCIE_MAC_CTRL));
}
#endif

static void set_bit(uint32_t reg_addr, uint32_t bit)
{
	uint32_t val;

	val = mmio_read_32(reg_addr);
	val |= 1 << bit;
	mmio_write_32(reg_addr, val);

#ifdef PCIE_RESET
	if (bit == PCIE0RST_ID) {
		set_pcie_axi_limit();
		set_pcie_addr_filter_flag();
	}
#endif
}
static void clear_bit(uint32_t reg_addr, uint32_t bit)
{
	uint32_t val;

	val = mmio_read_32(reg_addr);
	val &= ~(1 << bit);
	mmio_write_32(reg_addr, val);
}

int berlin_perif_reset(u_register_t x1, u_register_t x2)
{
	uint32_t type = (uint32_t)x1;
	uint32_t id = (uint32_t)x2;
	uint32_t reg_addr = SOC_REST_PERIF_BASE;

	if (id >= MAX_PERIF_ID) {
		ERROR("%s: unsupported id (0x%x)\n", __func__, id);
		return SMC_UNK;
	}

	INFO("reset type %x perif id 0x%d\n", type, id);

	switch (type) {
	case PERIF_RESET:
		// assert
		clear_bit(reg_addr, id);
		//wailt
		udelay(10);
		//deasset
		set_bit(reg_addr, id);
		break;
	case PERIF_ASSERT:
		clear_bit(reg_addr, id);
		break;
	case PERIF_DEASSERT:
		set_bit(reg_addr, id);
		break;
	default:
		ERROR("%s: unhandled reset type (0x%x)\n", __func__, type);
		return SMC_UNK;
		}

	return SMC_OK;
}



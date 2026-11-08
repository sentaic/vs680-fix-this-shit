/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "clock.h"
#include "klamath_memmap.h"


#define RA_Gbl_sysPll_0 			0x0200
#define RA_Gbl_ClkSwitch			0x0710

//#define MEMMAP_CA7_REG_BASE			0xF7920000
#define RA_CPU_WRP_PLL_REG			0x2000

#define MEMMAP_AVIO_REG_BASE		0xF7400000
#define AVIO_MEMMAP_AVIO_GBL_BASE	0x60000
#define RA_avioGbl_SWPDWN_CTRL		0x20E8

static void pll_bypass_en(clock_config_t *config)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data |= 0xff;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void pll_bypass_dis(clock_config_t *config)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data = 0;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void pll_power_on(clock_config_t *config)
{
	/* Power on the SYS pll0 */
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPll_0, 0x10);

	/* Power on the CPU pll */
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_WRP_PLL_REG, 0x10);

	/* Power on the APLL0, APLL1, VPLL0, VPLL1 */
	mmio_write_32(MEMMAP_AVIO_REG_BASE +  AVIO_MEMMAP_AVIO_GBL_BASE+ RA_avioGbl_SWPDWN_CTRL, 0x0);

}

static void pll_power_off(clock_config_t *config)
{
	/* Power off the APLL0, APLL1, VPLL0, VPLL1 */
	mmio_write_32(MEMMAP_AVIO_REG_BASE+  AVIO_MEMMAP_AVIO_GBL_BASE+ RA_avioGbl_SWPDWN_CTRL, 0xf);
	/* Power off the SYS pll0 */
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPll_0, 0x12);

	/* Power off the CPU pll */
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_WRP_PLL_REG, 0x12);

}

static clock_config_t clock_config;

void clock_set_config(clock_config_t *config)
{
	assert(config != NULL);

	memcpy(&clock_config, config, sizeof(clock_config_t));

	assert(clock_config.cpu_pll_base != NULL);
	assert(clock_config.mem_pll_base != NULL);
	assert(clock_config.sys_pll0_base != NULL);
}

void clock_suspend(void)
{
	pll_bypass_en(&clock_config);

	pll_power_off(&clock_config);
}

void clock_resume(void)
{
	pll_power_on(&clock_config);

	pll_bypass_dis(&clock_config);
}

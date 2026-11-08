/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "clock_regs.h"
#include "clock.h"
#include "avpll.h"

#define BFM_HOST_Bus_Write32(offset, val)		\
			mmio_write_32((unsigned long)offset, val)

#define BFM_HOST_Bus_Read32(offset, val)		\
			val = mmio_read_32((unsigned long)offset)

static void mempll_bypass_en(void)
{
	register unsigned int data;

	//enable mem fastrefclk
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memfastRefClk);
	data |= 1;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memfastRefClk, data);

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data |= MSK32Gbl_ClkSwitch_memPLLSWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}
static void mempll_bypass_dis(void)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data &= ~MSK32Gbl_ClkSwitch_memPLLSWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void syspll_bypass_en(void)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data |= MSK32Gbl_ClkSwitch_sysPLL_0SWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data |= MSK32Gbl_ClkSwitch_sysPLL_1SWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void syspll_bypass_dis(void)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data &= ~MSK32Gbl_ClkSwitch_sysPLL_0SWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);

	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data &= ~MSK32Gbl_ClkSwitch_sysPLL_1SWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void cpupll_bypass_en(void)
{
	register unsigned int data;

	//enable mem fastrefclk
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpufastRefClk);
	data |= 1;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpufastRefClk, data);


	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data |= MSK32Gbl_ClkSwitch_cpuPLLSWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void cpupll_bypass_dis(void)
{
	register unsigned int data;

	// global Pll bypass enable
	data = mmio_read_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch);
	data &= ~MSK32Gbl_ClkSwitch_cpuPLLSWBypass;
	mmio_write_32(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch, data);
}

static void pll_bypass_en(clock_config_t *config)
{
	/* MEM PLL bypass enable */
	mempll_bypass_en();

	/* SYS PLL bypass enable */
	syspll_bypass_en();

	/* CPU PLL bypass enable */
	cpupll_bypass_en();
}

static void pll_bypass_dis(clock_config_t *config)
{
	/* CPU PLL bypass disable */
	cpupll_bypass_dis();

	/* SYS PLL bypass disable */
	syspll_bypass_dis();

	/* MEM PLL bypass disable */
	mempll_bypass_dis();

}

static void pll_power_up(void *pll_base)
{
    unsigned int data;

	//reset
	data = mmio_read_32((unsigned long)pll_base + RA_abipll_ctrlA);
	data &= ~MSK32abipll_ctrlA_RESET;
	mmio_write_32((unsigned long)pll_base + RA_abipll_ctrlA, data);

	//delay 50us
	//udelay(50);

	//check for PLL lock
	do
	{
		data = mmio_read_32((unsigned long)pll_base + RA_abipll_status);
		data &= MSK32abipll_status_LOCK;
	}while(data==0);

}

static void pll_power_down(void *pll_base)
{
	unsigned int data;

	//reset PLL
	data = mmio_read_32((unsigned long)pll_base + RA_abipll_ctrlA);
	data |= (1 << LSb32abipll_ctrlA_RESET);
	mmio_write_32((unsigned long)pll_base + RA_abipll_ctrlA, data);
}

static void pll_power_on(clock_config_t *config)
{
	/* Power on the SYS pll0 */
	pll_power_up(config->sys_pll0_base);

	/* Power on the SYS pll1 */
	pll_power_up(config->sys_pll1_base);

	/* Power on the CPU pll */
	pll_power_up(config->cpu_pll_base);

	/* Power on the MEM pll */
	pll_power_up(config->mem_pll_base);
}

static void pll_power_off(clock_config_t *config)
{
	/* Power off the SYS pll0 */
	pll_power_down(config->sys_pll0_base);

	/* Power off the SYS pll1 */
	pll_power_down(config->sys_pll1_base);

	/* Power off the CPU pll */
	pll_power_down(config->cpu_pll_base);

	/* Power off the MEM pll */
	//pll_power_down(config->mem_pll_base);
}

static clock_config_t clock_config;

void clock_set_config(clock_config_t *config)
{
	assert(config != NULL);

	memcpy(&clock_config, config, sizeof(clock_config_t));

	assert(clock_config.cpu_pll_base != NULL);
	assert(clock_config.mem_pll_base != NULL);
	assert(clock_config.sys_pll0_base != NULL);
	assert(clock_config.sys_pll1_base != NULL);
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

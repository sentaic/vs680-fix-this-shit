/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>
#include <platform_def.h>

#define readl(a)		mmio_read_32((uintptr_t)(a))
#define writel(v, a)	mmio_write_32((uintptr_t)(a), v)

#ifndef CONFIG_DW_WDT_BASE
#define CONFIG_DW_WDT_BASE				0xF7E81400
#define RA_Gbl_WDTSysRstMask			0x0484
#endif
#define SOC_CHIP_CTRL_REG_BASE			MEMMAP_CHIP_CTRL_REG_BASE

#define CONFIG_DW_WDT_CLOCK_KHZ			25000
#define SYSTEM_RESET_MODE	0

#define DW_WDT_CR	0x00
#define DW_WDT_TORR	0x04
#define DW_WDT_CRR	0x0C

#define DW_WDT_CR_EN_OFFSET		0x00
#define DW_WDT_CR_RMOD_OFFSET	0x01
#define DW_WDT_CRR_RESTART_VAL	0x76
#define DW_WDT_CR_RPL_OFFSET	0x02
#define DW_WDT_CR_RPL_MASK		0x07
#define DW_WDT_CR_RPL_PCLK_8	0x02

static void berlin_dw_wdt_set_rspmode(int mode)
{
	unsigned int reg = readl(CONFIG_DW_WDT_BASE + DW_WDT_CR);
	if (mode)
		reg |= (1 << DW_WDT_CR_RMOD_OFFSET);
	else
		reg &= ~(1 << DW_WDT_CR_RMOD_OFFSET);
	writel(reg, CONFIG_DW_WDT_BASE + DW_WDT_CR);
}

static void berlin_dw_wdt_set_rpl(unsigned int rpl)
{
	unsigned int reg = readl(CONFIG_DW_WDT_BASE + DW_WDT_CR);
	reg &= ~(DW_WDT_CR_RPL_MASK << DW_WDT_CR_RPL_OFFSET);
	reg |= (rpl << DW_WDT_CR_RPL_OFFSET);
	writel(reg, CONFIG_DW_WDT_BASE + DW_WDT_CR);
}

static void berlin_dw_wdt_init(void)
{
	unsigned int wdt_mask;

	//Gbl + 0x0484 WDTSysRstMask
	wdt_mask = readl(SOC_CHIP_CTRL_REG_BASE + RA_Gbl_WDTSysRstMask);
	//Clear bit0 for wdt0 mask, bit1 for wdt1, bit2 for wdt2
	wdt_mask &= ~(1<<0);
	writel(wdt_mask, SOC_CHIP_CTRL_REG_BASE + RA_Gbl_WDTSysRstMask);

	//interrupt mode, and reset signal pulse repeat 8 pclk cycles
	berlin_dw_wdt_set_rspmode(SYSTEM_RESET_MODE);
	berlin_dw_wdt_set_rpl(DW_WDT_CR_RPL_PCLK_8);
	writel(0, (CONFIG_DW_WDT_BASE + DW_WDT_TORR));
}

static void berlin_dw_wdt_enable(void)
{
	unsigned int reg = readl(CONFIG_DW_WDT_BASE + DW_WDT_CR);
	reg |= 0x1 << DW_WDT_CR_EN_OFFSET;
	writel(reg, CONFIG_DW_WDT_BASE + DW_WDT_CR);
}

static unsigned int berlin_dw_wdt_is_enabled(void)
{
	unsigned long val;
	val = readl((CONFIG_DW_WDT_BASE + DW_WDT_CR));
	return val & 0x1;
}

static void berlin_dw_wdt_restart(void)
{
	if (berlin_dw_wdt_is_enabled())
		/* restart the watchdog counter */
		writel(DW_WDT_CRR_RESTART_VAL,
		       (CONFIG_DW_WDT_BASE + DW_WDT_CRR));
}

void reset_soc(void)
{
	berlin_dw_wdt_init();
	berlin_dw_wdt_enable();
	berlin_dw_wdt_restart();
	while (1)
		/*nothing*/;

}

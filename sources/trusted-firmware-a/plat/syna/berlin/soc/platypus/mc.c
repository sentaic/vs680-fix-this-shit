/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "mc.h"

#define MEMMAP_MCTRLSS_REG_BASE		0xF7940000
#define RA_mc_wrap_bufferen_core	0x03D0

#define RD_MC_REG(offset , data)	\
		data = mmio_read_32((unsigned long)((char*)base + offset))

#define WR_MC_REG(offset , value)	\
		mmio_write_32((unsigned long)((char*)base + offset), value)

#define MC_REG(offset)	(*(volatile unsigned int *)(unsigned long)((char*)base + (offset)))
#define MEMPLL_BYPASS_ENABLE	1

/*
 * DDR enter self-refresh
 * DIAG: G-Diag\Platypus\code\modules\mempll\self_refresh.c
 * function: enterselfrefresh
 */
static void ddr_enter_self_refresh(void *base)
{
	// 1.Program PCTRL_n.port_en = 0 – to block traffic on AXI port
	MC_REG(0x490) = 0x0;
	MC_REG(0x540) = 0x0;
	MC_REG(0x5f0) = 0x0;
	MC_REG(0x6a0) = 0x0;
	// 2.Poll PSTAT.rd_port_busy_n = 0 and PSTAT.wr_port_busy_n = 0 – wait for AXI ports are idle
	while (MC_REG(0x3fc) & ((0x1f<<0) | (0x1f<<16)));
	// 3.Program DBG1.dis_hif = 1 – no new command will be accepted by DDRC
	MC_REG(0x0304) |= 0x2;
	// 4.Poll DBGCAM.dbg_wr/rd_q_empty =1– to ensure that write and read data buffers are empty
	while (((MC_REG(0x0308) >> 25) & 0x3) != 0x3);
#if (MEMPLL_BYPASS_ENABLE == 1)
	// for lower-down/bypass/disable mempll when SR - LP2 mode, "Set DFILPCFG0.dfi_lp_en_sr = 0, if DFILPCFG0.dfi_lp_en_sr = 1, and wait until DFISTAT.dfi_lp_ack = 0."
	MC_REG(0x0198) &= ~(1<<8);
	while (MC_REG(0x01bc) & (1<<1));
#endif // (MEMPLL_BYPASS_ENABLE
	// 5.Program PWRCTL.selfref_sw = 1 – core to enter self-refresh mode
	MC_REG(0x0030) |= 0x20;
	// 6.Wait until STAT.operation_mode[1:0] = ‘b11 – to indicate that controller is in self-refresh mode, STAT.selfref_type = ‘b10 – to make sure transition to self-refresh was due to software
	while ((MC_REG(0x0004) & ((0x3<<0) | (0x3<<4) | (0x3<<8))) != ((0x3 << 0) | (0x2 << 4) | (0x2 << 8)));
}


/*
 * DDR exit self-refresh
 * DIAG: G-Diag\Platypus\code\modules\mempll\self_refresh.c
 * function: exitselfrefresh
 */
static void ddr_exit_self_refresh(void *base)
{
	// 1.Wait at least tCKESR, perform exit self-refresh by de-asserting PWRCTL.selfref_sw = 0
	MC_REG(0x0030) &= ~0x20;
	// 2.Poll for STAT.oprtating_mode !=11 indicate MC core is not in self-refresh mode
	while ((MC_REG(0x0004) & 0x3) == 0x3);
	// 3.Enable HIF interface by setting dis_hif = 0
	MC_REG(0x0304) &= ~0x2;
	// 4.Program PCTRL_n.port_en = 1 – to unblock traffic on AXI port
	MC_REG(0x490) = 0x1;
	MC_REG(0x540) = 0x1;
	MC_REG(0x5f0) = 0x1;
	MC_REG(0x6a0) = 0x1;
}


/*
 * Power down the PHY of MC6
 */
static void mc_phy_power_down(void *base)
{
	*(volatile unsigned int *)(unsigned long)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_bufferen_core) = 0x0;
#if (MEMPLL_BYPASS_ENABLE == 1)
	MC_REG(0x0320) &= ~(1<<0);	// Enable Quasi-Dynamic Register Programming
	MC_REG(0x01b0) &= ~(1<<0);	// Program DFIMISC.dfi_init_complete_en = 0
	MC_REG(0x01b0) |= (1<<5);	// Program DFIMISC.dfi_init_start = 1
	while (MC_REG(0x01bc) & (1<<0)); // Poll DFISTAT.dfi_init_complete = 0
	MC_REG(0x0320) |= (1<<0);  // Disable Quasi-Dynamic Register Programming
#endif // (MEMPLL_BYPASS_ENABLE == 1)
}

/*
 * Power up the PHY of MC6
 */
static void mc_phy_power_up(void* base)
{
	//fix me: DDR PHY power up is done in SM

}

static mem_config_t mem_config;

void mem_set_config(mem_config_t *config)
{
	assert(config != NULL);

	memcpy(&mem_config, config, sizeof(mem_config_t));

	assert(mem_config.mc6_base != NULL);
}

void mem_suspend(void)
{
	ddr_enter_self_refresh(mem_config.mc6_base);
	mc_phy_power_down(mem_config.mc6_base);
}

void mem_resume(void)
{
	mc_phy_power_up(mem_config.mc6_base);
	ddr_exit_self_refresh(mem_config.mc6_base);
}

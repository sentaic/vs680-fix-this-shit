/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "mc.h"
#include "clock_regs.h"

#define RD_MC_REG(offset , data)	\
		data = mmio_read_32((unsigned long)((char*)base + offset))

#define WR_MC_REG(offset , value)	\
		mmio_write_32((unsigned long)((char*)base + offset), value)

#define MC_REG(offset)	(*(volatile unsigned int *)(unsigned long)((char*)base + (offset)))

#define MEMPLL_BYPASS_ENABLE	1

static void soft_udelay(int us)
{
    int i;
    for (i = 0; i < (us * 100); i++);
}


/*
 * DDR enter self-refresh
 * Diag: G-Diag/BerlinBG7/code/modules/mempll/self_refresh.c
 * function:enterselfrefresh
 */
static void ddr_enter_self_refresh(void *base)
{
	// 1.Program PCTRL_n.port_en = 0 – to block traffic on AXI port
	MC_REG(0x490) = 0x0;
	MC_REG(0x540) = 0x0;
	MC_REG(0x5f0) = 0x0;
	MC_REG(0x6a0) = 0x0;
	MC_REG(0x750) = 0x0;
	// 2.Poll PSTAT.rd_port_busy_n = 0 and PSTAT.wr_port_busy_n = 0 – wait for AXI ports are idle
	while (MC_REG(0x3fc) & ((0x1f<<0) | (0x1f<<16)));
	// 3.Program DBG1.dis_hif = 1 – no new command will be accepted by DDRC
	MC_REG(0x0304) |= 0x2;
	MC_REG(0x1e04) |= 0x2;
	// 4.Poll DBGCAM.dbg_wr/rd_q_empty =1– to ensure that write and read data buffers are empty
	while (((MC_REG(0x0308) >> 25) & 0x3) != 0x3);
	while (((MC_REG(0x1e08) >> 25) & 0x3) != 0x3);

#if (MEMPLL_BYPASS_ENABLE == 1)
	MC_REG(0x0198) &= ~(1<<8);
	while (MC_REG(0x01bc) & (1<<1)) {
		soft_udelay(10);
	};
	while (MC_REG(0x1cbc) & (1<<1)) {
		soft_udelay(10);
	};
#endif
	// 5.Program PWRCTL.selfref_sw = 1 – core to enter self-refresh mode
	MC_REG(0x0030) |= 0x20;
	MC_REG(0x1b30) |= 0x20;
	// 6.Wait until STAT.operation_mode[1:0] = ‘b11 – to indicate that controller is in self-refresh mode, STAT.selfref_type = ‘b10 – to make sure transition to self-refresh was due to software
	while ((MC_REG(0x0004) & ((0x3<<0) | (0x3<<4) | (0x3<<8))) != ((0x3 << 0) | (0x2 << 4) | (0x2 << 8))) {
		soft_udelay(10);
	};
	while ((MC_REG(0x1b04) & ((0x3<<0) | (0x3<<4) | (0x3<<8))) != ((0x3 << 0) | (0x2 << 4) | (0x2 << 8))) {
		soft_udelay(10);
	};

#if (MEMPLL_BYPASS_ENABLE == 1)
		MC_REG(0x0320) &= ~(1<<0);	// Enable Quasi-Dynamic Register Programming
		MC_REG(0x01b0) &= ~(1<<0);	// Program DFIMISC.dfi_init_complete_en = 0
		MC_REG(0x01b0) |= (1<<5);	// Program DFIMISC.dfi_init_start = 1
		while (MC_REG(0x01bc) & (1<<0)); // Poll DFISTAT.dfi_init_complete = 0
		while (MC_REG(0x1cbc) & (1<<0)); // Poll DCH1.DFISTAT.dfi_init_complete = 0
		MC_REG(0x0320) |= (1<<0);  // Disable Quasi-Dynamic Register Programming

		*(volatile unsigned int*)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_ctrlA) |= (1<<LSb32abipll_ctrlA_BYPASS);
#endif // (MEMPLL_BYPASS_ENABLE == 1)

}


/*
 * DDR exit self-refresh
 */
static void ddr_exit_self_refresh(void *base)
{
#if (MEMPLL_BYPASS_ENABLE == 1)
	// disable mempll bypass
	*(volatile unsigned int*)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch) |= (1<<LSb32Gbl_ClkSwitch_memPLLSWBypass); // sw-bypass to memfastClk
	*(volatile unsigned int*)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_ctrlA) &= ~(1<<LSb32abipll_ctrlA_BYPASS);
	//udelay(50);
	while (!(*(volatile unsigned int*)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_status) & (1<<0)));
	//udelay(50);
	*(volatile unsigned int*)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch) &= ~(1<<LSb32Gbl_ClkSwitch_memPLLSWBypass); // disable sw-bypass

	MC_REG(0x0320) &= ~(1<<0);	// Enable Quasi-Dynamic Register Programming
	MC_REG(0x01b0) &= ~(1<<0);	// Program DFIMISC.dfi_init_complete_en = 0
	MC_REG(0x01b0) &= ~(1<<5);	 // Program DFIMISC.dfi_init_start = 0
	while (!(MC_REG(0x01bc) & (1<<0))); // Poll DFISTAT.dfi_init_complete = 1
	while (!(MC_REG(0x1cbc) & (1<<0))); // Poll DCH1.DFISTAT.dfi_init_complete = 1
	MC_REG(0x01b0) |= (1<<0);  // Program DFIMISC.dfi_init_complete_en = 1
	MC_REG(0x0320) |= (1<<0);  // Disable Quasi-Dynamic Register Programming

	// for lower-down/bypass/disable mempll when SR - LP2 mode, "Reset DFILPCFG0.dfi_lp_en_sr = 1 if DFILPCFG0.dfi_lp_en_sr has been set to 0 in step 10."
	MC_REG(0x0198) |= (1<<8);
#endif // (MEMPLL_BYPASS_ENABLE == 1)

	// 1.Wait at least tCKESR, perform exit self-refresh by de-asserting PWRCTL.selfref_sw = 0
	MC_REG(0x0030) &= ~0x20;
	MC_REG(0x1b30) &= ~0x20;
	// 2.Poll for STAT.oprtating_mode !=11 indicate MC core is not in self-refresh mode
	while ((MC_REG(0x0004) & 0x3) == 0x3);
	while ((MC_REG(0x1b04) & 0x3) == 0x3);
	// 3.Enable HIF interface by setting dis_hif = 0
	MC_REG(0x0304) &= ~0x2;
	MC_REG(0x1e04) &= ~0x2;
	// 4.Program PCTRL_n.port_en = 1 – to unblock traffic on AXI port
	MC_REG(0x490) = 0x1;
	MC_REG(0x540) = 0x1;
	MC_REG(0x5f0) = 0x1;
	MC_REG(0x6a0) = 0x1;
	MC_REG(0x750) = 0x1;
}


/*
 * Power down the PHY of MC6
 */
static void mc_phy_power_down(void *base)
{
	//fix me: DDR PHY power down is done in SM
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

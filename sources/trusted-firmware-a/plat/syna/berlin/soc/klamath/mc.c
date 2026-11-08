/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "berlin_def.h"
#include "klamath_memmap.h"
#include "mc.h"
#include "mc_wrap.h"
#include "global.h"

#define MEMMAP_MCTRLSS_REG_BASE		START_MCTRLSS_REG

#define MEMMAP_DDR_REG_BASE 0xF7CB0000
#define MC_REG(offset)    (*(volatile unsigned int *)(unsigned long)(MEMMAP_DDR_REG_BASE + (offset)))
#define MAX_LOOP_COUNT 10000
#define MEMPLL_BYPASS_ENABLE    1

static inline void reset_soc(void)
{
	*(volatile uint32_t*)SOC_RESET_CTRL_BASE = 1;
}

static void soft_udelay(int us)
{
	int i;
	for (i = 0; i < (us * 100); i++);
}

/*
 * DDR enter self-refresh
 * Code refers: G-Diag\DVF120\code\modules\standby\acpu_standby\source\acpu_standby_self_refresh.c
 */
static void ddr_enter_self_refresh(void)
{
	int i = 0;;
	// 1.Program PCTRL_n.port_en = 0 – to block traffic on AXI port
	MC_REG(0x490) = 0x0;
	MC_REG(0x540) = 0x0;

	// 2.Poll PSTAT.rd_port_busy_n = 0 and PSTAT.wr_port_busy_n = 0 – wait for AXI ports are idle
	while (MC_REG(0x3fc) & ((0x3<<0) | (0x3<<16))) {
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	// 3.Program DBG1.dis_hif = 1 – no new command will be accepted by DDRC
	MC_REG(0x0304) |= 0x2;

	// 4.Poll DBGCAM.dbg_wr/rd_q_empty =1– to ensure that write and read data buffers are empty
	i = 0;
	while (((MC_REG(0x0308) >> 25) & 0x3) != 0x3){
		soft_udelay(10);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

#if (MEMPLL_BYPASS_ENABLE == 1)
	// for lower-down/bypass/disable mempll when SR - LP2 mode, "Set DFILPCFG0.dfi_lp_en_sr = 0, if DFILPCFG0.dfi_lp_en_sr = 1, and wait until DFISTAT.dfi_lp_ack = 0."
	MC_REG(0x0198) &= ~(1<<8);
	i = 0;
	while (MC_REG(0x01bc) & (1<<1)){
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}
#endif // (MEMPLL_BYPASS_ENABLE == 1)

	// 5.Program PWRCTL.selfref_sw = 1 – core to enter self-refresh mode
	MC_REG(0x0030) |= 0x20;

	// 6.Wait until STAT.operation_mode[1:0] = ‘b11 – to indicate that controller is in self-refresh mode, STAT.selfref_type = ‘b10 – to make sure transition to self-refresh was due to software
	i = 0;
    while ((MC_REG(0x4) & ((0x3<<0) | (0x3<<4))) != ((0x3 << 0) | (0x2 << 4))) {
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	*(volatile unsigned int *)(unsigned long)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_bufferen_core) = 0x0;
#if (MEMPLL_BYPASS_ENABLE == 1)
	MC_REG(0x0320) &= ~(1<<0);	// Enable Quasi-Dynamic Register Programming
	MC_REG(0x01b0) &= ~(1<<0);	// Program DFIMISC.dfi_init_complete_en = 0
	MC_REG(0x01b0) |= (1<<5);	// Program DFIMISC.dfi_init_start = 1

	// Poll DFISTAT.dfi_init_complete = 0
	i = 0;
	while (MC_REG(0x01bc) & (1<<0)){
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	MC_REG(0x0320) |= (1<<0);  // Disable Quasi-Dynamic Register Programming

    *(volatile unsigned int*)(unsigned long)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_ctrlA) |= (1<<LSb32abipll_ctrlA_BYPASS);
#endif // (MEMPLL_BYPASS_ENABLE == 1)


}


/*
 * DDR exit self-refresh
 */
static void ddr_exit_self_refresh()
{
	int i = 0;

#if (MEMPLL_BYPASS_ENABLE == 1)
	// disable mempll bypass
	*(volatile unsigned int*)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch) |= (1<<LSb32Gbl_ClkSwitch_memPLLSWBypass); // sw-bypass to memfastClk
	*(volatile unsigned int*)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_ctrlA) &= ~(1<<LSb32abipll_ctrlA_BYPASS);
	soft_udelay(50);

	while (!(*(volatile unsigned int*)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll + RA_abipll_status) & (1<<0))){
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	soft_udelay(50);
	*(volatile unsigned int*)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch) &= ~(1<<LSb32Gbl_ClkSwitch_memPLLSWBypass); // disable sw-bypass

	MC_REG(0x0320) &= ~(1<<0);	// Enable Quasi-Dynamic Register Programming
	MC_REG(0x01b0) &= ~(1<<0);	// Program DFIMISC.dfi_init_complete_en = 0
	MC_REG(0x01b0) &= ~(1<<5);	 // Program DFIMISC.dfi_init_start = 0

	// Poll DFISTAT.dfi_init_complete = 1
	i = 0;
	while (!(MC_REG(0x01bc) & (1<<0))){
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	MC_REG(0x01b0) |= (1<<0);  // Program DFIMISC.dfi_init_complete_en = 1
	MC_REG(0x0320) |= (1<<0);  // Disable Quasi-Dynamic Register Programming

	// for lower-down/bypass/disable mempll when SR - LP2 mode, "Reset DFILPCFG0.dfi_lp_en_sr = 1 if DFILPCFG0.dfi_lp_en_sr has been set to 0 in step 10."
	MC_REG(0x0198) |= (1<<8);
#endif // (MEMPLL_BYPASS_ENABLE == 1)

    *(volatile unsigned int *)(unsigned long)(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_bufferen_core) = 0x1;
	// 1.Wait at least tCKESR, perform exit self-refresh by de-asserting PWRCTL.selfref_sw = 0
	MC_REG(0x0030) &= ~0x20;
	// 2.Poll for STAT.oprtating_mode !=11 indicate MC core is not in self-refresh mode
	i = 0;
	while ((MC_REG(0x0004) & 0x3) == 0x3){
		soft_udelay(50);
		if (++i == MAX_LOOP_COUNT) {
			ERROR("time out, reset soc!\n");
			reset_soc();
		}
	}

	// 3.Enable HIF interface by setting dis_hif = 0
	MC_REG(0x304) &= ~0x2;
	// 4.Program PCTRL_n.port_en = 1 – to unblock traffic on AXI port
	MC_REG(0x490) = 0x1;
	MC_REG(0x540) = 0x1;
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
	ddr_enter_self_refresh();
}

void mem_resume(void)
{
	ddr_exit_self_refresh();
}

/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <common/debug.h>
#include <platform_def.h>
#include <drivers/console.h>
#include "clock.h"
#include "mc6.h"
#include "sm.h"
#include "tz.h"
#include <drivers/ti/uart/uart_16550.h>

#pragma weak berlin_idle
#pragma weak berlin_suspend
#pragma weak berlin_resume

void berlin_idle(void)
{
	unsigned long scr = read_scr_el3();

	/* Enable FIQ trap into EL3 */
	write_scr_el3(scr | SCR_FIQ_BIT);

	dsbsy();
	wfi();

	/* Restore the FIQ bit in SCR_EL3 */
	write_scr_el3(scr);
}

void berlin_suspend(unsigned int power_state, void *param)
{
	tos_suspend(param, power_state);

	mem_suspend();

	clock_suspend();

	sm_do_system_warmdown(power_state);
}

void berlin_resume(void)
{
	clock_resume();

	mem_resume();
}

void suspend_early_setup(void)
{
	mem_config_t mem_config;
	clock_config_t clock_config;
	sm_config_t sm_config;

	mem_config.mc6_base = (void *)DDR_REG_BASE;
	mem_set_config(&mem_config);

	clock_config.cpu_pll_base = (void *)CPU_WRP_PLL_REG_BASE;
	clock_config.sys_pll0_base = (void *)GBL_SYSPLL0_BASE;
	clock_config.sys_pll1_base = (void *)GBL_SYSPLL1_BASE;
	clock_config.sys_pll2_base = (void *)GBL_SYSPLL2_BASE;
	clock_config.mem_pll_base = (void *)MC6CTRL_MEMPLL_BASE;
	clock_config.avpll_power_base = (void *)AVIO_REG_BASE;
	clock_set_config(&clock_config);

	sm_config.queue_base = (void *)SM_SEND_MSGQ;
	sm_config.sm_ctrl_base = (void *)SMREG_BASE;
	sm_set_config(&sm_config);
}

/*
 * x0		:resume entrypoint
 * x1		:power state
 * ...
 */
unsigned long suspend_main(unsigned long x0,
			unsigned long x1,
			unsigned long x2,
			unsigned long x3)
{
	static console_t console;

	console_16550_register(UART_0_BASE,
		BERLIN_UART_CLOCK,
		BERLIN_UART_BAUDRATE, &console);


	berlin_suspend(x1, (void *)x2);

	berlin_idle();

	berlin_resume();


	return x0;
}

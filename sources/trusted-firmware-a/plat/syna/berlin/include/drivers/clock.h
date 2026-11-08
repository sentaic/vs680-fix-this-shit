/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

typedef struct {
	void *cpu_pll_base;
	void *sys_pll_base;
	void *sys_pll0_base;
	void *sys_pll1_base;
	void *sys_pll2_base;
	void *mem_pll_base;
	void *avpll_power_base;
}clock_config_t;

void clock_set_config(clock_config_t *config);
void clock_suspend(void);
void clock_resume(void);

#endif

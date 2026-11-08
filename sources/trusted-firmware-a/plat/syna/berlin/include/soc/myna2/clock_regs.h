/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CLOCK_REGS_H__
#define __CLOCK_REGS_H__

#define RA_pll_ctrl						0x0000
#define MSK32pll_ctrl_RESET				0x00000002
#define LSb32pll_ctrl_RESET			1

#define RA_pll_ctrl1					0x0004
#define MSK32pll_ctrl_BYPASS_EN			0x04000000
#define LSb32pll_ctrl_BYPASS_EN			26

#define RA_pll_status					0x0018
#define MSK32pll_status_PLL_LOCK		0x00000001

#define RA_Gbl_ClkSwitch						0x0710
#define MSK32Gbl_ClkSwitch_sysPLL_0SWBypass		0x00000001
#define MSK32Gbl_ClkSwitch_sysPLL_1SWBypass		0x00000002
#define MSK32Gbl_ClkSwitch_memPLLSWBypass 		0x00000004
#define MSK32Gbl_ClkSwitch_cpuPLLSWBypass 		0x00000008

#endif

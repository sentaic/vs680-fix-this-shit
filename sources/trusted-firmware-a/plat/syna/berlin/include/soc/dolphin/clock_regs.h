/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CLOCK_REGS_H__
#define __CLOCK_REGS_H__

#define RA_abipll_ctrlA					0x0000
#define LSb32abipll_ctrlA_BYPASS		1
#define LSb32abipll_ctrlA_RESET			0
#define MSK32abipll_ctrlA_RESET			0x00000001
#define MSK32abipll_ctrlA_BYPASS		0x00000002

#define RA_abipll_status				0x001C
#define MSK32abipll_status_LOCK			0x00000001

#define MEMMAP_MCTRLSS_REG_BASE		0xF7940000
#define RA_mc_wrap_memPll			0x0008

#define RA_Gbl_ClkSwitch						0x0710
#define LSb32Gbl_ClkSwitch_memPLLSWBypass		3
#define MSK32Gbl_ClkSwitch_sysPLL_0SWBypass		0x00000001
#define MSK32Gbl_ClkSwitch_sysPLL_1SWBypass		0x00000002
#define MSK32Gbl_ClkSwitch_sysPLL_2SWBypass		0x00000004
#define MSK32Gbl_ClkSwitch_memPLLSWBypass 		0x00000008
#define MSK32Gbl_ClkSwitch_cpuPLLSWBypass 		0x00000010
#define RA_Gbl_cpufastRefClk					0x0720
#define RA_Gbl_memfastRefClk					0x0724

#endif

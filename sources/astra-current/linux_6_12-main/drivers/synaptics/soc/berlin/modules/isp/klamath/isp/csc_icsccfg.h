// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ICSC_CFG_
#define _ICSC_CFG_

#include <linux/types.h>
#include "csc_common.h"

/*-----------------------------------------------------------------------------
 * Macros
 *-----------------------------------------------------------------------------
 */

#define CSC_MAX_ICSC_MODES 20
#define CSC_MAX_ICSC_COEFF_ROWS 3
#define CSC_MAX_ICSC_COEFF_COLMS 3
#define CSC_MAX_ICSC_OFF 3

/*-----------------------------------------------------------------------------
 * External Variables
 *-----------------------------------------------------------------------------
 */
extern uint32_t gICscWindowCoeff[CSC_MAX_ICSC_MODES][CSC_MAX_ICSC_COEFF_ROWS]
	[CSC_MAX_ICSC_COEFF_COLMS];
extern uint32_t gICscWindowOffset_12bits[CSC_MAX_ICSC_MODES][CSC_MAX_ICSC_OFF];
extern uint32_t gICscWindowOffset_10bits[CSC_MAX_ICSC_MODES][CSC_MAX_ICSC_OFF];
extern uint32_t gICscWindowOffset_8bits[CSC_MAX_ICSC_MODES][CSC_MAX_ICSC_OFF];

#endif //_BDR_ICSC_CFG_

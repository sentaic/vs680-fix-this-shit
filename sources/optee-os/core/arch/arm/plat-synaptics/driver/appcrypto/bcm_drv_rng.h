/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_rng.h - BCM EBG Component related structure definition
//
//*****************************************************************************
#ifndef __BCM_DRV_RNG_H__
#define __BCM_DRV_RNG_H__

#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"

STATUS rom_init_random(void);
void rom_get_stack_cookie(uint32_t *cookie);
#endif //#ifndef __BCM_DRV_RNG_H__

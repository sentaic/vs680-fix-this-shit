/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_xbar.h - BCM XBAR Component related structure definition
//
//*****************************************************************************
#ifndef __BCM_DRV_XBAR_H__
#define __BCM_DRV_XBAR_H__

#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"


typedef enum _XBAR_PATH
{
    AES_PATH          = 0,
    DES_PATH          = 1, 
    RC4_PATH          = 2,
    HASH_AES_PATH     = 3,
    AES_HASH_PATH     = 4,
    SP_AES_PATH       = 5,
    SP_AES2_PATH      = 6,
    AES2_PATH         = 7, 
    AES_X2_PATH       = 8,
    AES_MRE_PATH      = 9, 
    ECP_AES_PATH      = 10, 
    ZMODP_AES_PATH    = 11,
    BYPASS_PATH       = 12,
    IN_DMA_PATH       = 13,
    OUT_DMA_PATH      = 14,
    GHASH_PATH        = 15,
    IN_OUT_DMA_PATH   = 16,
}XBAR_PATH;



STATUS bcm_drv_xbar_set_path(uint32_t path_id);

#endif //#ifndef __BCM_DRV_XBAR_H__

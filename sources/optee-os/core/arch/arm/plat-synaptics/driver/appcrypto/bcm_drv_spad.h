/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_spad.h - BCM SPAD Related Driver
//
//*****************************************************************************
#ifndef __BCM_DRV_SPAD_H__
#define __BCM_DRV_SPAD_H__

#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"
#include "bcm_external.h"

typedef enum _SPAD_ACCESS_MODE
{
    SPAD_SEQUENTIAL_ACCESS = 0,
    SPAD_RANDOM_ACCESS     = 1,
    SPAD_CPU_ACCESS        = 2,
    SPAD_WR_REVERSE_ACCESS = 3,
    SPAD_RD_REVERSE_ACCESS = 4
}SPAD_ACCESS_MODE;

void bcm_drv_spad_get_rd_ptr(uint32_t *rd_offset);
void bcm_drv_spad_set_rd_ptr(uint32_t rd_offset);
void bcm_drv_spad_get_wr_ptr(uint32_t* wr_offset);
void bcm_drv_spad_set_wr_ptr(uint32_t wr_offset);
void bcm_drv_spad_set_access_mode(SPAD_ACCESS_MODE access_mode);

void bcm_drv_spad_zeroize(void);

void bcm_drv_spad_reset(void);

void bcm_drv_spad_write(uint32_t* data, uint32_t size_in_word, uint32_t offset);

void bcm_drv_spad_read(uint32_t* data, uint32_t size_in_word, uint32_t offset, uint32_t *ref_size);

#endif //#ifndef __BCM_DRV_SPAD_H__

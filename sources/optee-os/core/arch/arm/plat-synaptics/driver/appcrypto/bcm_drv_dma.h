/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_dma.h - BCM DMA Component related structure definition
//
//*****************************************************************************

#ifndef __BCM_DRV_DMA_H__
#define __BCM_DRV_DMA_H__

#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"
#include "bcm_external.h"

#define DMA_CONTROL_REG_DEFAULT   ((1 << 13) | (0x10 << 8))
#define DMA_CONTROL_REG_CLEAR     ~((BIT(14) | BIT(13)| BIT(12)| BIT(11)| BIT(10)| BIT(9)| BIT(8)))

void bcm_drv_dma_init(void);
void bcm_drv_dma_setup_link_list(uint32_t direction, uint32_t xfer_adr,
        uint32_t xfer_cnt, uint32_t nx_ll_adr, uint32_t *ref_size);
STATUS bcm_drv_dma_wait_4_done(uint32_t direction, uint32_t time_out);
STATUS bcm_drv_dma_in_sp(uint32_t soc_src_addr, uint32_t size_word, uint32_t sp_wr_offset);
STATUS bcm_drv_dma_out_sp(uint32_t soc_dst_addr, uint32_t size_word, uint32_t sp_rd_offset);
STATUS bcm_drv_dma_in(uint32_t soc_src_addr, uint32_t size_word, uint32_t* local_buf);
STATUS bcm_drv_dma_out(uint32_t soc_dst_addr, uint32_t size_word, uint32_t* local_buf);

static inline void bcm_drv_dma_enable(uint32_t direction)
{
    volatile BCM_DMA *dma = (BCM_DMA *) BASE_DMA;

    if(direction)
        dma->output_dma_control_register|=0x1;
    else
        dma->input_dma_control_register |=0x1;
}

#endif  //#ifndef __BCM_DRV_DMA_H__

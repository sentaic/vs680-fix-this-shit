/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_spad.c - BCM SPAD Related Driver
//
//*****************************************************************************
#include "bcm_drv_spad.h"

void bcm_drv_spad_get_rd_ptr(uint32_t *rd_offset)
{
    volatile struct spad *pSPAD = (struct spad *)BASE_SPAD;
    *rd_offset = pSPAD->SPAD_FCT_RPTR.u.value;
}

void bcm_drv_spad_set_rd_ptr(uint32_t rd_offset)
{
    volatile struct spad *pSPAD = (struct spad *)BASE_SPAD;
    pSPAD->SPAD_FCT_RPTR.u.value = rd_offset;
}

void bcm_drv_spad_get_wr_ptr(uint32_t* wr_offset)
{
    volatile struct spad *pSPAD = (struct spad *)BASE_SPAD;
    *wr_offset = pSPAD->SPAD_FCT_WPTR.u.value;
}

void bcm_drv_spad_set_wr_ptr(uint32_t wr_offset)
{
    volatile struct spad *pSPAD = (struct spad *)BASE_SPAD;
    pSPAD->SPAD_FCT_WPTR.u.value = wr_offset; 
}

void bcm_drv_spad_set_access_mode(SPAD_ACCESS_MODE access_mode)
{
    volatile struct spad *pSPAD = (volatile struct spad *) BASE_SPAD;
    struct spad_SPAD_CFG temp;
    
    //set new mode
    temp.u.value = 0;
    switch(access_mode)
    {
        case SPAD_CPU_ACCESS:        temp.u.b.AccMode = 0x0; break;      
        case SPAD_RANDOM_ACCESS:     temp.u.b.AccMode = 0x1; break;      
        case SPAD_SEQUENTIAL_ACCESS: temp.u.b.AccMode = 0x2; break;      
        case SPAD_WR_REVERSE_ACCESS: temp.u.b.AccMode = 0x3; break;       
        case SPAD_RD_REVERSE_ACCESS: temp.u.b.AccMode = 0x4; break;
    }
    // write out the new value
    pSPAD->SPAD_CFG.u.value = temp.u.value;
}

void bcm_drv_spad_zeroize(void)
{
    volatile struct spad * pSPAD = (volatile struct spad *) BASE_SPAD;
    struct spad_SPAD_CMD spad;
    int32_t i = 0;

    bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS); //required for zeroization
    spad.u.b.ZeroStart = 0x1;  //the bit clears itself when finished
    pSPAD->SPAD_CMD.u.value = spad.u.value;

    // wait, but not forever, for the scratchpad to assert ZeroDone.  If this becomes a problem,
    // we may need to add some special handling.
    for (i = 0; i < 1000; i++) {
        if (pSPAD->SPAD_ENG_STATUS.u.b.ZeroDone)
            break;
    }
}

void bcm_drv_spad_reset(void)
{
    volatile struct spad * pSPAD = (volatile struct spad *) BASE_SPAD;

    pSPAD->SPAD_CMD.u.b.Reset = 1;
    pSPAD->SPAD_CMD.u.b.Reset = 0;
    bcm_drv_spad_zeroize();
}

void bcm_drv_spad_clean(void)
{
    uint32_t i = 0;
    volatile struct spad * pSPAD = (volatile struct spad *) BASE_SPAD;

    bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);

    pSPAD->SPAD_ACC_ADDR.u.value = 0; //SPAD auto-increments
    for(i = 0; i< BCM_V3_SPAD_SIZE_IN_WORD; i++) {
        //pSPAD->SPAD_ACC_ADDR.u.value = start_adr+i;
        pSPAD->SPAD_ACC_DATA.u.value = 0;
    }
}

void bcm_drv_spad_write(uint32_t* data, uint32_t size_in_word, uint32_t offset)
{
    uint32_t i = 0;
    volatile struct spad * pSPAD = (volatile struct spad *) BASE_SPAD;
    volatile uint32_t count = size_in_word;

    bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);

    pSPAD->SPAD_ACC_ADDR.u.value = offset; //SPAD auto-increments
    for(i = 0; i < size_in_word; i++) {
        //pSPAD->SPAD_ACC_ADDR.u.value = start_adr+i;         
        pSPAD->SPAD_ACC_DATA.u.value = *data++;
        count--;
    } 
    security_assert(count == 0);
}

void bcm_drv_spad_read(uint32_t* data, uint32_t size_in_word, uint32_t offset, uint32_t *ref_size)
{
    uint32_t i = 0;
    volatile struct spad * pSPAD = (volatile struct spad *) BASE_SPAD;
    volatile uint32_t count = size_in_word;

    *ref_size = 0;

    bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);
  
    pSPAD->SPAD_ACC_ADDR.u.value = offset; //SPAD auto-increments
    for(i = 0; i< size_in_word; i++) {
        *data++ = pSPAD->SPAD_ACC_DATA.u.value;
        count--;
    } 
    security_assert(count == 0);

    *ref_size = size_in_word;
}

STATUS bcm_drv_spad_dma_in_sp(uint32_t soc_src_addr, uint32_t size, uint32_t offset)
{
    return STATUS_NOT_IMPLEMENTED;
}

STATUS bcm_drv_spad_dma_out_sp(uint32_t soc_src_addr, uint32_t size, uint32_t offset)
{
    return STATUS_NOT_IMPLEMENTED;
}

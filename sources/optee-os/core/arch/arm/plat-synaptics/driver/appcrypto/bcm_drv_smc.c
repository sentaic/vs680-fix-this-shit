/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_smc.c - BCM SMC Engine related structure definition
//
//*****************************************************************************
#include "bcm_drv_smc.h"
#include "bcm_drv_spad.h"
#include "bcm_drv_xbar.h"
#include "bcm_drv_dma.h"
#include "bcm_drv_smc.h"

void bcm_drv_smc_select_engine(uint32_t engine_id)
{
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC; 
    struct smc_ENG_CFG cfg;
  
    cfg.u.value = smc->ENG_CFG.u.value; //retain previous value
    cfg.u.b.ExtCtrlEn = 0;

    switch(engine_id)
    {
      case SMC_AES_ENGINE:    cfg.u.b.Select=0x1;break; 
      case SMC_DES_ENGINE:    cfg.u.b.Select=0x2;break;      
      case SMC_RC4_ENGINE:    cfg.u.b.Select=0x3;break;     
      case DISABLE_ENGINE:
      default:                cfg.u.b.Select=0x0;break;
    }
    smc->ENG_CFG.u.value = cfg.u.value;
}


void bcm_drv_aes_reset(void)
{
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC; 
    struct smc_AES_CMD cmd;
    
    cmd.u.b.Reset = 1;
    smc->AES_CMD.u.value = cmd.u.value;

    cmd.u.b.Reset = 0;
    smc->AES_CMD.u.value = cmd.u.value;
}


void bcm_drv_aes_init(uint32_t aes_mode, uint32_t direction, uint32_t key_size_bit)
{
    volatile uint32_t key_len_mode    = 0;
    struct smc_AES_CFG  aes_cfg;
    struct smc_AES_CTRL aes_ctrl;
    struct smc_ENG_CFG  eng_cfg;
    volatile struct smc * smc    = (volatile struct smc *) BASE_SMC; 
  
    key_len_mode=(key_size_bit==128)?0x0:
                 (key_size_bit==192)?0x2:
                 (key_size_bit==256)?0x1:0x3;

    aes_cfg.u.value  = smc->AES_CFG.u.value;
    aes_ctrl.u.value = smc->AES_CTRL.u.value;
    eng_cfg.u.value  = smc->ENG_CFG.u.value;

    aes_cfg.u.b.OpMode     = aes_mode;
    aes_cfg.u.b.KeyLen     = key_len_mode;
    aes_cfg.u.b.Decrypt    = direction;

    aes_cfg.u.b.GcmIvSrc   = 0;
    aes_cfg.u.b.Rkek       = 0;
    aes_ctrl.u.b.OutputDis = 0;
    aes_ctrl.u.b.Resume    = 0;
    eng_cfg.u.b.DataSwap   = 1; //AES is now processing bytes in reverse order

    smc->AES_CFG.u.value   = aes_cfg.u.value;
    smc->AES_CTRL.u.value  = aes_ctrl.u.value;
    smc->ENG_CFG.u.value   = eng_cfg.u.value;
}

void bcm_drv_aes_set_key(uint32_t* key, uint32_t key_size_bit, uint32_t key_set)
{
    uint32_t i = 0;
    uint32_t key_len_mode  = 0;
    volatile uint32_t * pK = NULL;
    struct smc_AES_CFG cfg;
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC; 
    uint32_t counter = key_size_bit / 32;

    assert(counter != 0);
    key_len_mode=(key_size_bit==128)?0x0:
                 (key_size_bit==192)?0x2:
                 (key_size_bit==256)?0x1:0x3;

    //read-modify-write
    cfg.u.value    = smc->AES_CFG.u.value;
    cfg.u.b.KeyLen = key_len_mode;
    smc->AES_CFG.u.value = cfg.u.value;

    if (key_set)
        pK = (volatile uint32_t *) &smc->KEY2_DW0;
    else
        pK = (volatile uint32_t *) &smc->KEY1_DW0;

    // load key bits
    for(i=0; i< (key_size_bit/32); i++)
    {
        counter--;
        pK[i] = key[i];
    }

    security_assert(counter == 0);
}


void bcm_drv_aes_set_iv(uint32_t* iv, uint32_t iv_size)
{
    uint32_t i = 0;
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC; 
    volatile uint32_t * pIV = (volatile uint32_t *) &smc->IV_DW0; 
    uint32_t counter = 4;

    for(i=0; i<4; i++)
    {
        pIV[i] = iv[i];
        counter--;
    }
    smc->AES_IV_SIZE.u.value = iv_size;
    security_assert(counter == 0);
}


void bcm_drv_aes_start(uint32_t size_byte, uint32_t is_resume, uint32_t *ref_size)
{
    volatile struct smc * smc    = (volatile struct smc *) BASE_SMC; 
    struct smc_AES_CMD  aes_cmd;
    struct smc_AES_CTRL aes_ctrl;

    aes_ctrl.u.value    = smc->AES_CTRL.u.value;
    aes_ctrl.u.b.Resume = is_resume ? 1 : 0;
    aes_cmd.u.b.Start   = 1;

    smc->AES_STR_SIZE.u.value = size_byte;

    *ref_size = smc->AES_STR_SIZE.u.value;

    smc->AES_CTRL.u.value = aes_ctrl.u.value;
    smc->AES_CMD.u.value  = aes_cmd.u.value;
}

STATUS bcm_drv_aes_wait_4_done(uint32_t time_out)
{
    STATUS status = STATUS_FAILURE;
    struct smc_AES_INTP intr;
    volatile BCM_APB_ADEC *apb_adec= (BCM_APB_ADEC *) BASE_APB_ADEC;
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC; 
    
    while(time_out--)
    {
        intr.u.value = smc->AES_INTP.u.value;
        //check and clear interrupt
        if (intr.u.value != 0)
        {
            //something happened - error or done
            if (intr.u.b.LenErr) status = STATUS_AES_LENGTH_ERROR; 
            if (intr.u.b.TagErr) status = STATUS_AES_GCM_TAG_ERROR; 
            if (intr.u.b.TxErr)  status = STATUS_AES_DATA_TRANSFER_ERROR; 
            if (intr.u.b.Done)   status = STATUS_SUCCESS;

            //clear
            smc->AES_INTP.u.value = intr.u.value;
            apb_adec->address_decoder_interrupt_register = 0x2;//ENG1

            if (status != STATUS_SUCCESS)
                bcm_drv_aes_reset();
            
            return(status);
        }
    }  
    return (STATUS_AES_TIMEOUT); 
}

STATUS bcm_drv_aes_process(uint8_t* src, uint8_t* dst,
        uint32_t size_byte, uint32_t resume, uint32_t *proc_size)
{
    uint32_t round     = 0;
    uint32_t residual  = 0;
    uint32_t offset    = 0;
    uint32_t cur_size = 0;
    uint32_t i         = 0;
    uint32_t read_size_in_word = 0;
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    *proc_size = 0;

    residual = size_byte%(BCM_V3_SPAD_SIZE_IN_BYTE/2);
    round    = size_byte/(BCM_V3_SPAD_SIZE_IN_BYTE/2);

    round = (residual!=0)?(round+1):round;
    cur_size = (residual!=0)?residual:(BCM_V3_SPAD_SIZE_IN_BYTE/2);

    for(i=0;i<round;i++)
    {
        bcm_drv_spad_set_wr_ptr(0);
        bcm_drv_spad_set_rd_ptr(0);

        bcm_drv_spad_write((uint32_t*)(src + offset), cur_size/sizeof(uint32_t), 0);

        bcm_drv_xbar_set_path(SP_AES_PATH);

        bcm_drv_spad_set_wr_ptr(cur_size/sizeof(uint32_t));
        bcm_drv_spad_set_rd_ptr(0);

        bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);

        bcm_drv_aes_start(cur_size, resume, &ref_size);
        security_assert(ref_size == cur_size);
        ref_size = 0;

        read_size_in_word = 0;
        status = bcm_drv_aes_wait_4_done(10000000);

        if(status != STATUS_SUCCESS) return status;

        bcm_drv_spad_read((uint32_t*)(dst + offset),
                cur_size/sizeof(uint32_t), cur_size/sizeof(uint32_t), &read_size_in_word);
        security_assert(cur_size/sizeof(uint32_t) == read_size_in_word);

        offset   += cur_size;
        cur_size = BCM_V3_SPAD_SIZE_IN_BYTE/2;
        resume    = 1;

        *proc_size += read_size_in_word * sizeof(uint32_t);
    }

    return status;
}

STATUS bcm_drv_aes_process_ext(uint32_t soc_src, uint32_t soc_dst,
        uint32_t size_byte, uint32_t resume, uint32_t *proc_size)
{
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    *proc_size = 0;

    bcm_drv_xbar_set_path(AES_PATH);
    bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);

    // set source address to read
    bcm_drv_dma_setup_link_list(0x0, (uint32_t)soc_src, size_byte/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == size_byte/sizeof(uint32_t));
    ref_size = 0;

    isb();

    bcm_drv_dma_setup_link_list(0x1, (uint32_t)soc_dst, size_byte/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == size_byte/sizeof(uint32_t));
    ref_size = 0;
  
    //kick off dma
    bcm_drv_dma_enable(0x0);
    bcm_drv_dma_enable(0x1);

    bcm_drv_aes_start(size_byte, resume, &ref_size);
    security_assert(ref_size == size_byte);

    status = bcm_drv_dma_wait_4_done(0, DMA_TIMEOUT);
    if(status) return status;

    status = bcm_drv_dma_wait_4_done(1, DMA_TIMEOUT);
    if(status) return status;

    status = bcm_drv_aes_wait_4_done(AES_TIMEOUT);
    if(status) return status;

    *proc_size = ref_size;

    return status;
}


STATUS bcm_drv_aes(uint8_t* src, uint8_t* dst, uint32_t size, uint8_t* key, uint32_t key_size_bit,
        uint8_t* iv, uint32_t iv_size_byte, uint32_t mode, uint32_t direction, uint32_t *proc_size)
{
    STATUS status = STATUS_FAILURE;
    uint8_t clean_key[32] = {0};

    bcm_drv_aes_reset();

    bcm_drv_aes_set_key((uint32_t*)key, key_size_bit, 0);

    bcm_drv_smc_select_engine(SMC_AES_ENGINE);

    bcm_drv_aes_init(mode, direction, key_size_bit);

    if(iv != NULL)
        bcm_drv_aes_set_iv((uint32_t*)iv, iv_size_byte);

    status = bcm_drv_aes_process(src, dst, size, 0, proc_size);

    bcm_drv_aes_set_key((uint32_t*)clean_key, key_size_bit, 0);

    return status;

}

STATUS bcm_drv_aes_ext(uint32_t soc_src, uint32_t soc_dst, uint32_t size_byte, uint8_t* key,
        uint32_t key_size_bit, uint8_t* iv, uint32_t iv_size_byte, uint32_t mode,
        uint32_t direction, uint32_t *proc_size)
{
    STATUS status = STATUS_FAILURE;
    uint8_t clean_key[32] = {0};

    *proc_size = 0;

    bcm_drv_aes_reset();

    bcm_drv_aes_set_key((uint32_t*)key, key_size_bit, 0);

    bcm_drv_smc_select_engine(SMC_AES_ENGINE);

    bcm_drv_aes_init(mode, direction, key_size_bit);

    if(iv != NULL)
        bcm_drv_aes_set_iv((uint32_t*)iv, iv_size_byte);

    status = bcm_drv_aes_process_ext(soc_src, soc_dst, size_byte, 0, proc_size);

    bcm_drv_aes_set_key((uint32_t*)clean_key, key_size_bit, 0);

    return status;
}

void bcm_drv_aes_set_rkek(uint32_t enable)
{
    struct smc_AES_CFG cfg;
    volatile struct smc * smc = (volatile struct smc *) BASE_SMC;

    cfg.u.value    = smc->AES_CFG.u.value;
    cfg.u.b.Rkek = enable ? 1 : 0;;
    smc->AES_CFG.u.value = cfg.u.value;
}


STATUS bcm_drv_aes_rkek_sel(uint8_t* src, uint8_t* dst, uint32_t size, uint32_t key_size_bit,
        uint8_t* iv, uint32_t iv_size_byte, uint32_t mode, uint32_t direction, uint32_t *proc_size)
{
    STATUS status = STATUS_FAILURE;

    bcm_drv_aes_reset();

    bcm_drv_smc_select_engine(SMC_AES_ENGINE);

    bcm_drv_aes_init(mode, direction, key_size_bit);

    bcm_drv_aes_set_rkek(1);

    if(iv != NULL)
        bcm_drv_aes_set_iv((uint32_t*)iv, iv_size_byte);

    status = bcm_drv_aes_process(src, dst, size, 0, proc_size);

    bcm_drv_aes_set_rkek(0);

    return status;

}


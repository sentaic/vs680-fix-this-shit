/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_smc.h - BCM SMC Engine related structure definition
//
//*****************************************************************************
#ifndef __BCM_DRV_SMC__
#define __BCM_DRV_SMC__
#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"
#include "bcm_external.h"

typedef enum _SMC_CRYPT_ENGINE
{
    SMC_AES_ENGINE  = 0,
    SMC_DES_ENGINE  = 1, 
    SMC_RC4_ENGINE  = 2,
    DISABLE_ENGINE  = 3
}SMC_CRYPT_ENGINE;

typedef enum _SMC_AES_MODE
{
    SMC_AES_ECB       = 0,
    SMC_AES_CBC       = 1,
    SMC_AES_CTR       = 2,
    SMC_AES_XTS       = 3,
    SMC_AES_KEY_WRAP  = 4,
    SMC_AES_NIST_CS2  = 5
}SMC_AES_MODE;

typedef enum _SMC_AES_DIRECTION
{
    SMC_AES_ENCRYPTION  = 0,
    SMC_AES_DECRYPTION  = 1,
}SMC_AES_DIRECTION;

typedef enum _SMC_AES_KEY_LEN
{
    SMC_AES_KEY_LEN_128  = 0,
    SMC_AES_KEY_LEN_256  = 1,
    SMC_AES_KEY_LEN_192  = 2,
}SMC_AES_KEY_LEN;

typedef enum _SMC_IV_MODE
{
    SMC_IV_NONE  = 0,
    SMC_IV_SWAP  = 1,
    SMC_IV_XOR   = 2
}SMC_IV_MODE;

typedef enum _SMC_IV_MASK
{
    IV_MASK_MSB16  = 0,
    IV_MASK_MSB32  = 1,
    IV_MASK_MSB48  = 2,
    IV_MASK_MSB64  = 3,
    IV_MASK_LSB16  = 4,
    IV_MASK_LSB32  = 5,
    IV_MASK_LSB48  = 6,
    IV_MASK_LSB64  = 7
}SMC_IV_MASK;


typedef struct _AESIV
{
   union
   {
      uint128_t iv128;
      uint32_t iv32[4];
   } parts;
} AESIV;
void bcm_drv_smc_select_engine(uint32_t engine_id);
void bcm_drv_aes_reset(void);
void bcm_drv_aes_init(uint32_t aes_mode, uint32_t direction, uint32_t key_size_bit);
void bcm_drv_aes_set_key(uint32_t* key, uint32_t key_size_bit, uint32_t key_set);
void bcm_drv_aes_set_iv(uint32_t* iv, uint32_t iv_size);
void bcm_drv_aes_start(uint32_t size_byte, uint32_t is_resume, uint32_t *ref_size);
STATUS bcm_drv_aes_wait_4_done(uint32_t time_out);
STATUS bcm_drv_aes_process(uint8_t* src, uint8_t* dst,
        uint32_t size_byte, uint32_t resume, uint32_t *proc_size);

STATUS bcm_drv_aes_process_ext(uint32_t soc_src, uint32_t soc_dst,
        uint32_t size_byte, uint32_t resume, uint32_t *proc_size);
STATUS bcm_drv_aes(uint8_t* src, uint8_t* dst, uint32_t size, uint8_t* key, uint32_t key_size_bit,
        uint8_t* iv, uint32_t iv_size_byte, uint32_t mode, uint32_t direction, uint32_t *proc_size);

STATUS bcm_drv_aes_ext(uint32_t soc_src, uint32_t soc_dst, uint32_t size_byte, uint8_t* key,
        uint32_t key_size_bit, uint8_t* iv, uint32_t iv_size_byte, uint32_t mode,
        uint32_t direction, uint32_t *proc_size);


#endif//#ifndef __BCM_DRV_SMC__

/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_hash.h - BCM HASH Engine related structure definition
//
//*****************************************************************************
#ifndef __BCM_DRV_HASH__
#define __BCM_DRV_HASH__
#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"
#include "bcm_external.h"

typedef enum _HASH_ALG
{
   HASH_MODE_SHA1   = 0,
   HASH_MODE_SHA224 = 1,
   HASH_MODE_SHA256 = 2,
   HASH_MODE_SHA384 = 3,
   HASH_MODE_SHA512 = 4,
   HASH_MODE_MD5    = 5
} HASH_ALG;

void bcm_drv_hash_reset(void);
STATUS bcm_drv_hash_init(uint32_t  hash_mode);
STATUS bcm_drv_hash_update(uint8_t* msg, uint32_t size_byte);
STATUS bcm_drv_hash_update_ext(uint32_t soc_msg_addr, uint32_t size_byte);
STATUS bcm_drv_hash_final(uint8_t* msg, uint32_t size_byte, uint8_t* digest, uint32_t*digest_size);
STATUS bcm_drv_hash_final_ext(uint32_t soc_msg_addr, uint32_t size_byte, uint8_t* digest, uint32_t*digest_size);
STATUS bcm_drv_hmac_init(uint32_t hash_mode, uint8_t* key, uint32_t size_byte);
STATUS bcm_drv_hmac_update(uint8_t* msg, uint32_t size_byte);
STATUS bcm_drv_hmac_update_ext(uint32_t soc_msg_addr, uint32_t size_byte);
STATUS bcm_drv_hmac_finial(uint8_t* msg, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size);
STATUS bcm_drv_hmac_finial_ext(uint32_t soc_msg_addr, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size);
STATUS  bcm_drv_hash_wait_4_done(uint32_t time_out);
void bcm_drv_save_hash_context(uint8_t*context);
void bcm_drv_load_hash_context(uint8_t* context);

STATUS bcm_drv_hash_ext(uint32_t hash_mode,uint32_t soc_msg_addr,
        uint32_t msg_size_byte, uint32_t* digest, uint32_t* digest_size_byte);
STATUS bcm_drv_hmac(uint32_t hash_mode,
                       uint8_t* msg,
                       uint32_t msg_size_byte,
                       uint8_t* key,
                       uint32_t key_size,
                       uint32_t* digest,
                       uint32_t* digest_size_byte);
STATUS bcm_drv_hmac_ext(uint32_t hash_mode,
                       uint32_t soc_msg_addr,
                       uint32_t msg_size_byte,
                       uint8_t* key,
                       uint32_t key_size,
                       uint32_t* digest,
                       uint32_t* digest_size_byte);

void bcm_drv_hmac_get_digest(uint8_t* digest, uint32_t* digest_size_byte);
STATUS bcm_drv_hash(uint32_t hash_mode,uint8_t* msg, uint32_t msg_size_byte, uint32_t* digest, uint32_t* digest_size_byte);
STATUS bcm_drv_hmac_final(uint8_t* msg, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size);

STATUS bcm_drv_hmac_final_ext(uint32_t soc_msg_addr, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size);

#endif //#ifndef __BCM_DRV_HASH__

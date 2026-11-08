/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_com.h - BCM Driver Related Data Structure Definition
//
//*****************************************************************************
#ifndef __BCM_DRV_COM_H__
#define __BCM_DRV_COM_H__

#include "bcm_reg_map.h"
//#include "nexus_memmap.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include <tee_api_types.h>
#include <mm/core_memprot.h>
#include "plat_def.h"

#define BCM_V3_SPAD_SIZE_IN_WORD    1536
#define BCM_V3_SPAD_SIZE_IN_BYTE    6144   //6KB

#define START_BCM_CRYPTO                    START_MCU_APP_CRYPTO_S

#define BASE_BCM           (vaddr_t)phys_to_virt_io(MCU_DMA_CRYPT_REGION(START_BCM_CRYPTO), SIZE_MCU_APP_CRYPTO_S)
#define BASE_BIU           (BASE_BCM + BIU_BASE_ADDR)
#define BASE_APB_ADEC      (BASE_BCM + APB_ADEC_BASE_ADDR)
#define BASE_DMA           (BASE_BCM + DMA_BASE_ADDR)
#define BASE_MAHB          (BASE_BCM + MAHB_BASE_ADDR)
#define BASE_ACCEL_BUS_REG (BASE_BCM + ACCEL_BUS_REG_BASE_ADDR)
#define BASE_ABUS          (BASE_BCM + ACCEL_BUS_REG_BASE_ADDR)
#define BASE_AES           (BASE_BCM + AES_BASE_ADDR)
#define BASE_AES2          (BASE_BCM + AES2_BASE_ADDR)
#define BASE_ECP           (BASE_BCM + ECP_BASE_ADDR)
#define BASE_ECRSA_ECP     (BASE_BCM + ECRSA_ECP_BASE_ADDR)
#define BASE_HASH          (BASE_BCM + HASH_BASE_ADDR)
#define BASE_SCRATCH_PAD   (BASE_BCM + SCRATCH_PAD_BASE_ADDR)
#define BASE_ZMODP         (BASE_BCM + ZMODP_BASE_ADDR)
#define BASE_ECRSA_ZMP     (BASE_BCM + ECRSA_ZMP_BASE_ADDR)
#define BASE_MCT           (BASE_BCM + MCT_BASE_ADDR)
#define BASE_SPAD          (BASE_BCM + SCRATCH_PAD_BASE_ADDR)
#define BASE_SMC           (BASE_BCM + AES_BASE_ADDR)
#define BASE_AMC           (BASE_BCM + AMC_BASE_ADDR)

#define DMA_TIMEOUT        1000000000
#define AES_TIMEOUT        1000000000
#define RC4_TIMEOUT        1000000000
#define DES_TIMEOUT        1000000000
#define HASH_TIMEOUT       1000000000
#define GHASH_TIMEOUT      1000000000
#define ZMODP_TIMEOUT      1000000000
#define ECP_TIMEOUT        1000000000
#define OTP_TIMEOUT        1000000000
#define EBG_TIMEOUT        1000000000
//#define BIT(x) (1 << x)

typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

typedef TEE_Result STATUS;
#define STATUS_NOT_IMPLEMENTED TEE_ERROR_NOT_IMPLEMENTED
#define STATUS_SUCCESS TEE_SUCCESS
#define STATUS_FAILURE TEE_ERROR_GENERIC
#define STATUS_HASH_TIMEOUT TEE_ERROR_TIMEOUT


#define STATUS_DMA_BUS_ERROR TEE_ERROR_ACCESS_CONFLICT
#define STATUS_DMA_LINKED_LIST_ACCESS_ERROR TEE_ERROR_ACCESS_DENIED
#define STATUS_DMA_IDIOPATHIC_ERROR TEE_ERROR_SECURITY
#define STATUS_DMA_PARITY_ERROR TEE_ERROR_BAD_STATE
#define STATUS_DMA_PAUSE_COMPLETION_TIMEOUT TEE_ERROR_TIMEOUT
#define STATUS_DMA_TIMEOUT TEE_ERROR_TIMEOUT

#define STATUS_AES_LENGTH_ERROR TEE_ERROR_BAD_PARAMETERS
#define STATUS_AES_GCM_TAG_ERROR  TEE_ERROR_BAD_FORMAT
#define STATUS_AES_DATA_TRANSFER_ERROR TEE_ERROR_COMMUNICATION
#define STATUS_AES_TIMEOUT TEE_ERROR_TIMEOUT
#endif

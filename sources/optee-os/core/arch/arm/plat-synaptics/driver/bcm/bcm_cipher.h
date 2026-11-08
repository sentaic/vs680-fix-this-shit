// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef INCLUDE_BCM_CIPHER_H
#define INCLUDE_BCM_CIPHER_H

#define BCM_CIPHER_BLOCK		0xC000


enum {
	/* Block Cipher */
	BCM_CIPHER_AES = 1,
	BCM_CIPHER_AES192,
	BCM_CIPHER_AES256,
	BCM_CIPHER_TDES,
	BCM_CIPHER_DES,
	/* Stream Cipher */
	BCM_CIPHER_DVB_CSA,
	BCM_CIPHER_DVB_CSA2,
	BCM_CIPHER_DVB_CSA3,
	BCM_CIPHER_ASA64,
	BCM_CIPHER_ASA128,
	BCM_CIPHER_ALG_LAST,
};

// types of crypto mode of operations
enum {
	BCM_CIPHER_ECB,
	BCM_CIPHER_CBC,
	BCM_CIPHER_CTR,
	BCM_CIPHER_XTS,
	BCM_CIPHER_MODE_LAST,
};

//crypto operation type
enum {
	BCM_CIPHER_ENCRYPT,
	BCM_CIPHER_DECRYPT,
};

//Key types
enum {
	EClearKey = 0,
	ESecureKey,
	ERootKey,
	ECSFlashProtectionRootKey,
	EFlashProtectionKey,
	EZeroLevelProtectionKey,
	EOneLevelProtectionKey,
	ETwoLevelProtectionKey,
	ECertKey,
	ELegacyKey,
	EClearIV,
	ESecureIV
};

struct cipherConfigPara {
	uint32_t algo;		//Crypto Algorithm type (AES or TDES)
	uint32_t mode;		//mode: such as ECB, CBC, CTR
	uint32_t op;		//operation: encrypt/decrypt
	uint32_t keyType;	//type of key
	uint32_t IV;		//IV
	uint32_t key;		//key
	uint32_t inBuf;		//input buffer
	uint32_t outBuf;	//slot for output
	uint32_t inLen;		//length of the data
};

#endif /* INCLUDE_BCM_CIPHER_H */

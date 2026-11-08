// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _IMAGE_GENX_H_
#define _IMAGE_GENX_H_

#include "types_ext.h"


enum {
	// RSA Keys
	IMAGE_TYPE_K0_BOOT   = 0x01,
	IMAGE_TYPE_K0_TEE    = 0x02,
	IMAGE_TYPE_K0_REE    = 0x03,
	IMAGE_TYPE_K1_BOOT_A = 0x04,
	IMAGE_TYPE_K1_BOOT_B = 0x05,
	IMAGE_TYPE_K1_TEE_A  = 0x06,
	IMAGE_TYPE_K1_TEE_B  = 0x07,
	IMAGE_TYPE_K1_TEE_C  = 0x08,
	IMAGE_TYPE_K1_TEE_D  = 0x09,
	IMAGE_TYPE_K1_REE_A  = 0x0A,
	IMAGE_TYPE_K1_REE_B  = 0x0B,
	IMAGE_TYPE_K1_REE_C  = 0x0C,
	IMAGE_TYPE_K1_REE_D  = 0x0D,

	// SCS boot images
	IMAGE_TYPE_EROM            = 0x10,
	IMAGE_TYPE_SCS_DATA_PARAM  = 0x11,
	IMAGE_TYPE_BOOT_MONITOR    = 0x12,
	IMAGE_TYPE_SYS_INIT        = 0x13,
	IMAGE_TYPE_MINI_LOADER     = 0x14,

	// Post-SCS secure images
	IMAGE_TYPE_BCM_KERNEL          = 0x15,
	IMAGE_TYPE_TZ_KERNEL           = 0x16,
	IMAGE_TYPE_ATF                 = 0x17,
	IMAGE_TYPE_TZK_BOOT_PARAMETER  = 0x18,
	IMAGE_TYPE_TA_ROOT_CERT        = 0x19,
	IMAGE_TYPE_TA_CERT             = 0x1A,
	IMAGE_TYPE_TA                  = 0x1B,
	IMAGE_TYPE_TA_CONTAINER_HEADER = 0x1C,

	// ACPU OEM images
	IMAGE_TYPE_OEM_START         = 0x20,
	IMAGE_TYPE_BOOTLOADER        = 0x20,
	IMAGE_TYPE_LINUX_KERNEL      = 0x21,
	IMAGE_TYPE_TZK_OEM_SETTINGS  = 0x22,
	IMAGE_TYPE_AVB_KEYS          = 0x23,
	IMAGE_TYPE_LINUX             = 0x24,
	IMAGE_TYPE_UBOOT             = 0x25,
	IMAGE_TYPE_FASTBOOT          = 0x26,
	IMAGE_TYPE_FAST_LOGO         = 0x27,
	IMAGE_TYPE_OEM_END,

	// Other CPU FW images
	IMAGE_TYPE_FW_START = 0x30,
	IMAGE_TYPE_TSP_FW = 0x30,
	IMAGE_TYPE_DSP_FW = 0x31,
	IMAGE_TYPE_GPU_FW = 0x32,
	IMAGE_TYPE_NPU_FW = 0x33,
	IMAGE_TYPE_SM_FW  = 0x34,
	IMAGE_TYPE_FW_END,

	//DIF image
	IMAGE_TYPE_DIF = 0x40,

	//ROM Keys
	IMAGE_TYPE_ROM_ROOT_OBF_KEY = 0x50,

	//DDR Phy FW images
	IMAGE_TYPE_DDR_FW0 = 0x60,
	IMAGE_TYPE_DDR_FW1 = 0x61,

	// Runtime dynamically loaded image
	IMAGE_TYPE_DOLBY_AUDIO = 0x70

};

#define TA_TYPE_NORMAL          0
#define TA_TYPE_DOLBY_VISION    1
#define TA_TYPE_DEBUG           0xFFFFFFFF

struct ta_header {
	uint32_t image_type;			// IMAGE_TYPE_TA, Identification of the image
	uint32_t image_format_vesion;	// version of image format
	uint32_t length;				// 380 - TA image length, All-inclusive length
	uint8_t iv[16]; 				// IV for AEC-CBC
	uint8_t checksum[32];
	uint32_t ta_segid;				// TA_Vendor[n]_SegID
	uint32_t ta_segid_mask;			// TA SegID mask chosen by TA owner
	uint32_t version;				// TA_Vendor[n]_version
	uint32_t version_mask;			// TA Version mask chosen by TA owner
	uint32_t production_image_flag; // Whether this is a development image.
									// 0: development image, 1: production image
	uint32_t ta_vendor_gid; 		// vendor GID to which the root certificate is issued
	uint32_t ta_cert_id;			// Vendor defined 4-byte value. ID of this TA certificate
									// managed by TA vendor

	uint8_t name[16];				// TA Name
	uint8_t uuid[16];				// TA UUID
	uint32_t ta_type;				// Synaptics defined value.
									// 0: Generic TA, no checking OTP
									// 1: Dolby Vision, TZK checking Dolby_Vision_Disabled OTP bit
									// 2: Dolby Audio (reserved), TZK checking Dolby_Audio_Disabled OTP bit
									// 3: reserved (can be expanded to cover future special cases)

	uint8_t signature[256]; 		// Signature of fields with "S" in colum A
	uint8_t data[0];				// store TA permission information,
									// encrypted with TA vendor's TA_K0_AES key
};

struct ta_cert_header {
	uint32_t image_type;			// IMAGE_TYPE_TA_CERT, Identification of the image
	uint32_t image_format_vesion;	// version of image format
	uint32_t length;				// 852, All-inclusive length
	uint8_t iv[16]; 				// IV for AEC-CBC
	uint8_t checksum[32];
	uint32_t ta_segid;				// TA_Vendor[n]_SegID
	uint32_t ta_segid_mask;			// TA SegID mask chosen by TA owner
	uint32_t version;				// TA_Vendor[n]_version
	uint32_t version_mask;			// TA Version mask chosen by TA owner
	uint32_t production_image_flag; // Whether this is a development image.
									// 0: development image, 1: production image

	uint32_t ta_vendor_gid; 		// vendor GID to which the root certificate is issued
	uint32_t ta_cert_id;			// Vendor defined 4-byte value. ID of this TA certificate
									// managed by TA vendor

	uint8_t signature[256]; 		// Signature of fields with "S" in colum A
	uint8_t ta_k1_rsa[256]; 		// encrypted with TA vendor's TA_K0_AES key
	uint8_t ta_k1_aes[16];			// encrypted with TA vendor's TA_K0_AES key
	uint8_t ta_permission[0];		// store TA permission information,
									// encrypted with TA vendor's TA_K0_AES key
};

struct ta_root_cert_header {
	uint32_t image_type;			// IMAGE_TYPE_TA_ROOT_CERT, Identification of the image
	uint32_t image_format_vesion;	// version of image format
	uint32_t length;				// 612 + Lenght of TA permission structure
	uint8_t iv[16];					// IV for AEC-CBC
	uint8_t checksum[32];
	uint32_t ta_segid;				// control Data
	uint32_t ta_segid_mask;
	uint32_t version;				// TEE Version
	uint32_t version_mask;
	uint32_t production_image_flag; // Whether this is a development image
	uint32_t ta_vendor_gid;			// vendor GID to which the root certificate is issued
	uint8_t signature[256];			// Signature of fields with "S" in colum A
	uint8_t ta_k0_rsa[256];			// encrypted with image specific AES key derived from MDK
	uint8_t ta_k0_aes[16];			// encrypted with image specific AES key derived from MDK
	uint8_t ta_permission[0]; 		// store TA permission information
};

struct boot_image_header {
    uint32_t img_id;            /* Identification of the image. IMAGE_ID_BL, IMAGE_ID_SM, IMAGE_ID_UBOOT */
    uint32_t img_format_ver;    /* Version of the image format */
    uint32_t length;            /* All-inclusive length (628 + image body size) */
    uint8_t iv[16];             /* IV for AES-CBC If IV = 0, image is not encrypted */
    uint8_t checksum[64];       /* SHA512 hash of cleartext of image body */
    uint32_t segid;             /* Control Data (SYNA_SegID) */
    uint32_t version;           /* Control Data (MCU_IMAGE_Version) */
    uint32_t prod_img_flag;     /* Indicator of development or production software (0: Development SW 1: Production SW) */
    uint8_t signature[512];     /* Signature of fields with “S”; Signature size: RSA4096 - 512 bytes; ECDSA P384 - 96 bytes; ECDSA P521 - 132 bytes; */
};

int verify_image(paddr_t src, unsigned int size, paddr_t dst, unsigned int *dst_size, unsigned int image_type, unsigned int multi_keys);

#endif /* _IMAGE_H_ */

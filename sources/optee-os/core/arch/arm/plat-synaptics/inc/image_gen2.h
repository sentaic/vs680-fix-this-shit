// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _IMAGE_GEN2_H_
#define _IMAGE_GEN2_H_

#define MAKE_FOURCC(ch0, ch1, ch2, ch3) \
	((unsigned int)(char)(ch0) | ((unsigned int)(char)(ch1) << 8) | \
	((unsigned int)(char)(ch2) << 16) | ((unsigned int)(char)(ch3) << 24))

/*
 * Generic Image Format:
 *
 * +----------------------------+
 * | image_header		|
 * +----------------------------+
 * | chunk 0			|
 * +----------------------------+
 * | ...			|
 * +----------------------------+
 * | chunk n			|
 * +----------------------------+
 */

#define IMAGE_HEADER_MAGIC_NUM		MAKE_FOURCC('I', 'M', '*', 'H')
#define IMAGE_HEADER_VERSION		(0)

struct image_header {
	uint32_t header_magic_num;	/* 'IM*H' */
	uint32_t header_size;
	uint32_t header_version;
	uint32_t header_reserved;
	char     image_name[32];
	uint32_t image_version;
	uint32_t reserved[2];
	uint32_t chunk_num;
	struct {
		uint32_t id;
		uint32_t offset;	/* start from header, 16bytes aligned */
		uint32_t size;
		uint32_t attr0;		/* bit24:31 - compress method; */
		/* data can be in (dest_start, dest_start + dest_size)
		 * if dest_size == 0, then chunk data must always place at
		 * dest_start.
		 */
		uint64_t dest_start;
		uint32_t dest_size;
		uint32_t attr1;		/* for compressed data, it's original_size */
	} chunk[0];
};

/*
 * predefined chunk ID.
 */

#define IMAGE_CHUNK_ID_BOOTLOADER		MAKE_FOURCC('B', 'T', 'L', 'R')
#define IMAGE_CHUNK_ID_SYSTEM_MANAGER		MAKE_FOURCC('S', 'M', '*', '*')
#define IMAGE_CHUNK_ID_UBOOT			MAKE_FOURCC('U', 'B', 'T', '*')

#define IMAGE_CHUNK_ID_LINUX_BOOTIMG		MAKE_FOURCC('L', 'N', 'X', 'B')
#define IMAGE_CHUNK_ID_LINUX_DTB		MAKE_FOURCC('L', 'D', 'T', 'B')

#define IMAGE_CHUNK_ID_TZ_KERNEL		MAKE_FOURCC('T', 'Z', 'K', '*')
#define IMAGE_CHUNK_ID_TZ_BOOT_PARAM		MAKE_FOURCC('T', 'Z', 'B', 'P')

#define IMAGE_CHUNK_ID_TZ_LOADABLE_TA		MAKE_FOURCC('T', 'Z', 'T', 'A')

#define IMAGE_CHUNK_ATTR_COMPRESS(attr)		((attr & 0xff000000) >> 24)


#define IMAGE_CHUNK_ID_TZ30_LOADABLE_TA		MAKE_FOURCC('0', '3', 'A', 'T')
#define IMAGE_CHUNK_ID_TZ30_CERT_FILE		MAKE_FOURCC('E', 'C', 'A', 'T')
#define IMAGE_EMBEDDED_HEADER_MAGIC_NUM		MAKE_FOURCC('M', 'I', 'A', 'T')

#define IMAGE_CHUNK_ID_TZ50_LOADABLE_TA		MAKE_FOURCC('0', '5', 'A', 'T')
#define IMAGE_CHUNK_ID_TZ50_ROOT_CERT_FILE	MAKE_FOURCC('C', 'R', 'A', 'T')


#define RSA_KEY_SIZE 		1024
#define AES_KEY_SIZE 		16
#define HASH_SIZE    		64
#define CMAC_SIZE    		32
#define SHA256_DIGEST_SIZE 	32
#define CMAC_SIZE 		32
#define SZ_DIGEST_SHA1 		20
#define RSA_2K_KEY_SIZE 	256

struct ta30_image_header {
	uint32_t header_magic_num;	/* 'TA30' */
	uint32_t header_size;
	uint32_t ta_cert_start;
	uint32_t ta_cert_size;
	uint32_t ta_image_start;
	uint32_t ta_image_size;
	uint32_t reserved[2];
};

struct ta50_image_header {
	uint32_t header_magic_num;	/* 'TA50' */
	uint32_t header_size;
	uint32_t ta_root_cert_start;
	uint32_t ta_root_cert_size;
	uint32_t ta_combo_image_start;
	uint32_t ta_combo_image_size;
	uint32_t reserved[2];
};

struct ta_cert_header {
	uint32_t header_magic_num;	/* 'TACE' */
	uint32_t header_size;
	uint8_t vendor_id;
	uint8_t vendor_name[15];
	uint32_t cert_segid;
	uint32_t cert_segid_mask;
	uint32_t cert_version;
	uint32_t cert_version_mask;
	uint8_t ta_permission[0];
};

struct ta_embedded_header {
	uint32_t header_magic_num;
	uint32_t header_size;
	uint8_t vendor_id;
	uint8_t vendor_name[15];
	uint32_t image_segid;
	uint32_t image_segid_mask;
	uint32_t image_version;
	uint32_t image_version_mask;
	uint8_t stuffing[20];
	uint32_t footer_magic_num;
};

struct ta_root_cert_header {
	uint32_t header_magic_num;		// hardcoded to "TARC"
	uint32_t header_size;			// size from header_magic_number to
	uint8_t stuff;
	uint8_t vendor_name[15]; 		// like "synaptics"
	uint8_t vendor_uuid[16];		// only highest 14 bytes are used
	uint32_t cert_version; 		// only lowest 4 bits are effective due to
						// size limitation of OTP field
	uint32_t cert_version_mask;
	uint8_t ta_all_permission[0]; 		// data block with variable size to
						// store TA permission information
};

struct const_image_header {
	uint32_t    header_version;    		// 0x20000000
	uint16_t    magicnumber;      		// magic number showing this is an image
	uint8_t     custkey_type;      		// 0: clear 1: MRVL CustKey 2: OEM custkey
	uint8_t     code_type;         		// 0: BCM FW (EROM/BCM), 1: ARM Sys_Init,
						// 2. Marvell A/V FW 3. ARM TZ
						// 4:ARM Boot loader, Linux Kernel, RootFS,
						// 5: OEM FW, 6: ARM App, 7-15:resered
	uint8_t     security_level;    		// for BCM EROM to interprete
	uint8_t     reserved0;        		// Signing ExtRSA Key: 0:OEM ExtRSA 1:MRVL ExtRSA
	uint16_t    user_data;         		// User Data for google
	uint8_t     wrappeduserkey[AES_KEY_SIZE];// custkey encrypted user key,
	uint32_t    marketid;
	uint32_t    marketidmask;     		// should be all '1' (0xFFFFFFFF) in default
	uint8_t     version;
	uint8_t     versionmask;      		// should be all '1' (0xFF) in default
	uint8_t     headerhashsize;   		// Image Hash Type. 32 for SHA256/64 for SHA512   [fho] Should be uint16_t
	uint16_t    imagehashsize;    		// Image Hash Type. 32 for SHA256/64 for SHA512   [fho] Should be uint32_t
	uint32_t    imagesize;        		// image size in bytes,
	uint8_t     imagehash[HASH_SIZE];     	// hash of the image, use [0:31] for sha256
	uint8_t     signature[RSA_KEY_SIZE];  	// Signature of the header over all header
						// fields, use [0:255] only for rsa2k
};

#endif /* _IMAGE_GEN2_H_ */

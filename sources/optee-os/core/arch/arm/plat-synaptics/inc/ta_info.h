/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TA_INFO_H_
#define _TA_INFO_H_
#include <stdint.h>

#define MAX_VENDOR_NUMS			6

typedef enum {
	TAMgrCmd_FLAG_REGISTER = 0,
	TAMgrCmd_FLAG_GETINFO = 1,
	TAMgrCmd_FLAG_MAX
} TAMgrCmd_FLAG;

typedef struct ta_cert_info {
	uint8_t cert_vendor_id;
	uint8_t cert_vendor_name[15];
	uint32_t cert_segid;
	uint32_t cert_segid_mask;
	uint32_t cert_version;
	uint32_t cert_version_mask;
	uint8_t vendor_uuid[16];
} ta_cert_info_t;

typedef struct ta_img_info {
	uint8_t image_vendor_id;
	uint8_t image_vendor_name[15];
	uint32_t image_segid;
	uint32_t image_segid_mask;
	uint32_t image_version;
	uint32_t image_version_mask;
	uint8_t vendor_uuid[16];
} ta_img_info_t;

typedef struct ta_root_cert_info {
	uint8_t vendor_name[15]; 		// like "synaptics"
	uint8_t vendor_uuid[16];		// only highest 14 bytes are used
	uint32_t cert_version; 		// only lowest 4 bits are effective due to
						// size limitation of OTP field
	uint32_t cert_version_mask;
} ta_root_cert_info_t;

typedef struct ta_export_info {
	ta_root_cert_info_t ta_root_cert_info;
	ta_cert_info_t ta_cert_info;
	ta_img_info_t ta_img_info;
	int ta_getinfo_error;
} ta_export_info_t;


struct ta_vendor_info {
	uint8_t name[15];
	uint32_t reserved;
};

struct ta_info {
	uint32_t version;
	uint32_t size;
	struct ta_vendor_info vendor_info[MAX_VENDOR_NUMS];
};

#endif /* _TA_INFO_H_ */

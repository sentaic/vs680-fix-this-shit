// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef BCMIMG_H
#define BCMIMG_H

typedef struct {
	uint32_t    HeaderVersion;	/* version information */
	uint16_t    MagicNumber;	/* magic number showing this is an image */
	uint8_t     CustKeyType;	/* 0: no encryption,
					 * 1: marvell own cust key
					 * 2: generic custkey
					 */
	uint8_t     CodeType;		/* 0: BCM FW (EROM/BCM),
					 * 1: ARM Sys_Init,
					 * 2. Marvell A/V FW
					 * 3. ARM TZ
					 * 4:ARM Boot loader, Linux Kernel, RootFS,
					 * 5: OEM FW, 6: ARM App, 7-15:resered
					 */
	uint16_t    Reserved0;
	uint16_t    UserData;		/* User Data for google */
	uint8_t     WrappedUserKey[16];	/* custkey encrypted user key,
					 * fill with all 0s if CustKeyType==0 */
	uint32_t    MarketID;
	uint32_t    MarketIDMask;	/* should be all '1'
					 * (0xFFFFFFFF) in default */
	uint8_t     Version;
	uint8_t     VersionMask;	/* should be all '1' (0xFF) in default */
	uint16_t    Reserved2;
	uint32_t    ImageSize;		/* image size in bytes */
	uint8_t     ImageHash[32];	/* SHA256 hash of the image */
	uint8_t     Signature[256];	/* Signature of the header */
} MV_ImageHeader;

#endif

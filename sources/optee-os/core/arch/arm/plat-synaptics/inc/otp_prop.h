// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _OTP_PROP_H_
#define _OTP_PROP_H_

#include "types_ext.h"

typedef enum {
	/* feature 0 */
	OTP_DTS_ENABLE,
	OTP_MACROVERSION_ENABLE,
	OTP_DOLBY_ENABLE,
	OTP_IS_VERIMATRIX_PRODUCT,
	OTP_MPEG2_DEC_ENABLE,
	OTP_FULL_PERFORMANCE_ENABLE,
	OTP_BINDINFO_VERSION_ID,

	/* feature 1*/
	OTP_CORESIGHT_ENABLE,
	OTP_JTAG_BS,
	OTP_CPU,
	OTP_DDR,
	OTP_VCORE,
	OTP_CORESIGHT_PERMANENT_DISABLE,

	/* feature 2 */
	OTP_HANTROL_DEC_ENABLE,

	/* feature 3 */
	OTP_HANTROL_PP_ENABLE,

	/* feature 4 */
	OTP_LEAKAGE,
	OTP_DR0,
	OTP_TSEN,
	OTP_PVCOMP_REVISION,
	OTP_SILICON_REVISION_FLAG,

	/* entry 6 the first entry is entry 0 */
	OTP_AESK0_ECC,
	OTP_RKEK_CRC,
	OTP_CACRC,

	/* ecc, entry 21 */
	OTP_ECCCA_ROOT_KEY,
	OTP_ECCCA_M2M_KEY,
	OTP_ECCCA_KEY_ID,
	OTP_CKSUMCA_KEY,

	/* rom feature, entry 31 */
	OTP_ROMK_DISABLE,
	OTP_USB_DISABLE_BOOTSTRAP,
	OTP_USB_DISABLE_BLANK_MEDIA,
	OTP_PANIC_BEHAVIOR_RESET,
	OTP_USB_FULL_SPEED_ENABLE,
	OTP_USB_IGNORE_RESET_TRACKER,
	OTP_USB_DISABLE_BOOT,
	OTP_ALT_JTAG_KEY_ENABLE,
	OTP_USB_BOOT_TIMEOUT,

	/* market ID, entry 32 and 33 */
	OTP_MARKET_ID0,
	OTP_MARKET_ID1,
	OTP_MARKET_ID3,

	/* entry 34 */
	OTP_MRVL_SINGK_RIGHT,
	OTP_SIGNK_RIGHT,

	/* entry 46 */
	OTP_RKEK_ID,

	/* entry 0 */
	OTP_ULT,

	/* entry 43 */
	OTP_UNIQUE_CHIP_ID,

	/* feature 0 */
	OTP_4K_VIDEO_ENABLE,

	/* feature 3 */
	OTP_DRO0,
	OTP_DRO1,
	OTP_DRO2,
	OTP_DRO3,

	/* feature 4 */
	OTP_SIDD0,
	OTP_SIDD1,
	OTP_TSEN0,
	OTP_TSEN1,

	/* feature 0 */
	OTP_DOLBY_VISION_ENABLE,

	/* bg5ct new otp */
	OTP_SEGID,
	OTP_JTAG,			//b00  = JTAG is open for ATE mode access
					//b01 = JTAG is password  protected for ATE mode access
					//	b10 and b11 = JTAG is closed for ATE mode access.

	OTP_CA_KEY_SOURCE,
	OTP_TECHNICOLOR_HDR_ENABLE,

	OTP_MAX_PROP_NUM
} otp_prop_list;

typedef enum {
	OTP_PROP_VALUE,
	OTP_PROP_BUF,
} otp_prop_type;

typedef struct {
	union {
		uint32_t value;
		void *buf;
	} data;
	int length;
} otp_property;
#endif /* _OTP_PROP_H_ */

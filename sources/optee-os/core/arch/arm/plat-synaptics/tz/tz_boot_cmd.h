// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TZ_BOOT_CMD_H_
#define _TZ_BOOT_CMD_H_

#define SMC_FUNC_TOS_BOOT_START		(0xbf000000)
#define TOS_BOOT(number)		(SMC_FUNC_TOS_BOOT_START + (number))

enum tz_boot_func_id {
	/* param: none;
	 * return: version number */
	SMC_FUNC_TOS_BOOT_VERSION		= TOS_BOOT(0x00),
	/* param: {stage, mode};
	 * return: error code */
	SMC_FUNC_TOS_BOOT_STAGE			= TOS_BOOT(0x10),
	/* param: {attr_mask, attr_val};
	 * return: count */
	SMC_FUNC_TOS_MEM_REGION_COUNT		= TOS_BOOT(0x20),
	/* param: {attr_mask, attr_val, region, max_num};
	 * return: count */
	SMC_FUNC_TOS_MEM_REGION_LIST,
	/* param: none;
	 * return: error code */
	SMC_FUNC_TOS_OUTER_CACHE_ENABLE		= TOS_BOOT(0x30),
	/* param: none;
	 * return: error code */
	SMC_FUNC_TOS_OUTER_CACHE_DISABLE,
	/* param: none;
	 * return: error code */
	SMC_FUNC_TOS_OUTER_CACHE_RESUME,
	/* param: {src, src_len, dst, dst_len};
	 * return: {errcode,dec_len} */
	SMC_FUNC_TOS_CRYPTO_VERIFY_IMAGE	= TOS_BOOT(0x40),

	/* param: {start_phy_addr, size};
	 * return: error code */
	SMC_FUNC_TOS_SECURE_CACHE_CLEAN		= TOS_BOOT(0x50),
	/* param: {start_phy_addr, size};
	 * return: error code */
	SMC_FUNC_TOS_SECURE_CACHE_INVALIDATE,
	/* param: {start_phy_addr, size};
	 * return: error code */
	SMC_FUNC_TOS_SECURE_CACHE_CLEAN_INVALIDATE,

	/* param: {src, dst, len};
	 * return: {errcode} */
	SMC_FUNC_TOS_MEM_MOVE			= TOS_BOOT(0x60),

	/*
	 * param: {param_addr, param_len};
	 * return: {errcode}
	 */
	SMC_FUNC_TOS_FASTCALL_GENERIC		= TOS_BOOT(0x70),

	/* param: {addr, ops, value};
	 * return: {errcode} */
	SMC_FUNC_TOS_SECURE_REG			= TOS_BOOT(0x80),
};

enum tz_boot_stage {
	TZ_BOOT_STAGE_ROMCODE,
	TZ_BOOT_STAGE_SYSINIT,
	TZ_BOOT_STAGE_TRUSTZONE,
	TZ_BOOT_STAGE_BOOTLOADER,
	TZ_BOOT_STAGE_LINUX,
	TZ_BOOT_STAGE_ANDROID
};

enum tz_boot_mode {
	TZ_BOOT_MODE_NORMAL,
	TZ_BOOT_MODE_RECOVERY,
	TZ_BOOT_MODE_FASTBOOT
};

#endif /* _TZ_BOOT_CMD_H_ */

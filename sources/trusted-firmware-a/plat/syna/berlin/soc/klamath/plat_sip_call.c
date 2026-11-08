 /*
 * Copyright (c) 2026, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <common/runtime_svc.h>

#include "berlin_sip_svc.h"

#define SOC2MCU_DTCM_S              0xE1000000 //START_MCU_DTCM_S 0x30000000
#define OTP_DUMP_SIZE				(0x36C)

int plat_read_public_otp(uint64_t otp_buff, uint64_t size)
{
	uintptr_t va_otp_buf = (uintptr_t)otp_buff;
	unsigned int attr = MT_MEMORY | MT_RW | MT_NS;
	int ret = 0;
	const uint8_t *opt_base = (uint8_t *)(uintptr_t)SOC2MCU_DTCM_S;

	if (size > OTP_DUMP_SIZE)
		return -EINVAL;

	if (!otp_buff || (otp_buff % 64ULL) != 0ULL)
		return -EINVAL;

	ret = mmap_add_dynamic_region(otp_buff, va_otp_buf, PAGE_SIZE_4KB, attr);
	if (ret != 0) {
		ERROR("Failed to add dynamic mmap entry @0x%lx, size = 0x%lx, ret = %d\n", va_otp_buf, size, ret);
		panic();
	}

	inv_dcache_range(va_otp_buf, size);
	memcpy((void *)va_otp_buf, opt_base, size);
	flush_dcache_range(va_otp_buf, size);

	return ret;
}

/*******************************************************************************
 * This function is responsible for handling all klamath SiP calls
 ******************************************************************************/
uint64_t berlin_default_handler(uint32_t smc_fid,
		     uint64_t x1,
		     uint64_t x2,
		     uint64_t x3,
		     uint64_t x4,
		     const void *cookie,
		     void *handle,
		     uint64_t flags)
{
	int ret = 0;

	switch (smc_fid) {
	case SYNA_SIP_SMC32_RD_PUBLIC_OTP:
	case SYNA_SIP_SMC64_RD_PUBLIC_OTP:
		ret = plat_read_public_otp(x1, x2);
		break;
	default:
		ret = -ENOTSUP;
		break;
	}

	SMC_RET1(handle, ret);

}

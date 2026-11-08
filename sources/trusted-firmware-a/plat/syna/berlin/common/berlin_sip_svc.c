/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/debug.h>
#include <common/runtime_svc.h>

#include "berlin_sip_svc.h"
#include "secure_register.h"
#include "perif_reset.h"

#ifdef CFG_SYNA_BCM
#include "bcm.h"
#endif
#ifdef CFG_SYNAPROT_MAILBOX
#include "synaprot_mailbox.h"
#endif

#pragma weak berlin_set_wakeup_sources
#pragma weak berlin_perif_reset
#pragma weak berlin_default_handler

static int berlin_sip_setup(void)
{
	return 0;
}

uint64_t berlin_default_handler(uint32_t smc_fid,
				   uint64_t x1,
				   uint64_t x2,
				   uint64_t x3,
				   uint64_t x4,
				   void *cookie,
				   void *handle,
				   uint64_t flags)
{
	ERROR("%s: unhandled SMC (0x%x)\n", __func__, smc_fid);
	SMC_RET1(handle, SMC_UNK);
}

int berlin_set_wakeup_sources(u_register_t x1, u_register_t x2, u_register_t x3, u_register_t x4)
{
	return SMC_UNK;
}

int berlin_perif_reset(u_register_t x1, u_register_t x2)
{
	return SMC_UNK;
}

uintptr_t berlin_sip_handler(uint32_t smc_fid,
			 u_register_t x1,
			 u_register_t x2,
			 u_register_t x3,
			 u_register_t x4,
			 void *cookie,
			 void *handle,
			 u_register_t flags)
{
	uint32_t ret = 0;
	uint32_t op = (uint32_t)x1;
	uint32_t index = (uint32_t)x2;
	uint32_t value = (uint32_t)x3;
	u_register_t x5, x6, x7;

	switch (smc_fid) {
	case SYNA_SIP_SMC32_SREGISTER_OP:
	case SYNA_SIP_SMC64_SREGISTER_OP:
		ret = secure_register_op(op, index, &value);
		SMC_RET2(handle, ret, value);
	case SYNA_SIP_SMC32_WAKEUP_CONFIG:
	case SYNA_SIP_SMC64_WAKEUP_CONFIG:
		ret = berlin_set_wakeup_sources(x1, x2, x3, x4);
		SMC_RET1(handle, ret);
#ifdef CFG_SYNA_BCM
	case SYNA_SIP_SMC32_BCM_CMD:
	case SYNA_SIP_SMC64_BCM_CMD:
		if (is_caller_non_secure(flags) && !bcm_sip_cmd_is_supported(x1)) {
			SMC_RET1(handle, SMC_UNK);
		}

		x5 = SMC_GET_GP(handle, CTX_GPREG_X5);
		x6 = SMC_GET_GP(handle, CTX_GPREG_X6);
		x7 = SMC_GET_GP(handle, CTX_GPREG_X7);
		ret = bcm_cmd_handle(x1, x2, x3, x4, x5, x6, x7);
		SMC_RET1(handle, ret);

	case SYNA_SIP_SMC32_USB_LOAD:
	case SYNA_SIP_SMC64_USB_LOAD:
		ret = bcm_erom_load_usb_image(x1, x2, x3, x4);
		SMC_RET1(handle, ret);
	case SYNA_SIP_SMC32_OTP_PROGRAM:
	case SYNA_SIP_SMC64_OTP_PROGRAM:
		ret = bcm_erom_program_otp(x1, x2);
		SMC_RET1(handle, ret);
	case SYNA_SIP_SMC32_RD_PUBLIC_OTP:
	case SYNA_SIP_SMC64_RD_PUBLIC_OTP:
		ret = bcm_erom_read_public_otp(x1, x2);
		SMC_RET1(handle, ret);
	case SYNA_SIP_SMC32_OTP_READ_OTP:
	case SYNA_SIP_SMC64_OTP_READ_OTP:
		ret = bcm_otp_read_otp(x1, (uint32_t *)(uintptr_t)x2,
					(uint32_t *)(uintptr_t)x3);
		SMC_RET1(handle, ret);
	case SYNA_SIP_SMC32_OTP_WRITE_OTP:
	case SYNA_SIP_SMC64_OTP_WRITE_OTP:
		ret = bcm_otp_write_otp(x1, (uint32_t)x2, (uint32_t)x3);
		SMC_RET1(handle, ret);
	case SYNA_SIP_SMC32_SECURE_SRAM_ACCESS:
	case SYNA_SIP_SMC64_SECURE_SRAM_ACCESS:
		uint32_t access_type = (uint32_t)x1;
		uint32_t idx = (uint32_t)x2;
		uint64_t val= (uint64_t)x3;
		ret = bcm_access_secure_sram(access_type, idx, &val);
		SMC_RET2(handle, ret, val);

#elif defined(CFG_SYNAPROT_MAILBOX)
	case SYNA_SIP_SMC32_BCM_CMD:
	case SYNA_SIP_SMC64_BCM_CMD:
		mailbox_rsp_t rsp = {0};

		x5 = SMC_GET_GP(handle, CTX_GPREG_X5);
		x6 = SMC_GET_GP(handle, CTX_GPREG_X6);
		x7 = SMC_GET_GP(handle, CTX_GPREG_X7);
		ret = mailbox_cmd_handle(x1, x2, x3, x4, x5, x6, x7, &rsp);

		SMC_RET8(handle, ret, rsp.cmd_ret,
				rsp.cmd_status[0], rsp.cmd_status[1], rsp.cmd_status[2],
				rsp.cmd_status[3], rsp.cmd_status[4], rsp.cmd_status[5]);
#endif
	case SYNA_SIP_SMC32_PERIFRESET:
	case SYNA_SIP_SMC64_PERIFRESET:
		ret = berlin_perif_reset(x1, x2);
		SMC_RET1(handle, ret);

	default:
		return berlin_default_handler(smc_fid, x1, x2, x3, x4,
			cookie, handle, flags);
	}
}

/* Define a SPD runtime service descriptor for fast SIP SMC calls */
DECLARE_RT_SVC(
	berlinspd_sip_fast,
	OEN_SIP_START,
	OEN_SIP_END,
	SMC_TYPE_FAST,
	berlin_sip_setup,
	berlin_sip_handler
);

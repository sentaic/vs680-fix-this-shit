// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <string.h>
#include "otp_prop.h"
#include "hal_soc.h"
#include "tz_errcode.h"
#include "klamath_memmap.h"
#include "soc.h"
#include "global.h"
#include <kernel/misc.h>
#include <kernel/boot.h>
#include "tee_api_types.h"
#include <mm/core_memprot.h>
#include <kernel/pm.h>

#define SOC_RESET_CTRL_BASE		(START_CHIP_CTRL_REG + RA_Gbl_ResetTrigger)

#define SOC2MCU_DTCM_S              0xE1000000 //START_MCU_DTCM_S 0x30000000
#define OTP_DUMP_OFFSET      0x0
#define OTP_DUMP_SIZE        (0x36C)
#define SOC_OTP_BASE_ADDR    (vaddr_t)phys_to_virt_io(SOC2MCU_DTCM_S, OTP_DUMP_SIZE)

#define SOC_OTP_ROWS			152
#define SOC_OTP_ROW_LEN			4
static unsigned char otp_data[OTP_DUMP_SIZE];

void otp_init(void)
{
	uint8_t *pOtpBuf = otp_data;
	uint8_t *pOtpBase = (uint8_t *)SOC_OTP_BASE_ADDR;

	memcpy(pOtpBuf, pOtpBase, OTP_DUMP_SIZE);

	DMSG("OTP init done");
}

int soc_get_otp_raw_data(uint32_t row, uint8_t *buffer)
{
	if (row >= SOC_OTP_ROWS)
		return TZ_ERROR_BAD_PARAMETERS;

	memcpy(buffer, &otp_data[row * SOC_OTP_ROW_LEN], SOC_OTP_ROW_LEN);

	return TZ_SUCCESS;
}

void __attribute__((noreturn)) reset_soc(void)
{

	/* set soc reset bit to 1 and reset it */
	int i;
	while (1){
		*(volatile uint32_t*)SOC_RESET_CTRL_BASE = 1;
		i = 300;
		while(i--);
	}
}

unsigned long thread_cpu_suspend_handler(unsigned long a0 __unused,
					unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;

	if (get_core_pos() != 0)
		return 1;

	retstatus = pm_change_state(PM_OP_SUSPEND, 0);

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;

	return 0;
}


unsigned long thread_cpu_resume_handler(unsigned long a0 __unused,
					unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;
	if (get_core_pos() == 0) {
		boot_primary_init_intc();
	} else
		boot_secondary_init_intc();


	retstatus = pm_change_state(PM_OP_RESUME, 0);

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;

	return 0;
}


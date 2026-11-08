// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <string.h>
#include "otp_prop.h"
#include "hal_soc.h"
#include "tz_errcode.h"
#include "../chip/myna2/include/Galois_memmap.h"
#include "../chip/myna2/include/soc.h"
#include "../chip/myna2/include/global.h"
#include "../chip/myna2/include/tsp.h"
#include <kernel/misc.h>
#include <kernel/boot.h>
#include "tee_api_types.h"
#include <mm/core_memprot.h>
#include <kernel/pm.h>

#define MEMMAP_OTP_DUMP_OFFSET          0x0
#define MEMMAP_OTP_DUMP_SIZE            (0xC00)
#define SOC_OTP_BASE_ADDR             (vaddr_t)phys_to_virt_io((MEMMAP_SRAM_REG_BASE + MEMMAP_OTP_DUMP_OFFSET), MEMMAP_SRAM_REG_SIZE)

#define SOC_OTP_ROWS			192
#define SOC_OTP_ROW_LEN			16
static unsigned char otp_data[SOC_OTP_ROWS * SOC_OTP_ROW_LEN / 2];

#define SOC_RESET_CTRL_BASE		(vaddr_t)phys_to_virt_io((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), MEMMAP_CHIP_CTRL_REG_SIZE)

void otp_init(void)
{
	int i = 0;
	unsigned short *pOtpBuf = (unsigned short*)otp_data;
	unsigned int *pOtpBase = (unsigned int*)SOC_OTP_BASE_ADDR;

	/*re-layout the otp field to normal layout*/
	for (i = 0; i < SOC_OTP_ROWS << 2; i += SOC_OTP_ROW_LEN / 4) {
		pOtpBuf[i] =  pOtpBase[i] & 0xffff;
		pOtpBuf[i + 1] =  pOtpBase[i + 1] & 0xffff;
		pOtpBuf[i + 2] =  pOtpBase[i + 2] & 0xffff;
		pOtpBuf[i + 3] =  pOtpBase[i + 3] & 0xffff;
	}

	memset(pOtpBase, 0, SOC_OTP_ROWS * SOC_OTP_ROW_LEN);
}

int soc_get_otp_prop(uint32_t index, void* dst, int dst_len)
{
	int result = TZ_SUCCESS;

	if (dst_len < (int)sizeof(unsigned int))
		return TZ_ERROR_BAD_PARAMETERS;

	switch (index) {
	case OTP_DOLBY_VISION_ENABLE:
		*(unsigned int*)dst = (otp_data[(0x10 >> 1) + 1] >> 2) & 0x1 ? 0 : 1;
		break;
	default:
		result = TZ_ERROR_BAD_PARAMETERS;
		break;
	}

	return result;
}

int soc_get_otp_raw_data(uint32_t row, uint8_t *buffer)
{
	if (row >= SOC_OTP_ROWS)
		return TZ_ERROR_BAD_PARAMETERS;

	memcpy(buffer, &otp_data[row * SOC_OTP_ROW_LEN / 2], SOC_OTP_ROW_LEN / 2);

	return TZ_SUCCESS;
}

void clear_sram(void)
{
	/* clear ZSP SRAM */
	//memset((void*)MEMMAP_ZSP_REG_BASE, 0, 0x40000);
	/* clear TSP SRAM */
	/* FIGO0  Data Sram: 0xf7a40000~0xf7a47fff */
	//memset((void *)0xf7a40000, 0, 0x8000);
	/* FIGO0  Instruction Sram: 0xf7a50000~0xf7a5ffff */
	//memset((void *)0xf7a50000, 0, 0x10000);
	/* FIGO1  Data Sram: 0xf7a60000~0xf7a67fff */
	//memset((void *)0xf7a60000, 0, 0x8000);
	/* FIGO1  Instruction Sram: 0xf7a70000~0xf7a7ffff */
	//memset((void *)0xf7a68000, 0, 0x2000);

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


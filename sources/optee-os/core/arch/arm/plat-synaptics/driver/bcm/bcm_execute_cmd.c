// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <mm/core_memprot.h>
#include "bcm_mailbox.h"
#include "bcm_primitive.h"
#include "bcm_status.h"
#include "tz_errcode.h"
#include <string.h>
#include <stdint.h>
#include <kernel/mutex.h>
#include <tee_api_defines.h>
#include <trace.h>
#include "tz_boot_cmd.h"
#include <kernel/tee_time.h>
#include "bcm_execute_cmd.h"

extern unsigned long tz_get_boot_stage(void);
#define DEFAULT_WAIT_COUNT		10
static struct mutex bcm_mutex = MUTEX_INITIALIZER;

static inline void bcm_memclr(void *p, uint32_t numInt)
{
	volatile uint32_t *pStart = (uint32_t *)p;
	volatile uint32_t *pEnd = pStart + numInt;

	while (pStart < pEnd)
		*pStart++ = 0;
}

static inline void bcm_clear_command(BCM_MAILBOX_COMMAND * pCmd)
{
	bcm_memclr(pCmd, sizeof(BCM_MAILBOX_COMMAND)/sizeof(uint32_t));
}

static inline void bcm_clear_status(BCM_MAILBOX_STATUS * pStatus)
{
	bcm_memclr(pStatus, sizeof(BCM_MAILBOX_STATUS)/sizeof(uint32_t));
}

static int bcm_send_command(BCM_MAILBOX_COMMAND *pCmd)
{
	if (pCmd==NULL)
		return TZ_ERROR_BAD_PARAMETERS;

	BCM_Reg_Write32(BCM_COMMAND_PARAMETER0, pCmd->arg0);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER1, pCmd->arg1);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER2, pCmd->arg2);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER3, pCmd->arg3);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER4, pCmd->arg4);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER5, pCmd->arg5);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER6, pCmd->arg6);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER7, pCmd->arg7);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER8, pCmd->arg8);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER9, pCmd->arg9);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER10, pCmd->arg10);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER11, pCmd->arg11);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER12, pCmd->arg12);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER13, pCmd->arg13);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER14, pCmd->arg14);
	BCM_Reg_Write32(BCM_COMMAND_PARAMETER15, pCmd->arg15);
	BCM_Reg_Write32(BCM_SECURE_PROCESSOR_COMMAND, pCmd->command);
	DMSG("Sending BCM command, command=0x%x\n", pCmd->command);

	return TZ_SUCCESS;
}

static int bcm_wait_for_complete(uint32_t wait_count)
{
	volatile uint32_t intr_stat;
	while(1) {
		BCM_Reg_Read32(BCM_HOST_INTERRUPT_REGISTER, &intr_stat);
		if (intr_stat & 1)
			break;
		if (tz_get_boot_stage() == TZ_BOOT_STAGE_LINUX) {
			tee_time_busy_wait(wait_count);
		}
	}
	return TZ_SUCCESS;
}

static int bcm_get_status(BCM_MAILBOX_STATUS *pStatus)
{
	if (pStatus==NULL)
		return TZ_ERROR_BAD_PARAMETERS;

	BCM_Reg_Read32(BCM_COMMAND_RETURN_STATUS, &pStatus->return_status);
	BCM_Reg_Read32(BCM_COMMAND_STATUS0, &pStatus->arg0);
	BCM_Reg_Read32(BCM_COMMAND_STATUS1, &pStatus->arg1);
	BCM_Reg_Read32(BCM_COMMAND_STATUS2, &pStatus->arg2);
	BCM_Reg_Read32(BCM_COMMAND_STATUS3, &pStatus->arg3);
	BCM_Reg_Read32(BCM_COMMAND_STATUS4, &pStatus->arg4);
	BCM_Reg_Read32(BCM_COMMAND_STATUS5, &pStatus->arg5);
	BCM_Reg_Read32(BCM_COMMAND_STATUS6, &pStatus->arg6);
	BCM_Reg_Read32(BCM_COMMAND_STATUS7, &pStatus->arg7);
	BCM_Reg_Read32(BCM_COMMAND_STATUS8, &pStatus->arg8);
	BCM_Reg_Read32(BCM_COMMAND_STATUS9, &pStatus->arg9);
	BCM_Reg_Read32(BCM_COMMAND_STATUS10, &pStatus->arg10);
	BCM_Reg_Read32(BCM_COMMAND_STATUS11, &pStatus->arg11);
	BCM_Reg_Read32(BCM_COMMAND_STATUS12, &pStatus->arg12);
	BCM_Reg_Read32(BCM_COMMAND_STATUS13, &pStatus->arg13);
	BCM_Reg_Read32(BCM_COMMAND_STATUS14, &pStatus->arg14);
	BCM_Reg_Read32(BCM_COMMAND_STATUS15, &pStatus->arg15);

	if (pStatus->return_status!=0) {
		return TZ_ERROR_BAD_STATE;
	}

	return TZ_SUCCESS;
}

static void bcm_clear_interrupts(uint32_t __maybe_unused mask)
{
	BCM_Reg_Write32(BCM_HOST_INTERRUPT_REGISTER, 0x70001);
}

int execute_bcm_command(BCM_MAILBOX_COMMAND *pCmd, BCM_MAILBOX_STATUS *pStatus, uint32_t delay)
{
	int ret;

	if (pStatus==NULL || pCmd==NULL)
		return TZ_ERROR_BAD_PARAMETERS;

	ret = bcm_send_command(pCmd);
	if (ret)
		return ret;
	ret = bcm_wait_for_complete(delay);
	if (ret)
		return ret;
	ret = bcm_get_status(pStatus);
	bcm_clear_interrupts(~0);

	if (ret) {
		IMSG("!!!!BCM cmd 0x%x Return ERROR, return_status=0x%x!!!!\n",
				pCmd->command, pStatus->return_status);
	}

	return ret;
}

int sp_exec_cmd_ex(const BCM_MAILBOX_COMMAND *cmd, size_t cmd_len,
		BCM_MAILBOX_STATUS *result, size_t result_len,
		uint32_t delay)
{
	int res = 0;
	if (!cmd || (cmd_len > sizeof(BCM_MAILBOX_COMMAND)) ||
		!result || (result_len < sizeof(BCM_MAILBOX_STATUS)))
		return TEE_ERROR_BAD_PARAMETERS;

	bcm_clear_status(result);
	mutex_lock(&bcm_mutex);
	res = execute_bcm_command((BCM_MAILBOX_COMMAND *) cmd, result, delay);
	mutex_unlock(&bcm_mutex);

	return res;
}
int sp_exec_cmd(const BCM_MAILBOX_COMMAND *cmd, size_t cmd_len,
		BCM_MAILBOX_STATUS *result, size_t result_len)
{
	return sp_exec_cmd_ex(cmd, cmd_len, result, result_len, DEFAULT_WAIT_COUNT);
}

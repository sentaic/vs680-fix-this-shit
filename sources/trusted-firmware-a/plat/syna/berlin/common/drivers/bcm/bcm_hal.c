/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include "bcm_hal.h"
#include "bcm_reg.h"
#include <lib/spinlock.h>

#if 0
#define bcm_display INFO
#else
#define bcm_display(...)
#endif

#define BCM_STATUS_BCM_FAULT			(1<<10)
#define BCM_STATUS_BOOTSTRAP_IN_PROGRESS	(1<<9)
#define BCM_STATUS_BCM_READY			(1<<8)
#define BCM_STATUS_BCM_CMD_FLIP			(1<<7)

#define devmem_phy_to_virt(x) (x)

#ifndef BERLIN_SUSPEND_SRAM
static spinlock_t bcm_lock;

static void syna_bcm_lock(void)
{
	spin_lock(&bcm_lock);

}
static void syna_bcm_unlock(void)
{
	spin_unlock(&bcm_lock);

}
#else
static void syna_bcm_lock(void)
{

}
static void syna_bcm_unlock(void)
{

}
#endif
static void bcm_delay_us(unsigned us)
{
	volatile unsigned int i = us;
	for (;i>0;i--);
}

void bcm_memclr(void *p, int numInt)
{
	volatile unsigned int *pStart = (unsigned int *)p;
	volatile unsigned int *pEnd = pStart + numInt;
	while (pStart < pEnd)
		*pStart++ = 0;
}

void bcm_clear_command(BCM_MAILBOX_COMMAND * pCmd)
{
	bcm_memclr(pCmd, sizeof(BCM_MAILBOX_COMMAND)/sizeof(int));
}

void bcm_clear_status(BCM_MAILBOX_STATUS * pStatus)
{
	bcm_memclr(pStatus, sizeof(BCM_MAILBOX_STATUS)/sizeof(int));
}

static void bcm_send_command(const BCM_MAILBOX_COMMAND *pCmd)
{
	volatile _BCM_MAILBOX * biu =
		(_BCM_MAILBOX *)devmem_phy_to_virt(BCM_MAILBOX_BASE);

	biu->command_parameter0  = pCmd->arg0;
	biu->command_parameter1  = pCmd->arg1;
	biu->command_parameter2  = pCmd->arg2;
	biu->command_parameter3  = pCmd->arg3;
	biu->command_parameter4  = pCmd->arg4;
	biu->command_parameter5  = pCmd->arg5;
	biu->command_parameter6  = pCmd->arg6;
	biu->command_parameter7  = pCmd->arg7;
	biu->command_parameter8  = pCmd->arg8;
	biu->command_parameter9  = pCmd->arg9;
	biu->command_parameter10 = pCmd->arg10;
	biu->command_parameter11 = pCmd->arg11;
	biu->command_parameter12 = pCmd->arg12;
	biu->command_parameter13 = pCmd->arg13;
	biu->command_parameter14 = pCmd->arg14;
	biu->command_parameter15 = pCmd->arg15;
	biu->secure_processor_command = pCmd->command;
	bcm_display("Sending BCM command, command=0x%x\n", pCmd->command);
}


static int bcm_wait_for_complete(int wait_count)
{
	volatile _BCM_MAILBOX * biu =
			(_BCM_MAILBOX *)devmem_phy_to_virt(BCM_MAILBOX_BASE);
	volatile unsigned int intr_stat;

	while(1) {
		intr_stat = biu->host_interrupt_register; // 0xc8 Write-to-Clear
		if(intr_stat & 1)
			break;
		bcm_delay_us(1000);
	}

	return 0;
}

static void bcm_get_status(BCM_MAILBOX_STATUS *pStatus)
{
	volatile _BCM_MAILBOX * biu =
			(_BCM_MAILBOX *)devmem_phy_to_virt(BCM_MAILBOX_BASE);

	pStatus->return_status = biu->command_return_status;
	pStatus->arg0  = biu->command_status0;
	pStatus->arg1  = biu->command_status1;
	pStatus->arg2  = biu->command_status2;
	pStatus->arg3  = biu->command_status3;
	pStatus->arg4  = biu->command_status4;
	pStatus->arg5  = biu->command_status5;
	pStatus->arg6  = biu->command_status6;
	pStatus->arg7  = biu->command_status7;
	pStatus->arg8  = biu->command_status8;
	pStatus->arg9  = biu->command_status9;
	pStatus->arg10 = biu->command_status10;
	pStatus->arg11 = biu->command_status11;
	pStatus->arg12 = biu->command_status12;
	pStatus->arg13 = biu->command_status13;
	pStatus->arg14 = biu->command_status14;
	pStatus->arg15 = biu->command_status15;
}

static void bcm_clear_interrupts(unsigned int mask)
{
	volatile _BCM_MAILBOX * biu =
		(_BCM_MAILBOX *)devmem_phy_to_virt(BCM_MAILBOX_BASE);

	biu->host_interrupt_register = 0x70001;
}

static void execute_bcm_command(const BCM_MAILBOX_COMMAND *pCmd,
		BCM_MAILBOX_STATUS *pStatus)
{
	bcm_send_command(pCmd);
	bcm_wait_for_complete(0);
	bcm_get_status(pStatus);
	bcm_clear_interrupts(~0);
}

int bcm_exec_func(const BCM_MAILBOX_COMMAND* cmd)
{
	BCM_MAILBOX_STATUS status;

	bcm_clear_status(&status);
	syna_bcm_lock();
	execute_bcm_command(cmd, &status);
	syna_bcm_unlock();

	return status.return_status;
}

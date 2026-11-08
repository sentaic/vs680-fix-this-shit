/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <common/debug.h>
#include "syna_ipc.h"
#include "sm_ipc.h"

#define IPC_MB_TIMEOUT (1)

void sm_system_off(void)
{
	int ret = 0;

	ipc_mailbox_set_msg(SM_SYSTEM_OFF, IPC_MAILBOX_ID);

	ret = ipc_mailbox_wait_msg_done(IPC_MAILBOX_ID, IPC_MB_TIMEOUT);
	if (ret != 0) {
		ERROR("IPC command timeout!");
	}

}

void sm_system_reset(void)
{
	int ret = 0;
	ipc_mailbox_set_msg(SM_SYSTEM_RESET, IPC_MAILBOX_ID);

	ret = ipc_mailbox_wait_msg_done(IPC_MAILBOX_ID, IPC_MB_TIMEOUT);
	if (ret != 0) {
		ERROR("IPC command timeout!");
	}

}

void sm_system_warmdown(unsigned int power_state)
{
	unsigned int cmd = -1;
	int ret = 0;

	if (power_state) {
		cmd = SM_SYSTEM_SUSPEND_WFI;
	} else {
		cmd = SM_SYSTEM_SUSPEND_POWEROFF;
	}

	ipc_mailbox_set_msg(cmd, IPC_MAILBOX_ID);

	ret = ipc_mailbox_wait_msg_done(IPC_MAILBOX_ID, IPC_MB_TIMEOUT);
	if (ret != 0) {
		ERROR("IPC command timeout!");
	}
}

void sm_do_system_warmup(void)
{
	int ret = 0;

	ipc_mailbox_set_msg(SM_SYSTEM_WARMUP, IPC_MAILBOX_ID);

	ret = ipc_mailbox_wait_msg_done(IPC_MAILBOX_ID, IPC_MB_TIMEOUT);

	if (ret != 0) {
		ERROR("IPC command timeout!");
	}
}



/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SM_IPC_
#define _SM_IPC_

#define SM_SYSTEM_RESET					0xAFF1500E
#define SM_SYSTEM_OFF					0xAFF2500D
#define SM_SYSTEM_SUSPEND_POWEROFF		0xAFF3500C
#define SM_SYSTEM_SUSPEND_WFI			0xAFF4500B
#define SM_SYSTEM_WARMUP				0xAFF5500A

#define IPC_MAILBOX_ID			3

void sm_system_off(void);
void sm_system_reset(void);
void sm_system_suspend(void);
void sm_system_warmdown(unsigned int power_state);
#endif // _SM_IPC_

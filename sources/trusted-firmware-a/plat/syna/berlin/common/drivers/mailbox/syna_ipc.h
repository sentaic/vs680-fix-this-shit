/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SYNA_IPC_
#define _SYNA_IPC_

int ipc_lock_set(int id);
int ipc_lock_clear(int id);
int ipc_lock_status(int id);
int ipc_mailbox_set_msg(unsigned int msg, int mailbox_id);
int ipc_mailbox_wait_msg_done(int mailbox_id, int timeout_us);
int ipc_mailbox_get_msg(int mailbox_id, unsigned int* p_msg);

#endif // _SYNA_IPC_

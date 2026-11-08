// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _BCM_EXECUTE_CMD_H_
#define _BCM_EXECUTE_CMD_H_
#include "bcm_mailbox.h"
/** send command to secure processor and wait until its completion.
 *
 * @param cmd		input command pointer
 * @param cmd_len	input command length
 * @param result	return result pointer
 * @param result_len	the length of return result
 *
 * @retval 0	success
 * @retval !0	fail
 *
 */

int sp_exec_cmd(const BCM_MAILBOX_COMMAND *cmd, size_t cmd_len,
		BCM_MAILBOX_STATUS *result, size_t result_len);

/** send command to secure processor and wait until its completion.
 *  can set the delay number
 *
 * @param cmd		input command pointer
 * @param cmd_len	input command length
 * @param result	return result pointer
 * @param result_len	the length of return result
 * @param delay		delay ms number
 *
 * @retval 0	success
 * @retval !0	fail
 *
 */

int sp_exec_cmd_ex(const BCM_MAILBOX_COMMAND *cmd, size_t cmd_len,
		BCM_MAILBOX_STATUS *result, size_t result_len,
		uint32_t delay);
#endif /* _BCM_EXECUTE_CMD_H_ */

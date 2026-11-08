/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SYNA_MAILBOX_
#define _SYNA_MAILBOX_


//int mailbox_h2s_raw_msg_send(int mailbox_id, unsigned int dwords[], int n);
int mailbox_h2s_get_spk_version(unsigned int *version, int mailbox_id);

typedef enum {
	HOST2SYNA_SENDER_M52	= 0x1,
	HOST2SYNA_SENDER_A55	= 0x2,
	HOST2SYNA_SENDER_WLAN	= 0x5,
	HOST2SYNA_SENDER_BT		= 0x6,
}HOST2SYNA_SENDER_ID;

typedef enum {
	HOST2SYNA_CMD_SOC			= 0x0,
	HOST2SYNA_CMD_SM			= 0x1,
	HOST2SYNA_CMD_KLAMATH		= 0x2,
	HOST2SYNA_CMD_WIRELESS		= 0x3,
}HOST2SYNA_CMD_CATGORY;


typedef enum {
	SPI_PI_QUERY_SPK_VERSION	= 0x0,
	SPI_PI_VERIFY_SIGNATURE 	= 0x1,
	SPI_PI_DERIVE_KEY	= 0x2,
	SPI_PI_OTP_ACCESS	= 0x5,
}HOST2SYNA_SOC_CMD_ID;

typedef enum {
	SP_PI_QUERY_BOOT_CONTEXT	= 0x0,
}HOST2SYNA_SM_CMD_ID;

typedef enum {
	SP_PI_SECURE_SEPARATION_DONE	= 0x1,
}HOST2SYNA_KLAMATH_CMD_ID;

typedef enum {
	SP_PI_OTP_READ_WIRELESS		= 0x0,
	SP_PI_OTP_WRITE_WIRELESS	= 0x1,
}HOST2SYNA_WIRELESS_CMD_ID;


/*
 * Mailbox Command Format
 *	  [31:24]:	SENDER_ID
 *	  [23:16]:	NS 1:REE, 0: TEE
 *	  [15:8]:	CMD_CATGORY
 *	  [7:0]:	Command ID
 */
#define SP_PI_CMD(sendid, ns, catgory, cmdid)   (((sendid) << 24) | ((ns) << 16) | ((catgory) << 8) | (cmdid))

#define MAX_MAILBOX_DATA_LEN 	8
#define MAX_MAILBOX_PARAM_NUM 	(MAX_MAILBOX_DATA_LEN - 1)

typedef union {
	unsigned int cmd_val;
	struct {
		unsigned int cmd_id			:8;
		unsigned int cmd_catgory	:8;
		unsigned int ns				:8;
		unsigned int sender_id		:8;
	};
} cmd_t;

typedef struct mailbox_command {
	cmd_t command;
	unsigned int param[MAX_MAILBOX_PARAM_NUM];
	int param_num;
} mailbox_command_t;

typedef struct mailbox_rsp {
	unsigned int cmd_ret;
	unsigned int cmd_status[7];
} mailbox_rsp_t;

#define MAILBOX_NUM      2

#define MAILBOX_REE      1      // both secure and non-secure can access
#define MAILBOX_TEE      0      // only secure can access

#define SP_CMD_QUERY_SPK_VERSION          SP_PI_CMD(HOST2SYNA_SENDER_A55, MAILBOX_TEE, HOST2SYNA_CMD_SOC, SPI_PI_QUERY_SPK_VERSION)
#define SP_CMD_SECURE_SEPARATION_DONE     SP_PI_CMD(HOST2SYNA_SENDER_A55, MAILBOX_TEE, HOST2SYNA_CMD_KLAMATH, SP_PI_SECURE_SEPARATION_DONE)
#define SP_CMD_OTP_ACCESS                 SP_PI_CMD(HOST2SYNA_SENDER_A55, MAILBOX_TEE, HOST2SYNA_CMD_SOC, SPI_PI_OTP_ACCESS)

void mailbox_init(void);
int mailbox_cmd_handle(uint32_t cmd_id, uint32_t arg0, uint32_t arg1, uint32_t arg2,
		uint32_t arg3, uint32_t arg4 , uint32_t arg5, mailbox_rsp_t *rsp);


#endif // _SYNA_MAILBOX_

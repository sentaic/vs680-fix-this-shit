/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SYNA_MAILBOX_
#define _SYNA_MAILBOX_


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
/* SP_PI_VERIFY_SIGNATURE */
#define SP_CMD_VERIFY_SIGNATURE  		SP_PI_CMD(HOST2SYNA_SENDER_A55, MAILBOX_TEE, HOST2SYNA_CMD_SOC, SPI_PI_VERIFY_SIGNATURE)
#define PUB_KEY_ID_K0_SYNA              0x00000001
#define PUB_KEY_ID_K0_OEM               0x00000002
#define PUB_KEY_ID_K0_3RD               0x00000003
#define PUB_KEY_ID_K1_A                 0x00000004
#define PUB_KEY_ID_K1_B                 0x00000005
#define PUB_KEY_ID_K1_C                 0x00000006
#define PUB_KEY_ID_K1_D                 0x00000007
#define PUB_KEY_ID_K0_CONN_WLAN_KEY1    0x00000008
#define PUB_KEY_ID_K0_CONN_WLAN_KEY2    0x00000009
#define PUB_KEY_ID_K0_CONN_WLAN_KEY3    0x0000000A
#define PUB_KEY_ID_K0_CONN_WLAN_KEY4    0x0000000B
#define PUB_KEY_ID_K0_CONN_BT_KEY1      0x0000000C
#define PUB_KEY_ID_K0_CONN_BT_KEY2      0x0000000D
#define PUB_KEY_ID_K0_CONN_BT_KEY3      0x0000000E
#define PUB_KEY_ID_K0_CONN_BT_KEY4      0x0000000F
#define PUB_KEY_IN_MEM                  0x4B455949 /* in cmd param 6 */

#define ALG_ECC_P521_SHA512             0x00000001
#define ALG_ECC_P384_SHA384             0x00000002
#define ALG_ECC_P256_SHA256             0x00000003
#define ALG_RSA_4096_SHA384             0x00000004
#define ALG_ECC_3072_SHA256             0x00000005

/*------------------------------------------------------*/
/* SP_PI_DERIVE_KEY */
#define SP_CMD_DERIVE_KEY    SP_PI_CMD(HOST2SYNA_SENDER_A55, MAILBOX_TEE, HOST2SYNA_CMD_SOC, SPI_PI_DERIVE_KEY)
#define ROOT_KEY_ID_SCGK                0x00000001
#define ROOT_KEY_ID_SCUK                0x00000002
#define ROOT_KEY_ID_CCGK                0x00000003
#define ROOT_KEY_ID_CCUK                0x00000004
#define ROOT_KEY_ID_K0_CONN_WLAN_KEY1   0x00000005
#define ROOT_KEY_ID_K0_CONN_WLAN_KEY2   0x00000006
#define ROOT_KEY_ID_K0_CONN_BT_KEY1     0x00000007
#define ROOT_KEY_ID_K0_CONN_BT_KEY2     0x00000008

/* CMD_STATUS_0 of SP_PI_DERIVE_KEY */
#define STATUS_0_INVALID_CMD            0xFC000001
#define STATUS_0_INVALID_ROOT_KEY_ID    0xFC000002

TEE_Result mbox_send_command(mailbox_command_t *cmd, mailbox_rsp_t *rsp);
TEE_Result mailbox_derive_keys(unsigned int root_key_id,
                        unsigned char *context, unsigned int context_len,
                        unsigned char *label, unsigned int label_len,
                        unsigned char *key, unsigned int key_len_bits);
TEE_Result mailbox_verify_signature(unsigned int img_id, unsigned int alg,
                          paddr_t signature_pa, paddr_t hash_pa);

struct mcu2soc_map_regs {
	uint32_t start_org;
	uint32_t end_org;
	uint32_t off_org;
	uint32_t vaild;
	uint32_t slot_index;
};

int mcu2soc_maping_save(unsigned int mcu_addr, unsigned int size, unsigned int soc_addr, struct mcu2soc_map_regs *regs);
int mcu2soc_maping_restore(const struct mcu2soc_map_regs *regs);

TEE_Result plat_get_rpmb_huk(void);

#endif // _SYNA_MAILBOX_

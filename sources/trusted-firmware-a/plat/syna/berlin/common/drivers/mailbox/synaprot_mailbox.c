/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <assert.h>
#include "klamath_memmap.h"
#include "nexus_fw_include/mcu_cpu_ss.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include "nexus_fw_include/mcu_gbl_cfg.h"
#include "synaprot_fw_include/mailbox.h"
#include "synaprot_mailbox.h"

#define MCU_SYSCTRL_S_BASE_VA (MCU_PERIF_REGION(START_MCU_SYSCTRL_S))

#define MBOX_INTF_LOCK_CTRL (MCU_SYSCTRL_S_BASE_VA + RA_MCS_REG_SPROT_MBOX + RA_MCS_INTF_LOCK_INTF_LOCK_CTRL)
#define MBOX_INTF_LOCK_STAT (MCU_SYSCTRL_S_BASE_VA + RA_MCS_REG_SPROT_MBOX + RA_MCS_INTF_LOCK_INTF_LOCK_STAT)

#define TX_CTRL_STATUS_REG          RA_MailboxReg_H2S_ControlStatusRegister
#define TX_DATA_REG                 RA_MailboxReg_H2S_Data
#define TX_EMPTY_BIT_POS            LSb32MailboxReg_H2S_ControlStatusRegister_Empty

#define START_MCU_SPROT_MBOX_S_VA    MCU_DMA_CRYPT_REGION(START_MCU_SPROT_MBOX_S)
#define MID_A55 0x2

#define TIMEOUT_US                  (1*1000*1000) // 1s
static int g_tx_mode        = 0;                    // 0: polling mode, 1: interrupt mode
static int g_tx_status[]    = {0, 0};               // 0: write mode, 1: read mode

static const unsigned int mailbox_box_base_addr[] = // 1: REE, 0: TEE
{
    RA_MB_TEE_Mailbox,
    RA_MB_REE_Mailbox,
};

static int mbox_lock(void)
{
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	do {
		stat.u32 = mmio_read_32(MBOX_INTF_LOCK_STAT);

	} while(stat.uINTF_LOCK_STAT_locked);

	ctrl.uINTF_LOCK_CTRL_lock = 1;
	mmio_write_32(MBOX_INTF_LOCK_CTRL, ctrl.u32);

	stat.u32 = mmio_read_32(MBOX_INTF_LOCK_STAT);

	if (stat.uINTF_LOCK_STAT_locked && (stat.uINTF_LOCK_STAT_mid == MID_A55))
		return 0;

	/* FIXME: go back to loop and wait or return? */
	return -1;

}
static int32_t mbox_unlock(void)
{
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	stat.u32 = mmio_read_32(MBOX_INTF_LOCK_STAT);
	if (!stat.uINTF_LOCK_STAT_locked || (stat.uINTF_LOCK_STAT_mid != MID_A55))
		return -1;

	ctrl.u32 = 0;
	mmio_write_32(MBOX_INTF_LOCK_CTRL, ctrl.u32);
	return 0;
}

int mailbox_h2s_get_response(int mailbox_id, mailbox_rsp_t *rsp, int timeout)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
	unsigned int status;
	int i = 0;
	int len = -1;
	unsigned int *data = (unsigned int *)rsp;

	if (!data)
		return -1;

	do {
		status = mmio_read_32(base_address  + RA_MailboxReg_S2H_ControlStatusRegister_Host);
		timeout--;
		if (timeout < 0) {
			ERROR("Get response from synaprot timeout, status = 0x%x.\n", status);
			return -2;
		}
	} while (!(status & MSK32MailboxReg_S2H_ControlStatusRegister_Host_Full));

	len = status & MSK32MailboxReg_S2H_ControlStatusRegister_Host_Length;
	INFO("%s(), line %d: status = 0x%x.\n", __func__, __LINE__, status);

	for (i = 0; i <= len; i++) {
		data[i] = mmio_read_32(base_address +RA_MailboxReg_S2H_Data);
		INFO("%s(), line %d: data[%d] = 0x%x.\n", __func__, __LINE__, i, data[i]);
	}

	return 0;
}

static int mailbox_h2s_tx_wait_done(int mailbox_id, int timeout)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
    unsigned int status;

    do
    {
        if (g_tx_mode == 0)
        {
            status = mmio_read_32(base_address + TX_CTRL_STATUS_REG);
            if (status&(1<<TX_EMPTY_BIT_POS))   // read CSR will clear empty flag
            {
                mmio_write_32(base_address + TX_CTRL_STATUS_REG, 0);     // to clear interrupt
                g_tx_status[mailbox_id] = 0;
            }
        }

        if (g_tx_status[mailbox_id] == 0) // back to write mode
        {
            return 0;
        }

        udelay(1);
        timeout--;
    } while (timeout >= 0);

    WARN("wait tx done timeout, status:0x%08x, tx_status:%d\n",
    mmio_read_32(base_address + TX_CTRL_STATUS_REG), g_tx_status[mailbox_id]);

    return 0;
}

static int mailbox_h2s_raw_msg_send(int mailbox_id, unsigned int dwords[], int n)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
    int ret;
    int i;

    INFO("host send command to synaprot from %s mailbox\n", mailbox_id ? "REE":"TEE");

    mmio_write_32(base_address + TX_CTRL_STATUS_REG, n-1);

    g_tx_status[mailbox_id] = 1;  // set local flag first

    for (i = 0; i < n; i++) {
        mmio_write_32(base_address + TX_DATA_REG, dwords[i]);
		INFO("%s(), line %d: data = 0x%x.\n", __func__, __LINE__, dwords[i]);
	}
	INFO("%s(), line %d: status = 0x%x.\n", __func__, __LINE__, mmio_read_32(base_address + TX_CTRL_STATUS_REG));

    // wait tx data has been fetched by peer for each tx
    ret = mailbox_h2s_tx_wait_done(mailbox_id, TIMEOUT_US);

	if (ret != 0)
    {
        return ret;
    }
	INFO("%s(), line %d: status = 0x%x.\n", __func__, __LINE__, mmio_read_32(base_address + TX_CTRL_STATUS_REG));

    return 0;
}

int mailbox_h2s_send_command(mailbox_command_t *cmd)
{
	unsigned int data[MAX_MAILBOX_DATA_LEN];
	int i = 0;
	int ret;

	if (cmd->param_num > MAX_MAILBOX_PARAM_NUM)
		return -1;

	memset(data, 0, MAX_MAILBOX_DATA_LEN * 4);

	data[0] = cmd->command.cmd_val;
	for (i = 0; i < cmd->param_num; i++)
		data[i + 1] = cmd->param[i];

	ret = mailbox_h2s_raw_msg_send(cmd->command.ns, data, cmd->param_num + 1);

	return ret;
}

int mailbox_host_set_tx_mode(int mode) // 0: polling mode, 1: interrupt mode
{
    g_tx_mode = mode;
    return 0;
}

int mailbox_host_tx_init(int mailbox_id)
{
    g_tx_status[mailbox_id] = 0;

    return 0;
}

int mbox_send_command(mailbox_command_t *cmd, mailbox_rsp_t *rsp)
{
	int ret = -1;

	if (!cmd || !rsp)
		return -1;

	if (cmd->command.ns != MAILBOX_TEE && (cmd->command.sender_id != HOST2SYNA_SENDER_A55)) {
		ERROR("Sender id must A55 and TEE MAILBOX\n");
		return -2;
	}

	ret = mbox_lock();
	if (ret != 0) {
		ERROR("fail to get lock, busy!\n");
		return -1;
	}
	ret = mailbox_h2s_send_command(cmd);
	if (ret) {
		ERROR("fail to send command: 0x%x, ret = 0x%x.\n", cmd->command.cmd_id, ret);
		goto out;
	}

	ret = mailbox_h2s_get_response(MAILBOX_TEE, rsp, TIMEOUT_US);
	if (ret != 0)  {
		ERROR("Fail to get response from synaprot, ret = 0x%x.\n", ret);
		goto out;
	}

	ret = 0;

out:
	mbox_unlock();
	return ret;
}

static int mailbox_get_spk_version(unsigned int *version)
{
	mailbox_command_t cmd = {0};
	int ret = 0;
	mailbox_rsp_t rsp = {0};
	cmd.command.cmd_val = SP_CMD_QUERY_SPK_VERSION;

	ret = mbox_send_command(&cmd, &rsp);
	if (ret || rsp.cmd_ret) {
		ERROR("fail to send command: cmd_id = 0x%x, catgory =0x%x, ret = 0x%x, cmd_ret =0x%x.\n",
				cmd.command.cmd_id, cmd.command.cmd_catgory, ret, rsp.cmd_ret);
		return ret;
	}

	if (version)
		*version = rsp.cmd_status[0];

	return 0;
}

int seclib_security_separation_done(void)
{
	mailbox_command_t cmd = {0};
	mailbox_rsp_t rsp = {0};

	cmd.command.cmd_val = SP_CMD_SECURE_SEPARATION_DONE;

	return mbox_send_command(&cmd, &rsp);
}

void mailbox_init(void)
{
	unsigned int version = 0;
	INFO("mailbox enter.\n");
	mailbox_host_tx_init(MAILBOX_TEE);
	mailbox_host_set_tx_mode(0);

	mailbox_get_spk_version(&version);

	if (seclib_security_separation_done())
		 ERROR("fail to open JTAG\n");

	INFO("mailbox init done.\n");
}

static bool mailbox_sip_cmd_is_supported(uint32_t cmd_id)
{
	switch (cmd_id) {
		case SP_CMD_QUERY_SPK_VERSION:
		case SP_CMD_OTP_ACCESS:
			return true;
		default:
			return false;
	}
}

int mailbox_cmd_handle(uint32_t cmd_id, uint32_t arg0, uint32_t arg1, uint32_t arg2,
					uint32_t arg3, uint32_t arg4 , uint32_t arg5, mailbox_rsp_t *rsp)
{
	mailbox_command_t cmd = {0};

	if (!mailbox_sip_cmd_is_supported(cmd_id))
		return -1;

	cmd.command.cmd_val = cmd_id;
	cmd.param[0] = arg0;
	cmd.param[1] = arg1;
	cmd.param[2] = arg2;
	cmd.param[3] = arg3;
	cmd.param[4] = arg4;
	cmd.param[5] = arg5;

	cmd.param_num = 6;

	return mbox_send_command(&cmd, rsp);
}

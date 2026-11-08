/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "plat_def.h"
#include <io.h>
#include <util.h>
#include <types_ext.h>
#include <tee_api_types.h>
#include <string.h>
#include <stdint.h>
#include <mm/core_memprot.h>
#include <initcall.h>
#include <tee/cache.h>
#include <crypto/crypto.h>
#include <kernel/spinlock.h>

#include "klamath_memmap.h"
#include "synaprot_fw_include/mailbox.h"
#include "synaprot_mailbox.h"
#include "nexus_fw_include/mcu_cpu_ss.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include "nexus_fw_include/mcu_gbl_cfg.h"
#include "appcrypto_lock.h"
#include "syna_setup.h"

#define MCU2SOC_MAPPING_SLOT_START 0

#define MCU2SOC_MAPPING_SLOT_MAX  4

static struct mutex mailbox_mutex = MUTEX_INITIALIZER;

static unsigned int mcu2soc_mapping_global_lock __nex_bss = SPINLOCK_UNLOCK;

struct mcu2soc_mapping_slot {
	struct mutex slot_mutex;
	int used;
	int slot_index;
};

struct mcu2soc_mapping_slot mcu2soc_map_slots[MCU2SOC_MAPPING_SLOT_MAX];

#define MCU_SECCTRL_S_BASE	(vaddr_t)phys_to_virt_io(MCU_PERIF_REGION(START_MCU_SECCTRL_S), SIZE_MCU_SECCTRL_S)

#define MCU_SYSCTRL_S_BASE_VA (vaddr_t)phys_to_virt_io(MCU_PERIF_REGION(START_MCU_SYSCTRL_S), SIZE_MCU_SYSCTRL_S)

#define MBOX_INTF_LOCK_CTRL (MCU_SYSCTRL_S_BASE_VA + RA_MCS_REG_SPROT_MBOX + RA_MCS_INTF_LOCK_INTF_LOCK_CTRL)
#define MBOX_INTF_LOCK_STAT (MCU_SYSCTRL_S_BASE_VA + RA_MCS_REG_SPROT_MBOX + RA_MCS_INTF_LOCK_INTF_LOCK_STAT)

#define MID_A55 0x2


#define TX_CTRL_STATUS_REG          RA_MailboxReg_H2S_ControlStatusRegister
#define TX_DATA_REG                 RA_MailboxReg_H2S_Data
#define TX_EMPTY_BIT_POS            LSb32MailboxReg_H2S_ControlStatusRegister_Empty

#define START_MCU_SPROT_MBOX_S_VA      (vaddr_t)phys_to_virt_io(0xE3014000, 0x1000)
//#define START_MCU_SPROT_MBOX_S_VA      (vaddr_t)phys_to_virt_io(0xE6014000, 0x1000)

#define TIMEOUT_US                  (1*1000*1000) // 1s
static int g_tx_mode        = 0;                    // 0: polling mode, 1: interrupt mode
static int g_tx_status[]    = {0, 0};               // 0: write mode, 1: read mode

static const unsigned int mailbox_box_base_addr[] = // 1: REE, 0: TEE
{
    RA_MB_TEE_Mailbox,
    RA_MB_REE_Mailbox,
};

static TEE_Result mailbox_h2s_get_response(int mailbox_id, mailbox_rsp_t *rsp, int timeout)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
	unsigned int status;
	int i = 0;
	int len = -1;
	unsigned int *data = (unsigned int *)rsp;

	if (!data)
		return TEE_ERROR_BAD_PARAMETERS;

	do {
		status = io_read32(base_address  + RA_MailboxReg_S2H_ControlStatusRegister_Host);
		timeout--;
		if (timeout < 0) {
			EMSG("Get response from synaprot timeout, status = 0x%x.\n", status);
			return TEE_ERROR_TIMEOUT;
		}

		udelay(1);
	} while (!(status & MSK32MailboxReg_S2H_ControlStatusRegister_Host_Full));

	len = status & MSK32MailboxReg_S2H_ControlStatusRegister_Host_Length;
	DMSG("%s(), line %d: status = 0x%x.\n", __func__, __LINE__, status);

	for (i = 0; i <= len; i++) {
		data[i] = io_read32(base_address +RA_MailboxReg_S2H_Data);
		DMSG("%s(), line %d: data[%d] = 0x%x.\n", __func__, __LINE__, i, data[i]);
	}

	return TEE_SUCCESS;
}

static TEE_Result mailbox_h2s_tx_wait_done(int mailbox_id, int timeout)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
    unsigned int status;

    do
    {
        if (g_tx_mode == 0)
        {
            status = io_read32(base_address + TX_CTRL_STATUS_REG);
            if (status&(1<<TX_EMPTY_BIT_POS))   // read CSR will clear empty flag
            {
                io_write32(base_address + TX_CTRL_STATUS_REG, 0);     // to clear interrupt
                g_tx_status[mailbox_id] = 0;
            }
        }

        if (g_tx_status[mailbox_id] == 0) // back to write mode
        {
            return TEE_SUCCESS;
        }

        udelay(1);
        timeout--;
    } while (timeout >= 0);

    EMSG("wait tx done timeout, status:0x%08x, tx_status:%d\n",
    io_read32(base_address + TX_CTRL_STATUS_REG), g_tx_status[mailbox_id]);

    return TEE_ERROR_TIMEOUT;
}

static TEE_Result mailbox_h2s_raw_msg_send(int mailbox_id, unsigned int dwords[], int n)
{
    unsigned int base_address = START_MCU_SPROT_MBOX_S_VA + mailbox_box_base_addr[mailbox_id];
    TEE_Result ret = TEE_ERROR_GENERIC;
    int i;

    DMSG("host send command to synaprot from %s mailbox\n", mailbox_id ? "REE":"TEE");

    io_write32(base_address + TX_CTRL_STATUS_REG, n-1);

    g_tx_status[mailbox_id] = 1;  // set local flag first

    for (i = 0; i < n; i++) {
        io_write32(base_address + TX_DATA_REG, dwords[i]);
		DMSG("%s(), line %d: data = 0x%x.\n", __func__, __LINE__, dwords[i]);
	}
	DMSG("%s(), line %d: status = 0x%x.\n", __func__, __LINE__, io_read32(base_address + TX_CTRL_STATUS_REG));

    // wait tx data has been fetched by peer for each tx
    ret = mailbox_h2s_tx_wait_done(mailbox_id, TIMEOUT_US);

    return ret;
}

static int mailbox_h2s_send_command(mailbox_command_t *cmd)
{
	unsigned int data[MAX_MAILBOX_DATA_LEN];
	int i = 0;
	int ret;

	if (!cmd || cmd->param_num > MAX_MAILBOX_PARAM_NUM)
		return -1;

	memset(data, 0, MAX_MAILBOX_DATA_LEN * 4);

	data[0] = cmd->command.cmd_val;
	for (i = 0; i < cmd->param_num; i++)
		data[i + 1] = cmd->param[i];

	ret = mailbox_h2s_raw_msg_send(cmd->command.ns, data, cmd->param_num + 1);

	return ret;
}

static int mailbox_host_set_tx_mode(int mode) // 0: polling mode, 1: interrupt mode
{
    g_tx_mode = mode;
    return 0;
}

static int mailbox_host_tx_init(int mailbox_id)
{
    g_tx_status[mailbox_id] = 0;

    return 0;
}

static TEE_Result mbox_lock(void)
{
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	do {
		stat.u32 = io_read32(MBOX_INTF_LOCK_STAT);

	} while(stat.uINTF_LOCK_STAT_locked);

	ctrl.uINTF_LOCK_CTRL_lock = 1;
	io_write32(MBOX_INTF_LOCK_CTRL, ctrl.u32);

	stat.u32 = io_read32(MBOX_INTF_LOCK_STAT);

	if (stat.uINTF_LOCK_STAT_locked && (stat.uINTF_LOCK_STAT_mid == MID_A55))
		return TEE_SUCCESS;

	/* FIXME: go back to loop and wait or return? */
	return TEE_ERROR_BUSY;

}
static int32_t mbox_unlock(void)
{
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	stat.u32 = io_read32(MBOX_INTF_LOCK_STAT);
	if (!stat.uINTF_LOCK_STAT_locked || (stat.uINTF_LOCK_STAT_mid != MID_A55))
		return -1;

	ctrl.u32 = 0;
	io_write32(MBOX_INTF_LOCK_CTRL, ctrl.u32);
	return 0;
}

TEE_Result mbox_send_command(mailbox_command_t *cmd, mailbox_rsp_t *rsp)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	bool in_thread_ctx = thread_get_id_may_fail() != THREAD_ID_INVALID;

	if (!cmd && !rsp)
		return TEE_ERROR_BAD_PARAMETERS;

	if (cmd->command.ns != MAILBOX_TEE && (cmd->command.sender_id != HOST2SYNA_SENDER_A55)) {
		EMSG("Sender id must A55 and TEE MAILBOX\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	if (in_thread_ctx)
		mutex_lock(&mailbox_mutex);

	ret = mbox_lock();
	if (ret != TEE_SUCCESS) {
		if (in_thread_ctx)
			mutex_unlock(&mailbox_mutex);
		return TEE_ERROR_ACCESS_DENIED;
	}
	ret = mailbox_h2s_send_command(cmd);
	if (ret) {
		DMSG("fail to send command: 0x%x, ret = 0x%x.\n", cmd->command.cmd_id, ret);
		goto out;
	}

	ret = mailbox_h2s_get_response(MAILBOX_TEE, rsp, TIMEOUT_US);
	if (ret != TEE_SUCCESS)  {
		EMSG("Fail to get response from synaprot, ret = 0x%x.\n", ret);
		goto out;
	}

	ret = TEE_SUCCESS;

out:
	mbox_unlock();
	if (in_thread_ctx)
		mutex_unlock(&mailbox_mutex);
	return ret;
}

static TEE_Result mailbox_get_spk_version(unsigned int *version)
{
	mailbox_command_t cmd = {0};
	int ret = 0;
	mailbox_rsp_t rsp = {0};

	cmd.command.cmd_id = SPI_PI_QUERY_SPK_VERSION;
	cmd.command.cmd_catgory = HOST2SYNA_CMD_SOC;
	cmd.command.ns = MAILBOX_TEE;
	cmd.command.sender_id = HOST2SYNA_SENDER_A55;
	cmd.param_num = 0;

	ret = mbox_send_command(&cmd, &rsp);
	if (ret || rsp.cmd_ret) {
		EMSG("fail to send command: cmd_id = 0x%x, catgory =0x%x, ret = 0x%x, cmd_ret =0x%x.\n",
				cmd.command.cmd_id, cmd.command.cmd_catgory, ret, rsp.cmd_ret);
		return ret;
	}

	if (version)
		*version = rsp.cmd_status[0];

	return TEE_SUCCESS;
}

static TEE_Result mailbox_separation_done(void)
{
	mailbox_command_t cmd = {0};
	int ret = 0;
	mailbox_rsp_t rsp = {0};

	cmd.command.cmd_val = SP_CMD_SECURE_SEPARATION_DONE;
	cmd.param_num = 0;

	ret = mbox_send_command(&cmd, &rsp);
	if (ret || rsp.cmd_ret) {
		EMSG("fail to send command: cmd_id = 0x%x, catgory =0x%x, ret = 0x%x, cmd_ret =0x%x.\n",
				cmd.command.cmd_id, cmd.command.cmd_catgory, ret, rsp.cmd_ret);
		return ret;
	}

	return TEE_SUCCESS;
}


static int find_vaild_slot(void)
{
	int i = 0;
	uint32_t exceptions = 0;

	exceptions = cpu_spin_lock_xsave(&mcu2soc_mapping_global_lock);

	for (i = MCU2SOC_MAPPING_SLOT_START; i < MCU2SOC_MAPPING_SLOT_MAX; i++) {
		if (!mcu2soc_map_slots[i].used) {
			mcu2soc_map_slots[i].used = 1;
			mcu2soc_map_slots[i].slot_index = i;
			break;
		}
	}
	cpu_spin_unlock_xrestore(&mcu2soc_mapping_global_lock, exceptions);

	return i;
}

static void release_slot(int slot)
{
	int i = slot;
	uint32_t exceptions = 0;

	if (slot < MCU2SOC_MAPPING_SLOT_START || slot >= MCU2SOC_MAPPING_SLOT_MAX) {
	    EMSG("Invalid slot number %d\n", slot);
	    assert(0);
	}

	if (!mcu2soc_map_slots[i].used)
		assert(0);


	exceptions = cpu_spin_lock_xsave(&mcu2soc_mapping_global_lock);

	mcu2soc_map_slots[i].used = 0;

	cpu_spin_unlock_xrestore(&mcu2soc_mapping_global_lock, exceptions);

}

int mcu2soc_maping_restore(const struct mcu2soc_map_regs *regs)
{
	unsigned int base;
	int slot;
	bool in_thread_ctx = thread_get_id_may_fail() != THREAD_ID_INVALID;

	if (!regs)
		assert(0);

	slot = regs->slot_index;

	if (slot < MCU2SOC_MAPPING_SLOT_START || slot >= MCU2SOC_MAPPING_SLOT_MAX) {
	    EMSG("Invalid slot number %d\n", slot);
	    assert(0);
	}

	base= MCU_SECCTRL_S_BASE + RA_MCS_SEC_REG_SOC_INTF_REGION0 + slot * 0x10;


	io_write32(base + RA_MCS_SOC_INTF_REGION_START,  regs->start_org);
	io_write32(base + RA_MCS_SOC_INTF_REGION_END,    regs->end_org);
	io_write32(base + RA_MCS_SOC_INTF_REGION_OFFSET, regs->off_org);
	io_write32(base + RA_MCS_SOC_INTF_REGION_VALID,  regs->vaild);

	if (in_thread_ctx)
		mutex_unlock(&mcu2soc_map_slots[slot].slot_mutex);

	release_slot(slot);

	return 0;
}

int mcu2soc_maping_save(unsigned int mcu_addr, unsigned int size, unsigned int soc_addr, struct mcu2soc_map_regs *regs)
{
	unsigned int base;
	uint32_t start, end, off;
	int slot;
	bool in_thread_ctx = thread_get_id_may_fail() != THREAD_ID_INVALID;

	if (!regs)
		assert(0);

	slot = find_vaild_slot();

	if (slot < MCU2SOC_MAPPING_SLOT_START || slot >= MCU2SOC_MAPPING_SLOT_MAX) {
	    EMSG("Invalid slot number %d\n", slot);
	    assert(0);
	}

	if (!size ||  size & 0xFFFFF || !mcu_addr || mcu_addr & 0xFFFFF){
	    EMSG("invaild mcu_addr 0x%x, size 0x%x\n", mcu_addr, size);
	    assert(0);
	}
	regs->slot_index = slot;

	start = mcu_addr;
	end = start + size -1;

    DMSG("map 0x%x to mcu [0x%08x 0x%08x]\n", soc_addr, start, end);

	base = MCU_SECCTRL_S_BASE + RA_MCS_SEC_REG_SOC_INTF_REGION0 + slot * 0x10;

	if (in_thread_ctx)
		mutex_lock(&mcu2soc_map_slots[slot].slot_mutex);

	regs->start_org = io_read32(base + RA_MCS_SOC_INTF_REGION_START);
	regs->end_org = io_read32(base + RA_MCS_SOC_INTF_REGION_END);
	regs->off_org = io_read32(base + RA_MCS_SOC_INTF_REGION_OFFSET);
	regs->vaild = io_read32(base + RA_MCS_SOC_INTF_REGION_VALID);

	start >>= 20;
	end   >>= 20;
	off = (0x1000 + (soc_addr >> 20) - start) & 0xfff;

	io_write32(base + RA_MCS_SOC_INTF_REGION_START,  start);
	io_write32(base + RA_MCS_SOC_INTF_REGION_END,    end);
	io_write32(base + RA_MCS_SOC_INTF_REGION_OFFSET, off);
	io_write32(base + RA_MCS_SOC_INTF_REGION_VALID,  1);
	return 0;
}

TEE_Result mailbox_verify_signature(unsigned int img_id, unsigned int alg,
                          paddr_t signature_pa, paddr_t hash_pa)
{
	mailbox_command_t cmd = {0};
	mailbox_rsp_t rsp = {0};
	TEE_Result ret = TEE_ERROR_GENERIC;
	uint32_t mcu_map_size = 0x200000;
	uint32_t signature_mcu_pa, hash_mcu_pa;
	uint32_t signature_offset, hash_offset;
	struct mcu2soc_map_regs regs1 = {0};
	struct mcu2soc_map_regs regs2 = {0};

	if (!signature_pa || !hash_pa) {
		return TEE_ERROR_BAD_PARAMETERS;
	}
	signature_offset = signature_pa & 0xFFFFF;
	hash_offset = hash_pa & 0xFFFFF;

	signature_mcu_pa = START_MCU_SOC1_S;
	hash_mcu_pa = signature_mcu_pa + mcu_map_size;

	mcu2soc_maping_save(signature_mcu_pa, mcu_map_size, signature_pa, &regs1);
	mcu2soc_maping_save(hash_mcu_pa, mcu_map_size, hash_pa, &regs2);

	cmd.command.cmd_val = SP_CMD_VERIFY_SIGNATURE;

	cmd.param[0] = img_id;
	cmd.param[1] = alg;
	cmd.param[2] = signature_mcu_pa + signature_offset;
	cmd.param[3] = hash_mcu_pa + hash_offset;

	cmd.param_num = 4;
	ret = mbox_send_command(&cmd, &rsp);

	if (ret || rsp.cmd_ret) {
		EMSG("fail to send command: cmd_id = 0x%x, catgory =0x%x, ret = 0x%x, cmd_ret =0x%x.\n",
				cmd.command.cmd_id, cmd.command.cmd_catgory, ret, rsp.cmd_ret);
		goto out;
	}
out:
	mcu2soc_maping_restore(&regs1);
	mcu2soc_maping_restore(&regs2);
	return ret;
}

TEE_Result mailbox_derive_keys(unsigned int root_key_id,
                        unsigned char *context, unsigned int context_len,
                        unsigned char *label, unsigned int label_len,
                        unsigned char *key, unsigned int key_len_bits)
{
	mailbox_command_t cmd = {0};
	mailbox_rsp_t rsp = {0};
	TEE_Result ret = TEE_ERROR_GENERIC;
	uint32_t mcu_map_size = 0x200000;
	uint32_t mcu_context_pa, mcu_lable_pa, mcu_key_pa;
	uint32_t context_pa ,context_offset;
	uint32_t label_pa, label_offset;
	uint32_t key_pa, key_offset;
	struct mcu2soc_map_regs regs1 = {0};
	struct mcu2soc_map_regs regs2 = {0};
	struct mcu2soc_map_regs regs3 = {0};

	if (!context || !label || !key) {
		return TEE_ERROR_BAD_PARAMETERS;
	}
	context_pa = virt_to_phys(context);
	label_pa = virt_to_phys(label);
	key_pa = virt_to_phys(key);

	if (!context_pa || !label_pa || !key_pa) {
		EMSG("Invaild context @ 0x%p(0x%x), label @ 0x%p(0x%x) or key @0x%p(0x%x)",
				context, context_pa,
				label, label_pa, key, key_pa);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	mcu_context_pa = START_MCU_SOC0_S;
	mcu_lable_pa = mcu_context_pa + mcu_map_size;
	mcu_key_pa = mcu_lable_pa + mcu_map_size;

	mcu2soc_maping_save(mcu_context_pa, mcu_map_size, context_pa, &regs1);
	mcu2soc_maping_save(mcu_lable_pa, mcu_map_size, label_pa, &regs2);
	mcu2soc_maping_save(mcu_key_pa, mcu_map_size, key_pa, &regs3);

	cache_operation(TEE_CACHEFLUSH, context, context_len);
	cache_operation(TEE_CACHEFLUSH, label, label_len);
	cache_operation(TEE_CACHEFLUSH, key, key_len_bits/8);

	context_offset = context_pa & 0xFFFFF;
	label_offset = label_pa & 0xFFFFF;
	key_offset = key_pa & 0xFFFFF;

	cmd.command.cmd_val = SP_CMD_DERIVE_KEY;
	cmd.param[0] = root_key_id;
	cmd.param[1] = mcu_context_pa + context_offset;
	cmd.param[2] = context_len;
	cmd.param[3] = mcu_lable_pa + label_offset;
	cmd.param[4] = label_len;
	cmd.param[5] = key_len_bits;
	cmd.param[6] = mcu_key_pa + key_offset;

	cmd.param_num = 7;

	ret = mbox_send_command(&cmd, &rsp);

	cache_operation(TEE_CACHEINVALIDATE, key, key_len_bits/8);

	if (ret || rsp.cmd_ret) {
		EMSG("fail to send command: cmd_id = 0x%x, catgory =0x%x, ret = 0x%x, cmd_ret =0x%x.\n",
				cmd.command.cmd_id, cmd.command.cmd_catgory, ret, rsp.cmd_ret);
		goto out;
	}

out:
	mcu2soc_maping_restore(&regs1);
	mcu2soc_maping_restore(&regs2);
	mcu2soc_maping_restore(&regs3);
	return ret;
}

 TEE_Result syna_get_hw_unique_key(size_t key_len, uint8_t *key)
 {
	TEE_Result ret = TEE_ERROR_GENERIC;
	uint8_t context[] __aligned(CACHELINE_LEN) = "SYNA-RPMB";
	uint32_t label[4] __aligned(CACHELINE_LEN);
	uint8_t skey[32] __aligned(CACHELINE_LEN) = {0};

	memset(label, 0, sizeof(label));

	if(!key || !key_len || key_len > sizeof(skey))
		return TEE_ERROR_BAD_PARAMETERS;

	ret = mailbox_derive_keys(ROOT_KEY_ID_SCUK,
	                        context, sizeof(context) - 1,
	                        (uint8_t *)label, sizeof(label),
	                        skey, sizeof(skey) *8);
	if (ret != TEE_SUCCESS) {
		EMSG("Fail to derive key, ret = 0x%x.\n", ret);
		return ret;
	}

	memcpy(key, skey, key_len);
	memset(skey, 0, sizeof(skey));

	return TEE_SUCCESS;
}

static TEE_Result mailbox_init(void)
{
	unsigned int version = 0;
	int i;
	mailbox_host_tx_init(MAILBOX_TEE);
	mailbox_host_set_tx_mode(0);

	mailbox_get_spk_version(&version);

	IMSG("SPK version = 0x%x.\n", version);

	mailbox_separation_done();

	for (i = MCU2SOC_MAPPING_SLOT_START; i < MCU2SOC_MAPPING_SLOT_MAX; i++) {
		mutex_init(&mcu2soc_map_slots[i].slot_mutex);
	}

	return TEE_SUCCESS;
}

driver_init(mailbox_init);

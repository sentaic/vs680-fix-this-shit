/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <assert.h>
#include "tz_boot_param.h"
#include "bcm_hal.h"
#include "bcm_reg.h"
#include "bcm.h"

#define BCM_IMAGE_VERIFY			0x004E
#define BCM_GET_RANDOM				0x4003
#define BCM_EROM_LOAD_USB_IMAGE			0x0006
#define BCM_OTP_PROGRAM_OTP			0x5000
#define BCM_OTP_RD_PUBLIC_OTP		0x500A
#define BCM_OTP_UTILS_OTP_READ     	0x500D
#define BCM_OTP_UTILS_OTP_WRITE     0x500E
#define BCM_ACCESS_SRAM             0xD004

#define BCM_EROM_CFG_DDR_SCRAMBLE_KEY		0x0000
#define BCM_EROM_LOAD_MINILOADER		0x0001
#define BCM_EROM_CLEAN_UP			0x0002
#define BCM_EROM_GET_PUB_INO			0x0003
#define BCM_EROM_CHECK_DDR_HMAC			0x0004
#define BCM_EROM_SOC_IMAGE_VERIFY		0x0005
#define BCM_EROM_LOAD_USB_IMAGE			0x0006
#define BCM_EROM_TURN_OFF_USB_DEVICE		0x0007
#define BCM_EROM_GEN_DDR_SCRAMBLE_KEY		0x0008
#define BCM_EROM_QUERY_TEE_CFG			0x0009

#ifndef BCM_KERNEL_STORE_HASH
#define BCM_KERNEL_STORE_HASH			0xC014
#endif

#ifndef BCM_KERNEL_CALCULATE_STORE_HMAC
#define BCM_KERNEL_CALCULATE_STORE_HMAC	0x32
#endif

#define BCM_KERNEL_MAGIC_REG			0xF7FD0000
#define BCM_KERNEL_MAGIC_LOAD			0xCA5335AC
#define BCM_KERNEL_MAGIC_READY			0xDEADBEEF

#define PUBLIC_OTP_SIZE					(256)

void bcm_suspend_init(void)
{
}

void bcm_suspend(void)
{
}

void bcm_resume(void)
{
}

int bcm_erom_check_ddr_hmac(struct ddr_reg_info *ddr_reg)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (int)(unsigned long)ddr_reg;

	cmd.command = BCM_EROM_CHECK_DDR_HMAC;

	status = bcm_exec_func(&cmd);

	return status;

}
int bcm_calculate_and_store_hmac(struct ddr_reg_info *ddr_reg, unsigned int ddr_reg_num)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (int)(unsigned long)ddr_reg;
	cmd.arg1 = ddr_reg_num;

	cmd.command = BCM_KERNEL_CALCULATE_STORE_HMAC;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_kernel_genx_store_hash(void *hash_buff, unsigned int size)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (int)(unsigned long)hash_buff;
	cmd.arg1 = (int)(unsigned long)size;

	cmd.command = BCM_KERNEL_STORE_HASH;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_kernel_store_hash(struct ddr_reg_info *ddr_reg,
				unsigned int *tee_sha_buf,
				unsigned char *tee_buf,
				unsigned int tee_size)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (int)(unsigned long)ddr_reg;
	cmd.arg1 = (int)(unsigned long)tee_sha_buf;
	cmd.arg2 = (int)(unsigned long)tee_buf;
	cmd.arg3 = tee_size;

	cmd.command = BCM_KERNEL_STORE_HASH;

	status = bcm_exec_func(&cmd);

	return status;
}
int bcm_verify(unsigned int type, unsigned long src, unsigned long dst)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = type;
	cmd.arg1 = src;
	cmd.arg2 = dst;
	cmd.command = BCM_IMAGE_VERIFY;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_verify_miniloader(unsigned long src, unsigned int size)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (unsigned int)src;
	cmd.arg1 = size;
	cmd.command = BCM_EROM_LOAD_MINILOADER;

	status = bcm_exec_func(&cmd);

	return status;

}
int bcm_erom_cfg_ddr_scramble_key(void)
{

	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.command = BCM_EROM_CFG_DDR_SCRAMBLE_KEY;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_clean_up(void)
{

	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.command = BCM_EROM_CLEAN_UP;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_query_tee_config(void* tee_buf_out, int size,
			void *scs_pub_info,int *warmboot)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (unsigned int)(unsigned long)tee_buf_out;
	cmd.arg1 = size;
	cmd.arg2 = (unsigned int)(unsigned long)scs_pub_info;
	cmd.arg3 = (unsigned int)(unsigned long)warmboot;
	cmd.command = BCM_EROM_QUERY_TEE_CFG;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_load_bcm_kernel(void)
{
	volatile unsigned int *magic_reg = (unsigned int*)BCM_KERNEL_MAGIC_REG;

	*magic_reg = BCM_KERNEL_MAGIC_LOAD;

	while (1) {
		if (*magic_reg == BCM_KERNEL_MAGIC_READY)
			break;
	}

	return 0;
}

void bcm_parse_mem_region(void* scs_info, void *mem_region_info)
{
	int i;
	struct scs_pub_info *scs = (struct scs_pub_info*)scs_info;
	struct tz_verify_region *region = mem_region_info;

	assert(region->nums < 4 && region->nums >= 0);

	/* add the linux kernel region */
	i = region->nums;
	region->regs[i].base = (void*)(unsigned long)(scs->scs_data.pub_info.kernel_base);
	region->regs[i].size = scs->scs_data.pub_info.kernel_length;
	if (region->regs[i].base != 0 && region->regs[i].size != 0)
		region->nums++;

	/*
	 * we can extend to add more region into for verify
	 */
}

int bcm_check_priviledge_mode(void *scs_info)
{
	struct scs_pub_info *scs = (struct scs_pub_info*)scs_info;

	return scs->scs_data.pub_info.previlege_mode & 0x1;

}

int bcm_hwrand(void *    rand_addr, unsigned int rand_len)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	flush_dcache_range((uintptr_t)rand_addr, rand_len);
	bcm_clear_command(&cmd);
	cmd.arg0 = (unsigned int)(unsigned long)rand_addr;
	cmd.arg1 = rand_len;
	cmd.command = BCM_GET_RANDOM;

	status = bcm_exec_func(&cmd);
	if (!status) {
		inv_dcache_range((uintptr_t)rand_addr, rand_len);
	}
	return status;
}

void hw_get_random_bytes(uint8_t *data, uint32_t size)
{
	if (bcm_hwrand(data, size))
		panic();
}

int bcm_erom_load_usb_image(uint64_t image_type, uint64_t dest_addr,
				uint64_t file_name_addr, uint64_t file_name_len)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (uint32_t)image_type;
	cmd.arg1 = (uint32_t)dest_addr;
	cmd.arg2 = (uint32_t)file_name_addr;
	cmd.arg3 = (uint32_t)file_name_len;

	cmd.command = BCM_EROM_LOAD_USB_IMAGE;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_program_otp(uint64_t pphy_otp_layout, uint64_t size_opt_layout)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;
	struct otp_srv_resp resp __aligned(64);
	uint32_t size_resp = sizeof(struct otp_srv_resp);

	memset(&resp, 0, size_resp);
	flush_dcache_range((uintptr_t)&resp, size_resp);

	bcm_clear_command(&cmd);
	cmd.arg0 = (uint32_t)pphy_otp_layout;
	cmd.arg1 = (uint32_t)size_opt_layout;
	cmd.arg2 = (uint32_t)(uintptr_t)virt_to_phy(&resp);
	cmd.arg3 = size_resp;

	cmd.command = BCM_OTP_PROGRAM_OTP;

	status = bcm_exec_func(&cmd);

	if (status) {
		inv_dcache_range((uintptr_t)&resp, size_resp);
		status = resp.bitmap;
	}

	return status;
}

int bcm_otp_read_otp(uint32_t index, uint32_t *pdata, uint32_t *pmask)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = index;
	cmd.arg1 = (uint32_t)(uintptr_t)pdata;
	cmd.arg2 = (uint32_t)(uintptr_t)pmask;

	cmd.command = BCM_OTP_UTILS_OTP_READ;

	status = bcm_exec_func(&cmd);

	return status;
}
int bcm_access_secure_sram(uint32_t type, uint32_t idx, uint64_t *pdata)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

    if(!pdata) return -1;

    bcm_clear_command(&cmd);

    cmd.arg0 = (uint32_t)type;
    cmd.arg1 = idx;
    /* 1. Set the physical address for BCM to write the result into */
    cmd.arg2 = (uint32_t)(uintptr_t)virt_to_phy(pdata);
    cmd.command = BCM_ACCESS_SRAM;

	/* 2. Flush D-cache: Ensure memory is ready before BCM hardware access */
	flush_dcache_range((uintptr_t)pdata, sizeof(uint64_t));
	/* 3. Execute Mailbox command: This triggers the BCM firmware logic */
	status = bcm_exec_func(&cmd);
	/* 4. Invalidate D-cache: Force CPU to read the fresh data written by BCM
          from physical RAM instead of stale local cache lines */
	inv_dcache_range((uintptr_t)pdata, sizeof(uint64_t));
	return status;
}
int bcm_otp_write_otp(uint32_t index, uint32_t data, uint32_t mask)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = index;
	cmd.arg1 = data;
	cmd.arg2 = mask;

	cmd.command = BCM_OTP_UTILS_OTP_WRITE;

	status = bcm_exec_func(&cmd);

	return status;
}

int bcm_erom_read_public_otp(uint64_t otp_buff, uint64_t size)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = (uint32_t)otp_buff;
	cmd.arg1 = (uint32_t)size;

	cmd.command = BCM_OTP_RD_PUBLIC_OTP;

	status = bcm_exec_func(&cmd);

	return status;
}

#pragma weak is_plat_support_bcm_cmd

bool is_plat_support_bcm_cmd(uint32_t cmd_id)
{
	return false;
}

bool bcm_sip_cmd_is_supported(uint32_t cmd_id)
{
	if (is_plat_support_bcm_cmd(cmd_id))
		return true;

    switch (cmd_id)
    {
        case BCM_GET_RANDOM:
        case BCM_EROM_LOAD_USB_IMAGE:
        case BCM_OTP_PROGRAM_OTP:
        case BCM_OTP_RD_PUBLIC_OTP:
        case BCM_OTP_UTILS_OTP_READ:
        case BCM_OTP_UTILS_OTP_WRITE:
        case BCM_ACCESS_SRAM:
            return true;

        default:
            return false;
    }
}

int bcm_cmd_handle(uint32_t cmd_id, uint32_t arg0, uint32_t arg1, uint32_t arg2,
					uint32_t arg3, uint32_t arg4 , uint32_t arg5)
{
	unsigned int status;
	BCM_MAILBOX_COMMAND cmd;

	bcm_clear_command(&cmd);
	cmd.arg0 = arg0;
	cmd.arg1 = arg1;
	cmd.arg2 = arg2;
	cmd.arg3 = arg3;
	cmd.arg4 = arg4;
	cmd.arg5 = arg5;

	cmd.command = cmd_id;
	status = bcm_exec_func(&cmd);

	return status;
}

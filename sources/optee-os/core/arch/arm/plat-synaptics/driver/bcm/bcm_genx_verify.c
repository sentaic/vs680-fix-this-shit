// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <types_ext.h>
#include <util.h>
#include <string.h>
#include <stdint.h>
#include <mm/core_memprot.h>
#include <tee/cache.h>
#include "bcmREG.h"
#include "bcmImg.h"
#include "bcm_mailbox.h"
#include "tz_boot_cmd.h"
#include "mr_setup.h"
#include "bcm_genx_verify.h"
#include "image_genx.h"

#ifdef CONFIG_ANTIROLLBACK

/* anti-rollback parameters */
static int otp_comm_ver = 0;
static int otp_cust_ver = 0;
static bool in_recovery = false;

void set_antirollback_param(int comm_ver, int cust_ver, bool recovery)
{
	otp_comm_ver = comm_ver;
	otp_cust_ver = cust_ver;
	in_recovery = recovery;
}

#endif /* CONFIG_ANTIROLLBACK */

enum {
	BCM_KERNEL_PI_VERIFY_TSP_FW = 0x10,
	BCM_KERNEL_PI_VERIFY_DSP_FW = 0x11,
	BCM_KERNEL_PI_VERIFY_GPU_FW = 0x12,
	BCM_KERNEL_PI_VERIFY_ATF = 0x13,
	BCM_KERNEL_PI_VERIFY_TZK = 0x14,
	BCM_KERNEL_PI_VERIFY_TZK_BOOT_PARAM = 0x15,
	BCM_KERNEL_PI_VERIFY_TZK_OEM_SETTING = 0x16,
	BCM_KERNEL_PI_VERIFY_TA = 0x17,
	BCM_KERNEL_PI_VERIFY_BOOTLOADER = 0x18,
	BCM_KERNEL_PI_VERIFY_SM_FW = 0x19,
	BCM_KERNEL_PI_VERIFY_AVB_KEYS = 0x1A,
	BCM_KERNEL_PI_VERIFY_LINUX = 0x1B,
	BCM_KERNEL_PI_VERIFY_FASTBOOT = 0x1C,
	BCM_KERNEL_PI_VERIFY_FASTLOGO = 0x20
};

unsigned int image_get_permissions(const void * image __unused)
{
	/* default full permission */
	return 0xffffffff;
}

#ifdef CONFIG_IMAGE_NO_ENCRYPTION
int verify_image(const void *src, unsigned int size, paddr_t dst,
		unsigned int *dst_size, unsigned int image_type, uint32_t multi_keys)
{
	memcpy(dst, src, size);
	*dst_size = size;
	return 0;
}
#else
extern unsigned long tz_get_boot_stage(void);

int sp_exec_cmd(const void *cmd, size_t cmd_len,
		const void *result, size_t result_len);

static int bcm_verify_ta(const void *src, unsigned int src_size , void *pta, unsigned int ta_size,
			void *pta_all_permission, unsigned int all_permission_size,
			void *pta_permission, unsigned int subset_permission_size)
{
	int res = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd;
	BCM_MAILBOX_STATUS status = {0};

	memset(&cmd, 0, sizeof(cmd));
	cmd.arg0 = (unsigned int)virt_to_phys((void*)src);
	cmd.arg1 = src_size;
	cmd.arg2 = (unsigned int)virt_to_phys(pta_all_permission);
	cmd.arg3 = (unsigned int)virt_to_phys(pta_permission);
	cmd.arg4 = (unsigned int)virt_to_phys(pta);
	cmd.command = BCM_KERNEL_PI_VERIFY_TA;

	/* flush dcache before verification */
	cache_operation(TEE_CACHEFLUSH, (void *)src, src_size);

	if (tz_get_boot_stage() != TZ_BOOT_STAGE_BOOTLOADER) {
		res = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND), &status, sizeof(BCM_MAILBOX_STATUS));
	} else {
		res = execute_bcm_command(&cmd, &status, 0);
	}

	if (res) {
		EMSG("cmd 0x%x error with res = 0x%x\n", cmd.command, res);
		return res;
	}
	/* invalidate dcache after verification */
	cache_operation(TEE_CACHEINVALIDATE, pta, ta_size);
	cache_operation(TEE_CACHEINVALIDATE, pta_all_permission, all_permission_size);
	cache_operation(TEE_CACHEINVALIDATE, pta_permission, subset_permission_size);

	return status.return_status;
}

static int bcm_verify_fw(unsigned int image_type, paddr_t src, unsigned int src_size)
{
	int res = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd;
	BCM_MAILBOX_STATUS status = {0};
	const struct mem_region *mr = NULL;
	uint32_t dst = 0;
	uint32_t dst_size = 0;

	memset(&cmd, 0, sizeof(cmd));

	switch (image_type) {
	case IMAGE_TYPE_TSP_FW:
		cmd.command = BCM_KERNEL_PI_VERIFY_TSP_FW;
		break;
	case IMAGE_TYPE_DSP_FW:
		mr = mr_find_region_by_name("dsp");
		if(!mr)
			return TEE_ERROR_ACCESS_DENIED;
		dst = mr->base;
		dst_size = mr->size;

		cmd.command = BCM_KERNEL_PI_VERIFY_DSP_FW;
		break;
	case IMAGE_TYPE_GPU_FW:
		mr = mr_find_region_by_name("gpu");
		if(!mr)
			return TEE_ERROR_ACCESS_DENIED;
		dst = mr->base;
		dst_size = mr->size;

		cmd.command = BCM_KERNEL_PI_VERIFY_GPU_FW;
		break;
	case IMAGE_TYPE_SM_FW:
		cmd.command = BCM_KERNEL_PI_VERIFY_SM_FW;
		break;
	default:
		EMSG("fw image_type 0x%x is not support.\n", image_type);
		return TEE_ERROR_BAD_PARAMETERS;
	}
	cmd.arg0 = (unsigned int)src;
	cmd.arg1 = src_size;
	cmd.arg2 = dst;
	cmd.arg3 = dst_size;

	if (tz_get_boot_stage() > TZ_BOOT_STAGE_BOOTLOADER) {
		res = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND), &status, sizeof(BCM_MAILBOX_STATUS));
	} else {
		res = execute_bcm_command(&cmd, &status, 0);
	}

	if (res) {
		EMSG("cmd 0x%x error with res = 0x%x\n", cmd.command, res);
		return res;
	}

	return status.return_status;

}

static int bcm_verify_oem(unsigned int image_type, paddr_t src, unsigned int src_size)
{
	int res = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd;
	BCM_MAILBOX_STATUS status = {0};

	memset(&cmd, 0, sizeof(cmd));

	switch (image_type) {
	case IMAGE_TYPE_BOOTLOADER:
		cmd.command = BCM_KERNEL_PI_VERIFY_BOOTLOADER;
		break;
	case IMAGE_TYPE_LINUX_KERNEL:
		cmd.command = BCM_KERNEL_PI_VERIFY_LINUX;
		break;
	case IMAGE_TYPE_FASTBOOT:
		cmd.command = BCM_KERNEL_PI_VERIFY_FASTBOOT;
		break;
	case IMAGE_TYPE_AVB_KEYS:
		cmd.command = BCM_KERNEL_PI_VERIFY_AVB_KEYS;
		break;
	case IMAGE_TYPE_FAST_LOGO:
		cmd.command = BCM_KERNEL_PI_VERIFY_FASTLOGO;
		break;
	default:
		EMSG("oem image_type 0x%x is not support.\n", image_type);
		return TEE_ERROR_BAD_PARAMETERS;
	}
	cmd.arg0 = (unsigned int)src;
	cmd.arg1 = src_size;

	if (tz_get_boot_stage() > TZ_BOOT_STAGE_BOOTLOADER) {
		res = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND), &status, sizeof(BCM_MAILBOX_STATUS));
	} else {
		res = execute_bcm_command(&cmd, &status, 0);
	}

	if (res) {
		EMSG("cmd 0x%x error with res = 0x%x\n", cmd.command, res);
		return res;
	}

	return status.return_status;

}

int verify_ta_image(const void *src, unsigned int src_size,
		void *pta, unsigned int ta_size,
		void *pta_all_permission, unsigned int all_permission_size,
		void *pta_permission, unsigned int subset_permission_size)

{
	const struct mem_region *mr = mr_find_region(virt_to_phys(pta), ta_size);

	if (!mr || !TZ_IS_NNA(MR_PERM(mr))) {
		EMSG("ta@0x%p size = 0x%x must secure\n", pta, ta_size);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	mr = mr_find_region(virt_to_phys(pta_all_permission), all_permission_size);
	if (!mr || !TZ_IS_NNA(MR_PERM(mr))) {
		EMSG("pta_all_permission@%p size = 0x%x must secure\n",
				pta_all_permission, all_permission_size);
		return TEE_ERROR_BAD_PARAMETERS;
	}
	mr = mr_find_region(virt_to_phys(pta_permission), subset_permission_size);
	if (!mr || !TZ_IS_NNA(MR_PERM(mr))) {
		EMSG("pta_permission@%p size = 0x%x must secure\n",
				pta_permission, subset_permission_size);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return bcm_verify_ta(src, src_size, pta, ta_size,
			pta_all_permission, all_permission_size,
			pta_permission, subset_permission_size);

}

int verify_image(paddr_t src, unsigned int size, paddr_t dst __unused,
		unsigned int *dst_size __unused, unsigned int image_type, unsigned int multi_keys __unused)
{
	int ret = TEE_ERROR_GENERIC;

	if (!src) {
		EMSG("src can not be NULL\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	switch (image_type) {
		case IMAGE_TYPE_OEM_START ... IMAGE_TYPE_OEM_END:
			ret = bcm_verify_oem(image_type, src, size);
			break;
		case IMAGE_TYPE_FW_START ... IMAGE_TYPE_FW_END:
			ret = bcm_verify_fw(image_type, src, size);
			break;
		default:
			EMSG("image_type 0x%x is not support.\n", image_type);
			return TEE_ERROR_BAD_PARAMETERS;
	}

	return ret;
}
#endif /* CONFIG_IMAGE_NO_ENCRYPTION */

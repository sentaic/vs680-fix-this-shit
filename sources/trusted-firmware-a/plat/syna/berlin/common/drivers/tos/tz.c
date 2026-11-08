/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <common/debug.h>
#include <assert.h>
#include "tz_boot_param.h"
#include "bcm.h"
#include "rf.h"
#include "crypto.h"

void tos_reset_secure_rf_param(uint32_t rf_id,
				uint32_t rf_attr)
{
	struct tz_boot_param *rf_param;
	struct rf_window *rf;

	rf_param = tz_boot_param_get(TZ_BOOT_PARAM_REGFIREWALL);
	if (rf_param) {
		assert((rf_id + 1) * sizeof(*rf) <=
				rf_param->size - sizeof(*rf_param));
		rf = (struct rf_window *)rf_param->data;
		rf[rf_id].attr = rf_attr;
	}
}

void tos_save_secure_boot_param(void *nw_param_base,
				void *nw_region_base,
				void *sw_param_base,
				unsigned int sw_param_size)
{
	int ret, offset = 0;
	struct tz_boot_param *boot_param;
	struct tz_boot_param_header header;
	struct tz_verify_region *verify_region;
	int verify_region_size = 0;
	unsigned char *tee_config = (unsigned char*)sw_param_base;

	if (nw_param_base == NULL ||
			sw_param_base == NULL ||
			sw_param_size <= 0)
		return;

	ret = tz_boot_param_init(nw_param_base);
	if(ret)
		panic();
	memset(tee_config, 0, sw_param_size);

	/* reserve the header space */
	offset += sizeof(header);
	assert(offset <= sw_param_size);

	/* fill the memory zone */
	boot_param = tz_boot_param_get(TZ_BOOT_PARAM_MEMZONE);
	if (boot_param) {
		assert(offset + boot_param->size <= sw_param_size);
		memcpy(tee_config + offset, boot_param, boot_param->size);
		offset += boot_param->size;
	}

	/* fill the register firewall */
	boot_param = tz_boot_param_get(TZ_BOOT_PARAM_REGFIREWALL);
	if (boot_param) {
		assert(offset + boot_param->size <= sw_param_size);
		memcpy(tee_config + offset, boot_param, boot_param->size);
		offset += boot_param->size;
	}

	/* fill the secure master */
	boot_param = tz_boot_param_get(TZ_BOOT_PARAM_MASTER);
	if (boot_param) {
		assert(offset + boot_param->size <= sw_param_size);
		memcpy(tee_config + offset, boot_param, boot_param->size);
		offset += boot_param->size;
	}

	/* fill mem region for SHA256, just for nagra*/
	verify_region = (struct tz_verify_region *)nw_region_base;
	if (verify_region) {
		verify_region_size = verify_region->nums * sizeof(struct region);
		assert(offset + sizeof(struct tz_boot_param) + verify_region_size <= sw_param_size);
		boot_param = (struct tz_boot_param*)(tee_config + offset);
		boot_param->id = TZ_BOOT_VERIFY_REGION;
		boot_param->size = sizeof(struct tz_boot_param) + verify_region_size;
		memcpy(tee_config + offset + sizeof(struct tz_boot_param),
				verify_region->regs, verify_region_size);
		offset += boot_param->size;
	}

	/* fill the header */
	header.size = offset;
	header.magic_num = TZ_BOOT_PARAM_MAGIC_NUM;
	header.version = TZ_BOOT_PARAM_VERSION ;
	memcpy(tee_config, &header, sizeof(header));
	assert(offset <= sw_param_size);
}

void tos_resume(void* param)
{
}

static int tos_compute_ddr_sha(void *param)
{
	int i, num;
	struct tz_boot_param *boot_param;
	struct region value[MAX_DDR_REG_NUM] = {0};

	if (!param)
		return -1;

	if (tz_boot_param_init(param))
		return -1;

	boot_param = tz_boot_param_get(TZ_BOOT_VERIFY_REGION);
	if (boot_param) {
		/* the region need sha256 verify on nagra */
		num = (boot_param->size - sizeof(struct tz_boot_param))
			/ sizeof(struct region);

		assert(num <= MAX_DDR_REG_NUM);
		memcpy(value, boot_param->data, num*sizeof(struct region));

#if defined(BCM_CALCULATE_HASH) && defined(CFG_SYNA_BCM)
		struct ddr_reg_info ddr_info[MAX_DDR_REG_NUM];

		// BCM to calculate the DDR hash value

		for (i = 0; i < num; i++) {
			INFO("region %d, base 0x%p, len %x\n", i, value[i].base,
					value[i].size);
			ddr_info[i].ddr_info_type = 2;
			ddr_info[i].ddr_info_data.ddr_reg.base = (uintptr_t)value[i].base;
			ddr_info[i].ddr_info_data.ddr_reg.size = value[i].size;
		}

		bcm_calculate_and_store_hash(ddr_info, num);

		INFO("#######done#######\n");
#else
		struct ddr_reg_info ddr_info __aligned(64);

		// ACPU to calculate the DDR hash value
		ddr_info.ddr_info_type = 1;

		for (i = 0; i < num; i++) {
			hash_state md;
			INFO("region %d, base %p, len %x\n", i, value[i].base,
					value[i].size);
			sha256_init(&md);
			sha256_process(&md, (void *)(uintptr_t)(value[i].base), value[i].size);
			sha256_done(&md, (unsigned char*)ddr_info.ddr_info_data.hash_value + i * HASH_LEN);
			INFO("#######done#######\n");
		}

#ifdef CONFIG_BERLIN_LP_MMU_ENABLE
		flush_dcache_range((uintptr_t)&ddr_info, sizeof(ddr_info));
#endif

#ifdef CFG_SYNA_BCM
#if defined(CONFIG_GENX)
		bcm_kernel_genx_store_hash((void *)ddr_info.ddr_info_data.hash_value, i * HASH_LEN);
#else
		struct tz_boot_param_header *header = (struct tz_boot_param_header*)param;

		bcm_kernel_store_hash(&ddr_info, 0, param, header->size);
#endif
#endif
#endif
	}
	return 0;
}

void tos_suspend(void *param, uint32_t power_state)
{
	int ret;
	if (param && !power_state) {
		ret = tos_compute_ddr_sha(param);
		if (ret)
			panic();

	}
}

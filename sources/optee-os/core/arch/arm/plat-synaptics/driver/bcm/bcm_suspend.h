// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef __BCM_SUSPEND_H__
#define __BCM_SUSPEND_H__

#pragma pack (1)

#define TF_MZ_M_NS				(0x10)
#define TF_MZ_M_ID				(0x0f)

#define TF_MZ_S_NS				4
#define TF_MZ_S_ID				0

#define TF_MZ_ATTR(id, ns)		(((((ns) << TF_MZ_S_NS)) & TF_MZ_M_NS) | \
					((((id) << TF_MZ_S_ID)) & TF_MZ_M_ID))

struct tee_final_memory_zone {
	uint8_t			attr;	/* id + ns*/
	uint32_t		ns_rd;
	uint32_t		ns_wr;
	uint32_t		s_rd;
	uint32_t		s_wr;
	uint32_t		start;
	uint32_t		size;
};

/* register firewall window */
#define TF_RF_M_PERM			(0x7e00)
#define TF_RF_M_STAGE			(0x01c0)
#define TF_RF_M_ID				(0x003f)

#define TF_RF_S_PERM			9
#define TF_RF_S_STAGE			6
#define TF_RF_S_ID				0

#define TF_RF_ATTR(id, stage, perm)		(((((perm) << TF_RF_S_PERM)) & TF_RF_M_PERM) | \
					((((stage) << TF_RF_S_STAGE)) & TF_RF_M_STAGE) | \
					((((id) << TF_RF_S_ID)) & TF_RF_M_ID))

struct tee_final_rf_window {
	uint16_t		attr;	/*id + stage + perm */
	uint32_t		base;
	uint32_t		size;
	uint32_t		mst_group_ids;
};

// master setting
#define TF_MST_M_PERM			(0x3f00)
#define TF_MST_M_STAGE			(0x00e0)
#define TF_MST_M_ID				(0x001f)

#define TF_MST_S_PERM			8
#define TF_MST_S_STAGE			5
#define TF_MST_S_ID				0
#define TF_MST(id, stage, perm)		(((((perm) << TF_MST_S_PERM)) & TF_MST_M_PERM) | \
					((((stage) << TF_MST_S_STAGE)) & TF_MST_M_STAGE) | \
					((((id) << TF_MST_S_ID)) & TF_MST_M_ID))

// Rules
#define TF_RULE_M_PERM			(0xf000)
#define TF_RULE_M_MST			(0x01f0)
#define TF_RULE_M_ID			(0x000f)

#define TF_RULE_S_PERM			12
#define TF_RULE_S_MST			4
#define TF_RULE_S_ID			0
#define TF_RULE(id, mst, perm)		(((((perm) << TF_RULE_S_PERM)) & TF_RULE_M_PERM) | \
					((((mst) << TF_RULE_S_MST)) & TF_RULE_M_MST) | \
					((((id) << TF_RULE_S_ID)) & TF_RULE_M_ID))


struct tee_final_configuration {
	uint32_t rf_window_num;
	uint32_t master_setting_num;
	uint32_t mem_zone_num;
	uint32_t szc_rule_num;

	// store the data of rf_window, master_setting, mem_zone and rules
	uint8_t buff[0];
};

void bcm_suspend_init(void);
void bcm_suspend(void);
void bcm_resume(void);
int bcm_store_tee_data(void* buff, uint32_t offset, uint32_t size);
#endif

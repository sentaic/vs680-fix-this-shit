// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021, Synaptics Incorporated.
 */

#include "mem_region.h"
#include "mem_region_userdata.h"
#include "prot_id.h"
#include "tz_boot_cmd.h"
#include "tz_boot_param.h"
#include "tee_ta_perm_rule_def.h"
#include "szc_mst.h"
#include "szc.h"

#define ION_ATTR_SW_NONCACHED	\
	(ION_A_CO | ION_A_CC | ION_A_ND | ION_A_FD | ION_A_NC | ION_A_FS)
#define ION_ATTR_SW_OTHER	\
	(ION_A_CO | ION_A_CG | ION_A_CV | ION_A_CC | ION_A_ND | ION_A_FD | \
	 ION_A_NC | ION_A_FC | ION_A_FS)
#define ION_ATTR_NW_ALL	\
	(ION_A_CO | ION_A_CG | ION_A_CV | ION_A_CC | ION_A_ND | ION_A_FD | \
	 ION_A_FC | ION_A_NS)

/* Temporary add. It should be removed once TEE support mapping memory on demand. */
#define ION_ATTR_NW_NONCACHED \
	(ION_A_CO | ION_A_CG | ION_A_CV | ION_A_CC | ION_A_ND | ION_A_FD | \
	 ION_A_NC | ION_A_NS)

#define RF_ACPU_GROUP		RF_GROUP0
#define RF_SCPU_GROUP		RF_GROUP1
#define RF_SM_ACRYP_GROUP	RF_GROUP2
#define RF_PERIF_GROUP		RF_GROUP3
#define RF_JTAG2AHB_GROUP	RF_GROUP4
#define RF_CORESIGHT_GROUP	RF_GROUP5
#define RF_SM_OTHER_GROUP	RF_GROUP6

const struct rf_master_group rf_mst_group_param[] = {
	{RF_MST_ACPU,			RF_ACPU_GROUP},
	{RF_MST_SM_SPROT,		RF_SCPU_GROUP},
	{RF_MST_SM_ACRYP,		RF_SM_ACRYP_GROUP},
	{RF_MST_SM_OTHER,		RF_SM_OTHER_GROUP},
	{RF_MST_GE0,			RF_PERIF_GROUP},
	{RF_MST_GE1,			RF_PERIF_GROUP},
	{RF_MST_EMMC,			RF_PERIF_GROUP},
	{RF_MST_SDIO0,			RF_PERIF_GROUP},
	{RF_MST_SDIO1,			RF_PERIF_GROUP},
	{RF_MST_USB0,			RF_PERIF_GROUP},
	{RF_MST_USB1,			RF_PERIF_GROUP},
	{RF_MST_MTEST,			RF_PERIF_GROUP},
	{RF_MST_JTAG2AHB,		RF_JTAG2AHB_GROUP},
	{RF_MST_CORESIGHT,		RF_CORESIGHT_GROUP},
	{RF_MST_PDMA,			RF_PERIF_GROUP},
};
const uint32_t rf_mst_group_num = ARRAY_SIZE(rf_mst_group_param);

/* register firewall setting
 */
const struct rf_window rf_window_param[] = {
	// MPTS_REG
	{1, 0xf9020000, 0xf9040000 - 0xf9020000,
		RF_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZ_SRW_NNA),
		1 << RF_ACPU_GROUP | 1 << RF_SCPU_GROUP},

	// SYSMGR_INTF_REGION0-7, Mstgroup, protOR,SecureRegionCtrl0-3,PERIF_PPC0_CTRL0-3
	{2, 0xf7e2012c, 0xf7e22000 - 0xf7e2012c ,
		RF_ATTR(TZ_BOOT_STAGE_SYSINIT, TZ_SRW_NNA),
		1 << RF_SCPU_GROUP | 1 << RF_ACPU_GROUP},

	// CPU_WRP_CFG64_REG
	{3, 0xf7e30000, 0xf7e31000 - 0xf7e30000 ,
		RF_ATTR(TZ_BOOT_STAGE_SYSINIT, TZ_SRW_NNA),
		1 << RF_SCPU_GROUP | 1 << RF_ACPU_GROUP},

	// HIGH_ROM_VECTOR
	{4, 0xffff0000, 0xffffffff - 0xffff0000,
			RF_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZ_SRW_NNA),
			1 << RF_ACPU_GROUP | 1 << RF_SCPU_GROUP},

};

const uint32_t rf_window_num = ARRAY_SIZE(rf_window_param);

#define MASTER_ACPUS	(1 << MST_ID_ACPU0 | 1 << MST_ID_ACPU1)
#define MASTER_SCPU		(1 << MST_ID_SCPU_SPROT)
#define MASTER_AVIO		(1 << MST_ID_VIO | 1 << MST_ID_AIO)
#define MASTER_GFX		(1 << MST_ID_GPU)
#define MASTER_NPU		(1 << MST_ID_NPU)
#define MASTER_EMMC		(1 << MST_ID_EMMC)

// NS Zones: sys_init, bootloader, NonSecure, NonSecure
const struct mem_zone mem_zone_param[] = {
	{1, "tz",		 S_ZONE, 0x0, 0x0,
		MASTER_ACPUS | MASTER_SCPU,
		MASTER_ACPUS | MASTER_SCPU},

	{2, "Secure",	 S_ZONE, 0x0, 0x0,
		MASTER_ACPUS | MASTER_SCPU | MASTER_AVIO \
		| MASTER_GFX | MASTER_NPU,
		MASTER_ACPUS | MASTER_SCPU | MASTER_AVIO \
		| MASTER_GFX | MASTER_NPU},

	{3, "bootloader",	 NS_ZONE, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff},
	{4, "NonSecure",	 NS_ZONE, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff},
	{5, "NonSecure-NC",	 NS_ZONE, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff},
	{6, "rollback",		 NS_ZONE, 0, 0, 0, 0},
	{7, "pt",		 S_ZONE, 0x0, 0x0,
		MASTER_ACPUS | MASTER_AVIO,
		MASTER_ACPUS },
};

const uint32_t mem_zone_num = ARRAY_SIZE(mem_zone_param);

/* master setting */
const struct master_setting master_setting_param[] = {
	{PROT_ID_AIO,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_S)},
	{PROT_ID_VIO,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_S)},
	{PROT_ID_NPU,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
#if 0 // DONOT overit GPU masters, let GPU set it
	{PROT_ID_GPU,	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_S)},
#endif
	{PROT_ID_MTEST,  	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_GE0,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_GE1,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_EMMC,		MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_SDIO0,	 	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_SDIO1,	 	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_USB0,	 	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_USB1,	 	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
	{PROT_ID_PDMA,	 	MST_ATTR(TZ_BOOT_STAGE_TRUSTZONE, TZM_N)},
};

const uint32_t master_setting_num = ARRAY_SIZE(master_setting_param);

// szc rules definition
const uint32_t szc_rule_param[] = {

};
const uint32_t szc_rule_num = ARRAY_SIZE(szc_rule_param);

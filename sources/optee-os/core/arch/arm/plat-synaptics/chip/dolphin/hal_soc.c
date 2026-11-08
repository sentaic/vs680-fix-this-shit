// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <string.h>
#include "otp_prop.h"
#include "hal_soc.h"
#include "platform_config.h"
#include "tz_errcode.h"
#include "../chip/dolphin/include/Galois_memmap.h"
#include "../chip/dolphin/include/soc.h"
#include "../chip/dolphin/include/global.h"
#include "../chip/dolphin/include/tsp.h"
#include "bcm_suspend.h"
#include "tz_boot_param_value.h"
#include "szc.h"
#include <kernel/misc.h>
#include <kernel/boot.h>
#include "szc_mpt.h"
#include <mm/core_memprot.h>
#include <kernel/pm.h>

#define SUSPEND_WAKEUP_INTR		GIC_IID_SPI(IRQ_sm2socSwInt)

//using SM msg queue 1
#define SM_Send_MsgQ                    (0x1F400 + MEMMAP_SM_REG_BASE)

#define SOC_OTP_BASE_ADDR		(vaddr_t)phys_to_virt_io((MEMMAP_TSP_REG_BASE + 0x4000), MEMMAP_TSP_REG_SIZE)

#define SOC_OTP_ROWS			192
#define SOC_OTP_ROW_LEN			16

#define SOC_RESET_CTRL_BASE		(vaddr_t)phys_to_virt_io((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), MEMMAP_CHIP_CTRL_REG_SIZE)
static unsigned char otp_data[SOC_OTP_ROWS * SOC_OTP_ROW_LEN / 2];

void otp_init(void)
{
	int i = 0;
	unsigned short *pOtpBuf = (unsigned short*)otp_data;
	unsigned int *pOtpBase = (unsigned int*)SOC_OTP_BASE_ADDR;

	/*re-layout the otp field to normal layout*/
	for (i = 0; i < SOC_OTP_ROWS << 2; i += SOC_OTP_ROW_LEN / 4) {
		pOtpBuf[i] =  pOtpBase[i] & 0xffff;
		pOtpBuf[i + 1] =  pOtpBase[i + 1] & 0xffff;
		pOtpBuf[i + 2] =  pOtpBase[i + 2] & 0xffff;
		pOtpBuf[i + 3] =  pOtpBase[i + 3] & 0xffff;
	}

	memset(pOtpBase, 0, SOC_OTP_ROWS * SOC_OTP_ROW_LEN);
}

int soc_get_otp_prop(uint32_t index, void* dst, int dst_len)
{
	int result = TZ_SUCCESS;
	if (dst_len < (int)sizeof(unsigned int))
		return TZ_ERROR_BAD_PARAMETERS;

	switch (index) {
	case OTP_DOLBY_VISION_ENABLE:
		*(unsigned int*)dst = (otp_data[(0x10 >> 1) + 1] >> 2) & 0x1 ? 0 : 1;
		break;
	default:
		result = TZ_ERROR_BAD_PARAMETERS;
		break;
	}

	return result;
}

int soc_get_otp_raw_data(uint32_t row, uint8_t *buffer)
{
	if (row >= SOC_OTP_ROWS)
		return TZ_ERROR_BAD_PARAMETERS;

	memcpy(buffer, &otp_data[row * SOC_OTP_ROW_LEN / 2], SOC_OTP_ROW_LEN / 2);

	return TZ_SUCCESS;
}

void clear_sram(void)
{
	/* clear ZSP SRAM */
	//memset((void*)MEMMAP_ZSP_REG_BASE, 0, 0x40000);
	/* clear TSP SRAM */
	/* FIGO0  Data Sram: 0xf7a40000~0xf7a47fff */
	//memset((void *)0xf7a40000, 0, 0x8000);
	/* FIGO0  Instruction Sram: 0xf7a50000~0xf7a5ffff */
	//memset((void *)0xf7a50000, 0, 0x10000);
	/* FIGO1  Data Sram: 0xf7a60000~0xf7a67fff */
	//memset((void *)0xf7a60000, 0, 0x8000);
	/* FIGO1  Instruction Sram: 0xf7a70000~0xf7a7ffff */
	//memset((void *)0xf7a68000, 0, 0x2000);

}


void __attribute__((noreturn)) reset_soc(void)
{
	/* set soc reset bit to 1 and reset it */
	int i;
	while (1){
		*(volatile uint32_t*)SOC_RESET_CTRL_BASE = 1;
		i = 300;
		while(i--);
	}
}
#if 0
#define SUSPEND_WAKEUP_INTR		GIC_IID_SPI(IRQ_sm2socSwInt)

//using SM msg queue 1
#define SM_Send_MsgQ                    0xF7F97400
void sm_do_system_off(void);
void prot_set_by_stage(uint32_t stage);
void rf_set_by_stage(uint32_t stage);

#ifdef CONFIG_ENABLE_PIE
extern unsigned long mmu_temp_data[3];
#endif
#endif

extern struct mem_zone mzs[SZC_ZONE_NUM_ZONES_MAX];
extern int szc_zone_num;

void plat_save_tee_final_configuration(void)
{
	struct tee_final_configuration *tee_final_config;
	struct tee_final_rf_window * rf;
	struct tee_final_memory_zone *zone;
	uint16_t *mst_setting;
	uint16_t *rule;
	uint32_t rf_window_len, master_setting_len, mz_zone_len, rule_len;
	uint32_t i, attr, id, perm, stage, mst;
	uint32_t tee_final_config_len;

	rf_window_len = rf_window_num * sizeof(*rf);
	master_setting_len = master_setting_num * sizeof(*mst_setting);
	mz_zone_len = szc_zone_num * sizeof(*zone);
	rule_len = szc_rule_num *sizeof(*rule);

	tee_final_config_len = rf_window_len + master_setting_len + mz_zone_len + rule_len;

	DMSG("rf_window_num = %d, master_setting_len = %d, secure_memzone_num = %d, szc_rule_num = %d\n",
		rf_window_num, master_setting_num, szc_zone_num, szc_rule_num);

	tee_final_config = malloc(sizeof(*tee_final_config) + tee_final_config_len);
	assert(NULL != tee_final_config);

	memset(tee_final_config, 0, sizeof(struct tee_final_configuration) + tee_final_config_len);

	// copy rf windows
	tee_final_config->rf_window_num = rf_window_num;
	rf = (struct tee_final_rf_window *)tee_final_config->buff;
	for (i = 0; i < rf_window_num; i++) {
		attr = rf_window_param[i].attr;
		id = rf_window_param[i].id;
		stage = RF_GET_STAGE(rf_window_param[i].attr);
		perm = RF_GET_PERM(rf_window_param[i].attr);
		rf[i].base = rf_window_param[i].base;
		rf[i].size = rf_window_param[i].mask;
		rf[i].mst_group_ids = rf_window_param[i].mst_group_ids;
		rf[i].attr = TF_RF_ATTR(id, stage, perm);
	}

	// copy secure master setting
	tee_final_config->master_setting_num = master_setting_num;
	mst_setting = (uint16_t *)((uintptr_t)rf + rf_window_len);
	for (i = 0; i < master_setting_num; i++) {
		attr = master_setting_param[i].attr;
		id = master_setting_param[i].id;
		perm = MST_GET_PERM(attr);
		stage = MST_GET_STAGE(attr);
		mst_setting[i] = TF_MST(id, stage, perm);
	}

	// copy memory zone
	tee_final_config->mem_zone_num = szc_zone_num;
	zone = (struct tee_final_memory_zone *)((uintptr_t)mst_setting + master_setting_len);
	for (i = 0; i < szc_zone_num; i++) {
		zone[i].attr = TF_MZ_ATTR(mzs[i].id, mzs[i].ns);
		zone[i].ns_rd = mzs[i].ns_rd;
		zone[i].ns_wr = mzs[i].ns_wr;
		zone[i].s_rd = mzs[i].s_rd;
		zone[i].s_wr = mzs[i].s_wr;
		zone[i].start = mzs[i].start;
		zone[i].size =mzs[i].size;
	}

	// copy rule definitions
	tee_final_config->szc_rule_num = szc_rule_num;
	rule = (uint16_t *)((uintptr_t)zone + mz_zone_len);
	for (i = 0; i < szc_rule_num; i++) {
		attr = szc_rule_param[i];
		id = RULE_GET_ID(attr);
		mst = RULE_GET_MST(attr);
		perm = RULE_GET_PERM(attr);
		rule[i] = TF_RULE(id, mst, perm);
	}

	DMSG("%s(),line %d: tee_final_config_len = 0x%x, len = 0x%x.\n",
		__func__, __LINE__, tee_final_config_len, sizeof(*tee_final_config) + tee_final_config_len);

	if (bcm_store_tee_data(tee_final_config, 0, sizeof(*tee_final_config) + tee_final_config_len)){
		EMSG("fail to store suspend info.\n");
		assert(0);
	};

	memset(tee_final_config, 0, sizeof(*tee_final_config) + tee_final_config_len);

	free(tee_final_config);
}

/*-------------------------------------------------------------------
 * Unlike suspend in BG2Q or relevant chips, requirement is changed
 * in BG4CT in suspend implementation:
 *
 * a) Primary cpu in Linux kernel would power off secondaries and
 *    calls PSCI suspend command and enter secure monitor
 * b) secure monitor calls TOS's suspend handler
 * c) TOS's suspend handler takes platform specific action for
 *    suspend preparation and gives control back to secure monitor
 *    once done
 * d) secure monitor calls platform specific power management APIs
 *    to do suspend, this includes:
 *    1) Save TSP SRAM contents to DDR memory
 *    2) Copy suspend codes to TSP SRAM and jump to TSP SRAM to
 *       execute the suspend codes
 *    3) Put DDR into self refresh state and enter wfi state, waiting
 *       for interrupt events for wake up
 *
 * Below plat_suspend is the step c) as described above, mainly to
 * set up the interrupt sources which are used to wake up CPU from
 * wfi state
 *-------------------------------------------------------------------
 */
void plat_suspend(void)
{
	mpt_suspend();
	plat_save_tee_final_configuration();
}


static void plat_resume(int cpu_id)
{
#ifdef CONFIG_SZC_ICTL
	extern void szc_ictl_init(void);
	extern void mc6ctrl_axi_error_monitor_init(void);
	szc_ictl_init();
	mc6ctrl_axi_error_monitor_init();
#endif
#ifdef CONFIG_SZC_MPT
	mpt_resume();
#endif
}


unsigned long thread_cpu_suspend_handler(unsigned long a0 __unused,
					unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;

	if (get_core_pos() != 0)
		return 1;

	plat_suspend();

	retstatus = pm_change_state(PM_OP_SUSPEND, 0);

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;

	return 0;
}

unsigned long thread_cpu_resume_handler(unsigned long a0 __unused,
					unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;
	if (get_core_pos() == 0) {
		DMSG("resume for primary cpu.\n");
		boot_primary_init_intc();
		plat_resume(0);
	} else {
		DMSG("resume for secondary cpu.\n");
		boot_secondary_init_intc();
	}
	retstatus = pm_change_state(PM_OP_RESUME, 0);

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;

	return 0;
}

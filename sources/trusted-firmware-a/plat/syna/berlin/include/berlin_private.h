/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BERLIN_PRIVATE_H__
#define __BERLIN_PRIVATE_H__

#include <arch.h>
#include <platform_def.h>
#include <lib/psci/psci.h>

struct tz_boot_arg32 {
	uint32_t	flags;		/* w0. boot flags */
	uint32_t	boot_param;	/* w1. NULL for default */
	uint32_t	tos_entry;	/* w2 */
	uint32_t	nw_entry;	/* w3 */
	uint32_t	nw_param[4];	/* w4-w7 for nw w0-w3 */
};

enum {
	NORMAL_BOOT,
	WARM_BOOT,
	RECOVER = 0x80,
	GENX = 0x100
};

enum {
// Other CPU FW images
	IMAGE_TYPE_FW_START = 0x30,
	IMAGE_TYPE_TSP_FW = 0x30,
	IMAGE_TYPE_DSP_FW = 0x31,
	IMAGE_TYPE_GPU_FW = 0x32,
	IMAGE_TYPE_NPU_FW = 0x33,
	IMAGE_TYPE_SM_FW  = 0x34,
	IMAGE_TYPE_FW_END,
};

enum {
	BCM_KERNEL_PI_VERIFY_FW_START = 0x10,
	BCM_KERNEL_PI_VERIFY_TSP_FW = 0x10,
	BCM_KERNEL_PI_VERIFY_DSP_FW = 0x11,
	BCM_KERNEL_PI_VERIFY_GPU_FW = 0x12,
	BCM_KERNEL_PI_VERIFY_FW_END = 0x12,
};

extern unsigned int boot_mode;

void berlin_context_resume(void);
void berlin_init(void);
void berlin_cpu_standby(plat_local_state_t cpu_state);
int berlin_pwr_domain_on(u_register_t mpidr);
void berlin_pwr_domain_off(const psci_power_state_t *target_state);
void berlin_pwr_domain_suspend(const psci_power_state_t *target_state);
void berlin_pwr_domain_on_finish(const psci_power_state_t *target_state);
void berlin_pwr_domain_suspend_finish(
				const psci_power_state_t *target_state);
void __dead2 berlin_pwr_domain_pwr_down_wfi(
				const psci_power_state_t *target_state);
void __dead2 berlin_system_off(void);
void __dead2 berlin_system_reset(void);
int32_t berlin_validate_power_state(unsigned int power_state,
				psci_power_state_t *req_state);
int berlin_validate_ns_entrypoint(uintptr_t entrypoint);
void berlin_get_sys_suspend_power_state(psci_power_state_t *req_state);

#endif /* __BERLIN_PRIVATE_H__ */

/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <assert.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <drivers/arm/gicv2.h>
#include <plat/arm/common/plat_arm.h>
#include <lib/mmio.h>
#include <common/bl_common.h>
#include <context.h>
#include <lib/el3_runtime/context_mgmt.h>
#include <berlin_private.h>
#include <common/debug.h>
#include <drivers/generic_delay_timer.h>
#include <drivers/ti/uart/uart_16550.h>
#include "bcm.h"
#include "sm.h"
#include "rf.h"
#include "tz_boot_param.h"
#include "tz.h"
#include "tz_perm.h"

/*
 * The next 2 constants identify the extents of the code & RO data region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __RO_START__ and __RO_END__ linker symbols refer to page-aligned addresses.
 */
#define BL31_RO_BASE (unsigned long)(&__RO_START__)
#define BL31_RO_LIMIT (unsigned long)(&__RO_END__)
#define BL31_END (unsigned long)(&__BL31_END__)

#if USE_COHERENT_MEM
/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols
 * refer to page-aligned addresses.
 */
#define BL31_COHERENT_RAM_BASE (unsigned long)(&__COHERENT_RAM_START__)
#define BL31_COHERENT_RAM_LIMIT (unsigned long)(&__COHERENT_RAM_END__)
#endif

#define MAP_DEVICE0	MAP_REGION_FLAT(BERLIN_DEVICE0_BASE,		\
					BERLIN_DEVICE0_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#ifdef BERLIN_DEVICE1_SIZE
#define MAP_DEVICE1	MAP_REGION_FLAT(BERLIN_DEVICE1_BASE,            \
					BERLIN_DEVICE1_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)
#else

#define MAP_DEVICE1 {0}
#endif

/*
 * Table of regions for various BL stages to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * configure_mmu_elx() will give the available subset of that,
 */
#if IMAGE_BL31
const mmap_region_t berlin_mmap[] = {
	MAP_DEVICE0,
	MAP_DEVICE1,
	{0}
};
#endif

/*******************************************************************************
 * Macro generating the code for the function setting up the pagetables as per
 * the platform memory map & initialize the mmu, for the given exception level
 ******************************************************************************/
#if USE_COHERENT_MEM
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void berlin_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit,		\
				   unsigned long coh_start,		\
				   unsigned long coh_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add_region(coh_start, coh_start,			\
				coh_limit - coh_start,			\
				MT_DEVICE | MT_RW | MT_SECURE);		\
		mmap_add(berlin_mmap);					\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#else
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void berlin_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add(berlin_mmap);					\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#endif

/* Define EL1 and EL3 variants of the function initialising the MMU */
DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)

static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;

static unsigned char nagra_boot_params[MAX_SECURE_BOOT_PARA_SIZE];
unsigned char *nagra_boot_p = NULL;
unsigned int boot_mode = NORMAL_BOOT;
static uint64_t nw_entry_addr __unused;

/*******************************************************************************
 * Gets SPSR for BL32 entry
 ******************************************************************************/
uint32_t berlin_get_spsr_for_bl32_entry(void)
{
	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL32 image.
	 */
	return 0;
}

/*******************************************************************************
 * Gets SPSR for BL33 entry
 ******************************************************************************/
uint32_t berlin_get_spsr_for_bl33_entry(void)
{
	unsigned long el_status;
	unsigned int mode;
	uint32_t spsr;

	/* Figure out what mode we enter the non-secure world in */
	el_status = read_id_aa64pfr0_el1() >> ID_AA64PFR0_EL2_SHIFT;
	el_status &= ID_AA64PFR0_ELX_MASK;

	if (el_status)
		mode = MODE_EL2;
	else
		mode = MODE_EL1;

	/*
	 * TODO: Consider the possibility of specifying the SPSR in
	 * the FIP ToC and allowing the platform to have a say as
	 * well.
	 */
	spsr = SPSR_64(mode, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
	return spsr;
}

void berlin_context_resume(void)
{
	/*
	 * tell sm we back
	 */
	sm_do_system_warmup(0);
}

static inline void reset_soc(void)
{
	*(volatile uint32_t*)SOC_RESET_CTRL_BASE = 1;
}

/*******************************************************************************
 * Handler to get the System Counter Frequency
 ******************************************************************************/
unsigned int plat_get_syscnt_freq2(void)
{
	return BERLIN_SYSCNT_FREQ;
}

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for
 * security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type.
 ******************************************************************************/
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info = NULL;

	assert(sec_state_is_valid(type));

	next_image_info = (type == NON_SECURE) ?
		&bl33_image_ep_info :
		&bl32_image_ep_info;

	if (next_image_info && next_image_info->pc)
		return next_image_info;

	return NULL;
}

void bl31_update_bl32_pc(uint64_t offset)
{
	bl32_image_ep_info.pc += offset;
}

/*******************************************************************************
 * Perform any BL31 specific platform actions. Populate the BL33 and BL32 image
 * info.
 ******************************************************************************/
void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
		u_register_t arg2, u_register_t arg3)
{
	struct tz_boot_arg32 *args = NULL;
	struct tz_boot_arg32 boot_args = {0};
	void *plat_params_from_bl2 = (void *)arg1;
	static console_t console;

	if(arg0) {
		memcpy(&boot_args,  (struct tz_boot_arg32 *)arg0, sizeof(boot_args));
		args = &boot_args;
	}

	console_16550_register(UART_0_BASE,
		BERLIN_UART_CLOCK,
		BERLIN_UART_BAUDRATE, &console);

	/* Populate entry point information for BL3-2 and BL3-3 */
	SET_PARAM_HEAD(&bl32_image_ep_info,
			PARAM_EP,
			VERSION_1,
			0);
	SET_SECURITY_STATE(bl32_image_ep_info.h.attr, SECURE);
#ifdef BL32_BASE
	bl32_image_ep_info.pc = BL32_BASE;
	bl32_image_ep_info.spsr = berlin_get_spsr_for_bl32_entry();
#else
	bl32_image_ep_info.pc = 0;
#endif
#if RESET_TO_BL31
	bl32_image_ep_info.args.arg0 = 0;
#ifdef BL32_BOOT_PARAM
	bl32_image_ep_info.args.arg1 = BL32_BOOT_PARAM;
#else
	bl32_image_ep_info.args.arg1 = 0;
#endif
	bl32_image_ep_info.args.arg2 = 0;
	bl32_image_ep_info.args.arg3 = BL33_BASE;
	bl32_image_ep_info.args.arg4 = DTB_BASE;
	bl32_image_ep_info.args.arg5 = 0;
	bl32_image_ep_info.args.arg6 = 0;
	bl32_image_ep_info.args.arg7 = 0;
#else
	if (args) {
#if defined(SPD_opteed)
		bl32_image_ep_info.args.arg0 = 0;
#else
		bl32_image_ep_info.args.arg0 = args->flags;
#endif
		bl32_image_ep_info.args.arg1 = args->boot_param;
		bl32_image_ep_info.args.arg2 = 0;
		bl32_image_ep_info.args.arg3 = args->nw_entry;
		bl32_image_ep_info.args.arg4 = args->nw_param[0];
		bl32_image_ep_info.args.arg5 = args->nw_param[1];
		bl32_image_ep_info.args.arg6 = args->nw_param[2];
		bl32_image_ep_info.args.arg7 = args->nw_param[3];
		nw_entry_addr = args->nw_entry;
	}
#endif
	SET_PARAM_HEAD(&bl33_image_ep_info,
			PARAM_EP,
			VERSION_1,
			0);
	SET_SECURITY_STATE(bl33_image_ep_info.h.attr, NON_SECURE);
	bl33_image_ep_info.spsr = berlin_get_spsr_for_bl33_entry();
#if RESET_TO_BL31
	bl33_image_ep_info.pc = BL33_BASE;
	bl33_image_ep_info.args.arg0 = DTB_BASE;
#else
	if (args) {
		/*
		 * Tell BL31 where the non-trusted software image
		 * is located and the entry state information
		 */
		bl33_image_ep_info.pc = args->nw_entry;

		if (args->flags & GENX) {
			bl33_image_ep_info.args.arg0 = args->flags;
			boot_mode = args->flags;
		} else {
			bl33_image_ep_info.args.arg0 = args->nw_param[0];
		}
		bl33_image_ep_info.args.arg1 = args->nw_param[1];
		bl33_image_ep_info.args.arg2 = args->nw_param[2];
		bl33_image_ep_info.args.arg3 = args->nw_param[3];
	}
#endif

	if (bl32_image_ep_info.args.arg1)
		tz_boot_param_init((void *)(uintptr_t)bl32_image_ep_info.args.arg1);

	/*
	 * save the secure boot param when cold boot for suspend/resume
	 */
	if (plat_params_from_bl2) {
		tos_save_secure_boot_param((void *)(uintptr_t)args->boot_param,
				plat_params_from_bl2,
				nagra_boot_params,
				MAX_SECURE_BOOT_PARA_SIZE);
		nagra_boot_p = nagra_boot_params;
	} else {
		nagra_boot_p = NULL;
	}

	return;
}

/*******************************************************************************
 * Initialize the gic, configure the SCR.
 ******************************************************************************/
void bl31_platform_setup(void)
{
	generic_delay_timer_init();

	/* Initialize the gic cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();
	plat_arm_gic_cpuif_enable();

	/* Initialize the arch timer that Linux kernel requires */
	mmio_write_32(TCTL_BASE, 0x1);

	/* power initialization: turn off secondaries */
	berlin_init();

	return;
}

/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this only intializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl31_plat_arch_setup(void)
{
	berlin_configure_mmu_el3(BL31_RO_BASE,
				(BL31_END - BL31_RO_BASE),
				BL31_RO_BASE,
				BL31_RO_LIMIT
#if USE_COHERENT_MEM
				, BL31_COHERENT_RAM_BASE,
				BL31_COHERENT_RAM_LIMIT
#endif
				);

	return;
}

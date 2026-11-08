// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <kernel/misc.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <mm/mobj.h>
#include <stdint.h>
#include <string.h>
#include <tee/entry_fast.h>
#include <sm/optee_smc.h>
#include "tz_boot_cmd.h"
#include "syna_fastcall.h"
#include "syna_fastcall_entry.h"
#if 0
struct fastcall_generic_param {
	/*
	 * the first is always the sub command id
	 */
	unsigned int sub_cmd_id;
	unsigned int param_len;
	char param[0];
};

enum {
	OTP_ANTI_ROLLBACK =		1,
	OTP_SEGMENT_ID =		2,
	TSSS_FASTCALL_CMD =		3,
	RPMB_FASTCALL_CMD =		4,
	GENRIC_FASTCALL_MEMMOVE = 5,
	GENRIC_FASTCALL_CACH_OP = 6,
};

struct tee_memmove_param {
	 void *dst;
	 void *src;
	 size_t size;
 };

struct tee_cache_param {
	void *start;
	size_t size;
	enum utee_cache_operation op;
};

static struct mobj *syna_map_cmd_buffer(paddr_t parg, size_t args_size)
{
	struct mobj *mobj;
	struct fastcall_generic_param *arg;

	assert(!(parg & SMALL_PAGE_MASK));
	/* mobj_mapped_shm_alloc checks if parg resides in nonsec ddr */

	mobj = mobj_mapped_shm_alloc(&parg, 1, 0, 0);
	if (!mobj)
		return NULL;

	arg = mobj_get_va(mobj, 0, 1);
	if (!arg) {
		EMSG("Fail to get va address.\n");
		mobj_put(mobj);
		return NULL;
	}

	if (args_size > SMALL_PAGE_SIZE) {
		EMSG("Command buffer spans across page boundary");
		mobj_put(mobj);
		return NULL;
	}

	return mobj;
}

static struct mobj *syna_get_cmd_buffer(paddr_t parg, size_t args_size)
{
	struct fastcall_generic_param *arg;

	arg = phys_to_virt(parg, MEM_AREA_NSEC_SHM, 1);
	if (!arg) {
		EMSG("Fail to transfer va to pa address.\n");
		return NULL;
	}
	return mobj_shm_alloc(parg, args_size, 0);
}



static void syna_tee_entry_fastcall_generic_command(struct thread_smc_args *args)
{
	struct fastcall_generic_param *param;
	size_t param_len, total_param_len;
	paddr_t pa = 0;
	void *param_base;
	struct mobj *mobj;

	pa = reg_pair_to_64(args->a1, args->a2);

	total_param_len = reg_pair_to_64(args->a3, args->a4);

	/* Check if this region is in static shared space */
	if (core_pbuf_is(CORE_MEM_NSEC_SHM, pa, total_param_len)) {
		mobj = syna_get_cmd_buffer(pa, total_param_len);
	} else {
		if (pa & SMALL_PAGE_MASK) {
			EMSG("Command buffer spans across page boundary");
			args->a0 = OPTEE_SMC_RETURN_EBADADDR;
			return;
		}
		mobj = syna_map_cmd_buffer(pa, total_param_len);
	}

	if (!mobj) {
		args->a0 = OPTEE_SMC_RETURN_EBADADDR;
		return;
	}

	param = mobj_get_va(mobj, 0, 1);
	if (!param) {
		EMSG("Fail to get va address.\n");
		mobj_put(mobj);
		args->a0 = OPTEE_SMC_RETURN_EBADADDR;
		return;
	}

	if (!mobj || !IS_ALIGNED_WITH_TYPE(pa, struct fastcall_generic_param)) {
		EMSG("Bad arg address 0x%" PRIxPA, pa);
		args->a0 = OPTEE_SMC_RETURN_EBADADDR;
		mobj_put(mobj);
		return;
	}

	param_base = param->param;
	param_len = param->param_len;

	switch (param->sub_cmd_id) {
	case GENRIC_FASTCALL_MEMMOVE:
	case GENRIC_FASTCALL_CACH_OP:
	default:
		break;
	}

}
#endif

static void syna_tee_entry_fast(struct thread_smc_args *args)
{
	switch (args->a0) {
	case SMC_FUNC_TOS_BOOT_STAGE:
		DMSG("fast call: SMC_FUNC_TOS_BOOT_STAGE.\n");
		fastcall_enter_boot_stage(args);
		break;
	case SMC_FUNC_TOS_MEM_REGION_COUNT:
		DMSG("fast call: SMC_FUNC_TOS_MEM_REGION_COUNT.\n");
		fastcall_get_mem_region_count(args);
		break;
	case SMC_FUNC_TOS_MEM_REGION_LIST:
		DMSG("fast call: SMC_FUNC_TOS_MEM_REGION_LIST.\n");
		fastcall_get_mem_region_list(args);
		break;
	case SMC_FUNC_TOS_CRYPTO_VERIFY_IMAGE:
		DMSG("fast call: SMC_FUNC_TOS_CRYPTO_VERIFY_IMAGE.\n");
		fastcall_verify_image(args);
		break;
	default:
		__tee_entry_fast(args);
		break;
	}
}

/* Overriding the default __weak tee_entry_fast() */
void tee_entry_fast(struct thread_smc_args *args)
{
	syna_tee_entry_fast(args);
}

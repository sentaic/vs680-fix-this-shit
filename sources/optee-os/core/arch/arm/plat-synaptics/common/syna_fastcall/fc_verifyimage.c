// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <tee/entry_fast.h>
#include <trace.h>
#include <util.h>
#include "syna_setup.h"
#include <mm/core_memprot.h>
#include "mr_setup.h"
#include "mem_region.h"
#include "mem_region_userdata.h"
#include "tz_errcode.h"
#include "tz_perm.h"
#include "tz_boot_cmd.h"
#include "bcm/bcm_genx_verify.h"
#include "bcm/bcm_hw_function.h"
#include "syna_fastcall.h"
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <mm/mobj.h>
#include <tee/cache.h>
#include "image.h"

#ifdef CFG_IMAGE_NO_ENCRYPTION
static struct mobj *get_param_mobj(paddr_t pa, size_t len)
{
	int result = TZ_ERROR_GENERIC;
	struct mobj *mobj = NULL;
	paddr_t *pages = NULL;
	int i;
	void *va;
	int num_pages;

	//DMSG(" %s(), line %d pa = 0x%08" PRIxPA " len = 0x%" PRIxPASZ, __func__, __LINE__, pa, len);

	num_pages = len / SMALL_PAGE_SIZE + 1;
	if (core_pbuf_is(CORE_MEM_NSEC_SHM, pa, len)) {
		va = phys_to_virt(pa, MEM_AREA_NSEC_SHM, 1);
		if (!va) {
			EMSG("Fail to transfer va to pa address.\n");
			result = TZ_ERROR_BAD_PARAMETERS;
			goto out;
		}
		mobj = mobj_shm_alloc(pa, len, 0);
	} else {
		if (pa & SMALL_PAGE_MASK) {
			EMSG("Command buffer spans across page boundary pa = 0x%08" PRIxPA, pa);
			result = TZ_ERROR_BAD_PARAMETERS;
			goto out;
		}
		pages = malloc(sizeof(paddr_t) *num_pages);
		if (!pages) {
			result = TZ_ERROR_OUT_OF_MEMORY;
			goto out;
		}
		memset(pages, 0, sizeof(paddr_t) *num_pages);
		for (i = 0; i < num_pages; i++) {
			pages[i] = pa + i * SMALL_PAGE_SIZE;
		}
		mobj = mobj_mapped_shm_alloc(pages, num_pages, 0, 0);
	}
	result = TZ_SUCCESS;

out:
	if (pages) {
		memset(pages, 0, sizeof(paddr_t) *num_pages);
		free(pages);
	}

	if (result == TZ_SUCCESS)
		return mobj;

	if (mobj) {
		mobj_dec_map(mobj);
		mobj_put(mobj);
	}
	return NULL;
}

#define SIGN_LEN 616


static int memcpy_page(paddr_t dst, paddr_t src, size_t len)
{
	int result = TZ_ERROR_GENERIC;
	struct mobj *src_mobj = NULL;
	struct mobj *dst_mobj = NULL;
	void *src_va = NULL;
	void *dst_va = NULL;
	paddr_t pa_src;
	paddr_t pa_dst;
	pa_src = (~SMALL_PAGE_MASK) & src;
	pa_dst = (~SMALL_PAGE_MASK) & (uintptr_t)dst;

	if (len > SMALL_PAGE_SIZE)
		return TZ_ERROR_NOT_SUPPORTED;

	src_mobj = get_param_mobj(pa_src, SMALL_PAGE_SIZE);
	if (!src_mobj) {
		EMSG("Fail to get mobj for src @ 0x%08" PRIxPA, pa_src);
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}

	src_va = mobj_get_va(src_mobj, src - pa_src, SMALL_PAGE_SIZE);
	//DMSG(" %s(), line %d src_va = 0x%p", __func__, __LINE__, src_va);
	if (!src_va) {
		EMSG("Fail to get va address.\n");
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}

	dst_mobj = get_param_mobj(pa_dst, SMALL_PAGE_SIZE);
	if (!dst_mobj) {
		EMSG("Fail to get mobj for dst @0x%08" PRIxPA, pa_dst);
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}
	dst_va = mobj_get_va(dst_mobj, dst - pa_dst, SMALL_PAGE_SIZE);
	//DMSG(" %s(), line %d dst_va = 0x%p", __func__, __LINE__, dst_va);
	if (!dst_va) {
		EMSG("Fail to get va address.\n");
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}

	memcpy(dst_va, src_va, len);

	cache_operation(TEE_CACHEFLUSH, dst_va, SMALL_PAGE_SIZE);

	result =  TZ_SUCCESS;

out:
	if (src_mobj) {
		mobj_dec_map(src_mobj);
		mobj_put(src_mobj);
	}
	if (dst_mobj) {
		mobj_dec_map(dst_mobj);
		mobj_put(dst_mobj);
	}
	return result;
}

static int memclr_page(paddr_t dst, size_t len)
{
	int result = TZ_ERROR_GENERIC;
	struct mobj *dst_mobj = NULL;
	void *dst_va = NULL;
	paddr_t pa_dst;
	pa_dst = (~SMALL_PAGE_MASK) & (uintptr_t)dst;

	if (len > SMALL_PAGE_SIZE)
		return TZ_ERROR_NOT_SUPPORTED;

	dst_mobj = get_param_mobj(pa_dst, SMALL_PAGE_SIZE);
	if (!dst_mobj) {
		EMSG("Fail to get mobj for dst @0x%08" PRIxPA , pa_dst);
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}
	dst_va = mobj_get_va(dst_mobj, dst - pa_dst, SMALL_PAGE_SIZE);
	DMSG(" %s(), line %d dst_va = 0x%p.\n", __func__, __LINE__, dst_va);
	if (!dst_va) {
		EMSG("Fail to get va address.\n");
		result = TZ_ERROR_COMMUNICATION;
		goto out;
	}

	memset(dst_va, 0, len);

	cache_operation(TEE_CACHEFLUSH, dst_va, SMALL_PAGE_SIZE);

	result =  TZ_SUCCESS;

out:
	if (dst_mobj) {
		mobj_dec_map(dst_mobj);
		mobj_put(dst_mobj);
	}
	return result;
}

static tz_errcode_t verify_clear_image(paddr_t src, unsigned long src_len,
		paddr_t dst, unsigned long * dst_len __unused)
{
	int result = TZ_ERROR_GENERIC;
	paddr_t pa_src;
	paddr_t pa_dst;
	size_t len;
	size_t cp_len;

	if (!src_len|| src_len < SIGN_LEN)
		return TZ_ERROR_BAD_PARAMETERS;

	DMSG(" %s(), line %d src = 0x%08" PRIxPA " src_len = 0x%lx, dst = 0x%08" PRIxPA " dst_len = 0x%p.\n",
			__func__, __LINE__, src, src_len, dst, dst_len);

	if (src_len < SMALL_PAGE_SIZE) {
		result = memcpy_page(dst, src + SIGN_LEN,  src_len - SIGN_LEN);
		return result;
	}

	cp_len = SMALL_PAGE_SIZE;
	result = memcpy_page(dst, src + SIGN_LEN,  cp_len);
	if (result != TZ_SUCCESS) {
		EMSG("fail to copy memory.\n");
		return TZ_ERROR_ACCESS_CONFLICT;
	}

	pa_src = src + SIGN_LEN + cp_len;
	pa_dst = dst + cp_len;

	len = src_len - cp_len - SIGN_LEN;

	do {
		if (len < SMALL_PAGE_SIZE) {
			cp_len = len;
		} else
			cp_len = SMALL_PAGE_SIZE;

		result = memcpy_page(pa_dst, pa_src, cp_len);
		if (result != TZ_SUCCESS)
			goto out;

		pa_src += cp_len;
		pa_dst += cp_len;
		len -= cp_len;
	} while (len > 0);

	return TZ_SUCCESS;
out:
	while(pa_dst) {
		memclr_page(pa_dst, SMALL_PAGE_SIZE);
		pa_dst -= SMALL_PAGE_SIZE;
	}
	return result;
}
#endif
/** tz_sw_verify_image.
 *
 * @param src		source buffer address
 * @param src_len	source buffer length, input param
 * @param dst		dest buffer address
 * @param dst_len	dest buffer length, input and output param
 * @param code_type	code type pass to bcm
 *
 * @return		error code
 */
static tz_errcode_t tz_sw_verify_image(paddr_t src, unsigned long src_len,
		paddr_t dst, unsigned long *dst_len, unsigned long __unused code_type)
{
	int result = TZ_ERROR_GENERIC;

	/* this command can only be executed in bootloader */
	if (tz_get_boot_stage() != TZ_BOOT_STAGE_BOOTLOADER) {
		EMSG("can only be called in bootloader, crrent stage 0x%lx\n",
				tz_get_boot_stage());
		return TZ_ERROR_ACCESS_DENIED;
	}

	// dst address is not needed in genx flow
	if (!src || !src_len) {
		EMSG("bad parameters, src=0x%08" PRIxPA ", src_len=0x%lx\n",
				src, src_len);
		return TZ_ERROR_BAD_PARAMETERS;
	}
#if defined(CFG_BCM) || defined(CFG_SYNAPROT_MAILBOX)
	result = verify_image(src, src_len, dst, (unsigned int*)dst_len, code_type, 1);
	if (result) {
		EMSG("verify image fail, src=0x%08" PRIxPA ", len=0x%lx, dst=0x%08" PRIxPA "",
				src, src_len, dst);
		/* we use generic for we don't know the exact error code */
		return TZ_ERROR_GENERIC;
	}
#elif defined(CFG_IMAGE_NO_ENCRYPTION)
	result = verify_clear_image(src, src_len, (paddr_t)dst, (unsigned long*)dst_len);
	return result;
#else
	return TZ_ERROR_NOT_SUPPORTED;
#endif
	return TZ_SUCCESS;
}

void fastcall_verify_image(struct thread_smc_args *args)
{
	unsigned long result[2];
	paddr_t src = args->a1;
	unsigned long src_len = args->a2;
	paddr_t dst = args->a3;
	unsigned long code_type = args->a5;

	result[1] = args->a4;
	result[0] = tz_sw_verify_image(src, src_len, dst, &result[1], code_type);

	args->a0 = result[0];
	args->a1 = result[1];
}

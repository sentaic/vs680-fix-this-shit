// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <types_ext.h>
#include <string.h>
#include <kernel/tee_ta_manager.h>
#include <kernel/user_access.h>
#include <kernel/user_mode_ctx.h>
#include <mm/vm.h>
#include <mm/core_memprot.h>
#include <mm/mobj.h>
#include <tee/cache.h>

#include "syna_setup.h"
#include "mr_setup.h"
#include "svc_syna.h"

#include "mem_region_userdata.h"
#include "hal_soc.h"
#ifdef CFG_TA_PERM_CHECK
#include "tee_ta_perm_ctrl.h"
#endif
#include "addr_range_check.h"
#include "tz_boot_param_value.h"
#include "hal_soc.h"
#include "image.h"

int __weak verify_image(paddr_t src __unused, unsigned int size __unused, paddr_t dst __unused,
		unsigned int *dst_size __unused, unsigned int image_type __unused, unsigned int multi_keys __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

TEE_Result __weak syna_exec_cmd(uint32_t cmd_id __unused, const void *param __unused,
		uint32_t param_len __unused, const void *result __unused, uint32_t result_len __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

TEE_Result __weak syna_exec_cmd_ex(uint32_t cmd_id __unused, const void *param __unused,
		uint32_t param_len __unused, const void *result __unused,
		uint32_t result_len __unused, uint32_t delay __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

TEE_Result syscall_syna_mr_filter(void *region, uint32_t *max_num,
		unsigned long attr_mask, unsigned long attr_val)
{
	TEE_Result res = TEE_SUCCESS;
	struct mem_region *list = NULL;
	long len;

	if (max_num == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if ((region == NULL) && (*max_num == 0)) {
		*max_num = mr_filter(NULL, *max_num, attr_mask, attr_val);
		return TEE_SUCCESS;
	}

	if (region == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	len = *max_num * sizeof(*list);

	list = malloc(len);
	if (list == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	res = mr_filter(list, *max_num, attr_mask, attr_val);
	DMSG("mr_num = %d", res);
	len = res * sizeof(*list);
	*max_num = res;

	if (len)
		res = copy_to_user(region, list, len);

	if (res != TEE_SUCCESS) {
		EMSG("fail to get memory region, res = 0x%x.\n", res);
		res = TEE_ERROR_ITEM_NOT_FOUND;
	}
	free(list);
	return res;
}

TEE_Result syscall_mr_find_region(const void *buffer, uint32_t size, struct mem_region *mr)
{
	const struct mem_region *m;

	if (!buffer || !mr)
		return TEE_ERROR_BAD_PARAMETERS;

	m = mr_find_region(virt_to_phys((void *)buffer), size);
	if (!m)
		return TEE_ERROR_BAD_PARAMETERS;

	return  copy_to_user(mr, m, sizeof(*m));
}


TEE_Result syscall_syna_verify_image(const void *in, size_t inLen, void *out,
		uint32_t *outLen, uint32_t codeType)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	const struct mem_region *mr = NULL;
	paddr_t in_pa = virt_to_phys((void *)in);
	paddr_t out_pa = virt_to_phys((void *)out);

	if (!in || !out) {
		EMSG("in/out must not be NULL\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	mr = mr_find_region(in_pa, inLen);
	if ((!mr || !TEE_MR_USER_IS_ION(mr))) {
		EMSG("bad paramter in@%p, inLen = 0x%lx.\n", in, inLen);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	cache_operation(TEE_CACHEFLUSH, (void *)in, inLen);

	ret = verify_image(in_pa, inLen, out_pa, outLen, codeType, 0);

	if (ret)
		EMSG("verify image failed, ret=%08x\n", ret);

	cache_operation(TEE_CACHEINVALIDATE, out, inLen);

	return ret;
}

TEE_Result syscall_syna_exec_cmd(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result, uint32_t result_len)
{
	return syna_exec_cmd(cmd_id, param, param_len, result, result_len);

}

TEE_Result syscall_syna_exec_cmd_ex(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result,
		uint32_t result_len, uint32_t delay)
{
	return syna_exec_cmd_ex(cmd_id, param, param_len, result, result_len, delay);
}

#ifdef CFG_TA_PERM_CHECK
static int is_rf_protected_register(paddr_t pa, size_t len)
{
	int i = 0;
	const struct rf_window *rf = rf_window_param;
	int32_t rf_num = rf_window_num;

	for (i = 0; i < rf_num; i++) {
		if (ADDR_REGION_IS_OVERLAP((uintptr_t)pa, len, rf[i].base, rf[i].mask))
			return 1;
	}

	return 0;
}
#endif
TEE_Result syscall_syna_map_register(vaddr_t *va, size_t len, unsigned long prot,
		unsigned long flags, paddr_t pa)
{

	struct ts_session *s = ts_get_current_session();
	struct user_mode_ctx *uctx = to_user_mode_ctx(s->ctx);
#ifdef CFG_TA_PERM_CHECK
	bool allow = false;
	const TEE_UUID *uuid = &s->ctx->uuid;
#endif
	TEE_Result res = TEE_SUCCESS;
	vaddr_t va_copy = 0;
	struct mobj * mobj = NULL;
	size_t sz = ROUNDUP(len, SMALL_PAGE_SIZE);
	prot = TEE_MATTR_URW | TEE_MATTR_PRW;
	flags = VM_FLAG_PERMANENT;

#ifdef CFG_TA_PERM_CHECK
	if (is_rf_protected_register(pa, len)) {
		/* check ta permission only when the register is
		 * protected by rf. otherwise, map directly
		 */
		if (ta_perm_check_mmap(uuid, pa, len, &allow) != 0 || allow == false) {
			EMSG("Don't allow map range 0x%08" PRIxPA " size 0x%08" PRIxPASZ, pa, len);
			return TEE_ERROR_ACCESS_DENIED;
		}
	}
#endif
	mobj = mobj_phys_alloc(pa, sz, TEE_MATTR_MEM_TYPE_DEV, CORE_MEM_SEC_IO);
	if (mobj == NULL) {
		EMSG("fail to alloc mobj: pa = 0x%08" PRIxPA " len = 0x%08" PRIxPASZ, pa, len);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	res = vm_map(uctx, &va_copy, sz, prot, flags, mobj, 0);
	mobj_put(mobj);
	if (!res) {
		res = PUT_USER_SCALAR(va_copy, va);
		if (res) {
			res = vm_unmap(uctx, va_copy, len);
			if (res) {
				EMSG("vm_unmap(%#"PRIxVA", %#zx) returned %#"PRIx32,
				     va_copy, sz, res);
				panic("Can't restore memory map");
			}
		}
	} else {
		EMSG("fail to map register: pa = 0x%08" PRIxPA " len = 0x%08" PRIxPASZ " res = 0x%x",
				pa, len, res);
	}

	return res;
}

TEE_Result syscall_syna_unmap_register(vaddr_t va, size_t len)
{
	TEE_Result res = TEE_SUCCESS;
	struct ts_session *sess = ts_get_current_session();
	struct user_mode_ctx *uctx = to_user_mode_ctx(sess->ctx);
	size_t sz = ROUNDUP(len, SMALL_PAGE_SIZE);
	uint32_t vm_flags = 0;
	vaddr_t end_va = 0;

	if (ADD_OVERFLOW(va, sz, &end_va))
		return TEE_ERROR_BAD_PARAMETERS;

	res = vm_get_flags(uctx, va, sz, &vm_flags);
	if (res)
		return res;
	if (vm_flags & VM_FLAG_PERMANENT)
		return TEE_ERROR_ACCESS_DENIED;

	return vm_unmap(uctx, va, len);
}

TEE_Result syscall_syna_virt_to_phys(paddr_t *pa, void *va)
{
	paddr_t pa_copy = 0;
	pa_copy = virt_to_phys(va);

	if (pa)
		*pa = pa_copy;
	if (pa_copy)
		return TEE_SUCCESS;
	else
		return TEE_ERROR_GENERIC;
}

TEE_Result syscall_syna_get_otp_raw_data(uint32_t row, uint8_t *buffer)
{
	return soc_get_otp_raw_data(row, buffer);
}


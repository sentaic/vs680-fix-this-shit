// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include "string.h"
#include "malloc.h"
#include "tz_umem_mgr.h"
#include <kernel/tee_ta_manager.h>
#include <kernel/user_ta.h>
#include "svc_syna.h"

#if 0
TEE_Result syscall_syna_allocate_tzkmemory(size_t size, uint32_t *mem_id, void **va)
{
	int ret = TEE_ERROR_GENERIC;
	void *mem = NULL;
	uint32_t umem_id = 0;
	void *umem_va = 0;
	size_t umem_sz = 0;

	umem_operation_lock();

	umem_sz = ALIGN(size, PAGE_SIZE);

	mem = umem_allocate_tzkmemory(umem_sz);
	if (!mem)
		goto fail_allocate;

	ret = umem_register_memory(mem, umem_sz, UMEM_SHAREDMEMORY, &umem_id);
	if (ret)
		goto fail_registermem;

	ret = umem_add_memory_access(umem_id, tz_sw_task_uuid(this->task_id),
				NULL, UMEM_PERM_READ | UMEM_PERM_WRITE | UMEM_PERM_GRANT, 1);
	if (ret)
		goto fail_add_mem_access;

	ret = umem_map_memory(umem_id, &umem_va, &umem_sz);
	if (ret)
		goto fail_map;

	*mem_id = umem_id;
	*va = umem_va;

	goto success;

fail_map:
	umem_remove_memory_access(umem_id, tz_sw_task_uuid(this->task_id));
fail_add_mem_access:
	umem_unregister_memory(umem_id);
fail_registermem:
	umem_free_sharedmemory((void *)mem);
fail_allocate:
success:
	umem_operation_unlock();
	return ret;
}
#endif
TEE_Result syscall_syna_register_memory(paddr_t mem, size_t size, uint32_t type, uint32_t *mem_id)
{
	int ret = TEE_ERROR_GENERIC;
	uint32_t umem_id = 0;
	struct user_ta_ctx *utc = to_user_ta_ctx(ts_get_current_session()->ctx);
	//type = UMEM_FIXED_SA;

	umem_operation_lock();

	ret = umem_check_memory_region(mem, size);
	if (ret)
		goto fail_registermem;

	ret = umem_register_memory(mem, size, type, &umem_id);
	if (ret)
		goto fail_registermem;
	ret = umem_set_owner(umem_id, utc);
	if (ret) {
		umem_unregister_memory(umem_id);
		goto fail_registermem;
	}
#if 0
	/* by default, current ta has the access permission */
	/* mainly for secure memory. for non-secure memory, every ta can access */
	ret = umem_add_memory_access(umem_id, tz_sw_task_uuid(this->task_id),
				NULL, UMEM_PERM_READ | UMEM_PERM_WRITE | UMEM_PERM_GRANT, 1);
	if (ret)
		goto fail_add_mem_access;

	ret = umem_add_memory_access_2_whitelist(umem_id);
	if (ret)
		goto fail_add_mem_access;
#endif
	*mem_id = umem_id;

	goto success;

//fail_add_mem_access:
	umem_unregister_memory(umem_id);
fail_registermem:
success:
	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_unregister_memory(uint32_t mem_id)
{
	int ret = TEE_ERROR_GENERIC;

	umem_operation_lock();

	ret = umem_unregister_memory(mem_id);

	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_add_memory_access(uint32_t mem_id,
			TEE_UUID *uuid, void *sa, uint32_t permission)
{
	int ret = TEE_ERROR_GENERIC;

	umem_operation_lock();

	ret = umem_add_memory_access(mem_id, uuid, sa, permission, 0);

	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_map_memory(uint32_t mem_id, void **va, size_t *size)
{
	struct ts_session *s = ts_get_current_session();
	struct user_ta_ctx *utc = NULL;
	TEE_Result ret = TEE_SUCCESS;

	utc = to_user_ta_ctx(s->ctx);

	umem_operation_lock();

	ret = umem_map_memory(utc, mem_id, va, size);

	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_unmap_memory(uint32_t mem_id)
{
	struct ts_session *s = ts_get_current_session();
	struct user_ta_ctx *utc = NULL;
	TEE_Result ret = TEE_SUCCESS;

	utc = to_user_ta_ctx(s->ctx);

	umem_operation_lock();

	ret = umem_unmap_memory(utc, mem_id);

	if (ret)
		goto out;

	if (UMEM_TYPE(mem_id) == UMEM_SHAREDMEMORY) {
		/* try to unregister */
		ret = umem_unregister_memory(mem_id);
		/* if return busy, only mean ref count not equal 0 */
		if (ret == TEE_ERROR_BUSY)
			ret = TEE_SUCCESS;
	}

out:
	umem_operation_unlock();
	return ret;
}


TEE_Result syscall_syna_get_physical_by_memid(uint32_t mem_id, paddr_t *phy_addr)
{
	int ret = 0;

	umem_operation_lock();

	ret = umem_get_physical_by_memid(mem_id, phy_addr);

	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_get_memid_by_phys(uint32_t *mem_id, paddr_t phy_addr, size_t size)
{
	int ret = 0;

	umem_operation_lock();

	ret = umem_get_memid_by_phys(mem_id, phy_addr, size);

	umem_operation_unlock();
	return ret;

}

bool syscall_syna_check_memattr_by_memid(uint32_t mem_id, uint32_t attr)
{
	bool ret = false;

	umem_operation_lock();

	ret = umem_check_mem_attr(mem_id, attr);

	umem_operation_unlock();
	return ret;
}

bool sys_mem_validate_caller(uint32_t mem_id)
{
	bool ret = false;

	umem_operation_lock();

	ret = umem_validate_caller(mem_id);

	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_mem_bind_hw_virtaddr(uint32_t mem_id ,void *hw_virt)
{
	int ret = TEE_ERROR_GENERIC;

	umem_operation_lock();
	ret = umem_bind_hw_virtaddr(mem_id, hw_virt);
	umem_operation_unlock();

	return ret;
}

TEE_Result syscall_syna_mem_get_hw_virtaddr(uint32_t mem_id ,void **phw_virt)
{
	int ret = TEE_ERROR_GENERIC;

	umem_operation_lock();
	ret = umem_get_hw_virtaddr(mem_id, phw_virt);
	umem_operation_unlock();

	return ret;
}

TEE_Result syscall_syna_refcnt_op(uint32_t mem_id, uint32_t type, uint32_t *refc)
{
	struct ts_session *s = ts_get_current_session();
	struct user_ta_ctx *utc = NULL;
	TEE_Result ret = TEE_SUCCESS;

	utc = to_user_ta_ctx(s->ctx);


	umem_operation_lock();

	ret = umem_refcnt_op(utc, mem_id, type, refc);

	umem_operation_unlock();
	return ret;
}
#if 0
TEE_Result syscall_syna_attach_said(uint32_t mem_id, uint32_t sa_id)
{
	return 0;
}

TEE_Result syscall_syna_detach_said(uint32_t mem_id)
{
	return 0;
}
#endif
void sys_dump_umemlist(void)
{
	/* debug purpose */
	umem_dump();
}

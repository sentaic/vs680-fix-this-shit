// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <stdlib.h>
#include <string.h>
#include <string_ext.h>
#include <tee_api.h>
#include <tee_internal_api_extensions.h>
#include <types_ext.h>
#include <user_ta_header.h>
#include "utee_syscalls_syna.h"
#include "tee_api_private.h"
#include "tee_api_syna.h"

TEE_Result TEE_GetMemRegionList(TEE_MemRegion *region, uint32_t maxNum,
		uint32_t attrMask, uint32_t attrVal)
{
	if (!region || maxNum == 0) {
		EMSG("invalid parameters. region=0x%p, maxNum=%d\n",
				region, maxNum);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return _utee_syna_mr_filter(region, &maxNum, attrMask, attrVal);
}

uint32_t TEE_GetMemRegionCount(uint32_t attrMask, uint32_t attrVal)
{
	uint32_t maxNum = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = _utee_syna_mr_filter(NULL, &maxNum, attrMask, attrVal);
	if (ret != TEE_SUCCESS)
		return ret;

	return maxNum;
}

TEE_Result TEE_FindMemRegion(TEE_MemRegion *region,
		void *buffer, uint32_t size)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!buffer || !size || !region) {
		EMSG("invalid parameters. buffer=0x%p, size=0x%08x, region=0x%p\n",
				buffer, size, region);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	result = _utee_syna_mr_find_region(buffer, size, region);
	if (TEE_SUCCESS != result) {
		EMSG("can't find memory region, buffer=0x%p, size=0x%08x\n",
				buffer, size);
		return result;
	}

	return TEE_SUCCESS;
}

TEE_Result TEE_ExecCmd(size_t cmd_id, const void *param,
		size_t param_len, const void *status, size_t status_len)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!param || !status)
		return TEE_ERROR_BAD_PARAMETERS;

	/* call secure processor execute command */
	result = _utee_syna_exec_cmd(cmd_id, param, param_len,
			status, status_len);
	return result;
}

TEE_Result TEE_ExecCmdEx(size_t cmd_id, const void *param,
		size_t param_len, const void *status, size_t status_len, uint32_t delay)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!param || !status)
		return TEE_ERROR_BAD_PARAMETERS;

	/* call secure processor execute command */
	result = _utee_syna_exec_cmd_ex(cmd_id, param, param_len,
			status, status_len, delay);
	return result;
}

TEE_Result TEE_VerifyImage(const void *in, uint32_t inLen, void *out,
		uint32_t *outLen, uint32_t codeType)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!in || !out || !outLen)
		return TEE_ERROR_BAD_PARAMETERS;

	result = _utee_syna_verify_image(in, inLen, out, outLen, codeType);

	return result;
}

TEE_Result TEE_Map_Register(paddr_t pa, size_t len, uint32_t prot,
		uint32_t flags, void **va)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!va) {
		EMSG("invalid va buffer\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	result = _utee_syna_map_register(va, len, prot, flags, pa);

	return result;
}

TEE_Result TEE_MemMap(paddr_t pa, size_t len, uint32_t prot,
		uint32_t flags, void **va)
{
	return TEE_Map_Register(pa, len, prot, flags, va);
}

TEE_Result TEE_Unmap_Register(void *va, size_t len)
{
	if (_utee_syna_unmap_register(va, len))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

TEE_Result TEE_MemUnmap(void *va, size_t len)
{
	return TEE_Unmap_Register(va, len);
}

paddr_t TEE_VirtToPhys(void *va)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	unsigned long pa;

	result = _utee_syna_virt_to_phys(&pa, va);
	if (TEE_SUCCESS != result) {
		return 0;
	}

	return pa;
}

TEE_Result TEE_GetOtpRawData(uint32_t row, uint8_t *buffer)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!buffer)
		return TEE_ERROR_BAD_PARAMETERS;

	result = _utee_syna_get_otp_raw_data(row, buffer);

	return result;
}

enum {
	/* Memory from TZK heap */
	UMEM_SHAREDMEMORY = 0x40000000,
	/*
	 * The secure attribute of memory is determined by mpt on runtime
	 * It's allocated from system heap or CMA
	 */
	UMEM_DYNAMIC_SA = 0x20000000,
	/*
	 * The secure attribute of memory is fixed. It's allocated from
	 * szc memory zone (SA is up to memory zone) or CMA(non-secure).
	 */
	UMEM_FIXED_SA = 0x10000000,
};

TEE_Result  TEE_Register_Memory(paddr_t mem, size_t size, uint32_t *mem_id)
{
	return _utee_syna_register_memory(mem, size, UMEM_FIXED_SA, mem_id);
}

TEE_Result TEE_Unregister_Memory(uint32_t mem_id)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = _utee_syna_unregister_memory(mem_id);

	return ret;
}

TEE_Result TEE_Map_Memory(uint32_t mem_id, void **va, size_t *size)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	void *mem_va = 0;
	size_t mem_sz = 0;

	ret = _utee_syna_map_memory(mem_id, &mem_va, &mem_sz);
	if (ret)
		return TEE_ERROR_ACCESS_DENIED;

	*va = mem_va;

	if (size)
		*size = mem_sz;

	return ret;
}

TEE_Result TEE_Unmap_Memory(uint32_t mem_id)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = _utee_syna_unmap_memory(mem_id);

	if (ret)
		return TEE_ERROR_BAD_PARAMETERS;

	return ret;
}

TEE_Result TEE_GetPhys_By_Memid(uint32_t mem_id, paddr_t *phy_addr)
{
	if (_utee_syna_get_phy_by_memid(mem_id, phy_addr))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

bool TEE_Check_Memory_Attr(uint32_t mem_id, uint32_t attr)
{
	return _utee_syna_check_memattr_by_memid(mem_id, attr);
}

TEE_Result TEE_GetMemid_By_Phys(uint32_t *mem_id, paddr_t phy_addr, uint32_t size)
{
	return _utee_syna_get_memid_by_phys(mem_id, phy_addr, size);
}


TEE_Reservation *TEE_CreateReservation(void *s_list, unsigned int rule)
{
	TEE_Reservation *r = NULL;
	unsigned int size = 0;

	if (!s_list)
		return NULL;

	size = SIZE_OF_SCATTERLIST(((scatter_list_t *)s_list)->num);
	if (size <= sizeof(unsigned int))
		return NULL;

	r = TEE_Malloc(sizeof(TEE_Reservation), 0);
	if (!r)
		return NULL;

	r->s_list = (scatter_list_t *)TEE_Malloc(size, 0);
	if (!(r->s_list)) {
		TEE_Free(r);
		return NULL;
	}

	memcpy(r->s_list, s_list, size);
	r->a = rule;

	return r;
}

void TEE_DestroyReservation(TEE_Reservation *r)
{
	if (!r)
		return;

	if (!(r->s_list))
		return;

	TEE_Free(r->s_list);
	TEE_Free(r);
}

enum {
	UMEM_SGLIST_NUM = 0x10,
	UMEM_SGLIST_BUF = 0x20,
	UMEM_SGLIST_RULE = 0x40
};

enum {
	UMEM_REFC_VALUE = 0x100,
	UMEM_REFC_INC = 0x200,
	UMEM_REFC_DEC = 0x400
};

TEE_Result TEE_ReserveSecurePages(TEE_Reservation *r)
{
	if (!r || !r->s_list)
		return TEE_ERROR_BAD_PARAMETERS;

	return _utee_syna_reserve_reservation(r);
}

TEE_Result TEE_ReleaseSecurePages(TEE_Reservation *r)
{
	if (!r || !r->s_list)
		return TEE_ERROR_BAD_PARAMETERS;

	return _utee_syna_release_reservation(r);
}

TEE_Result TEE_Mem_GetSglistNum(uint32_t mem_id, uint32_t *num)
{
	return _utee_syna_reservation_get_info(mem_id, UMEM_SGLIST_NUM, (void *)num, 4);
}

TEE_Result TEE_Mem_GetSglist(uint32_t mem_id, scatter_list_t *sgt, uint32_t size)
{
	return _utee_syna_reservation_get_info(mem_id, UMEM_SGLIST_BUF, (void *)sgt, size);
}

TEE_Result TEE_Reservation_GetAttribute(uint32_t mem_id, uint32_t *rule)
{
	return _utee_syna_reservation_get_info(mem_id, UMEM_SGLIST_RULE, (void *)rule, 4);
}

TEE_Result TEE_Mem_GetRefCnt(uint32_t mem_id, uint32_t *refc)
{
	return _utee_syna_refcnt_op(mem_id, UMEM_REFC_VALUE, refc);
}

TEE_Result TEE_MemRef(uint32_t mem_id)
{
	return _utee_syna_refcnt_op(mem_id, UMEM_REFC_INC, NULL);
}

TEE_Result TEE_MemUnref(uint32_t mem_id)
{
	return _utee_syna_refcnt_op(mem_id, UMEM_REFC_DEC, NULL);
}

static uint64_t TEE_TimeToNS(TEE_Time* time)
{
	uint64_t ns = time->seconds * 1000000000ULL;
	ns += time->millis * 1000000ULL;
	return ns;
}

void TEE_GetSystemTimeNS(uint64_t *ns)
{
	TEE_Time time;

	TEE_GetSystemTime(&time);
	if (ns)
		*ns = TEE_TimeToNS(&time);
}


TEE_Result set_globaldata(uint32_t key, uint32_t value)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_set_globaldata(key, value);

	return result;
}

TEE_Result get_globaldata(uint32_t key, uint32_t *value)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_get_globaldata(key, value);

	return result;
}


/** Memory Transfter Permission Check.
 *
 * It is for memory tansfer policy control.
 *
 * @param dstPerm	destination memory access permission.
 * @param srcPerm	source memory access permission.
 * @param op		Operation, see TEE_MemoryOperation.
 *
 * @retval TEE_SUCCESS			Pass the check.
 * @retval TEE_ERROR_ACCESS_CONFLICT	Can't pass the check, master must not
 *					issue the transfer.
 */
static TEE_Result TEE_CheckMemoryTransferPerm(
		uint32_t		dstPerm,
		uint32_t		srcPerm,
		TEE_MemoryOperation	op)
{
	/* check source: must be secure readable */
	if (TZ_IS_SNR(srcPerm)) {
		EMSG("source must NOT be restricted domain\n");
		return TEE_ERROR_ACCESS_CONFLICT;
	}

	/* check destination: must be secure writeable */
	if (TZ_IS_SNW(dstPerm)) {
		EMSG("destination must NOT be restricted domain\n");
		return TEE_ERROR_ACCESS_CONFLICT;
	}

	switch (op) {
	case TEE_MT_ENCRYPT:
		/* ok to output encrypted data to secure or non-secure
		 * domain.
		 */
		break;
	case TEE_MT_DECRYPT:
		if (TZ_IS_NR(dstPerm)) {
			EMSG("can't decrypt to non-secure readable domain\n");
			return TEE_ERROR_ACCESS_CONFLICT;
		}
		break;
	default:
		/* must NOT copy secure data (non-secure non-readable) to
		 * non-secure readable region
		 */
		if (TZ_IS_NNR(srcPerm) && TZ_IS_NR(dstPerm)) {
			EMSG("can't copy secure data to non-secure readable "
					"domain\n");
			return TEE_ERROR_ACCESS_CONFLICT;
		}
	}

	return TEE_SUCCESS;
}

TEE_Result TEE_CheckMemoryTransferRights(
		const void*		dst,
		uint32_t		dstLen,
		const void*		src,
		uint32_t		srcLen,
		TEE_MemoryOperation	op)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	struct mem_region s;
	struct mem_region d;

	result = _utee_syna_mr_find_region(src, srcLen, &s);
	if (TEE_SUCCESS != result) {
		EMSG("can't find region for source "
				"base=%p, len=0x%08x\n", src, srcLen);
		return result;
	}

	result = _utee_syna_mr_find_region(dst, dstLen, &d);
	if (TEE_SUCCESS != result) {
		EMSG("can't find region for destination "
				"base=%p, len=0x%08x\n", dst, dstLen);
		return result;
	}

	return TEE_CheckMemoryTransferPerm(d.attr, s.attr, op);
}

TEE_Result TEE_CheckMemoryInputRights(
		const void*		src,
		uint32_t		srcLen,
		uint32_t		dstPerm,
		TEE_MemoryOperation	op)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	struct mem_region s;

	result = _utee_syna_mr_find_region(src, srcLen, &s);

	if (TEE_SUCCESS != result) {
		EMSG("can't find region for source "
				"base=%p, len=0x%08x\n", src, srcLen);
		return result;
	}

	return TEE_CheckMemoryTransferPerm(dstPerm, s.attr, op);

}

TEE_Result TEE_CheckMemoryOutputRights(
		const void*		dst,
		uint32_t		dstLen,
		uint32_t		srcPerm,
		TEE_MemoryOperation	op)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	struct mem_region d;

	result = _utee_syna_mr_find_region(dst, dstLen, &d);
	if (TEE_SUCCESS != result) {
		EMSG("can't find region for destination "
				"base=%p, len=0x%08x\n", dst, dstLen);
		return result;
	}

	return TEE_CheckMemoryTransferPerm(d.attr, srcPerm, op);

}

TEE_Result TEE_MutexCreateExt(TEE_MutexHandle *h)
{
	_utee_syna_mutex_alloc(h);

	return TEE_SUCCESS;
}

TEE_Result TEE_MutexDestroyExt(TEE_MutexHandle h)
{
	_utee_syna_mutex_destroy((int)h);
	return TEE_SUCCESS;
}

TEE_Result TEE_MutexLockExt(TEE_MutexHandle h)
{
	return _utee_syna_mutex_lock((int)h);
}

TEE_Result TEE_MutexTryLockExt(TEE_MutexHandle h)
{
	if (_utee_syna_mutex_trylock((int)h))
		return TEE_SUCCESS;
	else
		return TEE_ERROR_BUSY;
}

TEE_Result TEE_MutexUnlockExt(TEE_MutexHandle h)
{
	_utee_syna_mutex_unlock((int)h);
	return TEE_SUCCESS;
}


TEE_Result TEE_MutexCreate(void **lock, const char *name __unused)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if (!lock)
		return TEE_ERROR_BAD_PARAMETERS;

	result = TEE_MutexCreateExt((TEE_MutexHandle*)(lock));

	return result;
}

TEE_Result TEE_MutexDestroy(void *lock)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	result = TEE_MutexDestroyExt((TEE_MutexHandle)(uintptr_t)lock);

	return result;
}

TEE_Result TEE_MutexLock(void *lock)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	result = TEE_MutexLockExt((TEE_MutexHandle)(uintptr_t)lock);

	return result;
}

TEE_Result TEE_MutexTryLock(void *lock)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	result = TEE_MutexTryLockExt((TEE_MutexHandle)(uintptr_t)lock);

	return result;
}

TEE_Result TEE_MutexUnlock(void *lock)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	result = TEE_MutexUnlockExt((TEE_MutexHandle)(uintptr_t)lock);

	return result;
}

TEE_Result TEE_SemaphoreCreate(void **sem, int value, const char *name __unused)
{
	if (!sem)
		return TEE_ERROR_BAD_PARAMETERS;

	*(int*)sem = _utee_syna_semaphore_alloc(value);

	return TEE_SUCCESS;
}

TEE_Result TEE_SemaphoreTimedWait(void *sem, uint32_t timeout __unused)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_semaphore_timewait((int)(uintptr_t)sem, 0);

	return result;
}

TEE_Result TEE_SemaphoreDestroy(void *sem)
{
	_utee_syna_semaphore_destroy((int)(uintptr_t)sem);

	return TEE_SUCCESS;
}

TEE_Result TEE_SemaphoreWait(void *sem)
{
	return _utee_syna_semaphore_wait((int)(uintptr_t)sem);
}

TEE_Result TEE_SemaphoreTryWait(void *sem)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_semaphore_trywait((int)(uintptr_t)sem);
	if (TEE_SUCCESS != result)
		result = TEE_ERROR_ACCESS_CONFLICT;

	return result;
}

TEE_Result TEE_SemaphorePost(void *sem)
{
	_utee_syna_semaphore_post((int)(uintptr_t)sem);

	return TEE_SUCCESS;
}


TEE_Result getSecureKey(struct SecureKeyParam* sk)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_get_securekey((void *)sk);

	return result;
}

TEE_Result getSecureKey2(struct SecureKeyParam* sk)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_get_securekey2((void *)sk);

	return result;
}

TEE_Result getSecurityAttributeEntry(struct SecurityAttribute* sessionEntry,
				uint32_t keySessionId)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_get_sa_entry(sessionEntry, keySessionId);

	return result;
}

TEE_Result setSecurityAttritbuteEntry(struct SecurityAttribute* sessionEntry)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_set_sa_entry(sessionEntry);

	return result;
}

uint32_t getActiveEntries(void)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	int sa_num = 0;

	result = _utee_syna_get_active_entries(&sa_num);
	if (result == TEE_SUCCESS)
		return sa_num;
	else
		return -1;
}

TEE_Result getActiveSecurityAttributeEntry(struct SecurityAttribute*
				sessionEntry)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	result = _utee_syna_get_active_sa_entry(sessionEntry);

	return result;
}


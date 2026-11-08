// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include <string.h>
#include <malloc.h>
#include "tz_granule_table.h"
#include "mr_setup.h"
#include "mem_region_userdata.h"
#include "tz_umem_mgr.h"
#include "addr_range_check.h"
#include "syna_setup.h"
#include <kernel/mutex.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <trace.h>
#include <mm/mobj.h>
#include <mm/vm.h>
#include <kernel/tee_ta_manager.h>
#include <initcall.h>
#include <kernel/tee_misc.h>
#include <tee/entry_std.h>

#ifndef DEBUG_UMEM
#define DEBUG_UMEM 0
#endif

#if DEBUG_UMEM
#define debug_print(...) IMSG_RAW(__VA_ARGS__)
#else
#define debug_print(...) ((void)0)
#endif


/* unified memory management */

/* use array in order to find the entry quickly */
#define MAXNUM_UMEM (1024)
static struct umem *umem_list[MAXNUM_UMEM];
static int umem_cnt;
static struct mutex h;

#define SHM_ID(x) (x | UMEM_SHAREDMEMORY)
#define SMM_MPT_ID(x) (x | UMEM_DYNAMIC_SA)
#define SMM_CON_ID(x) (x | UMEM_FIXED_SA)
#define ID_2_INDEX(x) (x & (MAXNUM_UMEM - 1))

#define WRONG_MMU_ATTRIBUTE (0xFFFF)

#define TA_DEBUG_UUID	{0x1316a183, 0x894d, 0x43fe,\
			{0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x23}}

/* TA whose UUID is in whitelist can access memory freely */
static const TEE_UUID UMEM_ACCESS_WHITELIST[] = {TA_DEBUG_UUID};

/* find a valid umem id */
static int umem_next_id(uint32_t type, int *mem_id)
{
	int umid = 0;
	int idx = 0;
	bool found = false;

	if (!mem_id)
		return TEE_ERROR_BAD_PARAMETERS;

	for (idx = 0; idx < MAXNUM_UMEM; idx++) {
		if (umem_list[idx] == NULL) {
			found = true;
			break;
		}
	}

	/* must find a slot, otherwise increase the array */
	if(found != true)
		return TEE_ERROR_ITEM_NOT_FOUND;

	switch (type) {
	default:
		umid = idx;
		break;
	case UMEM_SHAREDMEMORY:
		umid = SHM_ID(idx);
		break;
	case UMEM_DYNAMIC_SA:
		umid = SMM_MPT_ID(idx);
		break;
	case UMEM_FIXED_SA:
		umid = SMM_CON_ID(idx);
		break;
	}

	*mem_id = umid;

	return TEE_SUCCESS;
}

static struct umem *umem_find(int umid)
{
	return umem_list[ID_2_INDEX(umid)];
}

static void print_uuid(const TEE_UUID *uuid __unused)
{
	debug_print("%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x\n", uuid->timeLow,
		uuid->timeMid, uuid->timeHiAndVersion,
		uuid->clockSeqAndNode[0],
		uuid->clockSeqAndNode[1],
		uuid->clockSeqAndNode[2],
		uuid->clockSeqAndNode[3],
		uuid->clockSeqAndNode[4],
		uuid->clockSeqAndNode[5],
		uuid->clockSeqAndNode[6],
		uuid->clockSeqAndNode[7]);
}

static struct umem_ta_perm *umem_find_perm(struct umem *um, const TEE_UUID *uuid)
{
	struct umem_ta_perm *taperm = NULL;
	bool found = false;

	LIST_FOREACH(taperm, &um->perm, list) {
		print_uuid((TEE_UUID *)uuid);
		print_uuid(&taperm->uuid);
		if (memcmp(uuid, &taperm->uuid, sizeof(TEE_UUID)) == 0) {
			found = true;
			break;
		}
	}

	return found ? taperm : NULL;
}

static struct umem_user *umem_find_user(struct umem *um, struct user_ta_ctx *utc)
{
	struct umem_user *user = NULL;

	LIST_FOREACH(user, &um->user, list) {
		if (utc == user->context)
			break;
	}

	return user;
}

static void umem_free_taperm_list(struct umem *um)
{
	struct umem_ta_perm *taperm = LIST_FIRST(&um->perm);

	while (taperm) {
		LIST_REMOVE(taperm, list);
		free(taperm);
		taperm = LIST_FIRST(&um->perm);
	}
}

static void umem_dump_slist(scatter_list_t *s_list)
{
	uint32_t i = 0;
	IMSG("#  scatter num = %d\n", s_list->num);
	for (i = 0; i < s_list->num; i++)
		IMSG("#  %x, %d\n", s_list->s_item[i].addr, s_list->s_item[i].npage);
}

static void umem_dump_user(struct umem_user *user)
{
	IMSG("#TA %d: ", user->task_id);
	IMSG("#    %08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x\n",
		user->uuid.timeLow,
		user->uuid.timeMid,
		user->uuid.timeHiAndVersion,
		user->uuid.clockSeqAndNode[0],
		user->uuid.clockSeqAndNode[1],
		user->uuid.clockSeqAndNode[2],
		user->uuid.clockSeqAndNode[3],
		user->uuid.clockSeqAndNode[4],
		user->uuid.clockSeqAndNode[5],
		user->uuid.clockSeqAndNode[6],
		user->uuid.clockSeqAndNode[7]);
}

static void umem_dump_perm(const struct umem_ta_perm *taperm __unused)
{
	IMSG("#    %08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x: 0x%x\n",
		taperm->uuid.timeLow,
		taperm->uuid.timeMid,
		taperm->uuid.timeHiAndVersion,
		taperm->uuid.clockSeqAndNode[0],
		taperm->uuid.clockSeqAndNode[1],
		taperm->uuid.clockSeqAndNode[2],
		taperm->uuid.clockSeqAndNode[3],
		taperm->uuid.clockSeqAndNode[4],
		taperm->uuid.clockSeqAndNode[5],
		taperm->uuid.clockSeqAndNode[6],
		taperm->uuid.clockSeqAndNode[7],
		taperm->perm);
}

static void umem_dump_umem(const struct umem *um)
{
	struct umem_ta_perm *taperm = NULL;
	struct umem_user *user = NULL;

	IMSG("#mem_id = %x\n", um->umid);
	IMSG("#type = %x\n", um->type);
	IMSG("#size = %lx\n", um->size);
	IMSG("#refcnt = %x\n", um->refcnt);
	IMSG("#va = %lx\n", um->vm_start);
	IMSG("#mt = %x\n", um->mt);
	IMSG("#rule = %x\n", um->rule);
	umem_dump_slist(um->s_list);

	LIST_FOREACH(taperm, &um->perm, list)
		umem_dump_perm(taperm);

	LIST_FOREACH(user, &um->user, list)
		umem_dump_user(user);
}

void umem_dump(void)
{
	int i = 0;

	IMSG("###total %d\n", umem_cnt);

	for (i = 0; i < MAXNUM_UMEM; i++) {
		if (umem_list[i])
			umem_dump_umem(umem_list[i]);
	}
}

struct umem *umem_find_by_address(void *addr, size_t size)
{
	int i = 0;
	struct umem *um = NULL;

	for (i = 0; i < MAXNUM_UMEM; i++) {
		um = umem_list[i];
		if (um && ADDR_REGION_IN_RANGE((uintptr_t)addr, size, (uintptr_t)um->vm_start, um->size))
			break;
	}

	return um;
}

static const struct umem *umem_find_by_phys(uintptr_t phy, size_t size)
{
	int i = 0;
	const struct umem *um = NULL;
	uintptr_t s_pa;
	size_t s_size;

	for (i = 0; i < MAXNUM_UMEM; i++) {
		um = umem_list[i];
		if (um && (1 == um->s_list->num)) {
			s_pa = um->s_list->s_item[0].addr;
			s_size = (um->s_list->s_item[0].npage << SMALL_PAGE_SHIFT);
			if(ADDR_REGION_IN_RANGE(phy, size, s_pa, s_size)) {
				break;
			}
		}
	}

	return um;
}

int umem_check_memory_permission(uint32_t mem_id, const TEE_UUID *uuid, uint32_t perm)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;
	struct umem_ta_perm *taperm = NULL;
	uint32_t thisperm = 0;

	um = umem_find(mem_id);
	if (!um)
		goto out;

	/* if non-secure memory, always pass */
	if (um->mt & MT_M_NONSECURE)
		return TEE_SUCCESS;

	/* check permission */
	taperm = umem_find_perm(um, uuid);
	if (!taperm) {
		EMSG("no access permission to the memory\n");
		ret = TEE_ERROR_ACCESS_DENIED;
		umem_dump_umem(um);
		goto out;
	}
	thisperm = taperm->perm;

	/* support read or read+write */
	if (!(thisperm & perm)) {
		EMSG("no access permission to the memory\n");
		ret = TEE_ERROR_ACCESS_DENIED;
		umem_dump_umem(um);
	}

	ret = TEE_SUCCESS;

out:
	return ret;
}

static uint32_t umem_get_mmu_type_from_mz(struct umem *um)
{
	const struct mem_region *mr = NULL;

	mr = mr_find_region(um->s_list->s_item[0].addr, um->size);
	if (NULL == mr) {
		EMSG("memory(0x%lx, 0x%lx) is not in any memory region!!!\n",
			(unsigned long)um->s_list->s_item[0].addr, (unsigned long)(um->s_list->s_item[0].addr + um->size));
		return WRONG_MMU_ATTRIBUTE;
	}

	if (!(TEE_MR_USER_IS_ION(mr) || TEE_MR_USER_IS_SYSTEM(mr)
		|| TEE_MR_USER_IS_PT(mr) || TEE_MR_USER_IS_VPP(mr))) {
		EMSG("memory(0x%lx, 0x%lx) is not in ION/System/PT/VPP memory region!!!\n",
			(unsigned long)um->s_list->s_item[0].addr, (unsigned long)(um->s_list->s_item[0].addr + um->size));
		return WRONG_MMU_ATTRIBUTE;
	}

	if (MR_IS_NONSECURE(mr)) {
		if (MR_IS_UNCACHEABLE(mr)) {
			um->attr = UMEM_NONSECURE | UMEM_NONCACHEABLE | UMEM_ION;
			return MT_USER_UNCACHED_NS;
		}
		if (MR_IS_CACHEABLE(mr)) {
			um->attr = UMEM_NONSECURE | UMEM_CACHEABLE | UMEM_ION;
			return MT_USER_CACHED_NS;
		}
	}

	um->attr = UMEM_SECURE | UMEM_CACHEABLE | UMEM_ION;
	return MT_USER_CACHED_S;
}
#if 0
static uint32_t umem_quick_get_mmu_type_from_mz(void *buff, size_t size)
{
	const struct mem_region *mr = NULL;

	mr = mr_find_region(buff, size);
	if (NULL == mr) {
		EMSG("memory(0x%x, 0x%x) is not in any memory region!!!\n",
			buff, buff + size);
		return WRONG_MMU_ATTRIBUTE;
	}

	if (MR_IS_NONSECURE(mr)) {
		if (MR_IS_UNCACHEABLE(mr)) {
			return MT_USER_UNCACHED_NS;
		}
		if (MR_IS_CACHEABLE(mr)) {
			return MT_USER_CACHED_NS;
		}
	}

	return MT_USER_CACHED_S;
}

#endif
static uint32_t umem_get_mmu_type(struct umem *um)
{
	uint32_t mt = WRONG_MMU_ATTRIBUTE;

	switch (um->type) {
	case UMEM_SHAREDMEMORY:
		mt = MT_USER_CACHED_S;
		um->attr = UMEM_SECURE | UMEM_CACHEABLE | UMEM_RESTRICTED;
		break;
	case UMEM_DYNAMIC_SA:
		um->attr = UMEM_CACHEABLE | UMEM_SYSTEM;

		if (um->s_list->num == 1) {
			paddr_t buff = um->s_list->s_item[0].addr;
			uint32_t size = um->s_list->s_item[0].npage * SMALL_PAGE_SIZE;

			if (TEE_SUCCESS == umem_check_memory_region(buff, size))
				um->attr |= UMEM_ION;
		}

		if (um->rule) {
			mt = MT_USER_CACHED_S;
			um->attr |= UMEM_SECURE;
		} else {
			mt = MT_USER_CACHED_NS;
			um->attr |= UMEM_NONSECURE;
		}
		break;
	case UMEM_FIXED_SA:
		mt = umem_get_mmu_type_from_mz(um);
		break;
	default:
		mt = MT_USER_CACHED_S;
	}

	return mt;
}
void umem_operation_lock(void)
{
	mutex_lock(&h);
}

void umem_operation_unlock(void)
{
	mutex_unlock(&h);
}

int umem_check_memory_region(paddr_t mem, size_t size)
{
	const struct mem_region *mr = mr_find_region(mem, size);

	if (!mr)
		return TEE_ERROR_ACCESS_DENIED;

	if ((!TEE_MR_USER_IS_ION(mr)) && (!TEE_MR_USER_IS_PT(mr)) && (!TEE_MR_USER_IS_VPP(mr)))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}
#if 0
//#ifdef CFG_CORE_DYN_SHM
static void register_um_shm(paddr_t buf, size_t sz,  uint64_t memid, bool map_buffer)
{
}

static void unregister_um_shm(struct optee_msg_arg *arg, uint32_t num_params)
{
	if (num_params == 1) {
		uint64_t cookie = arg->params[0].u.rmem.shm_ref;
		TEE_Result res = mobj_reg_shm_release_by_cookie(cookie);

		if (res)
			EMSG("Can't find mapping with given cookie");
		arg->ret = res;
	} else {
		arg->ret = TEE_ERROR_BAD_PARAMETERS;
		arg->ret_origin = TEE_ORIGIN_TEE;
	}
}
#endif /*CFG_CORE_DYN_SHM*/

static bool __maybe_unused umem_from_mobj(struct umem *umem, struct mobj *mobj,
					       const paddr_t pa,
					       const size_t sz)
{
	paddr_t b;

	/* Optional boot regions have a memory object when configured. */
	if (!mobj)
		return false;

	debug_print("pa = 0x%lx, sz = 0x%lx.\n", pa, sz);

	if (mobj_get_pa(mobj, 0, 0, &b) != TEE_SUCCESS)
		panic("mobj_get_pa failed");
	debug_print("mobj pa = 0x%lx, sz = 0x%lx.\n", b, mobj->size);

	if (!core_is_buffer_inside(pa, MAX(sz, 1UL), b, mobj->size))
		return false;

	umem->mobj = mobj_get(mobj);
	umem->offs = pa - b;
	umem->size = sz;

	debug_print("Find mobj for : pa = 0x%lx, sz = 0x%lx, successfully.\n", pa, sz);
	return true;
}

static bool umem_find_mobj(struct umem *um, scatter_list_t *s_list)
{
	struct mobj **mobj __maybe_unused = NULL;
	paddr_t *pages = NULL;
	int npage = 0;
	unsigned int i, j;
	paddr_t pa;
	size_t sz = 0;
	int id = 0;
	bool ret = true;

	debug_print("Find mobj for memid = 0x%x, type = 0x%x.\n", um->umid, um->type);

	if (!um || !s_list || !um->size)
		return false;

	pa = s_list->s_item[0].addr;
	sz = um->size;

	if (um->type != UMEM_DYNAMIC_SA) {
#ifdef CFG_SECURE_DATA_PATH
		/* Handle memory reference to Secure Data Path memory areas */
		for (mobj = sdp_mem_mobjs; *mobj; mobj++)
			if (umem_from_mobj(um, *mobj, pa, sz))
				return true;
#endif

		if (umem_from_mobj(um, syna_nsec_nc_mobj, pa, sz))
			return true;

		if (umem_from_mobj(um, nsec_mobj, pa, sz))
			return true;

		if (umem_from_mobj(um, syna_pt_mobj, pa, sz))
			return true;

		if (umem_from_mobj(um, syna_vpp_mobj, pa, sz))
			return true;
	}

	npage = sz / SMALL_PAGE_SIZE;
	pages = malloc(sizeof(paddr_t) *npage);
	debug_print("slist num = 0x%x, sl_size = 0x%x.\n", s_list->num, sz);
	memset(pages, 0, npage * sizeof(paddr_t));
	for (i = 0; i < s_list->num; i++) {
		for (j = 0; j < s_list->s_item[i].npage; j++) {
			pages[id++] = s_list->s_item[i].addr + j * SMALL_PAGE_SIZE;
		}
	}

	debug_print("npages = 0x%x, id = 0x%x.\n", npage, id);
	um->mobj =  mobj_reg_shm_alloc(pages, npage, 0, um->umid);
	if (um->mobj == NULL) {
		EMSG("fail to allocate shm mobj.\n");
		ret = false;
		goto out;
	}

	mobj_reg_shm_unguard(um->mobj);
	ret = true;
out:
	if (pages)
		free(pages);

	return ret;
}

/*
 * @param(input) mem: buffer pointer of contigious memory or scatter list structure
 *                    of non-contigious memory
 * @param(input) size: total size in Bytes
 * @param(input) type: memory type. it can be UMEM_SHAREDMEMORY, UMEM_DYNAMIC_SA,
 *                     UMEM_FIXED_SA
 * @param(output) mem_id: return a mem_id
 */
int umem_register_memory(paddr_t mem, size_t size,
				uint32_t type, uint32_t *mem_id)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;
	scatter_list_t *s_list = NULL;
	int sl_size = 0;
	bool is_set_gt = false;
	int npage = 0;

	debug_print("mem = 0x%lx, size = 0x%lx, type = 0x%x.\n", mem, size, type);

	um = malloc(sizeof(struct umem));
	if (!um) {
		EMSG("tzk heap dried up\n");
		ret = TEE_ERROR_SHORT_BUFFER;
		goto out;
	}
	memset(um, 0, sizeof(struct umem));

	switch (type) {
	case UMEM_SHAREDMEMORY:
	case UMEM_FIXED_SA:
		/* input is not scatter list */
		/* both mem and size should be 4KB aligned */
		if (((uint32_t)mem & (SMALL_PAGE_SIZE - 1)) != 0) {
			ret = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}
		if ((size & (SMALL_PAGE_SIZE - 1)) != 0) {
			ret = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}

		npage = size / SMALL_PAGE_SIZE;
		s_list = malloc(SIZE_OF_SCATTERLIST(1));
		if (!s_list) {
			EMSG("tzk heap dried up\n");
			ret = TEE_ERROR_SHORT_BUFFER;
			goto out;
		}

		s_list->num = 1;
		s_list->s_item[0].npage = npage;
		s_list->s_item[0].addr = (uint32_t)mem;

		break;
	case UMEM_DYNAMIC_SA:
		/* input is a scatter list */
		sl_size = SIZE_OF_SCATTERLIST(((scatter_list_t *)mem)->num);
		s_list = malloc(sl_size);

		if (!s_list) {
			EMSG("tzk heap dried up\n");
			ret = TEE_ERROR_SHORT_BUFFER;
			goto out;
		}

		memcpy(s_list, (void *)mem, sl_size);

		break;
	default:
		goto out;
	}

	/*
	 * the malloc/free of secure share memory is fully controled
	 * by tzk. so ignore the overlop check for it.
	 */
	if (type != UMEM_SHAREDMEMORY) {
		/* overlap check and mark in granule table */
		ret = mark_pages_in_granuletable(s_list);
		if (ret) {
			//error("task %s: can't register memory(0x%x, 0x%x), overlapped\n", this->name, mem, mem + size);
			umem_dump();
			ret = TEE_ERROR_ACCESS_CONFLICT;
			goto out;
		}
		is_set_gt = true;
	}

	um->s_list = s_list;
	um->type = type;
	um->size = size;
	um->refcnt = 0;

	ret = umem_next_id(um->type, &um->umid);
	if (ret != TEE_SUCCESS) {
		ret = TEE_ERROR_ITEM_NOT_FOUND;
		goto out;
	}
	/* memory with type UMEM_DYNAMIC_SA will set mt when rule is set
	 */
	if (type != UMEM_DYNAMIC_SA) {
		um->mt = umem_get_mmu_type(um);

		if (um->mt == WRONG_MMU_ATTRIBUTE) {
			ret = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}
	}

	umem_list[ID_2_INDEX(um->umid)] = um;
	umem_cnt++;


	if(!umem_find_mobj(um, s_list)) {
		EMSG("Cannot find mobj.\n");
		goto out;
	} else {
		debug_print("Find mobj for mem 0x%lx, sz = 0x%lx.\n", mem, size);
	}

	*mem_id = um->umid;

	ret = TEE_SUCCESS;

	debug_print("%x is registerred successfully cnt = %d\n", um->umid, umem_cnt);

out:
	if (ret) {
		if (is_set_gt)
			clear_pages_in_granuletable(s_list);
		if (um)
			free(um);
		if (s_list)
			free(s_list);
	}

	return ret;
}

const struct umem *umem_get_umem(uint32_t mem_id)
{
	return umem_find(mem_id);
}

int umem_set_rule(uint32_t mem_id, uint32_t rule)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (um) {
		if (um->type != UMEM_DYNAMIC_SA)
			return TEE_ERROR_ACCESS_DENIED;

		um->rule = rule;
		um->mt = umem_get_mmu_type(um);
		ret = TEE_SUCCESS;
	}

	return ret;
}

int umem_unregister_memory(uint32_t mem_id)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (um) {
#if 0
		ret = umem_check_memory_permission(mem_id, tz_sw_task_uuid(this->task_id), UMEM_PERM_RW);
		if (ret)
			goto out;
#endif
		if ((um->refcnt <= 0) && LIST_EMPTY(&um->user)) {
			/* remove from granule table */
			if (um->type != UMEM_SHAREDMEMORY)
				clear_pages_in_granuletable(um->s_list);
#if 0
			if (um->type == UMEM_SHAREDMEMORY)
				umem_free_sharedmemory((void *)um->s_list->s_item[0].addr);
#endif
			if (um->type == UMEM_DYNAMIC_SA) {
				ret = mobj_reg_shm_release_by_cookie(um->umid);
				if (ret) {
					EMSG("fail to release reg shm mobj: ret = 0x%x, list_num = 0x%x.\n", ret, um->s_list->num);
				}
			}
			free(um->s_list);
			umem_free_taperm_list(um);
			free(um);
			umem_list[ID_2_INDEX(mem_id)] = NULL;
			umem_cnt--;
			ret = TEE_SUCCESS;
		} else {
			umem_dump_umem(um);
			ret = TEE_ERROR_BUSY;
		}
	} else {
		EMSG("can't find the memory 0x%x\n", mem_id);
		ret = TEE_ERROR_BAD_PARAMETERS;
	}

	debug_print("unregister %x, cnt = %d\n", mem_id, umem_cnt);

//out:
	return ret;
}

/* Linux owns the DMA allocation; the TA owns this registration. */
int umem_set_owner(uint32_t mem_id, struct user_ta_ctx *utc)
{
    struct umem *um = umem_find(mem_id);

    if (!um || !utc || um->owner || um->owner_released)
        return TEE_ERROR_BAD_STATE;
    um->owner = utc;
    return TEE_SUCCESS;
}

/* Called before vm_info_final: that routine tears down every remaining VM map. */
void umem_release_user(struct user_ta_ctx *utc)
{
    unsigned int index;

    umem_operation_lock();
    for (index = 0; index < MAXNUM_UMEM; ++index) {
        struct umem *um = umem_list[index];
        struct umem_user *user;

        if (!um) continue;
        user = umem_find_user(um, utc);
        if (user) {
            LIST_REMOVE(user, list);
            free(user);
            um->refcnt--;
        }
        if (um->owner == utc) {
            um->owner = NULL;
            um->owner_released = true;
        }
        /* Keep registrations referenced by other live TAs. */
        if (um->owner_released && !um->refcnt && LIST_EMPTY(&um->user)) {
            int result = umem_unregister_memory(um->umid);
            if (result) EMSG("retire UMEM registration failed: %#x", result);
        }
    }
    umem_operation_unlock();
}

int umem_add_memory_access(uint32_t mem_id, const TEE_UUID *uuid,
				void *sa __unused, uint32_t permission, int force __unused)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;
	struct umem_ta_perm *taperm = NULL;
//	uint32_t thisperm = 0;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the memory 0x%x\n", mem_id);
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* if non-secure memory, don't need to control the permission */
	if (um->mt & MT_M_NONSECURE)
		return TEE_SUCCESS;

	/* if sa is attached, the permission is controled by sa. */
	/* TODO: in function to attach sa(not implemented), if permission
	 * is already granted, the function should return error.
	 */
	if (um->sa_id != 0)
		return TEE_ERROR_ACCESS_DENIED;

	/* support read or read+write. so read permission has to be added
	 */
	if (!(permission & UMEM_PERM_READ)) {
		EMSG("support read or read+write! %x\n", permission);
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}
#if 0
	/*
	 * the force flag is only set when grant permission to the first TA
	 * if not force, do stricter check
	 */
	if (!force) {
		/* check if running TA has grant permission */
		taperm = umem_find_perm(um, tz_sw_task_uuid(this->task_id));
		if (!taperm) {
			EMSG("no grant permission of this ta\n");
			ret = TEE_ERROR_ACCESS_DENIED;
			goto out;
		}
		thisperm = taperm->perm;
		if (!(thisperm & UMEM_PERM_GRANT)) {
			EMSG("no grant permission\n");
			ret = TEE_ERROR_ACCESS_DENIED;
			goto out;
		}

		/* check if running TA has sufficient permission */
		if ((~thisperm) &  permission) {
			EMSG("no corresponding permission\n");
			ret = TEE_ERROR_ACCESS_DENIED;
			goto out;
		}
	}
	#endif

	/* check if it already set the permission(can't overwrite) */
	taperm = umem_find_perm(um, uuid);
	if (taperm) {
		IMSG("permission already be set\n");
		return TEE_SUCCESS;
	}

	taperm = malloc(sizeof(struct umem_ta_perm));
	if (!taperm) {
		EMSG("tzk heap dried up\n");
		ret = TEE_ERROR_SHORT_BUFFER;
		goto out;
	}
	memset(taperm, 0, sizeof(struct umem_ta_perm));

	memcpy(&taperm->uuid, uuid, sizeof(TEE_UUID));
	taperm->perm = permission;

	LIST_INSERT_HEAD(&um->perm, taperm, list);

	ret = TEE_SUCCESS;

	debug_print("perm: %x to ", taperm->perm);
	print_uuid(&taperm->uuid);

out:
	return ret;
}

int umem_remove_memory_access(uint32_t mem_id,   		 const TEE_UUID *uuid)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;
	struct umem_ta_perm *taperm = NULL;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the memory 0x%x\n", mem_id);
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	taperm = umem_find_perm(um, uuid);
	if (!taperm) {
		EMSG("can't find the permission set\n");
		ret = TEE_ERROR_SECURITY;
		goto out;
	}

	LIST_REMOVE(taperm, list);
	free(taperm);

	ret = TEE_SUCCESS;

out:
	return ret;
}

int umem_map_memory(struct user_ta_ctx *utc, uint32_t mem_id, void **va, size_t *size)
{
	struct umem *um = NULL;
	struct mobj *mobj = NULL;
	vaddr_t v = 0;
	TEE_Result res = TEE_SUCCESS;
	uint32_t prot = TEE_MATTR_URW | TEE_MATTR_PRW;
	uint32_t vm_flags = 0;
	struct umem_user *user = NULL;
	unsigned int task_id;

	vm_flags |= VM_FLAG_SHAREABLE;
	um = umem_find(mem_id);
	if (um == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	task_id = utc->uctx.vm_info.asid;
	debug_print("asid = 0x%x memid = 0x%x", task_id, mem_id);
	user = umem_find_user(um, utc);
	if (user) {
		if (user->vm_start) {
			EMSG("memory id 0x%x is already mapped @ 0x%" PRIxVA, mem_id, user->vm_start);
			return  TEE_ERROR_ACCESS_CONFLICT;
		}
	} else {
		user = malloc(sizeof(struct umem_user));
		memset(user, 0, sizeof(struct umem_user));
		user->task_id = task_id;
		user->context = utc;
		LIST_INSERT_HEAD(&um->user, user, list);
		memcpy(&user->uuid, &utc->ta_ctx.ts_ctx.uuid, sizeof(TEE_UUID));
		um->refcnt++;
	}

	mobj = um->mobj;
	res = vm_map_pad(&utc->uctx, &v, um->size, prot, vm_flags, mobj, um->offs, 0, 0, 0);
	if (res != TEE_SUCCESS) {
		EMSG("Fail to map memory for memid 0x%x, res = 0x%x", mem_id, res);
		res =  TEE_ERROR_ACCESS_CONFLICT;
		goto out;
	}

	user->refcnt++;
	user->vm_start = v;

	*va = (void *)(uintptr_t)user->vm_start;

	if(size)
		*size = um->size;
	res = TEE_SUCCESS;
	debug_print("asid = 0x%x, memid = 0x%x, va = 0x%" PRIxVA ", refcnt = 0x%x user refcnt=0x%x",
			task_id, mem_id, user->vm_start, um->refcnt, user->refcnt);
out:
	if (res && user && !user->refcnt) {
		um->refcnt--;
		LIST_REMOVE(user, list);
		memset(user, 0, sizeof(*user));
		free(user);
	}
	return res;
}


int umem_unmap_memory(struct user_ta_ctx *utc, uint32_t mem_id)
{
	uint32_t vm_flags = 0;
	struct umem *um = NULL;
	TEE_Result res = TEE_SUCCESS;
	vaddr_t va;
	struct umem_user *user = NULL;

	um = umem_find(mem_id);
	if (um == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	debug_print("asid = 0x%x memid = 0x%x", utc->uctx.vm_info.asid, mem_id);

	user = umem_find_user(um, utc);
	if (!user) {
		EMSG("memid 0x%x is not mapped", mem_id);
		return TEE_ERROR_ACCESS_DENIED;
	}

	va = user->vm_start;

	res = vm_get_flags(&utc->uctx, va, um->size, &vm_flags);
	if (res) {
		EMSG("Fail to mem id 0x%x vm flags res =0x%x", mem_id, res);
		return res;
	}
	if (vm_flags & VM_FLAG_PERMANENT)
		return TEE_ERROR_ACCESS_DENIED;
	debug_print("memid = 0x%x, va = 0x%" PRIxVA ", refcnt = 0x%x user refcnt=0x%x",
			mem_id, user->vm_start, um->refcnt, user->refcnt);

	user->refcnt--;
	if(user->refcnt > 0) {
		return TEE_SUCCESS;
	}

	res = vm_unmap(&utc->uctx, user->vm_start, um->size);
	if (res != TEE_SUCCESS) {
		EMSG("Fail to umap memid 0x%x", mem_id);
		return res;
	}

	LIST_REMOVE(user, list);
	memset(user, 0, sizeof(*user));
	free(user);
	um->refcnt--;
	return TEE_SUCCESS;
}

int umem_get_memory_size(uint32_t mem_id, size_t *size)
{
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	*size = um->size;

	return 0;
}

int umem_get_physical_by_memid(uint32_t mem_id, paddr_t* phy_addr)
{
	int ret = 0;
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id\n");
		ret = TEE_ERROR_ITEM_NOT_FOUND;
		goto out;
	}

	/* return physical address only when the number of scatter is 1 */
	if (um->s_list->num != 1) {
		EMSG("can't get physical address of non-contigious memory\n");
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	*phy_addr = um->s_list->s_item[0].addr;

out:
	return ret;
}

int umem_get_memid_by_phys(uint32_t *mem_id, paddr_t phy_addr, size_t size)
{
	const struct umem *um = NULL;
	const struct mem_region *mr = NULL;

	mr = mr_find_region(phy_addr, size);
	if (NULL == mr) {
		EMSG("memory(0x%lx, 0x%lx) is not in any memory region!!!\n",
					(unsigned long)phy_addr, (unsigned long)(phy_addr + size));
		return TEE_ERROR_BAD_PARAMETERS;
	}

	um = umem_find_by_phys(phy_addr, size);
	if (um == NULL)
		return TEE_ERROR_ITEM_NOT_FOUND;

	*mem_id = um->umid;

	return TEE_SUCCESS;
}

bool umem_validate_caller(uint32_t mem_id __unused)
{
#if 0
	struct umem *um = NULL;
	struct umem_ta_perm *taperm = NULL;
	struct umem_ta_perm *caller_perm = NULL;
	uint32_t caller = this->caller;
	uint32_t task_id = this->task_id;

	if (TZ_TASK_ID_IS_NW(caller)) {
		IMSG("caller is from normal world.\n");
		return false;
	}

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the memory 0x%x, caller %s\n",
				mem_id, tz_sw_task_name(caller));
		return false;
	}

	caller_perm = umem_find_perm(um, tz_sw_task_uuid(caller));
	taperm = umem_find_perm(um, tz_sw_task_uuid(task_id));

	if (taperm == NULL || caller_perm == NULL) {
		EMSG("no access permission for memid = 0x%x\n", mem_id);
		return false;
	}
	// caller TA mem id permission should >= called TA
	if (~(caller_perm->perm) & taperm->perm) {
		EMSG("memid = 0x%x： caller permission 0x%x, current ta permission 0x%x\n",
				mem_id, caller_perm->perm, taperm->perm);
		return false;
	}else
		return true;
#else
	return true;

#endif
}

int umem_get_offset_by_memid(uint32_t mem_id, uintptr_t phy_addr, unsigned long *offset)
{
	struct umem *um = NULL;
	int ret = TEE_ERROR_GENERIC;
	uintptr_t pa;
	size_t size;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id\n");
		ret = TEE_ERROR_ITEM_NOT_FOUND;
		goto out;
	}

	/* return physical address only when the number of scatter is 1 */
	if (um->s_list->num != 1) {
		EMSG("can't get physical address of non-contigious memory\n");
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	pa = um->s_list->s_item[0].addr;
	size = um->s_list->s_item[0].npage * SMALL_PAGE_SIZE;
	if(!ADDR_IN_RANGE(phy_addr, pa, size)){
		EMSG("physical address 0x%lx is out of mem id range(0x%lx, 0x%lx)\n", phy_addr, pa, pa + size);
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}
	*offset = phy_addr - pa;

	return TEE_SUCCESS;

out:
	return ret;

}

bool umem_check_mem_attr(uint32_t mem_id, uint32_t attr)
{
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id\n");
		return false;
	}

	return ((um->attr & attr) == attr) ? true : false;
}

int umem_add_memory_access_2_whitelist(uint32_t mem_id)
{
	int ret = 0;
	int i, j;
	int count = (int)(sizeof(UMEM_ACCESS_WHITELIST) / sizeof(TEE_UUID));

	for (i = 0; i < count; i++) {
		ret = umem_add_memory_access(mem_id, &UMEM_ACCESS_WHITELIST[i], NULL,
			UMEM_PERM_RW, 1);
		if (ret != TEE_SUCCESS) {
			EMSG("Fail to add %d UMEM_PERM_RW to mem_id 0x%x, ret = 0x%x\n", i, mem_id, ret);
			umem_dump_umem(umem_get_umem(mem_id));
			for (j = 0; j < i; j++) {
				umem_remove_memory_access(mem_id, &UMEM_ACCESS_WHITELIST[j]);
			}
			return TEE_ERROR_ACCESS_DENIED;
		}
	}

	return TEE_SUCCESS;
}

static int umem_refcnt_inc(struct umem *um, struct user_ta_ctx *utc)
{
	struct umem_user *user = NULL;
	uint32_t task_id = -1;

	if (!um)
		return TEE_ERROR_BAD_PARAMETERS;
	debug_print("memid = 0x%x", um->umid);

	task_id = utc->uctx.vm_info.asid;

	user = umem_find_user(um, utc);
	if (user) {
		user->refcnt++;
		return TEE_SUCCESS;
	}

	user = malloc(sizeof(struct umem_user));
	if (!user) {
		EMSG("tzk heap dried up\n");
		return TEE_ERROR_SHORT_BUFFER;
	}

	memset(user, 0, sizeof(struct umem_user));
	user->task_id = task_id;
	user->context = utc;
	user->refcnt++;
	LIST_INSERT_HEAD(&um->user, user, list);
	um->refcnt++;
	debug_print("memid = 0x%x,  refcnt = 0x%x user refcnt=0x%x",
			um->umid, um->refcnt, user->refcnt);

	return TEE_SUCCESS;
}

static int umem_refcnt_dec(struct umem *um, struct user_ta_ctx *utc)
{
	struct umem_user *user = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!um)
		return TEE_ERROR_BAD_PARAMETERS;

	debug_print("asid = 0x%x, memid = 0x%x", utc->uctx.vm_info.asid, um->umid);

	user = umem_find_user(um, utc);
	if (!user) {
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	user->refcnt--;
	if(user->refcnt > 0)
		return TEE_SUCCESS;

	if (user->vm_start)	 {
		res = vm_unmap(&utc->uctx, user->vm_start, um->size);
		if (res != TEE_SUCCESS) {
			EMSG("fail to unmap memid 0x%x, res = 0x%x", um->umid, res);
			return res;
		}
	}
	debug_print("asid = 0x%x memid = 0x%x,  refcnt = 0x%x user refcnt=0x%x",
			utc->uctx.vm_info.asid, um->umid, um->refcnt-1, user->refcnt);

	LIST_REMOVE(user, list);
	memset(user, 0, sizeof(*user));
	free(user);
	um->refcnt--;

	return TEE_SUCCESS;
}

int umem_refcnt_op(struct user_ta_ctx *utc, uint32_t mem_id, uint32_t type, uint32_t *refc)
{
	int ret = TEE_ERROR_GENERIC;
	struct umem *um = NULL;

	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id 0x%x\n", mem_id);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	switch (type) {
	case UMEM_REFC_VALUE:
		*refc = um->refcnt;
		ret = TEE_SUCCESS;
		break;
	case UMEM_REFC_INC:
		ret = umem_refcnt_inc(um, utc);
		break;
	case UMEM_REFC_DEC:
		ret = umem_refcnt_dec(um, utc);
		break;
	default:
		EMSG("Unsupported type 0x%x.\n", type);
		return TEE_ERROR_BAD_PARAMETERS;
	};

	return ret;
}

int umem_bind_hw_virtaddr(uint32_t mem_id, void *hw_virtaddr)
{
	struct umem *um = NULL;
#if 0
	if (!virt_in_usr(hw_virtaddr, sizeof(void *)))
		return TEE_ERROR_ACCESS_DENIED;
#endif
	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id 0x%x\n", mem_id);
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	um->hw_virtaddr = hw_virtaddr;

	return TEE_SUCCESS;
}


int umem_get_hw_virtaddr(uint32_t mem_id, void **phw_virtaddr)
{
	struct umem *um = NULL;

	if (!phw_virtaddr)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(phw_virtaddr, sizeof(void *)))
		return TEE_ERROR_ACCESS_DENIED;
#endif
	um = umem_find(mem_id);
	if (!um) {
		EMSG("can't find the mem_id 0x%x\n", mem_id);
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	*phw_virtaddr = um->hw_virtaddr;

	return TEE_SUCCESS;
}

static TEE_Result umem_init(void)
{
	mutex_init(&h);
	return TEE_SUCCESS;
}

service_init(umem_init);

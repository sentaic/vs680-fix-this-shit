// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TZ_UMEM_MGR_H_
#define _TZ_UMEM_MGR_H_
#include <sys/queue.h>
#include <types_ext.h>
#include <tee_api_types.h>
#include <scatter_list.h>
#include <kernel/tee_ta_manager.h>

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

#define UMEM_TYPE(mem_id) (mem_id & 0xf0000000)

enum {
	UMEM_PERM_READ = 0x01,
	UMEM_PERM_WRITE = 0x02,
	UMEM_PERM_RW = 0x03,
	UMEM_PERM_GRANT = 0x4,
};

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

enum {
	UMEM_SECURE = 0x1,
	UMEM_NONSECURE = 0x2,

	UMEM_CACHEABLE = 0x10,
	UMEM_NONCACHEABLE = 0x20,

	UMEM_RESTRICTED = 0x100,
	UMEM_SYSTEM = 0x200,
	UMEM_ION = 0x400,
};

struct umem_user {
	struct user_ta_ctx *context;
	uint32_t task_id;
	vaddr_t vm_start;
	int refcnt;	/* how many times it is mapped to this TA */
	LIST_ENTRY(umem_user) list;
	TEE_UUID uuid;
};

struct umem_ta_perm {
	TEE_UUID uuid;
	uint32_t perm;	/* grant/read/write */
	LIST_ENTRY(umem_ta_perm) list;
};

struct umem {
	struct user_ta_ctx *owner;
	bool owner_released;
	int umid;	/* memory id */
	scatter_list_t *s_list;	/* memory scatter list */
	uint32_t rule; /* rule index if mpt */
	vaddr_t vm_start;	/* virtual address assign to it */
	size_t size;	/* total size in bytes */
	uint32_t mt;	/* mmu attribute */
	uint32_t type;	/* shared memory, SMM, etc. */
	uint32_t sa_id;	/* secure attribute id */
	int refcnt;	/* how many TAs it is mapped to */
	uint32_t attr;	/* S/NS, Cache/NonCache, ION/System/Restricted */
	void *hw_virtaddr;	/*Page table address for Hardware master which support IOMMU*/
	LIST_HEAD(umem_ta_perm_list, umem_ta_perm) perm;	/* uuid with permission */
	LIST_HEAD(umem_user_list, umem_user) user;	/* actual user */
	struct mobj *mobj;
	size_t offs;
};

int umem_set_owner(uint32_t mem_id, struct user_ta_ctx *utc);
void umem_release_user(struct user_ta_ctx *utc);
void umem_operation_lock(void);
void umem_operation_unlock(void);
void *umem_allocate_tzkmemory(size_t size);
void umem_free_sharedmemory(void *mem);
int umem_check_memory_permission(uint32_t mem_id, const TEE_UUID *uuid, uint32_t perm);
int umem_check_memory_region(paddr_t mem, size_t size);
int umem_register_memory(paddr_t mem, size_t size, uint32_t type, uint32_t *mem_id);
const struct umem *umem_get_umem(uint32_t mem_id);
int umem_set_rule(uint32_t mem_id, uint32_t rule);
int umem_unregister_memory(uint32_t mem_id);
int umem_add_memory_access(uint32_t mem_id,
				const TEE_UUID *uuid, void *sa, uint32_t permission, int force);
int umem_remove_memory_access(uint32_t mem__id, const TEE_UUID *uuid);
int umem_map_memory(struct user_ta_ctx *utc, uint32_t mem_id, void **va, size_t *size);
int umem_unmap_memory(struct user_ta_ctx *utc, uint32_t mem_id);
//int umem_unmap_memory_by_address(void *va);
//int umem_task_map_memory(int task_id, uint32_t mem_id, void **va, size_t *size);
//int umem_task_unmap_memory(int task_id, uint32_t mem_id);
//int umem_map_memory_to_all_instances(uint32_t mem_id, void **va, size_t *size);
//int umem_unmap_memory_from_all_instances(uint32_t mem_id);
int umem_get_memory_size(uint32_t mem_id, size_t *size);
void umem_dump(void);
struct umem *umem_find_by_address(void *addr, size_t size);
int umem_get_physical_by_memid(uint32_t mem_id, paddr_t* phy_addr);
int umem_get_memid_by_phys(uint32_t *mem_id, uintptr_t phy_addr, size_t size);
int umem_get_offset_by_memid(uint32_t mem_id, uintptr_t phy_addr, unsigned long *offset);
bool umem_check_mem_attr(uint32_t mem_id, uint32_t attr);
int umem_add_memory_access_2_whitelist(uint32_t mem_id);
int umem_refcnt_op(struct user_ta_ctx *utc, uint32_t mem_id, uint32_t type, uint32_t *refc);
int umem_bind_hw_virtaddr(uint32_t mem_id, void *hw_virtaddr);
int umem_get_hw_virtaddr(uint32_t mem_id, void **phw_virtaddr);
int umem_quick_register_memory(void *mem, size_t size, void **va);
void umem_quick_map_memory(uint32_t *ttb, void *pa, void *va, uint32_t size);
void umem_quick_unmap_memory(uint32_t *ttb, void *va, uint32_t size);
int umem_quick_unregister_memory(void *mem, size_t size, void *va);

bool umem_validate_caller(uint32_t mem_id);
#endif /* _TZ_UMEM_MGR_H_ */

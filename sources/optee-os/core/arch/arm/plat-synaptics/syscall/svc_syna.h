// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef SVC_SYNA_H
#define SVC_SYNA_H

#include <types_ext.h>
#include <tee_api_types.h>
#include "mr_setup.h"

TEE_Result syscall_syna_mr_filter(void *region, uint32_t *max_num,
		unsigned long attr_mask, unsigned long attr_val);
TEE_Result syscall_mr_find_region(const void *buffer, uint32_t size, struct mem_region *mr);
TEE_Result syscall_syna_verify_image(const void *in, size_t inLen, void *out,
		uint32_t *outLen, uint32_t codeType);
TEE_Result syscall_syna_exec_cmd(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result, uint32_t result_len);

TEE_Result syscall_syna_exec_cmd_ex(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result, uint32_t result_len, uint32_t delay);

TEE_Result syscall_syna_map_register(vaddr_t *va, size_t len, unsigned long prot,
		unsigned long flags, paddr_t pa);

TEE_Result syscall_syna_unmap_register(vaddr_t va, size_t len);
TEE_Result syscall_syna_virt_to_phys(unsigned long *pa, void *va);
TEE_Result syscall_syna_get_otp_raw_data(uint32_t row, uint8_t *buffer);


/*
 * global data
 */
TEE_Result syscall_syna_set_globaldata(uint32_t key, uint32_t value);
TEE_Result syscall_syna_get_globaldata(uint32_t key, uint32_t *value);

 /*
 * manage the memory used by TA
 */
//TEE_Result syscall_syna_allocate_tzkmemory(size_t size, uint32_t *mem_id, void **va);
TEE_Result syscall_syna_register_memory(paddr_t mem, size_t size, uint32_t type, uint32_t *mem_id);
TEE_Result syscall_syna_unregister_memory(uint32_t mem_id);
TEE_Result syscall_syna_add_memory_access(uint32_t mem_id, TEE_UUID *uuid, void *sa, uint32_t permission);
TEE_Result syscall_syna_map_memory(uint32_t mem_id, void **va, size_t *size);
TEE_Result syscall_syna_unmap_memory(uint32_t mem_id);
TEE_Result syscall_syna_get_physical_by_memid(uint32_t mem_id, paddr_t *phy_addr);
TEE_Result syscall_syna_get_memid_by_phys(uint32_t *mem_id, uintptr_t phy_addr, size_t size);
bool syscall_syna_check_memattr_by_memid(uint32_t mem_id, uint32_t attr);
bool sys_mem_validate_caller(uint32_t mem_id);
TEE_Result syscall_syna_refcnt_op(uint32_t mem_id, uint32_t type, uint32_t *refc);
TEE_Result syscall_syna_attach_said(uint32_t mem_id, uint32_t sa_id);
TEE_Result syscall_syna_detach_said(uint32_t mem_id);
void sys_dump_umemlist(void);
TEE_Result syscall_syna_mem_bind_hw_virtaddr(uint32_t mem_id ,void *hw_virt);
TEE_Result syscall_syna_mem_get_hw_virtaddr(uint32_t mem_id ,void **phw_virt);
TEE_Result syscall_syna_reserve_reservation(void * reservation);
TEE_Result syscall_syna_release_reservation(void * reservation);
TEE_Result syscall_syna_reservation_get_info(uint32_t mem_id, uint32_t info_type, void *buf, uint32_t buf_size);

/*
 * mutex
 */
TEE_Result syscall_syna_mutex_alloc(int *h);
void syscall_syna_mutex_destroy(int);
TEE_Result syscall_syna_mutex_lock(int);
void syscall_syna_mutex_unlock(int);
bool syscall_syna_mutex_trylock(int);

/*
 * semaphore
 */
TEE_Result syscall_syna_semaphore_alloc(int);
void syscall_syna_semaphore_destroy(int);
TEE_Result syscall_syna_semaphore_wait(int);
void syscall_syna_semaphore_post(int);
TEE_Result syscall_syna_semaphore_trywait(int);
TEE_Result syscall_syna_semaphore_timewait(int, uint32_t);

TEE_Result syscall_syna_get_securekey(void *sk);
TEE_Result syscall_syna_get_securekey2(void *sk);

TEE_Result syscall_syna_get_sa_entry(void* sessionEntry, uint32_t keySessionId);
TEE_Result syscall_syna_set_sa_entry(void *buff);
TEE_Result syscall_syna_get_active_entries(int *num);
TEE_Result syscall_syna_get_active_sa_entry(void *buf);

#endif /*SVC_SYNA_H*/

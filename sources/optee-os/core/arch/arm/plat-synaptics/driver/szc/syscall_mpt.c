// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "svc_syna.h"
#include <tee_api_types.h>
#include <tee_api_defines.h>

#ifdef CFG_SZC_MPT
#include <config.h>
#include <trace.h>
#include <string.h>
#include "malloc.h"
#include "scatter_list.h"
#include "mpt_rule.h"
#include "szc_mpt.h"
#include "tz_granule_table.h"
#include "tz_umem_mgr.h"
#include "mem_region_userdata.h"
#include <mm/core_mmu.h>
#include <addr_range_check.h>
#include <kernel/cache_helpers.h>


typedef struct {
	scatter_list_t *s_list; // raw pages
	unsigned int a;  // single security attribute
	uint32_t mem_id; // id returned by reserve
} TEE_Reservation;

typedef TEE_Reservation securepage_reservation_t;

extern void dumpstack(void);

struct mpt_valid_mem_info {
	unsigned int base;
	unsigned int size;
	unsigned int type;
};

#define MAX_VALID_MEM_NUM (3)
static struct mpt_valid_mem_info valid_mem[MAX_VALID_MEM_NUM] = {0};
static unsigned int valid_mem_num = 0;

static int check_mpt_uuid(const TEE_UUID *uuid)
{
	int i = 0;
	int ta_count = (int)(sizeof(PERMITTED_TA_UUID_LIST) / sizeof(TEE_UUID));
	/* TA whose UUID is in the list have the permission for the syscall */
	for (i = 0; i < ta_count; i++) {
		if (0 == memcmp(uuid,
					&PERMITTED_TA_UUID_LIST[i], sizeof(TEE_UUID)))
			return i; /* return the index */
	}

	return -1;
}

/*
 * FIXME: on VS680 z1, there is no configuration register for mpt granularity
 * and bits. the condition will be changed on VS680 A0 and VS640 and after
 * they come back we should read the registers to get the rule. Here we use
 * a simple check on VS680 Z1 in the following functions
 */
static int is_mptrule_valid(unsigned int r)
{
	return (r < 16);
}

static int is_mptrule_nonsecure(unsigned int r)
{
	return (r == NON_SECURE_RULE) ? 1 : 0;
}

static unsigned int convert_mptrule(unsigned r)
{
	//FIXME: there is only two rule for mpt on VS680 Z1. but for other masters
	//it could have maximum 16 rules
	//so here simply return 1 as secure rule
	//rule maker should define the same number or rules as mpt can handle
	return (is_mptrule_nonsecure(r)) ? r : 1;
}
/* end FIXME */

/*
static long check_rule(scatter_list_t *s_list, uint32_t rule)
{
	unsigned int i = 0, r = 0;
	unsigned int num = 0;
	scatter_item_t *s_item = NULL;

	num = s_list->num;
	s_item = s_list->s_item;
	for (i = 0; i < num; i++) {
		if (0 != mpt_get_rule(s_item[i].addr, (s_item[i].npage * SMALL_PAGE_SIZE), &r)) {
			EMSG("detect inconsistent mpt rule\n");
			return -1;
		}
		if (r != rule) {
			EMSG("detect inconsistent mpt rule\n");
			return -1;
		}
	}

	return 0;
}
*/

static unsigned int caculate_size_of_scatter_list(scatter_list_t *s_list)
{
	unsigned int i = 0;
	unsigned int size = 0;

	for (i = 0; i < s_list->num; i++)
		size += s_list->s_item[i].npage * SMALL_PAGE_SIZE;

	return size;
}

static int check_scatter_list_alignment(scatter_list_t *s_list)
{
	/* start addr and size of each scatter should align with the granularity */
	unsigned int i = 0;
	unsigned int alignment = mpt_get_granularity() - 1;
	unsigned int addr = 0, size = 0;

	for (i = 0; i < s_list->num; i++) {
		addr = s_list->s_item[i].addr;
		size = s_list->s_item[i].npage * SMALL_PAGE_SIZE;
		if ((addr & alignment) || (size & alignment))
			return -1;
	}

	return 0;
}

static int get_mpt_valid_memory(void)
{
	const struct mem_region *mr = NULL;

	/* do once */
	if (valid_mem_num)
		return 0;

	// Nonsecure region should be the first one
	mr = mr_find_region_by_name("NonSecure");
	if (mr) {
		if (TEE_MR_ACPU_ZONE(mr))
			valid_mem[0].type = UMEM_FIXED_SA;
		else
			valid_mem[0].type = UMEM_DYNAMIC_SA;

		valid_mem[0].base = mr->base;
		valid_mem[0].size = mr->size;
		valid_mem_num = 1;
	}

	/* must-have */
	mr = mr_find_region_by_name("system");
	if (NULL == mr)
		return -1;

	if (TEE_MR_ACPU_ZONE(mr))
		return -1;

	valid_mem[valid_mem_num].type = UMEM_DYNAMIC_SA;
	valid_mem[valid_mem_num].base = mr->base;
	valid_mem[valid_mem_num].size = mr->size;
	valid_mem_num++;

	/* optional */
	mr = mr_find_region_by_name("system_1");
	if (mr) {
		if (TEE_MR_ACPU_ZONE(mr))
			return -1;
		valid_mem[valid_mem_num].type = UMEM_DYNAMIC_SA;
		valid_mem[valid_mem_num].base = mr->base;
		valid_mem[valid_mem_num ++].size = mr->size;
	}

	mr = mr_find_region_by_name("bootloader");
	if (mr) {
		if (TEE_MR_ACPU_ZONE(mr))
			return 0;
		valid_mem[valid_mem_num].type = UMEM_DYNAMIC_SA;
		valid_mem[valid_mem_num].base = mr->base;
		valid_mem[valid_mem_num ++].size = mr->size;
	}

	mr = mr_find_region_by_name("vpp");
	if (mr) {
		if (TEE_MR_ACPU_ZONE(mr))
			return 0;
		valid_mem[valid_mem_num].type = UMEM_DYNAMIC_SA;
		valid_mem[valid_mem_num].base = mr->base;
		valid_mem[valid_mem_num ++].size = mr->size;
	}


	return 0;
}

static int check_scatter_list_in_valid_mem(scatter_list_t *s_list, unsigned int rule, unsigned int *ptype)
{
	unsigned int i = 0, j = 0;
	unsigned int addr = 0, size = 0;
	int is_err = 0;
	unsigned int type = 0;

	if (get_mpt_valid_memory())
		return -1;

	for (i = 0; i < s_list->num; i++) {
		addr = s_list->s_item[i].addr;
		size = s_list->s_item[i].npage * SMALL_PAGE_SIZE;
		is_err = 1;
		for (j = 0; j < valid_mem_num; j++) {
			if (ADDR_REGION_IN_RANGE(addr, size, valid_mem[j].base, valid_mem[j].size)) {

				if (rule > 0 && valid_mem[j].type != UMEM_DYNAMIC_SA)
					return -1;

				type = UMEM_DYNAMIC_SA;

				is_err = 0;

				break;
			}
		}

		if (is_err) {
			EMSG("(%x, %x, %x)\n", addr, addr + size, size);
			return -1;
		}
	}

	*ptype = type;
	return 0;
}

static void set_mpt(scatter_list_t *s_list, uint32_t rule)
{
	unsigned int i = 0;
	uint32_t start = 0;
	size_t size = 0;
	unsigned int num = s_list->num;
	scatter_item_t *s_item = s_list->s_item;

	start = s_item[0].addr;
	size = (s_item[0].npage * SMALL_PAGE_SIZE);

	/* For the size of each item in scatter list is fixed at 64KB when do
	 * ION allocation, no matter if the neighbouring items are contiguous.
	 * In this function, we collect the contiguous neighbouring items and send
	 * them together to mpt function so that we can get better performance.
	 */
	for (i = 1; i < num; i++) {
		if ((start + size) == s_item[i].addr)
			size += (s_item[i].npage * SMALL_PAGE_SIZE);
		else {
			mpt_configure_region(start, size, rule);
			start = s_item[i].addr;
			size = (s_item[i].npage * SMALL_PAGE_SIZE);
		}
	}
	/* set the last one */
	mpt_configure_region(start, size, rule);
}

TEE_Result syscall_syna_reserve_reservation(void * reservation)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	securepage_reservation_t *r = (securepage_reservation_t *)reservation;
	size_t size = 0;
	unsigned int rule = 0;
	unsigned int type = 0;
	unsigned int mem_id = 0;
	const struct umem *um = NULL;	
	struct ts_session *s = ts_get_current_session();
//	struct user_ta_ctx *utc = NULL;
	const TEE_UUID *uuid = &s->ctx->uuid;

	if (check_mpt_uuid(uuid) < 0) {
		EMSG("this TA is not granted to reserve reservation\n");
		return TEE_ERROR_ACCESS_DENIED;
	}
#if 0
	if (!virt_in_usr(r, sizeof(TEE_Reservation)) ||
		!virt_in_usr(r->s_list, SIZE_OF_SCATTERLIST(r->s_list->num))) {
		EMSG("reservation can't exceed user space\n");
		return TEE_ERROR_SECURITY;
	}
#endif
	if(!r || !r->s_list || r->s_list->num <= 0)
		return TEE_ERROR_BAD_PARAMETERS;

	umem_operation_lock();

	if (check_scatter_list_alignment(r->s_list)) {
		EMSG("address and size of reservation is not aligned\n");
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	rule = convert_mptrule(r->a);
	if (!is_mptrule_valid(r->a)) {
		EMSG("it's not a valid mpt rule\n");
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* must be in memory region system*/
	if (check_scatter_list_in_valid_mem(r->s_list, rule, &type) != 0) {
		EMSG("scatter list is not in system\n");
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* register it whatever secure rule or non-secure rule */
	size = caculate_size_of_scatter_list(r->s_list);
	ret = umem_register_memory((paddr_t)(r->s_list), size, type, &mem_id);
	if (ret != TEE_SUCCESS) {
		EMSG("fail to register the memory\n");
		goto out;
	}

	um = umem_get_umem(mem_id);
	if (!um) {
		/* something wrong */
		ret = TEE_ERROR_BAD_PARAMETERS;
		EMSG("fail to get the memory\n");
		goto out1;
	}

	ret = umem_set_rule(mem_id, rule);
	if (ret != TEE_SUCCESS) {
		EMSG("fail to get the memory\n");
		goto out1;
	}
	/* grant PTM ta access permission */
	ret = umem_add_memory_access(um->umid, uuid,
				NULL, UMEM_PERM_READ | UMEM_PERM_WRITE | UMEM_PERM_GRANT, 1);
	if (ret != TEE_SUCCESS) {
		EMSG("fail to add access\n");
		goto out1;
	}

	ret = umem_add_memory_access_2_whitelist(um->umid);
	if (ret != TEE_SUCCESS) {
		EMSG("fail to add access to whitelist\n");
		goto out2;
	}

	if (!is_mptrule_nonsecure(rule) && (type == UMEM_DYNAMIC_SA))
		set_mpt(um->s_list, rule);

	r->a = um->rule;
	r->mem_id = um->umid;

	umem_operation_unlock();
	return TEE_SUCCESS;

out2:
	umem_remove_memory_access(um->umid, uuid);
out1:
	umem_unregister_memory(mem_id);
out:
	umem_operation_unlock();
	return ret;
}

TEE_Result syscall_syna_release_reservation(void * reservation)
{
	long ret = TEE_ERROR_GENERIC;
	securepage_reservation_t *r = (securepage_reservation_t *)reservation;
	void *start = NULL;
	size_t size = 0;
	const struct umem *um = NULL;
	unsigned int mem_id = 0;
	struct ts_session *s = ts_get_current_session();
	struct user_ta_ctx *utc = NULL;
	const TEE_UUID *uuid = &s->ctx->uuid;

	utc = to_user_ta_ctx(s->ctx);

	if (check_mpt_uuid(uuid) < 0) {
		EMSG("this TA is not granted to release reservation\n");
		return TEE_ERROR_ACCESS_DENIED;
	}

	if(!r || !r->s_list || r->s_list->num <= 0)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(r, sizeof(TEE_Reservation)) ||
		!virt_in_usr(r->s_list, SIZE_OF_SCATTERLIST(r->s_list->num))) {
		EMSG("reservation can't exceed user space\n");
		return TEE_ERROR_SECURITY;
	}
#endif
	umem_operation_lock();

	mem_id = r->mem_id;
	um = umem_get_umem(mem_id);
	if (!um) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		EMSG("fail to get the memory\n");
		goto out;
	}

	if ((um->s_list->num != r->s_list->num) ||
		memcmp(um->s_list, r->s_list, SIZE_OF_SCATTERLIST(um->s_list->num)) != 0) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		EMSG("scatter list doesn't match\n");
		goto out;
	}

	ret = umem_check_memory_permission(mem_id, uuid, UMEM_PERM_RW);
	if (TEE_SUCCESS != ret)
		goto out;

	if ((um->refcnt > 0) || !LIST_EMPTY(&um->user)) {
		ret = TEE_ERROR_BUSY;
		EMSG("memory %x is still in using!!!\n", mem_id);
		goto out;
	}

	if ((um->type == UMEM_DYNAMIC_SA) && !is_mptrule_nonsecure(um->rule)) {

		/* must clean memory before release the reservation */
		umem_map_memory(utc, mem_id, &start, &size);
		memset(start, 0x0, size);
		dcache_cleaninv_range(start, size);
		umem_unmap_memory(utc, mem_id);

		/* clear mpt rule */
		set_mpt(um->s_list, NON_SECURE_RULE);
	}

	umem_unregister_memory(mem_id);

	r->a = NON_SECURE_RULE;
	r->mem_id = 0;
	ret = TEE_SUCCESS;

out:
	umem_operation_unlock();
	return ret;
}
TEE_Result syscall_syna_reservation_get_info(uint32_t mem_id, uint32_t info_type,
									void *buf, uint32_t buf_size)

{
	long ret = TEE_ERROR_GENERIC;
	const struct umem *um = NULL;
	uint32_t *sg_num = NULL;
	uint32_t *rule = NULL;

	if (!buf || buf_size <= 0)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(buf, buf_size)) {
		EMSG("reservation info buffer can't exceed user space\n");
		return TEE_ERROR_SECURITY;
	}
#endif
	umem_operation_lock();

	um = umem_get_umem(mem_id);
	if (!um) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	switch (info_type) {
	case UMEM_SGLIST_NUM:
		sg_num = (uint32_t *)buf;
		*sg_num = um->s_list->num;
		break;
	case UMEM_SGLIST_RULE:
		rule = (uint32_t *)buf;
		*rule = um->rule;
		break;
	case UMEM_SGLIST_BUF:
		if (buf_size < SIZE_OF_SCATTERLIST(um->s_list->num)) {
			ret = TEE_ERROR_EXCESS_DATA;
			goto out;
		}
		memcpy(buf, um->s_list, SIZE_OF_SCATTERLIST(um->s_list->num));
		break;
	default:
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	};

	ret = TEE_SUCCESS;

out:
	umem_operation_unlock();
	return ret;
}

#else
TEE_Result syscall_syna_reserve_reservation(void * reservation __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

TEE_Result syscall_syna_release_reservation(void * reservation __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

TEE_Result syscall_syna_reservation_get_info(uint32_t mem_id __unused,
					uint32_t info_type __unused, void *buf __unused, uint32_t buf_size __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;

}
#endif

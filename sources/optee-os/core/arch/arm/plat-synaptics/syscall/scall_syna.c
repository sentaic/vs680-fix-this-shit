// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <assert.h>
#include <kernel/scall.h>
#include <kernel/thread.h>
#include <speculation_barrier.h>
#include <tee/tee_svc.h>
#include <trace.h>

#include "tee_syscall_numbers_syna.h"
#include "svc_syna.h"
#include "svc_syna_entry.h"

#if (TRACE_LEVEL == TRACE_FLOW) && defined(CFG_TEE_CORE_TA_TRACE)
#define TRACE_SYSCALLS
#endif

struct syscall_entry {
	syscall_t fn;
#ifdef TRACE_SYSCALLS
	const char *name;
#endif
};

#ifdef TRACE_SYSCALLS
#define SYSCALL_ENTRY(_fn) { .fn = (syscall_t)_fn, .name = #_fn }
#else
#define SYSCALL_ENTRY(_fn) { .fn = (syscall_t)_fn }
#endif

/*
 * This array is ordered according to the SYSCALL ids TEE_SCN_SYNA_xxx
 */
static const struct syscall_entry tee_syscall_syna_table[] = {
	SYSCALL_ENTRY(syscall_syna_mr_filter),
	SYSCALL_ENTRY(syscall_mr_find_region),
	SYSCALL_ENTRY(syscall_syna_exec_cmd),
	SYSCALL_ENTRY(syscall_syna_exec_cmd_ex),
	SYSCALL_ENTRY(syscall_syna_verify_image),
	SYSCALL_ENTRY(syscall_syna_map_register),
	SYSCALL_ENTRY(syscall_syna_unmap_register),
	SYSCALL_ENTRY(syscall_syna_virt_to_phys),
	SYSCALL_ENTRY(syscall_syna_get_otp_raw_data),
	SYSCALL_ENTRY(syscall_syna_register_memory),
	SYSCALL_ENTRY(syscall_syna_unregister_memory),
	SYSCALL_ENTRY(syscall_syna_add_memory_access),
	SYSCALL_ENTRY(syscall_syna_map_memory),
	SYSCALL_ENTRY(syscall_syna_unmap_memory),
	SYSCALL_ENTRY(syscall_syna_get_physical_by_memid),
	SYSCALL_ENTRY(syscall_syna_get_memid_by_phys),
	SYSCALL_ENTRY(syscall_syna_check_memattr_by_memid),
	SYSCALL_ENTRY(syscall_syna_refcnt_op),
	SYSCALL_ENTRY(syscall_syna_reserve_reservation),
	SYSCALL_ENTRY(syscall_syna_release_reservation),
	SYSCALL_ENTRY(syscall_syna_reservation_get_info),
	SYSCALL_ENTRY(syscall_syna_set_globaldata),
	SYSCALL_ENTRY(syscall_syna_get_globaldata),

	SYSCALL_ENTRY(syscall_syna_mutex_alloc),
	SYSCALL_ENTRY(syscall_syna_mutex_destroy),
	SYSCALL_ENTRY(syscall_syna_mutex_lock),
	SYSCALL_ENTRY(syscall_syna_mutex_unlock),
	SYSCALL_ENTRY(syscall_syna_mutex_trylock),

	SYSCALL_ENTRY(syscall_syna_semaphore_alloc),
	SYSCALL_ENTRY(syscall_syna_semaphore_destroy),
	SYSCALL_ENTRY(syscall_syna_semaphore_wait),
	SYSCALL_ENTRY(syscall_syna_semaphore_post),
	SYSCALL_ENTRY(syscall_syna_semaphore_timewait),
	SYSCALL_ENTRY(syscall_syna_semaphore_trywait),

	SYSCALL_ENTRY(syscall_syna_get_securekey),
	SYSCALL_ENTRY(syscall_syna_get_securekey2),

	SYSCALL_ENTRY(syscall_syna_get_sa_entry),
	SYSCALL_ENTRY(syscall_syna_set_sa_entry),
	SYSCALL_ENTRY(syscall_syna_get_active_entries),
	SYSCALL_ENTRY(syscall_syna_get_active_sa_entry),
};

#ifdef TRACE_SYSCALLS
static void trace_syscall_syna(size_t num)
{
	if (num > TEE_SCN_SYNA_MAX)
		return;
	FMSG("syna syscall #%zu (%s)", num, tee_syscall_syna_table[num].name);
}
#else
static void trace_syscall_syna(size_t num __unused)
{
}
#endif

static syscall_t get_tee_syscall_syna_func(size_t num)
{
	/* Cast away const */
	struct syscall_entry *sc_syna_table = (void *)tee_syscall_syna_table;

	static_assert(ARRAY_SIZE(tee_syscall_syna_table) == (TEE_SCN_SYNA_MAX + 1));

	if (num > TEE_SCN_SYNA_MAX)
		return (syscall_t)syscall_not_supported;

	return load_no_speculate(&sc_syna_table[num].fn, &sc_syna_table[0].fn,
				 &sc_syna_table[TEE_SCN_SYNA_MAX].fn + 1);

}

uint32_t syna_scall_do_call(struct thread_scall_regs *regs, syscall_t func);

TEE_Result syscall_syna_entry(size_t num, uint64_t x0,
				uint64_t x1, uint64_t x2, uint64_t x3,
				uint64_t x4, uint64_t x5, uint64_t x6)
{
	struct thread_scall_regs regs;
	syscall_t scf = NULL;

	trace_syscall_syna(num);

	scf = get_tee_syscall_syna_func(num);

	/* Setup parameters for second level syscall */
	regs.x0 = x0;
	regs.x1 = x1;
	regs.x2 = x2;
	regs.x3 = x3;
	regs.x4 = x4;
	regs.x5 = x5;
	regs.x6 = x6;

	return syna_scall_do_call(&regs, scf);
}

// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <kernel/spinlock.h>
#include <kernel/mutex.h>
#include <malloc.h>
#include <string.h>
#include "svc_syna.h"

#define TEE_SEMAPHORE_DEBUG		0
#define SEMAPHORE_MAX_NUMBER_OF		64
struct semaphore_handle {
	unsigned int valid;
	unsigned int lock;	/* used when operating on this struct */
	int value;
	int mutex_handle;
	struct wait_queue wq;
	int num_waiters;
};

static struct semaphore_handle semaphore_handle_db[SEMAPHORE_MAX_NUMBER_OF];

static unsigned int semaphore_handle_db_spin_lock =  0;


static int semaphore_handle_db_alloc_handle(int value)
{
	int handle = -1, i;

	for (i = 0; i < SEMAPHORE_MAX_NUMBER_OF; i++) {
		if (semaphore_handle_db[i].valid == 0) {
			handle = i;
			break;
		}
	}

	assert(handle != -1);
	semaphore_handle_db[handle].value = value;
	semaphore_handle_db[handle].lock = 0;
	semaphore_handle_db[handle].num_waiters = 0;
	semaphore_handle_db[handle].valid = 1;
	return handle;
}

static void semaphore_handle_db_free_handle(int handle)
{
	uint32_t flags;

	assert(handle < SEMAPHORE_MAX_NUMBER_OF && handle >= 0);
	assert(semaphore_handle_db[handle].valid == 1);
	flags = cpu_spin_lock_xsave(&semaphore_handle_db_spin_lock);
	semaphore_handle_db[handle].value = 0;
	semaphore_handle_db[handle].num_waiters = 0;
	semaphore_handle_db[handle].valid = 0;
	cpu_spin_unlock_xrestore(&semaphore_handle_db_spin_lock, flags);

}

TEE_Result syscall_syna_semaphore_alloc(int value)
{
	uint32_t flags;
	uint32_t h;

	flags = cpu_spin_lock_xsave(&semaphore_handle_db_spin_lock);

	h = semaphore_handle_db_alloc_handle(value);

	cpu_spin_unlock_xrestore(&semaphore_handle_db_spin_lock, flags);

	return h;
}

TEE_Result syscall_syna_semaphore_wait(int handle)
{
	uint32_t flags;
	struct semaphore_handle *sem;
	struct wait_queue_elem wqe;

	assert(handle < SEMAPHORE_MAX_NUMBER_OF && handle >= 0);

	sem = &semaphore_handle_db[handle];
	assert(sem->valid == 1);

	while (true) {
		flags = cpu_spin_lock_xsave(&sem->lock);

		if (sem->value > 0) {
			sem->value--;
			cpu_spin_unlock_xrestore(&sem->lock, flags);
			return TEE_SUCCESS;
		}

		sem->num_waiters++;
		wq_wait_init(&sem->wq, &wqe, false);
		cpu_spin_unlock_xrestore(&sem->lock, flags);

		if (wq_wait_final(&sem->wq, &wqe, 0,
				  sem, __func__, __LINE__)) {
			return TEE_ERROR_TARGET_DEAD;
		}

	}
}

void syscall_syna_semaphore_post(int handle)
{
	uint32_t flags;
	struct semaphore_handle *sem;
	assert(handle < SEMAPHORE_MAX_NUMBER_OF && handle >= 0);

	sem = &semaphore_handle_db[handle];
	flags = cpu_spin_lock_xsave(&sem->lock);
	sem->value++;

	if (sem->num_waiters > 0) {
		sem->num_waiters--;
		cpu_spin_unlock_xrestore(&sem->lock, flags);
		wq_wake_next(&sem->wq, sem, __func__, __LINE__);
	} else
		cpu_spin_unlock_xrestore(&sem->lock, flags);
}

TEE_Result syscall_syna_semaphore_trywait(int handle)
{
	uint32_t flags;
	struct semaphore_handle *sem;

	assert(handle < SEMAPHORE_MAX_NUMBER_OF && handle >= 0);
	sem = &semaphore_handle_db[handle];
	flags = cpu_spin_lock_xsave(&sem->lock);

	/* Try to acquire the semaphore */
	if (sem->value > 0) {
		sem->value--;
		cpu_spin_unlock_xrestore(&sem->lock, flags);
		return TEE_SUCCESS;
	}

	/* Semaphore is unavailable */
	cpu_spin_unlock_xrestore(&sem->lock, flags);
	return TEE_ERROR_BUSY;
}

TEE_Result syscall_syna_semaphore_timewait(int handle, uint32_t timeout_ms)
{
	struct semaphore_handle *sem;
	uint32_t flags;
	struct wait_queue_elem wqe;
	uint32_t elapsed_ms = 0;

	assert(handle >= 0 && handle < SEMAPHORE_MAX_NUMBER_OF);
	sem = &semaphore_handle_db[handle];
	assert(sem->valid);

	while (true) {
		flags = cpu_spin_lock_xsave(&sem->lock);

		/* Fast path */
		if (sem->value > 0) {
			sem->value--;
			cpu_spin_unlock_xrestore(&sem->lock, flags);
			return TEE_SUCCESS;
		}

		/*
		 * Prepare to sleep:
		 * Register as waiter before releasing spinlock
		 */
		sem->num_waiters++;
		wq_wait_init(&sem->wq, &wqe, false);

		cpu_spin_unlock_xrestore(&sem->lock, flags);

		/* Sleep with timeout */
		if (wq_wait_final(&sem->wq, &wqe, timeout_ms, sem, __func__, __LINE__)) {
			return TEE_ERROR_TARGET_DEAD;
		}

		/* Timeout handling: check if the semaphore is available */
		elapsed_ms += timeout_ms;
		if (elapsed_ms >= timeout_ms) {
			/* Timeout expired */
			return TEE_ERROR_TIMEOUT;
		}
	}
}

void syscall_syna_semaphore_destroy(int handle)
{
	struct semaphore_handle *sem;
	uint32_t flags;

	assert(handle < SEMAPHORE_MAX_NUMBER_OF && handle >= 0);
	sem = &semaphore_handle_db[handle];

	/* Check if there are any waiters */
	flags = cpu_spin_lock_xsave(&sem->lock);
	if (sem->num_waiters > 0) {
		cpu_spin_unlock_xrestore(&sem->lock, flags);
		return;   /* Cannot destroy semaphore if waiters exist */
	}
	cpu_spin_unlock_xrestore(&sem->lock, flags);

	/* Destroy the semaphore */
	semaphore_handle_db_free_handle(handle);
}

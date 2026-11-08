// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */
 
#include <kernel/spinlock.h>
#include <kernel/mutex.h>
#include <malloc.h>
#include <string.h>
#include "svc_syna.h"

#ifndef MUTEX_MAX_NUMBER_OF
#define MUTEX_MAX_NUMBER_OF		128
#endif

typedef struct {
	struct mutex mutex;
	int32_t task_id;	/* hold by whom */
	unsigned int lock;	/* used when operating on this struct */
	int handle;		/* identifier of mutex passed to linux driver */
} TEE_Mutex;

struct mutex_handle {
	unsigned int valid;
	void *lock;
};

#define START_MUTEX_HANDLE (0x1000)  //give a start offset to avoid uninitialized input of syscall_mutex_alloc
#define END_MUTEX_HANDLE (START_MUTEX_HANDLE + 128)
#define IS_VALID_HMUTEX(h) ((h >= START_MUTEX_HANDLE) && (h < END_MUTEX_HANDLE))
#define HMUTEX_INDEX2ID(i) (i + START_MUTEX_HANDLE)
#define HMUTEX_ID2INDEX(h) (h - START_MUTEX_HANDLE)

static struct mutex_handle mutex_handle_db[MUTEX_MAX_NUMBER_OF];

static unsigned int mutex_handle_db_spin_lock = 0;


static int mutex_handle_db_alloc_handle(void)
{
	int index = -1, i;
	TEE_Mutex *m;
	uint32_t flags;

	m = (void *)malloc(sizeof(TEE_Mutex));
	if (!m)
		return TEE_ERROR_SHORT_BUFFER;

	m->task_id = -1;
	m->lock = 0;
	mutex_init(&m->mutex);

	flags = cpu_spin_lock_xsave(&mutex_handle_db_spin_lock);
	for (i = 0; i < MUTEX_MAX_NUMBER_OF; i++) {
		if (mutex_handle_db[i].valid == 0) {
			index = i;
			break;
		}
	}

	assert(index != -1);
	m->handle = HMUTEX_INDEX2ID(index);

	mutex_handle_db[index].valid = 1;
	mutex_handle_db[index].lock = m;
	assert(mutex_handle_db[index].lock != 0);
	cpu_spin_unlock_xrestore(&mutex_handle_db_spin_lock, flags);

	return HMUTEX_INDEX2ID(index);
}

static void *mutex_handle_db_get_mutex(int handle)
{
	int index = HMUTEX_ID2INDEX(handle);
	assert(index < MUTEX_MAX_NUMBER_OF && index >= 0);
	assert(mutex_handle_db[index].lock != 0);
	assert(mutex_handle_db[index].valid == 1);

	return mutex_handle_db[index].lock;

}
static void mutex_handle_db_free_handle(int handle)
{
	int index = HMUTEX_ID2INDEX(handle);
	assert(index < MUTEX_MAX_NUMBER_OF && index >= 0);
	assert(mutex_handle_db[index].lock != 0);
	assert(mutex_handle_db[index].valid == 1);

	free(mutex_handle_db[index].lock);
	mutex_handle_db[index].valid = 0;
	mutex_handle_db[index].lock = 0;

}

TEE_Result syscall_syna_mutex_alloc(int *h)
{
	uint32_t flags;

	flags = cpu_spin_lock_xsave(&mutex_handle_db_spin_lock);
	//check if it is already allocated
	if (IS_VALID_HMUTEX(*h)) {
		//it doesn't mean error. it mean h is already allocated
		cpu_spin_unlock_xrestore(&mutex_handle_db_spin_lock, flags);
		return TEE_SUCCESS;
	}
	cpu_spin_unlock_xrestore(&mutex_handle_db_spin_lock, flags);

	/*not check return value for it will assert if there is no valid slot*/
	*h = mutex_handle_db_alloc_handle();

	return TEE_SUCCESS;
}

TEE_Result syscall_syna_mutex_lock(int handle)
{
	TEE_Mutex *m;

	m = (TEE_Mutex *)mutex_handle_db_get_mutex(handle);
	assert(m->handle == handle);

	mutex_lock(&m->mutex);
	return TEE_SUCCESS;
}

void syscall_syna_mutex_unlock(int handle)
{
	TEE_Mutex *m;

	m = (TEE_Mutex *)mutex_handle_db_get_mutex(handle);
	assert(m->handle == handle);

	mutex_unlock(&m->mutex);
}

bool syscall_syna_mutex_trylock(int handle)
{
	TEE_Mutex *m;

	m = (TEE_Mutex *)mutex_handle_db_get_mutex(handle);
	assert(m->handle == handle);

	return mutex_trylock(&m->mutex);
}


void syscall_syna_mutex_destroy(int handle)
{
	TEE_Mutex *m;

	m = (TEE_Mutex *)mutex_handle_db_get_mutex(handle);
	assert(m->handle == handle);

	mutex_destroy(&m->mutex);
	
	mutex_handle_db_free_handle(handle);
}

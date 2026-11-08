// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <sys/queue.h>
#include <kernel/ts_manager.h>
#include <tee_api_types.h>
#include <kernel/mutex.h>
#include <malloc.h>
#include <string.h>
#include "svc_syna.h"

struct globaldata_list {
	TAILQ_ENTRY(globaldata_list) link;	/* global data list */
	TEE_UUID uuid;
	uint32_t key;
	uint32_t value;
};

static TAILQ_HEAD(globaldata_lists, globaldata_list) globaldata_lists=
			TAILQ_HEAD_INITIALIZER(globaldata_lists);

static struct mutex gd_mutex = MUTEX_INITIALIZER;

TEE_Result syscall_syna_set_globaldata(uint32_t key, uint32_t value)
{
	struct globaldata_list *o, *obj = NULL;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = &s->ctx->uuid;

	mutex_lock(&gd_mutex);

	/* Check if key is already existed*/
	TAILQ_FOREACH(o, &globaldata_lists, link) {
		if ((key == o->key) &&
			(memcmp(uuid, &o->uuid, sizeof(TEE_UUID)) == 0)) {
			obj = o;
			break;
		}
	}
	mutex_unlock(&gd_mutex);

	if (obj) {
		obj->value = value;
		return TEE_SUCCESS;
	}

	/* new global data */
	o = malloc(sizeof(struct globaldata_list));
	if (o == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	memset(o, 0, sizeof(struct globaldata_list));
	memcpy(&o->uuid, uuid, sizeof(TEE_UUID));
	o->key = key;
	o->value = value;

	mutex_lock(&gd_mutex);
	TAILQ_INSERT_TAIL(&globaldata_lists, o, link);
	mutex_unlock(&gd_mutex);
	return TEE_SUCCESS;

}

TEE_Result syscall_syna_get_globaldata(uint32_t key, uint32_t *value)
{
	struct globaldata_list *o, *obj = NULL;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = &s->ctx->uuid;

#if 0
	if (!virt_in_usr(value, sizeof(*value))) {
		error("value can't exceed user space\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
#endif

	mutex_lock(&gd_mutex);
	TAILQ_FOREACH(o, &globaldata_lists, link) {
		if ((key == o->key) &&
			(memcmp(uuid, &o->uuid, sizeof(TEE_UUID)) == 0)) {
			obj = o;
			break;
		}
	}
	mutex_unlock(&gd_mutex);

	if (obj) {
		*value = obj->value;
		return TEE_SUCCESS;
	}
	return TEE_ERROR_ITEM_NOT_FOUND;
}

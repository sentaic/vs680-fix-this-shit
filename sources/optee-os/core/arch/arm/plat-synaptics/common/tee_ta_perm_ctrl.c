// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <initcall.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tee_api_types.h>
#include <kernel/tee_ta_manager.h>
#include <kernel/mutex.h>
#include <kernel/user_ta.h>
#include <trace.h>
#include <types_ext.h>
#include <utee_types.h>
#include <util.h>
#include "tee_ta_perm_ctrl.h"
#include "tz_boot_param.h"
#include "tz_list.h"

/* This mutex protects the critical section in ta_perm_ctrl */
struct mutex ta_perm_mutex = MUTEX_INITIALIZER;

#define tz_container_of(addr, type, member) ({     \
    type tmp;                                    \
    int offset = (int)(uintptr_t)(&tmp.member) - (int)(uintptr_t)&tmp; \
    (type *)((uintptr_t)addr - offset);                \
})

/* runtime permission table */
typedef enum {
	OBJ_TYPE_IDX_REGISTER = 0,
	OBJ_TYPE_IDX_BCM_PI,
	OBJ_TYPE_IDX_MAX,
} object_type_idx_t;

typedef TEE_Result (*item_compare_fn)(const void *item1, const void *item2, int32_t *result);

#define MAX_ITERATOR_NODE_POINTER_COUNT (2)

/* embeded node for iterator */
/* For each node, the item to compare is next to the node, beside the root node */
typedef struct _iterator_node_t {
	void *p[MAX_ITERATOR_NODE_POINTER_COUNT];
	uint32_t item[0];
} iterator_node_t;

typedef struct _iterator_list_t {
	void *p[MAX_ITERATOR_NODE_POINTER_COUNT];
	item_compare_fn cmp_op;
} iterator_list_t;

typedef struct _iterator_t {
	void (*init_node)(iterator_node_t *node);
	void (*init_list)(iterator_list_t *header, const item_compare_fn cmp_op);
	TEE_Result (*insert)(iterator_node_t *node, iterator_list_t *header); //return error if overlap
	TEE_Result (*remove)(iterator_node_t *node);
	iterator_node_t *(*get_match)(const void *item, iterator_list_t *header);
	iterator_node_t *(*get_larger)(iterator_node_t *node, iterator_list_t *header);
	iterator_node_t *(*get_smaller)(iterator_node_t *node, iterator_list_t *header);
} iterator_t;

#define TEE_PC_ITEM_OBJECT_MAGIC     (0x746a626f) //ASCII("objt")
#define TEE_PC_ITEM_GROUP_MAGIC      (0x706f7267) //ASCII("grop")
#define TEE_PC_ITEM_RULE_MAGIC       (0x656c7572) //ASCII("rule")

typedef struct _object_desc_t {
	uint32_t          magic;
	object_type_idx_t type_idx;
	uint32_t          default_perm;

	iterator_node_t   name_node;
	//item field of name_node start
	name_t            name;
	//item field of name_node end

	iterator_node_t   data_node;
	//item field of node start
	data_t            data;
	//item field of node end

} object_desc_t;

typedef struct _rule_item_t {
	uint32_t        magic;
	iterator_node_t node;
	//item field of node start
	object_desc_t   *obj;
	//item field of node end

	perm_t          perm;
} rule_item_t;

typedef struct _group_desc_t {
	uint32_t        magic;
	iterator_node_t node;
	//item field of node start
	name_t          name;
	//item field of node end

	iterator_list_t rule_list[OBJ_TYPE_IDX_MAX];
} group_desc_t;

typedef struct _group_desc_list_t {
	uint32_t        count;
	group_desc_t    *groups[0];
} group_desc_list_t;

typedef TEE_Result (*check_perm_fn)(data_t *data,
		iterator_list_t *list, perm_t perm, bool *allow, bool *data_updated);

typedef struct _compare_policy_t {
	object_type_t        type;
	item_compare_fn      cmp;
	check_perm_fn        check;
} compare_policy_t;

static iterator_list_t _g_group_list;
static iterator_list_t _g_object_list_by_name;
static iterator_list_t _g_object_list_by_data[OBJ_TYPE_IDX_MAX];

static void _simple_iterator_init_node(iterator_node_t *node)
{
	tz_list_t *list_node = (tz_list_t *)node->p;

	tz_list_init_head(list_node);
}

static void _simple_iterator_init_list(iterator_list_t *header, const item_compare_fn cmp_op)
{
	tz_list_t *list_header = (tz_list_t *)header->p;

	assert(sizeof(header->p) >= sizeof(tz_list_t));

	tz_list_init_head(list_header);
	header->cmp_op = cmp_op;
}

static TEE_Result _simple_iterator_insert(iterator_node_t *node, iterator_list_t *header)
{
	tz_list_t *list_header = (tz_list_t *)header->p;
	tz_list_t *list_node = (tz_list_t *)node->p;
	tz_list_t *list_entry = NULL;
	iterator_node_t *entry = NULL;
	int32_t result = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	tz_list_init_head(list_node);

	if (tz_list_empty(list_header)) {
		tz_list_add(list_node, list_header);
		return TEE_SUCCESS;
	}

	tz_list_iterate(list_header, list_entry) {
		entry = tz_container_of(list_entry, iterator_node_t, p);
		ret = header->cmp_op(node->item, entry->item, &result);
		if (ret != TEE_SUCCESS) {
			return ret;
		}
		/* item duplicated */
		if (result == 0) {
			return TEE_ERROR_ACCESS_CONFLICT;
		} else if (result < 0) {
			tz_list_add_tail(list_node, list_entry);
			return TEE_SUCCESS;
		}
	}

	tz_list_add_tail(list_node, list_header);

	return TEE_SUCCESS;
}

static TEE_Result _simple_iterator_remove(iterator_node_t *node)
{
	tz_list_t *list_node = (tz_list_t *)node->p;

	tz_list_del(list_node);

	return TEE_SUCCESS;
}

static iterator_node_t *_simple_iterator_get_match(const void *item, iterator_list_t *header)
{
	tz_list_t *list_header = (tz_list_t *)header->p;
	tz_list_t *list_entry = NULL;
	iterator_node_t *entry = NULL;
	TEE_Result ret = TEE_ERROR_GENERIC;
	int32_t result = -1;

	tz_list_iterate(list_header, list_entry) {
		entry = tz_container_of(list_entry, iterator_node_t, p);
		ret = header->cmp_op(item, entry->item, &result);
		if (ret != TEE_SUCCESS) {
			return NULL;
		}
		/* item duplicated */
		if (result == 0) {
			return entry;
		}
	}

	return NULL;
}

static iterator_node_t *_simple_iterator_get_larger(iterator_node_t *node, iterator_list_t *header)
{
	tz_list_t *list_header = (tz_list_t *)header->p;
	tz_list_t *list_node = (tz_list_t *)node->p;
	tz_list_t *list_entry = tz_list_get_next(list_node);

	if (list_entry != list_header) {
		return tz_container_of(list_entry, iterator_node_t, p);
	} else {
		return NULL;
	}
}

static iterator_node_t *_simple_iterator_get_smaller(iterator_node_t *node, iterator_list_t *header)
{
	tz_list_t *list_header = (tz_list_t *)header->p;
	tz_list_t *list_node = (tz_list_t *)node->p;
	tz_list_t *list_entry = tz_list_get_prev(list_node);

	if (list_entry != list_header) {
		return tz_container_of(list_entry, iterator_node_t, p);
	} else {
		return NULL;
	}
}

static iterator_t _g_iterator =
{
	.init_node   = _simple_iterator_init_node,
	.init_list   = _simple_iterator_init_list,
	.insert      = _simple_iterator_insert,
	.remove      = _simple_iterator_remove,
	.get_match   = _simple_iterator_get_match,
	.get_larger  = _simple_iterator_get_larger,
	.get_smaller = _simple_iterator_get_smaller,

};

static inline void init_data_item(uint32_t *buf, uint32_t wsize, data_t *data)
{
	assert(data != NULL);

	data->magic = TEE_PC_ITEM_DATA_MAGIC;
	data->wsize = wsize;
	memset(data->buf, 0, TEE_PC_MAX_DATA_WORDS * sizeof(uint32_t));
	memcpy(data->buf, buf, wsize * sizeof(uint32_t));
}

static inline bool _is_legal_data_item(const void *item)
{
	const data_t *data = item;

	assert(item);

	if (data->magic == TEE_PC_ITEM_DATA_MAGIC) {
		return true;
	} else {
		return false;
	}
}

static inline bool _is_legal_name(const void *item)
{
	const name_t *name = item;

	assert(item);

	if (name->magic == TEE_PC_ITEM_NAME_MAGIC) {
		if (name->length > TEE_PC_MAX_NAME_LENGTH) {
			return false;
		}
		return true;
	} else {
		return false;
	}
}

static inline bool _is_legal_group_desc(const void *item)
{
	const group_desc_t *group = item;

	assert(item);

	if (group->magic == TEE_PC_ITEM_GROUP_MAGIC) {
		if(!_is_legal_name(&group->name)) {
			return false;
		}
		return true;
	} else {
		return false;
	}
}

static inline bool _is_legal_object_desc(const void *item)
{
	const object_desc_t *obj = item;

	assert(item);

	if (obj->magic == TEE_PC_ITEM_OBJECT_MAGIC) {
		if (obj->type_idx >= OBJ_TYPE_IDX_MAX) {
			return false;
		}
		if (!_is_legal_name(&obj->name)) {
			return false;
		}
		if (!_is_legal_data_item(&obj->data)) {
			return false;
		}
		return true;
	} else {
		return false;
	}
}

#ifdef CONFIG_SUPPORT_SINGLE_COMPARE
static TEE_Result _check_perm_single(data_t *data,
		iterator_list_t *list, perm_t perm, bool *allow, bool *updated)
{
	iterator_node_t *node = NULL;
	rule_item_t *rule = NULL;

	assert(allow != NULL);
	assert(updated != NULL);
	*allow = false;
	*updated = false;

	assert(_is_legal_data_item(data));
	assert(data->wsize == 1);
	assert(data->buf[1] == 0);

	node = _g_iterator.get_match(data, list);
	if (node == NULL) {
		return TEE_ERROR_ITEM_NOT_FOUND;
	}
	rule = tz_container_of(node, rule_item_t, node);
	assert(rule != NULL);

	if ((rule->perm & perm) == perm) {
		*allow = true;
	}

	return TEE_SUCCESS;
}

static TEE_Result _compare_data_single(const void *item1, const void *item2, int32_t *result)
{
	const data_t *data1 = item1;
	const data_t *data2 = item2;
	uint32_t value1 = 0;
	uint32_t value2 = 0;

	assert(result != NULL);
	assert(_is_legal_data_item(data1));
	assert(_is_legal_data_item(data2));
	assert(data1->wsize == 1);
	assert(data2->wsize == 1);

	value1 = data1->buf[0];
	value2 = data2->buf[0];

	if (value1 > value2) {
		*result = 1;
	} else if (value1 < value2){
		*result = -1;
	} else {
		*result = 0;
	}

	DMSG("compare single data obj %x vs %x, result %d\n", value1, value2, *result);

	return TEE_SUCCESS;
}
#endif

static TEE_Result _check_perm_range(data_t *data,
		iterator_list_t *list, perm_t perm, bool *allow, bool *updated)
{
	iterator_node_t *node = NULL;
	iterator_node_t *pre_node = NULL;
	rule_item_t *rule = NULL;
	data_t *cur_data;
	uint32_t target_start;
	uint32_t target_end;
	uint32_t cur_start;
	uint32_t cur_end;

	assert(allow != NULL);
	assert(updated != NULL);
	*allow = false;
	*updated = false;

	assert(_is_legal_data_item(data));
	assert(data->wsize == 2);

	node = _g_iterator.get_match(data, list);
	if (node == NULL) {
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	while (1) {
		rule = tz_container_of(node, rule_item_t, node);
		assert(rule != NULL);

		cur_data = &rule->obj->data;
		assert(_is_legal_data_item(cur_data));
		assert(cur_data->wsize == 2);

		target_start = data->buf[0];
		target_end = data->buf[0] + data->buf[1];
		cur_start = cur_data->buf[0];
		cur_end = cur_data->buf[0] + cur_data->buf[1];

		assert(target_start < target_end);
		assert(cur_start < cur_end);

		if (target_start < cur_start) {
			return TEE_ERROR_ITEM_NOT_FOUND;
		}

		if ((rule->perm & perm) != perm) {
			return TEE_SUCCESS;
		}

		if (target_end <= cur_end) {
			*allow = true;
			return TEE_SUCCESS;
		}

		data->buf[0] = cur_end;
		data->buf[1] = target_end - cur_end;

		*updated = true;

		pre_node = node;
		node = _g_iterator.get_larger(pre_node, list);
		if (node == NULL) {
			return TEE_ERROR_ITEM_NOT_FOUND;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result _compare_data_range(const void *item1, const void *item2, int32_t *result)
{
	const data_t *data1 = item1;
	const data_t *data2 = item2;
	uint32_t start1 = 0, end1 = 0;
	uint32_t start2 = 0, end2 = 0;

	assert(result != NULL);
	assert(_is_legal_data_item(data1));
	assert(_is_legal_data_item(data2));
	assert(data1->wsize == 2);
	assert(data2->wsize == 2);

	start1 = data1->buf[0];
	end1 = data1->buf[0] + data1->buf[1];
	start2 = data2->buf[0];
	end2 = data2->buf[0] + data2->buf[1];

	assert(start1 < end1);
	assert(start2 < end2);

	if (end1 <= start2) {
		*result = -1;
	} else if (start1 >= end2) {
		*result = 1;
	} else {
		/* ranges overlapped */
		*result = 0;
	}
	//DMSG("compare data range obj [%x,%x) vs [%x,%x), result %d\n",
	//		start1, end1, start2, end2, *result);

	return TEE_SUCCESS;
}

static compare_policy_t _g_compare_policy[OBJ_TYPE_IDX_MAX] =
{
	{
		.type         = TEE_OBJ_REG_RANGE,
		.cmp          = _compare_data_range,
		.check        = _check_perm_range,
	},
	{
		.type         = TEE_OBJ_BCM_PI_CATEGORY,
		.cmp          = _compare_data_range,
		.check        = _check_perm_range,
	},
};

static TEE_Result _compare_name_item(const void *item1, const void *item2, int32_t *result)
{
	const name_t *name1 = item1;
	const name_t *name2 = item2;
	uint32_t min_name_length;
	int32_t local_result = -1;

	assert(result != NULL);
	assert(_is_legal_name(name1));
	assert(_is_legal_name(name2));

	min_name_length = name1->length > name2->length ? name2->length : name1->length;

	local_result = memcmp(name1->name, name2->name, min_name_length);
	if (local_result == 0) {
		if (name1->length == name2->length) {
			*result = 0;
		} else if (name1->length > name2->length) {
			*result = 1;
		} else {
			*result = -1;
		}
	} else {
		*result = local_result;
	}

	//DMSG("compare name obj %s(%d) vs %s(%d), result %d\n",
		//	name1->name, name1->length, name2->name, name2->length, *result);

	return TEE_SUCCESS;
}

static inline data_t *_extract_data(const void *item, object_type_idx_t *index)
{
	*index = OBJ_TYPE_IDX_MAX;

	if (_is_legal_data_item(item)) {
		/* Don't know the type index */
		return (data_t *)item;
	} else if (_is_legal_object_desc(*(object_desc_t **)item)) {
		object_desc_t *obj = *(object_desc_t **)item;

		*index = obj->type_idx;
		assert(_is_legal_data_item(&obj->data));

		return &obj->data;
	} else {
		return NULL;
	}
}

static TEE_Result _compare_rule_item(const void *item1, const void *item2, int32_t *result)
{
	data_t *data1 = NULL;
	data_t *data2 = NULL;
	object_type_idx_t idx1 = OBJ_TYPE_IDX_MAX;
	object_type_idx_t idx2 = OBJ_TYPE_IDX_MAX;
	object_type_idx_t idx = OBJ_TYPE_IDX_MAX;

	assert(item1 != NULL);
	assert(item2 != NULL);
	assert(result != NULL);

	data1 = _extract_data(item1, &idx1);
	data2 = _extract_data(item2, &idx2);
	if (data1 == NULL || data2 == NULL) {
		EMSG("fail to extract item\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (idx1 == idx2) {
		if (idx1 == OBJ_TYPE_IDX_MAX) {
			return TEE_ERROR_BAD_PARAMETERS;
		}
		idx = idx1;
	} else {
		idx = (idx1 == OBJ_TYPE_IDX_MAX) ? idx2 : idx1;
	}

	return _g_compare_policy[idx].cmp(data1, data2, result);
}

static object_desc_t *_create_object_desc(const tz_object_t *obj)
{
	object_desc_t *obj_desc;

	obj_desc = malloc(sizeof(object_desc_t));
	if (obj_desc == NULL) {
		return NULL;
	}
	memset(obj_desc, 0, sizeof(object_desc_t));
	memcpy(&obj_desc->data, &obj->data, sizeof(data_t));

	_g_iterator.init_node(&obj_desc->name_node);
	memcpy(&obj_desc->name, &obj->name, sizeof(name_t));
	_g_iterator.init_node(&obj_desc->data_node);
	obj_desc->magic = TEE_PC_ITEM_OBJECT_MAGIC;

	return obj_desc;
}

static void _free_object_desc(object_desc_t *obj_desc)
{
	assert(obj_desc != NULL);

	memset(obj_desc, 0, sizeof(object_desc_t));

	free(obj_desc);
}

static TEE_Result _ta_perm_add_object(tz_object_t *obj)
{
	uint32_t idx;
	TEE_Result ret = TEE_ERROR_NOT_SUPPORTED;
	object_desc_t *obj_desc = NULL;

	assert(_is_legal_name(&obj->name));
	assert(_is_legal_data_item(&obj->data));

	for (idx = 0; idx < OBJ_TYPE_IDX_MAX; idx++) {
		if (_g_compare_policy[idx].type == obj->type) {
			obj_desc = _create_object_desc(obj);
			if (obj_desc == NULL) {
				EMSG("fail to create obj_desc\n");
				return TEE_ERROR_OUT_OF_MEMORY;
			}
			obj_desc->type_idx = idx;

			ret = _g_iterator.insert(&obj_desc->data_node, &_g_object_list_by_data[idx]);
			if (ret != TEE_SUCCESS) {
				EMSG("fail to insert to data list\n");
				goto failure_after_alloc;
			}
			ret = _g_iterator.insert(&obj_desc->name_node, &_g_object_list_by_name);
			if (ret != TEE_SUCCESS) {
				EMSG("fail to insert to name list\n");
				goto failure_after_insert_to_resouce_list;
			}
			return ret;
		}
	}

	return TEE_SUCCESS;

failure_after_insert_to_resouce_list:
	_g_iterator.remove(&obj_desc->data_node);
failure_after_alloc:
	_free_object_desc(obj_desc);
	obj_desc = NULL;

	return ret;
}

static object_desc_t *_get_object_desc(tz_perm_rule_t *rule)
{
	object_desc_t *obj = NULL;
	iterator_node_t *node = NULL;

	assert(rule != NULL);
	assert(_is_legal_name(&rule->obj_name));

	node = _g_iterator.get_match(
			&rule->obj_name, &_g_object_list_by_name);
	if (node == NULL) {
		return NULL;
	}
	obj = tz_container_of(node, object_desc_t, name_node);
	assert(_is_legal_object_desc(obj));

	return obj;
}

static group_desc_t *_get_group_desc(tz_perm_rule_t *rule)
{
	group_desc_t *group = NULL;
	iterator_node_t *node = NULL;
	TEE_Result ret = TEE_ERROR_GENERIC;
	uint32_t i;

	assert(rule != NULL);
	assert(_is_legal_name(&rule->group_name));

	node = _g_iterator.get_match(&rule->group_name, &_g_group_list);
	if (node == NULL) {
		group = malloc(sizeof(group_desc_t));
		if (group == NULL) {
			EMSG("fail to allocate group desc\n");
			return NULL;
		}

		memset(group, 0, sizeof(group_desc_t));
		group->magic = TEE_PC_ITEM_GROUP_MAGIC;
		memcpy(&group->name, &rule->group_name, sizeof(name_t));
		_g_iterator.init_node(&group->node);
		for (i = 0; i < MAX_ITERATOR_NODE_POINTER_COUNT; i++) {
			_g_iterator.init_list(&group->rule_list[i], _compare_rule_item);
		}
		ret = _g_iterator.insert(&group->node, &_g_group_list);
		if (ret != TEE_SUCCESS) {
			EMSG("fail to insert to group list\n");
			memset(group, 0, sizeof(group_desc_t));
			free(group);
			group = NULL;
			return NULL;
		}
	} else {
		group = tz_container_of(node, group_desc_t, node);
		assert(_is_legal_group_desc(group));
	}

	return group;
}

static TEE_Result _add_rule_item(group_desc_t *group, object_desc_t *obj, perm_t perm)
{
	rule_item_t *rule_item;

	assert(group != NULL);
	assert(obj != NULL);
	assert(_is_legal_object_desc(obj));
	assert(_is_legal_group_desc(group));

	rule_item = malloc(sizeof(rule_item_t));
	if (rule_item == NULL) {
		EMSG("fail to alloc rule item\n");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	memset(rule_item, 0, sizeof(rule_item_t));
	_g_iterator.init_node(&rule_item->node);
	rule_item->obj = obj;
	rule_item->perm = perm;

	return _g_iterator.insert(&rule_item->node, &group->rule_list[obj->type_idx]);
}

static TEE_Result _ta_perm_add_rule(tz_perm_rule_t *rule)
{
	group_desc_t *group = NULL;
	object_desc_t *obj = NULL;

	assert(rule != NULL);

	/* 1. Get object */
	obj = _get_object_desc(rule);
	if (obj == NULL) {
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	/* 2. Get group */
	group = _get_group_desc(rule);
	if (group == NULL) {
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	/* 3. Add rule item */
	return _add_rule_item(group, obj, rule->perm);
}
struct ta_permission {
	TEE_UUID uuid;
	TAILQ_ENTRY(ta_permission) link;
	void *group_list;

};

TAILQ_HEAD(ta_perm_head, ta_permission);

struct ta_perm_head ta_perms = TAILQ_HEAD_INITIALIZER(ta_perms);

static struct ta_permission *ta_perm_find(const TEE_UUID *uuid)
{
	struct ta_permission *ta_perm = NULL;

	TAILQ_FOREACH(ta_perm, &ta_perms, link) {
		if (memcmp(&ta_perm->uuid, uuid, sizeof(TEE_UUID)) == 0)
			return ta_perm;
	}

	return NULL;
}

void * ta_perm_get_group_info(const TEE_UUID *uuid)
{
	struct ta_permission *ta_perm = ta_perm_find(uuid);

	if (ta_perm)
		return ta_perm->group_list;
	else
		return NULL;
}

TEE_Result ta_perm_set_group_info(ta_group_header_t *root_groups,
		ta_group_header_t *groups,
		const TEE_UUID *uuid)
{
	group_desc_list_t *group_list = NULL;
	uint32_t group_list_size;
	iterator_node_t *node = NULL;
	uint32_t i = 0, j = 0, found = 0;
	uint32_t idx = 0;
	name_t name;
	struct ta_permission *ta_perm = NULL;


	if (groups == NULL || groups->magic != TEE_PC_GROUP_HEAD_MAGIC ||
			root_groups == NULL ||
			root_groups->magic != TEE_PC_GROUP_HEAD_MAGIC) {
		EMSG("Bad group header\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* just check != case because we will check the == case in final step */
	if (root_groups->group_count != 0xffff0000) {
		/* skip the == case because they are same */
		if (groups->group_count == 0xffff0000)
			groups = root_groups;
		else if (groups->group_count != 0) {
			for (i = 0; i < groups->group_count; i++) {
				found = 0;
				for (j = 0; j < root_groups->group_count; j++) {
					if (0 == memcmp(groups->group_names[i].name,
						root_groups->group_names[j].name,
						groups->group_names[i].length)) {
						found = 1;
						break;
					}
				}
				if (!found)
					return TEE_ERROR_BAD_FORMAT;

			}
			assert(found == 1);
		} else {
			// no permission group for this TA
			return TEE_SUCCESS;
		}
	}

	group_list_size = sizeof(group_desc_list_t) + groups->group_count * sizeof(group_desc_t *);
	group_list = malloc(group_list_size);
	if (group_list == NULL) {
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	group_list->count = groups->group_count;
	for (i = 0; i < group_list->count; i++) {
		name.magic = TEE_PC_ITEM_NAME_MAGIC;
		memcpy(&name.name, &groups->group_names[i].name, TEE_PC_MAX_NAME_LENGTH);
		name.length = groups->group_names[i].length;

		node = _g_iterator.get_match(&name, &_g_group_list);
		if(node == NULL) {
			EMSG("unknown group name found. Please contact TZK vendor\n");
			memset(group_list, 0, group_list_size);
			free(group_list);

			return TEE_ERROR_BAD_FORMAT;
		}
		group_list->groups[idx] = tz_container_of(node, group_desc_t, node);
		assert(_is_legal_group_desc(group_list->groups[idx]));
		idx++;
	}

	assert(idx == group_list->count);

	ta_perm = calloc(1, sizeof(struct ta_permission));
	if (!ta_perm)
		return TEE_ERROR_OUT_OF_MEMORY;
	ta_perm->group_list = group_list;
	ta_perm->uuid = *uuid;

	mutex_lock(&ta_perm_mutex);
	TAILQ_INSERT_TAIL(&ta_perms, ta_perm, link);
	mutex_unlock(&ta_perm_mutex);

	return TEE_SUCCESS;
}


static TEE_Result _ta_perm_check_perm(const TEE_UUID *uuid, uint32_t *target, uint32_t wsize,
			 object_type_idx_t type_idx, perm_t perm, bool *allow)
{
	data_t data;
	struct ta_permission *ta_perm = NULL;
	group_desc_list_t *group_list = NULL;
	uint32_t i;
	bool local_allow = false;
	/* updated and updater are for multi-object combination case, eg register address */
	bool updated = false;
	uint32_t updater = 0xFFFFFFFF;
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (allow == NULL || target == NULL) {
		EMSG("parameter is NULL\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	*allow = false;

	ta_perm = ta_perm_find(uuid);
	if (ta_perm == NULL) {
		EMSG("can't find permission infomation\n");
		return TEE_ERROR_ITEM_NOT_FOUND;
	}
	group_list = (group_desc_list_t *)ta_perm->group_list;
	if (group_list == NULL) {
		EMSG("can't find group infomation\n");
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	assert(group_list->count < updater);
	assert(type_idx < OBJ_TYPE_IDX_MAX);

	init_data_item(target, wsize, &data);

	/* check permission in each group.
	 * If permission for one of the groups is deny, then this access request should be denied
	 */
	while (1) {
		for (i = 0; i < group_list->count; i++) {
			if (updater == i) {
				/* skip updater */
				continue;
			}
			assert(_is_legal_group_desc(group_list->groups[i]));
			DMSG("checking group %s for perm\n", group_list->groups[i]->name.name);
			ret = _g_compare_policy[type_idx].check(&data,
					&group_list->groups[i]->rule_list[type_idx], perm,
					&local_allow, &updated);
			if (ret == TEE_SUCCESS) {
				if (local_allow) {
					*allow = local_allow;
					return TEE_SUCCESS;
				}
			} else if (ret != TEE_ERROR_ITEM_NOT_FOUND) {
				*allow = false;
				return ret;
			}

			if (updated) {
				updater = i;
				updated = false;
				/* start over if data has been updated during last round check */
				break;
			}
		}

		if (i == group_list->count) {
			*allow = local_allow;
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_GENERIC;
}

TEE_Result ta_perm_check_mmap(const TEE_UUID *uuid, paddr_t address, uint32_t size, bool *allow)
{
	uint32_t target[2];
	perm_t perm = TEE_TA_PERM_WRITE | TEE_TA_PERM_READ;

	target[0] = (uintptr_t)address;
	target[1] = size;

	if ((target[0] | target[1]) & 0x00000FFF) {
		EMSG("address must be aligned by 4k, target[0] = 0x%x, target[1] = 0%x\n", target[0], target[1]);
		*allow = false;
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return _ta_perm_check_perm(uuid, target, 2, OBJ_TYPE_IDX_REGISTER, perm, allow);
}

TEE_Result ta_perm_check_bcm_pi(const TEE_UUID *uuid, uint32_t pi, bool *allow)
{
	uint32_t target[2];
	perm_t perm = TEE_TA_PERM_EXEC;

	target[0] = pi;
	target[1] = 1;

	return _ta_perm_check_perm(uuid, target, 2, OBJ_TYPE_IDX_BCM_PI, perm, allow);
}


extern tz_object_t g_tz_object_list[];
extern const uint32_t g_tz_object_num;
extern tz_perm_rule_t g_tz_rule_list[];
extern const uint32_t g_tz_rule_num;

static TEE_Result ta_perm_init(void)
{
	uint32_t i;
	const struct tz_boot_param *param_obj;
	const struct tz_boot_param *param_rule;
	tz_object_t *object_list;
	uint32_t object_count;
	tz_perm_rule_t *rule_list;
	uint32_t rule_count;
	TEE_Result ret = TEE_ERROR_GENERIC;

	param_obj = tz_boot_param_get(TZ_BOOT_PARAM_PERM_OBJECT);
	if (param_obj == NULL) {
		object_list = g_tz_object_list;
		object_count = g_tz_object_num;
	} else {
		if (param_obj->size <= sizeof(struct tz_boot_param)) {
			return TEE_ERROR_BAD_PARAMETERS;
		}
		object_list = (tz_object_t*)param_obj->data;
		object_count = (param_obj->size - sizeof(struct tz_boot_param)) / sizeof(tz_object_t);
	}

	param_rule = tz_boot_param_get(TZ_BOOT_PARAM_PERM_RULE);
	if (param_rule == NULL) {
		rule_list = g_tz_rule_list;
		rule_count = g_tz_rule_num;
	} else {
		if (param_rule->size <= sizeof(struct tz_boot_param)) {
			return TEE_ERROR_BAD_PARAMETERS;
		}
		rule_list = (tz_perm_rule_t*)param_rule->data;
		rule_count = (param_rule->size - sizeof(struct tz_boot_param)) / sizeof(tz_perm_rule_t);
	}

	_g_iterator.init_list(&_g_group_list, _compare_name_item);
	_g_iterator.init_list(&_g_object_list_by_name, _compare_name_item);
	for (i = 0; i < OBJ_TYPE_IDX_MAX; i++) {
		_g_iterator.init_list(&_g_object_list_by_data[i], _g_compare_policy[i].cmp);
	}

	for (i = 0; i < object_count; i++) {
		ret = _ta_perm_add_object(object_list + i);
		if (ret != TEE_SUCCESS) {
			return ret;
		}
	}

	for (i = 0; i < rule_count; i++) {
		ret = _ta_perm_add_rule(rule_list + i);
		if (ret != TEE_SUCCESS) {
			return ret;
		}
	}

	return TEE_SUCCESS;
}

service_init(ta_perm_init);


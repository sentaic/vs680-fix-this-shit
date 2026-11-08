// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TZ_LIST_H_
#define _TZ_LIST_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct tz_list
{
    struct tz_list *next;
    struct tz_list *prev;
} tz_list_t;

#define TZ_LIST(head) \
    struct tz_list head = { &(head), &(head) }

#define TZ_LIST_HEAD(head) { &(head), &(head) }

static inline void tz_list_init_head(struct tz_list *head)
{
    head->next = head;
    head->prev = head;
}

static inline void tz_list_add(struct tz_list *entry, struct tz_list *head)
{
    entry->next = head->next;
    entry->prev = head;
    head->next->prev = entry;
    head->next = entry;
}

static inline void tz_list_add_tail(struct tz_list *entry, struct tz_list *head)
{
    entry->next = head;
    entry->prev = head->prev;
    head->prev->next = entry;
    head->prev = entry;
}

static inline void tz_list_del(struct tz_list *entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

static inline bool tz_list_empty(struct tz_list *head)
{
    return ((head->next == head) && (head->prev == head));
}

static inline struct tz_list *tz_list_get_next(struct tz_list *head)
{
    return head->next;
}

static inline struct tz_list *tz_list_get_prev(struct tz_list *head)
{
    return head->prev;
}

#define tz_list_iterate(head, entry) \
    for ((entry) = (head)->next; (entry) != (head); (entry) = (entry)->next)

#define tz_list_iterate_safe(head, entry, n) \
    for (entry = (head)->next, n = entry->next; entry != (head); \
         entry = n, n = entry->next)

#endif /*_TZ_LIST_H_*/

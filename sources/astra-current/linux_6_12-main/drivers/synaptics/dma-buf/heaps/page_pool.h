/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Page Pool kernel interface header
 *
 * Copyright (C) 2011 Google, Inc.
 * Copyright (c) 2026 Synaptics, Inc.
 */

#ifndef _PAGE_POOL_H
#define _PAGE_POOL_H

#include <linux/mm_types.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/list.h>

/**
 * functions for creating and destroying a heap pool -- allows you
 * to keep a pool of pre allocated memory to use from your heap.  Keeping
 * a pool of memory that is ready for dma, ie any cached mapping have been
 * invalidated from the cache, provides a significant performance benefit on
 * many systems
 */

/**
 * struct page_pool - pagepool struct
 * @count:		number of mem items in the pool
 * @items:		list of mem items
 * @lock:		lock protecting this struct and especially the count
 *			item list
 * @gfp_mask:		gfp_mask to use from alloc
 * @order:		order of pages in the pool
 * @list:		plist node for list of pools
 *
 * Allows you to keep a pool of pre allocated pages to use from your heap.
 * Keeping a pool of pages that is ready for dma, ie any cached mapping have
 * been invalidated from the cache, provides a significant performance benefit
 * on many systems
 */
struct page_pool {
	int count;
	struct list_head items;
	spinlock_t lock;
	gfp_t gfp_mask;
	unsigned int order;
	struct list_head list;
};

struct page_pool *sys_page_pool_create(gfp_t gfp_mask, unsigned int order);
void sys_page_pool_destroy(struct page_pool *pool);
struct page *sys_page_pool_alloc(struct page_pool *pool);
void sys_page_pool_free(struct page_pool *pool, struct page *page);
int sys_page_pool_nr_pages(struct page_pool *pool);

#endif /* _PAGE_POOL_H */

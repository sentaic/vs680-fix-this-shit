// SPDX-License-Identifier: GPL-2.0
/*
 * Memory Allocator page pool helpers
 *
 * Copyright (C) 2011 Google, Inc.
 * Copyright (C) 2026 Synaptics, Inc.
 */

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "page_pool.h"
#include <linux/module.h>

struct page *sys_page_pool_alloc(struct page_pool *pool)
{
	struct page *page = NULL;

	BUG_ON(!pool);

	spin_lock(&pool->lock);
	if (pool->count) {
		page = list_first_entry(&pool->items, struct page, lru);
		pool->count--;
	}
	if (page)
		list_del(&page->lru);
	spin_unlock(&pool->lock);
	return page;
}

void sys_page_pool_free(struct page_pool *pool, struct page *page)
{
	BUG_ON(pool->order != compound_order(page));

	spin_lock(&pool->lock);
	list_add_tail(&page->lru, &pool->items);
	pool->count++;
	spin_unlock(&pool->lock);
}

int sys_page_pool_nr_pages(struct page_pool *pool)
{
	int nr_total_pages;

	spin_lock(&pool->lock);
	nr_total_pages = pool->count << pool->order;
	spin_unlock(&pool->lock);

	return nr_total_pages;
}

struct page_pool *sys_page_pool_create(gfp_t gfp_mask, unsigned int order)
{
	struct page_pool *pool = kmalloc(sizeof(*pool), GFP_KERNEL);

	if (!pool)
		return NULL;
	pool->count = 0;
	INIT_LIST_HEAD(&pool->items);
	pool->gfp_mask = gfp_mask | __GFP_COMP;
	pool->order = order;
	spin_lock_init(&pool->lock);
	INIT_LIST_HEAD(&pool->list);

	return pool;
}

void sys_page_pool_destroy(struct page_pool *pool)
{
	kfree(pool);
}

MODULE_LICENSE("GPL v2");

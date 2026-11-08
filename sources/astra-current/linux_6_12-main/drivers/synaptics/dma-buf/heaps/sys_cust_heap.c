// SPDX-License-Identifier: GPL-2.0
/*
 * DMABUF System cust heap exporter
 *
 * Copyright (C) 2011 Google, Inc.
 * Copyright (C) 2019, 2020 Linaro Ltd.
 *
 * Portions based off of Andrew Davis' SRAM heap:
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 *	Andrew F. Davis <afd@ti.com>
 * Copyright (C) 2022, Synaptics, Inc.
 */

#include <linux/dma-buf.h>
#include <linux/dma-direct.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/iommu.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/scatterlist.h>
#include <linux/swiotlb.h>
#include <linux/vmalloc.h>
#include "berlin_meta.h"
#include "heap_extra.h"
#include <uapi/linux/dma-buf.h>
#include "kernel_compatibility.h"
#include "page_pool.h"

#define CREATE_TRACE_POINTS
#include "dmabuf_heap_trace.h"

/* include media frame buffer and GFX UI/Frame buffer in sys cust heap */
static int  max_rsv_threshold = 302;
static bool rsv_enabled;
static int err_cnt;
static time64_t rsv_start_time;

#define PAGE_CHUNK_NUM  64
#define MAX_PAGE_ALLOC_ERR_CNT  16
#define MIN_USING_THRESHOLD 80
#define RSV_DEFER 40000
#define RSV_TIMEOUT 15

struct sys_rsv_pool_s {
	struct page_pool *reserved_pool;
	struct delayed_work reserved_work;
	atomic_t heap_allocated;
};

struct sys_cust_heap_s {
	struct dma_heap *sys_heap;
	struct heap_extra heap_extra;
};

static struct sys_cust_heap_s sys_cust_heap;
static struct sys_cust_heap_s sys_cust_uncached_heap;
static struct sys_rsv_pool_s  sys_rsv_pool;

struct system_cust_heap_buffer {
	struct berlin_meta *meta;
	struct sys_cust_heap_s *heap;
	struct list_head attachments;
	struct mutex lock;
	unsigned long len;
	struct sg_table sg_table;
	int vmap_cnt;
	void *vaddr;

	bool uncached;
};

struct dma_heap_attachment {
	struct device *dev;
	struct sg_table *table;
	struct list_head list;
	bool mapped;

	bool uncached;
};

#define LOW_ORDER_GFP (GFP_HIGHUSER | __GFP_ZERO | __GFP_COMP)
#define HIGH_ORDER_GFP  (((GFP_HIGHUSER | __GFP_ZERO | __GFP_NOWARN \
				| __GFP_NORETRY) & ~__GFP_RECLAIM) \
				| __GFP_COMP)

static gfp_t order_flags[] = {HIGH_ORDER_GFP, HIGH_ORDER_GFP, LOW_ORDER_GFP,
	                      LOW_ORDER_GFP, LOW_ORDER_GFP};
static gfp_t rsv_order_gfp_flags  = GFP_HIGHUSER | __GFP_ZERO | __GFP_NOWARN;
/*
 * The selection of the orders used for allocation (1MB, 64K, 4K) is designed
 * to match with the sizes often found in IOMMUs. Using order 4 pages instead
 * of order 0 pages can significantly improve the performance of many IOMMUs
 * by reducing TLB pressure and time spent updating page tables.
 */
static const unsigned int orders[] = {4, 3, 2, 1, 0};
#define NUM_ORDERS ARRAY_SIZE(orders)

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0))
static bool needs_swiotlb_bounce(struct device *dev, struct sg_table *table)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct scatterlist *sg;
	int i;

	for_each_sgtable_dma_sg(table, sg, i) {
		// SG_DMA_SWIOTLB is set only for dma-iommu, not for dma-direct
		if (domain && IS_ENABLED(CONFIG_NEED_SG_DMA_FLAGS)) {
			if (sg_dma_is_swiotlb(table->sgl))
				return true;
		} else {
			phys_addr_t paddr = domain ?
					    iommu_iova_to_phys(domain, sg_dma_address(sg)) :
					    dma_to_phys(dev, sg_dma_address(sg));
			if (swiotlb_find_pool(dev, paddr))
				return true;
		}
	}
	return false;
}
#endif

static struct sg_table *dup_sg_table(struct sg_table *table)
{
	struct sg_table *new_table;
	int ret, i;
	struct scatterlist *sg, *new_sg;

	new_table = kzalloc(sizeof(*new_table), GFP_KERNEL);
	if (!new_table)
		return ERR_PTR(-ENOMEM);

	ret = sg_alloc_table(new_table, table->orig_nents, GFP_KERNEL);
	if (ret) {
		kfree(new_table);
		return ERR_PTR(-ENOMEM);
	}

	new_sg = new_table->sgl;
	for_each_sgtable_sg(table, sg, i) {
		sg_set_page(new_sg, sg_page(sg), sg->length, sg->offset);
		new_sg = sg_next(new_sg);
	}

	return new_table;
}

static int system_cust_heap_attach(struct dma_buf *dmabuf,
			      struct dma_buf_attachment *attachment)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a;
	struct sg_table *table;

	a = kzalloc(sizeof(*a), GFP_KERNEL);
	if (!a)
		return -ENOMEM;

	table = dup_sg_table(&buffer->sg_table);
	if (IS_ERR(table)) {
		kfree(a);
		return -ENOMEM;
	}

	a->table = table;
	a->dev = attachment->dev;
	INIT_LIST_HEAD(&a->list);
	a->mapped = false;
	a->uncached = buffer->uncached;
	attachment->priv = a;

	mutex_lock(&buffer->lock);
	list_add(&a->list, &buffer->attachments);
	mutex_unlock(&buffer->lock);

	return 0;
}

static void system_cust_heap_detach(struct dma_buf *dmabuf,
			       struct dma_buf_attachment *attachment)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a = attachment->priv;

	mutex_lock(&buffer->lock);
	list_del(&a->list);
	mutex_unlock(&buffer->lock);

	sg_free_table(a->table);
	kfree(a->table);
	kfree(a);
}

static struct sg_table *system_cust_heap_map_dma_buf(struct dma_buf_attachment *attachment,
						enum dma_data_direction direction)
{
	struct dma_heap_attachment *a = attachment->priv;
	struct sg_table *table = a->table;
	int attr = attachment->dma_map_attrs;
	int ret;

	if (a->uncached)
		attr |= DMA_ATTR_SKIP_CPU_SYNC;

	ret = dma_map_sgtable(attachment->dev, table, direction, attr);
	if (ret)
		return ERR_PTR(ret);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0))
	if (a->uncached && needs_swiotlb_bounce(attachment->dev, table)) {
		pr_err("Cannot map uncached system heap buffer for %s, as it requires SWIOTLB",
			dev_name(attachment->dev));
		dma_unmap_sgtable(attachment->dev, table, direction, attr);
		return ERR_PTR(-EINVAL);
	}
#endif
	a->mapped = true;
	return table;
}

static void system_cust_heap_unmap_dma_buf(struct dma_buf_attachment *attachment,
				      struct sg_table *table,
				      enum dma_data_direction direction)
{
	struct dma_heap_attachment *a = attachment->priv;
	int attr = attachment->dma_map_attrs;

	if (a->uncached)
		attr |= DMA_ATTR_SKIP_CPU_SYNC;
	a->mapped = false;
	dma_unmap_sgtable(attachment->dev, table, direction, attr);
}

static int system_cust_heap_dma_buf_begin_cpu_access(struct dma_buf *dmabuf,
						enum dma_data_direction direction)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a;

	mutex_lock(&buffer->lock);

	if (buffer->vmap_cnt)
		invalidate_kernel_vmap_range(buffer->vaddr, buffer->len);

	if (!buffer->uncached) {
		list_for_each_entry(a, &buffer->attachments, list) {
			if (!a->mapped)
				continue;
			dma_sync_sgtable_for_cpu(a->dev, a->table, direction);
		}
	}
	mutex_unlock(&buffer->lock);

	return 0;
}

static int system_cust_heap_dma_buf_end_cpu_access(struct dma_buf *dmabuf,
					      enum dma_data_direction direction)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a;

	mutex_lock(&buffer->lock);

	if (buffer->vmap_cnt)
		flush_kernel_vmap_range(buffer->vaddr, buffer->len);

	if (!buffer->uncached) {
		list_for_each_entry(a, &buffer->attachments, list) {
			if (!a->mapped)
				continue;
			dma_sync_sgtable_for_device(a->dev, a->table, direction);
		}
	}
	mutex_unlock(&buffer->lock);

	return 0;
}

static int system_cust_heap_mmap(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct sg_table *table = &buffer->sg_table;
	unsigned long addr = vma->vm_start;
	struct sg_page_iter piter;
	int ret;

	if (buffer->uncached)
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	for_each_sgtable_page(table, &piter, vma->vm_pgoff) {
		struct page *page = sg_page_iter_page(&piter);

		ret = remap_pfn_range(vma, addr, page_to_pfn(page), PAGE_SIZE,
				      vma->vm_page_prot);
		if (ret)
			return ret;
		addr += PAGE_SIZE;
		if (addr >= vma->vm_end)
			return 0;
	}
	return 0;
}

static void *system_cust_heap_do_vmap(struct system_cust_heap_buffer *buffer)
{
	struct sg_table *table = &buffer->sg_table;
	int npages = PAGE_ALIGN(buffer->len) / PAGE_SIZE;
	struct page **pages = vmalloc(sizeof(struct page *) * npages);
	struct page **tmp = pages;
	struct sg_page_iter piter;
	pgprot_t pgprot = PAGE_KERNEL;
	void *vaddr;

	if (!pages)
		return ERR_PTR(-ENOMEM);

	if (buffer->uncached)
		pgprot = pgprot_writecombine(PAGE_KERNEL);

	for_each_sgtable_page(table, &piter, 0) {
		WARN_ON(tmp - pages >= npages);
		*tmp++ = sg_page_iter_page(&piter);
	}

	vaddr = vmap(pages, npages, VM_MAP, pgprot);
	vfree(pages);

	if (!vaddr)
		return ERR_PTR(-ENOMEM);

	return vaddr;
}

static int system_cust_heap_vmap(struct dma_buf *dmabuf, struct iosys_map *map)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	void *vaddr;
	int ret = 0;

	mutex_lock(&buffer->lock);
	if (buffer->vmap_cnt) {
		buffer->vmap_cnt++;
		iosys_map_set_vaddr(map, buffer->vaddr);
		goto out;
	}

	vaddr = system_cust_heap_do_vmap(buffer);
	if (IS_ERR(vaddr)) {
		ret = PTR_ERR(vaddr);
		goto out;
	}

	buffer->vaddr = vaddr;
	buffer->vmap_cnt++;
	iosys_map_set_vaddr(map, buffer->vaddr);
out:
	mutex_unlock(&buffer->lock);

	return ret;
}

static void system_cust_heap_vunmap(struct dma_buf *dmabuf, struct iosys_map *map)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;

	mutex_lock(&buffer->lock);
	if (!--buffer->vmap_cnt) {
		vunmap(buffer->vaddr);
		buffer->vaddr = NULL;
	}
	mutex_unlock(&buffer->lock);
	iosys_map_clear(map);
}

static void free_buffer_page(struct page *page)
{
	unsigned int order = compound_order(page);
	bool rsv_low_wm, rsv_order_match;
	struct page_pool *rsv_pool = sys_rsv_pool.reserved_pool;

	rsv_low_wm = sys_page_pool_nr_pages(rsv_pool) <
		(max_rsv_threshold * (SZ_1M / PAGE_SIZE));
	rsv_order_match = (order == rsv_pool->order);

	if (rsv_order_match && rsv_low_wm) {
		sys_page_pool_free(rsv_pool, page);
	} else {
		__free_pages(page, compound_order(page));
	}
}

static void system_cust_heap_buf_free(struct dma_buf *dmabuf)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	struct sg_table *table;
	struct scatterlist *sg;
	int i;
	u64 start_time;
	bool uncached = false;
	unsigned long buf_size;
	int rsv_sz, total_alloc_sz;

	start_time = ktime_get_ns();

	buf_size = buffer->len;
	uncached = buffer->uncached;
	table = &buffer->sg_table;
	for_each_sgtable_sg(table, sg, i) {
		struct page *page = sg_page(sg);
		free_buffer_page(page);
	}
	sg_free_table(table);
	kfree(buffer->meta);
	kfree(buffer);
	total_alloc_sz = atomic_sub_return(buf_size, &sys_rsv_pool.heap_allocated);
	rsv_sz = sys_page_pool_nr_pages(sys_rsv_pool.reserved_pool) / (SZ_1M / PAGE_SIZE);
	trace_sys_cust_free(uncached, buf_size/1024, rsv_sz, total_alloc_sz / SZ_1M,
					 (ktime_get_ns()-start_time)/1000);
}

static int system_cust_heap_get_flags(struct dma_buf *dmabuf, unsigned long *flags)
{
	*flags = DMA_BUF_FLAG_CUST_HEAP;
	return 0;
}

static void system_cust_heap_dma_buf_release(struct dma_buf *dmabuf)
{
	struct system_cust_heap_buffer *buffer = dmabuf->priv;
	int (*free_cb)(struct dma_buf *dmabuf);

	free_cb = ((struct sys_cust_heap_s *) (buffer->heap))->heap_extra.free_cb;

	if (free_cb) {
		if (!free_cb(dmabuf))
			system_cust_heap_buf_free(dmabuf);
	} else
		system_cust_heap_buf_free(dmabuf);
}

static const struct dma_buf_ops system_cust_heap_buf_ops = {
	.attach = system_cust_heap_attach,
	.detach = system_cust_heap_detach,
	.map_dma_buf = system_cust_heap_map_dma_buf,
	.unmap_dma_buf = system_cust_heap_unmap_dma_buf,
	.begin_cpu_access = system_cust_heap_dma_buf_begin_cpu_access,
	.end_cpu_access = system_cust_heap_dma_buf_end_cpu_access,
	.mmap = system_cust_heap_mmap,
	.vmap = system_cust_heap_vmap,
	.vunmap = system_cust_heap_vunmap,
	.release = system_cust_heap_dma_buf_release,
	.get_flags = system_cust_heap_get_flags,
};

static void page_pool_add_pages(struct page_pool *rsv_pool,
						struct page **pages,
						u32 num)
{
	int i;

	for (i = 0; i < num; i++) {
		sys_page_pool_free(rsv_pool, pages[i]);
	}
}

static void page_pool_remove_pages(struct page_pool *rsv_pool)
{
	struct page *page;

	while (true) {
		page = sys_page_pool_alloc(rsv_pool);
		if (!page)
			break;
		__free_pages(page, compound_order(page));
	}
}

static struct page *alloc_buffer_page(gfp_t gfp_mask, unsigned long order)
{
	struct page *page = NULL;
	struct page_pool *rsv_pool = sys_rsv_pool.reserved_pool;

	if ((order == rsv_pool->order) && sys_page_pool_nr_pages(rsv_pool)) {
		page = sys_page_pool_alloc(rsv_pool);
	}

	if (!page) {
		if (fatal_signal_pending(current))
			return NULL;
		page = alloc_pages(gfp_mask, order);
	}
	return page;
}

static struct page *alloc_largest_available(unsigned long size,
											unsigned int max_order)
{
	struct page *page;
	int i;

	for (i = 0; i < NUM_ORDERS; i++) {
		if (size <  (PAGE_SIZE << orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

		page = alloc_buffer_page(order_flags[i], orders[i]);
		if (!page)
			continue;
		return page;
	}
	return NULL;
}


static struct dma_buf *system_cust_heap_do_allocate(struct dma_heap *heap,
					       unsigned long len,
					       FD_FLAGS_TYPE fd_flags,
					       HEAP_FLAGS_TYPE heap_flags,
					       bool uncached)
{
	struct system_cust_heap_buffer *buffer;
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
	unsigned long size_remaining = len;
	unsigned int max_order = orders[0];
	struct dma_buf *dmabuf;
	struct sg_table *table;
	struct scatterlist *sg;
	LIST_HEAD(pages);
	struct page *page, *tmp_page;
	char tmpbuf[64];
	int rsv_sz, total_alloc_sz;
	int i, ret = -ENOMEM;
	u64 start_time;
	start_time = ktime_get_ns();

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&buffer->attachments);
	mutex_init(&buffer->lock);
	buffer->heap = dma_heap_get_drvdata(heap);
	buffer->len = len;
	buffer->uncached = uncached;

	i = 0;
	while (size_remaining > 0) {
		/*
		 * Avoid trying to allocate memory if the process
		 * has been killed by SIGKILL
		 */
		if (fatal_signal_pending(current)) {
			ret = -EINTR;
			goto free_buffer;
		}

		page = alloc_largest_available(size_remaining, max_order);
		if (!page)
			goto free_buffer;

		list_add_tail(&page->lru, &pages);
		size_remaining -= page_size(page);
		max_order = compound_order(page);
		i++;
	}

	table = &buffer->sg_table;
	if (sg_alloc_table(table, i, GFP_KERNEL))
		goto free_buffer;

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &pages, lru) {
		sg_set_page(sg, page, page_size(page), 0);
		sg = sg_next(sg);
		list_del(&page->lru);
	}
	buffer->meta = kzalloc(sizeof(struct berlin_meta), GFP_KERNEL);
	if (!buffer->meta) {
		ret = -ENOMEM;
		goto free_pages;
	}

	/* create the dmabuf */
	exp_info.exp_name = dma_heap_get_name(heap);
	exp_info.ops = &system_cust_heap_buf_ops;
	exp_info.size = buffer->len;
	exp_info.flags = fd_flags;
	exp_info.priv = buffer;
	dmabuf = dma_buf_export(&exp_info);
	if (IS_ERR(dmabuf)) {
		ret = PTR_ERR(dmabuf);
		goto free_meta;
	}

	/*
	 * For uncached buffers, we need to initially flush cpu cache, since
	 * the __GFP_ZERO on the allocation means the zeroing was done by the
	 * cpu and thus it is likely cached. Map (and implicitly flush) and
	 * unmap it now so we don't get corruption later on.
	 */
	if (buffer->uncached) {
		dma_map_sgtable(dma_heap_get_dev(heap), table, DMA_BIDIRECTIONAL, 0);
		dma_unmap_sgtable(dma_heap_get_dev(heap), table, DMA_BIDIRECTIONAL, 0);
	}

	snprintf(tmpbuf, sizeof(tmpbuf), "%d %d %s\n", task_tgid_vnr(current),
				task_pid_vnr(current), current->comm);
	dma_buf_set_name(dmabuf, tmpbuf);

	total_alloc_sz = atomic_add_return(PAGE_ALIGN(len), &sys_rsv_pool.heap_allocated);
	rsv_sz = sys_page_pool_nr_pages(sys_rsv_pool.reserved_pool) / (SZ_1M / PAGE_SIZE);
	trace_sys_cust_alloc(uncached, len/1024, rsv_sz, total_alloc_sz / SZ_1M,
					  (ktime_get_ns() - start_time)/1000);
	return dmabuf;

free_meta:
	kfree(buffer->meta);
free_pages:
	for_each_sgtable_sg(table, sg, i) {
		struct page *p = sg_page(sg);
		__free_pages(p, compound_order(p));
	}
	sg_free_table(table);
free_buffer:
	list_for_each_entry_safe(page, tmp_page, &pages, lru)
		__free_pages(page, compound_order(page));
	kfree(buffer);

	return ERR_PTR(ret);
}

static struct dma_buf *system_cust_heap_allocate(struct dma_heap *heap,
					    unsigned long len,
					    FD_FLAGS_TYPE fd_flags,
					    HEAP_FLAGS_TYPE heap_flags)
{
	return system_cust_heap_do_allocate(heap, len, fd_flags, heap_flags, false);
}

static const struct dma_heap_ops system_cust_heap_ops = {
	.allocate = system_cust_heap_allocate,
};

static struct dma_buf *system_cust_uncached_heap_allocate(struct dma_heap *heap,
						     unsigned long len,
						     FD_FLAGS_TYPE fd_flags,
						     HEAP_FLAGS_TYPE heap_flags)
{
	return system_cust_heap_do_allocate(heap, len, fd_flags, heap_flags, true);
}

/* Dummy function to be used until we can call coerce_mask_and_coherent */
static struct dma_buf *system_cust_uncached_heap_not_initialized(struct dma_heap *heap,
							    unsigned long len,
							    FD_FLAGS_TYPE fd_flags,
							    HEAP_FLAGS_TYPE heap_flags)
{
	return ERR_PTR(-EBUSY);
}

static struct dma_heap_ops system_cust_uncached_heap_ops = {
	/* After system_cust_heap_create is complete, we will swap this */
	.allocate = system_cust_uncached_heap_not_initialized,
};

static void system_cust_heap_reserved_work(struct work_struct *work)
{
	struct sys_rsv_pool_s *sys_rsv = container_of(work,
				struct sys_rsv_pool_s, reserved_work.work);
	struct page *pages[PAGE_CHUNK_NUM];
	struct page_pool *rsv_pool = sys_rsv->reserved_pool;
	int i;
	long alloc_sz, rsv_sz, rsv_pages, using_sz, using_pages;
	bool is_timeout;

	using_sz = atomic_read(&sys_rsv->heap_allocated);
	rsv_sz = sys_page_pool_nr_pages(rsv_pool) / (SZ_1M / PAGE_SIZE);
	if (rsv_enabled) {
		rsv_pages = sys_page_pool_nr_pages(rsv_pool);
		using_pages = using_sz / PAGE_SIZE;

		pr_info("start reserve with %ldMB rsv, %ldMB in using\n", rsv_sz, using_sz / SZ_1M);
		alloc_sz = max_rsv_threshold * (SZ_1M / PAGE_SIZE) - rsv_pages - using_pages;
		alloc_sz = alloc_sz * PAGE_SIZE;
		i = 0;

		is_timeout = (ktime_get_seconds() - rsv_start_time) > RSV_TIMEOUT;
		while (alloc_sz > 0 && err_cnt < MAX_PAGE_ALLOC_ERR_CNT && !is_timeout && rsv_enabled) {
			pages[i] = alloc_pages(rsv_pool->gfp_mask, rsv_pool->order);
			if (!pages[i]) {
				err_cnt++;
				if (err_cnt == MAX_PAGE_ALLOC_ERR_CNT)
					pr_info("%s alloc page order %d failed with %d err, skip\n",
							__func__, rsv_pool->order, err_cnt);
				else
					schedule_delayed_work(&sys_rsv->reserved_work, msecs_to_jiffies(20));
				break;
			}
			i++;
			if (i == PAGE_CHUNK_NUM) {
				pr_debug("add %d pages\n", i);
				page_pool_add_pages(rsv_pool, pages, PAGE_CHUNK_NUM);
				i = 0;

			}
			rsv_pages = sys_page_pool_nr_pages(rsv_pool);
			using_pages = atomic_read(&sys_rsv->heap_allocated) / PAGE_SIZE;

			alloc_sz = max_rsv_threshold * (SZ_1M / PAGE_SIZE) - rsv_pages - using_pages;
			alloc_sz = alloc_sz * PAGE_SIZE;

			err_cnt = 0;
			is_timeout = (ktime_get_seconds() - rsv_start_time) > RSV_TIMEOUT;
			pr_debug("current %lld, start %lld\n", ktime_get_seconds(), rsv_start_time);
		}

		if (i)
			page_pool_add_pages(rsv_pool, pages, i);

		rsv_sz = sys_page_pool_nr_pages(rsv_pool) / (SZ_1M / PAGE_SIZE);
		using_sz = atomic_read(&sys_rsv->heap_allocated) / SZ_1M;
		pr_info("exit with %ldMB reserved mem, %ldMB in using with err %d, timeout %d\n",
					rsv_sz, using_sz, err_cnt, is_timeout);
	} else {
		pr_info("start free %ldMB reserved pool, %ldMB in using\n", rsv_sz, using_sz / SZ_1M);
		if (using_sz / SZ_1M <= MIN_USING_THRESHOLD) {
			page_pool_remove_pages(rsv_pool);
		} else {
			schedule_delayed_work(&sys_rsv->reserved_work, msecs_to_jiffies(RSV_DEFER));
		}
		rsv_sz = sys_page_pool_nr_pages(rsv_pool) / (SZ_1M / PAGE_SIZE);
		err_cnt = 0;
		pr_info("exit with %ldMB reserved mem\n", rsv_sz);
	}
}

static int create_reserved_pool(struct sys_rsv_pool_s *sys_rsv)
{
	struct page_pool *pool;

	pool = sys_page_pool_create(rsv_order_gfp_flags, orders[0]);
	if (!pool)
		return -ENOMEM;

	sys_rsv->reserved_pool = pool;
	INIT_DELAYED_WORK(&sys_rsv->reserved_work, system_cust_heap_reserved_work);
	atomic_set(&sys_rsv->heap_allocated, 0);
	return 0;
}

static void destroy_reserved_pool(struct sys_rsv_pool_s *sys_rsv)
{
	cancel_delayed_work_sync(&sys_rsv->reserved_work);
	page_pool_remove_pages(sys_rsv->reserved_pool);
	if (sys_rsv->reserved_pool)
		sys_page_pool_destroy(sys_rsv->reserved_pool);
}

static ssize_t rsv_enabled_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", rsv_enabled ? "true" : "false");
}

static ssize_t rsv_enabled_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t count)
{
	unsigned int value;
	bool is_enabled;
	int ret;
	unsigned int m;

	ret = kstrtouint(buf, 0, &value);
	if (ret < 0)
		return ret;
	if (value > 1)
		return -EINVAL;

	is_enabled = value ? true : false;
	pr_debug("reserved enabled %d, is_enabled %d", rsv_enabled, is_enabled);
	if (rsv_enabled != is_enabled) {
		rsv_enabled = is_enabled;
		m = rsv_enabled ? 0 : RSV_DEFER;
		cancel_delayed_work_sync(&sys_rsv_pool.reserved_work);
		rsv_start_time = ktime_get_seconds();
		schedule_delayed_work(&sys_rsv_pool.reserved_work, msecs_to_jiffies(m));
	}
	return count;
}

static ssize_t max_rsv_threshold_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%dMB\n", max_rsv_threshold);
}

static ssize_t max_rsv_threshold_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t count)
{
	unsigned int value;
	int ret;

	ret = kstrtouint(buf, 0, &value);
	if (ret < 0)
		return ret;
	if (!value || value * (SZ_1M / PAGE_SIZE) > totalram_pages() / 2) {
		pr_err("rsv size %dMB is invalid\n", value);
		return -ENOMEM;
	}
	if (max_rsv_threshold != value) {
		max_rsv_threshold = value;
	}
	return count;
}


static struct kobj_attribute rsv_enabled_attr =
	__ATTR(rsv_enabled, 0644, rsv_enabled_show,
		rsv_enabled_store);

static struct kobj_attribute max_rsv_threshold_attr =
	__ATTR(max_rsv_threshold, 0644, max_rsv_threshold_show,
		max_rsv_threshold_store);

static struct attribute *sys_cust_attrs[] = {
	&rsv_enabled_attr.attr,
	&max_rsv_threshold_attr.attr,
	NULL,
};

static struct attribute_group sys_cust_attr_group = {
	.attrs = sys_cust_attrs,
};

static int system_cust_init_sysfs(void)
{
	struct kobject *sys_cust_kobj;
	int ret;

	sys_cust_kobj = kobject_create_and_add("sys_cust", kernel_kobj);
	if (!sys_cust_kobj)
		return -ENOMEM;

	ret = sysfs_create_group(sys_cust_kobj, &sys_cust_attr_group);
	if (ret) {
		kobject_put(sys_cust_kobj);
		return ret;
	}
	return 0;
}

static int system_cust_heap_create(void)
{
	struct dma_heap_export_info exp_info;
	int ret;

	exp_info.name = "system_cust";
	exp_info.ops = &system_cust_heap_ops;
	exp_info.priv = &sys_cust_heap;

	sys_cust_heap.sys_heap = dma_heap_add(&exp_info);
	if (IS_ERR(sys_cust_heap.sys_heap))
		return PTR_ERR(sys_cust_heap.sys_heap);
	heap_extra_add_heap(&sys_cust_heap.heap_extra);

	exp_info.name = "system-cust-uncached";
	exp_info.ops = &system_cust_uncached_heap_ops;
	exp_info.priv = &sys_cust_uncached_heap;

	sys_cust_uncached_heap.sys_heap = dma_heap_add(&exp_info);
	if (IS_ERR(sys_cust_uncached_heap.sys_heap))
		return PTR_ERR(sys_cust_uncached_heap.sys_heap);
	heap_extra_add_heap(&sys_cust_uncached_heap.heap_extra);

	dma_coerce_mask_and_coherent(dma_heap_get_dev(sys_cust_uncached_heap.sys_heap),
					DMA_BIT_MASK(64));
	mb(); /* make sure we only set allocate after dma_mask is set */
	system_cust_uncached_heap_ops.allocate = system_cust_uncached_heap_allocate;

	ret = create_reserved_pool(&sys_rsv_pool);
	if (ret) {
		pr_err("system cust: create reserved pool failed.\n");
		goto err_heap_put;
	}

	ret = system_cust_init_sysfs();
	if (ret) {
		destroy_reserved_pool(&sys_rsv_pool);
		pr_err("system cust: failed to add sysfs attributes.\n");
		goto err_heap_put;
	}
	return ret;

err_heap_put:
	heap_extra_rm_heap(&sys_cust_heap.heap_extra);
	dma_heap_put(sys_cust_heap.sys_heap);
	heap_extra_rm_heap(&sys_cust_uncached_heap.heap_extra);
	dma_heap_put(sys_cust_uncached_heap.sys_heap);
	return ret;
}
module_init(system_cust_heap_create);
MODULE_LICENSE("GPL v2");
MODULE_IMPORT_NS(DMA_BUF);


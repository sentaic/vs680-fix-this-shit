// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/dma-buf.h>
#include <linux/refcount.h>
#include <linux/scatterlist.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <uapi/bm.h>
#include "isp_shm.h"

DEFINE_MUTEX(isp_shm_mutex);

struct isp_shm_data {
	struct device mem_device[SHM_MAX_MEMORY_TYPE];
	struct dma_heap *heap[SHM_MAX_MEMORY_TYPE];
};

struct isp_shm_buf {
	struct device *dev;
	void *vaddr;			/* Virtual address of kernel space */
	void *paddr;			/* Physical address frame */
	void *paddr_pt;			/* Physical address page table */
	unsigned long size;
	void *cookie;
	struct sg_table *dma_sgt;
	refcount_t refcount;

	/* DMABUF related */
	struct dma_buf_attachment *db_attach;
	struct iosys_map *map;
	struct bm_pt_param pt_param;
};

static struct isp_shm_data *isp_shm_data;

static void shm_release(struct device *dev)
{
	/*
	 * Nothing to do, just unregister sysfs here
	 *
	 */
}

/*
 * isp_shm_init - Initialize ISP shared memory subsystem
 * @dev: device pointer
 *
 * Initialize the shared memory subsystem for ISP. Only supports
 * SHM_NONSECURE_CONTIG memory type.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_init(struct device *dev)
{
	int ret = 0;
	const char *cma_heap_name = "linux,cma";

	ret = mutex_lock_interruptible(&isp_shm_mutex);
	if (ret) {
		pr_err("%s: can't lock isp shm mutex\n", __func__);
		ret = -EINVAL;
		goto lock_fail;
	}

	isp_shm_data = devm_kzalloc(dev, sizeof(struct isp_shm_data),
				      GFP_KERNEL);
	if (!isp_shm_data) {
		ret = -ENOMEM;
		goto mem_fail;
	}

	device_initialize(&isp_shm_data->mem_device[SHM_NONSECURE_CONTIG]);
	dev_set_name(&isp_shm_data->mem_device[SHM_NONSECURE_CONTIG],
		     "isp_shm");
	isp_shm_data->mem_device[SHM_NONSECURE_CONTIG].release = shm_release;

	ret = dma_coerce_mask_and_coherent(
			&isp_shm_data->mem_device[SHM_NONSECURE_CONTIG],
			DMA_BIT_MASK(32));
	if (ret) {
		pr_err("set dma mask 32b failed: %d\n", ret);
		goto device_free;
	}

	isp_shm_data->heap[SHM_NONSECURE_CONTIG] = dma_heap_find(cma_heap_name);
	if (!isp_shm_data->heap[SHM_NONSECURE_CONTIG]) {
		pr_err("dma_heap_find failed\n");
		ret = -ENOMEM;
		goto device_free;
	}

	mutex_unlock(&isp_shm_mutex);
	return 0;

device_free:
	put_device(&isp_shm_data->mem_device[SHM_NONSECURE_CONTIG]);
	devm_kfree(dev, isp_shm_data);
mem_fail:
	mutex_unlock(&isp_shm_mutex);
lock_fail:
	return ret;
}

/*
 * isp_shm_deinit - Deinitialize ISP shared memory subsystem
 * @dev: device pointer
 *
 * Clean up and deinitialize the shared memory subsystem for ISP.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_deinit(struct device *dev)
{
	int ret = 0;

	ret = mutex_lock_interruptible(&isp_shm_mutex);
	if (ret) {
		pr_err("%s: can't lock isp shm mutex\n", __func__);
		ret = -EINVAL;
		goto lock_fail;
	}

	if (isp_shm_data->heap[SHM_NONSECURE_CONTIG]) {
		dma_heap_put(isp_shm_data->heap[SHM_NONSECURE_CONTIG]);
		isp_shm_data->heap[SHM_NONSECURE_CONTIG] = NULL;
	}
	put_device(&isp_shm_data->mem_device[SHM_NONSECURE_CONTIG]);

	devm_kfree(dev, isp_shm_data);
	isp_shm_data = NULL;

	mutex_unlock(&isp_shm_mutex);
lock_fail:
	return ret;
}

/*
 * isp_shm_allocate - Allocate shared memory buffer
 * @mem_type_unused: unused parameter for compatibility
 * @size: size of buffer to allocate
 * @align: alignment requirement
 * @handle: pointer to store the allocated handle
 * @mem_type: memory type (only SHM_NONSECURE_CONTIG supported)
 *
 * Allocate a shared memory buffer of the specified size.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_allocate(unsigned int mem_type_unused, unsigned int size,
		    unsigned int align, shm_handle_t *handle,
		    enum isp_memory_type mem_type)
{
	struct isp_shm_buf *buf;
	int ret = 0;
	struct device *dev;

	if (mem_type >= SHM_MAX_MEMORY_TYPE)
		return -EINVAL;

	dev = &isp_shm_data->mem_device[mem_type];

	ret = mutex_lock_interruptible(&isp_shm_mutex);

	if (ret) {
		pr_err("%s: can't lock isp shm mutex\n", __func__);
		ret = -EINVAL;
		goto clean_lock;
	}

	buf = kzalloc(sizeof(struct isp_shm_buf), GFP_KERNEL);
	if (!buf) {
		ret = -ENOMEM;
		goto clean_buf;
	}

	buf->map = kzalloc(sizeof(struct iosys_map), GFP_KERNEL);
	if (IS_ERR_OR_NULL(buf->map)) {
		pr_err("%s: kzalloc failed for dma_map\n", __func__);
		ret = -ENOMEM;
		goto clean_alloc1;
	}

	buf->cookie = dma_heap_buffer_alloc(isp_shm_data->heap[mem_type], size, 0, 0);

	if (IS_ERR_OR_NULL(buf->cookie)) {
		pr_err("%s: alloc of size %d failed: %ld\n", __func__,
				size, PTR_ERR(buf->cookie));
		ret = -ENOMEM;
		goto clean_alloc2;
	}

	buf->db_attach = dma_buf_attach(buf->cookie, dev);
	if (IS_ERR_OR_NULL(buf->db_attach)) {
		pr_err("%s:dma attach failed\n",  __func__);
		ret = -ENOMEM;
		goto clean_attach;
	}

	buf->dma_sgt = dma_buf_map_attachment(buf->db_attach, DMA_BIDIRECTIONAL);
	if (IS_ERR_OR_NULL(buf->dma_sgt)) {
		pr_err("%s: dma map attachment failed for sgt\n", __func__);
		ret = -ENOMEM;
		goto clean_attachment;
	}

	if (dma_buf_begin_cpu_access(buf->cookie, DMA_BIDIRECTIONAL) != 0) {
		pr_err("%s: dma_buf_begin_cpu_access failed\n", __func__);
		ret = -ENOMEM;
		goto clean_cpu_access;
	}

	/* Only SHM_NONSECURE_CONTIG is supported */
	if (mem_type != SHM_NONSECURE_CONTIG) {
		pr_err("%s: Unsupported memory type: %d\n", __func__, mem_type);
		ret = -EINVAL;
		goto clean_cpu_access;
	}

	ret = dma_buf_vmap(buf->db_attach->dmabuf, buf->map);
	if (!ret && buf->map->vaddr) {
		buf->vaddr = buf->map->vaddr;
	} else {
		pr_err("%s: dma_buf_vmap failed\n", __func__);
		goto clean_vmap;
	}

	/* get the physical address of the memory */
	buf->paddr = (void *)sg_dma_address(buf->dma_sgt->sgl);
	/* Prevent the device from being released while the buffer is used */
	buf->dev = get_device(dev);
	buf->size = size;

	pr_debug("%s :The vaddr %lx paddr %lx size %d\n",
			__func__, (unsigned long)buf->vaddr, (unsigned long)buf->paddr, size);

	refcount_set(&buf->refcount, 1);

	*handle = (shm_handle_t) buf;
	mutex_unlock(&isp_shm_mutex);
	return 0;

clean_vmap:
	dma_buf_vunmap(buf->cookie, buf->map);
	dma_buf_end_cpu_access(buf->cookie, DMA_BIDIRECTIONAL);
clean_cpu_access:
	dma_buf_unmap_attachment(buf->db_attach, buf->dma_sgt, DMA_BIDIRECTIONAL);
clean_attachment:
	dma_buf_detach(buf->cookie, buf->db_attach);
clean_attach:
	dma_heap_buffer_free(buf->cookie);
clean_alloc2:
	kfree(buf->map);
clean_alloc1:
	kfree(buf);
clean_buf:
	mutex_unlock(&isp_shm_mutex);
clean_lock:
	*handle = (shm_handle_t) NULL;
	return ret;
}
EXPORT_SYMBOL(isp_shm_allocate);

/*
 * isp_shm_release - Release shared memory buffer
 * @handle: handle to the shared memory buffer
 *
 * Release a previously allocated shared memory buffer and free associated
 * resources when reference count reaches zero.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_release(shm_handle_t handle)
{
	struct isp_shm_buf *buf = (struct isp_shm_buf *)handle;
	int ret = 0;

	ret = mutex_lock_interruptible(&isp_shm_mutex);
	if (ret) {
		pr_err("%s: can't lock isp shm mutex\n", __func__);
		ret = -EINVAL;
		goto lock_fail;
	}

	if (!refcount_dec_and_test(&buf->refcount))
		goto non_zero_ref;

	if (buf->vaddr)
		dma_buf_vunmap(buf->cookie, buf->map);

	if (buf->db_attach) {
		dma_buf_unmap_attachment(buf->db_attach, buf->dma_sgt, DMA_BIDIRECTIONAL);
		dma_buf_detach(buf->cookie, buf->db_attach);
		buf->db_attach = NULL;
	}

	if (buf->cookie)
		dma_heap_buffer_free(buf->cookie);

	/* heap_extra.free_cb() would free its mem_id */
	dma_buf_put(buf->cookie);

	put_device(buf->dev);
	kfree(buf->map);
	kfree(buf);

non_zero_ref:
	mutex_unlock(&isp_shm_mutex);
lock_fail:
	return ret;
}
EXPORT_SYMBOL(isp_shm_release);

/*
 * isp_shm_mmap - Map shared memory buffer to user space
 * @handle: handle to the shared memory buffer
 * @vma: virtual memory area to map the buffer into
 *
 * Map the shared memory buffer into user space for direct access.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_mmap(shm_handle_t handle, struct vm_area_struct *vma)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		pr_err("%s: No buffer to map\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	vm_flags_set(vma, (vm_flags_t)(vma->vm_flags & (~VM_PFNMAP)));

	ret = dma_buf_mmap(buf->cookie, vma, 0);
	if (ret) {
		pr_err("%s: Remapping memory failed, error: %d\n", __func__, ret);
		goto out;
	}

	vm_flags_set(vma, (vm_flags_t)(vma->vm_flags | VM_DONTEXPAND | VM_DONTDUMP));
out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_mmap);

/*
 * isp_shm_clean_cache - Clean cache for shared memory buffer
 * @handle: handle to the shared memory buffer
 * @offset: offset within the buffer (unused)
 * @size: size to clean (unused)
 *
 * Clean the CPU cache for the shared memory buffer to ensure data coherency.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_clean_cache(shm_handle_t handle, unsigned int offset, unsigned int size)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		pr_err("%s: Invalid handle\n", __func__);
		goto out;
	}
	dma_buf_end_cpu_access(buf->cookie, DMA_BIDIRECTIONAL);

out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_clean_cache);

/*
 * isp_shm_invalidate_cache - Invalidate cache for shared memory buffer
 * @handle: handle to the shared memory buffer
 *
 * Invalidate the CPU cache for the shared memory buffer to ensure fresh
 * data is read from memory.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_invalidate_cache(shm_handle_t handle)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		pr_err("%s: Invalid handle\n", __func__);
		goto out;
	}
	if (dma_buf_begin_cpu_access(buf->cookie, DMA_BIDIRECTIONAL) != 0) {
		pr_err("%s: dma_buf_begin_cpu_access failed\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_invalidate_cache);

/*
 * isp_shm_get_virtual_address - Get virtual address of shared memory buffer
 * @handle: handle to the shared memory buffer
 * @offset: offset within the buffer (unused)
 * @virt_addr: pointer to store the virtual address
 *
 * Retrieve the virtual address of the shared memory buffer for kernel access.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_get_virtual_address(shm_handle_t handle, int offset, void **virt_addr)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		ret = -EINVAL;
		pr_err("%s: Invalid handle\n", __func__);
		goto out;
	}
	*virt_addr = buf->vaddr;
out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_get_virtual_address);

/*
 * isp_shm_get_physical_address - Get physical address of shared memory buffer
 * @handle: handle to the shared memory buffer
 * @offset: offset within the buffer (unused)
 * @phys_addr: pointer to store the physical address
 *
 * Retrieve the physical address of the shared memory buffer for DMA operations.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_get_physical_address(shm_handle_t handle, int offset, void **phys_addr)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		ret = -EINVAL;
		pr_err("%s: Invalid handle\n", __func__);
		goto out;
	}
	*phys_addr = buf->paddr;
out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_get_physical_address);

/*
 * isp_shm_get_page_table_address - Get page table address of shared memory buffer
 * @handle: handle to the shared memory buffer
 * @phys_addr: pointer to store the page table physical address
 *
 * Retrieve the page table physical address of the shared memory buffer
 * for MMU operations.
 *
 * Return: 0 on success, negative error code on failure
 */
int isp_shm_get_page_table_address(shm_handle_t handle, void **phys_addr)
{
	struct isp_shm_buf *buf;
	int ret = 0;

	buf = (struct isp_shm_buf *)handle;
	if (!buf) {
		ret = -EINVAL;
		pr_err("%s: Invalid handle\n", __func__);
		goto out;
	}
	*phys_addr = buf->paddr_pt;
out:
	return ret;
}
EXPORT_SYMBOL(isp_shm_get_page_table_address);

MODULE_IMPORT_NS(DMA_BUF);

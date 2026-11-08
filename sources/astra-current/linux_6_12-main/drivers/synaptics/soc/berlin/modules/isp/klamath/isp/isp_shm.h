// SPDX-License-Identifier: GPL-2.0
/*
* Copyright (C) 2021 - 2023 Synaptics Incorporated
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/

#ifndef __ISP_SHM_H__
#define __ISP_SHM_H__

#include <linux/types.h>
#include <linux/device.h>

#define SHM_NONSECURE	0

typedef u64 shm_handle_t;

enum isp_memory_type {
	SHM_NONSECURE_CONTIG = 0,
	SHM_MAX_MEMORY_TYPE,
};

int isp_shm_allocate(unsigned int mem_type_unused, unsigned int size,
		    unsigned int align, shm_handle_t *handle,
		    enum isp_memory_type mem_type);
int isp_shm_release(shm_handle_t handle);
int isp_shm_clean_cache(shm_handle_t handle, unsigned int offset,
		       unsigned int size);
int isp_shm_invalidate_cache(shm_handle_t handle);
int isp_shm_get_virtual_address(shm_handle_t handle, int offset,
			      void **virt_addr);
int isp_shm_get_physical_address(shm_handle_t handle, int offset,
			       void **phys_addr);
int isp_shm_get_page_table_address(shm_handle_t handle, void **phys_addr);
int isp_shm_init(struct device *dev);
int isp_shm_deinit(struct device *dev);
int isp_shm_mmap(shm_handle_t handle, struct vm_area_struct *vma);

#endif

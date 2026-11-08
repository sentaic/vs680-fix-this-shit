// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2025 Synaptics Incorporated */

#pragma once

#ifndef __KERNEL_COMPATIBILITY_H__
#define __KERNEL_COMPATIBILITY_H__

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0)
struct dma_buf_map {
	void *vaddr;
};
#define compatible_dma_buf_vmap(dmabuf, map) \
	({ \
		(map)->vaddr = dma_buf_vmap(dmabuf); \
		(map)->vaddr ? 0 : ((dmabuf) && (dmabuf)->ops->vmap) ? -ENOMEM : -EINVAL; \
	})
#define compatible_dma_buf_vunmap(dmabuf, map) \
	({ \
		dma_buf_vunmap(dmabuf, (map)->vaddr); \
		(map)->vaddr = NULL; \
	})
#else
#define compatible_dma_buf_vmap(dmabuf, map) dma_buf_vmap(dmabuf, map)
#define compatible_dma_buf_vunmap(dmabuf, map) dma_buf_vunmap(dmabuf, map)
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0) */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 18, 0)
#define dma_buf_map iosys_map
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 18, 0) */

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 11, 0)
#define compatible_get_user_pages(start, nr_pages, gup_flags, pages) \
	get_user_pages(start, nr_pages, gup_flags, pages, NULL)
#else
#define compatible_get_user_pages(start, nr_pages, gup_flags, pages) \
	get_user_pages(start, nr_pages, gup_flags, pages)
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(6, 11, 0) */

#endif


// SPDX-License-Identifier: GPL-2.0-only
#pragma once
#include <linux/version.h>
#include <linux/tee_drv.h>
#include <linux/kernel.h>
#ifndef TEE_IOCTL_LOGIN_REE_KERNEL
#define TEE_IOCTL_LOGIN_REE_KERNEL 0x80000000
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
#define tee_shm_alloc_kernel_buf(ctx, size) tee_shm_alloc((ctx), (size), TEE_SHM_MAPPED | TEE_SHM_DMA_BUF)
#define mmap_read_lock_killable(mm) down_read_killable(&(mm)->mmap_sem)
#define mmap_read_lock(mm) down_read(&(mm)->mmap_sem)
#define mmap_read_unlock(mm) up_read(&(mm)->mmap_sem)
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
#define sysfs_emit(buf, fmt, ...) scnprintf((buf), PAGE_SIZE, (fmt), ##__VA_ARGS__)
#endif

#ifndef DEVICE_NAME
#define DEVICE_NAME "synap"
#endif

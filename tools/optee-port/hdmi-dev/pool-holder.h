// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_POOL_HOLDER_H
#define VS680_HDMI_DEV_POOL_HOLDER_H

#include <stddef.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include "hdmi-dev.h"

/* ION custom(cmd=0) 返回分配的物理地址；B0 将 DMA-BUF 导入缓冲驱动并固定其引用。 */
struct pool_ion_heap { char name[32]; uint32_t type, id, reserved[3]; };
struct pool_ion_query { uint32_t count, reserved0; uint64_t heaps; uint32_t reserved[2]; };
struct pool_ion_alloc { uint64_t length; uint32_t mask, flags, fd, unused; };
struct pool_ion_custom { uint32_t fd, command; uint64_t argument; };
struct pool_ion_region { uint32_t command, offset; uint64_t address, length; };
struct pool_buffer_map { uint32_t fd, reserved; uint64_t address, length; };

#define POOL_ION_ALLOC _IOWR('I', 0, struct pool_ion_alloc)
#define POOL_ION_CUSTOM _IOWR('I', 6, struct pool_ion_custom)
#define POOL_ION_QUERY _IOWR('I', 8, struct pool_ion_query)
#define POOL_BUFFER_IMPORT _IOWR('B', 0, struct pool_buffer_map)
#define POOL_HOLDER_FIRST ((uint64_t)HDMI_DEV_POOL_BASE)
#define POOL_HOLDER_END ((uint64_t)HDMI_DEV_POOL_LIMIT)
#define POOL_HOLDER_ALIGNMENT ((uint64_t)HDMI_DEV_POOL_ALIGNMENT)
#define POOL_HOLDER_REQUIRED_BYTES HDMI_DEV_POOL_BYTES

_Static_assert(sizeof(struct pool_ion_heap) == 52, "ION heap ABI");
_Static_assert(sizeof(struct pool_ion_query) == 24, "ION query ABI");
_Static_assert(sizeof(struct pool_ion_alloc) == 24, "ION allocation ABI");
_Static_assert(sizeof(struct pool_ion_custom) == 16, "ION custom ABI");
_Static_assert(sizeof(struct pool_ion_region) == 24, "ION region ABI");
_Static_assert(sizeof(struct pool_buffer_map) == 24, "display pool import ABI");
_Static_assert(offsetof(struct pool_buffer_map, address) == 8, "display pool address ABI");
_Static_assert(offsetof(struct pool_buffer_map, length) == 16, "display pool length ABI");

struct pool_holder {
    int holder_fd, ion_fd, pool_fd;
    int imported;
    uint32_t address, bytes;
};
#define POOL_HOLDER_INIT { -1, -1, -1, 0, 0, 0 }

int pool_holder_parse_bytes(const char *text, uint32_t *bytes);
int pool_holder_region_valid(uint64_t address, uint64_t length, uint32_t requested);
int pool_holder_acquire(struct pool_holder *pool, uint32_t bytes, const char **stage);
/* 关闭用户态描述符；已导入的分配由内核保留至板级复位。 */
int pool_holder_close(struct pool_holder *pool);

#endif

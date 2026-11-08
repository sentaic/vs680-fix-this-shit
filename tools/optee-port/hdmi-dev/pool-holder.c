// SPDX-License-Identifier: GPL-2.0-only
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pool-holder.h"

int pool_holder_parse_bytes(const char *text, uint32_t *bytes)
{
    uint64_t value = 0;
    if (!text || !*text || !bytes) return -1;
    for (const char *p = text; *p; ++p) {
        if (*p < '0' || *p > '9') return -1;
        unsigned digit = (unsigned)(*p - '0');
        if (value > (POOL_HOLDER_REQUIRED_BYTES - digit) / 10) return -1;
        value = value * 10 + digit;
    }
    if (value != POOL_HOLDER_REQUIRED_BYTES) return -1;
    *bytes = (uint32_t)value;
    return 0;
}

int pool_holder_region_valid(uint64_t address, uint64_t length, uint32_t requested)
{
    return address >= POOL_HOLDER_FIRST && address <= UINT32_MAX &&
        hdmi_dev_pool_valid((uint32_t)address, requested) &&
        length >= requested && !(length % POOL_HOLDER_ALIGNMENT) &&
        length <= POOL_HOLDER_END - address;
}

static int close_once(int *fd)
{
    if (*fd < 0) return 0;
    int consumed = *fd;
    *fd = -1;
    /* close 后清除本地描述符记录，避免再次使用旧编号。 */
    return close(consumed);
}

int pool_holder_close(struct pool_holder *pool)
{
    int error = 0;
    /* 关闭分配描述符前，将已导入的引用交给内核保留。 */
    if (close_once(&pool->holder_fd)) error = errno;
    if (close_once(&pool->pool_fd) && !error) error = errno;
    if (close_once(&pool->ion_fd) && !error) error = errno;
    if (error) { errno = error; return -1; }
    return 0;
}

static int acquire(struct pool_holder *pool, uint32_t bytes, const char **stage, int cached)
{
    struct pool_ion_heap heaps[32] = {0};
    struct pool_ion_query query = {.count = 32, .heaps = (uintptr_t)heaps};
    struct pool_ion_alloc allocation = {.length = bytes,.flags = cached?1u:0u};
    const char *name=cached?"DisplayCached":"NonSecure";
    uint64_t first=cached?HDMI_DEV_CACHED_POOL_BASE:POOL_HOLDER_FIRST;
    uint64_t limit=cached?HDMI_DEV_CACHED_POOL_LIMIT:POOL_HOLDER_END;
    struct pool_ion_region region = {0};
    struct pool_buffer_map pinned = {0};
    unsigned matches = 0;
    *stage = "参数";
    if (pool->holder_fd != -1 || pool->ion_fd != -1 || pool->pool_fd != -1 ||
        pool->imported || bytes != (cached?HDMI_DEV_CACHED_POOL_BYTES:POOL_HOLDER_REQUIRED_BYTES) ||
        bytes % POOL_HOLDER_ALIGNMENT) { errno = EINVAL; return -1; }
    *stage = "打开显示池保留设备";
    pool->holder_fd = open(cached?"/dev/vs680-display-cached-pool-buffer":"/dev/vs680-display-pool-buffer", O_RDWR | O_CLOEXEC);
    if (pool->holder_fd < 0) return -1;
    *stage = "打开 ION";
    pool->ion_fd = open("/dev/ion", O_RDWR | O_CLOEXEC);
    if (pool->ion_fd < 0) return -1;
    *stage = "查询 ION 内存堆";
    if (ioctl(pool->ion_fd, POOL_ION_QUERY, &query)) return -1;
    if (query.count > 32) { errno = EOVERFLOW; return -1; }
    for (uint32_t i = 0; i < query.count; ++i) {
        if (strncmp(heaps[i].name, name, sizeof(heaps[i].name))) continue;
        if (heaps[i].id >= 32) { errno = EOVERFLOW; return -1; }
        allocation.mask = UINT32_C(1) << heaps[i].id;
        ++matches;
    }
    if (matches != 1) { errno = matches ? EINVAL : ENODEV; return -1; }
    *stage = "分配 ION 显示池";
    if (ioctl(pool->ion_fd, POOL_ION_ALLOC, &allocation)) return -1;
    if (allocation.fd > INT_MAX || (int)allocation.fd == pool->ion_fd ||
        (int)allocation.fd == pool->holder_fd) { errno = EOVERFLOW; return -1; }
    pool->pool_fd = (int)allocation.fd;
    if (allocation.length != bytes) { errno = EINVAL; return -1; }
    *stage = "设置分配描述符";
    if (fcntl(pool->pool_fd, F_SETFD, FD_CLOEXEC)) return -1;
    struct pool_ion_custom custom = {
        .fd = allocation.fd, .argument = (uintptr_t)&region
    };
    *stage = "查询 ION 物理地址";
    if (ioctl(pool->ion_fd, POOL_ION_CUSTOM, &custom)) return -1;
    if (region.address<first || region.address>=limit || region.length<bytes ||
        region.length>limit-region.address || region.length%POOL_HOLDER_ALIGNMENT ||
        !hdmi_dev_pool_valid(region.address,bytes)) {
        errno = ERANGE; return -1;
    }
    pinned.fd = allocation.fd;
    *stage = "固定显示池";
    /* 新打开的设备仅导入一次。驱动的所有失败路径均撤销本次导入引用。 */
    if (ioctl(pool->holder_fd, POOL_BUFFER_IMPORT, &pinned)) return -1;
    pool->imported = 1;
    *stage = "核对固定地址及完整长度";
    if (pinned.address != region.address || pinned.length != region.length ||
        pinned.address<first || pinned.address>=limit || pinned.length<bytes ||
        pinned.length>limit-pinned.address || pinned.length%POOL_HOLDER_ALIGNMENT ||
        !hdmi_dev_pool_valid(pinned.address,bytes)) {
        errno = ERANGE; return -1;
    }
    pool->address = (uint32_t)pinned.address;
    pool->bytes = bytes;
    return 0;
}

int pool_holder_acquire(struct pool_holder *pool, uint32_t bytes, const char **stage)
{ return acquire(pool,bytes,stage,0); }

#ifndef POOL_HOLDER_NO_MAIN
int main(int argc, char **argv)
{
    int cached=argc==2 && !strcmp(argv[1],"--cached");
    uint32_t bytes = cached?HDMI_DEV_CACHED_POOL_BYTES:POOL_HOLDER_REQUIRED_BYTES;
    if (argc > 2 || (argc == 2 && !cached && pool_holder_parse_bytes(argv[1], &bytes))) {
        fprintf(stderr, "用法: %s [8388608|--cached]\n"
                "默认使用NC 8MiB；--cached使用已预留缓存32MiB；"
                "物理地址须在显示池窗口内并按 4096 字节对齐。\n", argv[0]);
        return 2;
    }
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTERM);
    sigaddset(&signals, SIGHUP);
    /* 等待终止信号；退出后由内核继续持有已导入的缓冲。 */
    if (sigprocmask(SIG_BLOCK, &signals, NULL)) { perror("阻塞终止信号失败"); return 1; }
    struct pool_holder pool = POOL_HOLDER_INIT;
    const char *stage = NULL;
    if (acquire(&pool, bytes, &stage,cached)) {
        int error = errno;
        fprintf(stderr, "%s失败: %s\n", stage, strerror(error));
        if (pool.imported)
            fputs("导入已成功；本次分配由内核保留，板级复位后可开始新批次。\n", stderr);
        if (pool_holder_close(&pool)) perror("关闭描述符失败");
        return 1;
    }
    /* 发布物理地址和大小，供同一批次的客户端复用。 */
    if (printf("HDMI_DEV_POOL_PA=0x%08" PRIx32 "\nHDMI_DEV_POOL_BYTES=%" PRIu32 "\n",
               pool.address, pool.bytes) < 0 || fflush(stdout)) {
        perror("输出显示池配置失败");
        if (pool_holder_close(&pool)) perror("关闭描述符失败");
        return 1;
    }
    fprintf(stderr, "显示池已通过 %s 固定，保留进程 PID=%ld；命令: hdmi-client run 0x%08"
            PRIx32 " %" PRIu32 "\n"
            "保持本进程存活。CA 退出、TA Close/Panic/reload 均不释放显示池。\n"
            "本进程退出后由匹配的内核驱动继续保留；板级复位或完成硬件停止后回收。\n",
            cached?"/dev/vs680-display-cached-pool-buffer":"/dev/vs680-display-pool-buffer",
            (long)getpid(), pool.address, pool.bytes);
    int signal_number = 0;
    int error = sigwait(&signals, &signal_number);
    if (error) fprintf(stderr, "等待信号失败: %s\n", strerror(error));
    else fprintf(stderr, "收到信号 %d，内核继续保留已导入的分配。\n", signal_number);
    int closed = pool_holder_close(&pool);
    if (closed) perror("关闭描述符失败");
    return error || closed ? 1 : 128 + signal_number;
}
#endif

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef OWNER_LOCK_H
#define OWNER_LOCK_H
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

/* 参与进程使用固定锁文件路径。D 记录当前启动周期内的占用状态，板级复位后重新初始化。 */
struct owner_lock {
    int fd, dirty;
    char boot[37];
    const char *path;
};
#define OWNER_LOCK_INIT { .fd = -1 }

static inline int owner_lock_boot(char value[37])
{
    int fd = open("/proc/sys/kernel/random/boot_id", O_RDONLY | O_CLOEXEC);
    if (fd < 0) return -1;
    ssize_t size;
    do size = read(fd, value, 36); while (size < 0 && errno == EINTR);
    int closed = close(fd);
    if (size != 36 || closed) return -1;
    value[36] = 0;
    return 0;
}
#ifndef OWNER_LOCK_BOOT_READ
#define OWNER_LOCK_BOOT_READ owner_lock_boot
#endif

static inline int owner_lock_uuid(const char *value)
{
    for (unsigned i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (value[i] != '-') return 0;
        } else if (!((value[i] >= '0' && value[i] <= '9') ||
                     (value[i] >= 'a' && value[i] <= 'f') ||
                     (value[i] >= 'A' && value[i] <= 'F'))) return 0;
    }
    return 1;
}
static inline int owner_lock_inode(const struct owner_lock *owner)
{
    struct stat opened, named;
    if (owner->fd < 0 || !owner->path) { errno = EINVAL; return -1; }
    if (fstat(owner->fd, &opened) || lstat(owner->path, &named)) return -1;
    if (!S_ISREG(opened.st_mode) || opened.st_nlink != 1 ||
        opened.st_uid != geteuid() || (opened.st_mode & 0022) ||
        opened.st_dev != named.st_dev || opened.st_ino != named.st_ino) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}
static inline void owner_lock_release(struct owner_lock *owner)
{
    if (owner->fd >= 0) close(owner->fd);
    owner->fd = -1;
}
static inline int owner_lock_write(int fd, const void *bytes, size_t length,
                                   off_t offset)
{
    ssize_t written;
    do written = pwrite(fd, bytes, length, offset); while (written < 0 && errno == EINTR);
    if (written == (ssize_t)length) return 0;
    if (written >= 0) errno = EIO;
    return -1;
}
static inline int owner_lock_acquire(struct owner_lock *owner, const char *path)
{
    char record[39];
    *owner = (struct owner_lock)OWNER_LOCK_INIT;
    owner->path = path;
    if (!path || OWNER_LOCK_BOOT_READ(owner->boot) || !owner_lock_uuid(owner->boot)) {
        errno = EIO;
        return -1;
    }
    owner->fd = open(path, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK, 0600);
    if (owner->fd < 0) return -1;
    if (owner_lock_inode(owner) || flock(owner->fd, LOCK_EX | LOCK_NB) ||
        owner_lock_inode(owner)) goto fail;
    ssize_t size;
    do size = pread(owner->fd, record, sizeof(record), 0); while (size < 0 && errno == EINTR);
    /* 首次创建锁文件时，其内容为空。 */
    if (size && (size != 38 || !owner_lock_uuid(record) || record[36] != '\n' ||
                 (record[37] != 'C' && record[37] != 'D'))) {
        errno = EIO;
        goto fail;
    }
    if (size && !memcmp(record, owner->boot, 36) && record[37] == 'D') {
        errno = EBUSY;
        goto fail;
    }
    return 0;
fail:
    {
        int saved = errno;
        owner_lock_release(owner);
        errno = saved;
        return -1;
    }
}
static inline int owner_lock_begin(struct owner_lock *owner)
{
    char record[38];
    if (owner_lock_inode(owner)) return -1;
    memcpy(record, owner->boot, 36);
    record[36] = '\n';
    record[37] = 'D';
    owner->dirty = 1;
    if (owner_lock_write(owner->fd, record, sizeof(record), 0) ||
        ftruncate(owner->fd, sizeof(record)) || fsync(owner->fd)) return -1;
    return 0;
}
/* 硬件停止及资源清理完成后，将记录改为 C；文件锁在 release 时释放。 */
static inline int owner_lock_clean(struct owner_lock *owner)
{
    if (!owner->dirty) { errno = EINVAL; return -1; }
    if (owner_lock_inode(owner)) return -1;
    if (owner_lock_write(owner->fd, "C", 1, 37)) return -1;
    owner->dirty = 0;
    return 0;
}
#endif

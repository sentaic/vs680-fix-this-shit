/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DISPLAY_SHUTDOWN_H
#define DISPLAY_SHUTDOWN_H
#include "owner-lock.h"
#define DISPLAY_SHUTDOWN_RECORD "/tmp/vs680-display-shutdown.ready"
static inline int display_shutdown_boot(char value[37])
{
    return OWNER_LOCK_BOOT_READ(value);
}
static inline int display_shutdown_record(void)
{
    char boot[37];
    if (display_shutdown_boot(boot)) return -1;
    int fd = open(DISPLAY_SHUTDOWN_RECORD,
        O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW, 0600);
    if (fd < 0) return -1;
    ssize_t size = write(fd, boot, 36);
    int result = size == 36 ? 0 : -1;
    if (close(fd)) result = -1;
    return result;
}
static inline int display_shutdown_confirmed(void)
{
    char boot[37], record[37] = {0};
    if (display_shutdown_boot(boot)) return 0;
    int fd = open(DISPLAY_SHUTDOWN_RECORD, O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (fd < 0) return 0;
    ssize_t size = read(fd, record, sizeof(record));
    close(fd);
    return size == 36 && !memcmp(boot, record, 36);
}
#endif

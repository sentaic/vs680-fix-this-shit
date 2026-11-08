/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DISPLAY_OWNER_H
#define DISPLAY_OWNER_H
#include "owner-lock.h"
#include "display-shutdown.h"
#ifndef DISPLAY_OWNER_LOCK
#define DISPLAY_OWNER_LOCK "/run/vs680-display-owner.lock"
#endif
struct display_owner { struct owner_lock lock; };
#define DISPLAY_OWNER_INIT { .lock = OWNER_LOCK_INIT }

static inline void display_owner_release(struct display_owner *owner)
{
    owner_lock_release(&owner->lock);
}
static inline int display_owner_acquire(struct display_owner *owner,
                                        int require_shutdown)
{
    if (owner_lock_acquire(&owner->lock, DISPLAY_OWNER_LOCK)) return -1;
    if (require_shutdown && !display_shutdown_confirmed()) {
        display_owner_release(owner);
        errno = EBUSY;
        return -1;
    }
    return 0;
}
static inline int display_owner_begin(struct display_owner *owner,
                                      int invalidate_shutdown)
{
    if (owner_lock_begin(&owner->lock)) return -1;
    if (invalidate_shutdown && unlink(DISPLAY_SHUTDOWN_RECORD) && errno != ENOENT)
        return -1;
    return 0;
}
/* 本域硬件停止、TX 缓冲回收和清理成功后更新显示停止记录。 */
static inline int display_owner_stopped(struct display_owner *owner)
{
    if (!owner->lock.dirty) { errno = EINVAL; return -1; }
    if (owner_lock_inode(&owner->lock) || display_shutdown_record()) return -1;
    return owner_lock_clean(&owner->lock);
}
#endif

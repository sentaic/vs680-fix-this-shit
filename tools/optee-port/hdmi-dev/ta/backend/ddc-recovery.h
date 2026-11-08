/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DDC_RECOVERY_H
#define HDMI_DDC_RECOVERY_H
#include "vs680_ddc_diag.h"
#define VD_RECOVER_ENTERED 1u
#define VD_RECOVER_DONE 2u
#define VD_RECOVER_ACKNOWLEDGED 4u
#define VD_RECOVER_RESTORED 8u
#define VD_RECOVER_IDLE 16u
struct vd_recovery {
    int result, original_result;
    uint32_t flags, status, selectors[4];
};
/* 收取当前拥有者已发出的读事务，过期时返回错误。 */
int vd_recover(const struct vd_ops *, const struct vd_report *,
               struct vd_recovery *);
#endif

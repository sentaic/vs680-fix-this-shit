/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_CAPTURE_NOTIFY_H
#define RX_CAPTURE_NOTIFY_H
#include <stdint.h>
/* 采集器发布记录，读取方使用 acquire 顺序读取行数。 */
struct capture_notify {
    uint32_t rows, busy, queued, total, done, result;
};
#endif

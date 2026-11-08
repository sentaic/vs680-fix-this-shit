/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_AIO_EXCLUSIVE_COMMAND_H
#define RX_AIO_EXCLUSIVE_COMMAND_H
#include <stdint.h>
#define RX_AIO_EXCLUSIVE_COMMAND 0x7f36u
#define RX_AIO_EXCLUSIVE_VERSION 3u
#define RX_AIO_EXCLUSIVE_ACK 0x52584131u
enum rx_aio_exclusive_operation { RX_AIO_PREPARE=0, RX_AIO_RELEASE=1, RX_AIO_STATUS=2 };
/* issued 记录此 TA 发出的配置写入。 */
struct rx_aio_exclusive_command {
    uint32_t version, size, operation, acknowledgement;
    uint64_t epoch;
    uint32_t issued_channel_start, issued_channel_cfg, issued_command_fifo_cfg, issued_data_fifo_cfg;
    uint32_t bcm_empty, autopush, error, owned;
    uint32_t dhub_base, hbo_base, declared_base, declared_hbo;
};
#endif

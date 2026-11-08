/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_COMMAND_POOL_COMMAND_H
#define RX_COMMAND_POOL_COMMAND_H
#include <stdint.h>
#define RX_COMMAND_POOL_COMMAND 0x7f35u
#define RX_COMMAND_POOL_VERSION 1u
#define RX_COMMAND_POOL_ACK 0x52585031u
#define RX_COMMAND_POOL_PA 0x76400000u
#define RX_COMMAND_POOL_BYTES 0x00800000u
enum rx_command_pool_operation {
    RX_COMMAND_POOL_BIND = 0, RX_COMMAND_POOL_STATUS = 1, RX_COMMAND_POOL_UNBIND = 2
};
struct rx_command_pool_command {
    uint32_t version, size, operation, pa, bytes, reserved, mem_id, acknowledgement;
    uint64_t epoch;
};
#endif

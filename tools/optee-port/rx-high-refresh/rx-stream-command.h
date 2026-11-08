/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_STREAM_COMMAND_H
#define RX_STREAM_COMMAND_H
#include "rx-stream.h"
#define RX_STREAM_OPEN_COMMAND 0x7f33u
#define RX_STREAM_SERVICE_COMMAND 0x7f34u
#define RX_STREAM_VERSION 1u
#define RX_STREAM_ACK 0x52585331u
enum rx_stream_operation {
    RX_STREAM_OP_OPEN=0, RX_STREAM_OP_QUEUE=1, RX_STREAM_OP_SERVICE=2,
    RX_STREAM_OP_DEQUEUE=3, RX_STREAM_OP_RELEASE=4, RX_STREAM_OP_STOP=5,
    RX_STREAM_OP_STATUS=6, RX_STREAM_OP_CLOSE=7,
    RX_STREAM_OP_EXCLUSIVE_CAPABILITY=8, RX_STREAM_OP_PREPARE_EXCLUSIVE=9
};
struct rx_stream_command {
    uint32_t version, size, operation, slot, count, flags;
    uint64_t epoch, token, frame_bytes;
    struct rx_stream_buffer buffers[RX_STREAM_MAX_SLOTS];
    int32_t software_result;
    uint32_t acknowledgement, can_unregister, next_frame_supported;
};
#endif

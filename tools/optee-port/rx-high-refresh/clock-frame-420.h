/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef VS680_CLOCK_FRAME_420_H
#define VS680_CLOCK_FRAME_420_H
#include <stdint.h>
struct clock_frame_420_result {
    uint32_t timestamp, valid, invalid, different;
    uint32_t samples[3], errors[3], first_error[3][4];
};
int clock_frame_420_check(const uint8_t *data, unsigned transport_width,
    unsigned height, unsigned stride, unsigned cell, struct clock_frame_420_result *result);
#endif

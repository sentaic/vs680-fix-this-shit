// SPDX-License-Identifier: GPL-2.0-only
#include "video-pattern.h"
#include <string.h>

static int multiply_size(size_t a, size_t b, size_t *result)
{
    if (b != 0 && a > SIZE_MAX / b)
        return -1;
    *result = a * b;
    return 0;
}

int hdmi_dev_fill_pattern(void *mapped, size_t capacity, uint32_t width,
                          uint32_t height, size_t stride, uint32_t pattern,
                          uint32_t rgb)
{
    size_t row_bytes, frame_bytes;
    uint8_t *pixels = mapped;
    uint32_t x, y;

    /* Finish every validation before touching the destination. */
    if (mapped == NULL || !width || width > 2560u || !height || height > 1440u)
        return -1;
    if (pattern != HDMI_DEV_PATTERN_COORDINATE && pattern != HDMI_DEV_PATTERN_SOLID)
        return -1;
    if ((pattern == HDMI_DEV_PATTERN_COORDINATE && rgb != 0) ||
        (rgb & UINT32_C(0xff000000)) != 0)
        return -1;
    if (multiply_size((size_t)width, 3u, &row_bytes) != 0 || stride != row_bytes)
        return -1;
    if (multiply_size(stride, (size_t)height, &frame_bytes) != 0 ||
        frame_bytes > capacity)
        return -1;
    /* 检查映射地址与长度之和是否溢出。 */
    if ((uintptr_t)mapped > UINTPTR_MAX - (frame_bytes - 1u))
        return -1;

    /* 栈上逐行生成，再整行复制，减少非缓存帧池上的字节写入。 */
    uint8_t row[2560u * 3u];
    for (y = 0; y < height; ++y) {
        uint8_t *pixel = row;

        for (x = 0; x < width; ++x) {
            if (pattern == HDMI_DEV_PATTERN_COORDINATE) {
                pixel[0] = (uint8_t)(x & UINT32_C(255));
                pixel[1] = (uint8_t)(y & UINT32_C(255));
                pixel[2] = (uint8_t)((x >> 8) | ((y >> 8) << 4));
            } else {
                pixel[0] = (uint8_t)((rgb >> 16) & UINT32_C(255));
                pixel[1] = (uint8_t)((rgb >> 8) & UINT32_C(255));
                pixel[2] = (uint8_t)(rgb & UINT32_C(255));
            }
            pixel += 3;
        }
        memcpy(pixels + (size_t)y * stride, row, row_bytes);
    }
    return 0;
}

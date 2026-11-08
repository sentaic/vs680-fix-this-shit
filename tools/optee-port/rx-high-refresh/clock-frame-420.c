/* SPDX-License-Identifier: GPL-2.0-only */
#include "clock-frame-420.h"

static uint32_t checksum(uint32_t value)
{
    uint32_t crc = UINT32_MAX;
    for (unsigned byte = 0; byte < 4; byte++, value >>= 8) {
        crc ^= value & 255u;
        for (unsigned bit = 0; bit < 8; bit++)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

static unsigned sample(const uint8_t *data, unsigned stride, unsigned plane, unsigned x, unsigned y)
{
    /* 原始传输每组三字节为两个 Y 和一个色度分量，偶数行是 Cb，奇数行是 Cr。 */
    if (!plane) return data[y * stride + (x / 2u) * 3u + (x & 1u)];
    return data[(2u * y + plane - 1u) * stride + x * 3u + 2u];
}

static unsigned pattern_bit(uint64_t word, unsigned x, unsigned y, unsigned cell)
{
    x = (x / cell) % 12u; y = (y / cell) % 12u;
    if (!x || !y || x == 11u || y == 11u) return 1;
    if (x == 1u || y == 1u || x == 10u || y == 10u) return 0;
    return (unsigned)(word >> ((y - 2u) * 8u + x - 2u)) & 1u;
}

int clock_frame_420_check(const uint8_t *data, unsigned transport_width,
    unsigned height, unsigned stride, unsigned cell, struct clock_frame_420_result *out)
{
    if (!data || !out || !transport_width || !height || (height & 1u) ||
        cell < 2u || (cell & 1u) || stride < transport_width * 3u) return 0;
    struct clock_frame_420_result result = {0};
    for (unsigned plane = 0; plane < 3; plane++) {
        unsigned scale = plane ? 2u : 1u;
        unsigned width = transport_width * 2u / scale, rows = height / scale;
        unsigned step = cell / scale, tile = step * 12u;
        for (unsigned top = 0; top + tile <= rows; top += tile) {
            for (unsigned left = 0; left + tile <= width; left += tile) {
                uint64_t word = 0;
                unsigned border_ok = 1;
                for (unsigned y = 0; y < 12; y++) for (unsigned x = 0; x < 12; x++) {
                    unsigned high = sample(data, stride, plane,
                        left + x * step + step / 2u, top + y * step + step / 2u) >= 128u;
                    if (!x || !y || x == 11u || y == 11u) border_ok &= high;
                    else if (x == 1u || y == 1u || x == 10u || y == 10u) border_ok &= !high;
                    else if (high) word |= UINT64_C(1) << ((y - 2u) * 8u + x - 2u);
                }
                word ^= UINT64_C(0xa6d39b704c28e15f);
                uint32_t stamp = (uint32_t)word;
                if (!border_ok || (uint32_t)(word >> 32) != checksum(stamp)) result.invalid++;
                else {
                    if (!result.valid) result.timestamp = stamp;
                    else result.different += stamp != result.timestamp;
                    result.valid++;
                }
            }
        }
    }
    uint64_t word = (((uint64_t)checksum(result.timestamp) << 32) | result.timestamp) ^
                    UINT64_C(0xa6d39b704c28e15f);
    /* 测试源为 BT.709 限幅输出；色度水平滤波为 [1,2,1]/4，垂直按相邻两行取样。
     * 正负一个色阶的容差覆盖显卡的量化和抖动。 */
    static const unsigned levels[3][2] = {{89, 163}, {88, 168}, {90, 166}};
    for (unsigned plane = 0; plane < 3; plane++) {
        unsigned scale = plane ? 2u : 1u;
        unsigned width = transport_width * 2u / scale, rows = height / scale;
        for (unsigned y = 0; y < rows; y++) for (unsigned x = 0; x < width; x++) {
            unsigned value = sample(data, stride, plane, x, y);
            unsigned expected = levels[plane][pattern_bit(word, x * scale, y * scale, cell)];
            if (plane) {
                unsigned left = x ? x * 2u - 1u : 0u;
                unsigned right = x * 2u + 1u;
                expected = (levels[plane][pattern_bit(word, left, y * 2u, cell)] +
                    2u * expected + levels[plane][pattern_bit(word, right, y * 2u, cell)] + 2u) / 4u;
            }
            result.samples[plane]++;
            if (value + 1u < expected || value > expected + 1u) {
                if (!result.errors[plane]) {
                    result.first_error[plane][0] = x; result.first_error[plane][1] = y;
                    result.first_error[plane][2] = value; result.first_error[plane][3] = expected;
                }
                result.errors[plane]++;
            }
        }
    }
    *out = result;
    return result.valid && !result.invalid && !result.different &&
        !result.errors[0] && !result.errors[1] && !result.errors[2];
}

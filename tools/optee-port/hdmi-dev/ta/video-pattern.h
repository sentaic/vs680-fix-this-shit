// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_VIDEO_PATTERN_H
#define VS680_HDMI_DEV_VIDEO_PATTERN_H

#include <stddef.h>
#include <stdint.h>
#include "../hdmi-dev.h"

#define HDMI_DEV_RGB24_STRIDE ((size_t)HDMI_DEV_WIDTH * 3u)
#define HDMI_DEV_RGB24_SIZE (HDMI_DEV_RGB24_STRIDE * HDMI_DEV_HEIGHT)

/* 向已映射的帧填入紧密排列的 RGB24 像素；最大尺寸 2560×1440。坐标图要求 rgb=0，纯色使用 0xRRGGBB。参数错误返回 -1，成功返回 0；调用方负责缓存清理和提交。 */
int hdmi_dev_fill_pattern(void *mapped, size_t capacity, uint32_t width,
                          uint32_t height, size_t stride, uint32_t pattern,
                          uint32_t rgb);

#endif

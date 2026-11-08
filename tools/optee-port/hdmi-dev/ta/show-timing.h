/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_SHOW_TIMING_H
#define HDMI_SHOW_TIMING_H
#include <stdint.h>
#include <stddef.h>
#define HDMI_SHOW_TIMING_READ 0x100u
#define HDMI_SHOW_TIMING_ABI 2u
#define HDMI_SHOW_TIMING_CAPACITY 256u
enum hdmi_show_point {
    HDMI_SHOW_ENTER, HDMI_SHOW_BORROWED, HDMI_SHOW_FILLED,
    HDMI_SHOW_CLEAN_BEGIN, HDMI_SHOW_CACHE_BEGIN, HDMI_SHOW_CACHE_END,
    HDMI_SHOW_CLEAN_END, HDMI_SHOW_PRESENTED,
    HDMI_SHOW_STATUS_READ, HDMI_SHOW_RETURN, HDMI_SHOW_POINTS
};
struct hdmi_show_sample {
    uint64_t ms[HDMI_SHOW_POINTS];
    uint32_t points, pattern, rgb, result;
};
struct hdmi_show_trace {
    uint32_t abi, count, dropped, reserved;
    struct hdmi_show_sample samples[HDMI_SHOW_TIMING_CAPACITY];
};
#ifdef HDMI_DEV_SHOW_TIMING_ENABLED
void hdmi_show_timing_begin(uint32_t pattern,uint32_t rgb);
void hdmi_show_timing_mark(enum hdmi_show_point point);
uint32_t hdmi_show_timing_end(uint32_t result);
uint32_t hdmi_show_timing_read(void *buffer,size_t *bytes);
#else
static inline void hdmi_show_timing_begin(uint32_t pattern,uint32_t rgb)
{ (void)pattern;(void)rgb; }
static inline void hdmi_show_timing_mark(enum hdmi_show_point point) { (void)point; }
static inline uint32_t hdmi_show_timing_end(uint32_t result) { return result; }
#endif
#endif

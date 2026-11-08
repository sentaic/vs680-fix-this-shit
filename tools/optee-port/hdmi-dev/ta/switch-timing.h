/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_SWITCH_TIMING_H
#define HDMI_SWITCH_TIMING_H
#include <stddef.h>
#include <stdint.h>
#define HDMI_SWITCH_TIMING_READ 0x101u
#define HDMI_SWITCH_TIMING_ABI 1u
#define HDMI_SWITCH_TIMING_CAPACITY 256u
enum hdmi_switch_point {
 HDMI_SWITCH_QUEUED, HDMI_SWITCH_APPLIED, HDMI_SWITCH_SOURCE_MATCHED,
 HDMI_SWITCH_OLD_READ_DONE, HDMI_SWITCH_RELEASED, HDMI_SWITCH_POINTS
};
struct hdmi_switch_sample {
 uint64_t ms[HDMI_SWITCH_POINTS];
 uint32_t points, old_slot, new_slot;
};
struct hdmi_switch_trace {
 uint32_t abi, count, dropped, reserved;
 struct hdmi_switch_sample samples[HDMI_SWITCH_TIMING_CAPACITY];
};
#ifdef HDMI_DEV_SHOW_TIMING_ENABLED
void hdmi_switch_timing_begin(unsigned old_slot,unsigned new_slot);
void hdmi_switch_timing_mark(enum hdmi_switch_point point);
uint32_t hdmi_switch_timing_read(void *buffer,size_t *bytes);
#else
static inline void hdmi_switch_timing_begin(unsigned old_slot,unsigned new_slot)
{ (void)old_slot;(void)new_slot; }
static inline void hdmi_switch_timing_mark(enum hdmi_switch_point point) { (void)point; }
#endif
#endif

/* SPDX-License-Identifier: GPL-2.0 */
#include "switch-timing.h"
#include <tee_internal_api.h>
#include <string.h>
static struct hdmi_switch_trace trace={.abi=HDMI_SWITCH_TIMING_ABI};
static struct hdmi_switch_sample *current;
void hdmi_switch_timing_mark(enum hdmi_switch_point point)
{
 if (!current || point>=HDMI_SWITCH_POINTS || (current->points&(1u<<point))) return;
 TEE_Time time;TEE_GetSystemTime(&time);
 current->ms[point]=(uint64_t)time.seconds*1000u+time.millis;
 current->points|=1u<<point;
}
void hdmi_switch_timing_begin(unsigned old_slot,unsigned new_slot)
{
 if (trace.count==HDMI_SWITCH_TIMING_CAPACITY) { ++trace.dropped;current=NULL;return; }
 current=&trace.samples[trace.count++];
 *current=(struct hdmi_switch_sample){.old_slot=old_slot,.new_slot=new_slot};
 hdmi_switch_timing_mark(HDMI_SWITCH_QUEUED);
}
uint32_t hdmi_switch_timing_read(void *buffer,size_t *bytes)
{
 if (!buffer || !bytes) return TEE_ERROR_BAD_PARAMETERS;
 if (*bytes<sizeof(trace)) { *bytes=sizeof(trace);return TEE_ERROR_SHORT_BUFFER; }
 memcpy(buffer,&trace,sizeof(trace));*bytes=sizeof(trace);return TEE_SUCCESS;
}

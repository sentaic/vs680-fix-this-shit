/* SPDX-License-Identifier: GPL-2.0 */
#include "show-timing.h"
#include <tee_internal_api.h>
#include <string.h>
static struct hdmi_show_trace trace={.abi=HDMI_SHOW_TIMING_ABI};
static struct hdmi_show_sample *current;
void hdmi_show_timing_mark(enum hdmi_show_point point)
{
    if (!current || point>=HDMI_SHOW_POINTS) return;
    TEE_Time time;
    TEE_GetSystemTime(&time);
    current->ms[point]=(uint64_t)time.seconds*1000u+time.millis;
    current->points|=1u<<point;
}
void hdmi_show_timing_begin(uint32_t pattern,uint32_t rgb)
{
    if (trace.count==HDMI_SHOW_TIMING_CAPACITY) {
        ++trace.dropped;current=NULL;return;
    }
    current=&trace.samples[trace.count++];
    *current=(struct hdmi_show_sample){.pattern=pattern,.rgb=rgb};
    hdmi_show_timing_mark(HDMI_SHOW_ENTER);
}
uint32_t hdmi_show_timing_end(uint32_t result)
{
    if (current) {
        hdmi_show_timing_mark(HDMI_SHOW_RETURN);
        current->result=result;current=NULL;
    }
    return result;
}
uint32_t hdmi_show_timing_read(void *buffer,size_t *bytes)
{
    if (!buffer || !bytes) return TEE_ERROR_BAD_PARAMETERS;
    if (*bytes<sizeof(trace)) { *bytes=sizeof(trace);return TEE_ERROR_SHORT_BUFFER; }
    memcpy(buffer,&trace,sizeof(trace));*bytes=sizeof(trace);
    return TEE_SUCCESS;
}

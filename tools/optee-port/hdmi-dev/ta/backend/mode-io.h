/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_MODE_IO_H
#define HDMI_DEV_MODE_IO_H
#include <tee_internal_api.h>
struct hdmi_mode_io {
    void *opaque;
    TEE_Result (*read32)(void *opaque, uint32_t physical, uint32_t *value);
    TEE_Result (*write32)(void *opaque, uint32_t physical, uint32_t value);
    TEE_Result (*wait_ms)(void *opaque, uint32_t milliseconds);
};
#endif

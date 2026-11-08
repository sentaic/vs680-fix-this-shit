/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef HDMI_DEV_PIXEL_CLOCK_H
#define HDMI_DEV_PIXEL_CLOCK_H
#include "mode-io.h"
TEE_Result hdmi_pixel_clock_configure(const struct hdmi_mode_io *io, uint32_t rate);
#endif

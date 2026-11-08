/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_MODE_ACCESS_H
#define HDMI_DEV_MODE_ACCESS_H
#include "mode-io.h"
struct hdmi_mode_access {
    volatile uint32_t *avio;
    volatile uint32_t *chip;
};
TEE_Result hdmi_mode_access_bind(struct hdmi_mode_access *access,
                                 struct hdmi_mode_io *io);
#endif

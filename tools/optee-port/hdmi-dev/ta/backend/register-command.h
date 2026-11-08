/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_REGISTER_COMMAND_H
#define HDMI_DEV_REGISTER_COMMAND_H
#include <stdint.h>
struct hdmi_register_write { uint32_t value, address; };
_Static_assert(sizeof(struct hdmi_register_write)==8,"BCM register command size");
#endif

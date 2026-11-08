/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_MIXER_H
#define HDMI_DEV_MIXER_H
#include "mode-io.h"
#include "register-command.h"
#define HDMI_MIXER_WRITES 4u
struct hdmi_mixer { uint32_t background_rgb, alpha_q12; };
/* 构建VBI写入序列，保留其他图层的透明度与极性设置。 */
TEE_Result hdmi_mixer_program(const struct hdmi_mixer *mixer,
    const struct hdmi_mode_io *io, struct hdmi_register_write program[HDMI_MIXER_WRITES]);
#endif

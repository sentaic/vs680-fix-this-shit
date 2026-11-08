# SPDX-License-Identifier: GPL-2.0-only
global-incdirs-y += . ..
srcs-y += entry.c video-pattern.c
subdirs-y += backend
cflags-y += -Wall -Wextra -Werror

ifeq ($(CFG_HDMI_SHOW_TIMING),y)
global-cppflags-y += -DHDMI_DEV_SHOW_TIMING_ENABLED
srcs-y += show-timing.c switch-timing.c
endif

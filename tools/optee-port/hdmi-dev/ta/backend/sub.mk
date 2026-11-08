# SPDX-License-Identifier: GPL-2.0
# OP-TEE 为子目录内的路径自动加 backend/ 前缀。
hdmi_sdk_display := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../../../../sources/astra-current/tee-optee_dev/ta/display)

global-incdirs-y += .
global-incdirs_ext-y += $(hdmi_sdk_display)/common/include/dolphin
global-incdirs_ext-y += $(hdmi_sdk_display)/common/include
global-incdirs_ext-y += $(hdmi_sdk_display)/hal/common/include
global-incdirs_ext-y += $(hdmi_sdk_display)/ta/vpp/lib/dhub/include
global-incdirs_ext-y += $(hdmi_sdk_display)/ta/vpp/lib/dhub/configs/dolphin
global-incdirs_ext-y += $(hdmi_sdk_display)/ta/vpp/lib/osal/include
srcs-y += avio-hardware.c dma-pool.c payload-cache.c gfx-dma.c mode-access.c pixel-clock.c hardware-backend.c mode-720p.c frame-start.c mixer.c overlay.c overlay-mode.c
cflags-y += -DCONFIG_OPTEE

srcs-y += display-mode.c vpp-timing.c pixel-clock-plan.c tx-mode-adapter.c scdc-link.c tx-scdc-adapter.c

srcs-y += tx-ddc-runtime.c

srcs-y += vs680_phy_range.c
srcs-y += ddc-diagnostic.c
srcs-y += ddc-current.c vs680_ddc_diag.c

srcs-y += ddc-owner-prepare.c

srcs-y += ddc-recovery.c

srcs-y += tx-stop.c

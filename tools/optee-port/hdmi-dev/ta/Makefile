# SPDX-License-Identifier: GPL-2.0-only
BINARY := 4156150c-fa24-4acc-9c5a-7f788785473f

ifeq ($(wildcard backend/sub.mk),)
$(error Missing backend/sub.mk)
endif
ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk),)
$(error Set TA_DEV_KIT_DIR to the exported OP-TEE TA development kit)
endif

ifneq ($(TA_CROSS_COMPILE),)
CROSS_COMPILE ?= $(TA_CROSS_COMPILE)
endif
export CROSS_COMPILE
include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

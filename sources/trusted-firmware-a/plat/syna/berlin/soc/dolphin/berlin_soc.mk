#
# Copyright (c) 2015-2019, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2024, Synaptics Corporation. All rights reserved.
#

BL31_SOURCES		+=	lib/cpus/aarch64/cortex_a73.S		\
				${SOC_DIR}/suspend.S			\
				${SOC_DIR}/plat_setup.c			\
				${SOC_DIR}/plat_pm.c

BL31_CPPFLAGS +=-DSUSPEND_BIN='"${BUILD_PLAT}/suspend.bin"'

${BUILD_PLAT}/bl31/suspend.o:${BUILD_PLAT}/suspend.bin

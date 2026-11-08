#
# Copyright (c) 2015-2019, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2024, Synaptics Corporation. All rights reserved.
#

BL31_SOURCES		+=	lib/cpus/aarch64/cortex_a55.S		\
				${COMMON_DIR}/drivers/watchdog/wdt.c			\
				${SOC_DIR}/suspend.S			\
				${SOC_DIR}/plat_sip_call.c			\
				${SOC_DIR}/plat_setup.c			\
				${SOC_DIR}/plat_pm.c
ARM_PLAT_MT		:=	1
$(eval $(call add_define,ARM_PLAT_MT))

HW_ASSISTED_COHERENCY := 1
USE_COHERENT_MEM := 0
ERRATA_A55_1530923 := 1
COLD_BOOT_SINGLE_CPU         := 0
RESET_TO_BL31 := 1
#BERLIN_LP_MMU_ENABLE := 1

#ENABLE_PVD := 1

SYNA_BCM = 0
SYNA_SM = 0
SYNA_APPCRYPTO :=1
SYNA_IPC:=1
SYNAPROT_MAILBOX := 1

BL31_CPPFLAGS +=-DSUSPEND_BIN='"${BUILD_PLAT}/suspend.bin"'

${BUILD_PLAT}/bl31/suspend.o:${BUILD_PLAT}/suspend.bin

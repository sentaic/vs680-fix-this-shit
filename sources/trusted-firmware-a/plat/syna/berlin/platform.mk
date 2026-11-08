#
# Copyright (c) 2013-2022, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ERROR_DEPRECATED	:=	1
ENABLE_PLAT_COMPAT		:=	0
PROGRAMMABLE_RESET_ADDRESS	:=	1
COLD_BOOT_SINGLE_CPU		:=	1
MULTI_CONSOLE_API		:=	1
ENABLE_ASSERTIONS    := 0
NS_TIMER_SWITCH	:= 0

WORKAROUND_CVE_2017_5715 := 1
SPMD_SPM_AT_SEL2 :=0
TRNG_SUPPORT         :=      0

CRASH_REPORTING		:=	1
$(eval $(call add_define,CRASH_REPORTING))

ASM_ASSERTION		:=	1
$(eval $(call add_define,ASM_ASSERTION))

# Modify BUILD_PLAT to point to SoC specific build directory
BUILD_PLAT		:=	${BUILD_BASE}/${PLAT}/${TARGET_SOC}/${BUILD_TYPE}

# Disable the PSCI platform compatibility layer
ENABLE_PLAT_COMPAT	:=	0

ENABLE_STACK_PROTECTOR := strong

COMMON_DIR		:=	plat/syna/berlin/common
SOC_DIR			:=	plat/syna/berlin/soc/${TARGET_SOC}
SUSPEND_DIR		:=	plat/syna/berlin/suspend

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	:=	${XLAT_TABLES_LIB_SRCS}

include ${SOC_DIR}/berlin_soc.mk
include $(COMMON_DIR)/berlin_common.mk
include $(SUSPEND_DIR)/berlin_suspend.mk

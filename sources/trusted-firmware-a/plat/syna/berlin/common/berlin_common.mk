#
# Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2024, Synaptics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

PLAT_INCLUDES		:=	-Iplat/syna/berlin/include			\
				-Iplat/syna/berlin/include/drivers		\
				-Iplat/syna/berlin/include/soc/${TARGET_SOC}

# Include GICv2 driver files
include drivers/arm/gic/v2/gicv2.mk
BERLIN_GICv2_SOURCES	:=	\
			${GICV2_SOURCES}				\
			plat/common/plat_gicv2.c		\
			plat/arm/common/arm_gicv2.c

ifeq (${SYNA_GENX},1)
$(eval $(call add_define,CONFIG_GENX))
endif

ifeq (${SYNA_MP},1)
$(eval $(call add_define,SYNA_MP))
endif

ifneq (${ENABLE_STACK_PROTECTOR},0)
PLAT_BL_COMMON_SOURCES	+=	plat/syna/berlin/common/berlin_stack_protector.c
endif

ifeq (${TRNG_SUPPORT}, 1)
PLAT_BL_COMMON_SOURCES	+=	plat/syna/berlin/common/berlin_trng.c
endif
SYNA_BCM ?= 1
SYNA_SM ?= 1

BL31_SOURCES		+=	${BERLIN_GICv2_SOURCES}		\
				drivers/ti/uart/aarch64/16550_console.S		\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				plat/common/plat_psci_common.c			\
				${COMMON_DIR}/aarch64/berlin_helpers.S		\
				${COMMON_DIR}/aarch64/berlin_setup.S		\
				${COMMON_DIR}/drivers/tos/tz_boot_param.c	\
				${COMMON_DIR}/drivers/tos/tz.c			\
				${COMMON_DIR}/drivers/secure_register/secure_register.c	\
				${COMMON_DIR}/berlin_bl31_setup.c		\
				${COMMON_DIR}/berlin_pm.c			\
				${COMMON_DIR}/gicv2_ext.c			\
				${COMMON_DIR}/berlin_topology.c		\
				${COMMON_DIR}/berlin_sip_svc.c
ifeq (${SYNA_BCM}, 1)
BL31_SOURCES		+=	\
				${COMMON_DIR}/drivers/bcm/bcm.c			\
				${COMMON_DIR}/drivers/bcm/bcm_hal.c
$(eval $(call add_define,CFG_SYNA_BCM))
endif

ifeq (${SYNA_SM}, 1)
BL31_SOURCES		+= ${COMMON_DIR}/drivers/sm/sm.c
$(eval $(call add_define,CFG_SYNA_SM))
endif

ifeq (${SYNA_APPCRYPTO}, 1)
BL31_SOURCES		+=	\
				${COMMON_DIR}/drivers/appcrypto/appcrypto_lock.c \
				${COMMON_DIR}/drivers/appcrypto/appcrypto_rng.c
$(eval $(call add_define,CFG_SYNA_APPCRYPTO))
endif

ifeq (${SYNA_IPC}, 1)
BL31_SOURCES		+=	\
				${COMMON_DIR}/drivers/mailbox/syna_ipc.c \
				${COMMON_DIR}/drivers/mailbox/sm_ipc.c
PLAT_INCLUDES		+=	-I${COMMON_DIR}/drivers/mailbox
$(eval $(call add_define,CFG_SYNA_IPC))
endif

ifeq (${SYNAPROT_MAILBOX}, 1)
BL31_SOURCES		+=	\
				${COMMON_DIR}/drivers/mailbox/synaprot_mailbox.c \
$(eval $(call add_define,CFG_SYNAPROT_MAILBOX))
endif

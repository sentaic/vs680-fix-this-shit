#
# Copyright (c) 2013-2022, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include lib/libc/libc.mk
SUSPEND_COMMON_SOURCES	=				\
				common/tf_log.c				\
				common/${ARCH}/debug.S			\
				lib/${ARCH}/cache_helpers.S		\
				lib/${ARCH}/misc_helpers.S		\
				drivers/console/multi_console.c		\
				plat/common/plat_log_common.c		\
                ${LIBC_SRCS}
#				${COMPILER_RT_SRCS}

ifeq (${BERLIN_LP_MMU_ENABLE}, 1)
include lib/xlat_tables_v2/xlat_tables.mk
SUSPEND_COMMON_SOURCES	+=				\
				${XLAT_TABLES_LIB_SRCS}
CFLAGS		+=	-DCONFIG_BERLIN_LP_MMU_ENABLE
endif

ifeq (${DEBUG}, 1)
SUSPEND_COMMON_SOURCES +=				\
				common/backtrace/backtrace.c
endif

ifneq (${ENABLE_STACK_PROTECTOR},0)
#include lib/stack_protector/stack_protector.mk
SUSPEND_COMMON_SOURCES +=	plat/syna/berlin/common/berlin_stack_protector.c

  STACK_PROTECTOR_ENABLED := 1
  SUSPEND_COMMON_SOURCES +=	lib/stack_protector/stack_protector.c	\
				lib/stack_protector/${ARCH}/asm_stack_protector.S

  ifeq (${ENABLE_STACK_PROTECTOR},default)
    CFLAGS		+=	-fstack-protector
  else
    CFLAGS		+=	-fstack-protector-${ENABLE_STACK_PROTECTOR}
  endif
endif

MODULE_SOURCES      := $(SUSPEND_COMMON_SOURCES)                         \
                       drivers/ti/uart/aarch64/16550_console.S            \
                       plat/common/aarch64/platform_up_stack.S            \
                       ${COMMON_DIR}/aarch64/berlin_helpers.S             \
                       ${COMMON_DIR}/drivers/tos/tz_boot_param.c          \
                       ${COMMON_DIR}/drivers/tos/tz.c                     \
                       ${SUSPEND_DIR}/mem4.c                              \
                       ${SUSPEND_DIR}/sha256_ce.S                         \
                       ${SUSPEND_DIR}/sha256.c                            \
                       ${SUSPEND_DIR}/suspend_entrypoint.S                \
                       ${SOC_DIR}/suspend.c                               \
                       ${SOC_DIR}/clock.c                                 \
                       ${SOC_DIR}/mc.c

ifeq (${SYNA_SM}, 1)
MODULE_SOURCES      += ${COMMON_DIR}/drivers/sm/sm.c
endif

ifeq (${SYNA_BCM}, 1)
MODULE_SOURCES      += ${COMMON_DIR}/drivers/bcm/bcm_hal.c \
                       ${COMMON_DIR}/drivers/bcm/bcm.c
endif

ifeq (${SYNA_APPCRYPTO}, 1)
MODULE_SOURCES+=	\
				${COMMON_DIR}/drivers/appcrypto/appcrypto_lock.c \
				${COMMON_DIR}/drivers/appcrypto/appcrypto_rng.c
$(eval $(call add_define,CFG_SYNA_APPCRYPTO))
endif

ifeq (${SYNA_IPC}, 1)
MODULE_SOURCES+=	\
				${COMMON_DIR}/drivers/mailbox/syna_ipc.c \
				${COMMON_DIR}/drivers/mailbox/sm_ipc.c
PLAT_INCLUDES		+=	-I${COMMON_DIR}/drivers/mailbox
$(eval $(call add_define,CFG_SYNA_IPC))
endif

ifeq (${ENABLE_PVD}, 1)
MODULE_SOURCES      += ${SOC_DIR}/i2c.c			\
                       ${SOC_DIR}/i2c_pvd_tps62868.c
$(eval $(call add_define,CFG_ENABLE_PVD))
ifeq (${ENABLE_POWER_CONSUMPTION_PRINT}, 1)
MODULE_SOURCES      += ${SOC_DIR}/powermeasure.c
$(eval $(call add_define,CFG_ENABLE_POWER_CONSUMPTION_PRINT))
endif
endif

MODULE_LINKERSCRIPT := ${SUSPEND_DIR}/suspend.ld.S

MODULE_NAME         := suspend
MODULE_OUTPUT_DIR   := ${BUILD_PLAT}/$(MODULE_NAME)
MODULE_C_SOURCES    := $(filter %.c,$(MODULE_SOURCES))
MODULE_S_SOURCES    := $(filter %.S,$(MODULE_SOURCES))

COMPILEROBJECTS     := $(addprefix $(MODULE_OUTPUT_DIR)/,\
                           $(notdir $(patsubst %.c,%.o,$(MODULE_C_SOURCES))\
                                    $(patsubst %.S,%.o,$(MODULE_S_SOURCES))))
LINKERSCRIPT        := ${MODULE_OUTPUT_DIR}/$(MODULE_NAME).ld
LINKERMAP           := ${MODULE_OUTPUT_DIR}/$(MODULE_NAME).map
LINKERELF           := ${MODULE_OUTPUT_DIR}/$(MODULE_NAME).elf
BINUTILSDUMP        := ${MODULE_OUTPUT_DIR}/$(MODULE_NAME).dump
BINUTILSCOPY        := ${BUILD_PLAT}/$(MODULE_NAME).bin

CFLAGS += -I${SUSPEND_DIR} -I${SOC_DIR} -DBERLIN_SUSPEND_SRAM
LDFLAGS += --no-warn-rwx-segments
define CRULES
$(1): $(3)
	@echo "  CC      $$<"
	$$(Q)$$(CC) $$(TF_CFLAGS) $$(CFLAGS) -c $$< -o $$@

$(2): $(3) | $(MODULE_OUTPUT_DIR)
	@echo "  DEPS    $$@"
	$$(Q)$$(CC) $$(TF_CFLAGS) $$(CFLAGS) -M -MT $(1) -MF $$@ $$<

-include $(2)
endef

define SRULES
$(1): $(3)
	@echo "  AS      $$<"
	$$(Q)$$(AS) $$(ASFLAGS) -c $$< -o $$@

$(2): $(3) | $(MODULE_OUTPUT_DIR)
	@echo "  DEPS    $$@"
	$$(Q)$$(AS) $$(ASFLAGS) -M -MT $(1) -MF $$@ $$<

-include $(2)
endef

define LDRULES
$(1): $(3)
	@echo "  PP      $$<"
	$$(Q)$$(AS) $$(ASFLAGS) -P -E -D__LINKER__ -o $$@ $$<

$(2): $(3) | $(MODULE_OUTPUT_DIR)
	@echo "  DEPS    $$@"
	$$(Q)$$(AS) $$(ASFLAGS) -M -MT $(1) -MF $$@ $$<

-include $(2)
endef

$(MODULE_OUTPUT_DIR):
	$(Q)mkdir -p "$(MODULE_OUTPUT_DIR)"

$(foreach obj,$(MODULE_C_SOURCES),$(eval $(call CRULES,\
$(addprefix $(MODULE_OUTPUT_DIR)/,$(notdir $(patsubst %.c,%.o,$(obj)))),\
$(addprefix $(MODULE_OUTPUT_DIR)/,$(notdir $(patsubst %.c,%.d,$(obj)))),\
$(obj))))

$(foreach obj,$(MODULE_S_SOURCES),$(eval $(call SRULES,\
$(addprefix $(MODULE_OUTPUT_DIR)/,$(notdir $(patsubst %.S,%.o,$(obj)))),\
$(addprefix $(MODULE_OUTPUT_DIR)/,$(notdir $(patsubst %.S,%.d,$(obj)))),\
$(obj))))

$(eval $(call LDRULES,$(LINKERSCRIPT),$(LINKERSCRIPT).d,$(MODULE_LINKERSCRIPT)))

$(LINKERELF): $(COMPILEROBJECTS) $(LINKERSCRIPT) | $(MODULE_OUTPUT_DIR)
	@echo "  LD      $@"
	$(Q)$(LD) -o $@ $(LDFLAGS) -Map=$(LINKERMAP) --script $(LINKERSCRIPT) $(COMPILEROBJECTS)

$(BINUTILSDUMP): $(LINKERELF)
	@echo "  OD      $@"
	${Q}${OD} -dx $< > $@

$(BINUTILSCOPY): $(LINKERELF)
	@echo "  BIN     $@"
	$(Q)$(OC) -O binary $< $@
	@echo
	@echo "Built $@ successfully"
	@echo

.PHONY: $(MODULE_NAME)
$(MODULE_NAME): $(BINUTILSCOPY) $(BINUTILSDUMP)

all: $(MODULE_NAME)

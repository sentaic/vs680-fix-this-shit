PLATFORM_FLAVOR ?= platypus

include core/arch/arm/cpu/cortex-armv8-0.mk
include core/arch/arm/plat-synaptics/chip/$(PLATFORM_FLAVOR)/chip.mk
$(call force,CFG_GENERIC_BOOT,y)
$(call force,CFG_WITH_ARM_TRUSTED_FW,y)
$(call force,CFG_WITH_LPAE,y)
$(call force,CFG_ARM64_core,y)
$(call force,CFG_CACHE_API,y)
$(call force,CFG_8250_UART,y)
$(call force,CFG_GIC,y)
$(call force,CFG_CORE_ASLR,n)
$(call force,CFG_CORE_LARGE_PHYS_ADDR,y)
$(call force,CFG_CORE_PREALLOC_EL0_TBLS,y)
$(call force,CFG_CORE_RODATA_NOEXEC,y)
$(call force,CFG_SECURE_TIME_SOURCE_CNTPCT,y)

$(call force,CFG_GENX,y)

CFG_WITH_SOFTWARE_PRNG ?=n
CFG_TEE_CORE_NB_CORE ?= 4
CFG_SCALL_SYNA ?= y
CFG_SYNA_TA ?=y
CFG_BCM ?= y
CFG_RF ?= y
CFG_MASTER_PROTECT ?= y
CFG_MZONE ?= y
CFG_SECURE_TIME_SYNA ?= n
CFG_APB_TIMER ?= n

CFG_TEE_CORE_MALLOC_DEBUG ?=n

supported-ta-targets = ta_arm64
CFG_CRYPTO_WITH_CE ?= y

#CFG_NUM_THREADS ?= CFG_TEE_CORE_NB_CORE
CFG_NUM_THREADS ?= 32
CFG_WITH_STACK_CANARIES ?= y
CFG_TEE_CORE_EMBED_INTERNAL_TESTS ?= y
CFG_SECSTOR_TA_MGMT_PTA ?= n
CFG_REE_FS_TA ?= y
CFG_SECSTOR_TA ?= n

CFG_SYSTEM_PTA ?= y

CFG_CORE_RESERVED_SHM ?= n
CFG_SECURE_DATA_PATH ?= y
#CFG_DRAM_SIZE_GB ?= 2
CFG_TZDRAM_START ?= 0x00160000

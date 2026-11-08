global-incdirs-y += .
global-incdirs-y += ./inc
global-incdirs-y += ./tz
global-incdirs-y += ./driver
global-incdirs-y += ./common
global-incdirs-y += ./common/syna_fastcall
global-incdirs-y += ./chip/$(PLATFORM_FLAVOR)/include
global-incdirs-y += ../kernel/.

subdirs-y += driver
subdirs-y += syscall
subdirs-y += ./chip/$(PLATFORM_FLAVOR)

srcs-y += main.c
srcs-y += common/syna_entrypoint.S
srcs-y += common/syna_setup.c
srcs-y += common/syna_fastcall_entry.c
srcs-y += common/tz_umem_mgr.c
srcs-y += common/tz_granule_table.c
srcs-y += tz/tz_boot_param.c
srcs-y += tz/tz_boot_param_value_$(PLATFORM_FLAVOR).c
srcs-y += tz/mr_setup.c
srcs-y += common/syna_fastcall/fc_bootstage.c
srcs-y += common/syna_fastcall/fc_memoryregion.c
srcs-y += common/syna_fastcall/fc_verifyimage.c
srcs-$(CFG_SYNA_TA) += common/tee_genx_ta_verify.c
srcs-$(CFG_TA_PERM_CHECK) += common/tee_ta_perm_ctrl.c
srcs-$(CFG_SECURE_TIME_SYNA) += common/tee_time_syna.c

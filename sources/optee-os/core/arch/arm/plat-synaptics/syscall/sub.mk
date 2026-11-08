global-incdirs-y += .
srcs-$(CFG_SCALL_SYNA) += scall_syna.c
srcs-$(CFG_SCALL_SYNA) += svc_syna.c
srcs-$(CFG_SCALL_SYNA) += syscall_umem.c
srcs-$(CFG_SCALL_SYNA) += syscall_globaldata.c
srcs-$(CFG_SCALL_SYNA) += syscall_mutex.c
srcs-$(CFG_SCALL_SYNA) += syscall_semaphore.c
srcs-$(CFG_SCALL_SYNA) += syscall_securekey.c
srcs-$(CFG_SCALL_SYNA) += syscall_sa.c
srcs-$(CFG_SCALL_SYNA) += syna_scall_a64.S


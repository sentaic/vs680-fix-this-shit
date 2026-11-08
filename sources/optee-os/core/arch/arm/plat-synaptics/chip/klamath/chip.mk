$(call force,CFG_TEE_CORE_NB_CORE,2)
$(call force,CFG_SECURE_TIME_SOURCE_CNTPCT,y)
$(call force,CFG_SYNA_TA,n)
$(call force,CFG_BCM,n)
$(call force,CFG_APPCRYPTO,y)
$(call force,CFG_SECURE_TIME_SYNA,n)
$(call force,CFG_APB_TIMER,n)
$(call force,CFG_SZC,y)
$(call force,CFG_SZC_ZONE,y)
$(call force,CFG_SYNA_TRNG,y)
$(call force,CFG_SYNAPROT_MAILBOX,y)
$(call force,CFG_IMAGE_NO_ENCRYPTION,y)

CFG_TZDRAM_SIZE =  0x004A0000
CFG_CORE_HEAP_SIZE=0x040000

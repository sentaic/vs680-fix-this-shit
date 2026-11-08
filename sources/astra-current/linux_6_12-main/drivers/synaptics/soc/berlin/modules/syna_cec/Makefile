# SPDX-License-Identifier: GPL-2.0-only
HEADER_PATH +=  \
	-I$(srctree)/drivers/synaptics/include
ccflags-y += $(HEADER_PATH)
ccflags-y += -Werror
obj-$(CONFIG_SYNA_PLATFORM_CEC)	+= syna-cec.o
syna-cec-y  = syna_cec.o syna_cec_prv.o

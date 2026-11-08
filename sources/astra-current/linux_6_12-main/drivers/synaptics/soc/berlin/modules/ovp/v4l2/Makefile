###############################################################################
#
# OVP V4L2 Module
#
###############################################################################
BERLIN_MOD_PATH := $(srctree)/drivers/synaptics/soc/berlin/modules

###############################################################################
# Include paths
###############################################################################
MSGQ_INC_PATH := -I$(BERLIN_MOD_PATH)/msgQ

AVIO_INC_PATH := -I$(BERLIN_MOD_PATH)/avio/common \
		-I$(BERLIN_MOD_PATH)/avio/dhub

VPP_INC_PATH  :=  -I$(BERLIN_MOD_PATH)/vpp_mem/include \
		-I$(srctree)/drivers/synaptics/include

TZ_INC_PATH += -I$(BERLIN_MOD_PATH)/tzd/tee/client_api \
		-I$(BERLIN_MOD_PATH)/tzd/tee/common \
		-I$(BERLIN_MOD_PATH)/tzd/include

V4L2_INC_PATH := -I$(BERLIN_MOD_PATH)/ovp/v4l2 \
		-I$(BERLIN_MOD_PATH)/ovp \
		-I$(BERLIN_MOD_PATH)/ovp/ca

EXTERN_INC_PATH := $(MSGQ_INC_PATH) $(AVIO_INC_PATH) $(VPP_INC_PATH) $(TZ_INC_PATH) $(V4L2_INC_PATH)

###############################################################################
# Module objects
###############################################################################
ovp-v4l2-objs := ovp_v4l2.o

###############################################################################
# Compiler flags
###############################################################################
HEADER_PATH += $(EXTERN_INC_PATH)
ccflags-y += $(HEADER_PATH)
ccflags-y += -Werror

###############################################################################
# Module dependencies
###############################################################################
KBUILD_EXTRA_SYMBOLS += $(INSTALL_MOD_PATH)/lib/modules/$(KERNELRELEASE)/kernel/soc/berlin/modules/ovp/core/Module.symvers

###############################################################################
# Module registration
###############################################################################
obj-$(CONFIG_BERLIN_OVP_V4L2) += ovp-v4l2.o

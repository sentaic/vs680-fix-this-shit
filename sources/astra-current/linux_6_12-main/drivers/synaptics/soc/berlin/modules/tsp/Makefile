ifneq ($(CONFIG_OPTEE),y)
INC_PATH = -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/tee/client_api \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/tee/common \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/include
endif

HEADER_PATH +=  \
    -I$(srctree)/drivers/synaptics/soc/berlin/modules/msgQ \
    -I$(srctree)/drivers/synaptics/include
ccflags-y += $(INC_PATH)
ccflags-y += $(HEADER_PATH)
ccflags-y += -Werror
obj-$(CONFIG_BERLIN_TSP) := tsp.o
tsp-objs += tsp_core.o

ifeq ($(CONFIG_OPTEE),y)
tsp-objs += tsp_ca_optee.o
else
tsp-objs += tsp_ca_tzd.o
endif

INC_PATH = -I$(srctree)/drivers/synaptics/include

ifneq ($(CONFIG_OPTEE),y)
INC_PATH += -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/tee/client_api \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/tee/common \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/include \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/tzd/tz/common
endif

INC_PATH +=-I$(srctree)/drivers/synaptics/soc/berlin/modules/tsp \
           -I$(srctree)/drivers/synaptics/soc/berlin/modules/msgQ

ccflags-y += -Werror
ccflags-y += $(INC_PATH)

obj-$(CONFIG_BERLIN_M2M) := m2m.o
m2m-objs += m2m_drv.o

ifeq ($(CONFIG_OPTEE),y)
m2m-objs += m2m_wrapper_optee.o
else
m2m-objs += m2m_wrapper_tzd.o
endif

obj-$(CONFIG_SYNAPTICS_CRYPTO_TEE) += crypto_tee.o
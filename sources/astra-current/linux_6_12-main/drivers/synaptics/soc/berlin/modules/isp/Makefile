# If either ISP_DOLPHIN or ISP_DOLPHIN_A0 is 'y'
ifeq ($(filter y,$(CONFIG_ISP_DOLPHIN) $(CONFIG_ISP_DOLPHIN_A0)),y)
obj-y += dolphin/
else
# Else, if CONFIG_ISP_KLAMATH is not 'n'
ifneq ($(CONFIG_ISP_KLAMATH),n)
obj-$(CONFIG_ISP_KLAMATH) += klamath/
endif
endif

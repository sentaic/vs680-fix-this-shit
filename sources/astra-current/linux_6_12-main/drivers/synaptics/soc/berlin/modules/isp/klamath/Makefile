obj-$(CONFIG_ISP_KLAMATH) += csi_subdev.o
obj-$(CONFIG_ISP_KLAMATH) += camera_video.o
obj-$(CONFIG_ISP_KLAMATH) += isp_subdev.o

ISP_ABS_PATH := $(srctree)/drivers/synaptics/soc/berlin/modules/isp/klamath
AVIO_ABS_PATH := $(srctree)/drivers/synaptics/soc/berlin/modules/avio

HEADER_PATH += -I$(srctree)/drivers/synaptics/include \
		-I$(ISP_ABS_PATH) \
		-I$(ISP_ABS_PATH)/fw \
		-I$(ISP_ABS_PATH)/video \
		-I$(ISP_ABS_PATH)/csi \
		-I$(ISP_ABS_PATH)/isp \
		-I$(ISP_ABS_PATH)/isp/dhub \
		-I$(AVIO_ABS_PATH)/fw_inc/klamath

ccflags-y += $(HEADER_PATH)

ccflags-y += -DLINUX
csi_subdev-objs += csi/dw_mipi_csi.o \
	csi/snps_dphy_gen3.o \
	csi/snps_dphy_csi2.o \
	csi/snps_dphy_wrap.o

camera_video-objs += video/camera_video_register.o \
	video/camera_video_driver.o

isp_subdev-objs += isp/isp_shm.o \
	isp/csipipe.o \
	isp/dhub/csi_dhub_drv.o \
	isp/dhub/hal_dhub.o \
	isp/cam_bcmbuf.o \
	isp/camera_isp_driver.o \
	isp/intr_handler.o \
	isp/fvf.o \
	isp/csc.o \
	isp/csc_icsccfg.o \
	isp/demosaic.o \
	isp/wb.o \
	isp/camera_isp_wb_sysfs.o

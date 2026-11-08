/**
 * @file dw_mipi_csi.c
 * @brief MIPI CSI-2 controller driver
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */

#include <linux/version.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-fwnode.h>

#include "dw_mipi_csi.h"
#include "snps_dphy_csi2.h"
#include "snps_dphy_wrap.h"

#define DW_CSI_IPI_HSA	32
#define DW_CSI_IPI_HBP	32
#define DW_CSI_IPI_HSD	32
#define DW_CSI_IPI_VSA	5
#define DW_CSI_IPI_VBP	36
#define DW_CSI_IPI_VFP	4

static const char *csi_clock_list[] = {
	"avioipiclk",
};

/**
 * @short Video formats supported by the MIPI CSI-2
 */
static const struct mipi_fmt dw_mipi_csi_formats[] = {
	{
		.name = "RAW BGGR 8",
		.code = MEDIA_BUS_FMT_SBGGR8_1X8,
		.depth = 8,
	},
	{
		.name = "RAW10",
		.code = MEDIA_BUS_FMT_SBGGR10_2X8_PADHI_BE,
		.depth = 10,
	},
	{
		.name = "RGB888",
		.code = MEDIA_BUS_FMT_RGB888_2X12_LE,
		.depth = 24,
	},
	{
		.name = "BGR888",
		.code = MEDIA_BUS_FMT_RGB888_2X12_BE,
		.depth = 24,
	},
	{
		.name = "RGB888_1X24",
		.code = MEDIA_BUS_FMT_RGB888_1X24,
		.depth = 24,
	},
	{
		.name = "YUV422_8",
		.code = MEDIA_BUS_FMT_YUYV8_2X8,
		.depth = 16,
	},
	{
		.name = "UYVY422_8",
		.code = MEDIA_BUS_FMT_UYVY8_1X16,
		.depth = 16,
	},
	{
		.name = "YUV422_10",
		.code = MEDIA_BUS_FMT_YUYV10_2X10,
		.depth = 20,
	},
	{
		.name = "YUV420_SP",
		.code = MEDIA_BUS_FMT_YUYV8_1_5X8,
		.depth = 12,
	},
	{
		.name = "YUV444_8",
		.code = MEDIA_BUS_FMT_YUV8_1X24,
		.depth = 24,
	},
};

static void dw_mipi_csi_write(struct mipi_csi_dev *dev,
		unsigned int address, unsigned int data)
{
	pr_debug("W %04X = 0x%x\n", address, data);
	writel(data, dev->base_address + address);
}

static u32 dw_mipi_csi_read(struct mipi_csi_dev *dev, unsigned int address)
{
	uint32_t data;

	data = readl(dev->base_address + address);
	pr_debug("R 0x%x: 0X%X\n", address, data);
	return data;
}

static void dw_mipi_csi_write_part(struct mipi_csi_dev *dev,
		unsigned long address, unsigned long data,
		unsigned char shift, unsigned char width)
{
	u32 mask = (1 << width) - 1;
	u32 temp = ioread32(dev->base_address + address);

	temp &= ~(mask << shift);
	temp |= (data & mask) << shift;
	dw_mipi_csi_write(dev, address, temp);
}

static int csi_fetch_clocks(struct device *dev, struct clk **csi_clks)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(csi_clock_list); i++) {
		csi_clks[i] = devm_clk_get(dev, csi_clock_list[i]);
		if (IS_ERR(csi_clks[i])) {
			dev_err(dev, "failed to get %s ...!\n", csi_clock_list[i]);
			return PTR_ERR(csi_clks[i]);
		}
	}

	return 0;
}

static int csi_enable_clocks(struct device *dev, struct clk **csi_clks)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(csi_clock_list); i++) {
		ret = clk_prepare_enable(csi_clks[i]);
		if (ret < 0) {
			dev_err(dev, "%s prepare failed..!\n", csi_clock_list[i]);
			goto prepare_failure;
		}
	}
	return 0;
prepare_failure:
	while (--i >= 0)
		clk_disable_unprepare(csi_clks[i]);

	return ret;
}

void dw_mipi_csi_intr_enable(struct mipi_csi_dev *dev, int en)
{
	if (en) {
		dw_mipi_csi_write(dev, R_CSI2_N_LANES, (dev->hw[0].num_lanes - 1));

		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_PHY_FATAL, 0xFFFFFFFF);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_PHY, 0xFFFFFFFF);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_LINE, 0xFFFFFFFF);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_IPI, 0xFFFFFFFF);
	} else {
		/* set only one lane (lane 0) as active (ON) */
		dw_mipi_csi_write(dev, R_CSI2_N_LANES, 0);

		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_PHY_FATAL, 0);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_PHY, 0);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_LINE, 0);
		dw_mipi_csi_write(dev, R_CSI2_MASK_INT_IPI, 0);
	}
}

static void dw_mipi_csi_reset(struct mipi_csi_dev *dev)
{
	dw_mipi_csi_write(dev, R_CSI2_CTRL_RESETN, 0);
	dw_mipi_csi_write(dev, R_CSI2_CTRL_RESETN, 1);
}

static int dw_mipi_csi_mask_irq_power_off(struct mipi_csi_dev *dev)
{
	dw_mipi_csi_intr_enable(dev, 0);
	dw_mipi_csi_write(dev, R_CSI2_CTRL_RESETN, 0);

	return 0;
}

static int dw_mipi_csi_hw_stdby(struct mipi_csi_dev *dev)
{
	/* set only one lane (lane 0) as active (ON) */
	dw_mipi_csi_reset(dev);
	snps_dphy_init(&dev->phy);
	dw_mipi_csi_intr_enable(dev, 1);

	return 0;
}

void dw_mipi_csi_set_ipi_fmt(struct mipi_csi_dev *dev, int ipi, uint32_t code)
{
	int fmt;
	unsigned int feature;

	if (ipi >= MAX_IPI_NUM) {
		pr_info("IPI[%d] not supported\n", ipi);
		return;
	}

	switch (code) {
	case MEDIA_BUS_FMT_RGB565_2X8_BE:
	case MEDIA_BUS_FMT_RGB565_2X8_LE:
		fmt = CSI_2_RGB565;
		break;

	case MEDIA_BUS_FMT_RGB888_2X12_LE:
	case MEDIA_BUS_FMT_RGB888_2X12_BE:
	case MEDIA_BUS_FMT_RGB888_1X24:
		fmt = CSI_2_RGB888;
		break;
	case MEDIA_BUS_FMT_SBGGR10_2X8_PADHI_LE:
	case MEDIA_BUS_FMT_SBGGR10_2X8_PADLO_BE:
	case MEDIA_BUS_FMT_SBGGR10_2X8_PADLO_LE:
	case MEDIA_BUS_FMT_SBGGR10_2X8_PADHI_BE:
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
		fmt = CSI_2_RAW10;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_UYVY8_1X16:
		fmt = CSI_2_YUV422_8;
		break;
	case MEDIA_BUS_FMT_YUYV10_2X10:
		fmt = CSI_2_YUV422_10;
		break;
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
	case MEDIA_BUS_FMT_SBGGR8_1X8:
		fmt = CSI_2_RAW8;
		break;

	case MEDIA_BUS_FMT_SBGGR12_1X12:
	case MEDIA_BUS_FMT_SGBRG12_1X12:
	case MEDIA_BUS_FMT_SGRBG12_1X12:
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		fmt = CSI_2_RAW12;
		break;

	case MEDIA_BUS_FMT_SRGGB14_1X14:
		fmt = CSI_2_RAW14;
		break;
	case MEDIA_BUS_FMT_SRGGB16_1X16:
		fmt = CSI_2_RAW16;
		break;
	case MEDIA_BUS_FMT_UYVY8_2X8:
		fmt = CSI_2_YUV422_8;
		break;
	case MEDIA_BUS_FMT_UYVY10_2X10:
		fmt = CSI_2_YUV422_10;
		break;
	case MEDIA_BUS_FMT_YUYV8_1_5X8:
		fmt = CSI_2_YUV420_8;
		break;

	default:
		break;
	}
	dev->hw[ipi].data_type = fmt;

	//To handle embedded metadata
	feature = ((0<<24)|(1<<17)|(1<<16));
	if (ipi == 0)
		dw_mipi_csi_write(dev, R_CSI2_IPI_ADV_FEATURES, feature);
	else
		dw_mipi_csi_write(dev, R_CSI2_IPI2_ADV_FEATURES, feature);
}

void dw_mipi_csi_configure(struct mipi_csi_dev *dev)
{
	int hbp, hsa, hsd, vc = 0;
	unsigned int offset = 0, ipi;
	unsigned int dataid;
	unsigned int ipi_offsets[4] = {R_CSI2_IPI_MODE, R_CSI2_IPI2_MODE,
					R_CSI2_IPI3_MODE, R_CSI2_IPI4_MODE};
	int width;

	dw_mipi_csi_write(dev, R_CSI2_N_LANES, (dev->hw[0].num_lanes - 1));

	/*IPI Related Configuration*/
	for (ipi = 0; ipi < MAX_IPI_NUM; ipi++) {
		if ((dev->hw[ipi].output_type == IPI_OUT) ||
			(dev->hw[ipi].output_type == BOTH_OUT)) {

			if (dev->hw[ipi].hactive == 0 || dev->hw[ipi].vactive == 0) {
				pr_err("%s hactive vactive set to 0\n", __func__);
				continue;
			}

			vc = dev->hw[ipi].virtual_ch;
			offset = ipi_offsets[ipi] - ipi_offsets[0];

			dw_mipi_csi_write(dev, offset + R_CSI2_IPI_DATA_TYPE,
						dev->hw[ipi].data_type);

			dataid = dw_mipi_csi_read(dev, R_CSI2_DATA_IDS_1);
			dw_mipi_csi_write(dev, R_CSI2_DATA_IDS_VC_1, vc);

			dataid = dev->hw[ipi].data_type;
			dw_mipi_csi_write(dev, R_CSI2_DATA_IDS_1, dataid);

			dw_mipi_csi_write_part(dev, offset + R_CSI2_IPI_MODE,
					dev->hw[ipi].ipi_mode, 0, 1);

			dw_mipi_csi_write_part(dev, offset + R_CSI2_IPI_MODE,
					dev->hw[ipi].ipi_color_mode, 8, 1);

			dw_mipi_csi_write_part(dev, offset + R_CSI2_IPI_MODE,
					dev->hw[ipi].ipi_cut_through_en, 16, 1);
			dw_mipi_csi_write(dev, offset + R_CSI2_IPI_VCID, vc);

			dw_mipi_csi_write_part(dev, offset + R_CSI2_IPI_MEM_FLUSH,
					dev->hw[ipi].ipi_auto_flush, 8, 1);

			width = dev->hw[ipi].htotal - (dev->hw[ipi].hsa +
					dev->hw[ipi].hbp + dev->hw[ipi].hsd);


			hsa = dev->hw[ipi].hsa;
			hbp = dev->hw[ipi].hbp;
			hsd = dev->hw[ipi].hsd;

			dw_mipi_csi_write(dev, offset + R_CSI2_IPI_HSA_TIME, hsa);

			dw_mipi_csi_write(dev, offset + R_CSI2_IPI_HBP_TIME, hbp);

			dw_mipi_csi_write(dev, offset + R_CSI2_IPI_HSD_TIME, hsd);

			if (ipi == 0) {
				//controller timings
				dw_mipi_csi_write(dev, offset + R_CSI2_IPI_HLINE_TIME,
						dev->hw[ipi].htotal);

				dw_mipi_csi_write(dev, offset + R_CSI2_IPI_VSA_LINES,
						dev->hw[ipi].vsa);

				dw_mipi_csi_write(dev, offset + R_CSI2_IPI_VBP_LINES,
						dev->hw[ipi].vbp);

				dw_mipi_csi_write(dev, offset + R_CSI2_IPI_VFP_LINES,
						dev->hw[ipi].vfp);

				dw_mipi_csi_write(dev, offset + R_CSI2_IPI_VACTIVE_LINES,
						dev->hw[ipi].vactive);
			}
			dw_mipi_csi_write_part(dev, offset + R_CSI2_IPI_MODE, 1, 24, 1);
		}
	}
}

static void dw_mipi_csi_fill_timings(struct mipi_csi_dev *dev, u32 width, u32 height)
{
	dev->hw[0].hsa = DW_CSI_IPI_HSA;
	dev->hw[0].hbp = DW_CSI_IPI_HBP;
	dev->hw[0].hsd = DW_CSI_IPI_HSD;

	dev->hw[0].vsa = 0;
	dev->hw[0].vbp = 0;
	dev->hw[0].vfp = 0;

	dev->hw[0].htotal = width;
	dev->hw[0].hactive = width;
	dev->hw[0].vactive = height;
}

static void dw_mipi_csi_start(struct mipi_csi_dev *dev)
{
	dw_mipi_csi_configure(dev);
	snps_dphy_power_on(&dev->phy);
}

static int dw_mipi_csi_enum_mbus_code(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *state,
		struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index >= ARRAY_SIZE(dw_mipi_csi_formats))
		return -EINVAL;

	code->code = dw_mipi_csi_formats[code->index].code;
	return 0;
}

static struct v4l2_mbus_framefmt *__dw_mipi_csi_get_format(
		struct mipi_csi_dev *dev, struct v4l2_subdev_state *state,
		unsigned int pad,
		enum v4l2_subdev_format_whence which)
{
	if (which == V4L2_SUBDEV_FORMAT_TRY)
		return v4l2_subdev_state_get_format(state, pad);

	return &dev->format;
}

static int dw_mipi_csi_set_fmt(struct v4l2_subdev *sd, struct v4l2_subdev_state *state,
		struct v4l2_subdev_format *fmt)
{
	struct mipi_csi_dev *dev = v4l2_get_subdevdata(sd);
	struct media_pad *pad;
	struct v4l2_subdev *subdev;
	int ret;
	struct v4l2_subdev_format sensor_fmt = {
		.which = fmt->which,
		.pad = 0,
		.format = {
			.width  = fmt->format.width,
			.height = fmt->format.height,
			.code   = fmt->format.code,   /* forward requested format code to sensor */
		}
	};

	pr_debug("%s input w: %d h: %d code: 0x%x which: %d\n", __func__, fmt->format.width,
			fmt->format.height, fmt->format.code, fmt->which);

	pad = media_pad_remote_pad_first(&dev->pads[CSI_PAD_SINK]);

	if (pad) {
		if (is_media_entity_v4l2_subdev(pad->entity)) {
			subdev = media_entity_to_v4l2_subdev(pad->entity);
			if (!subdev)
				pr_err("subdev is null\n");
			fmt->pad = 0;
			ret = v4l2_subdev_call(subdev, pad, set_fmt, NULL, &sensor_fmt);
			if (ret)
				pr_debug("%s set_fmt failed %d\n", __func__, ret);
			else
				pr_err("%s support w: %d h: %d code: 0x%x\n", __func__,
					sensor_fmt.format.width, sensor_fmt.format.height,
					sensor_fmt.format.code);
		}

		dw_mipi_csi_fill_timings(dev, sensor_fmt.format.width,
				sensor_fmt.format.height);
		dw_mipi_csi_set_ipi_fmt(dev, 0, sensor_fmt.format.code);
		/* CSI IPI is in COLOR48 mode: even though the sensor sends YUV422 (UYVY),
		 * the IPI upsamples it to YUV444 internally. Tell the ISP YUV8_1X24 so
		 * csipipe configures the correct YUV444->YUV422 DNS pipeline.
		 */
		if (sensor_fmt.format.code == MEDIA_BUS_FMT_UYVY8_1X16)
			sensor_fmt.format.code = MEDIA_BUS_FMT_YUV8_1X24;
		*fmt = sensor_fmt;
	} else {
		/* Default values from DT get used */
		dw_mipi_csi_fill_timings(dev, fmt->format.width, fmt->format.height);
		dw_mipi_csi_set_ipi_fmt(dev, 0, dev->hw[0].v4l2_data_type);
		fmt->format.code = dev->hw[0].v4l2_data_type;
	}

	return 0;
}

static int dw_mipi_csi_get_fmt(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *state,
		struct v4l2_subdev_format *fmt)
{
	struct mipi_csi_dev *dev = v4l2_get_subdevdata(sd);
	struct v4l2_mbus_framefmt *mf;

	mf = __dw_mipi_csi_get_format(dev, state, fmt->pad, fmt->which);
	if (!mf)
		return -EINVAL;

	mutex_lock(&dev->lock);
	fmt->format = *mf;
	mutex_unlock(&dev->lock);
	return 0;
}

static int dw_mipi_csi_enum_frame_size(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *state,
		struct v4l2_subdev_frame_size_enum *fse)
{
	struct mipi_csi_dev *dev = v4l2_get_subdevdata(sd);
	struct media_pad *sensor_pad;
	struct v4l2_subdev *sensor_subdev;
	struct v4l2_subdev_frame_size_enum sensor_fse;
	int ret;

	/* Find sensor subdevice connected to CSI sink pad */
	sensor_pad = media_pad_remote_pad_first(&dev->pads[CSI_PAD_SINK]);
	if (!sensor_pad) {
		pr_err("%s: No sensor connected to CSI sink pad\n", __func__);
		return -ENODEV;
	}

	if (!is_media_entity_v4l2_subdev(sensor_pad->entity)) {
		pr_err("%s: Connected entity is not a V4L2 subdev\n", __func__);
		return -ENODEV;
	}

	sensor_subdev = media_entity_to_v4l2_subdev(sensor_pad->entity);
	if (!sensor_subdev) {
		pr_err("%s: Invalid sensor subdevice\n", __func__);
		return -ENODEV;
	}

	/* Forward frame size enumeration to sensor */
	sensor_fse = *fse;
	/* Sensor source pad */
	sensor_fse.pad = 0;

	ret = v4l2_subdev_call(sensor_subdev, pad, enum_frame_size, state, &sensor_fse);
	if (ret) {
		pr_debug("%s: Sensor enum_frame_size failed: %d\n", __func__, ret);
		return ret;
	}

	/* Copy sensor results back to CSI */
	fse->min_width = sensor_fse.min_width;
	fse->max_width = sensor_fse.max_width;
	fse->min_height = sensor_fse.min_height;
	fse->max_height = sensor_fse.max_height;

	return 0;
}

static int dw_mipi_csi_s_power(struct v4l2_subdev *sd, int on)
{
	struct mipi_csi_dev *dev = v4l2_get_subdevdata(sd);
	struct media_pad *pad;
	struct v4l2_subdev *subdev;
	int ret;

	pr_debug("%s on: %d base: %lx\n", __func__, on, (unsigned long)dev->base_address);

	pad = media_pad_remote_pad_first(&dev->pads[CSI_PAD_SINK]);

	if (on) {
		/* Init DPHY before sensor stream-on so LP11 stop-state is stable */
		dw_mipi_csi_hw_stdby(dev);
		dw_mipi_csi_start(dev);
	}

	if (pad) {
		if (is_media_entity_v4l2_subdev(pad->entity)) {
			subdev = media_entity_to_v4l2_subdev(pad->entity);
			ret = v4l2_subdev_call(subdev, video, s_stream, on);
		}

		if (ret)
			pr_err("error in stream: %d\n", ret);
	}

	if (!on)
		dw_mipi_csi_mask_irq_power_off(dev);

	return 0;
}

static int dw_mipi_csi_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct v4l2_mbus_framefmt *format = v4l2_subdev_state_get_format(fh->state, 0);

	format->colorspace = V4L2_COLORSPACE_SRGB;
	format->code = dw_mipi_csi_formats[0].code;
	format->width = MIN_WIDTH;
	format->height = MIN_HEIGHT;
	format->field = V4L2_FIELD_NONE;

	return 0;
}

static const struct v4l2_subdev_internal_ops dw_mipi_csi_sd_internal_ops = {
	.open = dw_mipi_csi_open,
};

static const struct v4l2_subdev_core_ops dw_mipi_csi_core_ops = {
	.s_power = dw_mipi_csi_s_power,
};

static const struct v4l2_subdev_pad_ops dw_mipi_csi_pad_ops = {
	.enum_mbus_code = dw_mipi_csi_enum_mbus_code,
	.enum_frame_size = dw_mipi_csi_enum_frame_size,
	.get_fmt = dw_mipi_csi_get_fmt,
	.set_fmt = dw_mipi_csi_set_fmt,
};

static const struct v4l2_subdev_ops dw_mipi_csi_subdev_ops = {
	.core = &dw_mipi_csi_core_ops,
	.pad = &dw_mipi_csi_pad_ops,
};

static int dw_mipi_csi_parse_dt(struct platform_device *pdev,
		struct mipi_csi_dev *dev)
{
	struct device_node *node = pdev->dev.of_node;
	int ret = 0;
	int ipi = 0;

	// Device tree information
	ret = of_property_read_u32(node, "data-lanes", &dev->hw[ipi].num_lanes);
	if (ret) {
		dev_err(&pdev->dev, "Couldn't read data-lanes\n");
		return ret;
	}

	ret = of_property_read_u32(node, "data-type", &dev->hw[ipi].v4l2_data_type);
	if (ret) {
		dev_err(&pdev->dev, "Couldn't read data-type\n");
		return ret;
	}

	ret = csi_fetch_clocks(&pdev->dev, dev->csi_clks);
	if (ret) {
		dev_err(&pdev->dev, "csi clock fetch failed...!\n");
		return ret;
	}

	dev->hw[ipi].output_type = IPI_OUT;
	dev->hw[ipi].ipi_mode = CAMERA_TIMING;
	dev->hw[ipi].ipi_auto_flush = 1;
	dev->hw[ipi].ipi_color_mode = COLOR48;
	dev->hw[ipi].virtual_ch = 0;
	dev->hw[ipi].ipi_cut_through_en = CTACTIVE;
	dev->index = 0;


	if (dev->index >= CSI_MAX_ENTITIES)
		return -ENXIO;

	return 0;
}

static int csi_subdev_notifier_bound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd,
		struct v4l2_async_connection *asc)
{
	int ret = 0;
	struct mipi_csi_dev *csi_dev = container_of(notifier,
			struct mipi_csi_dev, notifier);
	struct device *dev =  csi_dev->dev;
	struct fwnode_handle *ep = NULL;
	struct v4l2_fwnode_link link;
	struct media_entity *source, *sink;
	unsigned int source_pad, sink_pad;
	int port_id = 0;

	port_id = 0;
	ep = fwnode_graph_get_next_endpoint(sd->fwnode, ep);
	if (!ep) {
		pr_err("endpoint node not present!!\n");
		return -ENOLINK;
	}

	ret = v4l2_fwnode_parse_link(ep, &link);
	if (ret < 0) {
		dev_err(dev, "failed to parse link for %pOF: %d\n",
				to_of_node(ep), ret);
		fwnode_handle_put(ep);
		return ret;
	}

	if (sd->entity.pads[link.local_port].flags == MEDIA_PAD_FL_SINK) {
		pr_err("should not be sink node!!\n");
		return -ENOLINK;
	}

	source	   = &sd->entity;
	source_pad = link.local_port;
	sink	   = &csi_dev->sd.entity;
	sink_pad   = link.remote_port;
	v4l2_fwnode_put_link(&link);
	pr_err("%s: linking %s source_pad %d flags %ld and %s sink_pad %d flags %ld\n",
			__func__, source->name, source_pad, source->pads[source_pad].flags,
			sink->name, sink_pad, sink->pads[sink_pad].flags);
	ret = media_create_pad_link(source, source_pad,
			sink, sink_pad, MEDIA_LNK_FL_ENABLED);
	if (ret) {
		dev_err(dev, "failed to create %s:%u -> %s:%u link\n",
				source->name, source_pad,
				sink->name, sink_pad);
	}

	fwnode_handle_put(ep);

	return ret;
}

static void csi_subdev_notifier_unbound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd,
		struct v4l2_async_connection *asc)
{
}

static const struct v4l2_async_notifier_operations csi_subdev_notify_ops = {
	.bound	  = csi_subdev_notifier_bound,
	.unbind   = csi_subdev_notifier_unbound,
};

static int csi_subdev_unregister_notifier(struct mipi_csi_dev *csi_dev)
{
	v4l2_async_nf_unregister(&csi_dev->notifier);
	v4l2_async_nf_cleanup(&csi_dev->notifier);

	return 0;
}

static int csi_subdev_register_notifier(struct mipi_csi_dev *csi_dev)
{
	struct fwnode_handle *ep;
	struct fwnode_handle *remote_ep;
	struct v4l2_async_connection *asc;
	struct device *dev = csi_dev->dev;
	int ret = 0;
	int pad = 0;

	v4l2_async_subdev_nf_init(&csi_dev->notifier, &csi_dev->sd);

	csi_dev->notifier.ops = &csi_subdev_notify_ops;

	if (dev_fwnode(csi_dev->dev) == NULL)
		return 0;

	for (pad = 0; pad < CSI_PADS_NUM; pad++) {

		if (csi_dev->pads[pad].flags != MEDIA_PAD_FL_SINK)
			continue;

		ep = fwnode_graph_get_endpoint_by_id(dev_fwnode(dev),
				pad, 0, FWNODE_GRAPH_ENDPOINT_NEXT);
		if (!ep)
			continue;

		remote_ep = fwnode_graph_get_remote_endpoint(ep);
		if (!remote_ep) {
			fwnode_handle_put(ep);
			continue;
		}
		fwnode_handle_put(remote_ep);
		asc = v4l2_async_nf_add_fwnode_remote(&csi_dev->notifier,
				ep, struct v4l2_async_connection);

		fwnode_handle_put(ep);

		if (IS_ERR(asc)) {
			ret = PTR_ERR(asc);
			if (ret != -EEXIST) {
				v4l2_async_nf_cleanup(&csi_dev->notifier);
				return ret;
			}
		}
	}

	ret = v4l2_async_nf_register(&csi_dev->notifier);
	if (ret) {
		dev_err(csi_dev->dev, "Async notifier register error\n");
		v4l2_async_nf_cleanup(&csi_dev->notifier);
	}

	return ret;
}

static const struct of_device_id dw_mipi_csi_of_match[];

/**
 * @short Initialization routine - Entry point of the driver
 * @param[in] pdev pointer to the platform device structure
 * @return 0 on success and a negative number on failure
 * Refer to Linux errors.
 */
static int mipi_csi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res = NULL;
	struct mipi_csi_dev *mipi_csi;
	int ret = -ENOMEM;
	int i = 0;

	dev_info(&pdev->dev, "Installing MIPI CSI-2 module\n");

	dev_dbg(&pdev->dev, "Device registration\n");
	mipi_csi = devm_kzalloc(dev, sizeof(*mipi_csi), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	mipi_csi->csi_clks = devm_kzalloc(dev,
		sizeof(struct clk *) * ARRAY_SIZE(csi_clock_list), GFP_KERNEL);
	if (IS_ERR(mipi_csi->csi_clks)) {
		dev_err(dev, "failed to allocate memory for csi_clocks...!\n");
		return PTR_ERR(mipi_csi->csi_clks);
	}

	mutex_init(&mipi_csi->lock);
	spin_lock_init(&mipi_csi->slock);
	mipi_csi->pdev = pdev;

	ret = dw_mipi_csi_parse_dt(pdev, mipi_csi);
	if (ret < 0)
		return ret;

	ret = csi_enable_clocks(dev, mipi_csi->csi_clks);
	if (ret) {
		dev_err(dev, "CSI clock enable failed...!\n");
		return ret;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	mipi_csi->base_address = devm_ioremap_resource(dev, res);

	if (IS_ERR(mipi_csi->base_address)) {
		ret = PTR_ERR(mipi_csi->base_address);
		goto clk_cleanup;
	}

	pr_err("CSI Base address: %lx\n", (unsigned long)mipi_csi->base_address);
	mipi_csi->phy.mipi_base = mipi_csi->base_address;
	ret = snps_dphy_probe(&mipi_csi->phy, 0); //Initialize DPHY
	if (ret < 0) {
		pr_err("DPHY probe failed: %d\n", ret);
		goto clk_cleanup;
	}

	v4l2_subdev_init(&mipi_csi->sd, &dw_mipi_csi_subdev_ops);
	snprintf(mipi_csi->sd.name, sizeof(mipi_csi->sd.name), "%s.%d",
			CSI_DEVICE_NAME, mipi_csi->index);

	mipi_csi->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	mipi_csi->sd.dev =	&pdev->dev;
	mipi_csi->sd.owner = THIS_MODULE;
	mipi_csi->sd.entity.function = MEDIA_ENT_F_IO_V4L;
	mipi_csi->sd.entity.obj_type = MEDIA_ENTITY_TYPE_V4L2_SUBDEV;
	mipi_csi->sd.entity.name = mipi_csi->sd.name;

	mipi_csi->fmt = &dw_mipi_csi_formats[0];

	mipi_csi->format.code = dw_mipi_csi_formats[0].code;
	mipi_csi->format.width = MIN_WIDTH;
	mipi_csi->format.height = MIN_HEIGHT;

	mipi_csi->pads[CSI_PAD_SINK].flags = MEDIA_PAD_FL_SINK;
	mipi_csi->pads[CSI_PAD_SOURCE].flags = MEDIA_PAD_FL_SOURCE;
	ret = media_entity_pads_init(&mipi_csi->sd.entity,
			CSI_PADS_NUM, mipi_csi->pads);
	if (ret < 0) {
		dev_err(dev, "Media Entity init failed\n");
		goto entity_cleanup;
	}

	mipi_csi->dev = dev;
	/* This allows to retrieve the platform device id by the host driver */
	v4l2_set_subdevdata(&mipi_csi->sd, mipi_csi);

	ret = csi_subdev_register_notifier(mipi_csi);
	if (ret)
		goto entity_cleanup;

	platform_set_drvdata(pdev, mipi_csi);

	ret = v4l2_async_register_subdev(&mipi_csi->sd);
	if (ret) {
		dev_err(dev, "register subdev error\n");
		goto notifier_cleanup;
	}

	dev_info(dev, "DW MIPI CSI-2 Host registered successfully\n");

	return 0;

notifier_cleanup:
	csi_subdev_unregister_notifier(mipi_csi);
entity_cleanup:
	media_entity_cleanup(&mipi_csi->sd.entity);
clk_cleanup:
	for (i = 0; i < ARRAY_SIZE(csi_clock_list); i++)
		clk_disable_unprepare(mipi_csi->csi_clks[i]);

	return ret;
}

static void mipi_csi_remove(struct platform_device *pdev)
{
	struct mipi_csi_dev *mipi_csi = platform_get_drvdata(pdev);
	int i = 0;

	if (mipi_csi) {
		/* Unregister subdev and cleanup media entity */
		v4l2_async_unregister_subdev(&mipi_csi->sd);
		csi_subdev_unregister_notifier(mipi_csi);
		media_entity_cleanup(&mipi_csi->sd.entity);
		for (i = 0; i < ARRAY_SIZE(csi_clock_list); i++)
			clk_disable_unprepare(mipi_csi->csi_clks[i]);
	}
}

/**
 * @short of_device_id structure
 */
static const struct of_device_id dw_mipi_csi_of_match[] = {
	{
		.compatible = "snps,dw-mipi-csi"
	},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, dw_mipi_csi_of_match);


/**
 * @short Platform driver structure
 */
static struct platform_driver __refdata dw_mipi_csi_pdrv = {
	.remove = mipi_csi_remove,
	.probe	= mipi_csi_probe,
	.driver   = {
		.name  = CSI_DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = dw_mipi_csi_of_match,
	},
};

module_platform_driver(dw_mipi_csi_pdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ramiro Oliveira <roliveir@synopsys.com>");
MODULE_DESCRIPTION("Synopys DW MIPI CSI-2 Host driver");

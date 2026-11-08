// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/pinctrl/consumer.h>
#include <linux/gpio/consumer.h>
#include "loop-edid.h"

static struct platform_device *loop_device;
struct loop_board_pins {
    struct gpio_desc *hpd;
    struct gpio_desc *source_5v;
    struct loop_edid edid;
};

static ssize_t status_show(struct device *dev, struct device_attribute *attribute,
                           char *buffer)
{
    struct loop_board_pins *pins = dev_get_drvdata(dev);
    int direction = gpiod_get_direction(pins->hpd);
    int hpd = gpiod_get_value_cansleep(pins->hpd);
    int source_5v = gpiod_get_value_cansleep(pins->source_5v);
    bool edid_ready;
    u32 edid_control;
    int requested_hpd;
    mutex_lock(&pins->edid.lock);
    edid_ready = loop_edid_ready_locked(&pins->edid);
    edid_control = loop_edid_control_locked(&pins->edid);
    requested_hpd = pins->edid.requested_hpd;
    mutex_unlock(&pins->edid.lock);
    if (direction < 0) return direction;
    if (hpd < 0) return hpd;
    if (source_5v < 0) return source_5v;
    return scnprintf(buffer, PAGE_SIZE,
        "hpd_direction=%s gpio_read=%d requested_hpd=%d source_5v=%d edid_ready=%u edid_control=%#x ddc_scl_pad=%#x ddc_sda_pad=%#x\n",
        direction ? "input" : "output", hpd, requested_hpd, source_5v, edid_ready, edid_control,
        readl(pins->edid.ddc_pads), readl(pins->edid.ddc_pads + sizeof(u32)));
}
static DEVICE_ATTR_RO(status);

static ssize_t hpd_store(struct device *dev, struct device_attribute *attribute,
                         const char *buffer, size_t count)
{
    struct loop_board_pins *pins = dev_get_drvdata(dev);
    int value, error;
    if (sysfs_streq(buffer, "0")) value = 0;
    else if (sysfs_streq(buffer, "1")) value = 1;
    else return -EINVAL;
    mutex_lock(&pins->edid.lock);
    if (value && !loop_edid_ready_locked(&pins->edid)) error = -ENODATA;
    else {
        error = gpiod_direction_output(pins->hpd, value);
        if (!error) pins->edid.requested_hpd = value;
    }
    mutex_unlock(&pins->edid.lock);
    if (error) return error;
    dev_info(dev, "RX external HPD=%d\n", value);
    return count;
}
static DEVICE_ATTR_WO(hpd);

static struct attribute *loop_pin_attributes[] = {
    &dev_attr_status.attr, &dev_attr_hpd.attr, NULL
};
static const struct attribute_group loop_pin_group = {
    .attrs = loop_pin_attributes
};
static int loop_probe(struct platform_device *device)
{
    struct loop_board_pins *board;
    struct pinctrl *pins;
    int error;
    pins = devm_pinctrl_get_select_default(&device->dev);
    if (IS_ERR(pins)) return PTR_ERR(pins);
    board = devm_kzalloc(&device->dev, sizeof(*board), GFP_KERNEL);
    if (!board) return -ENOMEM;
    /* Preserve boot-time directions and levels until an explicit HPD request. */
    board->hpd = devm_gpiod_get(&device->dev, "hrxhpd", GPIOD_ASIS);
    if (IS_ERR(board->hpd)) return PTR_ERR(board->hpd);
    board->source_5v = devm_gpiod_get(&device->dev, "hrx5v", GPIOD_ASIS);
    if (IS_ERR(board->source_5v)) return PTR_ERR(board->source_5v);
    error = loop_edid_attach(&device->dev, &board->edid, board->hpd);
    if (error) return error;
    dev_set_drvdata(&device->dev, board);
    error = devm_device_add_group(&device->dev, &loop_pin_group);
    if (error) return error;
    dev_info(&device->dev, "RX pinctrl and GPIOs selected from board device tree\n");
    return 0;
}
static struct platform_driver loop_driver = {
    .probe = loop_probe,
    .driver = {.name = "vs680-loop-pinctrl"},
};
static int __init loop_init(void)
{
    struct device_node *node = of_find_node_by_path("/soc/hrx@f74a0000");
    int error;
    if (!node)
        return -ENODEV;
    error = platform_driver_register(&loop_driver);
    if (error) {
        of_node_put(node);
        return error;
    }
    loop_device = platform_device_alloc("vs680-loop-pinctrl", PLATFORM_DEVID_NONE);
    if (!loop_device) {
        of_node_put(node);
        platform_driver_unregister(&loop_driver);
        return -ENOMEM;
    }
    loop_device->driver_override = kstrdup("vs680-loop-pinctrl", GFP_KERNEL);
    if (!loop_device->driver_override) {
        of_node_put(node);
        platform_device_put(loop_device);
        platform_driver_unregister(&loop_driver);
        return -ENOMEM;
    }
    loop_device->dev.of_node = node;
    error = platform_device_add(loop_device);
    if (error) {
        platform_device_put(loop_device);
        platform_driver_unregister(&loop_driver);
    }
    return error;
}
static void __exit loop_exit(void)
{
    platform_device_unregister(loop_device);
    platform_driver_unregister(&loop_driver);
}
module_init(loop_init);
module_exit(loop_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HDMI RX pinctrl, HPD and EDID");

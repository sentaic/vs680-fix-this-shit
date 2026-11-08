/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_LOOP_EDID_H
#define VS680_LOOP_EDID_H
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
struct loop_edid {
    void __iomem *registers;
    void __iomem *ddc_pads;
    struct gpio_desc *hpd;
    int requested_hpd;
    struct mutex lock;
    struct bin_attribute attribute;
};
int loop_edid_attach(struct device *dev, struct loop_edid *edid,
                     struct gpio_desc *hpd);
bool loop_edid_ready_locked(struct loop_edid *edid);
u32 loop_edid_control_locked(struct loop_edid *edid);
#endif

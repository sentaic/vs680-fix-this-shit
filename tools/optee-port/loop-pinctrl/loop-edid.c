// SPDX-License-Identifier: GPL-2.0
#include <linux/of_address.h>
#include <linux/string.h>
#include "loop-edid.h"

/* Register fields and byte stride from vendor hrx-edid.c and edid.h. */
#define EDID_LENGTH 256
#define EDID_CTRL 0x400
#define EDID_ENABLE BIT(0)
#define EDID_ADDRESS_MASK GENMASK(7, 1)
#define EDID_WRITE_DONE BIT(8)
#define EDID_DONE_AUTO_CLEAR_DISABLE BIT(9)

static bool valid_edid(const u8 bytes[EDID_LENGTH])
{
    static const u8 header[] = {0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0};
    unsigned int block, i, sum;
    if (memcmp(bytes, header, sizeof(header)) || bytes[126] > 1)
        return false;
    for (block = 0; block <= bytes[126]; block++) {
        sum = 0;
        for (i = 0; i < 128; i++) sum += bytes[block * 128 + i];
        if (sum & 0xff) return false;
    }
    return true;
}

static void read_edid(struct loop_edid *edid, u8 bytes[EDID_LENGTH])
{
    unsigned int i;
    for (i = 0; i < EDID_LENGTH; i++) bytes[i] = readl(edid->registers + i * 4);
}

u32 loop_edid_control_locked(struct loop_edid *edid)
{
    return readl(edid->registers + EDID_CTRL);
}

bool loop_edid_ready_locked(struct loop_edid *edid)
{
    u8 bytes[EDID_LENGTH];
    u32 control = loop_edid_control_locked(edid);
    if (!(control & EDID_ENABLE) || !(control & EDID_WRITE_DONE) ||
        (control & EDID_ADDRESS_MASK) != (0x50 << 1)) return false;
    read_edid(edid, bytes);
    return valid_edid(bytes);
}

static ssize_t edid_read(struct file *file, struct kobject *object,
                         struct bin_attribute *attribute, char *buffer,
                         loff_t offset, size_t count)
{
    struct loop_edid *edid = container_of(attribute, struct loop_edid, attribute);
    u8 bytes[EDID_LENGTH];
    if (offset < 0) return -EINVAL;
    if (offset >= EDID_LENGTH) return 0;
    count = min_t(size_t, count, EDID_LENGTH - offset);
    mutex_lock(&edid->lock);
    read_edid(edid, bytes);
    memcpy(buffer, bytes + offset, count);
    mutex_unlock(&edid->lock);
    return count;
}

/* RX DDC pad setup follows VS680 U-Boot set_i2c_ds. HPD is held low. */
static int prepare_ddc_pads(struct loop_edid *edid)
{
    u32 before[2], wanted;
    unsigned int i;
    for (i = 0; i < ARRAY_SIZE(before); ++i) {
        before[i] = readl(edid->ddc_pads + i * sizeof(u32));
        if (!(before[i] & BIT(4))) return -EIO;
    }
    for (i = 0; i < ARRAY_SIZE(before); ++i) {
        wanted = (before[i] & ~0xfU) | 7U;
        if (before[i] != wanted)
            writel(wanted, edid->ddc_pads + i * sizeof(u32));
        if (readl(edid->ddc_pads + i * sizeof(u32)) != wanted)
            return -EIO;
    }
    return 0;
}

static ssize_t edid_write(struct file *file, struct kobject *object,
                          struct bin_attribute *attribute, char *buffer,
                          loff_t offset, size_t count)
{
    struct loop_edid *edid = container_of(attribute, struct loop_edid, attribute);
    u8 readback[EDID_LENGTH];
    u32 control;
    unsigned int i;
    int error;
    if (offset || count != EDID_LENGTH || !valid_edid((u8 *)buffer)) return -EINVAL;
    mutex_lock(&edid->lock);
    /* EDID writes are accepted after an HPD-low request. */
    if (edid->requested_hpd != 0) { error = -EBUSY; goto unlock; }
    error = prepare_ddc_pads(edid);
    if (error) goto unlock;
    control = readl(edid->registers + EDID_CTRL);
    control &= ~(EDID_ADDRESS_MASK | EDID_WRITE_DONE);
    control |= EDID_ENABLE | (0x50 << 1) | EDID_DONE_AUTO_CLEAR_DISABLE;
    writel(control, edid->registers + EDID_CTRL);
    for (i = 0; i < EDID_LENGTH; i++) writel((u8)buffer[i], edid->registers + i * 4);
    read_edid(edid, readback);
    if (memcmp(readback, buffer, EDID_LENGTH)) { error = -EIO; goto unlock; }
    writel(control | EDID_WRITE_DONE, edid->registers + EDID_CTRL);
    error = loop_edid_ready_locked(edid) ? (int)count : -EIO;
unlock:
    mutex_unlock(&edid->lock);
    return error;
}

static void remove_edid(void *argument)
{
    struct loop_edid *edid = argument;
    sysfs_remove_bin_file(edid->attribute.private, &edid->attribute);
}

int loop_edid_attach(struct device *dev, struct loop_edid *edid,
                     struct gpio_desc *hpd)
{
    struct resource resource;
    int index, error;
    index = of_property_match_string(dev->of_node, "reg-names", "edid_base");
    if (index < 0) return index;
    error = of_address_to_resource(dev->of_node, index, &resource);
    if (error) return error;
    if (resource_size(&resource) < EDID_CTRL + sizeof(u32)) return -EINVAL;
    edid->registers = devm_ioremap_resource(dev, &resource);
    if (IS_ERR(edid->registers)) return PTR_ERR(edid->registers);
    /* Map the two SCL/SDA pad registers. */
    edid->ddc_pads = devm_ioremap(dev, 0xf7fe2c1c, 2 * sizeof(u32));
    if (!edid->ddc_pads) return -ENOMEM;
    edid->hpd = hpd;
    edid->requested_hpd = -1;
    mutex_init(&edid->lock);
    sysfs_bin_attr_init(&edid->attribute);
    edid->attribute.attr.name = "edid";
    edid->attribute.attr.mode = 0600;
    edid->attribute.size = EDID_LENGTH;
    edid->attribute.read = edid_read;
    edid->attribute.write = edid_write;
    edid->attribute.private = &dev->kobj;
    error = sysfs_create_bin_file(&dev->kobj, &edid->attribute);
    if (error) return error;
    return devm_add_action_or_reset(dev, remove_edid, edid);
}

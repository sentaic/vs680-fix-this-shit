// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include "rx-transaction.h"
#include "rx-phy-context.h"
int loop_rx_start(void);
int loop_rx_start_rate(bool rate6g);
int loop_rx_video_config(void);
int loop_rx_wait_frames(unsigned frames);
int loop_rx_pattern_enable(void);
void loop_rx_pattern_restore(void);
void loop_rx_vendor_video_enable(bool initialize);
void loop_rx_vendor_video_restore(void);
ssize_t loop_rx_input(char *buffer);
u32 hrx_get_tmds_clk(struct syna_hrx_v4l2_dev *context);
struct syna_hrx_v4l2_dev loop_receiver;

static void __iomem *receiver;
static struct kobject *status_object;
static struct resource resource;

/* 串行访问 PHY 配置和信息包锁存寄存器；占用期间返回 EBUSY。 */
#define RX_SHOW(name) \
    static ssize_t name##_locked(struct kobject *, struct kobj_attribute *, char *); \
    static ssize_t name(struct kobject *object, struct kobj_attribute *attr, char *buffer) \
    { \
        ssize_t result; \
        int guard = loop_rx_register_acquire(); \
        if (guard) return guard; \
        result = name##_locked(object, attr, buffer); \
        loop_rx_register_release(); \
        return result; \
    }
#define RX_STORE(name) \
    static ssize_t name##_locked(struct kobject *, struct kobj_attribute *, const char *, size_t); \
    static ssize_t name(struct kobject *object, struct kobj_attribute *attr, const char *buffer, size_t count) \
    { \
        ssize_t result; \
        int guard = loop_rx_register_acquire(); \
        if (guard) return guard; \
        result = name##_locked(object, attr, buffer, count); \
        loop_rx_register_release(); \
        return result; \
    }
RX_SHOW(status_show)
RX_SHOW(input_show)
RX_SHOW(video_show)
RX_SHOW(link_show)
RX_STORE(start_store)
RX_STORE(video_store)
RX_STORE(settle_store)
RX_STORE(pattern_store)
RX_STORE(vendor_video_store)
#undef RX_SHOW
#undef RX_STORE

static ssize_t status_show_locked(struct kobject *object, struct kobj_attribute *attr,
                           char *buffer)
{
    u32 tmds_hz = hrx_get_tmds_clk(&loop_receiver);
    u32 core_id = readl(receiver + HDMI_CORE_ID);
    u32 status = readl(receiver + HDMI_MAINUNIT_STATUS);
    u32 horizontal = readl(receiver + HDMI_VMON_STATUS3);
    u32 vertical = readl(receiver + HDMI_VMON_STATUS6);
    u32 deframer = readl(receiver + HDMI_DEFRAMER_STATUS);
    return scnprintf(buffer, PAGE_SIZE,
        "core_id=%#x mainunit=%#x tmds_valid=%u hactive=%u htotal=%u vactive=%u vtotal=%u deframer=%#x tmds_hz=%u\n",
        core_id, status, !!(status & HDMI_MAINUNIT_STATUS_TMDSVALID_STABLE),
        horizontal & 0xffff, horizontal >> 16, vertical & 0xffff, vertical >> 16,
        deframer, tmds_hz);
}
static ssize_t start_store_locked(struct kobject *object, struct kobj_attribute *attr,
                           const char *buffer, size_t count)
{
    int result;
    bool rate6g;
    if (sysfs_streq(buffer, "1")) rate6g = false;
    else if (sysfs_streq(buffer, "6g")) rate6g = true;
    else return -EINVAL;
    result = loop_rx_start_rate(rate6g);
    pr_info("loop-rx: controller/PHY initialization result=%d\n", result);
    return result ? result : count;
}
static ssize_t input_show_locked(struct kobject *object, struct kobj_attribute *attr,
                          char *buffer)
{
    return loop_rx_input(buffer);
}
static ssize_t video_show_locked(struct kobject *object, struct kobj_attribute *attr, char *buffer)
{
    T32HDMI_RX_WRAP_HDMI_CONTROLLER_STATUS wrapper = {
        .reg = readl(loop_receiver.wrapper + RA_HDMI_RX_WRAP_HDMI_CONTROLLER_STATUS),
    };
    return scnprintf(buffer, PAGE_SIZE,
        "config1=%#x config2=%#x control=%#x cd=%#x pp=%#x "
        "wrapper=%#x output_mute=%u output_valid=%u output_depth=%u "
        "output_format=%u config_mute=%u hdcp14=%#x hdcp22=%#x hdcp14_config=%#x hdcp22_config=%#x\n",
        hrx_reg_read(&loop_receiver, HDMI_VIDEO_CONFIG1),
        hrx_reg_read(&loop_receiver, HDMI_VIDEO_CONFIG2),
        hrx_reg_read(&loop_receiver, HDMI_VIDEO_CONTROL),
        hrx_reg_read(&loop_receiver, HDMI_VIDEO_CD_CONFIG),
        hrx_reg_read(&loop_receiver, HDMI_VIDEO_PP_CONFIG), wrapper.reg,
        wrapper.uHDMI_CONTROLLER_STATUS_oavmute,
        wrapper.uHDMI_CONTROLLER_STATUS_tmdsvalid,
        wrapper.uHDMI_CONTROLLER_STATUS_opvo_color_depth,
        wrapper.uHDMI_CONTROLLER_STATUS_opvo_format,
        wrapper.uHDMI_CONTROLLER_STATUS_ocfg_avmute_sts,
        hrx_reg_read(&loop_receiver, HDMI_HDCP14_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_HDCP22_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_HDCP14_CONFIG),
        hrx_reg_read(&loop_receiver, HDMI_HDCP22_CONFIG));
}
static ssize_t video_store_locked(struct kobject *object, struct kobj_attribute *attr,
                           const char *buffer, size_t count)
{
    int result;
    if (!sysfs_streq(buffer, "1")) return -EINVAL;
    result = loop_rx_video_config();
    return result ? result : count;
}
static ssize_t link_show_locked(struct kobject *object, struct kobj_attribute *attr,
                         char *buffer)
{
    u32 locks = hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS3);
    return scnprintf(buffer, PAGE_SIZE,
        "scdc_lock=%#x clock=%u channels=%#x errors01=%#x errors2=%#x "
        "phy=%#x bch_acc=%#x bch_field=%#x psvg0=%#x psvg1=%#x "
        "global_enable=%#x descrambler_control=%#x descrambler_status=%#x "
        "scdc_status1=%#x phy_config=%#x cmu_count=%u cmu_status=%#x\n",
        locks, !!(locks & HDMI_SCDC_REGBANK_STATUS3_CLOCKDETECTED),
        (locks >> 1) & 7,
        hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS4),
        hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS5),
        hrx_reg_read(&loop_receiver, HDMI_PHY_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_PKTEX_BCHERR_ACC_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_PKTEX_FIELD_BCHERR_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_PSVG_CONFIG0),
        hrx_reg_read(&loop_receiver, HDMI_PSVG_CONFIG1),
        hrx_reg_read(&loop_receiver, HDMI_GLOBAL_SWENABLE),
        hrx_reg_read(&loop_receiver, HDMI_DESCRAND_EN_CONTROL),
        hrx_reg_read(&loop_receiver, HDMI_DESCRAND_STATUS),
        hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS1),
        hrx_reg_read(&loop_receiver, HDMI_PHY_CONFIG),
        hrx_reg_read(&loop_receiver, HDMI_CMU_TMDSQP_COUNT),
        hrx_reg_read(&loop_receiver, HDMI_CMU_STATUS));
}
static struct kobj_attribute link_attribute = __ATTR_RO(link);
static ssize_t settle_store_locked(struct kobject *object, struct kobj_attribute *attr,
                            const char *buffer, size_t count)
{
    int result;
    if (!sysfs_streq(buffer, "3")) return -EINVAL;
    result = loop_rx_wait_frames(3);
    return result ? result : count;
}
static struct kobj_attribute settle_attribute = __ATTR_WO(settle);
static ssize_t pattern_store_locked(struct kobject *object, struct kobj_attribute *attr,
                             const char *buffer, size_t count)
{
    int result;
    if (sysfs_streq(buffer, "0")) {
        loop_rx_pattern_restore();
        return count;
    }
    if (!sysfs_streq(buffer, "1")) return -EINVAL;
    result = loop_rx_pattern_enable();
    return result ? result : count;
}
static struct kobj_attribute pattern_attribute = __ATTR_WO(pattern);
static ssize_t vendor_video_store_locked(struct kobject *object, struct kobj_attribute *attr,
                                 const char *buffer, size_t count)
{
    if (sysfs_streq(buffer, "0")) loop_rx_vendor_video_restore();
    else if (sysfs_streq(buffer, "legacy")) loop_rx_vendor_video_enable(false);
    else if (sysfs_streq(buffer, "1")) loop_rx_vendor_video_enable(true);
    else return -EINVAL;
    return count;
}
static struct kobj_attribute vendor_video_attribute = __ATTR_WO(vendor_video);
static struct kobj_attribute video_attribute = __ATTR_RW(video);
static struct kobj_attribute input_attribute = __ATTR_RO(input);
static struct kobj_attribute start_attribute = __ATTR_WO(start);
static struct kobj_attribute status_attribute = __ATTR_RO(status);
static int __init receiver_init(void)
{
    struct device_node *node = of_find_node_by_path("/soc/hrx@f74a0000");
    int result;
    if (!node)
        return -ENODEV;
    result = of_address_to_resource(node, 0, &resource);
    of_node_put(node);
    if (result)
        return result;
    if (resource_size(&resource) < HDMI_VMON_STATUS6 + sizeof(u32))
        return -EINVAL;
    if (!request_mem_region(resource.start, resource_size(&resource), "loop-rx-status"))
        return -EBUSY;
    receiver = ioremap(resource.start, resource_size(&resource));
    if (!receiver) {
        result = -ENOMEM;
        goto release;
    }
    loop_receiver.receiver = receiver;
    /* Dolphin A0 wrapper base from vendor avio_memmap.h. */
    loop_receiver.wrapper = ioremap(0xf74ad800, 0x100);
    loop_receiver.phy_cfg_clk = 100;
    if (!loop_receiver.wrapper) {
        result = -ENOMEM;
        goto unmap;
    }
    status_object = kobject_create_and_add("vs680_rx_status", kernel_kobj);
    if (!status_object) {
        result = -ENOMEM;
        goto unmap;
    }
    result = sysfs_create_file(status_object, &status_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &start_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &input_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &video_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &link_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &settle_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &pattern_attribute.attr);
    if (!result)
        result = sysfs_create_file(status_object, &vendor_video_attribute.attr);
    if (!result)
        result = loop_rx_transaction_attach(status_object);
    if (!result)
        return 0;
    kobject_put(status_object);
unmap:
    if (loop_receiver.wrapper) iounmap(loop_receiver.wrapper);
    iounmap(receiver);
release:
    release_mem_region(resource.start, resource_size(&resource));
    return result;
}
static void __exit receiver_exit(void)
{
    kobject_put(status_object);
    loop_rx_maintenance_detach();
    loop_rx_pattern_restore();
    loop_rx_vendor_video_restore();
    if (loop_receiver.wrapper) iounmap(loop_receiver.wrapper);
    iounmap(receiver);
    release_mem_region(resource.start, resource_size(&resource));
}
module_init(receiver_init);
module_exit(receiver_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HDMI RX PHY initialization and measured input contract");

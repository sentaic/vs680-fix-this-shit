// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include "rx-transaction.h"

/* 该模块参数启用手动 RX 事务接口。 */
static bool transaction_debug;
module_param(transaction_debug, bool, 0);
MODULE_PARM_DESC(transaction_debug, "Enable experimental RX transaction endpoints");

static int parse_attempt(const char *buffer, size_t count, u64 *attempt)
{
    u64 value = 0;
    size_t i, length = count;
    if (!count || count > 21) return -EINVAL;
    if (buffer[length - 1] == '\n') length--;
    if (!length || length > 20 || buffer[0] == '0') return -EINVAL;
    for (i = 0; i < length; i++) {
        unsigned digit;
        if (buffer[i] < '0' || buffer[i] > '9') return -EINVAL;
        digit = buffer[i] - '0';
        if (value > (U64_MAX - digit) / 10) return -ERANGE;
        value = value * 10 + digit;
    }
    *attempt = value;
    return 0;
}

static ssize_t transaction_contract_show(struct kobject *object,
    struct kobj_attribute *attribute, char *buffer)
{
    if (!transaction_debug) return -EOPNOTSUPP;
    return scnprintf(buffer, PAGE_SIZE, "rx-prepare-v2-two-phase-dev\n");
}

static ssize_t transaction_status_show(struct kobject *object,
    struct kobj_attribute *attribute, char *buffer)
{
    return loop_rx_transaction_status(buffer);
}

static ssize_t transaction_prelink_store(struct kobject *object,
    struct kobj_attribute *attribute, const char *buffer, size_t count)
{
    int result;
    bool rate6g;
    if (!transaction_debug) return -EOPNOTSUPP;
    if (sysfs_streq(buffer, "1")) rate6g = false;
    else if (sysfs_streq(buffer, "6g")) rate6g = true;
    else return -EINVAL;
    result = loop_rx_transaction_prelink_rate(rate6g);
    if (result) return result;
    return (ssize_t)count;
}

static ssize_t transaction_command(const char *buffer, size_t count,
                                    int (*command)(u64))
{
    u64 attempt;
    int result;
    if (!transaction_debug) return -EOPNOTSUPP;
    result = parse_attempt(buffer, count, &attempt);
    if (result) return result;
    result = command(attempt);
    /* 同步操作完成后返回写入长度，poll 结果通过状态接口读取。 */
    if (result) return result;
    return (ssize_t)count;
}

static ssize_t transaction_begin_store(struct kobject *object,
    struct kobj_attribute *attribute, const char *buffer, size_t count)
{
    return transaction_command(buffer, count, loop_rx_transaction_begin);
}
static ssize_t transaction_poll_store(struct kobject *object,
    struct kobj_attribute *attribute, const char *buffer, size_t count)
{
    return transaction_command(buffer, count, loop_rx_transaction_poll);
}
static ssize_t transaction_release_store(struct kobject *object,
    struct kobj_attribute *attribute, const char *buffer, size_t count)
{
    return transaction_command(buffer, count, loop_rx_transaction_release);
}

static ssize_t maintenance_status_show(struct kobject *object,
    struct kobj_attribute *attribute, char *buffer)
{
    if (!transaction_debug) return -EOPNOTSUPP;
    return loop_rx_maintenance_status(buffer);
}

static ssize_t discard_failed_store(struct kobject *object,
    struct kobj_attribute *attribute, const char *buffer, size_t count)
{
    u64 generation, attempt;
    size_t i;
    int result;
    if (!transaction_debug) return -EOPNOTSUPP;
    if (!count || count > 42) return -EINVAL;
    for (i = 0; i < count && buffer[i] != ' '; i++);
    if (i == count || i > 20 || memchr(buffer, '\n', i)) return -EINVAL;
    result = parse_attempt(buffer, i, &generation);
    if (result) return result;
    result = parse_attempt(buffer + i + 1, count - i - 1, &attempt);
    if (result) return result;
    result = loop_rx_discard_failed(generation, attempt);
    return result ? result : (ssize_t)count;
}

static struct kobj_attribute contract_attribute = __ATTR_RO(transaction_contract);
static struct kobj_attribute status_attribute = __ATTR_RO(transaction_status);
static struct kobj_attribute prelink_attribute = __ATTR_WO(transaction_prelink);
static struct kobj_attribute begin_attribute = __ATTR_WO(transaction_begin);
static struct kobj_attribute poll_attribute = __ATTR_WO(transaction_poll);
static struct kobj_attribute release_attribute = __ATTR_WO(transaction_release);
static struct kobj_attribute maintenance_attribute = __ATTR_RO(maintenance_status);
static struct kobj_attribute discard_attribute = __ATTR_WO(discard_failed);
static struct attribute *transaction_attributes[] = {
    &contract_attribute.attr, &status_attribute.attr, &prelink_attribute.attr,
    &begin_attribute.attr, &poll_attribute.attr, &release_attribute.attr,
    &maintenance_attribute.attr, &discard_attribute.attr, NULL
};
static const struct attribute_group transaction_group = {
    .attrs = transaction_attributes,
};

int loop_rx_transaction_attach(struct kobject *object)
{
    int result = loop_rx_maintenance_init();
    return result ? result : sysfs_create_group(object, &transaction_group);
}

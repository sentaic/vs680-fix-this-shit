// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/version.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#ifndef RX_CAPTURE_CMA_BEGIN
#define RX_CAPTURE_CMA_BEGIN 0x75400000u
#endif
#ifndef RX_CAPTURE_CMA_END
#define RX_CAPTURE_CMA_END 0x76400000u
#endif
struct buffer_map { u32 fd, reserved; u64 address, length; };
struct held_buffer {
    struct dma_buf *buffer;
    struct dma_buf_attachment *attachment;
    struct sg_table *table;
    struct list_head orphan;
};
struct buffer_context {
    struct mutex lock;
    struct held_buffer *held;
    struct device *dma_device;
    u64 begin, end;
};
static struct miscdevice device;
static struct miscdevice display_pool_device;
static LIST_HEAD(orphan_buffers);
static DEFINE_MUTEX(orphan_lock);
static u64 dma_mask = DMA_BIT_MASK(32);
static void release_buffer(struct held_buffer *held)
{
    if (held->table) dma_buf_unmap_attachment(held->attachment, held->table, DMA_BIDIRECTIONAL);
    if (held->attachment) dma_buf_detach(held->buffer, held->attachment);
    if (held->buffer) dma_buf_put(held->buffer);
    kfree(held);
}
static long map_ioctl_locked(struct buffer_context *context, unsigned int command, unsigned long argument)
{
    struct buffer_map request;
    struct held_buffer *held;
    struct scatterlist *entry;
    u64 end, total = 0;
    unsigned i;
    int result = -EINVAL;
    if (command == _IO('B', 1)) {
        held = context->held;
        if (!held) return -EINVAL;
        /* TA 确认 DMA 停止后，由协调程序请求释放缓冲。 */
        context->held = NULL;
        release_buffer(held);
        module_put(THIS_MODULE);
        return 0;
    }
    if (command != _IOWR('B', 0, struct buffer_map)) return -ENOTTY;
    if (context->held) return -EBUSY;
    if (copy_from_user(&request, (void __user *)argument, sizeof(request))) return -EFAULT;
    if (request.reserved) return -EINVAL;
    held = kzalloc(sizeof(*held), GFP_KERNEL);
    if (!held) return -ENOMEM;
    held->buffer = dma_buf_get(request.fd);
    if (IS_ERR(held->buffer)) { result = PTR_ERR(held->buffer); held->buffer = NULL; goto fail; }
    held->attachment = dma_buf_attach(held->buffer, context->dma_device);
    if (IS_ERR(held->attachment)) { result = PTR_ERR(held->attachment); held->attachment = NULL; goto fail; }
#ifdef HDMI_CACHED_DISPLAY_POOL_ONLY
    /* 此设备固定物理池，TA 负责写入命令和像素并清理缓存；no-map 预留区使用 DMA_ATTR_SKIP_CPU_SYNC。 */
    held->attachment->dma_map_attrs |= DMA_ATTR_SKIP_CPU_SYNC;
#endif
    held->table = dma_buf_map_attachment(held->attachment, DMA_BIDIRECTIONAL);
    if (IS_ERR(held->table)) { result = PTR_ERR(held->table); held->table = NULL; goto fail; }
    request.address = sg_dma_address(held->table->sgl);
    if (request.address < context->begin || request.address >= context->end) goto fail;
    end = request.address;
    for_each_sg(held->table->sgl, entry, held->table->nents, i) {
        if (sg_dma_address(entry) != end || sg_dma_len(entry) > context->end - end) goto fail;
        total += sg_dma_len(entry); end += sg_dma_len(entry);
    }
    if (total < held->buffer->size) goto fail;
    request.length = held->buffer->size;
    if (copy_to_user((void __user *)argument, &request, sizeof(request))) { result = -EFAULT; goto fail; }
    __module_get(THIS_MODULE);
    context->held = held;
    return 0;
fail:
    release_buffer(held);
    return result;
}
static long map_ioctl(struct file *file, unsigned int command, unsigned long argument)
{
    struct buffer_context *context = file->private_data;
    long result;
    /* dup/fork 句柄共享同一个导入状态，因此串行处理导入和释放。 */
    mutex_lock(&context->lock);
    result = map_ioctl_locked(context, command, argument);
    mutex_unlock(&context->lock);
    return result;
}
static int buffer_open(struct inode *inode, struct file *file)
{
    struct miscdevice *opened = file->private_data;
    struct buffer_context *context = kzalloc(sizeof(*context), GFP_KERNEL);
    if (!context) return -ENOMEM;
    if (opened != &device && opened != &display_pool_device) {
        kfree(context);
        return -EINVAL;
    }
    context->dma_device = opened->this_device;
#ifdef HDMI_CACHED_DISPLAY_POOL_ONLY
    context->begin=0x6fc00000;context->end=0x71c00000;
#else
    context->begin = opened == &device ? RX_CAPTURE_CMA_BEGIN : 0x76400000;
    context->end = opened == &device ? RX_CAPTURE_CMA_END : 0x76e00000;
#endif
    mutex_init(&context->lock);
    file->private_data = context;
    return 0;
}
static int buffer_close(struct inode *inode, struct file *file)
{
    struct buffer_context *context = file->private_data;
    if (context->held) {
        mutex_lock(&orphan_lock);
        list_add_tail(&context->held->orphan, &orphan_buffers);
        mutex_unlock(&orphan_lock);
        pr_warn("capture-buffer: owner closed without confirmed DMA stop; allocation retained until reset\n");
    }
    file->private_data = NULL;
    kfree(context);
    return 0;
}
static const struct file_operations operations = {
    .owner = THIS_MODULE, .open = buffer_open, .release = buffer_close,
    .unlocked_ioctl = map_ioctl,
};
static struct miscdevice device = {
    .minor = MISC_DYNAMIC_MINOR, .name = "vs680-capture-buffer", .fops = &operations, .mode = 0600,
};
static struct miscdevice display_pool_device = {
    .minor = MISC_DYNAMIC_MINOR,
#ifdef HDMI_CACHED_DISPLAY_POOL_ONLY
    .name = "vs680-display-cached-pool-buffer",
#else
    .name = "vs680-display-pool-buffer",
#endif
    .fops = &operations, .mode = 0600,
};
static void buffer_set_dma_mask(struct miscdevice *target)
{
    target->this_device->dma_mask = &dma_mask;
    target->this_device->coherent_dma_mask = DMA_BIT_MASK(32);
}
static int __init buffer_init(void)
{
#ifdef HDMI_CACHED_DISPLAY_POOL_ONLY
    int result=misc_register(&display_pool_device);
    if (!result) buffer_set_dma_mask(&display_pool_device);
    return result;
#else
    int result = misc_register(&device);
    if (result) return result;
    buffer_set_dma_mask(&device);
    result = misc_register(&display_pool_device);
    if (result) {
        misc_deregister(&device);
        return result;
    }
    buffer_set_dma_mask(&display_pool_device);
    return 0;
#endif
}
static void __exit buffer_exit(void)
{
    misc_deregister(&display_pool_device);
#ifndef HDMI_CACHED_DISPLAY_POOL_ONLY
    misc_deregister(&device);
#endif
}
module_init(buffer_init); module_exit(buffer_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hold a contiguous CMA DMA buffer for HDMI loop capture");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 16, 0)
MODULE_IMPORT_NS(DMA_BUF);
#endif

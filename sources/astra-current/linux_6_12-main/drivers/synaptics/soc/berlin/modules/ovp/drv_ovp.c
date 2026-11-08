// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2026 Synaptics Incorporated */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/proc_fs.h>
#include <linux/io.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <linux/platform_device.h>

#include <linux/of_platform.h>
#include <linux/of_device.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/arm-smccc.h>

/*************************************************************************
 * Local header files
 */
#include "kernel_compatibility.h"
#include "ovp_debug.h"
#define _DRV_OVP_C_
#include "drv_ovp.h"
#include "drv_ovp_wrap.h"
#include <soc/berlin/berlin_sip.h>
#include "tee_ca_ovp.h"

typedef enum _tz_secure_reg_ {
	TZ_REG_SEM_INTR_ENABLE_1	=   SEM_INTR_ENABLE_1,
	TZ_REG_SEM_INTR_ENABLE_2	=   SEM_INTR_ENABLE_2,
	TZ_REG_SEM_INTR_ENABLE_3	=   SEM_INTR_ENABLE_3,
	TZ_REG_SEM_CHK_FULL		=   SEM_CHK_FULL,
	TZ_REG_SEM_POP			=   SEM_POP,
	TZ_REG_OVP_INTR_STATUS		=   OVP_INTSTATUS,
	TZ_REG_MAX,
} tz_secure_reg;

/***********************************************************************
 * Module Variable
 */
#define OVP_NODE_NAME		"syna,berlin-ovp"
#define OVP_INTR_NAME		"ovp_intr"
#define OVP_DEVICE_NAME		"ovp"
#define OVP_DEVICE_PATH		("/dev/" OVP_DEVICE_NAME)
#define OVP_MAX_DEVS		8
#define OVP_MINOR		0

#define OVP_ENABLE_FASTCALL_FOR_REG_ACCESS

static atomic_t ovp_dev_refcnt = ATOMIC_INIT(0);

/**********************************************************************
 * Module API
 */
#ifdef OVP_ENABLE_FASTCALL_FOR_REG_ACCESS

static int wrap_tz_secure_reg_rw(tz_secure_reg reg,
				 u32 ops, u32 *value)
{
	if (reg >= 0 && reg < TZ_REG_MAX) {
		struct arm_smccc_res res = {};
		u32 reg_val = *value;

		/* Valid register, so access it */
		arm_smccc_smc(SYNA_SIP_SMC64_SREGISTER_OP,
			      ops, reg, reg_val,
			      0, 0, 0, 0,
			      &res);
		if (res.a0) {
			ovp_trace("%s:%d:ERR:  reg:%x, ops:%d, val:%x, ret:0x%lx/%ld\n",
				  __func__, __LINE__, reg, ops, *value, res.a0, res.a0);
			return -EIO;
		} else if (ops == SYNA_SREGISTER_READ) {
			*value = res.a1;
		}
	} else {
		/* Invalid register, log error */
		ovp_trace("%s:%d:INVALID: reg:%x, ops:%d, val:%x\n",
			  __func__, __LINE__, reg, ops, *value);
		return -EINVAL;
	}

	return 0;
}

static tz_secure_reg ovp_tz_phy_to_secure_reg(unsigned int reg_addr)
{
	tz_secure_reg secure_reg = TZ_REG_MAX;

	if (reg_addr == OVP_INTR_STS)
		secure_reg = TZ_REG_OVP_INTR_STATUS;

	return secure_reg;
}

int ovp_wrap_register_read(unsigned int reg_addr, unsigned int *ptr_reg_val)
{
	tz_secure_reg secure_reg = ovp_tz_phy_to_secure_reg(reg_addr);

	if (!ptr_reg_val)
		return -EINVAL;

	return wrap_tz_secure_reg_rw(secure_reg, SYNA_SREGISTER_READ, ptr_reg_val);
}

int ovp_wrap_register_write(unsigned int reg_addr, unsigned int reg_val)
{
	tz_secure_reg secure_reg = ovp_tz_phy_to_secure_reg(reg_addr);

	return wrap_tz_secure_reg_rw(secure_reg, SYNA_SREGISTER_WRITE, &reg_val);
}

#else /* !OVP_ENABLE_FASTCALL_FOR_REG_ACCESS */

int ovp_wrap_register_read(unsigned int reg_addr, unsigned int *ptr_reg_val)
{
	if (!ptr_reg_val || !ovp_ctx.ovp_virt_base) {
		pr_err("OVP virt_base not initialized or invalid param\n");
		return -EINVAL;
	}

	*ptr_reg_val = readl_relaxed(ovp_ctx.ovp_virt_base + reg_addr);

	return 0;
}

int ovp_wrap_register_write(unsigned int reg_addr, unsigned int reg_val)
{
	if (!ovp_ctx.ovp_virt_base) {
		pr_err("OVP virt_base not initialized\n");
		return -EINVAL;
	}

	writel_relaxed(reg_val, ovp_ctx.ovp_virt_base + reg_addr);

	return 0;
}

#endif /* !OVP_ENABLE_FASTCALL_FOR_REG_ACCESS */

static int ovp_device_init(struct ovp_device_t *ovp_dev, unsigned int user)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	unsigned int err;

	mutex_init(&ptr_ovp_ctx->ovp_mutex);

	sema_init(&ptr_ovp_ctx->ovp_sem, 0);
	spin_lock_init(&ptr_ovp_ctx->ovp_msg_spinlock);

	err = AMPMsgQ_Init(&ptr_ovp_ctx->h_ovp_msg_q, OVP_ISR_MSGQ_SIZE);
	if (unlikely(err != S_OK)) {
		ovp_error("%s: h_ovp_msg_q init failed, err:%8x\n", __func__, err);
		return -ENOMEM;
	}

	ovp_trace("%s ok\n", __func__);

	return S_OK;
}

static int ovp_device_exit(struct ovp_device_t *ovp_dev, unsigned int user)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	unsigned int err;

	err = AMPMsgQ_Destroy(&ptr_ovp_ctx->h_ovp_msg_q);
	if (unlikely(err != S_OK)) {
		ovp_error("ovp MsgQ Destroy: failed, err:%8x\n", err);
		return -EIO;
	}
	ovp_trace("%s ok\n", __func__);

	return S_OK;
}

static int ovp_drv_open(struct inode *inode, struct file *filp)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	struct ovp_device_t *dev = NULL;
	int err = 0;

	mutex_lock(&ptr_ovp_ctx->ovp_mutex);

	ovp_trace("Start open ovp driver!\n");

	if (atomic_inc_return(&ovp_dev_refcnt) > 1) {
		ovp_trace("ovp driver reference count %d!\n",
			  atomic_read(&ovp_dev_refcnt));
		atomic_dec(&ovp_dev_refcnt);
		err = -EBUSY;
		goto err_exit;
	}

	dev = container_of(inode->i_cdev, struct ovp_device_t, cdev);
	filp->private_data = dev;

	err = wrap_ovp_drv_register_isr();
	if (err  < 0) {
		ovp_error("ovp driver register isr failed %d!\n", err);
		atomic_dec(&ovp_dev_refcnt);
		goto err_exit;
	}

	err = syna_ovpd_ca_initialize();
	if (err < 0) {
		ovp_error("ovp driver CA init failed %d!\n", err);
		free_irq(ptr_ovp_ctx->irq_num, (void *)ptr_ovp_ctx);
		atomic_dec(&ovp_dev_refcnt);
		goto err_exit;
	}

err_exit:
	mutex_unlock(&ptr_ovp_ctx->ovp_mutex);

	return err;
}

static int ovp_drv_release(struct inode *inode, struct file *filp)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;

	mutex_lock(&ptr_ovp_ctx->ovp_mutex);

	if (atomic_read(&ovp_dev_refcnt) == 0) {
		ovp_trace("ovp driver already released!\n");
		goto err_exit;
	}

	if (atomic_dec_return(&ovp_dev_refcnt)) {
		ovp_trace("ovp dev ref cnt after this release: %d!\n",
			  atomic_read(&ovp_dev_refcnt));
		goto err_exit;
	}

	wrap_ovp_drv_free_isr();

	syna_ovpd_ca_deinitialize();

	ovp_trace("%s ok\n", __func__);

err_exit:
	mutex_unlock(&ptr_ovp_ctx->ovp_mutex);

	return 0;
}

static long ovp_drv_ioctl_unlocked(struct file *filp, unsigned int cmd, unsigned long arg)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	int ret;

	switch (cmd) {
	case OVP_IOCTL_GET_MSG:
	{
		CC_MSG_t msg = { 0 };

		ret = wrap_ovp_drv_get_isr_msg(&msg);
		if (ret < 0)
			return ret;

		if (copy_to_user
		    ((void __user *)arg, &msg, sizeof(CC_MSG_t)))
			return -EFAULT;
		break;
	}
	case OVP_IOCTL_INTR:
	{
		INTR_MSG ovp_intr_info = { 0, 0 };

		if (copy_from_user
		    (&ovp_intr_info, (void __user *)arg,
		     sizeof(INTR_MSG)))
			return -EFAULT;

		ret = wrap_ovp_drv_set_intr(&ovp_intr_info);
		if (ret < 0)
			return ret;
		break;
	}
	case OVP_IOCTL_CLKCONTROL:
	{
		bool bovp_clock_control;

		if (copy_from_user
			(&bovp_clock_control, (void __user *)arg,
				sizeof(bool)))
			return -EFAULT;

		ret = wrap_ovp_drv_clk_ctrl(bovp_clock_control);
		if (ret < 0)
			return ret;
		break;
	}
	case OVP_IOCTL_DUMMY_INTR:
	{
		up(&ptr_ovp_ctx->ovp_sem);

		break;
	}
	default:
	{
		pr_err("Invalid ioctl command: 0x%x\n", cmd);
		return -ENOTTY;
	}
	} /* Switch end */

	return 0;
}

/*********************************************************************
 * Module Register API
 */
static const struct file_operations ovp_ops = {
	.open = ovp_drv_open,
	.release = ovp_drv_release,
	.unlocked_ioctl = ovp_drv_ioctl_unlocked,
	.compat_ioctl = ovp_drv_ioctl_unlocked,
	.owner = THIS_MODULE,
};

static struct ovp_device_t ovp_device = {
	.dev_name = OVP_DEVICE_NAME,
	.minor = OVP_MINOR,
	.dev_init = ovp_device_init,
	.dev_exit = ovp_device_exit,
	.fops = &ovp_ops,
};

static const struct of_device_id ovp_match[] = {
	{
		.compatible = OVP_NODE_NAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, ovp_match);

static int ovp_drv_init(struct ovp_device_t *ovp_device)
{
	struct cdev *dev = &ovp_device->cdev;
	struct device *created_dev;
	int res;

	/* Now setup cdevs. */
	cdev_init(dev, ovp_device->fops);
	dev->owner = THIS_MODULE;
	res = cdev_add(dev, MKDEV(ovp_device->major, ovp_device->minor), 1);
	if (res) {
		ovp_error("ovp_driver_setup_cdev failed.\n");
		res = -ENODEV;
		goto err_add_device;
	}
	ovp_trace("setup cdevs device minor [%d]\n", ovp_device->minor);

	/* add PE devices to sysfs */
	ovp_device->dev_class =
		SYNA_CLASS_CREATE(ovp_device->dev_name);
	if (IS_ERR(ovp_device->dev_class)) {
		ovp_error("class_create failed.\n");
		res = PTR_ERR(ovp_device->dev_class);
		ovp_device->dev_class = NULL;
		goto err_add_device;
	}

	created_dev = device_create(ovp_device->dev_class, NULL,
				    MKDEV(ovp_device->major, ovp_device->minor), NULL,
				    ovp_device->dev_name);
	if (IS_ERR(created_dev)) {
		ovp_error("device_create failed.\n");
		res = PTR_ERR(created_dev);
		goto err_add_device;
	}
	ovp_trace("create device sysfs [%s]\n", ovp_device->dev_name);

	/* create hw device */
	if (ovp_device->dev_init) {
		res = ovp_device->dev_init(ovp_device, 0);
		if (res != 0) {
			ovp_error("ovp_int_init failed !!! res = 0x%08X\n",
				  res);
			res = -ENODEV;
			goto err_add_device;
		}
	}

	return 0;

err_add_device:
	if (!IS_ERR_OR_NULL(ovp_device->dev_class)) {
		device_destroy(ovp_device->dev_class,
			       MKDEV(ovp_device->major, ovp_device->minor));
		class_destroy(ovp_device->dev_class);
		ovp_device->dev_class = NULL;
	}

	cdev_del(&ovp_device->cdev);

	return res;
}

static int ovp_drv_exit(struct ovp_device_t *ovp_device)
{
	int res;

	ovp_trace("%s [%s] enter\n", __func__, ovp_device->dev_name);

	/* destroy kernel API */
	if (ovp_device->dev_exit) {
		res = ovp_device->dev_exit(ovp_device, 0);
		if (res != 0)
			ovp_error("%s failed !!! res = 0x%08X\n",
				  __func__, res);
	}

	if (!IS_ERR_OR_NULL(ovp_device->dev_class)) {
		/* del sysfs entries */
		device_destroy(ovp_device->dev_class,
			       MKDEV(ovp_device->major, ovp_device->minor));
		ovp_trace("delete device sysfs [%s]\n", ovp_device->dev_name);

		class_destroy(ovp_device->dev_class);
		ovp_device->dev_class = NULL;
	}
	/* del cdev */
	cdev_del(&ovp_device->cdev);

	return 0;
}

static int ovp_drv_read_cfg(OVP_CTX *ptr_ovp_ctx,
			    struct platform_device *pdev)
{
	ptr_ovp_ctx->irq_num =
		platform_get_irq_byname(pdev, OVP_INTR_NAME);

	if (ptr_ovp_ctx->irq_num <= 0) {
		ovp_error("Failed to get irq(%s) for OVP\n", OVP_INTR_NAME);
		return ptr_ovp_ctx->irq_num;
	}

	ovp_trace("%s:%d: irq - %s:%x\n",
		  __func__, __LINE__,
		  OVP_INTR_NAME, ptr_ovp_ctx->irq_num);

	return 0;
}

static int ovp_drv_create_devioremap(OVP_CTX *ptr_ovp_ctx,
				     struct platform_device *pdev)
{
	int ret = 0;

	ptr_ovp_ctx->ovp_virt_base = NULL;

	ptr_ovp_ctx->p_ovp_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!ptr_ovp_ctx->p_ovp_res) {
		ovp_error("Failed to get OVP memory resource\n");
		return -ENOENT;
	}

	ptr_ovp_ctx->ovp_base = ptr_ovp_ctx->p_ovp_res->start;
	ptr_ovp_ctx->ovp_size = resource_size(ptr_ovp_ctx->p_ovp_res);
	ptr_ovp_ctx->ovp_virt_base = devm_ioremap_resource(&pdev->dev, ptr_ovp_ctx->p_ovp_res);

	if (IS_ERR(ptr_ovp_ctx->ovp_virt_base)) {
		ovp_trace("Fail to map address before it is used!\n");
		ret = PTR_ERR(ptr_ovp_ctx->ovp_virt_base);
		ptr_ovp_ctx->ovp_virt_base = NULL;
	} else {
		ovp_trace("ioremap %s: vir_addr: %p, size: 0x%llx, phy_addr: 0x%llx!\n",
			  ret ? "failed" : "success",
		  ptr_ovp_ctx->ovp_virt_base,
		  (unsigned long long)ptr_ovp_ctx->ovp_size,
		  (unsigned long long)ptr_ovp_ctx->ovp_base);
	}

	return ret;
}

static int ovp_drv_probe(struct platform_device *pdev)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	int ret;
	dev_t pedev;

	ovp_device.dev = &pdev->dev;

	/* Read/Parse configuration for all modules from DTS file */
	ret = ovp_drv_read_cfg(ptr_ovp_ctx, pdev);
	if (ret < 0) {
		ovp_error("OVP config read fail!\n");
		goto err_fail1;
	}

	/* Map only OVP space for direct register access */
	ret = ovp_drv_create_devioremap(ptr_ovp_ctx, pdev);
	if (ret < 0) {
		ovp_error("OVP ioremap fail!\n");
		goto err_fail1;
	}

	ptr_ovp_ctx->ovp_clk = devm_clk_get(ovp_device.dev, OVP_MODULE_CLK);
	if (IS_ERR(ptr_ovp_ctx->ovp_clk))
		return PTR_ERR(ptr_ovp_ctx->ovp_clk);

	ret = clk_prepare_enable(ptr_ovp_ctx->ovp_clk);
	if (ret < 0) {
		ovp_error("%s prepare failed..!\n", OVP_MODULE_CLK);
		goto err_fail2;
	}

	ret = alloc_chrdev_region(&pedev, 0, OVP_MAX_DEVS, OVP_DEVICE_NAME);
	if (ret < 0) {
		ovp_error("alloc_chrdev_region() failed for ovp\n");
		goto err_fail2;
	}
	ovp_device.major = MAJOR(pedev);
	ovp_trace("register cdev device major [%d]\n", ovp_device.major);

	ret = ovp_drv_init(&ovp_device);
	if (ret)
		goto err_drv_init;

	ovp_trace("%s OK\n", __func__);

	return 0;

err_drv_init:
	unregister_chrdev_region(MKDEV(ovp_device.major, 0), OVP_MAX_DEVS);
err_fail2:
	clk_disable_unprepare(ptr_ovp_ctx->ovp_clk);
err_fail1:
	ovp_trace("%s failed !!! (%d)\n", __func__, ret);

	return ret;
}

static RET_TYPE ovp_drv_remove(struct platform_device *pdev)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;

	ovp_drv_exit(&ovp_device);

	unregister_chrdev_region(MKDEV(ovp_device.major, 0), OVP_MAX_DEVS);
	ovp_trace("unregister cdev device major [%d]\n", ovp_device.major);
	ovp_device.major = 0;

	if (!IS_ERR_OR_NULL(ptr_ovp_ctx->ovp_clk))
		clk_disable_unprepare(ptr_ovp_ctx->ovp_clk);

	ovp_trace("%s OK\n", __func__);

	RETURN_VALUE;
}

static void ovp_drv_disable_irq(void)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;

	/* disable OVP interrupt */
	disable_irq_nosync(ptr_ovp_ctx->irq_num);
}

static void ovp_drv_shutdown(struct platform_device *pdev)
{
	if (atomic_read(&ovp_dev_refcnt))
		ovp_drv_disable_irq();
}

#ifdef CONFIG_PM_SLEEP
static void ovp_drv_enable_irq(void)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;

	/* disable OVP interrupt */
	enable_irq(ptr_ovp_ctx->irq_num);
}

static int ovp_drv_suspend(struct device *dev)
{
	int ret = 0;

	if (!atomic_read(&ovp_dev_refcnt))
		return ret;

	ovp_drv_disable_irq();

	/* Invoke syna_ovpd_ca_suspend with optimize false
	 * this requests "Full suspend from Kernel" in the TA
	 */
	ret = syna_ovpd_ca_suspend(false);
	if (ret) {
		ovp_error("%s OVP Suspend failed\n", __func__);
		/*
		 * Kernel will stop suspend and resume other modules back.
		 * so enable irq
		 */
		ovp_drv_enable_irq();
	}

	return ret;
}

static int ovp_drv_resume(struct device *dev)
{
	int ret = 0;

	if (!atomic_read(&ovp_dev_refcnt))
		return ret;

	/* Invoke syna_ovpd_ca_resume with optimize false
	 * this requests "Full resume from Kernel" in the TA
	 */
	ret = syna_ovpd_ca_resume(false);
	if (ret) {
		ovp_error("%s OVP Resume failed\n", __func__);
		return ret;
	}

	ovp_drv_enable_irq();

	return ret;
}
#endif /* CONFIG_PM_SLEEP */

static SIMPLE_DEV_PM_OPS(ovp_pmops, ovp_drv_suspend, ovp_drv_resume);

static struct platform_driver ovp_driver = {
	.probe = ovp_drv_probe,
	.remove = ovp_drv_remove,
	.shutdown = ovp_drv_shutdown,
	.driver = {
		.name = OVP_DEVICE_NAME,
		.of_match_table = ovp_match,
		.pm = &ovp_pmops,
	},
};
module_platform_driver(ovp_driver);

MODULE_AUTHOR("synaptics");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OVP module driver");

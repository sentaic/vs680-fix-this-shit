/* SPDX-License-Identifier: GPL-2.0
 * Copyright (C) 2026 Synaptics Incorporated
 */

#ifndef _OVP_DRIVER_H_
#define _OVP_DRIVER_H_
#include <linux/version.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>

#include "drv_msg.h"
#include "ovp_ioctl.h"
#include "tee_ca_ovp.h"

#define OVP_ISR_MSGQ_SIZE	8
#define OVP_CC_MSG		0x00
#define OVP_MODULE_NAME         "ovp_module"
#define OVP_MODULE_CLK		"ovp_coreclk"
#define OVP_INTR_STS		0x40

typedef struct _OVP_CONTEXT_ {
	unsigned int ovp_intr_status;
	AMPMsgQ_t h_ovp_msg_q;
	spinlock_t ovp_msg_spinlock;
	struct semaphore ovp_sem;
	/* mutex to protect ovp  device open/close */
	struct mutex ovp_mutex;

	int irq_num;

	struct resource *p_ovp_res;
	resource_size_t ovp_base;
	resource_size_t ovp_size;
	void *ovp_virt_base;

	struct clk *ovp_clk;
} OVP_CTX;

struct ovp_device_t {
	unsigned char *dev_name;
	struct cdev cdev;
	struct device *dev;
	struct class *dev_class;
	const struct file_operations *fops;

	int major;
	int minor;

	struct proc_dir_entry *dev_procdir;
	void *private_data;

	int (*dev_init)(struct ovp_device_t *dev, unsigned int flags);
	int (*dev_exit)(struct ovp_device_t *dev, unsigned int flags);
};

#ifdef _DRV_OVP_C_
OVP_CTX ovp_ctx;
#else
extern OVP_CTX ovp_ctx;
#endif

int ovp_wrap_register_write(unsigned int reg_addr, unsigned int reg_val);
int ovp_wrap_register_read(unsigned int reg_addr, unsigned int *ptr_reg_val);

#endif /* _OVP_DRIVER_H_ */

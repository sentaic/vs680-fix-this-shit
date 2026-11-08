// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2026 Synaptics Incorporated */

#include "drv_ovp.h"
#include "drv_ovp_wrap.h"
#include "ovp_debug.h"
#include <linux/irq.h>

static irqreturn_t wrap_ovp_drv_isr(int irq, void *dev_id)
{
	OVP_CTX *ptr_ovp_ctx = (OVP_CTX *)dev_id;
	unsigned int ovp_intr = 0;
	unsigned int intr_enabled;
	int ret = S_OK;
	unsigned long flags;

	/* Read interrupt status */
	ret = ovp_wrap_register_read(OVP_INTR_STS, &ovp_intr);
	if (ret < 0) {
		ovp_error("%s: failed to read interrupt status (%d)\n", __func__, ret);
		return IRQ_HANDLED;
	}

	/* Clear all interrupts */
	ret = ovp_wrap_register_write(OVP_INTR_STS, 0x3F);
	if (ret < 0)
		ovp_error("%s: failed to clear interrupt status (%d)\n", __func__, ret);

	spin_lock_irqsave(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
	intr_enabled = ptr_ovp_ctx->ovp_intr_status;
	spin_unlock_irqrestore(&ptr_ovp_ctx->ovp_msg_spinlock, flags);

	if (ovp_intr && intr_enabled) {
		CC_MSG_t msg = { OVP_CC_MSG, ovp_intr };

		spin_lock_irqsave(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
		ret = AMPMsgQ_Add(&ptr_ovp_ctx->h_ovp_msg_q, &msg);
		spin_unlock_irqrestore(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
		if (ret == S_OK)
			up(&ptr_ovp_ctx->ovp_sem);
	} else if (ovp_intr) {
		ovp_trace("ISR received but interrupts disabled, intr=0x%x\n", ovp_intr);
	}

	return IRQ_HANDLED;
}

int wrap_ovp_drv_register_isr(void)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	int err;

	/* register and enable OVP ISR  */
	err = request_irq(ptr_ovp_ctx->irq_num, wrap_ovp_drv_isr, 0,
			  OVP_MODULE_NAME, ptr_ovp_ctx);
	if (unlikely(err < 0))
		ovp_error("vec_num:%5d, err:%8x\n", ptr_ovp_ctx->irq_num, err);
	else {
		ovp_trace("%s ok\n", __func__);
	}

	return err;
}
EXPORT_SYMBOL(wrap_ovp_drv_register_isr);

void wrap_ovp_drv_free_isr(void)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;

	/* unregister OVP interrupt */
	free_irq(ptr_ovp_ctx->irq_num, (void *)ptr_ovp_ctx);
}
EXPORT_SYMBOL(wrap_ovp_drv_free_isr);

int wrap_ovp_drv_get_isr_msg(CC_MSG_t *msg)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	unsigned long flags;
	int rc = S_OK;

	if (!msg)
		return -EINVAL;

	/* wait for the ISR to sem up */
	rc = down_interruptible(&ptr_ovp_ctx->ovp_sem);
	if (rc < 0) {
		/* -EINTR & -ERESTARTSYS are expected during suspend flow interruption
		 * Reduce logging severity for these compared to other errors
		 */
		if (rc == -EINTR || rc == -ERESTARTSYS)
			ovp_trace("%s: interrupted while waiting for ISR msg (rc=%d)\n",
				  __func__, rc);
		else
			ovp_error("%s: down_interruptible failed (rc=%d)\n", __func__, rc);
		return rc;
	}

	spin_lock_irqsave(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
	/* only send latest message to task. */
	if (AMPMsgQ_Fullness(&ptr_ovp_ctx->h_ovp_msg_q) <= 0) {
		spin_unlock_irqrestore(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
		ovp_error(" E/[ovp isr task]  message queue empty\n");
		return -EFAULT;
	}
	AMPMsgQ_DequeueRead(&ptr_ovp_ctx->h_ovp_msg_q, msg);
	spin_unlock_irqrestore(&ptr_ovp_ctx->ovp_msg_spinlock, flags);

	return rc;
}
EXPORT_SYMBOL(wrap_ovp_drv_get_isr_msg);

int wrap_ovp_drv_set_intr(INTR_MSG *ptr_ovp_intr_info)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	unsigned long flags;

	if (!ptr_ovp_intr_info) {
		pr_err("%s: Invalid parameter\n", __func__);
		return -EINVAL;
	}

	spin_lock_irqsave(&ptr_ovp_ctx->ovp_msg_spinlock, flags);
	ptr_ovp_ctx->ovp_intr_status = ptr_ovp_intr_info->Enable;
	spin_unlock_irqrestore(&ptr_ovp_ctx->ovp_msg_spinlock, flags);

	return 0;
}
EXPORT_SYMBOL(wrap_ovp_drv_set_intr);

int wrap_ovp_drv_clk_ctrl(bool b_ovp_clock_control)
{
	OVP_CTX *ptr_ovp_ctx = &ovp_ctx;
	int ret = 0;

	if (b_ovp_clock_control) {
		ret = clk_prepare_enable(ptr_ovp_ctx->ovp_clk);
		if (ret < 0) {
			ovp_error("%s prepare failed..!\n", OVP_MODULE_CLK);
			return ret;
		}
	} else {
		clk_disable_unprepare(ptr_ovp_ctx->ovp_clk);
	}

	return ret;
}
EXPORT_SYMBOL(wrap_ovp_drv_clk_ctrl);

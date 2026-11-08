// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2025 Synaptics Incorporated */

#define pr_fmt(fmt) "[cec kernel driver]" fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <media/cec.h>
#include <media/cec-notifier.h>
#include "syna_cec.h"
#include "syna_cec_prv.h"
#include "kernel_compatibility.h"

#define CEC_DEVICE_NAME     "syna_cec"

#define RX_LINE_POLL_COUNTER 30
#define RX_LINE_POLL_INTERVALL (10000000)

/* CEC device logical address mapping - based on Spec HDMI 1.3a */
#define CEC_LOG_ADDR_TYPE_RESERVED 7
const char CEC_devLogAddrMap[] =
{
	CEC_LOG_ADDR_TYPE_TV,           // 0
	CEC_LOG_ADDR_TYPE_RECORD,       // 1
	CEC_LOG_ADDR_TYPE_RECORD,       // 2
	CEC_LOG_ADDR_TYPE_TUNER,        // 3
	CEC_LOG_ADDR_TYPE_PLAYBACK,     // 4
	CEC_LOG_ADDR_TYPE_AUDIOSYSTEM,  // 5
	CEC_LOG_ADDR_TYPE_TUNER,        // 6
	CEC_LOG_ADDR_TYPE_TUNER,        // 7
	CEC_LOG_ADDR_TYPE_PLAYBACK,     // 8
	CEC_LOG_ADDR_TYPE_RECORD,       // 9
	CEC_LOG_ADDR_TYPE_TUNER,        // 10 // 1.3a
	CEC_LOG_ADDR_TYPE_PLAYBACK,     // 11 // 1.3a
	CEC_LOG_ADDR_TYPE_RESERVED,     // 12
	CEC_LOG_ADDR_TYPE_RESERVED,     // 13
	CEC_LOG_ADDR_TYPE_SPECIFIC,     // 14 (since free use addr is also for TV)
};

/*******************************************************************************
  Module internal function
  */

/******************************************************************************
 * FUNCTION : Finds device type for the given logical address
 * PARAMS   : logAddr   - Logical address
 * RETURN   : Device type corresponding to the logical address
 *****************************************************************************/
static int cec_get_device_type_from_addr (int logAddr)
{
	if (logAddr > CEC_MAX_LOG_ADDR)
		return -EINVAL;

	return (CEC_devLogAddrMap[logAddr]);
}

static irqreturn_t syna_cec_irq_thread_handler(int irq, void *data)
{
#ifdef CEC_PLATFORM_DEBUG
	int i;
#endif
	struct cec_device_t *cec = (struct cec_device_t *)data;

	if (cec->tx_done) {
		cec_transmit_attempt_done(cec->adap, cec->tx_status);
		cec->tx_done = false;
	}
	if (cec->rx_done) {
		struct cec_msg msg = {};

		msg.len = cec->rx_buf_cnt;
		memcpy(msg.msg, cec->rx_buf, msg.len);
#ifdef CEC_PLATFORM_DEBUG
		pr_info("CEC RX data length (%d)\n", cec->rx_buf_cnt);
		for (i = 0; i < cec->rx_buf_cnt; i++)
			pr_info("RX data (%d) : 0x%x\n", i, cec->rx_buf[i]);
#endif
		cec_received_msg(cec->adap, &msg);
		cec->rx_done = false;
		cec->rx_buf_cnt = 0;
	}
	return IRQ_HANDLED;
}

static irqreturn_t cec_devices_isr(int irq, void *data)
{
	struct cec_device_t *cec = (struct cec_device_t *)data;
	unsigned short reg;
	int sts_info;
	int intr;
	int i;
	u8 dptr_len;
	u8 value;

	// Read CEC status register
	syna_cec_reg_read(cec, CEC_INTR_STATUS0_REG_ADDR, &value, 1);
	reg = (unsigned short) value;
	syna_cec_reg_read(cec, CEC_INTR_STATUS1_REG_ADDR, &value, 1);
	reg |= ((unsigned short) value << 8);

#ifdef CEC_PLATFORM_DEBUG
	pr_info("%s: intr reg (0x%x)\n", __func__, reg);
#endif
	// Clear syna_cec_enable_interrupta
	if (reg & SYNA_CEC_INTR_TX_FAIL) {
		intr = SYNA_CEC_INTR_TX_FAIL;
		syna_cec_get_fail_status(cec, SYNA_CEC_MODE_TX, &sts_info, reg);
		cec->tx_status = sts_info;
		value = 0;
		syna_cec_reg_write(cec, CEC_RDY_ADDR, &value, 1);
		syna_cec_reg_read(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		value &= ~(intr & 0x00ff);
		syna_cec_reg_write(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		cec->tx_done = true;
		return IRQ_WAKE_THREAD;
	}
	if (reg & SYNA_CEC_INTR_TX_COMPLETE) {
		intr = SYNA_CEC_INTR_TX_COMPLETE;
		value = 0;
		syna_cec_reg_write(cec, CEC_RDY_ADDR, &value, 1);
		syna_cec_reg_read(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		value &= ~(intr & 0x00ff);
		syna_cec_reg_write(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		cec->tx_done = true;
		cec->tx_status = CEC_TX_STATUS_OK;
		return IRQ_WAKE_THREAD;
	}
	if (reg & SYNA_CEC_INTR_RX_FAIL) {
		intr = SYNA_CEC_INTR_RX_FAIL;
		syna_cec_reg_read(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		value &= ~(intr & 0x00ff);
		syna_cec_reg_write(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		cec->rx_done = false;
		cec->rx_buf_cnt = 0;
		value = 0;
		syna_cec_reg_write(cec, CEC_RX_RDY_ADDR, &value, 1);
		value = 1;
		syna_cec_reg_write(cec, CEC_RX_RDY_ADDR, &value, 1);
	}
	if (reg & SYNA_CEC_INTR_RX_COMPLETE) {
		intr = SYNA_CEC_INTR_RX_COMPLETE;
		syna_cec_reg_read(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		value &= ~(intr & 0x00ff);
		syna_cec_reg_write(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		// read cec mesg from rx buffer
		syna_cec_reg_read(cec, CEC_RX_FIFO_DPTR, &dptr_len, 1);
		cec->rx_buf_cnt = dptr_len;
		value = 0x01;
		for (i = 0; i < dptr_len; i++) {
			syna_cec_reg_read(cec, CEC_RX_BUF_READ_REG_ADDR, &cec->rx_buf[i], 1);
			syna_cec_reg_write(cec, CEC_TOGGLE_FOR_READ_REG_ADDR, &value, 1);
		}
		value = 0;
		syna_cec_reg_write(cec, CEC_RX_RDY_ADDR, &value, 1);
		value = 1;
		syna_cec_reg_write(cec, CEC_RX_RDY_ADDR, &value, 1);
		syna_cec_reg_read(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		value |= (intr & 0x00ff);
		syna_cec_reg_write(cec, CEC_INTR_ENABLE0_REG_ADDR, &value, 1);
		cec->rx_done = true;
		return IRQ_WAKE_THREAD;
	}

	return IRQ_HANDLED;
}

#ifdef CONFIG_PM_SLEEP
static int cec_suspend(struct device *dev)
{
	struct cec_device_t *cec_dev = dev_get_drvdata(dev);

	pr_info("cec_suspend\n");

	disable_irq_nosync(cec_dev->cec_irq);

	return 0;
}

static int cec_resume(struct device *dev)
{
	struct cec_device_t *cec_dev = dev_get_drvdata(dev);

	pr_info("cec_resume\n");

	enable_irq(cec_dev->cec_irq);

	return 0;
}
#endif

static int syna_cec_adap_log_addr(struct cec_adapter *adap, u8 logical_addr)
{
	struct cec_device_t *cec_dev = adap->priv;

#ifdef CEC_PLATFORM_DEBUG
	pr_info("%s log addr (%d)\n", __func__, logical_addr);
#endif
	if (logical_addr != CEC_LOG_ADDR_INVALID) {
		u8 dev_type = cec_get_device_type_from_addr(logical_addr);
		syna_cec_enable_log_addr (cec_dev, true, dev_type, logical_addr);
	}

	return 0;
}

static int syna_cec_adap_enable(struct cec_adapter *adap, bool enable)
{
	struct cec_device_t *cec_dev = adap->priv;
	if (enable) {
		syna_cec_load_default_val(cec_dev);
	} else {
		syna_cec_set_mode (cec_dev, SYNA_CEC_MODE_TX, false);
		syna_cec_set_mode (cec_dev, SYNA_CEC_MODE_RX, false);
	}
	return 0;
}

static int syna_cec_adap_transmit(struct cec_adapter *adap, u8 attempts,
				   u32 signal_free_time_ms, struct cec_msg *msg)
{
	struct cec_device_t *cec_dev = adap->priv;

#ifdef CEC_PLATFORM_DEBUG
	u8 i;
	pr_info("%s: length (%d), SF time (%d)\n", __func__, msg->len, signal_free_time_ms);
	for (i = 0; i < msg->len; i++)
	   pr_info("data (%d) : 0x%x\n", i, msg->msg[i]);
#endif
	if (cec_dev->msg_in_transmit == true) {
		pr_err("Transit is in progress\n");
		return -EBUSY;
	}
	if ((syna_cec_rx_line_status(cec_dev) & 0x01) == 0x01) {	// line free
		syna_cec_transmit_data (cec_dev, msg, signal_free_time_ms);
	} else {
		memcpy(&cec_dev->msg, msg, sizeof(struct cec_msg));
		cec_dev->msg_in_transmit = true;
		cec_dev->tx_counter = 0;
		cec_dev->signal_free_time_ms = signal_free_time_ms;
		cec_dev->tx_time = ktime_get();
#ifdef CEC_PLATFORM_DEBUG
		pr_warn("%s: CEC rx line is busy cec_dev(%p)\n", __func__, cec_dev);
#endif
		hrtimer_start(&cec_dev->rx_line_poll_timer,
					cec_dev->rx_line_poll_timer_interval, HRTIMER_MODE_REL);
	}
	return 0;
}

static const struct cec_adap_ops syna_cec_ops = {
	.adap_enable = syna_cec_adap_enable,
	.adap_log_addr = syna_cec_adap_log_addr,
	.adap_transmit = syna_cec_adap_transmit,
};

static enum hrtimer_restart cec_transmit_timer_callback(struct hrtimer *timer)
{
	ktime_t curr_time;
	u64 elapsed_ns;
	int i;

	struct cec_device_t *cec_dev;

	cec_dev = container_of(timer, struct cec_device_t, rx_line_poll_timer);

	cec_dev->tx_counter++;
	curr_time = ktime_get();

	if ((syna_cec_rx_line_status(cec_dev) & 0x01) == 0x01) {
		cec_dev->tx_counter = 0;
		elapsed_ns = ktime_to_ns(ktime_sub(curr_time, cec_dev->tx_time));
		for (i = 0; i < cec_dev->msg.len; i++)
		if (elapsed_ns <= CEC_MSG_TX_TIMEOUT) {
			syna_cec_transmit_data (cec_dev, &cec_dev->msg, cec_dev->signal_free_time_ms);
		} else {
			pr_warn("CEC msg aborted due to timeout\n");
			cec_transmit_attempt_done(cec_dev->adap, CEC_TX_STATUS_ABORTED);
		}
		cec_dev->msg_in_transmit = false;
		return HRTIMER_NORESTART;

	} else {
		if (cec_dev->tx_counter > RX_LINE_POLL_COUNTER) {
			cec_dev->msg_in_transmit = false;
			cec_dev->tx_counter = 0;
			cec_transmit_attempt_done(cec_dev->adap, CEC_TX_STATUS_ABORTED);
			return HRTIMER_NORESTART;
		} else {
			hrtimer_forward_now(&cec_dev->rx_line_poll_timer, cec_dev->rx_line_poll_timer_interval);
		}
	}
	return HRTIMER_RESTART;
}

static int cec_probe(struct platform_device *pdev)
{
	int res;
	struct resource *cec_resource;
	struct cec_device_t *cec_dev;
	struct device *hdmi_dev;
	struct device_node *sub_node, *hdmi_node;

	hdmi_dev = cec_notifier_parse_hdmi_phandle(&pdev->dev);

	if (IS_ERR(hdmi_dev)) {
		pr_err("%s:%d ERROR : %ld\n", __func__, __LINE__, PTR_ERR(hdmi_dev));
		return PTR_ERR(hdmi_dev);
	}
	hdmi_node = hdmi_dev->of_node;

	if (!hdmi_node) {
		pr_err("HDMI device node not found\n");
		return -ENODEV;
	}

	sub_node = of_get_child_by_name(hdmi_node, "hdmi_tx");
	if (!sub_node) {
		pr_err("Subnode 'hdmi_tx' not found\n");
		return -ENODEV;
	}

	// Check if the subnode is enabled (status="okay" or property absent)
	if (!of_device_is_available(sub_node)) {
		pr_err("Subnode 'hdmi_tx' is disabled in DTS. Skipping initialization.\n");
		of_node_put(sub_node); // Release the node reference
		return -ENODEV; // Or handle as appropriate for your driver
	}
	of_node_put(sub_node);

	cec_dev = devm_kzalloc(&pdev->dev, sizeof(struct cec_device_t), GFP_KERNEL);
	if (!cec_dev)
		return -ENOMEM;

	platform_set_drvdata(pdev, cec_dev);

	cec_dev->cec_irq = platform_get_irq(pdev, 0);
	if (cec_dev->cec_irq < 0)
		return cec_dev->cec_irq;

	cec_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	cec_dev->cec_virt_addr = devm_ioremap_resource(&pdev->dev, cec_resource);
	if (IS_ERR(cec_dev->cec_virt_addr)) {
		res = PTR_ERR(cec_dev->cec_virt_addr);
		goto err_ioremap;
	}

	cec_dev->dev = &pdev->dev;

	res = devm_request_threaded_irq(&pdev->dev, cec_dev->cec_irq,
		cec_devices_isr, syna_cec_irq_thread_handler,
		0, "cec_irq", (void *)cec_dev);

	if (res) {
		dev_err(&pdev->dev,
			"Unable to request interrupt for device\n");
		goto err_irq;
	}

	cec_dev->adap = cec_allocate_adapter(&syna_cec_ops, cec_dev, CEC_DEVICE_NAME,
			CEC_CAP_DEFAULTS | CEC_CAP_MONITOR_ALL |
			CEC_CAP_CONNECTOR_INFO,
			CEC_MAX_LOG_ADDRS);
	if (IS_ERR(cec_dev->adap)) {
		res = PTR_ERR(cec_dev->adap);
		dev_err(&pdev->dev, "Couldn't create cec adapter\n");
		goto err_adapter_alloc;
	}

	cec_dev->notifier = cec_notifier_cec_adap_register(hdmi_dev, NULL,
							   cec_dev->adap);
	if (!cec_dev->notifier) {
		res = -ENOMEM;
		goto err_notifier;
	}

	res = cec_register_adapter(cec_dev->adap, &pdev->dev);
	if (res) {
		dev_err(&pdev->dev, "Couldn't register device\n");
		goto err_register_adapter;
	}

	hrtimer_init(&cec_dev->rx_line_poll_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	cec_dev->rx_line_poll_timer.function = cec_transmit_timer_callback;
	cec_dev->rx_line_poll_timer_interval = ktime_set(0, RX_LINE_POLL_INTERVALL); // 10ms
	return 0;

err_register_adapter:
	cec_notifier_cec_adap_unregister(cec_dev->notifier, cec_dev->adap);
err_notifier:
	cec_delete_adapter(cec_dev->adap);
err_adapter_alloc:
err_irq:
err_ioremap:
	pr_err("cec_probe failed !!! (%d)\n", res);
	return res;
}

static RET_TYPE cec_remove(struct platform_device *pdev)
{
	struct cec_device_t *cec_dev = platform_get_drvdata(pdev);

	if (cec_dev->msg_in_transmit)
		hrtimer_cancel(&cec_dev->rx_line_poll_timer);

	// Unregister CEC adapter
	cec_notifier_cec_adap_unregister(cec_dev->notifier, cec_dev->adap);
	cec_unregister_adapter(cec_dev->adap);
	RETURN_VALUE;
}

static const struct of_device_id cec_match[] = {
	{.compatible = "syna,berlin-cec",},
	{},
};
MODULE_DEVICE_TABLE(of, cec_match);

static SIMPLE_DEV_PM_OPS(cec_pmops, cec_suspend,
			 cec_resume);

static struct platform_driver syna_cec_driver = {
	.probe = cec_probe,
	.remove = cec_remove,
	.driver = {
		   .name = CEC_DEVICE_NAME,
		   .of_match_table = cec_match,
		   .pm = &cec_pmops,
	},
};
module_platform_driver(syna_cec_driver);

MODULE_AUTHOR("synaptics");
MODULE_DESCRIPTION("cec module driver");
MODULE_LICENSE("GPL v2");

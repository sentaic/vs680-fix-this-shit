// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Synaptics Incorporated
 *
 * Syna SMC reset driver
 *
 * Author: Jisheng Zhang <jszhang@kernel.org>
 */

#include <linux/arm-smccc.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/reset-controller.h>
#include <linux/slab.h>

#define SYNA_SMC_MAX_RESETS	32
#define SYNA_SIP_RESET		0xC200000E

enum reset_type {
	SYNA_SMC_RESET,
	SYNA_SMC_ASSERT,
	SYNA_SMC_DEASSERT,
};

#define to_syna_smc_reset_priv(p)		\
	container_of((p), struct syna_reset_priv, rcdev)

struct syna_reset_priv {
	spinlock_t			lock;
	struct reset_controller_dev	rcdev;
};

static int syna_smc_reset_assert(struct reset_controller_dev *rcdev,
				 unsigned long id)
{
	unsigned long flags;
	struct arm_smccc_res res;
	struct syna_reset_priv *priv = to_syna_smc_reset_priv(rcdev);

	spin_lock_irqsave(&priv->lock, flags);

	arm_smccc_smc(SYNA_SIP_RESET, SYNA_SMC_ASSERT, id, 0,
		      0, 0, 0, 0, &res);

	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

static int syna_smc_reset_deassert(struct reset_controller_dev *rcdev,
				   unsigned long id)
{
	unsigned long flags;
	struct arm_smccc_res res;
	struct syna_reset_priv *priv = to_syna_smc_reset_priv(rcdev);

	spin_lock_irqsave(&priv->lock, flags);

	arm_smccc_smc(SYNA_SIP_RESET, SYNA_SMC_DEASSERT, id, 0,
		      0, 0, 0, 0, &res);

	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

static const struct reset_control_ops syna_reset_ops = {
	.assert		= syna_smc_reset_assert,
	.deassert	= syna_smc_reset_deassert,
};

static int syna_smc_reset_probe(struct platform_device *pdev)
{
	struct syna_reset_priv *priv;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	spin_lock_init(&priv->lock);

	priv->rcdev.owner = THIS_MODULE;
	priv->rcdev.ops = &syna_reset_ops;
	priv->rcdev.of_node = pdev->dev.of_node;
	priv->rcdev.nr_resets = SYNA_SMC_MAX_RESETS;

	return devm_reset_controller_register(&pdev->dev, &priv->rcdev);
}

static const struct of_device_id syna_smc_reset_dt_match[] = {
	{ .compatible = "syna,smc-reset" },
	{ },
};
MODULE_DEVICE_TABLE(of, syna_smc_reset_dt_match);

static struct platform_driver syna_smc_reset_driver = {
	.probe	= syna_smc_reset_probe,
	.driver	= {
		.name = "syna-smc-reset",
		.of_match_table = syna_smc_reset_dt_match,
	},
};
module_platform_driver(syna_smc_reset_driver);

MODULE_AUTHOR("Jisheng Zhang <jszhang@kernel.org>");
MODULE_DESCRIPTION("Synaptics SMC reset driver");
MODULE_LICENSE("GPL");

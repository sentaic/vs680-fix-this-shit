// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * Author: Jisheng Zhang <jszhang@kernel.org>
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/mux/consumer.h>
#include <linux/of_mdio.h>
#include <linux/phy.h>
#include <linux/platform_device.h>

#define MAX_MDIO_PORT	2

struct mdio_demux_port {
	struct mii_bus *pb;
	struct mii_bus *bus;
	struct mdio_demux_priv *priv;
	int port;
};

struct mdio_demux_priv {
	struct mutex mdio_demux_lock;
	int current_port;
	bool do_deselect;
	struct mux_control *muxc;
	struct mdio_demux_port dmxport[MAX_MDIO_PORT];
};

static int parent_count;

static int mdio_demux_switch(struct mdio_demux_port *dmxport)
{
	struct mdio_demux_priv *priv = dmxport->priv;
	int r;

	if (!(priv->current_port ^ dmxport->port))
		return 0;

	if (priv->do_deselect) {
		r = mux_control_deselect(priv->muxc);
		if (r)
			return r;
	}

	r = mux_control_select(priv->muxc, dmxport->port);
	if (r) {
		priv->do_deselect = false;
		return r;
	} else {
		priv->do_deselect = true;
	}

	priv->current_port = dmxport->port;

	return 0;
}

static int mdio_demux_read(struct mii_bus *bus, int phy_id, int regnum)
{
	struct mdio_demux_port *dmxport = bus->priv;
	struct mdio_demux_priv *priv = dmxport->priv;
	int r;

	guard(mutex)(&priv->mdio_demux_lock);

	r = mdio_demux_switch(dmxport);
	if (r)
		return r;

	return dmxport->pb->read(dmxport->pb, phy_id, regnum);
}

static int mdio_demux_read_c45(struct mii_bus *bus, int phy_id, int dev_addr,
			       int regnum)
{
	struct mdio_demux_port *dmxport = bus->priv;
	struct mdio_demux_priv *priv = dmxport->priv;
	int r;

	guard(mutex)(&priv->mdio_demux_lock);

	r = mdio_demux_switch(dmxport);
	if (r)
		return r;

	return dmxport->pb->read_c45(dmxport->pb, phy_id, dev_addr, regnum);
}

static int mdio_demux_write(struct mii_bus *bus, int phy_id,
			    int regnum, u16 val)
{
	struct mdio_demux_port *dmxport = bus->priv;
	struct mdio_demux_priv *priv = dmxport->priv;
	int r;

	guard(mutex)(&priv->mdio_demux_lock);

	r = mdio_demux_switch(dmxport);
	if (r)
		return r;

	return dmxport->pb->write(dmxport->pb, phy_id, regnum, val);
}

static int mdio_demux_write_c45(struct mii_bus *bus, int phy_id, int dev_addr,
			        int regnum, u16 val)
{
	struct mdio_demux_port *dmxport = bus->priv;
	struct mdio_demux_priv *priv = dmxport->priv;
	int r;

	guard(mutex)(&priv->mdio_demux_lock);

	r = mdio_demux_switch(dmxport);
	if (r)
		return r;

	return dmxport->pb->write_c45(dmxport->pb, phy_id, dev_addr, regnum, val);
}

static int mdio_demux_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device_node *child_bus_node, *parent_bus_node;
	struct mii_bus *parent_bus;
	struct mii_bus *bus;
	struct mdio_demux_priv *priv;
	int r, v, i = 0;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->current_port = -1;
	mutex_init(&priv->mdio_demux_lock);

	priv->muxc = devm_mux_control_get(&pdev->dev, NULL);
	if (IS_ERR(priv->muxc))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->muxc), "Failed to get mux\n");

	for_each_available_child_of_node(np, child_bus_node) {
		if (i >= MAX_MDIO_PORT)
			return -EINVAL;

		parent_bus_node = of_parse_phandle(np, "mdio-parent-bus", i);
		if (!parent_bus_node)
			return -ENODEV;

		r = of_property_read_u32(child_bus_node, "reg", &v);
		if (r) {
			dev_err(&pdev->dev, "Error: Failed to find reg for child %p\n", np);
			goto err_parent_bus;
		}

		parent_bus = of_mdio_find_bus(parent_bus_node);
		if (!parent_bus) {
			r = -EPROBE_DEFER;
			goto err_parent_bus;
		}

		priv->dmxport[i].pb = parent_bus;
		priv->dmxport[i].port = v;
		priv->dmxport[i].priv = priv;

		bus = devm_mdiobus_alloc(&pdev->dev);
		if (!bus)
			return -ENOMEM;

		bus->priv = &priv->dmxport[i];
		bus->name = "mdio_demux";
		snprintf(bus->id, MII_BUS_ID_SIZE, "%s-%x.%x", bus->name, parent_count++, v);
		bus->parent = &pdev->dev;
		if (parent_bus->read)
			bus->read = mdio_demux_read;
		if (parent_bus->write)
			bus->write = mdio_demux_write;
		if (parent_bus->read_c45)
			bus->read_c45 = mdio_demux_read_c45;
		if (parent_bus->write_c45)
			bus->write_c45 = mdio_demux_write_c45;
		r = of_mdiobus_register(bus, child_bus_node);
		if (r)
			goto err_parent_bus;
		++i;
	}

	platform_set_drvdata(pdev, priv);

	return 0;

err_parent_bus:
	of_node_put(np);
	return r;
}

static void mdio_demux_remove(struct platform_device *pdev)
{
	struct mdio_demux_priv *priv = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < MAX_MDIO_PORT; i++)
		mdiobus_unregister(priv->dmxport[i].bus);

	if (priv->do_deselect) {
		mux_control_deselect(priv->muxc);
		priv->do_deselect = false;
	}
}

static int mdio_demux_suspend(struct device *dev)
{
	struct mdio_demux_priv *priv = dev_get_drvdata(dev);
	int ret = 0;

	guard(mutex)(&priv->mdio_demux_lock);

	priv->current_port = -1;
	if (priv->do_deselect) {
		ret = mux_control_deselect(priv->muxc);
		priv->do_deselect = false;
	}

	return ret;
}

static DEFINE_SIMPLE_DEV_PM_OPS(mdio_demux_pm_ops, mdio_demux_suspend, NULL);

static const struct of_device_id mdio_demux_dt_ids[] = {
	{ .compatible = "mdio-demux" },
	{ }
};
MODULE_DEVICE_TABLE(of, mdio_demux_dt_ids);

static struct platform_driver mdio_demux_driver = {
	.probe = mdio_demux_probe,
	.remove = mdio_demux_remove,
	.driver = {
		.name = "mdio-demux",
		.of_match_table = mdio_demux_dt_ids,
		.pm = pm_sleep_ptr(&mdio_demux_pm_ops),
	},
};
module_platform_driver(mdio_demux_driver);

MODULE_DESCRIPTION("MDIO bus demux driver");
MODULE_AUTHOR("Jisheng Zhang <jszhang@kernel.org>");
MODULE_LICENSE("GPL");

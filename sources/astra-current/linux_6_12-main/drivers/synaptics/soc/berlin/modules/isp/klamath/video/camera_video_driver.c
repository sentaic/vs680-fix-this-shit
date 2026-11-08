// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/of_reserved_mem.h>
#include <linux/of_graph.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/string.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mc.h>
#include <media/videobuf2-dma-contig.h>
#include <media/v4l2-fwnode.h>

#include "camera_video_driver.h"
#include "camera_video_register.h"

/**
 * camera_video_register_ports - Register video devices for all configured ports
 * @camera_mdev: Camera media device instance
 *
 * This function iterates through all configured ports and registers video devices
 * for non-M2M (memory-to-memory) ports. Each port corresponds to a separate
 * video device node (e.g., /dev/video8, /dev/video9).
 *
 * The function handles error cleanup by unregistering any successfully registered
 * ports if a later registration fails.
 *
 * Return: 0 on success, negative error code on failure
 */
static int camera_video_register_ports(struct camera_media_dev *camera_mdev)
{
	int ret = 0;
	int i;

	dev_dbg(camera_mdev->dev, "Registering %d video ports\n", camera_mdev->ports);

	/* Register video device for each configured port */
	for (i = 0; i < camera_mdev->ports; i++) {
		ret = camera_video_register(camera_mdev, i);
		if (ret) {
			dev_err(camera_mdev->dev,
				"Failed to register video device for port %d: %d\n",
				i, ret);
			goto err_register_video;
		}
		dev_info(camera_mdev->dev, "Registered video device for port %d\n", i);
	}

	dev_info(camera_mdev->dev, "Successfully registered %d video ports\n", camera_mdev->ports);
	return 0;

err_register_video:
	/* Cleanup: unregister any successfully registered ports */
	for (i = 0; i < camera_mdev->ports; i++)
		camera_video_unregister(camera_mdev, i);
	return ret;
}

/**
 * camera_video_unregister_ports - Unregister all video devices
 * @camera_mdev: Camera media device instance
 *
 * This function unregisters all video devices that were previously registered
 * for non-M2M ports. It's called during driver removal or error cleanup.
 *
 * Return: 0 on success
 */
static int camera_video_unregister_ports(struct camera_media_dev *camera_mdev)
{
	int i;

	dev_dbg(camera_mdev->dev, "Unregistering %d video ports\n", camera_mdev->ports);

	/* Unregister video device for each configured port */
	for (i = 0; i < camera_mdev->ports; i++) {
		camera_video_unregister(camera_mdev, i);
		dev_dbg(camera_mdev->dev, "Unregistered video device for port %d\n", i);
	}

	dev_info(camera_mdev->dev, "Successfully unregistered all video ports\n");
	return 0;
}

/** Handle subdevice binding */
static int camera_video_notifier_bound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd, struct v4l2_async_connection *asc)
{
	struct camera_media_dev *camera_mdev = container_of(notifier,
			struct camera_media_dev, notifier);
	struct fwnode_handle *ep = NULL;
	struct v4l2_fwnode_link link;
	struct camera_video_dev *camera_vdev;
	int ret;

	/* Create media links for each endpoint */
	while (1) {
		ep = fwnode_graph_get_next_endpoint(sd->fwnode, ep);
		if (!ep)
			break;

		ret = v4l2_fwnode_parse_link(ep, &link);
		if (ret || link.remote_port >= camera_mdev->ports) {
			v4l2_fwnode_put_link(&link);
			continue;
		}

		camera_vdev = camera_mdev->video_devs[link.remote_port];
		if (camera_vdev && camera_vdev->video) {
			if (!(camera_vdev->video) || !(camera_vdev->video->entity.pads)) {
				dev_err(camera_mdev->dev,
					"%s: %d returning -EINVAL\n",
					__func__, __LINE__);
				return -EINVAL;
			}
			/* Skip link if the source pad is a sink pad (matches vvcam logic) */
			if (sd->entity.pads[link.local_port].flags == MEDIA_PAD_FL_SINK)
				continue;
			ret = media_create_pad_link(&sd->entity, link.local_port,
					&camera_vdev->video->entity, 0,
					MEDIA_LNK_FL_ENABLED);
		}
		v4l2_fwnode_put_link(&link);

		if (ret)
			break;
	}
	fwnode_handle_put(ep);
	return ret;
}

/** Handle subdevice unbinding */
static void camera_video_notifier_unbound(struct v4l2_async_notifier *notifier,
		struct v4l2_subdev *sd, struct v4l2_async_connection *asc)
{
}

/** Complete async notifier setup */
static int camera_video_notifier_complete(struct v4l2_async_notifier *notifier)
{
	struct camera_media_dev *camera_mdev = container_of(notifier,
			struct camera_media_dev, notifier);
	return v4l2_device_register_subdev_nodes(&camera_mdev->v4l2_dev);
}

static const struct v4l2_async_notifier_operations camera__video_async_nf_ops = {
	.bound    = camera_video_notifier_bound,
	.unbind   = camera_video_notifier_unbound,
	.complete = camera_video_notifier_complete,
};

	static struct v4l2_async_connection *
camera_video_async_nf_add_fwnode_remote(struct list_head *endpoint_list,
		struct v4l2_async_notifier *notif,
		struct fwnode_handle *endpoint,
		unsigned int asc_struct_size)
{
	struct v4l2_async_connection *asc;
	struct fwnode_handle *remote;
	struct list_head *cur;
	unsigned int add_remote_node = 0;
	struct remote_node_handle *entry;
	struct remote_node_handle *new_node;

	remote = fwnode_graph_get_remote_port_parent(endpoint);
	if (!remote)
		return ERR_PTR(-ENOTCONN);

	if (!list_empty(endpoint_list)) {
		list_for_each(cur, endpoint_list) {
			entry = list_entry(cur, struct remote_node_handle, link);
			if (entry->remote == remote)
				return ERR_PTR(-EEXIST);

			add_remote_node = 1;
		}
	} else {
		add_remote_node = 1;
	}

	if (add_remote_node) {
		new_node = kzalloc(sizeof(struct remote_node_handle), GFP_KERNEL);
		new_node->remote = remote;
		list_add_tail(&new_node->link, endpoint_list);
		asc = __v4l2_async_nf_add_fwnode(notif, remote, asc_struct_size);
	} else {
		fwnode_handle_put(remote);
	}

	return asc;
}

/** Register async subdevice */
static int camera_video_async_register_subdev(struct camera_media_dev *camera_mdev)
{
	int ret = 0;
	struct fwnode_handle *ep;
	struct v4l2_async_connection *asc;
	unsigned int port_id = 0;
	struct list_head endpoint_list;
	struct remote_node_handle *pos = NULL, *next = NULL;

	INIT_LIST_HEAD(&endpoint_list);
	camera_mdev->notifier.ops = &camera__video_async_nf_ops;
	v4l2_async_nf_init(&camera_mdev->notifier, &camera_mdev->v4l2_dev);

	while (1) {
		ep = fwnode_graph_get_endpoint_by_id(dev_fwnode(camera_mdev->dev),
				port_id, 0, FWNODE_GRAPH_ENDPOINT_NEXT);
		if (!ep)
			break;

		asc = camera_video_async_nf_add_fwnode_remote(&endpoint_list,
				&camera_mdev->notifier, ep,
				sizeof(struct v4l2_async_connection));

		fwnode_handle_put(ep);

		if (IS_ERR(asc)) {
			ret = PTR_ERR(asc);
			if (ret != -EEXIST) {
				v4l2_async_nf_cleanup(&camera_mdev->notifier);
				goto clean_list;
			}
		}
		port_id++;
	}

	ret = v4l2_async_nf_register(&camera_mdev->notifier);
	if (ret) {
		v4l2_async_nf_cleanup(&camera_mdev->notifier);
		dev_err(camera_mdev->dev, "v4l2 async notifier register error %d\n", ret);
	}

clean_list:
	/* Delete the list */
	if (!list_empty(&endpoint_list)) {
		list_for_each_entry_safe(pos, next, &endpoint_list, link) {
			if (pos) {
				fwnode_handle_put(pos->remote);
				list_del(&pos->link);
				kfree(pos);
			}
		}
	}
	return ret;
}

/** Unregister async subdevice */
static int camera_video_async_unregister_subdev(struct camera_media_dev *camera_mdev)
{
	v4l2_async_nf_unregister(&camera_mdev->notifier);
	v4l2_async_nf_cleanup(&camera_mdev->notifier);
	return 0;
}

static const struct media_device_ops camera_video_media_ops = {
	.link_notify = v4l2_pipeline_link_notify,
};

/** Parse device tree parameters */
static int camera_video_parse_params(struct camera_media_dev *camera_mdev,
		struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct device_node *ports_node, *port_node;
	int port_count = 0;

	fwnode_property_read_u32(of_fwnode_handle(node), "id", &camera_mdev->id);

	/* Look for ports node first */
	ports_node = of_get_child_by_name(node, "ports");
	if (ports_node) {
		/* Count port nodes under ports */
		for_each_child_of_node(ports_node, port_node) {
			if (of_node_name_prefix(port_node, "port"))
				port_count++;
		}
		of_node_put(ports_node);
	} else {
		/* Fallback: look for direct port nodes */
		for_each_child_of_node(node, port_node) {
			if (of_node_name_prefix(port_node, "port"))
				port_count++;
		}
	}

	/* If no ports found in DT, default to 2 */
	if (port_count == 0) {
		dev_info(camera_mdev->dev,
			"No ports found in DT, defaulting to 2 ports for testing\n");
		port_count = CAMERA_VIDEO_PORT_MAX;
	}

	/* Limit to 2 ports maximum */
	camera_mdev->ports = (port_count > CAMERA_VIDEO_PORT_MAX) ?
		CAMERA_VIDEO_PORT_MAX : port_count;
	dev_info(camera_mdev->dev, "Found %d video ports in device tree\n", camera_mdev->ports);
	return 0;
}

/** Initialize camera video driver */
static int camera_video_probe(struct platform_device *pdev)
{
	struct camera_media_dev *camera_mdev;
	struct media_device *mdev;
	int ret;

	camera_mdev = devm_kzalloc(&pdev->dev, sizeof(*camera_mdev), GFP_KERNEL);
	if (!camera_mdev)
		return -ENOMEM;

	camera_mdev->dev = &pdev->dev;
	platform_set_drvdata(pdev, camera_mdev);

	ret = camera_video_parse_params(camera_mdev, pdev);
	if (ret)
		return ret;

	/* Initialize media device */
	mdev = &camera_mdev->mdev;
	mdev->dev = &pdev->dev;
	mdev->ops = &camera_video_media_ops;
	strscpy(mdev->model, "synaptics_isp", sizeof(mdev->model));
	media_device_init(mdev);

	/* Register V4L2 device */
	camera_mdev->v4l2_dev.mdev = mdev;
	ret = v4l2_device_register(&pdev->dev, &camera_mdev->v4l2_dev);
	if (ret)
		return ret;

	/* Register video ports */
	ret = camera_video_register_ports(camera_mdev);
	if (ret)
		goto err_v4l2;

	/* Register async notifier */
	ret = camera_video_async_register_subdev(camera_mdev);
	if (ret)
		goto err_ports;

	/* Register media device */
	ret = media_device_register(mdev);
	if (ret)
		goto err_async;

	return 0;

err_async:
	camera_video_async_unregister_subdev(camera_mdev);
err_ports:
	camera_video_unregister_ports(camera_mdev);
err_v4l2:
	v4l2_device_unregister(&camera_mdev->v4l2_dev);
	return ret;
}

/** Cleanup camera video driver */
static void camera_video_remove(struct platform_device *pdev)
{
	struct camera_media_dev *camera_mdev = platform_get_drvdata(pdev);

	if (camera_mdev) {
		media_device_unregister(&camera_mdev->mdev);
		camera_video_async_unregister_subdev(camera_mdev);
		camera_video_unregister_ports(camera_mdev);
		v4l2_device_unregister(&camera_mdev->v4l2_dev);
	}
}

static const struct of_device_id camera_video_of_match[] = {
	{.compatible = "syna,video",},
	{ /* sentinel */ },
};

MODULE_DEVICE_TABLE(of, camera_video_of_match);

static struct platform_driver camera_video_driver = {
	.probe  = camera_video_probe,
	.remove = camera_video_remove,
	.driver = {
		.name  = CAMERA_VIDEO_NAME,
		.owner = THIS_MODULE,
		.of_match_table = camera_video_of_match,
	},
};

/** Initialize module */
static int __init camera_video_init_module(void)
{
	return platform_driver_register(&camera_video_driver);
}

/** Exit module */
static void __exit camera_video_exit_module(void)
{
	platform_driver_unregister(&camera_video_driver);
}

module_init(camera_video_init_module);
module_exit(camera_video_exit_module);

MODULE_DESCRIPTION("Synaptics ISP video driver");
MODULE_AUTHOR("Synaptics ISP Team");
MODULE_LICENSE("GPL");


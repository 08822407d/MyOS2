// SPDX-License-Identifier: GPL-2.0
/*
 * drivers/base/core.c - core driver model code (device registration, etc)
 *
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 * Copyright (c) 2006 Greg Kroah-Hartman <gregkh@suse.de>
 * Copyright (c) 2006 Novell, Inc.
 */

// #include <linux/acpi.h>
// #include <linux/cpufreq.h>
#include <linux/device/device.h>
#include <linux/kernel/err.h>
// #include <linux/fwnode.h>
#include <linux/init/init.h>
// #include <linux/module.h>
#include <linux/kernel/slab.h>
#include <linux/lib/string.h>
#include <linux/kernel/kdev_t.h>
// #include <linux/notifier.h>
// #include <linux/of.h>
// #include <linux/of_device.h>
#include <linux/block/genhd.h>
#include <linux/kernel/mutex.h>
// #include <linux/pm_runtime.h>
// #include <linux/netdevice.h>
// #include <linux/sched/signal.h>
#include <linux/sched/mm.h>
// #include <linux/swiotlb.h>
// #include <linux/sysfs.h>
// #include <linux/dma-map-ops.h> /* for dma_default_coherent */

#include <linux/drivers/base.h>
// #include "power/power.h"


#include <obsolete/proto.h>


/**
 * device_add - add device to device hierarchy.
 * @dev: device.
 *
 * This is part 2 of device_register(), though may be called
 * separately _iff_ device_initialize() has been called separately.
 *
 * This adds @dev to the kobject hierarchy via kobject_add(), adds it
 * to the global and sibling lists for the device, then
 * adds it to the other relevant subsystems of the driver model.
 *
 * Do not call this routine or device_register() more than once for
 * any device structure.  The driver model core is not designed to work
 * with devices that get unregistered and then spring back to life.
 * (Among other things, it's very hard to guarantee that all references
 * to the previous incarnation of @dev have been dropped.)  Allocate
 * and register a fresh new struct device instead.
 *
 * NOTE: _Never_ directly free @dev after calling this function, even
 * if it returned an error! Always use put_device() to give up your
 * reference instead.
 *
 * Rule of thumb is: if device_add() succeeds, you should call
 * device_del() when you want to get rid of it. If device_add() has
 * *not* succeeded, use *only* put_device() to drop the reference
 * count.
 */
int device_add(device_s *dev)
{
	int error = -EINVAL;
	device_s *parent;
	kobj_s *kobj;
	// kobj_s *glue_dir = NULL;

	/*
	 * for statically allocated devices, which should all be converted
	 * some day, we need to initialize the name. We prevent reading back
	 * the name, and force the use of dev_name()
	 */
	if (dev->init_name) {
		dev->kobj.name = dev->init_name;
		dev->init_name = NULL;
	}

	parent = dev->parent;
	kobj = &parent->kobj;
	if (IS_ERR(kobj)) {
		error = PTR_ERR(kobj);
		goto parent_error;
	}
	if (kobj)
		dev->kobj.parent = kobj;

// 	/* first, register with generic layer. */
// 	/* we require the name to be set before, and pass NULL */
// 	error = kobject_add(&dev->kobj, dev->kobj.parent, NULL);
// 	if (error) {
// 		glue_dir = get_glue_dir(dev);
// 		goto Error;
// 	}

	if (MAJOR(dev->devt)) {
		// error = device_create_file(dev, &dev_attr_dev);
		// if (error)
		// 	goto DevAttrError;

		// error = device_create_sys_dev_entry(dev);
		// if (error)
		// 	goto SysEntryError;

		devtmpfs_create_node(dev);
	}

// 	/* Notify clients of device addition.  This call must come
// 	 * after dpm_sysfs_add() and before kobject_uevent().
// 	 */
// 	if (dev->bus)
// 		blocking_notifier_call_chain(&dev->bus->p->bus_notifier,
// 					     BUS_NOTIFY_ADD_DEVICE, dev);

// 	kobject_uevent(&dev->kobj, KOBJ_ADD);

// 	/*
// 	 * If all driver registration is done and a newly added device doesn't
// 	 * match with any driver, don't block its consumers from probing in
// 	 * case the consumer device is able to operate without this supplier.
// 	 */
// 	if (dev->fwnode && fw_devlink_drv_reg_done && !dev->can_match)
// 		fw_devlink_unblock_consumers(dev);

// 	if (parent)
// 		klist_add_tail(&dev->p->knode_parent,
// 			       &parent->p->klist_children);

// 	if (dev->class) {
// 		mutex_lock(&dev->class->p->mutex);
// 		/* tie the class to the device */
// 		klist_add_tail(&dev->p->knode_class,
// 			       &dev->class->p->klist_devices);

// 		/* notify any interfaces that the device is here */
// 		list_for_each_entry(class_intf,
// 				    &dev->class->p->interfaces, node)
// 			if (class_intf->add_dev)
// 				class_intf->add_dev(dev, class_intf);
// 		mutex_unlock(&dev->class->p->mutex);
// 	}
done:
// 	put_device(dev);
	return error;
 SysEntryError:
// 	if (MAJOR(dev->devt))
// 		device_remove_file(dev, &dev_attr_dev);
 Error:
// 	cleanup_glue_dir(dev, glue_dir);
parent_error:
// 	put_device(parent);
name_error:
	// kfree(dev->p);
	// dev->p = NULL;
	goto done;
}

/**
 * device_del - delete device from system.
 * @dev: device.
 *
 * This is the first part of the device unregistration
 * sequence. This removes the device from the lists we control
 * from here, has it removed from the other driver model
 * subsystems it was added to in device_add(), and removes it
 * from the kobject hierarchy.
 *
 * NOTE: this should be called manually _iff_ device_add() was
 * also called manually.
 */
void device_del(device_s *dev)
{
	// struct device *parent = dev->parent;
	// struct kobject *glue_dir = NULL;
	// struct class_interface *class_intf;
	// unsigned int noio_flag;

	// device_lock(dev);
	// kill_device(dev);
	// device_unlock(dev);

	// if (dev->fwnode && dev->fwnode->dev == dev)
	// 	dev->fwnode->dev = NULL;

	// /* Notify clients of device removal.  This call must come
	//  * before dpm_sysfs_remove().
	//  */
	// noio_flag = memalloc_noio_save();
	// if (dev->bus)
	// 	blocking_notifier_call_chain(&dev->bus->p->bus_notifier,
	// 				     BUS_NOTIFY_DEL_DEVICE, dev);

	// dpm_sysfs_remove(dev);
	// if (parent)
	// 	klist_del(&dev->p->knode_parent);
	// if (MAJOR(dev->devt)) {
	// 	devtmpfs_delete_node(dev);
	// 	device_remove_sys_dev_entry(dev);
	// 	device_remove_file(dev, &dev_attr_dev);
	// }
	// if (dev->class) {
	// 	device_remove_class_symlinks(dev);

	// 	mutex_lock(&dev->class->p->mutex);
	// 	/* notify any interfaces that the device is now gone */
	// 	list_for_each_entry(class_intf,
	// 			    &dev->class->p->interfaces, node)
	// 		if (class_intf->remove_dev)
	// 			class_intf->remove_dev(dev, class_intf);
	// 	/* remove the device from the class list */
	// 	klist_del(&dev->p->knode_class);
	// 	mutex_unlock(&dev->class->p->mutex);
	// }
	// device_remove_file(dev, &dev_attr_uevent);
	// device_remove_attrs(dev);
	// bus_remove_device(dev);
	// device_pm_remove(dev);
	// driver_deferred_probe_del(dev);
	// device_platform_notify_remove(dev);
	// device_links_purge(dev);

	// if (dev->bus)
	// 	blocking_notifier_call_chain(&dev->bus->p->bus_notifier,
	// 				     BUS_NOTIFY_REMOVED_DEVICE, dev);
	// kobject_uevent(&dev->kobj, KOBJ_REMOVE);
	// glue_dir = get_glue_dir(dev);
	// kobject_del(&dev->kobj);
	// cleanup_glue_dir(dev, glue_dir);
	// memalloc_noio_restore(noio_flag);
	// put_device(parent);
}

device_s *myos_device_create(class_s *class, dev_t devt, const char* devname)
{
	device_s *dev = NULL;
	int retval = -ENODEV;

	dev = kzalloc(sizeof(device_s), GFP_KERNEL);
	if (!dev) {
		retval = -ENOMEM;
		goto error;
	}

	dev->devt = devt;
	dev->class = class;
	dev->kobj.name = devname;

	retval = device_add(dev);
	if (retval)
		goto error;

	return dev;

error:
	// put_device(dev);
	return ERR_PTR(retval);
}
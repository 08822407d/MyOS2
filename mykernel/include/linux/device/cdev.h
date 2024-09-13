/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CDEV_H
#define _LINUX_CDEV_H

	#include <linux/kernel/kobject.h>
	#include <linux/kernel/kdev_t.h>
	#include <linux/device/device.h>
	#include <linux/kernel/lib.h>

	typedef struct cdev {
		kobj_s				kobj;
	// 	struct module *owner;
		const file_ops_s	*ops;
		List_s				list;
		dev_t				dev;
		uint				count;
	} cdev_s;

	void cdev_init(cdev_s *, const file_ops_s *ops);

	cdev_s *cdev_alloc(void);

	// void cdev_put(struct cdev *p);

	int cdev_add(cdev_s *, dev_t, unsigned);

	// void cdev_set_parent(struct cdev *p, struct kobject *kobj);
	// int cdev_device_add(struct cdev *cdev, struct device *dev);
	// void cdev_device_del(struct cdev *cdev, struct device *dev);

	void cdev_del(cdev_s *);

	// void cd_forget(inode_s *);

#endif
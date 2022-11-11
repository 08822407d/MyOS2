// SPDX-License-Identifier: GPL-2.0
/*
 *  gendisk handling
 *
 * Portions Copyright (C) 2020 Christoph Hellwig
 */

// #include <linux/module.h>
#include <linux/kernel/ctype.h>
#include <linux/fs/fs.h>
#include <linux/block/genhd.h>
#include <linux/kernel/kdev_t.h>
#include <linux/kernel/kernel.h>
#include <linux/block/blkdev.h>
// #include <linux/backing-dev.h>
#include <linux/init/init.h>
// #include <linux/spinlock.h>
// #include <linux/proc_fs.h>
// #include <linux/seq_file.h>
#include <linux/kernel/slab.h>
// #include <linux/kmod.h>
#include <uapi/kernel/major.h>
// #include <linux/mutex.h>
// #include <linux/idr.h>
#include <linux/kernel/log2.h>
// #include <linux/pm_runtime.h>
// #include <linux/badblocks.h>
// #include <linux/part_stat.h>

#include "blk.h"
// #include "blk-mq-sched.h"
// #include "blk-rq-qos.h"


/**
 * device_add_disk - add disk information to kernel list
 * @parent: parent device for the disk
 * @disk: per-device partitioning information
 * @groups: Additional per-device sysfs groups
 *
 * This function registers the partitioning information in @disk
 * with the kernel.
 */
int myos_device_add_disk(device_s *parent, gendisk_s *disk)
{
	device_s *ddev = disk_to_dev(disk);
	int ret;

	/*
	 * If the driver provides an explicit major number it also must provide
	 * the number of minors numbers supported, and those will be used to
	 * setup the gendisk.
	 * Otherwise just allocate the device numbers for both the whole device
	 * and all partitions from the extended dev_t space.
	 */
	if (disk->major) {
		if (!disk->minors)
			return -EINVAL;

		if (disk->minors > DISK_MAX_PARTS) {
			// pr_err("block: can't allocate more than %d partitions\n",
			// 		DISK_MAX_PARTS);
			disk->minors = DISK_MAX_PARTS;
		}
		if (disk->first_minor + disk->minors > MINORMASK + 1)
			return -EINVAL;
	} else {
		if (disk->minors)
			return -EINVAL;

		// ret = blk_alloc_ext_minor();
		// if (ret < 0)
		// 	return ret;
		disk->major = BLOCK_EXT_MAJOR;
		disk->first_minor = ret;
	}

	ddev->kobj.name = disk->disk_name;
	// if (!(disk->flags & GENHD_FL_HIDDEN))
	// 	ddev->devt = MKDEV(disk->major, disk->first_minor);
	ret = device_add(ddev);

	return ret;
}

class_s block_class = {
	.name		= "block",
	// .dev_uevent	= block_uevent,
};


gendisk_s *__alloc_disk_node(request_queue_s *q)
{
	gendisk_s *disk;
	if (q == NULL)
		return NULL;

	disk = kzalloc(sizeof(gendisk_s), GFP_KERNEL);
	if (disk == NULL)
		return ERR_PTR(-ENOMEM);

	disk->queue = q;

	disk->part0 = bdev_alloc(disk, 0);
	if (disk->part0 == NULL)
		goto out_free_disk;

out_free_disk:
	kfree(disk);
	return NULL;
}
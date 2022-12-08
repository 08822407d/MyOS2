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


void set_capacity(gendisk_s *disk, sector_t sectors)
{
	blk_dev_s *bdev = disk->part0;

	// spin_lock(&bdev->bd_size_lock);
	// i_size_write(bdev->bd_inode, (loff_t)sectors << SECTOR_SHIFT);
	bdev->bd_nr_sectors = sectors;
	// spin_unlock(&bdev->bd_size_lock);
}


int disk_scan_partitions(gendisk_s *disk, fmode_t mode)
{
	blk_dev_s *bdev;

	// if (disk->flags & (GENHD_FL_NO_PART | GENHD_FL_HIDDEN))
	// 	return -EINVAL;
	// if (disk->open_partitions)
	// 	return -EBUSY;

	// set_bit(GD_NEED_PART_SCAN, &disk->state);
	bdev = blkdev_get_by_dev(disk_devt(disk), mode);
	if (IS_ERR(bdev))
		return PTR_ERR(bdev);
	// blkdev_put(bdev, mode);
	return 0;
}

/**
 * device_add_disk - add disk information to kernel list
 * @parent: parent device for the disk
 * @disk: per-device partitioning information
 * @groups: Additional per-device sysfs groups
 *
 * This function registers the partitioning information in @disk
 * with the kernel.
 */
int myos_device_add_disk(gendisk_s *disk)
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
		ddev->devt = MKDEV(disk->major, disk->first_minor);
	ret = device_add(ddev);


	bdev_add(disk->part0, ddev->devt);
	if (get_capacity(disk))
		disk_scan_partitions(disk, FMODE_READ);


	return ret;
}

class_s block_class = {
	.name		= "block",
	// .dev_uevent	= block_uevent,
};


gendisk_s *__alloc_disk_node(req_queue_s *q)
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

gendisk_s *__myos_init_disk(gendisk_s *disk, req_queue_s *q)
{
	// disk->bdi = bdi_alloc(node_id);
	// if (!disk->bdi)
	// 	goto out_free_disk;

	/* bdev_alloc() might need the queue, set before the first call */
	disk->queue = q;

	disk->part0 = bdev_alloc(disk, 0);
	if (!disk->part0)
		goto out_free_bdi;
	// disk->node_id = node_id;
	// mutex_init(&disk->open_mutex);
	// xa_init(&disk->part_tbl);
	// if (xa_insert(&disk->part_tbl, 0, disk->part0, GFP_KERNEL))
	// 	goto out_destroy_part_tbl;

	// rand_initialize_disk(disk);
	disk_to_dev(disk)->class = &block_class;
	// disk_to_dev(disk)->type = &disk_type;
	// device_initialize(disk_to_dev(disk));
	// inc_diskseq(disk);
	// q->disk = disk;
	// lockdep_init_map(&disk->lockdep_map, "(bio completion)", lkclass, 0);
// #ifdef CONFIG_BLOCK_HOLDER_DEPRECATED
	// INIT_LIST_HEAD(&disk->slave_bdevs);
// #endif
	return disk;

out_destroy_part_tbl:
	// xa_destroy(&disk->part_tbl);
	// disk->part0->bd_disk = NULL;
	// iput(disk->part0->bd_inode);
out_free_bdi:
	// bdi_put(disk->bdi);
out_free_disk:
	kfree(disk);
	return NULL;
}

gendisk_s *__myos_alloc_disk(req_queue_s *q)
{
	gendisk_s *disk;

// 	if (!blk_get_queue(q))
// 		return NULL;

	disk = kzalloc(sizeof(struct gendisk), GFP_KERNEL);
	if (!disk)
		goto out_put_queue;

	disk = __myos_init_disk(disk, q);

out_put_queue:
	// blk_put_queue(q);
	return disk;
}
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
// #include <uapi/kernel/major.h>
// #include <linux/mutex.h>
// #include <linux/idr.h>
// #include <linux/log2.h>
// #include <linux/pm_runtime.h>
// #include <linux/badblocks.h>
// #include <linux/part_stat.h>

#include <linux/block/blk.h>
// #include "blk-mq-sched.h"
// #include "blk-rq-qos.h"


#include <obsolete/proto.h>


class_s block_class = {
	.name		= "block",
	// .dev_uevent	= block_uevent,
};


gendisk_s *__alloc_disk_node(request_queue_s *q)
{
	gendisk_s *disk;
	if (q == NULL)
		return NULL;

	disk = myos_kmalloc(sizeof(gendisk_s));
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
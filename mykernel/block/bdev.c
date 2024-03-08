// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2001  Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2016 - 2020 Christoph Hellwig
 */

#include <linux/init/init.h>
#include <linux/kernel/mm.h>
// #include <linux/kernel/slab.h>
// #include <linux/kmod.h>
#include <uapi/linux/major.h>
// #include <linux/device_cgroup.h>
#include <linux/block/blkdev.h>
// #include <linux/blk-integrity.h>
// #include <linux/backing-dev.h>
// #include <linux/module.h>
// #include <linux/blkpg.h>
#include <uapi/linux/magic.h>
// #include <linux/block/buffer_head.h>
// #include <linux/swap.h>
// #include <linux/writeback.h>
#include <linux/kernel/mount.h>
#include <linux/fs/pseudo_fs.h>
// #include <linux/uio.h>
#include <linux/fs/namei.h>
// #include <linux/part_stat.h>
#include <linux/kernel/uaccess.h>
#include <linux/fs/internal.h>
#include "blk.h"


#include <obsolete/printk.h>

typedef struct bdev_inode {
	blk_dev_s bdev;
	inode_s vfs_inode;
} bdev_inode_s;

static inline bdev_inode_s *BDEV_I(inode_s *inode)
{
	return container_of(inode, bdev_inode_s, vfs_inode);
}

blk_dev_s *I_BDEV(inode_s *inode)
{
	return &BDEV_I(inode)->bdev;
}

static inode_s *bdev_alloc_inode(super_block_s *sb)
{
	bdev_inode_s *ei = kmalloc(sizeof(bdev_inode_s), GFP_KERNEL);

	if (!ei)
		return NULL;
	memset(&ei->bdev, 0, sizeof(ei->bdev));
	return &ei->vfs_inode;
}

static void bdev_free_inode(inode_s *inode)
{
	blk_dev_s *bdev = I_BDEV(inode);

	// if (!bdev_is_partition(bdev)) {
	// 	if (bdev->bd_disk && bdev->bd_disk->bdi)
	// 		bdi_put(bdev->bd_disk->bdi);
	// 	kfree(bdev->bd_disk);
	// }

	// if (MAJOR(bdev->bd_dev) == BLOCK_EXT_MAJOR)
	// 	blk_free_ext_minor(MINOR(bdev->bd_dev));

	kfree(BDEV_I(inode));
}

static const super_ops_s bdev_sops = {
	// .statfs			= simple_statfs,
	.alloc_inode	= bdev_alloc_inode,
	.free_inode		= bdev_free_inode,
	// .drop_inode		= generic_delete_inode,
	// .evict_inode	= bdev_evict_inode,
};

static int bd_init_fs_context(fs_ctxt_s *fc)
{
	pseudo_fs_ctxt_s *ctx = init_pseudo(fc, BDEVFS_MAGIC);
	if (!ctx)
		return -ENOMEM;
	fc->s_iflags |= SB_I_CGROUPWB;
	ctx->ops = &bdev_sops;
	return 0;
}

static fs_type_s bd_type = {
	.name				= "bdev",
	.init_fs_context	= bd_init_fs_context,
	.kill_sb			= kill_anon_super,
};

super_block_s *blockdev_superblock;

void bdev_cache_init(void)
{
	int err;
	static vfsmount_s *bd_mnt;

	list_hdr_init(&bd_type.fs_supers);
	err = register_filesystem(&bd_type);
	if (err)
		color_printk(RED, BLACK, "Cannot register bdev pseudo-fs");
	bd_mnt = kern_mount(&bd_type);
	if (IS_ERR(bd_mnt))
		color_printk(RED, BLACK, "Cannot create bdev pseudo-fs");
	blockdev_superblock = bd_mnt->mnt_sb;   /* For writeback */
}

blk_dev_s *bdev_alloc(gendisk_s *disk, uint8_t partno)
{
	blk_dev_s *bdev;
	inode_s *inode;

	inode = new_inode(blockdev_superblock);
	if (inode == NULL)
		return NULL;
	inode->i_mode = S_IFBLK;
	inode->i_rdev = 0;
	// inode->i_data.a_ops = &def_blk_aops;

	bdev = I_BDEV(inode);
	bdev->bd_partno = partno;
	bdev->bd_inode = inode;
	bdev->bd_queue = disk->queue;
	// bdev->bd_stats = alloc_percpu(struct disk_stats);
	// if (!bdev->bd_stats) {
	// 	iput(inode);
	// 	return NULL;
	// }
	bdev->bd_disk = disk;
	return bdev;
}

void bdev_add(blk_dev_s *bdev, dev_t dev)
{
	bdev->bd_dev = dev;
	bdev->bd_inode->i_rdev = dev;
	bdev->bd_inode->i_ino = dev;
	// insert_inode_hash(bdev->bd_inode);
}



static int blkdev_get_whole(blk_dev_s *bdev, fmode_t mode)
{
	gendisk_s *disk = bdev->bd_disk;
	int ret;

	if (disk->fops->open) {
		ret = disk->fops->open(bdev, mode);
	// 	if (ret) {
	// 		/* avoid ghost partitions on a removed medium */
	// 		if (ret == -ENOMEDIUM &&
	// 		     test_bit(GD_NEED_PART_SCAN, &disk->state))
	// 			bdev_disk_changed(disk, true);
	// 		return ret;
	// 	}
	}

	// if (!bdev->bd_openers)
	// 	set_init_blocksize(bdev);
	// if (test_bit(GD_NEED_PART_SCAN, &disk->state))
		bdev_disk_changed(disk, false);
	// bdev->bd_openers++;
	return 0;;
}

static void blkdev_put_whole(blk_dev_s *bdev, fmode_t mode)
{
	// if (!--bdev->bd_openers)
	// 	blkdev_flush_mapping(bdev);
	// if (bdev->bd_disk->fops->release)
	// 	bdev->bd_disk->fops->release(bdev->bd_disk, mode);
}

static int blkdev_get_part(blk_dev_s *part, fmode_t mode)
{
	// gendisk_s *disk = part->bd_disk;
	int ret;

	// if (part->bd_openers)
	// 	goto done;

	ret = blkdev_get_whole(bdev_whole(part), mode);
	if (ret)
		return ret;

	ret = -ENXIO;
// 	if (!bdev_nr_sectors(part))
// 		goto out_blkdev_put;

// 	disk->open_partitions++;
// 	set_init_blocksize(part);
// done:
// 	part->bd_openers++;
// 	return 0;

out_blkdev_put:
	blkdev_put_whole(bdev_whole(part), mode);
	return ret;
}

static void blkdev_put_part(blk_dev_s *part, fmode_t mode)
{
	// struct block_device *whole = bdev_whole(part);

	// if (--part->bd_openers)
	// 	return;
	// blkdev_flush_mapping(part);
	// whole->bd_disk->open_partitions--;
	// blkdev_put_whole(whole, mode);
}

blk_dev_s *blkdev_get_no_open(dev_t dev)
{
	blk_dev_s *bdev;
	inode_s *inode;

	// inode = ilookup(blockdev_superblock, dev);
	// if (!inode) {
	// 	blk_request_module(dev);
		inode = ilookup(blockdev_superblock, dev);
		if (!inode)
			return NULL;
	// }

	/* switch from the inode reference to a device mode one: */
	bdev = &BDEV_I(inode)->bdev;
	// if (!kobject_get_unless_zero(&bdev->bd_device.kobj))
	// 	bdev = NULL;
	iput(inode);
	return bdev;
}

void blkdev_put_no_open(blk_dev_s *bdev)
{
	// put_device(&bdev->bd_device);
}

/**
 * blkdev_get_by_dev - open a block device by device number
 * @dev: device number of block device to open
 * @mode: FMODE_* mask
 * @holder: exclusive holder identifier
 *
 * Open the block device described by device number @dev. If @mode includes
 * %FMODE_EXCL, the block device is opened with exclusive access.  Specifying
 * %FMODE_EXCL with a %NULL @holder is invalid.  Exclusive opens may nest for
 * the same @holder.
 *
 * Use this interface ONLY if you really do not have anything better - i.e. when
 * you are behind a truly sucky interface and all you are given is a device
 * number.  Everything else should use blkdev_get_by_path().
 *
 * CONTEXT:
 * Might sleep.
 *
 * RETURNS:
 * Reference to the block_device on success, ERR_PTR(-errno) on failure.
 */
blk_dev_s *blkdev_get_by_dev(dev_t dev, fmode_t mode)
{
	// bool unblock_events = true;
	blk_dev_s *bdev;
	gendisk_s *disk;
	int ret;

// 	ret = devcgroup_check_permission(DEVCG_DEV_BLOCK,
// 			MAJOR(dev), MINOR(dev),
// 			((mode & FMODE_READ) ? DEVCG_ACC_READ : 0) |
// 			((mode & FMODE_WRITE) ? DEVCG_ACC_WRITE : 0));
// 	if (ret)
// 		return ERR_PTR(ret);

	bdev = blkdev_get_no_open(dev);
	if (!bdev)
		return ERR_PTR(-ENXIO);
	disk = bdev->bd_disk;

	// if (mode & FMODE_EXCL) {
	// 	ret = bd_prepare_to_claim(bdev, holder);
	// 	if (ret)
	// 		goto put_blkdev;
	// }

	// disk_block_events(disk);

	// mutex_lock(&disk->open_mutex);
	// ret = -ENXIO;
	// if (!disk_live(disk))
	// 	goto abort_claiming;
	// if (!try_module_get(disk->fops->owner))
	// 	goto abort_claiming;
	if (bdev_is_partition(bdev))
		ret = blkdev_get_part(bdev, mode);
	else
		ret = blkdev_get_whole(bdev, mode);
	// if (ret)
	// 	goto put_module;
	// if (mode & FMODE_EXCL) {
	// 	bd_finish_claiming(bdev, holder);

	// 	/*
	// 	 * Block event polling for write claims if requested.  Any write
	// 	 * holder makes the write_holder state stick until all are
	// 	 * released.  This is good enough and tracking individual
	// 	 * writeable reference is too fragile given the way @mode is
	// 	 * used in blkdev_get/put().
	// 	 */
	// 	if ((mode & FMODE_WRITE) && !bdev->bd_write_holder &&
	// 	    (disk->event_flags & DISK_EVENT_FLAG_BLOCK_ON_EXCL_WRITE)) {
	// 		bdev->bd_write_holder = true;
	// 		unblock_events = false;
	// 	}
	// }
	// mutex_unlock(&disk->open_mutex);

	// if (unblock_events)
	// 	disk_unblock_events(disk);
	return bdev;
put_module:
	// module_put(disk->fops->owner);
abort_claiming:
	// if (mode & FMODE_EXCL)
	// 	bd_abort_claiming(bdev, holder);
	// mutex_unlock(&disk->open_mutex);
	// disk_unblock_events(disk);
put_blkdev:
	blkdev_put_no_open(bdev);
	return ERR_PTR(ret);
}

/**
 * blkdev_get_by_path - open a block device by name
 * @path: path to the block device to open
 * @mode: FMODE_* mask
 * @holder: exclusive holder identifier
 *
 * Open the block device described by the device file at @path.  If @mode
 * includes %FMODE_EXCL, the block device is opened with exclusive access.
 * Specifying %FMODE_EXCL with a %NULL @holder is invalid.  Exclusive opens may
 * nest for the same @holder.
 *
 * CONTEXT:
 * Might sleep.
 *
 * RETURNS:
 * Reference to the block_device on success, ERR_PTR(-errno) on failure.
 */
blk_dev_s *blkdev_get_by_path(const char *path, fmode_t mode)
{
	// struct block_device *bdev;
	// dev_t dev;
	// int error;

	// error = lookup_bdev(path, &dev);
	// if (error)
	// 	return ERR_PTR(error);

	// bdev = blkdev_get_by_dev(dev, mode);
	// if (!IS_ERR(bdev) && (mode & FMODE_WRITE) && bdev_read_only(bdev)) {
	// 	blkdev_put(bdev, mode);
	// 	return ERR_PTR(-EACCES);
	// }

	// return bdev;
}
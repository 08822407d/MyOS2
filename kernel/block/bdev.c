// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2001  Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2016 - 2020 Christoph Hellwig
 */

#include <linux/init/init.h>
#include <linux/mm/mm.h>
// #include <linux/slab.h>
// #include <linux/kmod.h>
// #include <uapi/kernel/major.h>
// #include <linux/device_cgroup.h>
#include <linux/block/blkdev.h>
// #include <linux/blk-integrity.h>
// #include <linux/backing-dev.h>
// #include <linux/module.h>
// #include <linux/blkpg.h>
// #include <linux/magic.h>
#include <linux/block/buffer_head.h>
// #include <linux/swap.h>
// #include <linux/writeback.h>
// #include <linux/mount.h>
#include <linux/fs/pseudo_fs.h>
// #include <linux/uio.h>
#include <linux/fs/namei.h>
// #include <linux/part_stat.h>
// #include <linux/uaccess.h>
// #include "../fs/internal.h"
// #include "blk.h"


#include <linux/fs/fs.h>
#include <uapi/kernel/stat.h>
#include <uapi/kernel/magic.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>

typedef struct bdev_inode {
	block_device_s bdev;
	inode_s vfs_inode;
} bdev_inode_s;

static inline bdev_inode_s *BDEV_I(inode_s *inode)
{
	return container_of(inode, bdev_inode_s, vfs_inode);
}

block_device_s *I_BDEV(inode_s *inode)
{
	return &BDEV_I(inode)->bdev;
}

static inode_s *bdev_alloc_inode(super_block_s *sb)
{
	bdev_inode_s *ei = kmalloc(sizeof(bdev_inode_s));

	if (!ei)
		return NULL;
	memset(&ei->bdev, 0, sizeof(ei->bdev));
	return &ei->vfs_inode;
}

static void bdev_free_inode(inode_s *inode)
{
	block_device_s *bdev = I_BDEV(inode);

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

block_device_s *bdev_alloc(gendisk_s *disk, uint8_t partno)
{
	block_device_s *bdev;
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
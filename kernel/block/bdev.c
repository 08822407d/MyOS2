#include <linux/kernel/blkdev.h>
#include <linux/fs/fs.h>

#include <uapi/stat.h>

#include <stddef.h>

#include <include/proto.h>

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

// static int bd_init_fs_context(fs_context_s *fc)
// {
// 	struct pseudo_fs_context *ctx = init_pseudo(fc, BDEVFS_MAGIC);
// 	if (!ctx)
// 		return -ENOMEM;
// 	fc->s_iflags |= SB_I_CGROUPWB;
// 	ctx->ops = &bdev_sops;
// 	return 0;
// }

static fs_type_s bd_type = {
	.name				= "bdev",
	// .init_fs_context	= bd_init_fs_context,
	.kill_sb			= kill_anon_super,
};

super_block_s *blockdev_superblock;

void bdev_cache_init(void)
{
	int err;
	static vfsmount_s *bd_mnt;

	// bdev_cachep = kmem_cache_create("bdev_cache", sizeof(struct bdev_inode),
	// 		0, (SLAB_HWCACHE_ALIGN|SLAB_RECLAIM_ACCOUNT|
	// 			SLAB_MEM_SPREAD|SLAB_ACCOUNT|SLAB_PANIC),
	// 		init_once);
	err = register_filesystem(&bd_type);
	// if (err)
	// 	panic("Cannot register bdev pseudo-fs");
	bd_mnt = kern_mount(&bd_type);
	// if (IS_ERR(bd_mnt))
	// 	panic("Cannot create bdev pseudo-fs");
	blockdev_superblock = bd_mnt->mnt_sb;   /* For writeback */
}

block_device_s *bdev_alloc(gendisk_s *disk, uint8_t partno)
{
	block_device_s *bdev;
	inode_s *inode;

	inode = new_inode(blockdev_superblock);
	if (!inode)
		return NULL;
	inode->i_mode = S_IFBLK;
	inode->i_rdev = 0;
	// inode->i_data.a_ops = &def_blk_aops;

	bdev = I_BDEV(inode);
	bdev->bd_partno = partno;
	bdev->bd_inode = inode;
	// bdev->bd_queue = disk->queue;
	// bdev->bd_stats = alloc_percpu(struct disk_stats);
	// if (!bdev->bd_stats) {
	// 	iput(inode);
	// 	return NULL;
	// }
	// bdev->bd_disk = disk;
	return bdev;
}
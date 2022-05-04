// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/super.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  super.c contains code to handle: - mount structures
 *                                   - super-block tables
 *                                   - filesystem drivers list
 *                                   - mount system call
 *                                   - umount system call
 *                                   - ustat system call
 *
 * GK 2/5/95  -  Changed to support mounting the root fs via NFS
 *
 *  Added kerneld support: Jacques Gelinas and Bjorn Ekwall
 *  Added change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Added options to /proc/mounts:
 *    Torbjörn Lindh (torbjorn.lindh@gopta.se), April 14, 1996.
 *  Added devfs support: Richard Gooch <rgooch@atnf.csiro.au>, 13-JAN-1998
 *  Heavily rewritten for 'one fs - one tree' dcache architecture. AV, Mar 2000
 */

// #include <linux/export.h>
// #include <linux/slab.h>
#include <linux/kernel/blkdev.h>
#include <linux/kernel/mount.h>
// #include <linux/security.h>
// #include <linux/writeback.h>		/* for the emergency remount stuff */
// #include <linux/idr.h>
// #include <linux/mutex.h>
// #include <linux/backing-dev.h>
// #include <linux/rculist_bl.h>
// #include <linux/fscrypt.h>
// #include <linux/fsnotify.h>
// #include <linux/lockdep.h>
// #include <linux/user_namespace.h>
#include <linux/fs/fs_context.h>
#include <uapi/mount.h>
#include <linux/fs/internal.h>


#include <include/printk.h>
#include <linux/fs/fs.h>
#include <errno.h>

/**
 * get_anon_bdev - Allocate a block device for filesystems which don't have one.
 * @p: Pointer to a dev_t.
 *
 * Filesystems which don't use real block devices can call this function
 * to allocate a virtual block device.
 *
 * Context: Any context.  Frequently called while holding sb_lock.
 * Return: 0 on success, -EMFILE if there are no anonymous bdevs left
 * or -ENOMEM if memory allocation failed.
 */
int get_anon_bdev(dev_t *p)
{
	int dev;

	/*
	 * Many userspace utilities consider an FSID of 0 invalid.
	 * Always return at least 1 from get_anon_bdev.
	 */
	// dev = ida_alloc_range(&unnamed_dev_ida, 1, (1 << MINORBITS) - 1,
	// 		GFP_ATOMIC);
	if (dev == -ENOSPC)
		dev = -EMFILE;
	if (dev < 0)
		return dev;

	// *p = MKDEV(0, dev);
	return 0;
}

void free_anon_bdev(dev_t dev)
{
	// ida_free(&unnamed_dev_ida, MINOR(dev));
}

int set_anon_super(super_block_s *s, void *data)
{
	return get_anon_bdev(&s->s_dev);
}

void kill_anon_super(super_block_s *sb)
{
	dev_t dev = sb->s_dev;
	// generic_shutdown_super(sb);
	free_anon_bdev(dev);
}

void kill_litter_super(super_block_s *sb)
{
	// if (sb->s_root)
	// 	d_genocide(sb->s_root);
	kill_anon_super(sb);
}

/**
 * vfs_get_super - Get a superblock with a search key set in s_fs_info.
 * @fc: The filesystem context holding the parameters
 * @keying: How to distinguish superblocks
 * @fill_super: Helper to initialise a new superblock
 *
 * Search for a superblock and create a new one if not found.  The search
 * criterion is controlled by @keying.  If the search fails, a new superblock
 * is created and @fill_super() is called to initialise it.
 *
 * @keying can take one of a number of values:
 *
 * (1) vfs_get_single_super - Only one superblock of this type may exist on the
 *     system.  This is typically used for special system filesystems.
 *
 * (2) vfs_get_keyed_super - Multiple superblocks may exist, but they must have
 *     distinct keys (where the key is in s_fs_info).  Searching for the same
 *     key again will turn up the superblock for that key.
 *
 * (3) vfs_get_independent_super - Multiple superblocks may exist and are
 *     unkeyed.  Each call will get a new superblock.
 *
 * A permissions check is made by sget_fc() unless we're getting a superblock
 * for a kernel-internal mount or a submount.
 */
int vfs_get_super(fs_ctxt_s *fc,
				enum vfs_get_super_keying keying,
				int (*fill_super)(super_block_s *sb,
						fs_ctxt_s *fc))
{
	// int (*test)(super_block_s *, fs_ctxt_s *);
	// super_block_s *sb;
	// int err;

	// switch (keying) {
	// case vfs_get_single_super:
	// case vfs_get_single_reconf_super:
	// 	test = test_single_super;
	// 	break;
	// case vfs_get_keyed_super:
	// 	test = test_keyed_super;
	// 	break;
	// case vfs_get_independent_super:
	// 	test = NULL;
	// 	break;
	// default:
	// 	BUG();
	// }

// 	sb = sget_fc(fc, test, set_anon_super_fc);
// 	if (IS_ERR(sb))
// 		return PTR_ERR(sb);

// 	if (!sb->s_root) {
// 		err = fill_super(sb, fc);
// 		if (err)
// 			goto error;

// 		sb->s_flags |= SB_ACTIVE;
// 		fc->root = dget(sb->s_root);
// 	} else {
// 		fc->root = dget(sb->s_root);
// 		if (keying == vfs_get_single_reconf_super) {
// 			err = reconfigure_super(fc);
// 			if (err < 0) {
// 				dput(fc->root);
// 				fc->root = NULL;
// 				goto error;
// 			}
// 		}
// 	}

// 	return 0;

// error:
// 	deactivate_locked_super(sb);
// 	return err;
}

int get_tree_nodev(fs_ctxt_s *fc,
				int (*fill_super)(super_block_s *sb,
						fs_ctxt_s *fc))
{
	return vfs_get_super(fc, vfs_get_independent_super, fill_super);
}

static int set_bdev_super(super_block_s *s, void *data)
{
	s->s_bdev = data;
	s->s_dev = s->s_bdev->bd_dev;
	// s->s_bdi = bdi_get(s->s_bdev->bd_disk->bdi);

	// if (blk_queue_stable_writes(s->s_bdev->bd_disk->queue))
	// 	s->s_iflags |= SB_I_STABLE_WRITES;
	return 0;
}

static int test_bdev_super(super_block_s *s, void *data)
{
	return (void *)s->s_bdev == data;
}

dentry_s *mount_bdev(fs_type_s *fs_type, int flags,
				const char *dev_name, void *data,
				int (*fill_super)(super_block_s *, void *, int))
{
// 	block_device_s *bdev;
// 	super_block_s *s;
// 	fmode_t mode = FMODE_READ | FMODE_EXCL;
// 	int error = 0;

// 	if (!(flags & SB_RDONLY))
// 		mode |= FMODE_WRITE;

// 	bdev = blkdev_get_by_path(dev_name, mode, fs_type);
// 	if (IS_ERR(bdev))
// 		return ERR_CAST(bdev);

// 	/*
// 	 * once the super is inserted into the list by sget, s_umount
// 	 * will protect the lockfs code from trying to start a snapshot
// 	 * while we are mounting
// 	 */
// 	mutex_lock(&bdev->bd_fsfreeze_mutex);
// 	if (bdev->bd_fsfreeze_count > 0) {
// 		mutex_unlock(&bdev->bd_fsfreeze_mutex);
// 		error = -EBUSY;
// 		goto error_bdev;
// 	}
// 	s = sget(fs_type, test_bdev_super, set_bdev_super, flags | SB_NOSEC,
// 		 bdev);
// 	mutex_unlock(&bdev->bd_fsfreeze_mutex);
// 	if (IS_ERR(s))
// 		goto error_s;

// 	if (s->s_root) {
// 		if ((flags ^ s->s_flags) & SB_RDONLY) {
// 			deactivate_locked_super(s);
// 			error = -EBUSY;
// 			goto error_bdev;
// 		}

// 		/*
// 		 * s_umount nests inside open_mutex during
// 		 * __invalidate_device().  blkdev_put() acquires
// 		 * open_mutex and can't be called under s_umount.  Drop
// 		 * s_umount temporarily.  This is safe as we're
// 		 * holding an active reference.
// 		 */
// 		up_write(&s->s_umount);
// 		blkdev_put(bdev, mode);
// 		down_write(&s->s_umount);
// 	} else {
// 		s->s_mode = mode;
// 		snprintf(s->s_id, sizeof(s->s_id), "%pg", bdev);
// 		sb_set_blocksize(s, block_size(bdev));
// 		error = fill_super(s, data, flags & SB_SILENT ? 1 : 0);
// 		if (error) {
// 			deactivate_locked_super(s);
// 			goto error;
// 		}

// 		s->s_flags |= SB_ACTIVE;
// 		bdev->bd_super = s;
// 	}

// 	return dget(s->s_root);

// error_s:
// 	error = PTR_ERR(s);
// error_bdev:
// 	blkdev_put(bdev, mode);
// error:
// 	return ERR_PTR(error);
}

/**
 * vfs_get_tree - Get the mountable root
 * @fc: The superblock configuration context.
 *
 * The filesystem is invoked to get or create a superblock which can then later
 * be used for mounting.  The filesystem places a pointer to the root to be
 * used for mounting in @fc->root.
 */
int vfs_get_tree(fs_ctxt_s *fc)
{
	super_block_s *sb;
	int error;

	if (fc->root)
		return -EBUSY;

	/* Get the mountable root in fc->root, with a ref on the root and a ref
	 * on the superblock.
	 */
	error = fc->ops->get_tree(fc);
	if (error < 0)
		return error;

	if (!fc->root) {
		color_printk(RED, BLACK, "Filesystem %s get_tree() didn't set fc->root\n",
		       fc->fs_type->name);
		/* We don't know what the locking state of the superblock is -
		 * if there is a superblock.
		 */
	}

	sb = fc->root->d_sb;
	/*
	 * Write barrier is for super_cache_count(). We place it before setting
	 * SB_BORN as the data dependency between the two functions is the
	 * superblock structure contents that we just set up, not the SB_BORN
	 * flag.
	 */
	sb->s_flags |= SB_BORN;

	// error = security_sb_set_mnt_opts(sb, fc->security, 0, NULL);
	// if (unlikely(error)) {
	// 	fc_drop_locked(fc);
	// 	return error;
	// }
	return 0;
}
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

#include <linux/kernel/export.h>
#include <linux/kernel/mm_api.h>
#include <linux/block/blkdev.h>
#include <linux/kernel/mount.h>
// #include <linux/security.h>
// #include <linux/writeback.h>		/* for the emergency remount stuff */
// #include <linux/idr.h>
// #include <linux/kernel/mutex.h>
// #include <linux/backing-dev.h>
// #include <linux/rculist_bl.h>
// #include <linux/fscrypt.h>
// #include <linux/fsnotify.h>
// #include <linux/lockdep.h>
// #include <linux/user_namespace.h>
#include <linux/fs/fs.h>
#include <linux/fs/fs_context.h>
#include <uapi/linux/mount.h>
#include <linux/fs/internal.h>


#include <obsolete/printk.h>

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
 *	alloc_super	-	create new superblock
 *	@type:	filesystem type superblock should belong to
 *	@flags: the mount flags
 *	@user_ns: User namespace for the super_block
 *
 *	Allocates and initializes a new &struct super_block.  alloc_super()
 *	returns a pointer new superblock or %NULL if allocation had failed.
 */
super_block_s *alloc_super(fs_type_s *type, int flags)
{
	super_block_s *s = kzalloc(sizeof(super_block_s), GFP_KERNEL);
	static const super_ops_s default_op;
	int i;

	if (s == NULL)
		return ERR_PTR(-ENOMEM);

	// INIT_LIST_HEAD(&s->s_mounts);
	// s->s_user_ns = get_user_ns(user_ns);
	// init_rwsem(&s->s_umount);
	// lockdep_set_class(&s->s_umount, &type->s_umount_key);
	// /*
	//  * sget() can have s_umount recursion.
	//  *
	//  * When it cannot find a suitable sb, it allocates a new
	//  * one (this one), and tries again to find a suitable old
	//  * one.
	//  *
	//  * In case that succeeds, it will acquire the s_umount
	//  * lock of the old one. Since these are clearly distrinct
	//  * locks, and this object isn't exposed yet, there's no
	//  * risk of deadlocks.
	//  *
	//  * Annotate this by putting this lock in a different
	//  * subclass.
	//  */
	// down_write_nested(&s->s_umount, SINGLE_DEPTH_NESTING);

	// if (security_sb_alloc(s))
	// 	goto fail;

	// for (i = 0; i < SB_FREEZE_LEVELS; i++) {
	// 	if (__percpu_init_rwsem(&s->s_writers.rw_sem[i],
	// 				sb_writers_name[i],
	// 				&type->s_writers_key[i]))
	// 		goto fail;
	// }
	// init_waitqueue_head(&s->s_writers.wait_unfrozen);
	// s->s_bdi = &noop_backing_dev_info;
	s->s_flags = flags;
	// if (s->s_user_ns != &init_user_ns)
	// 	s->s_iflags |= SB_I_NODEV;
	// INIT_HLIST_NODE(&s->s_instances);
	// INIT_HLIST_BL_HEAD(&s->s_roots);
	// mutex_init(&s->s_sync_lock);
	// INIT_LIST_HEAD(&s->s_inodes);
	INIT_LIST_HEADER_S(&s->s_inodes);
	// spin_lock_init(&s->s_inode_list_lock);
	// INIT_LIST_HEAD(&s->s_inodes_wb);
	// spin_lock_init(&s->s_inode_wblist_lock);

	s->s_count = 1;
	s->s_maxbytes = MAX_NON_LFS;
	s->s_op = &default_op;
	// s->s_time_gran = 1000000000;
	// s->s_time_min = TIME64_MIN;
	// s->s_time_max = TIME64_MAX;

	return s;
}

/* Superblock refcounting  */

/*
 * Drop a superblock's refcount.  The caller must hold sb_lock.
 */
static void __put_super(super_block_s *s) {
	if (!--s->s_count) {
		// list_del_init(&s->s_list);
		// WARN_ON(s->s_dentry_lru.node);
		// WARN_ON(s->s_inode_lru.node);
		// WARN_ON(!list_is_empty_entry(&s->s_mounts));
		// security_sb_free(s);
		// fscrypt_sb_free(s);
		// put_user_ns(s->s_user_ns);
		// kfree(s->s_subtype);
		// call_rcu(&s->rcu, destroy_super_rcu);
	}
}

/**
 *	put_super	-	drop a temporary reference to superblock
 *	@sb: superblock in question
 *
 *	Drops a temporary reference, frees superblock if there's no
 *	references left.
 */
void put_super(super_block_s *sb)
{
	// spin_lock(&sb_lock);
	__put_super(sb);
	// spin_unlock(&sb_lock);
}

/**
 * sget_fc - Find or create a superblock
 * @fc:	Filesystem context.
 * @test: Comparison callback
 * @set: Setup callback
 *
 * Find or create a superblock using the parameters stored in the filesystem
 * context and the two callback functions.
 *
 * If an extant superblock is matched, then that will be returned with an
 * elevated reference count that the caller must transfer or discard.
 *
 * If no match is made, a new superblock will be allocated and basic
 * initialisation will be performed (s_type, s_fs_info and s_id will be set and
 * the set() callback will be invoked), the superblock will be published and it
 * will be returned in a partially constructed state with SB_BORN and SB_ACTIVE
 * as yet unset.
 */
super_block_s *sget_fc(fs_ctxt_s *fc)
{
	super_block_s *s = NULL;
	super_block_s *old;
	int err;

retry:
	// spin_lock(&sb_lock);
	List_s *s_lp = &fc->fs_type->fs_supers.anchor;
	for (List_s *lp = s_lp->next; lp != s_lp; lp = lp->next) {
		super_block_s *sbp = container_of(lp, super_block_s, s_instances);
	}
	if (s == NULL) {
		// spin_unlock(&sb_lock);
		s = alloc_super(fc->fs_type, fc->sb_flags);
		if (s == NULL)
			return ERR_PTR(-ENOMEM);
	}

	s->s_fs_info = fc->s_fs_info;
	// err = set(s, fc);
	// if (err) {
	// 	s->s_fs_info = NULL;
	// 	spin_unlock(&sb_lock);
	// 	destroy_unused_super(s);
	// 	return ERR_PTR(err);
	// }
	fc->s_fs_info = NULL;
	s->s_type = fc->fs_type;
	s->s_iflags |= fc->s_iflags;
	// strscpy(s->s_id, s->s_type->name, sizeof(s->s_id));
	// /*
	//  * Make the superblock visible on @super_blocks and @fs_supers.
	//  * It's in a nascent state and users should wait on SB_BORN or
	//  * SB_DYING to be set.
	//  */
	// list_add_to_prev(&s->s_list, &super_blocks);
	// hlist_add_head(&s->s_instances, &s->s_type->fs_supers);
	get_filesystem(s->s_type);
	return s;
}
EXPORT_SYMBOL(sget_fc);

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
int vfs_get_super(fs_ctxt_s *fc, enum vfs_get_super_keying keying,
		int (*fill_super)(super_block_s *sb, fs_ctxt_s *fc)) {

	super_block_s *sb;
	int err;

	sb = sget_fc(fc);
	if (IS_ERR(sb))
		return PTR_ERR(sb);

	if (sb->s_root == NULL) {
		err = fill_super(sb, fc);
		if (err)
			goto error;

		sb->s_flags |= SB_ACTIVE;
	}

	fc->root = dget(sb->s_root);
	return 0;

error:
	// deactivate_locked_super(sb);
	return err;
}

int get_tree_nodev(fs_ctxt_s *fc,
		int (*fill_super)(super_block_s *sb, fs_ctxt_s *fc))
{
	return vfs_get_super(fc, vfs_get_independent_super, fill_super);
}
EXPORT_SYMBOL(get_tree_nodev);

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
// 	blk_dev_s *bdev;
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
EXPORT_SYMBOL(mount_bdev);

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

	if (fc->root == NULL) {
		pr_err("Filesystem %s get_tree() didn't set fc->root\n", fc->fs_type->name);
		/* We don't know what the locking state of the superblock is -
		 * if there is a superblock.
		 */
		BUG();
	}

	sb = fc->root->d_sb;
	// WARN_ON(!sb->s_bdi);

	// /*
	//  * super_wake() contains a memory barrier which also care of
	//  * ordering for super_cache_count(). We place it before setting
	//  * SB_BORN as the data dependency between the two functions is
	//  * the superblock structure contents that we just set up, not
	//  * the SB_BORN flag.
	//  */
	// super_wake(sb, SB_BORN);

	// error = security_sb_set_mnt_opts(sb, fc->security, 0, NULL);
	// if (unlikely(error)) {
	// 	fc_drop_locked(fc);
	// 	return error;
	// }

	// /*
	//  * filesystems should never set s_maxbytes larger than MAX_LFS_FILESIZE
	//  * but s_maxbytes was an unsigned long long for many releases. Throw
	//  * this warning for a little while to try and catch filesystems that
	//  * violate this rule.
	//  */
	// WARN((sb->s_maxbytes < 0), "%s set sb->s_maxbytes to "
	// 	"negative value (%lld)\n", fc->fs_type->name, sb->s_maxbytes);

	return 0;
}
EXPORT_SYMBOL(vfs_get_tree);
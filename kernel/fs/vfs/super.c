#include <linux/fs/fs.h>

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
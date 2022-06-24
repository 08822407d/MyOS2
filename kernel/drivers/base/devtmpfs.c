// SPDX-License-Identifier: GPL-2.0
/*
 * devtmpfs - kernel-maintained tmpfs-based /dev
 *
 * Copyright (C) 2009, Kay Sievers <kay.sievers@vrfy.org>
 *
 * During bootup, before any driver core device is registered,
 * devtmpfs, a tmpfs-based filesystem is created. Every driver-core
 * device which requests a device node, will add a node in this
 * filesystem.
 * By default, all devices are named after the name of the device,
 * owned by root and have a default mode of 0600. Subsystems can
 * overwrite the default setting if needed.
 */

#include <linux/kernel/kernel.h>
// #include <linux/syscalls.h>
#include <linux/kernel/mount.h>
// #include <linux/device.h>
#include <linux/block/genhd.h>
#include <linux/fs/namei.h>
#include <linux/fs/fs.h>
#include <linux/mm/shmem_fs.h>
// #include <linux/ramfs.h>
// #include <linux/sched.h>
// #include <linux/slab.h>
// #include <linux/kthread.h>
// #include <linux/init_syscalls.h>
#include <uapi/mount.h>
#include <linux/drivers/base.h>

#include <include/proto.h>
#include <include/printk.h>

static struct vfsmount *mnt;

static dentry_s *public_dev_mount(fs_type_s *fs_type, int flags,
				const char *dev_name, void *data)
{
	super_block_s *s = mnt->mnt_sb;
	int err;

	return dget(s->s_root);
}

static fs_type_s dev_fs_type = {
	.name = "devtmpfs",
	.mount = public_dev_mount,
};

// static int handle_create(const char *nodename, umode_t mode, kuid_t uid,
// 				kgid_t gid, struct device *dev)
int handle_create(const char *nodename, umode_t mode, dev_t dev)
{
	dentry_s *dentry;
	path_s path;
	int err;

	dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	// if (dentry == ERR_PTR(-ENOENT)) {
	// 	create_path(nodename);
	// 	dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	// }
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	err = vfs_mknod(path.dentry->d_inode, dentry, mode, dev);
	if (!err) {
	// 	struct iattr newattrs;

	// 	newattrs.ia_mode = mode;
	// 	newattrs.ia_uid = uid;
	// 	newattrs.ia_gid = gid;
	// 	newattrs.ia_valid = ATTR_MODE|ATTR_UID|ATTR_GID;
	// 	inode_lock(d_inode(dentry));
	// 	notify_change(&init_user_ns, dentry, &newattrs, NULL);
	// 	inode_unlock(d_inode(dentry));

	// 	/* mark as kernel-created inode */
	// 	d_inode(dentry)->i_private = &thread;
	}
	done_path_create(&path, dentry);
	return err;
}

/*
 * The __ref is because devtmpfs_setup needs to be __init for the routines it
 * calls.  That call is done while devtmpfs_init, which is marked __init,
 * synchronously waits for it to complete.
 */
int devtmpfsd(void *p)
{

}

/*
 * Create devtmpfs instance, driver-core devices will add their device
 * nodes here.
 */
int devtmpfs_init(void)
{
	int err;
	list_hdr_init(&dev_fs_type.fs_supers);
	err = register_filesystem(&dev_fs_type);
	if (err) {
		color_printk(RED, BLACK, "devtmpfs: unable to register devtmpfs "
				"type %i\n", err);
		return err;
	}

	// thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	// if (!IS_ERR(thread)) {
	// 	wait_for_completion(&setup_done);
	// } else {
	// 	err = PTR_ERR(thread);
	// 	thread = NULL;
	// }

	// if (err) {
	// 	color_printk(RED, BLACK, "devtmpfs: unable to create devtmpfs %i\n", err);
	// 	unregister_filesystem(&dev_fs_type);
	// 	return err;
	// }

	color_printk(GREEN, BLACK, "devtmpfs: initialized\n");
	return 0;
}
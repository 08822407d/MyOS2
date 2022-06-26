// SPDX-License-Identifier: GPL-2.0
/*
 * fs/sysfs/symlink.c - operations for initializing and mounting sysfs
 *
 * Copyright (c) 2001-3 Patrick Mochel
 * Copyright (c) 2007 SUSE Linux Products GmbH
 * Copyright (c) 2007 Tejun Heo <teheo@suse.de>
 *
 * Please see Documentation/filesystems/sysfs.rst for more information.
 */

#include <linux/fs/fs.h>
// #include <linux/magic.h>
#include <linux/kernel/mount.h>
#include <linux/init/init.h>
// #include <linux/slab.h>
// #include <linux/user_namespace.h>
// #include <linux/fs_context.h>
// #include <net/net_namespace.h>
#include <linux/fs/sysfs.h>


#include <include/proto.h>

static kernfs_root_s *sysfs_root;
kernfs_node_s *sysfs_root_kn;

static fs_type_s sysfs_fs_type = {
	.name			= "sysfs",
	// .init_fs_context	= sysfs_init_fs_context,
	// .kill_sb		= sysfs_kill_sb,
	.fs_flags		= FS_USERNS_MOUNT,
};

int sysfs_init(void)
{
	int err = 0;

	sysfs_root = kernfs_create_root(NULL,
					KERNFS_ROOT_EXTRA_OPEN_PERM_CHECK, NULL);
	if (IS_ERR(sysfs_root))
		return PTR_ERR(sysfs_root);

	sysfs_root_kn = sysfs_root->kn;

	err = register_filesystem(&sysfs_fs_type);
	if (err) {
		kfree(sysfs_root);
		return err;
	}

	return 0;
}
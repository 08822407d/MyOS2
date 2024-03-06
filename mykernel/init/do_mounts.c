// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/sched.h>
#include <linux/kernel/ctype.h>
#include <linux/device/tty.h>
#include <linux/kernel/delay.h>
#include <linux/block/genhd.h>
#include <linux/kernel/mount.h>
#include <linux/device/device.h>
#include <linux/init/init.h>
#include <linux/fs/fs.h>
#include <linux/fs/shmem_fs.h>

#include <uapi/linux/mount.h>

#include <linux/init/do_mounts.h>


static int rootfs_init_fs_context(fs_ctxt_s *fc)
{
	// if (IS_ENABLED(CONFIG_TMPFS) && is_tmpfs)
		return shmem_init_fs_context(fc);

	// return ramfs_init_fs_context(fc);
}

fs_type_s rootfs_fs_type = {
	.name		= "rootfs",
	.init_fs_context = rootfs_init_fs_context,
	.kill_sb	= kill_litter_super,
};

void init_rootfs(void)
{
	// if (IS_ENABLED(CONFIG_TMPFS) && !saved_root_name[0] &&
	// 	(!root_fs_names || strstr(root_fs_names, "tmpfs")))
	// 	is_tmpfs = true;
	register_filesystem(&rootfs_fs_type);
}
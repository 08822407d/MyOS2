// SPDX-License-Identifier: GPL-2.0-only
// #include <linux/module.h>
// #include <linux/sched.h>
// #include <linux/ctype.h>
// #include <linux/fd.h>
// #include <linux/tty.h>
// #include <linux/suspend.h>
// #include <linux/root_dev.h>
// #include <linux/security.h>
// #include <linux/delay.h>
#include <linux/block/genhd.h>
#include <linux/kernel/mount.h>
// #include <linux/device.h>
#include <linux/init/init.h>
#include <linux/fs/fs.h>
// #include <linux/initrd.h>
// #include <linux/async.h>
#include <linux/kernel/sched/fs_struct.h>
// #include <linux/slab.h>
// #include <linux/ramfs.h>
#include <linux/mm/shmem_fs.h>

// #include <linux/nfs_fs.h>
// #include <linux/nfs_fs_sb.h>
// #include <linux/nfs_mount.h>
// #include <linux/raid/detect.h>
#include <uapi/mount.h>

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
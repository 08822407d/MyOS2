/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/kernel/kernel.h>
#include <linux/block/blkdev.h>
#include <linux/init/init.h>
#include <linux/kernel/syscalls.h>
#include <uapi/linux/unistd.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/mount.h>
#include <uapi/linux/major.h>
// #include <linux/root_dev.h>
// #include <linux/init_syscalls.h>

// void  mount_block_root(char *name, int flags);
// void  mount_root(void);
// extern int root_mountflags;

// static inline __init int create_dev(char *name, dev_t dev)
// {
// 	init_unlink(name);
// 	return init_mknod(name, S_IFBLK | 0600, new_encode_dev(dev));
// }

// #ifdef CONFIG_BLK_DEV_RAM

// int __init rd_load_disk(int n);
// int __init rd_load_image(char *from);

// #else

// static inline int rd_load_disk(int n) { return 0; }
// static inline int rd_load_image(char *from) { return 0; }

// #endif

// #ifdef CONFIG_BLK_DEV_INITRD

// bool __init initrd_load(void);

// #else

// static inline bool initrd_load(void) { return false; }

// #endif
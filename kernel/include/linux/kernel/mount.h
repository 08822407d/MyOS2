/* SPDX-License-Identifier: GPL-2.0 */
/*
 *
 * Definitions for mount interface. This describes the in the kernel build 
 * linkedlist with mounted filesystems.
 *
 * Author:  Marco van Wieringen <mvw@planets.elm.net>
 *
 */
#ifndef _LINUX_MOUNT_H

	#define _LINUX_MOUNT_H

	#include <linux/kernel/types.h>
		#include <linux/lib/list.h>
	// #include <linux/nodemask.h>
	// #include <linux/spinlock.h>
	// #include <linux/seqlock.h>
	// #include <linux/atomic.h>

	#include <linux/fs/vfs_s_defs.h>

	#define MNT_NOSUID		0x01
	#define MNT_NODEV		0x02
	#define MNT_NOEXEC		0x04
	#define MNT_NOATIME		0x08
	#define MNT_NODIRATIME	0x10
	#define MNT_RELATIME	0x20
	#define MNT_READONLY	0x40	/* does the user want this to be r/o? */
	#define MNT_NOSYMFOLLOW	0x80

	#define MNT_SHRINKABLE	0x100
	#define MNT_WRITE_HOLD	0x200

	#define MNT_SHARED		0x1000	/* if the vfsmount is a shared mount */
	#define MNT_UNBINDABLE	0x2000	/* if the vfsmount is a unbindable mount */
	/*
	* MNT_SHARED_MASK is the set of flags that should be cleared when a
	* mount becomes shared.  Currently, this is only the flag that says a
	* mount cannot be bind mounted, since this is how we create a mount
	* that shares events with another mount.  If you add a new MNT_*
	* flag, consider how it interacts with shared mounts.
	*/
	#define MNT_SHARED_MASK	(MNT_UNBINDABLE)
	#define MNT_USER_SETTABLE_MASK  (MNT_NOSUID | MNT_NODEV | MNT_NOEXEC \
					| MNT_NOATIME | MNT_NODIRATIME | MNT_RELATIME \
					| MNT_READONLY | MNT_NOSYMFOLLOW)
	#define MNT_ATIME_MASK (MNT_NOATIME | MNT_NODIRATIME | MNT_RELATIME )

	#define MNT_INTERNAL_FLAGS (MNT_SHARED | MNT_WRITE_HOLD | MNT_INTERNAL | \
					MNT_DOOMED | MNT_SYNC_UMOUNT | MNT_MARKED | \
					MNT_CURSOR)

	#define MNT_INTERNAL		0x4000

	#define MNT_LOCK_ATIME		0x040000
	#define MNT_LOCK_NOEXEC		0x080000
	#define MNT_LOCK_NOSUID		0x100000
	#define MNT_LOCK_NODEV		0x200000
	#define MNT_LOCK_READONLY	0x400000
	#define MNT_LOCKED			0x800000
	#define MNT_DOOMED			0x1000000
	#define MNT_SYNC_UMOUNT		0x2000000
	#define MNT_MARKED			0x4000000
	#define MNT_UMOUNT			0x8000000
	#define MNT_CURSOR			0x10000000

	typedef struct vfsmount {
		dentry_s		*mnt_root;	/* root of the mounted tree */
		super_block_s	*mnt_sb;	/* pointer to superblock */
		int				mnt_flags;
		// user_namespace_s *mnt_userns;
	} vfsmount_s;

	// static inline user_namespace_s *mnt_user_ns(const vfsmount_s *mnt)
	// {
	// 	/* Pairs with smp_store_release() in do_idmap_mount(). */
	// 	return smp_load_acquire(&mnt->mnt_userns);
	// }

	extern int mnt_want_write(vfsmount_s *mnt);
	extern int mnt_want_write_file(file_s *file);
	extern void mnt_drop_write(vfsmount_s *mnt);
	extern void mnt_drop_write_file(file_s *file);
	extern void mntput(vfsmount_s *mnt);
	extern vfsmount_s *mntget(vfsmount_s *mnt);
	extern vfsmount_s *mnt_clone_internal(const path_s *path);
	extern bool __mnt_is_readonly(vfsmount_s *mnt);
	extern bool mnt_may_suid(vfsmount_s *mnt);

	extern vfsmount_s *clone_private_mount(const path_s *path);
	extern int __mnt_want_write(vfsmount_s *);
	extern void __mnt_drop_write(vfsmount_s *);

	extern vfsmount_s *fc_mount(fs_ctxt_s *fc);
	extern vfsmount_s *vfs_create_mount(fs_ctxt_s *fc);
	extern vfsmount_s *vfs_kern_mount(fs_type_s *type,
					int flags, const char *name, void *data);
	extern vfsmount_s *vfs_submount(const dentry_s *mountpoint,
					fs_type_s *type, const char *name, void *data);

	// extern void mnt_set_expiry(vfsmount_s *mnt, list_head_s *expiry_list);
	// extern void mark_mounts_for_expiry(list_head_s *mounts);

	extern dev_t name_to_dev_t(const char *name);
	extern bool path_is_mountpoint(const path_s *path);

	extern void kern_unmount_array(vfsmount_s *mnt[], unsigned int num);

#endif /* _LINUX_MOUNT_H */
#ifndef _LINUX_MOUNT_H_
#define _LINUX_MOUNT_H_

#include <lib/utils.h>
#include <uapi/mount.h>

// #include "vfs_s_defs.h"

	typedef struct vfsmount {
		dentry_s *		mnt_root;	/* root of the mounted tree */
		super_block_s *	mnt_sb;		/* pointer to superblock */
		int 			mnt_flags;
	} vfsmount_s;

	typedef struct mountpoint
	{
		dentry_s *	m_dentry;
	} mountpoint_s;

	typedef struct mount
	{
		// List_s		mnt_list;	// in Linux it is hlist_head
		mount_s *	mnt_parent;
		dentry_s *	mnt_mountpoint;
		vfsmount_s	mnt;

		List_hdr_s	mnt_mounts;	/* list of children, anchored here */
		List_s		mnt_child;	/* and going through their mnt_child */
		mountpoint_s *	mnt_mp;	/* where is it mounted */
	} mount_s;

	static inline mount_s * real_mount(vfsmount_s * mnt)
	{
		return container_of(mnt, mount_s, mnt);
	}

	static inline int mnt_has_parent(mount_s * mnt)
	{
		return mnt != mnt->mnt_parent;
	}

#endif /* _LINUX_MOUNT_H_ */
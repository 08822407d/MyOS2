#include <stddef.h>

#include <include/fs/mount.h>

/*
 * find the first mount at @dentry on vfsmount @mnt.
 * call under rcu_read_lock()
 */
mount_s *__lookup_mnt(vfsmount_s * mnt, dentry_s * dentry)
{
	mount_s *p;

		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	return NULL;
}
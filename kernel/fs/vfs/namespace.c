#include <stddef.h>

#include <include/fs/mount.h>

List_hdr_s mount_lhdr;

/*
 * find the first mount at @dentry on vfsmount @mnt.
 * call under rcu_read_lock()
 */
mount_s *__lookup_mnt(vfsmount_s * mnt, dentry_s * dentry)
{
	mount_s *p;

	List_s * mnt_lp;
	for (mnt_lp = mount_lhdr.header.next; mnt_lp != &mount_lhdr.header; mnt_lp = mnt_lp->next)
	{
		p = container_of(mnt_lp, mount_s, mnt_list);
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	}
	return NULL;
}
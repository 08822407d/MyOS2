#include <sys/fcntl.h>

#include <stddef.h>

#include <include/fs/vfs.h>
#include <include/fs/namei.h>
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

// Linux function proto:
// long do_mount(const char *dev_name, const char __user *dir_name,
//		const char *type_page, unsigned long flags, void *data_page)
long do_mount(const char * dev_name, const char * dir_name, unsigned long flags)
{
	path_s path;
	int ret;

	// ret = user_path_at(AT_FDCWD, dir_name, LOOKUP_FOLLOW, &path);
	ret = user_path_at(AT_FDCWD, dir_name, flags, &path);
	if (ret)
		return ret;
	// ret = path_mount(dev_name, &path, flags);
	return ret;
}

void init_mount()
{
	list_hdr_init(&mount_lhdr);
}
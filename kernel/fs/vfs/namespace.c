#include <sys/fcntl.h>

#include <stddef.h>
#include <errno.h>

#include <include/fs/vfs.h>
#include <include/fs/namei.h>
#include <include/fs/mount.h>

mount_s root_mnt;

/*
 * find the first mount at @dentry on vfsmount @mnt.
 * call under rcu_read_lock()
 */
mount_s *__lookup_mnt(vfsmount_s * mnt, dentry_s * dentry)
{
	mount_s * p = container_of(mnt, mount_s, mnt);
	List_hdr_s * child_lhdr = &p->mnt_mounts;

	List_s * mnt_lp;
	for (mnt_lp = child_lhdr->header.next; mnt_lp != &child_lhdr->header; mnt_lp = mnt_lp->next)
	{
		p = container_of(mnt_lp, mount_s, mnt_list);
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	}
	return NULL;
}
/*
 * lookup_mnt - Return the first child mount mounted at path
 *
 * "First" means first mounted chronologically.  If you create the
 * following mounts:
 *
 * mount /dev/sda1 /mnt
 * mount /dev/sda2 /mnt
 * mount /dev/sda3 /mnt
 *
 * Then lookup_mnt() on the base /mnt dentry in the root mount will
 * return successively the root dentry and vfsmount of /dev/sda1, then
 * /dev/sda2, then /dev/sda3, then NULL.
 *
 * lookup_mnt takes a reference to the found vfsmount.
 */
vfsmount_s * lookup_mnt(const path_s * path)
{
	mount_s * child_mnt;
	vfsmount_s * m = NULL;
	child_mnt = __lookup_mnt(path->mnt, path->dentry);
	if (child_mnt != NULL)
		m = &child_mnt->mnt;

	return m;
}

/*
 * create a new mount for userspace and request it to be added into the
 * namespace's tree
 */
// Linux function proto:
// static int do_new_mount(struct path *path, const char *fstype, int sb_flags,
// 			int mnt_flags, const char *name, void *data)
static int do_new_mount(path_s * path, int sb_flags, int mnt_flags, const char * name)
{
	struct file_system_type *type;
	struct fs_context *fc;
	const char *subtype = NULL;
	int err = 0;

	// if (!fstype)
	// 	return -EINVAL;

	// type = get_fs_type(fstype);
	// if (!type)
	// 	return -ENODEV;

	// if (type->fs_flags & FS_HAS_SUBTYPE) {
	// 	subtype = strchr(fstype, '.');
	// 	if (subtype) {
	// 		subtype++;
	// 		if (!*subtype) {
	// 			put_filesystem(type);
	// 			return -EINVAL;
	// 		}
	// 	}
	// }

	// fc = fs_context_for_mount(type, sb_flags);
	// put_filesystem(type);
	// if (IS_ERR(fc))
	// 	return PTR_ERR(fc);

	// if (subtype)
	// 	err = vfs_parse_fs_string(fc, "subtype",
	// 				  subtype, strlen(subtype));
	// if (!err && name)
	// 	err = vfs_parse_fs_string(fc, "source", name, strlen(name));
	// if (!err)
	// 	err = parse_monolithic_mount_data(fc, data);
	// if (!err && !mount_capable(fc))
	// 	err = -EPERM;
	// if (!err)
	// 	err = vfs_get_tree(fc);
	// if (!err)
	// 	err = do_new_mount_fc(fc, path, mnt_flags);

	// put_fs_context(fc);
	return err;
}

static mount_s *__do_loopback(path_s * old_path, int recurse)
{
	// struct mount *mnt = ERR_PTR(-EINVAL), *old = real_mount(old_path->mnt);

	// if (IS_MNT_UNBINDABLE(old))
	// 	return mnt;

	// if (!check_mnt(old) && old_path->dentry->d_op != &ns_dentry_operations)
	// 	return mnt;

	// if (!recurse && has_locked_children(old, old_path->dentry))
	// 	return mnt;

	// if (recurse)
	// 	mnt = copy_tree(old, old_path->dentry, CL_COPY_MNT_NS_FILE);
	// else
	// 	mnt = clone_mnt(old, old_path->dentry, 0);

	// if (!IS_ERR(mnt))
	// 	mnt->mnt.mnt_flags &= ~MNT_LOCKED;

	// return mnt;
}

/*
 * do loopback mount.
 */
static int do_loopback(IN path_s * path, const char * old_name, int recurse)
{
	path_s old_path;
	mount_s * mnt = NULL, * parent;
	mountpoint_s * mp;
	int err;
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &old_path);
	if (err)
		return err;

// 	err = -EINVAL;
// 	if (mnt_ns_loop(old_path.dentry))
// 		goto out;

// 	mp = lock_mount(path);
// 	if (IS_ERR(mp)) {
// 		err = PTR_ERR(mp);
// 		goto out;
// 	}

// 	parent = real_mount(path->mnt);
// 	if (!check_mnt(parent))
// 		goto out2;

// 	mnt = __do_loopback(&old_path, recurse);
// 	if (IS_ERR(mnt)) {
// 		err = PTR_ERR(mnt);
// 		goto out2;
// 	}

// out2:
// 	unlock_mount(mp);
// out:
// 	return err;
}

/*
 * Flags is a 32-bit value that allows up to 31 non-fs dependent flags to
 * be given to the mount() call (ie: read-only, no-dev, no-suid etc).
 *
 * data is a (void *) that can point to any structure up to
 * PAGE_SIZE-1 bytes, which can contain arbitrary fs-dependent
 * information (or be NULL).
 *
 * Pre-0.97 versions of mount() didn't have a flags word.
 * When the flags word was introduced its top half was required
 * to have the magic value 0xC0ED, and this remained so until 2.4.0-test9.
 * Therefore, if this magic number is present, it carries no information
 * and must be discarded.
 */
// Linux function proto:
// int path_mount(const char *dev_name, struct path *path,
// 		const char *type_page, unsigned long flags, void *data_page)
int path_mount(const char * dev_name, IN path_s * path, unsigned long flags)
{
	unsigned int mnt_flags = 0, sb_flags;
	int ret;

	// if ((flags & (MS_REMOUNT | MS_BIND)) == (MS_REMOUNT | MS_BIND))
	// 	return do_reconfigure_mnt(path, mnt_flags);
	// if (flags & MS_REMOUNT)
	// 	return do_remount(path, flags, sb_flags, mnt_flags, data_page);
	if (flags & MS_BIND)
		return do_loopback(path, dev_name, flags & MS_REC);
	// if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
	// 	return do_change_type(path, flags);
	// if (flags & MS_MOVE)
	// 	return do_move_mount_old(path, dev_name);

	return do_new_mount(path, sb_flags, mnt_flags, dev_name);
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
	ret = path_mount(dev_name, &path, flags);
	return ret;
}

void init_mount()
{
	list_hdr_init(&root_mnt.mnt_mounts);
	list_init(&root_mnt.mnt_child, &root_mnt);
	list_init(&root_mnt.mnt_list, &root_mnt);

	root_mnt.mnt.mnt_sb = root_sb;
	root_mnt.mnt_parent = &root_mnt;
	root_mnt.mnt_mountpoint =
	root_mnt.mnt.mnt_root = root_sb->root;
	root_mnt.mnt_mp = NULL;
}
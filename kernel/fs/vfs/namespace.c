#include <sys/fcntl.h>

#include <stddef.h>
#include <errno.h>

#include <include/proto.h>
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
		p = container_of(mnt_lp, mount_s, mnt_child);
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

// Linux function proto:
// static struct mountpoint *get_mountpoint(struct dentry *dentry) 
static mountpoint_s * get_mountpoint(IN dentry_s * dentry)
{
	mountpoint_s * mp, * new = NULL;
	int ret;

	new = kmalloc(sizeof(mountpoint_s));
	if (new == NULL)
		goto done;

	/* Add the new mountpoint to the hash table */
	new->m_dentry = dentry;
	// new->m_count = 1;

	mp = new;
	new = NULL;
done:
	return mp;
}

// Linux function proto:
// static struct mountpoint *lock_mount(struct path *path)
static mountpoint_s * lock_mount(path_s * path)
{
	vfsmount_s * mnt;
	dentry_s * dentry = path->dentry;
retry:
	mnt = lookup_mnt(path);
	if (mnt == NULL) {
		mountpoint_s * mp = get_mountpoint(dentry);
		return mp;
	}
	path->mnt = mnt;
	dentry = path->dentry = mnt->mnt_root;
	goto retry;
}

/*
 *  @source_mnt : mount tree to be attached
 *  @nd         : place the mount tree @source_mnt is attached
 *  @parent_nd  : if non-null, detach the source_mnt from its parent and
 *  		   store the parent mount and mountpoint dentry.
 *  		   (done when source_mnt is moved)
 *
 *  NOTE: in the table below explains the semantics when a source mount
 *  of a given type is attached to a destination mount of a given type.
 * ---------------------------------------------------------------------------
 * |         BIND MOUNT OPERATION                                            |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (++)   |     shared (+) |     shared(+++)|  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+)    |      private   |      slave (*) |  invalid   |
 * ***************************************************************************
 * A bind operation clones the source mount and mounts the clone on the
 * destination mount.
 *
 * (++)  the cloned mount is propagated to all the mounts in the propagation
 * 	 tree of the destination mount and the cloned mount is added to
 * 	 the peer group of the source mount.
 * (+)   the cloned mount is created under the destination mount and is marked
 *       as shared. The cloned mount is added to the peer group of the source
 *       mount.
 * (+++) the mount is propagated to all the mounts in the propagation tree
 *       of the destination mount and the cloned mount is made slave
 *       of the same master as that of the source mount. The cloned mount
 *       is marked as 'shared and slave'.
 * (*)   the cloned mount is made a slave of the same master as that of the
 * 	 source mount.
 *
 * ---------------------------------------------------------------------------
 * |         		MOVE MOUNT OPERATION                                 |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (+)    |     shared (+) |    shared(+++) |  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+*)   |      private   |    slave (*)   | unbindable |
 * ***************************************************************************
 *
 * (+)  the mount is moved to the destination. And is then propagated to
 * 	all the mounts in the propagation tree of the destination mount.
 * (+*)  the mount is moved to the destination.
 * (+++)  the mount is moved to the destination and is then propagated to
 * 	all the mounts belonging to the destination mount's propagation tree.
 * 	the mount is marked as 'shared and slave'.
 * (*)	the mount continues to be a slave at the new location.
 *
 * if the source mount is a tree, the operations explained above is
 * applied to each mount in the tree.
 * Must be called without spinlocks held, since this function can sleep
 * in allocations.
 */
// 1static int attach_recursive_mnt(struct mount *source_mnt,
// 1			struct mount *dest_mnt,
// 1			struct mountpoint *dest_mp,
// 1			bool moving)
static int attach_recursive_mnt(mount_s * source_mnt,
			mount_s * dest_mnt, mountpoint_s * dest_mp)
{
	mountpoint_s *smp;
	mount_s *child, *p;
	int err;

	/* Preallocate a mountpoint in case the new mounts need
	 * to be tucked under other mounts.
	 */
	smp = get_mountpoint(source_mnt->mnt.mnt_root);
	// if (IS_ERR(smp))
	// 	return PTR_ERR(smp);

// 	hlist_for_each_entry_safe(child, n, &tree_list, mnt_hash) {
// 		struct mount *q;
// 		hlist_del_init(&child->mnt_hash);
// 		q = __lookup_mnt(&child->mnt_parent->mnt,
// 				 child->mnt_mountpoint);
// 		if (q)
// 			mnt_change_mountpoint(child, smp, q);
// 		/* Notice when we are propagating across user namespaces */
// 		if (child->mnt_parent->mnt_ns->user_ns != user_ns)
// 			lock_mnt_tree(child);
// 		child->mnt.mnt_flags &= ~MNT_LOCKED;
// 		commit_tree(child);
// 	}
// 	put_mountpoint(smp);
// 	unlock_mount_hash();

// 	return 0;

//  out_cleanup_ids:
// 	while (!hlist_empty(&tree_list)) {
// 		child = hlist_entry(tree_list.first, struct mount, mnt_hash);
// 		child->mnt_parent->mnt_ns->pending_mounts = 0;
// 		umount_tree(child, UMOUNT_SYNC);
// 	}
// 	unlock_mount_hash();
// 	cleanup_group_ids(source_mnt, NULL);
//  out:
// 	ns->pending_mounts = 0;

// 	read_seqlock_excl(&mount_lock);
// 	put_mountpoint(smp);
// 	read_sequnlock_excl(&mount_lock);

	return err;
}

// Linux function proto:
// static int graft_tree(struct mount *mnt, struct mount *p, struct mountpoint *mp)
static int graft_tree(mount_s * mnt, mount_s * p, mountpoint_s * mp)
{
	// if (d_is_dir(mp->m_dentry) !=
	//       d_is_dir(mnt->mnt.mnt_root))
	// 	return -ENOTDIR;

	return attach_recursive_mnt(mnt, p, mp);
}

// Linux function proto:
// static struct mount *clone_mnt(struct mount *old, struct dentry *root, int flag)
static mount_s * clone_mnt(IN mount_s * old, IN dentry_s * root)
{
	super_block_s * sb = old->mnt.mnt_sb;
	mount_s * mnt;
	int err;

	mnt = kmalloc((sizeof(mount_s)));
	// if (!mnt)
	// 	return ERR_PTR(-ENOMEM);

	list_hdr_init(&mnt->mnt_mounts);
	list_init(&mnt->mnt_child, mnt);
	mnt->mnt.mnt_sb = sb;
	mnt->mnt.mnt_root = root;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;

	return mnt;
}

static mount_s *__do_loopback(IN path_s * old_path)
{
	mount_s * mnt, * old = real_mount(old_path->mnt);

	// if (IS_MNT_UNBINDABLE(old))
	// 	return mnt;

	// if (!check_mnt(old) && old_path->dentry->d_op != &ns_dentry_operations)
	// 	return mnt;

	// if (!recurse && has_locked_children(old, old_path->dentry))
	// 	return mnt;

	// if (recurse)
	// 	mnt = copy_tree(old, old_path->dentry, CL_COPY_MNT_NS_FILE);
	// else
		mnt = clone_mnt(old, old_path->dentry);

	// if (!IS_ERR(mnt))
	// 	mnt->mnt.mnt_flags &= ~MNT_LOCKED;

	return mnt;
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
		goto out;

	parent = real_mount(path->mnt);
	mp = lock_mount(path);
	if (mp == NULL)
	{
		err = -ENOMEM;
		goto out;
	}

	mnt = __do_loopback(&old_path);
// 	if (IS_ERR(mnt)) {
// 		err = PTR_ERR(mnt);
// 		goto out2;
// 	}
	err = graft_tree(mnt, parent, mp);

// out2:
// 	unlock_mount(mp);
out:
	return err;
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

	root_mnt.mnt.mnt_sb = root_sb;
	root_mnt.mnt_parent = &root_mnt;
	root_mnt.mnt_mountpoint =
	root_mnt.mnt.mnt_root = root_sb->root;
	root_mnt.mnt_mp = NULL;
}
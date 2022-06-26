// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/namespace.c
 *
 * (C) Copyright Al Viro 2000, 2001
 *
 * Based on code from fs/super.c, copyright Linus Torvalds and others.
 * Heavily rewritten.
 */

// #include <linux/syscalls.h>
// #include <linux/export.h>
// #include <linux/capability.h>
// #include <linux/mnt_namespace.h>
// #include <linux/user_namespace.h>
#include <linux/fs/namei.h>
// #include <linux/security.h>
// #include <linux/cred.h>
// #include <linux/idr.h>
#include <linux/init/init.h>		/* init_rootfs */
// #include <linux/fs_struct.h>	/* get_fs_root et.al. */
// #include <linux/fsnotify.h>	/* fsnotify_vfsmount_delete */
#include <linux/fs/file.h>
// #include <linux/uaccess.h>
// #include <linux/proc_ns.h>
// #include <linux/magic.h>
// #include <linux/memblock.h>
// #include <linux/proc_fs.h>
// #include <linux/task_work.h>
// #include <linux/sched/task.h>
#include <uapi/mount.h>
#include <linux/fs/fs_context.h>
#include <linux/mm/shmem_fs.h>
// #include <linux/mnt_idmapping.h>
// #include "pnode.h"
#include <linux/fs/internal.h>


#include <linux/kernel/fcntl.h>
#include <linux/kernel/err.h>
#include <linux/kernel/kernfs.h>
#include <linux/kernel/sysfs.h>
#include <linux/fs/fs.h>
#include <linux/fs/mount.h>
#include <uapi/fcntl.h>
#include <errno.h>
#include <include/proto.h>
#include <include/printk.h>

mount_s root_mnt;


static mount_s *alloc_vfsmnt(const char *name)
{
	mount_s *mnt = kmalloc(sizeof(mount_s));
	if (mnt == NULL)
		goto out;

	if (name) {
		size_t len = strlen(name);
		mnt->mnt_devname = kmalloc(len + 1);
		if (mnt->mnt_devname == NULL)
			goto out_free_cache;

		((char *)mnt->mnt_devname)[len] = '\0';
		memcpy((char *)mnt->mnt_devname, name, len);
	}

	list_hdr_init(&mnt->mnt_mounts);
	list_init(&mnt->mnt_child, mnt);

	return mnt;

out_free_name:
	kfree((char *)mnt->mnt_devname);
out_free_cache:
	kfree(mnt);
out:
	return ERR_PTR(-ENOMEM);
}

/*
 * find the first mount at @dentry on vfsmount @mnt.
 * call under rcu_read_lock()
 */
mount_s *__lookup_mnt(IN vfsmount_s *mnt, IN dentry_s *dentry)
{
	mount_s *p = container_of(mnt, mount_s, mnt);
	List_hdr_s *child_lhdr = &p->mnt_mounts;

	List_s *mnt_lp;
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
vfsmount_s *lookup_mnt(IN const path_s *path)
{
	mount_s *child_mnt;
	vfsmount_s * m = NULL;
	child_mnt = __lookup_mnt(path->mnt, path->dentry);
	if (child_mnt != NULL)
		m = &child_mnt->mnt;

	return m;
}

// Linux function proto:
// static struct mountpoint *get_mountpoint(struct dentry *dentry) 
static mountpoint_s *get_mountpoint(IN dentry_s *dentry)
{
	mountpoint_s *mp, *new = NULL;
	int ret;

	new = kmalloc(sizeof(mountpoint_s));
	if (new == NULL)
		return ERR_PTR(-ENOMEM);

	/* Add the new mountpoint to the hash table */
	new->m_dentry = dentry;
	new->m_count = 1;

	mp = new;

done:
	return mp;
}

/*
 * vfsmount lock must be held.  Additionally, the caller is responsible
 * for serializing calls for given disposal list.
 */
/* called with namespace_lock and vfsmount lock */
static void put_mountpoint(mountpoint_s *mp)
{
	// static void __put_mountpoint(struct mountpoint *mp, struct list_head *list)
	// {
		if (--mp->m_count == 0) {
			dentry_s *dentry = mp->m_dentry;
			dentry->d_flags &= ~DCACHE_MOUNTED;
			// dput_to_list(dentry, list);
			kfree(mp);
		}
	// }
}

/*
 * vfsmount lock must be held for write
 */
void mnt_set_mountpoint(mount_s *parent_mnt, mountpoint_s *mp,
				mount_s *child_mnt)
{
	child_mnt->mnt_mountpoint = mp->m_dentry;
	child_mnt->mnt_parent = parent_mnt;
	child_mnt->mnt_mp = mp;
	list_hdr_append(&parent_mnt->mnt_mounts, &child_mnt->mnt_child);
}

/**
 * vfs_create_mount - Create a mount for a configured superblock
 * @fc: The configuration context with the superblock attached
 *
 * Create a mount to an already configured superblock.  If necessary, the
 * caller should invoke vfs_get_tree() before calling this.
 *
 * Note that this does not attach the mount to anything.
 */
vfsmount_s *vfs_create_mount(fs_ctxt_s *fc)
{
	mount_s *mnt;

	if (fc->root == NULL)
		return ERR_PTR(-EINVAL);

	mnt = alloc_vfsmnt(fc->source ?: "none");
	if (mnt == NULL)
		return ERR_PTR(-ENOMEM);

	if (fc->sb_flags & SB_KERNMOUNT)
		mnt->mnt.mnt_flags = MNT_INTERNAL;

	mnt->mnt.mnt_sb		= fc->root->d_sb;
	mnt->mnt.mnt_root	= fc->root;
	mnt->mnt_mountpoint	= mnt->mnt.mnt_root;
	mnt->mnt_parent		= mnt;

	// list_add_tail(&mnt->mnt_instance, &mnt->mnt.mnt_sb->s_mounts);
	return &mnt->mnt;
}

vfsmount_s *fc_mount(fs_ctxt_s *fc)
{
	int err = vfs_get_tree(fc);
	if (err == 0) {
		return vfs_create_mount(fc);
	}
	return ERR_PTR(err);
}

vfsmount_s *vfs_kern_mount(fs_type_s *type, int flags,
				const char *name, void *data)
{
	fs_ctxt_s *fc;
	vfsmount_s *mnt;
	// int ret = 0;

	if (type == NULL)
		return ERR_PTR(-EINVAL);

	fc = fs_context_for_mount(type, flags);
	if (IS_ERR(fc))
		return ERR_CAST(fc);
	// if (ret == 0)
		mnt = fc_mount(fc);
	// else
	// 	mnt = ERR_PTR(ret);

	put_fs_context(fc);
	return mnt;
}


// Linux function proto:
// static struct mountpoint *lock_mount(struct path *path)
static mountpoint_s *lock_mount(IN path_s *path)
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
// static int attach_recursive_mnt(struct mount *source_mnt,
// 				struct mount *dest_mnt,
// 				struct mountpoint *dest_mp,
// 				bool moving)
static int attach_recursive_mnt(mount_s *source_mnt,
				mount_s *dest_mnt, mountpoint_s *dest_mp)
{
	// mountpoint_s *smp;
	mount_s *child, *p;
	int err;

	// /* Preallocate a mountpoint in case the new mounts need
	//  * to be tucked under other mounts.
	//  */
	// smp = get_mountpoint(source_mnt->mnt.mnt_root);
	// if (IS_ERR(smp))
	// 	return PTR_ERR(smp);

	// if (moving) {
	// 	unhash_mnt(source_mnt);
	// 	attach_mnt(source_mnt, dest_mnt, dest_mp);
	// 	touch_mnt_namespace(source_mnt->mnt_ns);
	// } else {
	// 	if (source_mnt->mnt_ns) {
	// 		/* move from anon - the caller will destroy */
	// 		list_del_init(&source_mnt->mnt_ns->list);
	// 	}
		mnt_set_mountpoint(dest_mnt, dest_mp, source_mnt);
	// 	commit_tree(source_mnt);
	// }

	return -ENOERR;
}

static void unlock_mount(mountpoint_s *where)
{
	// dentry_s *dentry = where->m_dentry;

	// read_seqlock_excl(&mount_lock);
	// put_mountpoint(where);
	// read_sequnlock_excl(&mount_lock);

	// namespace_unlock();
	// inode_unlock(dentry->d_inode);
}

// Linux function proto:
// static int graft_tree(struct mount *mnt, struct mount *p,
//				struct mountpoint *mp)
static int graft_tree(IN mount_s *mnt, IN mount_s *p,
				IN mountpoint_s *mp)
{
	// if (d_is_dir(mp->m_dentry) !=
	//       d_is_dir(mnt->mnt.mnt_root))
	// 	return -ENOTDIR;

	return attach_recursive_mnt(mnt, p, mp);
}

// Linux function proto:
// static struct mount *clone_mnt(struct mount *old,
//				struct dentry *root, int flag)
static mount_s *clone_mnt(IN mount_s *old, IN dentry_s *root)
{
	super_block_s * sb = old->mnt.mnt_sb;
	mount_s * mnt;
	int err;

	mnt = kmalloc((sizeof(mount_s)));
	if (mnt == NULL)
		return ERR_PTR(-ENOMEM);

	list_hdr_init(&mnt->mnt_mounts);
	list_init(&mnt->mnt_child, mnt);
	mnt->mnt.mnt_sb = sb;
	mnt->mnt.mnt_root = root;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;

	return mnt;
}

static mount_s *__do_loopback(IN path_s *old_path)
{
	mount_s *mnt = ERR_PTR(-EINVAL),
			*old = real_mount(old_path->mnt);

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
static int do_loopback(IN path_s *path, const char *old_name)
{
	path_s old_path;
	mount_s *mnt = NULL, *parent;
	mountpoint_s *mp;
	int err;
	if (!old_name || !*old_name)
		return -EINVAL;

	err = kern_path(old_name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &old_path);
	if (err)
	return err;

	mp = lock_mount(path);
	if (IS_ERR(mp))
	{
		err = PTR_ERR(mp);
		goto out;
	}

	parent = real_mount(path->mnt);
	// if (!check_mnt(parent))
	// 	goto out2;

	mnt = __do_loopback(&old_path);
	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
		goto out2;
	}
	err = graft_tree(mnt, parent, mp);

out2:
	unlock_mount(mp);
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
static int do_new_mount(IN path_s *path, const char *fstype, int sb_flags,
				int mnt_flags, const char *name)
{
	fs_type_s	*type;
	fs_ctxt_s *fc;
	const char *subtype = NULL;
	int err = 0;

	if (!fstype)
		return -EINVAL;

	type = get_fs_type(fstype);
	if (!type)
		return -ENODEV;

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

	fc = fs_context_for_mount(type, sb_flags);
	put_filesystem(type);
	if (IS_ERR(fc))
		return PTR_ERR(fc);

	// if (subtype)
	// 	err = vfs_parse_fs_string(fc, "subtype",
	// 				  subtype, strlen(subtype));
	// if (!err && name)
	// 	err = vfs_parse_fs_string(fc, "source", name, strlen(name));
	// if (!err)
	// 	err = parse_monolithic_mount_data(fc, data);
	// if (!err && !mount_capable(fc))
	// 	err = -EPERM;
	if (!err)
		err = vfs_get_tree(fc);
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
int path_mount(const char *dev_name, IN path_s *path,
				const char *type_page, unsigned long flags)
{
	unsigned int mnt_flags = 0, sb_flags;
	int ret;

	// if ((flags & (MS_REMOUNT | MS_BIND)) == (MS_REMOUNT | MS_BIND))
	// 	return do_reconfigure_mnt(path, mnt_flags);
	// if (flags & MS_REMOUNT)
	// 	return do_remount(path, flags, sb_flags, mnt_flags, data_page);
	if (flags & MS_BIND)
		return do_loopback(path, dev_name);
	// if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
	// 	return do_change_type(path, flags);
	// if (flags & MS_MOVE)
	// 	return do_move_mount_old(path, dev_name);

	return do_new_mount(path, type_page, sb_flags, mnt_flags, dev_name);
}

// Linux function proto:
// long do_mount(const char *dev_name, const char __user *dir_name,
//		const char *type_page, unsigned long flags, void *data_page)
long do_mount(const char * dev_name, const char * dir_name,
				const char *type_page, unsigned long flags)
{
	path_s path;
	int ret;

	ret = user_path_at(AT_FDCWD, dir_name, flags, &path);
	if (ret)
		return ret;
	ret = path_mount(dev_name, &path, type_page, flags);
	return ret;
}

static void init_mount_tree(void)
{
	vfsmount_s *mnt;
	mount_s *m;
	path_s root;
	task_s *curr= curr_tsk;

	mnt = vfs_kern_mount(&rootfs_fs_type, 0, "rootfs", NULL);
	if (IS_ERR(mnt))
	{
		color_printk(RED, BLACK, "Can't create rootfs");
		while (1);
	}

	root.mnt = mnt;
	root.dentry = mnt->mnt_root;
	mnt->mnt_flags |= MNT_LOCKED;

	set_fs_pwd(curr->fs, &root);
	set_fs_root(curr->fs, &root);
}

void mnt_init(void)
{
	int err;

	kernfs_init();

	err = sysfs_init();
	if (err)
		color_printk(RED, BLACK, "sysfs_init error: %d\n", err);
	shmem_init();
	init_rootfs();
	init_mount_tree();
}

vfsmount_s *kern_mount(fs_type_s *type)
{
	vfsmount_s *mnt;
	mnt = vfs_kern_mount(type, SB_KERNMOUNT, type->name, NULL);
	return mnt;
}

void kern_unmount(vfsmount_s *mnt)
{
	/* release long term mount so mount point can be released */
	if (!IS_ERR_OR_NULL(mnt)) {
		mntput(mnt);
	}
}




extern PCB_u	task0_PCB;

void init_mount()
{
	list_hdr_init(&root_mnt.mnt_mounts);
	list_init(&root_mnt.mnt_child, &root_mnt);

	root_mnt.mnt.mnt_sb = root_sb;
	root_mnt.mnt_parent = &root_mnt;
	root_mnt.mnt_mountpoint =
	root_mnt.mnt.mnt_root = root_sb->s_root;
	root_mnt.mnt_mp = NULL;
}

void set_init_taskfs()
{
	task_s * curr = curr_tsk;
	// set cwd and root-dir of task1
	taskfs_s * taskfs_p = curr->fs;
	taskfs_p->pwd.dentry = 
	taskfs_p->root.dentry = root_sb->s_root;
	taskfs_p->pwd.mnt = 
	taskfs_p->root.mnt = &root_mnt.mnt;

	memcpy(task0_PCB.task.fs, taskfs_p, sizeof(taskfs_s));
}
// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/namei.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * Some corrections by tytso.
 */

/* [Feb 1997 T. Schoebel-Theuer] Complete rewrite of the pathname
 * lookup logic.
 */
/* [Feb-Apr 2000, AV] Rewrite to the new namespace architecture.
 */

#include <linux/init/init.h>
#include <linux/kernel/export.h>
#include <linux/kernel/kernel.h>
#include <linux/fs/fs.h>
#include <linux/fs/namei.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/mount.h>
#include <linux/fs/file.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/uaccess.h>
#include <linux/fs/internal.h>
#include <linux/fs/mount.h>


#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>

typedef struct nameidata
{
	path_s path;
	qstr_s last;
	path_s root;
	inode_s *inode; /* path.dentry.d_inode */

	int last_type;
	unsigned flags, state;
	unsigned depth;
	int total_link_count;
	filename_s *name;

	// struct filename	*name;
	// struct nameidata *saved;
	unsigned root_seq;
	int dfd;
} nameidata_s;

/*==============================================================================================*
 *								relative fuctions for vfs										*
 *==============================================================================================*/
static inline filename_s *__myos_getname()
{
	filename_s *name = kmalloc(sizeof(filename_s), GFP_KERNEL);
	if (name == NULL)
		return ERR_PTR(-ENOMEM);
	else
		return name;
}

// Linux function proto:
// struct filename *getname(const char __user * filename)
filename_s *getname(const char *u_filename)
{
	size_t len = strlen_user((void *)u_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);

	// Linux call stack:
	// struct filename *getname_flags(const char __user *filename,
	
	//					int flags, int *empty)
	// {
		filename_s *name = __myos_getname();
	// }

	name->len = len;
	name->name = kzalloc(len + 16, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
	}

	len = strncpy_from_user((void *)name->name, (void *)u_filename, len);
	if (len < 0)
	{
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
	}

	return name;
}

// Linux function proto:
// struct filename *getname_kernel(const char __user *filename)
filename_s *getname_kernel(const char *k_filename)
{
	filename_s *name = __myos_getname();

	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;

	name->name = kzalloc(len + 1, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
	}

	char * dest = strncpy((void *)name->name, (void *)k_filename, len);
	if (dest != name->name)
	{
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
	}

	return name;
}

// Linxu function proto:
// void putname(struct filename *name)
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}

// Linux function proto:
// static inline void set_nameidata(struct nameidata *p, int dfd,
// struct filename *name, const struct path *root)
static void set_nameidata(OUT nameidata_s *p, int dfd,
		IN filename_s *name, IN path_s *root)
{
	// linux call stack :
	// static void __set_nameidata(struct nameidata *p, int dfd,
	// struct filename *name)
	// {
		memset(p, 0, sizeof(nameidata_s));

		p->name = name;
		p->dfd = dfd;
		p->path.mnt = NULL;
		p->path.dentry = NULL;

		if (root != NULL)
		{
			p->root = *root;
		}
	// }
}

static void restore_nameidata(void)
{
	// nameidata_s *now = current->nameidata, *old = now->saved;

	// current->nameidata = old;
	// if (old)
	// 	old->total_link_count = now->total_link_count;
	// if (now->stack != now->internal)
	// 	kfree(now->stack);
}

static void terminate_walk(nameidata_s *nd)
{

}

/**
 * complete_walk - successful completion of path walk
 * @nd:  pointer nameidata
 *
 * If we had been in RCU mode, drop out of it and legitimize nd->path.
 * Revalidate the final result, unless we'd already done that during
 * the path walk or the filesystem doesn't ask for it.  Return 0 on
 * success, -error on failure.  In case of failure caller does not
 * need to drop nd->path.
 */
static int complete_walk(nameidata_s *nd)
{
	return 0;
}

/*==============================================================================================*
 *								private fuctions due with mount									*
 *==============================================================================================*/
// static bool choose_mountpoint(struct mount *m, const struct path *root,
// 			      struct path *path)
static bool choose_mountpoint(IN mount_s *m,
		IN const path_s *root, OUT path_s *path)
{
	while (mnt_has_parent(m))
	{
		dentry_s *mountpoint = m->mnt_mountpoint;

		m = m->mnt_parent;
		if (root->dentry == mountpoint &&
			root->mnt == &m->mnt)
			break;
		if (mountpoint != m->mnt.mnt_root)
		{
			path->mnt = &m->mnt;
			path->dentry = mountpoint;
			return true;
		}
	}
	return false;
}

// Linux function proto:
// static inline int traverse_mounts(struct path *path, bool *jumped,
// 				  int *count, unsigned lookup_flags)
static inline int traverse_mounts(IN OUT path_s *path)
{
	int ret_val = -ENOERR;
	// linux call stack :
	// __traverse_mounts(path, flags, jumped, count, lookup_flags);
	// {
	for (;;)
	{
		vfsmount_s *mounted = lookup_mnt(path);
		if (mounted) // ... in our namespace
		{
			path->mnt = mounted;
			path->dentry = mounted->mnt_root;
			// here we know it's positive
			continue;
		}
		return ret_val;
	}
	// }
}

// Linux function proto:
// static inline int handle_mounts(struct nameidata *nd, struct dentry *dentry,
// 			  struct path *path, struct inode **inode,
// 			  unsigned int *seqp)
static inline int handle_mounts(IN nameidata_s *nd,
		IN dentry_s *dentry, OUT path_s *path)
{
	path->mnt = nd->path.mnt;
	path->dentry = dentry;

	return traverse_mounts(path);
}

/*==============================================================================================*
 *								private fuctions for path walk									*
 *==============================================================================================*/
/**
 * path_get - get a reference to a path
 * @path: path to get the reference to
 *
 * Given a path increment the reference count to the dentry and the vfsmount.
 */
void path_get(const path_s *path)
{
	// mntget(path->mnt);
	dget(path->dentry);
}

/**
 * path_put - put a reference to a path
 * @path: path to put the reference to
 *
 * Given a path decrement the reference count to the dentry and the vfsmount.
 */
void path_put(const path_s *path)
{
	dput(path->dentry);
	// mntput(path->mnt);
}

/*
 * Do we need to follow links? We _really_ want to be able
 * to do this check without having to look at inode->i_op,
 * so we keep a cache of "no, this doesn't need follow_link"
 * for the common case.
 */
// Linux function proto:
// static const char *step_into(struct nameidata *nd, int flags,
//		     struct dentry *dentry, struct inode *inode, unsigned seq)
static const char *step_into(IN OUT nameidata_s *nd, IN dentry_s *dentry)
{
	path_s path;
	int err = handle_mounts(nd, dentry, &path);
	if (err < 0)
		return ERR_PTR(err);

	nd->path = path;

	return ERR_PTR(-ENOERR);
}

// Linux function proto:
// static struct dentry *__lookup_slow(const struct qstr *name,
//				    struct dentry *dir, unsigned int flags)
static dentry_s *__lookup_slow(IN qstr_s *name,
		IN dentry_s *dir, unsigned flags)
{
	dentry_s *dentry;

	// linux call stack :
	// struct dentry *d_alloc_parallel(struct dentry *parent,
	//			const struct qstr *name, wait_queue_head_t *wq)
	// {
	//		struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
	//		{
				dentry = __myos_d_alloc(dir->d_sb, name);
	//		}
	// }
	if (IS_ERR(dentry))
		return dentry;

	if (dir->d_inode->i_op->lookup(dir->d_inode, dentry, flags) == NULL)
	{
		// color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->d_name);
		kfree(dentry);
		return ERR_PTR(-ENOENT);
	}

	list_header_add_to_tail(&dir->d_subdirs, &dentry->d_child);
	dentry->d_parent = dir;

	return dentry;
}

/*
 * Parent directory has inode locked exclusive.  This is one
 * and only case when ->lookup() gets called on non in-lookup
 * dentries - as the matter of fact, this only gets called
 * when directory is guaranteed to have no in-lookup children
 * at all.
 */
static dentry_s *__lookup_hash(const qstr_s *name,
		dentry_s *base, unsigned int flags)
{
	dentry_s *dentry = __d_lookup(base, name);
	dentry_s *old;
	inode_s *dir = base->d_inode;

	if (dentry)
		return dentry;

	/* Don't create child dentry for a dead directory. */
	if (IS_DEADDIR(dir))
		return ERR_PTR(-ENOENT);

	dentry = __myos_d_alloc(base->d_sb, name);
	if (!dentry)
		return ERR_PTR(-ENOMEM);
	dentry->d_parent = base;
	list_header_add_to_tail(&base->d_subdirs, &dentry->d_child);

	old = dir->i_op->lookup(dir, dentry, flags);
	if (old) {
		dput(dentry);
		dentry = old;
	}
	return dentry;
}

// Linux function proto:
// static struct dentry *follow_dotdot(struct nameidata *nd,
//				 struct inode **inodep, unsigned *seqp)
static dentry_s *follow_dotdot(IN OUT nameidata_s *nd)
{
	dentry_s *parent, *old;

	if (nd->path.dentry == nd->path.mnt->mnt_root)
	{
		path_s path;
		if (!choose_mountpoint(
				real_mount(nd->path.mnt), &nd->root, &path))
			goto in_root;
		// here the "path" point to mountpoint of nd->path.mnt
		nd->path = path;
		/* we know that mountpoint was pinned */
	}
	old = nd->path.dentry;
	parent = old->d_parent;

	return parent;
in_root:
	return NULL;
}

// Linux function proto:
// static const char *handle_dots(struct nameidata *nd, int type)
static const char *handle_dots(IN nameidata_s *nd)
{
	if (nd->last_type == LAST_DOTDOT)
	{
		const char *error = NULL;
		dentry_s *parent;

		parent = follow_dotdot(nd);
		if (IS_ERR(parent))
			return ERR_CAST(parent);
		if (parent == NULL)
			error = step_into(nd, nd->path.dentry);
		else
			error = step_into(nd, parent);
		if (error)
			return error;
	}
	return NULL;
}

// Linux function proto:
// static const char *walk_component(struct nameidata *nd, int flags)
static const char *walk_component(IN nameidata_s *nd)
{
	dentry_s *dentry, *parent = nd->path.dentry;
	inode_s *inode;

	/*
	 * "." and ".." are special - ".." especially so because it has
	 * to be able to know about the current root directory and
	 * parent relationships.
	 */
	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}

	// linux call stack :
	//	static struct dentry *lookup_fast(struct nameidata *nd,
	//				  struct inode **inode, unsigned *seqp)
	// {
		dentry = __d_lookup(parent, &nd->last);
	// }
	if (IS_ERR(dentry))
		return ERR_CAST(dentry);

	if (dentry == NULL)
	{
		// linux call stack :
		//	static struct dentry *lookup_slow(const struct qstr *name,
		//			  struct dentry *dir, unsigned int flags)
		// {
			dentry = __lookup_slow(&nd->last, parent, nd->flags);
		// }
		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
	}

	return step_into(nd, dentry);
}

/*
 * Name resolution.
 * This is the basic name resolution function, turning a pathname into
 * the final dentry. We expect 'base' to be positive and a directory.
 *
 * Returns 0 and nd will have valid dentry and mnt on success.
 * Returns error and drops reference to input namei data on failure.
 */
// Linux function proto:
// static int link_path_walk(const char *name, struct nameidata *nd)
static int link_path_walk(IN const char *name, IN OUT nameidata_s *nd)
{
	int err;

	nd->last_type = LAST_ROOT;
	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name == '/')
		name++;
	if (*name == '\0')
	{
		return 0;
	}

	/* At this point we know we have a real path component. */
	for (;;)
	{
		int type;
		const char *link;

		// compute length of current dirname
		int i = 0;
		while (name[i] != 0 && name[i] != '/')
			i++;

		type = LAST_NORM;
		// detect . and .. dir
		if (name[0] == '.')
			switch (i)
			{
			case 2:
				if (name[1] == '.')
				{
					type = LAST_DOTDOT;
				}
				break;
			case 1:
				type = LAST_DOT;
			}

		nd->last.len = i;
		nd->last.name = name;
		nd->last_type = type;

		name += nd->last.len;
		/*
		 * If it wasn't NUL, we know it was '/'. Skip that
		 * slash, and continue until no more slashes.
		 */
		while (*name == '/')
			name++;

		if (*name == 0)
			return -ENOERR;
		else
		{
			link = walk_component(nd);
			if (IS_ERR(link))
				return PTR_ERR(link);
		}
	}
}

/*==============================================================================================*
 *								private fuctions for path walk									*
 *==============================================================================================*/
/* must be paired with terminate_walk() */
// Linux function proto:
// static const char *path_init(struct nameidata *nd, unsigned flags)
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;

	nd->flags = flags;

	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;

	return s;
}

// Linux function proto:
// static inline const char *lookup_last(struct nameidata *nd)
static inline const char *lookup_last(IN nameidata_s *nd)
{
	return walk_component(nd);
}

// Linux function proto:
/* Returns 0 and nd will be valid on success; Retuns error, otherwise. */
// static int path_lookupat(struct nameidata *nd, unsigned flags, struct path *path)
static int path_lookupat(IN nameidata_s *nd, unsigned flags, OUT path_s *path)
{
	const char *s = path_init(nd, flags);
	int err;

	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;

	if (err == 0)
		err = complete_walk(nd);

	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
	terminate_walk(nd);
	return err;
}

// Linux function proto:
// int filename_lookup(int dfd, struct filename *name, unsigned flags,
// 		    struct path *path, struct path *root)
int filename_lookup(int dfd, IN filename_s *name, unsigned flags,
				OUT path_s *path, IN path_s *root)
{
	int retval;
	nameidata_s nd;
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
	// if (unlikely(retval == -ECHILD))
	//	 retval = path_lookupat(&nd, flags, path);

	putname(name);
	return retval;
}

/* Returns 0 and nd will be valid on success; Retuns error, otherwise. */
static int path_parentat(nameidata_s *nd, unsigned flags, path_s *parent)
{
	const char *s = path_init(nd, flags);
	int err = link_path_walk(s, nd);
	if (err == 0)
		err = complete_walk(nd);
	if (err == 0) {
		*parent = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
	terminate_walk(nd);
	return err;
}

static int filename_parentat(int dfd, filename_s *name,
		unsigned int flags, path_s *parent, qstr_s *last, int *type)
{
	int retval;
	nameidata_s nd;

	if (IS_ERR(name))
		return PTR_ERR(name);
	set_nameidata(&nd, dfd, name, NULL);
	retval = path_parentat(&nd, flags, parent);
	if (retval == -ECHILD)
		retval = path_parentat(&nd, flags, parent);
	if (retval == 0) {
		*last = nd.last;
		*type = nd.last_type;
	}
	restore_nameidata();
	return retval;
}

int kern_path(const char *name, unsigned int flags, OUT path_s *path)
{
	return filename_lookup(AT_FDCWD, getname_kernel(name),
					flags, path, NULL);
}

/*==============================================================================================*
 *									fuctions for open											*
 *==============================================================================================*/
// Linux function proto:
// static const char *open_last_lookups(struct nameidata *nd,
//			file_s *file, const struct open_flags *op)
static const char *open_last_lookups(IN nameidata_s *nd, int open_flag)
{
	dentry_s *dentry, *dir = nd->path.dentry;
	const char *res;

	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}

	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
		{
			dentry = __lookup_slow(&nd->last, dir, nd->flags);
		}

		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
		if (dentry)
			goto finish_lookup;
	}
	// else
	// {
	// 	dentry = lookup_open(nd, file, op);
	// }

finish_lookup:
	res = step_into(nd, dentry);
	return res;
}

/*
 * Handle the last step of open()
 */
// Linux function proto:
// static int do_open(struct nameidata *nd,
//		   file_s *file, const struct open_flags *op)
static int do_open(IN nameidata_s *nd, OUT file_s *file, int open_flag)
{
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

	vfs_open(&nd->path, file);
}

// Linux function proto:
// static file_s *path_openat(struct nameidata *nd,
// 			const struct open_flags *op, unsigned flags)
// open the path in nd->name
static file_s *path_openat(IN nameidata_s *nd,
		const open_flags_s *op, unsigned flags)
{
	file_s *file;
	int error;

	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;

	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
	if (error == 0)
		error = do_open(nd, file, flags);
	terminate_walk(nd);
	if (error == 0)
	{
		// if (file->f_mode & FMODE_OPENED)
			return file;
		// error = -EINVAL;
	}
	fput(file);
	return ERR_PTR(error);
}

// Linux function proto:
// int user_path_at_empty(int dfd, const char __user *name, unsigned flags,
// 		 struct path *path, int *empty)
int user_path_at_empty(int dfd, const char *name,
		unsigned flags, OUT path_s *path)
{
	filename_s *fn = getname(name);
	return filename_lookup(dfd, fn, flags, path, NULL);
}

// Linux function proto:
// file_s *do_filp_open(int dfd, struct filename *pathname,
//			const struct open_flags *op)
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;

	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
	return filp;
}

/*==============================================================================================*
 *									fuctions for file create									*
 *==============================================================================================*/
static dentry_s *filename_create(int dfd, filename_s *name,
		path_s *path, unsigned int lookup_flags)
{
	dentry_s *dentry = ERR_PTR(-EEXIST);
	qstr_s last;
	int type;
	int err2;
	int error;
	bool is_dir = (lookup_flags & LOOKUP_DIRECTORY);

	/*
	 * Note that only LOOKUP_REVAL and LOOKUP_DIRECTORY matter here. Any
	 * other flags passed in are ignored!
	 */
	lookup_flags &= LOOKUP_REVAL;

	error = filename_parentat(dfd, name, lookup_flags, path, &last, &type);
	if (error)
		return ERR_PTR(error);

	/*
	 * Yucky last component or no last component at all?
	 * (foo/., foo/.., /////)
	 */
	if (type != LAST_NORM)
		goto out;

	/*
	 * Do the final lookup.
	 */
	lookup_flags |= LOOKUP_CREATE | LOOKUP_EXCL;
	dentry = __lookup_hash(&last, path->dentry, lookup_flags);
	if (IS_ERR(dentry))
		goto out;

	/*
	 * Special case - lookup gave negative, but... we had foo/bar/
	 * From the vfs_mknod() POV we just have a negative dentry -
	 * all is fine. Let's be bastards - you had / on the end, you've
	 * been asking for (non-existent) directory. -ENOENT for you.
	 */
	if (!is_dir && last.name[last.len] != '\0') {
		error = -ENOENT;
		goto fail;
	}
	return dentry;

fail:
	dput(dentry);
	dentry = ERR_PTR(error);
out:
	path_put(path);
	return dentry;
}

dentry_s *kern_path_create(int dfd, const char *pathname,
		path_s *path, unsigned int lookup_flags)
{
	filename_s *filename = getname_kernel(pathname);
	dentry_s *res = filename_create(dfd, filename, path, lookup_flags);

	putname(filename);
	return res;
}

void done_path_create(path_s *path, dentry_s *dentry)
{
	dput(dentry);
	path_put(path);
}

inline dentry_s *user_path_create(int dfd, const char *pathname,
		path_s *path, unsigned int lookup_flags)
{
	filename_s *filename = getname(pathname);
	dentry_s *res = filename_create(dfd, filename, path, lookup_flags);

	putname(filename);
	return res;
}

/*==============================================================================================*
 *									fuctions for mknod											*
 *==============================================================================================*/
/**
 * vfs_mknod - create device node or file
 * @mnt_userns:	user namespace of the mount the inode was found from
 * @dir:	inode of @dentry
 * @dentry:	pointer to dentry of the base directory
 * @mode:	mode of the new device node or file
 * @dev:	device number of device to create
 *
 * Create a device node or file.
 *
 * If the inode has been found through an idmapped mount the user namespace of
 * the vfsmount must be passed through @mnt_userns. This function will then take
 * care to map the inode according to @mnt_userns before checking permissions.
 * On non-idmapped mounts or if permission checking is to be performed on the
 * raw inode simply passs init_user_ns.
 */
int vfs_mknod(inode_s *dir, dentry_s *dentry, umode_t mode, dev_t dev)
{
	bool is_whiteout = S_ISCHR(mode) && dev == WHITEOUT_DEV;
	int error = 0;

	// if ((S_ISCHR(mode) || S_ISBLK(mode)) && !is_whiteout &&
	//     !capable(CAP_MKNOD))
	// 	return -EPERM;

	if (!dir->i_op->mknod)
		return -EPERM;

	// error = devcgroup_inode_mknod(mode, dev);
	// if (error)
	// 	return error;

	// error = security_inode_mknod(dir, dentry, mode, dev);
	// if (error)
	// 	return error;

	error = dir->i_op->mknod(dir, dentry, mode, dev);
	// if (!error)
	// 	fsnotify_create(dir, dentry);
	return error;
}

/*==============================================================================================*
 *										fuctions for mkdir										*
 *==============================================================================================*/
long do_creatat(int dfd, filename_s *name, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	inode_s *dir;
	int error;
	unsigned int lookup_flags = LOOKUP_DIRECTORY;

retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out_putname;

	// if (!IS_POSIXACL(path.dentry->d_inode))
	// 	mode &= ~current_umask();
	// error = security_path_mkdir(&path, dentry, mode);
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
	// if (retry_estale(error, lookup_flags)) {
	// 	lookup_flags |= LOOKUP_REVAL;
	// 	goto retry;
	// }
out_putname:
	putname(name);
	return error;
}

MYOS_SYSCALL_DEFINE2(creat, const char *, pathname, umode_t, mode)
{
	return do_creatat(AT_FDCWD, getname(pathname), mode);
}

/*==============================================================================================*
 *										fuctions for mkdir										*
 *==============================================================================================*/
/**
 * vfs_mkdir - create directory
 * @mnt_userns:	user namespace of the mount the inode was found from
 * @dir:	inode of @dentry
 * @dentry:	pointer to dentry of the base directory
 * @mode:	mode of the new directory
 *
 * Create a directory.
 *
 * If the inode has been found through an idmapped mount the user namespace of
 * the vfsmount must be passed through @mnt_userns. This function will then take
 * care to map the inode according to @mnt_userns before checking permissions.
 * On non-idmapped mounts or if permission checking is to be performed on the
 * raw inode simply passs init_user_ns.
 */
int vfs_mkdir(inode_s *dir, dentry_s *dentry, umode_t mode)
{
	int error = -ENOENT;
	if (!dir->i_op->mkdir)
		return -EPERM;

	mode &= (S_IRWXUGO|S_ISVTX);
	error = dir->i_op->mkdir(dir, dentry, mode);
	return error;
}

long do_mkdirat(int dfd, filename_s *name, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	inode_s *dir;
	int error;
	unsigned int lookup_flags = LOOKUP_DIRECTORY;

retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out_putname;

	// if (!IS_POSIXACL(path.dentry->d_inode))
	// 	mode &= ~current_umask();
	// error = security_path_mkdir(&path, dentry, mode);
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
	// if (retry_estale(error, lookup_flags)) {
	// 	lookup_flags |= LOOKUP_REVAL;
	// 	goto retry;
	// }
out_putname:
	putname(name);
	return error;
}

MYOS_SYSCALL_DEFINE2(mkdir, const char *, pathname, umode_t, mode)
{
	return do_mkdirat(AT_FDCWD, getname(pathname), mode);
}

/*==============================================================================================*
 *										fuctions for rmdir										*
 *==============================================================================================*/
/**
 * vfs_rmdir - remove directory
 * @mnt_userns:	user namespace of the mount the inode was found from
 * @dir:	inode of @dentry
 * @dentry:	pointer to dentry of the base directory
 *
 * Remove a directory.
 *
 * If the inode has been found through an idmapped mount the user namespace of
 * the vfsmount must be passed through @mnt_userns. This function will then take
 * care to map the inode according to @mnt_userns before checking permissions.
 * On non-idmapped mounts or if permission checking is to be performed on the
 * raw inode simply passs init_user_ns.
 */
int vfs_rmdir(inode_s *dir, dentry_s *dentry)
{
	int error = 0;

	if (!dir->i_op->rmdir)
		return -EPERM;

	dget(dentry);

	error = -EBUSY;
	error = dir->i_op->rmdir(dir, dentry);
	if (error)
		goto out;

	dentry->d_inode->i_flags |= S_DEAD;

out:
	dput(dentry);
	return error;
}

long do_rmdir(int dfd, filename_s *name)
{
	int error;
	dentry_s *dentry;
	path_s path;
	qstr_s last;
	int type;
	unsigned int lookup_flags = 0;
retry:
	error = filename_parentat(dfd, name, lookup_flags, &path, &last, &type);
	if (error)
		goto exit1;

	switch (type) {
	case LAST_DOTDOT:
		error = -ENOTEMPTY;
		goto exit2;
	case LAST_DOT:
		error = -EINVAL;
		goto exit2;
	case LAST_ROOT:
		error = -EBUSY;
		goto exit2;
	}

	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto exit3;
	if (!dentry->d_inode) {
		error = -ENOENT;
		goto exit4;
	}
	if (!S_ISDIR(dentry->d_inode->i_mode))
	{
		error = -ENOTDIR;
		goto exit4;
	}

	error = vfs_rmdir(path.dentry->d_inode, dentry);
exit4:
	dput(dentry);
exit3:
exit2:
	path_put(&path);
exit1:
	putname(name);
	return error;
}

MYOS_SYSCALL_DEFINE1(rmdir, const char *, pathname)
{
	return do_rmdir(AT_FDCWD, getname(pathname));
}

/*==============================================================================================*
 *										fuctions for unlink										*
 *==============================================================================================*/
/**
 * vfs_unlink - unlink a filesystem object
 * @mnt_userns:	user namespace of the mount the inode was found from
 * @dir:	parent directory
 * @dentry:	victim
 * @delegated_inode: returns victim inode, if the inode is delegated.
 *
 * The caller must hold dir->i_mutex.
 *
 * If vfs_unlink discovers a delegation, it will return -EWOULDBLOCK and
 * return a reference to the inode in delegated_inode.  The caller
 * should then break the delegation on that inode and retry.  Because
 * breaking a delegation may take a long time, the caller should drop
 * dir->i_mutex before doing so.
 *
 * Alternatively, a caller may pass NULL for delegated_inode.  This may
 * be appropriate for callers that expect the underlying filesystem not
 * to be NFS exported.
 *
 * If the inode has been found through an idmapped mount the user namespace of
 * the vfsmount must be passed through @mnt_userns. This function will then take
 * care to map the inode according to @mnt_userns before checking permissions.
 * On non-idmapped mounts or if permission checking is to be performed on the
 * raw inode simply passs init_user_ns.
 */
int vfs_unlink(inode_s *dir, dentry_s *dentry)
{
	struct inode *target = dentry->d_inode;
	int error = 0;

	if (!dir->i_op->unlink)
		return -EPERM;

	// error = security_inode_unlink(dir, dentry);
	// if (!error) {
		error = dir->i_op->unlink(dir, dentry);
	// 	if (!error) {
	// 		dont_mount(dentry);
	// 		detach_mounts(dentry);
	// 	}
	// }

	return error;
}

/*
 * Make sure that the actual truncation of the file will occur outside its
 * directory's i_mutex.  Truncate can take a long time if there is a lot of
 * writeout happening, and we don't want to prevent access to the directory
 * while waiting on the I/O.
 */
long do_unlinkat(int dfd, filename_s *name)
{
	int error;
	dentry_s *dentry;
	path_s path;
	qstr_s last;
	int type;
	inode_s *inode = NULL;
	unsigned int lookup_flags = 0;
retry:
	error = filename_parentat(dfd, name, lookup_flags, &path, &last, &type);
	if (error)
		goto exit1;

	error = -EISDIR;
	if (type != LAST_NORM)
		goto exit2;

	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (!IS_ERR(dentry)) {
// 		/* Why not before? Because we want correct error value */
// 		if (last.name[last.len])
// 			goto slashes;
		inode = dentry->d_inode;
		if (S_ISDIR(inode->i_mode))
		{
			error = -EISDIR;
			goto exit3;
		}

		error = vfs_unlink(path.dentry->d_inode, dentry);
exit3:
		dput(dentry);
	}
	if (inode)
		iput(inode);	/* truncate the inode here */
	inode = NULL;

exit2:
	path_put(&path);
exit1:
	putname(name);
	return error;
}

MYOS_SYSCALL_DEFINE1(unlink, const char *, pathname)
{
	return do_unlinkat(AT_FDCWD, getname(pathname));
}

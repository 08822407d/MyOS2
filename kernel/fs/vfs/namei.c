#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <sys/fcntl.h>

#include <include/err.h>
#include <include/proto.h>
#include <include/printk.h>
#include <include/fs/vfs.h>
#include <include/fs/namei.h>
#include <include/fs/dcache.h>
#include <include/fs/namespace.h>
#include <include/fs/mount.h>

#include <arch/amd64/include/arch_proto.h>

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
inline filename_s *__getname()
{
	filename_s *name = kmalloc(sizeof(filename_s));
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
	if (len > CONST_4K)
		return ERR_PTR(-ENAMETOOLONG);

	// Linux call stack:
	// struct filename *getname_flags(const char __user *filename,
	//					int flags, int *empty)
	// {
		filename_s *name = __getname();
	// }

	name->len = len;
	name->name = kmalloc(len + 1);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
	}

	memset((void *)name->name, 0, len + 1);
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
	filename_s *name = __getname();

	size_t len = strlen((void *)k_filename);
	if (len > CONST_4K)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;

	name->name = kmalloc(len + 1);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
	}

	memset((void *)name->name, 0, len + 1);
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
// static inline void set_nameidata(struct nameidata *p, int dfd, struct filename *name,
// 			  const struct path *root)
static void set_nameidata(OUT nameidata_s *p, int dfd, IN filename_s *name, IN path_s *root)
{
	// linux call stack :
	// static void __set_nameidata(struct nameidata *p, int dfd, struct filename *name)
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
}

/*==============================================================================================*
 *								private fuctions due with mount									*
 *==============================================================================================*/
// static bool choose_mountpoint(struct mount *m, const struct path *root,
// 			      struct path *path)
static bool choose_mountpoint(IN mount_s *m, IN const path_s *root, OUT path_s *path)
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
static inline int handle_mounts(IN nameidata_s *nd, IN dentry_s *dentry, OUT path_s *path)
{
	path->mnt = nd->path.mnt;
	path->dentry = dentry;

	return traverse_mounts(path);
}

/*==============================================================================================*
 *								private fuctions for path walk									*
 *==============================================================================================*/
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
static dentry_s *__lookup_slow(IN qstr_s *name, IN dentry_s *dir)
{
	dentry_s *dentry;

	// linux call stack :
	// struct dentry *d_alloc_parallel(struct dentry *parent,
	//			const struct qstr *name, wait_queue_head_t *wq)
	//					||
	//					\/
	// struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
	//					||
	//					\/
	dentry = __d_alloc(name);
	if (IS_ERR(dentry))
		return dentry;

	if (dir->dir_inode->inode_ops->lookup(dir->dir_inode, dentry) == NULL)
	{
		color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->name);
		kfree(dentry->name);
		kfree(dentry);
		return ERR_PTR(-ENOENT);
	}

	list_hdr_append(&dir->childdir_lhdr, &dentry->dirent_list);
	dentry->parent = dir;

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
	parent = old->parent;

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
			dentry = __lookup_slow(&nd->last, parent);
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
	nd->last_type = LAST_ROOT;
	int err;

	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name == '/')
		name++;
	if (!*name)
	{
		return 0;
	}

	/* At this point we know we have a real path component. */
	for (;;)
	{
		int type;

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
		while (*name != 0 && *name == '/')
			name++;

		if (*name == 0)
			return 0;
		else
			walk_component(nd);
	}
}

/*==============================================================================================*
 *								private fuctions for path walk									*
 *==============================================================================================*/
/* must be paired with terminate_walk() */
// Linux function proto:
// static const char *path_init(struct nameidata *nd, unsigned flags)
static const char *path_init(OUT nameidata_s *nd)
{
	const char *s = nd->name->name;

	nd->root = curr_tsk->task_fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = curr_tsk->task_fs->pwd;

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
	const char *s = path_init(nd);
	int err;

	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;

	// if (!err)
	// 	err = complete_walk(nd);

	if (!err)
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
int filename_lookup(int dfd, IN filename_s *name, unsigned flags, OUT path_s *path)
{
	int retval;
	nameidata_s nd;
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
	// if (unlikely(retval == -ECHILD))
	//	 retval = path_lookupat(&nd, flags, path);
	// if (unlikely(retval == -ESTALE))
	//	 retval = path_lookupat(&nd, flags | LOOKUP_REVAL, path);

	putname(name);
	return retval;
}

/* Returns 0 and nd will be valid on success; Retuns error, otherwise. */
static int path_parentat(nameidata_s *nd, path_s *parent)
{
	const char *s = path_init(nd);
	int err = link_path_walk(s, nd);
	// if (!err)
	// 	err = complete_walk(nd);
	if (!err) {
		*parent = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
	terminate_walk(nd);
	return err;
}

static filename_s *filename_parentat(int dfd, filename_s *name,
				unsigned int flags, path_s *parent, qstr_s *last)
{
	int retval;
	nameidata_s nd;

	if (IS_ERR(name))
		return name;
	set_nameidata(&nd, dfd, name, NULL);
	retval = path_parentat(&nd, parent);
	if (retval == -ECHILD)
		retval = path_parentat(&nd, parent);
	// if (retval == -ESTALE)
	// 	retval = path_parentat(&nd, flags | LOOKUP_REVAL, parent);
	if (!retval) {
		*last = nd.last;
		// *type = nd.last_type;
		// audit_inode(name, parent->dentry, AUDIT_INODE_PARENT);
	} else {
		putname(name);
		name = ERR_PTR(retval);
	}
	// restore_nameidata();
	return name;
}

int kern_path(const char *name, unsigned int flags, OUT path_s *path)
{
	return filename_lookup(AT_FDCWD, getname_kernel(name), flags, path);
}

// Linux function proto:
// static const char *open_last_lookups(struct nameidata *nd,
//			struct file *file, const struct open_flags *op)
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
			dentry = __lookup_slow(&nd->last, dir);
		}

		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
		if (likely(dentry))
			goto finish_lookup;
	}

finish_lookup:
	res = step_into(nd, dentry);
	return res;
}

/*
 * Handle the last step of open()
 */
// Linux function proto:
// static int do_open(struct nameidata *nd,
//		   struct file *file, const struct open_flags *op)
static int do_open(IN nameidata_s *nd, OUT file_s *file, int open_flag)
{
	__vfs_open(&nd->path, file);
}

// Linux function proto:
// static struct file *path_openat(struct nameidata *nd,
// 			const struct open_flags *op, unsigned flags)
static file_s *path_openat(IN nameidata_s *nd, unsigned flags)
{
	file_s *file;
	int error;

	file = kmalloc(sizeof(file_s));
	if (IS_ERR(file))
		return file;

	memset(file, 0, sizeof(file_s));
	const char *s = path_init(nd);
	while (!(error = link_path_walk(s, nd)) &&
		   (s = open_last_lookups(nd, flags)) != NULL)
		;
	if (!error)
		error = do_open(nd, file, flags);
	terminate_walk(nd);

	return file;
}

// Linux function proto:
// struct file *do_filp_open(int dfd, struct filename *pathname,
//			const struct open_flags *op)
file_s *do_filp_open(int dfd, IN filename_s * name, int flags)
{
	nameidata_s nd;
	file_s *filp;

	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ESTALE)))
	// 	filp = path_openat(&nd, op, flags | LOOKUP_REVAL);
	// restore_nameidata();
	return filp;
}

// Linux function proto:
// int user_path_at_empty(int dfd, const char __user *name, unsigned flags,
// 		 struct path *path, int *empty)
int user_path_at_empty(int dfd, const char *name, unsigned flags, OUT path_s *path)
{
	filename_s *fn = getname(name);
	return filename_lookup(dfd, fn, flags, path);
}

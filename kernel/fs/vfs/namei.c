#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <sys/fcntl.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/fs/vfs.h>
#include <include/fs/namei.h>
#include <include/fs/dcache.h>
#include <include/fs/namespace.h>
#include <include/fs/mount.h>

#include <arch/amd64/include/arch_proto.h>

typedef struct nameidata {
	path_s		path;
	qstr_s		last;
	path_s		root;
	inode_s	*	inode; /* path.dentry.d_inode */

	int			last_type;
	unsigned	flags, state;
	unsigned	depth;
	int			total_link_count;
	filename_s *name;

	// struct filename	*name;
	// struct nameidata *saved;
	unsigned	root_seq;
	int			dfd;
} nameidata_s;

/*==============================================================================================*
 *								relative fuctions for vfs										*
 *==============================================================================================*/
// Linux function proto:
// struct filename * getname(const char __user * filename)
filename_s * getname(const char * u_filename)
{
	// Linux call stack:
	// struct filename * getname_flags(const char __user *filename,
	//					int flags, int *empty)
	//					||
	//					\/
	size_t len = strnlen_user((void *)u_filename, CONST_4K);
	filename_s * name = kmalloc(sizeof(filename_s));
	name->len = len;
	name->name = kmalloc(len + 1);
	if (name->name != NULL)
	{
		memset((void *)name->name, 0, len + 1);
		strncpy_from_user((void *)name->name, (void *)u_filename, len);
	}

	return name;
}

// Linux function proto:
// struct filename * getname_kernel(const char __user * filename)
filename_s * getname_kernel(const char * k_filename)
{
	// Linux call stack:
	// struct filename * getname_flags(const char __user *filename,
	//					int flags, int *empty)
	//					||
	//					\/
	size_t len = strnlen((void *)k_filename, CONST_4K);
	filename_s * name = kmalloc(sizeof(filename_s));
	name->len = len;
	name->name = kmalloc(len + 1);
	if (name->name != NULL)
	{
		memset((void *)name->name, 0, len + 1);
		strncpy((void *)name->name, (void *)k_filename, len);
	}

	return name;
}

// Linxu function proto:
// void putname(struct filename *name)
void putname(filename_s * name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}

/*==============================================================================================*
 *								private fuctions due with mount									*
 *==============================================================================================*/
// static bool choose_mountpoint(struct mount *m, const struct path *root,
// 			      struct path *path)
static bool choose_mountpoint(mount_s * m, const path_s * root, path_s * path)
{
	while (mnt_has_parent(m)) {
		dentry_s * mountpoint = m->mnt_mountpoint;

		m = m->mnt_parent;
		if (root->dentry == mountpoint &&
			     root->mnt == &m->mnt)
			break;
		if (mountpoint != m->mnt.mnt_root) {
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
static inline int traverse_mounts(path_s * path)
{
	int ret_val = -ENOERR;
	// linux call stack :
	// __traverse_mounts(path, flags, jumped, count, lookup_flags);
	// {
	for (;;)
	{
		vfsmount_s * mounted = lookup_mnt(path);
		if (mounted)	// ... in our namespace
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
static inline int handle_mounts(nameidata_s * nd, dentry_s * dentry, path_s * path)
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
static const char * step_into(nameidata_s * nd, dentry_s * dentry)
{
	path_s path;

	handle_mounts(nd, dentry, &path);
	nd->path = path;

	return NULL;
}

// Linux function proto:
// static struct dentry *__lookup_slow(const struct qstr *name,
//				    struct dentry *dir, unsigned int flags)
static dentry_s * __lookup_slow(qstr_s * name, dentry_s * dir)
{
	dentry_s *	dentry = NULL;
	
	// linux call stack :
	// struct dentry *d_alloc_parallel(struct dentry *parent,
	//			const struct qstr *name, wait_queue_head_t *wq)
	//					||
	//					\/
	// struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
	//					||
	//					\/
	dentry = __d_alloc(name);

	if(dir->dir_inode->inode_ops->lookup(dir->dir_inode, dentry) == NULL)
	{
		color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->name);
		kfree(dentry->name);
		kfree(dentry);
		return NULL;
	}

	list_hdr_append(&dir->childdir_lhdr, &dentry->dirent_list);
	dentry->parent = dir;

	return dentry;
}

// Linux function proto:
// static struct dentry *follow_dotdot(struct nameidata *nd,
//				 struct inode **inodep, unsigned *seqp)
static dentry_s * follow_dotdot(nameidata_s * nd)
{
	dentry_s * parent, * old;

	if (nd->path.dentry == nd->path.mnt->mnt_root) {
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
static const char * handle_dots(nameidata_s * nd)
{
	const char * ret_val = NULL;
	if (nd->last_type == LAST_DOTDOT)
	{
		dentry_s * parent;

		parent = follow_dotdot(nd);
		ret_val = step_into(nd, parent);
	}
	return ret_val;
}

// Linux function proto:
// static const char *walk_component(struct nameidata *nd, int flags)
static const char * walk_component(nameidata_s * nd)
{
	dentry_s * dentry, * parent = nd->path.dentry;
	inode_s * inode;

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
	//					||
	//					\/
	dentry = __d_lookup(parent, &nd->last);

	if (dentry == NULL)
	{
		// linux call stack :
		//	static struct dentry *lookup_slow(const struct qstr *name,
		//			  struct dentry *dir, unsigned int flags)
		//					||
		//					\/
		dentry = __lookup_slow(&nd->last, parent);
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
static int link_path_walk(const char * name, nameidata_s * nd)
{
	nd->last_type = LAST_ROOT;

	while (*name=='/')
		name++;
	if (!*name) {
		return 0;
	}

	/* At this point we know we have a real path component. */
	for(;;) {
		int type;

		// compute length of current dirname
		int i = 0;
		while (name[i] != 0 && name[i] != '/')
			i++;

		type = LAST_NORM;
		// detect . and .. dir
		if (name[0] == '.') switch (i) {
			case 2:
				if (name[1] == '.') {
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
// Linux function proto:
// static void __set_nameidata(struct nameidata *p, int dfd, struct filename *name)
static void __set_nameidata(nameidata_s * p, int dfd, filename_s * name, path_s * root)
{
	memset(p, 0, sizeof(nameidata_s));

	p->name = name;
	p->dfd = dfd;
	p->path.mnt = NULL;
	p->path.dentry = NULL;

	if (root != NULL)
	{
		p->root = *root;
	}
}

static void terminate_walk(nameidata_s * nd)
{

}

/* must be paired with terminate_walk() */
// Linux function proto:
// static const char *path_init(struct nameidata *nd, unsigned flags)
static const char *path_init(nameidata_s * nd)
{
	const char * s = nd->name->name;
	nd->root = curr_tsk->task_fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = curr_tsk->task_fs->pwd;
	
	return s;
}

// Linux function proto:
// static const char *open_last_lookups(struct nameidata *nd,
//			struct file *file, const struct open_flags *op)
static const char *open_last_lookups(nameidata_s * nd, file_s * file, int open_flag)
{
	dentry_s * dentry, * dir = nd->path.dentry;

	if (nd->last_type != LAST_NORM) {
		if (nd->depth)
		return handle_dots(nd);
	}

	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
		{
			dentry = __lookup_slow(&nd->last, dir);
		}
	}

	return step_into(nd, dentry);
}

/*
 * Handle the last step of open()
 */
// Linux function proto:
// static int do_open(struct nameidata *nd,
//		   struct file *file, const struct open_flags *op)
static int do_open(nameidata_s *nd, file_s *file, int open_flag)
{
	__vfs_open(&nd->path, file);
}

// Linux function proto:
// static struct file *path_openat(struct nameidata *nd,
// 			const struct open_flags *op, unsigned flags)
static file_s * path_openat(nameidata_s * nd, unsigned flags)
{
	file_s * file;
	int err;

	file = kmalloc(sizeof(file_s));
	memset(file, 0, sizeof(file_s));
	const char *s = path_init(nd);
	while (!( err = link_path_walk(s, nd)) &&
			(s = open_last_lookups(nd, file, flags)) != NULL)
		;
	if (!err)
		err = do_open(nd, file, flags);
	terminate_walk(nd);

	return file;
}

// Linux function proto:
// struct file *do_filp_open(int dfd, struct filename *pathname,
//			const struct open_flags *op)
file_s * do_filp_open(int dfd, filename_s * name, int flags)
{
	nameidata_s nd;
	file_s * filp;

	__set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, flags);

	return filp;
}

// Linux function proto:
// static inline const char *lookup_last(struct nameidata *nd)
static inline const char * lookup_last(nameidata_s * nd)
{
	return walk_component(nd);
}

// Linux function proto:
/* Returns 0 and nd will be valid on success; Retuns error, otherwise. */
// static int path_lookupat(struct nameidata *nd, unsigned flags, struct path *path)
static int path_lookupat(nameidata_s * nd, unsigned flags, path_s * path)
{
	const char *s = path_init(nd);
	int err;

	while (!(err = link_path_walk(s, nd)) &&
	       (s = lookup_last(nd)) != NULL)
		;

	// if (!err && nd->flags & LOOKUP_DIRECTORY)
		// if (!d_can_lookup(nd->path.dentry))
		// 	err = -ENOTDIR;
	if (!err) {
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
int filename_lookup(int dfd, filename_s * name, unsigned flags, path_s * path)
{
	int retval;
	nameidata_s nd;

	__set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);

	return retval;
}

int kern_path(const char * name, unsigned int flags, path_s * path)
{
	return filename_lookup(AT_FDCWD, getname_kernel(name), flags, path);
}

/**
 * vfs_path_lookup - lookup a file path relative to a dentry-vfsmount pair
 * @dentry:  pointer to dentry of the base directory
 * @mnt: pointer to vfs mount of the base directory
 * @name: pointer to file name
 * @flags: lookup flags
 * @path: pointer to struct path to fill
 */
// Linux function proto:
// int vfs_path_lookup(struct dentry *dentry, struct vfsmount *mnt,
// 		    const char *name, unsigned int flags,
// 		    struct path *path)
int vfs_path_lookup(dentry_s * dentry, vfsmount_s * mnt,
		    const char * name, unsigned int flags, path_s * path)
{
	/* the first argument of filename_lookup() is ignored with root */
	return filename_lookup(AT_FDCWD, getname_kernel(name), flags , path);
}

// Linux function proto:
// static inline int user_path_at(int dfd, const char __user *name, unsigned flags,
//		 struct path *path)
int user_path_at(int dfd, const char * name, unsigned flags, path_s * path)
{
	filename_s * fn = getname(name);
	return filename_lookup(dfd, fn, flags, path);
}


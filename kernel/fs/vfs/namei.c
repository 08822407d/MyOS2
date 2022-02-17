#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <sys/fcntl.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/fs/vfs.h>
#include <include/fs/namei.h>
#include <include/fs/dcache.h>

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
unsigned long getname(filename_s * name, const char * u_filename)
{
	// Linux call stack:
	// struct filename * getname_flags(const char __user *filename,
	//					int flags, int *empty)
	//					||
	//					\/
	size_t len = strnlen_user((void *)u_filename, CONST_4K);
	if (len < 0)
	{
		return -EFAULT;
	}
	else if (len > CONST_4K -1)
	{
		return -ENAMETOOLONG;
	}

	name->len = len;
	name->name = kmalloc(len + 1);
	if (name->name == NULL)
	{
		return -ENOMEM;
	}
	memset((void *)name->name, 0, len + 1);
	strncpy_from_user((void *)name->name, (void *)u_filename, len);

	return ENOERR;
}

// Linxu function proto:
// void putname(struct filename *name)
void putname(filename_s * name)
{
	kfree((void *)name->name);
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

	nd->path.dentry = dentry;

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
	dentry_s * parent;

	parent = nd->path.dentry->parent;

	return parent;
}

// Linux function proto:
// static const char *handle_dots(struct nameidata *nd, int type)
static const char * handle_dots(nameidata_s *nd)
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
static void __set_nameidata(nameidata_s * p, int dfd, filename_s * name)
{
	memset(p, 0, sizeof(nameidata_s));

	p->name = name;
	p->dfd = dfd;
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
// struct file *do_filp_open(int dfd, struct filename *pathname,
//			const struct open_flags *op)
file_s * do_filp_open(int dfd, filename_s * name, int flags)
{
	nameidata_s nd;
	file_s * filp;
	int error;

	filp = kmalloc(sizeof(file_s));
	memset(filp, 0, sizeof(file_s));
	__set_nameidata(&nd, dfd, name);

	// Linux call stack:
	// static struct file *path_openat(struct nameidata *nd,
	//		const struct open_flags *op, unsigned flags)
	//					||
	//					\/
	// {
	const char *s = path_init(&nd);
	link_path_walk(s, &nd);
	open_last_lookups(&nd, filp, flags);
	// if (!error)
		error = do_open(&nd, filp, flags);
	terminate_walk(&nd);
	// }

	return filp;
}
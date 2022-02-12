#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/vfs.h>
#include <include/namei.h>
#include <include/dcache.h>


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
static const char * step_into(nameidata_s * nd, dirent_s * dentry)
{
	path_s path;

	nd->path.dentry = dentry;

	return NULL;
}

// Linux function proto:
// static struct dentry *__lookup_slow(const struct qstr *name,
//				    struct dentry *dir, unsigned int flags)
static dirent_s * __lookup_slow(nameidata_s * nd)
{
	dirent_s *	dentry = NULL,
			 *	parent = nd->path.dentry;
	
	// linux call stack :
	// struct dentry *d_alloc_parallel(struct dentry *parent,
	//			const struct qstr *name, wait_queue_head_t *wq)
	//					||
	//					\/
	// struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
	//					||
	//					\/
	dentry = __d_alloc(nd->last_name, nd->last_len);

	if(parent->dir_inode->inode_ops->lookup(parent->dir_inode, dentry) == NULL)
	{
		color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->name);
		kfree(dentry->name);
		kfree(dentry);
		return NULL;
	}

	list_hdr_append(&parent->childdir_lhdr, &dentry->dirent_list);
	dentry->parent = parent;

	return dentry;
}

// Linux function proto:
// static struct dentry *follow_dotdot(struct nameidata *nd,
//				 struct inode **inodep, unsigned *seqp)
static dirent_s * follow_dotdot(nameidata_s * nd)
{
	dirent_s * parent;

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
		dirent_s * parent;

		parent = follow_dotdot(nd);
		ret_val = step_into(nd, parent);
	}
	return ret_val;
}

// Linux function proto:
// static const char *walk_component(struct nameidata *nd, int flags)
static const char * walk_component(nameidata_s * nd)
{
	dirent_s * dentry;
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
	dentry = __d_lookup(nd);

	if (dentry == NULL)
	{
		// linux call stack :
		//	static struct dentry *lookup_slow(const struct qstr *name,
		//			  struct dentry *dir, unsigned int flags)
		//					||
		//					\/
		dentry = __lookup_slow(nd);
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

		nd->last_len = i;
		nd->last_name = name;
		nd->last_type = type;

		name += nd->last_len;
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
static void __set_nameidata(nameidata_s * p, int dfd, const char * name)
{
	memset(p, 0, sizeof(nameidata_s));

	p->last_name = name;
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
	nd->root = curr_tsk->task_fs->root;
	if (nd->last_name[0] == '/')
		nd->path = nd->root;
	else
		nd->path = curr_tsk->task_fs->pwd;
}

// Linux function proto:
// static const char *open_last_lookups(struct nameidata *nd,
//			struct file *file, const struct open_flags *op)
static const char *open_last_lookups(nameidata_s * nd, file_s * file)
{

}

// Linux function proto:
// struct file *do_filp_open(int dfd, struct filename *pathname,
//			const struct open_flags *op)
file_s * do_filp_open(int dfd, const char * pathname)
{
	nameidata_s nd;
	file_s * filp;
	int error;

	__set_nameidata(&nd, dfd, pathname);

	// Linux call stack:
	// static struct file *path_openat(struct nameidata *nd,
	//		const struct open_flags *op, unsigned flags)
	//					||
	//					\/
	// {
	const char *s = path_init(&nd);
	link_path_walk(pathname, &nd);
	// if (!error)
	// 	error = do_open(&nd, filp);
	terminate_walk(&nd);
	// }

	return filp;
}
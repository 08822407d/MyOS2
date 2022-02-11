#include <stddef.h>
#include <errno.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/vfs.h>
#include <include/namei.h>
#include <include/dcache.h>

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
	// int err = handle_mounts(nd, dentry, &path, &inode);

	// /* not a symlink or should not follow */
	// if (!(nd->flags & LOOKUP_RCU)) {
	// 	dput(nd->path.dentry);
	// 	if (nd->path.mnt != path.mnt)
	// 		mntput(nd->path.mnt);
	// }
	// nd->path = path;
	// nd->inode = inode;

	return NULL;
}

// Linux function proto:
// static struct dentry *__lookup_slow(const struct qstr *name,
//				    struct dentry *dir, unsigned int flags)
static dirent_s * __lookup_slow(nameidata_s * nd)
{
	dirent_s *	dentry = NULL,
			 *	parent = nd->path.dentry;
	
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
// static const char *handle_dots(struct nameidata *nd, int type)
static const char * handle_dots(nameidata_s *nd)
{
	if (nd->last_type == LAST_DOTDOT)
	{
		// const char *error = NULL;
		// dirent_s * parent;

		// if (!nd->root.mnt) {
		// 	error = ERR_PTR(set_root(nd));
		// 	if (error)
		// 		return error;
		// }

		// parent = follow_dotdot(nd, &inode, &seq);
		// if (IS_ERR(parent))
		// 	return ERR_CAST(parent);
		// if (unlikely(!parent))
		// 	error = step_into(nd, WALK_NOFOLLOW,
		// 			 nd->path.dentry, nd->inode);
		// else
		// 	error = step_into(nd, WALK_NOFOLLOW,
		// 			 parent, inode);
		// if (unlikely(error))
		// 	return error;
	}
}

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

	//	static struct dentry *lookup_fast(struct nameidata *nd,
	//				  struct inode **inode, unsigned *seqp)
	//					||
	//					\/
	dentry = __d_lookup(nd);

	if (dentry == NULL)
	{
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
	nd->flags |= LOOKUP_PARENT;

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
		while (name[i] != 0 || name[i] != '/')
			i++;

		type = LAST_NORM;
		// detect . and .. dir
		if (name[0] == '.') switch (nd->last_len) {
			case 2:
				if (name[1] == '.') {
					type = LAST_DOTDOT;
				}
				break;
			case 1:
				type = LAST_DOT;
		}

		nd->last_name = name;
		nd->last_type = type;

		name += nd->last_len;
		/*
		 * If it wasn't NUL, we know it was '/'. Skip that
		 * slash, and continue until no more slashes.
		 */
		do {
			name++;
		} while (*name == '/');

		walk_component(nd);
	}
}
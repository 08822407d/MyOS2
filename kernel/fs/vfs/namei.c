#include <stddef.h>
#include <errno.h>

#include <include/vfs.h>
#include <include/namei.h>

typedef struct nameidata {
	path_s		path;
	path_s		root;
	inode_s	*	inode; /* path.dentry.d_inode */

	size_t		last_len;
	char *		last_name;
	int			last_type;
	unsigned	flags, state;
	unsigned	depth;
	int			total_link_count;

	// struct filename	*name;
	// struct nameidata *saved;
	unsigned	root_seq;
	int			dfd;
} nameidata_s;

enum {WALK_TRAILING = 1, WALK_MORE = 2, WALK_NOFOLLOW = 4};

#define ND_ROOT_PRESET 1
#define ND_ROOT_GRABBED 2
#define ND_JUMPED 4


/*
 * Do we need to follow links? We _really_ want to be able
 * to do this check without having to look at inode->i_op,
 * so we keep a cache of "no, this doesn't need follow_link"
 * for the common case.
 */
static const char *step_into(nameidata_s *nd, int flags, dirent_s * dentry, inode_s *inode)
{
	path_s path;
	int err = handle_mounts(nd, dentry, &path, &inode);

	if (err < 0)
		return ERR_PTR(err);

	/* not a symlink or should not follow */
	if (!(nd->flags & LOOKUP_RCU)) {
		dput(nd->path.dentry);
		if (nd->path.mnt != path.mnt)
			mntput(nd->path.mnt);
	}
	nd->path = path;
	nd->inode = inode;

	return NULL;
}

// search nd->last_name through dirent cache
static dirent_s * lookup_fast(nameidata_s * nd, inode_s ** inode)
{
	dirent_s *	dentry = NULL,
			 *	parent = nd->path.dentry;
	
		dirent_s * dir_p;
		List_s * dir_lp;
		for (dir_lp = parent->childdir_lhdr.header.next;
				dir_lp != &parent->childdir_lhdr.header;
				dir_lp = dir_lp->next)
		{
			if ((dir_p = dir_lp->owner_p) != NULL &&
				!strncmp(nd->last_name, dir_p->name, nd->last_len))
			{
				dentry = dir_p;
				break;
			}
		}

	return dentry;
}

// search nd->last_name on disk
static dirent_s * lookup_slow(nameidata_s * nd, inode_s ** inode)
{
	dirent_s *	dentry = NULL,
			 *	parent = nd->path.dentry;

	return dentry;
}

static const char *walk_component(nameidata_s *nd, int flags)
{
	dirent_s * dentry;
	inode_s * inode;
	/*
	 * "." and ".." are special - ".." especially so because it has
	 * to be able to know about the current root directory and
	 * parent relationships.
	 */
	if (nd->last_type == LAST_DOT)
	{
		return NULL;
	}
	else if (nd->last_type == LAST_DOTDOT)
	{
	//						||
	//						\/
	//	static const char *handle_dots(struct nameidata *nd, int type)
		const char *error = NULL;
		dirent_s * parent;

		if (!nd->root.mnt) {
			error = ERR_PTR(set_root(nd));
			if (error)
				return error;
		}

		parent = follow_dotdot(nd, &inode, &seq);
		if (IS_ERR(parent))
			return ERR_CAST(parent);
		if (unlikely(!parent))
			error = step_into(nd, WALK_NOFOLLOW,
					 nd->path.dentry, nd->inode);
		else
			error = step_into(nd, WALK_NOFOLLOW,
					 parent, inode);
		if (unlikely(error))
			return error;
	}

	dentry = lookup_fast(nd, &inode);
	if (dentry == NULL)
		dentry = lookup_slow(nd, &inode);

	return step_into(nd, flags, dentry, inode);
}

/*
 * Name resolution.
 * This is the basic name resolution function, turning a pathname into
 * the final dentry. We expect 'base' to be positive and a directory.
 *
 * Returns 0 and nd will have valid dentry and mnt on success.
 * Returns error and drops reference to input namei data on failure.
 */
static int link_path_walk(const char *name, nameidata_s * nd)
{
	nd->last_type = LAST_ROOT;
	nd->flags |= LOOKUP_PARENT;

	while (*name=='/')
		name++;
	if (!*name) {
		// nd->dir_mode = 0; // short-circuit the 'hardening' idiocy
		return 0;
	}

	/* At this point we know we have a real path component. */
	for(;;) {
		int type;

		type = LAST_NORM;
		// compute length of current dirname
		int i = 0;
		while (name[i] != 0 || name[i] != '/')
			i++;
		// detect . and .. dir
		if (name[0] == '.') switch (nd->last_len) {
			case 2:
				if (name[1] == '.') {
					type = LAST_DOTDOT;
					nd->state |= ND_JUMPED;
				}
				break;
			case 1:
				type = LAST_DOT;
		}

		nd->last_name = name;
		nd->last_type = type;

		name += nd->last_len;
		if (!*name)
			goto OK;
		/*
		 * If it wasn't NUL, we know it was '/'. Skip that
		 * slash, and continue until no more slashes.
		 */
		do {
			name++;
		} while (*name == '/');
OK:
		walk_component(nd, 0);
	}
}
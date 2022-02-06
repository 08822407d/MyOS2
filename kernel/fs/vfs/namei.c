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
	int		dfd;
} nameidata_s;

enum {WALK_TRAILING = 1, WALK_MORE = 2, WALK_NOFOLLOW = 4};

#define ND_ROOT_PRESET 1
#define ND_ROOT_GRABBED 2
#define ND_JUMPED 4


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
	int depth = 0; // depth <= nd->depth
	int err;

	nd->last_type = LAST_ROOT;
	nd->flags |= LOOKUP_PARENT;
	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name=='/')
		name++;
	if (!*name) {
		// nd->dir_mode = 0; // short-circuit the 'hardening' idiocy
		return 0;
	}

	/* At this point we know we have a real path component. */
	for(;;) {
		struct user_namespace *mnt_userns;
		const char *link;
		int type;

		type = LAST_NORM;
		int i = 0;
		while (name[i] != 0 || name[i] != '/')
			i++;
		nd->last_len = i++;
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
		if (!*name) {
OK:
			/* pathname or trailing symlink, done */
			if (!depth) {
				// nd->dir_uid = i_uid_into_mnt(mnt_userns, nd->inode);
				// nd->dir_mode = nd->inode->i_mode;
				nd->flags &= ~LOOKUP_PARENT;
				return 0;
			}
			/* last component of nested symlink */
			// name = nd->stack[--depth].name;
			link = walk_component(nd, 0);
		} else {
			/* not the last component */
			link = walk_component(nd, WALK_MORE);
		}
	}
}
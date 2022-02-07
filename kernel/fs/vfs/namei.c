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
static const char *step_into(struct nameidata *nd, int flags,
		     struct dentry *dentry, struct inode *inode, unsigned seq)
{
	struct path path;
	int err = handle_mounts(nd, dentry, &path, &inode, &seq);

	if (err < 0)
		return ERR_PTR(err);
	if (likely(!d_is_symlink(path.dentry)) ||
	   ((flags & WALK_TRAILING) && !(nd->flags & LOOKUP_FOLLOW)) ||
	   (flags & WALK_NOFOLLOW)) {
		/* not a symlink or should not follow */
		if (!(nd->flags & LOOKUP_RCU)) {
			dput(nd->path.dentry);
			if (nd->path.mnt != path.mnt)
				mntput(nd->path.mnt);
		}
		nd->path = path;
		nd->inode = inode;
		nd->seq = seq;
		return NULL;
	}
	if (nd->flags & LOOKUP_RCU) {
		/* make sure that d_is_symlink above matches inode */
		if (read_seqcount_retry(&path.dentry->d_seq, seq))
			return ERR_PTR(-ECHILD);
	} else {
		if (path.mnt == nd->path.mnt)
			mntget(path.mnt);
	}
	return pick_link(nd, &path, inode, seq, flags);
}

static struct dentry *lookup_fast(struct nameidata *nd,
				  struct inode **inode,
			          unsigned *seqp)
{
	struct dentry *dentry, *parent = nd->path.dentry;
	int status = 1;

	/*
	 * Rename seqlock is not required here because in the off chance
	 * of a false negative due to a concurrent rename, the caller is
	 * going to fall back to non-racy lookup.
	 */
	if (nd->flags & LOOKUP_RCU) {
		unsigned seq;
		dentry = __d_lookup_rcu(parent, &nd->last, &seq);
		if (unlikely(!dentry)) {
			if (!try_to_unlazy(nd))
				return ERR_PTR(-ECHILD);
			return NULL;
		}

		/*
		 * This sequence count validates that the inode matches
		 * the dentry name information from lookup.
		 */
		*inode = d_backing_inode(dentry);
		if (unlikely(read_seqcount_retry(&dentry->d_seq, seq)))
			return ERR_PTR(-ECHILD);

		/*
		 * This sequence count validates that the parent had no
		 * changes while we did the lookup of the dentry above.
		 *
		 * The memory barrier in read_seqcount_begin of child is
		 *  enough, we can use __read_seqcount_retry here.
		 */
		if (unlikely(__read_seqcount_retry(&parent->d_seq, nd->seq)))
			return ERR_PTR(-ECHILD);

		*seqp = seq;
		status = d_revalidate(dentry, nd->flags);
		if (likely(status > 0))
			return dentry;
		if (!try_to_unlazy_next(nd, dentry, seq))
			return ERR_PTR(-ECHILD);
		if (status == -ECHILD)
			/* we'd been told to redo it in non-rcu mode */
			status = d_revalidate(dentry, nd->flags);
	} else {
		dentry = __d_lookup(parent, &nd->last);
		if (unlikely(!dentry))
			return NULL;
		status = d_revalidate(dentry, nd->flags);
	}
	if (unlikely(status <= 0)) {
		if (!status)
			d_invalidate(dentry);
		dput(dentry);
		return ERR_PTR(status);
	}
	return dentry;
}

// only handle dotdot, directly return when it is a single dot
static const char *handle_dots(struct nameidata *nd, int type)
{
	if (type == LAST_DOTDOT) {
		const char *error = NULL;
		struct dentry *parent;
		struct inode *inode;
		unsigned seq;

		if (!nd->root.mnt) {
			error = ERR_PTR(set_root(nd));
			if (error)
				return error;
		}
		if (nd->flags & LOOKUP_RCU)
			parent = follow_dotdot_rcu(nd, &inode, &seq);
		else
			parent = follow_dotdot(nd, &inode, &seq);
		if (IS_ERR(parent))
			return ERR_CAST(parent);
		if (unlikely(!parent))
			error = step_into(nd, WALK_NOFOLLOW,
					 nd->path.dentry, nd->inode, nd->seq);
		else
			error = step_into(nd, WALK_NOFOLLOW,
					 parent, inode, seq);
		if (unlikely(error))
			return error;

		if (unlikely(nd->flags & LOOKUP_IS_SCOPED)) {
			/*
			 * If there was a racing rename or mount along our
			 * path, then we can't be sure that ".." hasn't jumped
			 * above nd->root (and so userspace should retry or use
			 * some fallback).
			 */
			smp_rmb();
			if (unlikely(__read_seqcount_retry(&mount_lock.seqcount, nd->m_seq)))
				return ERR_PTR(-EAGAIN);
			if (unlikely(__read_seqcount_retry(&rename_lock.seqcount, nd->r_seq)))
				return ERR_PTR(-EAGAIN);
		}
	}
	return NULL;
}

static const char *walk_component(struct nameidata *nd, int flags)
{
	struct dentry *dentry;
	struct inode *inode;
	unsigned seq;
	/*
	 * "." and ".." are special - ".." especially so because it has
	 * to be able to know about the current root directory and
	 * parent relationships.
	 */
	if (nd->last_type != LAST_NORM) {
		return handle_dots(nd, nd->last_type);
	}
	dentry = lookup_fast(nd, &inode, &seq);

	return step_into(nd, flags, dentry, inode, seq);
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
			/* last component of nested symlink */
			link = walk_component(nd, 0);
		} else {
			/* not the last component */
			link = walk_component(nd, WALK_MORE);
		}
	}
}
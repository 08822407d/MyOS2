// SPDX-License-Identifier: GPL-2.0-only
/*
 * fs/dcache.c
 *
 * Complete reimplementation
 * (C) 1997 Thomas Schoebel-Theuer,
 * with heavy changes by Linus Torvalds
 */

/*
 * Notes on the allocation strategy:
 *
 * The dcache is a master of the icache - whenever a dcache entry
 * exists, the inode will always exist. "iput()" is done either when
 * the dcache entry is deleted or garbage collected.
 */

// #include <linux/ratelimit.h>
#include <linux/lib/string.h>
#include <linux/mm/mm.h>
#include <linux/fs/fs.h>
// #include <linux/fscrypt.h>
// #include <linux/fsnotify.h>
#include <linux/kernel/slab.h>
#include <linux/init/init.h>
// #include <linux/hash.h>
// #include <linux/cache.h>
// #include <linux/export.h>
// #include <linux/security.h>
// #include <linux/seqlock.h>
#include <linux/mm/memblock.h>
// #include <linux/bit_spinlock.h>
// #include <linux/rculist_bl.h>
// #include <linux/list_lru.h>
#include <linux/fs/internal.h>
#include <linux/fs/mount.h>


/*
 * Usage:
 * dcache->d_inode->i_lock protects:
 *   - i_dentry, d_u.d_alias, d_inode of aliases
 * dcache_hash_bucket lock protects:
 *   - the dcache hash table
 * s_roots bl list spinlock protects:
 *   - the s_roots list (see __d_drop)
 * dentry->d_sb->s_dentry_lru_lock protects:
 *   - the dcache lru lists and counters
 * d_lock protects:
 *   - d_flags
 *   - d_name
 *   - d_lru
 *   - d_count
 *   - d_unhashed()
 *   - d_parent and d_subdirs
 *   - childrens' d_child and d_parent
 *   - d_u.d_alias, d_inode
 *
 * Ordering:
 * dentry->d_inode->i_lock
 *   dentry->d_lock
 *     dentry->d_sb->s_dentry_lru_lock
 *     dcache_hash_bucket lock
 *     s_roots lock
 *
 * If there is an ancestor relationship:
 * dentry->d_parent->...->d_parent->d_lock
 *   ...
 *     dentry->d_parent->d_lock
 *       dentry->d_lock
 *
 * If no ancestor relationship:
 * arbitrary, since it's serialized on rename_lock
 */

/* 
 * This is dput
 *
 * This is complicated by the fact that we do not want to put
 * dentries that are no longer on any hash chain on the unused
 * list: we'd much rather just get rid of them immediately.
 *
 * However, that implies that we have to traverse the dentry
 * tree upwards to the parents which might _also_ now be
 * scheduled for deletion (it may have been only waiting for
 * its last child to go away).
 *
 * This tail recursion is done by hand as we don't want to depend
 * on the compiler to always get this right (gcc generally doesn't).
 * Real recursion would eat up our stack space.
 */

/*
 * dput - release a dentry
 * @dentry: dentry to release 
 *
 * Release a dentry. This will drop the usage count and if appropriate
 * call the dentry unlink method as well as removing it from the queues and
 * releasing its resources. If the parent dentries were scheduled for release
 * they too may now get deleted.
 */
void dput(dentry_s *dentry)
{
	// while (dentry) {
	// 	might_sleep();

	// 	rcu_read_lock();
	// 	if (likely(fast_dput(dentry))) {
	// 		rcu_read_unlock();
	// 		return;
	// 	}

	// 	/* Slow case: now with the dentry lock held */
	// 	rcu_read_unlock();

	// 	if (likely(retain_dentry(dentry))) {
	// 		spin_unlock(&dentry->d_lock);
	// 		return;
	// 	}

	// 	dentry = dentry_kill(dentry);
	// }
}

// Linux function proto:
// struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name)
dentry_s * __d_lookup(const dentry_s * parent, const qstr_s * name)
{
	dentry_s *	dentry = NULL;

	dentry_s * dir_p;
	List_s * dir_lp;
	for (dir_lp = parent->d_subdirs.header.next;
			dir_lp != &parent->d_subdirs.header;
			dir_lp = dir_lp->next)
	{
		if ((dir_p = dir_lp->owner_p) != NULL &&
			!strncmp(name->name, dir_p->d_name.name, name->len))
		{
			dentry = dir_p;
			break;
		}
	}
	return dentry;
}

/**
 * __d_alloc	-	allocate a dcache entry
 * @sb: filesystem it will belong to
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
// Linux function proto:
// static struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
dentry_s * __d_alloc(super_block_s *sb, const qstr_s * name)
{
	dentry_s * dentry = kzalloc(sizeof(dentry_s), GFP_KERNEL);
	if (dentry == NULL)
		return ERR_PTR(-ENOMEM);

	dentry->d_name.name = kzalloc(name->len + 1, GFP_KERNEL);
	if (dentry->d_name.name == NULL)
	{
		kfree(dentry);
		return ERR_PTR(-ENOMEM);
	}

	dentry->d_name.len = name->len;
	memcpy((void *)dentry->d_name.name, name->name, name->len);

	dentry->d_flags = 0;
	dentry->d_inode = NULL;
	dentry->d_parent = dentry;
	dentry->d_sb = sb;
	dentry->d_op = NULL;

	list_init(&dentry->d_child, dentry);
	list_hdr_init(&dentry->d_subdirs);

	// d_set_d_op(dentry, dentry->d_sb->s_d_op);

	return dentry;
}

/**
 * d_alloc	-	allocate a dcache entry
 * @parent: parent of entry to allocate
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
dentry_s *d_alloc(dentry_s * parent, const qstr_s *name)
{
	dentry_s *dentry = __d_alloc(parent->d_sb, name);
	if (dentry == NULL)
		return NULL;
	/*
	 * don't need child lock because it is not subject
	 * to concurrency here
	 */
	dentry->d_parent = parent;
	list_hdr_push(&parent->d_subdirs, &dentry->d_child);

	return dentry;
}

dentry_s *d_alloc_anon(super_block_s *sb)
{
	qstr_s q = {.name = "/", .len = 1};
	return __d_alloc(sb, &q);
}

dentry_s *d_alloc_cursor(dentry_s * parent)
{
	dentry_s *dentry = d_alloc_anon(parent->d_sb);
	if (dentry) {
		dentry->d_flags |= DCACHE_DENTRY_CURSOR;
		dentry->d_parent = dget(parent);
	}
	return dentry;
}

/**
 * d_alloc_pseudo - allocate a dentry (for lookup-less filesystems)
 * @sb: the superblock
 * @name: qstr of the name
 *
 * For a filesystem that just pins its dentries in memory and never
 * performs lookups at all, return an unhashed IS_ROOT dentry.
 * This is used for pipes, sockets et.al. - the stuff that should
 * never be anyone's children or parents.  Unlike all other
 * dentries, these will not have RCU delay between dropping the
 * last reference and freeing them.
 *
 * The only user is alloc_file_pseudo() and that's what should
 * be considered a public interface.  Don't use directly.
 */
dentry_s *d_alloc_pseudo(super_block_s *sb, const qstr_s *name)
{
	dentry_s *dentry = __d_alloc(sb, name);
	if (dentry)
		dentry->d_flags |= DCACHE_NORCU;
	return dentry;
}

dentry_s *d_alloc_name(dentry_s *parent, const char *name)
{
	qstr_s q;

	q.name = name;
	q.len = strlen(name);
	return d_alloc(parent, &q);
}

void d_set_d_op(dentry_s *dentry, const dentry_ops_s *op)
{
	dentry->d_op = op;
	if (op == NULL)
		return;
	if (op->d_hash)
		dentry->d_flags |= DCACHE_OP_HASH;
	if (op->d_compare)
		dentry->d_flags |= DCACHE_OP_COMPARE;
	if (op->d_revalidate)
		dentry->d_flags |= DCACHE_OP_REVALIDATE;
	if (op->d_weak_revalidate)
		dentry->d_flags |= DCACHE_OP_WEAK_REVALIDATE;
	if (op->d_delete)
		dentry->d_flags |= DCACHE_OP_DELETE;
	if (op->d_prune)
		dentry->d_flags |= DCACHE_OP_PRUNE;
	if (op->d_real)
		dentry->d_flags |= DCACHE_OP_REAL;
}

static inline void __d_set_inode_and_type(dentry_s *dentry,
				inode_s *inode, unsigned type_flags)
{
	unsigned flags;

	dentry->d_inode = inode;
	flags = dentry->d_flags;
	flags &= ~(DCACHE_ENTRY_TYPE | DCACHE_FALLTHRU);
	flags |= type_flags;
	dentry->d_flags = flags;
}

static unsigned d_flags_for_inode(inode_s *inode)
{
	unsigned add_flags = DCACHE_REGULAR_TYPE;

	if (inode == NULL)
		return DCACHE_MISS_TYPE;

	if (S_ISDIR(inode->i_mode)) {
		add_flags = DCACHE_DIRECTORY_TYPE;
		if (!(inode->i_opflags & IOP_LOOKUP)) {
			if (inode->i_op->lookup == NULL)
				add_flags = DCACHE_AUTODIR_TYPE;
			else
				inode->i_opflags |= IOP_LOOKUP;
		}
		goto type_determined;
	}

	if (!(inode->i_opflags & IOP_NOFOLLOW)) {
		inode->i_opflags |= IOP_NOFOLLOW;
	}

	if (!S_ISREG(inode->i_mode))
		add_flags = DCACHE_SPECIAL_TYPE;

type_determined:
	if (IS_AUTOMOUNT(inode))
		add_flags |= DCACHE_NEED_AUTOMOUNT;
	return add_flags;
}

static void __d_instantiate(dentry_s *dentry, inode_s *inode)
{
	unsigned add_flags = d_flags_for_inode(inode);
	__d_set_inode_and_type(dentry, inode, add_flags);
}

/**
 * d_instantiate - fill in inode information for a dentry
 * @entry: dentry to complete
 * @inode: inode to attach to this dentry
 *
 * Fill in inode information in the entry.
 *
 * This turns negative dentries into productive full members
 * of society.
 *
 * NOTE! This assumes that the inode count has been incremented
 * (or otherwise set) by the caller to indicate that it is now
 * in use by the dcache.
 */
 
void d_instantiate(dentry_s *entry, inode_s * inode)
{
	if (inode) {
		__d_instantiate(entry, inode);
	}
}

dentry_s *d_make_root(inode_s *root_inode)
{
	dentry_s *res = NULL;

	if (root_inode) {
		res = d_alloc_anon(root_inode->i_sb);
		if (res)
			d_instantiate(res, root_inode);
		else
			iput(root_inode);
	}
	return res;
}



void vfs_caches_init(void)
{
	// dcache_init();
	// inode_init();
	// files_init();
	// files_maxfiles_init();
	mnt_init();
	bdev_cache_init();
	chrdev_init();
}

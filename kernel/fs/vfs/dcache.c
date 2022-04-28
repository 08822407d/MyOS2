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
// #include <linux/string.h>
// #include <linux/mm.h>
#include <linux/fs/fs.h>
// #include <linux/fscrypt.h>
// #include <linux/fsnotify.h>
// #include <linux/slab.h>
// #include <linux/init.h>
// #include <linux/hash.h>
// #include <linux/cache.h>
// #include <linux/export.h>
// #include <linux/security.h>
// #include <linux/seqlock.h>
// #include <linux/memblock.h>
// #include <linux/bit_spinlock.h>
// #include <linux/rculist_bl.h>
// #include <linux/list_lru.h>
#include <linux/fs/internals.h>
#include <linux/fs/mount.h>


#include <linux/kernel/err.h>
#include <string.h>
#include <include/proto.h>

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
dentry_s * __d_alloc(qstr_s * name)
{
	dentry_s * dentry = kmalloc(sizeof(dentry_s));
	if (dentry == NULL)
		return ERR_PTR(-ENOMEM);

	dentry->d_name.name = kmalloc(name->len + 1);
	if (dentry->d_name.name == NULL)
	{
		kfree(dentry);
		return ERR_PTR(-ENOMEM);
	}

	memset((void *)dentry->d_name.name, 0, name->len + 1);
	dentry->d_name.len = name->len;
	memcpy((void *)dentry->d_name.name, name->name, name->len);

	list_init(&dentry->d_child, dentry);
	list_hdr_init(&dentry->d_subdirs);

	return dentry;
}
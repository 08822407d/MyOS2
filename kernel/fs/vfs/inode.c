// SPDX-License-Identifier: GPL-2.0-only
/*
 * (C) 1997 Linus Torvalds
 * (C) 1999 Andrea Arcangeli <andrea@suse.de> (dynamic inode allocation)
 */
// #include <linux/export.h>
#include <linux/fs/fs.h>
// #include <linux/mm.h>
// #include <linux/backing-dev.h>
// #include <linux/hash.h>
// #include <linux/swap.h>
// #include <linux/security.h>
// #include <linux/cdev.h>
// #include <linux/memblock.h>
// #include <linux/fsnotify.h>
#include <linux/kernel/mount.h>
// #include <linux/posix_acl.h>
// #include <linux/prefetch.h>
// #include <linux/buffer_head.h> /* for inode_has_buffers */
// #include <linux/ratelimit.h>
// #include <linux/list_lru.h>
// #include <linux/iversion.h>
// #include <trace/events/writeback.h>
#include <linux/fs/internal.h>


#include <include/proto.h>

/*
 * Inode locking rules:
 *
 * inode->i_lock protects:
 *   inode->i_state, inode->i_hash, __iget()
 * Inode LRU list locks protect:
 *   inode->i_sb->s_inode_lru, inode->i_lru
 * inode->i_sb->s_inode_list_lock protects:
 *   inode->i_sb->s_inodes, inode->i_sb_list
 * bdi->wb.list_lock protects:
 *   bdi->wb.b_{dirty,io,more_io,dirty_time}, inode->i_io_list
 * inode_hash_lock protects:
 *   inode_hashtable, inode->i_hash
 *
 * Lock ordering:
 *
 * inode->i_sb->s_inode_list_lock
 *   inode->i_lock
 *     Inode LRU list locks
 *
 * bdi->wb.list_lock
 *   inode->i_lock
 *
 * inode_hash_lock
 *   inode->i_sb->s_inode_list_lock
 *   inode->i_lock
 *
 * iunique_lock
 *   inode_hash_lock
 */

static inode_s *alloc_inode(super_block_s *sb)
{
	const super_ops_s *ops = sb->s_op;
	inode_s *inode;

	if (ops->alloc_inode != NULL)
		inode = ops->alloc_inode(sb);
	else
		inode = kmalloc(sizeof(inode_s));

	if (inode == NULL)
		return NULL;

	// if (unlikely(inode_init_always(sb, inode))) {
	// 	if (ops->destroy_inode) {
	// 		ops->destroy_inode(inode);
	// 		if (!ops->free_inode)
	// 			return NULL;
	// 	}
	// 	inode->free_inode = ops->free_inode;
	// 	i_callback(&inode->i_rcu);
	// 	return NULL;
	// }

	return inode;
}

/**
 *	new_inode 	- obtain an inode
 *	@sb: superblock
 *
 *	Allocates a new inode for given superblock. The default gfp_mask
 *	for allocations related to inode->i_mapping is GFP_HIGHUSER_MOVABLE.
 *	If HIGHMEM pages are unsuitable or it is known that pages allocated
 *	for the page cache are not reclaimable or migratable,
 *	mapping_set_gfp_mask() must be called with suitable flags on the
 *	newly created inode's mapping
 *
 */
inode_s *new_inode(super_block_s *sb)
{
	inode_s *inode = alloc_inode(sb);

	// struct inode *new_inode_pseudo(struct super_block *sb)
	// {
		if (inode != NULL)
			inode->i_sb = sb;
	// }
	return inode;
}

/**
 *	iput	- put an inode
 *	@inode: inode to put
 *
 *	Puts an inode, dropping its usage count. If the inode use count hits
 *	zero, the inode is then freed and may also be destroyed.
 *
 *	Consequently, iput() can sleep.
 */
void iput(inode_s *inode)
{
	if (inode == NULL)
		return;

	// input_final()
	// {
		kfree(inode);
	// }
}
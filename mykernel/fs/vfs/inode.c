// SPDX-License-Identifier: GPL-2.0-only
/*
 * (C) 1997 Linus Torvalds
 * (C) 1999 Andrea Arcangeli <andrea@suse.de> (dynamic inode allocation)
 */
#include <linux/kernel/export.h>
#include <linux/fs/fs.h>
#include <linux/kernel/mm.h>
// #include <linux/backing-dev.h>
// #include <linux/hash.h>
// #include <linux/swap.h>
// #include <linux/security.h>
#include <linux/device/cdev.h>
// #include <linux/mm/memblock.h>
// #include <linux/fsnotify.h>
#include <linux/kernel/mount.h>
// #include <linux/posix_acl.h>
#include <linux/kernel/prefetch.h>
// #include <linux/buffer_head.h> /* for inode_has_buffers */
// #include <linux/ratelimit.h>
// #include <linux/list_lru.h>
// #include <linux/iversion.h>
// #include <trace/events/writeback.h>
#include <linux/fs/internal.h>


#include <linux/kernel/mm_api.h>
#include <obsolete/printk.h>


/*
 * Empty aops. Can be used for the cases where the user does not
 * define any of the address_space operations.
 */
const addr_spc_ops_s empty_aops = {
};
EXPORT_SYMBOL(empty_aops);

static kmem_cache_s *inode_cachep __read_mostly;

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

static int no_open(inode_s *inode, file_s *file)
{
	return -ENXIO;
}

/**
 * inode_init_always - perform inode structure initialisation
 * @sb: superblock inode belongs to
 * @inode: inode to initialise
 *
 * These are initializations that need to be done on every inode
 * allocation as the fields are not initialised by slab allocation.
 */
int inode_init_always(super_block_s *sb, inode_s *inode)
{
	static const inode_ops_s empty_iops;
	static const file_ops_s no_open_fops = {.open = no_open};
	addr_spc_s *const mapping = &inode->i_data;

	inode->i_sb = sb;
	// inode->i_blkbits = sb->s_blocksize_bits;
	inode->i_flags = 0;
	// atomic64_set(&inode->i_sequence, 0);
	// atomic_set(&inode->i_count, 1);
	inode->i_op = &empty_iops;
	inode->i_fop = &no_open_fops;
	inode->i_ino = 0;
	// inode->__i_nlink = 1;
	inode->i_opflags = 0;
	i_uid_write(inode, 0);
	i_gid_write(inode, 0);
	// atomic_set(&inode->i_writecount, 0);
	inode->i_size = 0;
	// inode->i_write_hint = WRITE_LIFE_NOT_SET;
	inode->i_blocks = 0;
	inode->i_bytes = 0;
	// inode->i_generation = 0;
	// inode->i_pipe = NULL;
	inode->i_cdev = NULL;
	inode->i_link = NULL;
	inode->i_dir_seq = 0;
	inode->i_rdev = 0;
	// inode->dirtied_when = 0;


	// spin_lock_init(&inode->i_lock);
	// lockdep_set_class(&inode->i_lock, &sb->s_type->i_lock_key);

	// init_rwsem(&inode->i_rwsem);
	// lockdep_set_class(&inode->i_rwsem, &sb->s_type->i_mutex_key);

	// atomic_set(&inode->i_dio_count, 0);

	mapping->a_ops = &empty_aops;
	mapping->host = inode;
	mapping->flags = 0;
	// mapping->wb_err = 0;
	atomic_set(&mapping->i_mmap_writable, 0);
// #ifdef CONFIG_READ_ONLY_THP_FOR_FS
// 	atomic_set(&mapping->nr_thps, 0);
// #endif
	// mapping_set_gfp_mask(mapping, GFP_HIGHUSER_MOVABLE);
	mapping->private_data = NULL;
	mapping->writeback_index = 0;
	inode->i_private = NULL;
	inode->i_mapping = mapping;
	// INIT_HLIST_HEAD(&inode->i_dentry);	/* buggered by rcu freeing */

	return 0;
}

static inode_s *alloc_inode(super_block_s *sb)
{
	const super_ops_s *ops = sb->s_op;
	inode_s *inode;

	if (ops->alloc_inode != NULL)
		inode = ops->alloc_inode(sb);
	else
		inode = kmem_cache_alloc(inode_cachep, GFP_KERNEL);

	if (inode == NULL)
		return NULL;

	if (unlikely(inode_init_always(sb, inode))) {
		if (ops->destroy_inode) {
			ops->destroy_inode(inode);
			if (!ops->free_inode)
				return NULL;
		}
		if (ops->free_inode)
			ops->free_inode(inode);
		else
			kmem_cache_free(inode_cachep, inode);
		return NULL;
	}

	return inode;
}


static void __address_space_init_once(addr_spc_s *mapping)
{
	// xa_init_flags(&mapping->i_pages, XA_FLAGS_LOCK_IRQ | XA_FLAGS_ACCOUNT);
	init_rwsem(&mapping->i_mmap_rwsem);
	// INIT_LIST_HEAD(&mapping->i_private_list);
	// spin_lock_init(&mapping->i_private_lock);
	// mapping->i_mmap = RB_ROOT_CACHED;
}

void address_space_init_once(addr_spc_s *mapping)
{
	memset(mapping, 0, sizeof(*mapping));
	__address_space_init_once(mapping);
}
EXPORT_SYMBOL(address_space_init_once);

/*
 * These are initializations that only need to be done
 * once, because the fields are idempotent across use
 * of the inode, so let the slab aware of that.
 */
void inode_init_once(inode_s *inode)
{
	memset(inode, 0, sizeof(inode_s));

	// INIT_HLIST_NODE(&inode->i_hash);
	INIT_LIST_HEADER_S(&inode->i_devices);
	// INIT_LIST_HEAD(&inode->i_io_list);
	// INIT_LIST_HEAD(&inode->i_wb_list);
	// INIT_LIST_HEAD(&inode->i_lru);
	__address_space_init_once(&inode->i_data);
	// i_size_ordered_init(inode);
}


/**
 * inode_sb_list_add - add inode to the superblock list of inodes
 * @inode: inode to add
 */
void inode_sb_list_add(inode_s *inode)
{
	// spin_lock(&inode->i_sb->s_inode_list_lock);
	// list_add_to_next(&inode->i_sb_list, &inode->i_sb->s_inodes);
	list_header_add_to_tail(&inode->i_sb->s_inodes, &inode->i_sb_list);
	// spin_unlock(&inode->i_sb->s_inode_list_lock);
}

static inline void inode_sb_list_del(inode_s *inode)
{
	if (!list_is_empty_entry(&inode->i_sb_list)) {
	// 	spin_lock(&inode->i_sb->s_inode_list_lock);
	// 	list_del_init(&inode->i_sb_list);
	list_header_delete_node(&inode->i_sb->s_inodes, &inode->i_sb_list);
	// 	spin_unlock(&inode->i_sb->s_inode_list_lock);
	}
}

/*
 * find_inode_fast is the fast path version of find_inode, see the comment at
 * iget_locked for details.
 */
static inode_s *find_inode_fast(super_block_s *sb, unsigned long ino)
{
	inode_s *inode = NULL;
	List_s *lp = NULL;
repeat:
	// hlist_for_each_entry(inode, head, i_hash) {
	list_header_for_each_container(inode, &sb->s_inodes, i_sb_list) {
		if (inode->i_ino != ino)
			continue;
		if (inode->i_sb != sb)
			continue;
			
		// spin_lock(&inode->i_lock);
		// if (inode->i_state & (I_FREEING|I_WILL_FREE)) {
		// 	__wait_on_freeing_inode(inode);
		// 	goto repeat;
		// }
		// if (unlikely(inode->i_state & I_CREATING)) {
		// 	spin_unlock(&inode->i_lock);
		// 	return ERR_PTR(-ESTALE);
		// }
		// __iget(inode);
		// spin_unlock(&inode->i_lock);
		return inode;
	}
	return NULL;
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
	// inode_s *new_inode_pseudo(super_block_s *sb)
	// {
	inode_s *inode = alloc_inode(sb);
	if (inode != NULL)
	{
		inode->i_sb = sb;
		INIT_LIST_S(&inode->i_sb_list);
	}
	// }

	if (inode != NULL)
		inode_sb_list_add(inode);

	return inode;
}


/**
 * ilookup - search for an inode in the inode cache
 * @sb:		super block of file system to search
 * @ino:	inode number to search for
 *
 * Search for the inode @ino in the inode cache, and if the inode is in the
 * cache, the inode is returned with an incremented reference count.
 */
inode_s *ilookup(super_block_s *sb, unsigned long ino)
{
	// HList_hdr_s *head = inode_hashtable + hash(sb, ino);
	inode_s *inode;
again:
	// spin_lock(&inode_hash_lock);
	inode = find_inode_fast(sb, ino);
	// spin_unlock(&inode_hash_lock);

	// if (inode) {
	// 	if (IS_ERR(inode))
	// 		return NULL;
	// 	wait_on_inode(inode);
	// 	if (unlikely(inode_unhashed(inode))) {
	// 		iput(inode);
	// 		goto again;
	// 	}
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

	// BUG_ON(inode->i_state & I_CLEAR);
// retry:
	// if (atomic_dec_and_lock(&inode->i_count, &inode->i_lock)) {
	// 	if (inode->i_nlink && (inode->i_state & I_DIRTY_TIME)) {
	// 		atomic_inc(&inode->i_count);
	// 		spin_unlock(&inode->i_lock);
	// 		trace_writeback_lazytime_iput(inode);
	// 		mark_inode_dirty_sync(inode);
	// 		goto retry;
	// 	}
	// 	iput_final(inode);
	// }
}



void __init inode_init(void)
{
	/* inode slab cache */
	inode_cachep = kmem_cache_create("inode_cache",
					sizeof(inode_s), 0,
					(SLAB_RECLAIM_ACCOUNT | SLAB_PANIC |
						SLAB_MEM_SPREAD | SLAB_ACCOUNT),
					NULL);

	// /* Hash may have been set up in inode_init_early */
	// if (!hashdist)
	// 	return;

	// inode_hashtable =
	// 	alloc_large_system_hash("Inode-cache",
	// 				sizeof(HList_hdr_s),
	// 				ihash_entries,
	// 				14,
	// 				HASH_ZERO,
	// 				&i_hash_shift,
	// 				&i_hash_mask,
	// 				0,
	// 				0);
}

void init_special_inode(inode_s *inode, umode_t mode, dev_t rdev)
{
	inode->i_mode = mode;
	if (S_ISCHR(mode)) {
		inode->i_fop = &def_chr_fops;
		inode->i_rdev = rdev;
	} else if (S_ISBLK(mode)) {
		inode->i_fop = &def_blk_fops;
		inode->i_rdev = rdev;
	}
	// } else if (S_ISFIFO(mode))
	// 	inode->i_fop = &pipefifo_fops;
	else if (S_ISSOCK(mode))
		;	/* leave it no_open_fops */
	else
		color_printk(RED, BLACK, "init_special_inode: bogus i_mode (%o)"
						"for inode\n", mode);
}
#include <include/proto.h>
#include <linux/fs/fs.h>

static inode_s *alloc_inode(super_block_s *sb)
{
	const super_ops_s *ops = sb->s_op;
	inode_s *inode;

	if (ops->alloc_inode != NULL)
		inode = ops->alloc_inode(sb);
	else
		inode = kmalloc(sizeof(inode_s));

	if (!inode)
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
	if (!inode)
		return;

	// input_final()
	// {
		kfree(inode);
	// }
}
#include <linux/fs/vfs.h>

#include <linux/kernel/buffer_head.h>
#include <linux/kernel/blk_types.h>

#include <include/proto.h>

/*
 * __getblk_gfp() will locate (and, if necessary, create) the buffer_head
 * which corresponds to the passed block_device, block and size. The
 * returned buffer has its reference count incremented.
 *
 * __getblk_gfp() will lock up the machine if grow_dev_page's
 * try_to_free_buffers() attempt is failing.  FIXME, perhaps?
 */
buffer_head_s *
__getblk_gfp(block_device_s *bdev, sector_t block, unsigned size)
{
	// // struct buffer_head *bh = __find_get_block(bdev, block, size);
	// if (bh == NULL)
	// 	bh = __getblk_slow(bdev, block, size);
	
	buffer_head_s *bh = kmalloc(sizeof(buffer_head_s));

	return bh;
}

/*
 * Decrement a buffer_head's reference count.  If all buffers against a page
 * have zero reference count, are clean and unlocked, and if the page is clean
 * and unlocked then try_to_free_buffers() may strip the buffers from the page
 * in preparation for freeing it (sometimes, rarely, buffers are removed from
 * a page but it ends up not being freed, and buffers may later be reattached).
 */
void __brelse(buffer_head_s * buf)
{
	// if (atomic_read(&buf->b_count)) {
	// 	put_bh(buf);
	// 	return;
	// }

	if (buf != NULL)
	{
		if (buf->b_data != NULL)
			kfree(buf->b_data);

		kfree(buf);
	}
}

/**
 *  __bread_gfp() - reads a specified block and returns the bh
 *  @bdev: the block_device to read from
 *  @block: number of block
 *  @size: size (in bytes) to read
 *  @gfp: page allocation flag
 *
 *  Reads a specified block, and returns buffer head that contains it.
 *  The page cache can be allocated from non-movable area
 *  not to prevent page migration if you set gfp to zero.
 *  It returns NULL if the block was unreadable.
 */
buffer_head_s *
__bread_gfp(block_device_s *bdev, sector_t block, unsigned size)
{
	buffer_head_s *bh = __getblk_gfp(bdev, block, size);

	// if (likely(bh) && !buffer_uptodate(bh))
	// 	bh = __bread_slow(bh);
	return bh;
}
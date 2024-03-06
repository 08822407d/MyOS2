// SPDX-License-Identifier: GPL-2.0
/*
 * Slab allocator functions that are independent of the allocator strategy
 *
 * (C) 2012 Christoph Lameter <cl@linux.com>
 */
#include <linux/kernel/mm.h>


/*
 * To avoid unnecessary overhead, we pass through large allocation requests
 * directly to the page allocator. We use __GFP_COMP, because we will need to
 * know the allocation order to free the pages properly in kfree.
 */
void *kmalloc_order(size_t size, gfp_t flags, unsigned int order)
{
	void *ret = NULL;
	page_s *page;

	// if (unlikely(flags & GFP_SLAB_BUG_MASK))
	// 	flags = kmalloc_fix_flags(flags);

	flags |= __GFP_COMP;
	page = alloc_pages(flags, order);
	if (likely(page)) {
	// 	ret = page_address(page);
		ret = (void *)page_to_virt(page);
	// 	mod_lruvec_page_state(page, NR_SLAB_UNRECLAIMABLE_B,
	// 			      PAGE_SIZE << order);
	}
	// ret = kasan_kmalloc_large(ret, size, flags);
	// /* As ret might get tagged, call kmemleak hook after KASAN. */
	// kmemleak_alloc(ret, size, 1, flags);
	return ret;
}

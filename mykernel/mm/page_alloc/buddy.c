#define BUDDY_DEFINATION
#include "buddy.h"
#include "page-flags.h"

// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/mm/page_alloc.c
 *
 *  Manages the free list, the system allocates free pages here.
 *  Note that kmalloc() lives in slab.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *  Swap reorganised 29.12.95, Stephen Tweedie
 *  Support of BIGMEM added by Gerhard Wichert, Siemens AG, July 1999
 *  Reshaped it to be a zoned allocator, Ingo Molnar, Red Hat, 1999
 *  Discontiguous memory support, Kanoj Sarcar, SGI, Nov 1999
 *  Zone balancing, Kanoj Sarcar, SGI, Jan 2000
 *  Per cpu hot/cold page lists, bulk allocation, Martin J. Bligh, Sept 2002
 *          (lots of bits borrowed from Ingo Molnar & Andrew Morton)
 */
gfp_t gfp_allowed_mask __read_mostly = GFP_BOOT_MASK;

/*
 * results with 256, 32 in the lowmem_reserve sysctl:
 *	1G machine -> (16M dma, 800M-16M normal, 1G-800M high)
 *	1G machine -> (16M dma, 784M normal, 224M high)
 *	NORMAL allocation will leave 784M/256 of ram reserved in the ZONE_DMA
 *	HIGHMEM allocation will leave 224M/32 of ram reserved in ZONE_NORMAL
 *	HIGHMEM allocation will leave (224M+784M)/256 of ram reserved in ZONE_DMA
 *
 * TBD: should special case ZONE_DMA32 machines here - in those we normally
 * don't need any ZONE_NORMAL reservation
 */
int sysctl_lowmem_reserve_ratio[MAX_NR_ZONES] = {
	[ZONE_DMA] = 256,
	[ZONE_DMA32] = 256,
	[ZONE_NORMAL] = 32,
	[ZONE_MOVABLE] = 0,
};

char * const zone_names[MAX_NR_ZONES] = {
	"DMA",
	"DMA32",
	"Normal",
	"Movable",
	"Device",
};

uint nr_node_ids __read_mostly = MAX_NUMNODES;
uint nr_online_nodes __read_mostly = 1;
EXPORT_SYMBOL(nr_node_ids);
EXPORT_SYMBOL(nr_online_nodes);

/*==============================================================================================*
 *								private fuctions for buddy system								*
 *==============================================================================================*/
void prep_compound_page(page_s *page, uint order)
{
	int i;
	int nr_pages = 1 << order;

	__SetPageHead(page);
	for (i = 1; i < nr_pages; i++)
		prep_compound_tail(page, i);

	prep_compound_head(page, order);
}

static inline void
set_buddy_order(page_s *page, uint order) {
	page->private = (ulong)order;
	__SetPageBuddy(page);
}

/* Used for pages not on another list */
static inline void
add_to_free_list(page_s *page, zone_s *zone, uint order) {
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_header_add_to_head(fa_lhdr, &page->buddy_list);
}

/* Used for pages not on another list */
static inline void
add_to_free_list_tail(page_s *page, zone_s *zone, uint order) {
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_header_add_to_tail(fa_lhdr, &page->buddy_list);
}

static inline void
del_page_from_free_list(page_s *page, List_hdr_s *area) {
	list_header_delete_node(area, &page->buddy_list);
	__ClearPageBuddy(page);
	page->private = 0;
}

static inline page_s
*__list_null_or_page(List_s * pg_lp) {
	if (pg_lp == NULL) return NULL;
	else return container_of(pg_lp, page_s, buddy_list);
}
static inline page_s
*get_page_from_free_area(List_hdr_s *area) {
	List_s *pg_lp = list_header_remove_head(area);
	return __list_null_or_page(pg_lp);
}


/*
 * The order of subdivision here is critical for the IO subsystem.
 * Please do not alter this order without good reasons and regression
 * testing. Specifically, as large blocks of memory are subdivided,
 * the order in which smaller blocks are delivered depends on the order
 * they're subdivided in this function. This is the primary factor
 * influencing the order in which pages are delivered to the IO
 * subsystem according to empirical testing, and this is also justified
 * by considering the behavior of a buddy system containing a single
 * large block of memory acted on by a series of small allocations.
 * This behavior is a critical factor in sglist merging's success.
 *
 * -- nyc
 */
static inline void
expand(zone_s *zone, page_s *page, int low, int high) {
	ulong size = 1 << high;
	while (high > low) {
		high--;
		size >>= 1;
		page_s *p = &page[size];
		INIT_LIST_S(&p->buddy_list);

		add_to_free_list(p, zone, high);
		set_buddy_order(p, high);
	}
}

inline void
post_alloc_hook(page_s *page, uint order, gfp_t gfp_flags) {
	// bool init = !want_init_on_free() && want_init_on_alloc(gfp_flags) &&
	// 		!should_skip_init(gfp_flags);
	// bool zero_tags = init && (gfp_flags & __GFP_ZEROTAGS);
	// int i;

	// set_page_private(page, 0);
	// set_page_refcounted(page);

	// arch_alloc_page(page, order);
	// debug_pagealloc_map_pages(page, 1 << order);

	// /*
	//  * Page unpoisoning must happen before memory initialization.
	//  * Otherwise, the poison pattern will be overwritten for __GFP_ZERO
	//  * allocations and the page unpoisoning code will complain.
	//  */
	// kernel_unpoison_pages(page, 1 << order);

	// /*
	//  * As memory initialization might be integrated into KASAN,
	//  * KASAN unpoisoning and memory initializion code must be
	//  * kept together to avoid discrepancies in behavior.
	//  */

	// /*
	//  * If memory tags should be zeroed
	//  * (which happens only when memory should be initialized as well).
	//  */
	// if (zero_tags) {
	// 	/* Initialize both memory and memory tags. */
	// 	for (i = 0; i != 1 << order; ++i)
	// 		tag_clear_highpage(page + i);

	// 	/* Take note that memory was initialized by the loop above. */
	// 	init = false;
	// }
	// if (!should_skip_kasan_unpoison(gfp_flags) &&
	//     kasan_unpoison_pages(page, order, init)) {
	// 	/* Take note that memory was initialized by KASAN. */
	// 	if (kasan_has_integrated_init())
	// 		init = false;
	// } else {
	// 	/*
	// 	 * If memory tags have not been set by KASAN, reset the page
	// 	 * tags to ensure page_address() dereferencing does not fault.
	// 	 */
	// 	for (i = 0; i != 1 << order; ++i)
	// 		page_kasan_tag_reset(page + i);
	// }
	/* If memory is still not initialized, initialize it now. */
	// if (init)
	// kernel_init_pages(page, 1 << order);
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);

	// set_page_owner(page, order, gfp_flags);
	// page_table_check_alloc(page, order);
}

static void
prep_new_page(page_s *page, uint order, gfp_t gfp_flags) {
	post_alloc_hook(page, order, gfp_flags);

	if (order && (gfp_flags & __GFP_COMP))
		prep_compound_page(page, order);

	// /*
	//  * page is set pfmemalloc when ALLOC_NO_WATERMARKS was necessary to
	//  * allocate the page. The expectation is that the caller is taking
	//  * steps that will free more memory. The caller should avoid the page
	//  * being used for !PFMEMALLOC purposes.
	//  */
	// if (alloc_flags & ALLOC_NO_WATERMARKS)
	// 	set_page_pfmemalloc(page);
	// else
	// 	clear_page_pfmemalloc(page);
}

/*
 * Go through the free lists for the given migratetype and remove
 * the smallest available page from the freelists
 */
// Linux function proto :
// static __always_inline page_s *__rmqueue_smallest(struct zone *zone,
//					unsigned int order, int migratetype)
static inline page_s
*__rmqueue_smallest(zone_s *zone, uint order) {
	uint current_order;
	page_s *page;

	/* Find a page of the appropriate size in the preferred list */
	for (current_order = order; current_order < NR_PAGE_ORDERS; ++current_order) {
		List_hdr_s *area = &zone->free_area[current_order];
		page = get_page_from_free_area(area);
		if (page == NULL)
			continue;

		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

		return page;
	}

	return NULL;
}

/*
 * Freeing function for a buddy system allocator.
 *
 * The concept of a buddy system is to maintain direct-mapped table
 * (containing bit values) for memory blocks of various "orders".
 * The bottom level table contains the map for the smallest allocatable
 * units of memory (here, pages), and each level above it describes
 * pairs of units from the levels below, hence, "buddies".
 * At a high level, all that happens here is marking the table entry
 * at the bottom level available, and propagating the changes upward
 * as necessary, plus some accounting needed to play nicely with other
 * parts of the VM system.
 * At each level, we keep a list of pages, which are heads of continuous
 * free pages of length of (1 << order) and marked with PageBuddy.
 * Page's order is recorded in page_private(page) field.
 * So when we are allocating or freeing one, we can derive the state of the
 * other.  That is, if we allocate a small block, and both were
 * free, the remainder of the region must be split into blocks.
 * If a block is freed, and its buddy is also free, then this
 * triggers coalescing into a block of larger size.
 *
 * -- nyc
 */
// Linux function proto :
// static inline void __free_one_page(page_s *page, unsigned long pfn,
//					struct zone *zone, unsigned int order, int migratetype,
//					fpi_t fpi_flags)
static inline void
__myos_free_one_page(page_s *page, ulong pfn, zone_s *zone, uint order) {
	ulong buddy_pfn;
	ulong combined_pfn;
	page_s *buddy;

	while (order < MAX_ORDER) {
		buddy = find_buddy_page_pfn(page, pfn, order, &buddy_pfn);
		if (buddy == NULL)
			break;

		combined_pfn = buddy_pfn & pfn;
		page = page + (combined_pfn - pfn);
		pfn = combined_pfn;
		order++;
	}

	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
}

static __always_inline bool
free_pages_prepare(page_s *page, uint order)
{
	// int bad = 0;
	// bool skip_kasan_poison = should_skip_kasan_poison(page, fpi_flags);
	// bool init = want_init_on_free();

	// VM_BUG_ON_PAGE(PageTail(page), page);

	// trace_mm_page_free(page, order);
	// kmsan_free_page(page, order);

	// if (unlikely(PageHWPoison(page)) && !order) {
	// 	/*
	// 	 * Do not let hwpoison pages hit pcplists/buddy
	// 	 * Untie memcg state and reset page's owner
	// 	 */
	// 	if (memcg_kmem_online() && PageMemcgKmem(page))
	// 		__memcg_kmem_uncharge_page(page, order);
	// 	reset_page_owner(page, order);
	// 	page_table_check_free(page, order);
	// 	return false;
	// }

	// /*
	//  * Check tail pages before head page information is cleared to
	//  * avoid checking PageCompound for order-0 pages.
	//  */
	// if (unlikely(order)) {
	// 	bool compound = PageCompound(page);

	// 	// VM_BUG_ON_PAGE(compound && compound_order(page) != order, page);

	// 	if (compound)
	// 		page[1].flags &= ~PAGE_FLAGS_SECOND;
	// 	for (int i = 1; i < (1 << order); i++) {
	// 		if (compound)
	// 			bad += free_tail_page_prepare(page, page + i);
	// 		if (is_check_pages_enabled()) {
	// 			if (free_page_is_bad(page + i)) {
	// 				bad++;
	// 				continue;
	// 			}
	// 		}
	// 		(page + i)->flags &= ~PAGE_FLAGS_CHECK_AT_PREP;
	// 	}
	// }
	// if (PageMappingFlags(page))
	// 	page->mapping = NULL;
	// if (memcg_kmem_online() && PageMemcgKmem(page))
	// 	__memcg_kmem_uncharge_page(page, order);
	// if (is_check_pages_enabled()) {
	// 	if (free_page_is_bad(page))
	// 		bad++;
	// 	if (bad)
	// 		return false;
	// }

	// page_cpupid_reset_last(page);
	page->flags &= ~PAGE_FLAGS_CHECK_AT_PREP;
	// reset_page_owner(page, order);
	// page_table_check_free(page, order);

	// if (!PageHighMem(page)) {
	// 	debug_check_no_locks_freed(page_address(page),
	// 				   PAGE_SIZE << order);
	// 	debug_check_no_obj_freed(page_address(page),
	// 				   PAGE_SIZE << order);
	// }

	// kernel_poison_pages(page, 1 << order);

	// /*
	//  * As memory initialization might be integrated into KASAN,
	//  * KASAN poisoning and memory initialization code must be
	//  * kept together to avoid discrepancies in behavior.
	//  *
	//  * With hardware tag-based KASAN, memory tags must be set before the
	//  * page becomes unavailable via debug_pagealloc or arch_free_page.
	//  */
	// if (!skip_kasan_poison) {
	// 	kasan_poison_pages(page, order, init);

	// 	/* Memory is already initialized if KASAN did it internally. */
	// 	if (kasan_has_integrated_init())
	// 		init = false;
	// }
	// if (init)
	// 	kernel_init_pages(page, 1 << order);

	// /*
	//  * arch_free_page() can make the page's contents inaccessible.  s390
	//  * does this.  So nothing which can access the page's contents should
	//  * happen after this.
	//  */
	// arch_free_page(page, order);

	// debug_pagealloc_unmap_pages(page, 1 << order);

	// return true;
}

static void
__free_pages_ok(page_s *page, uint order) {
	ulong flags;
	int migratetype;
	ulong pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	free_pages_prepare(page, order);
	// if (!free_pages_prepare(page, order, fpi_flags))
	// 	return;

	// migratetype = get_pfnblock_migratetype(page, pfn);

	spin_lock_irqsave(&zone->lock, flags);
	// if (unlikely(has_isolate_pageblock(zone) ||
	// 	is_migrate_isolate(migratetype))) {
	// 	migratetype = get_pfnblock_migratetype(page, pfn);
	// }
	__myos_free_one_page(page, pfn, zone, order);
	spin_unlock_irqrestore(&zone->lock, flags);

	// __count_vm_events(PGFREE, 1 << order);
}


/*==============================================================================================*
 *									public fuctions for buddy system							*
 *==============================================================================================*/
/**
 * alloc_pages - Allocate pages.
 * @gfp: GFP flags.
 * @order: Power of two of number of pages to allocate.
 *
 * Allocate 1 << @order contiguous pages.  The physical address of the
 * first page is naturally aligned (eg an order-3 allocation will be aligned
 * to a multiple of 8 * PAGE_SIZE bytes).  The NUMA policy of the current
 * process is honoured when in process context.
 *
 * Context: Can be called from any context, providing the appropriate GFP
 * flags are used.
 * Return: The page on success or NULL if allocation fails.
 */
/*
 * This is the 'heart' of the zoned buddy allocator.
 */
// page_s *__alloc_pages(gfp_t gfp, unsigned int order,
// 		int preferred_nid, nodemask_t *nodemask)
page_s *__myos_alloc_pages(gfp_t gfp, uint order)
{
	/*
	 * There are several places where we assume that the order value is sane
	 * so bail out early if the request is out of bound.
	 */
	if (unlikely(order > MAX_ORDER)) {
		return NULL;
	}

	// static page_s *
	// get_page_from_freelist(gfp_t gfp_mask, unsigned int order,
	// 		int alloc_flags, const struct alloc_context *ac)
	// {
		int i;
		int prefered_zone_list[] = {ZONE_NORMAL, ZONE_DMA32, ZONE_DMA};
		int max_prefered_idx = NODE_DATA(0)->nr_zones;
		int start_prefered_idx = 0;

		if (gfp & GFP_DMA)
			start_prefered_idx = 2;
		else if (gfp & GFP_DMA32)
			start_prefered_idx = 1;
		else
			start_prefered_idx = 0;

		for (i = start_prefered_idx; i < max_prefered_idx; i++) {
			zone_s *zone = &(NODE_DATA(0)->node_zones[prefered_zone_list[i]]);
		// static inline page_s
		// *rmqueue(struct zone *preferred_zone, struct zone *zone, unsigned int order,
		// 		gfp_t gfp_flags, unsigned int alloc_flags, int migratetype)
		// {
			page_s *page = __rmqueue_smallest(zone, order);
			if (likely(page)) {
				prep_new_page(page, order, gfp);
				return page;
			}
			else
				continue;
		// }
		}
	// }
	return NULL;
}

/**
 * __free_pages - Free pages allocated with alloc_pages().
 * @page: The page pointer returned from alloc_pages().
 * @order: The order of the allocation.
 *
 * This function can free multi-page allocations that are not compound
 * pages.  It does not check that the @order passed in matches that of
 * the allocation, so it is easy to leak memory.  Freeing more memory
 * than was allocated will probably emit a warning.
 *
 * If the last reference to this page is speculative, it will be released
 * by put_page() which only frees the first page of a non-compound
 * allocation.  To prevent the remaining pages from being leaked, we free
 * the subsequent pages here.  If you want to use the page's reference
 * count to decide when to free the allocation, you should allocate a
 * compound page, and use put_page() instead of __free_pages().
 *
 * Context: May be called in interrupt context or while holding a normal
 * spinlock, but not in NMI context or while holding a raw spinlock.
 */
// Linux function proto :
// void __free_pages(page_s *page, unsigned int order)
void __free_pages(page_s *page, uint order) {
	/* get PageHead before we drop reference */
	// int head = PageHead(page);
	// if (!head)
	// 	while (order-- > 0)
	// 		free_the_page(page + (1 << order), order);

	__free_pages_ok(page, order);
}

void free_pages(ulong addr, uint order) {
	if (addr != 0)
		__free_pages(virt_to_page((void *)addr), order);
}

/*
 * Common helper functions. Never use with __GFP_HIGHMEM because the returned
 * address cannot represent highmem pages. Use alloc_pages and then kmap if
 * you need to access high mem.
 */
ulong __get_free_pages(gfp_t gfp_mask, uint order)
{
	page_s *page = alloc_pages(gfp_mask & ~__GFP_HIGHMEM, order);
	if (page == NULL)
		return 0;
	return (ulong)page_to_virt(page);
}

ulong get_zeroed_page(gfp_t gfp_mask) {
	return __get_free_pages(gfp_mask | __GFP_ZERO, 0);
}


/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
void __init
memblock_free_pages(page_s *page, ulong pfn, uint order)
{
// void __free_pages_core(page_s *page, unsigned int order)
// {
	uint nr_pages = 1 << order;
	page_s *p = page;

	/*
	 * When initializing the memmap, __init_single_page() sets the refcount
	 * of all pages to 1 ("allocated"/"not free"). We have to set the
	 * refcount of all involved pages to 0.
	 */
	for (uint loop = 0; loop < nr_pages; loop++, p++) {
		__ClearPageReserved(p);
		set_page_count(p, 0);
	}

	atomic_long_add(nr_pages, &myos_page_zone(page)->managed_pages);

	/*
	 * Bypass PCP and place fresh pages right to the tail, primarily
	 * relevant for memory onlining.
	 */
	__free_pages_ok(page, order);
// }
}

/**
 * get_pfn_range_for_nid - Return the start and end page frames for a node
 * @nid: The nid to return the range for. If MAX_NUMNODES, the min and max PFN are returned.
 * @start_pfn: Passed by reference. On return, it will have the node start_pfn.
 * @end_pfn: Passed by reference. On return, it will have the node end_pfn.
 *
 * It returns the start and end page frame of a node based on information
 * provided by memblock_set_node(). If called for a node
 * with no available memory, a warning is printed and the start and end
 * PFNs will be 0.
 */
void __init
get_pfn_range(ulong *start_pfn, ulong *end_pfn)
{
	ulong this_start_pfn, this_end_pfn;
	int i;

	*start_pfn = -1UL;
	*end_pfn = 0;

	for_each_mem_pfn_range(i, &this_start_pfn, &this_end_pfn) {
		*start_pfn = min(*start_pfn, this_start_pfn);
		*end_pfn = max(*end_pfn, this_end_pfn);
	}

	if (*start_pfn == -1UL)
		*start_pfn = 0;
}





/*==============================================================================================*
 *											swap.c												*
 *==============================================================================================*/

// static void __folio_put_small(struct folio *folio)
// {
// 	__page_cache_release(folio);
// 	mem_cgroup_uncharge(folio);
// 	free_unref_page(&folio->page, 0);
// }

// static void __folio_put_large(struct folio *folio)
// {
// 	/*
// 	 * __page_cache_release() is supposed to be called for thp, not for
// 	 * hugetlb. This is because hugetlb page does never have PageLRU set
// 	 * (it's never listed to any LRU lists) and no memcg routines should
// 	 * be called for hugetlb (it has a separate hugetlb_cgroup.)
// 	 */
// 	if (!folio_test_hugetlb(folio))
// 		__page_cache_release(folio);
// 	destroy_large_folio(folio);
// }

void __folio_put(folio_s *folio)
{
	// if (unlikely(folio_is_zone_device(folio)))
	// 	free_zone_device_page(&folio->page);
	// else if (unlikely(folio_test_large(folio)))
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
}
EXPORT_SYMBOL(__folio_put);
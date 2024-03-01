#define PAGEALLOC_DEFINATION

#include "page_alloc.h"

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
#include <linux/mm/mm.h>

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
#ifdef CONFIG_ZONE_DMA
	[ZONE_DMA] = 256,
#endif
#ifdef CONFIG_ZONE_DMA32
	[ZONE_DMA32] = 256,
#endif
	[ZONE_NORMAL] = 32,
// #ifdef CONFIG_HIGHMEM
// 	[ZONE_HIGHMEM] = 0,
// #endif
	// [ZONE_MOVABLE] = 0,
};

char * const zone_names[MAX_NR_ZONES] = {
#ifdef CONFIG_ZONE_DMA
	 "DMA",
#endif
#ifdef CONFIG_ZONE_DMA32
	 "DMA32",
#endif
	 "Normal",
// #ifdef CONFIG_HIGHMEM
// 	 "HighMem",
// #endif
	//  "Movable",
#ifdef CONFIG_ZONE_DEVICE
	 "Device",
#endif
};

const char * const migratetype_names[MIGRATE_TYPES] = {
	"Unmovable",
	"Movable",
	"Reclaimable",
	"HighAtomic",
#ifdef CONFIG_CMA
	"CMA",
#endif
#ifdef CONFIG_MEMORY_ISOLATION
	"Isolate",
#endif
};


/*==============================================================================================*
 *								private fuctions for buddy system								*
 *==============================================================================================*/
/*
 * This function returns the order of a free page in the buddy system. In
 * general, page_zone(page)->lock must be held by the caller to prevent the
 * page from being allocated in parallel and returning garbage as the order.
 * If a caller does not hold page_zone(page)->lock, it must guarantee that the
 * page cannot be allocated or merged in parallel. Alternatively, it must
 * handle invalid values gracefully, and use buddy_order_unsafe() below.
 */
static inline unsigned long
buddy_order(page_s *page) {

	/* PageBuddy() must be checked by the caller */
	return page_private(page);
}

static inline void
set_compound_order(page_s *page, unsigned int order) {
	page[1].compound_order = order;
	page[1].compound_nr = 1U << order;
}

static void prep_compound_head(page_s *page, unsigned int order)
{
	// set_compound_page_dtor(page, COMPOUND_PAGE_DTOR);
	set_compound_order(page, order);
	// atomic_set(compound_mapcount_ptr(page), -1);
	// if (hpage_pincount_available(page))
	// 	atomic_set(compound_pincount_ptr(page), 0);
}

static void prep_compound_tail(page_s *head, int tail_idx)
{
	page_s *p = head + tail_idx;

	// p->mapping = TAIL_MAPPING;
	set_compound_head(p, head);
}

void prep_compound_page(page_s *page, unsigned int order)
{
	int i;
	int nr_pages = 1 << order;

	__SetPageHead(page);
	for (i = 1; i < nr_pages; i++)
		prep_compound_tail(page, i);

	prep_compound_head(page, order);
}


static inline void
set_buddy_order(page_s *page, unsigned int order) {

	page->private = (unsigned long)order;
	__SetPageBuddy(page);
}

/* Used for pages not on another list */
static inline void
add_to_free_list(page_s *page,
		zone_s *zone, unsigned int order) {

	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_hdr_push(fa_lhdr, &page->lru);
}

/* Used for pages not on another list */
static inline void
add_to_free_list_tail(page_s *page,
		zone_s *zone, unsigned int order) {

	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_hdr_append(fa_lhdr, &page->lru);
}

static inline page_s
*get_page_from_free_area(List_hdr_s *area) {

	List_s *pg_lp = list_hdr_pop(area);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, page_s, lru);
}

static inline page_s
*del_page_from_free_list(page_s *page,
		zone_s * zone, unsigned int order) {

	List_s * pg_lp = list_hdr_delete(&zone->free_area[order],
			&page->lru);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, page_s, lru);
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

	unsigned long size = 1 << high;
	while (high > low) {
		high--;
		size >>= 1;
		page_s *p = &page[size];
		list_init(&p->lru, page);
		add_to_free_list(p, zone, high);
		set_buddy_order(p, high);
	}
}

/*
 * Locate the page_s for both the matching buddy in our
 * pair (buddy1) and the combined O(n+1) page they form (page).
 *
 * 1) Any buddy B1 will have an order O twin B2 which satisfies
 * the following equation:
 *     B2 = B1 ^ (1 << O)
 * For example, if the starting buddy (buddy2) is #8 its order
 * 1 buddy is #10:
 *     B2 = 8 ^ (1 << 1) = 8 ^ 2 = 10
 *
 * 2) Any buddy B will have an order O+1 parent P which
 * satisfies the following equation:
 *     P = B & ~(1 << O)
 *
 * Assumption: *_mem_map is contiguous at least up to MAX_ORDER
 */
static inline unsigned long
__find_buddy_pfn(unsigned long page_pfn, unsigned int order) {

	return page_pfn ^ (1 << order);
}

/*
 * This function checks whether a page is free && is the buddy
 * we can coalesce a page and its buddy if
 * (a) the buddy is not in a hole (check before calling!) &&
 * (b) the buddy is in the buddy system &&
 * (c) a page and its buddy have the same order &&
 * (d) a page and its buddy are in the same zone.
 *
 * For recording whether a page is in the buddy system, we set PageBuddy.
 * Setting, clearing, and testing PageBuddy is serialized by zone->lock.
 *
 * For recording page's order, we use page_private(page).
 */
static inline bool
page_is_buddy(page_s *page, page_s *buddy, unsigned int order) {

	if (!page_is_guard(page) && !PageBuddy(buddy)) return false;

	if (buddy_order(buddy) != order) return false;

	if (myos_page_zone(page) != myos_page_zone(buddy)) return false;

	return true;
}

static void
prep_new_page(page_s *page, unsigned int order, gfp_t gfp_flags)
{
	// post_alloc_hook(page, order, gfp_flags);

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
		clear_page_pfmemalloc(page);
}

/*
 * Go through the free lists for the given migratetype and remove
 * the smallest available page from the freelists
 */
// Linux function proto :
// static __always_inline page_s *__rmqueue_smallest(struct zone *zone,
//					unsigned int order, int migratetype)
static inline page_s *
__rmqueue_smallest(zone_s *zone, unsigned int order)
{
	unsigned int current_order;
	page_s *page;

	/* Find a page of the appropriate size in the preferred list */
	for (current_order = order; current_order < MAX_ORDER; ++current_order) {
		if (zone->free_area[current_order].count == 0)
			continue;

		List_s * page_lp = list_hdr_pop(&zone->free_area[current_order]);
		while (page_lp == NULL);

		page = container_of(page_lp, page_s, lru);
		__ClearPageBuddy(page);
		page->private = 0;

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
__myos_free_one_page(page_s *page, unsigned long pfn,
		zone_s *zone, unsigned int order)
{
	unsigned long buddy_pfn;
	unsigned long combined_pfn;
	page_s *buddy;

	while (order < MAX_ORDER) {
		buddy_pfn = __find_buddy_pfn(pfn, order);
		buddy = page + (buddy_pfn - pfn);

		if (page_is_buddy(page, buddy, order) == false ||
			!list_in_lhdr(&zone->free_area[order], &page->lru))
			break;

		list_hdr_delete(&zone->free_area[order], &page->lru);
		combined_pfn = buddy_pfn & pfn;
		page = page + (combined_pfn - pfn);
		pfn = combined_pfn;
		order++;
	}

	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
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
page_s *__myos_alloc_pages(gfp_t gfp, unsigned order)
{
	page_s *page;
	zone_s *zone = NULL;

	/*
	 * There are several places where we assume that the order value is sane
	 * so bail out early if the request is out of bound.
	 */
	if (unlikely(order >= MAX_ORDER)) {
		// WARN_ON_ONCE(!(gfp & __GFP_NOWARN));
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

		for (i = start_prefered_idx; i < max_prefered_idx; i++)
		{
			zone = &(NODE_DATA(0)->node_zones[prefered_zone_list[i]]);
		// static inline page_s
		// *rmqueue(struct zone *preferred_zone, struct zone *zone, unsigned int order,
		// 		gfp_t gfp_flags, unsigned int alloc_flags, int migratetype)
		// {
			page = __rmqueue_smallest(zone, order);
			if (likely(page))
			{
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
void __free_pages(page_s *page, unsigned int order)
{
	unsigned long pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	// linux call stack :
	// static inline void free_the_page(page_s *page, unsigned int order)
	//								||
	//								\/
	// static void __free_pages_ok(page_s *page, unsigned int order, fpi_t fpi_flags)
	//								||
	//								\/
	// static inline void __free_one_page(page_s *page, unsigned long pfn,
	//					struct zone *zone, unsigned int order, int migratetype,
	//					fpi_t fpi_flags)

	__myos_free_one_page(page, pfn, zone, order);
}

void free_pages(unsigned long addr, unsigned int order) {
	if (addr != 0) {
		// VM_BUG_ON(!virt_addr_valid((void *)addr));
		__free_pages(virt_to_page((void *)addr), order);
	}
}

/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
static void
__free_pages_ok(page_s *page, unsigned int order)
{
	unsigned long flags;
	int migratetype;
	unsigned long pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	// if (!free_pages_prepare(page, order, true, fpi_flags))
	// 	return;

	// migratetype = get_pfnblock_migratetype(page, pfn);

	// spin_lock_irqsave(&zone->lock, flags);
	// if (unlikely(has_isolate_pageblock(zone) ||
	// 	is_migrate_isolate(migratetype))) {
	// 	migratetype = get_pfnblock_migratetype(page, pfn);
	// }
	__myos_free_one_page(page, pfn, zone, order);
	// spin_unlock_irqrestore(&zone->lock, flags);

	// __count_vm_events(PGFREE, 1 << order);
}

void __init
memblock_free_pages(page_s *page, unsigned long pfn, unsigned int order)
{
	// {
		unsigned int nr_pages = 1 << order;
		page_s *p = page;
		unsigned int loop;

		/*
		 * When initializing the memmap, __init_single_page() sets the refcount
		 * of all pages to 1 ("allocated"/"not free"). We have to set the
		 * refcount of all involved pages to 0.
		 */
		// prefetchw(p);
		// for (loop = 0; loop < (nr_pages - 1); loop++, p++) {
		for (loop = 0; loop < nr_pages; loop++, p++) {
			// prefetchw(p + 1);
			__ClearPageReserved(p);
			set_page_count(p, 0);
		}
		// __ClearPageReserved(p);
		// set_page_count(p, 0);

		atomic_long_add(nr_pages, &myos_page_zone(page)->managed_pages);

		/*
		 * Bypass PCP and place fresh pages right to the tail, primarily
		 * relevant for memory onlining.
		 */
		__free_pages_ok(page, order);
	// }
}


/*
 * Common helper functions. Never use with __GFP_HIGHMEM because the returned
 * address cannot represent highmem pages. Use alloc_pages and then kmap if
 * you need to access high mem.
 */
unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order)
{
	page_s *page;

	page = alloc_pages(gfp_mask & ~__GFP_HIGHMEM, order);
	if (!page)
		return 0;
	return (unsigned long)page_to_virt(page);
}

unsigned long get_zeroed_page(gfp_t gfp_mask)
{
	unsigned long retval = __get_free_pages(gfp_mask | __GFP_ZERO, 0);
	memset((void *)retval, 0, PAGE_SIZE);
	return retval;
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
get_pfn_range( unsigned long *start_pfn, unsigned long *end_pfn)
{
	unsigned long this_start_pfn, this_end_pfn;
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
 *									myos page funcs for buddy system							*
 *==============================================================================================*/
page_s *paddr_to_page(phys_addr_t paddr)
{
	unsigned long pfn = round_up((uint64_t)paddr, PAGE_SIZE) / PAGE_SIZE;
	// return &mem_info.pages[pg_idx];
	return pfn_to_page(pfn);
}
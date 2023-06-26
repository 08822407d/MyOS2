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

#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>
// #include <linux/highmem.h>
// #include <linux/swap.h>
// #include <linux/swapops.h>
// #include <linux/interrupt.h>
// #include <linux/pagemap.h>
// #include <linux/jiffies.h>
#include <linux/mm/memblock.h>
#include <linux/kernel/compiler.h>
#include <linux/kernel/kernel.h>
// #include <linux/kasan.h>
// #include <linux/module.h>
// #include <linux/suspend.h>
// #include <linux/pagevec.h>
#include <linux/block/blkdev.h>
#include <linux/kernel/slab.h>
// #include <linux/ratelimit.h>
// #include <linux/oom.h>
// #include <linux/topology.h>
// #include <linux/sysctl.h>
#include <linux/kernel/cpu.h>
// #include <linux/cpuset.h>
// #include <linux/memory_hotplug.h>
// #include <linux/nodemask.h>
// #include <linux/vmalloc.h>
// #include <linux/vmstat.h>
// #include <linux/mempolicy.h>
// #include <linux/memremap.h>
// #include <linux/stop_machine.h>
// #include <linux/random.h>
// #include <linux/sort.h>
#include <linux/mm/pfn.h>
// #include <linux/backing-dev.h>
// #include <linux/fault-inject.h>
// #include <linux/page-isolation.h>
// #include <linux/debugobjects.h>
// #include <linux/kmemleak.h>
// #include <linux/compaction.h>
// #include <trace/events/kmem.h>
// #include <trace/events/oom.h>
// #include <linux/prefetch.h>
// #include <linux/mm_inline.h>
// #include <linux/mmu_notifier.h>
// #include <linux/migrate.h>
// #include <linux/hugetlb.h>
// #include <linux/sched/rt.h>
#include <linux/sched/mm.h>
// #include <linux/page_owner.h>
// #include <linux/page_table_check.h>
#include <linux/kernel/kthread.h>
// #include <linux/memcontrol.h>
// #include <linux/ftrace.h>
// #include <linux/lockdep.h>
// #include <linux/nmi.h>
// #include <linux/psi.h>
// #include <linux/padata.h>
// #include <linux/khugepaged.h>
// #include <linux/buffer_head.h>
// #include <linux/delayacct.h>
#include <asm/sections.h>
// #include <asm/tlbflush.h>
#include <asm/div64.h>
#include "internal.h"
// #include "shuffle.h"
// #include "page_reporting.h"


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
	// [ZONE_MOVABLE] = 0,
};

static char * const zone_names[MAX_NR_ZONES] = {
#ifdef CONFIG_ZONE_DMA
	 "DMA",
#endif
#ifdef CONFIG_ZONE_DMA32
	 "DMA32",
#endif
	 "Normal",
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

static unsigned long arch_zone_lowest_possible_pfn[MAX_NR_ZONES] __initdata;
static unsigned long arch_zone_highest_possible_pfn[MAX_NR_ZONES] __initdata;


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

/*
 * Higher-order pages are called "compound pages".  They are structured thusly:
 *
 * The first PAGE_SIZE page is called the "head page" and have PG_head set.
 *
 * The remaining PAGE_SIZE pages are called "tail pages". PageTail() is encoded
 * in bit 0 of page->compound_head. The rest of bits is pointer to head page.
 *
 * The first tail page's ->compound_dtor holds the offset in array of compound
 * page destructors. See compound_page_dtors.
 *
 * The first tail page's ->compound_order holds the order of allocation.
 * This usage means that zero-order pages may not be compound.
 */
void free_compound_page(page_s *page)
{
	// mem_cgroup_uncharge(page_folio(page));
	// free_the_page(page, compound_order(page));
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
add_to_free_list(page_s *page, zone_s *zone,
		unsigned int order)
{
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_hdr_push(fa_lhdr, &page->lru);
}

/* Used for pages not on another list */
static inline void
add_to_free_list_tail(page_s *page, zone_s *zone,
		unsigned int order)
{
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_hdr_append(fa_lhdr, &page->lru);
}

static inline page_s *
get_page_from_free_area(List_hdr_s *area)
{
	List_s *pg_lp = list_hdr_pop(area);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, page_s, lru);
}

static inline page_s *
del_page_from_free_list(page_s *page, zone_s * zone,
		unsigned int order)
{
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
expand(zone_s *zone, page_s *page, int low, int high)
{
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
page_is_buddy(page_s *page, page_s *buddy, unsigned int order)
{
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
	// 	clear_page_pfmemalloc(page);
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
__free_one_page(page_s *page, unsigned long pfn,
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

static void __meminit
__init_single_page(page_s *page, unsigned long pfn, unsigned long zone)
{
	// mm_zero_struct_page(page);
	// set_page_links(page, zone, nid, pfn);
	// init_page_count(page);
	// page_mapcount_reset(page);
	// page_cpupid_reset_last(page);
	// page_kasan_tag_reset(page);

	list_init(&page->lru, page);
// #ifdef WANT_PAGE_VIRTUAL
	// /* The shift won't overflow because ZONE_NORMAL is below 4G. */
	// if (!is_highmem_idx(zone))
	// 	set_page_address(page, __va(pfn << PAGE_SHIFT));
// #endif
}

void __meminit
reserve_bootmem_region(phys_addr_t start, phys_addr_t end)
{
	unsigned long start_pfn = PFN_DOWN(start);
	unsigned long end_pfn = PFN_UP(end);

	for (; start_pfn < end_pfn; start_pfn++) {
		if (pfn_valid(start_pfn)) {
			page_s *page = pfn_to_page(start_pfn);

			// init_reserved_page(start_pfn);

			/* Avoid false-positive PageTail() */
			list_init(&page->lru, &page);

			/*
			 * no need for atomic set_bit because the struct
			 * page is not visible yet so nobody should
			 * access it yet.
			 */
			__SetPageReserved(page);
		}
	}
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
page_s *__alloc_pages(gfp_t gfp, unsigned order)
{
	page_s *page;
	zone_s *zone = NULL;

	/*
	 * There are several places where we assume that the order value is sane
	 * so bail out early if the request is out of bound.
	 */
	if (order >= MAX_ORDER) return NULL;

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
			if (page)
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

	__free_one_page(page, pfn, zone, order);
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
	__free_one_page(page, pfn, zone, order);
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
	return (unsigned long) page_address(page);
}

unsigned long get_zeroed_page(gfp_t gfp_mask)
{
	return __get_free_pages(gfp_mask | __GFP_ZERO, 0);
}


static void __init
memmap_init(unsigned long *max_zone_pfn)
{
	unsigned long start_pfn, end_pfn;
	start_pfn = 1;

	for (int i = 0; i < MAX_NR_ZONES; i++) {
		end_pfn = max(max_zone_pfn[i], start_pfn);
		zone_s *zone = &(NODE_DATA(0)->node_zones[i]);

		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
		zone->name = zone_names[i];
		zone->zone_pgdat = NODE_DATA(0);

		for (int j = 0; j < MAX_ORDER; j++)
			list_hdr_init(&zone->free_area[j]);

		for (unsigned long pfn = start_pfn; pfn < end_pfn; pfn++)
		{
			page_s *page = pfn_to_page(pfn);
			__init_single_page(page, pfn, i);
		}

		start_pfn = end_pfn;
	}
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


static unsigned long __init
__myos_calculate_node_pages(unsigned long zone_type,
		unsigned long node_start_pfn, unsigned long node_end_pfn,
		unsigned long *zone_start_pfn, unsigned long *zone_end_pfn,
		unsigned long *nr_absent, unsigned long *nr_spanned)
{
	unsigned long zone_low = arch_zone_lowest_possible_pfn[zone_type];
	unsigned long zone_high = arch_zone_highest_possible_pfn[zone_type];
	/* When hotadd a new node from cpu_up(), the node should be empty */
	if (!node_start_pfn && !node_end_pfn)
		return 0;

	/* Get the start and end of the zone */
	*zone_start_pfn = clamp(node_start_pfn, zone_low, zone_high);
	*zone_end_pfn = clamp(node_end_pfn, zone_low, zone_high);

	/* Check that this node has pages within the zone's required range */
	if (*zone_end_pfn < node_start_pfn || *zone_start_pfn > node_end_pfn)
		return 0;

	/* count absent pages */
	// unsigned long __init __absent_pages_in_range(
	// 	unsigned long range_start_pfn, unsigned long range_end_pfn)
	// {
		*nr_absent = *zone_end_pfn - *zone_start_pfn;
		unsigned long start_pfn, end_pfn;
		int i;

		for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
			start_pfn = clamp(start_pfn, *zone_start_pfn, *zone_end_pfn);
			end_pfn = clamp(end_pfn, *zone_start_pfn, *zone_end_pfn);
			*nr_absent -= end_pfn - start_pfn;
		}
	// }

	/* count spanned pages */
	*zone_end_pfn = min(*zone_end_pfn, node_end_pfn);
	*zone_start_pfn = max(*zone_start_pfn, node_start_pfn);
	*nr_spanned = *zone_end_pfn - *zone_start_pfn;
	return *nr_spanned;
}

static void __init
calculate_node_totalpages(pg_data_t *pgdat,
		unsigned long node_start_pfn, unsigned long node_end_pfn)
{
	unsigned long realtotalpages = 0, totalpages = 0;
	enum zone_type i;

	for (i = 0; i < MAX_NR_ZONES; i++) {
		zone_s *zone = pgdat->node_zones + i;
		zone->zone_pgdat = pgdat;
		unsigned long zone_start_pfn, zone_end_pfn;
		unsigned long spanned, absent;
		unsigned long real_size;

		__myos_calculate_node_pages(i,
				node_start_pfn, node_end_pfn,
				&zone_start_pfn, &zone_end_pfn,
				&absent, &spanned);

		real_size = spanned - absent;

		if (spanned)
			zone->zone_start_pfn = zone_start_pfn;
		else
			zone->zone_start_pfn = 0;
		zone->spanned_pages = spanned;
		zone->present_pages = real_size;

		totalpages += spanned;
		realtotalpages += real_size;
	}

	pgdat->nr_zones = MAX_NR_ZONES;
	pgdat->node_spanned_pages = totalpages;
	pgdat->node_present_pages = realtotalpages;
	// pr_debug("On node %d totalpages: %lu\n", pgdat->node_id, realtotalpages);
}

static void __meminit
pgdat_init_internals(pg_data_t *pgdat)
{
	// int i;

	// pgdat_resize_init(pgdat);

	// pgdat_init_split_queue(pgdat);
	// pgdat_init_kcompactd(pgdat);

	// init_waitqueue_head(&pgdat->kswapd_wait);
	// init_waitqueue_head(&pgdat->pfmemalloc_wait);

	// for (i = 0; i < NR_VMSCAN_THROTTLE; i++)
	// 	init_waitqueue_head(&pgdat->reclaim_wait[i]);

	// pgdat_page_ext_init(pgdat);
	// lruvec_init(&pgdat->__lruvec);
}

static void __meminit
zone_init_internals(zone_s *zone, enum zone_type idx,
		unsigned long remaining_pages)
{
	// atomic_long_set(&zone->managed_pages, remaining_pages);
	zone->name = zone_names[idx];
	zone->zone_pgdat = NODE_DATA(0);
	// spin_lock_init(&zone->lock);
	// zone_seqlock_init(zone);
	// zone_pcp_init(zone);
}

/*
 * Set up the zone data structures:
 *   - mark all pages reserved
 *   - mark all memory queues empty
 *   - clear the memory bitmaps
 *
 * NOTE: pgdat should get zeroed by caller.
 * NOTE: this function is only called during early init.
 */
static void __init
free_area_init_core(pg_data_t *pgdat)
{
	enum zone_type j;

	pgdat_init_internals(pgdat);
	// pgdat->per_cpu_nodestats = &boot_nodestats;

	for (j = 0; j < MAX_NR_ZONES; j++)
	{
		zone_s *zone = pgdat->node_zones + j;
		unsigned long size, freesize, memmap_pages;

		size = zone->spanned_pages;
		freesize = zone->present_pages;

		/*
		 * Adjust freesize so that it accounts for how much memory
		 * is used by this zone for memmap. This affects the watermark
		 * and per-cpu initialisations
		 */
		// memmap_pages = calc_memmap_size(size, freesize);
		// if (!is_highmem_idx(j)) {
		// 	if (freesize >= memmap_pages) {
		// 		freesize -= memmap_pages;
		// 		if (memmap_pages)
		// 			pr_debug("  %s zone: %lu pages used for memmap\n",
		// 				 zone_names[j], memmap_pages);
		// 	} else
		// 		pr_warn("  %s zone: %lu memmap pages exceeds freesize %lu\n",
		// 			zone_names[j], memmap_pages, freesize);
		// }

		// /* Account for reserved pages */
		// if (j == 0 && freesize > dma_reserve) {
		// 	freesize -= dma_reserve;
		// 	pr_debug("  %s zone: %lu pages reserved\n", zone_names[0], dma_reserve);
		// }

		// nr_kernel_pages += freesize;

		/*
		 * Set an approximate value for lowmem here, it will be adjusted
		 * when the bootmem allocator frees pages into the buddy system.
		 * And all highmem pages will be managed by the buddy system.
		 */
		zone_init_internals(zone, j, freesize);

		if (!size)
			continue;

	// 	set_pageblock_order();
	// 	setup_usemap(zone);
	// 	init_currently_empty_zone(zone, zone->zone_start_pfn, size);
	}
}

static void __init
alloc_node_mem_map(pg_data_t *pgdat)
{
	unsigned long __maybe_unused start = 0;
	unsigned long __maybe_unused offset = 0;

	/* Skip empty nodes */
	if (!pgdat->node_spanned_pages)
		return;

	start = pgdat->node_start_pfn & ~(MAX_ORDER_NR_PAGES - 1);
	offset = pgdat->node_start_pfn - start;
	if (!pgdat->node_mem_map) {
		unsigned long size, end;
		page_s *map;

		/*
		 * The zone's endpoints aren't required to be MAX_ORDER
		 * aligned but the node_mem_map endpoints must be in order
		 * for the buddy allocator to function correctly.
		 */
		end = pgdat->node_start_pfn + pgdat->node_spanned_pages;
		end = ALIGN(end, MAX_ORDER_NR_PAGES);
		size =  (end - start) * sizeof(page_s);

		map = (void *)myos_phys2virt(memblock_alloc_range(
				size, SMP_CACHE_BYTES, MAX_DMA_PFN, 0));
		if (!map)
		{
			while (1);
			
			// panic("Failed to allocate %ld bytes for node %d memory map\n",
			// 		size, pgdat->node_id);
		}
		memset(map, 0, size);
		pgdat->node_mem_map = mem_map = map + offset;
	}
	// pr_debug("%s: node %d, pgdat %08lx, node_mem_map %08lx\n",
	// 			__func__, pgdat->node_id, (unsigned long)pgdat,
	// 			(unsigned long)pgdat->node_mem_map);


	for (int i = 0; i < NODE_DATA(0)->node_spanned_pages; i++)
	{
		page_s *page = &(NODE_DATA(0)->node_mem_map[i]);
		list_init(&page->lru, page);
	}
}

/**
 * free_area_init - Initialise all pg_data_t and zone data
 * @max_zone_pfn: an array of max PFNs for each zone
 *
 * This will call free_area_init_node() for each active node in the system.
 * Using the page ranges provided by memblock_set_node(), the size of each
 * zone in each node and their holes is calculated. If the maximum PFN
 * between two adjacent zones match, it is assumed that the zone is empty.
 * For example, if arch_max_dma_pfn == arch_max_dma32_pfn, it is assumed
 * that arch_max_dma32_pfn has no pages. It is also assumed that a zone
 * starts where the previous one ended. For example, ZONE_DMA32 starts
 * at arch_max_dma_pfn.
 */
// static void __init free_area_init_node(int nid)
void __init
free_area_init(unsigned long *max_zone_pfn)
{
	unsigned long start_pfn = 1;
	unsigned long end_pfn = 0;
	/* Record where the zone boundaries are */
	memset(arch_zone_lowest_possible_pfn, 0,
				sizeof(arch_zone_lowest_possible_pfn));
	memset(arch_zone_highest_possible_pfn, 0,
				sizeof(arch_zone_highest_possible_pfn));

	for (int i = 0; i < MAX_NR_ZONES; i++) {
		end_pfn = max(max_zone_pfn[i], start_pfn);
		arch_zone_lowest_possible_pfn[i] = start_pfn;
		arch_zone_highest_possible_pfn[i] = end_pfn;

		start_pfn = end_pfn;
	}

	// static void __init free_area_init_node(int nid)
	// {
		pg_data_t *pgdat = NODE_DATA(0);
		memset(pgdat, 0, sizeof(pg_data_t));

		start_pfn = 0;
		end_pfn = 0;

		get_pfn_range(&start_pfn, &end_pfn);

		pgdat->node_start_pfn = start_pfn;
		calculate_node_totalpages(pgdat, start_pfn, end_pfn);

		alloc_node_mem_map(pgdat);
		free_area_init_core(pgdat);
	// }

	memmap_init(max_zone_pfn);
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
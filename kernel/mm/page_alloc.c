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
// #include <linux/compiler.h>
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
// #include <linux/cpu.h>
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
// #include <linux/sched/mm.h>
// #include <linux/page_owner.h>
// #include <linux/page_table_check.h>
// #include <linux/kthread.h>
// #include <linux/memcontrol.h>
// #include <linux/ftrace.h>
// #include <linux/lockdep.h>
// #include <linux/nmi.h>
// #include <linux/psi.h>
// #include <linux/padata.h>
// #include <linux/khugepaged.h>
// #include <linux/buffer_head.h>
// #include <linux/delayacct.h>
// #include <asm/sections.h>
// #include <asm/tlbflush.h>
// #include <asm/div64.h>
#include "internal.h"
// #include "shuffle.h"
// #include "page_reporting.h"


#include <obsolete/glo.h>
#include <obsolete/mutex.h>

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
#ifdef CONFIG_HIGHMEM
	[ZONE_HIGHMEM] = 0,
#endif
	[ZONE_MOVABLE] = 0,
};

static char * const zone_names[MAX_NR_ZONES] = {
#ifdef CONFIG_ZONE_DMA
	 "DMA",
#endif
#ifdef CONFIG_ZONE_DMA32
	 "DMA32",
#endif
	 "Normal",
#ifdef CONFIG_HIGHMEM
	 "HighMem",
#endif
	 "Movable",
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

recurs_lock_T	page_alloc_lock;

pg_data_t		pg_list;

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
static inline unsigned long get_buddy_order(page_s * page)
{
	/* PageBuddy() must be checked by the caller */
	return page->buddy_order;
}

static inline void set_buddy_order(page_s * page, unsigned int order)
{
	page->buddy_order = order;
}

/* Used for pages not on another list */
static inline void add_to_free_list(page_s * page,
		zone_s * zone, unsigned int order)
{
	List_hdr_s * fa_lhdr = &zone->free_area[order];
	list_hdr_push(fa_lhdr, &page->free_list);
	zone->zone_pgdat->node_present_pages += 1 << order;
}

/* Used for pages not on another list */
static inline void add_to_free_list_tail(page_s * page,
		zone_s * zone, unsigned int order)
{
	List_hdr_s * fa_lhdr = &zone->free_area[order];
	list_hdr_append(fa_lhdr, &page->free_list);
	zone->zone_pgdat->node_present_pages += 1 << order;
}

static inline page_s * get_page_from_free_area(List_hdr_s * area)
{
	List_s * pg_lp = list_hdr_pop(area);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, page_s, free_list);
}

static inline page_s * del_page_from_free_list(page_s * page,
		zone_s * zone, unsigned int order)
{
	List_s * pg_lp = list_hdr_delete(&zone->free_area[order], &page->free_list);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, page_s, free_list);
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
static inline void expand(zone_s * zone, page_s * page, int low, int high)
{
	unsigned long size = 1 << high;

	while (high > low) {
		high--;
		size >>= 1;

		add_to_free_list(&page[size], zone, high);
		set_buddy_order(&page[size], high);
	}
}

/*
 * Locate the struct page for both the matching buddy in our
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
__find_buddy_pfn(unsigned long page_pfn, unsigned int order)
{
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
static inline bool page_is_buddy(page_s *page, page_s *buddy, unsigned int order)
{
	if (get_buddy_order(buddy) != order)
		return false;

	if (myos_page_zone(page) != myos_page_zone(buddy))
		return false;

	return true;
}

/*
 * Go through the free lists for the given migratetype and remove
 * the smallest available page from the freelists
 */
// Linux function proto :
// static __always_inline struct page *__rmqueue_smallest(struct zone *zone,
//					unsigned int order, int migratetype)
static inline 
page_s * __rmqueue_smallest(zone_s * zone, unsigned int order)
{
	unsigned int current_order;
	page_s * page;

	/* Find a page of the appropriate size in the preferred list */
	for (current_order = order; current_order < MAX_ORDER; ++current_order) {
		if (zone->free_area[current_order].count == 0)
			continue;

		List_s * page_lp = list_hdr_pop(&zone->free_area[current_order]);
		while (page_lp == NULL);

		page = container_of(page_lp, page_s, free_list);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			page[i].ref_count++;

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
// static inline void __free_one_page(struct page *page, unsigned long pfn,
//					struct zone *zone, unsigned int order, int migratetype,
//					fpi_t fpi_flags)
static inline void __free_one_page(page_s *page, unsigned long pfn,
		zone_s *zone, unsigned int order)
{
	unsigned long buddy_pfn;
	unsigned long combined_pfn;
	page_s * buddy;

	while (order < MAX_ORDER) {
		buddy_pfn = __find_buddy_pfn(pfn, order);
		buddy = page + (buddy_pfn - pfn);

		if (page_is_buddy(page, buddy, order) == false ||
			!list_in_lhdr(&zone->free_area[order], &page->free_list))
			break;

		list_hdr_delete(&zone->free_area[order], &page->free_list);
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
page_s *alloc_pages(gfp_t gfp, unsigned order)
{
	page_s * page;
	zone_s * zone = &pg_list.node_zones[gfp];

	// linux call stack :
	// struct page *__alloc_pages(gfp_t gfp, unsigned int order, int preferred_nid,
	//					nodemask_t *nodemask)
	//								||
	//								\/
	// static struct page * get_page_from_freelist(gfp_t gfp_mask, unsigned int order,
	//					int alloc_flags, const struct alloc_context *ac)
	//								||
	//								\/
	// static inline struct page *rmqueue(struct zone *preferred_zone, struct zone *zone,
	//					unsigned int order, gfp_t gfp_flags, unsigned int alloc_flags,
	// 					int migratetype)
	//								||
	//								\/
	// static __always_inline struct page *__rmqueue_smallest(struct zone *zone,
	//					unsigned int order, int migratetype)

	page = __rmqueue_smallest(zone, order);
	return page;
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
// void free_pages(unsigned long addr, unsigned int order)
void free_pages(page_s * page, unsigned int order)
{
	unsigned long pfn = page_to_pfn(page);
	zone_s * zone = myos_page_zone(page);

	// linux call stack :
	// void __free_pages(struct page *page, unsigned int order)
	//								||
	//								\/
	// static inline void free_the_page(struct page *page, unsigned int order)
	//								||
	//								\/
	// static void __free_pages_ok(struct page *page, unsigned int order, fpi_t fpi_flags)
	//								||
	//								\/
	// static inline void __free_one_page(struct page *page, unsigned long pfn,
	//					struct zone *zone, unsigned int order, int migratetype,
	//					fpi_t fpi_flags)

	__free_one_page(page, pfn, zone, order);
}


/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
void __init memblock_free_pages(page_s * page,
		unsigned long pfn, unsigned int order)
{
	zone_s * zone = myos_page_zone(page);
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
}

static void __init memmap_init(unsigned long *max_zone_pfn)
{
	unsigned long start_pfn, end_pfn;
	start_pfn = 1;

	for (int i = 0; i < MAX_NR_ZONES; i++) {
		end_pfn = max(max_zone_pfn[i], start_pfn);
		zone_s * zone = &pg_list.node_zones[i];
		if (i == ZONE_MOVABLE)
			continue;

		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
		zone->name = zone_names[i];
		zone->zone_pgdat = &pg_list;

		for (int j = 0; j < MAX_ORDER; j++)
			list_hdr_init(&zone->free_area[j]);

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
void __init get_pfn_range( unsigned long *start_pfn, unsigned long *end_pfn)
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

static void __init calculate_node_totalpages(pg_data_t *pgdat,
		unsigned long node_start_pfn, unsigned long node_end_pfn)
{
	// unsigned long realtotalpages = 0, totalpages = 0;
	// enum zone_type i;

	// for (i = 0; i < MAX_NR_ZONES; i++) {
	// 	struct zone *zone = pgdat->node_zones + i;
	// 	unsigned long zone_start_pfn, zone_end_pfn;
	// 	unsigned long spanned, absent;
	// 	unsigned long size, real_size;

	// 	spanned = zone_spanned_pages_in_node(pgdat->node_id, i,
	// 					     node_start_pfn,
	// 					     node_end_pfn,
	// 					     &zone_start_pfn,
	// 					     &zone_end_pfn);
	// 	absent = zone_absent_pages_in_node(pgdat->node_id, i,
	// 					   node_start_pfn,
	// 					   node_end_pfn);

	// 	size = spanned;
	// 	real_size = size - absent;

	// 	if (size)
	// 		zone->zone_start_pfn = zone_start_pfn;
	// 	else
	// 		zone->zone_start_pfn = 0;
	// 	zone->spanned_pages = size;
	// 	zone->present_pages = real_size;

	// 	totalpages += size;
	// 	realtotalpages += real_size;
	// }

	pgdat->node_spanned_pages = node_end_pfn - node_start_pfn;
	// pgdat->node_spanned_pages = totalpages;
	// pgdat->node_present_pages = realtotalpages;
	// pr_debug("On node %d totalpages: %lu\n", pgdat->node_id, realtotalpages);
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
void __init free_area_init(unsigned long *max_zone_pfn)
{
	// static void __init free_area_init_node(int nid)
	// {
		pg_data_t *pgdat = &pg_list;
		unsigned long start_pfn = 0;
		unsigned long end_pfn = 0;

		get_pfn_range(&start_pfn, &end_pfn);

		pgdat->node_start_pfn = start_pfn;
		calculate_node_totalpages(pgdat, start_pfn, end_pfn);
	// }

	memmap_init(max_zone_pfn);
}


/*==============================================================================================*
 *									myos page funcs for buddy system							*
 *==============================================================================================*/
page_s * paddr_to_page(phys_addr_t paddr)
{
	unsigned long pfn = round_up((uint64_t)paddr, PAGE_SIZE) / PAGE_SIZE;
	// return &mem_info.pages[pg_idx];
	return pfn_to_page(pfn);
}

phys_addr_t page_to_paddr(page_s * page)
{
	unsigned long pfn = page_to_pfn(page);
	return (phys_addr_t)(pfn * PAGE_SIZE);
}


/*==============================================================================================*
 *								myos early init fuctions for buddy system						*
 *==============================================================================================*/
void myos_preinit_page()
{
	memset(&pg_list, 0, sizeof(pg_list));

	pg_list.nr_zones = MAX_NR_ZONES;
	pg_list.node_start_pfn = pg_list.node_zones[0].zone_start_pfn;
	pg_list.node_spanned_pages = max_low_pfn;
	mem_map = pg_list.node_mem_map = (void *)myos_phys2virt(
					memblock_alloc_range(sizeof(page_s) * pg_list.node_spanned_pages,
							sizeof(size_t), MAX_DMA_PFN, 0));
	memset(mem_map, 0, sizeof(page_s) * pg_list.node_spanned_pages);
	for (int i = 0; i < pg_list.node_spanned_pages; i++)
	{
		page_s * page = &pg_list.node_mem_map[i];
		list_init(&page->free_list, page);
		page->page_start_addr = (phys_addr_t)(i * PAGE_SIZE);
	}
}
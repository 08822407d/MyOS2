#include <sys/cdefs.h>
#include <sys/types.h>

#include <string.h>
#include <stddef.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/mutex.h>

#include <include/glo.h>
#include <include/mm.h>
#include <include/ktypes.h>
#include <include/printk.h>
#include <include/memblock.h>
#include <include/mmzone.h>

recurs_lock_T	page_alloc_lock;

pglist_data_s	pg_list;
Page_s *		mem_map;

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
static inline unsigned long get_buddy_order(Page_s * page)
{
	/* PageBuddy() must be checked by the caller */
	return page->buddy_order;
}

static inline void set_buddy_order(Page_s * page, unsigned int order)
{
	page->buddy_order = order;
}

/* Used for pages not on another list */
static inline void add_to_free_list(Page_s * page,
		zone_s * zone, unsigned int order)
{
	List_hdr_s * fa_lhdr = &zone->free_area[order];
	list_hdr_push(fa_lhdr, &page->free_list);
	zone->zone_pgdat->node_present_pages += 1 << order;
}

/* Used for pages not on another list */
static inline void add_to_free_list_tail(Page_s * page,
		zone_s * zone, unsigned int order)
{
	List_hdr_s * fa_lhdr = &zone->free_area[order];
	list_hdr_append(fa_lhdr, &page->free_list);
	zone->zone_pgdat->node_present_pages += 1 << order;
}

static inline Page_s * get_page_from_free_area(List_hdr_s * area)
{
	List_s * pg_lp = list_hdr_pop(area);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, Page_s, free_list);
}

static inline Page_s * del_page_from_free_list(Page_s * page,
		zone_s * zone, unsigned int order)
{
	List_s * pg_lp = list_hdr_delete(&zone->free_area[order], &page->free_list);
	if (pg_lp == NULL)
		return NULL;
	else
		return container_of(pg_lp, Page_s, free_list);
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
static inline void expand(zone_s * zone, Page_s * page, int low, int high)
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
static inline bool page_is_buddy(Page_s *page, Page_s *buddy, unsigned int order)
{
	if (get_buddy_order(buddy) != order)
		return false;

	if (page_zone(page) != page_zone(buddy))
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
Page_s * __rmqueue_smallest(zone_s * zone, unsigned int order)
{
	unsigned int current_order;
	Page_s * page;

	/* Find a page of the appropriate size in the preferred list */
	for (current_order = order; current_order < MAX_ORDER; ++current_order) {
		if (zone->free_area[current_order].count == 0)
			continue;

		List_s * page_lp = list_hdr_pop(&zone->free_area[current_order]);
		while (page_lp == NULL);

		page = container_of(page_lp, Page_s, free_list);
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
static inline void __free_one_page(Page_s *page, unsigned long pfn,
		zone_s *zone, unsigned int order)
{
	unsigned long buddy_pfn;
	unsigned long combined_pfn;
	Page_s * buddy;

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

/*===========================================3yy===================================================*
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
// Linux function proto :
// struct page *alloc_pages(gfp_t gfp, unsigned order)
Page_s * alloc_pages(unsigned int gfp, unsigned int order)
{
	Page_s * page;
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
void free_pages(Page_s * page, unsigned int order)
{
	unsigned long pfn = page_to_pfn(page);
	zone_s * zone = page_zone(page);

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

Page_s * paddr_to_page(phys_addr_t paddr)
{
	unsigned long pfn = PAGE_ROUND_UP((uint64_t)paddr) / PAGE_SIZE;
	// return &mem_info.pages[pg_idx];
	return pfn_to_page(pfn);
}

phys_addr_t page_to_paddr(Page_s * page)
{
	unsigned long pfn = page_to_pfn(page);
	return (phys_addr_t)(pfn * PAGE_SIZE);
}


/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
void preinit_page()
{
	memset(&pg_list, 0, sizeof(pg_list));

	pg_list.nr_zones = MAX_NR_ZONES;
	pg_list.node_start_pfn = pg_list.node_zones[0].zone_start_pfn;
	pg_list.node_spanned_pages = kparam.phys_page_nr;
	mem_map =
	pg_list.node_mem_map = phys2virt(memblock_alloc_range(sizeof(Page_s) * pg_list.node_spanned_pages,
								sizeof(size_t), (phys_addr_t)MAX_DMA_PFN, NULL));
	memset(mem_map, 0, sizeof(Page_s) * pg_list.node_spanned_pages);
	for (int i = 0; i < pg_list.node_spanned_pages; i++)
	{
		Page_s * page = &pg_list.node_mem_map[i];
		list_init(&page->free_list, page);
		page->page_start_addr = (phys_addr_t)(i * PAGE_SIZE);
	}
}

void init_page()
{
	zone_sizes_init();

	memblock_free_all();

	// set init flag
	kparam.init_flags.buddy = 1;
}

void memblock_free_pages(Page_s * page, unsigned long pfn,
							unsigned int order)
{
	zone_s * zone = page_zone(page);
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
}


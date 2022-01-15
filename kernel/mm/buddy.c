#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>

#include <string.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/mutex.h>

#include <include/glo.h>
#include <include/mm.h>
#include <include/ktypes.h>
#include <include/printk.h>
#include <include/memblock.h>
#include <include/mmzone.h>

memory_info_s	mem_info;
recurs_lock_T	page_alloc_lock;

pglist_data_s	pg_list;

/*==============================================================================================*
 *								fuction relate to physical page									*
 *==============================================================================================*/
void init_page_manage()
{	
	#ifdef DEBUG
		// make sure have get memory layout
		while (!kparam.arch_init_flags.memory_layout);
		while (!kparam.arch_init_flags.reload_bsp_arch_page);
	#endif

	memset(&mem_info.page_bitmap, ~0, sizeof(mem_info.page_bitmap));
	init_recurs_lock(&page_alloc_lock);

	phys_addr_t low_bound = 0, high_bound = 0;
	uint64_t pg_start_idx = 0, pg_end_idx = 0;

	int i;
	int j;
	int page_count;
	int zone_count = 0;
	for(i = 0; i < mem_info.mb_memmap_nr; i++)
	{
		multiboot_memory_map_s *mbmap_curr = &mem_info.mb_memmap[i];
		low_bound = (phys_addr_t)mbmap_curr->addr;
		high_bound = (phys_addr_t)(mbmap_curr->len + low_bound);


		pg_start_idx = PAGE_ROUND_UP((uint64_t)low_bound) / PAGE_SIZE;
		pg_end_idx   = PAGE_ROUND_DOWN((uint64_t)high_bound) / PAGE_SIZE;
		if(pg_end_idx <= pg_start_idx)
			continue;

		// set value of memzone_s members
		memzone_s *mz_curr = &mem_info.memzones[zone_count];
		mz_curr->attr				= mem_info.mb_memmap[i].type;
		mz_curr->page_zone			= &mem_info.pages[pg_start_idx];
		mz_curr->page_nr			= pg_end_idx - pg_start_idx;
		mz_curr->page_free_nr		= mz_curr->page_nr;
		mz_curr->zone_start_addr	= (phys_addr_t)(pg_start_idx * PAGE_SIZE);
		mz_curr->zone_end_addr		= (phys_addr_t)(pg_end_idx * PAGE_SIZE);
		mz_curr->zone_size			= mz_curr->zone_end_addr - mz_curr->zone_start_addr;
		page_count 					+= mz_curr->page_nr;

		// set value of Page_s members in current zone and corresponding bit in page bit map
		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			Page_s *pg_curr = &mem_info.pages[j];
			pg_curr->page_start_addr = (phys_addr_t)(j * PAGE_SIZE);
			bm_clear_bit(mem_info.page_bitmap, j);
		}
		zone_count++;
	}
	mem_info.memzone_total_nr	= zone_count;
	mem_info.page_total_nr		= page_count;

	// set kernel used Page_s in right status
	// map physical pages for kernel
	size_t k_phy_pgbase = 0;
	long pde_nr   = PAGE_ROUND_UP(kparam.kernel_vir_end - (virt_addr_t)phys2virt(0)) / PAGE_SIZE;
	for (long i = 0; i < pde_nr; i++)
	{
		unsigned long pg_idx = k_phy_pgbase / PAGE_SIZE;
		// set page struct
		bm_set_bit(mem_info.page_bitmap, pg_idx);
		mem_info.pages[pg_idx].attr = PG_Kernel | PG_Kernel_Init | PG_PTable_Maped;
		mem_info.pages[pg_idx].ref_count++;
		k_phy_pgbase += PAGE_SIZE;
	}
	// set init flag
	kparam.init_flags.page_mm = 1;
}

/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
void init_page()
{
	memset(&pg_list, 0, sizeof(pg_list));

	pg_list.node_spanned_pages = kparam.phys_page_nr;
	pg_list.node_mem_map = memblock_alloc(sizeof(Page_s) * kparam.phys_page_nr, 1);

	memblock_free_all();
}

void memblock_free_pages(Page_s * page, unsigned long pfn,
							unsigned int order)
{
	// add_to_free_list(page, zone, order);
}

/*==============================================================================================*
 *									core fuctions for buddy system								*
 *==============================================================================================*/
Page_s * page_alloc(void)
{
	lock_recurs_lock(&page_alloc_lock);
	unsigned long freepage_idx = bm_get_freebit_idx(mem_info.page_bitmap, 0, mem_info.page_total_nr);
	if (freepage_idx >= mem_info.page_total_nr)
	{
		color_printk(WHITE, RED, "PAGE ALLOC FAILED!\n");
		while (1);
	}
	Page_s * ret_page = &mem_info.pages[freepage_idx];
	bm_set_bit(mem_info.page_bitmap, freepage_idx);
	ret_page->ref_count++;
	unlock_recurs_lock(&page_alloc_lock);
	return ret_page;
}

Page_s * get_page(phys_addr_t paddr)
{
	long pg_idx = PAGE_ROUND_UP((uint64_t)paddr) / PAGE_SIZE;
	return &mem_info.pages[pg_idx];
}

/* Used for pages not on another list */
static inline void add_to_free_list(Page_s * page,
		zone_s * zone, unsigned int order)
{
	struct free_area *area = &zone->free_area[order];

	// list_add(&page->lru, &area->free_list[migratetype]);
	// area->nr_free++;
}

// /* Used for pages not on another list */
// static inline void add_to_free_list_tail(struct page *page, struct zone *zone,
// 					 unsigned int order, int migratetype)
// {
// 	struct free_area *area = &zone->free_area[order];

// 	list_add_tail(&page->lru, &area->free_list[migratetype]);
// 	area->nr_free++;
// }

// /*
//  * Used for pages which are on another list. Move the pages to the tail
//  * of the list - so the moved pages won't immediately be considered for
//  * allocation again (e.g., optimization for memory onlining).
//  */
// static inline void move_to_free_list(struct page *page, struct zone *zone,
// 				     unsigned int order, int migratetype)
// {
// 	struct free_area *area = &zone->free_area[order];

// 	list_move_tail(&page->lru, &area->free_list[migratetype]);
// }

// static inline void del_page_from_free_list(struct page *page, struct zone *zone,
// 					   unsigned int order)
// {
// 	/* clear reported state and update reported page count */
// 	if (page_reported(page))
// 		__ClearPageReported(page);

// 	list_del(&page->lru);
// 	__ClearPageBuddy(page);
// 	set_page_private(page, 0);
// 	zone->free_area[order].nr_free--;
// }

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
	if (!page_is_guard(buddy) && !PageBuddy(buddy))
		return false;

	if (buddy_order(buddy) != order)
		return false;

	/*
	 * zone check is done late to avoid uselessly calculating
	 * zone/node ids for pages that could never merge.
	 */
	if (page_zone_id(page) != page_zone_id(buddy))
		return false;

	return true;
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
static inline void __free_one_page(Page_s *page, unsigned long pfn,
		zone_s *zone, unsigned int order, int migratetype)
{
	struct capture_control *capc = task_capc(zone);
	unsigned long buddy_pfn;
	unsigned long combined_pfn;
	Page_s * buddy;
	bool to_tail;

continue_merging:
	while (order < MAX_ORDER) {
		buddy_pfn = __find_buddy_pfn(pfn, order);
		buddy = page + (buddy_pfn - pfn);

		if (!page_is_buddy(page, buddy, order))
			goto done_merging;
		/*
		 * Our buddy is free or it is CONFIG_DEBUG_PAGEALLOC guard page,
		 * merge with it and move up one order.
		 */
		if (page_is_guard(buddy))
			clear_page_guard(zone, buddy, order, migratetype);
		else
			del_page_from_free_list(buddy, zone, order);
		combined_pfn = buddy_pfn & pfn;
		page = page + (combined_pfn - pfn);
		pfn = combined_pfn;
		order++;
	}
	if (order < MAX_ORDER - 1) {
		/* If we are here, it means order is >= pageblock_order.
		 * We want to prevent merge between freepages on isolate
		 * pageblock and normal pageblock. Without this, pageblock
		 * isolation could cause incorrect freepage or CMA accounting.
		 *
		 * We don't want to hit this code for the more frequent
		 * low-order merging.
		 */
		if (unlikely(has_isolate_pageblock(zone))) {
			int buddy_mt;

			buddy_pfn = __find_buddy_pfn(pfn, order);
			buddy = page + (buddy_pfn - pfn);
			buddy_mt = get_pageblock_migratetype(buddy);

			if (migratetype != buddy_mt
					&& (is_migrate_isolate(migratetype) ||
						is_migrate_isolate(buddy_mt)))
				goto done_merging;
		}
		goto continue_merging;
	}

done_merging:
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
}

static void free_one_page(zone_s *zone,
				Page_s *page, unsigned long pfn,
				unsigned int order,
				int migratetype)
{
	__free_one_page(page, pfn, zone, order, migratetype);
}
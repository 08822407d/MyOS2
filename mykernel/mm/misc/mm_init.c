// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0-only
/*
 * mm_init.c - Memory initialisation verification and debugging
 *
 * Copyright 2008 IBM Corporation, 2008
 * Author Mel Gorman <mel@csn.ul.ie>
 *
 */
#include <linux/kernel/kernel.h>
#include <linux/kernel/export.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/nmi.h>
#include <linux/init/init.h>
#include <linux/kernel/mm.h>


static unsigned long arch_zone_lowest_possible_pfn[MAX_NR_ZONES] __initdata;
static unsigned long arch_zone_highest_possible_pfn[MAX_NR_ZONES] __initdata;

extern char *const zone_names[MAX_NR_ZONES];


static void __meminit
__init_single_page(page_s *page, unsigned long pfn, unsigned long zone)
{
	// mm_zero_struct_page(page);
	// set_page_links(page, zone, nid, pfn);
	init_page_count(page);
	// page_mapcount_reset(page);
	// page_cpupid_reset_last(page);
	// page_kasan_tag_reset(page);

	INIT_LIST_S(&page->lru);
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
			// static void __meminit init_reserved_page(unsigned long pfn)
			// {
				// pg_data_t *pgdat;
				// int nid, zid;

				// if (early_page_initialised(pfn))
				// 	return;

				// nid = early_pfn_to_nid(pfn);
				// pgdat = NODE_DATA(nid);

				// for (zid = 0; zid < MAX_NR_ZONES; zid++) {
				// 	struct zone *zone = &pgdat->node_zones[zid];

				// 	if (zone_spans_pfn(zone, pfn))
				// 		break;
				// }
				__init_single_page(page, start_pfn, 0);
			// }

			/*
			 * no need for atomic set_bit because the struct
			 * page is not visible yet so nobody should
			 * access it yet.
			 */
			__SetPageReserved(page);
		}
	}
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

		if (highest_memmap_pfn < end_pfn - 1)
			highest_memmap_pfn = end_pfn - 1;

		for (int j = 0; j < MAX_ORDER; j++)
			INIT_LIST_HEADER_S(&zone->free_area[j]);

		for (unsigned long pfn = start_pfn; pfn < end_pfn; pfn++)
		{
			page_s *page = pfn_to_page(pfn);
			__init_single_page(page, pfn, i);
		}

		start_pfn = end_pfn;
	}
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

		map = (void *)phys_to_virt(memblock_alloc_range(
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
		INIT_LIST_S(&page->lru);
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



/*
 * Set up kernel memory allocators
 */
void __init mm_core_init(void)
{
	// /* Initializations relying on SMP setup */
	// build_all_zonelists(NULL);
	// page_alloc_init_cpuhp();

	// /*
	//  * page_ext requires contiguous pages,
	//  * bigger than MAX_ORDER unless SPARSEMEM.
	//  */
	// page_ext_init_flatmem();
	// mem_debugging_and_hardening_init();
	// kfence_alloc_pool();
	// report_meminit();
	// kmsan_init_shadow();
	// stack_depot_early_init();
	mem_init();
	// mem_init_print_info();
	kmem_cache_init();
	// /*
	//  * page_owner must be initialized after buddy is ready, and also after
	//  * slab is ready so that stack_depot_init() works properly
	//  */
	// page_ext_init_flatmem_late();
	// kmemleak_init();
	// ptlock_cache_init();
	// pgtable_cache_init();
	// debug_objects_mem_init();
	// vmalloc_init();
	// /* If no deferred init page_ext now, as vmap is fully initialized */
	// if (!deferred_struct_pages)
	// 	page_ext_init();
	// /* Should be run before the first non-init thread is created */
	// init_espfix_bsp();
	// /* Should be run after espfix64 is set up. */
	// pti_init();
	// kmsan_init_runtime();
	mm_cache_init();
}
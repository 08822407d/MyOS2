#ifndef _LINUX_MMZONE_TYPES_H_
#define _LINUX_MMZONE_TYPES_H_

	#include "../mm_type_declaration.h"


	struct zone {
		/* Read-mostly fields */

		// /* zone watermarks, access with *_wmark_pages(zone) macros */
		// unsigned long _watermark[NR_WMARK];
		// unsigned long watermark_boost;

		// unsigned long nr_reserved_highatomic;

		// /*
		// * We don't know if the memory that we're going to allocate will be
		// * freeable or/and it will be released eventually, so to avoid totally
		// * wasting several GB of ram we must reserve some of the lower zone
		// * memory (otherwise we risk to run OOM on the lower zones despite
		// * there being tons of freeable ram on the higher zones).  This array is
		// * recalculated at runtime if the sysctl_lowmem_reserve_ratio sysctl
		// * changes.
		// */
		// long lowmem_reserve[MAX_NR_ZONES];

		pg_data_t		*zone_pgdat;
		// struct per_cpu_pages __percpu *per_cpu_pageset;
		// struct per_cpu_zonestat __percpu *per_cpu_zonestats;
		// /*
		// * the high and batch values are copied to individual pagesets for
		// * faster access
		// */
		// int pageset_high;
		// int pageset_batch;

		// /*
		// * Flags for a pageblock_nr_pages block. See pageblock-flags.h.
		// * In SPARSEMEM, this map is stored in struct mem_section
		// */
		// unsigned long *pageblock_flags;

		/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
		ulong			zone_start_pfn;

		/*
		 * spanned_pages is the total pages spanned by the zone, including
		 * holes, which is calculated as:
		 * 	spanned_pages = zone_end_pfn - zone_start_pfn;
		 *
		 * present_pages is physical pages existing within the zone, which
		 * is calculated as:
		 *	present_pages = spanned_pages - absent_pages(pages in holes);
		 *
		 * present_early_pages is present pages existing within the zone
		 * located on memory available since early boot, excluding hotplugged
		 * memory.
		 *
		 * managed_pages is present pages managed by the buddy system, which
		 * is calculated as (reserved_pages includes pages allocated by the
		 * bootmem allocator):
		 *	managed_pages = present_pages - reserved_pages;
		 *
		 * cma pages is present pages that are assigned for CMA use
		 * (MIGRATE_CMA).
		 *
		 * So present_pages may be used by memory hotplug or memory power
		 * management logic to figure out unmanaged pages by checking
		 * (present_pages - managed_pages). And managed_pages should be used
		 * by page allocator and vm scanner to calculate all kinds of watermarks
		 * and thresholds.
		 *
		 * Locking rules:
		 *
		 * zone_start_pfn and spanned_pages are protected by span_seqlock.
		 * It is a seqlock because it has to be read outside of zone->lock,
		 * and it is done in the main allocator path.  But, it is written
		 * quite infrequently.
		 *
		 * The span_seq lock is declared along with zone->lock because it is
		 * frequently read in proximity to zone->lock.  It's good to
		 * give them a chance of being in the same cacheline.
		 *
		 * Write access to present_pages at runtime should be protected by
		 * mem_hotplug_begin/end(). Any reader who can't tolerant drift of
		 * present_pages should get_online_mems() to get a stable value.
		 */
		atomic_long_t	managed_pages;
		ulong			spanned_pages;
		ulong			present_pages;
	// #ifdef CONFIG_CMA
	// 	unsigned long cma_pages;
	// #endif

		const char		*name;

		int				initialized;

		// /* Write-intensive fields used from the page allocator */
		// ZONE_PADDING(_pad1_)

		/* free areas of different sizes */
		// struct free_area free_area[NR_PAGE_ORDERS];
		List_hdr_s		free_area[NR_PAGE_ORDERS];

		/* zone flags, see below */
		ulong			flags;

		/* Primarily protects free_area */
		spinlock_t		lock;

		// /* Write-intensive fields used by compaction and vmstats. */
		// ZONE_PADDING(_pad2_)

		// /*
		// * When free pages are below this point, additional steps are taken
		// * when reading the number of free pages to avoid per-cpu counter
		// * drift allowing watermarks to be breached
		// */
		// unsigned long percpu_drift_mark;

	// #if defined CONFIG_COMPACTION || defined CONFIG_CMA
	// 	/* pfn where compaction free scanner should start */
	// 	unsigned long compact_cached_free_pfn;
	// 	/* pfn where compaction migration scanner should start */
	// 	unsigned long compact_cached_migrate_pfn[ASYNC_AND_SYNC];
	// 	unsigned long compact_init_migrate_pfn;
	// 	unsigned long compact_init_free_pfn;
	// 	/* Set to true when the PG_migrate_skip bits should be cleared */
	// 	bool compact_blockskip_flush;
	// #endif

		// bool contiguous;

		// ZONE_PADDING(_pad3_)
		// /* Zone statistics */
		// atomic_long_t vm_stat[NR_VM_ZONE_STAT_ITEMS];
		// atomic_long_t vm_numa_event[NR_VM_NUMA_EVENT_ITEMS];
	};

	/*
	 * On NUMA machines, each NUMA node would have a pg_data_t to describe
	 * it's memory layout. On UMA machines there is a single pglist_data which
	 * describes the whole memory.
	 *
	 * Memory statistics and page replacement data structures are maintained on a
	 * per-zone basis.
	 */
	struct pglist_data {
		/*
		 * node_zones contains just the zones for THIS node. Not all of the
		 * zones may be populated, but it is the full list. It is referenced by
		 * this node's node_zonelists as well as other node's node_zonelists.
		 */
		// struct zone node_zones[MAX_NR_ZONES];
		zone_s		node_zones[MAX_NR_ZONES];

		/*
		 * node_zonelists contains references to all zones in all nodes.
		 * Generally the first zones will be references to this node's
		 * node_zones.
		 */
		// struct zonelist node_zonelists[MAX_ZONELISTS];

		int			nr_zones;	  /* number of populated zones in this node */
		// page_s *node_mem_map;
		page_s		*node_mem_map;
	// #ifdef CONFIG_PAGE_EXTENSION
	// 	struct page_ext *node_page_ext;
	// #endif
		ulong		node_start_pfn;
		ulong		node_present_pages; /* total number of physical pages */
		ulong		node_spanned_pages; /* total size of physical page
											range, including holes */
		// int node_id;
		// wait_queue_head_t kswapd_wait;
		// wait_queue_head_t pfmemalloc_wait;

		// /* workqueues for throttling reclaim for different reasons. */
		// wait_queue_head_t reclaim_wait[NR_VMSCAN_THROTTLE];

		// atomic_t nr_writeback_throttled; /* nr of writeback-throttled tasks */
		// unsigned long nr_reclaim_start;	 /* nr pages written while throttled
		// 								* when throttling started. */
		// task_s *kswapd;		 /* Protected by
		// 					mem_hotplug_begin/end() */
		// int kswapd_order;
		// enum zone_type kswapd_highest_zoneidx;

		// int kswapd_failures; /* Number of 'reclaimed == 0' runs */

		/*
		 * This is a per-node reserve of pages that are not available
		 * to userspace allocations.
		 */
		ulong		totalreserve_pages;

		// /* Write-intensive fields used by page reclaim */
		// ZONE_PADDING(_pad1_)

	// #ifdef CONFIG_DEFERRED_STRUCT_PAGE_INIT
	// 	/*
	// 	* If memory initialisation on large machines is deferred then this
	// 	* is the first PFN that needs to be initialised.
	// 	*/
	// 	unsigned long first_deferred_pfn;
	// #endif /* CONFIG_DEFERRED_STRUCT_PAGE_INIT */

	// #ifdef CONFIG_TRANSPARENT_HUGEPAGE
	// 	struct deferred_split deferred_split_queue;
	// #endif

		// /* Fields commonly accessed by the page reclaim scanner */

		// /*
		// * NOTE: THIS IS UNUSED IF MEMCG IS ENABLED.
		// *
		// * Use mem_cgroup_lruvec() to look up lruvecs.
		// */
		// struct lruvec __lruvec;

		ulong		flags;

		// ZONE_PADDING(_pad2_)

		// /* Per-node vmstats */
		// struct per_cpu_nodestat __percpu *per_cpu_nodestats;
		// atomic_long_t vm_stat[NR_VM_NODE_STAT_ITEMS];
	};

#endif /* _LINUX_MMZONE_TYPES_H_ */
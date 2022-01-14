#ifndef _LINUX_MMZONE_H_
#define _LINUX_MMZONE_H_

#include <lib/utils.h>

#define MAX_ORDER 11
#define MAX_NR_ZONES 2 /* __MAX_NR_ZONES */


struct pglist_data;
typedef struct pglist_data pglist_data_s;

typedef struct zone {
	/* Read-mostly fields */

	pglist_data_s	*zone_pgdat;
	// struct per_cpu_pages	__percpu *per_cpu_pageset;
	// struct per_cpu_zonestat	__percpu *per_cpu_zonestats;
	// /*
	//  * the high and batch values are copied to individual pagesets for
	//  * faster access
	//  */
	// int pageset_high;
	// int pageset_batch;

	/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
	unsigned long		zone_start_pfn;

	/*
	 * spanned_pages is the total pages spanned by the zone, including
	 * holes, which is calculated as:
	 * 	spanned_pages = zone_end_pfn - zone_start_pfn;
	 *
	 * present_pages is physical pages existing within the zone, which
	 * is calculated as:
	 *	present_pages = spanned_pages - absent_pages(pages in holes);
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
	// atomic_long_t		managed_pages;
	unsigned long		spanned_pages;
	unsigned long		present_pages;

	const char		*name;

	int initialized;

	/* Write-intensive fields used from the page allocator */

	/* free areas of different sizes */
	List_hdr_s	free_area[MAX_ORDER];

	/* zone flags, see below */
	unsigned long		flags;

	// /* Primarily protects free_area */
	// spinlock_t		lock;

	/* Write-intensive fields used by compaction and vmstats. */

	/*
	 * When free pages are below this point, additional steps are taken
	 * when reading the number of free pages to avoid per-cpu counter
	 * drift allowing watermarks to be breached
	 */
	unsigned long percpu_drift_mark;

	bool			contiguous;

	// /* Zone statistics */
	// atomic_long_t		vm_stat[NR_VM_ZONE_STAT_ITEMS];
	// atomic_long_t		vm_numa_event[NR_VM_NUMA_EVENT_ITEMS];
} zone_s;

typedef struct pglist_data {
	/*
	 * node_zones contains just the zones for THIS node. Not all of the
	 * zones may be populated, but it is the full list. It is referenced by
	 * this node's node_zonelists as well as other node's node_zonelists.
	 */
	zone_s node_zones[MAX_NR_ZONES];

	int nr_zones; /* number of populated zones in this node */
	struct page *node_mem_map;

	unsigned long node_start_pfn;
	unsigned long node_present_pages; /* total number of physical pages */
	unsigned long node_spanned_pages; /* total size of physical page
					     range, including holes */
	int node_id;

	// wait_queue_head_t kswapd_wait;
	// wait_queue_head_t pfmemalloc_wait;
	// struct task_struct *kswapd;	/* Protected by
	// 				   mem_hotplug_begin/end() */
	// int kswapd_order;
	// enum zone_type kswapd_highest_zoneidx;

	// int kswapd_failures;		/* Number of 'reclaimed == 0' runs */

	/*
	 * This is a per-node reserve of pages that are not available
	 * to userspace allocations.
	 */
	unsigned long		totalreserve_pages;

	/* Fields commonly accessed by the page reclaim scanner */

	/*
	 * NOTE: THIS IS UNUSED IF MEMCG IS ENABLED.
	 *
	 * Use mem_cgroup_lruvec() to look up lruvecs.
	 */
	// struct lruvec		__lruvec;

	unsigned long		flags;

	/* Per-node vmstats */
	// struct per_cpu_nodestat __percpu *per_cpu_nodestats;
	// atomic_long_t		vm_stat[NR_VM_NODE_STAT_ITEMS];
} pg_data_s;

#endif /* _LINUX_MMZONE_H_ */
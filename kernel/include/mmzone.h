#ifndef _LINUX_MMZONE_H_
#define _LINUX_MMZONE_H_

#include <lib/utils.h>

#define MAX_ORDER 11


typedef struct zone {
	/* Read-mostly fields */

	struct pglist_data	*zone_pgdat;
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

#endif /* _LINUX_MMZONE_H_ */
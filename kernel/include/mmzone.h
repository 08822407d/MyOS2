#ifndef _LINUX_MMZONE_H_
#define _LINUX_MMZONE_H_

#include <lib/utils.h>

#include <include/page.h>

#include <arch/amd64/include/arch_config.h>

	#define MAX_ORDER 11
	#define MAX_NR_ZONES __MAX_NR_ZONES /* __MAX_NR_ZONES */

	enum zone_type {
		/*
		* ZONE_DMA and ZONE_DMA32 are used when there are peripherals not able
		* to DMA to all of the addressable memory (ZONE_NORMAL).
		* On architectures where this area covers the whole 32 bit address
		* space ZONE_DMA32 is used. ZONE_DMA is left for the ones with smaller
		* DMA addressing constraints. This distinction is important as a 32bit
		* DMA mask is assumed when ZONE_DMA32 is defined. Some 64-bit
		* platforms may need both zones as they support peripherals with
		* different DMA addressing limitations.
		*/
	#ifdef CONFIG_ZONE_DMA
		ZONE_DMA,
	#endif
	#ifdef CONFIG_ZONE_DMA32
		ZONE_DMA32,
	#endif
		/*
		* Normal addressable memory is in ZONE_NORMAL. DMA operations can be
		* performed on pages in ZONE_NORMAL if the DMA devices support
		* transfers to all addressable memory.
		*/
		ZONE_NORMAL,
	#ifdef CONFIG_HIGHMEM
		/*
		* A memory area that is only addressable by the kernel through
		* mapping portions into its own address space. This is for example
		* used by i386 to allow the kernel to address the memory beyond
		* 900MB. The kernel will set up special mappings (page
		* table entries on i386) for each page that the kernel needs to
		* access.
		*/
		ZONE_HIGHMEM,
	#endif
		/*
		* ZONE_MOVABLE is similar to ZONE_NORMAL, except that it contains
		* movable pages with few exceptional cases described below. Main use
		* cases for ZONE_MOVABLE are to make memory offlining/unplug more
		* likely to succeed, and to locally limit unmovable allocations - e.g.,
		* to increase the number of THP/huge pages. Notable special cases are:
		*
		* 1. Pinned pages: (long-term) pinning of movable pages might
		*    essentially turn such pages unmovable. Therefore, we do not allow
		*    pinning long-term pages in ZONE_MOVABLE. When pages are pinned and
		*    faulted, they come from the right zone right away. However, it is
		*    still possible that address space already has pages in
		*    ZONE_MOVABLE at the time when pages are pinned (i.e. user has
		*    touches that memory before pinning). In such case we migrate them
		*    to a different zone. When migration fails - pinning fails.
		* 2. memblock allocations: kernelcore/movablecore setups might create
		*    situations where ZONE_MOVABLE contains unmovable allocations
		*    after boot. Memory offlining and allocations fail early.
		* 3. Memory holes: kernelcore/movablecore setups might create very rare
		*    situations where ZONE_MOVABLE contains memory holes after boot,
		*    for example, if we have sections that are only partially
		*    populated. Memory offlining and allocations fail early.
		* 4. PG_hwpoison pages: while poisoned pages can be skipped during
		*    memory offlining, such pages cannot be allocated.
		* 5. Unmovable PG_offline pages: in paravirtualized environments,
		*    hotplugged memory blocks might only partially be managed by the
		*    buddy (e.g., via XEN-balloon, Hyper-V balloon, virtio-mem). The
		*    parts not manged by the buddy are unmovable PG_offline pages. In
		*    some cases (virtio-mem), such pages can be skipped during
		*    memory offlining, however, cannot be moved/allocated. These
		*    techniques might use alloc_contig_range() to hide previously
		*    exposed pages from the buddy again (e.g., to implement some sort
		*    of memory unplug in virtio-mem).
		* 6. ZERO_PAGE(0), kernelcore/movablecore setups might create
		*    situations where ZERO_PAGE(0) which is allocated differently
		*    on different platforms may end up in a movable zone. ZERO_PAGE(0)
		*    cannot be migrated.
		* 7. Memory-hotplug: when using memmap_on_memory and onlining the
		*    memory to the MOVABLE zone, the vmemmap pages are also placed in
		*    such zone. Such pages cannot be really moved around as they are
		*    self-stored in the range, but they are treated as movable when
		*    the range they describe is about to be offlined.
		*
		* In general, no unmovable allocations that degrade memory offlining
		* should end up in ZONE_MOVABLE. Allocators (like alloc_contig_range())
		* have to expect that migrating pages in ZONE_MOVABLE can fail (even
		* if has_unmovable_pages() states that there are no unmovable pages,
		* there can be false negatives).
		*/
		ZONE_MOVABLE,
	#ifdef CONFIG_ZONE_DEVICE
		ZONE_DEVICE,
	#endif
		__MAX_NR_ZONES

	};

	struct pglist_data;
	typedef struct pglist_data pglist_data_s;

	typedef struct zone {
		/* Read-mostly fields */
		pglist_data_s	*zone_pgdat;

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
		unsigned long		spanned_pages;
		unsigned long		present_pages;
		const char			*name;
		int 				initialized;

		/* free areas of different sizes */
		List_hdr_s			free_area[MAX_ORDER];

		/* zone flags, see below */
		unsigned long		flags;
	} zone_s;

	typedef struct pglist_data {
		/*
		* node_zones contains just the zones for THIS node. Not all of the
		* zones may be populated, but it is the full list. It is referenced by
		* this node's node_zonelists as well as other node's node_zonelists.
		*/
		zone_s			node_zones[MAX_NR_ZONES];

		int				nr_zones; 			/* number of populated zones in this node */
		Page_s *		node_mem_map;

		unsigned long	node_start_pfn;
		unsigned long	node_present_pages; /* total number of physical pages */
		unsigned long	node_spanned_pages; /* total size of physical page
												range, including holes */

		/*
		* This is a per-node reserve of pages that are not available
		* to userspace allocations.
		*/
		unsigned long	totalreserve_pages;

		unsigned long	flags;
	} pg_data_s;

	void zone_sizes_init(void);

	static inline Page_s * get_page_from_free_area(List_hdr_s * area)
	{
		return list_hdr_pop(area)->owner_p;
	}

#endif /* _LINUX_MMZONE_H_ */
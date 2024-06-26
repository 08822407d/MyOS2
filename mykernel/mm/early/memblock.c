// source: linux-6.6.30
#define MEMBLOCK_DEFINATION
#include "memblock.h"

// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Procedures for maintaining information about logical memory blocks.
 *
 * Peter Bergner, IBM Corp.	June 2001.
 * Copyright (C) 2001 Peter Bergner.
 */

/**
 * DOC: memblock overview
 *
 * Memblock is a method of managing memory regions during the early
 * boot period when the usual kernel memory allocators are not up and
 * running.
 *
 * Memblock views the system memory as collections of contiguous
 * regions. There are several types of these collections:
 *
 * * ``memory`` - describes the physical memory available to the
 *   kernel; this may differ from the actual physical memory installed
 *   in the system, for instance when the memory is restricted with
 *   ``mem=`` command line parameter
 * * ``reserved`` - describes the regions that were allocated
 * * ``physmem`` - describes the actual physical memory available during
 *   boot regardless of the possible restrictions and memory hot(un)plug;
 *   the ``physmem`` type is only available on some architectures.
 *
 * Each region is represented by struct memblock_region that
 * defines the region extents, its attributes and NUMA node id on NUMA
 * systems. Every memory type is described by the struct memblock_type
 * which contains an array of memory regions along with
 * the allocator metadata. The "memory" and "reserved" types are nicely
 * wrapped with struct memblock. This structure is statically
 * initialized at build time. The region arrays are initially sized to
 * %INIT_MEMBLOCK_REGIONS for "memory" and %INIT_MEMBLOCK_RESERVED_REGIONS
 * for "reserved". The region array for "physmem" is initially sized to
 * %INIT_PHYSMEM_REGIONS.
 * The memblock_allow_resize() enables automatic resizing of the region
 * arrays during addition of new regions. This feature should be used
 * with care so that memory allocated for the region array will not
 * overlap with areas that should be reserved, for example initrd.
 *
 * The early architecture setup should tell memblock what the physical
 * memory layout is by using simple_mmblk_add() or memblock_add_node()
 * functions. The first function does not assign the region to a NUMA
 * node and it is appropriate for UMA systems. Yet, it is possible to
 * use it on NUMA systems as well and assign the region to a NUMA node
 * later in the setup process using memblock_set_node(). The
 * memblock_add_node() performs such an assignment directly.
 *
 * Once memblock is setup the memory can be allocated using one of the
 * API variants:
 *
 * * memblock_phys_alloc*() - these functions return the **physical**
 *   address of the allocated memory
 * * memblock_alloc*() - these functions return the **virtual** address
 *   of the allocated memory.
 *
 * Note, that both API variants use implicit assumptions about allowed
 * memory ranges and the fallback methods. Consult the documentation
 * of memblock_alloc_internal() and memblock_alloc_range_nid()
 * functions for more elaborate description.
 *
 * As the system boot progresses, the architecture specific mem_init()
 * function frees all the memory to the buddy page allocator.
 *
 * Unless an architecture enables %CONFIG_ARCH_KEEP_MEMBLOCK, the
 * memblock data structures (except "physmem") will be discarded after the
 * system initialization completes.
 */

ulong		max_low_pfn;
ulong		min_low_pfn;
ulong		max_pfn;
ulonglong	max_possible_pfn;

static mmblk_rgn_s mmblk_memory_regions[INIT_MMBLK_MEMORY_REGIONS] __initdata_memblock; 
static mmblk_rgn_s mmblk_reserved_regions[INIT_MMBLK_RESERVED_REGIONS] __initdata_memblock;

memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
	.memory.name		= "memory",

	.reserved.regions	= mmblk_reserved_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MMBLK_RESERVED_REGIONS,
	.reserved.name		= "reserved",

	.bottom_up			= true,
	.current_limit		= (phys_addr_t)MEMBLOCK_ALLOC_ANYWHERE,
};

/*==============================================================================================*
 *									fuctions for add/remove regions								*
 *==============================================================================================*/
static inline phys_addr_t
memblock_cap_size(phys_addr_t base, IN OUT phys_addr_t *size) {
	return *size = min(*size, PHYS_ADDR_MAX - base);
}

/* adjust *@size so that (@base + *@size) doesn't overflow, return new size */
__init_memblock static void
simple_mmblk_remove_rgn(mmblk_type_s *type, ulong rgn_idx) {

	type->total_size -= type->regions[rgn_idx].size;
	memmove(&type->regions[rgn_idx], &type->regions[rgn_idx + 1],
		(type->cnt - (rgn_idx + 1)) * sizeof(type->regions[rgn_idx]));
	type->cnt--;
	
	/*
	 * There is no need to clean array tail
	 * since we have @type->cnt
	 */
}

/**
 * simple_mmblk_merge_rgn - merge neighboring compatible regions
 * @type: memblock type to scan
 *
 * Scan @type and merge neighboring compatible regions.
 */
// static void __init_memblock
// memblock_merge_regions(struct memblock_type *type,
// 		unsigned long start_rgn, unsigned long end_rgn)
__init_memblock static void
simple_mmblk_merge_rgn(IN mmblk_type_s *type,
		ulong start_rgn, ulong end_rgn) {

	int i = 0;
	if (start_rgn)
		i = start_rgn - 1;
	end_rgn = min(end_rgn, type->cnt - 1);
	while (i < end_rgn) {
		mmblk_rgn_s *this = &type->regions[i];
		mmblk_rgn_s *next = &type->regions[i + 1];
		phys_addr_t this_end = this->base + this->size;

		if (this_end != next->base || this->flags != next->flags) {
			BUG_ON(this_end > next->base);
			i++;
			continue;
		}

		this->size += next->size;
		type->total_size += next->size;
		/* move forward from next + 1, index of which is i + 2 */
		simple_mmblk_remove_rgn(type, i + 1);
		end_rgn--;
	}
}

/**
 * memblock_insert_region - insert new memblock region
 * @type:	memblock type to insert into
 * @idx:	index for the insertion point
 * @base:	base address of the new region
 * @size:	size of the new region
 * @flags:	flags of the new region
 *
 * Insert new memblock region [@base, @base + @size) into @type at @idx.
 * @type must already have extra room to accommodate the new region.
 */
// static void __init_memblock memblock_insert_region(
// 		struct memblock_type *type, int idx, phys_addr_t base,
// 		phys_addr_t size, int nid, enum memblock_flags flags)
__init_memblock static void
memblock_insert_region(IN mmblk_type_s *type, int idx,
		phys_addr_t base, phys_addr_t size, enum mmblk_flags flags) {

	mmblk_rgn_s *rgn = &type->regions[idx];

	BUG_ON(type->cnt >= type->max);
	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	type->cnt++;
	type->total_size += size;
}

/**
 * simple_mmblk_add_range - add new memblock region
 * @type: memblock type to add new region into
 * @base: base address of the new region
 * @size: size of the new region
 * @flags: flags of the new region
 *
 * Add new memblock region [@base, @base + @size) into @type.  The new region
 * is allowed to overlap with existing ones - overlaps don't affect already
 * existing regions.  @type is guaranteed to be minimal (all neighbouring
 * compatible regions are merged) after the addition.
 *
 * Return:
 * 0 on success, -errno on failure.
 */
// static int __init_memblock memblock_add_range(
// 		struct memblock_type *type, phys_addr_t base,
// 		phys_addr_t size, int nid, enum memblock_flags flags)
__init static int
simple_mmblk_add_range(IN mmblk_type_s *type, phys_addr_t base,
		phys_addr_t size, enum mmblk_flags flags) {

	int idx, start_rgn, end_rgn;
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;

	/*
	 * This algorism asserts that in each iteration,
	 * @base must be bigger than the end of former rgn
	 * In each itertion we only due with the part of new-region which 
	 * is below @rend of rgn, so the remaining cases(is the same to the 
	 * part which beyond @rend of rgn) will be treated in next iteration.
	 */
	for_each_memblock_type(idx, type, rgn) {
		mmblk_rgn_s *prev = rgn - 1;
		if (idx > 0)
			BUG_ON(base < (prev->base + prev->size));

		phys_addr_t rbase = rgn->base;
		phys_addr_t rend = rbase + rgn->size;
		size_t insert_base, insert_size;
		if ((rend <= base) && (idx != type->cnt))
			continue;
		/*
		 * Now r-end > base, cut the part of new-region which below r-end
		 * and to see weahter the cut part can be merged to this rgn.
		 */
		insert_base = base;
		if ((idx == type->cnt) || (rbase >= end)) {
		/* case1: Scan reach the start of empty records */
		/* case2: ner-region fully below rgn */
			insert_size = end - base;
			base = end;
		}
		else if (rbase < end) {
		/* case3: Head part of new-region may be cut by or merged to rgn */
			insert_size = rbase - base;
			base = rend;
		}
		else {
		/* case4: There is no part of new-region need to be inserted or merged */
			base = rend;
			goto no_update;
		}
		
		memblock_insert_region(type, idx, insert_base, insert_size, flags);
		simple_mmblk_merge_rgn(type, idx - 1, idx);
	
	no_update:
		if (base >= end)	/* if @base meets @end, insertion is finished */
			break;
	}
	BUG_ON(type->cnt >= (type->max - 2));
	return ENOERR;
}

/**
 * simple_mmblk_add - add new memblock region
 * @base: base address of the new region
 * @size: size of the new region
 *
 * Add new memblock region [@base, @base + @size) to the "memory"
 * type. See simple_mmblk_add_range() description for mode details
 *
 * Return:
 * 0 on success, -errno on failure.
 */
// int __init_memblock memblock_add(phys_addr_t base, phys_addr_t size)
__init_memblock int
simple_mmblk_add(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.memory, base, size, 0);
}
// int __init_memblock memblock_reserve(phys_addr_t base, phys_addr_t size)
__init_memblock int
simple_mmblk_reserve(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.reserved, base, size, 0);
}


/*==============================================================================================*
 *									core fuctions for alloc/free								*
 *==============================================================================================*/
static bool
should_skip_region(IN mmblk_type_s *type,
		IN mmblk_rgn_s *m, enum mmblk_flags flags) {

	/* we never skip regions when iterating memblock.reserved or physmem */
	if (type != &memblock.memory)
		return false;

	/* skip hotpluggable memory regions if needed */
	if (!(flags & MEMBLOCK_HOTPLUG) && memblock_is_hotpluggable(m))
		return true;

	/* if we want mirror memory skip non-mirror memory regions */
	if ((flags & MEMBLOCK_MIRROR) && !memblock_is_mirror(m))
		return true;

	/* skip nomap memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_NOMAP) && memblock_is_nomap(m))
		return true;

	/* skip driver-managed memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_DRIVER_MANAGED) && memblock_is_driver_managed(m))
		return true;

	return false;
}
/**
 * __simple_next_mem_range - next function for for_each_free_mem_range() etc.
 * @idx: pointer to uint64_t loop variable
 * @nid: node selector, %NUMA_NO_NODE for all nodes
 * @flags: pick from blocks based on memory attributes
 * @type_a: pointer to memblock_type from where the range is taken
 * @type_b: pointer to memblock_type which excludes memory from being taken
 * @out_start: ptr to phys_addr_t for start address of the range, can be %NULL
 * @out_end: ptr to phys_addr_t for end address of the range, can be %NULL
 * @out_nid: ptr to int for nid of the range, can be %NULL
 *
 * Find the first area from *@idx which matches @nid, fill the out
 * parameters, and update *@idx for the next iteration.  The lower 32bit of
 * *@idx contains index into type_a and the upper 32bit indexes the
 * areas before each region in type_b.	For example, if type_b regions
 * look like the following,
 *
 *	0:[0-16), 1:[32-48), 2:[128-130)
 *
 * The upper 32bit indexes the following regions.
 *
 *	0:[0-0), 1:[16-32), 2:[48-128), 3:[130-MAX)
 *
 * As both region arrays are sorted, the function advances the two indices
 * in lockstep and returns each intersection.
 */
void
__simple_next_mem_range(OUT uint64_t *idx, enum mmblk_flags flags,
		IN mmblk_type_s *within_type, IN mmblk_type_s *exclude_type,
		OUT phys_addr_t *out_start, OUT phys_addr_t *out_end) {

	BUG_ON(within_type == NULL || exclude_type == NULL);

	for (int idx_a = *idx & 0xffffffff, idx_b = *idx >> 32;
		idx_a < within_type->cnt; idx_a++)
	{
		mmblk_rgn_s *m = &within_type->regions[idx_a];
		phys_addr_t m_start = m->base;
		phys_addr_t m_end = m->base + m->size;
		if (should_skip_region(within_type, m, flags))
			continue;

		/* scan areas before each reservation */
		for (; idx_b < exclude_type->cnt + 1; idx_b++)
		{
			mmblk_rgn_s *r;
			// @r_gap_start and @r_gap_end is the start and end of
			// range which between two regions of type reserved.
			phys_addr_t r_gap_start;
			phys_addr_t r_gap_end;

			r = &exclude_type->regions[idx_b];
			r_gap_start = (idx_b == 0) ? 0 : r[-1].base + r[-1].size;
			r_gap_end = (idx_b < exclude_type->cnt) ?
							r->base : (phys_addr_t)PHYS_ADDR_MAX;
			/*
			 * if idx_b advanced past idx_a,
			 * break out to advance idx_a
			 */
			if (m_end <= r_gap_start)
				break;
			// here @m_end > @r_gap_start
			/* if the two regions intersect, we're done */
			if (m_start < r_gap_end)
			{
				if (out_start)
					*out_start = max(m_start, r_gap_start);
				if (out_end)
					*out_end = min(m_end, r_gap_end);
				/*
				 * The region which ends first is
				 * advanced for the next iteration.
				 */
				if (m_end <= r_gap_end)
					idx_a++;
				else
					idx_b++;
				*idx = (uint32_t)idx_a | (uint64_t)idx_b << 32;
				return;
			}
		}
	}

	/* signal end of iteration */
	*idx = ULLONG_MAX;
}

/**
 * __memblock_find_range_bottom_up - find free area utility in bottom-up
 * @start: start of candidate range
 * @end: end of candidate range, can be %MEMBLOCK_ALLOC_ANYWHERE or
 *       %MEMBLOCK_ALLOC_ACCESSIBLE
 * @size: size of free area to find
 * @align: alignment of free area to find
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
 * @flags: pick from blocks based on memory attributes
 *
 * Utility called from memblock_find_in_range_node(), find free area bottom-up.
 *
 * Return:
 * Found address on success, 0 on failure.
 */
__init_memblock static phys_addr_t
__memblock_find_range_bottom_up(phys_addr_t start, phys_addr_t end,
		phys_addr_t size, phys_addr_t align, enum mmblk_flags flags) {

	phys_addr_t this_start, this_end, cand;
	uint64_t i = 0;

	for_each_free_mem_range (i, flags, &this_start, &this_end) {
		this_start = clamp(this_start, start, end);
		this_end = clamp(this_end, start, end);

		cand = (phys_addr_t)round_up((size_t)this_start, (size_t)align);
		if (cand < this_end && this_end - cand >= size)
			return cand;
	}

	return 0;
}

/**
 * memblock_alloc_range_nid - allocate boot memory block
 * @size: size of memory block to be allocated in bytes
 * @align: alignment of the region and block's size
 * @start: the lower bound of the memory region to allocate (phys address)
 * @end: the upper bound of the memory region to allocate (phys address)
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
 * @exact_nid: control the allocation fall back to other nodes
 *
 * The allocation is performed from memory region limited by
 * memblock.current_limit if @end == %MEMBLOCK_ALLOC_ACCESSIBLE.
 *
 * If the specified node can not hold the requested memory and @exact_nid
 * is false, the allocation falls back to any node in the system.
 *
 * For systems with memory mirroring, the allocation is attempted first
 * from the regions with mirroring enabled and then retried from any
 * memory region.
 *
 * In addition, function sets the min_count to 0 using kmemleak_alloc_phys for
 * allocated boot memory block, so that it is never reported as leaks.
 *
 * Return:
 * Physical address of allocated memory block on success, %0 on failure.
 */
// phys_addr_t __init memblock_alloc_range_nid(phys_addr_t size, phys_addr_t align,
// 				phys_addr_t start, phys_addr_t end, int nid, bool exact_nid)
__init phys_addr_t
memblock_alloc_range(phys_addr_t size, phys_addr_t align,
		phys_addr_t start, phys_addr_t end) {

	enum mmblk_flags flags = MEMBLOCK_NONE;
	phys_addr_t found;
	if (align == 0)
		align = SMP_CACHE_BYTES;

	/* pump up @end */
	if (end == MEMBLOCK_ALLOC_ACCESSIBLE ||
		end == MEMBLOCK_ALLOC_NOLEAKTRACE)
		end = memblock.current_limit;

	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);

	found = __memblock_find_range_bottom_up(
				start, end, size, align, flags);
	if (found && !simple_mmblk_reserve(found, size))
		return found;
	else
		return 0;
}

/**
 * memblock_alloc_internal - allocate boot memory block
 * @size: size of memory block to be allocated in bytes
 * @align: alignment of the region and block's size
 * @min_addr: the lower bound of the memory region to allocate (phys address)
 * @max_addr: the upper bound of the memory region to allocate (phys address)
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
 * @exact_nid: control the allocation fall back to other nodes
 *
 * Allocates memory block using memblock_alloc_range_nid() and
 * converts the returned physical address to virtual.
 *
 * The @min_addr limit is dropped if it can not be satisfied and the allocation
 * will fall back to memory below @min_addr. Other constraints, such
 * as node and mirrored memory will be handled again in
 * memblock_alloc_range_nid().
 *
 * Return:
 * Virtual address of allocated memory block on success, NULL on failure.
 */
__init static void
*memblock_alloc_internal(phys_addr_t size, phys_addr_t align,
		phys_addr_t min_addr, phys_addr_t max_addr) {

	phys_addr_t alloc;
	/*
	 * Detect any accidental use of these APIs after slab is ready, as at
	 * this moment memblock may be deinitialized already and its
	 * internal data may be destroyed (after execution of memblock_free_all)
	 */
	if (max_addr > memblock.current_limit)
		max_addr = memblock.current_limit;

	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);

	if (!alloc)
		return NULL;

	return (void *)phys_to_virt(alloc);
}

/**
 * memblock_alloc_try_nid - allocate boot memory block
 * @size: size of memory block to be allocated in bytes
 * @align: alignment of the region and block's size
 * @min_addr: the lower bound of the memory region from where the allocation
 *	  is preferred (phys address)
 * @max_addr: the upper bound of the memory region from where the allocation
 *	      is preferred (phys address), or %MEMBLOCK_ALLOC_ACCESSIBLE to
 *	      allocate only from memory limited by memblock.current_limit value
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
 *
 * Public function, provides additional debug information (including caller
 * info), if enabled. This function zeroes the allocated memory.
 *
 * Return:
 * Virtual address of allocated memory block on success, NULL on failure.
 */
// Linux function proto :
// void *__init memblock_alloc_try_nid(phys_addr_t size, phys_addr_t align,
//			    phys_addr_t min_addr, phys_addr_t max_addr, int nid)
__init void
*memblock_alloc(phys_addr_t size, phys_addr_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MEMBLOCK_LOW_LIMIT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void *
myos_memblock_alloc_DMA32(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MAX_DMA32_PFN << PAGE_SHIFT);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void *
myos_memblock_alloc_DMA(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MEMBLOCK_LOW_LIMIT, MAX_DMA_PFN << PAGE_SHIFT);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void *
myos_memblock_alloc_normal(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

__init_memblock void
memblock_trim_memory(phys_addr_t align) {
	phys_addr_t start, end, orig_start, orig_end;
	mmblk_rgn_s *r;

	for_each_memory_region_topdown(r) {
		orig_start = r->base;
		orig_end = r->base + r->size;
		start = round_up(orig_start, align);
		end = round_down(orig_end, align);

		if (start == orig_start && end == orig_end)
			continue;

		if (start < end) {
			r->base = start;
			r->size = end - start;
		} else {
			simple_mmblk_remove_rgn(&memblock.memory,
					r - memblock.memory.regions);
			r--;
		}
	}
}


/*
 * Common iterator interface used to define for_each_mem_pfn_range().
 */
__init_memblock void
__next_mem_pfn_range(OUT int *idx,
		OUT ulong *out_start_pfn,
		OUT ulong *out_end_pfn) {

	mmblk_type_s *type = &memblock.memory;
	mmblk_rgn_s *r;
	while (++*idx < type->cnt) {
		r = &type->regions[*idx];
		if (PFN_UP(r->base) < PFN_DOWN(r->base + r->size))
			break;;
	}
	if (*idx >= type->cnt) {
		*idx = -1;
		return;
	}

	if (out_start_pfn)
		*out_start_pfn = PFN_UP(r->base);
	if (out_end_pfn)
		*out_end_pfn = PFN_DOWN(r->base + r->size);
}




/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
// Mark all pages in reserved regions "Reserved"
__init static void
memmap_init_reserved_pages(void) {
	mmblk_rgn_s *region;
	phys_addr_t start, end;
	u64 i = 0;

	// /*
	//  * set nid on all reserved pages and also treat struct
	//  * pages for the NOMAP regions as PageReserved
	//  */
	// for_each_memory_region(region) {
	// 	start = region->base;
	// 	end = start + region->size;
	// 	if (memblock_is_nomap(region))
	// 		reserve_bootmem_region(start, end);
	// }

	/* initialize struct pages for the reserved regions */
	for_each_memblock_type(i, &memblock.reserved, region) {
		start = region->base;
		end = start + region->size;
		reserve_bootmem_region(region->base, region->base + region->size);
	}
}

/**
 * memblock_free_all - release free pages to the buddy allocator
 */
__init void
memblock_free_all(void) {
	ulong pages;
	uint64_t i = 0;
	phys_addr_t start, end;

	memmap_init_reserved_pages();

	/*
	 * We need to use NUMA_NO_NODE instead of NODE_DATA(0)->node_id
	 *  because in some case like Node0 doesn't have RAM installed
	 *  low ram will be on Node1
	 */
	for_each_free_mem_range (i, MEMBLOCK_NONE, &start, &end) {
		ulong start_pfn = PFN_UP(start);
		ulong end_pfn = min_t(ulong, PFN_DOWN(end), max_low_pfn);
		if (start_pfn >= end_pfn)
			continue;

		pages += end_pfn - start_pfn;

		// Cut regions to fit page^order bound
		while (start_pfn < end_pfn) {
			int order = min_t(int, MAX_ORDER, __ffs(start_pfn));
			while (start_pfn + (1UL << order) > end_pfn)
				order--;
			memblock_free_pages(pfn_to_page(start_pfn), start_pfn, order);
			start_pfn += (1UL << order);
		}
	}
}
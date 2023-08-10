// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Procedures for maintaining information about logical memory blocks.
 *
 * Peter Bergner, IBM Corp.	June 2001.
 * Copyright (C) 2001 Peter Bergner.
 */

#include <linux/kernel/kernel.h>
#include <linux/kernel/slab.h>
#include <linux/init/init.h>
#include <linux/kernel/bitops.h>
// #include <linux/poison.h>
#include <linux/mm/pfn.h>
// #include <linux/debugfs.h>
// #include <linux/kmemleak.h>
// #include <linux/seq_file.h>
#include <linux/mm/memblock.h>

#include <asm/sections.h>
#include <linux/kernel/io.h>

#include "internal.h"


#include <linux/kernel/asm-generic/bitops.h>


#define INIT_MEMBLOCK_REGIONS			128
#define INIT_PHYSMEM_REGIONS			4

#ifndef INIT_MEMBLOCK_RESERVED_REGIONS
#	define INIT_MEMBLOCK_RESERVED_REGIONS	INIT_MEMBLOCK_REGIONS
#endif

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
 * memory layout is by using memblock_add() or memblock_add_node()
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

unsigned long max_low_pfn;
unsigned long min_low_pfn;
unsigned long max_pfn;
unsigned long long max_possible_pfn;

static mmblk_rgn_s memblock_memory_init_regions[INIT_MEMBLOCK_REGIONS] __initdata_memblock = 
		{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0} };
static mmblk_rgn_s memblock_reserved_init_regions[INIT_MEMBLOCK_RESERVED_REGIONS] __initdata_memblock =
		{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0} };

// static mmblk_rgn_s memblock_memory_init_regions[INIT_MEMBLOCK_REGIONS] __initdata_memblock; 
// static mmblk_rgn_s memblock_reserved_init_regions[INIT_MEMBLOCK_RESERVED_REGIONS] __initdata_memblock;

memblock_s memblock __initdata_memblock = {
	.memory.regions		= memblock_memory_init_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MEMBLOCK_REGIONS,
	.memory.total_size	= 0,
	.memory.name		= "memory",

	.reserved.regions	= memblock_reserved_init_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MEMBLOCK_RESERVED_REGIONS,
	.reserved.total_size= 0,
	.reserved.name		= "reserved",

	.bottom_up			= true,
	.current_limit		= (phys_addr_t)MEMBLOCK_ALLOC_ANYWHERE,
};

/*==============================================================================================*
 *								core fuctions for add/remove regions							*
 *==============================================================================================*/
#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)			\
			for (rgn_idx = 0, rgn_ptr = &memblock_type->regions[0];		\
				rgn_idx < memblock_type->cnt + 1;						\
				rgn_idx++, rgn_ptr = &memblock_type->regions[rgn_idx])

/* adjust *@size so that (@base + *@size) doesn't overflow, return new size */
static inline phys_addr_t
memblock_cap_size(phys_addr_t base, phys_addr_t *size) {
	return *size = min(*size, PHYS_ADDR_MAX - base);
}

/*
 * Address comparison utilities
 */
static unsigned long __init_memblock
memblock_addrs_overlap(phys_addr_t base1, phys_addr_t size1,
		phys_addr_t base2, phys_addr_t size2)
{
	return ((base1 < (base2 + size2)) && (base2 < (base1 + size1)));
}

bool __init_memblock
memblock_overlaps_region(mmblk_type_s *type,
		phys_addr_t base, phys_addr_t size)
{
	unsigned long i;
	memblock_cap_size(base, &size);

	for (i = 0; i < type->cnt; i++)
		if (memblock_addrs_overlap(base, size,
					type->regions[i].base,
					type->regions[i].size))
			break;
	return i < type->cnt;
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
static phys_addr_t __init_memblock
__memblock_find_range_bottom_up(phys_addr_t start,
		phys_addr_t end, phys_addr_t size, phys_addr_t align)
{
	phys_addr_t this_start, this_end, cand;
	uint64_t i;

	for_each_free_mem_range (i, &this_start, &this_end) {
		this_start = clamp(this_start, start, end);
		this_end = clamp(this_end, start, end);

		cand = (phys_addr_t)round_up((size_t)this_start, (size_t)align);
		if (cand < this_end && this_end - cand >= size)
			return cand;
	}

	return 0;
}

/**
 * memblock_find_in_range_node - find free area in given range and node
 * @size: size of free area to find
 * @align: alignment of free area to find
 * @start: start of candidate range
 * @end: end of candidate range, can be %MEMBLOCK_ALLOC_ANYWHERE or
 *       %MEMBLOCK_ALLOC_ACCESSIBLE
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
 * @flags: pick from blocks based on memory attributes
 *
 * Find @size free area aligned to @align in the specified range and node.
 *
 * Return:
 * Found address on success, 0 on failure.
 */
static phys_addr_t __init_memblock
memblock_find_in_range(phys_addr_t size, phys_addr_t align,
		phys_addr_t start, phys_addr_t end)
{
	/* pump up @end */
	if (end == MEMBLOCK_ALLOC_ACCESSIBLE ||
		end == MEMBLOCK_ALLOC_NOLEAKTRACE)
		end = memblock.current_limit;

	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);

	return __memblock_find_range_bottom_up(start, end, size, align);
}

static void __init_memblock
memblock_remove_region(mmblk_type_s *type, unsigned long r)
{
	type->total_size -= type->regions[r].size;
	memmove(&type->regions[r], &type->regions[r + 1],
		(type->cnt - (r + 1)) * sizeof(type->regions[r]));
	type->cnt--;

	/* Special case for empty arrays */
	if (type->cnt == 0)
	{
		// WARN_ON(type->total_size != 0);
		type->cnt = 1;
		type->regions[0].base = 0;
		type->regions[0].size = 0;
		type->regions[0].flags = 0;
	}
}

/**
 * memblock_merge_regions - merge neighboring compatible regions
 * @type: memblock type to scan
 *
 * Scan @type and merge neighboring compatible regions.
 */
static void __init_memblock
memblock_merge_regions(mmblk_type_s *type)
{
	int i = 0;
	/* cnt never goes below 1 */
	while (i < type->cnt - 1)
	{
		mmblk_rgn_s *this = &type->regions[i];
		mmblk_rgn_s *next = &type->regions[i + 1];

		// if (this->base + this->size != next->base ||
		// 	// memblock_get_region_node(this) !=
		// 	// memblock_get_region_node(next) ||
		// 	this->flags != next->flags) {
		// 	BUG_ON(this->base + this->size > next->base);
		// 	i++;
		// 	continue;
		// }
		// this->size += next->size;

		phys_addr_t this_end = this->base + this->size;
		phys_addr_t next_end = next->base + next->size;
		if (this_end < next->base) {
			i++;
			continue;
		}
		if (this_end < next_end)
			this->size = next_end - this->base;
		/* move forward from next + 1, index of which is i + 2 */
		memmove(next, next + 1, (type->cnt - (i + 1)) * sizeof(*next));
		type->cnt--;
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
static void __init_memblock
memblock_insert_region(mmblk_type_s *type, int idx,
		phys_addr_t base, phys_addr_t size, enum mmblk_flags flags)
{
	mmblk_rgn_s *rgn = &type->regions[idx];

	BUG_ON(type->cnt >= type->max);
	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	type->cnt++;
	type->total_size += size;
}

/**
 * memblock_add_range - add new memblock region
 * @type: memblock type to add new region into
 * @base: base address of the new region
 * @size: size of the new region
 * @nid: nid of the new region
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
static int __init
memblock_add_range(mmblk_type_s *type, phys_addr_t base,
		phys_addr_t size, enum mmblk_flags flags)
{
	int idx;
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;
	if (type->cnt + 2 > type->max)
		return -ENOMEM;

	/*
	 * 
	 */
	for_each_memblock_type(idx, type, rgn) {
		phys_addr_t rbase = rgn->base;
		phys_addr_t rend = rbase + rgn->size;
		if (rend <= base)
			continue;

		if (rbase >= end || rgn->size == 0)
		{
			memblock_insert_region(type, idx, base, end - base, flags);
			break;
		}
		if (rbase > base)
		{
			memblock_insert_region(type, idx, base, rbase - base, flags);
			idx++;
			base = min(end, rend);
			if (base = end)
				break;
		}
	}
	while (type->cnt >= type->max - 2);

	memblock_merge_regions(type);
	return ENOERR;
}

/**
 * memblock_add - add new memblock region
 * @base: base address of the new region
 * @size: size of the new region
 *
 * Add new memblock region [@base, @base + @size) to the "memory"
 * type. See memblock_add_range() description for mode details
 *
 * Return:
 * 0 on success, -errno on failure.
 */
int __init_memblock
memblock_add(phys_addr_t base, phys_addr_t size)
{
	return memblock_add_range(&memblock.memory, base, size, 0);
}

int __init_memblock
memblock_reserve(phys_addr_t base, phys_addr_t size)
{
	return memblock_add_range(&memblock.reserved, base, size, 0);
}

/**
 * memblock_isolate_range - isolate given range into disjoint memblocks
 * @type: memblock type to isolate range for
 * @base: base of range to isolate
 * @size: size of range to isolate
 * @start_rgn: out parameter for the start of isolated region
 * @end_rgn: out parameter for the end of isolated region
 *
 * Walk @type and ensure that regions don't cross the boundaries defined by
 * [@base, @base + @size).  Crossing regions are split at the boundaries,
 * which may create at most two more regions.  The index of the first
 * region inside the range is returned in *@start_rgn and end in *@end_rgn.
 *
 * Return:
 * 0 on success, -errno on failure.
 */
static int __init_memblock
memblock_isolate_range(mmblk_type_s *type, phys_addr_t base,
		phys_addr_t size, int *start_rgn, int *end_rgn)
{
	phys_addr_t end = base + memblock_cap_size(base, &size);
	int idx;
	mmblk_rgn_s *rgn;
	*start_rgn = *end_rgn = 0;
	if (!size)
		return 0;
	/* we'll create at most two more regions */
	while (type->cnt >= type->max - 2);

	for_each_memblock_type(idx, type, rgn) {
		phys_addr_t rbase = rgn->base;
		phys_addr_t rend = rbase + rgn->size;
		if (rbase >= end)
			break;
		if (rend <= base)
			continue;

		if (rbase < base)
		{
			/*
			 * @rgn intersects from below.  Split and continue
			 * to process the next region - the new top half.
			 */
			rgn->base = base;
			rgn->size -= base - rbase;
			type->total_size -= base - rbase;
			memblock_insert_region(type, idx,
					rbase, base - rbase, rgn->flags);
		} else if (rend > end) {
			/*
			 * @rgn intersects from above.  Split and redo the
			 * current region - the new bottom half.
			 */
			rgn->base = end;
			rgn->size -= end - rbase;
			type->total_size -= end - rbase;
			memblock_insert_region(type, idx--,
					rbase, end - rbase, rgn->flags);
		} else {
			/* @rgn is fully contained, record it */
			if (!*end_rgn)
				*start_rgn = idx;
			*end_rgn = idx + 1;
		}
	}

	return 0;
}

static int __init_memblock
memblock_remove_range(mmblk_type_s *type,
		phys_addr_t base, phys_addr_t size)
{
	int start_rgn, end_rgn;
	int ret = memblock_isolate_range(type, base, size, &start_rgn, &end_rgn);
	if (ret)
		return ret;

	for (int i = end_rgn - 1; i >= start_rgn; i--)
		memblock_remove_region(type, i);
	return 0;
}

int __init_memblock
memblock_remove(phys_addr_t base, phys_addr_t size)
{
	phys_addr_t end = base + size - 1;
	mmblk_type_s *type = &memblock.memory;

	return memblock_remove_range(type, base, size);
}


/*==============================================================================================*
 *									core fuctions for alloc/free								*
 *==============================================================================================*/
/**
 * __next_mem_range - next function for for_each_free_mem_range() etc.
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
__next_mem_range(uint64_t *idx, mmblk_type_s *type_a, mmblk_type_s *type_b,
		phys_addr_t *out_start, phys_addr_t *out_end)
{
	int idx_a = *idx & 0xffffffff;
	int idx_b = *idx >> 32;

	for (; idx_a < type_a->cnt; idx_a++)
	{
		mmblk_rgn_s *m = &type_a->regions[idx_a];
		phys_addr_t m_start = m->base;
		phys_addr_t m_end = m->base + m->size;
		if (type_a != &memblock.memory)
			continue;

		if (!type_b) {
			if (out_start)
				*out_start = m_start;
			if (out_end)
				*out_end = m_end;
			idx_a++;
			*idx = (uint32_t)idx_a | (uint64_t)idx_b << 32;
			return;
		}

		/* scan areas before each reservation */
		for (; idx_b < type_b->cnt + 1; idx_b++)
		{
			mmblk_rgn_s *r;
			phys_addr_t r_start;
			phys_addr_t r_end;

			r = &type_b->regions[idx_b];
			r_start = idx_b ? r[-1].base + r[-1].size : 0;
			r_end = idx_b < type_b->cnt ? r->base : (phys_addr_t)PHYS_ADDR_MAX;
			/*
			 * if idx_b advanced past idx_a,
			 * break out to advance idx_a
			 */
			if (r_start >= m_end)
				break;
			/* if the two regions intersect, we're done */
			if (m_start < r_end)
			{
				if (out_start)
					*out_start = max(m_start, r_start);
				if (out_end)
					*out_end = min(m_end, r_end);
				/*
				 * The region which ends first is
				 * advanced for the next iteration.
				 */
				if (m_end <= r_end)
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

/*
 * Common iterator interface used to define for_each_mem_pfn_range().
 */
void __init_memblock
__next_mem_pfn_range(int *idx, unsigned long *out_start_pfn,
		unsigned long *out_end_pfn)
{
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
phys_addr_t __init
memblock_alloc_range(phys_addr_t size, phys_addr_t align,
		phys_addr_t start, phys_addr_t end)
{
	phys_addr_t found;

	found = memblock_find_in_range(size, align, start, end);
	if (found && !memblock_reserve(found, size))
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
static void *__init
memblock_alloc_internal(phys_addr_t size, phys_addr_t align,
		phys_addr_t min_addr, phys_addr_t max_addr)
{
	while (align == 0);

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
void *__init
memblock_alloc(phys_addr_t size, phys_addr_t align)
{
	void *ptr = memblock_alloc_internal(size, align,
			MEMBLOCK_LOW_LIMIT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void *myos_memblock_alloc_DMA(size_t size, size_t align)
{
	void *ptr = memblock_alloc_internal(size, align,
			MEMBLOCK_LOW_LIMIT, MAX_DMA_PFN << PAGE_SHIFT);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void *myos_memblock_alloc_normal(size_t size, size_t align)
{
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

/**
 * memblock_free - free boot memory allocation
 * @ptr: starting address of the  boot memory allocation
 * @size: size of the boot memory block in bytes
 *
 * Free boot memory block previously allocated by memblock_alloc_xx() API.
 * The freeing memory will not be released to the buddy allocator.
 */
void __init_memblock
memblock_free(void *ptr, size_t size)
{
	if (ptr)
	{
		phys_addr_t base = virt_to_phys((virt_addr_t)ptr);
	// int __init_memblock memblock_phys_free(phys_addr_t base, phys_addr_t size)
	// {
		phys_addr_t end = base + size - 1;

		// memblock_dbg("%s: [%pa-%pa] %pS\n", __func__,
		// 		&base, &end, (void *)_RET_IP_);

		// kmemleak_free_part_phys(base, size);
		memblock_remove_range(&memblock.reserved, base, size);
	// }
	}
}


/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
void __init_memblock
memblock_trim_memory(phys_addr_t align)
{
	phys_addr_t start, end, orig_start, orig_end;
	mmblk_rgn_s *r;

	for_each_mem_region(r) {
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
			memblock_remove_region(&memblock.memory,
					r - memblock.memory.regions);
			r--;
		}
	}
}

static void __init
memmap_init_reserved_pages(void)
{
	struct memblock_region *region;
	phys_addr_t start, end;
	u64 i;

	/* initialize struct pages for the reserved regions */
	for_each_reserved_mem_range(i, &start, &end)
		reserve_bootmem_region(start, end);

	/* and also treat struct pages for the NOMAP regions as PageReserved */
	for_each_mem_region(region) {
		if (region->flags & MEMBLOCK_NOMAP) {
			start = region->base;
			end = start + region->size;
			reserve_bootmem_region(start, end);
		}
	}
}

static unsigned long __init
free_low_memory_core_early(void)
{
	unsigned long count = 0;
	phys_addr_t start, end;
	uint64_t i;

	memmap_init_reserved_pages();
	/*
	 * We need to use NUMA_NO_NODE instead of NODE_DATA(0)->node_id
	 *  because in some case like Node0 doesn't have RAM installed
	 *  low ram will be on Node1
	 */
	for_each_free_mem_range (i, &start, &end)
	{
	// count += __free_memory_core(start, end);
	// static unsigned long __init __free_memory_core(
	// 		phys_addr_t start, phys_addr_t end)
	// {
		unsigned long start_pfn = PFN_UP(start);
		unsigned long end_pfn = min_t(unsigned long,
				PFN_DOWN(end), max_low_pfn);

		if (start_pfn >= end_pfn)
			continue;

		count += end_pfn - start_pfn;
		// static void __init __free_pages_memory(
		// 		unsigned long start, unsigned long end)
		// {
			int order;
			while (start_pfn < end_pfn) {
				order = min(MAX_ORDER - 1UL, __ffs(start_pfn));
				while (start_pfn + (1UL << order) > end_pfn)
					order--;
				memblock_free_pages(pfn_to_page(start_pfn), start_pfn, order);
				start_pfn += (1UL << order);
			}
		// }
	// }
	}

	return count;
}

/**
 * memblock_free_all - release free pages to the buddy allocator
 */
void __init
memblock_free_all(void)
{
	unsigned long pages;

	// free_unused_memmap();
	// reset_all_zones_managed_pages();

	pages = free_low_memory_core_early();
	// totalram_pages_add(pages);
}
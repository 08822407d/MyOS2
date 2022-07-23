// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Procedures for maintaining information about logical memory blocks.
 *
 * Peter Bergner, IBM Corp.	June 2001.
 * Copyright (C) 2001 Peter Bergner.
 */

// #include <linux/kernel.h>
// #include <linux/slab.h>
#include <linux/init/init.h>
#include <linux/kernel/bitops.h>
// #include <linux/poison.h>
// #include <linux/pfn.h>
// #include <linux/debugfs.h>
// #include <linux/kmemleak.h>
// #include <linux/seq_file.h>
#include <linux/mm/memblock.h>


#include <linux/kernel/types.h>
#include <linux/kernel/minmax.h>
#include <linux/kernel/math.h>
#include <linux/kernel/asm-generic/bitops/__ffs.h>
#include <linux/mm/mm.h>
#include <linux/mm/mmzone.h>
#include <linux/mm/page.h>
#include <linux/lib/string.h>

#include <include/glo.h>

#include <arch/amd64/include/arch_config.h>
#include <arch/amd64/include/arch_proto.h>

#define INIT_MEMBLOCK_REGIONS			128
#define INIT_PHYSMEM_REGIONS			4

#ifndef INIT_MEMBLOCK_RESERVED_REGIONS
#define INIT_MEMBLOCK_RESERVED_REGIONS		INIT_MEMBLOCK_REGIONS
#endif

unsigned long max_low_pfn;
unsigned long min_low_pfn;

static memblock_region_s memblock_memory_init_regions[INIT_MEMBLOCK_REGIONS] = 
	{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0}};
static memblock_region_s memblock_reserved_init_regions[INIT_MEMBLOCK_RESERVED_REGIONS] =
	{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0}};

memblock_s memblock = {
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

	.bottom_up			= false,
	.current_limit		= (phys_addr_t)MEMBLOCK_ALLOC_ANYWHERE,
};


/*==============================================================================================*
 *									core fuctions for buddy system								*
 *==============================================================================================*/
#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)		\
		for (rgn_idx = 0, rgn_ptr = &memblock_type->regions[0];		\
			rgn_idx < memblock_type->cnt + 1;						\
			rgn_idx++, rgn_ptr = &memblock_type->regions[rgn_idx])

/* adjust *@size so that (@base + *@size) doesn't overflow, return new size */
static inline size_t memblock_cap_size(phys_addr_t base, size_t *size)
{
	return *size = min(*size, PHYS_ADDR_MAX - (size_t)base);
}

/*
 * Address comparison utilities
 */
static unsigned long memblock_addrs_overlap(phys_addr_t base1, size_t size1,
				       						phys_addr_t base2, size_t size2)
{
	return ((base1 < (base2 + size2)) && (base2 < (base1 + size1)));
}

bool memblock_overlaps_region(	memblock_type_s *type,
								phys_addr_t base, size_t size)
{
	unsigned long i;

	memblock_cap_size(base, &size);

	for (i = 0; i < type->cnt; i++)
		if (memblock_addrs_overlap(base, size, type->regions[i].base,
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
static phys_addr_t __memblock_find_range_bottom_up(
				phys_addr_t start, phys_addr_t end,
				size_t size, size_t align)
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
static phys_addr_t memblock_find_in_range(
				size_t size, size_t align,
				phys_addr_t start, phys_addr_t end)
{
	/* pump up @end */
	if (end == (phys_addr_t)MEMBLOCK_ALLOC_ACCESSIBLE || end == (phys_addr_t)MEMBLOCK_ALLOC_KASAN)
		end = memblock.current_limit;

	/* avoid allocating the first page */
	// start = max_t(phys_addr_t, start, PAGE_SIZE);
	start = max(start, (phys_addr_t)PAGE_SIZE);
	end = max(start, end);

	return __memblock_find_range_bottom_up(start, end, size, align);
}

/**
 * memblock_merge_regions - merge neighboring compatible regions
 * @type: memblock type to scan
 *
 * Scan @type and merge neighboring compatible regions.
 */
static void memblock_merge_regions(memblock_type_s *type)
{
	int i = 0;

	/* cnt never goes below 1 */
	while (i < type->cnt - 1) {
		memblock_region_s *this = &type->regions[i];
		memblock_region_s *next = &type->regions[i + 1];

		if (this->base + this->size != next->base) {
			// BUG_ON(this->base + this->size > next->base);
			while (this->base + this->size > next->base);
			i++;
			continue;
		}

		this->size += next->size;
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
static void memblock_insert_region(	memblock_type_s *type,
									int idx, phys_addr_t base,
									size_t size,
									enum memblock_flags flags)
{
	memblock_region_s *rgn = &type->regions[idx];

	while(type->cnt >= type->max);

	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	// memblock_set_region_node(rgn, nid);
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
static int memblock_add_range(	memblock_type_s *type,
								phys_addr_t base, size_t size,
								enum memblock_flags flags)
{
	phys_addr_t end = base + memblock_cap_size(base, &size);
	int idx;
	memblock_region_s *rgn;

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
		if (rbase > base) {
			memblock_insert_region(type, idx, base, rbase - base, flags);
			idx++;
			base = min(end, rend);
			if (base = end)
				break;
		}
	}

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
int memblock_add(phys_addr_t base, size_t size)
{
	while (!kparam.init_flags.memblock)
	{
		// only can be used while buddy-system not setup
	}

	return memblock_add_range(&memblock.memory, base, size, 0);
}

int memblock_reserve(phys_addr_t base, size_t size)
{
	while (!kparam.init_flags.memblock)
	{
		// only can be used while buddy-system not setup
	}

	return memblock_add_range(&memblock.reserved, base, size, 0);
}

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
void __next_mem_range(uint64_t *idx, memblock_type_s *type_a, memblock_type_s *type_b,
			  			phys_addr_t *out_start, phys_addr_t *out_end)
{
	int idx_a = *idx & 0xffffffff;
	int idx_b = *idx >> 32;

	for (; idx_a < type_a->cnt; idx_a++) {
		memblock_region_s *m = &type_a->regions[idx_a];

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
		for (; idx_b < type_b->cnt + 1; idx_b++) {
			memblock_region_s *r;
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
			if (m_start < r_end) {
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
phys_addr_t memblock_alloc_range(size_t size, size_t align,
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
static void * memblock_alloc_internal(size_t size, size_t align,
					    phys_addr_t min_addr, phys_addr_t max_addr)
{
	while (!kparam.init_flags.memblock)
	{
		// only can be used while buddy-system not setup
	}
	
	phys_addr_t alloc;

	/*
	 * Detect any accidental use of these APIs after slab is ready, as at
	 * this moment memblock may be deinitialized already and its
	 * internal data may be destroyed (after execution of memblock_free_all)
	 */
	// if (WARN_ON_ONCE(slab_is_available()))
	// 	return kzalloc_node(size, GFP_NOWAIT, nid);

	if (max_addr > memblock.current_limit)
		max_addr = memblock.current_limit;

	alloc = memblock_alloc_range(size, align, min_addr, max_addr);

	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);

	if (!alloc)
		return NULL;

	return phys2virt(alloc);
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
static inline void * memblock_alloc(size_t size, size_t align)
{
	while (align == 0);

	// linux call stack :
	// void *__init memblock_alloc_try_nid(phys_addr_t size, phys_addr_t align,
	//					phys_addr_t min_addr, phys_addr_t max_addr, int nid)
	//								||
	//								\/
	// static void *__init memblock_alloc_internal(phys_addr_t size, phys_addr_t align,
	//					phys_addr_t min_addr, phys_addr_t max_addr, int nid, bool exact_nid)
	//								||
	//								\/
	// phys_addr_t __init memblock_alloc_range_nid(phys_addr_t size, phys_addr_t align,
	//				    phys_addr_t start, phys_addr_t end, int nid, bool exact_nid)
	//								||
	//								\/
	// static phys_addr_t __init_memblock memblock_find_in_range_node(phys_addr_t size,
	//					phys_addr_t align, phys_addr_t start, phys_addr_t end,
	//					int nid, enum memblock_flags flags)
	//								||
	//								\/
	// static phys_addr_t __init_memblock __memblock_find_range_bottom_up(phys_addr_t start,
	//					phys_addr_t end, phys_addr_t size, phys_addr_t align,
	//					int nid, enum memblock_flags flags)
	//								||
	//								\/
	// #define __for_each_mem_range(i, type_a, type_b, nid, flags, p_start, p_end, p_nid)
	//								||
	//								\/
	// void __next_mem_range(uint64_t *idx, int nid, enum memblock_flags flags, struct memblock_type *type_a,
	//	      struct memblock_type *type_b, phys_addr_t *out_start, phys_addr_t *out_end, int *out_nid)

	void *ptr;

	ptr = memblock_alloc_internal(size, align, MEMBLOCK_LOW_LIMIT,
					MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void * memblock_alloc_DMA(size_t size, size_t align)
{
	void *ptr;

	ptr = memblock_alloc_internal(size, align, MEMBLOCK_LOW_LIMIT,
					(phys_addr_t)MAX_DMA_ADDR);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

void * memblock_alloc_normal(size_t size, size_t align)
{
	void *ptr;

	ptr = memblock_alloc_internal(size, align, (phys_addr_t)MAX_DMA_ADDR,
					MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

/*==============================================================================================*
 *								early init fuctions for buddy system							*
 *==============================================================================================*/
static void __free_pages_memory(unsigned long start, unsigned long end)
{
	int order;

	while (start < end) {
		order = min(MAX_ORDER - 1UL, __ffs(start));

		while (start + (1UL << order) > end)
			order--;

		memblock_free_pages(pfn_to_page(start), start, order);

		start += (1UL << order);
	}
}

static unsigned long __free_memory_core(phys_addr_t start, phys_addr_t end)
{
	unsigned long start_pfn = PFN_UP(start);
	// unsigned long end_pfn = min(PFN_DOWN(end), max_low_pfn);
	unsigned long end_pfn = PFN_DOWN(end);

	if (start_pfn >= end_pfn)
		return 0;

	__free_pages_memory(start_pfn, end_pfn);

	return end_pfn - start_pfn;
}

static unsigned long free_low_memory_core_early(void)
{
	unsigned long count = 0;
	phys_addr_t start, end;
	uint64_t i;

	/*
	 * We need to use NUMA_NO_NODE instead of NODE_DATA(0)->node_id
	 *  because in some case like Node0 doesn't have RAM installed
	 *  low ram will be on Node1
	 */
	for_each_free_mem_range (i, &start, &end)
		count += __free_memory_core(start, end);

	return count;
}

/**
 * memblock_free_all - release free pages to the buddy allocator
 */
void memblock_free_all(void)
{
	unsigned long pages;
	pages = free_low_memory_core_early();
	
	kparam.init_flags.memblock = 0;
}
// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Logical memory blocks.
 *
 * Copyright (C) 2001 Peter Bergner, IBM Corp.
 */

#ifndef _LINUX_MEMBLOCK_H_
#define _LINUX_MEMBLOCK_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/kernel/mm.h>
	#include <asm/dma.h>

	/*
	 * highest page
	 */
	extern unsigned long max_pfn;
	/*
	 * highest possible page
	 */
	extern unsigned long long max_possible_pfn;
	extern unsigned long max_low_pfn;
	extern unsigned long min_low_pfn;
	extern memblock_s memblock;


	#ifdef DEBUG

		extern bool
		memblock_is_hotpluggable(mmblk_rgn_s *m);

		extern bool
		memblock_is_mirror(mmblk_rgn_s *m);

		extern bool
		memblock_is_nomap(mmblk_rgn_s *m);

		extern bool
		memblock_is_driver_managed(mmblk_rgn_s *m);
		
		extern __init_memblock bool
		memblock_bottom_up(void);

	#endif

	#if defined(MEMBLOCK_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		bool
		memblock_is_hotpluggable(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_HOTPLUG;
		}

		PREFIX_STATIC_INLINE
		bool
		memblock_is_mirror(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_MIRROR;
		}

		PREFIX_STATIC_INLINE
		bool
		memblock_is_nomap(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_NOMAP;
		}

		PREFIX_STATIC_INLINE
		bool
		memblock_is_driver_managed(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_DRIVER_MANAGED;
		}

		/*
		 * Check if the allocation direction is bottom-up or not.
		 * if this is true, that said, memblock will allocate memory
		 * in bottom-up direction.
		 */
		PREFIX_STATIC_INLINE
		__init_memblock bool
		memblock_bottom_up(void) {
			return memblock.bottom_up;
		}

		PREFIX_STATIC_INLINE
		phys_addr_t
		memblock_cap_size(phys_addr_t base, IN OUT phys_addr_t *size) {
			return *size = min(*size, PHYS_ADDR_MAX - base);
		}

	#endif

	#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)			\
				for (rgn_idx = 0, rgn_ptr = &(memblock_type)->regions[0];	\
					rgn_idx < (memblock_type)->cnt + 1;						\
					rgn_idx++, rgn_ptr = &(memblock_type)->regions[rgn_idx])

	/**
	 * __for_each_mem_range - iterate through memblock areas from type_a and not
	 * included in type_b. Or just type_a if type_b is NULL.
	 * @i: u64 used as loop variable
	 * @type_a: ptr to memblock_type to iterate
	 * @type_b: ptr to memblock_type which excludes from the iteration
	 * @nid: node selector, %NUMA_NO_NODE for all nodes
	 * @flags: pick from blocks based on memory attributes
	 * @p_start: ptr to phys_addr_t for start address of the range, can be %NULL
	 * @p_end: ptr to phys_addr_t for end address of the range, can be %NULL
	 * @p_nid: ptr to int for nid of the range, can be %NULL
	 */
	#define __for_each_mem_range(i, within_type, exclude_type,			\
					flags, p_start, p_end)								\
				for (i = 0, __simple_next_mem_range(&i, flags,			\
					within_type, exclude_type, p_start, p_end);			\
					i != (uint64_t)ULLONG_MAX;							\
					__simple_next_mem_range(&i, flags, within_type,		\
							exclude_type, p_start, p_end))

	/**
	 * for_each_free_mem_range - iterate through free memblock areas
	 * @i: u64 used as loop variable
	 * @nid: node selector, %NUMA_NO_NODE for all nodes
	 * @flags: pick from blocks based on memory attributes
	 * @p_start: ptr to phys_addr_t for start address of the range, can be %NULL
	 * @p_end: ptr to phys_addr_t for end address of the range, can be %NULL
	 * @p_nid: ptr to int for nid of the range, can be %NULL
	 *
	 * Walks over free (memory && !reserved) areas of memblock.  Available as
	 * soon as memblock is initialized.
	 */
	#define for_each_free_mem_range(i, flags, p_start, p_end)			\
				__for_each_mem_range(i, &memblock.memory,				\
						&memblock.reserved, flags, p_start, p_end)

	/**
	 * for_each_mem_pfn_range - early memory pfn range iterator
	 * @i: an integer used as loop variable
	 * @nid: node selector, %MAX_NUMNODES for all nodes
	 * @p_start: ptr to ulong for start pfn of the range, can be %NULL
	 * @p_end: ptr to ulong for end pfn of the range, can be %NULL
	 * @p_nid: ptr to int for nid of the range, can be %NULL
	 *
	 * Walks over configured memory ranges.
	 */
	#define for_each_mem_pfn_range(i, p_start, p_end)					\
				for (i = -1, __next_mem_pfn_range(&i, p_start, p_end);	\
					i >= 0; __next_mem_pfn_range(&i, p_start, p_end))

	/**
	 * for_each_memory_region - itereate over memory regions
	 * @region: loop variable
	 */
	#define for_each_memory_region(region)					\
				for (region = memblock.memory.regions;		\
					region < (memblock.memory.regions +		\
								memblock.memory.cnt);		\
					region++)

	/**
	 * for_each_memory_region - itereate over memory regions topdown
	 * @region: loop variable
	 */
	#define for_each_memory_region_topdown(region)			\
				for (region = &memblock.memory.regions[		\
								memblock.memory.cnt - 1];	\
					region >= memblock.memory.regions;		\
					region--)

#endif /* _LINUX_MEMBLOCK_H_ */
/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _LINUX_MEMBLOCK_H_
#define _LINUX_MEMBLOCK_H_

#include <sys/types.h>

	/*
	* Logical memory blocks.
	*
	* Copyright (C) 2001 Peter Bergner, IBM Corp.
	*/

	// extern unsigned long max_low_pfn;
	// extern unsigned long min_low_pfn;

	/*
	* highest page
	*/
	// extern unsigned long max_pfn;
	/*
	* highest possible page
	*/
	// extern unsigned long long max_possible_pfn;

	/* Flags for memblock allocation APIs */
	#define MEMBLOCK_ALLOC_ANYWHERE		(~0ULL)
	#define MEMBLOCK_ALLOC_ACCESSIBLE	0
	#define MEMBLOCK_ALLOC_KASAN		1

	/* We are using top down, so it is safe to use 0 here */
	#define MEMBLOCK_LOW_LIMIT 0

	#ifndef ARCH_LOW_ADDRESS_LIMIT
	#define ARCH_LOW_ADDRESS_LIMIT  0xffffffffUL
	#endif


	/**
	 * enum memblock_flags - definition of memory region attributes
	 * @MEMBLOCK_NONE: no special request
	 * @MEMBLOCK_HOTPLUG: hotpluggable region
	 * @MEMBLOCK_MIRROR: mirrored region
	 * @MEMBLOCK_NOMAP: don't add to kernel direct mapping and treat as
	 * reserved in the memory map; refer to memblock_mark_nomap() description
	 * for further details
	 */
	enum memblock_flags {
		MEMBLOCK_NONE		= 0x0,	/* No special request */
		MEMBLOCK_HOTPLUG	= 0x1,	/* hotpluggable region */
		MEMBLOCK_MIRROR		= 0x2,	/* mirrored region */
		MEMBLOCK_NOMAP		= 0x4,	/* don't add to kernel direct mapping */
	};

	/**
	 * struct memblock_region - represents a memory region
	 * @base: base address of the region
	 * @size: size of the region
	 * @flags: memory region attributes
	 * @nid: NUMA node id
	 */
	typedef struct memblock_region {
		phys_addr_t	base;
		size_t		size;
		// enum memblock_flags flags;
	// #ifdef CONFIG_NUMA
	// 	int nid;
	// #endif
	} memblock_region_s;

	/**
	 * struct memblock_type - collection of memory regions of certain type
	 * @cnt: number of regions
	 * @max: size of the allocated array
	 * @total_size: size of all regions
	 * @regions: array of regions
	 * @name: the memory type symbolic name
	 */
	typedef struct memblock_type {
		unsigned long	cnt;
		unsigned long	max;
		size_t			total_size;
		memblock_region_s *	regions;
		char *name;
	} memblock_type_s;

	/**
	 * struct memblock - memblock allocator metadata
	 * @bottom_up: is bottom up direction?
	 * @current_limit: physical address of the current allocation limit
	 * @memory: usable memory regions
	 * @reserved: reserved memory regions
	 */
	typedef struct memblock {
		bool bottom_up;  /* is bottom up direction? */
		phys_addr_t current_limit;
		memblock_type_s memory;
		memblock_type_s reserved;
	} memblock_s;

	extern memblock_s memblock;

	int memblock_add(phys_addr_t base, size_t size);
	int memblock_remove(phys_addr_t base, size_t size);
	int memblock_free(phys_addr_t base, size_t size);
	int memblock_reserve(phys_addr_t base, size_t size);
	void * memblock_alloc_try(size_t size, size_t align,
					phys_addr_t min_addr, phys_addr_t max_addr);
	void * memblock_alloc(size_t size, size_t align);
	void memblock_free_all(void);

	/* Low level functions */
	void __next_mem_range(uint64_t *idx, memblock_type_s *type_a, memblock_type_s *type_b,
							phys_addr_t *out_start, phys_addr_t *out_end);

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
	#define __for_each_mem_range(i, type_a, type_b,			\
									p_start, p_end)			\
		for (i = 0, __next_mem_range(&i, type_a, type_b,	\
										p_start, p_end);	\
				i != (uint64_t)ULLONG_MAX;					\
				__next_mem_range(&i, type_a, type_b,		\
									p_start, p_end))

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
	#define for_each_free_mem_range(i, p_start, p_end)			\
		__for_each_mem_range(i, &memblock.memory, &memblock.reserved,	\
								p_start, p_end)


#endif /* _LINUX_MEMBLOCK_H_ */
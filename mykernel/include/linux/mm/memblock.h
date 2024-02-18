// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _LINUX_MEMBLOCK_H_
#define _LINUX_MEMBLOCK_H_

	/*
	 * Logical memory blocks.
	 *
	 * Copyright (C) 2001 Peter Bergner, IBM Corp.
	 */

	#include <linux/init/init.h>
	#include <linux/mm/mm.h>
	#include <asm/dma.h>

	extern unsigned long max_low_pfn;
	extern unsigned long min_low_pfn;

	/*
	 * highest page
	 */
	extern unsigned long max_pfn;
	/*
	 * highest possible page
	 */
	extern unsigned long long max_possible_pfn;

	/**
	 * enum memblock_flags - definition of memory region attributes
	 * @MEMBLOCK_NONE: no special request
	 * @MEMBLOCK_HOTPLUG: memory region indicated in the firmware-provided memory
	 * map during early boot as hot(un)pluggable system RAM (e.g., memory range
	 * that might get hotunplugged later). With "movable_node" set on the kernel
	 * commandline, try keeping this memory region hotunpluggable. Does not apply
	 * to memblocks added ("hotplugged") after early boot.
	 * @MEMBLOCK_MIRROR: mirrored region
	 * @MEMBLOCK_NOMAP: don't add to kernel direct mapping and treat as
	 * reserved in the memory map; refer to memblock_mark_nomap() description
	 * for further details
	 * @MEMBLOCK_DRIVER_MANAGED: memory region that is always detected and added
	 * via a driver, and never indicated in the firmware-provided memory map as
	 * system RAM. This corresponds to IORESOURCE_SYSRAM_DRIVER_MANAGED in the
	 * kernel resource tree.
	 */
	enum mmblk_flags {
		MEMBLOCK_NONE			= 0x0,	/* No special request */
		MEMBLOCK_HOTPLUG		= 0x1,	/* hotpluggable region */
		MEMBLOCK_MIRROR			= 0x2,	/* mirrored region */
		MEMBLOCK_NOMAP			= 0x4,	/* don't add to kernel direct mapping */
		MEMBLOCK_DRIVER_MANAGED	= 0x8,	/* always detected via a driver */
	};

	/**
	 * struct memblock_region - represents a memory region
	 * @base: base address of the region
	 * @size: size of the region
	 * @flags: memory region attributes
	 * @nid: NUMA node id
	 */
	typedef struct memblock_region {
		phys_addr_t			base;
		phys_addr_t			size;
		enum mmblk_flags	flags;
		// int nid;
	} mmblk_rgn_s;

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
		phys_addr_t		total_size;
		mmblk_rgn_s *	regions;
		char			*name;
	} mmblk_type_s;

	/**
	 * struct memblock - memblock allocator metadata
	 * @bottom_up: is bottom up direction?
	 * @current_limit: physical address of the current allocation limit
	 * @memory: usable memory regions
	 * @reserved: reserved memory regions
	 */
	typedef struct memblock {
		bool			bottom_up;  /* is bottom up direction? */
		phys_addr_t		current_limit;
		mmblk_type_s	memory;
		mmblk_type_s	reserved;
	} memblock_s;

	extern memblock_s memblock;

	#define __init_memblock __meminit
	#define __initdata_memblock __meminitdata

	int simple_mmblk_add(phys_addr_t base, phys_addr_t size);
	int simple_mmblk_reserve(phys_addr_t base, phys_addr_t size);
	void memblock_trim_memory(phys_addr_t align);
	void memblock_free_all(void);
	// void memblock_free(void *ptr, size_t size);

	static inline bool
	memblock_is_hotpluggable(mmblk_rgn_s *m) {
		return m->flags & MEMBLOCK_HOTPLUG;
	}

	static inline bool
	memblock_is_mirror(mmblk_rgn_s *m) {
		return m->flags & MEMBLOCK_MIRROR;
	}

	static inline bool
	memblock_is_nomap(mmblk_rgn_s *m) {
		return m->flags & MEMBLOCK_NOMAP;
	}

	static inline bool
	memblock_is_driver_managed(mmblk_rgn_s *m) {
		return m->flags & MEMBLOCK_DRIVER_MANAGED;
	}

	/* Low level functions */
	void
	__simple_next_mem_range(uint64_t *idx, enum mmblk_flags flags,
			mmblk_type_s *type_a, mmblk_type_s *type_b,
			phys_addr_t *out_start, phys_addr_t *out_end);


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


	void __next_mem_pfn_range(int *idx, unsigned long *out_start_pfn,
			unsigned long *out_end_pfn);

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


	/* Flags for memblock allocation APIs */
	#define MEMBLOCK_ALLOC_ANYWHERE			(~(phys_addr_t)0)
	#define MEMBLOCK_ALLOC_ACCESSIBLE		0
	#define MEMBLOCK_ALLOC_NOLEAKTRACE		1

	/* We are using top down, so it is safe to use 0 here */
	#define MEMBLOCK_LOW_LIMIT				0

	#ifndef ARCH_LOW_ADDRESS_LIMIT
	#	define ARCH_LOW_ADDRESS_LIMIT		0xffffffffUL
	#endif

	// phys_addr_t
	// memblock_alloc_range_nid(phys_addr_t size, phys_addr_t align,
	// 		phys_addr_t start, phys_addr_t end, int nid, bool exact_nid);
	// phys_addr_t
	// memblock_phys_alloc_try_nid(phys_addr_t size, phys_addr_t align, int nid);
	phys_addr_t
	memblock_alloc_range(phys_addr_t size, phys_addr_t align,
			phys_addr_t start, phys_addr_t end);

	// static __always_inline void
	// *memblock_alloc(phys_addr_t size, phys_addr_t align)
	void *memblock_alloc(phys_addr_t size, phys_addr_t align);

	/*
	 * Check if the allocation direction is bottom-up or not.
	 * if this is true, that said, memblock will allocate memory
	 * in bottom-up direction.
	 */
	static inline __init_memblock bool
	memblock_bottom_up(void) {
		return memblock.bottom_up;
	}

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


	void *myos_memblock_alloc_DMA(size_t size, size_t align);
	void *myos_memblock_alloc_DMA32(size_t size, size_t align);
	void *myos_memblock_alloc_normal(size_t size, size_t align);

#endif /* _LINUX_MEMBLOCK_H_ */
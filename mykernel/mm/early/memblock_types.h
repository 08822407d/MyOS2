#ifndef _LINUX_MEMBLOCK_TYPES_H_
#define _LINUX_MEMBLOCK_TYPES_H_

	#include "../mm_type_declaration.h"

	#include "memblock_const.h"

	#define __init_memblock __meminit
	#define __initdata_memblock __meminitdata


	/**
	 * struct memblock_region - represents a memory region
	 * @base: base address of the region
	 * @size: size of the region
	 * @flags: memory region attributes
	 * @nid: NUMA node id
	 */
	struct memblock_region {
		phys_addr_t			base;
		phys_addr_t			size;
		enum mmblk_flags	flags;
		// int nid;
	};

	/**
	 * struct memblock_type - collection of memory regions of certain type
	 * @cnt: number of regions
	 * @max: size of the allocated array
	 * @total_size: size of all regions
	 * @regions: array of regions
	 * @name: the memory type symbolic name
	 */
	struct memblock_type {
		ulong			cnt;
		ulong			max;
		phys_addr_t		total_size;
		mmblk_rgn_s *	regions;
		char			*name;
	};

	/**
	 * struct memblock - memblock allocator metadata
	 * @bottom_up: is bottom up direction?
	 * @current_limit: physical address of the current allocation limit
	 * @memory: usable memory regions
	 * @reserved: reserved memory regions
	 */
	struct memblock {
		bool			bottom_up;  /* is bottom up direction? */
		phys_addr_t		current_limit;
		mmblk_type_s	memory;
		mmblk_type_s	reserved;
	};

#endif /* _LINUX_MEMBLOCK_TYPES_H_ */
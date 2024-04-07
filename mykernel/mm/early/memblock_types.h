#ifndef _LINUX_MEMBLOCK_TYPES_H_
#define _LINUX_MEMBLOCK_TYPES_H_

	// #include <linux/kernel/mm_consts.h>

	#define __init_memblock __meminit
	#define __initdata_memblock __meminitdata

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

#endif /* _LINUX_MEMBLOCK_TYPES_H_ */
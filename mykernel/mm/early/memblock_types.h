#ifndef _LINUX_MEMBLOCK_TYPES_H_
#define _LINUX_MEMBLOCK_TYPES_H_

	#include "../mm_type_declaration.h"

	#define __init_memblock __meminit
	#define __initdata_memblock __meminitdata

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
		ulong			cnt;
		ulong			max;
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
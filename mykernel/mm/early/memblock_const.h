#ifndef _LINUX_MEMBLOCK_CONST_H_
#define _LINUX_MEMBLOCK_CONST_H_


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


	/* Flags for memblock allocation APIs */
	#define MEMBLOCK_ALLOC_ANYWHERE			(~(phys_addr_t)0)
	#define MEMBLOCK_ALLOC_ACCESSIBLE		0
	#define MEMBLOCK_ALLOC_NOLEAKTRACE		1

	/* We are using top down, so it is safe to use 0 here */
	#define MEMBLOCK_LOW_LIMIT				0

	#ifndef ARCH_LOW_ADDRESS_LIMIT
	#  define ARCH_LOW_ADDRESS_LIMIT		0xffffffffUL
	#endif

	#define INIT_MEMBLOCK_REGIONS			128
	// #define INIT_PHYSMEM_REGIONS			4

	#ifndef INIT_MMBLK_RESERVED_REGIONS
	#  define INIT_MMBLK_RESERVED_REGIONS	(2 * INIT_MEMBLOCK_REGIONS)
	#endif

	#ifndef INIT_MMBLK_MEMORY_REGIONS
	#  define INIT_MMBLK_MEMORY_REGIONS	INIT_MEMBLOCK_REGIONS
	#endif

#endif /* _LINUX_MEMBLOCK_CONST_H_ */
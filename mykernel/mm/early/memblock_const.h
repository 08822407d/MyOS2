#ifndef _LINUX_MEMBLOCK_CONST_H_
#define _LINUX_MEMBLOCK_CONST_H_


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
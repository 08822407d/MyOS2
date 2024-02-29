#ifndef _LINUX_EARLYMEM_API_H_
#define _LINUX_EARLYMEM_API_H_

	#include "early/memblock_const.h"
	#include "early/memblock_types.h"
	#include "early/memblock.h"

	int simple_mmblk_add(phys_addr_t base, phys_addr_t size);
	int simple_mmblk_reserve(phys_addr_t base, phys_addr_t size);
	void memblock_trim_memory(phys_addr_t align);
	void memblock_free_all(void);
	// void memblock_free(void *ptr, size_t size);

	void __simple_next_mem_range(uint64_t *idx, enum mmblk_flags flags,
			mmblk_type_s *type_a, mmblk_type_s *type_b,
			phys_addr_t *out_start, phys_addr_t *out_end);

	void __next_mem_pfn_range(int *idx, unsigned long *out_start_pfn,
			unsigned long *out_end_pfn);

	phys_addr_t memblock_alloc_range(phys_addr_t size,
			phys_addr_t align, phys_addr_t start, phys_addr_t end);
	void *memblock_alloc(phys_addr_t size, phys_addr_t align);
	void *myos_memblock_alloc_DMA(size_t size, size_t align);
	void *myos_memblock_alloc_DMA32(size_t size, size_t align);
	void *myos_memblock_alloc_normal(size_t size, size_t align);

	void reserve_bootmem_region(phys_addr_t start, phys_addr_t end);

#endif /* _LINUX_EARLYMEM_API_H_ */
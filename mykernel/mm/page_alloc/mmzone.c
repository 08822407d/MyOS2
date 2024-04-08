#include <asm/dma.h>

#include "buddy.h"

void __init zone_sizes_init(void)
{
	pg_data_t *nodes = myos_memblock_alloc_normal(
			MAX_NUMNODES *sizeof(pg_data_t), SMP_CACHE_BYTES);
	for (int i = 0; i < MAX_NUMNODES; i++)
		NODE_DATA(i) = nodes + i;

	unsigned long max_zone_pfns[MAX_NR_ZONES];
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;

	free_area_init(max_zone_pfns);
}
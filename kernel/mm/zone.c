#include <string.h>
#include <stddef.h>

#include <include/memblock.h>
#include <include/mmzone.h>

#include <arch/amd64/include/arch_config.h>

static char * const zone_names[MAX_NR_ZONES] = {
#ifdef CONFIG_ZONE_DMA
	 "DMA",
#endif
#ifdef CONFIG_ZONE_DMA32
	 "DMA32",
#endif
	 "Normal",
#ifdef CONFIG_HIGHMEM
	 "HighMem",
#endif
	 "Movable",
#ifdef CONFIG_ZONE_DEVICE
	 "Device",
#endif
};


extern pglist_data_s	pg_list;

/**
 * free_area_init - Initialise all pg_data_t and zone data
 * @max_zone_pfn: an array of max PFNs for each zone
 *
 * This will call free_area_init_node() for each active node in the system.
 * Using the page ranges provided by memblock_set_node(), the size of each
 * zone in each node and their holes is calculated. If the maximum PFN
 * between two adjacent zones match, it is assumed that the zone is empty.
 * For example, if arch_max_dma_pfn == arch_max_dma32_pfn, it is assumed
 * that arch_max_dma32_pfn has no pages. It is also assumed that a zone
 * starts where the previous one ended. For example, ZONE_DMA32 starts
 * at arch_max_dma_pfn.
 */
static void free_area_init(unsigned long *max_zone_pfn)
{
	unsigned long start_pfn, end_pfn;
	start_pfn = 1;

	for (int i = 0; i < MAX_NR_ZONES; i++) {
		end_pfn = max(max_zone_pfn[i], start_pfn);
		zone_s * zone = &pg_list.node_zones[i];
		if (i == ZONE_MOVABLE)
			continue;

		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
		zone->name = zone_names[i];
		zone->zone_pgdat = &pg_list;

		for (int j = 0; j < MAX_ORDER; j++)
			list_hdr_init(&zone->free_area[j]);

		start_pfn = end_pfn;
	}
}

void zone_sizes_init(void)
{
	unsigned long max_zone_pfns[MAX_NR_ZONES];

	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

#ifdef CONFIG_ZONE_DMA
	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
#endif
#ifdef CONFIG_ZONE_DMA32
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
#endif
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;
#ifdef CONFIG_HIGHMEM
	max_zone_pfns[ZONE_HIGHMEM]	= max_pfn;
#endif

	free_area_init(max_zone_pfns);
}
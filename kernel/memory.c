#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_proto.h"
#include "include/ktypes.h"
#include "include/const.h"
#include "proto.h"

memory_info_s		mem_info;
multiboot_memory_map_s	mem_distribution[MAXMEMZONE];

void mem_init()
{
	memset(&mem_info.page_bitmap, ~0, sizeof(mem_info.page_bitmap));

	phy_addr low_bound = 0, high_bound = 0;
	uint64_t pg_start_idx = 0, pg_end_idx = 0;

	int i;
	int j;
	int page_count;
	int zone_count = 0;
	for(i = 0; i < mem_info.mb_memmap_nr; i++)
	{
		multiboot_memory_map_s *mbmap_curr = &mem_info.mb_memmap[i];
		low_bound = (phy_addr)mbmap_curr->addr;
		high_bound = (phy_addr)(mbmap_curr->len + low_bound);


		pg_start_idx = CONFIG_PAGE_ALIGH((uint64_t)low_bound) / CONFIG_PAGE_SIZE;
		pg_end_idx   = CONFIG_PAGE_MASKF((uint64_t)high_bound) / CONFIG_PAGE_SIZE;
		if(pg_end_idx <= pg_start_idx)
			continue;

		memzone_s *mz_curr = &mem_info.memzones[zone_count];
		mz_curr->attr				= mem_info.mb_memmap[i].type;
		mz_curr->page_zone			= &mem_info.pages[pg_start_idx];
		mz_curr->page_nr			= pg_end_idx - pg_start_idx;
		mz_curr->page_free_nr		= mz_curr->page_nr;
		mz_curr->zone_start_addr	= (phy_addr)(pg_start_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_end_addr		= (phy_addr)(pg_end_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_size			= mz_curr->zone_end_addr - mz_curr->zone_start_addr;
		page_count 					+= mz_curr->page_nr;

		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			page_s *pg_curr = &mem_info.pages[j];
			pg_curr->zone_belonged	= mz_curr;
			pg_curr->page_start_addr = (phy_addr)(j * CONFIG_PAGE_SIZE);
			mem_info.page_bitmap[page_count / BITMAP_UNITSIZE] &=
									~(1UL << (j % BITMAP_UNITSIZE));
		}

		zone_count++;
	}

	mem_info.memzone_total_nr	= zone_count;
	mem_info.page_total_nr		= page_count;
}
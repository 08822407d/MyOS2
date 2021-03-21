#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_proto.h"
#include "include/ktypes.h"

memory_info_s		mem_info;
multiboot_memory_map_s	mem_distribution[MAXMEMZONE];

void mem_init()
{
	int i;
	for(i = 0; i < mem_info.mb_memmap_nr; i++)
	{
		phy_addr low_bound = mem_info.mb_memmap[i].addr;
		phy_addr high_bound = mem_info.mb_memmap[i].len + low_bound;
		uint64_t pg_start_idx = 0, pg_end_idx = 0;

		if(mem_info.mb_memmap[i].type == 1)
		{
			pg_start_idx = (low_bound + CONFIG_PAGE_SIZE - 1) / CONFIG_PAGE_SIZE ;
			pg_end_idx   = high_bound / CONFIG_PAGE_SIZE;
		}
		else
		{
			pg_start_idx = low_bound / CONFIG_PAGE_SIZE;
			pg_end_idx   = (high_bound + CONFIG_PAGE_SIZE - 1) / CONFIG_PAGE_SIZE;
		}
		mem_info.memzones[i].attr = mem_info.mb_memmap[i].type;
		mem_info.memzones[i].zone_start_addr = low_bound;
		mem_info.memzones[i].zone_end_addr = high_bound;
		mem_info.memzones[i].page_group = &mem_info.pages[pg_start_idx];
		mem_info.memzones[i].page_nr = pg_end_idx - pg_start_idx + 1;

		int j;
		for(j = pg_start_idx; j <= pg_end_idx; j++)
		{
			mem_info.pages[j].zone_belonged = &mem_info.memzones[i];
			mem_info.pages[i].page_start_addr = (phy_addr)(i * CONFIG_PAGE_SIZE);
		}
	}
}
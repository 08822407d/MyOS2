#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>
#include <lib/utils.h>

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_proto.h"

#include "include/param.h"
#include "include/ktypes.h"
#include "include/const.h"
#include "include/proto.h"
#include "include/slab.h"

extern kinfo_s kparam;

memory_info_s		mem_info;
multiboot_memory_map_s	mem_distribution[MAXMEMZONE];

Slab_Cache_s slab_cache_groups[SLAB_LEVEL];

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

		// set value of memzone_s members
		memzone_s *mz_curr = &mem_info.memzones[zone_count];
		mz_curr->attr				= mem_info.mb_memmap[i].type;
		mz_curr->page_zone			= &mem_info.pages[pg_start_idx];
		mz_curr->page_nr			= pg_end_idx - pg_start_idx;
		mz_curr->page_free_nr		= mz_curr->page_nr;
		mz_curr->zone_start_addr	= (phy_addr)(pg_start_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_end_addr		= (phy_addr)(pg_end_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_size			= mz_curr->zone_end_addr - mz_curr->zone_start_addr;
		page_count 					+= mz_curr->page_nr;

		// set value of page_s members in current zone and corresponding bit in page bit map
		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			page_s *pg_curr = &mem_info.pages[j];
			pg_curr->zone_belonged	= mz_curr;
			pg_curr->page_start_addr = (phy_addr)(j * CONFIG_PAGE_SIZE);
			bm_clear_bit(mem_info.page_bitmap, j);

			// mem_info.page_bitmap[page_count / BITMAP_UNITSIZE] &=
			// 						~(1UL << (j % BITMAP_UNITSIZE));
		}

		zone_count++;
	}

	mem_info.memzone_total_nr	= zone_count;
	mem_info.page_total_nr		= page_count;

	// set kernel used page_s in right status
	// map physical pages for kernel
	phy_addr k_phy_pgbase = (phy_addr)CONFIG_PAGE_MASKF((uint64_t)kparam.kernel_phy_base);
	vir_addr k_vir_pgbase = (vir_addr)CONFIG_PAGE_MASKF((uint64_t)kparam.kernel_vir_base);
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_size) / CONFIG_PAGE_SIZE;
	pde_nr++;
	for (long i = 0; i < pde_nr; i++)
	{
		unsigned long pg_idx = (unsigned long)k_phy_pgbase / CONFIG_PAGE_SIZE;
		// map lower mem
		pg_domap(k_phy_pgbase, k_phy_pgbase, 0);
		// map higher mem
		pg_domap(k_vir_pgbase, k_phy_pgbase, 0);
		// set page struct
		bm_set_bit(mem_info.page_bitmap, pg_idx);
		mem_info.pages[pg_idx].attr = PG_Kernel | PG_Kernel_Init | PG_PTable_Maped;
		mem_info.pages[pg_idx].ref_count++;
		k_vir_pgbase += CONFIG_PAGE_SIZE;
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
}

page_s * page_alloc(void)
{
	unsigned long freepage_idx = bm_get_freebit_idx(mem_info.page_bitmap, mem_info.page_total_nr);
	bm_set_bit(mem_info.page_bitmap, freepage_idx);
	page_s * ret_page = &mem_info.pages[freepage_idx];
	ret_page->ref_count++;
	return ret_page;
}

void page_ref(page_s * page)
{

}

void page_free(page_s * page)
{
	unsigned long page_idx = (unsigned long)page->page_start_addr / CONFIG_PAGE_SIZE;
	bm_clear_bit(mem_info.page_bitmap, page_idx);
	page->attr = 0;
	page->ref_count = 0;

	page->zone_belonged->page_total_ref--;
	page->zone_belonged->page_free_nr++;
}
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>

#include <string.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/mutex.h>

#include <include/glo.h>
#include <include/memory.h>
#include <include/ktypes.h>
#include <include/printk.h>
#include <include/mmzone.h>

memory_info_s	mem_info;
recurs_lock_T	page_alloc_lock;

/*==============================================================================================*
 *								fuction relate to physical page									*
 *==============================================================================================*/
void init_page_manage()
{	
	#ifdef DEBUG
		// make sure have get memory layout
		while (!kparam.arch_init_flags.memory_layout);
		while (!kparam.arch_init_flags.reload_bsp_arch_page);
	#endif

	memset(&mem_info.page_bitmap, ~0, sizeof(mem_info.page_bitmap));
	init_recurs_lock(&page_alloc_lock);

	phys_addr_t low_bound = 0, high_bound = 0;
	uint64_t pg_start_idx = 0, pg_end_idx = 0;

	int i;
	int j;
	int page_count;
	int zone_count = 0;
	for(i = 0; i < mem_info.mb_memmap_nr; i++)
	{
		multiboot_memory_map_s *mbmap_curr = &mem_info.mb_memmap[i];
		low_bound = (phys_addr_t)mbmap_curr->addr;
		high_bound = (phys_addr_t)(mbmap_curr->len + low_bound);


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
		mz_curr->zone_start_addr	= (phys_addr_t)(pg_start_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_end_addr		= (phys_addr_t)(pg_end_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_size			= mz_curr->zone_end_addr - mz_curr->zone_start_addr;
		page_count 					+= mz_curr->page_nr;

		// set value of Page_s members in current zone and corresponding bit in page bit map
		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			Page_s *pg_curr = &mem_info.pages[j];
			pg_curr->zone_belonged	= mz_curr;
			pg_curr->page_start_addr = (phys_addr_t)(j * CONFIG_PAGE_SIZE);
			bm_clear_bit(mem_info.page_bitmap, j);
		}
		zone_count++;
	}
	mem_info.memzone_total_nr	= zone_count;
	mem_info.page_total_nr		= page_count;

	// set kernel used Page_s in right status
	// map physical pages for kernel
	size_t k_phy_pgbase = 0;
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_vir_end - (virt_addr_t)phys2virt(0)) / CONFIG_PAGE_SIZE;
	for (long i = 0; i < pde_nr; i++)
	{
		unsigned long pg_idx = k_phy_pgbase / CONFIG_PAGE_SIZE;
		// set page struct
		bm_set_bit(mem_info.page_bitmap, pg_idx);
		mem_info.pages[pg_idx].attr = PG_Kernel | PG_Kernel_Init | PG_PTable_Maped;
		mem_info.pages[pg_idx].ref_count++;
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
	// set init flag
	kparam.init_flags.page_mm = 1;
}

void init_page()
{

}

Page_s * page_alloc(void)
{
	lock_recurs_lock(&page_alloc_lock);
	unsigned long freepage_idx = bm_get_freebit_idx(mem_info.page_bitmap, 0, mem_info.page_total_nr);
	if (freepage_idx >= mem_info.page_total_nr)
	{
		color_printk(WHITE, RED, "PAGE ALLOC FAILED!\n");
		while (1);
	}
	Page_s * ret_page = &mem_info.pages[freepage_idx];
	bm_set_bit(mem_info.page_bitmap, freepage_idx);
	ret_page->ref_count++;
	unlock_recurs_lock(&page_alloc_lock);
	return ret_page;
}

void page_free(Page_s * page_p)
{
	lock_recurs_lock(&page_alloc_lock);
	unsigned long page_idx = (unsigned long)page_p->page_start_addr / CONFIG_PAGE_SIZE;
	bm_clear_bit(mem_info.page_bitmap, page_idx);
	page_p->attr = 0;
	page_p->ref_count = 0;

	page_p->zone_belonged->page_total_ref--;
	page_p->zone_belonged->page_free_nr++;
	unlock_recurs_lock(&page_alloc_lock);
}

Page_s * get_page(phys_addr_t paddr)
{
	long pg_idx = CONFIG_PAGE_ALIGH((uint64_t)paddr) / CONFIG_PAGE_SIZE;
	return &mem_info.pages[pg_idx];
}
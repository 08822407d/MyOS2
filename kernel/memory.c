#include <sys/types.h>
#include <sys/cdefs.h>
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
#include "include/printk.h"

extern kinfo_s kparam;

memory_info_s		mem_info;
multiboot_memory_map_s	mem_distribution[MAXMEMZONE];

Slab_Cache_s	slab_cache_groups[SLAB_LEVEL];
Slab_s			base_slabs[SLAB_LEVEL];
uint8_t			base_slab_page[SLAB_LEVEL][CONFIG_PAGE_SIZE] __aligned(CONFIG_PAGE_SIZE);

/*===========================================================================*
 *						fuction relate to physical page						 *
 *===========================================================================*/

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

		// set value of Page_s members in current zone and corresponding bit in page bit map
		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			Page_s *pg_curr = &mem_info.pages[j];
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

	// set kernel used Page_s in right status
	// map physical pages for kernel
	phy_addr k_phy_pgbase = 0;
	vir_addr k_vir_pgbase = (vir_addr)phy2vir(0);
	uint64_t page_attr = ARCH_PG_PRESENT | ARCH_PG_USER | ARCH_PG_RW;
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_vir_end - k_vir_pgbase) / CONFIG_PAGE_SIZE;
	for (long i = 0; i < pde_nr; i++)
	{
		unsigned long pg_idx = (unsigned long)k_phy_pgbase / CONFIG_PAGE_SIZE;
		// map lower mem
		pg_domap(k_phy_pgbase, k_phy_pgbase, page_attr);
		// map higher mem
		pg_domap(k_vir_pgbase, k_phy_pgbase, page_attr);
		// set page struct
		bm_set_bit(mem_info.page_bitmap, pg_idx);
		mem_info.pages[pg_idx].attr = PG_Kernel | PG_Kernel_Init | PG_PTable_Maped;
		mem_info.pages[pg_idx].ref_count++;
		k_vir_pgbase += CONFIG_PAGE_SIZE;
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
}

Page_s * page_alloc(void)
{
	unsigned long freepage_idx = bm_get_freebit_idx(mem_info.page_bitmap, 0, mem_info.page_total_nr);
	bm_set_bit(mem_info.page_bitmap, freepage_idx);
	Page_s * ret_page = &mem_info.pages[freepage_idx];
	ret_page->ref_count++;
	return ret_page;
}

void page_free(Page_s * page_p)
{
	unsigned long page_idx = (unsigned long)page_p->page_start_addr / CONFIG_PAGE_SIZE;
	bm_clear_bit(mem_info.page_bitmap, page_idx);
	page_p->attr = 0;
	page_p->ref_count = 0;

	page_p->zone_belonged->page_total_ref--;
	page_p->zone_belonged->page_free_nr++;
}

/*===========================================================================*
 *					fuction relate to alloc virtual memory					 *
 *===========================================================================*/

void slab_init()
{
	for (int i = 0; i < SLAB_LEVEL; i++)
	{
		Slab_Cache_s * scgp = &slab_cache_groups[i];
		Slab_s * bsp = &base_slabs[i];

		scgp->obj_size = SLAB_SIZE_BASE << i;
		unsigned long obj_nr = CONFIG_PAGE_SIZE / scgp->obj_size;
		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
		// init 3 status of slab list
		scgp->normal_slab = bsp;
		scgp->nslab_count = 1;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;

		scgp->dma_slab = NULL;
		scgp->dslab_count = 0;
		scgp->dsobj_free_count = 0;
		scgp->dsobj_used_count = 0;

		// init the basic slab
		list_init(&bsp->slab_list);
		bsp->colormap = (bitmap_t *)(kparam.kernel_vir_end + 0x10);
		kparam.kernel_vir_end += cm_size + 0x10;
		bsp->slabcache_ptr = scgp;
		bsp->free =
		bsp->total = obj_nr;
		bsp->vir_addr = (vir_addr)base_slab_page[i];
		bsp->page->attr |= PG_Slab;
		int pg_idx = (uint64_t)vir2phy(bsp->vir_addr) / CONFIG_PAGE_SIZE;
		bsp->page = &mem_info.pages[pg_idx];
		bsp->page->slab_ptr = bsp;
	}
	kparam.kernel_vir_end += 0x10;
}

Slab_s * slab_alloc(size_t obj_count)
{
	Page_s * pg = page_alloc();
	if (!(pg->attr & PG_PTable_Maped))
		pg_domap(phy2vir(pg->page_start_addr), pg->page_start_addr, 0);
	
	pg->attr |= PG_Slab;
	Slab_s * sp = (Slab_s *)kmalloc(sizeof(Slab_s));
	sp->page = pg;
	pg->slab_ptr = sp;
	sp->colormap = (bitmap_t *)kmalloc(obj_count / sizeof(bitmap_t));
	sp->total =
	sp->free = obj_count;
	sp->vir_addr = phy2vir(pg->page_start_addr);
	list_init(&sp->slab_list);

	return sp;
}

void slab_free(Slab_s * slp)
{
	kfree(slp->colormap);
	slp->page->attr &= ~PG_Slab;
	slp->page->slab_ptr = NULL;
	list_delete(&slp->slab_list);

	kfree(slp);
}

void * kmalloc(size_t size)
{
	if (size > CONST_1M)
	{
		color_printk(RED, WHITE, "Mem required too big\n");
		while (1);
	}

	void * ret_val = NULL;
	// find suitable slab group
	size--;
	size /= SLAB_SIZE_BASE;
	int sc_idx = 0;
	if (size > 0)
	{
		sc_idx++;
		while(size >> sc_idx)
		{
			sc_idx++;
		}
	}

	// find a usable slab
	Slab_Cache_s * scgp = &slab_cache_groups[sc_idx];
	Slab_s * slp = scgp->normal_slab;
	while (slp->free == 0)
	{
		List_s * next = list_get_next(&slp->slab_list);
		slp = container_of(&slp->slab_list, Slab_s, slab_list);
	}
	// count the virtual addr of suitable object
	unsigned long obj_idx = bm_get_freebit_idx(slp->colormap, 0, slp->total);
	if (obj_idx < slp->total)
	{
		size_t offset = scgp->obj_size * obj_idx;
		ret_val = (void *)((size_t)slp->vir_addr + offset);
		// refresh status of slab
		slp->free--;
		scgp->nsobj_free_count--;
		scgp->nsobj_used_count++;
		bm_set_bit(slp->colormap, obj_idx);
		// assure usable memory more than one page
		if (scgp->nsobj_free_count < slp->total)
		{
			scgp->nsobj_free_count += slp->total;
			Slab_s * new_slab = slab_alloc(slp->total);
			if (new_slab == NULL)
			{
				color_printk(RED, WHITE, "Alloc new slab failed!\n");
				scgp->nsobj_free_count -= slp->total;
				while (1);
			}
			new_slab->slabcache_ptr = scgp;
			list_insert_back(&new_slab->slab_list, &scgp->normal_slab->slab_list);
			scgp->nslab_count++;
		}
	}

	return ret_val;
}

void kfree(void * obj_p)
{
	if (obj_p == NULL)
		return;

	phy_addr pg_addr = vir2phy((vir_addr)CONFIG_PAGE_MASKF((size_t)obj_p));
	unsigned long pg_idx = (size_t)pg_addr / CONFIG_PAGE_SIZE;
	Page_s * pgp = &mem_info.pages[pg_idx];
	Slab_s * slp = pgp->slab_ptr;
	Slab_Cache_s * scgp = slp->slabcache_ptr;
	unsigned long obj_idx = (obj_p - slp->vir_addr) / scgp->obj_size;
	bm_clear_bit(slp->colormap, obj_idx);
	slp->free++;
	scgp->nsobj_free_count++;
	scgp->nsobj_used_count--;
	if ((slp->free == slp->total) &&
		(scgp->nsobj_free_count >= (slp->total * 3)) &&
		(slp != scgp->normal_slab))
	{
		slab_free(slp);
		scgp->nslab_count--;
		scgp->nsobj_free_count -= slp->total;
	}
}
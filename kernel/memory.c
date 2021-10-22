#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_glo.h"
#include "arch/amd64/include/arch_proto.h"

#include "include/param.h"
#include "include/ktypes.h"
#include "include/const.h"
#include "include/proto.h"
#include "include/printk.h"

#include "klib/data_structure.h"

extern kinfo_s kparam;

memory_info_s	mem_info;
recurs_lock_T	page_alloc_lock;
multiboot_memory_map_s	mem_distribution[MAXMEMZONE];

slab_cache_s	slab_cache_groups[SLAB_LEVEL];
slab_s			base_slabs[SLAB_LEVEL];
uint8_t			base_slab_page[SLAB_LEVEL][CONFIG_PAGE_SIZE] __aligned(CONFIG_PAGE_SIZE);
slab_cache_list_s	slab_cache_list;
recurs_lock_T	slab_alloc_lock;


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

	phys_addr low_bound = 0, high_bound = 0;
	uint64_t pg_start_idx = 0, pg_end_idx = 0;

	int i;
	int j;
	int page_count;
	int zone_count = 0;
	for(i = 0; i < mem_info.mb_memmap_nr; i++)
	{
		multiboot_memory_map_s *mbmap_curr = &mem_info.mb_memmap[i];
		low_bound = (phys_addr)mbmap_curr->addr;
		high_bound = (phys_addr)(mbmap_curr->len + low_bound);


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
		mz_curr->zone_start_addr	= (phys_addr)(pg_start_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_end_addr		= (phys_addr)(pg_end_idx * CONFIG_PAGE_SIZE);
		mz_curr->zone_size			= mz_curr->zone_end_addr - mz_curr->zone_start_addr;
		page_count 					+= mz_curr->page_nr;

		// set value of Page_s members in current zone and corresponding bit in page bit map
		for( j = pg_start_idx; j < pg_end_idx; j++)
		{
			Page_s *pg_curr = &mem_info.pages[j];
			pg_curr->zone_belonged	= mz_curr;
			pg_curr->page_start_addr = (phys_addr)(j * CONFIG_PAGE_SIZE);
			bm_clear_bit(mem_info.page_bitmap, j);
		}
		zone_count++;
	}
	mem_info.memzone_total_nr	= zone_count;
	mem_info.page_total_nr		= page_count;

	// set kernel used Page_s in right status
	// map physical pages for kernel
	size_t k_phy_pgbase = 0;
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_vir_end - (virt_addr)phys2virt(0)) / CONFIG_PAGE_SIZE;
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

Page_s * get_page(phys_addr paddr)
{
	long pg_idx = CONFIG_PAGE_ALIGH((uint64_t)paddr) / CONFIG_PAGE_SIZE;
	return &mem_info.pages[pg_idx];
}

/*==============================================================================================*
 *								fuction relate to alloc virtual memory							*
 *==============================================================================================*/

void init_slab()
{
	#ifdef DEBUG
		// make sure have init page management
		while (!kparam.init_flags.page_mm);
	#endif

	init_recurs_lock(&slab_alloc_lock);
	m_init_list_header(&slab_cache_list);

	for (int i = 0; i < SLAB_LEVEL; i++)
	{
		slab_cache_s * scgp = &slab_cache_groups[i];
		m_push_list(scgp, &slab_cache_list);
		m_list_init(scgp);

		m_init_list_header(&scgp->normal_slab_free);
		m_init_list_header(&scgp->normal_slab_used);
		m_init_list_header(&scgp->normal_slab_full);

		slab_s * bslp = &base_slabs[i];
		m_list_init(bslp);

		scgp->obj_size = SLAB_SIZE_BASE << i;
		unsigned long obj_nr = CONFIG_PAGE_SIZE / scgp->obj_size;
		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
		// init 3 status of slab list
		m_push_list(bslp, &scgp->normal_slab_free);
		scgp->normal_base_slab = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;

		// scgp->dma_slab = NULL;
		// scgp->dslab_count = 0;
		// scgp->dsobj_free_count = 0;
		// scgp->dsobj_used_count = 0;

		// init the basic slab
		bslp->colormap = (bitmap_t *)(kparam.kernel_vir_end + 0x10);
		kparam.kernel_vir_end += cm_size + 0x10;
		bslp->slabcache_ptr = scgp;
		bslp->free =
		bslp->total = obj_nr;
		bslp->virt_addr = (virt_addr)base_slab_page[i];
		bslp->page->attr |= PG_Slab;
		int pg_idx = (uint64_t)virt2phys(bslp->virt_addr) / CONFIG_PAGE_SIZE;
		bslp->page = &mem_info.pages[pg_idx];
		bslp->page->slab_ptr = bslp;
	}
	kparam.kernel_vir_end += 0x10;
	// set init flag
	kparam.init_flags.slab = 1;
}

slab_s * slab_alloc(slab_s * cslp)
{
	Page_s * pgp = page_alloc();
	if (!(pgp->attr & PG_PTable_Maped))
		arch_page_domap(phys2virt(pgp->page_start_addr), pgp->page_start_addr,
									ARCH_PG_RW | ARCH_PG_PRESENT, &curr_tsk->mm_struct->cr3);
	
	pgp->attr |= PG_PTable_Maped | PG_Kernel | PG_Slab;
	slab_s * nslp = (slab_s *)kmalloc(sizeof(slab_s));
	m_list_init(nslp);

	nslp->page = pgp;
	pgp->slab_ptr = nslp;
	nslp->colormap = (bitmap_t *)kmalloc( cslp->total / sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->virt_addr = phys2virt(pgp->page_start_addr);

	return nslp;
}

void slab_free(slab_s * slp)
{
	kfree(slp->colormap);
	slp->page->attr &= ~PG_Slab;
	slp->page->slab_ptr = NULL;
	m_remove_from_list(slp);
	slp->slabcache_ptr->normal_slab_free.count--;

	kfree(slp);
}

void * kmalloc(size_t size)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!kparam.init_flags.slab);
	#endif

	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		color_printk(RED, WHITE, "Mem required too big\n");
	}
	else
	{
		// find suitable slab group
		slab_cache_s *	scgp = &slab_cache_groups[0];
		while (size > scgp->obj_size)
			scgp = m_list_get_next(scgp);

		lock_recurs_lock(&slab_alloc_lock);
		// find a usable slab and if it is in free list, move it to used list
		// or if it is in used list and has only one free slot, move it to full list
		slab_s *	slp = NULL;
		if (scgp->normal_slab_used.count > 0)
		{
			m_pop_list(slp, &scgp->normal_slab_used);
			if (slp->free == 1)
				m_push_list(slp, &scgp->normal_slab_full);
			else
				m_push_list(slp, &scgp->normal_slab_used);
		}
		else if (scgp->normal_slab_free.count > 0)
		{
			m_pop_list(slp, &scgp->normal_slab_free);
			m_push_list(slp, &scgp->normal_slab_used);
		}
		else
		{
			color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
			
		// count the virtual addr of suitable object
		unsigned long obj_idx = bm_get_freebit_idx(slp->colormap, 0, slp->total);
		if (obj_idx < slp->total)
		{
			size_t offset = scgp->obj_size * obj_idx;
			ret_val = (void *)((size_t)slp->virt_addr + offset);
			// refresh status of slab
			slp->free--;
			scgp->nsobj_free_count--;
			scgp->nsobj_used_count++;
			bm_set_bit(slp->colormap, obj_idx);
		}

		// make sure free slab more than one
		if (scgp->normal_slab_free.count < 1)
		{
			scgp->normal_slab_free.count++;
			slab_s * new_slab = slab_alloc(slp);
			scgp->normal_slab_free.count--;
			if (new_slab == NULL)
			{
				color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
			}
			new_slab->slabcache_ptr = scgp;
			m_push_list(new_slab, &scgp->normal_slab_free);
			scgp->normal_slab_total++;
			scgp->nsobj_free_count += new_slab->total;
		}

		unlock_recurs_lock(&slab_alloc_lock);
	}

	return ret_val;
}

void kfree(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!kparam.init_flags.slab);
	#endif

	if (obj_p == NULL)
		return;

	// find which slab does the pointer belonged to
	phys_addr pg_addr = virt2phys((virt_addr)CONFIG_PAGE_MASKF((size_t)obj_p));
	unsigned long pg_idx = (size_t)pg_addr / CONFIG_PAGE_SIZE;
	Page_s * pgp = &mem_info.pages[pg_idx];
	slab_s * slp = pgp->slab_ptr;
	slab_cache_s * scgp = slp->slabcache_ptr;
	// of coures it should not in an empty-slab
	if (slp->free == slp->total)
	{
		color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
	}

	unsigned long obj_idx = (obj_p - slp->virt_addr) / scgp->obj_size;
	if (!bm_get_assigned_bit(slp->colormap, obj_idx))
	{
		color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
		while (1);
	}
	lock_recurs_lock(&slab_alloc_lock);
	bm_clear_bit(slp->colormap, obj_idx);
	slp->free++;
	scgp->nsobj_free_count++;
	scgp->nsobj_used_count--;
	// if the corresponding slab in full list, move it to used list
	// or if it is in used list and only use one slot, move it to free list
	if (slp->free == 1)
	{
		m_list_delete(slp);
		scgp->normal_slab_full.count--;
		m_push_list(slp, &scgp->normal_slab_used);
	}
	else if (slp->free == 0)
	{
		m_list_delete(slp);
		scgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
		if (slp != scgp->normal_base_slab)
			m_push_list(slp, &scgp->normal_slab_free);
		else
			m_append_to_list(slp, &scgp->normal_slab_free);
	}
	// if there is too many free slab, free some of them
	if (scgp->normal_slab_free.count > 2)
	{
		slab_s * tmp_slp = scgp->normal_slab_free.head_p->prev;
		scgp->normal_slab_free.count--;
		slab_free(scgp->normal_slab_free.head_p);
		scgp->normal_slab_total;
		scgp->nsobj_free_count -= scgp->normal_slab_free.head_p->total;
	}

	unlock_recurs_lock(&slab_alloc_lock);
}
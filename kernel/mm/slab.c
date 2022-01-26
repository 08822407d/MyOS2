#include <sys/cdefs.h>

#include <string.h>
#include <stddef.h>
#include <sys/types.h>

#include <lib/utils.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/archtypes.h>
#include <arch/amd64/include/arch_glo.h>
#include <arch/amd64/include/arch_proto.h>

#include <include/glo.h>
#include <include/slab.h>
#include <include/mm.h>
#include <include/memblock.h>
#include <include/proto.h>
#include <include/printk.h>

List_hdr_s		slabcache_lhdr;
recurs_lock_T	slab_alloc_lock;

slab_cache_s *	slab_cache_groups_p;
slab_s *		base_slabs_p;
uint8_t	*		base_slab_pages_p;

/*==============================================================================================*
 *								fuction relate to alloc virtual memory							*
 *==============================================================================================*/
void preinit_slab()
{
	slab_cache_groups_p = memblock_alloc_normal(sizeof(slab_cache_s) * SLAB_LEVEL, 1);
	base_slabs_p = memblock_alloc_normal(sizeof(slab_s) * SLAB_LEVEL, 1);
	base_slab_pages_p = memblock_alloc_normal(SLAB_LEVEL * PAGE_SIZE, PAGE_SIZE);

	for (int i = 0; i < SLAB_LEVEL; i++)
	{
		int obj_size = SLAB_SIZE_BASE << i;
		int obj_nr = PAGE_SIZE / obj_size;
		int bm_size = round_up(obj_nr, BITMAP_UNITSIZE) / BITMAP_UNITSIZE;

		slab_s * bslp = base_slabs_p + i;
		bslp->total = obj_nr;
		bslp->colormap = memblock_alloc_normal(bm_size * sizeof(bitmap_t), 8);
	}
}

void init_slab()
{
	#ifdef DEBUG
		// make sure have init page management
		while (!kparam.init_flags.buddy);
	#endif

	init_recurs_lock(&slab_alloc_lock);
	list_hdr_init(&slabcache_lhdr);

	for (int i = 0; i < SLAB_LEVEL; i++)
	{
		slab_cache_s * scgp = slab_cache_groups_p + i;
		list_init(&scgp->slabcache_list, scgp);
		list_hdr_append(&slabcache_lhdr, &scgp->slabcache_list);

		list_hdr_init(&scgp->normal_slab_free);
		list_hdr_init(&scgp->normal_slab_used);
		list_hdr_init(&scgp->normal_slab_full);

		slab_s * bslp = base_slabs_p + i;
		list_init(&bslp->slab_list, bslp);

		// init 3 status of slab list
		list_hdr_append(&scgp->normal_slab_free, &bslp->slab_list);
		scgp->normal_base_slab = bslp;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;
		scgp->obj_size = SLAB_SIZE_BASE << i;

		bslp->slabcache_ptr = scgp;
		bslp->free = bslp->total;
		bslp->virt_addr = (virt_addr_t)(base_slab_pages_p + i * PAGE_SIZE);
		bslp->page->attr |= PG_Slab;
		bslp->page = paddr_to_page(virt2phys(bslp->virt_addr));;
		bslp->page->slab_ptr = bslp;
	}
	// set init flag
	kparam.init_flags.slab = 1;
}

slab_s * slab_alloc(slab_s * cslp)
{
	Page_s * pgp = alloc_pages(ZONE_NORMAL, 0);
	phys_addr_t page_paddr = page_to_paddr(pgp);
	if (!(pgp->attr & PG_PTable_Maped))
		arch_page_domap(phys2virt(page_paddr), page_paddr,
				ARCH_PG_RW | ARCH_PG_PRESENT, &curr_tsk->mm_struct->cr3);
	
	pgp->attr |= PG_PTable_Maped | PG_Kernel | PG_Slab;
	slab_s * nslp = (slab_s *)kmalloc(sizeof(slab_s));
	list_init(&nslp->slab_list, nslp);

	nslp->page = pgp;
	pgp->slab_ptr = nslp;
	nslp->colormap = (bitmap_t *)kmalloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->virt_addr = phys2virt(page_paddr);

	return nslp;
}

void slab_free(slab_s * slp)
{
	kfree(slp->colormap);
	slp->page->attr &= ~PG_Slab;
	slp->page->slab_ptr = NULL;
	list_delete(&slp->slab_list);
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
		slab_cache_s *	scgp = slab_cache_groups_p;
		while (size > scgp->obj_size)
			scgp = container_of(list_get_next(&scgp->slabcache_list), slab_cache_s, slabcache_list);

		lock_recurs_lock(&slab_alloc_lock);
		// find a usable slab and if it is in free list, move it to used list
		// or if it is in used list and has only one free slot, move it to full list
		slab_s *	slp = NULL;
		if (scgp->normal_slab_used.count > 0)
		{
			List_s * slp_lp = list_hdr_pop(&scgp->normal_slab_used);
			while (slp_lp == 0);
			
			slp = container_of(slp_lp, slab_s, slab_list);
			if (slp->free == 1)
				list_hdr_push(&scgp->normal_slab_full, &slp->slab_list);
			else
				list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
		}
		else if (scgp->normal_slab_free.count > 0)
		{
			List_s * slp_lp = list_hdr_pop(&scgp->normal_slab_free);
			while (slp_lp == 0);

			slp = container_of(slp_lp, slab_s, slab_list);
			list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
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
			list_hdr_push(&scgp->normal_slab_free, &new_slab->slab_list);
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
	phys_addr_t pg_addr = virt2phys((virt_addr_t)PAGE_ROUND_DOWN((size_t)obj_p));
	unsigned long pg_idx = (size_t)pg_addr / PAGE_SIZE;
	Page_s * pgp = &mem_map[pg_idx];
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
		list_delete(&slp->slab_list);
		scgp->normal_slab_full.count--;
		list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
	}
	else if (slp->free == 0)
	{
		list_delete(&slp->slab_list);
		scgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
		if (slp != scgp->normal_base_slab)
			list_hdr_push(&scgp->normal_slab_free, &slp->slab_list);
		else
			list_hdr_append(&scgp->normal_slab_free, &slp->slab_list);
	}
	// if there is too many free slab, free some of them
	if (scgp->normal_slab_free.count > 2)
	{
		slab_s * tmp_slp = container_of(scgp->normal_slab_free.header.prev, slab_s, slab_list);
		scgp->normal_slab_free.count--;
		slab_free(tmp_slp);
		scgp->normal_slab_total;
		scgp->nsobj_free_count -= tmp_slp->total;
	}

	unlock_recurs_lock(&slab_alloc_lock);
}
#include <stdlib.h>

#include "_malloc.h"

size_t brk_end = 0;

// List_hdr_s		uslabcache_lhdr;
// uslab_cache_s	slab_cache_groups[SLAB_LEVEL];
// uslab_s			base_slabs[SLAB_LEVEL];

// /*==============================================================================================*
//  *								fuction relate to alloc virtual memory							*
//  *==============================================================================================*/
// void init_uslab()
// {
// 	brk_end = brk(0);
// 	list_hdr_init(&uslabcache_lhdr);

// 	for (int i = 0; i < SLAB_LEVEL; i++)
// 	{
// 		uslab_cache_s * scgp = &slab_cache_groups[i];
// 		list_init(&scgp->slabcache_list, scgp);
// 		list_hdr_append(&slabcache_lhdr, &scgp->slabcache_list);

// 		list_hdr_init(&scgp->normal_slab_free);
// 		list_hdr_init(&scgp->normal_slab_used);
// 		list_hdr_init(&scgp->normal_slab_full);

// 		uslab_s * bslp = &base_slabs[i];
// 		list_init(&bslp->slab_list, bslp);

// 		scgp->obj_size = SLAB_SIZE_BASE << i;
// 		unsigned long obj_nr = CONFIG_PAGE_SIZE / scgp->obj_size;
// 		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
// 		// init 3 status of slab list
// 		list_hdr_append(&scgp->normal_slab_free, &bslp->slab_list);
// 		scgp->normal_base_slab = bslp;
// 		scgp->nsobj_free_count = obj_nr;
// 		scgp->nsobj_used_count = 0;
// 		scgp->normal_slab_total = 1;

// 		bslp->colormap = (bitmap_t *)(kparam.kernel_vir_end + 0x10);
// 		kparam.kernel_vir_end += cm_size + 0x10;
// 		bslp->slabcache_ptr = scgp;
// 		bslp->free =
// 		bslp->total = obj_nr;
// 		bslp->virt_addr = (virt_addr)base_slab_page[i];
// 		int pg_idx = (uint64_t)virt2phys(bslp->virt_addr) / CONFIG_PAGE_SIZE;
// 		bslp->page = &mem_info.pages[pg_idx];
// 		bslp->page->slab_ptr = bslp;
// 	}
// 	kparam.kernel_vir_end += 0x10;
// 	// set init flag
// 	kparam.init_flags.slab = 1;
// }

// uslab_s * slab_alloc(uslab_s * cslp)
// {
// 	Page_s * pgp = page_alloc();
// 	if (!(pgp->attr & PG_PTable_Maped))
// 		arch_page_domap(phys2virt(pgp->page_start_addr), pgp->page_start_addr,
// 									ARCH_PG_RW | ARCH_PG_PRESENT, &curr_tsk->mm_struct->cr3);
	
// 	pgp->attr |= PG_PTable_Maped | PG_Kernel | PG_Slab;
// 	uslab_s * nslp = (uslab_s *)kmalloc(sizeof(uslab_s));
// 	list_init(&nslp->slab_list, nslp);

// 	nslp->page = pgp;
// 	pgp->slab_ptr = nslp;
// 	nslp->colormap = (bitmap_t *)kmalloc( cslp->total / sizeof(bitmap_t));
// 	nslp->total =
// 	nslp->free = cslp->total;
// 	nslp->virt_addr = phys2virt(pgp->page_start_addr);

// 	return nslp;
// }

// void slab_free(uslab_s * slp)
// {
// 	kfree(slp->colormap);
// 	slp->page->attr &= ~PG_Slab;
// 	slp->page->slab_ptr = NULL;
// 	list_delete(&slp->slab_list);
// 	slp->slabcache_ptr->normal_slab_free.count--;

// 	kfree(slp);
// }

void * malloc(size_t size)
{
	void * ret_val = NULL;
// 	if (size > CONST_1M)
// 	{
// 		// color_printk(RED, WHITE, "Mem required too big\n");
// 	}
// 	else
// 	{
// 		// find suitable slab group
// 		uslab_cache_s *	scgp = &slab_cache_groups[0];
// 		while (size > scgp->obj_size)
// 			scgp = container_of(list_get_next(&scgp->slabcache_list), uslab_cache_s, slabcache_list);

// 		// find a usable slab and if it is in free list, move it to used list
// 		// or if it is in used list and has only one free slot, move it to full list
// 		uslab_s *	slp = NULL;
// 		if (scgp->normal_slab_used.count > 0)
// 		{
// 			slp = container_of(list_hdr_pop(&scgp->normal_slab_used), uslab_s, slab_list);
// 			if (slp->free == 1)
// 				list_hdr_push(&scgp->normal_slab_full, &slp->slab_list);
// 			else
// 				list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
// 		}
// 		else if (scgp->normal_slab_free.count > 0)
// 		{
// 			slp = container_of(list_hdr_pop(&scgp->normal_slab_free), uslab_s, slab_list);
// 			list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
// 		}
// 		else
// 		{
// 			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
// 			while (1);
// 		}
			
// 		// count the virtual addr of suitable object
// 		unsigned long obj_idx = bm_get_freebit_idx(slp->colormap, 0, slp->total);
// 		if (obj_idx < slp->total)
// 		{
// 			size_t offset = scgp->obj_size * obj_idx;
// 			ret_val = (void *)((size_t)slp->virt_addr + offset);
// 			// refresh status of slab
// 			slp->free--;
// 			scgp->nsobj_free_count--;
// 			scgp->nsobj_used_count++;
// 			bm_set_bit(slp->colormap, obj_idx);
// 		}

// 		// make sure free slab more than one
// 		if (scgp->normal_slab_free.count < 1)
// 		{
// 			scgp->normal_slab_free.count++;
// 			uslab_s * new_slab = slab_alloc(slp);
// 			scgp->normal_slab_free.count--;
// 			if (new_slab == NULL)
// 			{
// 				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
// 				while (1);
// 			}
// 			new_slab->slabcache_ptr = scgp;
// 			list_hdr_push(&scgp->normal_slab_free, &new_slab->slab_list);
// 			scgp->normal_slab_total++;
// 			scgp->nsobj_free_count += new_slab->total;
// 		}

// 		unlock_recurs_lock(&slab_alloc_lock);
// 	}

	return ret_val;
}

void free(void * obj_p)
{
// 	if (obj_p == NULL)
// 		return;

// 	// find which slab does the pointer belonged to
// 	phys_addr pg_addr = virt2phys((virt_addr)CONFIG_PAGE_MASKF((size_t)obj_p));
// 	unsigned long pg_idx = (size_t)pg_addr / CONFIG_PAGE_SIZE;
// 	Page_s * pgp = &mem_info.pages[pg_idx];
// 	uslab_s * slp = pgp->slab_ptr;
// 	uslab_cache_s * scgp = slp->slabcache_ptr;
// 	// of coures it should not in an empty-slab
// 	if (slp->free == slp->total)
// 	{
// 		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
// 		while (1);
// 	}

// 	unsigned long obj_idx = (obj_p - slp->virt_addr) / scgp->obj_size;
// 	if (!bm_get_assigned_bit(slp->colormap, obj_idx))
// 	{
// 		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
// 		while (1);
// 	}
// 	bm_clear_bit(slp->colormap, obj_idx);
// 	slp->free++;
// 	scgp->nsobj_free_count++;
// 	scgp->nsobj_used_count--;
// 	// if the corresponding slab in full list, move it to used list
// 	// or if it is in used list and only use one slot, move it to free list
// 	if (slp->free == 1)
// 	{
// 		list_delete(&slp->slab_list);
// 		scgp->normal_slab_full.count--;
// 		list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
// 	}
// 	else if (slp->free == 0)
// 	{
// 		list_delete(&slp->slab_list);
// 		scgp->normal_slab_used.count--;
// 		// if slp is base-slab, add it to tail
// 		if (slp != scgp->normal_base_slab)
// 			list_hdr_push(&scgp->normal_slab_free, &slp->slab_list);
// 		else
// 			list_hdr_append(&scgp->normal_slab_free, &slp->slab_list);
// 	}
// 	// if there is too many free slab, free some of them
// 	if (scgp->normal_slab_free.count > 2)
// 	{
// 		uslab_s * tmp_slp = container_of(scgp->normal_slab_free.header.prev, uslab_s, slab_list);
// 		scgp->normal_slab_free.count--;
// 		slab_free(tmp_slp);
// 		scgp->normal_slab_total;
// 		scgp->nsobj_free_count -= tmp_slp->total;
// 	}
}
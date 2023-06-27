#include <linux/kernel/slab.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/mm/myos_slab.h>
#include <linux/lib/string.h>

#include <klib/utils.h>
#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>


#define SLAB_SIZE_BASE		32

List_hdr_s		slabcache_lhdr;
recurs_lock_T	slab_alloc_lock;

slab_cache_s *	slab_cache_groups_p;
slab_s *		base_slabs_p;
int		slab_level = 0;

/*==============================================================================================*
 *								fuction relate to alloc virtual memory							*
 *==============================================================================================*/
void myos_preinit_slab()
{
	slab_level = ilog2(KMALLOC_MAX_CACHE_SIZE / SLAB_SIZE_BASE) + 1;
	slab_cache_groups_p = myos_memblock_alloc_normal(sizeof(slab_cache_s) * slab_level, 1);
	base_slabs_p = myos_memblock_alloc_normal(sizeof(slab_s) * slab_level, 1);

	for (int i = 0; i < slab_level; i++)
	{
		int obj_size = SLAB_SIZE_BASE << i;
		int obj_nr = PAGE_SIZE / obj_size;
		int bm_size = round_up(obj_nr, BITMAP_UNITSIZE) / BITMAP_UNITSIZE;

		slab_s * bslp = base_slabs_p + i;
		bslp->total = obj_nr;
		bslp->colormap = myos_memblock_alloc_normal(bm_size * sizeof(bitmap_t), 8);
	}
}

void myos_init_slab()
{
	init_recurs_lock(&slab_alloc_lock);
	list_hdr_init(&slabcache_lhdr);

	for (int i = 0; i < slab_level; i++)
	{
		slab_cache_s *scgp = slab_cache_groups_p + i;
		list_init(&scgp->slabcache_list, scgp);
		list_hdr_append(&slabcache_lhdr, &scgp->slabcache_list);

		list_hdr_init(&scgp->normal_slab_free);
		list_hdr_init(&scgp->normal_slab_used);
		list_hdr_init(&scgp->normal_slab_full);

		slab_s *bslp = base_slabs_p + i;
		list_init(&bslp->slab_list, bslp);

		// init 3 status of slab list
		list_hdr_append(&scgp->normal_slab_free, &bslp->slab_list);
		scgp->normal_base_slab = bslp;
		scgp->nsobj_used_count = 0;
		scgp->nsobj_free_count += bslp->total;
		scgp->normal_slab_total = 1;
		scgp->obj_size = SLAB_SIZE_BASE << i;

		bslp->slabcache_ptr = scgp;
		bslp->free = bslp->total;
		page_s *page = __alloc_pages(GFP_KERNEL, 0);
		bslp->page = page;
		__SetPageSlab(page);
		bslp->virt_addr = page_to_virt(page);
		bslp->page->private = (unsigned long)bslp;
	}
	// set init flag
	kparam.init_flags.slab = 1;
}

slab_s * slab_alloc(slab_s *cslp)
{
	page_s *page = alloc_page(ZONE_NORMAL);
	
	__SetPageSlab(page);
	slab_s *nslp = (slab_s *)kzalloc(sizeof(slab_s), GFP_KERNEL);
	list_init(&nslp->slab_list, nslp);

	nslp->page = page;
	page->private = (unsigned long)nslp;
	nslp->colormap = (bitmap_t *)kzalloc(cslp->total /
					sizeof(bitmap_t) + sizeof(bitmap_t), GFP_KERNEL);
	nslp->total =
	nslp->free = cslp->total;
	nslp->virt_addr = page_to_virt(page);

	return nslp;
}

void slab_free(slab_s *slp)
{
	while (!PageSlab(slp->page));
	
	kfree(slp->colormap);
	__ClearPageSlab(slp->page);
	slp->page->private = 0;
	list_delete(&slp->slab_list);
	slp->slabcache_ptr->normal_slab_free.count--;

	kfree(slp);
}

void * __kmalloc(size_t size, gfp_t flags)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!kparam.init_flags.slab);
	#endif

	void *ret_val = NULL;
	while (size > KMALLOC_MAX_CACHE_SIZE);

	// find suitable slab group
	slab_cache_s *scgp = slab_cache_groups_p;
	while (size > scgp->obj_size)
		scgp = container_of(list_get_next(&scgp->slabcache_list), slab_cache_s, slabcache_list);

	lock_recurs_lock(&slab_alloc_lock);
	// find a usable slab and if it is in free list, move it to used list
	// or if it is in used list and has only one free slot, move it to full list
	slab_s *slp = NULL;
	List_s *slp_lp = NULL;
	if (scgp->normal_slab_used.count > 0)
		slp_lp = list_hdr_pop(&scgp->normal_slab_used);
	else if (scgp->normal_slab_free.count > 0)
		slp_lp = list_hdr_pop(&scgp->normal_slab_free);
	else
	{
		color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
		while (1);
	}
	while (slp_lp == 0);
	slp = container_of(slp_lp, slab_s, slab_list);

	if (slp->free == 1)
		list_hdr_push(&scgp->normal_slab_full, &slp->slab_list);
	else
		list_hdr_push(&scgp->normal_slab_used, &slp->slab_list);
		
	// count the virtual addr of suitable object
	unsigned long obj_idx = bm_get_freebit_idx(slp->colormap, 0, slp->total);
	while (obj_idx >= slp->total);
	size_t offset = scgp->obj_size * obj_idx;
	ret_val = (void *)((size_t)slp->virt_addr + offset);
	// refresh status of slab
	slp->free--;
	scgp->nsobj_free_count--;
	scgp->nsobj_used_count++;
	bm_set_bit(slp->colormap, obj_idx);

	// make sure free slab more than one
	if (scgp->normal_slab_free.count == 0)
	{
		scgp->normal_slab_free.count++;
		slab_s *new_slab = slab_alloc(slp);
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
	return ret_val;
}
void *kmalloc(size_t size, gfp_t flags)
{
	void *ret_val = 0;
// 	if (__builtin_constant_p(size))
// 	{
	// unsigned int index;
	if (size > KMALLOC_MAX_CACHE_SIZE)
	{
		unsigned int order = get_order(size);
		ret_val = kmalloc_order(size, flags, order);
	}
	else
	{
		// index = kmalloc_index(size);

		// if (!index)
		// 	return ZERO_SIZE_PTR;

		// return kmem_cache_alloc_trace(
		// 	kmalloc_caches[kmalloc_type(flags)][index],
		// 	flags, size);
		ret_val = __kmalloc(size, flags);
	}
// 	}

	if (flags & __GFP_ZERO)
		memset(ret_val, 0, size);

	return ret_val;
}


void kfree(const void *objp)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!kparam.init_flags.slab);
	#endif

	if (objp == NULL)
		return;

	page_s *page = virt_to_page(objp);
	if (PageHead(page))
	{
		unsigned int order = page->private;
		__free_pages(page, order);
		__ClearPageHead(page);
	}
	else if (PageSlab(page))
	{
		// find which slab does the pointer belonged to
		slab_s *slp = (slab_s *)page->private;
		slab_cache_s * scgp = slp->slabcache_ptr;
		// of coures it should not in an empty-slab
		if (slp->free == slp->total)
		{
			color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
			while (1);
		}

		unsigned long obj_idx = ((virt_addr_t)objp - slp->virt_addr) / scgp->obj_size;
		if (!bm_get_assigned_bit(slp->colormap, obj_idx))
		{
			color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", objp);
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
}
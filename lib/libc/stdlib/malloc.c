#include <arch/amd64/include/arch_config.h>

#include <stdlib.h>
#include <unistd.h>
#include <const.h>
#include <stdbool.h>

#include "_malloc.h"

bool initiated = false;

virt_addr_t brk_end = NULL;

List_hdr_s		uslabcache_lhdr;
uslab_cache_s	uslab_cache_groups[USLAB_LEVEL];
uslab_s			base_uslabs[USLAB_LEVEL];
uPage_s			base_upages[USLAB_LEVEL];
List_hdr_s		free_upage_lhdr;
List_hdr_s		used_upage_lhdr;

bitmap_t		cm_1MB[1];		// 2		bits
bitmap_t		cm_512KB[1];	// 4		bits
bitmap_t		cm_256KB[1];	// 8		bits
bitmap_t		cm_128KB[1];	// 16		bits
bitmap_t		cm_64KB[1];		// 32		bits
bitmap_t		cm_32KB[1];		// 64		bits
bitmap_t		cm_16KB[2];		// 128		bits
bitmap_t		cm_8KB[4];		// 256		bits
bitmap_t		cm_4KB[8];		// 512		bits
bitmap_t		cm_2KB[16];		// 1024		bits
bitmap_t		cm_1KB[32];		// 2048		bits
bitmap_t		cm_512B[64];	// 4096		bits
bitmap_t		cm_256B[128];	// 8192		bits
bitmap_t		cm_128B[256];	// 16384	bits
bitmap_t		cm_64B[512];	// 32768	bits
bitmap_t		cm_32B[1024];	// 65536	bits

bitmap_t *		base_colormaps[USLAB_LEVEL] = {
	cm_32B,
	cm_64B,
	cm_128B,
	cm_256B,
	cm_512B,
	cm_1KB,
	cm_2KB,
	cm_4KB,
	cm_8KB,
	cm_16KB,
	cm_32KB,
	cm_64KB,
	cm_128KB,
	cm_256KB,
	cm_512KB,
	cm_1MB,
};

/*==============================================================================================*
 *								fuction relate to alloc user page								*
 *==============================================================================================*/
uPage_s * upage_alloc()
{
	uPage_s * ret_val = NULL;
	if (free_upage_lhdr.count > 0)
	{
		List_s * upg_lp = list_hdr_pop(&free_upage_lhdr);
		while (upg_lp == 0);
		
		ret_val = container_of(upg_lp, uPage_s, upage_list);
		list_hdr_push(&used_upage_lhdr, &ret_val->upage_list);
	}
	else
	{
		uPage_s * upgp = malloc(sizeof(uPage_s));
		virt_addr_t pg_vaddr = brk(brk_end + PAGE_SIZE);
		if (pg_vaddr != NULL)
		{
			brk_end += PAGE_SIZE;
			upgp->vaddr = pg_vaddr;
			list_init(&upgp->upage_list, upgp);
			list_hdr_append(&used_upage_lhdr, &upgp->upage_list);
			ret_val = upgp;
		}
		else
		{
			free(upgp);
			ret_val = NULL;
		}
	}
	return ret_val;
}

void upage_free(uPage_s * upgp)
{
	list_delete(&upgp->upage_list);
	used_upage_lhdr.count--;
	list_hdr_push(&free_upage_lhdr, &upgp->upage_list);
	upgp->uslab_p = NULL;
}

uPage_s * upage_search(virt_addr_t vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (virt_addr_t)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
	for (upg_lp = used_upage_lhdr.header.next;
		 upg_lp != &used_upage_lhdr.header;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
	}

	return ret_val;
}

/*==============================================================================================*
 *								fuction relate to alloc virtual memory							*
 *==============================================================================================*/
void init_uslab()
{
	brk_end = brk(0);
	list_hdr_init(&uslabcache_lhdr);
	list_hdr_init(&free_upage_lhdr);
	list_hdr_init(&used_upage_lhdr);

	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
		list_init(&scgp->uslabcache_list, scgp);
		list_hdr_append(&uslabcache_lhdr, &scgp->uslabcache_list);

		list_hdr_init(&scgp->normal_slab_free);
		list_hdr_init(&scgp->normal_slab_used);
		list_hdr_init(&scgp->normal_slab_full);

		uslab_s * bslp = &base_uslabs[i];
		list_init(&bslp->uslab_list, bslp);

		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
		// init 3 status of slab list
		list_hdr_append(&scgp->normal_slab_free, &bslp->uslab_list);
		scgp->normal_base_slab_p = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;

		bslp->colormap = base_colormaps[i];
		bslp->uslabcache_p = scgp;
		bslp->free =
		bslp->total = obj_nr;

		// init upages
		uPage_s * upgp = &base_upages[i];
		list_init(&upgp->upage_list, upgp);
		upgp->vaddr =
		brk_end = brk((virt_addr_t)((size_t)brk_end + PAGE_SIZE));
		list_hdr_append(&free_upage_lhdr, &upgp->upage_list);

		bslp->upage_p = upage_alloc();
		bslp->virt_addr = bslp->upage_p->vaddr;
		bslp->upage_p->uslab_p = bslp;
	}
	initiated = true;
}

uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	list_init(&nslp->uslab_list, nslp);

	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
	nslp->upage_p->uslab_p = nslp;

	return nslp;
}

void uslab_free(uslab_s * slp)
{
	upage_free(slp->upage_p);

	free(slp->colormap);
	list_delete(&slp->uslab_list);
	slp->uslabcache_p->normal_slab_free.count--;

	free(slp);
}

void * malloc(size_t size)
{
	if (!initiated)
		init_uslab();

	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
	else
	{
		// find suitable slab group
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
			uscgp = container_of(list_get_next(&uscgp->uslabcache_list), uslab_cache_s, uslabcache_list);

		// lock_recurs_lock(&slab_alloc_lock);

		// find a usable slab and if it is in free list, move it to used list
		// or if it is in used list and has only one free slot, move it to full list
		uslab_s *	uslp = NULL;
		if (uscgp->normal_slab_used.count > 0)
		{
			List_s * uslp_lp = list_hdr_pop(&uscgp->normal_slab_used);
			while (uslp_lp == 0);
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			if (uslp->free == 1)
				list_hdr_push(&uscgp->normal_slab_full, &uslp->uslab_list);
			else
				list_hdr_push(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
		else if (uscgp->normal_slab_free.count > 0)
		{
			List_s * uslp_lp = list_hdr_pop(&uscgp->normal_slab_free);
			while (uslp_lp == 0);
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			list_hdr_push(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
		else
		{
			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
			
		// count the virtual addr of suitable object
		unsigned long obj_idx = bm_get_freebit_idx(uslp->colormap, 0, uslp->total);
		if (obj_idx < uslp->total)
		{
			size_t offset = uscgp->obj_size * obj_idx;
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
			uscgp->nsobj_used_count++;
			bm_set_bit(uslp->colormap, obj_idx);
		}

		// make sure free slab more than one
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
			uscgp->normal_slab_free.count--;
			if (new_uslp == NULL)
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
			}
			new_uslp->uslabcache_p = uscgp;
			list_hdr_push(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
		}

		// unlock_recurs_lock(&slab_alloc_lock);
	}

	return ret_val;
}

void free(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!initiated);
	#endif

	if (obj_p == NULL)
		return;

	// find which slab does the pointer belonged to
	uPage_s * upgp = upage_search(obj_p);
	uslab_s * uslp = upgp->uslab_p;
	uslab_cache_s * uscgp = uslp->uslabcache_p;
	// of coures it should not in an empty-slab
	if (uslp->free == uslp->total)
	{
		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
	}

	unsigned long obj_idx = (obj_p - uslp->virt_addr) / uscgp->obj_size;
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
	{
		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
		while (1);
	}

	// lock_recurs_lock(&slab_alloc_lock);

	bm_clear_bit(uslp->colormap, obj_idx);
	uslp->free++;
	uscgp->nsobj_free_count++;
	uscgp->nsobj_used_count--;
	// if the corresponding slab in full list, move it to used list
	// or if it is in used list and only use one slot, move it to free list
	if (uslp->free == 1)
	{
		list_delete(&uslp->uslab_list);
		uscgp->normal_slab_full.count--;
		list_hdr_push(&uscgp->normal_slab_used, &uslp->uslab_list);
	}
	else if (uslp->free == 0)
	{
		list_delete(&uslp->uslab_list);
		uscgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
		if (uslp != uscgp->normal_base_slab_p)
			list_hdr_push(&uscgp->normal_slab_free, &uslp->uslab_list);
		else
			list_hdr_append(&uscgp->normal_slab_free, &uslp->uslab_list);
	}
	// if there is too many free slab, free some of them
	if (uscgp->normal_slab_free.count > 2)
	{
		uslab_s * tmp_uslp = container_of(uscgp->normal_slab_free.header.prev, uslab_s, uslab_list);
		uscgp->normal_slab_free.count--;
		uslab_free(tmp_uslp);
		uscgp->normal_slab_total;
		uscgp->nsobj_free_count -= tmp_uslp->total;
	}

	// unlock_recurs_lock(&slab_alloc_lock);
}
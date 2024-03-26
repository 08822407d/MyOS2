#ifndef _SLAB_H_
#define _SLAB_H_

	#include <linux/lib/list.h>

	struct  slab;
	typedef struct slab slab_s;

	struct	slab_cache;
	typedef struct slab_cache slab_cache_s;

	struct page;
	typedef struct page page_s;

	typedef struct slab
	{
		List_s			slab_list;
		slab_cache_s *	slabcache_ptr;

		ulong			total;
		ulong			free;

		virt_addr_t		virt_addr;
		page_s *		page;
		bitmap_t *		colormap;
	} slab_s;

	typedef struct slab_cache
	{
		List_s			slabcache_list;

		unsigned long	obj_size;

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		slab_s *		normal_base_slab;
	} slab_cache_s;

#endif /* _SLAB_H_ */
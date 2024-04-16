#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "../../include/utils.h"

	// uslab consts
	#define USLAB_LEVEL			16
	#define USLAB_SIZE_BASE		32

	struct  uslab;
	typedef struct uslab uslab_s;

	struct	uslab_cache;
	typedef struct uslab_cache uslab_cache_s;

/*==============================================================================================*
 *									kernel malloc and slab										*
 *==============================================================================================*/
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
	} uPage_s;

	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
		bitmap_t		*colormap;
	} uslab_s;

	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
	} uslab_cache_s;


#endif /* _MALLOC_H_ */
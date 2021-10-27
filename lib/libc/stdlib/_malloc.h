#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <sys/cdefs.h>
#include <lib/utils.h>

#include <stddef.h>
#include <stdint.h>

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
	typedef struct uslab
	{
		List_s			slab_list;
		uslab_cache_s *	slabcache_ptr;

		unsigned long	total;
		unsigned long	free;

		virt_addr		virt_addr;
		bitmap_t *		colormap;
	} uslab_s;

	typedef struct uslab_cache
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
		uslab_s *		normal_base_slab;
	} uslab_cache_s;


#endif /* _MALLOC_H_ */
#ifndef _LINUX_PAGE_ALLOC_H_
#define _LINUX_PAGE_ALLOC_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/mm/mm.h>

	#include "page_alloc_types.h"

	struct zone;
	typedef struct zone zone_s;

	/*
	 * page_private can be used on tail pages.  However, PagePrivate is only
	 * checked by the VM on the head page.  So page_private on the tail pages
	 * should be used for data that's ancillary to the head page (eg attaching
	 * buffer heads to tail pages after attaching buffer heads to the head page)
	 */
	#define page_private(page)	((page)->private)

	#ifdef DEBUG

		extern bool
		page_is_guard(page_s *page);
		
		extern void
		clear_page_pfmemalloc(page_s *page);

		extern void
		get_page(page_s *page);

		extern void
		put_page(page_s *page);

		extern zone_s
		*myos_page_zone(const page_s * page);

	#endif
	
	#if defined(PAGEALLOC_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		bool
		page_is_guard(page_s *page) {
			return false;
		}

		PREFIX_STATIC_INLINE
		void
		clear_page_pfmemalloc(page_s *page) {
			// page->lru.next = NULL;
			list_init(&page->lru, page);
		}

		PREFIX_STATIC_INLINE
		void
		get_page(page_s *page) {
			atomic_inc(&page->_refcount);
		}

		PREFIX_STATIC_INLINE
		void
		put_page(page_s *page) {
			atomic_dec(&page->_refcount);
		}

		PREFIX_STATIC_INLINE
		zone_s
		*myos_page_zone(const page_s * page) {
			unsigned long pfn = page_to_pfn(page);
			for (int i = 0 ; i < MAX_NR_ZONES; i ++)
			{
				zone_s * zone = &(NODE_DATA(0)->node_zones[i]);
				if (pfn >= zone->zone_start_pfn && 
					pfn < (zone->zone_start_pfn + zone->spanned_pages))
					return zone;
			}
		}

	#endif

#endif /* _LINUX_PAGE_ALLOC_H_ */
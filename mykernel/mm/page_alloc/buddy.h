#ifndef _LINUX_PAGE_ALLOC_H_
#define _LINUX_PAGE_ALLOC_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	/*
	 * page_private can be used on tail pages.  However, PagePrivate is only
	 * checked by the VM on the head page.  So page_private on the tail pages
	 * should be used for data that's ancillary to the head page (eg attaching
	 * buffer heads to tail pages after attaching buffer heads to the head page)
	 */
	#define page_private(page)	((page)->private)

	#ifdef DEBUG

		extern folio_s
		*virt_to_folio(const void *x);

		extern uint
		compound_order(page_s *page);

		extern uint
		folio_order(folio_s *folio);

		extern void
		get_page(page_s *page);
		extern void
		put_page(page_s *page);

		extern void
		set_page_count(page_s *page, int v);
		extern void
		init_page_count(page_s *page);

		extern bool
		page_is_guard(page_s *page);
		
		extern void
		clear_page_pfmemalloc(page_s *page);

		extern pgoff_t
		linear_page_index(vma_s *vma, ulong address);

		extern zone_s
		*myos_page_zone(const page_s * page);

	#endif
	
	#if defined(BUDDY_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		page_s
		*virt_to_head_page(const void *x) {
			page_s *page = virt_to_page(x);
			return compound_head(page);
		}
		PREFIX_STATIC_INLINE
		folio_s
		*virt_to_folio(const void *x) {
			page_s *page = virt_to_page(x);
			return page_folio(page);
		}

		/*
		 * compound_order() can be called without holding a reference, which means
		 * that niceties like page_folio() don't work.  These callers should be
		 * prepared to handle wild return values.  For example, PG_head may be
		 * set before _folio_order is initialised, or this may be a tail page.
		 * See compaction.c for some good examples.
		 */
		PREFIX_STATIC_INLINE
		uint
		compound_order(page_s *page) {
			folio_s *folio = (folio_s *)page;

			if (!test_bit(PG_head, &folio->flags))
				return 0;
			return folio->_folio_order;
		}
		/**
		 * folio_order - The allocation order of a folio.
		 * @folio: The folio.
		 *
		 * A folio is composed of 2^order pages.  See get_order() for the definition
		 * of order.
		 *
		 * Return: The order of the folio.
		 */
		PREFIX_STATIC_INLINE
		uint
		folio_order(folio_s *folio) {
			// if (!folio_test_large(folio))
			// 	return 0;
			return folio->_folio_order;
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
		void
		clear_page_pfmemalloc(page_s *page) {
			// page->lru.next = NULL;
			INIT_LIST_S(&page->lru);
		}

		PREFIX_STATIC_INLINE
		pgoff_t
		linear_page_index(vma_s *vma, ulong address) {
			pgoff_t pgoff;
			// if (unlikely(is_vm_hugetlb_page(vma)))
			// 	return linear_hugepage_index(vma, address);
			pgoff = (address - vma->vm_start) >> PAGE_SHIFT;
			pgoff += vma->vm_pgoff;
			return pgoff;
		}
		
		PREFIX_STATIC_INLINE
		bool
		page_is_guard(page_s *page) {
			return false;
		}


		PREFIX_STATIC_INLINE
		void
		set_page_count(page_s *page, int v) {
			atomic_set(&page->_refcount, v);
		}
		/*
		 * Setup the page count before being freed into the page allocator for
		 * the first time (boot or memory hotplug)
		 */
		PREFIX_STATIC_INLINE
		void
		init_page_count(page_s *page) {
			set_page_count(page, 1);
		}

		PREFIX_STATIC_INLINE
		zone_s
		*myos_page_zone(const page_s * page) {
			ulong pfn = page_to_pfn(page);
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
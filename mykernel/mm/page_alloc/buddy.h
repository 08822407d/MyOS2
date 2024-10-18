#ifndef _LINUX_PAGE_ALLOC_H_
#define _LINUX_PAGE_ALLOC_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern page_s
		*virt_to_head_page(const void *x);
		extern folio_s
		*virt_to_folio(const void *x);

		extern void
		page_ref_inc(page_s *page);
		extern void
		page_ref_dec(page_s *page);

		extern void
		folio_ref_inc(folio_s *folio);
		extern void
		folio_ref_dec(folio_s *folio);

		extern uint
		compound_order(page_s *page);

		extern uint
		folio_order(folio_s *folio);

		extern void
		folio_get(folio_s *folio);
		extern void
		folio_put(folio_s *folio);

		extern void
		get_page(page_s *page);
		extern void
		put_page(page_s *page);
		
		extern void
		clear_page_pfmemalloc(page_s *page);

		extern pgoff_t
		linear_page_index(vma_s *vma, ulong address);

		extern bool
		page_is_guard(page_s *page);

		extern void
		set_page_count(page_s *page, int v);
		extern void
		init_page_count(page_s *page);

		extern long
		folio_nr_pages(folio_s *folio);
		extern ulong
		compound_nr(page_s *page);	
		extern folio_s
		*folio_next(folio_s *folio);
		extern uint
		folio_shift(folio_s *folio);
		extern size_t
		folio_size(folio_s *folio);

		extern zone_s
		*myos_page_zone(const page_s * page);

	#endif

	#include "buddy_macro.h"
	
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

		PREFIX_STATIC_INLINE
		void
		page_ref_inc(page_s *page) {
			atomic_inc(&page->_refcount);
			// if (page_ref_tracepoint_active(page_ref_mod))
			// 	__page_ref_mod(page, 1);
		}
		PREFIX_STATIC_INLINE
		void
		page_ref_dec(page_s *page) {
			atomic_dec(&page->_refcount);
			// if (page_ref_tracepoint_active(page_ref_mod))
			// 	__page_ref_mod(page, -1);
		}

		PREFIX_STATIC_INLINE
		void
		folio_ref_inc(folio_s *folio) {
			page_ref_inc(&folio->page);
		}
		PREFIX_STATIC_INLINE
		void
		folio_ref_dec(folio_s *folio) {
			page_ref_dec(&folio->page);
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
			return folio->_flags_1 & 0xff;
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
			if (!folio_test_large(folio))
				return 0;
			return folio->_flags_1 & 0xff;
		}

		/**
		 * folio_get - Increment the reference count on a folio.
		 * @folio: The folio.
		 *
		 * Context: May be called in any context, as long as you know that
		 * you have a refcount on the folio.  If you do not already have one,
		 * folio_try_get() may be the right interface for you to use.
		 */
		PREFIX_STATIC_INLINE
		void
		folio_get(folio_s *folio) {
			// VM_BUG_ON_FOLIO(folio_ref_zero_or_close_to_overflow(folio), folio);
			folio_ref_inc(folio);
		}
		/**
		 * folio_put - Decrement the reference count on a folio.
		 * @folio: The folio.
		 *
		 * If the folio's reference count reaches zero, the memory will be
		 * released back to the page allocator and may be used by another
		 * allocation immediately.  Do not access the memory or the struct folio
		 * after calling folio_put() unless you can be sure that it wasn't the
		 * last reference.
		 *
		 * Context: May be called in process or interrupt context, but not in NMI
		 * context.  May be called while holding a spinlock.
		 */
		PREFIX_STATIC_INLINE
		void
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
		}

		PREFIX_STATIC_INLINE
		void
		get_page(page_s *page) {
			folio_get(page_folio(page));
		}
		PREFIX_STATIC_INLINE
		void
		put_page(page_s *page) {
			// /*
			//  * For some devmap managed pages we need to catch refcount transition
			//  * from 2 to 1:
			//  */
			// if (put_devmap_managed_page(&folio->page))
			// 	return;
			folio_put(page_folio(page));
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

		/**
		 * folio_nr_pages - The number of pages in the folio.
		 * @folio: The folio.
		 *
		 * Return: A positive power of two.
		 */
		PREFIX_STATIC_INLINE
		long
		folio_nr_pages(folio_s *folio) {
			if (!folio_test_large(folio))
				return 1;
			return folio->_folio_nr_pages;
		}
		/*
		 * compound_nr() returns the number of pages in this potentially compound
		 * page.  compound_nr() can be called on a tail page, and is defined to
		 * return 1 in that case.
		 */
		PREFIX_STATIC_INLINE
		ulong
		compound_nr(page_s *page) {
			folio_s *folio = (folio_s *)page;

			if (!test_bit(PG_head, &folio->flags))
				return 1;
			return folio->_folio_nr_pages;
		}
		/**
		 * folio_next - Move to the next physical folio.
		 * @folio: The folio we're currently operating on.
		 *
		 * If you have physically contiguous memory which may span more than
		 * one folio (eg a &struct bio_vec), use this function to move from one
		 * folio to the next.  Do not use it if the memory is only virtually
		 * contiguous as the folios are almost certainly not adjacent to each
		 * other.  This is the folio equivalent to writing ``page++``.
		 *
		 * Context: We assume that the folios are refcounted and/or locked at a
		 * higher level and do not adjust the reference counts.
		 * Return: The next struct folio.
		 */
		PREFIX_STATIC_INLINE
		folio_s
		*folio_next(folio_s *folio) {
			return (folio_s *)folio_page(folio, folio_nr_pages(folio));
		}
		/**
		 * folio_shift - The size of the memory described by this folio.
		 * @folio: The folio.
		 *
		 * A folio represents a number of bytes which is a power-of-two in size.
		 * This function tells you which power-of-two the folio is.  See also
		 * folio_size() and folio_order().
		 *
		 * Context: The caller should have a reference on the folio to prevent
		 * it from being split.  It is not necessary for the folio to be locked.
		 * Return: The base-2 logarithm of the size of this folio.
		 */
		PREFIX_STATIC_INLINE
		uint
		folio_shift(folio_s *folio) {
			return PAGE_SHIFT + folio_order(folio);
		}
		/**
		 * folio_size - The number of bytes in a folio.
		 * @folio: The folio.
		 *
		 * Context: The caller should have a reference on the folio to prevent
		 * it from being split.  It is not necessary for the folio to be locked.
		 * Return: The number of bytes in this folio.
		 */
		PREFIX_STATIC_INLINE
		size_t
		folio_size(folio_s *folio) {
			return PAGE_SIZE << folio_order(folio);
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


	/*==================================================================================*
	 *								"internal" functions								*
	 *==================================================================================*/
	#if defined(BUDDY_DEFINATION) || !(DEBUG)

		/*
		 * This function returns the order of a free page in the buddy system. In
		 * general, page_zone(page)->lock must be held by the caller to prevent the
		 * page from being allocated in parallel and returning garbage as the order.
		 * If a caller does not hold page_zone(page)->lock, it must guarantee that the
		 * page cannot be allocated or merged in parallel. Alternatively, it must
		 * handle invalid values gracefully, and use buddy_order_unsafe() below.
		 */
		static inline ulong
		buddy_order(page_s *page) {
			/* PageBuddy() must be checked by the caller */
			return page_private(page);
		}

		static void
		prep_compound_head(page_s *page, uint order) {
			folio_s *folio = (folio_s *)page;

		// static inline void folio_set_order(struct folio *folio, unsigned int order)
		// {
			if (WARN_ON_ONCE(!order || !folio_test_head(folio)))
				return;

			folio->_flags_1 = (folio->_flags_1 & ~0xffUL) | order;
			folio->_folio_nr_pages = 1U << order;
		// }
			atomic_set(&folio->_entire_mapcount, -1);
			atomic_set(&folio->_nr_pages_mapped, 0);
			atomic_set(&folio->_pincount, 0);
		}

		static void
		prep_compound_tail(page_s *head, int tail_idx) {
			page_s *p = head + tail_idx;

			// p->mapping = TAIL_MAPPING;
			set_compound_head(p, head);
			p->private = 0;
		}

		/*
		 * Locate the page_s for both the matching buddy in our
		 * pair (buddy1) and the combined O(n+1) page they form (page).
		 *
		 * 1) Any buddy B1 will have an order O twin B2 which satisfies
		 * the following equation:
		 *     B2 = B1 ^ (1 << O)
		 * For example, if the starting buddy (buddy2) is #8 its order
		 * 1 buddy is #10:
		 *     B2 = 8 ^ (1 << 1) = 8 ^ 2 = 10
		 *
		 * 2) Any buddy B will have an order O+1 parent P which
		 * satisfies the following equation:
		 *     P = B & ~(1 << O)
		 *
		 * Assumption: *_mem_map is contiguous at least up to MAX_ORDER
		 */
		static inline ulong
		__find_buddy_pfn(ulong page_pfn, uint order) {
			return page_pfn ^ (1 << order);
		}

		/*
		 * This function checks whether a page is free && is the buddy
		 * we can coalesce a page and its buddy if
		 * (a) the buddy is not in a hole (check before calling!) &&
		 * (b) the buddy is in the buddy system &&
		 * (c) a page and its buddy have the same order &&
		 * (d) a page and its buddy are in the same zone.
		 *
		 * For recording whether a page is in the buddy system, we set PageBuddy.
		 * Setting, clearing, and testing PageBuddy is serialized by zone->lock.
		 *
		 * For recording page's order, we use page_private(page).
		 */
		static inline bool
		page_is_buddy(page_s *page, page_s *buddy, uint order) {
			if (!page_is_guard(page) && !PageBuddy(buddy))
				return false;
			if (buddy_order(buddy) != order)
				return false;
			if (myos_page_zone(page) != myos_page_zone(buddy))
				return false;
			return true;
		}

		/*
		 * Find the buddy of @page and validate it.
		 * @page: The input page
		 * @pfn: The pfn of the page, it saves a call to page_to_pfn() when the
		 *       function is used in the performance-critical __free_one_page().
		 * @order: The order of the page
		 * @buddy_pfn: The output pointer to the buddy pfn, it also saves a call to
		 *             page_to_pfn().
		 *
		 * The found buddy can be a non PageBuddy, out of @page's zone, or its order is
		 * not the same as @page. The validation is necessary before use it.
		 *
		 * Return: the found buddy page or NULL if not found.
		 */
		static inline page_s
		*find_buddy_page_pfn(page_s *page, ulong pfn,
				uint order, ulong *buddy_pfn) {

			ulong __buddy_pfn = __find_buddy_pfn(pfn, order);
			page_s *buddy = page + (__buddy_pfn - pfn);
			if (buddy_pfn)
				*buddy_pfn = __buddy_pfn;

			if (page_is_buddy(page, buddy, order))
				return buddy;
			return NULL;
		}

	#endif

#endif /* _LINUX_PAGE_ALLOC_H_ */
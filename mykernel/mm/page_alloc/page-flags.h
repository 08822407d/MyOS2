/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Macros for manipulating and testing page->flags
 */

#ifndef _LINUX_PAGE_FLAGS_H_
#define _LINUX_PAGE_FLAGS_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		// compound pages / folio
		extern ulong
		_compound_head(const page_s *page);
		extern int
		PageTail(page_s *page);
		extern
		int PageCompound(page_s *page);
		extern ulong
		*folio_flags(folio_s *folio, uint n);
		extern void
		set_compound_head(page_s *page, page_s *head);


		// Slab / Slub
		extern bool
		folio_test_slab(folio_s *folio);
		extern int
		PageSlab(page_s *page);
		extern void
		__folio_set_slab(folio_s *folio);
		extern void
		__SetPageSlab(page_s *page);
		extern void
		__folio_clear_slab(folio_s *folio);
		extern void
		__ClearPageSlab(page_s *page);


		// reserved pages
		extern bool
		folio_test_reserved(folio_s *folio);
		extern int
		PageReserved(page_s *page);
		extern void
		folio_set_reserved(folio_s *folio);
		extern void
		SetPageReserved(page_s *page);
		extern void
		folio_clear_reserved(folio_s *folio);
		extern void
		ClearPageReserved(page_s *page);
		extern void
		__folio_clear_reserved(folio_s *folio);
		extern void
		__ClearPageReserved(page_s *page);
		extern void
		__folio_set_reserved(folio_s *folio);
		extern void
		__SetPageReserved(page_s *page);


		// compound head
		extern bool
		folio_test_head(folio_s *folio);
		extern void
		__folio_set_head(folio_s *folio);
		extern int
		PageHead(page_s *page);
		extern void
		__SetPageHead(page_s *page);
		extern void
		__folio_clear_head(folio_s *folio);
		extern void
		__ClearPageHead(page_s *page);
		extern void
		folio_clear_head(folio_s *folio);
		extern void
		ClearPageHead(page_s *page);


		// Page currently available in Buddy-System
		extern bool
		folio_test_buddy(const folio_s *folio);
		extern void
		__folio_set_buddy(folio_s *folio);
		extern void
		__folio_clear_buddy(folio_s *folio);
		extern int
		PageBuddy(page_s *page);
		extern void
		__SetPageBuddy(page_s *page);
		extern void
		__ClearPageBuddy(page_s *page);


		// Page is used for Mem-Mapping tables
		extern bool
		folio_test_pgtable(const folio_s *folio);
		extern void
		__folio_set_pgtable(folio_s *folio);
		extern void
		__folio_clear_pgtable(folio_s *folio);
		extern int
		PageTable(page_s *page);
		extern void
		__SetPageTable(page_s *page);
		extern void
		__ClearPageTable(page_s *page);

	#endif

	#include "page-flags_macro.h"
	
	#if defined(BUDDY_DEFINATION) || !(DEBUG)

		// compound pages / folio
		PREFIX_STATIC_INLINE
		ulong
		_compound_head(const page_s *page) {
			ulong head = READ_ONCE(page->compound_head);
			if (unlikely(head & 1))
				return head - 1;
			return (ulong)page;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int PageTail(page_s *page) {
			return READ_ONCE(page->compound_head) & 1;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int PageCompound(page_s *page) {
			return test_bit(PG_head, &page->flags) ||
					READ_ONCE(page->compound_head) & 1;
		}
		PREFIX_STATIC_INLINE
		ulong *folio_flags(folio_s *folio, uint n) {
			page_s *page = &folio->page;
			// VM_BUG_ON_PGFLAGS(PageTail(page), page);
			// VM_BUG_ON_PGFLAGS(n > 0 && !test_bit(PG_head, &page->flags), page);
			return &page[n].flags;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		set_compound_head(page_s *page, page_s *head) {
			WRITE_ONCE(page->compound_head, (ulong)head + 1);
		}


		// __PAGEFLAG(Locked, locked, PF_NO_TAIL)


		// PAGEFLAG(Referenced, referenced, PF_HEAD)
		// TESTCLEARFLAG(Referenced, referenced, PF_HEAD)
		// __SETPAGEFLAG(Referenced, referenced, PF_HEAD)


		// Uptodate Placeholder


		// PAGEFLAG(Dirty, dirty, PF_HEAD)
		// TESTSCFLAG(Dirty, dirty, PF_HEAD)
		// __CLEARPAGEFLAG(Dirty, dirty, PF_HEAD)


		// PAGEFLAG(LRU, lru, PF_HEAD)
		// __CLEARPAGEFLAG(LRU, lru, PF_HEAD)
		// TESTCLEARFLAG(LRU, lru, PF_HEAD)


		// PAGEFLAG(Active, active, PF_HEAD)
		// __CLEARPAGEFLAG(Active, active, PF_HEAD)
		// TESTCLEARFLAG(Active, active, PF_HEAD)


		// PAGEFLAG(Workingset, workingset, PF_HEAD)
		// TESTCLEARFLAG(Workingset, workingset, PF_HEAD)


		// PAGEFLAG(Waiters, waiters, PF_ONLY_HEAD)


		// PAGEFLAG(Error, error, PF_NO_TAIL)
		// TESTCLEARFLAG(Error, error, PF_NO_TAIL)


		// Slab / Slub
		// __PAGEFLAG(Slab, slab, PF_NO_TAIL)
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		folio_test_slab(folio_s *folio) {
			return (__builtin_constant_p((PG_slab)) ?
						constant_test_bit((PG_slab), (folio_flags(folio, 0))) :
						variable_test_bit((PG_slab), (folio_flags(folio, 0)))
					);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		PageSlab(page_s *page) {
			return (__builtin_constant_p((PG_slab)) ?
						constant_test_bit((PG_slab), (&((typeof(page))_compound_head(page))->flags)) :
						variable_test_bit((PG_slab), (&((typeof(page))_compound_head(page))->flags))
					);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_set_slab(folio_s *folio) {
			arch___set_bit(PG_slab, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageSlab(page_s *page) {
			arch___set_bit(PG_slab, &((typeof(page))_compound_head(page))->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_clear_slab(folio_s *folio) {
			arch___clear_bit(PG_slab, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageSlab(page_s *page) {
			arch___clear_bit(PG_slab, &((typeof(page))_compound_head(page))->flags);
		}


		// PAGEFLAG(OwnerPriv1, owner_priv_1, PF_ANY)
		// TESTCLEARFLAG(OwnerPriv1, owner_priv_1, PF_ANY)


		// Pages are reserved, for example MemBlock used pages
		// PAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
		// __CLEARPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
		// __SETPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		folio_test_reserved(folio_s *folio) {
			return (__builtin_constant_p((PG_reserved)) ?
						constant_test_bit((PG_reserved), (folio_flags(folio, 0))) :
						variable_test_bit((PG_reserved), (folio_flags(folio, 0)))
					);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		PageReserved(page_s *page) {
			return (__builtin_constant_p((PG_reserved)) ?
						constant_test_bit((PG_reserved), (&(page)->flags)) :
						variable_test_bit((PG_reserved), (&(page)->flags))
					);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		folio_set_reserved(folio_s *folio) {
			arch_set_bit(PG_reserved, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		SetPageReserved(page_s *page) {
			arch_set_bit(PG_reserved, &(page)->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		folio_clear_reserved(folio_s *folio) {
			arch_clear_bit(PG_reserved, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		ClearPageReserved(page_s *page) {
			arch_clear_bit(PG_reserved, &(page)->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_clear_reserved(folio_s *folio) {
			arch___clear_bit(PG_reserved, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageReserved(page_s *page) {
			arch___clear_bit(PG_reserved, &(page)->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_set_reserved(folio_s *folio) {
			arch___set_bit(PG_reserved, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageReserved(page_s *page) {
			arch___set_bit(PG_reserved, &(page)->flags);
		}


		// PAGEFLAG(Private, private, PF_ANY)
		// PAGEFLAG(Private2, private_2, PF_ANY)
		// TESTSCFLAG(Private2, private_2, PF_ANY)


		// /*
		//  * Only test-and-set exist for PG_writeback.  The unconditional operators are
		//  * risky: they bypass page accounting.
		//  */
		// TESTPAGEFLAG(Writeback, writeback, PF_NO_TAIL)
		// TESTSCFLAG(Writeback, writeback, PF_NO_TAIL)


		// compound head
		// __SETPAGEFLAG(Head, head, PF_ANY)
		// __CLEARPAGEFLAG(Head, head, PF_ANY)
		// CLEARPAGEFLAG(Head, head, PF_ANY)
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		folio_test_head(folio_s *folio) {
			return (__builtin_constant_p((PG_head)) ?
						constant_test_bit((PG_head), (folio_flags(folio, 0))) :
						variable_test_bit((PG_head), (folio_flags(folio, 0)))
					);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_set_head(folio_s *folio) {
			arch___set_bit(PG_head, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_clear_head(folio_s *folio) {
			arch___clear_bit(PG_head, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		PageHead(page_s *page) {
			// PF_POISONED_CHECK(page);
			return test_bit(PG_head, &page->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageHead(page_s *page) {
			arch___set_bit(PG_head, &(page)->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageHead(page_s *page) {
			arch___clear_bit(PG_head, &(page)->flags);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		folio_clear_head(folio_s *folio) {
			arch_clear_bit(PG_head, folio_flags(folio, 0));
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		ClearPageHead(page_s *page) {
			arch_clear_bit(PG_head, &(page)->flags);
		}


		// PAGEFLAG(MappedToDisk, mappedtodisk, PF_NO_TAIL)


		// /* PG_readahead is only used for reads; PG_reclaim is only for writes */
		// PAGEFLAG(Reclaim, reclaim, PF_NO_TAIL)
		// TESTCLEARFLAG(Reclaim, reclaim, PF_NO_TAIL)


		// PAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
		// __CLEARPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
		// __SETPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)


		// PAGEFLAG(Unevictable, unevictable, PF_HEAD)
		// __CLEARPAGEFLAG(Unevictable, unevictable, PF_HEAD)
		// TESTCLEARFLAG(Unevictable, unevictable, PF_HEAD)


		// PAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
		// __CLEARPAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
		// TESTSCFLAG(Mlocked, mlocked, PF_NO_TAIL)


		// PAGEFLAG(Uncached, uncached, PF_NO_COMPOUND)


		// PAGEFLAG(HWPoison, hwpoison, PF_ANY)
		// TESTSCFLAG(HWPoison, hwpoison, PF_ANY)


		// TESTPAGEFLAG(Young, young, PF_ANY)
		// SETPAGEFLAG(Young, young, PF_ANY)
		// TESTCLEARFLAG(Young, young, PF_ANY)
		// PAGEFLAG(Idle, idle, PF_ANY)



		/*
		 * PageBuddy() indicates that the page is free and in the buddy system
		 * (see mm/page_alloc.c).
		 */
		// PAGE_TYPE_OPS(Buddy, buddy, buddy)
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		folio_test_buddy(const folio_s *folio) {
			return ( (folio->page.page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_set_buddy(folio_s *folio) {
			folio->page.page_type &= ~PG_buddy;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_clear_buddy(folio_s *folio) {
			folio->page.page_type |= PG_buddy;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		PageBuddy(page_s *page) {
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageBuddy(page_s *page) {
			page->page_type &= ~PG_buddy;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageBuddy(page_s *page) {
			page->page_type |= PG_buddy;
		}


		// /*
		//  * PageOffline() indicates that the page is logically offline although the
		//  * containing section is online. (e.g. inflated in a balloon driver or
		//  * not onlined when onlining the section).
		//  * The content of these pages is effectively stale. Such pages should not
		//  * be touched (read/write/dump/save) except by their owner.
		//  *
		//  * If a driver wants to allow to offline unmovable PageOffline() pages without
		//  * putting them back to the buddy, it can do so via the memory notifier by
		//  * decrementing the reference count in MEM_GOING_OFFLINE and incrementing the
		//  * reference count in MEM_CANCEL_OFFLINE. When offlining, the PageOffline()
		//  * pages (now with a reference count of zero) are treated like free pages,
		//  * allowing the containing memory block to get offlined. A driver that
		//  * relies on this feature is aware that re-onlining the memory block will
		//  * require to re-set the pages PageOffline() and not giving them to the
		//  * buddy via online_page_callback_t.
		//  *
		//  * There are drivers that mark a page PageOffline() and expect there won't be
		//  * any further access to page content. PFN walkers that read content of random
		//  * pages should check PageOffline() and synchronize with such drivers using
		//  * page_offline_freeze()/page_offline_thaw().
		//  */
		// PAGE_TYPE_OPS(Offline, offline, offline)


		/*
		 * Marks pages in use as page tables.
		 */
		// PAGE_TYPE_OPS(Table, table, pgtable)
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		folio_test_pgtable(const folio_s *folio) {
			return ( (folio->page.page_type & (PAGE_TYPE_BASE | PG_table)) == PAGE_TYPE_BASE );
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_set_pgtable(folio_s *folio) {
			folio->page.page_type &= ~PG_table;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__folio_clear_pgtable(folio_s *folio) {
			folio->page.page_type |= PG_table;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		PageTable(page_s *page) {
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_table)) == PAGE_TYPE_BASE );
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageTable(page_s *page) {
			page->page_type &= ~PG_table;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageTable(page_s *page) {
			page->page_type |= PG_table;
		}


		// /*
		//  * Marks guardpages used with debug_pagealloc.
		//  */
		// PAGE_TYPE_OPS(Guard, guard, guard)

	#endif

#endif /* _LINUX_PAGE_FLAGS_H_ */

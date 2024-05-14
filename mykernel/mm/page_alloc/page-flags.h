/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Macros for manipulating and testing page->flags
 */

#ifndef PAGE_FLAGS_H
#define PAGE_FLAGS_H

	#include <linux/kernel/types.h>
	#include <linux/kernel/mm.h>
	#include <generated/bounds.h>

	#include <asm/insns.h>

	/*
	 * Various page->flags bits:
	 *
	 * PG_reserved is set for special pages. The "page_s" of such a page
	 * should in general not be touched (e.g. set dirty) except by its owner.
	 * Pages marked as PG_reserved include:
	 * - Pages part of the kernel image (including vDSO) and similar (e.g. BIOS,
	 *   initrd, HW tables)
	 * - Pages reserved or allocated early during boot (before the page allocator
	 *   was initialized). This includes (depending on the architecture) the
	 *   initial vmemmap, initial page tables, crashkernel, elfcorehdr, and much
	 *   much more. Once (if ever) freed, PG_reserved is cleared and they will
	 *   be given to the page allocator.
	 * - Pages falling into physical memory gaps - not IORESOURCE_SYSRAM. Trying
	 *   to read/write these pages might end badly. Don't touch!
	 * - The zero page(s)
	 * - Pages not added to the page allocator when onlining a section because
	 *   they were excluded via the online_page_callback() or because they are
	 *   PG_hwpoison.
	 * - Pages allocated in the context of kexec/kdump (loaded kernel image,
	 *   control pages, vmcoreinfo)
	 * - MMIO/DMA pages. Some architectures don't allow to ioremap pages that are
	 *   not marked PG_reserved (as they might be in use by somebody else who does
	 *   not respect the caching strategy).
	 * - Pages part of an offline section (page_ss of offline sections should
	 *   not be trusted as they will be initialized when first onlined).
	 * - MCA pages on ia64
	 * - Pages holding CPU notes for POWER Firmware Assisted Dump
	 * - Device memory (e.g. PMEM, DAX, HMM)
	 * Some PG_reserved pages will be excluded from the hibernation image.
	 * PG_reserved does in general not hinder anybody from dumping or swapping
	 * and is no longer required for remap_pfn_range(). ioremap might require it.
	 * Consequently, PG_reserved for a page mapped into user space can indicate
	 * the zero page, the vDSO, MMIO pages or device memory.
	 *
	 * The PG_private bitflag is set on pagecache pages if they contain filesystem
	 * specific data (which is normally at page->private). It can be used by
	 * private allocations for its own usage.
	 *
	 * During initiation of disk I/O, PG_locked is set. This bit is set before I/O
	 * and cleared when writeback _starts_ or when read _completes_. PG_writeback
	 * is set before writeback starts and cleared when it finishes.
	 *
	 * PG_locked also pins a page in pagecache, and blocks truncation of the file
	 * while it is held.
	 *
	 * page_waitqueue(page) is a wait queue of all tasks waiting for the page
	 * to become unlocked.
	 *
	 * PG_swapbacked is set when a page uses swap as a backing storage.  This are
	 * usually PageAnon or shmem pages but please note that even anonymous pages
	 * might lose their PG_swapbacked flag when they simply can be dropped (e.g. as
	 * a result of MADV_FREE).
	 *
	 * PG_referenced, PG_reclaim are used for page reclaim for anonymous and
	 * file-backed pagecache (see mm/vmscan.c).
	 *
	 * PG_error is set to indicate that an I/O error occurred on this page.
	 *
	 * PG_arch_1 is an architecture specific page state bit.  The generic code
	 * guarantees that this bit is cleared for a page when it first is entered into
	 * the page cache.
	 *
	 * PG_hwpoison indicates that a page got corrupted in hardware and contains
	 * data with incorrect ECC bits that triggered a machine check. Accessing is
	 * not safe since it may cause another machine check. Don't touch!
	 */

	#define PAGEFLAGS_MASK ((1UL << NR_PAGEFLAGS) - 1)

	#ifndef __GENERATING_BOUNDS_H

		static inline unsigned long
		_compound_head(const page_s *page) {
			unsigned long head = READ_ONCE(page->compound_head);
			if (head & 1) return head - 1;
			return (unsigned long)page;
		}
		#define compound_head(page)	((typeof(page))_compound_head(page))
		#define page_folio(p)		((folio_s *)_compound_head(p))

	// 	/**
	// 	 * page_folio - Converts from page to folio.
	// 	 * @p: The page.
	// 	 *
	// 	 * Every page is part of a folio.  This function cannot be called on a
	// 	 * NULL pointer.
	// 	 *
	// 	 * Context: No reference, nor lock is required on @page.  If the caller
	// 	 * does not hold a reference, this call may race with a folio split, so
	// 	 * it should re-check the folio still contains this page after gaining
	// 	 * a reference on the folio.
	// 	 * Return: The folio which contains this page.
	// 	 */
	// 	#define page_folio(p) (												\
	// 				_Generic(												\
	// 					(p),												\
	// 					const page_s * : (const folio_s *)_compound_head(p),\
	// 					page_s * : (folio_s *)_compound_head(p)				\
	// 				)														\
	// 			)

		/**
		 * folio_page - Return a page from a folio.
		 * @folio: The folio.
		 * @n: The page number to return.
		 *
		 * @n is relative to the start of the folio.  This function does not
		 * check that the page number lies within @folio; the caller is presumed
		 * to have a reference to the page.
		 */
	#	define folio_page(folio, n)	nth_page(&(folio)->page, n)

		// static __always_inline int PageTail(page_s *page)
		// {
		// 	return READ_ONCE(page->compound_head) & 1;
		// }

		// static __always_inline int PageCompound(page_s *page)
		// {
		// 	return test_bit(PG_head, &page->flags) || PageTail(page);
		// }

	// #	define PAGE_POISON_PATTERN -1L
	// 	static inline int
	// 	PagePoisoned(const page_s *page) {
	// 		return READ_ONCE(page->flags) == PAGE_POISON_PATTERN;
	// 	}

	// 	#ifdef CONFIG_DEBUG_VM
	// 		void page_init_poison(page_s *page, size_t size);
	// 	#else
	// 		static inline void page_init_poison(page_s *page, size_t size)
	// 		{
	// 		}
	// 	#endif

		// static unsigned long
		// *folio_flags(folio_s *folio, unsigned n) {
		// 	page_s *page = &folio->page;

		// 	// VM_BUG_ON_PGFLAGS(PageTail(page), page);
		// 	// VM_BUG_ON_PGFLAGS(n > 0 && !test_bit(PG_head, &page->flags), page);
		// 	return &page[n].flags;
		// }

		/*
		 * Page flags policies wrt compound pages
		 *
		 * PF_POISONED_CHECK
		 *     check if this page_s poisoned/uninitialized
		 *
		 * PF_ANY:
		 *     the page flag is relevant for small, head and tail pages.
		 *
		 * PF_HEAD:
		 *     for compound page all operations related to the page flag applied to
		 *     head page.
		 *
		 * PF_ONLY_HEAD:
		 *     for compound page, callers only ever operate on the head page.
		 *
		 * PF_NO_TAIL:
		 *     modifications of the page flag must be done on small or head pages,
		 *     checks can be done on tail pages too.
		 *
		 * PF_NO_COMPOUND:
		 *     the page flag is not relevant for compound pages.
		 *
		 * PF_SECOND:
		 *     the page flag is stored in the first tail page.
		 */
		// #define PF_POISONED_CHECK(page) ({								\
		// 			VM_BUG_ON_PGFLAGS(PagePoisoned(page), page); page;	\
		// 		})
		// #define PF_ANY(page, enforce) PF_POISONED_CHECK(page)
		// #define PF_HEAD(page, enforce) PF_POISONED_CHECK(compound_head(page))
		// #define PF_ONLY_HEAD(page, enforce) ({					\
		// 			VM_BUG_ON_PGFLAGS(PageTail(page), page);	\
		// 			PF_POISONED_CHECK(page);					\
		// 		})
		// #define PF_NO_TAIL(page, enforce) ({							\
		// 			VM_BUG_ON_PGFLAGS(enforce && PageTail(page), page);	\
		// 			PF_POISONED_CHECK(compound_head(page));				\
		// 		})
		// #define PF_NO_COMPOUND(page, enforce) ({							\
		// 			VM_BUG_ON_PGFLAGS(enforce && PageCompound(page), page);	\
		// 			PF_POISONED_CHECK(page);								\
		// 		})
		// #define PF_SECOND(page, enforce) ({						\
		// 			VM_BUG_ON_PGFLAGS(!PageHead(page), page);	\
		// 			PF_POISONED_CHECK(&page[1]);				\
		// 		})
		// for myos, mmdebug not implemented
		#define PF_ANY(page, enforce)			(page)
		#define PF_HEAD(page, enforce)			compound_head(page)
		#define PF_ONLY_HEAD(page, enforce)		(page)
		#define PF_NO_TAIL(page, enforce)		compound_head(page)
		#define PF_NO_COMPOUND(page, enforce)	(page)
		#define PF_SECOND(page, enforce)		(&page[1])

		/* Which page is the flag stored in */
		#define FOLIO_PF_ANY			0
		#define FOLIO_PF_HEAD			0
		#define FOLIO_PF_ONLY_HEAD		0
		#define FOLIO_PF_NO_TAIL		0
		#define FOLIO_PF_NO_COMPOUND	0
		#define FOLIO_PF_SECOND			1

		/*
		 * Macros to create function definitions for page flags
		 */
	#	define TESTPAGEFLAG(uname, lname, policy)					\
				static __always_inline int							\
				Page##uname(page_s *page) {							\
					return test_bit(PG_##lname,						\
							&policy(page, 0)->flags);				\
				}													\
				// static __always_inline bool							\
				// folio_test_##lname(folio_s *folio) {				\
				// 	return test_bit(PG_##lname,						\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define SETPAGEFLAG(uname, lname, policy)					\
				static __always_inline void							\
				SetPage##uname(page_s *page) {						\
					set_bit(PG_##lname, &policy(page, 1)->flags);	\
				}													\
				// static __always_inline void							\
				// folio_set_##lname(folio_s *folio) {					\
				// 	set_bit(PG_##lname,								\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define CLEARPAGEFLAG(uname, lname, policy)					\
				static __always_inline void							\
				ClearPage##uname(page_s *page) {					\
					clear_bit(PG_##lname,							\
							&policy(page, 1)->flags);				\
				}													\
				// static __always_inline void							\
				// folio_clear_##lname(folio_s *folio) {				\
				// 	clear_bit(PG_##lname,							\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define __SETPAGEFLAG(uname, lname, policy)					\
				static __always_inline void							\
				__SetPage##uname(page_s *page) {					\
					__set_bit(PG_##lname,							\
							&policy(page, 1)->flags);				\
				}													\
				// static __always_inline void							\
				// __folio_set_##lname(folio_s *folio) {				\
				// 	__set_bit(PG_##lname,							\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define __CLEARPAGEFLAG(uname, lname, policy)				\
				static __always_inline void							\
				__ClearPage##uname(page_s *page) {					\
					__clear_bit(PG_##lname,							\
					&policy(page, 1)->flags);						\
				}													\
				// static __always_inline void							\
				// __folio_clear_##lname(folio_s *folio) {				\
				// 	__clear_bit(PG_##lname,							\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define TESTSETFLAG(uname, lname, policy)					\
				static __always_inline int							\
				TestSetPage##uname(page_s *page) {					\
					return test_and_set_bit(PG_##lname,				\
							&policy(page, 1)->flags);				\
				}													\
				// static __always_inline bool							\
				// folio_test_set_##lname(folio_s *folio) {			\
				// 	return test_and_set_bit(PG_##lname,				\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define TESTCLEARFLAG(uname, lname, policy)					\
				static __always_inline int							\
				TestClearPage##uname(page_s *page) {				\
					return test_and_clear_bit(PG_##lname,			\
							&policy(page, 1)->flags);				\
				}													\
				// static __always_inline bool							\
				// folio_test_clear_##lname(folio_s *folio) {			\
				// 	return test_and_clear_bit(PG_##lname,			\
				// 			folio_flags(folio, FOLIO_##policy));	\
				// }

	#	define PAGEFLAG(uname, lname, policy)				\
					TESTPAGEFLAG(uname, lname, policy)		\
					SETPAGEFLAG(uname, lname, policy)		\
					CLEARPAGEFLAG(uname, lname, policy)

	#	define __PAGEFLAG(uname, lname, policy)				\
					TESTPAGEFLAG(uname, lname, policy)		\
					__SETPAGEFLAG(uname, lname, policy)		\
					__CLEARPAGEFLAG(uname, lname, policy)

	#	define TESTSCFLAG(uname, lname, policy)				\
					TESTSETFLAG(uname, lname, policy)		\
					TESTCLEARFLAG(uname, lname, policy)

	#	define TESTPAGEFLAG_FALSE(uname, lname)							\
					static inline int									\
					Page##uname(const page_s *page) { return 0; }		\
					static inline bool									\
					folio_test_##lname(const folio_s *folio) { return false; }

	#	define SETPAGEFLAG_NOOP(uname, lname)							\
					static inline void									\
					SetPage##uname(page_s *page) {}						\
					static inline void									\
					folio_set_##lname(folio_s *folio) {}

	#	define CLEARPAGEFLAG_NOOP(uname, lname)							\
					static inline void									\
					ClearPage##uname(page_s *page) {}					\
					static inline void									\
					folio_clear_##lname(folio_s *folio) {}

	#	define __CLEARPAGEFLAG_NOOP(uname, lname)						\
					static inline void									\
					__ClearPage##uname(page_s *page) {}					\
					static inline void									\
					__folio_clear_##lname(folio_s *folio) {}

	#	define TESTSETFLAG_FALSE(uname, lname)							\
					static inline int									\
					TestSetPage##uname(page_s *page) { return 0; }		\
					static inline bool									\
					folio_test_set_##lname(folio_s *folio) { return 0; }

	#	define TESTCLEARFLAG_FALSE(uname, lname)						\
					static inline int									\
					TestClearPage##uname(page_s *page) { return 0; }	\
					static inline bool									\
					folio_test_clear_##lname(folio_s *folio) { return 0; }                                                                \

	#	define PAGEFLAG_FALSE(uname, lname)				\
					TESTPAGEFLAG_FALSE(uname, lname)	\
					SETPAGEFLAG_NOOP(uname, lname)		\
					CLEARPAGEFLAG_NOOP(uname, lname)

	#	define TESTSCFLAG_FALSE(uname, lname)			\
					TESTSETFLAG_FALSE(uname, lname)		\
					TESTCLEARFLAG_FALSE(uname, lname)

		// __PAGEFLAG(Locked, locked, PF_NO_TAIL)

		// PAGEFLAG(Waiters, waiters, PF_ONLY_HEAD)
		// __CLEARPAGEFLAG(Waiters, waiters, PF_ONLY_HEAD)

		// PAGEFLAG(Error, error, PF_NO_TAIL)
		// TESTCLEARFLAG(Error, error, PF_NO_TAIL)

		// PAGEFLAG(Referenced, referenced, PF_HEAD)
		// TESTCLEARFLAG(Referenced, referenced, PF_HEAD)
		// __SETPAGEFLAG(Referenced, referenced, PF_HEAD)

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

		__PAGEFLAG(Slab, slab, PF_NO_TAIL)

		// PAGEFLAG(Checked, checked, PF_NO_COMPOUND) /* Used by some filesystems */

		// /* Xen */
		// PAGEFLAG(Pinned, pinned, PF_NO_COMPOUND)
		// TESTSCFLAG(Pinned, pinned, PF_NO_COMPOUND)

		// PAGEFLAG(SavePinned, savepinned, PF_NO_COMPOUND);

		// PAGEFLAG(Foreign, foreign, PF_NO_COMPOUND);

		// PAGEFLAG(XenRemapped, xen_remapped, PF_NO_COMPOUND)
		// TESTCLEARFLAG(XenRemapped, xen_remapped, PF_NO_COMPOUND)

		PAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
		__CLEARPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
		__SETPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)

		// PAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
		// __CLEARPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
		// __SETPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)

		// /*
		//  * Private page markings that may be used by the filesystem that owns the page
		//  * for its own purposes.
		//  * - PG_private and PG_private_2 cause releasepage() and co to be invoked
		//  */
		// PAGEFLAG(Private, private, PF_ANY)

		// PAGEFLAG(Private2, private_2, PF_ANY)
		// TESTSCFLAG(Private2, private_2, PF_ANY)

		// PAGEFLAG(OwnerPriv1, owner_priv_1, PF_ANY)
		// TESTCLEARFLAG(OwnerPriv1, owner_priv_1, PF_ANY)

		// /*
		//  * Only test-and-set exist for PG_writeback.  The unconditional operators are
		//  * risky: they bypass page accounting.
		//  */
		// TESTPAGEFLAG(Writeback, writeback, PF_NO_TAIL)
		// TESTSCFLAG(Writeback, writeback, PF_NO_TAIL)

		// PAGEFLAG(MappedToDisk, mappedtodisk, PF_NO_TAIL)

		// /* PG_readahead is only used for reads; PG_reclaim is only for writes */
		// PAGEFLAG(Reclaim, reclaim, PF_NO_TAIL)
		// TESTCLEARFLAG(Reclaim, reclaim, PF_NO_TAIL)

		// PAGEFLAG(Readahead, readahead, PF_NO_COMPOUND)
		// TESTCLEARFLAG(Readahead, readahead, PF_NO_COMPOUND)

		// PAGEFLAG_FALSE(HighMem, highmem)

		// static __always_inline bool
		// folio_test_swapcache(folio_s *folio) {
		// 	return folio_test_swapbacked(folio) &&
		// 		test_bit(PG_swapcache, folio_flags(folio, 0));
		// }

		// static __always_inline bool
		// PageSwapCache(page_s *page) {
		// 	return folio_test_swapcache(page_folio(page));
		// }

		// SETPAGEFLAG(SwapCache, swapcache, PF_NO_TAIL)
		// CLEARPAGEFLAG(SwapCache, swapcache, PF_NO_TAIL)


		// PAGEFLAG(Unevictable, unevictable, PF_HEAD)
		// __CLEARPAGEFLAG(Unevictable, unevictable, PF_HEAD)
		// TESTCLEARFLAG(Unevictable, unevictable, PF_HEAD)

	// 		PAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
	// 		__CLEARPAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
	// 		TESTSCFLAG(Mlocked, mlocked, PF_NO_TAIL)

	// 	#ifdef CONFIG_ARCH_USES_PG_UNCACHED
	// 		PAGEFLAG(Uncached, uncached, PF_NO_COMPOUND)
	// 	#else
	// 		PAGEFLAG_FALSE(Uncached, uncached)
	// 	#endif

	// 	#ifdef CONFIG_MEMORY_FAILURE
	// 		PAGEFLAG(HWPoison, hwpoison, PF_ANY)
	// 		TESTSCFLAG(HWPoison, hwpoison, PF_ANY)
	// 		#define __PG_HWPOISON (1UL << PG_hwpoison)
	// 		#define MAGIC_HWPOISON 0x48575053U /* HWPS */
	// 		extern void SetPageHWPoisonTakenOff(page_s *page);
	// 		extern void ClearPageHWPoisonTakenOff(page_s *page);
	// 		extern bool take_page_off_buddy(page_s *page);
	// 		extern bool put_page_back_buddy(page_s *page);
	// 	#else
	// 		PAGEFLAG_FALSE(HWPoison, hwpoison)
	// 		#define __PG_HWPOISON 0
	// 	#endif

	// 	#if defined(CONFIG_PAGE_IDLE_FLAG) && defined(CONFIG_64BIT)
	// 		TESTPAGEFLAG(Young, young, PF_ANY)
	// 		SETPAGEFLAG(Young, young, PF_ANY)
	// 		TESTCLEARFLAG(Young, young, PF_ANY)
	// 		PAGEFLAG(Idle, idle, PF_ANY)
	// 	#endif

	// 	#ifdef CONFIG_KASAN_HW_TAGS
	// 		PAGEFLAG(SkipKASanPoison, skip_kasan_poison, PF_HEAD)
	// 	#else
	// 		PAGEFLAG_FALSE(SkipKASanPoison, skip_kasan_poison)
	// 	#endif

		// /*
		//  * PageReported() is used to track reported free pages within the Buddy
		//  * allocator. We can use the non-atomic version of the test and set
		//  * operations as both should be shielded with the zone lock to prevent
		//  * any possible races on the setting or clearing of the bit.
		//  */
		// __PAGEFLAG(Reported, reported, PF_NO_COMPOUND)

	// 	/*
	// 	* On an anonymous page mapped into a user virtual memory area,
	// 	* page->mapping points to its anon_vma, not to a struct address_space;
	// 	* with the PAGE_MAPPING_ANON bit set to distinguish it.  See rmap.h.
	// 	*
	// 	* On an anonymous page in a VM_MERGEABLE area, if CONFIG_KSM is enabled,
	// 	* the PAGE_MAPPING_MOVABLE bit may be set along with the PAGE_MAPPING_ANON
	// 	* bit; and then page->mapping points, not to an anon_vma, but to a private
	// 	* structure which KSM associates with that merged page.  See ksm.h.
	// 	*
	// 	* PAGE_MAPPING_KSM without PAGE_MAPPING_ANON is used for non-lru movable
	// 	* page and then page->mapping points a struct address_space.
	// 	*
	// 	* Please note that, confusingly, "page_mapping" refers to the inode
	// 	* address_space which maps the page from disk; whereas "page_mapped"
	// 	* refers to user virtual address space into which the page is mapped.
	// 	*/
	// 	#define PAGE_MAPPING_ANON 0x1
	// 	#define PAGE_MAPPING_MOVABLE 0x2
	// 	#define PAGE_MAPPING_KSM (PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)
	// 	#define PAGE_MAPPING_FLAGS (PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)

	// 	static __always_inline int PageMappingFlags(page_s *page)
	// 	{
	// 		return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) != 0;
	// 	}

	// 	static __always_inline bool folio_test_anon(folio_s *folio)
	// 	{
	// 		return ((unsigned long)folio->mapping & PAGE_MAPPING_ANON) != 0;
	// 	}

	// 	static __always_inline bool PageAnon(page_s *page)
	// 	{
	// 		return folio_test_anon(page_folio(page));
	// 	}

	// 	static __always_inline int __PageMovable(page_s *page)
	// 	{
	// 		return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) ==
	// 			PAGE_MAPPING_MOVABLE;
	// 	}

	// 	#ifdef CONFIG_KSM
	// 		/*
	// 		* A KSM page is one of those write-protected "shared pages" or "merged pages"
	// 		* which KSM maps into multiple mms, wherever identical anonymous page content
	// 		* is found in VM_MERGEABLE vmas.  It's a PageAnon page, pointing not to any
	// 		* anon_vma, but to that page's node of the stable tree.
	// 		*/
	// 		static __always_inline bool folio_test_ksm(folio_s *folio)
	// 		{
	// 			return ((unsigned long)folio->mapping & PAGE_MAPPING_FLAGS) ==
	// 				PAGE_MAPPING_KSM;
	// 		}

	// 		static __always_inline bool PageKsm(page_s *page)
	// 		{
	// 			return folio_test_ksm(page_folio(page));
	// 		}
	// 	#else
	// 		TESTPAGEFLAG_FALSE(Ksm, ksm)
	// 	#endif

	// 	u64 stable_page_flags(page_s *page);

	// 	/**
	// 	 * folio_test_uptodate - Is this folio up to date?
	// 	 * @folio: The folio.
	// 	 *
	// 	 * The uptodate flag is set on a folio when every byte in the folio is
	// 	 * at least as new as the corresponding bytes on storage.  Anonymous
	// 	 * and CoW folios are always uptodate.  If the folio is not uptodate,
	// 	 * some of the bytes in it may be; see the is_partially_uptodate()
	// 	 * address_space operation.
	// 	 */
	// 	static inline bool folio_test_uptodate(folio_s *folio)
	// 	{
	// 		bool ret = test_bit(PG_uptodate, folio_flags(folio, 0));
	// 		/*
	// 		* Must ensure that the data we read out of the folio is loaded
	// 		* _after_ we've loaded folio->flags to check the uptodate bit.
	// 		* We can skip the barrier if the folio is not uptodate, because
	// 		* we wouldn't be reading anything from it.
	// 		*
	// 		* See folio_mark_uptodate() for the other side of the story.
	// 		*/
	// 		if (ret)
	// 			smp_rmb();

	// 		return ret;
	// 	}

	// 	static inline int PageUptodate(page_s *page)
	// 	{
	// 		return folio_test_uptodate(page_folio(page));
	// 	}

	// 	static __always_inline void __folio_mark_uptodate(folio_s *folio)
	// 	{
	// 		smp_wmb();
	// 		__set_bit(PG_uptodate, folio_flags(folio, 0));
	// 	}

	// 	static __always_inline void folio_mark_uptodate(folio_s *folio)
	// 	{
	// 		/*
	// 		* Memory barrier must be issued before setting the PG_uptodate bit,
	// 		* so that all previous stores issued in order to bring the folio
	// 		* uptodate are actually visible before folio_test_uptodate becomes true.
	// 		*/
	// 		smp_wmb();
	// 		set_bit(PG_uptodate, folio_flags(folio, 0));
	// 	}

	// 	static __always_inline void __SetPageUptodate(page_s *page)
	// 	{
	// 		__folio_mark_uptodate((folio_s *)page);
	// 	}

	// 	static __always_inline void SetPageUptodate(page_s *page)
	// 	{
	// 		folio_mark_uptodate((folio_s *)page);
	// 	}

		// CLEARPAGEFLAG(Uptodate, uptodate, PF_NO_TAIL)

	// 	bool __folio_start_writeback(folio_s *folio, bool keep_write);
	// 	bool set_page_writeback(page_s *page);

	// 	#define folio_start_writeback(folio) \
	// 		__folio_start_writeback(folio, false)
	// 	#define folio_start_writeback_keepwrite(folio) \
	// 		__folio_start_writeback(folio, true)

	// 	static inline void set_page_writeback_keepwrite(page_s *page)
	// 	{
	// 		folio_start_writeback_keepwrite(page_folio(page));
	// 	}

	// 	static inline bool test_set_page_writeback(page_s *page)
	// 	{
	// 		return set_page_writeback(page);
	// 	}

		__PAGEFLAG(Head, head, PF_ANY)
		CLEARPAGEFLAG(Head, head, PF_ANY)

	// 		/**
	// 		 * folio_test_large() - Does this folio contain more than one page?
	// 		 * @folio: The folio to test.
	// 		 *
	// 		 * Return: True if the folio is larger than one page.
	// 		 */
	// 		static inline bool folio_test_large(folio_s *folio)
	// 	{
	// 		return folio_test_head(folio);
	// 	}

		static __always_inline
		void set_compound_head(page_s *page, page_s *head) {
			WRITE_ONCE(page->compound_head, (unsigned long)head + 1);
		}

	// 	static __always_inline void clear_compound_head(page_s *page)
	// 	{
	// 		WRITE_ONCE(page->compound_head, 0);
	// 	}

	// 	#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	// 		static inline void ClearPageCompound(page_s *page)
	// 		{
	// 			BUG_ON(!PageHead(page));
	// 			ClearPageHead(page);
	// 		}
	// 	#endif

	// 	#define PG_head_mask ((1UL << PG_head))

	// 	#ifdef CONFIG_HUGETLB_PAGE
	// 		int PageHuge(page_s *page);
	// 		int PageHeadHuge(page_s *page);
	// 		static inline bool folio_test_hugetlb(folio_s *folio)
	// 		{
	// 			return PageHeadHuge(&folio->page);
	// 		}
	// 	#else
	// 		TESTPAGEFLAG_FALSE(Huge, hugetlb)
	// 		TESTPAGEFLAG_FALSE(HeadHuge, headhuge)
	// 	#endif

	// 	#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	// 		/*
	// 		* PageHuge() only returns true for hugetlbfs pages, but not for
	// 		* normal or transparent huge pages.
	// 		*
	// 		* PageTransHuge() returns true for both transparent huge and
	// 		* hugetlbfs pages, but not normal pages. PageTransHuge() can only be
	// 		* called only in the core VM paths where hugetlbfs pages can't exist.
	// 		*/
	// 		static inline int PageTransHuge(page_s *page)
	// 		{
	// 			VM_BUG_ON_PAGE(PageTail(page), page);
	// 			return PageHead(page);
	// 		}

	// 		static inline bool folio_test_transhuge(folio_s *folio)
	// 		{
	// 			return folio_test_head(folio);
	// 		}

	// 		/*
	// 		* PageTransCompound returns true for both transparent huge pages
	// 		* and hugetlbfs pages, so it should only be called when it's known
	// 		* that hugetlbfs pages aren't involved.
	// 		*/
	// 		static inline int PageTransCompound(page_s *page)
	// 		{
	// 			return PageCompound(page);
	// 		}

	// 		/*
	// 		* PageTransTail returns true for both transparent huge pages
	// 		* and hugetlbfs pages, so it should only be called when it's known
	// 		* that hugetlbfs pages aren't involved.
	// 		*/
	// 		static inline int PageTransTail(page_s *page)
	// 		{
	// 			return PageTail(page);
	// 		}

	// 		/*
	// 		* PageDoubleMap indicates that the compound page is mapped with PTEs as well
	// 		* as PMDs.
	// 		*
	// 		* This is required for optimization of rmap operations for THP: we can postpone
	// 		* per small page mapcount accounting (and its overhead from atomic operations)
	// 		* until the first PMD split.
	// 		*
	// 		* For the page PageDoubleMap means ->_mapcount in all sub-pages is offset up
	// 		* by one. This reference will go away with last compound_mapcount.
	// 		*
	// 		* See also __split_huge_pmd_locked() and page_remove_anon_compound_rmap().
	// 		*/
	// 		PAGEFLAG(DoubleMap, double_map, PF_SECOND)
	// 		TESTSCFLAG(DoubleMap, double_map, PF_SECOND)
	// 	#else
	// 		TESTPAGEFLAG_FALSE(TransHuge, transhuge)
	// 		TESTPAGEFLAG_FALSE(TransCompound, transcompound)
	// 		TESTPAGEFLAG_FALSE(TransCompoundMap, transcompoundmap)
	// 		TESTPAGEFLAG_FALSE(TransTail, transtail)
	// 		PAGEFLAG_FALSE(DoubleMap, double_map)
	// 		TESTSCFLAG_FALSE(DoubleMap, double_map)
	// 	#endif

	// 	#if defined(CONFIG_MEMORY_FAILURE) && defined(CONFIG_TRANSPARENT_HUGEPAGE)
	// 		/*
	// 		* PageHasHWPoisoned indicates that at least one subpage is hwpoisoned in the
	// 		* compound page.
	// 		*
	// 		* This flag is set by hwpoison handler.  Cleared by THP split or free page.
	// 		*/
	// 		PAGEFLAG(HasHWPoisoned, has_hwpoisoned, PF_SECOND)
	// 		TESTSCFLAG(HasHWPoisoned, has_hwpoisoned, PF_SECOND)
	// 	#else
	// 		PAGEFLAG_FALSE(HasHWPoisoned, has_hwpoisoned)
	// 		TESTSCFLAG_FALSE(HasHWPoisoned, has_hwpoisoned)
	// 	#endif

	// 	/*
	// 	* Check if a page is currently marked HWPoisoned. Note that this check is
	// 	* best effort only and inherently racy: there is no way to synchronize with
	// 	* failing hardware.
	// 	*/
	// 	static inline bool is_page_hwpoison(page_s *page)
	// 	{
	// 		if (PageHWPoison(page))
	// 			return true;
	// 		return PageHuge(page) && PageHWPoison(compound_head(page));
	// 	}

		/*
		 * For pages that are never mapped to userspace (and aren't PageSlab),
		 * page_type may be used.  Because it is initialised to -1, we invert the
		 * sense of the bit, so __SetPageFoo *clears* the bit used for PageFoo, and
		 * __ClearPageFoo *sets* the bit used for PageFoo.  We reserve a few high and
		 * low bits so that an underflow or overflow of page_mapcount() won't be
		 * mistaken for a page type value.
		 */

		#define PAGE_TYPE_BASE			0xf0000000
		/* Reserve		0x0000007f to catch underflows of page_mapcount */
		#define PAGE_MAPCOUNT_RESERVE	-128
		#define PG_buddy				0x00000080
		#define PG_offline				0x00000100
		#define PG_table				0x00000200
		#define PG_guard				0x00000400

		#define PageType(page, flag)				\
					((page->page_type &				\
						(PAGE_TYPE_BASE | flag)) ==	\
					PAGE_TYPE_BASE)

		// static inline int
		// page_has_type(page_s *page) {
		// 	return (int)page->page_type < PAGE_MAPCOUNT_RESERVE;
		// }

		#define PAGE_TYPE_OPS(uname, lname)							\
					static __always_inline int						\
					Page##uname(page_s *page) {						\
						return PageType(page, PG_##lname);			\
					}												\
					static __always_inline							\
					void __SetPage##uname(page_s *page) {			\
						/* VM_BUG_ON_PAGE(!PageType(page, 0), page); */	\
						page->page_type &= ~PG_##lname;				\
					}												\
					static __always_inline void						\
					__ClearPage##uname(page_s *page) {				\
						/* VM_BUG_ON_PAGE(!Page##uname(page), page); */	\
						page->page_type |= PG_##lname;				\
					}

		/*
		 * PageBuddy() indicates that the page is free and in the buddy system
		 * (see mm/page_alloc.c).
		 */
		PAGE_TYPE_OPS(Buddy, buddy)

	// 	/*
	// 	 * PageOffline() indicates that the page is logically offline although the
	// 	 * containing section is online. (e.g. inflated in a balloon driver or
	// 	 * not onlined when onlining the section).
	// 	 * The content of these pages is effectively stale. Such pages should not
	// 	 * be touched (read/write/dump/save) except by their owner.
	// 	 *
	// 	 * If a driver wants to allow to offline unmovable PageOffline() pages without
	// 	 * putting them back to the buddy, it can do so via the memory notifier by
	// 	 * decrementing the reference count in MEM_GOING_OFFLINE and incrementing the
	// 	 * reference count in MEM_CANCEL_OFFLINE. When offlining, the PageOffline()
	// 	 * pages (now with a reference count of zero) are treated like free pages,
	// 	 * allowing the containing memory block to get offlined. A driver that
	// 	 * relies on this feature is aware that re-onlining the memory block will
	// 	 * require to re-set the pages PageOffline() and not giving them to the
	// 	 * buddy via online_page_callback_t.
	// 	 *
	// 	 * There are drivers that mark a page PageOffline() and expect there won't be
	// 	 * any further access to page content. PFN walkers that read content of random
	// 	 * pages should check PageOffline() and synchronize with such drivers using
	// 	 * page_offline_freeze()/page_offline_thaw().
	// 	 */
	// 	PAGE_TYPE_OPS(Offline, offline)

	// // 	extern void page_offline_freeze(void);
	// // 	extern void page_offline_thaw(void);
	// // 	extern void page_offline_begin(void);
	// // 	extern void page_offline_end(void);

		/*
		 * Marks pages in use as page tables.
		 */
		PAGE_TYPE_OPS(Table, table)

	// 	/*
	// 	 * Marks guardpages used with debug_pagealloc.
	// 	 */
	// 	PAGE_TYPE_OPS(Guard, guard)

	// 	extern bool is_free_buddy_page(page_s *page);

	// 	__PAGEFLAG(Isolated, isolated, PF_ANY);

	// 		#define __PG_MLOCKED	(1UL << PG_mlocked)

	// 	/*
	// 	* Flags checked when a page is freed.  Pages being freed should not have
	// 	* these flags set.  If they are, there is a problem.
	// 	*/
	// 	#define PAGE_FLAGS_CHECK_AT_FREE                \
	// 			(1UL << PG_lru | 1UL << PG_locked |         \
	// 			1UL << PG_private | 1UL << PG_private_2 |  \
	// 			1UL << PG_writeback | 1UL << PG_reserved | \
	// 			1UL << PG_slab | 1UL << PG_active |        \
	// 			1UL << PG_unevictable | __PG_MLOCKED)

	// 	/*
	// 	* Flags checked when a page is prepped for return by the page allocator.
	// 	* Pages being prepped should not have these flags set.  If they are set,
	// 	* there has been a kernel bug or page_s corruption.
	// 	*
	// 	* __PG_HWPOISON is exceptional because it needs to be kept beyond page's
	// 	* alloc-free cycle to prevent from reusing the page.
	// 	*/
	// 	#define PAGE_FLAGS_CHECK_AT_PREP \
	// 			(PAGEFLAGS_MASK & ~__PG_HWPOISON)

	// 	#define PAGE_FLAGS_PRIVATE \
	// 			(1UL << PG_private | 1UL << PG_private_2)
	// 	/**
	// 	 * page_has_private - Determine if page has private stuff
	// 	 * @page: The page to be checked
	// 	 *
	// 	 * Determine if a page has private stuff, indicating that release routines
	// 	 * should be invoked upon it.
	// 	 */
	// 	static inline int page_has_private(page_s *page)
	// 	{
	// 		return !!(page->flags & PAGE_FLAGS_PRIVATE);
	// 	}

	// 	static inline bool folio_has_private(folio_s *folio)
	// 	{
	// 		return page_has_private(&folio->page);
	// 	}

	#	undef PF_ANY
	#	undef PF_HEAD
	#	undef PF_ONLY_HEAD
	#	undef PF_NO_TAIL
	#	undef PF_NO_COMPOUND
	#	undef PF_SECOND

	#endif /* !__GENERATING_BOUNDS_H */

	#define __folio_set_slab(folio)		__SetPageSlab(&(folio)->page)
	#define __folio_clear_slab(folio)	__ClearPageSlab(&(folio)->page)
	#define folio_test_slab(folio)		PageSlab(&(folio)->page)

#endif /* PAGE_FLAGS_H */

/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Macros for manipulating and testing page->flags
 */

#ifndef _LINUX_PAGE_FLAGS_MACRO_H_
#define _LINUX_PAGE_FLAGS_MACRO_H_

	#define PAGEFLAGS_MASK ((1UL << NR_PAGEFLAGS) - 1)

	#define compound_head(page)		((typeof(page))_compound_head(page))
	/**
	 * page_folio - Converts from page to folio.
	 * @p: The page.
	 *
	 * Every page is part of a folio.  This function cannot be called on a
	 * NULL pointer.
	 *
	 * Context: No reference, nor lock is required on @page.  If the caller
	 * does not hold a reference, this call may race with a folio split, so
	 * it should re-check the folio still contains this page after gaining
	 * a reference on the folio.
	 * Return: The folio which contains this page.
	 */
	#define page_folio(p)			(_Generic((p),						\
				const page_s *:		(const folio_s *)_compound_head(p), \
				page_s *:			(folio_s *)_compound_head(p)))

	/**
	 * folio_page - Return a page from a folio.
	 * @folio: The folio.
	 * @n: The page number to return.
	 *
	 * @n is relative to the start of the folio.  This function does not
	 * check that the page number lies within @folio; the caller is presumed
	 * to have a reference to the page.
	 */
	#define folio_page(folio, n)	nth_page(&(folio)->page, n)


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
	// #define PF_POISONED_CHECK(page) ({					\
	// 		VM_BUG_ON_PGFLAGS(PagePoisoned(page), page);		\
	// 		page; })
	// #define PF_ANY(page, enforce)	PF_POISONED_CHECK(page)
	// #define PF_HEAD(page, enforce)	PF_POISONED_CHECK(compound_head(page))
	// #define PF_ONLY_HEAD(page, enforce) ({					\
	// 		VM_BUG_ON_PGFLAGS(PageTail(page), page);		\
	// 		PF_POISONED_CHECK(page); })
	// #define PF_NO_TAIL(page, enforce) ({					\
	// 		VM_BUG_ON_PGFLAGS(enforce && PageTail(page), page);	\
	// 		PF_POISONED_CHECK(compound_head(page)); })
	// #define PF_NO_COMPOUND(page, enforce) ({				\
	// 		VM_BUG_ON_PGFLAGS(enforce && PageCompound(page), page);	\
	// 		PF_POISONED_CHECK(page); })
	// #define PF_SECOND(page, enforce) ({					\
	// 		VM_BUG_ON_PGFLAGS(!PageHead(page), page);		\
	// 		PF_POISONED_CHECK(&page[1]); })

	// for myos, mmdebug not implemented
	#define PF_ANY(page, enforce)			(page)
	#define PF_HEAD(page, enforce)			compound_head(page)
	#define PF_ONLY_HEAD(page, enforce)		(page)
	#define PF_NO_TAIL(page, enforce)		compound_head(page)
	#define PF_NO_COMPOUND(page, enforce)	(page)
	#define PF_SECOND(page, enforce)		(&page[1])


	/*
	 * Macros to create function definitions for page flags
	 */
	#define TESTPAGEFLAG(uname, lname, policy)					\
				static __always_inline							\
				bool folio_test_##lname(folio_s *folio) {		\
					return test_bit(PG_##lname,					\
							folio_flags(folio, FOLIO_##policy));\
				}												\
				static __always_inline							\
				int Page##uname(page_s *page) {			\
					return test_bit(PG_##lname,					\
							&policy(page, 0)->flags);			\
				}

	#define SETPAGEFLAG(uname, lname, policy)					\
				static __always_inline							\
				void folio_set_##lname(folio_s *folio) {		\
					set_bit(PG_##lname,							\
						folio_flags(folio, FOLIO_##policy));	\
				}												\
				static __always_inline void						\
				SetPage##uname(page_s *page) {				\
					set_bit(PG_##lname,							\
						&policy(page, 1)->flags);				\
				}

	#define CLEARPAGEFLAG(uname, lname, policy)					\
				static __always_inline							\
				void folio_clear_##lname(folio_s *folio) {		\
					clear_bit(PG_##lname,						\
						folio_flags(folio, FOLIO_##policy));	\
				}												\
				static __always_inline							\
				void ClearPage##uname(page_s *page) {		\
					clear_bit(PG_##lname,						\
						&policy(page, 1)->flags);				\
				}

	#define __SETPAGEFLAG(uname, lname, policy)					\
				static __always_inline							\
				void __folio_set_##lname(folio_s *folio) {		\
					__set_bit(PG_##lname,						\
						folio_flags(folio, FOLIO_##policy));	\
				}												\
				static __always_inline							\
				void __SetPage##uname(page_s *page) {		\
					__set_bit(PG_##lname,						\
						&policy(page, 1)->flags);				\
				}

	#define __CLEARPAGEFLAG(uname, lname, policy)				\
				static __always_inline							\
				void __folio_clear_##lname(folio_s *folio) {	\
					__clear_bit(PG_##lname,						\
						folio_flags(folio, FOLIO_##policy));	\
				}												\
				static __always_inline							\
				void __ClearPage##uname(page_s *page) {	\
					__clear_bit(PG_##lname,						\
						&policy(page, 1)->flags);				\
				}

	#define TESTSETFLAG(uname, lname, policy)					\
				static __always_inline							\
				bool folio_test_set_##lname(folio_s *folio) {	\
					return test_and_set_bit(PG_##lname,			\
							folio_flags(folio, FOLIO_##policy));\
				}												\
				static __always_inline							\
				int TestSetPage##uname(page_s *page) {		\
					return test_and_set_bit(PG_##lname,			\
							&policy(page, 1)->flags);			\
				}

	#define TESTCLEARFLAG(uname, lname, policy)					\
				static __always_inline							\
				bool folio_test_clear_##lname(folio_s *folio) {	\
					return test_and_clear_bit(PG_##lname,		\
							folio_flags(folio, FOLIO_##policy));\
				}												\
				static __always_inline							\
				int TestClearPage##uname(page_s *page) {	\
					return test_and_clear_bit(PG_##lname,		\
							&policy(page, 1)->flags);			\
				}


	#define PAGEFLAG(uname, lname, policy)				\
				TESTPAGEFLAG(uname, lname, policy)		\
				SETPAGEFLAG(uname, lname, policy)		\
				CLEARPAGEFLAG(uname, lname, policy)

	#define __PAGEFLAG(uname, lname, policy)			\
				TESTPAGEFLAG(uname, lname, policy)		\
				__SETPAGEFLAG(uname, lname, policy)		\
				__CLEARPAGEFLAG(uname, lname, policy)

	#define TESTSCFLAG(uname, lname, policy)			\
				TESTSETFLAG(uname, lname, policy)		\
				TESTCLEARFLAG(uname, lname, policy)


	/*
	 * For pages that are never mapped to userspace (and aren't PageSlab),
	 * page_type may be used.  Because it is initialised to -1, we invert the
	 * sense of the bit, so __SetPageFoo *clears* the bit used for PageFoo, and
	 * __ClearPageFoo *sets* the bit used for PageFoo.  We reserve a few high and
	 * low bits so that an underflow or overflow of page_mapcount() won't be
	 * mistaken for a page type value.
	 */
	#define PageType(page, flag) (								\
				(page->page_type & (PAGE_TYPE_BASE | flag)) ==	\
					PAGE_TYPE_BASE								\
			)
	#define folio_test_type(folio, flag) (						\
				(folio->page.page_type &						\
					(PAGE_TYPE_BASE | flag)) == PAGE_TYPE_BASE	\
			)

	#define FOLIO_TYPE_OPS(lname, fname)						\
				static __always_inline							\
				bool folio_test_##fname(const folio_s *folio) {	\
					return folio_test_type(folio, PG_##lname);	\
				}												\
				static __always_inline							\
				void __folio_set_##fname(folio_s *folio) {		\
					folio->page.page_type &= ~PG_##lname;		\
				}												\
				static __always_inline							\
				void __folio_clear_##fname(folio_s *folio) {	\
					folio->page.page_type |= PG_##lname;		\
				}

	#define PAGE_TYPE_OPS(uname, lname, fname)					\
				FOLIO_TYPE_OPS(lname, fname)					\
				static __always_inline int						\
				Page##uname(page_s *page) {						\
					return PageType(page, PG_##lname);			\
				}												\
				static __always_inline							\
				void __SetPage##uname(page_s *page) {			\
					page->page_type &= ~PG_##lname;				\
				}												\
				static __always_inline void						\
				__ClearPage##uname(page_s *page) {				\
					page->page_type |= PG_##lname;				\
				}


	#undef PF_ANY
	#undef PF_HEAD
	#undef PF_ONLY_HEAD
	#undef PF_NO_TAIL
	#undef PF_NO_COMPOUND
	#undef PF_SECOND

#endif /* _LINUX_PAGE_FLAGS_MACRO_H_ */

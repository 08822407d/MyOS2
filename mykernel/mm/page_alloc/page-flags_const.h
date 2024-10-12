/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PAGE_FLAGS_CONST_H_
#define _LINUX_PAGE_FLAGS_CONST_H_

	#define PAGEFLAGS_MASK			((1UL << NR_PAGEFLAGS) - 1)

	/* Which page is the flag stored in */
	#define FOLIO_PF_ANY			0
	#define FOLIO_PF_HEAD			0
	#define FOLIO_PF_ONLY_HEAD		0
	#define FOLIO_PF_NO_TAIL		0
	#define FOLIO_PF_NO_COMPOUND	0
	#define FOLIO_PF_SECOND			1


	#define PAGE_TYPE_BASE			0xf0000000
	/* Reserve		0x0000007f to catch underflows of page_mapcount */
	#define PAGE_MAPCOUNT_RESERVE	-128
	#define PG_buddy				0x00000080
	#define PG_offline				0x00000100
	#define PG_table				0x00000200
	#define PG_guard				0x00000400

	#define __PG_HWPOISON			(1UL << PG_hwpoison)
	#define MAGIC_HWPOISON			0x48575053U	/* HWPS */

	/*
	 * Flags checked when a page is prepped for return by the page allocator.
	 * Pages being prepped should not have these flags set.  If they are set,
	 * there has been a kernel bug or page_s corruption.
	 *
	 * __PG_HWPOISON is exceptional because it needs to be kept beyond page's
	 * alloc-free cycle to prevent from reusing the page.
	 */
	// #define PAGE_FLAGS_CHECK_AT_PREP	(			\
	// 			(PAGEFLAGS_MASK & ~__PG_HWPOISON) |	\
	// 			LRU_GEN_MASK | LRU_REFS_MASK		\
	// 		)
	#define PAGE_FLAGS_CHECK_AT_PREP	(			\
				(PAGEFLAGS_MASK & ~__PG_HWPOISON)	\
			)

#endif /* _LINUX_PAGE_FLAGS_CONST_H_ */

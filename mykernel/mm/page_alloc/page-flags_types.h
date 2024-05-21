/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PAGE_FLAGS_TYPES_H_
#define _LINUX_PAGE_FLAGS_TYPES_H_

	#include "../mm_type_declaration.h"


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
	/*
	 * Don't use the pageflags directly.  Use the PageFoo macros.
	 *
	 * The page flags field is split into two parts, the main flags area
	 * which extends from the low bits upwards, and the fields area which
	 * extends from the high bits downwards.
	 *
	 *  | FIELD | ... | FLAGS |
	 *  N-1           ^       0
	 *               (NR_PAGEFLAGS)
	 *
	 * The fields area is reserved for fields mapping zone, node (for NUMA) and
	 * SPARSEMEM section (for variants of SPARSEMEM that require section ids like
	 * SPARSEMEM_EXTREME with !SPARSEMEM_VMEMMAP).
	 */
	// detailed explanations:
	// https://www.kernel.org/doc/html/latest/admin-guide/mm/pagemap.html?highlight=kpageflags
	enum pageflags {
		PG_locked,			/* Page is locked. Don't touch. */
		PG_writeback,		/* Page is under writeback */
		PG_referenced,
		PG_uptodate,
		PG_dirty,
		PG_lru,
		PG_head,			/* Must be in bit 6 */
		PG_waiters,			/* Page has waiters, check its waitqueue. Must be bit #7 and in the same byte as "PG_locked" */
		PG_active,
		PG_workingset,
		PG_error,
		PG_slab,
		PG_owner_priv_1,	/* Owner use. If pagecache, fs may use*/
		PG_arch_1,
		PG_reserved,
		PG_private,			/* If pagecache, has fs-private data */
		PG_private_2,		/* If pagecache, has fs aux data */
		PG_mappedtodisk,	/* Has blocks allocated on-disk */
		PG_reclaim,			/* To be reclaimed asap */
		PG_swapbacked,		/* Page is backed by RAM/swap */
		PG_unevictable,		/* Page is "unevictable"  */
	#ifdef CONFIG_ARCH_USES_PG_UNCACHED
		PG_uncached,		/* Page has been mapped as uncached */
	#endif
	// #ifdef CONFIG_MEMORY_FAILURE
		PG_hwpoison,		/* hardware poisoned page. Don't touch */
	// #endif
	#if defined(CONFIG_PAGE_IDLE_FLAG)
		PG_young,
		PG_idle,
	#endif
	#ifdef CONFIG_ARCH_USES_PG_ARCH_X
		PG_arch_2,
		PG_arch_3,
	#endif
		__NR_PAGEFLAGS,

		PG_readahead		= PG_reclaim,

		/*
		* Depending on the way an anonymous folio can be mapped into a page
		* table (e.g., single PMD/PUD/CONT of the head page vs. PTE-mapped
		* THP), PG_anon_exclusive may be set only for the head page or for
		* tail pages of an anonymous folio. For now, we only expect it to be
		* set on tail pages for PTE-mapped THP.
		*/
		PG_anon_exclusive	= PG_mappedtodisk,

		/* Filesystems */
		PG_checked			= PG_owner_priv_1,

		/* SwapBacked */
		PG_swapcache		= PG_owner_priv_1,	/* Swap page: swp_entry_t in private */

		/* Two page bits are conscripted by FS-Cache to maintain local caching
		* state.  These bits are set on pages belonging to the netfs's inodes
		* when those inodes are being locally cached.
		*/
		PG_fscache			= PG_private_2,	/* page backed by cache */

		/* XEN */
		/* Pinned in Xen as a read-only pagetable page. */
		PG_pinned			= PG_owner_priv_1,
		/* Pinned as part of domain save (see xen_mm_pin_all()). */
		PG_savepinned		= PG_dirty,
		/* Has a grant mapping of another (foreign) domain's page. */
		PG_foreign			= PG_owner_priv_1,
		/* Remapped by swiotlb-xen. */
		PG_xen_remapped		= PG_owner_priv_1,

		/* non-lru isolated movable page */
		PG_isolated			= PG_reclaim,

		/* Only valid for buddy pages. Used to track pages that are reported */
		PG_reported			= PG_uptodate,

		/*
		* Flags only valid for compound pages.  Stored in first tail page's
		* flags word.  Cannot use the first 8 flags or any flag marked as
		* PF_ANY.
		*/

		/* At least one page in this folio has the hwpoison flag set */
		PG_has_hwpoisoned	= PG_error,
		PG_large_rmappable	= PG_workingset, /* anon or file-backed */
	};

	typedef struct pgflag_defs {
		unsigned long
			PG_locked		:1,
			PG_writeback	:1,
			PG_referenced	:1,
			PG_uptodate		:1,
			PG_dirty		:1,
			PG_lru			:1,
			PG_head			:1,
			PG_waiters		:1,
			PG_active		:1,
			PG_workingset	:1,
			PG_error		:1,
			PG_slab			:1,
			PG_owner_priv_1	:1,
			PG_arch_1		:1,
			PG_reserved		:1,
			PG_private		:1,
			PG_private_2	:1,
			PG_mappedtodisk	:1,
			PG_reclaim		:1,
			PG_swapbacked	:1,
			PG_unevictable	:1,
			PG_mlocked		:1,/* Page is vma mlocked */
		#ifdef CONFIG_ARCH_USES_PG_UNCACHED
			PG_uncached		:1,/* Page has been mapped as uncached */
		#endif
		#ifdef CONFIG_MEMORY_FAILURE
			PG_hwpoison		:1,/* hardware poisoned page. Don't touch */
		#endif
		#if defined(CONFIG_PAGE_IDLE_FLAG)
			PG_young		:1,
			PG_idle			:1,
		#endif
			PG_arch_2		:1,
			PG_arch_3		:1
			;
	}__attribute__((packed)) pgflag_defs_s;

#endif /* _LINUX_PAGE_FLAGS_TYPES_H_ */

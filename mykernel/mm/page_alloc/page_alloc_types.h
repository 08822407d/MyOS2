#ifndef _PAGE_ALLOC_TYPES_H_
#define _PAGE_ALLOC_TYPES_H_

	#include <linux/kernel/types.h>
	#include <linux/kernel/lock_ipc.h>


	struct mm_struct;
	typedef struct mm_struct mm_s;

	/*
	 * Each physical page in the system has a page_s associated with
	 * it to keep track of whatever it is we are using the page for at the
	 * moment. Note that we have no way to track which tasks are using
	 * a page, though if it is a pagecache page, rmap structures can tell us
	 * who is mapping it.
	 *
	 * If you allocate the page using alloc_pages(), you can use some of the
	 * space in page_s for your own purposes.  The five words in the main
	 * union are available, except for bit 0 of the first word which must be
	 * kept clear.  Many users use this word to store a pointer to an object
	 * which is guaranteed to be aligned.  If you use the same storage as
	 * page->mapping, you must restore it to NULL before freeing the page.
	 *
	 * If your page will not be mapped to userspace, you can also use the four
	 * bytes in the mapcount union, but you must call page_mapcount_reset()
	 * before freeing it.
	 *
	 * If you want to use the refcount field, it must be used in such a way
	 * that other CPUs temporarily incrementing and then decrementing the
	 * refcount does not cause problems.  On receiving the page from
	 * alloc_pages(), the refcount will be positive.
	 *
	 * If you allocate pages of order > 0, you can use some of the fields
	 * in each subpage, but you may need to restore some of their values
	 * afterwards.
	 *
	 * SLUB uses cmpxchg_double() to atomically update its freelist and counters.
	 * That requires that freelist & counters in struct slab be adjacent and
	 * double-word aligned. Because struct slab currently just reinterprets the
	 * bits of page_s, we align all page_ss to double-word boundaries,
	 * and ensure that 'freelist' is aligned within struct slab.
	 */
	#define _struct_page_alignment __aligned(2 * sizeof(unsigned long))

	typedef struct __attribute__((packed)) {
		unsigned long
			PG_locked		:1,
			PG_referenced	:1,
			PG_uptodate		:1,
			PG_dirty		:1,
			PG_lru			:1,
			PG_active		:1,
			PG_workingset	:1,
			PG_waiters		:1,
			PG_error		:1,
			PG_slab			:1,
			PG_owner_priv_1	:1,
			PG_arch_1		:1,
			PG_reserved		:1,
			PG_private		:1,
			PG_private_2	:1,
			PG_writeback	:1,
			PG_head			:1,
			PG_mappedtodisk	:1,
			PG_reclaim		:1,
			PG_swapbacked	:1,
			PG_unevictable	:1,
		#ifdef CONFIG_MMU
			PG_mlocked		:1,/* Page is vma mlocked */
		#endif
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
			PG_arch_2		:1
		#ifdef CONFIG_KASAN_HW_TAGS
			,
			PG_skip_kasan_poison	:1,
		#endif
			;
	} pgflag_defs_s;

	typedef struct page {
		union
		{
			unsigned long	flags;	/* Atomic flags, some possibly
									 * updated asynchronously */
			pgflag_defs_s	flag_defs;
		};
		
		/*
		 * Five words (20/40 bytes) are available in this union.
		 * WARNING: bit 0 of the first word is used for PageTail(). That
		 * means the other users of this union MUST NOT use the bit to
		 * avoid collision and false-positive PageTail().
		 */
		union
		{
			struct
			{	/* Page cache and anonymous pages */
				/**
				 * @lru: Pageout list, eg. active_list protected by
				 * lruvec->lru_lock.  Sometimes used as a generic list
				 * by the page owner.
				 */
				List_s		lru;
				// /* See page-flags.h for PAGE_MAPPING_FLAGS */
				// struct address_space *mapping;
				pgoff_t		index; /* Our offset within mapping. */
				/**
				 * @private: Mapping-private opaque data.
				 * Usually used for buffer_heads if PagePrivate.
				 * Used for swp_entry_t if PageSwapCache.
				 * Indicates order in the buddy system if PageBuddy.
				 */
				unsigned long	private;
			};
			// struct
			// { /* page_pool used by netstack */
			// 	/**
			// 	 * @pp_magic: magic value to avoid recycling non
			// 	 * page_pool allocated pages.
			// 	 */
			// 	unsigned long pp_magic;
			// 	page_s_pool *pp;
			// 	unsigned long _pp_mapping_pad;
			// 	unsigned long dma_addr;
			// 	union
			// 	{
			// 		/**
			// 		 * dma_addr_upper: might require a 64-bit
			// 		 * value on 32-bit architectures.
			// 		 */
			// 		unsigned long dma_addr_upper;
			// 		/**
			// 		 * For frag page support, not supported in
			// 		 * 32-bit architectures with 64-bit DMA.
			// 		 */
			// 		atomic_long_t pp_frag_count;
			// 	};
			// };
			struct
			{	/* Tail pages of compound page */
				unsigned long	compound_head;		/* Bit zero is set */

				/* First tail page only */
				unsigned char	compound_dtor;
				unsigned char	compound_order;
				atomic_t		compound_mapcount;
				unsigned int	compound_nr;		/* 1 << compound_order */
			};
			// struct
			// {	/* Second tail page of compound page */
			// 	unsigned long _compound_pad_1; /* compound_head */
			// 	atomic_t hpage_pinned_refcount;
			// 	/* For both global and memcg */
			// 	List_s deferred_list;
			// };
			struct
			{							 /* Page table pages */
				unsigned long _pt_pad_1; /* compound_head */
				pgtable_t pmd_huge_pte;	 /* protected by page->ptl */
				unsigned long _pt_pad_2; /* mapping */
				mm_s			*pt_mm;   /* x86 pgds only */
	// #if ALLOC_SPLIT_PTLOCKS
				// spinlock_t *ptl;
	// #else
				// spinlock_t ptl;
	// #endif
			};
			// struct
			// { /* ZONE_DEVICE pages */
			// 	/** @pgmap: Points to the hosting device page map. */
			// 	struct dev_pagemap *pgmap;
			// 	void *zone_device_data;
			// 	/*
			// 	* ZONE_DEVICE private pages are counted as being
			// 	* mapped so the next 3 words hold the mapping, index,
			// 	* and private fields from the source anonymous or
			// 	* page cache page while the page is migrated to device
			// 	* private memory.
			// 	* ZONE_DEVICE MEMORY_DEVICE_FS_DAX pages also
			// 	* use the mapping, index, and private fields when
			// 	* pmem backed DAX files are mapped.
			// 	*/
			// };

			// /** @rcu_head: You can use this to free a page by RCU. */
			// struct rcu_head rcu_head;
		};

		union
		{ /* This union is 4 bytes in size. */
			/*
			 * If the page can be mapped to userspace, encodes the number
			 * of times this page is referenced by a page table.
			 */
			atomic_t _mapcount;

			/*
			 * If the page is neither PageSlab nor mappable to userspace,
			 * the value stored here may help determine what this page
			 * is used for.  See page-flags.h for a list of page types
			 * which are currently stored here.
			 */
			unsigned int page_type;
		};

		/* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
		atomic_t _refcount;

	// #ifdef CONFIG_MEMCG
	// 	unsigned long memcg_data;
	// #endif

	// #ifdef LAST_CPUPID_NOT_IN_PAGE_FLAGS
	// 	int _last_cpupid;
	// #endif
	} page_s _struct_page_alignment;

	/**
	 * struct folio - Represents a contiguous set of bytes.
	 * @flags: Identical to the page flags.
	 * @lru: Least Recently Used list; tracks how recently this folio was used.
	 * @mapping: The file this page belongs to, or refers to the anon_vma for
	 *    anonymous memory.
	 * @index: Offset within the file, in units of pages.  For anonymous memory,
	 *    this is the index from the beginning of the mmap.
	 * @private: Filesystem per-folio data (see folio_attach_private()).
	 *    Used for swp_entry_t if folio_test_swapcache().
	 * @_mapcount: Do not access this member directly.  Use folio_mapcount() to
	 *    find out how many times this folio is mapped by userspace.
	 * @_refcount: Do not access this member directly.  Use folio_ref_count()
	 *    to find how many references there are to this folio.
	 * @memcg_data: Memory Control Group data.
	 *
	 * A folio is a physically, virtually and logically contiguous set
	 * of bytes.  It is a power-of-two in size, and it is aligned to that
	 * same power-of-two.  It is at least as large as %PAGE_SIZE.  If it is
	 * in the page cache, it is at a file offset which is a multiple of that
	 * power-of-two.  It may be mapped into userspace at an address which is
	 * at an arbitrary page offset, but its kernel virtual address is aligned
	 * to its size.
	 */
	typedef struct folio
	{
		/* private: don't document the anon union */
		union
		{
			struct
			{
				/* public: */
				unsigned long	flags;
				// List_s		lru;
				// struct address_space	*mapping;
				pgoff_t			index;
				void 			*private;
				atomic_t		_mapcount;
				atomic_t		_refcount;
	// #ifdef CONFIG_MEMCG
	// 			unsigned long	memcg_data;
	// #endif
				/* private: the union with page_s is transitional */
			};
			page_s page;
		};
	} folio_s;

#endif /* _PAGE_ALLOC_TYPES_H_ */
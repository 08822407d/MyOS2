/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RMAP_H
#define _LINUX_RMAP_H
/*
 * Declarations for Reverse Mapping functions in mm/rmap.c
 */

	#include <linux/lib/list.h>
	// #include <linux/kernel/slab.h>
	#include <linux/mm/mm.h>
	// #include <linux/rwsem.h>
	// #include <linux/memcontrol.h>
	// #include <linux/highmem.h>

	struct anon_vma;
	typedef struct anon_vma anon_vma_s;

	/*
	 * The anon_vma heads a list of private "related" vmas, to scan if
	 * an anonymous page pointing to this anon_vma needs to be unmapped:
	 * the vmas on the list will be related by forking, or by splitting.
	 *
	 * Since vmas come and go as they are split and merged (particularly
	 * in mprotect), the mapping field of an anonymous page cannot point
	 * directly to a vma: instead it points to an anon_vma, on whose list
	 * the related vmas can be easily linked or unlinked.
	 *
	 * After unlinking the last vma on the list, we must garbage collect
	 * the anon_vma object itself: we're guaranteed no page can be
	 * pointing to this anon_vma once its vma list is empty.
	 */
	typedef struct anon_vma {
		anon_vma_s	*root;		/* Root of this anon_vma tree */
		// struct rw_semaphore rwsem;	/* W: modification, R: walking the list */
		/*
		 * The refcount is taken on an anon_vma when there is no
		 * guarantee that the vma of page tables will exist for
		 * the duration of the operation. A caller that takes
		 * the reference is responsible for clearing up the
		 * anon_vma if they are the last user on release
		 */
		atomic_t	refcount;

		/*
		 * Count of child anon_vmas and VMAs which points to this anon_vma.
		 *
		 * This counter is used for making decision about reusing anon_vma
		 * instead of forking new one. See comments in function anon_vma_clone.
		 */
		unsigned	degree;

		anon_vma_s	*parent;	/* Parent of this anon_vma */

		// /*
		// * NOTE: the LSB of the rb_root.rb_node is set by
		// * mm_take_all_locks() _after_ taking the above lock. So the
		// * rb_root must only be read/written after taking the above lock
		// * to be sure to see a valid next pointer. The LSB bit itself
		// * is serialized by a system wide lock only visible to
		// * mm_take_all_locks() (mm_all_locks_mutex).
		// */

		// /* Interval tree of private "related" vmas */
		// struct rb_root_cached rb_root;
	} anon_vma_s;

	/*
	 * The copy-on-write semantics of fork mean that an anon_vma
	 * can become associated with multiple processes. Furthermore,
	 * each child process will have its own anon_vma, where new
	 * pages for that process are instantiated.
	 *
	 * This structure allows us to find the anon_vmas associated
	 * with a VMA, or the VMAs associated with an anon_vma.
	 * The "same_vma" list contains the anon_vma_chains linking
	 * all the anon_vmas associated with this VMA.
	 * The "rb" field indexes on an interval tree the anon_vma_chains
	 * which link all the VMAs associated with this anon_vma.
	 */
	typedef struct anon_vma_chain {
		vma_s		*vma;
		anon_vma_s	*anon_vma;
		List_hdr_s	same_vma; 	  /* locked by mmap_lock & page_table_lock */
		// struct rb_node rb;			/* locked by anon_vma->rwsem */
		// unsigned long rb_subtree_last;
	// #ifdef CONFIG_DEBUG_VM_RB
	// 	unsigned long cached_vma_start, cached_vma_last;
	// #endif
	} anon_vma_chain_s;

	// enum ttu_flags {
	// 	TTU_SPLIT_HUGE_PMD	= 0x4,	/* split huge PMD if any */
	// 	TTU_IGNORE_MLOCK	= 0x8,	/* ignore mlock */
	// 	TTU_SYNC		= 0x10,	/* avoid racy checks with PVMW_SYNC */
	// 	TTU_IGNORE_HWPOISON	= 0x20,	/* corrupted page is recoverable */
	// 	TTU_BATCH_FLUSH		= 0x40,	/* Batch TLB flushes where possible
	// 					* and caller guarantees they will
	// 					* do a final flush if necessary */
	// 	TTU_RMAP_LOCKED		= 0x80,	/* do not grab rmap lock:
	// 					* caller holds it */
	// };

	// #ifdef CONFIG_MMU
	// static inline void get_anon_vma(anon_vma_s *anon_vma)
	// {
	// 	atomic_inc(&anon_vma->refcount);
	// }

	// void __put_anon_vma(anon_vma_s *anon_vma);

	// static inline void put_anon_vma(anon_vma_s *anon_vma)
	// {
	// 	if (atomic_dec_and_test(&anon_vma->refcount))
	// 		__put_anon_vma(anon_vma);
	// }

	// static inline void anon_vma_lock_write(anon_vma_s *anon_vma)
	// {
	// 	down_write(&anon_vma->root->rwsem);
	// }

	// static inline void anon_vma_unlock_write(anon_vma_s *anon_vma)
	// {
	// 	up_write(&anon_vma->root->rwsem);
	// }

	// static inline void anon_vma_lock_read(anon_vma_s *anon_vma)
	// {
	// 	down_read(&anon_vma->root->rwsem);
	// }

	// static inline void anon_vma_unlock_read(anon_vma_s *anon_vma)
	// {
	// 	up_read(&anon_vma->root->rwsem);
	// }


	// /*
	// * anon_vma helper functions.
	// */
	// void anon_vma_init(void);	/* create anon_vma_cachep */
	// int  __anon_vma_prepare(vma_s *);
	void unlink_anon_vmas(vma_s *);
	int anon_vma_clone(vma_s *, vma_s *);
	// int anon_vma_fork(vma_s *, vma_s *);

	// static inline int anon_vma_prepare(vma_s *vma)
	// {
	// 	if (likely(vma->anon_vma))
	// 		return 0;

	// 	return __anon_vma_prepare(vma);
	// }

	// static inline void anon_vma_merge(vma_s *vma,
	// 				vma_s *next)
	// {
	// 	VM_BUG_ON_VMA(vma->anon_vma != next->anon_vma, vma);
	// 	unlink_anon_vmas(next);
	// }

	// anon_vma_s *page_get_anon_vma(page_s *page);

	// /* bitflags for do_page_add_anon_rmap() */
	// #define RMAP_EXCLUSIVE 0x01
	// #define RMAP_COMPOUND 0x02

	// /*
	// * rmap interfaces called when adding or removing pte of page
	// */
	// void page_move_anon_rmap(page_s *, vma_s *);
	// void page_add_anon_rmap(page_s *, vma_s *,
	// 		unsigned long, bool);
	// void do_page_add_anon_rmap(page_s *, vma_s *,
	// 			unsigned long, int);
	// void page_add_new_anon_rmap(page_s *, vma_s *,
	// 		unsigned long, bool);
	// void page_add_file_rmap(page_s *, bool);
	// void page_remove_rmap(page_s *, bool);

	// void hugepage_add_anon_rmap(page_s *, vma_s *,
	// 				unsigned long);
	// void hugepage_add_new_anon_rmap(page_s *, vma_s *,
	// 				unsigned long);

	// static inline void page_dup_rmap(page_s *page, bool compound)
	// {
	// 	atomic_inc(compound ? compound_mapcount_ptr(page) : &page->_mapcount);
	// }

	// /*
	// * Called from mm/vmscan.c to handle paging out
	// */
	// int page_referenced(page_s *, int is_locked,
	// 			struct mem_cgroup *memcg, unsigned long *vm_flags);

	// void try_to_migrate(page_s *page, enum ttu_flags flags);
	// void try_to_unmap(page_s *, enum ttu_flags flags);

	// int make_device_exclusive_range(mm_s *mm, unsigned long start,
	// 				unsigned long end, page_s **pages,
	// 				void *arg);

	// /* Avoid racy checks */
	// #define PVMW_SYNC		(1 << 0)
	// /* Look for migarion entries rather than present PTEs */
	// #define PVMW_MIGRATION		(1 << 1)

	// struct page_vma_mapped_walk {
	// 	page_s *page;
	// 	vma_s *vma;
	// 	unsigned long address;
	// 	pmd_t *pmd;
	// 	pte_t *pte;
	// 	spinlock_t *ptl;
	// 	unsigned int flags;
	// };

	// static inline void page_vma_mapped_walk_done(struct page_vma_mapped_walk *pvmw)
	// {
	// 	/* HugeTLB pte is set to the relevant page table entry without pte_mapped. */
	// 	if (pvmw->val && !PageHuge(pvmw->page))
	// 		pte_unmap(pvmw->val);
	// 	if (pvmw->ptl)
	// 		spin_unlock(pvmw->ptl);
	// }

	// bool page_vma_mapped_walk(struct page_vma_mapped_walk *pvmw);

	// /*
	// * Used by swapoff to help locate where page is expected in vma.
	// */
	// unsigned long page_address_in_vma(page_s *, vma_s *);

	// /*
	// * Cleans the PTEs of shared mappings.
	// * (and since clean PTEs should also be readonly, write protects them too)
	// *
	// * returns the number of cleaned PTEs.
	// */
	// int folio_mkclean(struct folio *);

	// /*
	// * called in munlock()/munmap() path to check for other vmas holding
	// * the page mlocked.
	// */
	// void page_mlock(page_s *page);

	// void remove_migration_ptes(page_s *old, page_s *new, bool locked);

	// /*
	// * Called by memory-failure.c to kill processes.
	// */
	// anon_vma_s *page_lock_anon_vma_read(page_s *page);
	// void page_unlock_anon_vma_read(anon_vma_s *anon_vma);
	// int page_mapped_in_vma(page_s *page, vma_s *vma);

	// /*
	// * rmap_walk_control: To control rmap traversing for specific needs
	// *
	// * arg: passed to rmap_one() and invalid_vma()
	// * rmap_one: executed on each vma where page is mapped
	// * done: for checking traversing termination condition
	// * anon_lock: for getting anon_lock by optimized way rather than default
	// * invalid_vma: for skipping uninterested vma
	// */
	// struct rmap_walk_control {
	// 	void *arg;
	// 	/*
	// 	* Return false if page table scanning in rmap_walk should be stopped.
	// 	* Otherwise, return true.
	// 	*/
	// 	bool (*rmap_one)(page_s *page, vma_s *vma,
	// 					unsigned long addr, void *arg);
	// 	int (*done)(page_s *page);
	// 	anon_vma_s *(*anon_lock)(page_s *page);
	// 	bool (*invalid_vma)(vma_s *vma, void *arg);
	// };

	// void rmap_walk(page_s *page, struct rmap_walk_control *rwc);
	// void rmap_walk_locked(page_s *page, struct rmap_walk_control *rwc);

	// #else	/* !CONFIG_MMU */

	// #define anon_vma_init()		do {} while (0)
	// #define anon_vma_prepare(vma)	(0)
	// #define anon_vma_link(vma)	do {} while (0)

	// static inline int page_referenced(page_s *page, int is_locked,
	// 				struct mem_cgroup *memcg,
	// 				unsigned long *vm_flags)
	// {
	// 	*vm_flags = 0;
	// 	return 0;
	// }

	// static inline void try_to_unmap(page_s *page, enum ttu_flags flags)
	// {
	// }

	// static inline int folio_mkclean(struct folio *folio)
	// {
	// 	return 0;
	// }
	// #endif	/* CONFIG_MMU */

	// static inline int page_mkclean(page_s *page)
	// {
	// 	return folio_mkclean(page_folio(page));
	// }

#endif	/* _LINUX_RMAP_H */

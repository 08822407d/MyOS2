/*
 * mm/rmap.c - physical to virtual reverse mappings
 *
 * Copyright 2001, Rik van Riel <riel@conectiva.com.br>
 * Released under the General Public License (GPL).
 *
 * Simple, low overhead reverse mapping scheme.
 * Please try to keep this thing as modular as possible.
 *
 * Provides methods for unmapping each kind of mapped page:
 * the anon methods track anonymous pages, and
 * the file methods track pages belonging to an inode.
 *
 * Original design by Rik van Riel <riel@conectiva.com.br> 2001
 * File methods by Dave McCracken <dmccr@us.ibm.com> 2003, 2004
 * Anonymous methods by Andrea Arcangeli <andrea@suse.de> 2004
 * Contributions by Hugh Dickins 2003, 2004
 */

/*
 * Lock ordering in mm:
 *
 * inode->i_rwsem	(while writing or truncating, not reading or faulting)
 *   mm->mmap_lock
 *     mapping->invalidate_lock (in filemap_fault)
 *       page->flags PG_locked (lock_page)   * (see hugetlbfs below)
 *         hugetlbfs_i_mmap_rwsem_key (in huge_pmd_share)
 *           mapping->i_mmap_rwsem
 *             hugetlb_fault_mutex (hugetlbfs specific page fault mutex)
 *             anon_vma->rwsem
 *               mm->page_table_lock or pte_lock
 *                 swap_lock (in swap_duplicate, swap_info_get)
 *                   mmlist_lock (in mmput, drain_mmlist and others)
 *                   mapping->private_lock (in __set_page_dirty_buffers)
 *                     lock_page_memcg move_lock (in __set_page_dirty_buffers)
 *                       i_pages lock (widely used)
 *                         lruvec->lru_lock (in folio_lruvec_lock_irq)
 *                   inode->i_lock (in set_page_dirty's __mark_inode_dirty)
 *                   bdi.wb->list_lock (in set_page_dirty's __mark_inode_dirty)
 *                     sb_lock (within inode_lock in fs/fs-writeback.c)
 *                     i_pages lock (widely used, in set_page_dirty,
 *                               in arch-dependent flush_dcache_mmap_lock,
 *                               within bdi.wb->list_lock in __sync_single_inode)
 *
 * anon_vma->rwsem,mapping->i_mmap_rwsem   (memory_failure, collect_procs_anon)
 *   ->tasklist_lock
 *     pte map lock
 *
 * * hugetlbfs PageHuge() pages take locks in this order:
 *         mapping->i_mmap_rwsem
 *           hugetlb_fault_mutex (hugetlbfs specific page fault mutex)
 *             page->flags PG_locked (lock_page)
 */

#include <linux/mm/mm.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/mm/pagemap.h>
// #include <linux/swap.h>
// #include <linux/swapops.h>
#include <linux/kernel/slab.h>
#include <linux/init/init.h>
// #include <linux/ksm.h>
#include <linux/mm/rmap.h>
// #include <linux/rcupdate.h>
// #include <linux/export.h>
// #include <linux/memcontrol.h>
// #include <linux/mmu_notifier.h>
// #include <linux/migrate.h>
// #include <linux/hugetlb.h>
// #include <linux/huge_mm.h>
// #include <linux/backing-dev.h>
// #include <linux/page_idle.h>
// #include <linux/memremap.h>
// #include <linux/userfaultfd_k.h>

// #include <asm/tlbflush.h>

// #include <trace/events/tlb.h>

#include "internal.h"

// static struct kmem_cache *anon_vma_cachep;
// static struct kmem_cache *anon_vma_chain_cachep;

// static inline anon_vma_s *anon_vma_alloc(void) {
// 	anon_vma_s *anon_vma;

// 	anon_vma = kmalloc(sizeof(anon_vma_s), GFP_KERNEL);
// 	if (anon_vma) {
// 		atomic_set(&anon_vma->refcount, 1);
// 		anon_vma->degree = 1;	/* Reference for first vma */
// 		anon_vma->parent = anon_vma;
// 		/*
// 		 * Initialise the anon_vma root to point to itself. If called
// 		 * from fork, the root will be reset to the parents anon_vma.
// 		 */
// 		anon_vma->root = anon_vma;
// 	}

// 	return anon_vma;
// }

// static inline void anon_vma_free(anon_vma_s *anon_vma) {
// 	// VM_BUG_ON(atomic_read(&anon_vma->refcount));

// 	/*
// 	 * Synchronize against page_lock_anon_vma_read() such that
// 	 * we can safely hold the lock without the anon_vma getting
// 	 * freed.
// 	 *
// 	 * Relies on the full mb implied by the atomic_dec_and_test() from
// 	 * put_anon_vma() against the acquire barrier implied by
// 	 * down_read_trylock() from page_lock_anon_vma_read(). This orders:
// 	 *
// 	 * page_lock_anon_vma_read()	VS	put_anon_vma()
// 	 *   down_read_trylock()		  atomic_dec_and_test()
// 	 *   LOCK				  MB
// 	 *   atomic_read()			  rwsem_is_locked()
// 	 *
// 	 * LOCK should suffice since the actual taking of the lock must
// 	 * happen _before_ what follows.
// 	 */
// 	// might_sleep();
// 	// if (rwsem_is_locked(&anon_vma->root->rwsem)) {
// 	// 	anon_vma_lock_write(anon_vma);
// 	// 	anon_vma_unlock_write(anon_vma);
// 	// }

// 	kfree(anon_vma);
// }

// static inline anon_vma_chain_s *anon_vma_chain_alloc(gfp_t gfp) {
// 	return kmalloc(sizeof(anon_vma_chain_s), gfp);
// }

// static void anon_vma_chain_free(anon_vma_chain_s *anon_vma_chain) {
// 	kfree(anon_vma_chain);
// }

// static void anon_vma_chain_link(vma_s *vma,
// 		anon_vma_chain_s *avc, anon_vma_s *anon_vma) {
// 	avc->vma = vma;
// 	avc->anon_vma = anon_vma;
// 	// list_add(&avc->same_vma, &vma->anon_vma_chain);
// 	// anon_vma_interval_tree_insert(avc, &anon_vma->rb_root);
// }


// /*
//  * Attach the anon_vmas from src to dst.
//  * Returns 0 on success, -ENOMEM on failure.
//  *
//  * anon_vma_clone() is called by __vma_adjust(), __split_vma(), copy_vma() and
//  * anon_vma_fork(). The first three want an exact copy of src, while the last
//  * one, anon_vma_fork(), may try to reuse an existing anon_vma to prevent
//  * endless growth of anon_vma. Since dst->anon_vma is set to NULL before call,
//  * we can identify this case by checking (!dst->anon_vma && src->anon_vma).
//  *
//  * If (!dst->anon_vma && src->anon_vma) is true, this function tries to find
//  * and reuse existing anon_vma which has no vmas and only one child anon_vma.
//  * This prevents degradation of anon_vma hierarchy to endless linear chain in
//  * case of constantly forking task. On the other hand, an anon_vma with more
//  * than one child isn't reused even if there was no alive vma, thus rmap
//  * walker has a good chance of avoiding scanning the whole hierarchy when it
//  * searches where page is mapped.
//  */
// int anon_vma_clone(vma_s *dst, vma_s *src)
// {
// 	anon_vma_chain_s *avc, *pavc;
// 	anon_vma_s *root = NULL;

// 	// list_for_each_entry_reverse(pavc, &src->anon_vma_chain, same_vma) {
// 	// 	anon_vma_s *anon_vma;

// 	// 	avc = anon_vma_chain_alloc(GFP_NOWAIT | __GFP_NOWARN);
// 	// 	if (unlikely(!avc)) {
// 	// 		unlock_anon_vma_root(root);
// 	// 		root = NULL;
// 	// 		avc = anon_vma_chain_alloc(GFP_KERNEL);
// 	// 		if (!avc)
// 	// 			goto enomem_failure;
// 	// 	}
// 	// 	anon_vma = pavc->anon_vma;
// 	// 	root = lock_anon_vma_root(root, anon_vma);
// 	// 	anon_vma_chain_link(dst, avc, anon_vma);

// 	// 	/*
// 	// 	 * Reuse existing anon_vma if its degree lower than two,
// 	// 	 * that means it has no vma and only one anon_vma child.
// 	// 	 *
// 	// 	 * Do not chose parent anon_vma, otherwise first child
// 	// 	 * will always reuse it. Root anon_vma is never reused:
// 	// 	 * it has self-parent reference and at least one child.
// 	// 	 */
// 	// 	if (!dst->anon_vma && src->anon_vma &&
// 	// 		anon_vma != src->anon_vma && anon_vma->degree < 2)
// 	// 		dst->anon_vma = anon_vma;
// 	// }
// 	// if (dst->anon_vma)
// 	// 	dst->anon_vma->degree++;
// 	// unlock_anon_vma_root(root);
// 	// return 0;

// //  enomem_failure:
// 	// /*
// 	//  * dst->anon_vma is dropped here otherwise its degree can be incorrectly
// 	//  * decremented in unlink_anon_vmas().
// 	//  * We can safely do this because callers of anon_vma_clone() don't care
// 	//  * about dst->anon_vma if anon_vma_clone() failed.
// 	//  */
// 	// dst->anon_vma = NULL;
// 	// unlink_anon_vmas(dst);
// 	// return -ENOMEM;
// }



// void unlink_anon_vmas(vma_s *vma)
// {
// 	anon_vma_chain_s *avc, *next;
// 	anon_vma_s *root = NULL;

// 	// /*
// 	//  * Unlink each anon_vma chained to the VMA.  This list is ordered
// 	//  * from newest to oldest, ensuring the root anon_vma gets freed last.
// 	//  */
// 	// list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
// 	// 	struct anon_vma *anon_vma = avc->anon_vma;

// 	// 	root = lock_anon_vma_root(root, anon_vma);
// 	// 	anon_vma_interval_tree_remove(avc, &anon_vma->rb_root);

// 	// 	/*
// 	// 	 * Leave empty anon_vmas on the list - we'll need
// 	// 	 * to free them outside the lock.
// 	// 	 */
// 	// 	if (RB_EMPTY_ROOT(&anon_vma->rb_root.rb_root)) {
// 	// 		anon_vma->parent->degree--;
// 	// 		continue;
// 	// 	}

// 	// 	list_del(&avc->same_vma);
// 	// 	anon_vma_chain_free(avc);
// 	// }
// 	// if (vma->anon_vma) {
// 	// 	vma->anon_vma->degree--;

// 	// 	/*
// 	// 	 * vma would still be needed after unlink, and anon_vma will be prepared
// 	// 	 * when handle fault.
// 	// 	 */
// 	// 	vma->anon_vma = NULL;
// 	// }
// 	// unlock_anon_vma_root(root);

// 	// /*
// 	//  * Iterate the list once more, it now only contains empty and unlinked
// 	//  * anon_vmas, destroy them. Could not do before due to __put_anon_vma()
// 	//  * needing to write-acquire the anon_vma->root->rwsem.
// 	//  */
// 	// list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
// 	// 	struct anon_vma *anon_vma = avc->anon_vma;

// 	// 	VM_WARN_ON(anon_vma->degree);
// 	// 	put_anon_vma(anon_vma);

// 	// 	list_del(&avc->same_vma);
// 	// 	anon_vma_chain_free(avc);
// 	// }
// }
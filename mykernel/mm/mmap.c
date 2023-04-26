// SPDX-License-Identifier: GPL-2.0-only
/*
 * mm/mmap.c
 *
 * Written by obz.
 *
 * Address space accounting code	<alan@lxorguk.ukuu.org.uk>
 */

// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/kernel.h>
#include <linux/kernel/slab.h>
// #include <linux/backing-dev.h>
#include <linux/mm/mm.h>
// #include <linux/mm_inline.h>
// #include <linux/vmacache.h>
// #include <linux/shm.h>
// #include <linux/mman.h>
// #include <linux/pagemap.h>
// #include <linux/swap.h>
#include <linux/kernel/syscalls.h>
// #include <linux/capability.h>
#include <linux/init/init.h>
#include <linux/fs/file.h>
#include <linux/fs/fs.h>
// #include <linux/personality.h>
// #include <linux/security.h>
// #include <linux/hugetlb.h>
#include <linux/mm/shmem_fs.h>
// #include <linux/profile.h>
// #include <linux/export.h>
#include <linux/kernel/mount.h>
// #include <linux/mempolicy.h>
// #include <linux/rmap.h>
// #include <linux/mmu_notifier.h>
// #include <linux/mmdebug.h>
// #include <linux/perf_event.h>
// #include <linux/audit.h>
// #include <linux/khugepaged.h>
// #include <linux/uprobes.h>
// #include <linux/rbtree_augmented.h>
// #include <linux/notifier.h>
// #include <linux/memory.h>
// #include <linux/printk.h>
// #include <linux/userfaultfd_k.h>
// #include <linux/moduleparam.h>
// #include <linux/pkeys.h>
// #include <linux/oom.h>
#include <linux/sched/mm.h>

// #include <linux/uaccess.h>
// #include <asm/cacheflush.h>
// #include <asm/tlb.h>
// #include <asm/mmu_context.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/mmap.h>

#include "internal.h"


#include <linux/sched/signal.h>


static bool ignore_rlimit_data = false;



static int
myos_if_vma_overlaps(mm_s *mm, unsigned long addr,
		unsigned long end, vma_s **pprev) {
	vma_s	*vma = NULL,
			*tmp = mm->mmap;

	while (tmp) {
		if (addr < tmp->vm_end && end > tmp->vm_start ||
			end > tmp->vm_start && addr < tmp->vm_end) {
			return -ENOMEM;
		}
		if (tmp->vm_next = mm->mmap)
			break;
		tmp = tmp->vm_next;
	}

	*pprev = NULL;
	return 0;
}

/*
 * vma_next() - Get the next VMA.
 * @mm: The mm_struct.
 * @vma: The current vma.
 *
 * If @vma is NULL, return the first vma in the mm.
 *
 * Returns: The next VMA after @vma.
 */
static inline vma_s
*vma_next(mm_s *mm, vma_s *vma) {
	if (!vma)
		return mm->mmap;

	return vma->vm_next;
}

/*
 * munmap_vma_range() - munmap VMAs that overlap a range.
 * @mm: The mm struct
 * @start: The start of the range.
 * @len: The length of the range.
 * @pprev: pointer to the pointer that will be set to previous vm_area_struct
 * @rb_link: the rb_node
 * @rb_parent: the parent rb_node
 *
 * Find all the vm_area_struct that overlap from @start to
 * @end and munmap them.  Set @pprev to the previous vm_area_struct.
 *
 * Returns: -ENOMEM on munmap failure or 0 on success.
 */
static inline int
munmap_vma_range(mm_s *mm, unsigned long start,
		unsigned long len, vma_s **pprev) {

	while (myos_if_vma_overlaps(mm, start, start + len, pprev))
		if (__do_munmap(mm, start, len, false))
			return -ENOMEM;

	return 0;
}



unsigned long
mmap_region(file_s *file, unsigned long addr, unsigned long len,
		vm_flags_t vm_flags, unsigned long pgoff, List_s *uf)
{
	mm_s *mm = current->mm;
	vma_s *vma, *prev, *merge;
	int error;
	// struct rb_node **rb_link, *rb_parent;
	unsigned long charged = 0;

	// /* Check against address space limit. */
	// if (!may_expand_vm(mm, vm_flags, len >> PAGE_SHIFT)) {
	// 	unsigned long nr_pages;

	// 	/*
	// 	 * MAP_FIXED may remove pages of mappings that intersects with
	// 	 * requested mapping. Account for the pages it would unmap.
	// 	 */
	// 	nr_pages = count_vma_pages_range(mm, addr, addr + len);

	// 	if (!may_expand_vm(mm, vm_flags,
	// 				(len >> PAGE_SHIFT) - nr_pages))
	// 		return -ENOMEM;
	// }

	/* Clear old maps, set up prev, rb_link, rb_parent, and uf */
	if (munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
// 	/*
// 	 * Private writable mapping: check memory availability
// 	 */
// 	if (accountable_mapping(file, vm_flags)) {
// 		charged = len >> PAGE_SHIFT;
// 		if (security_vm_enough_memory_mm(mm, charged))
// 			return -ENOMEM;
// 		vm_flags |= VM_ACCOUNT;
// 	}

// 	/*
// 	 * Can we just expand an old mapping?
// 	 */
// 	vma = vma_merge(mm, prev, addr, addr + len, vm_flags,
// 			NULL, file, pgoff, NULL, NULL_VM_UFFD_CTX, NULL);
// 	if (vma)
// 		goto out;

// 	/*
// 	 * Determine the object being mapped and call the appropriate
// 	 * specific mapper. the address has already been validated, but
// 	 * not unmapped, but the maps are removed from the list.
// 	 */
// 	vma = vm_area_alloc(mm);
// 	if (!vma) {
// 		error = -ENOMEM;
// 		goto unacct_error;
// 	}

// 	vma->vm_start = addr;
// 	vma->vm_end = addr + len;
// 	vma->vm_flags = vm_flags;
// 	vma->vm_page_prot = vm_get_page_prot(vm_flags);
// 	vma->vm_pgoff = pgoff;

// 	if (file) {
// 		if (vm_flags & VM_SHARED) {
// 			error = mapping_map_writable(file->f_mapping);
// 			if (error)
// 				goto free_vma;
// 		}

// 		vma->vm_file = get_file(file);
// 		error = call_mmap(file, vma);
// 		if (error)
// 			goto unmap_and_free_vma;

// 		/* Can addr have changed??
// 		 *
// 		 * Answer: Yes, several device drivers can do it in their
// 		 *         f_op->mmap method. -DaveM
// 		 * Bug: If addr is changed, prev, rb_link, rb_parent should
// 		 *      be updated for vma_link()
// 		 */
// 		WARN_ON_ONCE(addr != vma->vm_start);

// 		addr = vma->vm_start;

// 		/* If vm_flags changed after call_mmap(), we should try merge vma again
// 		 * as we may succeed this time.
// 		 */
// 		if (unlikely(vm_flags != vma->vm_flags && prev)) {
// 			merge = vma_merge(mm, prev, vma->vm_start, vma->vm_end, vma->vm_flags,
// 				NULL, vma->vm_file, vma->vm_pgoff, NULL, NULL_VM_UFFD_CTX, NULL);
// 			if (merge) {
// 				/* ->mmap() can change vma->vm_file and fput the original file. So
// 				 * fput the vma->vm_file here or we would add an extra fput for file
// 				 * and cause general protection fault ultimately.
// 				 */
// 				fput(vma->vm_file);
// 				vm_area_free(vma);
// 				vma = merge;
// 				/* Update vm_flags to pick up the change. */
// 				vm_flags = vma->vm_flags;
// 				goto unmap_writable;
// 			}
// 		}

// 		vm_flags = vma->vm_flags;
// 	} else if (vm_flags & VM_SHARED) {
// 		error = shmem_zero_setup(vma);
// 		if (error)
// 			goto free_vma;
// 	} else {
// 		vma_set_anonymous(vma);
// 	}

// 	/* Allow architectures to sanity-check the vm_flags */
// 	if (!arch_validate_flags(vma->vm_flags)) {
// 		error = -EINVAL;
// 		if (file)
// 			goto unmap_and_free_vma;
// 		else
// 			goto free_vma;
// 	}

// 	vma_link(mm, vma, prev, rb_link, rb_parent);
// 	/* Once vma denies write, undo our temporary denial count */
// unmap_writable:
// 	if (file && vm_flags & VM_SHARED)
// 		mapping_unmap_writable(file->f_mapping);
// 	file = vma->vm_file;
// out:
// 	perf_event_mmap(vma);

// 	vm_stat_account(mm, vm_flags, len >> PAGE_SHIFT);
// 	if (vm_flags & VM_LOCKED) {
// 		if ((vm_flags & VM_SPECIAL) || vma_is_dax(vma) ||
// 					is_vm_hugetlb_page(vma) ||
// 					vma == get_gate_vma(current->mm))
// 			vma->vm_flags &= VM_LOCKED_CLEAR_MASK;
// 		else
// 			mm->locked_vm += (len >> PAGE_SHIFT);
// 	}

// 	if (file)
// 		uprobe_mmap(vma);

// 	/*
// 	 * New (or expanded) vma always get soft dirty status.
// 	 * Otherwise user-space soft-dirty page tracker won't
// 	 * be able to distinguish situation when vma area unmapped,
// 	 * then new mapped in-place (which must be aimed as
// 	 * a completely new data area).
// 	 */
// 	vma->vm_flags |= VM_SOFTDIRTY;

// 	vma_set_page_prot(vma);

// 	return addr;

// unmap_and_free_vma:
// 	fput(vma->vm_file);
// 	vma->vm_file = NULL;

// 	/* Undo any partial mapping done by a device driver. */
// 	unmap_region(mm, vma, prev, vma->vm_start, vma->vm_end);
// 	charged = 0;
// 	if (vm_flags & VM_SHARED)
// 		mapping_unmap_writable(file->f_mapping);
// free_vma:
// 	vm_area_free(vma);
// unacct_error:
// 	if (charged)
// 		vm_unacct_memory(charged);
// 	return error;
}


/* Look up the first VMA which satisfies  addr < vm_end,  NULL if none. */
vma_s *myos_find_vma(mm_s *mm, unsigned long addr)
{
	vma_s	*vma = NULL,
			*tmp = mm->mmap;

	while (tmp) {
		if (addr >= tmp->vm_start && addr < tmp->vm_end ) {
			vma = tmp;
			break;
		}
		if (tmp->vm_next = mm->mmap)
			break;
		tmp = tmp->vm_next;
	}

	return vma;
}

/*
 * Same as find_vma, but also return a pointer to the previous VMA in *pprev.
 */
vma_s
*find_vma_prev(mm_s *mm, unsigned long addr, vma_s **pprev)
{
	vma_s *vma = NULL;

	vma = myos_find_vma(mm, addr);
	if (vma)
		*pprev = vma->vm_prev;

	return vma;
}




/* Munmap is split into 2 main parts -- this part which finds
 * what needs doing, and the areas themselves, which do the
 * work.  This now handles partial unmappings.
 * Jeremy Fitzhardinge <jeremy@goop.org>
 */
int __do_munmap(mm_s *mm, unsigned long start, size_t len, bool downgrade)
{
	// unsigned long end;
	// struct vm_area_struct *vma, *prev, *last;

	// if ((offset_in_page(start)) || start > TASK_SIZE || len > TASK_SIZE-start)
	// 	return -EINVAL;

	// len = PAGE_ALIGN(len);
	// end = start + len;
	// if (len == 0)
	// 	return -EINVAL;

	// /*
	//  * arch_unmap() might do unmaps itself.  It must be called
	//  * and finish any rbtree manipulation before this code
	//  * runs and also starts to manipulate the rbtree.
	//  */
	// arch_unmap(mm, start, end);

	// /* Find the first overlapping VMA where start < vma->vm_end */
	// vma = find_vma_intersection(mm, start, end);
	// if (!vma)
	// 	return 0;
	// prev = vma->vm_prev;

	// /*
	//  * If we need to split any vma, do it now to save pain later.
	//  *
	//  * Note: mremap's move_vma VM_ACCOUNT handling assumes a partially
	//  * unmapped vm_area_struct will remain in use: so lower split_vma
	//  * places tmp vma above, and higher split_vma places tmp vma below.
	//  */
	// if (start > vma->vm_start) {
	// 	int error;

	// 	/*
	// 	 * Make sure that map_count on return from munmap() will
	// 	 * not exceed its limit; but let map_count go just above
	// 	 * its limit temporarily, to help free resources as expected.
	// 	 */
	// 	if (end < vma->vm_end && mm->map_count >= sysctl_max_map_count)
	// 		return -ENOMEM;

	// 	error = __split_vma(mm, vma, start, 0);
	// 	if (error)
	// 		return error;
	// 	prev = vma;
	// }

	// /* Does it split the last one? */
	// last = find_vma(mm, end);
	// if (last && end > last->vm_start) {
	// 	int error = __split_vma(mm, last, end, 1);
	// 	if (error)
	// 		return error;
	// }
	// vma = vma_next(mm, prev);

	// if (unlikely(uf)) {
	// 	/*
	// 	 * If userfaultfd_unmap_prep returns an error the vmas
	// 	 * will remain split, but userland will get a
	// 	 * highly unexpected error anyway. This is no
	// 	 * different than the case where the first of the two
	// 	 * __split_vma fails, but we don't undo the first
	// 	 * split, despite we could. This is unlikely enough
	// 	 * failure that it's not worth optimizing it for.
	// 	 */
	// 	int error = userfaultfd_unmap_prep(vma, start, end, uf);
	// 	if (error)
	// 		return error;
	// }

	// /*
	//  * unlock any mlock()ed ranges before detaching vmas
	//  */
	// if (mm->locked_vm)
	// 	unlock_range(vma, end);

	// /* Detach vmas from rbtree */
	// if (!detach_vmas_to_be_unmapped(mm, vma, prev, end))
	// 	downgrade = false;

	// if (downgrade)
	// 	mmap_write_downgrade(mm);

	// unmap_region(mm, vma, prev, start, end);

	// /* Fix up all other VM information */
	// remove_vma_list(mm, vma);

	// return downgrade ? 1 : 0;
}
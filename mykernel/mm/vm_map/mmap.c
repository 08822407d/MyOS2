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
#include <linux/kernel/syscalls.h>
#include <linux/kernel/export.h>
#include <linux/kernel/mount.h>
#include <linux/kernel/uaccess.h>
#include <linux/init/init.h>
#include <linux/fs/file.h>
#include <linux/fs/fs.h>
#include <linux/fs/shmem_fs.h>
#include <linux/debug/printk.h>
#include <linux/sched/signal.h>


#include "../mm_types.h"


static bool ignore_rlimit_data = false;
ulong mmap_min_addr = PAGE_SIZE;


vma_s *vm_area_alloc(mm_s *mm)
{
	vma_s *vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (vma) vma_init(vma, mm);
	return vma;
}

vma_s *vm_area_creat_dup(vma_s *orig)
{
	vma_s *new = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);

	if (new) {
		// ASSERT_EXCLUSIVE_WRITER(orig->vm_flags);
		// ASSERT_EXCLUSIVE_WRITER(orig->vm_file);
		// /*
		//  * orig->shared.rb may be modified concurrently, but the clone
		//  * will be reinitialized.
		//  */
		// *new = data_race(*orig);
		*new = *orig;
		// INIT_LIST_HEAD(&new->anon_vma_chain);
		INIT_LIST_S(&new->list);
		// dup_anon_vma_name(orig, new);
	}
	return new;
}

void vm_area_free(vma_s *vma)
{
	// free_anon_vma_name(vma);
	kmem_cache_free(vm_area_cachep, vma);
}


static inline vma_s
*__get_vma_test_header(List_s *lp, List_hdr_s *lhdr) {
	if (lp == &lhdr->anchor) return NULL;
	else return LIST_TO_VMA(lp);
}

vma_s *vma_prev_vma(vma_s *vma) {
	// BUG_ON(vma == NULL);
	while (vma == NULL);
	
	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.prev;
	return __get_vma_test_header(lp, lhdr);
}

vma_s *vma_next_vma(vma_s *vma) {
	// BUG_ON(vma == NULL);
	while (vma == NULL);

	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.next;
	return __get_vma_test_header(lp, lhdr);
}

/*
 * vma_next() - Get the next VMA.
 * @mm: The mm_struct.
 * @vma: The current vma.
 *
 * If @vma is NULL, return the first vma in the mm.
 *
 * Returns: The next VMA after @vma.
 *			If reach list end, returns NULL
 */
vma_s *vma_next(mm_s *mm, vma_s *vma)
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));

	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.next;
	if (vma != NULL)
		lp = vma->list.next;
	return __get_vma_test_header(lp, lhdr);
}

vma_s *vma_prev(mm_s *mm, vma_s *vma)
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));

	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.prev;
	if (vma != NULL)
		lp = vma->list.prev;
	return __get_vma_test_header(lp, lhdr);
}


//	Linux implementation changed, proto not exist in linux-6.6
/*
 * simple_find_vma_links() - find the lowest vma which has intersection
 * with @addr-@end area and the prev of @vma
 * @mm: The mm struct
 * @pprev: A output variable stores the vma that just below the @addr-@end area
 * Returns: true on found a intersected vma
 */
static bool
simple_find_vma_links(mm_s *mm, ulong addr, ulong end, vma_s **pprev) {
	vma_s *vma = NULL;
	*pprev = NULL;

	for_each_vma(mm, vma) {
		// Update the vma in each iteration. When iteration breaks,
		// we found the true prev-vma of @addr-@end area
		*pprev = vma_prev_vma(vma);

		if ((addr >= vma->vm_start && addr < vma->vm_end) ||
				(end > vma->vm_start && end <= vma->vm_end))
			// case1:	Found intersected vma
			return true;
		else if (end <= vma->vm_start)
			// case2:	Since we iterate vmas from low-addr to high-addr,
			//			although there is no intersection, we still found
			//			a proper postion for @addr-@end area in vma-chain.
			//			Obviously the latter vmas won't have intersection
			//			with @addr-@end area, and they are all "next" vmas.
			break;
	}
	return false;
}


//	Linux implementation changed, proto not exist in linux-6.6
/*
 * munmap_vma_range() - munmap VMAs that overlap a range.
 * @mm: The mm struct
 * @start: The start of the range.
 * @len: The length of the range.
 * @pprev: pointer to the pointer that will be set to previous vm_area_struct
 *
 * Find all the vm_area_struct that overlap from @start to
 * @end and munmap them.  Set @pprev to the previous vm_area_struct.
 *
 * Returns: -ENOMEM on munmap failure or 0 on success.
 */
static inline int
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	while (simple_find_vma_links(mm, start, start + len, pprev))
		if (__do_munmap(mm, start, len, false))
			return -ENOMEM;
	return 0;
}


static void
__vma_link_file(vma_s *vma) {
	file_s *file;

	file = vma->vm_file;
	if (file) {
		// struct address_space *mapping = file->f_mapping;

		// if (vma->vm_flags & VM_SHARED)
		// 	mapping_allow_writable(mapping);

		// flush_dcache_mmap_lock(mapping);
		// vma_interval_tree_insert(vma, &mapping->i_mmap);
		// flush_dcache_mmap_unlock(mapping);
	}
}

// static int vma_link(struct mm_struct *mm, struct vm_area_struct *vma)
static void
simple_vma_link(mm_s *mm, vma_s *vma, vma_s *prev) {
	// struct address_space *mapping = NULL;

	if (vma->vm_file) {
		// mapping = vma->vm_file->f_mapping;
		// i_mmap_lock_write(mapping);
	}

	__vma_link_list(mm, vma, prev);
	__vma_link_file(vma);

	// if (mapping)
	// 	i_mmap_unlock_write(mapping);

	mm->map_count++;
	// validate_mm(mm);
}


//	Linux implementation changed, proto not exist in linux-6.6
/*
 * __simple_vma_adjust() - adjust @vma address bounds to @start and @end,
 * 			simultaneously due with overlapping situations
 * !!! Assumes that when overlapping situation happens, they are mergable !!!
 * @vma the vm_area to adjust 
 * @start vm_start adjust tartget
 * @end vm_end adjust target
 * 
 */
int __simple_vma_adjust(vma_s *vma, ulong start, ulong end)
{
	mm_s *mm = vma->vm_mm;
	// file_s *file = vma->vm_file;
	vma_s *next = vma_next_vma(vma);

	// adjust vma's bounds
	vma->vm_start = start;
	vma->vm_pgoff += (start - vma->vm_pgoff) >> PAGE_SHIFT;
	vma->vm_end = end;

	// due with totally overlapping situation
	while (next != NULL && end >= next->vm_end) {
		vma_s *tmp_next = next;
		next = vma_next_vma(next);
		__vma_unlink_list(mm, tmp_next);
	}
	next = vma_next_vma(vma);
	// due with partially overlapping situation
	if (next != NULL && end > next->vm_start) {
		next->vm_pgoff += (end - next->vm_start) >> PAGE_SHIFT;
		next->vm_start = end;
	}

	// validate_mm(mm);
	return 0;
}

/*
 * If the vma has a ->close operation then the driver probably needs to release
 * per-vma resources, so we don't attempt to merge those if the caller indicates
 * the current vma may be removed as part of the merge.
 */
// static inline bool is_mergeable_vma(struct vm_area_struct *vma,
// 		struct file *file, unsigned long vm_flags,
// 		struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
// 		struct anon_vma_name *anon_name, bool may_remove_vma)
static inline bool
simple_is_mergeable_vma(vma_s *vma, file_s *file, ulong vm_flags) {
	/*
	 * VM_SOFTDIRTY should not prevent from VMA merging, if we
	 * match the flags but dirty bit -- the caller should mark
	 * merged VMA as dirty. If dirty bit won't be excluded from
	 * comparison, we increase pressure on the memory system forcing
	 * the kernel to generate new VMAs when old one could be
	 * extended instead.
	 */
	if ((vma->vm_flags ^ vm_flags) & ~VM_SOFTDIRTY)
		return false;
	if (vma->vm_file != file)
		return false;
	if (vma->vm_ops && vma->vm_ops->close)
		return false;
	return true;
}

/*
 * Return true if we can merge this (vm_flags,anon_vma,file,vm_pgoff)
 * in front of (at a lower virtual address and file offset than) the vma.
 *
 * We cannot merge two vmas if they have differently assigned (non-NULL)
 * anon_vmas, nor if same anon_vma is assigned but offsets incompatible.
 *
 * We don't check here for the merged mmap wrapping around the end of pagecache
 * indices (16TB on ia32) because do_mmap() does not permit mmap's which
 * wrap, nor mmaps which cover the final page at index -1UL.
 *
 * We assume the vma may be removed as part of the merge.
 */
// static bool
// can_vma_merge_before(struct vm_area_struct *vma, unsigned long vm_flags,
// 		struct anon_vma *anon_vma, struct file *file,
// 		pgoff_t vm_pgoff, struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
// 		struct anon_vma_name *anon_name)
static bool
simple_can_vma_merge_before(vma_s *vma, ulong vm_flags,
		file_s *file, pgoff_t vm_pgoff) {

	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff == vm_pgoff))
		return true;
	return false;
}

/*
 * Return true if we can merge this (vm_flags,anon_vma,file,vm_pgoff)
 * beyond (at a higher virtual address and file offset than) the vma.
 *
 * We cannot merge two vmas if they have differently assigned (non-NULL)
 * anon_vmas, nor if same anon_vma is assigned but offsets incompatible.
 *
 * We assume that vma is not removed as part of the merge.
 */
// static bool
// can_vma_merge_after(struct vm_area_struct *vma, unsigned long vm_flags,
// 		struct anon_vma *anon_vma, struct file *file,
// 		pgoff_t vm_pgoff, struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
// 		struct anon_vma_name *anon_name)
static bool
simple_can_vma_merge_after(vma_s *vma, ulong vm_flags,
		file_s *file, pgoff_t vm_pgoff) {

	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff + vma_pages(vma) == vm_pgoff))
		return true;
	return false;
}

// Only adjust old vmas, won't creat new one for @addr-@end area
/*
 * Given a mapping request (addr,end,vm_flags,file,pgoff,anon_name),
 * figure out whether that can be merged with its predecessor or its
 * successor.  Or both (it neatly fills a hole).
 *
 * In most cases - when called for mmap, brk or mremap - [addr,end) is
 * certain not to be mapped by the time vma_merge is called; but when
 * called for mprotect, it is certain to be already mapped (either at
 * an offset within prev, or at the start of next), and the flags of
 * this area are about to be changed to vm_flags - and the no-change
 * case has already been eliminated.
 *
 * The following mprotect cases have to be considered, where **** is
 * the area passed down from mprotect_fixup, never extending beyond one
 * vma, PPPP is the previous vma, CCCC is a concurrent vma that starts
 * at the same address as **** and is of the same or larger span, and
 * NNNN the next vma after ****:
 *
 *     ****             ****                   ****
 *    PPPPPPNNNNNN    PPPPPPNNNNNN       PPPPPPCCCCCC
 *    cannot merge    might become       might become
 *                    PPNNNNNNNNNN       PPPPPPPPPPCC
 *    mmap, brk or    case 4 below       case 5 below
 *    mremap move:
 *                        ****               ****
 *                    PPPP    NNNN       PPPPCCCCNNNN
 *                    might become       might become
 *                    PPPPPPPPPPPP 1 or  PPPPPPPPPPPP 6 or
 *                    PPPPPPPPNNNN 2 or  PPPPPPPPNNNN 7 or
 *                    PPPPNNNNNNNN 3     PPPPNNNNNNNN 8
 *
 * It is important for case 8 that the vma CCCC overlapping the
 * region **** is never going to extended over NNNN. Instead NNNN must
 * be extended in region **** and CCCC must be removed. This way in
 * all cases where vma_merge succeeds, the moment vma_merge drops the
 * rmap_locks, the properties of the merged vma will be already
 * correct for the whole merged range. Some of those properties like
 * vm_page_prot/vm_flags may be accessed by rmap_walks and they must
 * be correct for the whole merged range immediately after the
 * rmap_locks are released. Otherwise if NNNN would be removed and
 * CCCC would be extended over the NNNN range, remove_migration_ptes
 * or other rmap walkers (if working on addresses beyond the "end"
 * parameter) may establish ptes with the wrong permissions of CCCC
 * instead of the right permissions of NNNN.
 *
 * In the code below:
 * PPPP is represented by *prev
 * CCCC is represented by *curr or not represented at all (NULL)
 * NNNN is represented by *next or not represented at all (NULL)
 * **** is not represented - it will be merged and the vma containing the
 *      area is returned, or the function will return NULL
 */
// struct vm_area_struct *vma_merge(struct vma_iterator *vmi, struct mm_struct *mm,
// 			struct vm_area_struct *prev, unsigned long addr,
// 			unsigned long end, unsigned long vm_flags,
// 			struct anon_vma *anon_vma, struct file *file,
// 			pgoff_t pgoff, struct mempolicy *policy,
// 			struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
// 			struct anon_vma_name *anon_name)
vma_s
*simple_vma_merge(mm_s *mm, vma_s *prev, ulong addr, ulong end,
		ulong vm_flags, file_s *file, pgoff_t pgoff)
{
	pgoff_t pglen = (end - addr) >> PAGE_SHIFT;
	vma_s *curr, *next, *adjust_target;
	int err;
	pgoff_t vma_pgoff;
	bool merge_prev = false;
	bool merge_next = false;
	ulong vma_start = addr;
	ulong vma_end = end;

	/*
	 * We later require that vma->vm_flags == vm_flags,
	 * so this tests vma->vm_flags & VM_SPECIAL, too.
	 */
	if (vm_flags & VM_SPECIAL)
		return NULL;

	/* Does the input range span an existing VMA? (cases 5 - 8) */
	curr = find_vma_intersection(mm, prev ? prev->vm_end : 0, end);
	if (curr == NULL ||					/* cases 1 - 4 */
			end == curr->vm_end)	/* cases 6 - 8, adjacent VMA */
		next = vma_lookup(mm, end);
	else
		next = NULL;				/* case 5 */
	
	/* Can we merge the predecessor? */
	if (prev && addr == prev->vm_end
			&& simple_can_vma_merge_after(prev, vm_flags, file, pgoff)) {
		vma_start = prev->vm_start;
		vma_pgoff = prev->vm_pgoff;
		adjust_target = prev;
		merge_prev = true;
	}
	/* Can we merge the successor? */
	if (next && simple_can_vma_merge_before(next,
			vm_flags, file, pgoff+pglen)) {
		vma_end = next->vm_end;
		if (!prev)
			adjust_target = next;
		merge_next = true;
	}
	if (adjust_target == NULL)
		adjust_target = curr;

	if (!merge_prev && !merge_next)
		return NULL; /* Not mergeable. */
	while (prev == NULL && curr == NULL && next == NULL);

	if (__simple_vma_adjust(adjust_target, vma_start, vma_end) != ENOERR)
		return NULL;
	else
		return adjust_target;

	// next = vma_next(mm, prev);
	// BUG_ON(next->vm_end == end);

	// /*
	//  * Can it merge with the @prev?
	//  */
	// if (prev && prev->vm_end == addr &&
	// 		simple_can_vma_merge_after(prev, vm_flags, file, pgoff)) {
	// 	/*
	// 	 * OK, it can.  Can we now merge in the successor as well?
	// 	 */
	// 	if (next && end == next->vm_start &&
	// 			simple_can_vma_merge_before(next, vm_flags, file, pgoff+pglen)) {
	// 		/* cases 1, 6 */
	// 		err = __simple_vma_adjust(prev, prev->vm_start, next->vm_end);
	// 	} else {
	// 		/* cases 2, 5, 7 */
	// 		err = __simple_vma_adjust(prev, prev->vm_start, end);
	// 	}
	// 	if (err)
	// 		return NULL;
	// 	return prev;
	// }

	// /*
	//  * Can this new request be merged in front of next?
	//  */
	// if (next && end == next->vm_start &&
	// 		simple_can_vma_merge_before(next, vm_flags, file, pgoff+pglen)) {
	// 	if (prev && addr < prev->vm_end) {
	// 		/* case 4 */
	// 		err = __simple_vma_adjust(prev, prev->vm_start, addr);
	// 	} else {
	// 		/* cases 3, 8 */
	// 		err = __simple_vma_adjust(curr, addr, next->vm_end);
	// 		/*
	// 		 * In case 3 area is already equal to next and
	// 		 * this is a noop, but in case 8 "area" has
	// 		 * been removed and next was expanded over it.
	// 		 */
	// 		curr = next;
	// 	}
	// 	if (err)
	// 		return NULL;
	// 	return curr;
	// }

	return NULL;
}


/*
 * The caller must write-lock current->mm->mmap_lock.
 */
// 代码分析文章 https://blog.csdn.net/gatieme/article/details/51628257
ulong
do_mmap(file_s *file, ulong addr, ulong len, ulong prot,
		ulong flags, ulong pgoff, ulong *populate)
{
	mm_s *mm = current->mm;
	vm_flags_t vm_flags;
	int pkey = 0;

	*populate = 0;

	if (!len)
		return -EINVAL;

	/*
	 * Does the application expect PROT_READ to imply PROT_EXEC?
	 *
	 * (the exception is when the underlying filesystem is noexec
	 *  mounted, in which case we dont add PROT_EXEC.)
	 */
	// if ((prot & PROT_READ) && (current->personality & READ_IMPLIES_EXEC))
	// 	if (!(file && path_noexec(&file->f_path)))
	// 		prot |= PROT_EXEC;

	/* force arch specific MAP_FIXED handling in get_unmapped_area */
	if (flags & MAP_FIXED_NOREPLACE)
		flags |= MAP_FIXED;

	// if (!(flags & MAP_FIXED))
	// 	addr = round_hint_to_min(addr);

	/* Careful about overflows.. */
	len = PAGE_ALIGN(len);
	if (!len)
		return -ENOMEM;

	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
		return -EOVERFLOW;

	// /* Too many mappings? */
	// if (mm->map_count > sysctl_max_map_count)
	// 	return -ENOMEM;

	/*
	 * Obtain the address to map to. we verify (or select) it and ensure
	 * that it represents a valid section of the address space.
	 */
	addr = get_unmapped_area(file, addr, len, pgoff, flags);
	if (IS_ERR_VALUE(addr))
		return addr;

	// if (flags & MAP_FIXED_NOREPLACE) {
	// 	if (find_vma_intersection(mm, addr, addr + len))
	// 		return -EEXIST;
	// }

	// if (prot == PROT_EXEC) {
	// 	pkey = execute_only_pkey(mm);
	// 	if (pkey < 0)
	// 		pkey = 0;
	// }

	/*
	 * Do simple checking here so the lower-level routines won't have
	 * to. we assume access permissions have been handled by the open
	 * of the memory object, so we don't do any here.
	 */
	vm_flags = calc_vm_prot_bits(prot, pkey) | calc_vm_flag_bits(flags) |
				mm->def_flags | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC;

	// if (flags & MAP_LOCKED)
	// 	if (!can_do_mlock())
	// 		return -EPERM;

	// if (mlock_future_check(mm, vm_flags, len))
	// 	return -EAGAIN;

	if (file) {
		// inode_s *inode = file_inode(file);
		// unsigned long flags_mask;

		// if (!file_mmap_ok(file, inode, pgoff, len))
		// 	return -EOVERFLOW;

		// flags_mask = LEGACY_MAP_MASK | file->f_op->mmap_supported_flags;

		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			/*
			 * Force use of MAP_SHARED_VALIDATE with non-legacy
			 * flags. E.g. MAP_SYNC is dangerous to use with
			 * MAP_SHARED as you don't know which consistency model
			 * you will get. We silently ignore unsupported flags
			 * with MAP_SHARED to preserve backward compatibility.
			 */
			flags &= LEGACY_MAP_MASK;
			fallthrough;
		case MAP_SHARED_VALIDATE:
			// if (flags & ~flags_mask)
			// 	return -EOPNOTSUPP;
			// if (prot & PROT_WRITE) {
			// 	if (!(file->f_mode & FMODE_WRITE))
			// 		return -EACCES;
			// 	if (IS_SWAPFILE(file->f_mapping->host))
			// 		return -ETXTBSY;
			// }

			// /*
			//  * Make sure we don't allow writing to an append-only
			//  * file..
			//  */
			// if (IS_APPEND(inode) && (file->f_mode & FMODE_WRITE))
			// 	return -EACCES;

			vm_flags |= VM_SHARED | VM_MAYSHARE;
			if (!(file->f_mode & FMODE_WRITE))
				vm_flags &= ~(VM_MAYWRITE | VM_SHARED);
			fallthrough;
		case MAP_PRIVATE:
			// if (!(file->f_mode & FMODE_READ))
			// 	return -EACCES;
			// if (path_noexec(&file->f_path)) {
			// 	if (vm_flags & VM_EXEC)
			// 		return -EPERM;
			// 	vm_flags &= ~VM_MAYEXEC;
			// }

			// if (!file->f_op->mmap)
			// 	return -ENODEV;
			// if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
			// 	return -EINVAL;
			break;

		default:
			return -EINVAL;
		}
	} else {
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
			/*
			 * Ignore pgoff.
			 */
			pgoff = 0;
			vm_flags |= VM_SHARED | VM_MAYSHARE;
			break;
		case MAP_PRIVATE:
			/*
			 * Set pgoff according to addr for anon_vma.
			 */
			pgoff = addr >> PAGE_SHIFT;
			break;
		default:
			return -EINVAL;
		}
	}

	// /*
	//  * Set 'VM_NORESERVE' if we should not account for the
	//  * memory use of this mapping.
	//  */
	// if (flags & MAP_NORESERVE) {
	// 	/* We honor MAP_NORESERVE if allowed to overcommit */
	// 	if (sysctl_overcommit_memory != OVERCOMMIT_NEVER)
	// 		vm_flags |= VM_NORESERVE;

	// 	/* hugetlb applies strict overcommit unless MAP_NORESERVE */
	// 	if (file && is_file_hugepages(file))
	// 		vm_flags |= VM_NORESERVE;
	// }

	addr = myos_mmap_region(file, addr, len, vm_flags, pgoff);
	// if (!IS_ERR_VALUE(addr) &&
	//     ((vm_flags & VM_LOCKED) ||
	//      (flags & (MAP_POPULATE | MAP_NONBLOCK)) == MAP_POPULATE))
	// 	*populate = len;
	return addr;
}


//	Linux proto:
//	unsigned long mmap_region(file_s *file, unsigned long addr,
// 		unsigned long len, vm_flags_t vm_flags, unsigned long pgoff,
// 		struct list_head *uf)
ulong
myos_mmap_region(file_s *file, ulong addr,
		ulong len, vm_flags_t vm_flags, ulong pgoff)
{
	mm_s *mm = current->mm;
	vma_s *vma, *prev, *merge;
	int error;
	// ulong charged = 0;

	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
	// /*
	//  * Private writable mapping: check memory availability
	//  */
	// if (accountable_mapping(file, vm_flags)) {
	// 	charged = len >> PAGE_SHIFT;
	// 	if (security_vm_enough_memory_mm(mm, charged))
	// 		return -ENOMEM;
	// 	vm_flags |= VM_ACCOUNT;
	// }

	/*
	 * Can we just expand an old mapping?
	 */
	vma = simple_vma_merge(mm, prev, addr, addr + len, vm_flags, file, pgoff);
	if (vma)
		goto out;

	/*
	 * Determine the object being mapped and call the appropriate
	 * specific mapper. the address has already been validated, but
	 * not unmapped, but the maps are removed from the list.
	 */
	vma = vm_area_alloc(mm);
	if (!vma) {
		error = -ENOMEM;
		goto unacct_error;
	}

	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	// vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;

	if (file) {
		// if (vm_flags & VM_SHARED) {
		// 	error = mapping_map_writable(file->f_mapping);
		// 	if (error)
		// 		goto free_vma;
		// }

		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error)
			goto unmap_and_free_vma;

		// /* Can addr have changed??
		//  *
		//  * Answer: Yes, several device drivers can do it in their
		//  *         f_op->mmap method. -DaveM
		//  * Bug: If addr is changed, prev, rb_link, rb_parent should
		//  *      be updated for simple_vma_link()
		//  */
		// WARN_ON_ONCE(addr != vma->vm_start);

		addr = vma->vm_start;

		/* If vm_flags changed after call_mmap(), we should try merge vma again
		 * as we may succeed this time.
		 */
		if (unlikely(vm_flags != vma->vm_flags && prev)) {
			merge = simple_vma_merge(mm, prev, vma->vm_start, vma->vm_end,
					vma->vm_flags, vma->vm_file, vma->vm_pgoff);
			if (merge) {
				/* ->mmap() can change vma->vm_file and fput the original file. So
				 * fput the vma->vm_file here or we would add an extra fput for file
				 * and cause general protection fault ultimately.
				 */
				fput(vma->vm_file);
				vm_area_free(vma);
				vma = merge;
				/* Update vm_flags to pick up the change. */
				vm_flags = vma->vm_flags;
				goto unmap_writable;
			}
		}

		vm_flags = vma->vm_flags;
	} else if (vm_flags & VM_SHARED) {
		// error = shmem_zero_setup(vma);
		// if (error)
		// 	goto free_vma;
	} else {
		vma_set_anonymous(vma);
	}

	// /* Allow architectures to sanity-check the vm_flags */
	// if (!arch_validate_flags(vma->vm_flags)) {
	// 	error = -EINVAL;
	// 	if (file)
	// 		goto unmap_and_free_vma;
	// 	else
	// 		goto free_vma;
	// }

	simple_vma_link(mm, vma, prev);
	// /* Once vma denies write, undo our temporary denial count */
unmap_writable:
	// if (file && vm_flags & VM_SHARED)
	// 	mapping_unmap_writable(file->f_mapping);
	file = vma->vm_file;
out:
	// perf_event_mmap(vma);

	// vm_stat_account(mm, vm_flags, len >> PAGE_SHIFT);
	// if (vm_flags & VM_LOCKED) {
	// 	if ((vm_flags & VM_SPECIAL) || vma_is_dax(vma) ||
	// 				is_vm_hugetlb_page(vma) ||
	// 				vma == get_gate_vma(current->mm))
	// 		vma->vm_flags &= VM_LOCKED_CLEAR_MASK;
	// 	else
	// 		mm->locked_vm += (len >> PAGE_SHIFT);
	// }

	// if (file)
	// 	uprobe_mmap(vma);

	/*
	 * New (or expanded) vma always get soft dirty status.
	 * Otherwise user-space soft-dirty page tracker won't
	 * be able to distinguish situation when vma area unmapped,
	 * then new mapped in-place (which must be aimed as
	 * a completely new data area).
	 */
	vma->vm_flags |= VM_SOFTDIRTY;

	// vma_set_page_prot(vma);

	return addr;

unmap_and_free_vma:
	fput(vma->vm_file);
	vma->vm_file = NULL;

	// /* Undo any partial mapping done by a device driver. */
	// unmap_region(mm, vma, prev, vma->vm_start, vma->vm_end);
	// charged = 0;
	// if (vm_flags & VM_SHARED)
	// 	mapping_unmap_writable(file->f_mapping);
free_vma:
	vm_area_free(vma);
unacct_error:
	// if (charged)
	// 	vm_unacct_memory(charged);
	return error;
}



/**
 * unmapped_area() - Find an area between the low_limit and the high_limit with
 * the correct alignment and offset, all from @info. Note: current->mm is used
 * for the search.
 *
 * @info: The unmapped area information including the range [low_limit -
 * high_limit), the alignment offset and mask.
 *
 * Return: A memory address or -ENOMEM.
 */
static ulong
simple_unmapped_area(vma_unmapped_info_s *info,
		ulong length, ulong low_limit, ulong high_limit) {
	ulong gap;
	vma_s *tmp;

	// gap = mas.index;
	// gap += (info->align_offset - gap) & info->align_mask;
	// tmp = mas_next(&mas, ULONG_MAX);
	// if (tmp && (tmp->vm_flags & VM_STARTGAP_FLAGS)) { /* Avoid prev check if possible */
	// 	if (vm_start_gap(tmp) < gap + length - 1) {
	// 		low_limit = tmp->vm_end;
	// 		mas_reset(&mas);
	// 		goto retry;
	// 	}
	// } else {
	// 	tmp = mas_prev(&mas, 0);
	// 	if (tmp && vm_end_gap(tmp) > gap) {
	// 		low_limit = vm_end_gap(tmp);
	// 		mas_reset(&mas);
	// 		goto retry;
	// 	}
	// }

	return gap;
}

/**
 * simple_unmapped_area_topdown() - Find an area between the low_limit and the
 * high_limit with the correct alignment and offset at the highest available
 * address, all from @info. Note: current->mm is used for the search.
 *
 * @info: The unmapped area information including the range [low_limit -
 * high_limit), the alignment offset and mask.
 *
 * Return: A memory address or -ENOMEM.
 */
static ulong
simple_unmapped_area_topdown(vma_unmapped_info_s *info,
		ulong length, ulong low_limit, ulong high_limit) {
	ulong gap, gap_end;
	vma_s *tmp;

	// gap = mas.last + 1 - info->length;
	// gap -= (gap - info->align_offset) & info->align_mask;
	// gap_end = mas.last;
	// tmp = mas_next(&mas, ULONG_MAX);
	// if (tmp && (tmp->vm_flags & VM_STARTGAP_FLAGS)) { /* Avoid prev check if possible */
	// 	if (vm_start_gap(tmp) <= gap_end) {
	// 		high_limit = vm_start_gap(tmp);
	// 		mas_reset(&mas);
	// 		goto retry;
	// 	}
	// } else {
	// 	tmp = mas_prev(&mas, 0);
	// 	if (tmp && vm_end_gap(tmp) > gap) {
	// 		high_limit = tmp->vm_start;
	// 		mas_reset(&mas);
	// 		goto retry;
	// 	}
	// }

	return gap;
}

/*
 * Search for an unmapped address range.
 *
 * We are looking for a range that:
 * - does not intersect with any VMA;
 * - is contained within the [low_limit, high_limit) interval;
 * - is at least the desired size.
 * - satisfies (begin_addr & align_mask) == (align_offset & align_mask)
 */
ulong vm_unmapped_area(vma_unmapped_info_s *info)
{
	ulong addr;
	ulong length, low_limit, high_limit;

	/* Adjust search length to account for worst case alignment overhead */
	length = info->length + info->align_mask;
	if (length < info->length)
		return -ENOMEM;

	low_limit = info->low_limit;
	if (low_limit < mmap_min_addr)
		low_limit = mmap_min_addr;
	high_limit = info->high_limit;

	if (info->flags & VM_UNMAPPED_AREA_TOPDOWN)
		addr = simple_unmapped_area_topdown(info, length, low_limit, high_limit);
	else
		addr = simple_unmapped_area(info, length, low_limit, high_limit);

	// trace_vm_unmapped_area(addr, info);
	return addr;
}


ulong get_unmapped_area(file_s *file, ulong addr,
		ulong len, ulong pgoff, ulong flags)
{
	ulong (*get_area)(file_s *, ulong, ulong, ulong, ulong);

	/* Careful about overflows.. */
	if (len > TASK_SIZE)
		return -ENOMEM;

	get_area = current->mm->get_unmapped_area;
	if (file) {
		// if (file->f_op->get_unmapped_area)
		// 	get_area = file->f_op->get_unmapped_area;
	} else if (flags & MAP_SHARED) {
		/*
		 * mmap_region() will call shmem_zero_setup() to create a file,
		 * so use shmem's get_unmapped_area in case it can be huge.
		 * do_mmap() will clear pgoff, so match alignment.
		 */
		pgoff = 0;
		// get_area = shmem_get_unmapped_area;
	}

	addr = get_area(file, addr, len, pgoff, flags);
	if (IS_ERR_VALUE(addr))
		return addr;

	if (addr > TASK_SIZE - len)
		return -ENOMEM;
	if (offset_in_page(addr))
		return -EINVAL;

	// error = security_mmap_addr(addr);
	// return error ? error : addr;
	return addr;
}
EXPORT_SYMBOL(get_unmapped_area);


/* enforced gap between the expanding stack and other mappings. */
ulong stack_guard_gap = 256UL<<PAGE_SHIFT;

int expand_stack(vma_s *vma, ulong address)
{
	mm_s *mm = vma->vm_mm;
	vma_s *prev;
	int error = 0;

	address &= PAGE_MASK;
	// if (address < mmap_min_addr)
	// 	return -EPERM;

	// /* Enforce stack_guard_gap */
	// prev = vma->vm_prev;
	// /* Check that both stack segments have the same anon_vma? */
	// if (prev && !(prev->vm_flags & VM_GROWSDOWN) &&
	// 		vma_is_accessible(prev)) {
	// 	if (address - prev->vm_end < stack_guard_gap)
	// 		return -ENOMEM;
	// }

	// /* We must make sure the anon_vma is allocated. */
	// if (unlikely(anon_vma_prepare(vma)))
	// 	return -ENOMEM;

	// /*
	// * vma->vm_start/vm_end cannot change under us because the caller
	// * is required to hold the mmap_lock in read mode.  We need the
	// * anon_vma lock to serialize against concurrent expand_stacks.
	// */
	// anon_vma_lock_write(vma->anon_vma);

	/* Somebody else might have raced and expanded it already */
	if (address < vma->vm_start) {
		ulong size, grow;

		size = vma->vm_end - address;
		grow = (vma->vm_start - address) >> PAGE_SHIFT;

		// error = -ENOMEM;
		if (grow <= vma->vm_pgoff) {
			// error = acct_stack_growth(vma, size, grow);
			// if (!error) {
				/*
					* vma_gap_update() doesn't support concurrent
					* updates, but we only hold a shared mmap_lock
					* lock here, so we need to protect against
					* concurrent vma expansions.
					* anon_vma_lock_write() doesn't help here, as
					* we don't guarantee that all growable vmas
					* in a mm share the same root anon vma.
					* So, we reuse mm->page_table_lock to guard
					* against concurrent vma expansions.
					*/
			// 	spin_lock(&mm->page_table_lock);
			// 	if (vma->vm_flags & VM_LOCKED)
			// 		mm->locked_vm += grow;
			// 	vm_stat_account(mm, vma->vm_flags, grow);
			// 	anon_vma_interval_tree_pre_update_vma(vma);
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
			// 	anon_vma_interval_tree_post_update_vma(vma);
			// 	vma_gap_update(vma);
			// 	spin_unlock(&mm->page_table_lock);

			// 	perf_event_mmap(vma);
			// }
		}
	}
	// anon_vma_unlock_write(vma->anon_vma);
	// khugepaged_enter_vma_merge(vma, vma->vm_flags);
	// validate_mm(mm);
	return error;
}

// /*
//  * Ok - we have the memory areas we should free on the vma list,
//  * so release them, and do the vma updates.
//  *
//  * Called with the mm semaphore held.
//  */
// static void
// remove_vma_list(mm_s *mm, vma_s *vma) {
// 	ulong nr_accounted = 0;

// 	/* Update high watermark before we lower total_vm */
// 	update_hiwater_vm(mm);
// 	do {
// 		long nrpages = vma_pages(vma);

// 		if (vma->vm_flags & VM_ACCOUNT)
// 			nr_accounted += nrpages;
// 		vm_stat_account(mm, vma->vm_flags, -nrpages);
// 		vma = remove_vma(vma);
// 	} while (vma);
// 	vm_unacct_memory(nr_accounted);
// 	validate_mm(mm);
// }

// /*
//  * Get rid of page table information in the indicated region.
//  *
//  * Called with the mm semaphore held.
//  */
// static void
// unmap_region(mm_s *mm, vma_s *vma, vma_s *prev, ulong start, ulong end) {

// 	vma_s *next = vma_next(mm, prev);
// 	struct mmu_gather tlb;

// 	lru_add_drain();
// 	tlb_gather_mmu(&tlb, mm);
// 	update_hiwater_rss(mm);
// 	unmap_vmas(&tlb, vma, start, end);
// 	free_pgtables(&tlb, vma, prev ? prev->vm_end : FIRST_USER_ADDRESS,
// 				 next ? next->vm_start : USER_PGTABLES_CEILING);
// 	tlb_finish_mmu(&tlb);

// 	kfree(prev);
// }

//	Linux implementation changed, proto not exist in linux-6.6
/*
 * remove all vmas away from @mm->mm_mt
 * start from @vma, end to the vma's vm_end reaches the @end
 */
static bool
myos_detach_vmas_to_be_unmapped(mm_s *mm, vma_s *vma, vma_s *prev, ulong end) {
	do {
		vma_s *tail_vma = vma;
		mm->map_count--;
		vma = vma_next_vma(vma);
		BUG_ON(!list_header_contains(&mm->mm_mt, &tail_vma->list));
		list_header_delete_node(&mm->mm_mt, &tail_vma->list);
	} while (vma && vma->vm_start < end);

	/*
	 * Do not downgrade mmap_lock if we are next to VM_GROWSDOWN or
	 * VM_GROWSUP VMA. Such VMAs can change their size under
	 * down_read(mmap_lock) and collide with the VMA we are about to unmap.
	 */
	if (vma && (vma->vm_flags & VM_GROWSDOWN) ||
		prev && (prev->vm_flags & VM_GROWSUP))
		return false;
	return true;
}

/*
 * __split_vma() bypasses sysctl_max_map_count checking.  We use this where it
 * has already been checked or doesn't make sense to fail.
 */
int __split_vma(mm_s *mm, vma_s *vma, ulong addr, int new_below)
{
	vma_s *new;
	int err;

	// if (vma->vm_ops && vma->vm_ops->may_split) {
	// 	err = vma->vm_ops->may_split(vma, addr);
	// 	if (err)
	// 		return err;
	// }

	new = vm_area_creat_dup(vma);
	if (!new)
		return -ENOMEM;

	// err = vma_dup_policy(vma, new);
	// if (err)
	// 	goto out_free_vma;

	if (new->vm_file)
		get_file(new->vm_file);

	if (new->vm_ops && new->vm_ops->open)
		new->vm_ops->open(new);

	if (new_below) {
		new->vm_end = addr;
		err = __simple_vma_adjust(vma, addr, vma->vm_end);
		__vma_link_list(mm, new, vma);
	} else {
		new->vm_start = addr;
		new->vm_pgoff += ((addr - vma->vm_start) >> PAGE_SHIFT);
		err = __simple_vma_adjust(vma, vma->vm_start, addr);
		__vma_link_list(mm, vma, new);
	}

	/* Success. */
	if (!err)
		return 0;

	/* Clean everything up if vma_adjust failed. */
	if (new->vm_ops && new->vm_ops->close)
		new->vm_ops->close(new);
	if (new->vm_file)
		fput(new->vm_file);
	// unlink_anon_vmas(new);
 out_free_vma:
	vm_area_free(new);
	return err;
}


//	Linux implementation changed, proto not exist in linux-6.6
/*
 * unmap the area from @start to @end, if no vma has intersection with
 * it do nothing
 * 
 */
int __do_munmap(mm_s *mm, ulong start, size_t len, bool downgrade)
{
	ulong end;
	vma_s *vma, *prev, *last;

	if (offset_in_page(start) ||
			start > TASK_SIZE ||
			len > (TASK_SIZE-start))
		return -EINVAL;

	len = PAGE_ALIGN(len);
	end = start + len;
	if (len == 0)
		return -EINVAL;

	/* Find the first overlapping VMA where start < vma->vm_end */
	vma = find_vma_intersection(mm, start, end);
	if (vma == NULL)
		return 0;
	prev = vma_prev_vma(vma);

	/*
	 * Note: mremap's move_vma VM_ACCOUNT handling assumes a partially
	 * unmapped vm_area_struct will remain in use: so lower split_vma
	 * places tmp vma above, and higher split_vma places tmp vma below.
	 */
	if (start > vma->vm_start) {
		int error = __split_vma(mm, vma, start, 0);
		if (error != ENOERR)
			return error;
		prev = vma;
	}

	/* Does it split the last one? */
	last = simple_find_vma(mm, end);
	if (last && end > last->vm_start) {
		int error = __split_vma(mm, last, end, 1);
		if (error != ENOERR)
			return error;
	}
	vma = vma_next_vma(prev);

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

	/* Detach vmas from rbtree */
	if (!myos_detach_vmas_to_be_unmapped(mm, vma, prev, end))
		downgrade = false;

	// if (downgrade)
	// 	mmap_write_downgrade(mm);

	// unmap_region(mm, vma, prev, start, end);

	// /* Fix up all other VM information */
	// remove_vma_list(mm, vma);

	return downgrade ? 1 : 0;
}

int __vm_munmap(ulong start, size_t len, bool downgrade)
{
	int ret;
	mm_s *mm = current->mm;
	// LIST_HEAD(uf);

	// if (mmap_write_lock_killable(mm))
	// 	return -EINTR;

	ret = __do_munmap(mm, start, len, downgrade);
	// /*
	//  * Returning 1 indicates mmap_lock is downgraded.
	//  * But 1 is not legal return value of vm_munmap() and munmap(), reset
	//  * it to 0 before return.
	//  */
	// if (ret == 1) {
	// 	mmap_read_unlock(mm);
	// 	ret = 0;
	// } else
	// 	mmap_write_unlock(mm);

	// userfaultfd_unmap_complete(mm, &uf);
	return ret;
}


/*
 *  this is really a simplified "do_mmap".  it only handles
 *  anonymous maps.  eventually we may be able to do some
 *  brk-specific accounting here.
 */
// static int do_brk_flags(unsigned long addr, unsigned long len,
// 		unsigned long flags, struct list_head *uf)
int
do_brk_flags(ulong addr, ulong len, ulong flags)
{
	mm_s *mm = current->mm;
	vma_s *vma, *prev;
	// struct rb_node **rb_link, *rb_parent;
	pgoff_t pgoff = addr >> PAGE_SHIFT;
	int error;
	ulong mapped_addr;

	/* Until we need other flags, refuse anything except VM_EXEC. */
	if ((flags & (~VM_EXEC)) != 0)
		return -EINVAL;
	flags |= VM_DATA_DEFAULT_FLAGS | VM_ACCOUNT | mm->def_flags;

	mapped_addr = get_unmapped_area(NULL, addr, len, 0, MAP_FIXED);
	if (IS_ERR_VALUE(mapped_addr))
		return mapped_addr;

	// error = mlock_future_check(mm, mm->def_flags, len);
	// if (error)
	// 	return error;

	/* Clear old maps, set up prev, rb_link, rb_parent, and uf */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;

	// /* Check against address space limits *after* clearing old maps... */
	// if (!may_expand_vm(mm, flags, len >> PAGE_SHIFT))
	// 	return -ENOMEM;

	// if (mm->map_count > sysctl_max_map_count)
	// 	return -ENOMEM;

	// if (security_vm_enough_memory_mm(mm, len >> PAGE_SHIFT))
	// 	return -ENOMEM;

	/* Can we just expand an old private anonymous mapping? */
	vma = simple_vma_merge(mm, prev, addr, addr + len, flags, NULL, pgoff);
	if (vma)
		goto out;

	/*
	 * create a vma struct for an anonymous mapping
	 */
	vma = vm_area_alloc(mm);
	if (!vma) {
		// vm_unacct_memory(len >> PAGE_SHIFT);
		return -ENOMEM;
	}

	vma_set_anonymous(vma);
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_pgoff = pgoff;
	vma->vm_flags = flags;
	// vma->vm_page_prot = vm_get_page_prot(flags);
	simple_vma_link(mm, vma, prev);
out:
	// perf_event_mmap(vma);
	mm->total_vm += len >> PAGE_SHIFT;
	mm->data_vm += len >> PAGE_SHIFT;
	if (flags & VM_LOCKED)
		mm->locked_vm += (len >> PAGE_SHIFT);
	vma->vm_flags |= VM_SOFTDIRTY;
	return 0;
}

int vm_brk_flags(ulong addr, ulong request, ulong flags)
{
	mm_s *mm = current->mm;
	ulong len;
	int ret;
	bool populate;
	// LIST_HEAD(uf);

	len = PAGE_ALIGN(request);
	if (len < request)
		return -ENOMEM;
	if (!len)
		return 0;

	// if (mmap_write_lock_killable(mm))
	// 	return -EINTR;

	ret = do_brk_flags(addr, len, flags);
	// populate = ((mm->def_flags & VM_LOCKED) != 0);
	// mmap_write_unlock(mm);
	// userfaultfd_unmap_complete(mm, &uf);
	// if (populate && !ret)
	// 	mm_populate(addr, len);
	return ret;
}

// /* Release all mmaps. */
// void exit_mmap(mm_s *mm)
// {
// 	struct mmu_gather tlb;
// 	struct vm_area_struct *vma;
// 	unsigned long nr_accounted = 0;

// 	/* mm's last user has gone, and its about to be pulled down */
// 	mmu_notifier_release(mm);

// 	if (unlikely(mm_is_oom_victim(mm))) {
// 		/*
// 		 * Manually reap the mm to free as much memory as possible.
// 		 * Then, as the oom reaper does, set MMF_OOM_SKIP to disregard
// 		 * this mm from further consideration.  Taking mm->mmap_lock for
// 		 * write after setting MMF_OOM_SKIP will guarantee that the oom
// 		 * reaper will not run on this mm again after mmap_lock is
// 		 * dropped.
// 		 *
// 		 * Nothing can be holding mm->mmap_lock here and the above call
// 		 * to mmu_notifier_release(mm) ensures mmu notifier callbacks in
// 		 * __oom_reap_task_mm() will not block.
// 		 *
// 		 * This needs to be done before calling unlock_range(),
// 		 * which clears VM_LOCKED, otherwise the oom reaper cannot
// 		 * reliably test it.
// 		 */
// 		(void)__oom_reap_task_mm(mm);

// 		set_bit(MMF_OOM_SKIP, &mm->flags);
// 	}

// 	mmap_write_lock(mm);
// 	if (mm->locked_vm)
// 		unlock_range(mm->mmap, ULONG_MAX);

// 	arch_exit_mmap(mm);

// 	vma = mm->mmap;
// 	if (!vma) {
// 		/* Can happen if dup_mmap() received an OOM */
// 		mmap_write_unlock(mm);
// 		return;
// 	}

// 	lru_add_drain();
// 	flush_cache_mm(mm);
// 	tlb_gather_mmu_fullmm(&tlb, mm);
// 	/* update_hiwater_rss(mm) here? but nobody should be looking */
// 	/* Use -1 here to ensure all VMAs in the mm are unmapped */
// 	unmap_vmas(&tlb, vma, 0, -1);
// 	free_pgtables(&tlb, vma, FIRST_USER_ADDRESS, USER_PGTABLES_CEILING);
// 	tlb_finish_mmu(&tlb);

// 	/* Walk the list again, actually closing and freeing it. */
// 	while (vma) {
// 		if (vma->vm_flags & VM_ACCOUNT)
// 			nr_accounted += vma_pages(vma);
// 		vma = remove_vma(vma);
// 		cond_resched();
// 	}
// 	mm->mmap = NULL;
// 	mmap_write_unlock(mm);
// 	vm_unacct_memory(nr_accounted);
// }

/*
 * Insert vm structure into process list sorted by address
 * and into the inode's i_mmap tree.  If vm_file is non-NULL
 * then i_mmap_rwsem is taken here.
 */
int insert_vm_struct(mm_s *mm, vma_s *vma)
{
	vma_s *prev;
	find_vma_prev(mm, vma->vm_start, &prev);
	// if ((vma->vm_flags & VM_ACCOUNT) &&
	//      security_vm_enough_memory_mm(mm, vma_pages(vma)))
	// 	return -ENOMEM;

	/*
	 * The vm_pgoff of a purely anonymous vma should be irrelevant
	 * until its first write fault, when page's anon_vma and index
	 * are set.  But now set the vm_pgoff it will almost certainly
	 * end up with (unless mremap moves it elsewhere before that
	 * first wfault), so /proc/pid/maps tells a consistent story.
	 *
	 * By setting it to reflect the virtual start address of the
	 * vma, merges and splits can happen in a seamless way, just
	 * using the existing file pgoff checks and manipulations.
	 * Similarly in do_mmap and in do_brk_flags.
	 */
	if (vma_is_anonymous(vma)) {
		// BUG_ON(vma->anon_vma);
		vma->vm_pgoff = vma->vm_start >> PAGE_SHIFT;
	}

	simple_vma_link(mm, vma, prev);
	return 0;
}
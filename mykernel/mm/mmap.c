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


/*
 * We cannot adjust vm_start, vm_end, vm_pgoff fields of a vma that
 * is already present in an i_mmap tree without adjusting the tree.
 * The following helper function should be used when such adjustments
 * are necessary.  The "insert" vma (if any) is to be inserted
 * before we drop the necessary locks.
 */
int __myos_vma_adjust(vma_s *vma, unsigned long start, unsigned long end,
		pgoff_t pgoff, vma_s *insert, vma_s *expand)
{
	mm_s *mm = vma->vm_mm;
	vma_s *next = vma->vm_next, *orig_vma = vma;
	// struct address_space *mapping = NULL;
	// struct rb_root_cached *root = NULL;
	// struct anon_vma *anon_vma = NULL;
	file_s *file = vma->vm_file;
	bool start_changed = false, end_changed = false;
	long adjust_next = 0;
	int remove_next = 0;

	// if (next && !insert) {
	// 	struct vm_area_struct *exporter = NULL, *importer = NULL;

	// 	if (end >= next->vm_end) {
	// 		/*
	// 		 * vma expands, overlapping all the next, and
	// 		 * perhaps the one after too (mprotect case 6).
	// 		 * The only other cases that gets here are
	// 		 * case 1, case 7 and case 8.
	// 		 */
	// 		if (next == expand) {
	// 			/*
	// 			 * The only case where we don't expand "vma"
	// 			 * and we expand "next" instead is case 8.
	// 			 */
	// 			VM_WARN_ON(end != next->vm_end);
	// 			/*
	// 			 * remove_next == 3 means we're
	// 			 * removing "vma" and that to do so we
	// 			 * swapped "vma" and "next".
	// 			 */
	// 			remove_next = 3;
	// 			VM_WARN_ON(file != next->vm_file);
	// 			swap(vma, next);
	// 		} else {
	// 			VM_WARN_ON(expand != vma);
	// 			/*
	// 			 * case 1, 6, 7, remove_next == 2 is case 6,
	// 			 * remove_next == 1 is case 1 or 7.
	// 			 */
	// 			remove_next = 1 + (end > next->vm_end);
	// 			VM_WARN_ON(remove_next == 2 &&
	// 				   end != next->vm_next->vm_end);
	// 			/* trim end to next, for case 6 first pass */
	// 			end = next->vm_end;
	// 		}

	// 		exporter = next;
	// 		importer = vma;

	// 		/*
	// 		 * If next doesn't have anon_vma, import from vma after
	// 		 * next, if the vma overlaps with it.
	// 		 */
	// 		if (remove_next == 2 && !next->anon_vma)
	// 			exporter = next->vm_next;

	// 	} else if (end > next->vm_start) {
	// 		/*
	// 		 * vma expands, overlapping part of the next:
	// 		 * mprotect case 5 shifting the boundary up.
	// 		 */
	// 		adjust_next = (end - next->vm_start);
	// 		exporter = next;
	// 		importer = vma;
	// 		VM_WARN_ON(expand != importer);
	// 	} else if (end < vma->vm_end) {
	// 		/*
	// 		 * vma shrinks, and !insert tells it's not
	// 		 * split_vma inserting another: so it must be
	// 		 * mprotect case 4 shifting the boundary down.
	// 		 */
	// 		adjust_next = -(vma->vm_end - end);
	// 		exporter = vma;
	// 		importer = next;
	// 		VM_WARN_ON(expand != importer);
	// 	}

	// 	/*
	// 	 * Easily overlooked: when mprotect shifts the boundary,
	// 	 * make sure the expanding vma has anon_vma set if the
	// 	 * shrinking vma had, to cover any anon pages imported.
	// 	 */
	// 	if (exporter && exporter->anon_vma && !importer->anon_vma) {
	// 		int error;

	// 		importer->anon_vma = exporter->anon_vma;
	// 		error = anon_vma_clone(importer, exporter);
	// 		if (error)
	// 			return error;
	// 	}
	// }
// again:
	// vma_adjust_trans_huge(orig_vma, start, end, adjust_next);

	// if (file) {
	// 	mapping = file->f_mapping;
	// 	root = &mapping->i_mmap;
	// 	uprobe_munmap(vma, vma->vm_start, vma->vm_end);

	// 	if (adjust_next)
	// 		uprobe_munmap(next, next->vm_start, next->vm_end);

	// 	i_mmap_lock_write(mapping);
	// 	if (insert) {
	// 		/*
	// 		 * Put into interval tree now, so instantiated pages
	// 		 * are visible to arm/parisc __flush_dcache_page
	// 		 * throughout; but we cannot insert into address
	// 		 * space until vma start or end is updated.
	// 		 */
	// 		__vma_link_file(insert);
	// 	}
	// }

	// anon_vma = vma->anon_vma;
	// if (!anon_vma && adjust_next)
	// 	anon_vma = next->anon_vma;
	// if (anon_vma) {
	// 	VM_WARN_ON(adjust_next && next->anon_vma &&
	// 		   anon_vma != next->anon_vma);
	// 	anon_vma_lock_write(anon_vma);
	// 	anon_vma_interval_tree_pre_update_vma(vma);
	// 	if (adjust_next)
	// 		anon_vma_interval_tree_pre_update_vma(next);
	// }

	// if (file) {
	// 	flush_dcache_mmap_lock(mapping);
	// 	vma_interval_tree_remove(vma, root);
	// 	if (adjust_next)
	// 		vma_interval_tree_remove(next, root);
	// }

	// if (start != vma->vm_start) {
	// 	vma->vm_start = start;
	// 	start_changed = true;
	// }
	// if (end != vma->vm_end) {
	// 	vma->vm_end = end;
	// 	end_changed = true;
	// }
	// vma->vm_pgoff = pgoff;
	// if (adjust_next) {
	// 	next->vm_start += adjust_next;
	// 	next->vm_pgoff += adjust_next >> PAGE_SHIFT;
	// }

	// if (file) {
	// 	if (adjust_next)
	// 		vma_interval_tree_insert(next, root);
	// 	vma_interval_tree_insert(vma, root);
	// 	flush_dcache_mmap_unlock(mapping);
	// }

	// if (remove_next) {
	// 	/*
	// 	 * vma_merge has merged next into vma, and needs
	// 	 * us to remove next before dropping the locks.
	// 	 */
	// 	if (remove_next != 3)
	// 		__vma_unlink(mm, next, next);
	// 	else
	// 		/*
	// 		 * vma is not before next if they've been
	// 		 * swapped.
	// 		 *
	// 		 * pre-swap() next->vm_start was reduced so
	// 		 * tell validate_mm_rb to ignore pre-swap()
	// 		 * "next" (which is stored in post-swap()
	// 		 * "vma").
	// 		 */
	// 		__vma_unlink(mm, next, vma);
	// 	if (file)
	// 		__remove_shared_vm_struct(next, file, mapping);
	// } else if (insert) {
	// 	/*
	// 	 * split_vma has split insert from vma, and needs
	// 	 * us to insert it before dropping the locks
	// 	 * (it may either follow vma or precede it).
	// 	 */
	// 	__insert_vm_struct(mm, insert);
	// } else {
	// 	if (start_changed)
	// 		vma_gap_update(vma);
	// 	if (end_changed) {
	// 		if (!next)
	// 			mm->highest_vm_end = vm_end_gap(vma);
	// 		else if (!adjust_next)
	// 			vma_gap_update(next);
	// 	}
	// }

	// if (anon_vma) {
	// 	anon_vma_interval_tree_post_update_vma(vma);
	// 	if (adjust_next)
	// 		anon_vma_interval_tree_post_update_vma(next);
	// 	anon_vma_unlock_write(anon_vma);
	// }

	// if (file) {
	// 	i_mmap_unlock_write(mapping);
	// 	uprobe_mmap(vma);

	// 	if (adjust_next)
	// 		uprobe_mmap(next);
	// }

	// if (remove_next) {
	// 	if (file) {
	// 		uprobe_munmap(next, next->vm_start, next->vm_end);
	// 		fput(file);
	// 	}
	// 	if (next->anon_vma)
	// 		anon_vma_merge(vma, next);
	// 	mm->map_count--;
	// 	mpol_put(vma_policy(next));
	// 	vm_area_free(next);
	// 	/*
	// 	 * In mprotect's case 6 (see comments on vma_merge),
	// 	 * we must remove another next too. It would clutter
	// 	 * up the code too much to do both in one go.
	// 	 */
	// 	if (remove_next != 3) {
	// 		/*
	// 		 * If "next" was removed and vma->vm_end was
	// 		 * expanded (up) over it, in turn
	// 		 * "next->vm_prev->vm_end" changed and the
	// 		 * "vma->vm_next" gap must be updated.
	// 		 */
	// 		next = vma->vm_next;
	// 	} else {
	// 		/*
	// 		 * For the scope of the comment "next" and
	// 		 * "vma" considered pre-swap(): if "vma" was
	// 		 * removed, next->vm_start was expanded (down)
	// 		 * over it and the "next" gap must be updated.
	// 		 * Because of the swap() the post-swap() "vma"
	// 		 * actually points to pre-swap() "next"
	// 		 * (post-swap() "next" as opposed is now a
	// 		 * dangling pointer).
	// 		 */
	// 		next = vma;
	// 	}
	// 	if (remove_next == 2) {
	// 		remove_next = 1;
	// 		end = next->vm_end;
	// 		goto again;
	// 	}
	// 	else if (next)
	// 		vma_gap_update(next);
	// 	else {
	// 		/*
	// 		 * If remove_next == 2 we obviously can't
	// 		 * reach this path.
	// 		 *
	// 		 * If remove_next == 3 we can't reach this
	// 		 * path because pre-swap() next is always not
	// 		 * NULL. pre-swap() "next" is not being
	// 		 * removed and its next->vm_end is not altered
	// 		 * (and furthermore "end" already matches
	// 		 * next->vm_end in remove_next == 3).
	// 		 *
	// 		 * We reach this only in the remove_next == 1
	// 		 * case if the "next" vma that was removed was
	// 		 * the highest vma of the mm. However in such
	// 		 * case next->vm_end == "end" and the extended
	// 		 * "vma" has vma->vm_end == next->vm_end so
	// 		 * mm->highest_vm_end doesn't need any update
	// 		 * in remove_next == 1 case.
	// 		 */
	// 		VM_WARN_ON(mm->highest_vm_end != vm_end_gap(vma));
	// 	}
	// }
	// if (insert && file)
	// 	uprobe_mmap(insert);

	// validate_mm(mm);

	return 0;
}


/*
 * The caller must write-lock current->mm->mmap_lock.
 */
unsigned long do_mmap(file_s *file,
		unsigned long addr, unsigned long len,
		unsigned long prot, unsigned long flags,
		unsigned long pgoff, unsigned long *populate)
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

	// /* force arch specific MAP_FIXED handling in get_unmapped_area */
	// if (flags & MAP_FIXED_NOREPLACE)
	// 	flags |= MAP_FIXED;

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

	// /* Obtain the address to map to. we verify (or select) it and ensure
	//  * that it represents a valid section of the address space.
	//  */
	// addr = get_unmapped_area(file, addr, len, pgoff, flags);
	// if (IS_ERR_VALUE(addr))
	// 	return addr;

	// if (flags & MAP_FIXED_NOREPLACE) {
	// 	if (find_vma_intersection(mm, addr, addr + len))
	// 		return -EEXIST;
	// }

	// if (prot == PROT_EXEC) {
	// 	pkey = execute_only_pkey(mm);
	// 	if (pkey < 0)
	// 		pkey = 0;
	// }

	// /* Do simple checking here so the lower-level routines won't have
	//  * to. we assume access permissions have been handled by the open
	//  * of the memory object, so we don't do any here.
	//  */
	// vm_flags = calc_vm_prot_bits(prot, pkey) | calc_vm_flag_bits(flags) |
	// 		mm->def_flags | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC;

	// if (flags & MAP_LOCKED)
	// 	if (!can_do_mlock())
	// 		return -EPERM;

	// if (mlock_future_check(mm, vm_flags, len))
	// 	return -EAGAIN;

	// if (file) {
	// 	struct inode *inode = file_inode(file);
	// 	unsigned long flags_mask;

	// 	if (!file_mmap_ok(file, inode, pgoff, len))
	// 		return -EOVERFLOW;

	// 	flags_mask = LEGACY_MAP_MASK | file->f_op->mmap_supported_flags;

	// 	switch (flags & MAP_TYPE) {
	// 	case MAP_SHARED:
	// 		/*
	// 		 * Force use of MAP_SHARED_VALIDATE with non-legacy
	// 		 * flags. E.g. MAP_SYNC is dangerous to use with
	// 		 * MAP_SHARED as you don't know which consistency model
	// 		 * you will get. We silently ignore unsupported flags
	// 		 * with MAP_SHARED to preserve backward compatibility.
	// 		 */
	// 		flags &= LEGACY_MAP_MASK;
	// 		fallthrough;
	// 	case MAP_SHARED_VALIDATE:
	// 		if (flags & ~flags_mask)
	// 			return -EOPNOTSUPP;
	// 		if (prot & PROT_WRITE) {
	// 			if (!(file->f_mode & FMODE_WRITE))
	// 				return -EACCES;
	// 			if (IS_SWAPFILE(file->f_mapping->host))
	// 				return -ETXTBSY;
	// 		}

	// 		/*
	// 		 * Make sure we don't allow writing to an append-only
	// 		 * file..
	// 		 */
	// 		if (IS_APPEND(inode) && (file->f_mode & FMODE_WRITE))
	// 			return -EACCES;

	// 		vm_flags |= VM_SHARED | VM_MAYSHARE;
	// 		if (!(file->f_mode & FMODE_WRITE))
	// 			vm_flags &= ~(VM_MAYWRITE | VM_SHARED);
	// 		fallthrough;
	// 	case MAP_PRIVATE:
	// 		if (!(file->f_mode & FMODE_READ))
	// 			return -EACCES;
	// 		if (path_noexec(&file->f_path)) {
	// 			if (vm_flags & VM_EXEC)
	// 				return -EPERM;
	// 			vm_flags &= ~VM_MAYEXEC;
	// 		}

	// 		if (!file->f_op->mmap)
	// 			return -ENODEV;
	// 		if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
	// 			return -EINVAL;
	// 		break;

	// 	default:
	// 		return -EINVAL;
	// 	}
	// } else {
	// 	switch (flags & MAP_TYPE) {
	// 	case MAP_SHARED:
	// 		if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
	// 			return -EINVAL;
	// 		/*
	// 		 * Ignore pgoff.
	// 		 */
	// 		pgoff = 0;
	// 		vm_flags |= VM_SHARED | VM_MAYSHARE;
	// 		break;
	// 	case MAP_PRIVATE:
	// 		/*
	// 		 * Set pgoff according to addr for anon_vma.
	// 		 */
	// 		pgoff = addr >> PAGE_SHIFT;
	// 		break;
	// 	default:
	// 		return -EINVAL;
	// 	}
	// }

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

// unsigned long ksys_mmap_pgoff(
// 		unsigned long addr, unsigned long len,
// 		unsigned long prot, unsigned long flags,
// 		unsigned long fd, unsigned long pgoff)
// {
// 	struct file *file = NULL;
// 	unsigned long retval;

// 	if (!(flags & MAP_ANONYMOUS)) {
// 		audit_mmap_fd(fd, flags);
// 		file = fget(fd);
// 		if (!file)
// 			return -EBADF;
// 		if (is_file_hugepages(file)) {
// 			len = ALIGN(len, huge_page_size(hstate_file(file)));
// 		} else if (unlikely(flags & MAP_HUGETLB)) {
// 			retval = -EINVAL;
// 			goto out_fput;
// 		}
// 	} else if (flags & MAP_HUGETLB) {
// 		struct hstate *hs;

// 		hs = hstate_sizelog((flags >> MAP_HUGE_SHIFT) & MAP_HUGE_MASK);
// 		if (!hs)
// 			return -EINVAL;

// 		len = ALIGN(len, huge_page_size(hs));
// 		/*
// 		 * VM_NORESERVE is used because the reservations will be
// 		 * taken when vm_ops->mmap() is called
// 		 * A dummy user value is used because we are not locking
// 		 * memory so no accounting is necessary
// 		 */
// 		file = hugetlb_file_setup(HUGETLB_ANON_FILE, len,
// 				VM_NORESERVE,
// 				HUGETLB_ANONHUGE_INODE,
// 				(flags >> MAP_HUGE_SHIFT) & MAP_HUGE_MASK);
// 		if (IS_ERR(file))
// 			return PTR_ERR(file);
// 	}

// 	retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
// out_fput:
// 	if (file)
// 		fput(file);
// 	return retval;
// }


unsigned long
myos_mmap_region(file_s *file, unsigned long addr, unsigned long len,
		vm_flags_t vm_flags, unsigned long pgoff)
{
	mm_s *mm = current->mm;
	vma_s *vma, *prev, *merge;
	int error;
	unsigned long charged = 0;

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
vma_s *find_vma_prev(mm_s *mm, unsigned long addr, vma_s **pprev)
{
	vma_s *vma = NULL;

	vma = myos_find_vma(mm, addr);
	if (vma)
		*pprev = vma->vm_prev;

	return vma;
}


/*
 * __split_vma() bypasses sysctl_max_map_count checking.  We use this where it
 * has already been checked or doesn't make sense to fail.
 */
int __split_vma(mm_s *mm, vma_s *vma, unsigned long addr, int new_below)
{
	vma_s *new;
	int err;

	if (vma->vm_ops && vma->vm_ops->may_split) {
		err = vma->vm_ops->may_split(vma, addr);
		if (err)
			return err;
	}

	new = vm_area_dup(vma);
	if (!new)
		return -ENOMEM;

	if (new_below)
		new->vm_end = addr;
	else {
		new->vm_start = addr;
		new->vm_pgoff += ((addr - vma->vm_start) >> PAGE_SHIFT);
	}

	// err = vma_dup_policy(vma, new);
	// if (err)
	// 	goto out_free_vma;

	// err = anon_vma_clone(new, vma);
	// if (err)
	// 	goto out_free_mpol;

	// if (new->vm_file)
	// 	get_file(new->vm_file);

	if (new->vm_ops && new->vm_ops->open)
		new->vm_ops->open(new);

	if (new_below)
		err = __myos_vma_adjust(vma, addr, vma->vm_end,
				vma->vm_pgoff + ((addr - new->vm_start) >> PAGE_SHIFT),
				new, NULL);
	else
		err = __myos_vma_adjust(vma, vma->vm_start, addr,
				vma->vm_pgoff, new, NULL);

	/* Success. */
	if (!err)
		return 0;

	/* Clean everything up if vma_adjust failed. */
	if (new->vm_ops && new->vm_ops->close)
		new->vm_ops->close(new);
	// if (new->vm_file)
	// 	fput(new->vm_file);
	// unlink_anon_vmas(new);
//  out_free_mpol:
// 	mpol_put(vma_policy(new));
 out_free_vma:
	vm_area_free(new);
	return err;
}

/*
 * Split a vma into two pieces at address 'addr', a new vma is allocated
 * either for the first part or the tail.
 */
int split_vma(mm_s *mm, vma_s *vma, unsigned long addr, int new_below)
{
	if (mm->map_count >= sysctl_max_map_count)
		return -ENOMEM;

	return __split_vma(mm, vma, addr, new_below);
}

/* Munmap is split into 2 main parts -- this part which finds
 * what needs doing, and the areas themselves, which do the
 * work.  This now handles partial unmappings.
 * Jeremy Fitzhardinge <jeremy@goop.org>
 */
int __do_munmap(mm_s *mm, unsigned long start, size_t len, bool downgrade)
{
	unsigned long end;
	vma_s *vma, *prev, *last;

	if ((offset_in_page(start)) || start > TASK_SIZE ||
			len > TASK_SIZE-start)
		return -EINVAL;

	len = PAGE_ALIGN(len);
	end = start + len;
	if (len == 0)
		return -EINVAL;

	// arch_unmap(mm, start, end); x86 do not do this

	/* Find the first overlapping VMA where start < vma->vm_end */
	vma = find_vma_intersection(mm, start, end);
	if (!vma)
		return 0;
	prev = vma->vm_prev;

	/*
	 * If we need to split any vma, do it now to save pain later.
	 *
	 * Note: mremap's move_vma VM_ACCOUNT handling assumes a partially
	 * unmapped vm_area_struct will remain in use: so lower split_vma
	 * places tmp vma above, and higher split_vma places tmp vma below.
	 */
	if (start > vma->vm_start) {
		int error;

		/*
		 * Make sure that map_count on return from munmap() will
		 * not exceed its limit; but let map_count go just above
		 * its limit temporarily, to help free resources as expected.
		 */
		if (end < vma->vm_end && mm->map_count >= sysctl_max_map_count)
			return -ENOMEM;

		error = __split_vma(mm, vma, start, 0);
		if (error)
			return error;
		prev = vma;
	}

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
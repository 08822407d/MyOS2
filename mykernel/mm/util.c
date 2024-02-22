// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm/mm.h>
#include <linux/kernel/slab.h>
#include <linux/lib/string.h>
#include <linux/compiler/compiler.h>
#include <linux/kernel/export.h>
#include <linux/kernel/err.h>
#include <linux/kernel/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task_stack.h>
// #include <linux/security.h>
// #include <linux/swap.h>
// #include <linux/swapops.h>
#include <linux/mm/mman.h>
// #include <linux/hugetlb.h>
// #include <linux/vmalloc.h>
// #include <linux/userfaultfd_k.h>
#include <linux/kernel/elf.h>
// #include <linux/elf-randomize.h>
// #include <linux/personality.h>
// #include <linux/random.h>
// #include <linux/processor.h>
#include <linux/kernel/sizes.h>
// #include <linux/compat.h>

#include <linux/kernel/uaccess.h>

#include "internal.h"



void __vma_link_list(mm_s *mm, vma_s *vma, vma_s *prev)
{
	vma_s *next;

	vma->vm_prev = prev;
	if (prev) {
		next = prev->vm_next;
		prev->vm_next = vma;
	} else {
		next = mm->mmap;
		mm->mmap = vma;
	}
	vma->vm_next = next;
	if (next)
		next->vm_prev = vma;
}

void __vma_unlink_list(mm_s *mm, vma_s *vma)
{
	vma_s *prev, *next;

	next = vma->vm_next;
	prev = vma->vm_prev;
	if (prev)
		prev->vm_next = next;
	else
		mm->mmap = next;
	if (next)
		next->vm_prev = prev;
}


unsigned long vm_mmap_pgoff(file_s *file, unsigned long addr,
		unsigned long len, unsigned long prot,
		unsigned long flag, unsigned long pgoff)
{
	unsigned long ret;
	mm_s *mm = current->mm;
	unsigned long populate;
	// LIST_HEAD(uf);

	// ret = security_mmap_file(file, prot, flag);
	// if (!ret) {
	// 	if (mmap_write_lock_killable(mm))
	// 		return -EINTR;
		ret = do_mmap(file, addr, len, prot, flag, pgoff, &populate);
	// 	mmap_write_unlock(mm);
	// 	userfaultfd_unmap_complete(mm, &uf);
	// 	if (populate)
	// 		mm_populate(ret, populate);
	// }
	return ret;
}

unsigned long vm_mmap(file_s *file, unsigned long addr,
		unsigned long len, unsigned long prot,
		unsigned long flag, unsigned long offset)
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;

	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
}


int sysctl_max_map_count __read_mostly = DEFAULT_MAX_MAP_COUNT;
unsigned long sysctl_user_reserve_kbytes __read_mostly = 1UL << 17; /* 128MB */
unsigned long sysctl_admin_reserve_kbytes __read_mostly = 1UL << 13; /* 8MB */
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/lib/string.h>
#include <linux/compiler/compiler.h>
#include <linux/kernel/export.h>
#include <linux/kernel/err.h>
#include <linux/kernel/sched.h>
#include <linux/sched/signal.h>
#include <linux/kernel/elf.h>
#include <linux/kernel/sizes.h>

#include <linux/kernel/uaccess.h>


void __vma_link_list(mm_s *mm, vma_s *vma, vma_s *prev)
{
	BUG_ON(!list_header_contains(&mm->mm_mt, &prev->list));
	INIT_LIST_S(&vma->list);
	list_add_to_next(&vma->list, &prev->list);

	// vma_s *next;

	// vma->vm_prev = prev;
	// if (prev) {
	// 	next = prev->vm_next;
	// 	prev->vm_next = vma;
	// } else {
	// 	next = mm->mmap;
	// 	mm->mmap = vma;
	// }
	// vma->vm_next = next;
	// if (next)
	// 	next->vm_prev = vma;
}

void __vma_unlink_list(mm_s *mm, vma_s *vma)
{
	BUG_ON(!list_header_contains(&mm->mm_mt, &vma->list));
	list_header_delete_node(&mm->mm_mt, &vma->list);

	// vma_s *prev, *next;

	// next = vma->vm_next;
	// prev = vma->vm_prev;
	// if (prev)
	// 	prev->vm_next = next;
	// else
	// 	mm->mmap = next;
	// if (next)
	// 	next->vm_prev = prev;
}


ulong
vm_mmap_pgoff(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong pgoff)
{
	ulong ret;
	mm_s *mm = current->mm;
	ulong populate;
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

ulong
vm_mmap(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong offset)
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;

	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
}


int sysctl_max_map_count __read_mostly = DEFAULT_MAX_MAP_COUNT;
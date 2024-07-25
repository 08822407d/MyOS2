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
	INIT_LIST_S(&vma->list);
	if (prev == NULL) {
		list_header_add_to_head(&mm->mm_mt, &vma->list);
	} else {
		// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &prev->list)));
		while (vma != NULL && !list_header_contains(&mm->mm_mt, &prev->list));

		list_add_to_next(&vma->list, &prev->list);
		mm->mm_mt.count++;
	}
}

void __vma_unlink_list(mm_s *mm, vma_s *vma)
{
	// BUG_ON(!list_header_contains(&mm->mm_mt, &vma->list));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	list_header_delete_node(&mm->mm_mt, &vma->list);
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
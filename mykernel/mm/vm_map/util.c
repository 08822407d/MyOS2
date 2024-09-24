// SPDX-License-Identifier: GPL-2.0-only
#include <linux/lib/string.h>
#include <linux/compiler/compiler.h>
#include <linux/kernel/export.h>
#include <linux/kernel/err.h>
#include <linux/kernel/sched.h>
#include <linux/sched/signal.h>
#include <linux/kernel/elf.h>
#include <linux/kernel/sizes.h>
#include <linux/kernel/mmap_lock.h>

#include <linux/kernel/uaccess.h>


void __vma_link_to_list(mm_s *mm, vma_s *vma, vma_s *prev)
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
	mm->map_count++;
}

void __vma_unlink_from_list(mm_s *mm, vma_s *vma)
{
	// BUG_ON(!list_header_contains(&mm->mm_mt, &vma->list));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
}


/* MyOS2 doesn't support randomize_stack_top */
ulong randomize_stack_top(ulong stack_top)
{
	// ulong random_variable = 0;

	// if (current->flags & PF_RANDOMIZE) {
	// 	random_variable = get_random_long();
	// 	random_variable &= STACK_RND_MASK;
	// 	random_variable <<= PAGE_SHIFT;
	// }

	// return PAGE_ALIGN(stack_top) - random_variable;

	return stack_top;
}

/**
 * randomize_page - Generate a random, page aligned address
 * @start:	The smallest acceptable address the caller will take.
 * @range:	The size of the area, starting at @start, within which the
 *		random address must fall.
 *
 * If @start + @range would overflow, @range is capped.
 *
 * NOTE: Historical use of randomize_range, which this replaces, presumed that
 * @start was already page aligned.  We now align it regardless.
 *
 * Return: A page aligned address within [start, start + range).  On error,
 * @start is returned.
 */
ulong randomize_page(ulong start, ulong range)
{
	// if (!PAGE_ALIGNED(start)) {
	// 	range -= PAGE_ALIGN(start) - start;
	// 	start = PAGE_ALIGN(start);
	// }

	// if (start > ULONG_MAX - range)
	// 	range = ULONG_MAX - start;

	// range >>= PAGE_SHIFT;

	// if (range == 0)
	// 	return start;

	// return start + (get_random_long() % range << PAGE_SHIFT);
}


ulong vm_mmap_pgoff(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong pgoff)
{
	ulong ret;
	mm_s *mm = current->mm;

	if (mmap_write_lock_killable(mm))
		return -EINTR;
	ret = do_mmap(file, addr, len, prot, flag, pgoff);
	mmap_write_unlock(mm);

	return ret;
}

ulong vm_mmap(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong offset)
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;

	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
}


int sysctl_max_map_count __read_mostly = DEFAULT_MAX_MAP_COUNT;
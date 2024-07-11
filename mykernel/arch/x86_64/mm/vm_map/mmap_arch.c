#include "../mm_types_arch.h"

#include "vm_map_api_arch.h"

#include "uapi/linux/resource.h"


void arch_pick_mmap_layout(mm_s * mm, rlimit_s *rlim_stack)
{
	// if (mmap_is_legacy())
	// 	mm->get_unmapped_area = arch_get_unmapped_area;
	// else
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;

	// arch_pick_mmap_base(&mm->mmap_base, &mm->mmap_legacy_base,
	// 		arch_rnd(mmap64_rnd_bits), task_size_64bit(0),
	// 		rlim_stack);
}

ulong get_mmap_base(int is_legacy)
{
	mm_s *mm = current->mm;
	return is_legacy ? mm->mmap_legacy_base : mm->mmap_base;
}

/**
 * mmap_address_hint_valid - Validate the address hint of mmap
 * @addr:	Address hint
 * @len:	Mapping length
 *
 * Check whether @addr and @addr + @len result in a valid mapping.
 *
 * On 32bit this only checks whether @addr + @len is <= TASK_SIZE.
 *
 * On 64bit with 5-level page tables another sanity check is required
 * because mappings requested by mmap(@addr, 0) which cross the 47-bit
 * virtual address boundary can cause the following theoretical issue:
 *
 *  An application calls mmap(addr, 0), i.e. without MAP_FIXED, where @addr
 *  is below the border of the 47-bit address space and @addr + @len is
 *  above the border.
 *
 *  With 4-level paging this request succeeds, but the resulting mapping
 *  address will always be within the 47-bit virtual address space, because
 *  the hint address does not result in a valid mapping and is
 *  ignored. Hence applications which are not prepared to handle virtual
 *  addresses above 47-bit work correctly.
 *
 *  With 5-level paging this request would be granted and result in a
 *  mapping which crosses the border of the 47-bit virtual address
 *  space. If the application cannot handle addresses above 47-bit this
 *  will lead to misbehaviour and hard to diagnose failures.
 *
 * Therefore ignore address hints which would result in a mapping crossing
 * the 47-bit virtual address boundary.
 *
 * Note, that in the same scenario with MAP_FIXED the behaviour is
 * different. The request with @addr < 47-bit and @addr + @len > 47-bit
 * fails on a 4-level paging machine but succeeds on a 5-level paging
 * machine. It is reasonable to expect that an application does not rely on
 * the failure of such a fixed mapping request, so the restriction is not
 * applied.
 */
bool mmap_address_hint_valid(ulong addr, ulong len)
{
	if (TASK_SIZE - len < addr)
		return false;

	return (addr > DEFAULT_MAP_WINDOW) == (addr + len > DEFAULT_MAP_WINDOW);
}



static void
find_start_end(ulong addr, ulong flags, ulong *begin, ulong *end) {
	// if (!in_32bit_syscall() && (flags & MAP_32BIT)) {
	// 	/* This is usually used needed to map code in small
	// 	   model, so it needs to be in the first 31bit. Limit
	// 	   it to that.  This means we need to move the
	// 	   unmapped base down for this case. This can give
	// 	   conflicts with the heap, but we assume that glibc
	// 	   malloc knows how to fall back to mmap. Give it 1GB
	// 	   of playground for now. -AK */
	// 	*begin = 0x40000000;
	// 	*end = 0x80000000;
	// 	if (current->flags & PF_RANDOMIZE) {
	// 		*begin = randomize_page(*begin, 0x02000000);
	// 	}
	// 	return;
	// }

	*begin	= get_mmap_base(1);
	// if (in_32bit_syscall())
	// 	*end = task_size_32bit();
	// else
	// 	*end = task_size_64bit(addr > DEFAULT_MAP_WINDOW);
}


ulong
arch_get_unmapped_area(file_s *filp, ulong addr,
		ulong len, ulong pgoff, ulong flags)
{
	mm_s *mm = current->mm;
	vma_s *vma;
	vma_unmapped_info_s info;
	ulong begin, end;

	if (flags & MAP_FIXED)
		return addr;

	find_start_end(addr, flags, &begin, &end);

	if (len > end)
		return -ENOMEM;

	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = myos_find_vma(mm, addr);
		if (end - len >= addr &&
		    (!vma || addr + len <= vm_start_gap(vma)))
			return addr;
	}

	info.flags = 0;
	info.length = len;
	info.low_limit = begin;
	info.high_limit = end;
	info.align_mask = 0;
	info.align_offset = pgoff << PAGE_SHIFT;
	// if (filp) {
	// 	info.align_mask = get_align_mask();
	// 	info.align_offset += get_align_bits();
	// }
	return vm_unmapped_area(&info);
}

ulong
arch_get_unmapped_area_topdown(file_s *filp, const ulong addr0,
		const ulong len, const ulong pgoff, const ulong flags)
{
	vma_s *vma;
	mm_s *mm = current->mm;
	ulong addr = addr0;
	vma_unmapped_info_s info;

	/* requested length too big for entire address space */
	if (len > TASK_SIZE)
		return -ENOMEM;

	/* No address checking. See comment at mmap_address_hint_valid() */
	if (flags & MAP_FIXED)
		return addr;

	/* requesting a specific address */
	if (addr) {
		addr &= PAGE_MASK;
		if (!mmap_address_hint_valid(addr, len))
			goto get_unmapped_area;

		vma = myos_find_vma(mm, addr);
		if (!vma || addr + len <= vm_start_gap(vma))
			return addr;
	}
get_unmapped_area:

	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	// if (!in_32bit_syscall() && (flags & MAP_ABOVE4G))
	// 	info.low_limit = SZ_4G;
	// else
		info.low_limit = PAGE_SIZE;

	info.high_limit = get_mmap_base(0);

	/*
	 * If hint address is above DEFAULT_MAP_WINDOW, look for unmapped area
	 * in the full address space.
	 *
	 * !in_32bit_syscall() check to avoid high addresses for x32
	 * (and make it no op on native i386).
	 */
	if (addr > DEFAULT_MAP_WINDOW)
		info.high_limit += TASK_SIZE_MAX - DEFAULT_MAP_WINDOW;

	info.align_mask = 0;
	info.align_offset = pgoff << PAGE_SHIFT;
	// if (filp) {
	// 	info.align_mask = get_align_mask();
	// 	info.align_offset += get_align_bits();
	// }
	addr = vm_unmapped_area(&info);
	if ((addr & ~PAGE_MASK) != 0)
		return addr;
	// VM_BUG_ON(addr != -ENOMEM);

bottomup:
	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
	return arch_get_unmapped_area(filp, addr0, len, pgoff, flags);
}
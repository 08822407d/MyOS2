/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PAGE_CONST_H_
#define _ASM_X86_PAGE_CONST_H_


	#include <uapi/linux/const.h>

	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))

	#define THREAD_SIZE_ORDER		3
	#define THREAD_SIZE				(PAGE_SIZE << THREAD_SIZE_ORDER)


	#define IOREMAP_MAX_ORDER		(PUD_SHIFT)
	#define __PHYSICAL_MASK			((phys_addr_t)((1ULL << __PHYSICAL_MASK_SHIFT) - 1))


	// #define __VIRTUAL_MASK			((1UL << __VIRTUAL_MASK_SHIFT) - 1)

	/* Cast *PAGE_MASK to a signed type so that it is sign-extended if
	virtual addresses are 32-bits but physical addresses are larger
	(ie, 32-bit PAE). */
	#define PHYSICAL_PAGE_MASK		(((signed long)PAGE_MASK) & __PHYSICAL_MASK)
	// #define PHYSICAL_PMD_PAGE_MASK	(((signed long)PMD_PAGE_MASK) & __PHYSICAL_MASK)
	// #define PHYSICAL_PUD_PAGE_MASK	(((signed long)PUD_PAGE_MASK) & __PHYSICAL_MASK)

	// #define HPAGE_SHIFT				PMD_SHIFT
	// #define HPAGE_SIZE				(_AC(1,UL) << HPAGE_SHIFT)
	// #define HPAGE_MASK				(~(HPAGE_SIZE - 1))
	// #define HUGETLB_PAGE_ORDER		(HPAGE_SHIFT - PAGE_SHIFT)

	// #define HUGE_MAX_HSTATE			2

	#define PAGE_OFFSET				((unsigned long)__PAGE_OFFSET)

	// #define VM_DATA_DEFAULT_FLAGS	VM_DATA_FLAGS_TSK_EXEC

	// #define __PHYSICAL_START		ALIGN(CONFIG_PHYSICAL_START, \
	// 									CONFIG_PHYSICAL_ALIGN)

	// #define __START_KERNEL			(__START_KERNEL_map + __PHYSICAL_START)



	// #define EXCEPTION_STACK_ORDER	(1 + KASAN_STACK_ORDER)
	// #define EXCEPTION_STKSZ			(PAGE_SIZE << EXCEPTION_STACK_ORDER)

	// #define IRQ_STACK_ORDER			(2 + KASAN_STACK_ORDER)
	// #define IRQ_STACK_SIZE			(PAGE_SIZE << IRQ_STACK_ORDER)

	// /*
	//  * The index for the tss.ist[] array. The hardware limit is 7 entries.
	//  */
	// #define IST_INDEX_DF		0
	// #define IST_INDEX_NMI		1
	// #define IST_INDEX_DB		2
	// #define IST_INDEX_MCE		3
	// #define IST_INDEX_VC		4

	/*
	 * Set __PAGE_OFFSET to the most negative possible address +
	 * PGDIR_SIZE*17 (pgd slot 273).
	 *
	 * The gap is to allow a space for LDT remap for PTI (1 pgd slot) and space for
	 * a hypervisor (16 slots). Choosing 16 slots for a hypervisor is arbitrary,
	 * but it's what Xen requires.
	 */
	// #define __PAGE_OFFSET_BASE_L5	_AC(0xff11000000000000, UL)
	// #define __PAGE_OFFSET_BASE_L4	_AC(0xffff888000000000, UL)
	#define __PAGE_OFFSET_BASE_L4	_AC(0xffff800000000000, UL)

	#define __PAGE_OFFSET			__PAGE_OFFSET_BASE_L4

	#define __START_KERNEL_map		_AC(0xffffffff80000000, UL)

	/* See Documentation/x86/x86_64/mm.rst for a description of the memory map. */

	#define __PHYSICAL_MASK_SHIFT	52

	#define __VIRTUAL_MASK_SHIFT	47
	#define task_size_max()			((_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)

	#define TASK_SIZE_MAX			task_size_max()
	#define DEFAULT_MAP_WINDOW		((1UL << 47) - PAGE_SIZE)

	// /* This decides where the kernel will search for a free chunk of vm
	// * space during mmap's.
	// */
	// #define IA32_PAGE_OFFSET		((current->personality & ADDR_LIMIT_3GB) ? 0xc0000000 : 0xFFFFe000)

	// #define TASK_SIZE_LOW			(test_thread_flag(TIF_ADDR32) ? IA32_PAGE_OFFSET : DEFAULT_MAP_WINDOW)
	#define TASK_SIZE_LOW			DEFAULT_MAP_WINDOW
	// #define TASK_SIZE				(test_thread_flag(TIF_ADDR32) ? IA32_PAGE_OFFSET : TASK_SIZE_MAX)
	#define TASK_SIZE				TASK_SIZE_MAX
	// #define TASK_SIZE_OF(child)		((test_tsk_thread_flag(child, TIF_ADDR32)) ? IA32_PAGE_OFFSET : TASK_SIZE_MAX)

	#define STACK_TOP				TASK_SIZE_LOW
	#define STACK_TOP_MAX			TASK_SIZE_MAX

#endif /* _ASM_X86_PAGE_CONST_H_ */
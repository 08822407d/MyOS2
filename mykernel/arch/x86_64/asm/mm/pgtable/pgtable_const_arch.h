/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_CONST_H_
#define _ASM_X86_PGTABLE_CONST_H_

    #include <linux/kernel/types.h>

    #include <asm/mm.h>

	#define _PAGE_BIT_PRESENT			0	/* is present */
	#define _PAGE_BIT_RW				1	/* writeable */
	#define _PAGE_BIT_USER				2	/* userspace addressable */
	#define _PAGE_BIT_PWT				3	/* page write through */
	#define _PAGE_BIT_PCD				4	/* page cache disabled */
	#define _PAGE_BIT_ACCESSED			5	/* was accessed (raised by CPU) */
	#define _PAGE_BIT_DIRTY				6	/* was written to (raised by CPU) */
	#define _PAGE_BIT_PSE				7	/* 4 MB (or 2MB) page */
	#define _PAGE_BIT_PAT				7	/* on 4KB pages */
	#define _PAGE_BIT_GLOBAL			8	/* Global TLB entry PPro+ */
	#define _PAGE_BIT_SOFTW1			9	/* available for programmer */
	#define _PAGE_BIT_SOFTW2			10	/* " */
	#define _PAGE_BIT_SOFTW3			11	/* " */
	#define _PAGE_BIT_PAT_LARGE			12	/* On 2MB or 1GB pages */
	#define _PAGE_BIT_SOFTW4			58	/* available for programmer */
	#define _PAGE_BIT_PKEY_BIT0			59	/* Protection Keys, bit 1/4 */
	#define _PAGE_BIT_PKEY_BIT1			60	/* Protection Keys, bit 2/4 */
	#define _PAGE_BIT_PKEY_BIT2			61	/* Protection Keys, bit 3/4 */
	#define _PAGE_BIT_PKEY_BIT3			62	/* Protection Keys, bit 4/4 */
	#define _PAGE_BIT_NX				63	/* No execute: only valid after cpuid check */

	#define _PAGE_BIT_SPECIAL			_PAGE_BIT_SOFTW1
	#define _PAGE_BIT_CPA_TEST			_PAGE_BIT_SOFTW1
	#define _PAGE_BIT_UFFD_WP			_PAGE_BIT_SOFTW2 /* userfaultfd wrprotected */
	#define _PAGE_BIT_SOFT_DIRTY		_PAGE_BIT_SOFTW3 /* software dirty tracking */
	#define _PAGE_BIT_DEVMAP			_PAGE_BIT_SOFTW4

	/* If _PAGE_BIT_PRESENT is clear, we use these: */
	/* - if the user mapped it with PROT_NONE; arch_pte_present gives true */
	#define _PAGE_BIT_PROTNONE		    _PAGE_BIT_GLOBAL

	#define _PAGE_PRESENT				(_AT(pteval_t, 1) << _PAGE_BIT_PRESENT)
	#define _PAGE_RW					(_AT(pteval_t, 1) << _PAGE_BIT_RW)
	#define _PAGE_USER					(_AT(pteval_t, 1) << _PAGE_BIT_USER)
	#define _PAGE_PWT					(_AT(pteval_t, 1) << _PAGE_BIT_PWT)
	#define _PAGE_PCD					(_AT(pteval_t, 1) << _PAGE_BIT_PCD)
	#define _PAGE_ACCESSED				(_AT(pteval_t, 1) << _PAGE_BIT_ACCESSED)
	#define _PAGE_DIRTY					(_AT(pteval_t, 1) << _PAGE_BIT_DIRTY)
	#define _PAGE_PSE					(_AT(pteval_t, 1) << _PAGE_BIT_PSE)
	#define _PAGE_GLOBAL				(_AT(pteval_t, 1) << _PAGE_BIT_GLOBAL)
	#define _PAGE_SOFTW1				(_AT(pteval_t, 1) << _PAGE_BIT_SOFTW1)
	#define _PAGE_SOFTW2				(_AT(pteval_t, 1) << _PAGE_BIT_SOFTW2)
	#define _PAGE_SOFTW3				(_AT(pteval_t, 1) << _PAGE_BIT_SOFTW3)
	#define _PAGE_PAT					(_AT(pteval_t, 1) << _PAGE_BIT_PAT)
	#define _PAGE_PAT_LARGE				(_AT(pteval_t, 1) << _PAGE_BIT_PAT_LARGE)
	#define _PAGE_SPECIAL				(_AT(pteval_t, 1) << _PAGE_BIT_SPECIAL)
	#define _PAGE_CPA_TEST				(_AT(pteval_t, 1) << _PAGE_BIT_CPA_TEST)
	#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	#  define _PAGE_PKEY_BIT0			(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT0)
	#  define _PAGE_PKEY_BIT1			(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT1)
	#  define _PAGE_PKEY_BIT2			(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT2)
	#  define _PAGE_PKEY_BIT3			(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT3)
	#else
	#  define _PAGE_PKEY_BIT0			(_AT(pteval_t, 0))
	#  define _PAGE_PKEY_BIT1			(_AT(pteval_t, 0))
	#  define _PAGE_PKEY_BIT2			(_AT(pteval_t, 0))
	#  define _PAGE_PKEY_BIT3			(_AT(pteval_t, 0))
	#endif

	#define _PAGE_PKEY_MASK (						\
				_PAGE_PKEY_BIT0 | _PAGE_PKEY_BIT1 | \
				_PAGE_PKEY_BIT2 | _PAGE_PKEY_BIT3	\
			)

	#define _PAGE_KNL_ERRATUM_MASK		(_PAGE_DIRTY | _PAGE_ACCESSED)

	#ifdef CONFIG_MEM_SOFT_DIRTY
	#  define _PAGE_SOFT_DIRTY			(_AT(pteval_t, 1) << _PAGE_BIT_SOFT_DIRTY)
	#else
	#  define _PAGE_SOFT_DIRTY			(_AT(pteval_t, 0))
	#endif

	/*
	 * Tracking soft dirty bit when a page goes to a swap is tricky.
	 * We need a bit which can be stored in pte _and_ not conflict
	 * with swap entry format. On x86 bits 1-4 are *not* involved
	 * into swap entry computation, but bit 7 is used for thp migration,
	 * so we borrow bit 1 for soft dirty tracking.
	 *
	 * Please note that this bit must be treated as swap dirty page
	 * mark if and only if the PTE/PMD has present bit clear!
	 */
	#ifdef CONFIG_MEM_SOFT_DIRTY
	#  define _PAGE_SWP_SOFT_DIRTY	_PAGE_RW
	#else
	#  define _PAGE_SWP_SOFT_DIRTY	(_AT(pteval_t, 0))
	#endif

	#ifdef CONFIG_HAVE_ARCH_USERFAULTFD_WP
	#  define _PAGE_UFFD_WP			(_AT(pteval_t, 1) << _PAGE_BIT_UFFD_WP)
	#  define _PAGE_SWP_UFFD_WP		_PAGE_USER
	#else
	#  define _PAGE_UFFD_WP			(_AT(pteval_t, 0))
	#  define _PAGE_SWP_UFFD_WP		(_AT(pteval_t, 0))
	#endif

	#define _PAGE_NX				(_AT(pteval_t, 1) << _PAGE_BIT_NX)
	#define _PAGE_DEVMAP			(_AT(u64, 1) << _PAGE_BIT_DEVMAP)
	#define _PAGE_PROTNONE			(_AT(pteval_t, 1) << _PAGE_BIT_PROTNONE)

	/*
	 * Set of bits not changed in pte_modify.  The pte's
	 * protection key is treated like _PAGE_RW, for
	 * instance, and is *not* included in this mask since
	 * pte_modify() does modify it.
	 */
	#define _PAGE_CHG_MASK	(									\
				PTE_PFN_MASK | _PAGE_PCD | _PAGE_PWT |			\
				_PAGE_SPECIAL | _PAGE_ACCESSED | _PAGE_DIRTY |	\
				_PAGE_SOFT_DIRTY | _PAGE_DEVMAP | _PAGE_ENC |	\
				_PAGE_UFFD_WP									\
			)
	#define _HPAGE_CHG_MASK	(_PAGE_CHG_MASK | _PAGE_PSE)

	/*
	 * The cache modes defined here are used to translate between pure SW usage
	 * and the HW defined cache mode bits and/or PAT entries.
	 *
	 * The resulting bits for PWT, PCD and PAT should be chosen in a way
	 * to have the WB mode at index 0 (all bits clear). This is the default
	 * right now and likely would break too much if changed.
	 */
	#ifndef __ASSEMBLY__
		// enum page_cache_mode {
		// 	_PAGE_CACHE_MODE_WB			= 0,
		// 	_PAGE_CACHE_MODE_WC			= 1,
		// 	_PAGE_CACHE_MODE_UC_MINUS	= 2,
		// 	_PAGE_CACHE_MODE_UC			= 3,
		// 	_PAGE_CACHE_MODE_WT			= 4,
		// 	_PAGE_CACHE_MODE_WP			= 5,

		// 	_PAGE_CACHE_MODE_NUM		= 8
		// };

		// 取值对照表, 源码的取值过于罗嗦就直接拉过来对照着改值
		// static uint16_t __cachemode2pte_tbl[_PAGE_CACHE_MODE_NUM] = {
		// 	[_PAGE_CACHE_MODE_WB      ]	= 0         | 0        ,
		// 	[_PAGE_CACHE_MODE_WC      ]	= 0         | _PAGE_PCD,
		// 	[_PAGE_CACHE_MODE_UC_MINUS]	= 0         | _PAGE_PCD,
		// 	[_PAGE_CACHE_MODE_UC      ]	= _PAGE_PWT | _PAGE_PCD,
		// 	[_PAGE_CACHE_MODE_WT      ]	= 0         | _PAGE_PCD,
		// 	[_PAGE_CACHE_MODE_WP      ]	= 0         | _PAGE_PCD,
		// };

		// unsigned long cachemode2protval(enum page_cache_mode pcm)
		// {
		// 	if (likely(pcm == 0))
		// 		return 0;
		// 	return __cachemode2pte_tbl[pcm];
		// }
	#endif

	// #define _PAGE_ENC					(_AT(pteval_t, sme_me_mask))
	// memory encypt not supported
	#define _PAGE_ENC					(_AT(pteval_t, 0))

	#define _PAGE_CACHE_MASK			(_PAGE_PWT | _PAGE_PCD | _PAGE_PAT)
	#define _PAGE_LARGE_CACHE_MASK		(_PAGE_PWT | _PAGE_PCD | _PAGE_PAT_LARGE)
	#define _PAGE_NOCACHE				(_PAGE_PWT | _PAGE_PCD)
	#define _PAGE_CACHE_WP				(_PAGE_PCD)

	#define __PP	_PAGE_PRESENT
	#define __RW	_PAGE_RW
	#define _USR	_PAGE_USER
	#define ___A	_PAGE_ACCESSED
	#define ___D	_PAGE_DIRTY
	#define ___G	_PAGE_GLOBAL
	#define __NX	_PAGE_NX
	#define _ENC	_PAGE_ENC
	#define __WP	_PAGE_CACHE_WP
	#define __NC	_PAGE_NOCACHE
	#define _PSE	_PAGE_PSE

	#define pgprot_val(x)				((x).pgprot)
	// #define __pgprot(x)					((pgprot_t) { (x) } )
	#define __pgprot(x)					((u64) { (x) } )
	#define __pg(x)						__pgprot(x)

	#define PAGE_NONE					__pg(   0|   0|   0|___A|   0|   0|   0|___G)
	#define PAGE_SHARED					__pg(__PP|__RW|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_SHARED_EXEC			__pg(__PP|__RW|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY_NOEXEC			__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_COPY_EXEC				__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY					__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY				__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY_EXEC			__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)

	#define __PAGE_KERNEL				(__PP|__RW|   0|___A|__NX|___D|   0|___G)
	#define __PAGE_KERNEL_EXEC			(__PP|__RW|   0|___A|   0|___D|   0|___G)
	#define _KERNPG_TABLE_NOENC			(__PP|__RW|   0|___A|   0|___D|   0|   0)
	#define _KERNPG_TABLE				(__PP|__RW|   0|___A|   0|___D|   0|   0| _ENC)
	#define _PAGE_TABLE_NOENC			(__PP|__RW|_USR|___A|   0|___D|   0|   0)
	#define _PAGE_TABLE					(__PP|__RW|_USR|___A|   0|___D|   0|   0| _ENC)
	#define __PAGE_KERNEL_RO			(__PP|   0|   0|___A|__NX|___D|   0|___G)
	#define __PAGE_KERNEL_ROX			(__PP|   0|   0|___A|   0|___D|   0|___G)
	// #define __PAGE_KERNEL_NOCACHE		(__PP|__RW|   0|___A|__NX|___D|   0|___G| __NC)
	#define __PAGE_KERNEL_NOCACHE		(__PP|__RW|   0|___A|	0|___D|   0|___G| __NC)
	#define __PAGE_KERNEL_VVAR			(__PP|   0|_USR|___A|__NX|___D|   0|___G)
	#define __PAGE_KERNEL_LARGE			(__PP|__RW|   0|___A|__NX|___D|_PSE|___G)
	#define __PAGE_KERNEL_LARGE_EXEC	(__PP|__RW|   0|___A|   0|___D|_PSE|___G)
	#define __PAGE_KERNEL_WP			(__PP|__RW|   0|___A|__NX|___D|   0|___G| __WP)
	#define __PAGE_KERNEL_IO			__PAGE_KERNEL
	#define __PAGE_KERNEL_IO_NOCACHE	__PAGE_KERNEL_NOCACHE

	#ifndef __ASSEMBLY__
	#  define __PAGE_KERNEL_ENC		    (__PAGE_KERNEL    | _ENC)
	#  define __PAGE_KERNEL_ENC_WP		(__PAGE_KERNEL_WP | _ENC)
	#  define __PAGE_KERNEL_NOENC		(__PAGE_KERNEL    |    0)
	#  define __PAGE_KERNEL_NOENC_WP	(__PAGE_KERNEL_WP |    0)

	#  define __pgprot_mask(x)			__pgprot((x) & __default_kernel_pte_mask)

	#  define PAGE_KERNEL				__pgprot_mask(__PAGE_KERNEL            | _ENC)
	#  define PAGE_KERNEL_NOENC		    __pgprot_mask(__PAGE_KERNEL            |    0)
	#  define PAGE_KERNEL_RO			__pgprot_mask(__PAGE_KERNEL_RO         | _ENC)
	#  define PAGE_KERNEL_EXEC			__pgprot_mask(__PAGE_KERNEL_EXEC       | _ENC)
	#  define PAGE_KERNEL_EXEC_NOENC	__pgprot_mask(__PAGE_KERNEL_EXEC       |    0)
	#  define PAGE_KERNEL_ROX			__pgprot_mask(__PAGE_KERNEL_ROX        | _ENC)
	#  define PAGE_KERNEL_NOCACHE		__pgprot_mask(__PAGE_KERNEL_NOCACHE    | _ENC)
	#  define PAGE_KERNEL_LARGE		    __pgprot_mask(__PAGE_KERNEL_LARGE      | _ENC)
	#  define PAGE_KERNEL_LARGE_EXEC	__pgprot_mask(__PAGE_KERNEL_LARGE_EXEC | _ENC)
	#  define PAGE_KERNEL_VVAR			__pgprot_mask(__PAGE_KERNEL_VVAR       | _ENC)
	#  define PAGE_KERNEL_IO			__pgprot_mask(__PAGE_KERNEL_IO)
	#  define PAGE_KERNEL_IO_NOCACHE	__pgprot_mask(__PAGE_KERNEL_IO_NOCACHE)
	#endif	/* __ASSEMBLY__ */

	/*         xwr */
	#define __P000	PAGE_NONE
	#define __P001	PAGE_READONLY
	#define __P010	PAGE_COPY
	#define __P011	PAGE_COPY
	#define __P100	PAGE_READONLY_EXEC
	#define __P101	PAGE_READONLY_EXEC
	#define __P110	PAGE_COPY_EXEC
	#define __P111	PAGE_COPY_EXEC

	#define __S000	PAGE_NONE
	#define __S001	PAGE_READONLY
	#define __S010	PAGE_SHARED
	#define __S011	PAGE_SHARED
	#define __S100	PAGE_READONLY_EXEC
	#define __S101	PAGE_READONLY_EXEC
	#define __S110	PAGE_SHARED_EXEC
	#define __S111	PAGE_SHARED_EXEC

	// #define SHARED_KERNEL_PMD 0

	/*
	 * PGDIR_SHIFT determines what a top-level page table entry can map
	 */
	#define PGDIR_SHIFT			39
	#define PTRS_PER_PGD		512
	#define MAX_PTRS_PER_P4D	1
	/*
	 * 3rd level page
	 */
	#define PUD_SHIFT			30
	#define PTRS_PER_PUD		512
	/*
	 * PMD_SHIFT determines the size of the area a middle-level
	 * page table can map
	 */
	#define PMD_SHIFT			21
	#define PTRS_PER_PMD		512
	/*
	 * entries per page directory level
	 */
	#define PTRS_PER_PTE		512

	#define PMD_SIZE			(_AC(1, UL) << PMD_SHIFT)
	#define PMD_MASK			(~(PMD_SIZE - 1))
	#define PUD_SIZE			(_AC(1, UL) << PUD_SHIFT)
	#define PUD_MASK			(~(PUD_SIZE - 1))
	#define PGDIR_SIZE			(_AC(1, UL) << PGDIR_SHIFT)
	#define PGDIR_MASK			(~(PGDIR_SIZE - 1))

	/*
	 * See Documentation/x86/x86_64/mm.rst for a description of the memory map.
	 *
	 * Be very careful vs. KASLR when changing anything here. The KASLR address
	 * range must not overlap with anything except the KASAN shadow area, which
	 * is correct as KASAN disables KASLR.
	 */
	#define MAXMEM (1UL << MAX_PHYSMEM_BITS)

	// #define GUARD_HOLE_PGD_ENTRY	-256UL
	// #define GUARD_HOLE_SIZE			(16UL << PGDIR_SHIFT)
	// #define GUARD_HOLE_BASE_ADDR	(GUARD_HOLE_PGD_ENTRY << PGDIR_SHIFT)
	// #define GUARD_HOLE_END_ADDR		(GUARD_HOLE_BASE_ADDR + GUARD_HOLE_SIZE)

	// #define LDT_PGD_ENTRY			-240UL
	// #define LDT_BASE_ADDR			(LDT_PGD_ENTRY << PGDIR_SHIFT)
	// #define LDT_END_ADDR			(LDT_BASE_ADDR + PGDIR_SIZE)

	// #define __VMALLOC_BASE_L4		0xffffc90000000000UL
	// #define __VMALLOC_BASE_L5		0xffa0000000000000UL

	// #define VMALLOC_SIZE_TB_L4		32UL
	// #define VMALLOC_SIZE_TB_L5		12800UL

	// #define __VMEMMAP_BASE_L4		0xffffea0000000000UL
	// #define __VMEMMAP_BASE_L5		0xffd4000000000000UL

	// #define VMALLOC_START			__VMALLOC_BASE_L5
	// #define VMALLOC_SIZE_TB			VMALLOC_SIZE_TB_L4
	// #define VMEMMAP_START			__VMEMMAP_BASE_L4

	// #define VMALLOC_END		(VMALLOC_START + (VMALLOC_SIZE_TB << 40) - 1)

	// #define MODULES_VADDR	(__START_KERNEL_map + KERNEL_IMAGE_SIZE)
	// /* The module sections ends with the start of the fixmap */
	// #ifndef CONFIG_DEBUG_KMAP_LOCAL_FORCE_MAP
	// #	define MODULES_END		_AC(0xffffffffff000000, UL)
	// #else
	// #	define MODULES_END		_AC(0xfffffffffe000000, UL)
	// #endif
	// #define MODULES_LEN		(MODULES_END - MODULES_VADDR)

	// #define ESPFIX_PGD_ENTRY	_AC(-2, UL)
	// #define ESPFIX_BASE_ADDR	(ESPFIX_PGD_ENTRY << P4D_SHIFT)

	// #define CPU_ENTRY_AREA_PGD	_AC(-4, UL)
	// #define CPU_ENTRY_AREA_BASE	(CPU_ENTRY_AREA_PGD << P4D_SHIFT)

	// #define EFI_VA_START		(-4 * (_AC(1, UL) << 30))
	// #define EFI_VA_END			(-68 * (_AC(1, UL) << 30))

	// #define EARLY_DYNAMIC_PAGE_TABLES	64

	#define PGD_KERNEL_START	((PAGE_SIZE / 2) / sizeof(pgd_t))

	// /*
	//  * We borrow bit 3 to remember PG_anon_exclusive.
	//  */
	// #define _PAGE_SWP_EXCLUSIVE	_PAGE_PWT



	/* Extracts the PFN from a (pte|pmd|pud|pgd)val_t of a 4KB page */
	#	define PTE_PFN_MASK		((pteval_t)PHYSICAL_PAGE_MASK)

	/*
	 *  Extracts the flags from a (pte|pmd|pud|pgd)val_t
	 *  This includes the protection key value.
	 */
	#	define PTE_FLAGS_MASK	(~PTE_PFN_MASK)

	#define PGD_ALLOWED_BITS	(~0ULL)
	#define P4D_SHIFT			PGDIR_SHIFT
	#define PTRS_PER_P4D		1
	#define P4D_SIZE			(1UL << P4D_SHIFT)
	#define P4D_MASK			(~(P4D_SIZE-1))

#endif /* _ASM_X86_PGTABLE_CONST_H_ */
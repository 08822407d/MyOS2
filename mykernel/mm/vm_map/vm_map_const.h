#ifndef _LINUX_VM_MAP_CONST_H_
#define _LINUX_VM_MAP_CONST_H_

	/*
	 * Default maximum number of active map areas, this limits the number of vmas
	 * per mm struct. Users can overwrite this number by sysctl but there is a
	 * problem.
	 *
	 * When a program's coredump is generated as ELF format, a section is created
	 * per a vma. In ELF, the number of sections is represented in unsigned short.
	 * This means the number of sections should be smaller than 65535 at coredump.
	 * Because the kernel adds some informative sections to a image of program at
	 * generating coredump, we need some margin. The number of extra sections is
	 * 1-3 now and depends on arch. We use "5" as safe margin, here.
	 *
	 * ELF extended numbering allows more than 65535 sections, so 16-bit bound is
	 * not a hard limit any more. Although some userspace tools can be surprised by
	 * that.
	 */
	#define MAPCOUNT_ELF_CORE_MARGIN	(5)
	#define DEFAULT_MAX_MAP_COUNT		(USHRT_MAX - MAPCOUNT_ELF_CORE_MARGIN)

//	+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//	| 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
//	+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//	   |    |    |    |         |    |    |    |    |    |    |    |    |    |    |_____VM_READ
//	   |    |    |    |         |    |    |    |    |    |    |    |    |    |__________VM_WRITE
//	   |    |    |    |         |    |    |    |    |    |    |    |    |_______________VM_EXEC
//	   |    |    |    |         |    |    |    |    |    |    |    |____________________VM_SHARED
//	   |    |    |    |         |    |    |    |    |    |    |
//	   |    |    |    |         |    |    |    |    |    |    |_________________________VM_MAYREAD
//	   |    |    |    |         |    |    |    |    |    |______________________________VM_MAYWRITE
//	   |    |    |    |         |    |    |    |    |___________________________________VM_MAYEXEC
//	   |    |    |    |         |    |    |    |________________________________________VM_MAYSHARE
//	   |    |    |    |         |    |    |
//	   |    |    |    |         |    |    |_____________________________________________VM_GROWSDOWN
//	   |    |    |    |         |    |__________________________________________________VM_UFFD_MISSING
//	   |    |    |    |         |_______________________________________________________VM_PFNMAP
//	   |    |    |    |
//	   |    |    |    |
//	   |    |    |    |_________________________________________________________________VM_UFFD_WP
//	   |    |    |______________________________________________________________________VM_LOCKED
//	   |    |___________________________________________________________________________VM_IO
//	   |________________________________________________________________________________VM_SEQ_READ
//	

//	+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//	| 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
//	+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//	   |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |_____VM_RAND_READ
//	   |    |    |    |    |    |    |    |    |    |    |    |    |    |    |__________VM_DONTCOPY
//	   |    |    |    |    |    |    |    |    |    |    |    |    |    |_______________VM_DONTEXPAND
//	   |    |    |    |    |    |    |    |    |    |    |    |    |____________________VM_LOCKONFAULT
//	   |    |    |    |    |    |    |    |    |    |    |    |
//	   |    |    |    |    |    |    |    |    |    |    |    |_________________________VM_ACCOUNT
//	   |    |    |    |    |    |    |    |    |    |    |______________________________VM_NORESERVE
//	   |    |    |    |    |    |    |    |    |    |___________________________________VM_HUGETLB
//	   |    |    |    |    |    |    |    |    |________________________________________VM_SYNC
//	   |    |    |    |    |    |    |    |
//	   |    |    |    |    |    |    |    |_____________________________________________VM_ARCH_1
//	   |    |    |    |    |    |    |__________________________________________________VM_WIPEONFORK
//	   |    |    |    |    |    |_______________________________________________________VM_DONTDUMP
//	   |    |    |    |    |____________________________________________________________VM_SOFTDIRTY
//	   |    |    |    |
//	   |    |    |    |_________________________________________________________________VM_MIXEDMAP
//	   |    |    |______________________________________________________________________VM_HUGEPAGE
//	   |    |___________________________________________________________________________VM_NOHUGEPAGE
//	   |________________________________________________________________________________VM_MERGEABLE
//	

	/*
	* vm_flags in vm_area_struct, see mm_types.h.
	* When changing, update also include/trace/events/mmflags.h
	*/
	#	define VM_NONE				0x00000000

	#	define VM_READ				0x00000001	/* currently active flags */
	#	define VM_WRITE				0x00000002
	#	define VM_EXEC				0x00000004
	#	define VM_SHARED			0x00000008

	/* mprotect() hardcodes VM_MAYREAD >> 4 == VM_READ, and so for r/w/x bits. */
	#	define VM_MAYREAD			0x00000010	/* limits for mprotect() etc */
	#	define VM_MAYWRITE			0x00000020
	#	define VM_MAYEXEC			0x00000040
	#	define VM_MAYSHARE			0x00000080

	#	define VM_GROWSDOWN			0x00000100	/* general info on the segment */
	#	define VM_UFFD_MISSING		0x00000200	/* missing pages tracking */
	#	define VM_PFNMAP			0x00000400	/* Page-ranges managed without "page_s", just pure PFN */
	#	define VM_UFFD_WP			0x00001000	/* wrprotect pages tracking */

	#	define VM_LOCKED			0x00002000
	#	define VM_IO				0x00004000	/* Memory mapped I/O or similar */

												/* Used by sys_madvise() */
	#	define VM_SEQ_READ			0x00008000	/* App will access data sequentially */
	#	define VM_RAND_READ			0x00010000	/* App will not benefit from clustered reads */

	#	define VM_DONTCOPY			0x00020000	/* Do not copy this vma on fork */
	#	define VM_DONTEXPAND		0x00040000	/* Cannot expand with mremap() */
	#	define VM_LOCKONFAULT		0x00080000	/* Lock the pages covered when they are faulted in */
	#	define VM_ACCOUNT			0x00100000	/* Is a VM accounted object */
	#	define VM_NORESERVE			0x00200000	/* should the VM suppress accounting */
	#	define VM_HUGETLB			0x00400000	/* Huge TLB Page VM */
	#	define VM_SYNC				0x00800000	/* Synchronous page faults */
	#	define VM_ARCH_1			0x01000000	/* Architecture-specific flag */
	#	define VM_WIPEONFORK		0x02000000	/* Wipe VMA contents in child. */
	#	define VM_DONTDUMP			0x04000000	/* Do not include in the core dump */

	#	ifdef CONFIG_MEM_SOFT_DIRTY
	#		define VM_SOFTDIRTY		0x08000000	/* Not soft dirty clean area */
	#	else
	#		define VM_SOFTDIRTY		0
	#	endif

	#	define VM_MIXEDMAP			0x10000000	/* Can contain "page_s" and pure PFN pages */
	#	define VM_HUGEPAGE			0x20000000	/* MADV_HUGEPAGE marked this vma */
	#	define VM_NOHUGEPAGE		0x40000000	/* MADV_NOHUGEPAGE marked this vma */
	#	define VM_MERGEABLE			0x80000000	/* KSM may merge identical pages */

	// #ifdef CONFIG_ARCH_USES_HIGH_VMA_FLAGS
	// #define VM_HIGH_ARCH_BIT_0	32	/* bit only usable on 64-bit architectures */
	// #define VM_HIGH_ARCH_BIT_1	33	/* bit only usable on 64-bit architectures */
	// #define VM_HIGH_ARCH_BIT_2	34	/* bit only usable on 64-bit architectures */
	// #define VM_HIGH_ARCH_BIT_3	35	/* bit only usable on 64-bit architectures */
	// #define VM_HIGH_ARCH_BIT_4	36	/* bit only usable on 64-bit architectures */
	// #define VM_HIGH_ARCH_0	BIT(VM_HIGH_ARCH_BIT_0)
	// #define VM_HIGH_ARCH_1	BIT(VM_HIGH_ARCH_BIT_1)
	// #define VM_HIGH_ARCH_2	BIT(VM_HIGH_ARCH_BIT_2)
	// #define VM_HIGH_ARCH_3	BIT(VM_HIGH_ARCH_BIT_3)
	// #define VM_HIGH_ARCH_4	BIT(VM_HIGH_ARCH_BIT_4)
	// #endif /* CONFIG_ARCH_USES_HIGH_VMA_FLAGS */

	// #ifdef CONFIG_ARCH_HAS_PKEYS
	// # define VM_PKEY_SHIFT	VM_HIGH_ARCH_BIT_0
	// # define VM_PKEY_BIT0	VM_HIGH_ARCH_0	/* A protection key is a 4-bit value */
	// # define VM_PKEY_BIT1	VM_HIGH_ARCH_1	/* on x86 and 5-bit value on ppc64   */
	// # define VM_PKEY_BIT2	VM_HIGH_ARCH_2
	// # define VM_PKEY_BIT3	VM_HIGH_ARCH_3
	// #ifdef CONFIG_PPC
	// # define VM_PKEY_BIT4  VM_HIGH_ARCH_4
	// #else
	// # define VM_PKEY_BIT4  0
	// #endif
	// #endif /* CONFIG_ARCH_HAS_PKEYS */

	// #if defined(CONFIG_X86)
	// # define VM_PAT		VM_ARCH_1	/* PAT reserves whole VMA at once (x86) */
	// #elif defined(CONFIG_PPC)
	// # define VM_SAO		VM_ARCH_1	/* Strong Access Ordering (powerpc) */
	// #elif defined(CONFIG_PARISC)
	// # define VM_GROWSUP	VM_ARCH_1
	// #elif defined(CONFIG_IA64)
	// # define VM_GROWSUP	VM_ARCH_1
	// #elif defined(CONFIG_SPARC64)
	// # define VM_SPARC_ADI	VM_ARCH_1	/* Uses ADI tag for access control */
	// # define VM_ARCH_CLEAR	VM_SPARC_ADI
	// #elif defined(CONFIG_ARM64)
	// # define VM_ARM64_BTI	VM_ARCH_1	/* BTI guarded page, a.k.a. GP bit */
	// # define VM_ARCH_CLEAR	VM_ARM64_BTI
	// #elif !defined(CONFIG_MMU)
	// # define VM_MAPPED_COPY	VM_ARCH_1	/* T if mapped copy of data (nommu mmap) */
	// #endif

	// #if defined(CONFIG_ARM64_MTE)
	// # define VM_MTE		VM_HIGH_ARCH_0	/* Use Tagged memory for access control */
	// # define VM_MTE_ALLOWED	VM_HIGH_ARCH_1	/* Tagged memory permitted */
	// #else
	// # define VM_MTE		VM_NONE
	// # define VM_MTE_ALLOWED	VM_NONE
	// #endif

	#ifndef VM_GROWSUP
	#	define VM_GROWSUP	VM_NONE
	#endif

	// #ifdef CONFIG_HAVE_ARCH_USERFAULTFD_MINOR
	// # define VM_UFFD_MINOR_BIT	37
	// # define VM_UFFD_MINOR		BIT(VM_UFFD_MINOR_BIT)	/* UFFD minor faults */
	// #else /* !CONFIG_HAVE_ARCH_USERFAULTFD_MINOR */
	// # define VM_UFFD_MINOR		VM_NONE
	// #endif /* CONFIG_HAVE_ARCH_USERFAULTFD_MINOR */

	// /* Bits set in the VMA until the stack is in its final location */
	#define VM_STACK_INCOMPLETE_SETUP	(VM_RAND_READ | VM_SEQ_READ)

	// #define TASK_EXEC ((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0)
	#define TASK_EXEC	VM_EXEC

	/* Common data flag combinations */
	#define VM_DATA_FLAGS_TSK_EXEC (					\
				VM_READ | VM_WRITE | TASK_EXEC |		\
				VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC	\
			)
	#define VM_DATA_FLAGS_NON_EXEC (				\
				VM_READ | VM_WRITE | VM_MAYREAD |	\
				VM_MAYWRITE | VM_MAYEXEC			\
			)
	#define VM_DATA_FLAGS_EXEC (						\
				VM_READ | VM_WRITE | VM_EXEC |			\
				VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC	\
			)

	#ifndef VM_DATA_DEFAULT_FLAGS		/* arch can override this */
	#define VM_DATA_DEFAULT_FLAGS  VM_DATA_FLAGS_EXEC
	#endif

	#ifndef VM_STACK_DEFAULT_FLAGS		/* arch can override this */
	#define VM_STACK_DEFAULT_FLAGS VM_DATA_DEFAULT_FLAGS
	#endif

	#define VM_STACK		VM_GROWSDOWN

	#define VM_STACK_FLAGS		(VM_STACK | VM_STACK_DEFAULT_FLAGS | VM_ACCOUNT)

	/* VMA basic access permission flags */
	#define VM_ACCESS_FLAGS		(VM_READ | VM_WRITE | VM_EXEC)


	/*
	 * Special vmas that are non-mergable, non-mlock()able.
	 */
	#define VM_SPECIAL			(VM_IO | VM_DONTEXPAND | VM_PFNMAP | VM_MIXEDMAP)

	/* This mask prevents VMA from being scanned with khugepaged */
	#define VM_NO_KHUGEPAGED	(VM_SPECIAL | VM_HUGETLB)

	/* This mask defines which mm->def_flags a process can inherit its parent */
	#define VM_INIT_DEF_MASK	VM_NOHUGEPAGE

	/* This mask is used to clear all the VMA flags used by mlock */
	#define VM_LOCKED_CLEAR_MASK	(~(VM_LOCKED | VM_LOCKONFAULT))

	// /* Arch-specific flags to clear when updating VM flags on protection change */
	// #ifndef VM_ARCH_CLEAR
	// # define VM_ARCH_CLEAR	VM_NONE
	// #endif
	// #define VM_FLAGS_CLEAR	(ARCH_VM_PKEY_FLAGS | VM_ARCH_CLEAR)

#endif /* _LINUX_VM_MAP_CONST_H_ */
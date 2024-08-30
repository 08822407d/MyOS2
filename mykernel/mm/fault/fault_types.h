#ifndef _MMFAULT_TYPES_H_
#define _MMFAULT_TYPES_H_

	#include "../mm_type_declaration.h"


	/**
	 * enum vm_fault_reason - Page fault handlers return a bitmask of
	 * these values to tell the core VM what happened when handling the
	 * fault. Used to decide whether a process gets delivered SIGBUS or
	 * just gets major/minor fault counters bumped up.
	 *
	 * @VM_FAULT_OOM:		Out Of Memory
	 * @VM_FAULT_SIGBUS:		Bad access
	 * @VM_FAULT_MAJOR:		Page read from storage
	 * @VM_FAULT_HWPOISON:		Hit poisoned small page
	 * @VM_FAULT_HWPOISON_LARGE:	Hit poisoned large page. Index encoded
	 *				in upper bits
	 * @VM_FAULT_SIGSEGV:		segmentation fault
	 * @VM_FAULT_NOPAGE:		->fault installed the pte, not return page
	 * @VM_FAULT_LOCKED:		->fault locked the returned page
	 * @VM_FAULT_RETRY:		->fault blocked, must retry
	 * @VM_FAULT_FALLBACK:		huge page fault failed, fall back to small
	 * @VM_FAULT_DONE_COW:		->fault has fully handled COW
	 * @VM_FAULT_NEEDDSYNC:		->fault did not modify page tables and needs
	 *				fsync() to complete (for synchronous page faults
	 *				in DAX)
	 * @VM_FAULT_COMPLETED:		->fault completed, meanwhile mmap lock released
	 * @VM_FAULT_HINDEX_MASK:	mask HINDEX value
	 *
	 */
	enum vm_fault_reason {
		VM_FAULT_OOM			= (__force vm_fault_t)0x000001,
		VM_FAULT_SIGBUS			= (__force vm_fault_t)0x000002,
		VM_FAULT_MAJOR			= (__force vm_fault_t)0x000004,
		VM_FAULT_HWPOISON		= (__force vm_fault_t)0x000010,
		VM_FAULT_HWPOISON_LARGE	= (__force vm_fault_t)0x000020,
		VM_FAULT_SIGSEGV		= (__force vm_fault_t)0x000040,
		VM_FAULT_NOPAGE			= (__force vm_fault_t)0x000100,
		VM_FAULT_LOCKED			= (__force vm_fault_t)0x000200,
		VM_FAULT_RETRY			= (__force vm_fault_t)0x000400,
		VM_FAULT_FALLBACK		= (__force vm_fault_t)0x000800,
		VM_FAULT_DONE_COW		= (__force vm_fault_t)0x001000,
		VM_FAULT_NEEDDSYNC		= (__force vm_fault_t)0x002000,
		VM_FAULT_COMPLETED		= (__force vm_fault_t)0x004000,
		VM_FAULT_HINDEX_MASK	= (__force vm_fault_t)0x0f0000,
	};

	/**
	 * enum fault_flag - Fault flag definitions.
	 * @FAULT_FLAG_WRITE: Fault was a write fault.
	 * @FAULT_FLAG_MKWRITE: Fault was mkwrite of existing PTE.
	 * @FAULT_FLAG_ALLOW_RETRY: Allow to retry the fault if blocked.
	 * @FAULT_FLAG_RETRY_NOWAIT: Don't drop mmap_lock and wait when retrying.
	 * @FAULT_FLAG_KILLABLE: The fault task is in SIGKILL killable region.
	 * @FAULT_FLAG_TRIED: The fault has been tried once.
	 * @FAULT_FLAG_USER: The fault originated in userspace.
	 * @FAULT_FLAG_REMOTE: The fault is not for current task/mm.
	 * @FAULT_FLAG_INSTRUCTION: The fault was during an instruction fetch.
	 * @FAULT_FLAG_INTERRUPTIBLE: The fault can be interrupted by non-fatal signals.
	 *
	 * About @FAULT_FLAG_ALLOW_RETRY and @FAULT_FLAG_TRIED: we can specify
	 * whether we would allow page faults to retry by specifying these two
	 * fault flags correctly.  Currently there can be three legal combinations:
	 *
	 * (a) ALLOW_RETRY and !TRIED:  this means the page fault allows retry, and
	 *                              this is the first try
	 *
	 * (b) ALLOW_RETRY and TRIED:   this means the page fault allows retry, and
	 *                              we've already tried at least once
	 *
	 * (c) !ALLOW_RETRY and !TRIED: this means the page fault does not allow retry
	 *
	 * The unlisted combination (!ALLOW_RETRY && TRIED) is illegal and should never
	 * be used.  Note that page faults can be allowed to retry for multiple times,
	 * in which case we'll have an initial fault with flags (a) then later on
	 * continuous faults with flags (b).  We should always try to detect pending
	 * signals before a retry to make sure the continuous page faults can still be
	 * interrupted if necessary.
	 */
	enum fault_flag {
		FAULT_FLAG_WRITE			= 1 << 0,
		FAULT_FLAG_MKWRITE			= 1 << 1,
		FAULT_FLAG_ALLOW_RETRY		= 1 << 2,
		FAULT_FLAG_RETRY_NOWAIT		= 1 << 3,
		FAULT_FLAG_KILLABLE			= 1 << 4,
		FAULT_FLAG_TRIED			= 1 << 5,
		FAULT_FLAG_USER				= 1 << 6,
		FAULT_FLAG_REMOTE			= 1 << 7,
		FAULT_FLAG_INSTRUCTION		= 1 << 8,
		FAULT_FLAG_INTERRUPTIBLE	= 1 << 9,
		FAULT_FLAG_UNSHARE			= 1 << 10,
		FAULT_FLAG_ORIG_PTE_VALID	= 1 << 11,
		FAULT_FLAG_VMA_LOCK			= 1 << 12,
	};

	/*
	 * vm_fault is filled by the pagefault handler and passed to the vma's
	 * ->fault function. The vma's ->fault is responsible for returning a bitmask
	 * of VM_FAULT_xxx flags that give details about how the fault was handled.
	 *
	 * MM layer fills up gfp_mask for page allocations but fault handler might
	 * alter it if its implementation requires a different allocation context.
	 *
	 * pgoff should be used in favour of virtual_address, if possible.
	 */
	typedef struct vm_fault {
		const struct {
			vma_s		*vma;		/* Target VMA */
			gfp_t		gfp_mask;	/* gfp mask to be used for allocations */
			pgoff_t		pgoff;		/* Logical page offset based on vma */
			ulong		address;	/* Faulting virtual address */
		};
		union {
			enum fault_flag	flags;		/* FAULT_FLAG_xxx flags
										* XXX: should really be 'const' */
			struct {
				unchar
					FF_WRITE			: 1,
					FF_MKWRITE			: 1,
					FF_ALLOW_RETRY		: 1,
					FF_RETRY_NOWAIT		: 1,
					FF_KILLABLE			: 1,
					FF_TRIED			: 1,
					FF_USER				: 1,
					FF_REMOTE			: 1,
					FF_INSTRUCTION		: 1,
					FF_INTERRUPTIBLE	: 1,
					FF_UNSHARE			: 1,
					FF_ORIG_PTE_VALID	: 1,
					FF_VMA_LOCK			: 1;
			} flag_defs;
		};

		p4d_t			*p4d;
		pud_t			*pud;		/* Pointer to pud entry matching
									 * the 'address'
									 */
		pmd_t			*pmd;		/* Pointer to pmd entry matching
									 * the 'address' */
		pte_t			*pte;		/* Pointer to pte entry matching
									 * the 'address'. NULL if the page
									 * table hasn't been allocated.
									 */
		// union {
			pte_t		orig_pte;	/* Value of PTE at the time of fault */
		// 	pmd_t		orig_pmd;	/* Value of PMD at the time of fault,
		// 							 * used by PMD fault only.
		// 							 */
		// };

		page_s			*cow_page;	/* Page handler may use for COW fault */
		page_s			*page;		/* ->fault handlers should return a
									 * page here, unless VM_FAULT_NOPAGE
									 * is set (which is also implied by
									 * VM_FAULT_ERROR).
									 */
		/* These three entries are valid only while holding ptl lock */
		// spinlock_t		*ptl;		/* Page table lock.
		// 							 * Protects pte page table if 'pte'
		// 							 * is not NULL, otherwise pmd.
		// 							 */
		// pgtable_t		prealloc_pte;
		// 							/* Pre-allocated pte page table.
		// 							 * vm_ops->map_pages() sets up a page
		// 							 * table from atomic context.
		// 							 * do_fault_around() pre-allocates
		// 							 * page table to avoid allocation from
		// 							 * atomic context.
		// 							 */

		task_s			*curr_tsk;
	} vm_fault_s;

#endif /* _MMFAULT_TYPES_H_ */

#ifndef _VM_MAP_TYPES_H_
#define _VM_MAP_TYPES_H_

	#include "vm_map_const.h"

	struct mm_struct;
	typedef struct mm_struct mm_s;
	struct vm_operations_struct;
	typedef struct vm_operations_struct vm_ops_s;
	struct file;
	typedef struct file file_s;
	struct vm_area_struct;
	typedef struct vm_area_struct vma_s;

	/*
	 * This struct describes a virtual memory area. There is one of these
	 * per VM-area/task. A VM area is any part of the process virtual memory
	 * space that has a special rule for the page-fault handlers (ie a shared
	 * library, the executable area etc).
	 */
	typedef struct vm_area_struct {
		/* The first cache line has the info for VMA tree walking. */

		unsigned long	vm_start;	/* Our start address within vm_mm. */
		unsigned long	vm_end;		/* The first byte after our end address
									   within vm_mm. */

		/* linked list of VM areas per task, sorted by address */
		vma_s			*vm_next, *vm_prev;

		// struct rb_node vm_rb;

		/*
		* Largest free memory gap in bytes to the left of this VMA.
		* Either between this VMA and vma->vm_prev, or between one of the
		* VMAs below us in the VMA rbtree and its ->vm_prev. This helps
		* get_unmapped_area find a free area of the right size.
		*/
		// unsigned long rb_subtree_gap;

		/* Second cache line starts here. */

		mm_s			*vm_mm;		/* The address space we belong to. */

		/*
		* Access permissions of this VMA.
		* See vmf_insert_mixed_prot() for discussion.
		*/
		// pgprot_t vm_page_prot;
		unsigned long	vm_flags;	/* Flags, see mm.h. */

		/*
		 * For areas with an address space and backing store,
		 * linkage into the address_space->i_mmap interval tree.
		 *
		 * For private anonymous mappings, a pointer to a null terminated string
		 * containing the name given to the vma, or NULL if unnamed.
		 */

		// union
		// {
		// 	struct
		// 	{
		// 		struct rb_node rb;
		// 		unsigned long rb_subtree_last;
		// 	} shared;
		// 	/*
		// 	* Serialized by mmap_sem. Never use directly because it is
		// 	* valid only when vm_file is NULL. Use anon_vma_name instead.
		// 	*/
		// 	struct anon_vma_name *anon_name;
		// };

		/*
		 * A file's MAP_PRIVATE vma can be in both i_mmap tree and anon_vma
		 * list, after a COW of one of the file pages.	A MAP_SHARED vma
		 * can only be in the i_mmap tree.  An anonymous MAP_PRIVATE, stack
		 * or brk vma (with NULL file) can only be in an anon_vma list.
		 */
		// List_s			anon_vma_chain; /* Serialized by mmap_lock &
		// 								* page_table_lock */
		// anon_vma_s		*anon_vma;		 /* Serialized by page_table_lock */

		/* Function pointers to deal with this struct. */
		const vm_ops_s	*vm_ops;

		/* Information about our backing store: */
		unsigned long	vm_pgoff;	/* Offset (within vm_file) in PAGE_SIZE units */
		file_s			*vm_file;	/* File we map to (can be NULL). */
		// void *vm_private_data;	/* was vm_pte (shared mem) */

	// #ifdef CONFIG_SWAP
	// 	atomic_long_t swap_readahead_info;
	// #endif
	// #ifndef CONFIG_MMU
	// 	struct vm_region *vm_region; /* NOMMU mapping region */
	// #endif
	// #ifdef CONFIG_NUMA
	// 	struct mempolicy *vm_policy; /* NUMA policy for the VMA */
	// #endif
	// 	struct vm_userfaultfd_ctx vm_userfaultfd_ctx;
	} vma_s;

	/*
	 * These are the virtual MM functions - opening of an area, closing and
	 * unmapping it (needed to keep files on disk up-to-date etc), pointer
	 * to the functions called when a no-page or a wp-page exception occurs.
	 */
	typedef struct vm_operations_struct {
		void	(*open)(vma_s * area);
		/**
		 * @close: Called when the VMA is being removed from the MM.
		 * Context: User context.  May sleep.  Caller holds mmap_lock.
		 */
		void	(*close)(vma_s * area);
		/* Called any time before splitting to check if it's allowed */
		int		(*may_split)(vma_s *area, unsigned long addr);
		int		(*mremap)(vma_s *area);
		// /*
		//  * Called by mprotect() to make driver-specific permission
		//  * checks before mprotect() is finalised.   The VMA must not
		//  * be modified.  Returns 0 if eprotect() can proceed.
		//  */
		// int (*mprotect)(vma_s *vma, unsigned long start,
		// 		unsigned long end, unsigned long newflags);
		vm_fault_t (*fault)(vm_fault_s *vmf);
		// vm_fault_t (*huge_fault)(vm_fault_s *vmf,
		// 		enum page_entry_size pe_size);
		vm_fault_t (*map_pages)(vm_fault_s *vmf,
				pgoff_t start_pgoff, pgoff_t end_pgoff);
		// unsigned long (*pagesize)(vma_s * area);

		/* notification that a previously read-only page is about to become
		 * writable, if an error is returned it will cause a SIGBUS */
		vm_fault_t (*page_mkwrite)(vm_fault_s *vmf);

		// /* same as page_mkwrite when using VM_PFNMAP|VM_MIXEDMAP */
		// vm_fault_t (*pfn_mkwrite)(vm_fault_s *vmf);

		// /* called by access_process_vm when get_user_pages() fails, typically
		//  * for use by special VMAs. See also generic_access_phys() for a generic
		//  * implementation useful for any iomem mapping.
		//  */
		// int (*access)(vma_s *vma, unsigned long addr,
		// 		void *buf, int len, int write);

		// /* Called by the /proc/PID/maps code to ask the vma whether it
		//  * has a special name.  Returning non-NULL will also cause this
		//  * vma to be dumped unconditionally. */
		// const char *(*name)(vma_s *vma);

	// #ifdef CONFIG_NUMA
		// /*
		//  * set_policy() op must add a reference to any non-NULL @new mempolicy
		//  * to hold the policy upon return.  Caller should pass NULL @new to
		//  * remove a policy and fall back to surrounding context--i.e. do not
		//  * install a MPOL_DEFAULT policy, nor the task or system default
		//  * mempolicy.
		//  */
		// int (*set_policy)(vma_s *vma, struct mempolicy *new);

		// /*
		//  * get_policy() op must add reference [mpol_get()] to any policy at
		//  * (vma,addr) marked as MPOL_SHARED.  The shared policy infrastructure
		//  * in mm/mempolicy.c will do this automatically.
		//  * get_policy() must NOT add a ref if the policy at (vma,addr) is not
		//  * marked as MPOL_SHARED. vma policies are protected by the mmap_lock.
		//  * If no [shared/vma] mempolicy exists at the addr, get_policy() op
		//  * must return NULL--i.e., do not "fallback" to task or system default
		//  * policy.
		//  */
		// struct mempolicy *(*get_policy)(vma_s *vma, unsigned long addr);
	// #endif
		// /*
		//  * Called by vm_normal_page() for special PTEs to find the
		//  * page for @addr.  This is useful if the default behavior
		//  * (using pte_page()) would not find the correct page.
		//  */
		// page_s *(*find_special_page)(vma_s *vma, unsigned long addr);
	} vm_ops_s;

#endif /* _VM_MAP_TYPES_H_ */
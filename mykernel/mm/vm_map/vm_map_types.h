#ifndef _VM_MAP_TYPES_H_
#define _VM_MAP_TYPES_H_

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

#endif /* _VM_MAP_TYPES_H_ */
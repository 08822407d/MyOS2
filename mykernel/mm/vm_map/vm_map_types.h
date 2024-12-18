#ifndef _LINUX_VM_MAP_TYPES_H_
#define _LINUX_VM_MAP_TYPES_H_

	#include "../mm_type_declaration.h"


	/*
	 * This struct describes a virtual memory area. There is one of these
	 * per VM-area/task. A VM area is any part of the process virtual memory
	 * space that has a special rule for the page-fault handlers (ie a shared
	 * library, the executable area etc).
	 */
	struct vm_area_struct {
		/* The first cache line has the info for VMA tree walking. */

		union {
			struct {
				/* VMA covers [vm_start; vm_end) addresses within mm */
				ulong	vm_start;
				ulong	vm_end;
			};
	// #ifdef CONFIG_PER_VMA_LOCK
	// 		struct rcu_head vm_rcu;	/* Used for deferred freeing. */
	// #endif
		};

		mm_s			*vm_mm;		/* The address space we belong to. */
		pgprot_t		vm_page_prot;

		/*
		 * Flags, see mm.h.
		 * To modify use vm_flags_{init|reset|set|clear|mod} functions.
		 */
		// union {
		// 	const vm_flags_t		vm_flags;
		// 	vm_flags_t __private	__vm_flags;
		// };
		vm_flags_t		vm_flags;


		// /*
		//  * For areas with an address space and backing store,
		//  * linkage into the address_space->i_mmap interval tree.
		//  *
		//  */
		// struct {
		// 	struct rb_node rb;
		// 	unsigned long rb_subtree_last;
		// } shared;
		List_s			list;

		/*
		 * A file's MAP_PRIVATE vma can be in both i_mmap tree and anon_vma
		 * list, after a COW of one of the file pages.	A MAP_SHARED vma
		 * can only be in the i_mmap tree.  An anonymous MAP_PRIVATE, stack
		 * or brk vma (with NULL file) can only be in an anon_vma list.
		 */
		List_s			anon_vma_chain;		/* Serialized by mmap_lock &
											 * page_table_lock */
		anon_vma_s		*anon_vma;			/* Serialized by page_table_lock */

		/* Function pointers to deal with this struct. */
		const vm_ops_s	*vm_ops;

		/* Information about our backing store: */
		ulong			vm_pgoff;			/* Offset (within vm_file) in PAGE_SIZE units */
		file_s			*vm_file;			/* File we map to (can be NULL). */
		void			*vm_private_data;	/* was vm_pte (shared mem) */

	// #ifdef CONFIG_ANON_VMA_NAME
	// 	/*
	// 	 * For private and shared anonymous mappings, a pointer to a null
	// 	 * terminated string containing the name given to the vma, or NULL if
	// 	 * unnamed. Serialized by mmap_lock. Use anon_vma_name to access.
	// 	 */
	// 	struct anon_vma_name *anon_name;
	// #endif
	// #ifdef CONFIG_SWAP
	// 	atomic_long_t swap_readahead_info;
	// #endif
	// #ifndef CONFIG_MMU
	// 	struct vm_region *vm_region;	/* NOMMU mapping region */
	// #endif
	// #ifdef CONFIG_NUMA
	// 	struct mempolicy *vm_policy;	/* NUMA policy for the VMA */
	// #endif
	// #ifdef CONFIG_NUMA_BALANCING
	// 	struct vma_numab_state *numab_state;	/* NUMA Balancing state */
	// #endif
	// 	struct vm_userfaultfd_ctx vm_userfaultfd_ctx;
	};

	/*
	 * These are the virtual MM functions - opening of an area, closing and
	 * unmapping it (needed to keep files on disk up-to-date etc), pointer
	 * to the functions called when a no-page or a wp-page exception occurs.
	 */
	struct vm_operations_struct {
		void		(*open)(vma_s * area);
		/**
		 * @close: Called when the VMA is being removed from the MM.
		 * Context: User context.  May sleep.  Caller holds mmap_lock.
		 */
		void		(*close)(vma_s * area);
		/* Called any time before splitting to check if it's allowed */
		// int			(*may_split)(vma_s *area, ulong addr);
		int			(*mremap)(vma_s *area);
		// /*
		//  * Called by mprotect() to make driver-specific permission
		//  * checks before mprotect() is finalised.   The VMA must not
		//  * be modified.  Returns 0 if eprotect() can proceed.
		//  */
		// int (*mprotect)(vma_s *vma, unsigned long start,
		// 		unsigned long end, unsigned long newflags);
		vm_fault_t	(*fault)(vm_fault_s *vmf);
		// vm_fault_t (*huge_fault)(vm_fault_s *vmf,
		// 		enum page_entry_size pe_size);
		vm_fault_t	(*map_single_page)(vm_fault_s *vmf, pgoff_t pgoff);
		vm_fault_t	(*map_pages)(vm_fault_s *vmf,
						pgoff_t start_pgoff, pgoff_t end_pgoff);
		// unsigned long (*pagesize)(vma_s * area);

		/* notification that a previously read-only page is about to become
		 * writable, if an error is returned it will cause a SIGBUS */
		vm_fault_t	(*page_mkwrite)(vm_fault_s *vmf);

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
	};

	/*
	 * The anon_vma heads a list of private "related" vmas, to scan if
	 * an anonymous page pointing to this anon_vma needs to be unmapped:
	 * the vmas on the list will be related by forking, or by splitting.
	 *
	 * Since vmas come and go as they are split and merged (particularly
	 * in mprotect), the mapping field of an anonymous page cannot point
	 * directly to a vma: instead it points to an anon_vma, on whose list
	 * the related vmas can be easily linked or unlinked.
	 *
	 * After unlinking the last vma on the list, we must garbage collect
	 * the anon_vma object itself: we're guaranteed no page can be
	 * pointing to this anon_vma once its vma list is empty.
	 */
	struct anon_vma {
		anon_vma_s	*root;		/* Root of this anon_vma tree */
		// rwsem_t rwsem;	/* W: modification, R: walking the list */
		/*
		 * The refcount is taken on an anon_vma when there is no
		 * guarantee that the vma of page tables will exist for
		 * the duration of the operation. A caller that takes
		 * the reference is responsible for clearing up the
		 * anon_vma if they are the last user on release
		 */
		atomic_t	refcount;

		// /*
		//  * Count of child anon_vmas. Equals to the count of all anon_vmas that
		//  * have ->parent pointing to this one, including itself.
		//  *
		//  * This counter is used for making decision about reusing anon_vma
		//  * instead of forking new one. See comments in function anon_vma_clone.
		//  */
		// ulong		num_children;
		// /* Count of VMAs whose ->anon_vma pointer points to this object. */
		// ulong		num_active_vmas;

		anon_vma_s	*parent;	/* Parent of this anon_vma */

		// /*
		//  * NOTE: the LSB of the rb_root.rb_node is set by
		//  * mm_take_all_locks() _after_ taking the above lock. So the
		//  * rb_root must only be read/written after taking the above lock
		//  * to be sure to see a valid next pointer. The LSB bit itself
		//  * is serialized by a system wide lock only visible to
		//  * mm_take_all_locks() (mm_all_locks_mutex).
		//  */

		// /* Interval tree of private "related" vmas */
		// struct rb_root_cached rb_root;
	};

	/*
	 * The copy-on-write semantics of fork mean that an anon_vma
	 * can become associated with multiple processes. Furthermore,
	 * each child process will have its own anon_vma, where new
	 * pages for that process are instantiated.
	 *
	 * This structure allows us to find the anon_vmas associated
	 * with a VMA, or the VMAs associated with an anon_vma.
	 * The "same_vma" list contains the anon_vma_chains linking
	 * all the anon_vmas associated with this VMA.
	 * The "rb" field indexes on an interval tree the anon_vma_chains
	 * which link all the VMAs associated with this anon_vma.
	 */
	struct anon_vma_chain {
		vma_s		*vma;
		anon_vma_s	*anon_vma;
		List_hdr_s	same_vma;		/* locked by mmap_lock & page_table_lock */
		// struct rb_node	rb;			/* locked by anon_vma->rwsem */
		// ulong			rb_subtree_last;
	// #ifdef CONFIG_DEBUG_VM_RB
	// 	unsigned long cached_vma_start, cached_vma_last;
	// #endif
	};

	struct vm_unmapped_area_info {
	#define VM_UNMAPPED_AREA_TOPDOWN 1
		ulong	flags;
		ulong	length;
		ulong	low_limit;
		ulong	high_limit;
		ulong	align_mask;
		ulong	align_offset;
		ulong	start_gap;
	};

#endif /* _LINUX_VM_MAP_TYPES_H_ */
#ifndef _MM_MISC_TYPES_H_
#define _MM_MISC_TYPES_H_

	#include "../mm_type_declaration.h"


	typedef struct mm_struct {
		// size_t				entry_point;
		struct {
			List_hdr_s		mm_mt;
			// u64 vmacache_seqnum; /* per-thread vmacache */
			ulong (*get_unmapped_area)(file_s *filp, ulong addr,
					ulong len, ulong pgoff, ulong flags);
			ulong			mmap_base;			/* base of mmap area */
			ulong			mmap_legacy_base; /* base of mmap area in bottom-up allocations */
	// #ifdef CONFIG_HAVE_ARCH_COMPAT_MMAP_BASES
			// /* Base addresses for compatible mmap() */
			// ulong mmap_compat_base;
			// ulong mmap_compat_legacy_base;
	// #endif
			ulong			task_size;			/* size of task vm space */
			ulong			highest_vm_end;		/* highest vma end address */
			pgd_t			*pgd;

	// #ifdef CONFIG_MEMBARRIER
			// /**
			//  * @membarrier_state: Flags controlling membarrier behavior.
			//  *
			//  * This field is close to @pgd to hopefully fit in the same
			//  * cache-line, which needs to be touched by switch_mm().
			//  */
			// atomic_t membarrier_state;
	// #endif

			/**
			 * @mm_users: The number of users including userspace.
			 *
			 * Use mmget()/mmget_not_zero()/mmput() to modify. When this
			 * drops to 0 (i.e. when the task exits and there are no other
			 * temporary reference holders), we also release a reference on
			 * @mm_count (which may then free the &mm_s if
			 * @mm_count also drops to 0).
			 */
			atomic_t		mm_users;

			/**
			 * @mm_count: The number of references to &mm_s
			 * (@mm_users count as 1).
			 *
			 * Use mmgrab()/mmdrop() to modify. When this drops to 0, the
			 * &mm_s is freed.
			 */
			atomic_t		mm_refcount;
			atomic_long_t	pgtables_bytes;		/* PTE page table pages */
			int				map_count;			/* number of VMAs */
			spinlock_t		page_table_lock;	/* Protects page tables and some
												   counters */
			// /*
			// * With some kernel config, the current mmap_lock's offset
			// * inside 'mm_struct' is at 0x120, which is very optimal, as
			// * its two hot fields 'count' and 'owner' sit in 2 different
			// * cachelines,  and when mmap_lock is highly contended, both
			// * of the 2 fields will be accessed frequently, current layout
			// * will help to reduce cache bouncing.
			// *
			// * So please be careful with adding new fields before
			// * mmap_lock, which can easily push the 2 fields into one
			// * cacheline.
			// */
			// rwsem_t mmap_lock;

			// List_s mmlist; /* List of maybe swapped mm's.	These
			// 						* are globally strung together off
			// 						* init_mm.mmlist, and are protected
			// 						* by mmlist_lock
			// 						*/

			// ulong hiwater_rss; /* High-watermark of RSS usage */
			// ulong hiwater_vm;  /* High-water virtual memory usage */

			ulong			total_vm;			/* Total pages mapped */
			ulong			locked_vm;			/* Pages that have PG_mlocked set */
			atomic64_t		pinned_vm;			/* Refcount permanently increased */
			ulong			data_vm;			/* VM_WRITE & ~VM_SHARED & ~VM_STACK */
			ulong			exec_vm;			/* VM_EXEC & ~VM_WRITE & ~VM_STACK */
			ulong			stack_vm;			/* VM_STACK */
			ulong			def_flags;

			// /**
			//  * @write_protect_seq: Locked when any thread is write
			//  * protecting pages mapped by this mm to enforce a later COW,
			//  * for instance during page table copying for fork().
			//  */
			// seqcount_t write_protect_seq;

			// spinlock_t arg_lock; /* protect the below fields */

			ulong			start_code, end_code, start_data, end_data;
			ulong			start_brk, brk, start_stack;
			ulong			arg_start, arg_end, env_start, env_end;

			ulong			saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */

			// /*
			// * Special counters, in some configurations protected by the
			// * page_table_lock, in other configurations by being atomic.
			// */
			// struct mm_rss_stat rss_stat;

			linux_bfmt_s	*binfmt;

			// /* Architecture-specific MM context */
			// mm_context_t context;

			ulong			flags;				/* Must use atomic bitops to access */

	// #ifdef CONFIG_AIO
	// 		spinlock_t ioctx_lock;
	// 		struct kioctx_table __rcu *ioctx_table;
	// #endif
			// /*
			//  * "owner" points to a task that is regarded as the canonical
			//  * user/owner of this mm. All of the following must be true in
			//  * order for it to be changed:
			//  *
			//  * current == mm->owner
			//  * current->mm != mm
			//  * new_owner->mm == mm
			//  * new_owner->alloc_lock is held
			//  */
			// task_s __rcu	*owner;
			// struct user_namespace *user_ns;

			/* store ref to file /proc/<pid>/exe symlink points to */
			file_s __rcu	*exe_file;
	// #ifdef CONFIG_MMU_NOTIFIER
	// 		struct mmu_notifier_subscriptions *notifier_subscriptions;
	// #endif
	// #if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
	// 		pgtable_t pmd_huge_pte; /* protected by page_table_lock */
	// #endif
	// #ifdef CONFIG_NUMA_BALANCING
	// 		/*
	// 		* numa_next_scan is the next time that the PTEs will be marked
	// 		* pte_numa. NUMA hinting faults will gather statistics and
	// 		* migrate pages to new nodes if necessary.
	// 		*/
	// 		ulong numa_next_scan;

	// 		/* Restart point for scanning and setting pte_numa */
	// 		ulong numa_scan_offset;

	// 		/* numa_scan_seq prevents two threads setting pte_numa */
	// 		int numa_scan_seq;
	// #endif
	// 		/*
	// 		* An operation with batched TLB flushing is going on. Anything
	// 		* that can move process memory needs to flush the TLB when
	// 		* moving a PROT_NONE or PROT_NUMA mapped page.
	// 		*/
	// 		atomic_t tlb_flush_pending;
	// #ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
	// 		/* See flush_tlb_batched_pending() */
	// 		atomic_t tlb_flush_batched;
	// #endif
	// 		struct uprobes_state uprobes_state;
	// #ifdef CONFIG_PREEMPT_RT
	// 		struct rcu_head delayed_drop;
	// #endif
	// #ifdef CONFIG_HUGETLB_PAGE
	// 		atomic_long_t hugetlb_usage;
	// #endif
	// 		struct work_struct async_put_work;

	// #ifdef CONFIG_IOMMU_SUPPORT
	// 		u32 pasid;
	// #endif
		};

	// 	/*
	// 	* The mm_cpumask needs to be at the end of mm_struct, because it
	// 	* is dynamically sized based on nr_cpu_ids.
	// 	*/
	// 	unsigned long cpu_bitmap[];
	} mm_s;

#endif /* _MM_MISC_TYPES_H_ */

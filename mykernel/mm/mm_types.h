#ifndef _LINUX_MM_TYPES_H_
#define _LINUX_MM_TYPES_H_

	#include "fault/mmfault_types.h"
    #include "page_alloc/page_alloc_types.h"
    #include "vm_map/vm_map_types.h"


	struct mm_struct;
	typedef struct mm_struct mm_s;

	typedef struct mm_struct {
		size_t				entry_point;
		struct
		{
			vma_s			*mmap;				/* list of VMAs */
			// struct rb_root mm_rb;
			// u64 vmacache_seqnum; /* per-thread vmacache */
	// #ifdef CONFIG_MMU
			// unsigned long (*get_unmapped_area)(file_s *filp,
			// 								unsigned long addr, unsigned long len,
			// 								unsigned long pgoff, unsigned long flags);
	// #endif
			unsigned long	mmap_base;			/* base of mmap area */
			// unsigned long mmap_legacy_base; /* base of mmap area in bottom-up allocations */
	// #ifdef CONFIG_HAVE_ARCH_COMPAT_MMAP_BASES
			// /* Base addresses for compatible mmap() */
			// unsigned long mmap_compat_base;
			// unsigned long mmap_compat_legacy_base;
	// #endif
			unsigned long	task_size;			/* size of task vm space */
			unsigned long	highest_vm_end;		/* highest vma end address */
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

	// #ifdef CONFIG_MMU
			atomic_long_t	pgtables_bytes;		/* PTE page table pages */
	// #endif
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
			// struct rw_semaphore mmap_lock;

			// List_s mmlist; /* List of maybe swapped mm's.	These
			// 						* are globally strung together off
			// 						* init_mm.mmlist, and are protected
			// 						* by mmlist_lock
			// 						*/

			// unsigned long hiwater_rss; /* High-watermark of RSS usage */
			// unsigned long hiwater_vm;  /* High-water virtual memory usage */

			unsigned long	total_vm;			/* Total pages mapped */
			unsigned long	locked_vm;			/* Pages that have PG_mlocked set */
			atomic64_t		pinned_vm;			/* Refcount permanently increased */
			unsigned long	data_vm;			/* VM_WRITE & ~VM_SHARED & ~VM_STACK */
			unsigned long	exec_vm;			/* VM_EXEC & ~VM_WRITE & ~VM_STACK */
			unsigned long	stack_vm;			/* VM_STACK */
			unsigned long	def_flags;

			// /**
			//  * @write_protect_seq: Locked when any thread is write
			//  * protecting pages mapped by this mm to enforce a later COW,
			//  * for instance during page table copying for fork().
			//  */
			// seqcount_t write_protect_seq;

			// spinlock_t arg_lock; /* protect the below fields */

			unsigned long	start_code, end_code, start_data, end_data;
			unsigned long	start_brk, brk, start_stack;
			unsigned long	arg_start, arg_end, env_start, env_end;

			// unsigned long saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */

			// /*
			// * Special counters, in some configurations protected by the
			// * page_table_lock, in other configurations by being atomic.
			// */
			// struct mm_rss_stat rss_stat;

			// struct linux_binfmt *binfmt;

			// /* Architecture-specific MM context */
			// mm_context_t context;

			unsigned long	flags;				/* Must use atomic bitops to access */

	// #ifdef CONFIG_AIO
	// 		spinlock_t ioctx_lock;
	// 		struct kioctx_table __rcu *ioctx_table;
	// #endif
	// #ifdef CONFIG_MEMCG
	// 		/*
	// 		* "owner" points to a task that is regarded as the canonical
	// 		* user/owner of this mm. All of the following must be true in
	// 		* order for it to be changed:
	// 		*
	// 		* current == mm->owner
	// 		* current->mm != mm
	// 		* new_owner->mm == mm
	// 		* new_owner->alloc_lock is held
	// 		*/
			task_s __rcu	*owner;
	// #endif
	// 		struct user_namespace *user_ns;

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
	// 		unsigned long numa_next_scan;

	// 		/* Restart point for scanning and setting pte_numa */
	// 		unsigned long numa_scan_offset;

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

	extern mm_s init_mm;

#endif /* _LINUX_MM_TYPES_H_ */
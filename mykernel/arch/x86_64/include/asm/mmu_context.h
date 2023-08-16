// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MMU_CONTEXT_H
#define _ASM_X86_MMU_CONTEXT_H

	#include <asm/desc.h>
	#include <linux/kernel/atomic.h>
	#include <linux/mm/mm_types.h>
	// #include <linux/pkeys.h>

	// #include <trace/events/tlb.h>

	#include <asm/tlbflush.h>
	// #include <asm/paravirt.h>
	// #include <asm/debugreg.h>
	// #include <asm/gsseg.h>

	// extern atomic64_t last_mm_ctx_id;

	// #ifdef CONFIG_PERF_EVENTS
	// DECLARE_STATIC_KEY_FALSE(rdpmc_never_available_key);
	// DECLARE_STATIC_KEY_FALSE(rdpmc_always_available_key);
	// void cr4_update_pce(void *ignored);
	// #endif

	// #ifdef CONFIG_MODIFY_LDT_SYSCALL
	// /*
	// * ldt_structs can be allocated, used, and freed, but they are never
	// * modified while live.
	// */
	// struct ldt_struct {
	// 	/*
	// 	* Xen requires page-aligned LDTs with special permissions.  This is
	// 	* needed to prevent us from installing evil descriptors such as
	// 	* call gates.  On native, we could merge the ldt_struct and LDT
	// 	* allocations, but it's not worth trying to optimize.
	// 	*/
	// 	struct desc_struct	*entries;
	// 	unsigned int		nr_entries;

	// 	/*
	// 	* If PTI is in use, then the entries array is not mapped while we're
	// 	* in user mode.  The whole array will be aliased at the addressed
	// 	* given by ldt_slot_va(slot).  We use two slots so that we can allocate
	// 	* and map, and enable a new LDT without invalidating the mapping
	// 	* of an older, still-in-use LDT.
	// 	*
	// 	* slot will be -1 if this LDT doesn't have an alias mapping.
	// 	*/
	// 	int			slot;
	// };

	// /*
	// * Used for LDT copy/destruction.
	// */
	// static inline void init_new_context_ldt(mm_s *mm)
	// {
	// 	mm->context.ldt = NULL;
	// 	init_rwsem(&mm->context.ldt_usr_sem);
	// }
	// int ldt_dup_context(mm_s *oldmm, mm_s *mm);
	// void destroy_context_ldt(mm_s *mm);
	// void ldt_arch_exit_mmap(mm_s *mm);
	// #else	/* CONFIG_MODIFY_LDT_SYSCALL */
	// static inline void init_new_context_ldt(mm_s *mm) { }
	// static inline int ldt_dup_context(mm_s *oldmm,
	// 				mm_s *mm)
	// {
	// 	return 0;
	// }
	// static inline void destroy_context_ldt(mm_s *mm) { }
	// static inline void ldt_arch_exit_mmap(mm_s *mm) { }
	// #endif

	// #ifdef CONFIG_MODIFY_LDT_SYSCALL
	// extern void load_mm_ldt(mm_s *mm);
	// extern void switch_ldt(mm_s *prev, mm_s *next);
	// #else
	// static inline void load_mm_ldt(mm_s *mm)
	// {
	// 	clear_LDT();
	// }
	// static inline void switch_ldt(mm_s *prev, mm_s *next)
	// {
	// 	DEBUG_LOCKS_WARN_ON(preemptible());
	// }
	// #endif

	// #ifdef CONFIG_ADDRESS_MASKING
	// static inline unsigned long mm_lam_cr3_mask(mm_s *mm)
	// {
	// 	return mm->context.lam_cr3_mask;
	// }

	// static inline void dup_lam(mm_s *oldmm, mm_s *mm)
	// {
	// 	mm->context.lam_cr3_mask = oldmm->context.lam_cr3_mask;
	// 	mm->context.untag_mask = oldmm->context.untag_mask;
	// }

	// #define mm_untag_mask mm_untag_mask
	// static inline unsigned long mm_untag_mask(mm_s *mm)
	// {
	// 	return mm->context.untag_mask;
	// }

	// static inline void mm_reset_untag_mask(mm_s *mm)
	// {
	// 	mm->context.untag_mask = -1UL;
	// }

	// #define arch_pgtable_dma_compat arch_pgtable_dma_compat
	// static inline bool arch_pgtable_dma_compat(mm_s *mm)
	// {
	// 	return !mm_lam_cr3_mask(mm) ||
	// 		test_bit(MM_CONTEXT_FORCE_TAGGED_SVA, &mm->context.flags);
	// }
	// #else

	// static inline unsigned long mm_lam_cr3_mask(mm_s *mm)
	// {
	// 	return 0;
	// }

	// static inline void dup_lam(mm_s *oldmm, mm_s *mm)
	// {
	// }

	// static inline void mm_reset_untag_mask(mm_s *mm)
	// {
	// }
	// #endif

	// #define enter_lazy_tlb enter_lazy_tlb
	// extern void enter_lazy_tlb(mm_s *mm, struct task_struct *tsk);

	// /*
	// * Init a new mm.  Used on mm copies, like at fork()
	// * and on mm's that are brand-new, like at execve().
	// */
	// #define init_new_context init_new_context
	// static inline int init_new_context(struct task_struct *tsk,
	// 				mm_s *mm)
	// {
	// 	mutex_init(&mm->context.lock);

	// 	mm->context.ctx_id = atomic64_inc_return(&last_mm_ctx_id);
	// 	atomic64_set(&mm->context.tlb_gen, 0);

	// #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	// 	if (cpu_feature_enabled(X86_FEATURE_OSPKE)) {
	// 		/* pkey 0 is the default and allocated implicitly */
	// 		mm->context.pkey_allocation_map = 0x1;
	// 		/* -1 means unallocated or invalid */
	// 		mm->context.execute_only_pkey = -1;
	// 	}
	// #endif
	// 	mm_reset_untag_mask(mm);
	// 	init_new_context_ldt(mm);
	// 	return 0;
	// }

	// #define destroy_context destroy_context
	// static inline void destroy_context(mm_s *mm)
	// {
	// 	destroy_context_ldt(mm);
	// }

	extern void switch_mm(mm_s *prev, mm_s *next,
				struct task_struct *tsk);

	extern void switch_mm_irqs_off(mm_s *prev, mm_s *next, task_s *tsk);
	// #define switch_mm_irqs_off switch_mm_irqs_off

	// #define activate_mm(prev, next)			\
	// do {						\
	// 	paravirt_activate_mm((prev), (next));	\
	// 	switch_mm((prev), (next), NULL);	\
	// } while (0);
	#define activate_mm(prev, next) do {			\
				switch_mm((prev), (next), NULL);	\
			} while (0);

	// #define deactivate_mm(tsk, mm)			\
	// do {						\
	// 	load_gs_index(0);			\
	// 	loadsegment(fs, 0);			\
	// } while (0)

	// static inline void arch_dup_pkeys(mm_s *oldmm,
	// 				mm_s *mm)
	// {
	// #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	// 	if (!cpu_feature_enabled(X86_FEATURE_OSPKE))
	// 		return;

	// 	/* Duplicate the oldmm pkey state in mm: */
	// 	mm->context.pkey_allocation_map = oldmm->context.pkey_allocation_map;
	// 	mm->context.execute_only_pkey   = oldmm->context.execute_only_pkey;
	// #endif
	// }

	// static inline int arch_dup_mmap(mm_s *oldmm, mm_s *mm)
	// {
	// 	arch_dup_pkeys(oldmm, mm);
	// 	paravirt_enter_mmap(mm);
	// 	dup_lam(oldmm, mm);
	// 	return ldt_dup_context(oldmm, mm);
	// }

	// static inline void arch_exit_mmap(mm_s *mm)
	// {
	// 	paravirt_arch_exit_mmap(mm);
	// 	ldt_arch_exit_mmap(mm);
	// }

	// static inline bool is_64bit_mm(mm_s *mm)
	// {
	// 	return	!IS_ENABLED(CONFIG_IA32_EMULATION) ||
	// 		!test_bit(MM_CONTEXT_UPROBE_IA32, &mm->context.flags);
	// }

	// static inline void arch_unmap(mm_s *mm, unsigned long start,
	// 				unsigned long end)
	// {
	// }

	// /*
	// * We only want to enforce protection keys on the current process
	// * because we effectively have no access to PKRU for other
	// * processes or any way to tell *which * PKRU in a threaded
	// * process we could use.
	// *
	// * So do not enforce things if the VMA is not from the current
	// * mm, or if we are in a kernel thread.
	// */
	// static inline bool arch_vma_access_permitted(struct vm_area_struct *vma,
	// 		bool write, bool execute, bool foreign)
	// {
	// 	/* pkeys never affect instruction fetches */
	// 	if (execute)
	// 		return true;
	// 	/* allow access if the VMA is not one from this process */
	// 	if (foreign || vma_is_foreign(vma))
	// 		return true;
	// 	return __pkru_allows_pkey(vma_pkey(vma), write);
	// }

	// unsigned long __get_current_cr3_fast(void);

	// #include <asm-generic/mmu_context.h>

#endif /* _ASM_X86_MMU_CONTEXT_H */

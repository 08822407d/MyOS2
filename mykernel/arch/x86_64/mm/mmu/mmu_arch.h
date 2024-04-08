/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MMU_H_
#define _ASM_X86_MMU_H_

	#include <linux/compiler/myos_optimize_option.h>

    #include "../mm_const_arch.h"
    #include "../mm_types_arch.h"
    #include "../mm_api_arch.h"


	#ifdef DEBUG

		extern int
		init_new_context(task_s *tsk, mm_s *mm);

	#endif

	#include "mmu_macro_arch.h"
	
	#if defined(ARCH_MMU_ARCH_DEFINATION) || !(DEBUG)

		/*
		 * Init a new mm.  Used on mm copies, like at fork()
		 * and on mm's that are brand-new, like at execve().
		 */
		PREFIX_STATIC_INLINE
		int
		init_new_context(task_s *tsk, mm_s *mm) {
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
		}

	#endif

#endif /* _ASM_X86_MMU_H_ */
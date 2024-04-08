#ifndef _ASM_X86_MMU_API_H_
#define _ASM_X86_MMU_API_H_

    #include "mmu_arch.h"


	extern void switch_mm(mm_s *prev, mm_s *next, task_s *tsk);
	extern void switch_mm_irqs_off(mm_s *prev, mm_s *next, task_s *tsk);

	void __flush_tlb_all(void);
	void flush_tlb_local(void);

#endif /* _ASM_X86_MMU_API_H_ */
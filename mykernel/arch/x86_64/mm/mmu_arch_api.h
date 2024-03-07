#ifndef _ASM_X86_MMU_API_H_
#define _ASM_X86_MMU_API_H_

    #include "mmu/mmu_const_arch.h"
    #include "mmu/mmu_types_arch.h"
    #include "mmu/mmu_arch.h"


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

	extern void switch_mm(mm_s *prev, mm_s *next, task_s *tsk);
	extern void switch_mm_irqs_off(mm_s *prev, mm_s *next, task_s *tsk);

	void __flush_tlb_all(void);
	void flush_tlb_local(void);

#endif /* _ASM_X86_MMU_API_H_ */
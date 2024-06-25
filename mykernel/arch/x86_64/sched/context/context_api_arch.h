#ifndef _ASM_X86_CONTEXT_API_H_
#define _ASM_X86_CONTEXT_API_H_


	#include "switch_to_arch.h"
	#include "thread_info_arch.h"


	task_s *__switch_to_asm(task_s *prev, task_s *next);
	__visible task_s *__switch_to(task_s *prev, task_s *next);
	asmlinkage void ret_from_fork_asm(void);
	extern void *task_stack_page(const task_s *task);


	/* Save actual FS/GS selectors and bases to current->thread */
	void current_save_fsgs(void);
	// extern void fpu_thread_struct_whitelist(unsigned long *offset, unsigned long *size);
	extern void
	start_thread(pt_regs_s *regs, unsigned long new_ip, unsigned long new_sp);

#endif /* _ASM_X86_CONTEXT_API_H_ */

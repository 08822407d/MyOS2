/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SWITCH_TO_MACRO_H_
#define _ASM_X86_SWITCH_TO_MACRO_H_


    #define __preempt_count_ref	(this_cpu_ptr(&pcpu_hot)->preempt_count)

	#define switch_to(prev, next, last)						\
			do {											\
				((last) = __switch_to_asm((prev), (next)));	\
			} while (0)

	#define TOP_OF_INIT_STACK (										\
				(unsigned long)&init_stack + sizeof(init_stack) -	\
					TOP_OF_KERNEL_STACK_PADDING						\
			)
	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})

	#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))

#endif /* _ASM_X86_SWITCH_TO_MACRO_H_ */

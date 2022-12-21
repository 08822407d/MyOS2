/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SWITCH_TO_H
#define _ASM_X86_SWITCH_TO_H

	#include <linux/sched/task_stack.h>


	#include <asm/ptrace.h>


	task_s *__switch_to_asm(task_s *prev, task_s *next);

	__visible task_s *__switch_to(task_s *prev, task_s *next);

	asmlinkage void ret_from_fork(void);

	/*
	 * This is the structure pointed to by thread.sp for an inactive task.  The
	 * order of the fields must match the code in __switch_to_asm().
	 */
	typedef struct inactive_task_frame {
		unsigned long r15;
		unsigned long r14;
		unsigned long r13;
		unsigned long r12;
		unsigned long bx;
		/*
		 * These two fields must be together.  They form a stack frame header,
		 * needed by get_frame_pointer().
		 */
		unsigned long bp;
		unsigned long ret_addr;
	} task_kframe_s;

	typedef struct fork_frame {
		task_kframe_s	frame;
		pt_regs_s		regs;
	} fork_frame_s;

	#define switch_to(prev, next, last)						\
			do {											\
				((last) = __switch_to_asm((prev), (next)));	\
			} while (0)

	// /* This is used when switching tasks or entering/exiting vm86 mode. */
	// static inline void update_task_stack(task_s *task) {
	// 	/* Xen PV enters the kernel on the thread stack. */
	// 	if (static_cpu_has(X86_FEATURE_XENPV))
	// 		load_sp0(task_top_of_stack(task));
	// }

	static inline void
	kthread_frame_init(task_kframe_s *frame, unsigned long fun, unsigned long arg) {
		frame->bx = fun;
		frame->r12 = arg;
	}

#endif /* _ASM_X86_SWITCH_TO_H */

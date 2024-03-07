/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SWITCH_TO_TYPES_H_
#define _ASM_X86_SWITCH_TO_TYPES_H_

	#include <asm/ptrace.h>

	/*
	 * This is the structure pointed to by thread.sp for an inactive task.  The
	 * order of the fields must match the code in __switch_to_asm().
	 */
	typedef struct inactive_task_frame {
		reg_t	r15;
		reg_t	r14;
		reg_t	r13;
		reg_t	r12;
		reg_t	bx;
		/*
		 * These two fields must be together.  They form a stack frame header,
		 * needed by get_frame_pointer().
		 */
		reg_t	bp;
		reg_t	ret_addr;
	} task_kframe_s;

	typedef struct fork_frame {
		task_kframe_s	frame;
		pt_regs_s		regs;
	} fork_frame_s;

#endif /* _ASM_X86_SWITCH_TO_TYPES_H_ */

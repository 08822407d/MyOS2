#ifndef _ASM_X86_SCHED_TYPE_DECLARATION_H_
#define _ASM_X86_SCHED_TYPE_DECLARATION_H_

	struct task_struct;
	typedef struct task_struct task_s;

	#include <linux/kernel/types.h>
	#include <asm/ptrace.h>


	/* switch_to */
	struct inactive_task_frame;
	typedef struct inactive_task_frame task_kframe_s;
	struct fork_frame;
	typedef struct fork_frame fork_frame_s;

	/* thread_info */
	struct thread_info;
	typedef struct thread_info thread_info_s;


	// #include <linux/kernel/mm_type_declaration.h>

#endif /* _ASM_X86_SCHED_TYPE_DECLARATION_H_ */
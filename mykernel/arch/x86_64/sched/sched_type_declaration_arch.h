#ifndef _ASM_X86_SCHED_TYPE_DECLARATION_H_
#define _ASM_X86_SCHED_TYPE_DECLARATION_H_

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

#endif /* _ASM_X86_SCHED_TYPE_DECLARATION_H_ */
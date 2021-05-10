#ifndef _AMD64_PROC_H_
#define _AMD64_PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "arch_proto.h"

typedef struct arch_PCB
{
	reg_t		tss_rsp0;	// point to curr-task's kernel stack bottom
							// current cpu's tss-rsp0 equal to this at switch-time
	reg_t		k_rip;		// point to curr_task's switch-time rip
	reg_t		k_rsp;		// point to curr_task's switch-time rsp

	reg_t		fs;
	reg_t		gs;

	reg_t		cr2;
	uint64_t	intr_vec;
	uint64_t	err_code;

	reg_t		cr3;
} arch_PCB_s;

typedef struct arch_PCB_stackframe
{
	uint8_t			skip[TASK_KSTACK_SIZE - sizeof(stack_frame_s)];
	stack_frame_s	pcb_sf_top;
} arch_PCB_stackframe_s;


#endif /* _AMD64_PROC_H_ */
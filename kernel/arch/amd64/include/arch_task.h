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
} arch_PCB_s;

typedef struct arch_PCB_stackframe
{
	reg_t			skip[(TASK_KSTACK_SIZE - sizeof(stack_frame_s)) / sizeof(reg_t)];
	stack_frame_s	pcb_sf_top;
} arch_PCB_stackframe_s;

typedef struct mm
{
	PML4E_T	*	pml4;
	PDPTE_T **	pdpt;
	PDE_T	***	pd;

	uint64_t	start_code,end_code;
	uint64_t	start_rodata,end_rodata;
	uint64_t	start_data,end_data;
	uint64_t	start_bss,end_bss;
	uint64_t	start_brk,end_brk;
	uint64_t	start_stack;
} mm_s;


#endif /* _AMD64_PROC_H_ */
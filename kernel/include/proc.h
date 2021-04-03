#ifndef _PROC_H_
#define _PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "../arch/amd64/include/arch_proc.h"

	typedef struct proc
	{
		List_s		PCB_list;
		arch_PCB_s	arch_struct;

		volatile long	state;
		unsigned long	flags;

		long	pid;
		long	counter;
		long	signal;
		long	prior;
	} proc_s;


	typedef union
	{
		proc_s		proc;
		uint64_t	stack[PSTACK_SIZE / sizeof(uint64_t)];
	} PCB_u __attribute__((aligned(8)));

	void __switch_to(proc_s *, proc_s *);
	void switch_to(proc_s *, proc_s *);

#endif /* _PROC_H_ */
#ifndef _PROC_H_
#define _PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "../arch/amd64/include/arch_proc.h"

	#define TASK_RUNNING			(1 << 0)
	#define TASK_INTERRUPTIBLE		(1 << 1)
	#define	TASK_UNINTERRUPTABLE	(1 << 2)
	#define	TASK_ZOMBIE				(1 << 3)	
	#define	TASK_STOPPED			(1 << 4)

	#define PF_KTHREAD			(1 << 0)

	#define CLONE_FS			(1 << 0)
	#define CLONE_FILES			(1 << 1)
	#define CLONE_SIGNAL		(1 << 2)

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
		uint64_t	stack[PROC_KSTACK_SIZE / sizeof(uint64_t)];
	} PCB_u __attribute__((aligned(8)));

	void init_proc0(void);

	void arch_init_proc0(void);
	proc_s * get_current();
	void __switch_to(proc_s *, proc_s *);
	void switch_to(proc_s *, proc_s *);

#endif /* _PROC_H_ */
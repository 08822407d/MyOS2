#ifndef _PROC_H_
#define _PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "../arch/amd64/include/arch_proc.h"

	#define MAX_PID				0x8000
	#define PS_RUNNING			(1L << 0)
	#define PS_INTERRUPTIBLE	(1L << 1)
	#define	PS_UNINTERRUPTABLE	(1L << 2)
	#define	PS_ZOMBIE			(1L << 3)	
	#define	PS_STOPPED			(1L << 4)

	#define PF_KTHREAD			(1UL << 0)
	#define PF_NEED_SCHEDULE	(1UL << 1)
	#define PF_VFORK			(1UL << 2)

	#define CLONE_FS			(1 << 0)
	#define CLONE_FILES			(1 << 1)
	#define CLONE_SIGNAL		(1 << 2)

	struct proc;
	typedef struct proc proc_s;
	typedef struct proc
	{
		proc_s *	prev;
		proc_s *	next;
		arch_PCB_s	arch_struct;

		volatile long	state;
		unsigned long	flags;

		long	pid;
		long	signal;
		long	prior;

		unsigned long	proc_jiffies;
	} proc_s;


	typedef union PCB
	{
		proc_s		proc;
		uint64_t	stack[PROC_KSTACK_SIZE / sizeof(uint64_t)];
		arch_PCB_stackframe_s	arch_sf;
	} PCB_u __attribute__((aligned(8)));

	void proc_init(void);

	void arch_init_proc(void);
	proc_s * get_current(void);
	unsigned long get_newpid(void);
	void __switch_to(proc_s * curr, proc_s * target, percpu_data_s * cpudata);
	void switch_to(proc_s * curr, proc_s * target, percpu_data_s * cpudata);

#endif /* _PROC_H_ */
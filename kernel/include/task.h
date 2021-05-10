#ifndef _PROC_H_
#define _PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "../arch/amd64/include/arch_task.h"

	#define MAX_PID				0x8000
	#define PS_RUNNING			(1L << 0)
	#define PS_INTERRUPTIBLE	(1L << 1)
	#define	PS_UNINTERRUPTABLE	(1L << 2)
	#define	PS_ZOMBIE			(1L << 3)	
	#define	PS_STOPPED			(1L << 4)
	#define PS_WAITING			(1L << 5)

	#define PF_KTHREAD			(1UL << 0)
	#define PF_NEED_SCHEDULE	(1UL << 1)
	#define PF_VFORK			(1UL << 2)

	#define CLONE_FS			(1 << 0)
	#define CLONE_FILES			(1 << 1)
	#define CLONE_SIGNAL		(1 << 2)

	// choose the task list of cpudata
	#define CPU_WATING			0UL
	#define CPU_FINISHED		~0UL

	struct task;
	typedef struct task task_s;
	typedef struct task
	{
		task_s *	prev;
		task_s *	next;
		arch_PCB_s	arch_struct;

		volatile long	state;
		unsigned long	flags;

		long	pid;
		long	signal;
		long	prior;

		unsigned long	task_jiffies;
	} task_s;

	typedef union PCB
	{
		task_s		task;
		uint64_t	stack[TASK_KSTACK_SIZE / sizeof(uint64_t)];
		arch_PCB_stackframe_s	arch_sf;
	} PCB_u __attribute__((aligned(8)));

	typedef struct idle_task_queue
	{
		task_s **	queue;
		task_s *	sched_task;
		unsigned	nr_max;
		unsigned	nr_curr;
		unsigned	head;	// point to the firt non-null unit in queue
		unsigned	tail;	// point to next unit of the last non-null
	} idle_task_queue_s;


	void init_task(void);

	void arch_init_task(void);
	task_s * get_current(void);
	unsigned long get_newpid(void);
	void __switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p);
	void switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p);
	void schedule(void);
	void idle_enqueue(task_s * idle);
	task_s * idle_dequeue(void);


	void cpu_list_push_task(percpu_data_s * cpudata_p, task_s * task, unsigned long target_list_flag);
	task_s * cpu_list_pop_task(percpu_data_s * cpudata_p, unsigned long target_list_flag);

#endif /* _PROC_H_ */
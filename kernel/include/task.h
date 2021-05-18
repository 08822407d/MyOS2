#ifndef _PROC_H_
#define _PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "../arch/amd64/include/arch_task.h"
#include "../klib/data_structure.h"

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

	m_define_list_header(task);

	typedef struct task
	{
		task_s *		prev;
		task_s *		next;

		task_list_s *	list_header;

		arch_PCB_s		arch_struct;

		volatile long	state;
		unsigned long	flags;

		unsigned long	pid;
		unsigned long	signal;
		unsigned long	time_slice;
		unsigned long	vruntime;

		unsigned long	spin_count;
		unsigned long	semaphore_count;
	} task_s;

	typedef union PCB
	{
		task_s		task;
		uint64_t	stack[TASK_KSTACK_SIZE / sizeof(uint64_t)];
		arch_PCB_stackframe_s	arch_sf;
	} PCB_u __attribute__((aligned(8)));

	typedef struct task_queue
	{
		task_s **	queue;
		task_s *	sched_task;
		unsigned	nr_max;
		unsigned	nr_curr;
		unsigned	head;	// point to the firt non-null unit in queue
		unsigned	tail;	// point to next unit of the last non-null
	} task_queue_s;

	typedef struct percpu_info
	{
		task_s *		curr_task;
		task_s *		idle_task;

		task_list_s		ready_tasks;
		task_list_s		finished_tasks;

		unsigned		is_idle_flag;
		unsigned		scheduleing_flag;

		unsigned long	last_jiffies;	// abs jiffies when curr-task loaded
		unsigned long	time_slice;	// max jiffies for running of this task

		size_t			cpu_idx;
		arch_percpu_data_s	arch_info;
		char 		(* cpu_stack_start)[CONFIG_CPUSTACK_SIZE];
	} percpu_data_s;

	void init_task(void);

	void arch_init_task(void);
	task_s * get_current_task(void);
	#define curr_tsk get_current_task()
	percpu_data_s * get_current_cpu(void);
	#define curr_cpu get_current_cpu()
	unsigned long get_newpid(void);
	void __switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p);
	void switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p);
	void wakeup_task(task_s * task);
	void load_balance(void);
	void idle_enqueue(task_s * idle);
	task_s * idle_dequeue(void);
	void task_list_push(task_list_s * list, task_s * task);
	task_s * task_list_pop(task_list_s * list);

#endif /* _PROC_H_ */
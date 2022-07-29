#ifndef _PROC_H_
#define _PROC_H_

#include <klib/utils.h>

#include <linux/kernel/sched/fs_struct.h>
#include <linux/kernel/types.h>
#include <linux/mm/mm.h>

#include "../arch/amd64/include/arch_task.h"
#include "../arch/amd64/include/arch_config.h"

	#define MAX_PID				0x8000
	#define PS_RUNNING			(1L << 0)
	#define PS_INTERRUPTIBLE	(1L << 1)
	#define	PS_UNINTERRUPTIBLE	(1L << 2)
	#define	PS_ZOMBIE			(1L << 3)	
	#define	PS_STOPPED			(1L << 4)
	#define PS_WAITING			(1L << 5)

	#define PF_KTHREAD			(1UL << 0)
	#define PF_NEED_SCHEDULE	(1UL << 1)
	#define PF_VFORK			(1UL << 2)

	#define CLONE_VM			(1 << 0)	/* shared Virtual Memory between processes */
	#define CLONE_FS			(1 << 1)	/* shared fs info between processes */
	#define CLONE_SIGNAL		(1 << 2)	/* shared signal between processes */

	// choose the task list of cpudata
	#define CPU_WATING			0UL
	#define CPU_FINISHED		~0UL

	struct task;
	typedef struct task task_s;

	#define MAX_FILE_NR 256
	typedef struct task
	{
		List_s			schedule_list;

		task_s *		parent;
		List_s			child_list;
		List_hdr_s		child_lhdr;

		arch_task_s		arch_struct;

		file_s *		fps[MAX_FILE_NR];
		mm_s *			mm_struct;

		taskfs_s *		fs;

		volatile long	state;
		unsigned long	flags;

		unsigned long	pid;
		unsigned long	signal;
		unsigned long	time_slice;
		unsigned long	vruntime;

		long			exit_code;

		unsigned long	spin_count;
		unsigned long	semaphore_count;

		wait_queue_hdr_s	wait_childexit;
	} task_s;

	typedef union PCB {
		task_s		task;
		reg_t		stack[TASK_KSTACK_SIZE / sizeof(reg_t)];
	} PCB_u __attribute__((aligned(8)));

	typedef struct task_queue {
		task_s **	queue;
		task_s *	sched_task;
		unsigned	nr_max;
		unsigned	nr_curr;
		unsigned	head;	// point to the firt non-null unit in queue
		unsigned	tail;	// point to next unit of the last non-null
	} task_queue_s;

	task_s * get_current_task(void);
	#define curr_tsk get_current_task()

	void early_init_task(size_t lcpu_nr);
	void	init_task(size_t lcpu_nr);
	void	preinit_arch_task(void);
	void init_arch_task(size_t cpu_idx);
	unsigned long gen_newpid(void);
	void	__switch_to(task_s * curr, task_s * target);
	void	switch_to(task_s * curr, task_s * target);
	void	wakeup_task(task_s * task);
	int		load_balance(void);
	void	idle_enqueue(task_s * idle);
	task_s * idle_dequeue(void);

#endif /* _PROC_H_ */
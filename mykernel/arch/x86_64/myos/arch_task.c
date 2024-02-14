#include <linux/kernel/slab.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/delay.h>
#include <linux/mm/mm.h>
#include <linux/fs/fs.h>
#include <linux/fs/mount.h>
#include <linux/fs/binfmts.h>
#include <linux/sched/mm.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>
#include <linux/lib/list.h>
#include <asm/processor.h>
#include <asm/setup.h>
#include <asm/fsgsbase.h>
#include <asm/switch_to.h>


#include <klib/utils.h>
#include <obsolete/glo.h>
#include <obsolete/printk.h>

#include <obsolete/arch_proto.h>
#include <obsolete/device.h>

#define	USER_CODE_ADDR		0x6000000
#define USER_MEM_LENGTH		0x800000
#define MAX_PID				0x8000

extern char		ist_stack0;

static DEFINE_SPINLOCK(newpid_lock);
bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
unsigned long	curr_pid;

#define LOAD_ELF

extern int myos_exit_thread(task_s * new_task);

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void myos_init_pid_allocator()
{
	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
}

unsigned long myos_pid_nr()
{
	spin_lock(&newpid_lock);
	unsigned long newpid = bm_get_freebit_idx(pid_bm, curr_pid, MAX_PID);
	if (newpid >= MAX_PID || newpid < curr_pid)
	{
		newpid = bm_get_freebit_idx(pid_bm, 0, curr_pid);
		if (newpid >= curr_pid || newpid == 0)
		{
			// pid bitmap used out
			while (1);
		}
	}
	curr_pid = newpid;
	bm_set_bit(pid_bm, newpid);
	spin_unlock_no_resched(&newpid_lock);

	return curr_pid;
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void kjmp_to_doexecve()
{
	// color_printk(BLUE, BLACK, "Hang Here\n");
	// while (1);
	
	// here if derictly use macro:current will cause unexpected rewriting memory
	task_s * curr = current;
	pt_regs_s *curr_ptregs = task_pt_regs(curr);
	curr->flags &= ~PF_KTHREAD;

#ifdef LOAD_ELF
	const char *initd_name = "/initd";
#else
	const char *initd_name = "/initd.bin";
#endif
	const char *const argv[] = { initd_name, NULL };
	const char *const envp[] = { NULL };

	kernel_execve(initd_name, argv, envp);

	asm volatile(	"movq	%0,	%%rsp		\n\t"
					"sti					\n\t"
					"jmp	sysexit_entp	\n\t"
				:
				:	"m"(curr_ptregs)
				:
				);
}

static void exit_notify(void)
{
	task_s * curr = current;
	while (curr->children.count != 0)
	{
		List_s * child_lp = list_hdr_pop(&curr->children);
		while (child_lp == 0);

		task_s *task_initd = myos_find_task_by_pid(1);
		list_hdr_append(&task_initd->children, child_lp);
	}
}

unsigned long do_exit(unsigned long exit_code)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	task_s * curr = current;
	color_printk(RED, WHITE, "Core-%d: task:%d exited.\n", cpudata_p->cpu_idx, curr->pid);

do_exit_again:
	asm volatile("cli");
	current->exit_code = exit_code;
	myos_exit_thread(current);
	exit_files(current);
	exit_notify();
	asm volatile("sti");

	current->__state = EXIT_ZOMBIE;
	schedule();

	goto do_exit_again;
	return 0;
}

/*==============================================================================================*
 *									schedule functions											*
 *==============================================================================================*/
inline __always_inline task_s * myos_get_current()
{
	task_s * curr_task = NULL;
	asm volatile(	"andq 	%%rsp,	%1		\n\t"
				:	"=r"(curr_task)
				:	"r"(~(THREAD_SIZE - 1))
				:
				);
	return curr_task;
}

static __always_inline void myos_switch_mm(task_s * curr, task_s * target)
{
	load_cr3(target->mm->pgd);
	wrmsrl(MSR_IA32_SYSENTER_ESP, (unsigned long)target->stack);
}

void myos_schedule(void)
{
	per_cpudata_s *	cpudata_p = curr_cpu;
	task_s *		curr_task = cpudata_p->curr_task;
	task_s *		next_task = NULL;
	// curr_task must exists
	while (curr_task == NULL);

	if (cpudata_p->running_lhdr.count > 0)
	{
		cpudata_p->scheduleing_flag = 1;

		// fetch a task from running_list
		List_s * next_lp = list_hdr_pop(&cpudata_p->running_lhdr);
		while (next_lp == 0);

		// and insert curr_task back to running_list
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == cpudata_p->idle_task)
			{
				// insert idle task to cpu's running-list tail
				list_hdr_enqueue(&cpudata_p->running_lhdr, &curr_task->tasks);
			}
			else
			{
				List_s * tmp_list = cpudata_p->running_lhdr.header.next;
				while ((curr_task->se.vruntime > container_of(tmp_list, task_s, tasks)->se.vruntime) &&
						tmp_list != &cpudata_p->running_lhdr.header)
				{
					tmp_list = tmp_list->next;
				}
				list_insert_prev(tmp_list, &curr_task->tasks);
				cpudata_p->running_lhdr.count++;
			}
		}

		unsigned long used_jiffies = jiffies - cpudata_p->last_jiffies;
		curr_task->total_jiffies += used_jiffies;
		if (curr_task != cpudata_p->idle_task)
			curr_task->se.vruntime += used_jiffies;

		next_task = container_of(next_lp, task_s, tasks);
		cpudata_p->curr_task = next_task;
		cpudata_p->time_slice = next_task->rt.time_slice;

		cpudata_p->last_jiffies = jiffies;
		cpudata_p->scheduleing_flag = 0;

		curr_task->flags &= ~PF_NEED_SCHEDULE;

		while (curr_task == next_task);
		
		myos_switch_mm(curr_task, next_task);
		switch_to(curr_task, next_task);
	}
}

void try_sched()
{
	per_cpudata_s *	cpudata_p = curr_cpu;
	task_s *		curr_task = cpudata_p->curr_task;

	unsigned long used_jiffies = jiffies - cpudata_p->last_jiffies;
	// if running time out, make the need_schedule flag of current task
	if (used_jiffies >= cpudata_p->time_slice)
		cpudata_p->curr_task->flags |= PF_NEED_SCHEDULE;

	if ((curr_task == cpudata_p->idle_task) && (cpudata_p->running_lhdr.count == 0))
		return;

	if (((curr_task->__state == TASK_RUNNING) && !(curr_task->flags & PF_NEED_SCHEDULE)))
		return;

	if (cpudata_p->preempt_count != 0)
		return;

	// normal sched
	if (curr_task->flags & PF_NEED_SCHEDULE)
		schedule();
}
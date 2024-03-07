// #include <linux/kernel/slab.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/mm.h>
#include <linux/fs/fs.h>
#include <linux/fs/mount.h>
#include <linux/fs/binfmts.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>
#include <linux/lib/list.h>
#include <asm/setup.h>
#include <asm/insns.h>


#include <klib/utils.h>
#include <obsolete/glo.h>
#include <obsolete/printk.h>

#include <obsolete/arch_proto.h>
#include <obsolete/device.h>

#define LOAD_ELF


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

extern int myos_exit_thread(task_s * new_task);
void __noreturn do_exit(long exit_code)
{
	pcpu_hot_s *pcpu = this_cpu_ptr(&pcpu_hot);
	task_s * curr = current;
	color_printk(RED, WHITE, "Core-%d: task:%d exited.\n", pcpu->cpu_number, curr->pid);

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
}


// void myos_schedule(void)
// {
// 	rq_s			*this_rq = this_cpu_ptr(&runqueues);
// 	myos_rq_s		*myos_rq = &(this_rq->myos);
// 	pcpu_hot_s		*pcpu = this_cpu_ptr(&pcpu_hot);
// 	task_s *		curr_task = pcpu->current_task;
// 	task_s *		next_task = NULL;
// 	// curr_task must exists
// 	while (curr_task == NULL);

// 	if (myos_rq->running_lhdr.count > 0)
// 	{
// 		// fetch a task from running_list
// 		List_s * next_lp = list_hdr_pop(&myos_rq->running_lhdr);
// 		while (next_lp == 0);

// 		// and insert curr_task back to running_list
// 		if (curr_task->__state == TASK_RUNNING)
// 		{
// 			if (curr_task == this_rq->idle)
// 			{
// 				// insert idle task to cpu's running-list tail
// 				list_hdr_enqueue(&myos_rq->running_lhdr, &curr_task->tasks);
// 			}
// 			else
// 			{
// 				List_s * tmp_list = myos_rq->running_lhdr.header.next;
// 				while ((curr_task->se.vruntime > container_of(tmp_list, task_s, tasks)->se.vruntime) &&
// 						tmp_list != &myos_rq->running_lhdr.header)
// 				{
// 					tmp_list = tmp_list->next;
// 				}
// 				list_insert_prev(tmp_list, &curr_task->tasks);
// 				myos_rq->running_lhdr.count++;
// 			}
// 		}

// 		unsigned long used_jiffies = jiffies - myos_rq->last_jiffies;
// 		if (curr_task != this_rq->idle)
// 			curr_task->se.vruntime += used_jiffies;

// 		next_task = container_of(next_lp, task_s, tasks);
// 		myos_rq->time_slice = next_task->rt.time_slice;

// 		myos_rq->last_jiffies = jiffies;

// 		clear_tsk_need_resched(curr_task);

// 		while (curr_task == next_task);
		
// 		switch_mm_irqs_off(curr_task->mm, next_task->mm, next_task);
// #if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
// 		wrmsrl(MSR_IA32_SYSENTER_ESP, task_top_of_stack(next_task));
// #endif
// 		switch_to(curr_task, next_task, curr_task);
// 	}
// }

// void try_sched()
// {
// 	rq_s			*this_rq = this_cpu_ptr(&runqueues);
// 	pcpu_hot_s		*pcpu = this_cpu_ptr(&pcpu_hot);
// 	task_s			*curr_task = current;

// 	unsigned long used_jiffies = jiffies - this_rq->last_jiffies;
// 	// if running time out, make the need_schedule flag of current task
// 	if (used_jiffies >= this_rq->time_slice)
// 		pcpu->current_task->flags |= PF_NEED_SCHEDULE;

// 	if ((curr_task == this_rq->idle) && (this_rq->running_lhdr.count == 0))
// 		return;

// 	if (((curr_task->__state == TASK_RUNNING) && !(curr_task->flags & PF_NEED_SCHEDULE)))
// 		return;

// 	if (preempt_count() != 0)
// 		return;

// 	// normal sched
// 	if (curr_task->flags & PF_NEED_SCHEDULE)
// 		schedule();
// }
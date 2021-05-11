#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"
#include "include/arch_proto.h"

#include "../../include/glo.h"
#include "../../include/proto.h"
#include "../../include/task.h"
#include "../../include/printk.h"
#include "../../include/const.h"
#include "../../klib/data_structure.h"

extern tss64_s	bsp_tmp_tss;
extern char		ist_stack0;

extern PCB_u	task0_PCB;

bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
spinlock_T		newpid_lock;
unsigned long	curr_pid;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
inline __always_inline task_s * get_current()
{
#ifdef current
#undef current
#endif
	task_s * current = NULL;
	__asm__ __volatile__("andq %%rsp,%0	\n\t"
						 : "=r"(current)
						 : "0"(~(TASK_KSTACK_SIZE - 1)));
	return current;
}

unsigned long get_newpid()
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
	spin_unlock(&newpid_lock);

	return curr_pid;
}

stack_frame_s * get_stackframe(task_s * task_p)
{
	PCB_u * pcb_p = container_of(task_p, PCB_u, task);
	return &(pcb_p->arch_sf.pcb_sf_top);
}

inline __always_inline void __switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p)
{
	tss64_s * curr_tss = cpudata_p->arch_info->tss;
	curr_tss->rsp0 = target->arch_struct.tss_rsp0;

	// since when intel cpu reload gs and fs, gs_base and fs_base will be reset
	// here need to save and restore them

	// __asm__ __volatile__("movq	%%fs,	%0 \n\t"
	// 					 : "=a"(curr->arch_struct.fs));
	// __asm__ __volatile__("movq	%%gs,	%0 \n\t"
	// 					 : "=a"(curr->arch_struct.gs));

	// __asm__ __volatile__("movq	%0,	%%fs \n\t"
	// 					 :
	// 					 :"a"(target->arch_struct.fs));
	// __asm__ __volatile__("movq	%0,	%%gs \n\t"
	// 					 :
	// 					 :"a"(target->arch_struct.gs));

}

void inline __always_inline switch_to(task_s * curr, task_s * target, percpu_data_s * cpudata_p)
{
	__asm__ __volatile__("pushq	%%rbp				\n\
						  pushq %%rax				\n\
						  movq	%%rsp, %0			\n\
						  movq	%2, %%rsp			\n\
						  leaq	1f(%%rip), %%rax	\n\
						  movq	%%rax, %1			\n\
						  pushq	%3					\n\
						  jmp	__switch_to			\n\
						  1:						\n\
						  popq	%%rax				\n\
						  popq	%%rbp				\n	"
						 :"=m"(curr->arch_struct.k_rsp),
						  "=m"(curr->arch_struct.k_rip)
						 :"m"(target->arch_struct.k_rsp),
						  "m"(target->arch_struct.k_rip),
						  "D"(curr), "S"(target), "d"(cpudata_p)
						 :"memory");
}

unsigned long init(unsigned long arg)
{
	color_printk(RED,BLACK,"init task is running, arg:%#018lx\n",arg);
	return 1;
}

int sys_call(int syscall_nr)
{
	int ret_val = 0;
	__asm__ __volatile__("pushq	%%rcx		\n\
						  pushq	%%r10		\n\
						  pushq	%%r11		\n\
						  syscall			\n\
						  popq	%%r11		\n\
						  popq	%%r10		\n\
						  popq	%%rcx		\n	"
						  :"=a"(ret_val)
						  :
						  :);
	return ret_val;
}

int do_syscall(int syscall_nr)
{
	return 0x12345678;
}

unsigned long do_execve(stack_frame_s * sf_regs)
{
	return 1;
}

void wakeup_task(task_s * task)
{
	task->state |= PS_WAITING;
	task_list_push(&global_waiting_task, task);
}

unsigned long do_fork(stack_frame_s * sf_regs,
						unsigned long clone_flags,
						unsigned long tmp_kstack_start,
						unsigned long stack_size)
{
	PCB_u * curr_pcb	= container_of(get_current(), PCB_u, task);
	PCB_u * new_pcb		= (PCB_u *)kmalloc(sizeof(PCB_u));
	task_s * curr_task	= &curr_pcb->task;
	task_s * new_task	= &new_pcb->task;
	if (new_pcb == NULL)
	{
		goto alloc_newtask_fail;
	}

	memset(new_pcb, 0, sizeof(PCB_u));
	memcpy(new_task, curr_pcb, sizeof(task_s));

	m_list_init(new_task);
	new_pcb->task.pid = get_newpid();
	new_pcb->task.state = PS_UNINTERRUPTABLE;

	stack_frame_s * new_sf_regs = get_stackframe(new_task);
	memcpy(new_sf_regs, sf_regs, sizeof(stack_frame_s));

	new_task->arch_struct.tss_rsp0 = (reg_t)new_pcb + TASK_KSTACK_SIZE;
	new_task->arch_struct.k_rip = sf_regs->rip;
	new_task->arch_struct.k_rsp = (reg_t)new_sf_regs;

	wakeup_task(new_task);

	goto do_fork_success;

	alloc_newtask_fail:
		kfree(new_task);

	do_fork_success:
		return new_task->pid;
}

unsigned long do_exit(unsigned long code)
{
	color_printk(RED,BLACK,"exit task is running,arg:%#018lx\n",code);
	while(1);
}

int kernel_thread(unsigned long (* fn)(unsigned long), unsigned long arg, unsigned long flags)
{
	stack_frame_s sf_regs;
	memset(&sf_regs,0,sizeof(sf_regs));

	sf_regs.rbx = (reg_t)fn;
	sf_regs.rdx = (reg_t)arg;

	sf_regs.cs = KERN_CS_SELECTOR;
	sf_regs.ds = KERN_SS_SELECTOR;
	sf_regs.es = KERN_SS_SELECTOR;
	sf_regs.ss = KERN_SS_SELECTOR;
	sf_regs.rflags = (1 << 9);
	sf_regs.rip = (reg_t)kernel_thread_func;

	return do_fork(&sf_regs, flags, 0, 0);
}

void arch_init_task()
{
	// init MSR sf_regs related to syscall/sysret
	wrmsr(MSR_IA32_LSTAR, (uint64_t)enter_syscall);
	wrmsr(MSR_IA32_STAR, ((uint64_t)KERN_SS_SELECTOR << 48) | ((uint64_t)KERN_CS_SELECTOR << 32));
	wrmsr(MSR_IA32_FMASK, EFL_DF | EFL_IF | EFL_TF | EFL_NT);

	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
	spin_init(&newpid_lock);
}

/*==============================================================================================*
 *									schedule related functions									*
 *==============================================================================================*/

void reschedule(percpu_data_s * cpudata_p)
{
	// if running time out, make the need_schedule flag of current task
	unsigned long used_jiffies = jiffies - cpudata_p->last_jiffies;
	if (used_jiffies >= cpudata_p->task_jiffies)
		cpudata_p->curr_task->flags |= PF_NEED_SCHEDULE;

	// there are 4 case
	// 1: current not idle - waiting not null
	// 2: current not idle - waiting is null
	// 3: current is idle - waiting not null
	// 4: current id idle - waiting is null

	// if case 4, just do nothing
	if (((cpudata_p->waiting_tasks.count < 1) && (cpudata_p->is_idle_flag)) ||
			!(cpudata_p->curr_task->flags & PF_NEED_SCHEDULE) ||
			cpudata_p->scheduleing_flag)
		return;

	cpudata_p->scheduleing_flag = 1;
	// now it must be one of case 1/2/3
		task_s * curr_task = NULL;
		task_s * next_task = NULL;
		curr_task = cpudata_p->curr_task;
	// case 1: curr->finished; waiting->curr
	if ((!cpudata_p->is_idle_flag) &&
		(cpudata_p->waiting_tasks.count > 0))
	{
		task_list_push(&cpudata_p->finished_tasks, curr_task);
		next_task =
		cpudata_p->curr_task = task_list_pop(&cpudata_p->waiting_tasks);
		cpudata_p->is_idle_flag = 0;
	}
	// case 2: curr->finished; idle_queue->curr
	if ((!cpudata_p->is_idle_flag) &&
		(cpudata_p->waiting_tasks.count < 1))
	{
		task_list_push(&cpudata_p->finished_tasks, curr_task);
		next_task =
		cpudata_p->curr_task = idle_dequeue();
		cpudata_p->is_idle_flag = 1;
	}
	//case 3: curr->idle_queue; waiting->curr
	if ((cpudata_p->is_idle_flag) &&
		(cpudata_p->waiting_tasks.count > 0))
	{
		idle_enqueue(curr_task);
		next_task =
		cpudata_p->curr_task = task_list_pop(&cpudata_p->waiting_tasks);
		cpudata_p->is_idle_flag = 0;
	}

	curr_task->state &= ~PS_RUNNING;
	next_task->state &= PS_WAITING;
	next_task->state |= PS_RUNNING;

	cpudata_p->last_jiffies = jiffies;
	cpudata_p->task_jiffies = cpudata_p->curr_task->task_jiffies;

	cpudata_p->scheduleing_flag = 0;

	switch_to(curr_task, next_task, cpudata_p);
}

void schedule()
{
	percpu_data_s * cpudata_p = smp_info[0];
	if (cpudata_p->scheduleing_flag)
		return;

	cpudata_p->scheduleing_flag = 1;
	// retrieve finished tasks, then assign them back to cpu
	while (cpudata_p->finished_tasks.count)
	{
		task_s * tmp = task_list_pop(&cpudata_p->finished_tasks);
		task_list_push(&global_waiting_task, tmp);
	}
	while (global_waiting_task.count)
	{
		task_s * tmp = task_list_pop(&global_waiting_task);
		task_list_push(&cpudata_p->waiting_tasks, tmp);
	}
	// insert schedule self to cpu's waiting list end
	task_s * current = get_current();
	current->flags |= PF_NEED_SCHEDULE;
	cpudata_p->scheduleing_flag = 0;

	// reschedule(cpudata_p);
}
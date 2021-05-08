#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proc.h"
#include "include/arch_proto.h"

#include "../../include/glo.h"
#include "../../include/proto.h"
#include "../../include/proc.h"
#include "../../include/printk.h"
#include "../../include/const.h"
#include "../../klib/data_structure.h"

extern tss64_s	tss_bsp;
extern char		ist_stack0;

extern PCB_u	proc0_PCB;

bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
spinlock_T		newpid_lock;
unsigned long	curr_pid;

inline __always_inline proc_s * get_current()
{
#ifdef current
#undef current
#endif
	proc_s * current = NULL;
	__asm__ __volatile__("andq %%rsp,%0	\n\t"
						 : "=r"(current)
						 : "0"(~(PROC_KSTACK_SIZE - 1)));
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

stack_frame_s * get_stackframe(proc_s * proc_p)
{
	PCB_u * pcb_p = container_of(proc_p, PCB_u, proc);
	return &(pcb_p->arch_sf.pcb_sf_top);
}

inline __always_inline void __switch_to(proc_s * curr, proc_s * target, percpu_data_s * cpudata)
{
	tss64_s * curr_tss = cpudata->arch_info->tss;
	curr_tss->rsp0 = target->arch_struct.tss_rsp0;

	__asm__ __volatile__("movq	%%fs,	%0 \n\t"
						 : "=a"(curr->arch_struct.fs));
	__asm__ __volatile__("movq	%%gs,	%0 \n\t"
						 : "=a"(curr->arch_struct.gs));

	__asm__ __volatile__("movq	%0,	%%fs \n\t"
						 :
						 :"a"(target->arch_struct.fs));
	__asm__ __volatile__("movq	%0,	%%gs \n\t"
						 :
						 :"a"(target->arch_struct.gs));

}

void inline __always_inline switch_to(proc_s * curr, proc_s * target, percpu_data_s * cpudata)
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
						  "D"(curr), "S"(target), "d"(cpudata)
						 :"memory");
}

unsigned long test_proc_a(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;

		color_printk(WHITE, BLACK, "-A- ");
	}
}

unsigned long test_proc_b(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-B- ");
	}
}

unsigned long test_proc_c(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-C- ");
	}
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

void wakeup_proc(proc_s * proc)
{
	if (proc_waiting_list == NULL)
	{
		proc_waiting_list = proc;
	}
	else
	{
		m_list_insert_back(proc, proc_waiting_list);
	}
	waiting_proc_count++;
}

unsigned long do_fork(stack_frame_s * sf_regs,
						unsigned long clone_flags,
						unsigned long stack_start,
						unsigned long stack_size)
{
	PCB_u * curr_pcb	= container_of(get_current(), PCB_u, proc);
	PCB_u * new_pcb		= (PCB_u *)kmalloc(sizeof(PCB_u));
	proc_s * curr_proc	= &curr_pcb->proc;
	proc_s * new_proc	= &new_pcb->proc;
	if (new_pcb == NULL)
	{
		goto alloc_newproc_fail;
	}

	memset(new_pcb, 0, sizeof(PCB_u));
	memcpy(new_proc, curr_pcb, sizeof(proc_s));

	m_list_init(new_proc);
	new_pcb->proc.pid = get_newpid();
	new_pcb->proc.state = PS_UNINTERRUPTABLE;

	stack_frame_s * new_sf_regs = get_stackframe(new_proc);
	memcpy(new_sf_regs, sf_regs, sizeof(stack_frame_s));

	new_proc->arch_struct.tss_rsp0 = (reg_t)new_proc + PROC_KSTACK_SIZE;
	new_proc->arch_struct.k_rip = sf_regs->rip;
	new_proc->arch_struct.k_rsp = (reg_t)new_sf_regs;

	wakeup_proc(new_proc);
	new_proc->state = PS_RUNNING;
	goto do_fork_success;

	alloc_newproc_fail:
		kfree(new_proc);

	do_fork_success:
		return new_proc->pid;
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

void arch_init_proc()
{
	// init MSR sf_regs related to syscall/sysret
	wrmsr(MSR_IA32_LSTAR, (uint64_t)enter_syscall);
	wrmsr(MSR_IA32_STAR, ((uint64_t)KERN_SS_SELECTOR << 48) | ((uint64_t)KERN_CS_SELECTOR << 32));
	wrmsr(MSR_IA32_FMASK, EFL_DF | EFL_IF | EFL_TF | EFL_NT);

	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
	spin_init(&newpid_lock);

	// kernel_thread(init, 20, 0);
	// kernel_thread(test_proc_a, 0, 0);
	// kernel_thread(test_proc_b, 0, 0);
	// kernel_thread(test_proc_c, 0, 0);
}

void reschedule(percpu_data_s * cpudata)
{
	unsigned long used_jiffies = jiffies - cpudata->last_jiffies;
	if (cpudata->waiting_count < 1)
		return;

	if (used_jiffies >= cpudata->proc_jiffies)
		cpudata->curr_proc->flags |= PF_NEED_SCHEDULE;
	// if ((cpudata->curr_proc == cpudata->idle_proc) && (cpudata->waiting_count == 0))
	// 	cpudata->curr_proc->flags &= ~PF_NEED_SCHEDULE;
		
	if (cpudata->curr_proc->flags & PF_NEED_SCHEDULE)
	{
		proc_s * curr_proc = cpudata->curr_proc;
		proc_s * next_proc = NULL;
		// move current to finished list
		// if current is idle. just let current = NULL
		cpudata->curr_proc = NULL;
		// if (cpudata->curr_proc == cpudata->idle_proc)
		// {
			// if (cpudata->finished_proc == NULL)
			// 	cpudata->finished_proc = curr_proc;
			// else
			// 	m_list_insert_back(curr_proc, cpudata->finished_proc);
			// cpudata->finished_count++;
		// }
		// get waiting proc
		// if no waiting proc, let idle run
		// if (cpudata->waiting_count > 0)
		// {
			next_proc = cpudata->waiting_proc;
			// move the first in waiting list to current
			cpudata->curr_proc = next_proc;
			if (cpudata->waiting_count < 2)
				cpudata->waiting_proc = NULL;
			else
			{
				cpudata->waiting_proc = next_proc->next;
				m_list_delete(next_proc);
			}
			cpudata->waiting_count--;
		// }
		// else
		// {
		// 	next_proc = cpudata->idle_proc;
		// }
		switch_to(curr_proc, next_proc, cpudata);
		cpudata->last_jiffies = jiffies;
		cpudata->proc_jiffies = curr_proc->proc_jiffies;
	}
}

void schedule()
{
	percpu_data_s * cpudata = smp_info[0];
	for ( ; cpudata->finished_count > 0; cpudata->finished_count--)
	{
		// pop out cpu's finished proc
		proc_s * finished = cpudata->finished_proc->prev;
		if (cpudata->finished_count == 1)
			cpudata->finished_proc = NULL;
		else
			m_list_delete(finished);
		// push finished proc to waiting list
		if (waiting_proc_count < 1)
			proc_waiting_list = finished;
		else
			m_list_insert_front(finished, proc_waiting_list);
		
		waiting_proc_count++;
	}
	for ( ; waiting_proc_count > 0; waiting_proc_count--)
	{
		// pop out waiting proc
		proc_s * waiting = proc_waiting_list->next;
		if (waiting_proc_count == 1)
			proc_waiting_list = NULL;
		else
			m_list_delete(waiting);
		//push waiting proc to cpu's waiting list
		if (cpudata->waiting_count < 1)
			cpudata->waiting_proc = waiting;
		else
			m_list_insert_back(waiting, cpudata->waiting_proc);

		cpudata->waiting_count++;
	}
	// insert schedule self to cpu's waiting list end
	proc_s * current = get_current();
	current->flags |= PF_NEED_SCHEDULE;
	reschedule(cpudata);
}
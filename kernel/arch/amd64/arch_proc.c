#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proc.h"
#include "include/arch_proto.h"

#include "../../include/proto.h"
#include "../../include/proc.h"
#include "../../include/printk.h"
#include "../../include/const.h"
#include "../../klib/data_structure.h"

extern tss64_s	tss_bsp;
extern char		ist_stack0;

extern PCB_u	proc0_PCB;
extern PCB_u	proc1_PCB;

void test_proc1(void);
char user_stack[PROC_KSTACK_SIZE];

int user_func()
{
	color_printk(GREEN, BLACK, "user function is running... \n");
	
	int syscall_retval = sys_call(0x87654321);
	color_printk(GREEN, BLACK, "syscall retturn value : %#08lx \n", syscall_retval);

	while (1);
	
	return 0;
}

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

stack_frame_s * get_current_stackframe(proc_s *curr_proc)
{
	return &((PCB_u *)curr_proc)->arch_sf.pcb_sf_top;
}

inline __always_inline void __switch_to(proc_s * curr, proc_s * target)
{
	tss_bsp.rsp0 = target->arch_struct.tss_rsp0;
	tss_bsp.rsp1 =
	tss_bsp.rsp2 =
	tss_bsp.ist1 =
	tss_bsp.ist2 =
	tss_bsp.ist3 =
	tss_bsp.ist4 =
	tss_bsp.ist5 =
	tss_bsp.ist6 =
	tss_bsp.ist7 = (reg_t)&ist_stack0 + CONFIG_CPUSTACK_SIZE;

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

void inline __always_inline switch_to(proc_s * curr, proc_s * target)
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
						  "D"(curr), "S"(target)
						 :"memory");
}



unsigned long init(unsigned long arg)
{
	color_printk(RED,BLACK,"init task is running, arg:%#018lx\n",arg);
	proc_s * curr = get_current();
	stack_frame_s * sf_regs = get_current_stackframe(curr);
	curr->arch_struct.k_rip = (reg_t)ret_from_syscall;
	curr->arch_struct.k_rsp = (reg_t)sf_regs;

	__asm__ __volatile__("movq	%1, %%rsp		\n\
						  pushq	%2				\n\
						  jmp	do_execve		\n	"
						 :
						 :"D"(sf_regs), "m"(curr->arch_struct.k_rsp), "m"(curr->arch_struct.k_rip)
						 :"memory");

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
	sf_regs->rcx =
	sf_regs->rip = (reg_t)user_func;
	sf_regs->rsp = (reg_t)user_stack + PROC_KSTACK_SIZE;
	return 1;
}

unsigned long do_fork(stack_frame_s * sf_regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size)
{
	proc_s *tsk_curr = get_current();
	proc_s *tsk_new = NULL;
	stack_frame_s *tsk_new_stackfram = NULL;
	
	tsk_new_stackfram = get_current_stackframe(tsk_new);
	tsk_new = (proc_s *)kmalloc(sizeof(proc_s));
	arch_PCB_s *arch_tsk = &tsk_new->arch_struct;

	memset(tsk_new, 0, sizeof(proc_s));
	*tsk_new = *tsk_curr;

	// list_init(&tsk_new->PCB_list);
	m_list_init(tsk_new);
	// list_insert_front(&tsk_new->PCB_list, &tsk_curr->PCB_list);
	m_list_insert_front(tsk_new, tsk_curr);
	tsk_new->pid++;	
	tsk_new->state = TASK_UNINTERRUPTABLE;

	memcpy((void *)tsk_new_stackfram, sf_regs, sizeof(stack_frame_s));

	arch_tsk->tss_rsp0 = (reg_t)tsk_new + PROC_KSTACK_SIZE;
	arch_tsk->k_rip = sf_regs->rip;
	arch_tsk->k_rsp = (reg_t)tsk_new_stackfram;

	if(!(tsk_new->flags & PF_KTHREAD))
		arch_tsk->k_rip = sf_regs->rip = (reg_t)ret_from_syscall;

	tsk_new->state = TASK_RUNNING;

	return 0;
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

	return do_fork(&sf_regs,flags,0,0);
}

void arch_init_proc()
{
	// init MSR sf_regs related to syscall/sysret
	wrmsr(MSR_IA32_LSTAR, (uint64_t)enter_syscall);
	wrmsr(MSR_IA32_STAR, ((uint64_t)KERN_SS_SELECTOR << 48) | ((uint64_t)KERN_CS_SELECTOR << 32));
	wrmsr(MSR_IA32_FMASK, EFL_DF | EFL_IF | EFL_TF | EFL_NT);

	proc_s * curr_proc = get_current();
	curr_proc->flags = PF_KTHREAD;

	// kernel_thread(init, 20, CLONE_FS | CLONE_FILES | CLONE_SIGNAL);
}

void schedule(percpu_data_s * curr_cpu_data)
{
	if (curr_cpu_data->waiting_count == 0)
		return;
	else
	{
		proc_s * curr_proc = curr_cpu_data->curr_proc;
		proc_s * next_proc = curr_cpu_data->waiting_proc;
		// move current to finished list
		curr_cpu_data->curr_proc = NULL;
		if (curr_cpu_data->finished_proc == NULL)
			curr_cpu_data->finished_proc = curr_proc;
		else
			m_list_insert_back(curr_proc, curr_cpu_data->finished_proc);
		curr_cpu_data->finished_count++;
		// move the first in waiting list to current
		curr_cpu_data->curr_proc = next_proc;
		if (curr_cpu_data->waiting_count == 1)
			curr_cpu_data->waiting_proc = NULL;
		else
		{
			curr_cpu_data->waiting_proc = next_proc->next;
			m_list_delete(next_proc);
		}
		curr_cpu_data->waiting_count--;
		
		switch_to(curr_proc, next_proc);
	}
}
#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proc.h"
#include "include/arch_proto.h"

#include "../../include/proc.h"
#include "../../include/printk.h"
#include "../../include/const.h"

extern tss64_s	tss[CONFIG_MAX_CPUS];
extern char		ist_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE];

extern PCB_u	proc0_PCB;
extern PCB_u	proc1_PCB;

void test_proc1(void);
char user_stack[PROC_KSTACK_SIZE];

int user_func()
{
	color_printk(GREEN, BLACK, "user function is running... \n");
	
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
	tss[0].rsp0 = target->arch_struct.tss_rsp0;
	tss[0].rsp1 =
	tss[0].rsp2 =
	tss[0].ist1 =
	tss[0].ist2 =
	tss[0].ist3 =
	tss[0].ist4 =
	tss[0].ist5 =
	tss[0].ist6 =
	tss[0].ist7 = (reg_t)&ist_stacks[0] + CONFIG_KSTACK_SIZE;

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
	stack_frame_s * sf = get_current_stackframe(curr);
	curr->arch_struct.k_rip = (reg_t)ret_from_syscall;
	curr->arch_struct.k_rsp = (reg_t)sf;

	__asm__ __volatile__("movq	%1, %%rsp		\n\
						  pushq	%2				\n\
						  jmp	do_execve		\n	"
						 :
						 :"D"(sf), "m"(curr->arch_struct.k_rsp), "m"(curr->arch_struct.k_rip)
						 :"memory");

	return 1;
}

void syscall(int syscall_nr)
{

}

int do_syscall(int syscall_nr)
{
	return 0;
}

unsigned long do_execve(stack_frame_s * sf)
{
	sf->rcx =
	sf->rip = (reg_t)user_func;
	sf->rsp = (reg_t)user_stack + PROC_KSTACK_SIZE;
	return 1;
}

unsigned long do_fork(stack_frame_s * regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size)
{
	proc_s *tsk_curr = get_current();
	proc_s *tsk_new = NULL;
	stack_frame_s *tsk_new_stackfram;
	
	tsk_new = &proc1_PCB.proc;
	tsk_new_stackfram = get_current_stackframe(tsk_new);
	// tsk_new = (proc_s *)malloc(sizeof(proc_s));
	arch_PCB_s *arch_tsk = &tsk_new->arch_struct;

	memset(tsk_new, 0, sizeof(proc_s));
	*tsk_new = *tsk_curr;

	list_init(&tsk_new->PCB_list);
	list_insert_front(&tsk_curr->PCB_list, &tsk_new->PCB_list);
	tsk_new->pid++;	
	tsk_new->state = TASK_UNINTERRUPTABLE;

	memcpy((void *)tsk_new_stackfram, regs, sizeof(stack_frame_s));

	arch_tsk->tss_rsp0 = (reg_t)tsk_new + PROC_KSTACK_SIZE;
	arch_tsk->k_rip = regs->rip;
	arch_tsk->k_rsp = (reg_t)tsk_new_stackfram;

	if(!(tsk_new->flags & PF_KTHREAD))
		arch_tsk->k_rip = regs->rip = (reg_t)ret_from_syscall;

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
	stack_frame_s regs;
	memset(&regs,0,sizeof(regs));

	regs.rbx = (reg_t)fn;
	regs.rdx = (reg_t)arg;

	regs.cs = KERN_CS_SELECTOR;
	regs.ds = KERN_SS_SELECTOR;
	regs.es = KERN_SS_SELECTOR;
	regs.ss = KERN_SS_SELECTOR;
	regs.rflags = (1 << 9);
	regs.rip = (reg_t)kernel_thread_func;

	return do_fork(&regs,flags,0,0);
}

void arch_init_proc()
{
	// init MSR regs related to syscall/sysret
	wrmsr(MSR_IA32_LSTAR, (uint64_t)enter_syscall);
	wrmsr(MSR_IA32_STAR, ((uint64_t)KERN_SS_SELECTOR << 48) | ((uint64_t)KERN_CS_SELECTOR << 32));
	wrmsr(MSR_IA32_FMASK, EFL_DF | EFL_IF | EFL_TF | EFL_NT);

	proc_s * curr_proc = get_current();
	curr_proc->flags = PF_KTHREAD;
	proc_s *proc_1 = &proc1_PCB.proc;

	kernel_thread(init, 20, CLONE_FS | CLONE_FILES | CLONE_SIGNAL);

	switch_to(curr_proc, proc_1);
}
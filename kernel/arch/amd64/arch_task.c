#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/errno.h>
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

#define	USER_CODE_ADDR		0x6000000
#define USER_MEM_LENGTH		0x800000

extern tss64_T	bsp_tmp_tss;
extern char		ist_stack0;

extern PCB_u	task0_PCB;

bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
spinlock_T		newpid_lock;
unsigned long	curr_pid;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
inline __always_inline task_s * get_current_task()
{
#ifdef current
#undef current
#endif
	task_s * current = NULL;
	__asm__ __volatile__(	"andq 	%%rsp,	%0		\n\t"
						:	"=r"(current)
						:	"0"(~(TASK_KSTACK_SIZE - 1))
						:
						);
	return current;
}

unsigned long get_newpid()
{
	lock_spin_lock(&newpid_lock);
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
	unlock_spin_lock(&newpid_lock);

	return curr_pid;
}

stack_frame_s * get_stackframe(task_s * task_p)
{
	PCB_u * pcb_p = container_of(task_p, PCB_u, task);
	return &(pcb_p->arch_sf.pcb_sf_top);
}

inline __always_inline void switch_mm(task_s * curr, task_s * target)
{
	__asm__ __volatile__(	"movq	%0,	%%cr3		\n\t"
						:
						:	"r"(target->mm_struct->cr3)
						:	"memory"
						);
}

inline __always_inline void __switch_to(task_s * curr, task_s * target)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	tss64_T * curr_tss = cpudata_p->arch_info.tss;
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

void inline __always_inline switch_to(task_s * curr, task_s * target)
{
	__asm__ __volatile__(	"pushq	%%rbp				\n\t"
							"pushq	%%rax				\n\t"
							"movq	%%rsp,	%0			\n\t"
							"movq	%2,		%%rsp		\n\t"
							"leaq	1f(%%rip),	%%rax	\n\t"
							"movq	%%rax,	%1			\n\t"
							"pushq	%3					\n\t"
							"jmp	__switch_to			\n\t"
							"1:							\n\t"
							"popq	%%rax				\n\t"
							"popq	%%rbp				\n\t"
						:	"=m"(curr->arch_struct.k_rsp),
							"=m"(curr->arch_struct.k_rip)
						:	"m"(target->arch_struct.k_rsp),
							"m"(target->arch_struct.k_rip),
							"D"(curr), "S"(target)
						:	"memory"
						);
}

unsigned long init(unsigned long arg)
{
	color_printk(RED,BLACK,"init task is running, arg:%#018lx\n",arg);
	return 1;
}

inline __always_inline int sys_call(int syscall_nr)
{
	int ret_val = 0;
	__asm__ __volatile__(	"pushq	%%rcx				\n\t"
							"pushq	%%rdx				\n\t"
							"pushq	%%r10				\n\t"
							"pushq	%%r11				\n\t"
							"movq	serp(%%rip), %%rdx	\n\t"
							"movq	%%rsp,	%%rcx		\n\t"
							"sysenter					\n\t"
							"serp:						\n\t"
							"popq	%%r11				\n\t"
							"popq	%%r10				\n\t"
							"popq	%%rdx				\n\t"
							"popq	%%rcx				\n\t"
						:	"=a"(ret_val)
						:	"a"(syscall_nr)
						:
						);
	return ret_val;
}

void user_func()
{
	int sc_nr = 0x987654;
	int i = 0;

	// i = sys_call(sc_nr);
	__asm__ __volatile__(	"pushq	%%r10				\n\t"
							"pushq	%%r11				\n\t"
							"leaq	serptmp(%%rip), %%r10	\n\t"
							"movq	%%rsp,	%%r11		\n\t"
							"sysenter					\n\t"
							"serptmp:						\n\t"
							"xchgq	%%rdx,	%%r10		\n\t"
							"xchgq	%%rcx,	%%r11		\n\t"
							"popq	%%r11				\n\t"
							"popq	%%r10				\n\t"
						:	"=a"(i)
						:	"a"(sc_nr)
						:
						);

}

int do_syscall(int syscall_nr)
{
	color_printk(GREEN, BLACK, "enter syscall function, SC_NR = %#08lx", syscall_nr);
	return 0x12345678;
}

unsigned long do_execve(stack_frame_s * sf_regs)
{
	return 1;
}

/*==============================================================================================*
 *									subcopy & exit funcstions									*
 *==============================================================================================*/
PML4E_T * create_userpage()
{
	int user_page_nr = 4;
	uint64_t attr = ARCH_PG_PRESENT | ARCH_PG_RW | ARCH_PG_USER;
	PML4E_T	* user_pml4 = (PML4E_T *)kmalloc(PGENT_SIZE);
	memset(user_pml4, 0, PGENT_SIZE);
	PDPTE_T	* user_pdpt = (PDPTE_T *)kmalloc(1 * PGENT_SIZE);
	memset(user_pdpt, 0, PGENT_SIZE);
	PDE_T	* user_pd	= (PDE_T *)kmalloc(1 * 1 * PGENT_SIZE);
	memset(user_pd, 0, PGENT_SIZE);

	user_pml4[0].ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(user_pdpt)) | ARCH_PGE_NOT_LAST(attr);
	user_pdpt[0].ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(user_pd)) | ARCH_PGE_NOT_LAST(attr);
	for (int i = 0; i < user_page_nr; i ++)
	{
		Page_s * pg = page_alloc();
		user_pd[i].ENT = MASKF_2M((uint64_t)pg->page_start_addr) | ARCH_PGE_IS_LAST(attr);
	}
}

unsigned long copy_flags(unsigned long clone_flags, task_s * new_tsk)
{ 
	if(clone_flags & CLONE_VM)
		new_tsk->flags |= PF_VFORK;
	return 0;
}

unsigned long copy_files(unsigned long clone_flags, task_s * new_tsk)
{
	return 0;
}
unsigned long exit_files(task_s * new_tsk)
{
	return 0;
}

unsigned long copy_mm(unsigned long clone_flags, task_s * new_tsk)
{
	int error = 0;
	Page_s * page = NULL;
	PML4E_T * new_cr3 = NULL;
	if(clone_flags & CLONE_VM)
	{
		new_tsk->mm_struct = curr_tsk->mm_struct;
		goto exit_cpmm;
	}

	new_cr3 = (PML4E_T *)kmalloc(PGENT_SIZE);
	if (new_cr3 == NULL)
	{
		error = -ENOMEM;
		goto exit_cpmm;
	}
	memset(new_cr3, 0, PGENT_SIZE);
	page = page_alloc();
	new_tsk->mm_struct->cr3 = (PML4E_T *)virt2phys((virt_addr)new_cr3);
	arch_page_domap((virt_addr)USER_CODE_ADDR, page->page_start_addr,
					ARCH_PG_PRESENT | ARCH_PG_USER | ARCH_PG_RW, new_cr3);

exit_cpmm:
	return error;
}
unsigned long exit_mm(task_s * new_tsk)
{
	return 0;
}

unsigned long copy_thread(unsigned long clone_flags,unsigned long stack_start, task_s * new_task,stack_frame_s * sf_regs)
{
	sf_regs->rax = 0;
	// memcpy(regs,childregs,sizeof(struct pt_regs));
	// childregs->rsp = stack_start;

	// thd->rsp0 = (unsigned long)tsk + STACK_SIZE;
	// thd->rsp = (unsigned long)childregs;
	// thd->fs = current->thread->fs;
	// thd->gs = current->thread->gs;

	// if(tsk->flags & PF_KTHREAD)
	// 	thd->rip = (unsigned long)kernel_thread_func;
	// else
	// 	thd->rip = (unsigned long)ret_system_call; 

	// color_printk(WHITE,BLACK,"current user ret addr:%#018lx,rsp:%#018lx\n",regs->r10,regs->r11);
	// color_printk(WHITE,BLACK,"new user ret addr:%#018lx,rsp:%#018lx\n",childregs->r10,childregs->r11);

	return 0;
}
void exit_thread(task_s * new_task)
{

}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void wakeup_task(task_s * task)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	task->state |= PS_RUNNING;
	m_push_list(task, &(cpudata_p->ready_tasks));
}

unsigned long do_fork(stack_frame_s * sf_regs,
						unsigned long clone_flags,
						unsigned long tmp_kstack_start,
						unsigned long stack_size)
{
	long ret_val = 0;
	PCB_u * curr_pcb	= container_of(get_current_task(), PCB_u, task);
	PCB_u * new_pcb		= (PCB_u *)kmalloc(sizeof(PCB_u));
	task_s * curr_task	= &curr_pcb->task;
	task_s * new_task	= &new_pcb->task;
	if (new_pcb == NULL)
	{
		ret_val = -EAGAIN;
		goto alloc_newtask_fail;
	}

	memset(new_pcb, 0, sizeof(PCB_u));
	memcpy(new_task, curr_pcb, sizeof(task_s));

	m_list_init(new_task);
	new_pcb->task.pid = get_newpid();

	stack_frame_s * new_sf_regs = get_stackframe(new_task);
	memcpy(new_sf_regs, sf_regs, sizeof(stack_frame_s));

	new_task->vruntime = 0;
	new_task->arch_struct.tss_rsp0 = (reg_t)new_pcb + TASK_KSTACK_SIZE;
	new_task->arch_struct.k_rip = sf_regs->rip;
	new_task->arch_struct.k_rsp = (reg_t)new_sf_regs;

	ret_val = -ENOMEM;
	//	copy flags
	if(copy_flags(clone_flags, new_task))
		goto copy_flags_fail;

	//	copy mm struct
	if(copy_mm(clone_flags, new_task))
		goto copy_mm_fail;

	//	copy file struct
	if(copy_files(clone_flags, new_task))
		goto copy_files_fail;

	// copy thread struct
	if(copy_thread(clone_flags, stack_size, new_task, new_sf_regs))
		goto copy_thread_fail;

	ret_val = new_task->pid;
	wakeup_task(new_task);
	goto do_fork_success;

	copy_thread_fail:
		// exit_thread(new_task);
	copy_files_fail:
		exit_files(new_task);
	copy_mm_fail:
		exit_mm(new_task);
	copy_flags_fail:
	alloc_newtask_fail:
		kfree(new_task);

	do_fork_success:
		return ret_val;
}

unsigned long do_exit(unsigned long code)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	color_printk(RED,WHITE,"Core-%d:exit task is running,arg:%#018lx\n", cpudata_p->cpu_idx, code);
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

	return do_fork(&sf_regs, flags | CLONE_VM, 0, 0);
}

void arch_init_task()
{
	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
	init_spin_lock(&newpid_lock);
}

/*==============================================================================================*
 *										test threads											*
 *==============================================================================================*/

/*==============================================================================================*
 *									load_balance related functions								*
 *==============================================================================================*/
void insert_running_list(per_cpudata_s * cpudata_p)
{
	task_s * curr_task = cpudata_p->curr_task;
	if (curr_task != NULL)
	{
		if (curr_task == cpudata_p->idle_task)
			m_append_to_list(curr_task, &cpudata_p->ready_tasks);
		else
		{
			task_s * tail = cpudata_p->ready_tasks.head_p->prev;
			task_s * tmp = cpudata_p->ready_tasks.head_p;
			while((curr_task->vruntime > tmp->vruntime) &&
					(tmp != tail))
				tmp = tmp->next;
			__m_list_insert_front(curr_task, tmp);
			curr_task->list_header = &cpudata_p->ready_tasks;
			cpudata_p->ready_tasks.count++;
			if (tmp == cpudata_p->ready_tasks.head_p)
				cpudata_p->ready_tasks.head_p = tmp->prev;
		}
	}
}

void schedule()
{
	per_cpudata_s *	cpudata_p = curr_cpu;
	task_s *		next_task = NULL;
	task_s *		curr_task = cpudata_p->curr_task;

	unsigned long used_jiffies = jiffies - cpudata_p->last_jiffies;
	if (curr_task != cpudata_p->idle_task)
		curr_task->vruntime += used_jiffies;
	// if running time out, make the need_schedule flag of current task
	if (used_jiffies >= cpudata_p->time_slice)
		cpudata_p->curr_task->flags |= PF_NEED_SCHEDULE;

	if (((curr_task->state == PS_RUNNING) && !(curr_task->flags & PF_NEED_SCHEDULE)) ||
		((curr_task == cpudata_p->idle_task) && (cpudata_p->ready_tasks.count == 0)) ||
		(curr_task->spin_count != 0) ||
		(curr_task->semaphore_count != 0))
	{
		return;
	}
	else if (curr_task->state == PS_RUNNING &&
			(curr_task->flags & PF_NEED_SCHEDULE))
	{
		// normal schedule
		insert_running_list(cpudata_p);
	}

	cpudata_p->scheduleing_flag = 1;
	// get next task
	// find a suit position for current insertion

	m_pop_list(next_task, &cpudata_p->ready_tasks);
	cpudata_p->curr_task = next_task;

	cpudata_p->last_jiffies = jiffies;
	cpudata_p->time_slice = next_task->time_slice;

	cpudata_p->scheduleing_flag = 0;
	switch_mm(curr_task, next_task);
	switch_to(curr_task, next_task);
}

void load_balance()
{
	// per_cpudata_s * cpudata_p = &percpu_data[0]->cpudata;
	// if (cpudata_p->scheduleing_flag)
	// 	return;

	// cpudata_p->scheduleing_flag = 1;
	// // retrieve finished tasks, then assign them back to cpu
	// while (cpudata_p->finished_tasks.count)
	// {
	// 	task_s * tmp = task_list_pop(&cpudata_p->finished_tasks);
	// 	task_list_push(&global_ready_task, tmp);
	// }
	// while (global_ready_task.count)
	// {
	// 	task_s * tmp = task_list_pop(&global_ready_task);
	// 	task_list_push(&cpudata_p->ready_tasks, tmp);
	// }
	// // insert load_balance self to cpu's waiting list end
	// task_s * current = get_current_task();
	// current->flags |= PF_NEED_SCHEDULE;
	// cpudata_p->scheduleing_flag = 0;
}
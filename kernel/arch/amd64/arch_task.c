#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/errno.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>
#include <lib/fcntl.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"
#include "include/arch_proto.h"
#include "include/arch_glo.h"

#include "../../include/glo.h"
#include "../../include/proto.h"
#include "../../include/task.h"
#include "../../include/printk.h"
#include "../../include/const.h"
#include "../../include/vfs.h"
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
	wrmsr(MSR_IA32_SYSENTER_ESP, target->arch_struct.tss_rsp0);
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

file_s * open_exec_file(char * path)
{
	dirent_s * dentry = NULL;
	file_s * filp = NULL;

	dentry = path_walk(path, 0);

	if(dentry == NULL)
		return (void *)-ENOENT;
	if(dentry->dir_inode->attribute == FS_ATTR_DIR)
		return (void *)-ENOTDIR;

	filp = (file_s *)kmalloc(sizeof(file_s));
	if(filp == NULL)
		return (void *)-ENOMEM;

	filp->position = 0;
	filp->mode = 0;
	filp->dentry = dentry;
	filp->mode = O_RDONLY;
	filp->f_ops = dentry->dir_inode->f_ops;

	return filp;
}

int read_exec_mm(file_s * fp, task_s * curr)
{
	mm_s * mm = curr->mm_struct;

	mm->start_code = USER_CODE_ADDR;
	mm->start_rodata =
	mm->end_code = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 1;
	mm->start_data =
	mm->end_rodata = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 2;
	mm->start_bss =
	mm->end_data = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 3;
	mm->start_brk =
	mm->end_bss = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 4;
	mm->end_brk = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 5;
	mm->start_stack = USER_CODE_ADDR + CONFIG_PAGE_SIZE * 6;
}

void wakeup_task(task_s * task)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	task->state = PS_RUNNING;
	m_push_list(task, &(cpudata_p->ready_tasks));
}

/*==============================================================================================*
 *									subcopy & exit funcstions									*
 *==============================================================================================*/
unsigned long copy_flags(unsigned long clone_flags, task_s * new_tsk)
{ 
	if(clone_flags & CLONE_VM)
		new_tsk->flags |= PF_VFORK;
	return 0;
}

unsigned long copy_files(unsigned long clone_flags, task_s * new_tsk)
{
	task_s * curr = curr_tsk;
	int error = 0;
	int i = 0;
	if(clone_flags & CLONE_FS)
		goto out;
	
	for(i = 0; i < MAX_FILE_NR; i++)
		if(curr->fps[i] != NULL)
		{
			new_tsk->fps[i] = (file_s *)kmalloc(sizeof(file_s));
			memcpy(new_tsk->fps[i], curr->fps[i], sizeof(file_s));
		}
out:
	return error;
}
void exit_files(task_s * new_tsk)
{
	int i = 0;
	if(new_tsk->flags & PF_VFORK)
		;
	else
		for(i = 0; i < MAX_FILE_NR; i++)
			if(new_tsk->fps[i] != NULL)
			{
				kfree(new_tsk->fps[i]);
			}

	memset(new_tsk->fps, 0, sizeof(file_s *) * MAX_FILE_NR);	//clear current->file_struct
}

unsigned long copy_mm(unsigned long clone_flags, task_s * new_tsk)
{
	int error = 0;

	Page_s * page = NULL;
	PML4E_T * new_cr3 = NULL;
	task_s * curr = curr_tsk;
	reg_t curr_endstack = get_stackframe(curr)->rsp;

	if(clone_flags & CLONE_VM)
	{
		new_tsk->mm_struct = curr->mm_struct;
		goto exit_cpmm;
	}
	else
	{
		new_tsk->mm_struct = (mm_s *)kmalloc(sizeof(mm_s));
		memcpy(new_tsk->mm_struct, curr->mm_struct, sizeof(mm_s));
		prepair_COW(curr);
	}

exit_cpmm:
	return error;
}
void exit_mm(task_s * new_tsk)
{
	if(new_tsk->flags & PF_VFORK)
		return;
}

unsigned long copy_thread(unsigned long clone_flags, unsigned long stack_start,
							task_s * child_task, 	stack_frame_s * parent_context)
{
	stack_frame_s * child_context = get_stackframe(child_task);
	memcpy(child_context,  parent_context, sizeof(stack_frame_s));

	child_context->rsp = stack_start;
	child_context->rax = 0;

	child_task->arch_struct.tss_rsp0 = (reg_t)child_task + TASK_KSTACK_SIZE;
	child_task->arch_struct.k_rsp = (reg_t)child_context;

	if(child_task->flags & PF_KTHREAD)
		child_task->arch_struct.k_rip = (unsigned long)kernel_thread_func;
	else
		child_task->arch_struct.k_rip = (unsigned long)ret_from_sysenter;

	return 0;
}
unsigned long exit_thread(task_s * new_task)
{
	return 0;
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
unsigned long do_fork(stack_frame_s * parent_context,
						unsigned long clone_flags,
						unsigned long tmp_kstack_start,
						unsigned long stack_size)
{
	long ret_val = 0;
	PCB_u * parent_PCB = container_of(get_current_task(), PCB_u, task);
	PCB_u * child_PCB = (PCB_u *)kmalloc(sizeof(PCB_u));
	task_s * parent_task = &parent_PCB->task;
	task_s * child_task = &child_PCB->task;
	if (child_PCB == NULL)
	{
		ret_val = -EAGAIN;
		goto alloc_newtask_fail;
	}

	memset(child_PCB, 0, sizeof(PCB_u));
	memcpy(child_task, parent_task, sizeof(task_s));

	child_task->state = PS_UNINTERRUPTIBLE;
	child_task->pid = get_newpid();
	child_task->vruntime = 0;
	child_task->parent = parent_task;
	m_list_init(child_task);

	ret_val = -ENOMEM;
	//	copy flags
	if(copy_flags(clone_flags, child_task))
		goto copy_flags_fail;
	//	copy mm struct
	if(copy_mm(clone_flags, child_task))
		goto copy_mm_fail;
	//	copy file struct
	if(copy_files(clone_flags, child_task))
		goto copy_files_fail;
	// copy thread struct
	if(copy_thread(clone_flags, stack_size, child_task, parent_context))
		goto copy_thread_fail;

	// do_fork successed
	ret_val = child_task->pid;
	wakeup_task(child_task);
	goto do_fork_success;

	// if failed clean memory
	copy_thread_fail:
		exit_thread(child_task);
	copy_files_fail:
		exit_files(child_task);
	copy_mm_fail:
		exit_mm(child_task);
	copy_flags_fail:
	alloc_newtask_fail:
		kfree(child_task);

	do_fork_success:
		return ret_val;
}

unsigned long do_execve(stack_frame_s * curr_context, char *name, char *argv[], char *envp[])
{
	int ret_val = 0;

	task_s * curr = curr_tsk;
	exit_files(curr);
	file_s * fp = open_exec_file(name);

	if (curr->flags & PF_VFORK)
	{
		curr->mm_struct = (mm_s *)kmalloc(sizeof(mm_s));
		memset(curr->mm_struct, 0, sizeof(mm_s));

		PML4E_T * virt_cr3 = (PML4E_T *)kmalloc(PGENT_SIZE);
		curr->mm_struct->cr3 = (reg_t)virt2phys(virt_cr3);
		memcpy(virt_cr3 + PGENT_NR / 2,
				&KERN_PML4[PGENT_NR / 2],
				PGENT_SIZE / 2);
		memset(virt_cr3, 0, PGENT_SIZE / 2);
	}
	read_exec_mm(fp, curr);
	creat_exec_addrspace(curr);
	pg_load_cr3(curr->mm_struct->cr3);
	curr->flags &= ~PF_VFORK;

	long argv_pos = 0;
	if(argv != NULL)
	{
		int argc = 0;
		int len = 0;
		int i = 0;
		char ** dargv = (char **)(curr->mm_struct->start_stack - 10 * sizeof(char *));
		argv_pos = (unsigned long)dargv;

		for(i = 0; i < 10 && argv[i] != NULL; i++)
		{
			len = strnlen_user(argv[i], 1024) + 1;
			strcpy((char *)(argv_pos - len), argv[i]);
			dargv[i] = (char *)(argv_pos - len);
			argv_pos -= len;
		}
		curr->mm_struct->start_stack = argv_pos - 10;
		curr_context->rdi = i;	//argc
		curr_context->rsi = (unsigned long)dargv;	//argv
	}

	memset((virt_addr)curr->mm_struct->start_code, 0, curr->mm_struct->end_data - curr->mm_struct->start_code);
	long fp_pos = 0;
	ret_val = fp->f_ops->read(fp, (void *)curr->mm_struct->start_code, fp->dentry->dir_inode->file_size, &fp_pos);

	curr_context->ds =
	curr_context->es =
	curr_context->ss = USER_SS_SELECTOR;
	curr_context->cs = USER_CS_SELECTOR;
	curr_context->r10 = curr->mm_struct->start_code;
	curr_context->r11 = curr->mm_struct->start_stack;
	curr_context->rax = 1;

	return ret_val;
}

void exit_notify(void)
{
	// wakeup(&current->parent->wait_childexit,TASK_INTERRUPTIBLE);
}

unsigned long do_exit(unsigned long exit_code)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	task_s * curr = curr_tsk;
	// color_printk(RED,WHITE,"Core-%d:exit task is running,arg:%#018lx\n", cpudata_p->cpu_idx, exit_code);

do_exit_again:
	cli();
	curr->state = PS_ZOMBIE;
	curr->exit_code = exit_code;
	exit_thread(curr);
	exit_files(curr);
	sti();

	exit_notify();
	schedule();

	goto do_exit_again;
	return 0;
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
 *										*
 *==============================================================================================*/
unsigned long init(unsigned long arg)
{
	init_vfs();

	color_printk(GREEN, BLACK, "VFS initiated.\n");

	task_s * curr = curr_tsk;
	stack_frame_s * curr_sfp = get_stackframe(curr);
	curr->arch_struct.k_rip = (reg_t)ret_from_sysenter;
	curr->arch_struct.k_rsp = (reg_t)curr_sfp;
	curr->flags &= ~PF_KTHREAD;


	__asm__	__volatile__(	"movq	%1,	%%rsp	\n\t"
							"pushq	%2			\n\t"
							"jmp	do_execve	\n\t"
						:
						:	"D"(curr->arch_struct.k_rsp),"m"(curr->arch_struct.k_rsp),
							"m"(curr->arch_struct.k_rip),"S"("/init.bin"),"d"(NULL),"c"(NULL)
						:	"memory"
						);

	return 1;
}


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

}
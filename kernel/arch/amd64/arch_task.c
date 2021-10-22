#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/_null.h>
#include <sys/errno.h>

#include <string.h>
#include <lib/utils.h>
#include <lib/fcntl.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"
#include "include/arch_proto.h"
#include "include/arch_glo.h"
#include "include/device.h"

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
task_s * task_init = NULL;

bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
spinlock_T		newpid_lock;
unsigned long	curr_pid;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void arch_init_task()
{
	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
	init_spin_lock(&newpid_lock);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
unsigned long gen_newpid()
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
	int error = 0;
	int i = 0;
	if(clone_flags & CLONE_FS)
		goto out;
	
	for(i = 0; i < MAX_FILE_NR; i++)
		if(curr_tsk->fps[i] != NULL)
		{
			new_tsk->fps[i] = (file_s *)kmalloc(sizeof(file_s));
			memcpy(new_tsk->fps[i], curr_tsk->fps[i], sizeof(file_s));
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
	reg_t curr_endstack = get_stackframe(curr_tsk)->rsp;

	if(clone_flags & CLONE_VM)
	{
		new_tsk->mm_struct = curr_tsk->mm_struct;
		goto exit_cpmm;
	}
	else
	{
		new_tsk->mm_struct = (mm_s *)kmalloc(sizeof(mm_s));
		memcpy(new_tsk->mm_struct, curr_tsk->mm_struct, sizeof(mm_s));
		prepair_COW(curr_tsk);
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
	{
		child_task->arch_struct.k_rip = (unsigned long)entp_kernel_thread;
		child_context->restore_retp = (reg_t)ra_kthd_retp;
	}
	else
	{
		child_task->arch_struct.k_rip = (unsigned long)dofork_child_entp;
		child_context->restore_retp = (reg_t)ra_sysex_retp;
	}

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

	list_init(&child_task->schedule_list, child_task);
	list_init(&child_task->child_list, child_task);
	list_hdr_init(&child_task->child_lhdr);
	list_hdr_init(&child_task->wait_childexit);
	child_task->state = PS_UNINTERRUPTIBLE;
	child_task->pid = gen_newpid();
	child_task->vruntime = 0;
	child_task->parent = parent_task;
	list_hdr_append(&parent_task->child_lhdr, &child_task->child_list);

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

	exit_files(curr_tsk);
	file_s * fp = open_exec_file(name);

	if (curr_tsk->flags & PF_VFORK)
	{
		curr_tsk->mm_struct = (mm_s *)kmalloc(sizeof(mm_s));
		memset(curr_tsk->mm_struct, 0, sizeof(mm_s));

		PML4E_T * virt_cr3 = (PML4E_T *)kmalloc(PGENT_SIZE);
		curr_tsk->mm_struct->cr3 = (reg_t)virt2phys(virt_cr3);
		memcpy(virt_cr3 + PGENT_NR / 2,
				&KERN_PML4[PGENT_NR / 2],
				PGENT_SIZE / 2);
		memset(virt_cr3, 0, PGENT_SIZE / 2);
	}
	read_exec_mm(fp, curr_tsk);
	creat_exec_addrspace(curr_tsk);
	pg_load_cr3(curr_tsk->mm_struct->cr3);
	curr_tsk->flags &= ~PF_VFORK;

	long argv_pos = 0;
	if(argv != NULL)
	{
		int argc = 0;
		int len = 0;
		int i = 0;
		char ** dargv = (char **)(curr_tsk->mm_struct->start_stack - 10 * sizeof(char *));
		argv_pos = (unsigned long)dargv;

		for(i = 0; i < 10 && argv[i] != NULL; i++)
		{
			len = strnlen_user(argv[i], 1024) + 1;
			strcpy((char *)(argv_pos - len), argv[i]);
			dargv[i] = (char *)(argv_pos - len);
			argv_pos -= len;
		}
		curr_tsk->mm_struct->start_stack = argv_pos - 10;
		curr_context->rdi = i;	//argc
		curr_context->rsi = (unsigned long)dargv;	//argv
	}

	memset((virt_addr)curr_tsk->mm_struct->start_code, 0, curr_tsk->mm_struct->end_data - curr_tsk->mm_struct->start_code);
	long fp_pos = 0;
	ret_val = fp->f_ops->read(fp, (void *)curr_tsk->mm_struct->start_code, fp->dentry->dir_inode->file_size, &fp_pos);

	curr_context->ss = USER_SS_SELECTOR;
	curr_context->cs = USER_CS_SELECTOR;
	curr_context->r10 = curr_tsk->mm_struct->start_code;
	curr_context->r11 = curr_tsk->mm_struct->start_stack;
	curr_context->rax = 1;

	return ret_val;
}

void exit_notify(void)
{
	task_s * curr = curr_tsk;
	while (curr->child_lhdr.count != 0)
	{
		List_s * child_lp = list_hdr_pop(&curr->child_lhdr);
		list_hdr_append(&task_init->child_lhdr, child_lp);
	}
	wq_wakeup(&curr_tsk->wait_childexit, PS_INTERRUPTIBLE);
}

unsigned long do_exit(unsigned long exit_code)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	task_s * curr = curr_tsk;
	color_printk(RED, WHITE, "Core-%d: task:%d exited.\n", cpudata_p->cpu_idx, curr->pid);

do_exit_again:
	cli();
	curr_tsk->exit_code = exit_code;
	exit_thread(curr_tsk);
	exit_files(curr_tsk);
	exit_notify();
	sti();

	curr_tsk->state = PS_ZOMBIE;
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
	sf_regs.ss = KERN_SS_SELECTOR;
	sf_regs.rflags = (1 << 9);
	sf_regs.rip = (reg_t)entp_kernel_thread;

	return do_fork(&sf_regs, flags | CLONE_VM, 0, 0);
}


/*==============================================================================================*
 *										task2 -- init()											*
 *==============================================================================================*/
unsigned long init(unsigned long arg)
{
	init_vfs();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");
	creat_dev_file();
	// color_printk(GREEN, BLACK, "Device files created.\n");

	// here if derictly use macro:curr_tsk will cause unexpected rewriting memory
	task_s * curr = curr_tsk;
	task_init = curr;
	stack_frame_s * curr_sfp = get_stackframe(curr);
	curr_sfp->restore_retp = (reg_t)ra_sysex_retp;

	reg_t ctx_rip =
	curr->arch_struct.k_rip = (reg_t)sysexit_entp;
	reg_t ctx_rsp =
	curr->arch_struct.k_rsp = (reg_t)curr_sfp;
	curr->flags &= ~PF_KTHREAD;

	__asm__	__volatile__(	"movq	%1,	%%rsp	\n\t"
							"pushq	%2			\n\t"
							"jmp	do_execve	\n\t"
						:
						:	"D"(ctx_rsp), "m"(ctx_rsp),
							"m"(ctx_rip), "S"("/init.bin"),
							"d"(NULL), "c"(NULL)
						:	"memory"
						);

	return 1;
}

/*==============================================================================================*
 *									schedule functions											*
 *==============================================================================================*/
inline __always_inline task_s * get_current_task()
{
	task_s * curr_task = NULL;
	__asm__ __volatile__(	"andq 	%%rsp,	%1		\n\t"
						:	"=r"(curr_task)
						:	"r"(~(TASK_KSTACK_SIZE - 1))
						:
						);
	return curr_task;
}

stack_frame_s * get_stackframe(task_s * task_p)
{
	PCB_u * pcb_p = container_of(task_p, PCB_u, task);
	return (stack_frame_s *)(pcb_p + 1) - 1;
}

int load_balance()
{
	int i;
	int min_load = 99999999;
	int min_idx = 0;
	for (i = 0; i < kparam.nr_lcpu; i++)
	{
		per_cpudata_s * cpu_p = &percpu_data[i]->cpudata;
		if (cpu_p->ruuning_lhdr.count < min_load)
		{
			min_load = cpu_p->ruuning_lhdr.count;
			min_idx = i;
		}
	}
	return min_idx;
}

void wakeup_task(task_s * task)
{
	int target_cpu_idx = load_balance();
	per_cpudata_s * target_cpu_p = &percpu_data[target_cpu_idx]->cpudata;
	task->state = PS_RUNNING;
	list_hdr_push(&target_cpu_p->ruuning_lhdr, &task->schedule_list);

	// color_printk(BLUE, WHITE, "Task-%d inserted to cpu:%d", task->pid, target_cpu_idx);
}

void reinsert_to_running_list(per_cpudata_s * cpudata_p)
{
	task_s * curr_task = cpudata_p->curr_task;
	if (curr_task != NULL)
	{
		if (curr_task == cpudata_p->idle_task)
		{
			list_hdr_enqueue(&cpudata_p->ruuning_lhdr, &curr_task->schedule_list);
		}
		else
		{
			List_s * tmp_list = cpudata_p->ruuning_lhdr.header.next;
			while ((curr_task->vruntime > container_of(tmp_list, task_s, schedule_list)->vruntime) &&
					tmp_list != &cpudata_p->ruuning_lhdr.header)
			{
				tmp_list = tmp_list->next;
			}
			list_insert_prev(tmp_list, &curr_task->schedule_list);
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
		((curr_task == cpudata_p->idle_task) && (cpudata_p->ruuning_lhdr.count == 0)) ||
		(curr_task->spin_count != 0) ||
		(curr_task->semaphore_count != 0))
	{
		return;
	}
	else if (curr_task->state == PS_RUNNING &&
			(curr_task->flags & PF_NEED_SCHEDULE))
	{
		// normal schedule
		reinsert_to_running_list(cpudata_p);
	}

	cpudata_p->scheduleing_flag = 1;
	// get next task
	// find a suit position for current insertion
	next_task = container_of(list_hdr_pop(&cpudata_p->ruuning_lhdr), task_s, schedule_list);
	cpudata_p->curr_task = next_task;

	cpudata_p->last_jiffies = jiffies;
	cpudata_p->time_slice = next_task->time_slice;

	cpudata_p->scheduleing_flag = 0;
	switch_mm(curr_task, next_task);
	switch_to(curr_task, next_task);
}

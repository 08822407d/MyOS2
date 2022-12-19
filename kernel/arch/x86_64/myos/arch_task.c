#include <linux/kernel/slab.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>
#include <linux/fs/fs.h>
#include <linux/fs/mount.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>
#include <linux/lib/list.h>
#include <asm/processor.h>
#include <asm/setup.h>
#include <asm/fsgsbase.h>


#include <klib/utils.h>
#include <obsolete/glo.h>
#include <obsolete/printk.h>

#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_proto.h>
#include <obsolete/arch_glo.h>
#include <obsolete/device.h>

#define	USER_CODE_ADDR		0x6000000
#define USER_MEM_LENGTH		0x800000
#define MAX_PID				0x8000

extern char		ist_stack0;

extern PCB_u	task0_PCB;
task_s			*task_idle = NULL;
task_s			*task_init = NULL;
task_s			*task_shell = NULL;

bitmap_t		pid_bm[MAX_PID / sizeof(bitmap_t)];
myos_spinlock_T		newpid_lock;
unsigned long	curr_pid;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void myos_preinit_arch_task()
{
	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	curr_pid = 0;
	init_spin_lock(&newpid_lock);
}

void myos_init_arch_task(size_t cpu_idx)
{
	PCB_u * idle_pcb = idle_tasks[cpu_idx];
	tss64_T * tss_p = tss_ptr_arr + cpu_idx;
	idle_pcb->task.thread.tss_rsp0 = (reg_t)idle_pcb + THREAD_SIZE;
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
unsigned long myos_pid_nr()
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

static __always_inline void switch_mm(task_s * curr, task_s * target)
{
	asm volatile(	"movq	%0,	%%cr3		\n\t"
				:
				:	"r"(target->mm->pgd_ptr)
				:	"memory"
				);
	wrmsr(MSR_IA32_SYSENTER_ESP, target->thread.tss_rsp0);
}

inline __always_inline void __myos_switch_to(task_s * curr, task_s * target)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	tss64_T * curr_tss = cpudata_p->arch_info.tss;
	curr_tss->rsp0 = target->thread.tss_rsp0;

	// since when intel cpu reload gs and fs, gs_base and fs_base will be reset
	// here need to save and restore them

	// asm volatile("movq	%%fs,	%0 \n\t"
	// 			: "=a"(curr->thread.fs));
	// asm volatile("movq	%%gs,	%0 \n\t"
	// 			: "=a"(curr->thread.gs));

	// asm volatile("movq	%0,	%%fs \n\t"
	// 			:
	// 			:"a"(target->thread.fs));
	// asm volatile("movq	%0,	%%gs \n\t"
	// 			:
	// 			:"a"(target->thread.gs));

}

inline __always_inline void myos_switch_to(task_s * curr, task_s * target)
{
	asm volatile(	"pushq	%%rbp				\n\t"
					"pushq	%%rax				\n\t"
					"movq	%%rsp,	%0			\n\t"
					"movq	%2,		%%rsp		\n\t"
					"leaq	1f(%%rip),	%%rax	\n\t"
					"movq	%%rax,	%1			\n\t"
					"pushq	%3					\n\t"
					"jmp	__myos_switch_to	\n\t"
					"1:							\n\t"
					"popq	%%rax				\n\t"
					"popq	%%rbp				\n\t"
				:	"=m"(curr->thread.sp),
					"=m"(curr->thread.k_rip)
				:	"m"(target->thread.sp),
					"m"(target->thread.k_rip),
					"D"(curr), "S"(target)
				:	"memory"
				);
}

extern int myos_exit_thread(task_s * new_task);

// read memory distribution of the executive
static int read_exec_mm(file_s * fp, task_s * curr)
{
	mm_s * mm = curr->mm;

	mm->start_code = USER_CODE_ADDR;
	mm->start_data =
	mm->end_code = USER_CODE_ADDR + SZ_2M * 1;
	mm->start_brk =
	mm->end_data = USER_CODE_ADDR + SZ_2M * 2;
	mm->brk = USER_CODE_ADDR + SZ_2M * 3;
	mm->start_stack = USERADDR_LIMIT + 1 - SZ_2M;
}

extern void myos_close_files(files_struct_s * files);
unsigned long do_execve(stack_frame_s *curr_context, char *exec_filename, char *argv[], char *envp[])
{
	int ret_val = 0;
	task_s *curr = current;

	//
	if (task_idle != NULL && task_init != NULL)
		task_shell = curr;
	if (task_idle != NULL && task_init == NULL)
		task_init = curr;
	//
	myos_close_files(curr->files);
	(curr);
	file_s *fp = filp_open(exec_filename, O_RDONLY, 0);

	if (curr->flags & CLONE_VFORK)
	{
		curr->mm = (mm_s *)kzalloc(sizeof(mm_s), GFP_KERNEL);

		pgd_t *virt_cr3 = (pgd_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		// pgd_t *new_pgd = kzalloc(sizeof(pgd_t), GFP_KERNEL);
		// new_pgd->pgd = myos_virt2phys((virt_addr_t)virt_cr3);
		// curr->mm->pgd = new_pgd;
		curr->mm->pgd_ptr = myos_virt2phys((virt_addr_t)virt_cr3);
		int hfent_nr = PGENT_NR / 2;
		memcpy(virt_cr3 + hfent_nr, &init_top_pgt[hfent_nr], hfent_nr);
	}
	read_exec_mm(fp, curr);
	creat_exec_addrspace(curr);
	load_cr3(curr->mm->pgd_ptr);
	curr->flags &= ~CLONE_VFORK;

	long argv_pos = 0;
	if(argv != NULL)
	{
		int argc = 0;
		int len = 0;
		int i = 0;
		char ** dargv = (char **)(curr->mm->start_stack - 10 * sizeof(char *));
		argv_pos = (unsigned long)dargv;

		for(i = 0; i < 10 && argv[i] != NULL; i++)
		{
			len = strnlen(argv[i], 1024) + 1;
			if (len <= 0)
				continue;
			strcpy((char *)(argv_pos - len), argv[i]);
			dargv[i] = (char *)(argv_pos - len);
			argv_pos -= len;
		}
		curr->mm->start_stack = argv_pos - 10;
		curr_context->rdi = i;	//argc
		curr_context->rsi = (unsigned long)dargv;	//argv
	}

	memset((void *)curr->mm->start_code, 0,
			curr->mm->end_data - curr->mm->start_code);
	loff_t fp_pos = 0;
	ret_val = fp->f_op->read(fp, (void *)curr->mm->start_code,
			fp->f_path.dentry->d_inode->i_size, &fp_pos);

	curr->name = exec_filename;

	curr_context->ss = USER_SS_SELECTOR;
	curr_context->cs = USER_CS_SELECTOR;
	curr_context->r10 = curr->mm->start_code;
	curr_context->r11 = curr->mm->start_stack;
	curr_context->rax = 1;

	return ret_val;
}

void kjmp_to_doexecve()
{
	task_idle = &task0_PCB.task;
	// here if derictly use macro:current will cause unexpected rewriting memory
	task_s * curr = current;
	stack_frame_s * curr_sfp = get_stackframe(curr);
	curr_sfp->restore_retp = (virt_addr_t)ra_sysex_retp;

	reg_t ctx_rip =
	curr->thread.k_rip = (reg_t)sysexit_entp;
	reg_t ctx_rsp =
	curr->thread.sp = (reg_t)curr_sfp;
	curr->flags &= ~PF_KTHREAD;

	char *argv[] = {"task_init", NULL};
	asm volatile(	"movq	%1,	%%rsp	\n\t"
					"pushq	%2			\n\t"
					"jmp	do_execve	\n\t"
				:
				:	"D"(ctx_rsp), "m"(ctx_rsp),
					"m"(ctx_rip), "S"("/init.bin"),
					"d"(argv), "c"(NULL)
				:	"memory"
				);
}

static void exit_notify(void)
{
	task_s * curr = current;
	while (curr->children.count != 0)
	{
		List_s * child_lp = list_hdr_pop(&curr->children);
		while (child_lp == 0);
		
		list_hdr_append(&task_init->children, child_lp);
	}
	wq_wakeup(&current->wait_childexit, TASK_INTERRUPTIBLE);
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
	myos_schedule();

	goto do_exit_again;
	return 0;
}

task_s *myos_kernel_thread(int (* fn)(void *),
		unsigned long arg, unsigned long flags, char *taskname)
{
	task_s *ret_val = NULL;
	kclone_args_s args = {
		.thread_name	= taskname,

		.flags			= ((lower_32_bits(flags) | CLONE_VM | PF_KTHREAD |
							CLONE_UNTRACED) & ~CSIGNAL),
		.exit_signal	= 0,
		.stack			= (unsigned long)fn,
		.stack_size		= (unsigned long)arg,
	};
	do_fork(&args, &ret_val);

	if (ret_val != NULL)
		ret_val->name = taskname;

	return ret_val;
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

stack_frame_s * get_stackframe(task_s * task_p)
{
	PCB_u * pcb_p = container_of(task_p, PCB_u, task);
	return (stack_frame_s *)(pcb_p + 1) - 1;
}

void myos_schedule()
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

		next_task = container_of(next_lp, task_s, tasks);
		cpudata_p->curr_task = next_task;
		cpudata_p->time_slice = next_task->rt.time_slice;

		cpudata_p->last_jiffies = jiffies;
		cpudata_p->scheduleing_flag = 0;

		curr_task->flags &= ~PF_NEED_SCHEDULE;

		while (curr_task == next_task);
		
		switch_mm(curr_task, next_task);
		myos_switch_to(curr_task, next_task);
	}
}

void try_sched()
{
	per_cpudata_s *	cpudata_p = curr_cpu;
	task_s *		curr_task = cpudata_p->curr_task;

	unsigned long used_jiffies = jiffies - cpudata_p->last_jiffies;
	if (curr_task != cpudata_p->idle_task)
		curr_task->se.vruntime += used_jiffies;
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
		myos_schedule();
}
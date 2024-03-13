#include <linux/kernel/sched.h>
#include <linux/fs/binfmts.h>


#define LOAD_ELF

void kjmp_to_doexecve()
{
	// here if derictly use macro:current will cause unexpected rewriting memory
	task_s * curr = current;
	pt_regs_s *curr_ptregs = task_pt_regs(curr);
	curr->flags &= ~PF_KTHREAD;

#ifdef LOAD_ELF
	const char *initd_name = "/initd";
#else
	const char *initd_name = "/initd.bin";
#endif
	const char *const argv[] =
			{ initd_name , "kernarg_test_1", "kernarg_test_1", NULL };
	const char *const envp[] =
			{ "kernenv_test_1", "kernenv_test_2", "kernenv_test_3", NULL };

	// const char *const argv[] = { "/initd" , NULL };
	// const char *const envp[] = { NULL };

	// 因为这里是内核线程，要通过execve进用户态
	// 需要通过current->mm和active_mm的检查，所以设置下mm
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);

	asm volatile(	"movq	%0,	%%rsp		\n\t"
					"sti					\n\t"
					"jmp	sysexit_entp	\n\t"
				:
				:	"m"(curr_ptregs)
				:
				);
}
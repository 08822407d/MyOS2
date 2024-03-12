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
			{ "arg_test_1", "arg_test_1", NULL };
	const char *const envp[] =
			{ "env_test_1", "env_test_2", "env_test_3", NULL };

	kernel_execve(initd_name, argv, envp);

	asm volatile(	"movq	%0,	%%rsp		\n\t"
					"sti					\n\t"
					"jmp	sysexit_entp	\n\t"
				:
				:	"m"(curr_ptregs)
				:
				);
}
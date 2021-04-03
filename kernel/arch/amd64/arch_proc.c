#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proc.h"

#include "../../include/proc.h"

extern tss64_s	tss[CONFIG_MAX_CPUS];
extern char		ist_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE] __aligned(CONFIG_KSTACK_SIZE);

extern PCB_u	proc0_PCB;

inline void __switch_to(proc_s * curr, proc_s * target)
{
	tss[0].rsp0 = target->arch_struct.rsp0;
}

void inline switch_to(proc_s * curr, proc_s * target)
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
						 :"=m"(curr->arch_struct.rsp),
						  "=m"(curr->arch_struct.rip)
						 :"m"(target->arch_struct.rsp),
						  "m"(target->arch_struct.rip),
						  "D"(curr), "S"(target)
						 :"memory");
}

void arch_init_proc0()
{

}
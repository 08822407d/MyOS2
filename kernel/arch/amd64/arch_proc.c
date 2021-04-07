#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>
#include <lib/utils.h>
#include <lib/stddef.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proc.h"

#include "../../include/proc.h"
#include "../../include/printk.h"

extern tss64_s	tss[CONFIG_MAX_CPUS];
extern char		ist_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE];

extern PCB_u	proc0_PCB;
extern PCB_u	proc1_PCB;

void test_proc1(void);
extern char		ist_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE];

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

inline __always_inline void __switch_to(proc_s * curr, proc_s * target)
{
	tss[0].rsp0 = target->arch_struct.rsp0;
	tss[0].rsp1 =
	tss[0].rsp2 =
	tss[0].ist1 =
	tss[0].ist2 =
	tss[0].ist3 =
	tss[0].ist4 =
	tss[0].ist5 =
	tss[0].ist6 =
	tss[0].ist7 = (uint64_t)&ist_stacks[0] + CONFIG_KSTACK_SIZE;

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
						 :"=m"(curr->arch_struct.rsp),
						  "=m"(curr->arch_struct.rip)
						 :"m"(target->arch_struct.rsp),
						  "m"(target->arch_struct.rip),
						  "D"(curr), "S"(target)
						 :"memory");
}

void arch_init_proc0()
{
	proc_s * curr_proc = get_current();

	proc_s *proc_1 = &proc1_PCB.proc;
	proc_1->arch_struct.fs =
	proc_1->arch_struct.gs = KERN_DS_SELECTOR;
	proc_1->arch_struct.rsp0 = (uint64_t)&proc1_PCB.stack + PROC_KSTACK_SIZE;
	proc_1->arch_struct.rsp = (uint64_t)&ist_stacks[2][0];
	proc_1->arch_struct.rip = (uint64_t)test_proc1;

	switch_to(curr_proc, proc_1);
}

void test_proc1()
{
	color_printk(RED, BLACK, "Now in proc 1\n");
}
#define ASM_FILE 1

#include <linux/kernel/linkage.h>

#include "../arch/amd64/include/archconst.h"

.code64
.balign 8
/*==============================================================================================*
 *									higher half text section							 		*
 *==============================================================================================*/
ENTRY(higher_half_entry)
	movabsq	$task0_PCB,	%rbx
	addq	$TASK_KSTACK_SIZE,	%rbx
	movq    $0x10,		%rax
	movq    %rax,		%ss
	movq    %rbx,		%rsp

	pushq   $0
	popfq
	pushq   $0

	// here init single core ia32-e environment
	callq	start_kernel

	// here is the idle task for all cores
	movq	$0,			%rdi
	callq	idle

#include "arch/amd64/asm.h"

#include "../../include/const.h"

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
	callq	kmain

	// here is the idle task for all cores
	movq	$0,			%rdi
	callq	idle

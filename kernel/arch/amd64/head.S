#include "arch/amd64/asm.h"

.section .headertext

ENTRY(start)
    movw    %ss, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movq    stack_top(%rip), %rsp

    callq   _C_LABEL(pre_init)

    callq   _C_LABEL(kmain)

hang:
	jmp hang

.data
stack_start:
    .fill  512,8,0
stack_top:
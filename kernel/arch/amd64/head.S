#include "arch/amd64/asm.h"

.section .headertext

ENTRY(start)
    movw    %ss, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movq    $0x8000, %rsp

    callq   _C_LABEL(pre_init)

    callq   _C_LABEL(kmain)

hang:
	jmp hang
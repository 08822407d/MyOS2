#include "arch/amd64/asm.h"

.text

ENTRY(start)
    mov     %ss, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    movq    $load_stack_start, %rsp

    call    _C_LABEL(pre_init)

    call    _C_LABEL(kmain)

hang:
	jmp hang

.data
load_stack:
    .fill	512,8,0
load_stack_start:
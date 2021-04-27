#include "arch/amd64/asm.h"
#include "../../include/const.h"

.balign	0x1000

.text
.code16

ENTRY(_APU_boot_start)

_APU_boot_base = .

	cli
	wbinvd

	mov		%cs,		%ax
	mov		%ax,		%ds
	mov		%ax,		%es
	mov		%ax,		%ss
	mov		%ax,		%fs
	mov		%ax,		%gs

#	get base address
	mov		%cs,		%ax
	movzx	%ax,		%esi
	shll	$4,			%esi

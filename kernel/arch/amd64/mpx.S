#include "arch/amd64/asm.h"
#include "include/archconst.h"
#include "include/interrupt.h"

.align 16
/*
 * called by the exception interrupt vectors. If the exception does not push
 * errorcode, we assume that the vector handler pushed 0 instead. Next pushed
 * thing is the vector number. From this point on we can continue as if every
 * exception pushes an error code
 */
R15	=	0x00
R14	=	0x08
R13	=	0x10
R12	=	0x18
R11	=	0x20
R10	=	0x28
R9	=	0x30
R8	=	0x38
RBX	=	0x40
RCX	=	0x48
RDX	=	0x50
RSI	=	0x58
RDI	=	0x60
RBP	=	0x68
DS	=	0x70
ES	=	0x78
RAX	=	0x80
FUNC	=	0x88
ERRCODE	=	0x90
RIP	=	0x98
CS	=	0xa0
RFLAGS	=	0xa8
OLDRSP	=	0xb0
OLDSS	=	0xb8

RESTORE_ALL:
	popq	%r15;		
	popq	%r14;		
	popq	%r13;		
	popq	%r12;		
	popq	%r11;		
	popq	%r10;		
	popq	%r9;		
	popq	%r8;		
	popq	%rbx;		
	popq	%rcx;		
	popq	%rdx;		
	popq	%rsi;		
	popq	%rdi;		
	popq	%rbp;		
	popq	%rax;		
	movq	%rax,	%ds;	
	popq	%rax;		
	movq	%rax,	%es;	
	popq	%rax;		
	addq	$0x10,	%rsp;	
	iretq;

#define GET_CURRENT(reg)	\
	movq	$-32768,reg;	\
	andq	%rsp,	reg

ret_from_exception:
	/*GET_CURRENT(%ebx)	need rewrite*/
ENTRY(ret_from_intr)
	jmp	RESTORE_ALL	/*need rewrite*/

exception_entry:
	pushq	%rax
	movq	%es,	%rax
	pushq	%rax
	movq	%ds,	%rax
	pushq	%rax
	xorq	%rax,	%rax

	pushq	%rbp
	pushq	%rdi
	pushq	%rsi
	pushq	%rdx
	pushq	%rcx
	pushq	%rbx
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15	
	
	cld
	movq	ERRCODE(%rsp),	%rsi
	movq	FUNC(%rsp),	%rdx	

	movq	$0x10,	%rdi
	movq	%rdi,	%ds
	movq	%rdi,	%es

	movq	%rsp,	%rdi
	////GET_CURRENT(%ebx)
	callq 	exception_handler

	jmp	ret_from_exception	

/*===========================================================================*/
/*				exception handlers			     */
/*===========================================================================*/

#define EXCEPTION_ERR_CODE(vector)	\
	pushq	$vector			;\
	jmp	exception_entry

#define EXCEPTION_NO_ERR_CODE(vector)	\
	pushq	$0		;\
	EXCEPTION_ERR_CODE(vector)

LABEL(divide_error)
	EXCEPTION_NO_ERR_CODE(DIVIDE_ERR_VEC)

LABEL(debug)
	EXCEPTION_NO_ERR_CODE(DEBUG_VEC)

LABEL(nmi)
// #ifndef USE_WATCHDOG
// 	EXCEPTION_NO_ERR_CODE(NMI_VEC)
// #else
// 	/*
// 	 * We have to be very careful as this interrupt can occur anytime. On
// 	 * the other hand, if it interrupts a user process, we will resume the
// 	 * same process which makes things a little simpler. We know that we are
// 	 * already on kernel stack whenever it happened and we can be
// 	 * conservative and save everything as we don't need to be extremely
// 	 * efficient as the interrupt is infrequent and some overhead is already
// 	 * expected.
// 	 */

// 	/*
// 	 * save the important registers. We don't save %cs and %ss and they are
// 	 * saved and restored by CPU
// 	 */
// 	pushw	%ds
// 	pushw	%es
// 	pushw	%fs
// 	pushw	%gs
// 	pusha

// 	/*
// 	 * We cannot be sure about the state of the kernel segment register,
// 	 * however, we always set %ds and %es to the same as %ss
// 	 */
// 	mov	%ss, %si
// 	mov	%si, %ds
// 	mov	%si, %es

// 	push	%esp
// 	call	_C_LABEL(nmi_watchdog_handler)
// 	add	$4, %esp

// 	/* restore all the important registers as they were before the trap */
// 	popa
// 	popw	%gs
// 	popw	%fs
// 	popw	%es
// 	popw	%ds

// 	iret
// #endif

LABEL(breakpoint_exception)
	EXCEPTION_NO_ERR_CODE(BREAKPOINT_VEC)

LABEL(overflow)
	EXCEPTION_NO_ERR_CODE(OVERFLOW_VEC)

LABEL(bounds_exceed)
	EXCEPTION_NO_ERR_CODE(BOUNDS_VEC)

LABEL(invalid_opcode)
	EXCEPTION_NO_ERR_CODE(INVAL_OP_VEC)

LABEL(dev_not_available)
// 	TEST_INT_IN_KERNEL(4, copr_not_available_in_kernel)
// 	cld			/* set direction flag to a known value */
// 	SAVE_PROCESS_CTX(0, KTS_INT_HARD)
// 	/* stop user process cycles */
// 	push	%ebp
// 	mov	$0, %ebp
// 	call	_C_LABEL(context_stop)
// 	call	_C_LABEL(copr_not_available_handler)
// 	/* reached upon failure only */
// 	jmp	_C_LABEL(switch_to_user)

// copr_not_available_in_kernel:
// 	pushl	$0
// 	pushl	$DEV_NOT_AVAIL_VEC
// 	jmp	exception_entry_nested

LABEL(double_fault)
	EXCEPTION_ERR_CODE(DOUBLE_FAULT_VEC)

LABEL(coproc_seg_overrun)
	EXCEPTION_NO_ERR_CODE(COPROC_SEG_VEC)

LABEL(invalid_tss)
	EXCEPTION_ERR_CODE(INVAL_TSS_VEC)

LABEL(segment_not_present)
	EXCEPTION_ERR_CODE(SEG_NOT_PRES_VEC)

LABEL(stack_segfault)
	EXCEPTION_ERR_CODE(STACK_SEGFAULT_VEC)

LABEL(general_protection)
	EXCEPTION_ERR_CODE(GEN_PROT_VEC)

LABEL(page_fault)
	EXCEPTION_ERR_CODE(PAGE_FAULT_VEC)

LABEL(x87_fpu_error)
	EXCEPTION_NO_ERR_CODE(X87_FPU_ERR_VEC)

LABEL(alignment_check)
	EXCEPTION_NO_ERR_CODE(ALIGNMENT_CHECK_VEC)

LABEL(machine_check)
	EXCEPTION_NO_ERR_CODE(MACHINE_CHECK_VEC)

LABEL(simd_exception)
	EXCEPTION_NO_ERR_CODE(SIMD_EXCEP_VEC)

LABEL(virtualization_exception)
	EXCEPTION_NO_ERR_CODE(VIRTUAL_EXCEP_VEC)

LABEL(control_protection_exception)
	EXCEPTION_ERR_CODE(CTRL_PROT_EXCEP_VEC)
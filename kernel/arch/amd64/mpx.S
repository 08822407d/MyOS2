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
ENTRY(enter_syscall)
	pushq	%rsp


ENTRY(ret_from_syscall)
	popq	%rax		
	movq	%rax,	%ds
	popq	%rax
	movq	%rax,	%es
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rcx
	popq	%rax
	addq	$0x10,	%rsp	// skip err-code and vec-nr
	
	popq	%rbx
	addq	$0x08,	%rsp	// skip cs
	popq	%r11
	popq	%rsp
	sysretq

RESTORE_ALL:
	popq	%rax		
	movq	%rax,	%ds
	popq	%rax
	movq	%rax,	%es
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax
	addq	$0x10,	%rsp	// skip err-code and vec-nr
	iretq;

#define GET_CURRENT(reg)	\
	movq	$-32768,reg;	\
	andq	%rsp,	reg

ret_from_exception:
	/*GET_CURRENT(%ebx)	need rewrite*/
ENTRY(ret_from_intr)
	jmp	RESTORE_ALL	/*need rewrite*/

SAVE_ALL:
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15	

	movq	%es,	%rax
	pushq	%rax
	movq	%ds,	%rax
	pushq	%rax
	xorq	%rax,	%rax

	cld
	movq	$KERN_SS_SELECTOR,	%rdi
	movq	%rdi,	%ds
	movq	%rdi,	%es

	movq	%rsp,	%rdi
	callq 	excep_intr_c_entry

	jmp		ret_from_exception	

/*===========================================================================*/
/*				exception handlers			     */
/*===========================================================================*/
#define EXCEPTION_ERR_CODE(vector)	\
	pushq	$vector			;\
	jmp		SAVE_ALL

#define EXCEPTION_NO_ERR_CODE(vector)	\
	pushq	$0		;\
	EXCEPTION_ERR_CODE(vector)

LABEL(divide_error)
	EXCEPTION_NO_ERR_CODE(DIVIDE_ERR_VEC)

LABEL(debug)
	EXCEPTION_NO_ERR_CODE(DEBUG_VEC)

LABEL(nmi)
//
//
//

LABEL(breakpoint_exception)
	EXCEPTION_NO_ERR_CODE(BREAKPOINT_VEC)

LABEL(overflow)
	EXCEPTION_NO_ERR_CODE(OVERFLOW_VEC)

LABEL(bounds_exceed)
	EXCEPTION_NO_ERR_CODE(BOUNDS_VEC)

LABEL(invalid_opcode)
	EXCEPTION_NO_ERR_CODE(INVAL_OP_VEC)

LABEL(dev_not_available)
//
//
//

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

/*===========================================================================*/
/*				hwint00 - 07				     */
/*===========================================================================*/
/* Note this is a macro, it just looks like a subroutine. */

#define hwint_master(irq)	\
	nop;		\
	EXCEPTION_NO_ERR_CODE(irq + IRQ0_VEC);


/* Each of these entry points is an expansion of the hwint_master macro */
ENTRY(hwint00)
/* Interrupt routine for irq 0 (the clock). */
	hwint_master(0)

ENTRY(hwint01)
/* Interrupt routine for irq 1 (keyboard) */
	hwint_master(1)

ENTRY(hwint02)
/* Interrupt routine for irq 2 (cascade!) */
	hwint_master(2)

ENTRY(hwint03)
/* Interrupt routine for irq 3 (second serial) */
	hwint_master(3)

ENTRY(hwint04)
/* Interrupt routine for irq 4 (first serial) */
	hwint_master(4)

ENTRY(hwint05)
/* Interrupt routine for irq 5 (XT winchester) */
	hwint_master(5)

ENTRY(hwint06)
/* Interrupt routine for irq 6 (floppy) */
	hwint_master(6)

ENTRY(hwint07)
/* Interrupt routine for irq 7 (printer) */
	hwint_master(7)

/*===========================================================================*/
/*				hwint08 - 15				     */
/*===========================================================================*/
#define hwint_slave(irq)	\
	nop;		\
	EXCEPTION_NO_ERR_CODE(irq + IRQ8_VEC);


/* Each of these entry points is an expansion of the hwint_slave macro */
ENTRY(hwint08)
/* Interrupt routine for irq 8 (realtime clock) */
	hwint_slave(8)

ENTRY(hwint09)
/* Interrupt routine for irq 9 (irq 2 redirected) */
	hwint_slave(9)

ENTRY(hwint10)
/* Interrupt routine for irq 10 */
	hwint_slave(10)

ENTRY(hwint11)
/* Interrupt routine for irq 11 */
	hwint_slave(11)

ENTRY(hwint12)
/* Interrupt routine for irq 12 */
	hwint_slave(12)

ENTRY(hwint13)
/* Interrupt routine for irq 13 (FPU exception) */
	hwint_slave(13)

ENTRY(hwint14)
/* Interrupt routine for irq 14 (AT winchester) */
	hwint_slave(14)

ENTRY(hwint15)
/* Interrupt routine for irq 15 */
	hwint_slave(15)

ENTRY(kernel_thread_func)
	popq	%rax
	movq	%rax,	%ds
	popq	%rax
	movq	%rax,	%es
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax
	addq	$0x38,	%rsp	//skip vec_nr,err_code,rip,cs,rflags,rsp,ss
	/////////////////////////////////
	movq	%rdx,	%rdi
	callq	*%rbx
	movq	%rax,	%rdi
	callq	do_exit
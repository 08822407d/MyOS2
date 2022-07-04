#include <arch/amd64/asm.h>

#include "include/archconst.h"
#include "include/interrupt.h"

.align 16
/*
 * called by the exception interrupt vectors. If the exception does not push
 * errorcode, we assume that the vector handler pushed 0 instead. Next pushed
 * thing is the vector number. From this point on we can continue as if every
 * exception pushes an error code
 */
#define SAVEALL_SIZE		0x70
SAVE_ALL:
	cli
	pushq		%rax
	pushq		%rbx
	pushq		%rcx
	pushq		%rdx
	pushq		%rbp
	pushq		%rdi
	pushq		%rsi
	pushq		%r8
	pushq		%r9
	pushq		%r10
	pushq		%r11
	pushq		%r12
	pushq		%r13
	pushq		%r14
	pushq		%r15	
	subq		$0x08,		%rsp
	retq

RESTORE_ALL:
	cli
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
	popq		%r11
	popq		%r10
	popq		%r9
	popq		%r8
	popq		%rsi
	popq		%rdi
	popq		%rbp
	popq		%rdx
	popq		%rcx
	popq		%rbx
	popq		%rax
	retq

#define SAVEALL_RETP		(SAVEALL_SIZE + 0x08)
// set the ret_point when return from SAVE_ALL and RESTORE_ALL
#define	SET_RET_POINTS(ent_rp, ex_rp)				\
	subq		$0x08,			%RSP				;\
	pushq		%rax								;\
	leaq		ex_rp(%rip),	%rax				;\
	movq		%rax,			0x08(%rsp)			;\
	leaq		ent_rp(%rip),	%rax				;\
	movq		%rax,			-SAVEALL_RETP(%rsp)	;\
	popq		%rax								;\

/*==============================================================================================*/
/*									kernel thread func							     			*/
/*==============================================================================================*/
ENTRY(entp_kernel_thread)
	jmp			RESTORE_ALL
ENTRY(ra_kthd_retp)
	addq		$0x38,		%rsp	//skip vec_nr,err_code,rip,cs,rflags,rsp,ss
	/////////////////////////////////
	movq		%rdx,		%rdi
	sti
	pushq		%rbx
	callq		*%rbx
	popq		%rbx
	movq		%rax,		%rdi
	movq		%rbx,		%rsi
	sti
	callq		do_exit

/*==============================================================================================*/
/*									syscenter and sysexit entry						 			*/
/*==============================================================================================*/
// user RSP -> R11 by hand
// user RIP -> R10 by hand
// kernel CS = IA32_SYSENTER_CS
// kernel SS = IA32_SYSENTER_CS + 8
// kernel RIP = IA32_SYSENTER_EIP
// kernel RSP = IA32_SYSENTER_ESP
ENTRY(sysenter_entp)
	subq		$0x38,		%rsp
	SET_RET_POINTS(sa_sysent_retp, ra_sysex_retp)
	jmp			SAVE_ALL
sa_sysent_retp:
	leaq		syscall_table(%rip),	%rbx
	callq		*(%rbx,%rax,8)
	movq		%rax,					SAVEALL_SIZE(%rsp)	// save return value to stack

// RDX -> user RSP
// RCX -> user RIP
// user CS = IA32_SYSENTER_CS + 32
// user SS = IA32_SYSENTER_CS + 40
ENTRY(sysexit_entp)
	jmp			RESTORE_ALL
ENTRY(ra_sysex_retp)
	addq		$0x38,		%rsp
	xchgq		%r10,		%rdx
	xchgq		%r11,		%rcx
	// swapgs
	sti
	.byte	0x48
	sysexitq

// /*==============================================================================================*/
// /*									syscall and sysret entry						 			*/
// /*==============================================================================================*/
// // user Rflags -> R11
// // user RIP -> RCX
// // kernel CS = IA32_STAR[47:32]
// // kernel SS = IA32_STAR[47:32] + 8
// // kernel RIP = IA32_LSTAR
// // kernel Rflags &= IA32_FMASK
// // !!! user RSP should be store by hand !!!
// // !!! kernel RSP should be load by hand !!!
// ENTRY(enter_syscall)
// 	cli
// 	swapgs
// 	movabsq		$cpustack_off,	%r10
// 	movq		(%r10),		%r10
// 	movq		%gs:(%r10),	%r10
// 	xchgq		%rsp,		%r10
// 	pushq		%r10				// save user rsp
// 	pushq		%r11				// save user rflags
// 	subq		$0x08,		%rsp
// 	pushq		%rcx
// 	/////////////////////////////////
// 	subq		$0x10,		%rsp	// skip vec_nr and err_code
// 	/////////////////////////////////

// 	movq		%rsp,		%rdi

// // R11 -> user Rflags
// // RCX -> user RIP
// // user CS = IA32_STAR[63:48] + 16
// // user SS = IA32_STAR[63:48] + 8
// // !!! user RSP should be restore by hand !!!
// // !!! kernel RSP should be store by hand !!!
// ENTRY(ret_from_syscall)

// 	/////////////////////////////////
// 	addq		$0x10,		%rsp	// skip err-code and vec-nr
// 	/////////////////////////////////
// 	// now user rip already in rcx, rflags in r11
// 	popq		%rcx
// 	addq		$0x10,		%rsp	// skip cs
// 	popq		%rsp
// 	swapgs
// 	sti

// 	sysretq

/*==============================================================================================*/
/*										interrupt entry								 			*/
/*==============================================================================================*/
sa_entintr_retp:
	cld
	movq		%rsp,		%rdi
	callq 		excep_hwint_entry
	callq		try_sched
	// sti
	// movq		$-1,		%rcx
	// testq		softirq_status(%rip),	%rcx	////check softirq	
	// callq		do_softirq
	// cli

	jmp			RESTORE_ALL
ra_exintr_retp:
	addq		$0x10,		%rsp	// skip err-code and vec-nr
	sti
	iretq;

/*==============================================================================================*/
/*									exception handler entries					     			*/
/*==============================================================================================*/
#define EXCEPTION_ERR_CODE(vector)				\
	cli											;\
	pushq		$vector							;\
	SET_RET_POINTS(sa_entintr_retp, ra_exintr_retp)	;\
	jmp			SAVE_ALL

#define EXCEPTION_NO_ERR_CODE(vector)	\
	pushq		$0						;\
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

LABEL(cotask_seg_overrun)
	EXCEPTION_NO_ERR_CODE(COtask_sEG_VEC)

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

/*==============================================================================================*/
/*										hwint handler entries						     		*/
/*==============================================================================================*/
#define hwint_entry(irq)	\
	EXCEPTION_NO_ERR_CODE(irq + HWINT0_VEC);

ENTRY(hwint00) /* Interrupt routine for irq 0 (the clock). */
	hwint_entry(0)

ENTRY(hwint01) /* Interrupt routine for irq 1 (keyboard) */
	hwint_entry(1)

ENTRY(hwint02) /* Interrupt routine for irq 2 (cascade!) */
	hwint_entry(2)

ENTRY(hwint03) /* Interrupt routine for irq 3 (second serial) */
	hwint_entry(3)

ENTRY(hwint04) /* Interrupt routine for irq 4 (first serial) */
	hwint_entry(4)

ENTRY(hwint05) /* Interrupt routine for irq 5 (XT winchester) */
	hwint_entry(5)

ENTRY(hwint06) /* Interrupt routine for irq 6 (floppy) */
	hwint_entry(6)

ENTRY(hwint07) /* Interrupt routine for irq 7 (printer) */
	hwint_entry(7)

ENTRY(hwint08) /* Interrupt routine for irq 8 (realtime clock) */
	hwint_entry(8)

ENTRY(hwint09) /* Interrupt routine for irq 9 (irq 2 redirected) */
	hwint_entry(9)

ENTRY(hwint10) /* Interrupt routine for irq 10 */
	hwint_entry(10)

ENTRY(hwint11) /* Interrupt routine for irq 11 */
	hwint_entry(11)

ENTRY(hwint12) /* Interrupt routine for irq 12 */
	hwint_entry(12)

ENTRY(hwint13) /* Interrupt routine for irq 13 (FPU exception) */
	hwint_entry(13)

ENTRY(hwint14) /* Interrupt routine for irq 14 (AT winchester) */
	hwint_entry(14)

ENTRY(hwint15) /* Interrupt routine for irq 15 */
	hwint_entry(15)

ENTRY(hwint16) /* APIC_IRQ */
	hwint_entry(16)

ENTRY(hwint17) /* APIC_IRQ */
	hwint_entry(17)

ENTRY(hwint18) /* APIC_IRQ */
	hwint_entry(18)

ENTRY(hwint19) /* APIC_IRQ */
	hwint_entry(19)

ENTRY(hwint20) /* APIC_IRQ */
	hwint_entry(20)

ENTRY(hwint21) /* APIC_IRQ */
	hwint_entry(21)

ENTRY(hwint22) /* APIC_IRQ */
	hwint_entry(22)

ENTRY(hwint23) /* APIC_IRQ */
	hwint_entry(23)

/*==============================================================================================*/
/*										hwint handler entries						     		*/
/*==============================================================================================*/
#define lapic_ipi_entry(irq)	\
	EXCEPTION_NO_ERR_CODE(irq + APIC_IPI0_VEC);

ENTRY(lapic_ipi00) /* IPI routine for irq 0 (LVT CMCI). */
	lapic_ipi_entry(0)

ENTRY(lapic_ipi01) /* IPI routine for irq 1 (ICR). */
	lapic_ipi_entry(1)

ENTRY(lapic_ipi02) /* IPI routine for irq 1 (LVT timer) */
	lapic_ipi_entry(2)

ENTRY(lapic_ipi03) /* IPI routine for irq 2 (LVT thermal sensor) */
	lapic_ipi_entry(3)

ENTRY(lapic_ipi04) /* IPI routine for irq 3 (LVT performance counter) */
	lapic_ipi_entry(4)

ENTRY(lapic_ipi05) /* IPI routine for irq 4 (LVT lint0) */
	lapic_ipi_entry(5)

ENTRY(lapic_ipi06) /* IPI routine for irq 5 (LVT lint1) */
	lapic_ipi_entry(6)

ENTRY(lapic_ipi07) /* IPI routine for irq 6 (LVT error) */
	lapic_ipi_entry(7)

#include <linux/kernel/linkage.h>

#include <obsolete/archconst.h>
#include <obsolete/interrupt.h>

.align 16
/*
 * called by the exception interrupt vectors. If the exception does not push
 * errorcode, we assume that the vector handler pushed 0 instead. Next pushed
 * thing is the vector number. From this point on we can continue as if every
 * exception pushes an error code
 */
#define SAVEALL_SIZE		0x70

.macro PUSH_REGS
	pushq		%rdi
	pushq		%rsi
	pushq		%rdx
	pushq		%rcx
	pushq		%rax
	pushq		%r8
	pushq		%r9
	pushq		%r10
	pushq		%r11
	pushq		%rbx
	pushq		%rbp
	pushq		%r12
	pushq		%r13
	pushq		%r14
	pushq		%r15	
.endm

.macro POP_REGS
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
	popq		%rbp
	popq		%rbx
	popq		%r11
	popq		%r10
	popq		%r9
	popq		%r8
	popq		%rax
	popq		%rcx
	popq		%rdx
	popq		%rsi
	popq		%rdi
.endm

/*==============================================================================================*/
/*									syscenter and sysexit entry						 			*/
/*==============================================================================================*/
// user RSP -> R11 by hand
// user RIP -> R10 by hand
// kernel CS = IA32_SYSENTER_CS
// kernel SS = IA32_SYSENTER_CS + 8
// kernel RIP = IA32_SYSENTER_EIP
// kernel RSP = IA32_SYSENTER_ESP
SYM_CODE_START(sysenter_entp)
	cli
	subq		$0x38,		%rsp
	PUSH_REGS
	sti

	leaq		sys_call_table(%rip),	%rbx
	callq		*(%rbx,%rax,8)
	movq		%rax,		(SAVEALL_SIZE - 0x20)(%rsp)	// save return value to stack
// RDX -> user RSP
// RCX -> user RIP
// user CS = IA32_SYSENTER_CS + 32
// user SS = IA32_SYSENTER_CS + 40
SYM_CODE_START(sysexit_entp)
	cli
	POP_REGS
	addq		$0x38,		%rsp
	xchgq		%r10,		%rdx
	xchgq		%r11,		%rcx
	// swapgs
	sti

	.byte	0x48
	sysexit

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
// SYM_CODE_START(enter_syscall)
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
// SYM_CODE_START(ret_from_syscall)

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

	callq 		excep_hwint_context

	POP_REGS
	addq		$0x10,		%rsp	// skip err-code and vec-nr
	sti
	iretq;

/*==============================================================================================*/
/*									exception handler entries					     			*/
/*==============================================================================================*/
#define EXCEPTION_ERR_CODE(vector)				\
	cli											;\
	pushq		$vector							;\
	PUSH_REGS									;\
	jmp			sa_entintr_retp

#define EXCEPTION_NO_ERR_CODE(vector)	\
	cli									;\
	pushq		$0						;\
	EXCEPTION_ERR_CODE(vector)

SYM_CODE_START(divide_error)
	EXCEPTION_NO_ERR_CODE(DIVIDE_ERR_VEC)

SYM_CODE_START(debug)
	EXCEPTION_NO_ERR_CODE(DEBUG_VEC)

SYM_CODE_START(nmi)
//
//
//

SYM_CODE_START(breakpoint_exception)
	EXCEPTION_NO_ERR_CODE(BREAKPOINT_VEC)

SYM_CODE_START(overflow)
	EXCEPTION_NO_ERR_CODE(OVERFLOW_VEC)

SYM_CODE_START(bounds_exceed)
	EXCEPTION_NO_ERR_CODE(BOUNDS_VEC)

SYM_CODE_START(invalid_opcode)
	EXCEPTION_NO_ERR_CODE(INVAL_OP_VEC)

SYM_CODE_START(dev_not_available)
//
//
//

SYM_CODE_START(double_fault)
	EXCEPTION_ERR_CODE(DOUBLE_FAULT_VEC)

SYM_CODE_START(cotask_seg_overrun)
	EXCEPTION_NO_ERR_CODE(COtask_sEG_VEC)

SYM_CODE_START(invalid_tss)
	EXCEPTION_ERR_CODE(INVAL_TSS_VEC)

SYM_CODE_START(segment_not_present)
	EXCEPTION_ERR_CODE(SEG_NOT_PRES_VEC)

SYM_CODE_START(stack_segfault)
	EXCEPTION_ERR_CODE(STACK_SEGFAULT_VEC)

SYM_CODE_START(general_protection)
	EXCEPTION_ERR_CODE(GEN_PROT_VEC)

SYM_CODE_START(page_fault)
	EXCEPTION_ERR_CODE(PAGE_FAULT_VEC)

SYM_CODE_START(x87_fpu_error)
	EXCEPTION_NO_ERR_CODE(X87_FPU_ERR_VEC)

SYM_CODE_START(alignment_check)
	EXCEPTION_NO_ERR_CODE(ALIGNMENT_CHECK_VEC)

SYM_CODE_START(machine_check)
	EXCEPTION_NO_ERR_CODE(MACHINE_CHECK_VEC)

SYM_CODE_START(simd_exception)
	EXCEPTION_NO_ERR_CODE(SIMD_EXCEP_VEC)

SYM_CODE_START(virtualization_exception)
	EXCEPTION_NO_ERR_CODE(VIRTUAL_EXCEP_VEC)

SYM_CODE_START(control_protection_exception)
	EXCEPTION_ERR_CODE(CTRL_PROT_EXCEP_VEC)

/*==============================================================================================*/
/*										hwint handler entries						     		*/
/*==============================================================================================*/
#define hwint_entry(irq)	\
	EXCEPTION_NO_ERR_CODE(irq + HWINT0_VEC);

SYM_CODE_START(hwint00) /* Interrupt routine for irq 0 (the clock). */
	hwint_entry(0)

SYM_CODE_START(hwint01) /* Interrupt routine for irq 1 (keyboard) */
	hwint_entry(1)

SYM_CODE_START(hwint02) /* Interrupt routine for irq 2 (cascade!) */
	hwint_entry(2)

SYM_CODE_START(hwint03) /* Interrupt routine for irq 3 (second serial) */
	hwint_entry(3)

SYM_CODE_START(hwint04) /* Interrupt routine for irq 4 (first serial) */
	hwint_entry(4)

SYM_CODE_START(hwint05) /* Interrupt routine for irq 5 (XT winchester) */
	hwint_entry(5)

SYM_CODE_START(hwint06) /* Interrupt routine for irq 6 (floppy) */
	hwint_entry(6)

SYM_CODE_START(hwint07) /* Interrupt routine for irq 7 (printer) */
	hwint_entry(7)

SYM_CODE_START(hwint08) /* Interrupt routine for irq 8 (realtime clock) */
	hwint_entry(8)

SYM_CODE_START(hwint09) /* Interrupt routine for irq 9 (irq 2 redirected) */
	hwint_entry(9)

SYM_CODE_START(hwint10) /* Interrupt routine for irq 10 */
	hwint_entry(10)

SYM_CODE_START(hwint11) /* Interrupt routine for irq 11 */
	hwint_entry(11)

SYM_CODE_START(hwint12) /* Interrupt routine for irq 12 */
	hwint_entry(12)

SYM_CODE_START(hwint13) /* Interrupt routine for irq 13 (FPU exception) */
	hwint_entry(13)

SYM_CODE_START(hwint14) /* Interrupt routine for irq 14 (AT winchester) */
	hwint_entry(14)

SYM_CODE_START(hwint15) /* Interrupt routine for irq 15 */
	hwint_entry(15)

SYM_CODE_START(hwint16) /* APIC_IRQ */
	hwint_entry(16)

SYM_CODE_START(hwint17) /* APIC_IRQ */
	hwint_entry(17)

SYM_CODE_START(hwint18) /* APIC_IRQ */
	hwint_entry(18)

SYM_CODE_START(hwint19) /* APIC_IRQ */
	hwint_entry(19)

SYM_CODE_START(hwint20) /* APIC_IRQ */
	hwint_entry(20)

SYM_CODE_START(hwint21) /* APIC_IRQ */
	hwint_entry(21)

SYM_CODE_START(hwint22) /* APIC_IRQ */
	hwint_entry(22)

SYM_CODE_START(hwint23) /* APIC_IRQ */
	hwint_entry(23)

/*==============================================================================================*/
/*										hwint handler entries						     		*/
/*==============================================================================================*/
#define lapic_ipi_entry(irq)	\
	EXCEPTION_NO_ERR_CODE(irq + APIC_IPI0_VEC);

SYM_CODE_START(lapic_ipi00) /* IPI routine for irq 0 (LVT CMCI). */
	lapic_ipi_entry(0)

SYM_CODE_START(lapic_ipi01) /* IPI routine for irq 1 (ICR). */
	lapic_ipi_entry(1)

SYM_CODE_START(lapic_ipi02) /* IPI routine for irq 1 (LVT timer) */
	lapic_ipi_entry(2)

SYM_CODE_START(lapic_ipi03) /* IPI routine for irq 2 (LVT thermal sensor) */
	lapic_ipi_entry(3)

SYM_CODE_START(lapic_ipi04) /* IPI routine for irq 3 (LVT performance counter) */
	lapic_ipi_entry(4)

SYM_CODE_START(lapic_ipi05) /* IPI routine for irq 4 (LVT lint0) */
	lapic_ipi_entry(5)

SYM_CODE_START(lapic_ipi06) /* IPI routine for irq 5 (LVT lint1) */
	lapic_ipi_entry(6)

SYM_CODE_START(lapic_ipi07) /* IPI routine for irq 6 (LVT error) */
	lapic_ipi_entry(7)


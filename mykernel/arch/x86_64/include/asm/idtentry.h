// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_IDTENTRY_H
#define _ASM_X86_IDTENTRY_H

	/* Interrupts/Exceptions */
	#include <asm/trapnr.h>
	#include <asm/irq_vectors.h>

	#define HAS_KERNEL_IBT	0
	#define IDT_ALIGN	(8 * (2 + HAS_KERNEL_IBT))

	#ifndef __ASSEMBLY__
	// #	include <linux/entry-common.h>
	// #	include <linux/hardirq.h>

	// #	include <asm/irq_stack.h>

	// 	/**
	// 	 * DECLARE_IDTENTRY - Declare functions for simple IDT entry points
	// 	 *		      No error code pushed by hardware
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Declares three functions:
	// 	 * - The ASM entry point: asm_##func
	// 	 * - The XEN PV trap entry point: xen_##func (maybe unused)
	// 	 * - The C handler called from the ASM entry point
	// 	 *
	// 	 * Note: This is the C variant of DECLARE_IDTENTRY(). As the name says it
	// 	 * declares the entry points for usage in C code. There is an ASM variant
	// 	 * as well which is used to emit the entry stubs in entry_32/64.S.
	// 	 */
	// #	define DECLARE_IDTENTRY(vector, func)					\
	// 		asmlinkage void asm_##func(void);				\
	// 		asmlinkage void xen_asm_##func(void);				\
	// 		__visible void func(pt_regs_s *regs)

	// 	/**
	// 	 * DEFINE_IDTENTRY - Emit code for simple IDT entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * @func is called from ASM entry code with interrupts disabled.
	// 	 *
	// 	 * The macro is written so it acts as function definition. Append the
	// 	 * body with a pair of curly brackets.
	// 	 *
	// 	 * irqentry_enter() contains common code which has to be invoked before
	// 	 * arbitrary code in the body. irqentry_exit() contains common code
	// 	 * which has to run before returning to the low level assembly code.
	// 	 */
	// #	define DEFINE_IDTENTRY(func)						\
	// 	static __always_inline void __##func(pt_regs_s *regs);		\
	// 										\
	// 	__visible noinstr void func(pt_regs_s *regs)			\
	// 	{									\
	// 		irqentry_state_t state = irqentry_enter(regs);			\
	// 										\
	// 		instrumentation_begin();					\
	// 		__##func (regs);						\
	// 		instrumentation_end();						\
	// 		irqentry_exit(regs, state);					\
	// 	}									\
	// 										\
	// 	static __always_inline void __##func(pt_regs_s *regs)

	// 	/* Special case for 32bit IRET 'trap' */
	// #	define DECLARE_IDTENTRY_SW	DECLARE_IDTENTRY
	// #	define DEFINE_IDTENTRY_SW	DEFINE_IDTENTRY

	// 	/**
	// 	 * DECLARE_IDTENTRY_ERRORCODE - Declare functions for simple IDT entry points
	// 	 *				Error code pushed by hardware
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Declares three functions:
	// 	 * - The ASM entry point: asm_##func
	// 	 * - The XEN PV trap entry point: xen_##func (maybe unused)
	// 	 * - The C handler called from the ASM entry point
	// 	 *
	// 	 * Same as DECLARE_IDTENTRY, but has an extra error_code argument for the
	// 	 * C-handler.
	// 	 */
	// #	define DECLARE_IDTENTRY_ERRORCODE(vector, func)			\
	// 		asmlinkage void asm_##func(void);				\
	// 		asmlinkage void xen_asm_##func(void);				\
	// 		__visible void func(pt_regs_s *regs, unsigned long error_code)

	// 	/**
	// 	 * DEFINE_IDTENTRY_ERRORCODE - Emit code for simple IDT entry points
	// 	 *			       Error code pushed by hardware
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Same as DEFINE_IDTENTRY, but has an extra error_code argument
	// 	 */
	// #	define DEFINE_IDTENTRY_ERRORCODE(func)					\
	// 	static __always_inline void __##func(pt_regs_s *regs,		\
	// 						unsigned long error_code);		\
	// 										\
	// 	__visible noinstr void func(pt_regs_s *regs,			\
	// 					unsigned long error_code)			\
	// 	{									\
	// 		irqentry_state_t state = irqentry_enter(regs);			\
	// 										\
	// 		instrumentation_begin();					\
	// 		__##func (regs, error_code);					\
	// 		instrumentation_end();						\
	// 		irqentry_exit(regs, state);					\
	// 	}									\
	// 										\
	// 	static __always_inline void __##func(pt_regs_s *regs,		\
	// 						unsigned long error_code)

	// 	/**
	// 	 * DECLARE_IDTENTRY_RAW - Declare functions for raw IDT entry points
	// 	 *		      No error code pushed by hardware
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY().
	// 	 */
	// #	define DECLARE_IDTENTRY_RAW(vector, func)				\
	// 		DECLARE_IDTENTRY(vector, func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_RAW - Emit code for raw IDT entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * @func is called from ASM entry code with interrupts disabled.
	// 	 *
	// 	 * The macro is written so it acts as function definition. Append the
	// 	 * body with a pair of curly brackets.
	// 	 *
	// 	 * Contrary to DEFINE_IDTENTRY() this does not invoke the
	// 	 * idtentry_enter/exit() helpers before and after the body invocation. This
	// 	 * needs to be done in the body itself if applicable. Use if extra work
	// 	 * is required before the enter/exit() helpers are invoked.
	// 	 */
	// #	define DEFINE_IDTENTRY_RAW(func)					\
	// 	__visible noinstr void func(pt_regs_s *regs)

	// 	/**
	// 	 * DECLARE_IDTENTRY_RAW_ERRORCODE - Declare functions for raw IDT entry points
	// 	 *				    Error code pushed by hardware
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY_ERRORCODE()
	// 	 */
	// #	define DECLARE_IDTENTRY_RAW_ERRORCODE(vector, func)			\
	// 		DECLARE_IDTENTRY_ERRORCODE(vector, func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_RAW_ERRORCODE - Emit code for raw IDT entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * @func is called from ASM entry code with interrupts disabled.
	// 	 *
	// 	 * The macro is written so it acts as function definition. Append the
	// 	 * body with a pair of curly brackets.
	// 	 *
	// 	 * Contrary to DEFINE_IDTENTRY_ERRORCODE() this does not invoke the
	// 	 * irqentry_enter/exit() helpers before and after the body invocation. This
	// 	 * needs to be done in the body itself if applicable. Use if extra work
	// 	 * is required before the enter/exit() helpers are invoked.
	// 	 */
	// #	define DEFINE_IDTENTRY_RAW_ERRORCODE(func)				\
	// 	__visible noinstr void func(pt_regs_s *regs, unsigned long error_code)

	// 	/**
	// 	 * DECLARE_IDTENTRY_IRQ - Declare functions for device interrupt IDT entry
	// 	 *			  points (common/spurious)
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY_ERRORCODE()
	// 	 */
	// #	define DECLARE_IDTENTRY_IRQ(vector, func)				\
	// 		DECLARE_IDTENTRY_ERRORCODE(vector, func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_IRQ - Emit code for device interrupt IDT entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * The vector number is pushed by the low level entry stub and handed
	// 	 * to the function as error_code argument which needs to be truncated
	// 	 * to an u8 because the push is sign extending.
	// 	 *
	// 	 * irq_enter/exit_rcu() are invoked before the function body and the
	// 	 * KVM L1D flush request is set. Stack switching to the interrupt stack
	// 	 * has to be done in the function body if necessary.
	// 	 */
	// #	define DEFINE_IDTENTRY_IRQ(func)					\
	// 	static void __##func(pt_regs_s *regs, u32 vector);			\
	// 										\
	// 	__visible noinstr void func(pt_regs_s *regs,			\
	// 					unsigned long error_code)			\
	// 	{									\
	// 		irqentry_state_t state = irqentry_enter(regs);			\
	// 		u32 vector = (u32)(u8)error_code;				\
	// 										\
	// 		instrumentation_begin();					\
	// 		kvm_set_cpu_l1tf_flush_l1d();					\
	// 		run_irq_on_irqstack_cond(__##func, regs, vector);		\
	// 		instrumentation_end();						\
	// 		irqentry_exit(regs, state);					\
	// 	}									\
	// 										\
	// 	static noinline void __##func(pt_regs_s *regs, u32 vector)

	// 	/**
	// 	 * DECLARE_IDTENTRY_SYSVEC - Declare functions for system vector entry points
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Declares three functions:
	// 	 * - The ASM entry point: asm_##func
	// 	 * - The XEN PV trap entry point: xen_##func (maybe unused)
	// 	 * - The C handler called from the ASM entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY().
	// 	 */
	// #	define DECLARE_IDTENTRY_SYSVEC(vector, func)				\
	// 		DECLARE_IDTENTRY(vector, func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_SYSVEC - Emit code for system vector IDT entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * irqentry_enter/exit() and irq_enter/exit_rcu() are invoked before the
	// 	 * function body. KVM L1D flush request is set.
	// 	 *
	// 	 * Runs the function on the interrupt stack if the entry hit kernel mode
	// 	 */
	// #	define DEFINE_IDTENTRY_SYSVEC(func)					\
	// 	static void __##func(pt_regs_s *regs);				\
	// 										\
	// 	__visible noinstr void func(pt_regs_s *regs)			\
	// 	{									\
	// 		irqentry_state_t state = irqentry_enter(regs);			\
	// 										\
	// 		instrumentation_begin();					\
	// 		kvm_set_cpu_l1tf_flush_l1d();					\
	// 		run_sysvec_on_irqstack_cond(__##func, regs);			\
	// 		instrumentation_end();						\
	// 		irqentry_exit(regs, state);					\
	// 	}									\
	// 										\
	// 	static noinline void __##func(pt_regs_s *regs)

	// 	/**
	// 	 * DEFINE_IDTENTRY_SYSVEC_SIMPLE - Emit code for simple system vector IDT
	// 	 *				   entry points
	// 	* @func:	Function name of the entry point
	// 	*
	// 	* Runs the function on the interrupted stack. No switch to IRQ stack and
	// 	* only the minimal __irq_enter/exit() handling.
	// 	*
	// 	* Only use for 'empty' vectors like reschedule IPI and KVM posted
	// 	* interrupt vectors.
	// 	*/
	// #	define DEFINE_IDTENTRY_SYSVEC_SIMPLE(func)				\
	// 	static __always_inline void __##func(pt_regs_s *regs);		\
	// 										\
	// 	__visible noinstr void func(pt_regs_s *regs)			\
	// 	{									\
	// 		irqentry_state_t state = irqentry_enter(regs);			\
	// 										\
	// 		instrumentation_begin();					\
	// 		__irq_enter_raw();						\
	// 		kvm_set_cpu_l1tf_flush_l1d();					\
	// 		__##func (regs);						\
	// 		__irq_exit_raw();						\
	// 		instrumentation_end();						\
	// 		irqentry_exit(regs, state);					\
	// 	}									\
	// 										\
	// 	static __always_inline void __##func(pt_regs_s *regs)

	// 	/**
	// 	 * DECLARE_IDTENTRY_XENCB - Declare functions for XEN HV callback entry point
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Declares three functions:
	// 	 * - The ASM entry point: asm_##func
	// 	 * - The XEN PV trap entry point: xen_##func (maybe unused)
	// 	 * - The C handler called from the ASM entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY(). Distinct entry point to handle the 32/64-bit
	// 	 * difference
	// 	 */
	// #	define DECLARE_IDTENTRY_XENCB(vector, func)				\
	// 		DECLARE_IDTENTRY(vector, func)

	// 	/**
	// 	 * DECLARE_IDTENTRY_IST - Declare functions for IST handling IDT entry points
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY_RAW, but declares also the NOIST C handler
	// 	 * which is called from the ASM entry point on user mode entry
	// 	 */
	// #	define DECLARE_IDTENTRY_IST(vector, func)				\
	// 		DECLARE_IDTENTRY_RAW(vector, func);				\
	// 		__visible void noist_##func(pt_regs_s *regs)

	// 	/**
	// 	 * DECLARE_IDTENTRY_VC - Declare functions for the VC entry point
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY_RAW_ERRORCODE, but declares also the
	// 	 * safe_stack C handler.
	// 	 */
	// #	define DECLARE_IDTENTRY_VC(vector, func)				\
	// 		DECLARE_IDTENTRY_RAW_ERRORCODE(vector, func);			\
	// 		__visible noinstr void kernel_##func(pt_regs_s *regs, unsigned long error_code);	\
	// 		__visible noinstr void   user_##func(pt_regs_s *regs, unsigned long error_code)

	// 	/**
	// 	 * DEFINE_IDTENTRY_IST - Emit code for IST entry points
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DEFINE_IDTENTRY_RAW
	// 	 */
	// #	define DEFINE_IDTENTRY_IST(func)					\
	// 		DEFINE_IDTENTRY_RAW(func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_NOIST - Emit code for NOIST entry points which
	// 	 *			   belong to a IST entry point (MCE, DB)
	// 	* @func:	Function name of the entry point. Must be the same as
	// 	*		the function name of the corresponding IST variant
	// 	*
	// 	* Maps to DEFINE_IDTENTRY_RAW().
	// 	*/
	// #	define DEFINE_IDTENTRY_NOIST(func)					\
	// 		DEFINE_IDTENTRY_RAW(noist_##func)

	// 	/**
	// 	 * DECLARE_IDTENTRY_DF - Declare functions for double fault
	// 	 * @vector:	Vector number (ignored for C)
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DECLARE_IDTENTRY_RAW_ERRORCODE
	// 	 */
	// #	define DECLARE_IDTENTRY_DF(vector, func)				\
	// 		DECLARE_IDTENTRY_RAW_ERRORCODE(vector, func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_DF - Emit code for double fault
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DEFINE_IDTENTRY_RAW_ERRORCODE
	// 	 */
	// #	define DEFINE_IDTENTRY_DF(func)					\
	// 		DEFINE_IDTENTRY_RAW_ERRORCODE(func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_VC_KERNEL - Emit code for VMM communication handler
	// 					 when raised from kernel mode
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DEFINE_IDTENTRY_RAW_ERRORCODE
	// 	 */
	// #	define DEFINE_IDTENTRY_VC_KERNEL(func)				\
	// 		DEFINE_IDTENTRY_RAW_ERRORCODE(kernel_##func)

	// 	/**
	// 	 * DEFINE_IDTENTRY_VC_USER - Emit code for VMM communication handler
	// 					 when raised from user mode
	// 	 * @func:	Function name of the entry point
	// 	 *
	// 	 * Maps to DEFINE_IDTENTRY_RAW_ERRORCODE
	// 	 */
	// #	define DEFINE_IDTENTRY_VC_USER(func)				\
	// 		DEFINE_IDTENTRY_RAW_ERRORCODE(user_##func)

	// 	/* C-Code mapping */
	// #	define DECLARE_IDTENTRY_NMI		DECLARE_IDTENTRY_RAW
	// #	define DEFINE_IDTENTRY_NMI		DEFINE_IDTENTRY_RAW

	// #	define DECLARE_IDTENTRY_MCE		DECLARE_IDTENTRY_IST
	// #	define DEFINE_IDTENTRY_MCE		DEFINE_IDTENTRY_IST
	// #	define DEFINE_IDTENTRY_MCE_USER	DEFINE_IDTENTRY_NOIST

	// #	define DECLARE_IDTENTRY_DEBUG		DECLARE_IDTENTRY_IST
	// #	define DEFINE_IDTENTRY_DEBUG		DEFINE_IDTENTRY_IST
	// #	define DEFINE_IDTENTRY_DEBUG_USER	DEFINE_IDTENTRY_NOIST

	#include <asm/ptrace.h>
	extern void exc_page_fault(pt_regs_s *regs, unsigned long error_code);

	#endif /* __ASSEMBLY__ */

#endif

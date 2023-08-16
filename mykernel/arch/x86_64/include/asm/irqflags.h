// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _X86_IRQFLAGS_H_
#define _X86_IRQFLAGS_H_

	#include <asm/processor-flags.h>

	#ifndef __ASSEMBLY__

	// #	include <asm/nospec-branch.h>

		/*
		 * Interrupt control:
		 */

		/* Declaration required for gcc < 4.9 to prevent -Werror=missing-prototypes */
		// extern inline unsigned long native_save_fl(void);
		// static __always_inline unsigned long native_save_fl(void) {
		static __always_inline unsigned long arch_save_fl(void) {
			unsigned long flags;
			/*
			 * "=rm" is safe here, because "pop" adjusts the stack before
			 * it evaluates its effective address -- this is part of the
			 * documented behavior of the "pop" instruction.
			 */
			asm volatile(	"pushfq				\n\t"
							"popq		%0		\n\t"
						:	"=rm"(flags)
						:	/* no input */
						:	"memory"
						);
			return flags;
		}

		// static __always_inline void native_irq_disable(void) {
		static __always_inline void arch_irq_disable(void) {
			asm volatile(	"cli				\t\n"
						:
						:
						:	"memory"
						);
		}

		// static __always_inline void native_irq_enable(void) {
		static __always_inline void arch_irq_enable(void) {
			asm volatile(	"sti				\t\n"
						:
						:
						:	"memory"
						);
		}

		// static inline void native_safe_halt(void) {
		static inline void arch_safe_halt(void) {
			// mds_idle_clear_cpu_buffers();
			asm volatile(	"sti				\t\n"
							"hlt				\t\n"
						:
						:
						:	"memory"
						);
		}

		// static inline void native_halt(void) {
		static inline void arch_halt(void) {
			// mds_idle_clear_cpu_buffers();
			asm volatile(	"hlt				\t\n"
						:
						:
						:	"memory"
						);
		}

	#endif

	// #ifdef CONFIG_PARAVIRT_XXL
	// #include <asm/paravirt.h>
	// #else
	#ifndef __ASSEMBLY__
	#	include <linux/kernel/types.h>

	// static __always_inline unsigned long
	// arch_local_save_flags(void) {
	// 	return arch_save_fl();
	// }

	// static __always_inline void
	// arch_local_irq_disable(void) {
	// 	arch_irq_disable();
	// }

	// static __always_inline void
	// arch_local_irq_enable(void) {
	// 	arch_irq_enable();
	// }

	// /*
	//  * Used in the idle loop; sti takes one instruction cycle
	//  * to complete:
	//  */
	// static inline void arch_safe_halt(void) {
	// 	arch_safe_halt();
	// }

	// /*
	//  * Used when interrupts are already enabled or to
	//  * shutdown the processor:
	//  */
	// static inline void halt(void) {
	// 	arch_halt();
	// }

	/*
	 * For spinlocks, etc:
	 */
	static __always_inline unsigned long arch_local_irq_save(void) {
		unsigned long flags = arch_save_fl();
		arch_irq_disable();
		return flags;
	}
	#else

	#	ifdef CONFIG_DEBUG_ENTRY
	#		define SAVE_FLAGS		pushfq; popq %rax
	#	endif

	#endif /* __ASSEMBLY__ */
	// #endif /* CONFIG_PARAVIRT_XXL */

	#ifndef __ASSEMBLY__
		static __always_inline int
		arch_irqs_disabled_flags(unsigned long flags) {
			return !(flags & X86_EFLAGS_IF);
		}

		static __always_inline int
		arch_irqs_disabled(void) {
			unsigned long flags = arch_save_fl();
			return arch_irqs_disabled_flags(flags);
		}

		static __always_inline void
		arch_local_irq_restore(unsigned long flags) {
			if (!arch_irqs_disabled_flags(flags))
				arch_irq_enable();
		}
	#endif /* !__ASSEMBLY__ */

#endif

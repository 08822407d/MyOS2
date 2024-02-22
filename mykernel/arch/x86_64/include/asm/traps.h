// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_TRAPS_H
#define _ASM_X86_TRAPS_H

	// #include <linux/context_tracking_state.h>
	// #include <linux/kprobes.h>

	// #include <asm/debugreg.h>
	#include <asm/idtentry.h>
	// #include <asm/siginfo.h>			/* TRAP_TRACE, ... */
	#include <asm/trap_pf.h>

	// asmlinkage __visible notrace pt_regs_s *sync_regs(pt_regs_s *eregs);
	// asmlinkage __visible notrace
	// struct bad_iret_stack *fixup_bad_iret(struct bad_iret_stack *s);
	// void __init trap_init(void);
	// asmlinkage __visible noinstr pt_regs_s *vc_switch_off_ist(pt_regs_s *eregs);

	// #ifdef CONFIG_X86_F00F_BUG
	// /* For handling the FOOF bug */
	// void handle_invalid_op(pt_regs_s *regs);
	// #endif

	// static inline int get_si_code(unsigned long condition)
	// {
	// 	if (condition & DR_STEP)
	// 		return TRAP_TRACE;
	// 	else if (condition & (DR_TRAP0|DR_TRAP1|DR_TRAP2|DR_TRAP3))
	// 		return TRAP_HWBKPT;
	// 	else
	// 		return TRAP_BRKPT;
	// }

	// extern int panic_on_unrecovered_nmi;

	// void math_emulate(struct math_emu_info *);

	// bool fault_in_kernel_space(unsigned long address);

	// #ifdef CONFIG_VMAP_STACK
	// void __noreturn handle_stack_overflow(pt_regs_s *regs,
	// 					unsigned long fault_address,
	// 					struct stack_info *info);
	// #endif

#endif /* _ASM_X86_TRAPS_H */

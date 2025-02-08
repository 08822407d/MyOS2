// SPDX-License-Identifier: GPL-2.0
#ifndef __LINUX_KBUILD_H
#  error "Please do not build this file directly, build asm-offsets.c instead"
#endif

#include <asm/ptrace.h>

// #if defined(CONFIG_KVM_GUEST)
// #include <asm/kvm_para.h>
// #endif

int main(void)
{
// #ifdef CONFIG_PARAVIRT
// #ifdef CONFIG_PARAVIRT_XXL
// #ifdef CONFIG_DEBUG_ENTRY
// 	OFFSET(PV_IRQ_save_fl, paravirt_patch_template, irq.save_fl);
// #endif
// #endif
// 	BLANK();
// #endif

// #if defined(CONFIG_KVM_GUEST)
// 	OFFSET(KVM_STEAL_TIME_preempted, kvm_steal_time, preempted);
// 	BLANK();
// #endif

#define ENTRY(entry) OFFSET(pt_regs_ ## entry, pt_regs, entry)
	ENTRY(bx);
	ENTRY(cx);
	ENTRY(dx);
	ENTRY(sp);
	ENTRY(bp);
	ENTRY(si);
	ENTRY(di);
	ENTRY(r8);
	ENTRY(r9);
	ENTRY(r10);
	ENTRY(r11);
	ENTRY(r12);
	ENTRY(r13);
	ENTRY(r14);
	ENTRY(r15);
	ENTRY(flags);

	BLANK();
#undef ENTRY

	OFFSET(R15, pt_regs, r15);
	OFFSET(R14, pt_regs, r14);
	OFFSET(R13, pt_regs, r13);
	OFFSET(R12, pt_regs, r12);
	OFFSET(RBP, pt_regs, bp);
	OFFSET(RBX, pt_regs, bx);
	OFFSET(R11, pt_regs, r11);
	OFFSET(R10, pt_regs, r10);
	OFFSET(R9, pt_regs, r9);
	OFFSET(R8, pt_regs, r8);
	OFFSET(RAX, pt_regs, ax);
	OFFSET(RCX, pt_regs, cx);
	OFFSET(RDX, pt_regs, dx);
	OFFSET(RSI, pt_regs, si);
	OFFSET(RDI, pt_regs, di);
	OFFSET(IRQ_NR, pt_regs, irq_nr);
	OFFSET(ORIG_RAX, pt_regs, orig_ax);
	OFFSET(RIP, pt_regs, ip);
	OFFSET(CS, pt_regs, cs);
	OFFSET(EFLAGS, pt_regs, flags);
	OFFSET(RSP, pt_regs, sp);
	OFFSET(SS, pt_regs, ss);

	BLANK();

// #define ENTRY(entry) OFFSET(saved_context_ ## entry, saved_context, entry)
// 	ENTRY(cr0);
// 	ENTRY(cr2);
// 	ENTRY(cr3);
// 	ENTRY(cr4);
// 	ENTRY(gdt_desc);
// 	BLANK();
// #undef ENTRY

	BLANK();

// #ifdef CONFIG_STACKPROTECTOR
// 	OFFSET(FIXED_stack_canary, fixed_percpu_data, stack_canary);
// 	BLANK();
// #endif
	return 0;
}

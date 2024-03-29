/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_KDEBUG_H
#define _ASM_X86_KDEBUG_H

	// #include <linux/notifier.h>

	struct pt_regs;
	typedef struct pt_regs pt_regs_s;

	/* Grossly misnamed. */
	enum die_val {
		DIE_OOPS = 1,
		DIE_INT3,
		DIE_DEBUG,
		DIE_PANIC,
		DIE_NMI,
		DIE_DIE,
		DIE_KERNELDEBUG,
		DIE_TRAP,
		DIE_GPF,
		DIE_CALL,
		DIE_PAGE_FAULT,
		DIE_NMIUNKNOWN,
	};

	enum show_regs_mode {
		SHOW_REGS_SHORT,
		/*
		* For when userspace crashed, but we don't think it's our fault, and
		* therefore don't print kernel registers.
		*/
		SHOW_REGS_USER,
		SHOW_REGS_ALL
	};

	extern void die(const char *, pt_regs_s *,long);
	void die_addr(const char *str, pt_regs_s *regs, long err, long gp_addr);
	// extern int __must_check __die(const char *, pt_regs_s *, long);
	extern void show_stack_regs(pt_regs_s *regs);
	extern void __show_regs(pt_regs_s *regs, enum show_regs_mode,
				const char *log_lvl);
	extern void show_iret_regs(pt_regs_s *regs, const char *log_lvl);
	extern unsigned long oops_begin(void);
	extern void oops_end(unsigned long, pt_regs_s *, int signr);

#endif /* _ASM_X86_KDEBUG_H */

/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __ASM_GENERIC_UCONTEXT_H
#define __ASM_GENERIC_UCONTEXT_H

	struct ucontext {
		ulong				uc_flags;
		ucontext_s			*uc_link;
		stack_t				uc_stack;
		struct sigcontext	uc_mcontext;
		sigset_t			uc_sigmask;	/* mask last for extensibility */
	};

#endif /* __ASM_GENERIC_UCONTEXT_H */

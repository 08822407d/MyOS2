/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SIGNAL_API_H_
#define _ASM_X86_SIGNAL_API_H_

#include "signal.h"

#include <linux/kernel/lock_ipc_type_declaration.h>
#include <asm/ptrace.h>

	extern void arch_do_signal_or_restart(pt_regs_s *regs);
	extern int x64_setup_rt_frame(ksignal_s *ksig, pt_regs_s *regs);
	extern void *get_sigframe(ksignal_s *ksig, pt_regs_s *regs,
			size_t frame_size, void __user **fpstate);

#endif /* _ASM_X86_SIGNAL_API_H_ */

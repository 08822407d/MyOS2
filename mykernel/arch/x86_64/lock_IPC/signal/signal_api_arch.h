/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SIGNAL_API_H_
#define _ASM_X86_SIGNAL_API_H_

#include "signal.h"

#include <asm/ptrace.h>

	extern void arch_do_signal_or_restart(pt_regs_s *regs);

#endif /* _ASM_X86_SIGNAL_API_H_ */

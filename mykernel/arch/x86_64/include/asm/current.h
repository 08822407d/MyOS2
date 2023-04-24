/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CURRENT_H
#define _ASM_X86_CURRENT_H

	#include <linux/kernel/compiler.h>
	// #include <asm/percpu.h>

	#ifndef __ASSEMBLY__
		struct task_struct;
		typedef struct task_struct task_s;

		// DECLARE_PER_CPU(task_s *, current_task);

		// static __always_inline task_s *get_current(void)
		// {
		// 	return this_cpu_read_stable(current_task);
		// }

		extern task_s *myos_get_current(void);
	// #	define current get_current()
	#	define current myos_get_current()
	#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_CURRENT_H */

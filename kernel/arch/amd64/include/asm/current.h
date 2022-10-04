/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CURRENT_H

	#define _ASM_X86_CURRENT_H

	#include <linux/kernel/compiler.h>
	// #include <asm/percpu.h>

	#ifndef __ASSEMBLY__
		struct task_struct;
		typedef struct task_struct task_s;

		// DECLARE_PER_CPU(struct task_struct *, current_task);

		// static __always_inline task_s *get_current(void)
		// {
		// 	return this_cpu_read_stable(current_task);
		// }

		// #define current get_current()
		task_s *get_current_task(void);
		#define curr_tsk get_current_task()

	#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_CURRENT_H */

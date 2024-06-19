// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CURRENT_H
#define _ASM_X86_CURRENT_H

#include <linux/compiler/compiler.h>

	#ifndef __ASSEMBLY__

	#include <linux/kernel/sched_type_declaration.h>
	#include <linux/kernel/cache.h>
	#include <asm/percpu.h>
	#include <asm/percpu_area.h>

	typedef struct pcpu_hot {
		union {
			struct {
				task_s			*current_task;
				int				preempt_count;
				int				cpu_number;
				u64				call_depth;
				unsigned long	top_of_stack;
				void			*hardirq_stack_ptr;
				u16				softirq_pending;
				bool			hardirq_stack_inuse;
			};
			u8	pad[64];
		};
	} pcpu_hot_s;

	static __always_inline task_s *get_current(void) {
		return *this_cpu_ptr(&pcpu_hot.current_task);
	}

	#define current get_current()

	#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_CURRENT_H */

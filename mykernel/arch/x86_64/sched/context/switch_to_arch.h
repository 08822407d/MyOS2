/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SWITCH_TO_H_
#define _ASM_X86_SWITCH_TO_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <asm/processor.h>

	#include "../sched_const_arch.h"
	#include "../sched_types_arch.h"
	#include "../sched_api_arch.h"


	#ifdef DEBUG

		extern void
		update_task_stack(task_s *task);

		extern void
		kthread_frame_init(task_kframe_s *frame, int (*fun)(void *), void *arg);

	#endif

	#include "switch_to_macro_arch.h"

	#if defined(ARCH_SWITCH_TO_DEFINATION) || !(DEBUG)

		/* This is used when switching tasks or entering/exiting vm86 mode. */
		PREFIX_STATIC_INLINE
		void
		update_task_stack(task_s *task) {
			/* sp0 always points to the entry trampoline stack, which is constant: */
			/* Xen PV enters the kernel on the thread stack. */
			// if (cpu_feature_enabled(X86_FEATURE_XENPV))
				load_sp0(task_top_of_stack(task));
		}

		PREFIX_STATIC_INLINE
		void
		kthread_frame_init(task_kframe_s *frame, int (*fun)(void *), void *arg) {
			frame->bx = (reg_t)fun;
			frame->r12 = (reg_t)arg;
		}

	#endif


#endif /* _ASM_X86_SWITCH_TO_H_ */

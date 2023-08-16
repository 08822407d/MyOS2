// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_FSGSBASE_H
#define _ASM_FSGSBASE_H

	#include <linux/kernel/compiler.h>

	#ifndef __ASSEMBLY__

	#	include <asm/msr-index.h>

	// /*
	//  * Read/write a task's FSBASE or GSBASE. This returns the value that
	//  * the FS/GS base would have (if the task were to be resumed). These
	//  * work on the current task or on a non-running (typically stopped
	//  * ptrace child) task.
	//  */
	// extern unsigned long x86_fsbase_read_task(task_s *task);
	// extern unsigned long x86_gsbase_read_task(task_s *task);
	// extern void x86_fsbase_write_task(task_s *task, unsigned long fsbase);
	// extern void x86_gsbase_write_task(task_s *task, unsigned long gsbase);

	/* Must be protected by X86_FEATURE_FSGSBASE check. */

	static __always_inline unsigned long
	rdfsbase(void) {
		unsigned long fsbase;
		asm volatile(	"rdfsbase	%0		\t\n"
					:	"=r" (fsbase)
					:
					:	"memory");
		return fsbase;
	}

	static __always_inline unsigned long
	rdgsbase(void) {
		unsigned long gsbase;
		asm volatile(	"rdgsbase	%0		\t\n"
					:	"=r" (gsbase)
					:
					:	"memory");
		return gsbase;
	}

	static __always_inline void
	wrfsbase(unsigned long fsbase) {
		asm volatile(	"wrfsbase	%0		\t\n"
					:
					:	"r" (fsbase)
					:	"memory");
	}

	static __always_inline void
	wrgsbase(unsigned long gsbase) {
		asm volatile(	"wrgsbase	%0		\t\n"
					:
					:	"r" (gsbase)
					:	"memory");
	}

	#	include <asm/cpufeature.h>

	// /* Helper functions for reading/writing FS/GS base */

	// static inline unsigned long x86_fsbase_read_cpu(void)
	// {
	// 	unsigned long fsbase;

	// 	if (boot_cpu_has(X86_FEATURE_FSGSBASE))
	// 		fsbase = rdfsbase();
	// 	else
	// 		rdmsrl(MSR_FS_BASE, fsbase);

	// 	return fsbase;
	// }

	// static inline void x86_fsbase_write_cpu(unsigned long fsbase)
	// {
	// 	if (boot_cpu_has(X86_FEATURE_FSGSBASE))
	// 		wrfsbase(fsbase);
	// 	else
	// 		wrmsrl(MSR_FS_BASE, fsbase);
	// }

	// extern unsigned long x86_gsbase_read_cpu_inactive(void);
	// extern void x86_gsbase_write_cpu_inactive(unsigned long gsbase);
	// extern unsigned long x86_fsgsbase_read_task(task_s *task,
	// 						unsigned short selector);

	#endif /* __ASSEMBLY__ */

#endif /* _ASM_FSGSBASE_H */

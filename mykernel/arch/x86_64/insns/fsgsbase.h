// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_FSGSBASE_H_
#define _ASM_X86_FSGSBASE_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <asm/processor.h>

	#include "x86msr.h"

	#ifdef DEBUG
	
		extern unsigned long
		rdfsbase(void);
		extern unsigned long
		rdgsbase(void);
		extern void
		wrfsbase(unsigned long fsbase);
		extern void
		wrgsbase(unsigned long gsbase);

		extern unsigned long
		x86_fsbase_read_cpu(void);

		extern void
		x86_fsbase_write_cpu(unsigned long fsbase);

	#endif

	#if defined(ARCH_INSTRUCTION_DEFINATION) || !(DEBUG)

		/*
		 * Read/write a task's FSBASE or GSBASE. This returns the value that
		 * the FS/GS base would have (if the task were to be resumed). These
		 * work on the current task or on a non-running (typically stopped
		 * ptrace child) task.
		 */

		/* Must be protected by X86_FEATURE_FSGSBASE check. */

		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long
		rdfsbase(void) {
			unsigned long fsbase;
			asm volatile(	"rdfsbase	%0		\t\n"
						:	"=r" (fsbase)
						:
						:	"memory");
			return fsbase;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long
		rdgsbase(void) {
			unsigned long gsbase;
			asm volatile(	"rdgsbase	%0		\t\n"
						:	"=r" (gsbase)
						:
						:	"memory");
			return gsbase;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		wrfsbase(unsigned long fsbase) {
			asm volatile(	"wrfsbase	%0		\t\n"
						:
						:	"r" (fsbase)
						:	"memory");
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		wrgsbase(unsigned long gsbase) {
			asm volatile(	"wrgsbase	%0		\t\n"
						:
						:	"r" (gsbase)
						:	"memory");
		}

		/* Helper functions for reading/writing FS/GS base */

		PREFIX_STATIC_INLINE
		unsigned long
		x86_fsbase_read_cpu(void) {
			u64 fsbase;

			// if (boot_cpu_has(X86_FEATURE_FSGSBASE))
			if (boot_cpu_data.x86_capa_bits.FSGSBASE)
				fsbase = rdfsbase();
			else
				rdmsrl(MSR_FS_BASE, &fsbase);

			return (unsigned long)fsbase;
		}

		PREFIX_STATIC_INLINE
		void
		x86_fsbase_write_cpu(unsigned long fsbase) {
			// if (boot_cpu_has(X86_FEATURE_FSGSBASE))
			if (boot_cpu_data.x86_capa_bits.FSGSBASE)
				wrfsbase(fsbase);
			else
				wrmsrl(MSR_FS_BASE, fsbase);
		}

	#endif

#endif /* _ASM_FSGSBASE_H_ */

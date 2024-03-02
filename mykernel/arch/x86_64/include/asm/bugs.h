// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BUGS_H
#define _ASM_X86_BUGS_H

	extern void check_bugs(void);

	static inline int ppro_with_ram_bug(void) { return 0; }

	// extern void cpu_bugs_smt_update(void);

#endif /* _ASM_X86_BUGS_H */

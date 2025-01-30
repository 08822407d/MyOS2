/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MMU_MACRO_H_
#define _ASM_X86_MMU_MACRO_H_

	// #define activate_mm(prev, next)			\
	// do {						\
	// 	paravirt_activate_mm((prev), (next));	\
	// 	switch_mm((prev), (next), NULL);	\
	// } while (0);
	#define activate_mm(prev, next) do {			\
				switch_mm((prev), (next), NULL);	\
			} while (0);

	#define deactivate_mm(tsk, mm)			\
			do {							\
				load_gs_index(0);			\
				loadsegment(fs, 0);			\
			} while (0)

#endif /* _ASM_X86_MMU_MACRO_H_ */
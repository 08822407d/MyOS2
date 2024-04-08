#ifndef _ASM_X86_EARLY_API_H_
#define _ASM_X86_EARLY_API_H_

	#include "e820.h"


	extern void e820__print_table(char *who);
	extern unsigned long e820__end_of_ram_pfn(void);
	extern unsigned long e820__end_of_low_ram_pfn(void);
	extern void e820__memblock_setup(void);
	extern void e820__memory_setup(void);

#endif /* _ASM_X86_EARLY_API_H_ */
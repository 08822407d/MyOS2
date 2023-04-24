#ifndef _AMD64_GLO_H_
#define _AMD64_GLO_H_

#include "archtypes.h"

	extern uint64_t	apic_id[CONFIG_NR_CPUS];

	extern desctblptr64_T	gdt_ptr;
	extern desctblptr64_T	idt_ptr;
	extern tss64_T *		tss_ptr_arr;

	extern phys_addr_t 		kernel_cr3;

#endif /* _AMD64_GLO_H_ */
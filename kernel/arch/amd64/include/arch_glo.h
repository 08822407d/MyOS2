#ifndef _AMD64_GLO_H_
#define _AMD64_GLO_H_

#include <sys/types.h>

#include "archconst.h"
#include "archtypes.h"
#include "arch_proto.h"

	extern memory_info_s	mem_info;
	
	extern uint64_t	apic_id[CONFIG_MAX_CPUS];
	extern struct cputopo	smp_topos[CONFIG_MAX_CPUS];

	extern desctblptr64_T	gdt_ptr;
	extern desctblptr64_T	idt_ptr;
	extern tss64_T **		tss_ptr_arr;

	extern PML4E_T *		KERN_PML4;
	extern phys_addr_t 		kernel_cr3;

#endif /* _AMD64_GLO_H_ */
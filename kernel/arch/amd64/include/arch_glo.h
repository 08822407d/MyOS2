#ifndef _AMD64_GLO_H_
#define _AMD64_GLO_H_

#include <sys/types.h>

#include "archconst.h"
#include "archtypes.h"
#include "arch_proto.h"

	extern uint64_t	apic_id[CONFIG_MAX_CPUS];
	extern struct cputopo	smp_topos[CONFIG_MAX_CPUS];

	extern desctblptr64_s	gdt_ptr;
	extern desctblptr64_s	idt_ptr;
	extern tss64_s **		tss_ptr_arr;

#endif /* _AMD64_GLO_H_ */
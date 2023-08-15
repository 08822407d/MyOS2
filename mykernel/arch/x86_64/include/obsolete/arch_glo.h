#ifndef _AMD64_GLO_H_
#define _AMD64_GLO_H_

#include "archtypes.h"

	extern uint64_t	apic_id[CONFIG_NR_CPUS];
	extern phys_addr_t 		kernel_cr3;

	#define USERADDR_LIMIT		0x00007FFFFFFFFFFF

#endif /* _AMD64_GLO_H_ */
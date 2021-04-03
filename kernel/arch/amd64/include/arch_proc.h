#ifndef _ARCH_PROC_H_
#define _ARCH_PROC_H_

#include <sys/types.h>
#include <lib/utils.h>

typedef struct
{
	reg_t		rsp0;

	reg_t		rip;
	reg_t		rsp;

	reg_t		fs;
	reg_t		gs;

	reg_t		cr2;
	uint64_t	intr_vec;
	uint64_t	err_code;

	reg_t		cr3;
} arch_PCB_s;

#endif /* _ARCH_PROC_H_ */
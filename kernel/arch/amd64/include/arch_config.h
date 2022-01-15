#ifndef _ARCH_CONFIG_H_
#define _ARCH_CONFIG_H_

#include <const.h>

#include <include/math.h>

	#define CONFIG_X86_64
	/* Constants for protected mode. */
	#define CONFIG_MAX_CPUS		256
	#define CPUSTACK_SIZE		4096
	#define PAGE_SIZE			CONST_2M
	#define PAGE_ROUND_UP(x)	round_up((x), PAGE_SIZE)
	#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
	#define PAGE_SHIFT			SHIFT_2M

	#define CONFIG_MAX_VIRMEM	(512 * CONST_1G)
	#define PDE_NR				(CONFIG_MAX_VIRMEM >> SHIFT_PDE)

#endif /* _ARCH_CONFIG_H_ */
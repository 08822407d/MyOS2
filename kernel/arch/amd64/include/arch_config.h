#ifndef _ARCH_CONFIG_H_
#define _ARCH_CONFIG_H_

#include <linux/kernel/math.h>

#include <klib/const.h>

	/* Constants for protected mode. */
	#define CONFIG_MAX_CPUS		256

	#define CONFIG_MAX_VIRMEM	(512 * CONST_1G)
	#define PDE_NR				(CONFIG_MAX_VIRMEM >> SHIFT_PDE)

#endif /* _ARCH_CONFIG_H_ */
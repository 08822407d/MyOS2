#ifndef _ARCH_CONFIG_H_
#define _ARCH_CONFIG_H_

#include <linux/kernel/math.h>

#include <const.h>

	#define CONFIG_X86_64
	#define CONFIG_ZONE_DMA

	/* Constants for protected mode. */
	#define CONFIG_MAX_CPUS		256
	#define CPUSTACK_SIZE		4096
	#define PAGE_SIZE			CONST_2M
	#define PAGE_ROUND_UP(x)	round_up((x), PAGE_SIZE)
	#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
	#define PAGE_SHIFT			SHIFT_2M

	#define CONFIG_MAX_VIRMEM	(512 * CONST_1G)
	#define PDE_NR				(CONFIG_MAX_VIRMEM >> SHIFT_PDE)

	/* 16MB ISA DMA zone */
	#define MAX_DMA_ADDR	(16 * CONST_1M)
	#define MAX_DMA_PFN		(MAX_DMA_ADDR >> PAGE_SHIFT)

	/* 4GB broken PCI/AGP hardware bus master zone */
	#define MAX_DMA32_PFN (1UL << (32 - PAGE_SHIFT))

#endif /* _ARCH_CONFIG_H_ */
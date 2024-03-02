// SPDX-License-Identifier: GPL-2.0
#include <asm/processor.h>
#include "cpu.h"

static const cpu_dev_s intel_cpu_dev = {
	.c_vendor		= "Intel",
	.c_ident		= {"GenuineIntel"},
	// .c_detect_tlb	= intel_detect_tlb,
	// .c_early_init	= early_init_intel,
	// .c_bsp_init		= bsp_init_intel,
	// .c_init			= init_intel,
	.c_x86_vendor	= X86_VENDOR_INTEL,
};
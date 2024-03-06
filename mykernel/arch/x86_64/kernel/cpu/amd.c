// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/export.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/elf.h>
#include <linux/kernel/mm.h>

#include <linux/kernel/sched.h>
#include <linux/sched/clock.h>
#include <asm/cpu.h>
#include <asm/smp.h>

#include "cpu.h"


static const cpu_dev_s amd_cpu_dev = {
	.c_vendor		= "AMD",
	.c_ident		= {"AuthenticAMD"},
	// .c_early_init	= early_init_amd,
	// .c_detect_tlb	= cpu_detect_tlb_amd,
	// .c_bsp_init		= bsp_init_amd,
	// .c_init			= init_amd,
	.c_x86_vendor	= X86_VENDOR_AMD,
};
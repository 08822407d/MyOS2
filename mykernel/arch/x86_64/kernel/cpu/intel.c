// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel/kernel.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/uaccess.h>
#include <linux/kernel/delay.h>
#include <linux/lib/string.h>
#include <linux/smp/smp.h>
#include <linux/kernel/sched.h>
#include <linux/sched/clock.h>
#include <linux/init/init.h>

#include <asm/cpufeature.h>
#include <asm/msr.h>
#include <asm/bugs.h>
#include <asm/cpu.h>
#include <asm/elf.h>
#include <asm/traps.h>

#include "cpu.h"

#include <asm/apic.h>


static const cpu_dev_s intel_cpu_dev = {
	.c_vendor		= "Intel",
	.c_ident		= {"GenuineIntel"},
	// .c_detect_tlb	= intel_detect_tlb,
	// .c_early_init	= early_init_intel,
	// .c_bsp_init		= bsp_init_intel,
	// .c_init			= init_intel,
	.c_x86_vendor	= X86_VENDOR_INTEL,
};
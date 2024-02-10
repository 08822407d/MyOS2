// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/export.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/elf.h>
#include <linux/mm/mm.h>

// #include <linux/kernel/io.h>
#include <linux/kernel/sched.h>
#include <linux/sched/clock.h>
// #include <linux/random.h>
// #include <linux/topology.h>
#include <asm/processor.h>
// #include <asm/apic.h>
// #include <asm/cacheinfo.h>
#include <asm/cpu.h>
// #include <asm/spec-ctrl.h>
#include <asm/smp.h>
// #include <asm/numa.h>
// #include <asm/pci-direct.h>
// #include <asm/delay.h>
// #include <asm/debugreg.h>
// #include <asm/resctrl.h>

// #include <asm/mmconfig.h>

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
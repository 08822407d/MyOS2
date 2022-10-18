// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel/kernel.h>
// #include <linux/pgtable.h>

#include <linux/lib/string.h>
#include <linux/kernel/bitops.h>
// #include <linux/smp.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/clock.h>
// #include <linux/thread_info.h>
#include <linux/init/init.h>
// #include <linux/uaccess.h>
// #include <linux/delay.h>

// #include <asm/cpufeature.h>
// #include <asm/msr.h>
// #include <asm/bugs.h>
// #include <asm/cpu.h>
// #include <asm/intel-family.h>
// #include <asm/microcode_intel.h>
// #include <asm/hwcap2.h>
#include <asm/elf.h>
// #include <asm/cpu_device_id.h>
// #include <asm/cmdline.h>
// #include <asm/traps.h>
// #include <asm/resctrl.h>
// #include <asm/numa.h>
// #include <asm/thermal.h>

// #include <linux/topology.h>

#include "cpu.h"

#ifdef CONFIG_X86_LOCAL_APIC
// #	include <asm/mpspec.h>
// #	include <asm/apic.h>
#endif


static const cpu_dev_s intel_cpu_dev = {
	.c_vendor		= "Intel",
	.c_ident		= {"GenuineIntel"},
	// .c_detect_tlb	= intel_detect_tlb,
	// .c_early_init	= early_init_intel,
	// .c_bsp_init		= bsp_init_intel,
	// .c_init			= init_intel,
	.c_x86_vendor	= X86_VENDOR_INTEL,
};
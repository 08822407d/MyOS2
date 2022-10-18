// SPDX-License-Identifier: GPL-2.0-only
/* cpu_feature_enabled() cannot be used this early */
// #define USE_EARLY_PGTABLE_L5

#include <linux/mm/memblock.h>
#include <linux/kernel/linkage.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/kernel.h>
// #include <linux/export.h>
// #include <linux/percpu.h>
#include <linux/lib/string.h>
#include <linux/kernel/ctype.h>
// #include <linux/delay.h>
// #include <linux/sched/mm.h>
// #include <linux/sched/clock.h>
// #include <linux/sched/task.h>
// #include <linux/sched/smt.h>
#include <linux/init/init.h>
// #include <linux/kprobes.h>
// #include <linux/kgdb.h>
// #include <linux/smp.h>
// #include <linux/io.h>
// #include <linux/syscore_ops.h>
// #include <linux/pgtable.h>

// #include <asm/cmdline.h>
// #include <asm/stackprotector.h>
// #include <asm/perf_event.h>
// #include <asm/mmu_context.h>
// #include <asm/doublefault.h>
// #include <asm/archrandom.h>
// #include <asm/hypervisor.h>
// #include <asm/processor.h>
// #include <asm/tlbflush.h>
// #include <asm/debugreg.h>
// #include <asm/sections.h>
// #include <asm/vsyscall.h>
// #include <linux/topology.h>
// #include <linux/cpumask.h>
// #include <linux/atomic.h>
// #include <asm/proto.h>
#include <asm/setup.h>
// #include <asm/apic.h>
// #include <asm/desc.h>
// #include <asm/fpu/api.h>
// #include <asm/mtrr.h>
// #include <asm/hwcap2.h>
// #include <linux/numa.h>
// #include <asm/numa.h>
// #include <asm/asm.h>
// #include <asm/bugs.h>
// #include <asm/cpu.h>
// #include <asm/mce.h>
// #include <asm/msr.h>
// #include <asm/memtype.h>
// #include <asm/microcode.h>
// #include <asm/microcode_intel.h>
// #include <asm/intel-family.h>
// #include <asm/cpu_device_id.h>
// #include <asm/uv/uv.h>
// #include <asm/sigframe.h>

#include "cpu.h"

void __init early_cpu_init(void)
{
// 	const struct cpu_dev *const *cdev;
// 	int count = 0;

// #ifdef CONFIG_PROCESSOR_SELECT
// 	pr_info("KERNEL supported cpus:\n");
// #endif

// 	for (cdev = __x86_cpu_dev_start; cdev < __x86_cpu_dev_end; cdev++)
// 	{
// 		const struct cpu_dev *cpudev = *cdev;

// 		if (count >= X86_VENDOR_NUM)
// 			break;
// 		cpu_devs[count] = cpudev;
// 		count++;

// #ifdef CONFIG_PROCESSOR_SELECT
// 		{
// 			unsigned int j;

// 			for (j = 0; j < 2; j++)
// 			{
// 				if (!cpudev->c_ident[j])
// 					continue;
// 				pr_info("  %s %s\n", cpudev->c_vendor,
// 						cpudev->c_ident[j]);
// 			}
// 		}
// #endif
// 	}
// 	early_identify_cpu(&boot_cpu_data);
}
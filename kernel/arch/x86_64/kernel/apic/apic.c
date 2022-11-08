// SPDX-License-Identifier: GPL-2.0-only
/*
 *	Local APIC handling, local APIC timers
 *
 *	(c) 1999, 2000, 2009 Ingo Molnar <mingo@redhat.com>
 *
 *	Fixes
 *	Maciej W. Rozycki	:	Bits for genuine 82489DX APICs;
 *					thanks to Eric Gilmore
 *					and Rolf G. Tews
 *					for testing these extensively.
 *	Maciej W. Rozycki	:	Various updates and fixes.
 *	Mikael Pettersson	:	Power Management for UP-APIC.
 *	Pavel Machek and
 *	Mikael Pettersson	:	PM converted to driver model.
 */

// #include <linux/perf_event.h>
// #include <linux/kernel_stat.h>
// #include <linux/mc146818rtc.h>
// #include <linux/acpi_pmtmr.h>
// #include <linux/clockchips.h>
// #include <linux/interrupt.h>
#include <linux/mm/memblock.h>
// #include <linux/ftrace.h>
// #include <linux/ioport.h>
// #include <linux/export.h>
// #include <linux/syscore_ops.h>
// #include <linux/delay.h>
// #include <linux/timex.h>
// #include <linux/i8253.h>
// #include <linux/dmar.h>
#include <linux/init/init.h>
// #include <linux/cpu.h>
// #include <linux/dmi.h>
// #include <linux/smp.h>
#include <linux/mm/mm.h>

// #include <asm/trace/irq_vectors.h>
// #include <asm/irq_remapping.h>
// #include <asm/pc-conf-reg.h>
// #include <asm/perf_event.h>
// #include <asm/x86_init.h>
#include <linux/kernel/atomic.h>
#include <asm/barrier.h>
// #include <asm/mpspec.h>
// #include <asm/i8259.h>
// #include <asm/proto.h>
// #include <asm/traps.h>
#include <asm/apic.h>
// #include <asm/acpi.h>
// #include <asm/io_apic.h>
// #include <asm/desc.h>
// #include <asm/hpet.h>
// #include <asm/mtrr.h>
// #include <asm/time.h>
// #include <asm/smp.h>
// #include <asm/mce.h>
// #include <asm/tsc.h>
// #include <asm/hypervisor.h>
// #include <asm/cpu_device_id.h>
// #include <asm/intel-family.h>
// #include <asm/irq_regs.h>


#include <obsolete/arch_proto.h>

unsigned int num_processors;

int x2apic_mode;
enum {
	X2APIC_OFF,
	X2APIC_ON,
	X2APIC_DISABLED,
};
static int x2apic_state;

void __init check_x2apic(void)
{
	bool x2apic_enable = false;
	u64 msr;

	msr = rdmsr(MSR_IA32_APICBASE);
	x2apic_enable = msr & X2APIC_ENABLE;
	if (x2apic_enable) {
		// pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
		x2apic_mode = 1;
		x2apic_state = X2APIC_ON;
	}
	// else if (!boot_cpu_has(X86_FEATURE_X2APIC))
	// {
	// 	x2apic_state = X2APIC_DISABLED;
	// }
}
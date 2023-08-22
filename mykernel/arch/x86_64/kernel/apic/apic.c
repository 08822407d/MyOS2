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
#include <linux/kernel/ioport.h>
// #include <linux/export.h>
// #include <linux/syscore_ops.h>
#include <linux/kernel/delay.h>
// #include <linux/timex.h>
// #include <linux/i8253.h>
// #include <linux/dmar.h>
#include <linux/init/init.h>
#include <linux/kernel/cpu.h>
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
#include <asm/desc.h>
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
#include <asm/cpu.h>



int x2apic_mode;

enum {
	X2APIC_OFF,
	X2APIC_DISABLED,
	/* All states below here have X2APIC enabled */
	X2APIC_ON,
	X2APIC_ON_LOCKED
};
static int x2apic_state;

// static bool x2apic_hw_locked(void)
// {
// 	u64 ia32_cap;
// 	u64 msr;

// 	ia32_cap = x86_read_arch_cap_msr();
// 	if (ia32_cap & ARCH_CAP_XAPIC_DISABLE) {
// 		rdmsrl(MSR_IA32_XAPIC_DISABLE_STATUS, msr);
// 		return (msr & LEGACY_XAPIC_DISABLED);
// 	}
// 	return false;
// }

// static void __x2apic_disable(void)
// {
// 	u64 msr;

// 	if (!boot_cpu_has(X86_FEATURE_APIC))
// 		return;

// 	rdmsrl(MSR_IA32_APICBASE, msr);
// 	if (!(msr & X2APIC_ENABLE))
// 		return;
// 	/* Disable xapic and x2apic first and then reenable xapic mode */
// 	wrmsrl(MSR_IA32_APICBASE, msr & ~(X2APIC_ENABLE | XAPIC_ENABLE));
// 	wrmsrl(MSR_IA32_APICBASE, msr & ~X2APIC_ENABLE);
// 	printk_once(KERN_INFO "x2apic disabled\n");
// }

static void __x2apic_enable(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (msr & X2APIC_ENABLE)
		return;
	wrmsrl(MSR_IA32_APICBASE, msr | X2APIC_ENABLE);
	// printk_once(KERN_INFO "x2apic enabled\n");
}

// static int __init setup_nox2apic(char *str)
// {
// 	if (x2apic_enabled()) {
// 		int apicid = native_apic_msr_read(APIC_ID);

// 		if (apicid >= 255) {
// 			pr_warn("Apicid: %08x, cannot enforce nox2apic\n",
// 				apicid);
// 			return 0;
// 		}
// 		if (x2apic_hw_locked()) {
// 			pr_warn("APIC locked in x2apic mode, can't disable\n");
// 			return 0;
// 		}
// 		pr_warn("x2apic already enabled.\n");
// 		__x2apic_disable();
// 	}
// 	setup_clear_cpu_cap(X86_FEATURE_X2APIC);
// 	x2apic_state = X2APIC_DISABLED;
// 	x2apic_mode = 0;
// 	return 0;
// }
// early_param("nox2apic", setup_nox2apic);

// /* Called from cpu_init() to enable x2apic on (secondary) cpus */
// void x2apic_setup(void)
// {
// 	/*
// 	 * Try to make the AP's APIC state match that of the BSP,  but if the
// 	 * BSP is unlocked and the AP is locked then there is a state mismatch.
// 	 * Warn about the mismatch in case a GP fault occurs due to a locked AP
// 	 * trying to be turned off.
// 	 */
// 	if (x2apic_state != X2APIC_ON_LOCKED && x2apic_hw_locked())
// 		pr_warn("x2apic lock mismatch between BSP and AP.\n");
// 	/*
// 	 * If x2apic is not in ON or LOCKED state, disable it if already enabled
// 	 * from BIOS.
// 	 */
// 	if (x2apic_state < X2APIC_ON) {
// 		__x2apic_disable();
// 		return;
// 	}
// 	__x2apic_enable();
// }

// static __init void x2apic_disable(void)
// {
// 	u32 x2apic_id, state = x2apic_state;

// 	x2apic_mode = 0;
// 	x2apic_state = X2APIC_DISABLED;

// 	if (state != X2APIC_ON)
// 		return;

// 	x2apic_id = read_apic_id();
// 	if (x2apic_id >= 255)
// 		panic("Cannot disable x2apic, id: %08x\n", x2apic_id);

// 	if (x2apic_hw_locked()) {
// 		pr_warn("Cannot disable locked x2apic, id: %08x\n", x2apic_id);
// 		return;
// 	}

// 	__x2apic_disable();
// 	register_lapic_address(mp_lapic_addr);
// }

// static __init void x2apic_enable(void)
// {
// 	if (x2apic_state != X2APIC_OFF)
// 		return;

// 	x2apic_mode = 1;
// 	x2apic_state = X2APIC_ON;
// 	__x2apic_enable();
// }

// static __init void try_to_enable_x2apic(int remap_mode)
// {
// 	if (x2apic_state == X2APIC_DISABLED)
// 		return;

// 	if (remap_mode != IRQ_REMAP_X2APIC_MODE) {
// 		u32 apic_limit = 255;

// 		/*
// 		 * Using X2APIC without IR is not architecturally supported
// 		 * on bare metal but may be supported in guests.
// 		 */
// 		if (!x86_init.hyper.x2apic_available()) {
// 			pr_info("x2apic: IRQ remapping doesn't support X2APIC mode\n");
// 			x2apic_disable();
// 			return;
// 		}

// 		/*
// 		 * If the hypervisor supports extended destination ID in
// 		 * MSI, that increases the maximum APIC ID that can be
// 		 * used for non-remapped IRQ domains.
// 		 */
// 		if (x86_init.hyper.msi_ext_dest_id()) {
// 			virt_ext_dest_id = 1;
// 			apic_limit = 32767;
// 		}

// 		/*
// 		 * Without IR, all CPUs can be addressed by IOAPIC/MSI only
// 		 * in physical mode, and CPUs with an APIC ID that cannot
// 		 * be addressed must not be brought online.
// 		 */
// 		x2apic_set_max_apicid(apic_limit);
// 		x2apic_phys = 1;
// 	}
// 	x2apic_enable();
// }

void __init check_x2apic(void)
{
	if (boot_cpu_data.x86_capa_bits.x2APIC &&
		apic_is_x2apic_enabled()) {
		// pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
		x2apic_mode = 1;
		// if (x2apic_hw_locked())
		// 	x2apic_state = X2APIC_ON_LOCKED;
		// else
			x2apic_state = X2APIC_ON;
	} else if (!boot_cpu_data.x86_capa_bits.x2APIC) {
		x2apic_state = X2APIC_DISABLED;
	}
}
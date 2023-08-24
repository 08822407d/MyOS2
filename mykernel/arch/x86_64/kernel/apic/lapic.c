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
#include <asm/io_apic.h>
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



/*
 * Local APIC start and shutdown
 */

/**
 * clear_local_APIC - shutdown the local APIC
 *
 * This is called, when a CPU is disabled and before rebooting, so the state of
 * the local APIC has no dangling leftovers. Also used to cleanout any BIOS
 * leftovers during boot.
 */
void clear_local_APIC(void)
{
// 	int maxlvt;
// 	u32 v;

// 	/* APIC hasn't been mapped yet */
// 	if (!x2apic_mode && !apic_phys)
// 		return;

// 	maxlvt = lapic_get_maxlvt();
// 	/*
// 	 * Masking an LVT entry can trigger a local APIC error
// 	 * if the vector is zero. Mask LVTERR first to prevent this.
// 	 */
// 	if (maxlvt >= 3) {
// 		v = ERROR_APIC_VECTOR; /* any non-zero vector will do */
// 		apic_write(APIC_LVTERR, v | APIC_LVT_MASKED);
// 	}
// 	/*
// 	 * Careful: we have to set masks only first to deassert
// 	 * any level-triggered sources.
// 	 */
// 	v = apic_read(APIC_LVTT);
// 	apic_write(APIC_LVTT, v | APIC_LVT_MASKED);
// 	v = apic_read(APIC_LVT0);
// 	apic_write(APIC_LVT0, v | APIC_LVT_MASKED);
// 	v = apic_read(APIC_LVT1);
// 	apic_write(APIC_LVT1, v | APIC_LVT_MASKED);
// 	if (maxlvt >= 4) {
// 		v = apic_read(APIC_LVTPC);
// 		apic_write(APIC_LVTPC, v | APIC_LVT_MASKED);
// 	}

// 	/* lets not touch this if we didn't frob it */
// #ifdef CONFIG_X86_THERMAL_VECTOR
// 	if (maxlvt >= 5) {
// 		v = apic_read(APIC_LVTTHMR);
// 		apic_write(APIC_LVTTHMR, v | APIC_LVT_MASKED);
// 	}
// #endif
// #ifdef CONFIG_X86_MCE_INTEL
// 	if (maxlvt >= 6) {
// 		v = apic_read(APIC_LVTCMCI);
// 		if (!(v & APIC_LVT_MASKED))
// 			apic_write(APIC_LVTCMCI, v | APIC_LVT_MASKED);
// 	}
// #endif

// 	/*
// 	 * Clean APIC state for other OSs:
// 	 */
// 	apic_write(APIC_LVTT, APIC_LVT_MASKED);
// 	apic_write(APIC_LVT0, APIC_LVT_MASKED);
// 	apic_write(APIC_LVT1, APIC_LVT_MASKED);
// 	if (maxlvt >= 3)
// 		apic_write(APIC_LVTERR, APIC_LVT_MASKED);
// 	if (maxlvt >= 4)
// 		apic_write(APIC_LVTPC, APIC_LVT_MASKED);

// 	/* Integrated APIC (!82489DX) ? */
// 	if (lapic_is_integrated()) {
// 		if (maxlvt > 3)
// 			/* Clear ESR due to Pentium errata 3AP and 11AP */
// 			apic_write(APIC_ESR, 0);
// 		apic_read(APIC_ESR);
// 	}
}


/*
 * An initial setup of the virtual wire mode.
 */
void __init init_bsp_APIC(void)
{
	unsigned int value;

	/*
	 * Don't do the setup now if we have a SMP BIOS as the
	 * through-I/O-APIC virtual wire mode might be active.
	 */
	// if (smp_found_config || !boot_cpu_has(X86_FEATURE_APIC))
	if (!boot_cpu_data.x86_capa_bits.APIC_On_Chip)
		return;

	/*
	 * Do not trust the local APIC being empty at bootup.
	 */
	clear_local_APIC();

	/*
	 * Enable APIC.
	 */
	value = apic_msr_read(APIC_SPIV);
	value &= ~APIC_VECTOR_MASK;
	value |= APIC_SPIV_APIC_ENABLED;

	// value |= APIC_SPIV_FOCUS_DISABLED;
	value |= APIC_SPIV_DIRECTED_EOI;
	// value |= SPURIOUS_APIC_VECTOR;
	apic_msr_write(APIC_SPIV, value);

	/*
	 * Set up the virtual wire mode.
	 */
	apic_msr_write(APIC_LVT0, APIC_DM_EXTINT);
	value = APIC_DM_NMI;
	// if (!lapic_is_integrated())		/* 82489DX */
	// 	value |= APIC_LVT_LEVEL_TRIGGER;
	// if (apic_extnmi == APIC_EXTNMI_NONE)
	// 	value |= APIC_LVT_MASKED;
	apic_msr_write(APIC_LVT1, value);
}

// /* Init the interrupt delivery mode for the BSP */
// // void __init apic_intr_mode_init(void)
// void intr_mode_init(void)
// {
// 	// bool upmode = IS_ENABLED(CONFIG_UP_LATE_INIT);

// 	// switch (apic_intr_mode) {
// 	// case APIC_PIC:
// 	// 	pr_info("APIC: Keep in PIC mode(8259)\n");
// 	// 	return;
// 	// case APIC_VIRTUAL_WIRE:
// 	// 	pr_info("APIC: Switch to virtual wire mode setup\n");
// 	// 	break;
// 	// case APIC_VIRTUAL_WIRE_NO_CONFIG:
// 	// 	pr_info("APIC: Switch to virtual wire mode setup with no configuration\n");
// 	// 	upmode = true;
// 	// 	break;
// 	// case APIC_SYMMETRIC_IO:
// 	// 	pr_info("APIC: Switch to symmetric I/O mode setup\n");
// 	// 	break;
// 	// case APIC_SYMMETRIC_IO_NO_ROUTING:
// 	// 	pr_info("APIC: Switch to symmetric I/O mode setup in no SMP routine\n");
// 	// 	break;
// 	// }

// 	// default_setup_apic_routing();
// 	// void __init default_setup_apic_routing(void)
// 	// {
// 		try_to_enable_x2apic(0);
// 	// }

// 	// if (x86_platform.apic_post_init)
// 	// 	x86_platform.apic_post_init();

// 	// apic_bsp_setup(upmode);
// }


enum {
	X2APIC_OFF,
	X2APIC_DISABLED,
	/* All states below here have X2APIC enabled */
	X2APIC_ON,
	X2APIC_ON_LOCKED
};
static int x2apic_state;

static bool x2apic_hw_locked(void)
{
	u64 ia32_cap;
	u64 msr;

	// ia32_cap = x86_read_arch_cap_msr();
	// if (ia32_cap & ARCH_CAP_XAPIC_DISABLE) {
	if (boot_cpu_data.x86_capa_bits.ARCH_CAPABILITIES_MSR) {
		rdmsrl(MSR_IA32_XAPIC_DISABLE_STATUS, msr);
		return (msr & LEGACY_XAPIC_DISABLED);
	}
	return false;
}

static void __x2apic_disable(void)
{
	u64 msr;

	if (!boot_cpu_data.x86_capa_bits.APIC_On_Chip)
		return;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (!(msr & X2APIC_ENABLE))
		return;
	/* Disable xapic and x2apic first and then reenable xapic mode */
	wrmsrl(MSR_IA32_APICBASE, msr & ~(X2APIC_ENABLE | XAPIC_ENABLE));
	wrmsrl(MSR_IA32_APICBASE, msr & ~X2APIC_ENABLE);
	// printk_once(KERN_INFO "x2apic disabled\n");
}

static void __x2apic_enable(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (msr & X2APIC_ENABLE)
		return;
	if (!(msr & XAPIC_ENABLE))
		msr |= XAPIC_ENABLE;
	wrmsrl(MSR_IA32_APICBASE, msr | X2APIC_ENABLE);
	// printk_once(KERN_INFO "x2apic enabled\n");
}

/* Called from cpu_init() to enable x2apic on (secondary) cpus */
void x2apic_setup(void)
{
	/*
	 * Try to make the AP's APIC state match that of the BSP,  but if the
	 * BSP is unlocked and the AP is locked then there is a state mismatch.
	 * Warn about the mismatch in case a GP fault occurs due to a locked AP
	 * trying to be turned off.
	 */
	if (x2apic_state != X2APIC_ON_LOCKED && x2apic_hw_locked())
		x2apic_state = X2APIC_ON_LOCKED;
	// 	pr_warn("x2apic lock mismatch between BSP and AP.\n");
	/*
	 * If x2apic is not in ON or LOCKED state, disable it if already enabled
	 * from BIOS.
	 */
	if (x2apic_state < X2APIC_ON) {
		__x2apic_disable();
		// return;
	}
	__x2apic_enable();
}



// static __init void try_to_enable_x2apic(int remap_mode)
// {
// 	if (x2apic_state == X2APIC_DISABLED)
// 		return;

// 	// if (remap_mode != IRQ_REMAP_X2APIC_MODE) {
// 	// 	u32 apic_limit = 255;

// 	// 	/*
// 	// 	 * Using X2APIC without IR is not architecturally supported
// 	// 	 * on bare metal but may be supported in guests.
// 	// 	 */
// 	// 	if (!x86_init.hyper.x2apic_available()) {
// 	// 		pr_info("x2apic: IRQ remapping doesn't support X2APIC mode\n");
// 	// 		static __init void x2apic_disable(void)
// 	// 		{
// 	// 			u32 x2apic_id, state = x2apic_state;

// 	// 			x2apic_mode = 0;
// 	// 			x2apic_state = X2APIC_DISABLED;

// 	// 			if (state != X2APIC_ON)
// 	// 				return;

// 	// 			x2apic_id = read_apic_id();
// 	// 			if (x2apic_id >= 255)
// 	// 				panic("Cannot disable x2apic, id: %08x\n", x2apic_id);

// 	// 			if (x2apic_hw_locked()) {
// 	// 				pr_warn("Cannot disable locked x2apic, id: %08x\n", x2apic_id);
// 	// 				return;
// 	// 			}

// 	// 			__x2apic_disable();
// 	// 			register_lapic_address(mp_lapic_addr);
// 	// 		}
// 	// 		return;
// 	// 	}

// 	// 	/*
// 	// 	 * If the hypervisor supports extended destination ID in
// 	// 	 * MSI, that increases the maximum APIC ID that can be
// 	// 	 * used for non-remapped IRQ domains.
// 	// 	 */
// 	// 	if (x86_init.hyper.msi_ext_dest_id()) {
// 	// 		virt_ext_dest_id = 1;
// 	// 		apic_limit = 32767;
// 	// 	}

// 	// 	/*
// 	// 	 * Without IR, all CPUs can be addressed by IOAPIC/MSI only
// 	// 	 * in physical mode, and CPUs with an APIC ID that cannot
// 	// 	 * be addressed must not be brought online.
// 	// 	 */
// 	// 	x2apic_set_max_apicid(apic_limit);
// 	// 	x2apic_phys = 1;
// 	// }

// 	// static __init void x2apic_enable(void)
// 	// {
// 		if (x2apic_state != X2APIC_OFF)
// 			return;

// 		x2apic_mode = 1;
// 		x2apic_state = X2APIC_ON;
// 		__x2apic_enable();
// 	// }
// }

void __init check_x2apic(void)
{
	if (boot_cpu_data.x86_capa_bits.x2APIC &&
		apic_is_x2apic_enabled()) {
		// pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
		x2apic_mode = 1;
		if (x2apic_hw_locked())
			x2apic_state = X2APIC_ON_LOCKED;
		else
			x2apic_state = X2APIC_ON;
	} else if (!boot_cpu_data.x86_capa_bits.x2APIC) {
		x2apic_state = X2APIC_DISABLED;
	}
}


/**
 * init_apic_mappings - initialize APIC mappings
 */
void __init init_apic_mappings(void)
{
	// unsigned int new_apicid;

	// if (apic_validate_deadline_timer())
	// 	pr_info("TSC deadline timer available\n");

	// if (x2apic_mode) {
	// 	boot_cpu_physical_apicid = read_apic_id();
	// 	return;
	// }

	// /* If no local APIC can be found return early */
	// if (!smp_found_config && detect_init_APIC()) {
	// 	/* lets NOP'ify apic operations */
	// 	pr_info("APIC: disable apic facility\n");
	// 	apic_disable();
	// } else {
	// 	apic_phys = mp_lapic_addr;

	// 	/*
	// 	 * If the system has ACPI MADT tables or MP info, the LAPIC
	// 	 * address is already registered.
	// 	 */
	// 	if (!acpi_lapic && !smp_found_config)
	// 		register_lapic_address(apic_phys);
			myos_kernel_physical_mapping_init(IO_APIC_DEFAULT_PHYS_BASE,
					IO_APIC_DEFAULT_PHYS_BASE + PAGE_SIZE);
	// }

	// /*
	//  * Fetch the APIC ID of the BSP in case we have a
	//  * default configuration (or the MP table is broken).
	//  */
	// new_apicid = read_apic_id();
	// if (boot_cpu_physical_apicid != new_apicid) {
	// 	boot_cpu_physical_apicid = new_apicid;
	// 	/*
	// 	 * yeah -- we lie about apic_version
	// 	 * in case if apic was disabled via boot option
	// 	 * but it's not a problem for SMP compiled kernel
	// 	 * since apic_intr_mode_select is prepared for such
	// 	 * a case and disable smp mode
	// 	 */
	// 	boot_cpu_apic_version = GET_APIC_VERSION(apic_read(APIC_LVR));
	// }
}
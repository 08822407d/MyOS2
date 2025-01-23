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
#include <linux/kernel/types.h>
// #include <linux/debug/ftrace.h>
// #include <linux/kernel/ioport.h>
// #include <linux/kernel/export.h>
// #include <linux/kernel/delay.h>
#include <linux/init/init.h>
// #include <linux/kernel/cpu.h>
// #include <linux/smp/smp.h>
// #include <linux/kernel/mm.h>

#include <asm/apic.h>
// #include <asm/io_apic.h>
// #include <asm/time.h>
// #include <asm/smp.h>
// #include <asm/tsc.h>
// #include <asm/cpu.h>
#include <asm/apicdef.h>
#include <asm/irq_vectors.h>
#include <insns/x86msr_const.h>
#include <processor/processor_arch_api.h>


int x2apic_mode;

uint64_t apicid_to_cpunr[CONFIG_NR_CPUS] = {
	[0 ... CONFIG_NR_CPUS-1] = -1,
};


/**
 * lapic_get_maxlvt - get the maximum number of local vector table entries
 */
int lapic_get_maxlvt(void)
{
	/*
	 * - we always have APIC integrated on 64bit mode
	 * - 82489DXs do not report # of LVT entries
	 */
	return GET_APIC_MAXLVT(apic_msr_read(APIC_LVR));
}

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
	int maxlvt;
	u32 v;

	/* APIC hasn't been mapped yet */
	// if (!x2apic_mode && !apic_phys)
	if (!x2apic_mode)
		return;

	maxlvt = lapic_get_maxlvt();
	/*
	 * Masking an LVT entry can trigger a local APIC error
	 * if the vector is zero. Mask LVTERR first to prevent this.
	 */
	if (maxlvt >= 3) {
		v = ERROR_APIC_VECTOR; /* any non-zero vector will do */
		apic_msr_write(APIC_LVTERR, v | APIC_LVT_MASKED);
	}
	/*
	 * Careful: we have to set masks only first to deassert
	 * any level-triggered sources.
	 */
	v = apic_msr_read(APIC_LVTT);
	apic_msr_write(APIC_LVTT, v | APIC_LVT_MASKED);
	v = apic_msr_read(APIC_LVT0);
	apic_msr_write(APIC_LVT0, v | APIC_LVT_MASKED);
	v = apic_msr_read(APIC_LVT1);
	apic_msr_write(APIC_LVT1, v | APIC_LVT_MASKED);
	if (maxlvt >= 4) {
		v = apic_msr_read(APIC_LVTPC);
		apic_msr_write(APIC_LVTPC, v | APIC_LVT_MASKED);
	}

	/* lets not touch this if we didn't frob it */
	if (maxlvt >= 5) {
		v = apic_msr_read(APIC_LVTTHMR);
		apic_msr_write(APIC_LVTTHMR, v | APIC_LVT_MASKED);
	}
	if (maxlvt >= 6) {
		v = apic_msr_read(APIC_LVTCMCI);
		if (!(v & APIC_LVT_MASKED))
			apic_msr_write(APIC_LVTCMCI, v | APIC_LVT_MASKED);
	}

	/*
	 * Clean APIC state for other OSs:
	 */
	apic_msr_write(APIC_LVTT, APIC_LVT_MASKED);
	apic_msr_write(APIC_LVT0, APIC_LVT_MASKED);
	apic_msr_write(APIC_LVT1, APIC_LVT_MASKED);
	if (maxlvt >= 3)
		apic_msr_write(APIC_LVTERR, APIC_LVT_MASKED);
	if (maxlvt >= 4)
		apic_msr_write(APIC_LVTPC, APIC_LVT_MASKED);

	if (maxlvt > 3)
		/* Clear ESR due to Pentium errata 3AP and 11AP */
		apic_msr_write(APIC_ESR, 0);
	apic_msr_read(APIC_ESR);
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
	value |= SPURIOUS_APIC_VECTOR;
	apic_msr_write(APIC_SPIV, value);

	/*
	 * Set up the virtual wire mode.
	 */
	apic_msr_write(APIC_LVT0, APIC_DM_EXTINT);
	value = APIC_DM_NMI;
	// if (apic_extnmi == APIC_EXTNMI_NONE)
	// 	value |= APIC_LVT_MASKED;
	apic_msr_write(APIC_LVT1, value);
}


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

	// x86_arch_cap_msr = x86_read_arch_cap_msr();
	// if (x86_arch_cap_msr & ARCH_CAP_XAPIC_DISABLE) {
	if (boot_cpu_data.x86_capa_bits.ARCH_CAPABILITIES_MSR) {
		rdmsrl(MSR_IA32_XAPIC_DISABLE_STATUS, &msr);
		return (msr & LEGACY_XAPIC_DISABLED);
	}
	return false;
}

static void __x2apic_disable(void)
{
	u64 msr;

	// if (!boot_cpu_has(X86_FEATURE_APIC))
	if (!boot_cpu_data.x86_capa_bits.APIC_On_Chip)
		return;

	rdmsrl(MSR_IA32_APICBASE, &msr);
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

	rdmsrl(MSR_IA32_APICBASE, &msr);
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
		return;
	}
	__x2apic_enable();
}


void __init myos_try_enable_x2apic(void)
{
	if (boot_cpu_data.x86_capa_bits.x2APIC) {
		__x2apic_enable();
		if(apic_is_x2apic_enabled()) {
			// pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
			x2apic_mode = 1;
			if (x2apic_hw_locked())
				x2apic_state = X2APIC_ON_LOCKED;
			else
				x2apic_state = X2APIC_ON;
		}
		init_bsp_APIC();
	} else if (!boot_cpu_data.x86_capa_bits.x2APIC) {
		x2apic_state = X2APIC_DISABLED;
	}
}
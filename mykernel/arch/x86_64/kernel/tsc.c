// SPDX-License-Identifier: GPL-2.0-only
// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/kernel.h>
#include <linux/kernel/sched.h>
#include <linux/sched/clock.h>
#include <linux/init/init.h>
#include <linux/kernel/export.h>
// #include <linux/timer.h>
// #include <linux/acpi_pmtmr.h>
// #include <linux/cpufreq.h>
#include <linux/kernel/delay.h>
#include <linux/smp/percpu.h>
// #include <linux/static_key.h>
// #include <linux/static_call.h>

#include <asm/hpet.h>
#include <asm/timer.h>
// #include <asm/vgtod.h>
#include <asm/time.h>
#include <asm/delay.h>
#include <asm/hypervisor.h>
// #include <asm/nmi.h>
#include <asm/x86_init.h>
// #include <asm/geode.h>
#include <asm/apic.h>
// #include <asm/intel-family.h>
// #include <asm/i8259.h>
// #include <asm/uv/uv.h>


#define __cyc2ns_read		__simple_cyc2ns_read
#define read_tsc			simple_read_tsc


uint __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */

uint __read_mostly tsc_khz;

#ifndef DUMMY_TSC_KHZ
#  define DUMMY_TSC_KHZ (3LL * 1000LL * 1000LL)	/* 3 GHz */
#endif // !DUMMY_TSC_KHZ
#define KHZ	1000

/*
 * TSC can be unstable due to cpufreq or due to unsynced TSCs
 */
static int __read_mostly tsc_unstable;
static uint __initdata tsc_early_khz;

static bool __use_tsc = false;


static u64 read_tsc(clocksrc_s *cs);


/*
 * Must mark VALID_FOR_HRES early such that when we unregister tsc_early
 * this one will immediately take over. We will only register if TSC has
 * been found good.
 */
static clocksrc_s clocksource_tsc = {
	.name				= "tsc",
	.rating				= 300,
	.read				= read_tsc,
	.mask				= CLOCKSOURCE_MASK(64),
	.flags				= CLOCK_SOURCE_IS_CONTINUOUS |
							CLOCK_SOURCE_VALID_FOR_HRES |
							CLOCK_SOURCE_MUST_VERIFY |
							CLOCK_SOURCE_VERIFY_PERCPU,
	// .id					= CSID_X86_TSC,
	// .vdso_clock_mode	= VDSO_CLOCKMODE_TSC,
	// .enable				= tsc_cs_enable,
	// .resume				= tsc_resume,
	// .mark_unstable		= tsc_cs_mark_unstable,
	// .tick_stable		= tsc_cs_tick_stable,
	.list				= LIST_HEAD_INIT(clocksource_tsc.list),
};

__always_inline void
__simple_cyc2ns_read(struct cyc2ns_data *data) {
	cyc2ns_s *c = this_cpu_ptr(&cyc2ns);
	data->cyc2ns_offset = READ_ONCE(c->data.cyc2ns_offset);
	data->cyc2ns_mul    = READ_ONCE(c->data.cyc2ns_mul);
	data->cyc2ns_shift  = READ_ONCE(c->data.cyc2ns_shift);
}

PREFIX_STATIC_AWLWAYS_INLINE void
cyc2ns_read_begin(struct cyc2ns_data *data) {
	// preempt_disable_notrace();
	__cyc2ns_read(data);
}

PREFIX_STATIC_AWLWAYS_INLINE void
cyc2ns_read_end(void) {
	// preempt_enable_notrace();
}

/*
 * Accelerators for sched_clock()
 * convert from cycles(64bits) => nanoseconds (64bits)
 *  basic equation:
 *              ns = cycles / (freq / ns_per_sec)
 *              ns = cycles * (ns_per_sec / freq)
 *              ns = cycles * (10^9 / (cpu_khz * 10^3))
 *              ns = cycles * (10^6 / cpu_khz)
 *
 *      Then we use scaling math (suggested by george@mvista.com) to get:
 *              ns = cycles * (10^6 * SC / cpu_khz) / SC
 *              ns = cycles * cyc2ns_scale / SC
 *
 *      And since SC is a constant power of two, we can convert the div
 *  into a shift. The larger SC is, the more accurate the conversion, but
 *  cyc2ns_scale needs to be a 32-bit value so that 32-bit multiplication
 *  (64-bit result) can be used.
 *
 *  We can use khz divisor instead of mhz to keep a better precision.
 *  (mathieu.desnoyers@polymtl.ca)
 *
 *                      -johnstul@us.ibm.com "math is hard, lets go shopping!"
 */
PREFIX_STATIC_AWLWAYS_INLINE ulonglong
__cycles_2_ns(ulonglong cyc) {
	struct cyc2ns_data data;
	ulonglong ns;

	__cyc2ns_read(&data);

	ns = data.cyc2ns_offset;
	ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);

	return ns;
}

PREFIX_STATIC_AWLWAYS_INLINE ulonglong
cycles_2_ns(ulonglong cyc) {
	ulonglong ns;
	// preempt_disable_notrace();
	ns = __cycles_2_ns(cyc);
	// preempt_enable_notrace();
	return ns;
}

static void
__set_cyc2ns_scale(ulong khz, int cpu, ulonglong tsc_now) {
	ulonglong ns_now;
	struct cyc2ns_data data;
	cyc2ns_s *c2n;

	ns_now = cycles_2_ns(tsc_now);

	/*
	 * Compute a new multiplier as per the above comment and ensure our
	 * time function is continuous; see the comment near struct
	 * cyc2ns_data.
	 */
	clocks_calc_mult_shift(&data.cyc2ns_mul, &data.cyc2ns_shift, khz,
			       NSEC_PER_MSEC, 0);

	/*
	 * cyc2ns_shift is exported via arch_perf_update_userpage() where it is
	 * not expected to be greater than 31 due to the original published
	 * conversion algorithm shifting a 32-bit value (now specifies a 64-bit
	 * value) - refer perf_event_mmap_page documentation in perf_event.h.
	 */
	if (data.cyc2ns_shift == 32) {
		data.cyc2ns_shift = 31;
		data.cyc2ns_mul >>= 1;
	}

	data.cyc2ns_offset = ns_now -
		mul_u64_u32_shr(tsc_now, data.cyc2ns_mul, data.cyc2ns_shift);

	c2n = per_cpu_ptr(&cyc2ns, cpu);

	// raw_write_seqcount_latch(&c2n->seq);
	// c2n->data[0] = data;
	// raw_write_seqcount_latch(&c2n->seq);
	// c2n->data[1] = data;
	c2n->data = data;
}

// static void
// set_cyc2ns_scale(ulong khz, int cpu, ulonglong tsc_now) {
// 	ulong flags;

// 	local_irq_save(flags);
// 	sched_clock_idle_sleep_event();

// 	if (khz)
// 		__set_cyc2ns_scale(khz, cpu, tsc_now);

// 	sched_clock_idle_wakeup_event();
// 	local_irq_restore(flags);
// }

/*
 * Initialize cyc2ns for boot cpu
 */
static void __init
cyc2ns_init_boot_cpu(void) {
	cyc2ns_s *c2n = this_cpu_ptr(&cyc2ns);

	// seqcount_latch_init(&c2n->seq);
	__set_cyc2ns_scale(tsc_khz, smp_processor_id(), rdtsc());
}


static u64
simple_read_tsc(clocksrc_s *cs) {
	return (u64)rdtsc();
}




/*
 * Scheduler clock - returns current time in nanosec units.
 */
noinstr u64
native_sched_clock(void) {
	if (__use_tsc) {
		u64 tsc_now = rdtsc();
		/* return the value in ns */
		return cycles_2_ns(tsc_now);
	}

	/*
	 * Fall back to jiffies if there's no TSC available:
	 * ( But note that we still use it if the TSC is marked
	 *   unstable. We do this because unlike Time Of Day,
	 *   the scheduler clock tolerates small errors and it's
	 *   very important for it to be as fast as the platform
	 *   can achieve it. )
	 */
	/* No locking but a rare wrong value is not a big deal: */
	return (jiffies_64 - INITIAL_JIFFIES) * (1000000000 / HZ);
}



/**
 * native_calibrate_tsc - determine TSC frequency
 * Determine TSC frequency via CPUID, else return 0.
 */
ulong native_calibrate_tsc(void) {
	uint eax_denominator, ebx_numerator, ecx_hz, edx;
	uint crystal_khz;

	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		return 0;

	if (boot_cpu_data.cpuid_level < CPUID_LEAF_TSC)
		return 0;

	eax_denominator = ebx_numerator = ecx_hz = edx = 0;

	/* CPUID 15H TSC/Crystal ratio, plus optionally Crystal Hz */
	cpuid(CPUID_LEAF_TSC, &eax_denominator, &ebx_numerator, &ecx_hz, &edx);

	if (ebx_numerator == 0 || eax_denominator == 0)
		return 0;

	crystal_khz = ecx_hz / 1000;

	// /*
	//  * Denverton SoCs don't report crystal clock, and also don't support
	//  * CPUID_LEAF_FREQ for the calculation below, so hardcode the 25MHz
	//  * crystal clock.
	//  */
	// if (crystal_khz == 0 &&
	// 		boot_cpu_data.x86_vfm == INTEL_ATOM_GOLDMONT_D)
	// 	crystal_khz = 25000;

	// /*
	//  * TSC frequency reported directly by CPUID is a "hardware reported"
	//  * frequency and is the most accurate one so far we have. This
	//  * is considered a known frequency.
	//  */
	// if (crystal_khz != 0)
	// 	setup_force_cpu_cap(X86_FEATURE_TSC_KNOWN_FREQ);

	/*
	 * Some Intel SoCs like Skylake and Kabylake don't report the crystal
	 * clock, but we can easily calculate it to a high degree of accuracy
	 * by considering the crystal ratio and the CPU speed.
	 */
	if (crystal_khz == 0 && boot_cpu_data.cpuid_level >= CPUID_LEAF_FREQ) {
		unsigned int eax_base_mhz, ebx, ecx, edx;

		cpuid(CPUID_LEAF_FREQ, &eax_base_mhz, &ebx, &ecx, &edx);
		crystal_khz = eax_base_mhz * 1000 *
			eax_denominator / ebx_numerator;
	}

	if (crystal_khz == 0)
		return 0;

	// /*
	//  * For Atom SoCs TSC is the only reliable clocksource.
	//  * Mark TSC reliable so no watchdog on it.
	//  */
	// if (boot_cpu_data.x86_vfm == INTEL_ATOM_GOLDMONT)
	// 	setup_force_cpu_cap(X86_FEATURE_TSC_RELIABLE);

#ifdef CONFIG_X86_LOCAL_APIC
	/*
	 * The local APIC appears to be fed by the core crystal clock
	 * (which sounds entirely sensible). We can set the global
	 * lapic_timer_period here to avoid having to calibrate the APIC
	 * timer later.
	 */
	lapic_timer_period = crystal_khz * 1000 / HZ;
#endif

	return crystal_khz * ebx_numerator / eax_denominator;
}

static ulong
cpu_khz_from_cpuid(void) {
	uint eax_base_mhz, ebx_max_mhz, ecx_bus_mhz, edx;

	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		return 0;

	if (boot_cpu_data.cpuid_level < CPUID_LEAF_FREQ)
		return 0;

	eax_base_mhz = ebx_max_mhz = ecx_bus_mhz = edx = 0;

	// cpuid(CPUID_LEAF_FREQ, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);
	cpuid(CPUID_LEAF_TSC, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);

	return eax_base_mhz * 1000;
}

/**
 * native_calibrate_cpu_early - can calibrate the cpu early in boot
 */
ulong native_calibrate_cpu_early(void) {
	ulong flags, fast_calibrate = cpu_khz_from_cpuid();

	// if (!fast_calibrate)
	// 	fast_calibrate = cpu_khz_from_msr();
	// if (!fast_calibrate) {
	// 	local_irq_save(flags);
	// 	fast_calibrate = quick_pit_calibrate();
	// 	local_irq_restore(flags);
	// }
	if (!fast_calibrate)
		fast_calibrate = DUMMY_TSC_KHZ;

	return fast_calibrate;
}

/**
 * native_calibrate_cpu - calibrate the cpu
 */
static ulong
native_calibrate_cpu(void) {
	ulong tsc_freq = native_calibrate_cpu_early();

	// if (!tsc_freq)
	// 	tsc_freq = pit_hpet_ptimer_calibrate_cpu();

	return tsc_freq;
}



static bool __init
determine_cpu_tsc_frequencies(bool early) {
	/* Make sure that cpu and tsc are not already calibrated */
	WARN_ON(cpu_khz || tsc_khz);

	if (early) {
		cpu_khz = x86_platform.calibrate_cpu();
		if (tsc_early_khz) {
			tsc_khz = tsc_early_khz;
		} else {
			tsc_khz = x86_platform.calibrate_tsc();
			clocksource_tsc.freq_khz = tsc_khz;
		}
	} else {
		/* We should not be here with non-native cpu calibration */
		// WARN_ON(x86_platform.calibrate_cpu != native_calibrate_cpu);
		// cpu_khz = pit_hpet_ptimer_calibrate_cpu();
	}

	/*
	 * Trust non-zero tsc_khz as authoritative,
	 * and use it to sanity check cpu_khz,
	 * which will be off if system timer is off.
	 */
	if (tsc_khz == 0)
		tsc_khz = cpu_khz;
	else if (abs(cpu_khz - tsc_khz) * 10 > tsc_khz)
		cpu_khz = tsc_khz;

	if (tsc_khz == 0)
		return false;

	pr_info("Detected %lu.%03lu MHz processor\n",
		(ulong)cpu_khz / KHZ,
		(ulong)cpu_khz % KHZ);

	if (cpu_khz != tsc_khz) {
		pr_info("Detected %lu.%03lu MHz TSC",
			(ulong)tsc_khz / KHZ,
			(ulong)tsc_khz % KHZ);
	}
	return true;
}

static ulong __init
get_loops_per_jiffy(void) {
	u64 lpj = (u64)tsc_khz * KHZ;

	do_div(lpj, HZ);
	return lpj;
}

static void __init
tsc_enable_sched_clock(void) {
	loops_per_jiffy = get_loops_per_jiffy();
	// use_tsc_delay();

	// /* Sanitize TSC ADJUST before cyc2ns gets initialized */
	// tsc_store_and_check_tsc_adjust(true);
	cyc2ns_init_boot_cpu();
	// static_branch_enable(&__use_tsc);
	__use_tsc = true;
}

void __init
tsc_early_init(void) {
	// if (!boot_cpu_has(X86_FEATURE_TSC))
	// 	return;
	// /* Don't change UV TSC multi-chassis synchronization */
	// if (is_early_uv_system())
	// 	return;
	if (!determine_cpu_tsc_frequencies(true))
		return;
	tsc_enable_sched_clock();
}

void __init
tsc_init(void) {
	// if (!cpu_feature_enabled(X86_FEATURE_TSC)) {
	// 	setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
	// 	return;
	// }

	// /*
	//  * native_calibrate_cpu_early can only calibrate using methods that are
	//  * available early in boot.
	//  */
	// if (x86_platform.calibrate_cpu == native_calibrate_cpu_early)
	// 	x86_platform.calibrate_cpu = native_calibrate_cpu;

	if (!tsc_khz) {
		/* We failed to determine frequencies earlier, try again */
		if (!determine_cpu_tsc_frequencies(false)) {
			// mark_tsc_unstable("could not calculate TSC khz");
			// setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
			return;
		}
		tsc_enable_sched_clock();
	}

	// cyc2ns_init_secondary_cpus();

	// if (!no_sched_irq_time)
	// 	enable_sched_clock_irqtime();

	// lpj_fine = get_loops_per_jiffy();

	// check_system_tsc_reliable();

	// if (unsynchronized_tsc()) {
	// 	mark_tsc_unstable("TSCs unsynchronized");
	// 	return;
	// }

	// if (tsc_clocksource_reliable || no_tsc_watchdog)
	// 	tsc_disable_clocksource_watchdog();

	// clocksource_register_khz(&clocksource_tsc_early, tsc_khz);
	// detect_art();

	// if (boot_cpu_data.x86_capa_bits.Invariant_TSC)
	// 	clocksource_register_khz(&clocksource_tsc, tsc_khz);


	// Now MyOS2 running on vmware can't init TSC
	clocksource_mark_unstable(&clocksource_tsc);
}
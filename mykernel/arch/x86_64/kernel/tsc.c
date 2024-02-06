// SPDX-License-Identifier: GPL-2.0-only
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/kernel.h>
#include <linux/kernel/sched.h>
#include <linux/sched/clock.h>
#include <linux/init/init.h>
// #include <linux/export.h>
// #include <linux/timer.h>
// #include <linux/acpi_pmtmr.h>
// #include <linux/cpufreq.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/clocksource.h>
#include <linux/smp/percpu.h>
#include <linux/kernel/timex.h>
// #include <linux/static_key.h>
// #include <linux/static_call.h>

#include <asm/hpet.h>
#include <asm/timer.h>
// #include <asm/vgtod.h>
#include <asm/time.h>
#include <asm/delay.h>
// #include <asm/hypervisor.h>
// #include <asm/nmi.h>
// #include <asm/x86_init.h>
// #include <asm/geode.h>
#include <asm/apic.h>
// #include <asm/intel-family.h>
// #include <asm/i8259.h>
// #include <asm/uv/uv.h>


#include <linux/kernel/jiffies.h>

unsigned int __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */

unsigned int __read_mostly tsc_khz;

#define KHZ	1000

/*
 * TSC can be unstable due to cpufreq or due to unsynced TSCs
 */
static int __read_mostly tsc_unstable;
static unsigned int __initdata tsc_early_khz;

static bool __use_tsc = false;




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

static __always_inline unsigned long long cycles_2_ns(unsigned long long cyc) {
	// struct cyc2ns_data data;
	// unsigned long long ns;

	// cyc2ns_read_begin(&data);

	// ns = data.cyc2ns_offset;
	// ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);

	// cyc2ns_read_end();

	// return ns;
}


/*
 * Scheduler clock - returns current time in nanosec units.
 */
noinstr u64 native_sched_clock(void)
{
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

// /*
//  * How many MSB values do we want to see? We aim for
//  * a maximum error rate of 500ppm (in practice the
//  * real error is much smaller), but refuse to spend
//  * more than 50ms on it.
//  */
// #define MAX_QUICK_PIT_MS 50
// #define MAX_QUICK_PIT_ITERATIONS (MAX_QUICK_PIT_MS * PIT_TICK_RATE / 1000 / 256)

// static unsigned long quick_pit_calibrate(void)
// {
// // 	int i;
// // 	u64 tsc, delta;
// // 	unsigned long d1, d2;

// // 	if (!has_legacy_pic())
// // 		return 0;

// // 	/* Set the Gate high, disable speaker */
// // 	outb((inb(0x61) & ~0x02) | 0x01, 0x61);

// // 	/*
// // 	 * Counter 2, mode 0 (one-shot), binary count
// // 	 *
// // 	 * NOTE! Mode 2 decrements by two (and then the
// // 	 * output is flipped each time, giving the same
// // 	 * final output frequency as a decrement-by-one),
// // 	 * so mode 0 is much better when looking at the
// // 	 * individual counts.
// // 	 */
// // 	outb(0xb0, 0x43);

// // 	/* Start at 0xffff */
// // 	outb(0xff, 0x42);
// // 	outb(0xff, 0x42);

// // 	/*
// // 	 * The PIT starts counting at the next edge, so we
// // 	 * need to delay for a microsecond. The easiest way
// // 	 * to do that is to just read back the 16-bit counter
// // 	 * once from the PIT.
// // 	 */
// // 	pit_verify_msb(0);

// // 	if (pit_expect_msb(0xff, &tsc, &d1)) {
// // 		for (i = 1; i <= MAX_QUICK_PIT_ITERATIONS; i++) {
// // 			if (!pit_expect_msb(0xff-i, &delta, &d2))
// // 				break;

// // 			delta -= tsc;

// // 			/*
// // 			 * Extrapolate the error and fail fast if the error will
// // 			 * never be below 500 ppm.
// // 			 */
// // 			if (i == 1 &&
// // 			    d1 + d2 >= (delta * MAX_QUICK_PIT_ITERATIONS) >> 11)
// // 				return 0;

// // 			/*
// // 			 * Iterate until the error is less than 500 ppm
// // 			 */
// // 			if (d1+d2 >= delta >> 11)
// // 				continue;

// // 			/*
// // 			 * Check the PIT one more time to verify that
// // 			 * all TSC reads were stable wrt the PIT.
// // 			 *
// // 			 * This also guarantees serialization of the
// // 			 * last cycle read ('d2') in pit_expect_msb.
// // 			 */
// // 			if (!pit_verify_msb(0xfe - i))
// // 				break;
// // 			goto success;
// // 		}
// // 	}
// // 	pr_info("Fast TSC calibration failed\n");
// // 	return 0;

// // success:
// // 	/*
// // 	 * Ok, if we get here, then we've seen the
// // 	 * MSB of the PIT decrement 'i' times, and the
// // 	 * error has shrunk to less than 500 ppm.
// // 	 *
// // 	 * As a result, we can depend on there not being
// // 	 * any odd delays anywhere, and the TSC reads are
// // 	 * reliable (within the error).
// // 	 *
// // 	 * kHz = ticks / time-in-seconds / 1000;
// // 	 * kHz = (t2 - t1) / (I * 256 / PIT_TICK_RATE) / 1000
// // 	 * kHz = ((t2 - t1) * PIT_TICK_RATE) / (I * 256 * 1000)
// // 	 */
// // 	delta *= PIT_TICK_RATE;
// // 	do_div(delta, i*256*1000);
// // 	pr_info("Fast TSC calibration using PIT\n");
// // 	return delta;
// }

// /**
//  * native_calibrate_tsc
//  * Determine TSC frequency via CPUID, else return 0.
//  */
// unsigned long native_calibrate_tsc(void)
// {
// 	unsigned int eax_denominator, ebx_numerator, ecx_hz, edx;
// 	unsigned int crystal_khz;

// 	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
// 		return 0;

// 	if (boot_cpu_data.cpuid_level < 0x15)
// 		return 0;

// 	eax_denominator = ebx_numerator = ecx_hz = edx = 0;

// 	/* CPUID 15H TSC/Crystal ratio, plus optionally Crystal Hz */
// 	cpuid(0x15, &eax_denominator, &ebx_numerator, &ecx_hz, &edx);

// 	if (ebx_numerator == 0 || eax_denominator == 0)
// 		return 0;

// 	crystal_khz = ecx_hz / 1000;

// 	// /*
// 	//  * Denverton SoCs don't report crystal clock, and also don't support
// 	//  * CPUID.0x16 for the calculation below, so hardcode the 25MHz crystal
// 	//  * clock.
// 	//  */
// 	// if (crystal_khz == 0 &&
// 	// 		boot_cpu_data.x86_model == INTEL_FAM6_ATOM_GOLDMONT_D)
// 	// 	crystal_khz = 25000;

// 	// /*
// 	//  * TSC frequency reported directly by CPUID is a "hardware reported"
// 	//  * frequency and is the most accurate one so far we have. This
// 	//  * is considered a known frequency.
// 	//  */
// 	// if (crystal_khz != 0)
// 	// 	setup_force_cpu_cap(X86_FEATURE_TSC_KNOWN_FREQ);

// 	/*
// 	 * Some Intel SoCs like Skylake and Kabylake don't report the crystal
// 	 * clock, but we can easily calculate it to a high degree of accuracy
// 	 * by considering the crystal ratio and the CPU speed.
// 	 */
// 	if (crystal_khz == 0 && boot_cpu_data.cpuid_level >= 0x16) {
// 		unsigned int eax_base_mhz, ebx, ecx, edx;

// 		cpuid(0x16, &eax_base_mhz, &ebx, &ecx, &edx);
// 		crystal_khz = eax_base_mhz * 1000 *
// 			eax_denominator / ebx_numerator;
// 	}

// 	if (crystal_khz == 0)
// 		return 0;

// 	// /*
// 	//  * For Atom SoCs TSC is the only reliable clocksource.
// 	//  * Mark TSC reliable so no watchdog on it.
// 	//  */
// 	// if (boot_cpu_data.x86_model == INTEL_FAM6_ATOM_GOLDMONT)
// 	// 	setup_force_cpu_cap(X86_FEATURE_TSC_RELIABLE);

// 	// /*
// 	//  * The local APIC appears to be fed by the core crystal clock
// 	//  * (which sounds entirely sensible). We can set the global
// 	//  * lapic_timer_period here to avoid having to calibrate the APIC
// 	//  * timer later.
// 	//  */
// 	// lapic_timer_period = crystal_khz * 1000 / HZ;

// 	return crystal_khz * ebx_numerator / eax_denominator;
// }



// static unsigned long cpu_khz_from_cpuid(void)
// {
// 	unsigned int eax_base_mhz, ebx_max_mhz, ecx_bus_mhz, edx;

// 	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
// 		return 0;

// 	if (boot_cpu_data.cpuid_level < 0x16)
// 		return 0;

// 	eax_base_mhz = ebx_max_mhz = ecx_bus_mhz = edx = 0;

// 	cpuid(0x16, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);

// 	return eax_base_mhz * 1000;
// }

// /*
//  * calibrate cpu using pit, hpet, and ptimer methods. They are available
//  * later in boot after acpi is initialized.
//  */
// static unsigned long pit_hpet_ptimer_calibrate_cpu(void)
// {
// 	// u64 tsc1, tsc2, delta, ref1, ref2;
// 	// unsigned long tsc_pit_min = ULONG_MAX, tsc_ref_min = ULONG_MAX;
// 	// unsigned long flags, latch, ms;
// 	// int hpet = is_hpet_enabled(), i, loopmin;

// 	// /*
// 	//  * Run 5 calibration loops to get the lowest frequency value
// 	//  * (the best estimate). We use two different calibration modes
// 	//  * here:
// 	//  *
// 	//  * 1) PIT loop. We set the PIT Channel 2 to oneshot mode and
// 	//  * load a timeout of 50ms. We read the time right after we
// 	//  * started the timer and wait until the PIT count down reaches
// 	//  * zero. In each wait loop iteration we read the TSC and check
// 	//  * the delta to the previous read. We keep track of the min
// 	//  * and max values of that delta. The delta is mostly defined
// 	//  * by the IO time of the PIT access, so we can detect when
// 	//  * any disturbance happened between the two reads. If the
// 	//  * maximum time is significantly larger than the minimum time,
// 	//  * then we discard the result and have another try.
// 	//  *
// 	//  * 2) Reference counter. If available we use the HPET or the
// 	//  * PMTIMER as a reference to check the sanity of that value.
// 	//  * We use separate TSC readouts and check inside of the
// 	//  * reference read for any possible disturbance. We discard
// 	//  * disturbed values here as well. We do that around the PIT
// 	//  * calibration delay loop as we have to wait for a certain
// 	//  * amount of time anyway.
// 	//  */

// 	// /* Preset PIT loop values */
// 	// latch = CAL_LATCH;
// 	// ms = CAL_MS;
// 	// loopmin = CAL_PIT_LOOPS;

// 	// for (i = 0; i < 3; i++) {
// 	// 	unsigned long tsc_pit_khz;

// 	// 	/*
// 	// 	 * Read the start value and the reference count of
// 	// 	 * hpet/pmtimer when available. Then do the PIT
// 	// 	 * calibration, which will take at least 50ms, and
// 	// 	 * read the end value.
// 	// 	 */
// 	// 	local_irq_save(flags);
// 	// 	tsc1 = tsc_read_refs(&ref1, hpet);
// 	// 	tsc_pit_khz = pit_calibrate_tsc(latch, ms, loopmin);
// 	// 	tsc2 = tsc_read_refs(&ref2, hpet);
// 	// 	local_irq_restore(flags);

// 	// 	/* Pick the lowest PIT TSC calibration so far */
// 	// 	tsc_pit_min = min(tsc_pit_min, tsc_pit_khz);

// 	// 	/* hpet or pmtimer available ? */
// 	// 	if (ref1 == ref2)
// 	// 		continue;

// 	// 	/* Check, whether the sampling was disturbed */
// 	// 	if (tsc1 == ULLONG_MAX || tsc2 == ULLONG_MAX)
// 	// 		continue;

// 	// 	tsc2 = (tsc2 - tsc1) * 1000000LL;
// 	// 	if (hpet)
// 	// 		tsc2 = calc_hpet_ref(tsc2, ref1, ref2);
// 	// 	else
// 	// 		tsc2 = calc_pmtimer_ref(tsc2, ref1, ref2);

// 	// 	tsc_ref_min = min(tsc_ref_min, (unsigned long) tsc2);

// 	// 	/* Check the reference deviation */
// 	// 	delta = ((u64) tsc_pit_min) * 100;
// 	// 	do_div(delta, tsc_ref_min);

// 	// 	/*
// 	// 	 * If both calibration results are inside a 10% window
// 	// 	 * then we can be sure, that the calibration
// 	// 	 * succeeded. We break out of the loop right away. We
// 	// 	 * use the reference value, as it is more precise.
// 	// 	 */
// 	// 	if (delta >= 90 && delta <= 110) {
// 	// 		pr_info("PIT calibration matches %s. %d loops\n",
// 	// 			hpet ? "HPET" : "PMTIMER", i + 1);
// 	// 		return tsc_ref_min;
// 	// 	}

// 	// 	/*
// 	// 	 * Check whether PIT failed more than once. This
// 	// 	 * happens in virtualized environments. We need to
// 	// 	 * give the virtual PC a slightly longer timeframe for
// 	// 	 * the HPET/PMTIMER to make the result precise.
// 	// 	 */
// 	// 	if (i == 1 && tsc_pit_min == ULONG_MAX) {
// 	// 		latch = CAL2_LATCH;
// 	// 		ms = CAL2_MS;
// 	// 		loopmin = CAL2_PIT_LOOPS;
// 	// 	}
// 	// }

// 	// /*
// 	//  * Now check the results.
// 	//  */
// 	// if (tsc_pit_min == ULONG_MAX) {
// 	// 	/* PIT gave no useful value */
// 	// 	pr_warn("Unable to calibrate against PIT\n");

// 	// 	/* We don't have an alternative source, disable TSC */
// 	// 	if (!hpet && !ref1 && !ref2) {
// 	// 		pr_notice("No reference (HPET/PMTIMER) available\n");
// 	// 		return 0;
// 	// 	}

// 	// 	/* The alternative source failed as well, disable TSC */
// 	// 	if (tsc_ref_min == ULONG_MAX) {
// 	// 		pr_warn("HPET/PMTIMER calibration failed\n");
// 	// 		return 0;
// 	// 	}

// 	// 	/* Use the alternative source */
// 	// 	pr_info("using %s reference calibration\n",
// 	// 		hpet ? "HPET" : "PMTIMER");

// 	// 	return tsc_ref_min;
// 	// }

// 	// /* We don't have an alternative source, use the PIT calibration value */
// 	// if (!hpet && !ref1 && !ref2) {
// 	// 	pr_info("Using PIT calibration value\n");
// 	// 	return tsc_pit_min;
// 	// /**
//  * native_calibrate_cpu - calibrate the cpu
//  */
// static unsigned long native_calibrate_cpu(void)
// {
// 	// unsigned long native_calibrate_cpu_early(void)
// 	// {
// 		// unsigned long flags, tsc_freq = cpu_khz_from_cpuid();

// 		// if (!tsc_freq)
// 		// 	tsc_freq = cpu_khz_from_msr();
// 		// if (!tsc_freq) {
// 		// 	local_irq_save(flags);
// 		// 	tsc_freq = quick_pit_calibrate();
// 		// 	local_irq_restore(flags);
// 		// 		// unsigned long flags, tsc_freq = cpu_khz_from_cpuid();

// 		// if (!tsc_freq)
// 		// 	tsc_freq = cpu_khz_from_msr();
// 		// if (!tsc_freq) {
// 		// 	local_irq_save(flags);
// 		// 	tsc_freq = quick_pit_calibrate();
// 		// 	local_irq_restore(flags);
// 		// }}
// 	// }

// 	// if (!tsc_freq)
// 	// 	tsc_freq = pit_hpet_ptimer_calibrate_cpu();

// 	// return tsc_freq;
// 	return 0;
// }


// static bool __init determine_cpu_tsc_frequencies(bool early)
// {
// 	if (early) {
// 		cpu_khz = native_calibrate_cpu();
// 		if (tsc_early_khz)
// 			tsc_khz = tsc_early_khz;
// 		else
// 			tsc_khz = native_calibrate_tsc();
// 	} else {
// 		// /* We should not be here with non-native cpu calibration */
// 		// cpu_khz = pit_hpet_ptimer_calibrate_cpu();
// 	}

// 	/*
// 	 * Trust non-zero tsc_khz as authoritative,
// 	 * and use it to sanity check cpu_khz,
// 	 * which will be off if system timer is off.
// 	 */
// 	if (tsc_khz == 0)
// 		tsc_khz = cpu_khz;
// 	else if (abs(cpu_khz - tsc_khz) * 10 > tsc_khz)
// 		cpu_khz = tsc_khz;

// 	if (tsc_khz == 0)
// 		return false;

// 	// pr_info("Detected %lu.%03lu MHz processor\n",
// 	// 	(unsigned long)cpu_khz / KHZ,
// 	// 	(unsigned long)cpu_khz % KHZ);

// 	// if (cpu_khz != tsc_khz) {
// 	// 	pr_info("Detected %lu.%03lu MHz TSC",
// 	// 		(unsigned long)tsc_khz / KHZ,
// 	// 		(unsigned long)tsc_khz % KHZ);
// 	// }
// 	return true;
// }

// static void __init tsc_enable_sched_clock(void)
// {
// 	// get_loops_per_jiffy(void)
// 	// {
// 		loops_per_jiffy = (u64)tsc_khz * KHZ;
// 		do_div(loops_per_jiffy, HZ);
// 	// }
// 	// use_tsc_delay();

// 	// /* Sanitize TSC ADJUST before cyc2ns gets initialized */
// 	// tsc_store_and_check_tsc_adjust(true);
// 	// cyc2ns_init_boot_cpu();
// 	__use_tsc = true;
// }}

// 	// /* The alternative source failed, use the PIT calibration value */
// 	// if (tsc_ref_min == ULONG_MAX) {
// 	// 	pr_warn("HPET/PMTIMER calibration failed. Using PIT calibration.\n");
// 	// 	return tsc_pit_min;
// 	// }

// 	// /*
// 	//  * The calibration values differ too much. In doubt, we use
// 	//  * the PIT value as we know that there are PMTIMERs around
// 	//  * running at double speed. At least we let the user know:
// 	//  */
// 	// pr_warn("PIT calibration deviates from %s: %lu %lu\n",
// 	// 	hpet ? "HPET" : "PMTIMER", tsc_pit_min, tsc_ref_min);
// 	// pr_info("Using PIT calibration value\n");
// 	// return tsc_pit_min;
// }

// /**
//  * native_calibrate_cpu - calibrate the cpu
//  */
// static unsigned long native_calibrate_cpu(void)
// {
// 	// unsigned long native_calibrate_cpu_early(void)
// 	// {
// 		// unsigned long flags, tsc_freq = cpu_khz_from_cpuid();

// 		// if (!tsc_freq)
// 		// 	tsc_freq = cpu_khz_from_msr();
// 		// if (!tsc_freq) {
// 		// 	local_irq_save(flags);
// 		// 	tsc_freq = quick_pit_calibrate();
// 		// 	local_irq_restore(flags);
// 		// 		// unsigned long flags, tsc_freq = cpu_khz_from_cpuid();

// 		// if (!tsc_freq)
// 		// 	tsc_freq = cpu_khz_from_msr();
// 		// if (!tsc_freq) {
// 		// 	local_irq_save(flags);
// 		// 	tsc_freq = quick_pit_calibrate();
// 		// 	local_irq_restore(flags);
// 		// }}
// 	// }

// 	// if (!tsc_freq)
// 	// 	tsc_freq = pit_hpet_ptimer_calibrate_cpu();

// 	// return tsc_freq;
// 	return 0;
// }


// static bool __init determine_cpu_tsc_frequencies(bool early)
// {
// 	if (early) {
// 		cpu_khz = native_calibrate_cpu();
// 		if (tsc_early_khz)
// 			tsc_khz = tsc_early_khz;
// 		else
// 			tsc_khz = native_calibrate_tsc();
// 	} else {
// 		// /* We should not be here with non-native cpu calibration */
// 		// cpu_khz = pit_hpet_ptimer_calibrate_cpu();
// 	}

// 	/*
// 	 * Trust non-zero tsc_khz as authoritative,
// 	 * and use it to sanity check cpu_khz,
// 	 * which will be off if system timer is off.
// 	 */
// 	if (tsc_khz == 0)
// 		tsc_khz = cpu_khz;
// 	else if (abs(cpu_khz - tsc_khz) * 10 > tsc_khz)
// 		cpu_khz = tsc_khz;

// 	if (tsc_khz == 0)
// 		return false;

// 	// pr_info("Detected %lu.%03lu MHz processor\n",
// 	// 	(unsigned long)cpu_khz / KHZ,
// 	// 	(unsigned long)cpu_khz % KHZ);

// 	// if (cpu_khz != tsc_khz) {
// 	// 	pr_info("Detected %lu.%03lu MHz TSC",
// 	// 		(unsigned long)tsc_khz / KHZ,
// 	// 		(unsigned long)tsc_khz % KHZ);
// 	// }
// 	return true;
// }

// static void __init tsc_enable_sched_clock(void)
// {
// 	// get_loops_per_jiffy(void)
// 	// {
// 		loops_per_jiffy = (u64)tsc_khz * KHZ;
// 		do_div(loops_per_jiffy, HZ);
// 	// }
// 	// use_tsc_delay();

// 	// /* Sanitize TSC ADJUST before cyc2ns gets initialized */
// 	// tsc_store_and_check_tsc_adjust(true);
// 	// cyc2ns_init_boot_cpu();
// 	__use_tsc = true;
// }

void __init tsc_early_init(void)
{
	// if (!determine_cpu_tsc_frequencies(true))
	// 	return;
	// tsc_enable_sched_clock();
}
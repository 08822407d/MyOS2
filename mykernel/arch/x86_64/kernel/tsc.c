// SPDX-License-Identifier: GPL-2.0-only
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

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
// #include <asm/hypervisor.h>
// #include <asm/nmi.h>
// #include <asm/x86_init.h>
// #include <asm/geode.h>
#include <asm/apic.h>
// #include <asm/intel-family.h>
// #include <asm/i8259.h>
// #include <asm/uv/uv.h>


#define cyc2ns_read_begin	simple_cyc2ns_read_begin
#define read_tsc			simple_read_tsc


unsigned int __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */

unsigned int __read_mostly tsc_khz;

#ifndef DUMMY_TSC_KHZ
#  define DUMMY_TSC (3LL * 1000LL * 1000LL)	/* 3 GHz */
#endif // !DUMMY_TSC_KHZ
#define KHZ	1000

/*
 * TSC can be unstable due to cpufreq or due to unsynced TSCs
 */
static int __read_mostly tsc_unstable;
static unsigned int __initdata tsc_early_khz;

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


PREFIX_STATIC_AWLWAYS_INLINE void
simple_cyc2ns_read_begin(struct cyc2ns_data *data) {
	// preempt_disable_notrace();
	cyc2ns_s *c = this_cpu_ptr(&cyc2ns);
	data->cyc2ns_offset = READ_ONCE(c->data.cyc2ns_offset);
	data->cyc2ns_mul    = READ_ONCE(c->data.cyc2ns_mul);
	data->cyc2ns_shift  = READ_ONCE(c->data.cyc2ns_shift);
}

PREFIX_STATIC_AWLWAYS_INLINE void
cyc2ns_read_end(void) {
	// preempt_enable_notrace();
}

static u64
simple_read_tsc(clocksrc_s *cs) {
	return (u64)rdtsc();
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
cycles_2_ns(ulonglong cyc) {
	struct cyc2ns_data data;
	ulonglong ns;

	cyc2ns_read_begin(&data);
	ns = data.cyc2ns_offset;
	ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);
	cyc2ns_read_end();

	return ns;
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



void __init
tsc_early_init(void) {
	// if (!boot_cpu_has(X86_FEATURE_TSC))
	// 	return;
	// /* Don't change UV TSC multi-chassis synchronization */
	// if (is_early_uv_system())
	// 	return;
	// if (!determine_cpu_tsc_frequencies(true))
	// 	return;
	// 在虚拟机中运行时，无法正确获取TSC频率，使用一个假的3GHz值
	// 因为早期还没设置好percpu area, cyc2ns的percpu静态初始化时直接赋值
	// tsc_enable_sched_clock();
	__use_tsc = true;
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

	// if (!tsc_khz) {
	// 	/* We failed to determine frequencies earlier, try again */
	// 	if (!determine_cpu_tsc_frequencies(false)) {
	// 		mark_tsc_unstable("could not calculate TSC khz");
	// 		setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
	// 		return;
	// 	}
	// 	tsc_enable_sched_clock();
	// }

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
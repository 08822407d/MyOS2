// SPDX-License-Identifier: GPL-2.0-or-later
/*  KVM paravirtual clock driver. A clocksource implementation
    Copyright (C) 2008 Glauber de Oliveira Costa, Red Hat Inc.
*/

// #include <linux/clocksource.h>
#include <linux/kernel/kvm_para.h>
// #include <asm/pvclock.h>
// #include <asm/msr.h>
// #include <asm/apic.h>
// #include <linux/percpu.h>
// #include <linux/hardirq.h>
// #include <linux/cpuhotplug.h>
// #include <linux/sched.h>
// #include <linux/sched/clock.h>
#include <linux/kernel/mm.h>
// #include <linux/slab.h>
// #include <linux/set_memory.h>
// #include <linux/cc_platform.h>

// #include <asm/hypervisor.h>
// #include <asm/x86_init.h>
// #include <asm/kvmclock.h>


#include <linux/init/init.h>



static int kvmclock __initdata = 1;
static int kvmclock_vsyscall __initdata = 1;
static int msr_kvm_system_time __ro_after_init;
static int msr_kvm_wall_clock __ro_after_init;
static u64 kvm_sched_clock_offset __ro_after_init;


void __init kvmclock_init(void)
{
	u8 flags;

	if (!kvm_para_available() || !kvmclock)
		return;

	if (kvm_para_has_feature(KVM_FEATURE_CLOCKSOURCE2)) {
		msr_kvm_system_time = MSR_KVM_SYSTEM_TIME_NEW;
		msr_kvm_wall_clock = MSR_KVM_WALL_CLOCK_NEW;
	} else if (kvm_para_has_feature(KVM_FEATURE_CLOCKSOURCE)) {
		msr_kvm_system_time = MSR_KVM_SYSTEM_TIME;
		msr_kvm_wall_clock = MSR_KVM_WALL_CLOCK;
	} else {
		return;
	}

	// if (cpuhp_setup_state(CPUHP_BP_PREPARE_DYN, "kvmclock:setup_percpu",
	// 		      kvmclock_setup_percpu, NULL) < 0) {
	// 	return;
	// }

	pr_info("kvm-clock: Using msrs %x and %x",
		msr_kvm_system_time, msr_kvm_wall_clock);

	// this_cpu_write(hv_clock_per_cpu, &hv_clock_boot[0]);
	// kvm_register_clock("primary cpu clock");
	// pvclock_set_pvti_cpu0_va(hv_clock_boot);

	// if (kvm_para_has_feature(KVM_FEATURE_CLOCKSOURCE_STABLE_BIT))
	// 	pvclock_set_flags(PVCLOCK_TSC_STABLE_BIT);

	// flags = pvclock_read_flags(&hv_clock_boot[0].pvti);
	// kvm_sched_clock_init(flags & PVCLOCK_TSC_STABLE_BIT);

	// x86_platform.calibrate_tsc = kvm_get_tsc_khz;
	// x86_platform.calibrate_cpu = kvm_get_tsc_khz;
	// x86_platform.get_wallclock = kvm_get_wallclock;
	// x86_platform.set_wallclock = kvm_set_wallclock;
// #ifdef CONFIG_X86_LOCAL_APIC
// 	x86_cpuinit.early_percpu_clock_init = kvm_setup_secondary_clock;
// #endif
	// x86_platform.save_sched_clock_state = kvm_save_sched_clock_state;
	// x86_platform.restore_sched_clock_state = kvm_restore_sched_clock_state;
	// kvm_get_preset_lpj();

	// /*
	//  * X86_FEATURE_NONSTOP_TSC is TSC runs at constant rate
	//  * with P/T states and does not stop in deep C-states.
	//  *
	//  * Invariant TSC exposed by host means kvmclock is not necessary:
	//  * can use TSC as clocksource.
	//  *
	//  */
	// if (boot_cpu_has(X86_FEATURE_CONSTANT_TSC) &&
	//     boot_cpu_has(X86_FEATURE_NONSTOP_TSC) &&
	//     !check_tsc_unstable())
	// 	kvm_clock.rating = 299;

	// clocksource_register_hz(&kvm_clock, NSEC_PER_SEC);
	// pv_info.name = "KVM";
}
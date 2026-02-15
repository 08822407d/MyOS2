/*
 * VMware Detection code.
 *
 * Copyright (C) 2008, VMware, Inc.
 * Author : Alok N Kataria <akataria@vmware.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

// #include <linux/dmi.h>
#include <linux/init/init.h>
#include <linux/kernel/export.h>
// #include <linux/clocksource.h>
// #include <linux/cpu.h>
// #include <linux/efi.h>
// #include <linux/reboot.h>
// #include <linux/static_call.h>
// #include <asm/div64.h>
#include <asm/x86_init.h>
#include <asm/hypervisor.h>
#include <asm/timer.h>
#include <asm/apic.h>
#include <asm/vmware.h>
// #include <asm/svm.h>

// #undef pr_fmt
// #define pr_fmt(fmt)	"vmware: " fmt

#define CPUID_VMWARE_INFO_LEAF			0x40000000
#define CPUID_VMWARE_FEATURES_LEAF		0x40000010

#define GETVCPU_INFO_LEGACY_X2APIC		BIT(3)
#define GETVCPU_INFO_VCPU_RESERVED		BIT(31)

#define STEALCLOCK_NOT_AVAILABLE		(-1)
#define STEALCLOCK_DISABLED				0
#define STEALCLOCK_ENABLED				1


static ulong vmware_tsc_khz		__ro_after_init;
static u8 vmware_hypercall_mode	__ro_after_init;


// PREFIX_STATIC_INLINE int
// __vmware_platform(void) {
// 	u32 eax, ebx, ecx;

// 	eax = vmware_hypercall3(VMWARE_CMD_GETVERSION, 0, &ebx, &ecx);
// 	return eax != UINT_MAX && ebx == VMWARE_HYPERVISOR_MAGIC;
// }

static ulong
vmware_get_tsc_khz(void) {
	return vmware_tsc_khz;
}


// #ifdef CONFIG_PARAVIRT

// 	static struct cyc2ns_data vmware_cyc2ns __ro_after_init;
// 	static bool vmw_sched_clock __initdata = true;
// 	// static DEFINE_PER_CPU_DECRYPTED(struct vmware_steal_time, vmw_steal_time) __aligned(64);
// 	// static bool has_steal_clock;
// 	// static bool steal_acc __initdata = true; /* steal time accounting */

// 	static noinstr u64
// 	vmware_sched_clock(void) {
// 		unsigned long long ns;

// 		ns =
// 			mul_u64_u32_shr(rdtsc(),
// 				vmware_cyc2ns.cyc2ns_mul,
// 				vmware_cyc2ns.cyc2ns_shift
// 			);
// 		ns -= vmware_cyc2ns.cyc2ns_offset;
// 		return ns;
// 	}

// 	static void __init
// 	vmware_cyc2ns_setup(void) {
// 		struct cyc2ns_data *d = &vmware_cyc2ns;
// 		ulonglong tsc_now = rdtsc();

// 		clocks_calc_mult_shift(
// 			&d->cyc2ns_mul,
// 			&d->cyc2ns_shift,
// 			vmware_tsc_khz,
// 			NSEC_PER_MSEC, 0
// 		);
// 		d->cyc2ns_offset =
// 			mul_u64_u32_shr(tsc_now,
// 				d->cyc2ns_mul,
// 				d->cyc2ns_shift
// 			);

// 		pr_info("using clock offset of %llu ns\n", d->cyc2ns_offset);
// 	}


// 	static void __init
// 	vmware_paravirt_ops_setup(void) {
// 		pv_info.name = "VMware hypervisor";
// 		pv_ops.cpu.io_delay = paravirt_nop;

// 		if (vmware_tsc_khz == 0)
// 			return;

// 		vmware_cyc2ns_setup();

// 		if (vmw_sched_clock)
// 			paravirt_set_sched_clock(vmware_sched_clock);

// 		if (vmware_is_stealclock_available()) {
// 			has_steal_clock = true;
// 			static_call_update(pv_steal_clock, vmware_steal_clock);

// 			/* We use reboot notifier only to disable steal clock */
// 			register_reboot_notifier(&vmware_pv_reboot_nb);

// 	#ifdef CONFIG_SMP
// 			smp_ops.smp_prepare_boot_cpu =
// 				vmware_smp_prepare_boot_cpu;
// 			if (cpuhp_setup_state_nocalls(CPUHP_AP_ONLINE_DYN,
// 							"x86/vmware:online",
// 							vmware_cpu_online,
// 							vmware_cpu_down_prepare) < 0)
// 				pr_err("vmware_guest: Failed to install cpu hotplug callbacks\n");
// 	#else
// 			vmware_guest_cpu_init();
// 	#endif
// 		}
// 	}
// #else
// #  define vmware_paravirt_ops_setup() do {} while (0)
// #endif


static void __init
vmware_platform_setup(void) {
	u32 eax, ebx, ecx;
	u64 lpj, tsc_khz;

	eax = vmware_hypercall3(VMWARE_CMD_GETHZ, UINT_MAX, &ebx, &ecx);

	if (ebx != UINT_MAX) {
		lpj = tsc_khz = eax | (((u64)ebx) << 32);
		do_div(tsc_khz, 1000);
		WARN_ON(tsc_khz >> 32);
		pr_info("TSC freq read from hypervisor : %lu.%03lu MHz\n",
			(ulong) tsc_khz / 1000,
			(ulong) tsc_khz % 1000);

		// if (!preset_lpj) {
		// 	do_div(lpj, HZ);
		// 	preset_lpj = lpj;
		// }

		vmware_tsc_khz = tsc_khz;
		x86_platform.calibrate_tsc = vmware_get_tsc_khz;
		x86_platform.calibrate_cpu = vmware_get_tsc_khz;

#ifdef CONFIG_X86_LOCAL_APIC
		/* Skip lapic calibration since we know the bus frequency. */
		lapic_timer_period = ecx / HZ;
		pr_info("Host bus clock speed read from hypervisor : %u Hz\n",
			ecx);
#endif
	} else {
		pr_warn("Failed to get TSC freq from the hypervisor\n");
	}

	// if (cc_platform_has(CC_ATTR_GUEST_SEV_SNP) && !efi_enabled(EFI_BOOT))
	// 	x86_init.mpparse.find_mptable = mpparse_find_mptable;

	// vmware_paravirt_ops_setup();

#ifdef CONFIG_X86_IO_APIC
	no_timer_check = 1;
#endif

// 	vmware_set_capabilities();
}

static u8 __init
vmware_select_hypercall(void) {
	int eax, ebx, ecx, edx;

	cpuid(CPUID_VMWARE_FEATURES_LEAF, &eax, &ebx, &ecx, &edx);
	return (ecx & (CPUID_VMWARE_FEATURES_ECX_VMMCALL |
		       CPUID_VMWARE_FEATURES_ECX_VMCALL));
}

/*
 * While checking the dmi string information, just checking the product
 * serial key should be enough, as this will always have a VMware
 * specific string when running under VMware hypervisor.
 * If !boot_cpu_has(X86_FEATURE_HYPERVISOR), vmware_hypercall_mode
 * intentionally defaults to 0.
 */
static u32 __init
vmware_platform(void) {
	// if (boot_cpu_has(X86_FEATURE_HYPERVISOR)) {
	if (boot_cpu_data.x86_capability[CPUID_1_ECX] & (1<<31)) {
		uint eax;
		uint hyper_vendor_id[3];

		cpuid(CPUID_VMWARE_INFO_LEAF, &eax, (uint *)&hyper_vendor_id[0],
		      (uint *)&hyper_vendor_id[1], (uint *)&hyper_vendor_id[2]);
		if (!memcmp(hyper_vendor_id, "VMwareVMware", 12)) {
			if (eax >= CPUID_VMWARE_FEATURES_LEAF)
				vmware_hypercall_mode =
					vmware_select_hypercall();

			pr_info("hypercall mode: 0x%02x\n",
				(uint) vmware_hypercall_mode);

			return CPUID_VMWARE_INFO_LEAF;
		}
	}
	// else if (dmi_available && dmi_name_in_serial("VMware") &&
	// 	   __vmware_platform())
	// 	return 1;

	return 0;
}

/* Checks if hypervisor supports x2apic without VT-D interrupt remapping. */
static bool __init
vmware_legacy_x2apic_available(void) {
	u32 eax;

	eax = vmware_hypercall1(VMWARE_CMD_GETVCPU_INFO, 0);
	return !(eax & GETVCPU_INFO_VCPU_RESERVED) &&
		(eax & GETVCPU_INFO_LEGACY_X2APIC);
}




const __initconst hypervisor_x86_s x86_hyper_vmware = {
	.name						= "VMware",
	.detect						= vmware_platform,
	.type						= X86_HYPER_VMWARE,
	.init.init_platform			= vmware_platform_setup,
	.init.x2apic_available		= vmware_legacy_x2apic_available,
// #ifdef CONFIG_AMD_MEM_ENCRYPT
// 	.runtime.sev_es_hcall_prepare	= vmware_sev_es_hcall_prepare,
// 	.runtime.sev_es_hcall_finish	= vmware_sev_es_hcall_finish,
// #endif
};
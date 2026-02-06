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
// #include <linux/init.h>
// #include <linux/export.h>
// #include <linux/clocksource.h>
// #include <linux/cpu.h>
// #include <linux/efi.h>
// #include <linux/reboot.h>
// #include <linux/static_call.h>
// #include <asm/div64.h>
// #include <asm/x86_init.h>
// #include <asm/hypervisor.h>
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


static inline int
__vmware_platform(void) {
	u32 eax, ebx, ecx;

	eax = vmware_hypercall3(VMWARE_CMD_GETVERSION, 0, &ebx, &ecx);
	return eax != UINT_MAX && ebx == VMWARE_HYPERVISOR_MAGIC;
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


void __init simple_init_hypervisor_platform(void)
{
	// -> detect_hypervisor_vendor() ->
	vmware_platform();
}
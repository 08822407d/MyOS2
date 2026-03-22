// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * KVM paravirt_ops implementation
 *
 * Copyright (C) 2007, Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 * Copyright IBM Corporation, 2007
 *   Authors: Anthony Liguori <aliguori@us.ibm.com>
 */

// #define pr_fmt(fmt) "kvm-guest: " fmt

// #include <linux/context_tracking.h>
#include <linux/init/init.h>
// #include <linux/irq.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/kvm_para.h>
#include <asm/kvm_para.h>
#include <linux/kernel/cpu.h>
#include <linux/kernel/mm.h>
// #include <linux/highmem.h>
// #include <linux/hardirq.h>
// #include <linux/notifier.h>
// #include <linux/reboot.h>
// #include <linux/hash.h>
// #include <linux/sched.h>
// #include <linux/slab.h>
// #include <linux/kprobes.h>
// #include <linux/nmi.h>
// #include <linux/swait.h>
// #include <linux/syscore_ops.h>
// #include <linux/cc_platform.h>
// #include <linux/efi.h>
#include <asm/timer.h>
#include <asm/cpu.h>
// #include <asm/traps.h>
// #include <asm/desc.h>
// #include <asm/tlbflush.h>
// #include <asm/apic.h>
// #include <asm/apicdef.h>
#include <asm/hypervisor.h>
// #include <asm/mtrr.h>
// #include <asm/tlb.h>
// #include <asm/cpuidle_haltpoll.h>
// #include <asm/msr.h>
// #include <asm/ptrace.h>
// #include <asm/reboot.h>
// #include <asm/svm.h>
// #include <asm/e820/api.h>



static noinline uint32_t
__kvm_cpuid_base(void) {
	if (boot_cpu_data.cpuid_level < 0)
		return 0;	/* So we don't blow up on old processors */

	// if (boot_cpu_has(X86_FEATURE_HYPERVISOR))
	if (boot_cpu_data.x86_capability[CPUID_1_ECX] & (1<<31))
		return cpuid_base_hypervisor(KVM_SIGNATURE, 0);

	return 0;
}

static inline uint32_t
kvm_cpuid_base(void) {
	static int kvm_cpuid_base = -1;

	if (kvm_cpuid_base == -1)
		kvm_cpuid_base = __kvm_cpuid_base();

	return kvm_cpuid_base;
}

bool kvm_para_available(void) {
	return kvm_cpuid_base() != 0;
}
EXPORT_SYMBOL_GPL(kvm_para_available);

uint kvm_arch_para_features(void) {
	return cpuid_eax(kvm_cpuid_base() | KVM_CPUID_FEATURES);
}

#define kvm_detect __kvm_cpuid_base


static void __init
kvm_init_platform(void) {
	// u64 tolud = PFN_PHYS(e820__end_of_low_ram_pfn());
	// /*
	//  * Note, hardware requires variable MTRR ranges to be power-of-2 sized
	//  * and naturally aligned.  But when forcing guest MTRR state, Linux
	//  * doesn't program the forced ranges into hardware.  Don't bother doing
	//  * the math to generate a technically-legal range.
	//  */
	// struct mtrr_var_range pci_hole = {
	// 	.base_lo = tolud | X86_MEMTYPE_UC,
	// 	.mask_lo = (u32)(~(SZ_4G - tolud - 1)) | MTRR_PHYSMASK_V,
	// 	.mask_hi = (BIT_ULL(boot_cpu_data.x86_phys_bits) - 1) >> 32,
	// };

	// if (cc_platform_has(CC_ATTR_GUEST_MEM_ENCRYPT) &&
	//     kvm_para_has_feature(KVM_FEATURE_MIGRATION_CONTROL)) {
	// 	unsigned long nr_pages;
	// 	int i;

	// 	pv_ops.mmu.notify_page_enc_status_changed =
	// 		kvm_sev_hc_page_enc_status;

	// 	/*
	// 	 * Reset the host's shared pages list related to kernel
	// 	 * specific page encryption status settings before we load a
	// 	 * new kernel by kexec. Reset the page encryption status
	// 	 * during early boot instead of just before kexec to avoid SMP
	// 	 * races during kvm_pv_guest_cpu_reboot().
	// 	 * NOTE: We cannot reset the complete shared pages list
	// 	 * here as we need to retain the UEFI/OVMF firmware
	// 	 * specific settings.
	// 	 */

	// 	for (i = 0; i < e820_table->nr_entries; i++) {
	// 		struct e820_entry *entry = &e820_table->entries[i];

	// 		if (entry->type != E820_TYPE_RAM)
	// 			continue;

	// 		nr_pages = DIV_ROUND_UP(entry->size, PAGE_SIZE);

	// 		kvm_sev_hypercall3(KVM_HC_MAP_GPA_RANGE, entry->addr,
	// 			       nr_pages,
	// 			       KVM_MAP_GPA_RANGE_ENCRYPTED | KVM_MAP_GPA_RANGE_PAGE_SZ_4K);
	// 	}

	// 	/*
	// 	 * Ensure that _bss_decrypted section is marked as decrypted in the
	// 	 * shared pages list.
	// 	 */
	// 	early_set_mem_enc_dec_hypercall((unsigned long)__start_bss_decrypted,
	// 					__end_bss_decrypted - __start_bss_decrypted, 0);

	// 	/*
	// 	 * If not booted using EFI, enable Live migration support.
	// 	 */
	// 	if (!efi_enabled(EFI_BOOT))
	// 		wrmsrq(MSR_KVM_MIGRATION_CONTROL,
	// 		       KVM_MIGRATION_READY);
	// }
	kvmclock_init();
	// x86_platform.apic_post_init = kvm_apic_init;

	// /*
	//  * Set WB as the default cache mode for SEV-SNP and TDX, with a single
	//  * UC range for the legacy PCI hole, e.g. so that devices that expect
	//  * to get UC/WC mappings don't get surprised with WB.
	//  */
	// guest_force_mtrr_state(&pci_hole, 1, MTRR_TYPE_WRBACK);
}

const __initconst hypervisor_x86_s x86_hyper_kvm = {
	.name							= "KVM",
	.detect							= kvm_detect,
	.type							= X86_HYPER_KVM,
// 	.init.guest_late_init			= kvm_guest_init,
	.init.x2apic_available			= kvm_para_available,
// 	.init.msi_ext_dest_id			= kvm_msi_ext_dest_id,
	.init.init_platform				= kvm_init_platform,
// #if defined(CONFIG_AMD_MEM_ENCRYPT)
// 	.runtime.sev_es_hcall_prepare	= kvm_sev_es_hcall_prepare,
// 	.runtime.sev_es_hcall_finish	= kvm_sev_es_hcall_finish,
// #endif
};
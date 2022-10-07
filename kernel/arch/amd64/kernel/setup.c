// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 1995  Linus Torvalds
 *
 * This file contains the setup_arch() code, which handles the architecture-dependent
 * parts of early kernel initialization.
 */
// #include <linux/acpi.h>
// #include <linux/console.h>
// #include <linux/crash_dump.h>
// #include <linux/dma-map-ops.h>
// #include <linux/dmi.h>
// #include <linux/efi.h>
// #include <linux/init_ohci1394_dma.h>
// #include <linux/initrd.h>
// #include <linux/iscsi_ibft.h>
#include <linux/mm/memblock.h>
// #include <linux/panic_notifier.h>
// #include <linux/pci.h>
// #include <linux/root_dev.h>
// #include <linux/hugetlb.h>
// #include <linux/tboot.h>
// #include <linux/usb/xhci-dbgp.h>
// #include <linux/static_call.h>
// #include <linux/swiotlb.h>

#include <uapi/mount.h>

// #include <xen/xen.h>

// #include <asm/apic.h>
// #include <asm/numa.h>
// #include <asm/bios_ebda.h>
// #include <asm/bugs.h>
// #include <asm/cpu.h>
// #include <asm/efi.h>
// #include <asm/gart.h>
// #include <asm/hypervisor.h>
// #include <asm/io_apic.h>
// #include <asm/kasan.h>
// #include <asm/kaslr.h>
// #include <asm/mce.h>
// #include <asm/memtype.h>
// #include <asm/mtrr.h>
// #include <asm/realmode.h>
// #include <asm/olpc_ofw.h>
// #include <asm/pci-direct.h>
// #include <asm/prom.h>
// #include <asm/proto.h>
// #include <asm/thermal.h>
// #include <asm/unwind.h>
// #include <asm/vsyscall.h>
// #include <linux/vmalloc.h>



#include <linux/init/init.h>
#include <linux/kernel/sizes.h>
#include <linux/kernel/math.h>

#include <asm/setup.h>

/*
 * max_low_pfn_mapped: highest directly mapped pfn < 4 GB
 * max_pfn_mapped:     highest directly mapped pfn > 4 GB
 *
 * The direct mapping only covers E820_TYPE_RAM regions, so the ranges and gaps are
 * represented by pfn_mapped[].
 */
unsigned long max_low_pfn_mapped;
unsigned long max_pfn_mapped;

// #ifdef CONFIG_DMI
// RESERVE_BRK(dmi_alloc, 65536);
// #endif


// /*
//  * Range of the BSS area. The size of the BSS area is determined
//  * at link time, with RESERVE_BRK() facility reserving additional
//  * chunks.
//  */
// unsigned long _brk_start = (unsigned long)__brk_base;
// unsigned long _brk_end   = (unsigned long)__brk_base;

// struct boot_params boot_params;

// /*
//  * These are the four main kernel memory regions, we put them into
//  * the resource tree so that kdump tools and other debugging tools
//  * recover it:
//  */

// static struct resource rodata_resource = {
// 	.name	= "Kernel rodata",
// 	.start	= 0,
// 	.end	= 0,
// 	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
// };

// static struct resource data_resource = {
// 	.name	= "Kernel data",
// 	.start	= 0,
// 	.end	= 0,
// 	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
// };

// static struct resource code_resource = {
// 	.name	= "Kernel code",
// 	.start	= 0,
// 	.end	= 0,
// 	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
// };

// static struct resource bss_resource = {
// 	.name	= "Kernel bss",
// 	.start	= 0,
// 	.end	= 0,
// 	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
// };


static void __init early_reserve_memory(void)
{
	/*
	 * Reserve the memory occupied by the kernel between _text and
	 * __end_of_kernel_reserve symbols. Any kernel sections after the
	 * __end_of_kernel_reserve symbol must be explicitly reserved with a
	 * separate memblock_reserve() or they will be discarded.
	 */
	// memblock_reserve(__pa_symbol(_text),
	// 		 (unsigned long)__end_of_kernel_reserve - (unsigned long)_text);
extern char _k_phys_start;
extern char _k_virt_start;
extern char _end;
	memblock_reserve(round_down((phys_addr_t)&_k_phys_start, PAGE_SIZE),
					round_up((phys_addr_t)&_end, PAGE_SIZE) -
					round_down((phys_addr_t)&_k_virt_start, PAGE_SIZE));

	/*
	 * The first 4Kb of memory is a BIOS owned area, but generally it is
	 * not listed as such in the E820 table.
	 *
	 * Reserve the first 64K of memory since some BIOSes are known to
	 * corrupt low memory. After the real mode trampoline is allocated the
	 * rest of the memory below 640k is reserved.
	 *
	 * In addition, make sure page 0 is always reserved because on
	 * systems with L1TF its contents can be leaked to user processes.
	 */
	memblock_reserve(0, SZ_64K);

	// early_reserve_initrd();

	// memblock_x86_reserve_range_setup_data();

	// reserve_ibft_region();
	// reserve_bios_regions();
	// trim_snb_memory();
}

/*
 * Determine if we were loaded by an EFI loader.  If so, then we have also been
 * passed the efi memmap, systab, etc., so we should use these data structures
 * for initialization.  Note, the efi init code path is determined by the
 * global efi_enabled. This allows the same kernel image to be used on existing
 * systems (with a traditional BIOS) as well as on EFI systems.
 */
/*
 * setup_arch - architecture-specific boot-time initializations
 *
 * Note: On x86_64, fixmaps are ready for use even before this is called.
 */

void __init setup_arch(char **cmdline_p)
{
	/*
	 * Do some memory reservations *before* memory is added to memblock, so
	 * memblock allocations won't overwrite it.
	 *
	 * After this point, everything still needed from the boot loader or
	 * firmware or kernel text should be early reserved or marked not RAM in
	 * e820. All other memory is free game.
	 *
	 * This call needs to happen before e820__memory_setup() which calls the
	 * xen_memory_setup() on Xen dom0 which relies on the fact that those
	 * early reservations have happened already.
	 */
	early_reserve_memory();
}
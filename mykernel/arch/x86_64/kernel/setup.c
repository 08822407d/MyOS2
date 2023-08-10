// source: linux-6.4.9

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
#include <linux/block/efi.h>
// #include <linux/ima.h>
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
// #include <linux/random.h>

#include <uapi/linux/mount.h>

// #include <xen/xen.h>

#include <asm/apic.h>
// #include <asm/numa.h>
// #include <asm/bios_ebda.h>
#include <asm/bugs.h>
// #include <asm/cacheinfo.h>
#include <asm/cpu.h>
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


#include <asm/sections.h>
#include <asm/e820-api.h>

#include <obsolete/proto.h>
#include <obsolete/glo.h>

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

unsigned long _brk_start = (unsigned long)__brk_base;
unsigned long _brk_end   = (unsigned long)__brk_base;

// struct boot_params boot_params;

/*
 * These are the four main kernel memory regions, we put them into
 * the resource tree so that kdump tools and other debugging tools
 * recover it:
 */

static resource_s rodata_resource = {
	.name	= "Kernel rodata",
	.start	= 0,
	.end	= 0,
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
};

static resource_s data_resource = {
	.name	= "Kernel data",
	.start	= 0,
	.end	= 0,
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
};

static resource_s code_resource = {
	.name	= "Kernel code",
	.start	= 0,
	.end	= 0,
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
};

static resource_s bss_resource = {
	.name	= "Kernel bss",
	.start	= 0,
	.end	= 0,
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM
};


cpuinfo_x86_s boot_cpu_data __read_mostly;


void * __init extend_brk(size_t size, size_t align)
{
	size_t mask = align - 1;
	void *ret;

	BUG_ON(_brk_start == 0);
	BUG_ON(align & mask);

	_brk_end = (_brk_end + mask) & ~mask;
	// BUG_ON((char *)(_brk_end + size) > __brk_limit);

	ret = (void *)_brk_end;
	_brk_end += size;

	memset(ret, 0, size);

	return ret;
}

static void __init reserve_brk(void)
{
	if (_brk_end > _brk_start)
		memblock_reserve(__pa(_brk_start),
				_brk_end - _brk_start);

	/* Mark brk area as locked down and no longer taking any
	   new allocations */
	_brk_start = 0;
}

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
	memblock_reserve((phys_addr_t)&_k_phys_start,
					(phys_addr_t)&_end - (phys_addr_t)&_k_virt_start);

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
	// void __init reserve_bios_regions(void)
	// {
		phys_addr_t bios_start = 0x9F000;
		memblock_reserve(bios_start, SZ_1M - bios_start);
	// }
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
extern void myos_early_init_arch_data(size_t lcpu_nr);
extern void myos_init_arch(size_t cpu_idx);
extern void myos_init_smp(size_t lcpu_nr);


	// printk(KERN_INFO "Command line: %s\n", boot_command_line);
	boot_cpu_data.x86_phys_bits = MAX_PHYSMEM_BITS;
	
	early_cpu_init();

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
	// 将一些保留内存段在memblock里标记为已使用,比如bios,kernel等已占用空间
	early_reserve_memory();
	// 将固件生成的e820分布提取存储到内核结构中
	e820__memory_setup();

	setup_initial_init_mm(_text, _etext, _edata, (void *)-1ULL);

	code_resource.start = __pa(_text);
	code_resource.end = __pa(_etext)-1;
	rodata_resource.start = __pa(__start_rodata);
	rodata_resource.end = __pa(__end_rodata)-1;
	data_resource.start = __pa(_sdata);
	data_resource.end = __pa(_edata)-1;
	bss_resource.start = __pa(__bss_start);
	bss_resource.end = __pa(__bss_stop)-1;

	/*
	 * partially used pages are not usable - thus
	 * we are rounding upwards:
	 */
	// 计算内存最大页数目
	max_pfn = e820__end_of_ram_pfn();
	max_possible_pfn = max_pfn;

	// check_x2apic();

	// /* need this before calling reserve_initrd */
	max_low_pfn = max_pfn;
	high_memory = (void *)__va(max_pfn * PAGE_SIZE - 1) + 1;

	// 为内核页映射预留内存空间并构建映射树
	early_alloc_pgt_buf();

	/*
	 * Need to conclude brk, before e820__memblock_setup()
	 * it could use memblock_find_in_range, could overlap with
	 * brk area.
	 */
	reserve_brk();

	// 这里用e820获取的内存分布初始化memblock分配器
	e820__memblock_setup();
	// 现在可以使用memblock分配内存了


	myos_early_init_arch_data(kparam.nr_lcpu);

	myos_init_arch(0);
	// 初始化x86架构物理页映射
	init_mem_mapping();


	myos_early_init_task(kparam.nr_lcpu);
	myos_init_smp(kparam.nr_lcpu);
	myos_init_video();


	// x86_init.paging.pagetable_init()
	paging_init();
}
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Low level x86 E820 memory map handling functions.
 *
 * The firmware and bootloader passes us the "E820 table", which is the primary
 * physical memory layout description available about x86 systems.
 *
 * The kernel takes the E820 memory layout and optionally modifies it with
 * quirks and other tweaks, and feeds that into the generic Linux memory
 * allocation code routines via a platform independent interface (memblock, etc.).
 */
// #include <linux/crash_dump.h>
#include <linux/mm/memblock.h>
// #include <linux/suspend.h>
// #include <linux/acpi.h>
// #include <linux/firmware-map.h>
// #include <linux/sort.h>
// #include <linux/memory_hotplug.h>

#include <asm/e820-api.h>
#include <asm/setup.h>
#include <asm/pgtable_64_types.h>



#include <linux/kernel/stddef.h>
#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>

extern efi_machine_conf_s	*machine_info;

/*
 * We organize the E820 table into three main data structures:
 *
 * - 'e820_table_firmware': the original firmware version passed to us by the
 *   bootloader - not modified by the kernel. It is composed of two parts:
 *   the first 128 E820 memory entries in boot_params.e820_table and the remaining
 *   (if any) entries of the SETUP_E820_EXT nodes. We use this to:
 *
 *       - inform the user about the firmware's notion of memory layout
 *         via /sys/firmware/memmap
 *
 *       - the hibernation code uses it to generate a kernel-independent CRC32
 *         checksum of the physical memory layout of a system.
 *
 * - 'e820_table_kexec': a slightly modified (by the kernel) firmware version
 *   passed to us by the bootloader - the major difference between
 *   e820_table_firmware[] and this one is that, the latter marks the setup_data
 *   list created by the EFI boot stub as reserved, so that kexec can reuse the
 *   setup_data information in the second kernel. Besides, e820_table_kexec[]
 *   might also be modified by the kexec itself to fake a mptable.
 *   We use this to:
 *
 *       - kexec, which is a bootloader in disguise, uses the original E820
 *         layout to pass to the kexec-ed kernel. This way the original kernel
 *         can have a restricted E820 map while the kexec()-ed kexec-kernel
 *         can have access to full memory - etc.
 *
 * - 'e820_table': this is the main E820 table that is massaged by the
 *   low level x86 platform code, or modified by boot parameters, before
 *   passed on to higher level MM layers.
 *
 * Once the E820 map has been converted to the standard Linux memory layout
 * information its role stops - modifying it has no effect and does not get
 * re-propagated. So itsmain role is a temporary bootstrap storage of firmware
 * specific memory layout data during early bootup.
 */
// static e820_table_s e820_table_init		__initdata;
e820_table_s e820_table	__initdata;
// static struct e820_table e820_table_kexec_init		__initdata;
// static struct e820_table e820_table_firmware_init	__initdata;

// struct e820_table *e820_table __refdata			= &e820_table_init;
// struct e820_table *e820_table_kexec __refdata		= &e820_table_kexec_init;
// struct e820_table *e820_table_firmware __refdata	= &e820_table_firmware_init;

// e820_entry_s *e820_table;

#define MAX_ARCH_PFN MAXMEM >> PAGE_SHIFT


void __init e820__memory_setup(void)
{
		int i = 0;
	e820_table.nr_entries = 0;
	// char *__init e820__memory_setup_default(void)
	// {
	//							||
	//							\/
	// static int __init append_e820_table(struct boot_e820_entry *entries, u32 nr_entries)
	// {
		for (mb_memmap_s *mmap_entp = machine_info->mb_mmap;
			i < sizeof(machine_info->mb_mmap)/sizeof(mb_memmap_s);
			i++, mmap_entp++)
		{
			if (mmap_entp->len == 0)
			{
				e820_table.nr_entries = i;
				break;
			}
			
			e820_entry_s *e820_entp = &e820_table.entries[i];
			e820_entp->addr = mmap_entp->addr;
			e820_entp->size = mmap_entp->len;
			e820_entp->type = mmap_entp->type;
		}
	// }
	// }
}


unsigned long __init e820__end_of_ram_pfn(void)
{
	// /*
	// * Find the highest page frame number we have available
	// */
	// static unsigned long __init e820_end_pfn(unsigned long limit_pfn, enum e820_type type)
	// {
		int i;
		unsigned long last_pfn = 0;
		unsigned long limit_pfn = MAX_ARCH_PFN;
		unsigned long max_arch_pfn = MAX_ARCH_PFN;

		for (i = 0; i < e820_table.nr_entries; i++)
		{
			e820_entry_s *entry = &e820_table.entries[i];
			unsigned long start_pfn;
			unsigned long end_pfn;

			if (entry->size == 0)
				break;
			if (entry->type != E820_TYPE_RAM)
				continue;

			start_pfn = entry->addr >> PAGE_SHIFT;
			end_pfn = (entry->addr + entry->size) >> PAGE_SHIFT;

			if (start_pfn >= limit_pfn)
				continue;
			if (end_pfn > limit_pfn)
			{
				last_pfn = limit_pfn;
				break;
			}
			if (end_pfn > last_pfn)
				last_pfn = end_pfn;
		}

		if (last_pfn > max_arch_pfn)
			last_pfn = max_arch_pfn;

		return last_pfn;
	// }
}

void __init e820__memblock_setup(void)
{
	int i;
	u64 end;

	// /*
	//  * The bootstrap memblock region count maximum is 128 entries
	//  * (INIT_MEMBLOCK_REGIONS), but EFI might pass us more E820 entries
	//  * than that - so allow memblock resizing.
	//  *
	//  * This is safe, because this call happens pretty late during x86 setup,
	//  * so we know about reserved memory regions already. (This is important
	//  * so that memblock resizing does no stomp over reserved areas.)
	//  */
	for (i = 0; i < e820_table.nr_entries; i++)
	{
		e820_entry_s *entry = &e820_table.entries[i];
		enum e820_type type = entry->type;

		if (type == E820_TYPE_SOFT_RESERVED)
			memblock_reserve(entry->addr, entry->size);

		if (type != E820_TYPE_RAM &&
			type != E820_TYPE_ACPI &&
			type != E820_TYPE_NVS &&
			type != E820_TYPE_RESERVED_KERN)
			continue;

		memblock_add(entry->addr, entry->size);
		if (type == E820_TYPE_ACPI ||
			type == E820_TYPE_NVS)
			memblock_reserve(entry->addr, entry->size);
	}

	/* Throw away partial pages: */
	memblock_trim_memory(PAGE_SIZE);

	// memblock_dump_all();
}
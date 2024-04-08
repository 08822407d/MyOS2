// source: linux-6.4.9

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
#include "e820.h"

#include <linux/kernel/mm.h>
#include <asm/setup.h>


#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>


extern mbi_mmap_ent_s	*ram_map;


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
static e820_table_s e820_table_init		__initdata;

e820_table_s *e820_table __refdata			= &e820_table_init;

#define MAX_ARCH_PFN MAXMEM >> PAGE_SHIFT

static void __init e820_print_type(enum e820_type type)
{
	switch (type) {
	case E820_TYPE_RAM:				/* Fall through: */
	case E820_TYPE_RESERVED_KERN:	pr_cont("usable");						break;
	case E820_TYPE_RESERVED:		pr_cont("reserved");					break;
	case E820_TYPE_SOFT_RESERVED:	pr_cont("soft reserved");				break;
	case E820_TYPE_ACPI:			pr_cont("ACPI data");					break;
	case E820_TYPE_NVS:				pr_cont("ACPI NVS");					break;
	case E820_TYPE_UNUSABLE:		pr_cont("unusable");					break;
	case E820_TYPE_PMEM:			/* Fall through: */
	case E820_TYPE_PRAM:			pr_cont("persistent (type %u)", type);	break;
	default:						pr_cont("type %u", type);				break;
	}
}

void __init e820__print_table(char *who)
{
	int i;

	for (i = 0; i < e820_table->nr_entries; i++) {
		pr_info("%s: [mem %#018Lx-%#018Lx] ",
			who,
			e820_table->entries[i].addr,
			e820_table->entries[i].addr + e820_table->entries[i].size - 1);

		e820_print_type(e820_table->entries[i].type);
		pr_cont("\n");
	}
}

void __init e820__memory_setup(void)
{
	int i = 0;
	e820_table->nr_entries = 0;
	char *who = "BIOS-e820";

	for (mbi_mmap_ent_s *mmap_entp = ram_map;
		mmap_entp->len != 0 && mmap_entp->type != 0;
		i++, mmap_entp++)
	{
		e820_entry_s *e820_entp = &(e820_table->entries[i]);
		e820_entp->addr = mmap_entp->addr;
		e820_entp->size = mmap_entp->len;
		e820_entp->type = mmap_entp->type;
	}
	e820_table->nr_entries = i;

	pr_info("BIOS-provided physical RAM map:\n");
	e820__print_table(who);
}


unsigned long __init e820__end_of_ram_pfn(void)
{
	int i;
	unsigned long last_pfn = 0;
	unsigned long limit_pfn = MAX_ARCH_PFN;
	unsigned long max_arch_pfn = MAX_ARCH_PFN;

	for (i = 0; i < e820_table->nr_entries; i++)
	{
		e820_entry_s *entry = &(e820_table->entries[i]);
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
}

void __init e820__memblock_setup(void)
{
	int i;
	u64 end;

	/*
	 * The bootstrap memblock region count maximum is 128 entries
	 * (INIT_MEMBLOCK_REGIONS), but EFI might pass us more E820 entries
	 * than that - so allow memblock resizing.
	 *
	 * This is safe, because this call happens pretty late during x86 setup,
	 * so we know about reserved memory regions already. (This is important
	 * so that memblock resizing does no stomp over reserved areas.)
	 */
	// memblock_allow_resize();

	for (i = 0; i < e820_table->nr_entries; i++)
	{
		e820_entry_s *entry = &(e820_table->entries[i]);
		enum e820_type type = entry->type;

		end = entry->addr + entry->size;
		if (end != (resource_size_t)end)
			continue;

		if (type == E820_TYPE_SOFT_RESERVED)
			simple_mmblk_reserve(entry->addr, entry->size);

		if (type != E820_TYPE_RAM &&
			type != E820_TYPE_RESERVED_KERN)
			continue;

		simple_mmblk_add(entry->addr, entry->size);

		// if (type == E820_TYPE_ACPI ||
		// 	type == E820_TYPE_NVS)
		// 	simple_mmblk_reserve(entry->addr, entry->size);
	}

	/* Throw away partial pages: */
	memblock_trim_memory(PAGE_SIZE);

	// memblock_dump_all();
}
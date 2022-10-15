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


#include <linux/kernel/stddef.h>
#include <uefi/multiboot2.h>

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
// static struct e820_table e820_table_init		__initdata;
// static struct e820_table e820_table_kexec_init		__initdata;
// static struct e820_table e820_table_firmware_init	__initdata;

// struct e820_table *e820_table __refdata			= &e820_table_init;
// struct e820_table *e820_table_kexec __refdata		= &e820_table_kexec_init;
// struct e820_table *e820_table_firmware __refdata	= &e820_table_firmware_init;

mb_memmap_s *e820_table;

void __init myos_e820__memblock_setup(void)
{
	if (e820_table == NULL)
		while (1);
		
	while (e820_table->len != 0)
	{
		if (e820_table->type != 1)
			memblock_reserve(e820_table->addr, e820_table->len);
		else if(e820_table->len != 0)
			memblock_add(e820_table->addr, e820_table->len);
		e820_table++;
	}
}
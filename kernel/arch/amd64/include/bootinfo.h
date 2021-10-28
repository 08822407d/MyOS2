#ifndef _BOOTINFO_H_
#define _BOOTINFO_H_

#include <sys/types.h>
#include <sys/cdefs.h>

#include "efi_typedef.h"

#define BOOTINFO_ADDR 0x60000

	typedef struct EFI_CPU_DESCRIPTOR
	{
		UINT32	pack_id;
		UINT32	core_id;
		UINT32	thd_id;
		UINT32	status;
		UINT64	proccessor_id;
	} efi_cpudesc_s;

	typedef struct EFI_SMP_INFO
	{
		UINT32	core_num;
		UINT32	core_available;
		efi_cpudesc_s cpus[256];
	} efi_smpinfo_s;

	typedef struct EFI_GRAPHICS_OUTPUT_INFORMATION
	{
		UINT32 HorizontalResolution;
		UINT32 VerticalResolution;
		UINT32 PixelsPerScanLine;

		UINT64 FrameBufferBase;
		UINT64 FrameBufferSize;
	} efi_vbeinfo_s;

	typedef struct EFI_E820_MEMORY_DESCRIPTOR
	{
		UINT64 address;
		UINT64 length;
		UINT64 type;
	} efi_e820entry_s;

	typedef struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION
	{
		UINT64 e820_entry_count;
		efi_e820entry_s e820_entry[256];
	} efi_meminfo_s;

	typedef struct KERNEL_BOOT_PARAMETER_INFORMATION
	{
		efi_meminfo_s efi_e820_info;
		efi_vbeinfo_s efi_graphics_info;
		efi_smpinfo_s efi_smp_info;
	} efi_machine_conf_s;

// enum e820_type {
// 	E820_TYPE_RAM		= 1,
// 	E820_TYPE_RESERVED	= 2,
// 	E820_TYPE_ACPI		= 3,
// 	E820_TYPE_NVS		= 4,
// 	E820_TYPE_UNUSABLE	= 5,
// 	E820_TYPE_PMEM		= 7,

// 	/*
// 	 * This is a non-standardized way to represent ADR or
// 	 * NVDIMM regions that persist over a reboot.
// 	 *
// 	 * The kernel will ignore their special capabilities
// 	 * unless the CONFIG_X86_PMEM_LEGACY=y option is set.
// 	 *
// 	 * ( Note that older platforms also used 6 for the same
// 	 *   type of memory, but newer versions switched to 12 as
// 	 *   6 was assigned differently. Some time they will learn... )
// 	 */
// 	E820_TYPE_PRAM		= 12,

// 	/*
// 	 * Special-purpose memory is indicated to the system via the
// 	 * EFI_MEMORY_SP attribute. Define an e820 translation of this
// 	 * memory type for the purpose of reserving this range and
// 	 * marking it with the IORES_DESC_SOFT_RESERVED designation.
// 	 */
// 	E820_TYPE_SOFT_RESERVED	= 0xefffffff,

// 	/*
// 	 * Reserved RAM used by the kernel itself if
// 	 * CONFIG_INTEL_TXT=y is enabled, memory of this type
// 	 * will be included in the S3 integrity calculation
// 	 * and so should not include any memory that the BIOS
// 	 * might alter over the S3 transition:
// 	 */
// 	E820_TYPE_RESERVED_KERN	= 128,
// };

#endif	/* _BOOTINFO_H_ */
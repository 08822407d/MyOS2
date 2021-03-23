#ifndef _BOOTINFO_H_
#define _BOOTINFO_H_

#include <sys/types.h>
#include <sys/cdefs.h>

#define BOOTINFO_ADDR 0x60000

struct EFI_GRAPHICS_OUTPUT_INFORMATION
{
	uint32_t HorizontalResolution;
	uint32_t VerticalResolution;
	uint32_t PixelsPerScanLine;

	uint64_t FrameBufferBase;
	uint64_t FrameBufferSize;
};

struct EFI_E820_MEMORY_DESCRIPTOR
{
	uint64_t address;
	uint64_t length;
	uint32_t  type;
}__attribute__((packed));

struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION
{
	uint32_t E820_Entry_count;
	struct EFI_E820_MEMORY_DESCRIPTOR E820_Entry[0];
};

struct KERNEL_BOOT_PARAMETER_INFORMATION
{
	struct EFI_GRAPHICS_OUTPUT_INFORMATION Graphics_Info;
	struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION E820_Info;
};

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
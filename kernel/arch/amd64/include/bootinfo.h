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

#endif	/* _BOOTINFO_H_ */
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

#include "multiboot2.h"

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
		efi_e820entry_s e820_entry[16];
		multiboot_memory_map_s mb_mmap[16];
	} efi_meminfo_s;

	typedef struct KERNEL_BOOT_PARAMETER_INFORMATION
	{
		efi_meminfo_s efi_e820_info;
		efi_vbeinfo_s efi_graphics_info;
		efi_smpinfo_s efi_smp_info;
	} efi_machine_conf_s;

	EFI_STATUS LocateMPP(EFI_MP_SERVICES_PROTOCOL** mpp);
	EFI_STATUS testMPPInfo(efi_machine_conf_s * machine_info);
	EFI_STATUS load_kernel_image(IN EFI_HANDLE ImageHandle);
	void get_vbe_info(efi_machine_conf_s * machine_info);
	void get_machine_memory_info(efi_machine_conf_s * machine_info);
	void get_machine_memory_info2(efi_machine_conf_s * machine_info);

#endif /* _BOOTLOADER_H_ */
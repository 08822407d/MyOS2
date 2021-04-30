#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

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
		unsigned int HorizontalResolution;
		unsigned int VerticalResolution;
		unsigned int PixelsPerScanLine;

		unsigned long FrameBufferBase;
		unsigned long FrameBufferSize;
	} efi_vbeinfo_s;

	typedef struct EFI_E820_MEMORY_DESCRIPTOR
	{
		unsigned long address;
		unsigned long length;
		unsigned int  type;
	} __attribute__((packed)) efi_e820entry_s;

	typedef struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION
	{
		unsigned int E820_Entry_count;
		efi_e820entry_s E820_Entry[0];
	} efi_meminfo_s;

	typedef struct KERNEL_BOOT_PARAMETER_INFORMATION
	{
		efi_vbeinfo_s Graphics_Info;
		efi_meminfo_s E820_Info;
		efi_smpinfo_s smp_info;
	} efi_machine_conf_s;

	EFI_STATUS LocateMPP(EFI_MP_SERVICES_PROTOCOL** mpp);
	EFI_STATUS testMPPInfo(efi_machine_conf_s * machine_info);
	EFI_STATUS load_kernel_image(IN EFI_HANDLE ImageHandle);
	void get_vbe_info(efi_machine_conf_s * machine_info);
	void get_machine_memory_info(efi_machine_conf_s * machine_info);

#endif /* _BOOTLOADER_H_ */
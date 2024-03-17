#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

	#ifdef __PI_UEFI_H__
		#include <Library/UefiLib.h>
		#include <Library/UefiBootServicesTableLib.h>
		#include <Library/UefiRuntimeServicesTableLib.h>
		#include <Protocol/SimpleFileSystem.h>
		#include <Protocol/LoadedImage.h>
		#include <Guid/FileInfo.h>
		#include "Pi/PiDxeCis.h"
		#include "Protocol/MpService.h"
	#endif

	#include "multiboot2.h"

	typedef struct EFI_CPU_DESCRIPTOR
	{
		multiboot_uint32_t	pack_id;
		multiboot_uint32_t	core_id;
		multiboot_uint32_t	thd_id;
		multiboot_uint32_t	status;
		multiboot_uint64_t	proccessor_id;
	} efi_cpudesc_s;

	typedef struct EFI_SMP_INFO
	{
		multiboot_uint32_t	core_num;
		multiboot_uint32_t	core_available;
		efi_cpudesc_s cpus[256];
	} efi_smpinfo_s;

	typedef struct KERNEL_BOOT_PARAMETER_INFORMATION
	{
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

	extern mb2_hdr_s * mb2_hdr_p;
	extern mb2_hdr_tag_kaddr_s * mb2_kaddr_p;

	#ifdef __PI_UEFI_H__
		EFI_STATUS LocateMPP(EFI_MP_SERVICES_PROTOCOL** mpp);
		EFI_STATUS testMPPInfo(EFI_MP_SERVICES_PROTOCOL *mpp, efi_machine_conf_s * machine_info);
		EFI_STATUS read_mb2head(IN EFI_LOADED_IMAGE	*LoadedImage);
		void set_video_mode(IN EFI_HANDLE ImageHandle, int mode);
		mbi_tag_s *fill_framebuffer_info(IN EFI_HANDLE ImageHandle, mbi_tag_s *mb2_infotag_ptr);
		mbi_tag_s *fill_mmap_info(IN EFI_HANDLE ImageHandle, mbi_tag_s *mb2_infotag_ptr);
		mbi_tag_s *fill_acpi_RSDT_info(EFI_SYSTEM_TABLE *SystemTable, mbi_tag_s *mb2_infotag_ptr);

		void parse_mb2_header(EFI_PHYSICAL_ADDRESS start_addr);
		mbi_tag_s *init_info_tag_addr(PHYSICAL_ADDRESS start_addr);
		mbi_tag_s *next_info_tag_addr(mbi_tag_s *prev_info_tag);
		mbi_tag_s *fill_info_tag_end(mbi_tag_s *info_tag_ptr);
        void fill_info_tag_header(mbi_tags_header_s *mbi_tags_header_ptr,
        		PHYSICAL_ADDRESS start_addr, mbi_tag_s *info_tag_end);

		EFI_STATUS load_elf_kernel(IN EFI_LOADED_IMAGE	*LoadedImage,
			CHAR16 *filename, PHYSICAL_ADDRESS *entry_addr);
	#endif

#endif /* _BOOTLOADER_H_ */
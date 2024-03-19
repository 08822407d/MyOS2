/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#include <Uefi.h>
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi65.h>

#include "utils.h"

#include "bootloader.h"
#include "multiboot2.h"




EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_LOADED_IMAGE	*LoadedImage;
	mbi_tags_header_s	*mbi_tags_header_ptr;
	mbi_tag_s			*mb2_infotag_ptr;
	PHYSICAL_ADDRESS	k_load_addr = 0;

	gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID*)&LoadedImage);

//////////////////////

	set_video_mode(ImageHandle, -1);
	load_elf_kernel(LoadedImage, L"kernel", &k_load_addr);

///////////////////

	UINTN page_count = 32;
	EFI_PHYSICAL_ADDRESS pages = 0;
	gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, page_count, &pages);
	gBS->SetMem((void*)pages, page_count * PAGE_SIZE, 0);
	mbi_tags_header_ptr = (mbi_tags_header_s *)pages;
	mb2_infotag_ptr = init_info_tag_addr(pages);
	

	mb2_infotag_ptr = fill_framebuffer_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_mmap_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_acpi_RSDT_info(SystemTable, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_cputopo_info(mb2_infotag_ptr);


	mb2_infotag_ptr = fill_info_tag_end(mb2_infotag_ptr);
	fill_info_tag_header(mbi_tags_header_ptr, pages, mb2_infotag_ptr);

/////////////////////

	Print(L"Allocate Pages At: 0x%llx.\n", pages);
	Print(L"Entry Point At: 0x%llx.\n", k_load_addr);
	Print(L"Call ExitBootServices.\n");
	Print(L"Jmp to Kernel.\n");

	gBS->CloseProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, ImageHandle, NULL);

	void (*func)(void);
	func = (void *)k_load_addr;

	// while (1);

	asm volatile(	"movq	%0,		%%rax		\n"
					"movq	%1,		%%rbx		\n"
					"callq	*%2					\n"
				:
				:	"i" (MULTIBOOT2_BOOTLOADER_MAGIC),
					"m" (pages),
					"m" (func)
				:
				);

	while (1);
	
	return EFI_SUCCESS;
}

EFI_STATUS read_mb2head(IN EFI_LOADED_IMAGE	*LoadedImage)
{
	UINT64	bufsize = MULTIBOOT_SEARCH;
	PHYSICAL_ADDRESS	mb2_load_addr = 0x1000000;

	LoadFile(LoadedImage, L"kernel.bin", mb2_load_addr, bufsize);
	parse_mb2_header(mb2_load_addr);

	return EFI_SUCCESS;
}

void set_video_mode(IN EFI_HANDLE ImageHandle, int mode)
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gGraphicsOutput = 0;
	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
	if (mode != -1)
		gGraphicsOutput->SetMode(gGraphicsOutput, mode);
	else
	{
		int i;
		EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode = 0;
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = 0;
		UINTN InfoSize = 0;

		Mode = gGraphicsOutput->Mode;

		long H_V_Resolution = Mode->Info->HorizontalResolution * Mode->Info->VerticalResolution;
		int MaxResolutionMode = gGraphicsOutput->Mode->Mode;
		// 选择最大分辨率
		for(i = 0; i < Mode->MaxMode; i++)
		{
			gGraphicsOutput->QueryMode(gGraphicsOutput, i, &InfoSize, &Info);

			// Print(L"Mode: %02d, Version: %x, Format: %d, Horizontal: %d, Vertical: %d, ScanLine: %d\n", i, Info->Version, Info->PixelFormat, Info->HorizontalResolution, Info->VerticalResolution, Info->PixelsPerScanLine);

			if((Info->PixelFormat == 1) && (Info->HorizontalResolution * Info->VerticalResolution > H_V_Resolution))
			{
				H_V_Resolution = Info->HorizontalResolution * Info->VerticalResolution;
				MaxResolutionMode = i;
			}
			gBS->FreePool(Info);
		}

		gGraphicsOutput->SetMode(gGraphicsOutput,MaxResolutionMode);
		gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);
		Mode = gGraphicsOutput->Mode;
		
		// 查看所有分辨率
		for(i = 0; i < Mode->MaxMode; i++)
		{
			gGraphicsOutput->QueryMode(gGraphicsOutput, i, &InfoSize, &Info);
			Print(L"Mode: %02d, Version: %x, Format: %d, Horizontal: %d, Vertical: %d, ScanLine: %d\n", i, Info->Version, Info->PixelFormat, Info->HorizontalResolution, Info->VerticalResolution, Info->PixelsPerScanLine);
			gBS->FreePool(Info);
		}
	}

	gBS->CloseProtocol(gGraphicsOutput, &gEfiGraphicsOutputProtocolGuid,ImageHandle, NULL);
}

mbi_tag_s *fill_framebuffer_info(IN EFI_HANDLE ImageHandle, mbi_tag_s *mb2_infotag_ptr)
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphicsOutput = 0;
	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode = gGraphicsOutput->Mode;


	mbi_framebuffer_s *framebuffer_info = (mbi_framebuffer_s *)mb2_infotag_ptr;
	mbi_framebuffer_common_s *common = &framebuffer_info->common;

	common->framebuffer_addr = Mode->FrameBufferBase;
	common->framebuffer_width = Mode->Info->HorizontalResolution;
	common->framebuffer_height = Mode->Info->VerticalResolution;
	common->framebuffer_bpp = Mode->FrameBufferSize * 8 /
			(common->framebuffer_width * common->framebuffer_height);
	common->framebuffer_pitch = Mode->Info->PixelsPerScanLine * common->framebuffer_bpp / 8;
	common->framebuffer_type = Mode->Mode;
	common->reserved = 0;

	gBS->CloseProtocol(gGraphicsOutput, &gEfiGraphicsOutputProtocolGuid,ImageHandle, NULL);

	common->type = MULTIBOOT_TAG_TYPE_FRAMEBUFFER;
	common->size = sizeof(mbi_framebuffer_s);
	return next_info_tag_addr(mb2_infotag_ptr);
}

mbi_tag_s *fill_mmap_info(IN EFI_HANDLE ImageHandle, mbi_tag_s *mb2_infotag_ptr)
{
	EFI_STATUS status = EFI_SUCCESS;

	UINT32 DesVersion = 0;
	UINTN MapKey = 0;
	UINTN MemMapSize = 0;
	UINTN DescriptorSize = 0;
	EFI_MEMORY_DESCRIPTOR* MemMap = 0;

	int i;
	int e820_nr = 0;
	unsigned long last_end = 0;
	mbi_mmap_s *memmap_info = (mbi_mmap_s *)mb2_infotag_ptr;
	mbi_mmap_ent_s *last_mb_mmap = NULL;
	mbi_mmap_ent_s *mb_mmap = memmap_info->entries;

	// 计算缓冲区大小
	gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DesVersion);
	MemMapSize += DescriptorSize * 5;
	// 申请缓冲区
	gBS->AllocatePool(EfiRuntimeServicesData, MemMapSize, (VOID **)&MemMap);
	gBS->SetMem((void*)MemMap, MemMapSize, 0);
	// 重新获取内存分布
	status = gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DesVersion);
	if(EFI_ERROR(status))
	{
		Print(L"GetMemoryMap FAIL. Status: %018lx\n",status);
		while (1);
	}

	for(i = 0; i < MemMapSize / DescriptorSize; i++)
	{
		int MemType = 0;
		// EFI_MEMORY_DESCRIPTOR的大小不一定等于DescriptorSize
		EFI_MEMORY_DESCRIPTOR *MMap = (EFI_MEMORY_DESCRIPTOR*) ((CHAR8*)MemMap + i * DescriptorSize);
		if(MMap->NumberOfPages == 0)
			continue;
		switch(MMap->Type)
		{
			case EfiReservedMemoryType:
			case EfiMemoryMappedIO:
			case EfiMemoryMappedIOPortSpace:
			case EfiPalCode:
				MemType= 2;	//2:ROM or Reserved
				break;

			case EfiUnusableMemory:
				MemType= 5;	//5:Unusable
				break;

			case EfiACPIReclaimMemory:
				MemType= 3;	//3:ACPI Reclaim Memory
				break;

			case EfiLoaderCode:
			case EfiLoaderData:
			case EfiBootServicesCode:
			case EfiBootServicesData:
			case EfiRuntimeServicesCode:
			case EfiRuntimeServicesData:
			case EfiConventionalMemory:
			case EfiPersistentMemory:
				MemType= 1;	//1:RAM
				break;

			case EfiACPIMemoryNVS:
				MemType= 4;	//4:ACPI NVS Memory
				break;

			default:
				Print(L"Invalid UEFI Memory Type:%4d\n", MMap->Type);
				continue;
		}

		// check if same type and address contiguous
		if((last_mb_mmap != NULL) &&
			(last_mb_mmap->type == MemType) && 
			(MMap->PhysicalStart == last_end))
		{
			last_mb_mmap->len += MMap->NumberOfPages << PAGE_SHIFT;
			last_end += MMap->NumberOfPages << PAGE_SHIFT;
		}
		else
		{
			mb_mmap->addr = MMap->PhysicalStart;
			mb_mmap->len = MMap->NumberOfPages << PAGE_SHIFT;
			mb_mmap->type = MemType;

			last_mb_mmap = mb_mmap;
			mb_mmap++;

			last_end = MMap->PhysicalStart + (MMap->NumberOfPages << 12);
			e820_nr++;			
		}
	}

	// 排序e820表，使其按照addr从小到大排列
	last_mb_mmap = memmap_info->entries;
	int j = 0;
	for(i = 0; i< e820_nr; i++)
	{
		mbi_mmap_ent_s * e820i = last_mb_mmap + i;
		mbi_mmap_ent_s mmap_ent;
		for(j = i + 1; j < e820_nr; j++)
		{
			mbi_mmap_ent_s * e820j = last_mb_mmap + j;
			if(e820i->addr > e820j->addr)
			{
				mmap_ent = *e820i;
				*e820i = *e820j;
				*e820j = mmap_ent;
			}
		}
	}

	gBS->FreePool(MemMap);
	gBS->ExitBootServices(ImageHandle, MapKey);

	memmap_info->type = MULTIBOOT_TAG_TYPE_MMAP;
	memmap_info->size = sizeof(mbi_mmap_s) + sizeof(mbi_mmap_ent_s) * (e820_nr + 1);
	memmap_info->entry_size = sizeof(mbi_mmap_ent_s);
	return next_info_tag_addr(mb2_infotag_ptr);
}

mbi_tag_s *fill_acpi_RSDT_info(EFI_SYSTEM_TABLE *SystemTable, mbi_tag_s *mb2_infotag_ptr)
{
	mbi_acpi_new_s *rsdt_new = (mbi_acpi_new_s *)mb2_infotag_ptr;

	UINTN		i;
	EFI_GUID	AcpiTableGuid  = ACPI_TABLE_GUID;
	EFI_GUID	Acpi2TableGuid = EFI_ACPI_TABLE_GUID;
	EFI_CONFIGURATION_TABLE		*configTab=NULL;  
	EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER  *Root;
	UINT64		RSDP_SIGNATUR = EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_SIGNATURE;
	Print(L"\nRSDP Signature: 0x%llx\n", RSDP_SIGNATUR); 

	Print(L"List ACPI Table:\n");
	configTab = gST->ConfigurationTable;
	for (i = 0; i < gST->NumberOfTableEntries; i++)
	{   
		//Step1. Find the table for ACPI
		if ((CompareGuid(&configTab->VendorGuid, &AcpiTableGuid) == 0) ||
			(CompareGuid(&configTab->VendorGuid, &Acpi2TableGuid) == 0))
		{
			Root = configTab->VendorTable;
			Print(L"\nTable Signature: 0x%llx\n", Root->Signature); 

			// Step2. Check the Revision, we olny accept Revision >= 2
			if (Root->Signature == RSDP_SIGNATUR && Root->Revision >= EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION)
			{
				ZeroMem(&rsdt_new->rsdp, sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER));
				CopyMem(&rsdt_new->rsdp, Root, sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER));
				break;
			}
		}
		configTab++;
	}

	// while (1);

	rsdt_new->type = MULTIBOOT_TAG_TYPE_ACPI_NEW;
	rsdt_new->size = sizeof(mbi_acpi_new_s) + sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER);
	return next_info_tag_addr(mb2_infotag_ptr);
}

EFI_STATUS load_elf_kernel(IN EFI_LOADED_IMAGE	*LoadedImage,
	CHAR16 *filename, OUT PHYSICAL_ADDRESS *entry_addr)
{
	EFI_STATUS status = EFI_SUCCESS;
	// Check Format
	PHYSICAL_ADDRESS raw_img_addr = 0x2000000;
	LoadFile(LoadedImage, filename, raw_img_addr, 0);
	BOOLEAN isElf = IsElfFormat((UINT8 *)raw_img_addr);
	if (!isElf)
	{
		Print(L"PANIC: Kernel: %s is NOT Elf format.\n", filename);
		while (1);
	}

	// Read info and load
	ELF_IMAGE_CONTEXT ElfCtx;
	status = ParseElfImage((VOID *)raw_img_addr, &ElfCtx);
	if (status != EFI_SUCCESS)
	{
		Print(L"PANIC: ElfParse kernel: %s failed.\n", filename);
		while (1);
	}
	Print(L"Elf file size: 0x%llx\n", ElfCtx.FileSize);
	Print(L"Elf Image size: 0x%llx\n", ElfCtx.ImageSize);
	Print(L"Elf Image loaded address: 0x%llx\n", ElfCtx.ImageAddress);
	Print(L"Elf Image preferred address: 0x%llx\n", ElfCtx.PreferredImageAddress);
	Print(L"Elf Image entry point: 0x%llx\n", ElfCtx.EntryPoint);
	Print(L"Elf Image need to reload: %d\n", ElfCtx.ReloadRequired);
	// while (1);
	
	EFI_PHYSICAL_ADDRESS loaded_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.ImageAddress;
	*entry_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.EntryPoint;
	// Alloc Image Memory
	ElfCtx.ImageAddress = ElfCtx.PreferredImageAddress;
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(ElfCtx.ImageSize + PAGE_SIZE - 1) / PAGE_SIZE, &loaded_addr);
	LoadElfImage(&ElfCtx);

	return status;
}

mbi_tag_s *fill_cputopo_info(mbi_tag_s *mb2_infotag_ptr)
{
	myos2_tag_smpinfo_s *smp_info = (myos2_tag_smpinfo_s *)mb2_infotag_ptr;

	UINTN i = 0;
	UINTN nCores = 0, nRunning = 0;
	EFI_MP_SERVICES_PROTOCOL *mpp = 0;

	gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mpp);
	mpp->GetNumberOfProcessors(mpp, &nCores, &nRunning);
	smp_info->core_num = nCores;
	smp_info->core_available = nRunning;
	// Print(L"System has %d cores, %d cores are running\n", nCores, nRunning);
	for(i =0; i< nCores; i++){
		EFI_PROCESSOR_INFORMATION mcpuInfo;
		mpp -> GetProcessorInfo( mpp, i, &mcpuInfo);
		smp_info->cpus[i].proccessor_id	= mcpuInfo.ProcessorId;
		smp_info->cpus[i].status		= mcpuInfo.StatusFlag;
		smp_info->cpus[i].pack_id		= mcpuInfo.Location.Package;
		smp_info->cpus[i].core_id		= mcpuInfo.Location.Core;
		smp_info->cpus[i].thd_id		= mcpuInfo.Location.Thread;
	}

	smp_info->type = MYOS2_TAG_TYPE_CPU_TOPO;
	smp_info->size = sizeof(myos2_tag_smpinfo_s) + nCores * sizeof(myos2_cpudesc_s);

	// Print(L"core-num: 0x%lx; tag_size: 0x%lx\n", smp_info->core_num, smp_info->size);
	// while (1);
	
	return next_info_tag_addr(mb2_infotag_ptr);
}
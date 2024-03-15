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

#include "utils.h"

#include "bootloader.h"
#include "multiboot2.h"


EFI_MP_SERVICES_PROTOCOL* mpp;
EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphicsOutput = 0;
UINTN MapKey = 0;
UINTN DescriptorSize = 0;
UINT32 DesVersion = 0;


EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_LOADED_IMAGE		*LoadedImage;
	EFI_PHYSICAL_ADDRESS	pages;
	efi_machine_conf_s		*machine_info = NULL;

	gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID*)&LoadedImage);
//////////////////////

	read_mb2head(LoadedImage);
	PHYSICAL_ADDRESS k_load_addr = (PHYSICAL_ADDRESS)mb2_kaddr_p->load_addr;
	
	// LoadFile(LoadedImage, L"kernel.bin", k_load_addr, 0);

	load_elf_kernel(LoadedImage, L"kernel", &k_load_addr);

///////////////////

	pages = MACHINE_CONF_ADDR;
	gBS->AllocatePages(AllocateAddress, EfiLoaderData, 4, &pages);
	gBS->SetMem((void*)pages, 0x4000, 0);
	machine_info = (efi_machine_conf_s *)pages;

	get_vbe_info(machine_info);
	get_machine_memory_info(machine_info->mb_mmap);
	
/////////////////////

    LocateMPP(&mpp);
    testMPPInfo(machine_info);

/////////////////////

	Print(L"Call ExitBootServices.\n");
	Print(L"Jmp to Kernel.\n");

	gBS->CloseProtocol(LoadedImage->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,ImageHandle,NULL);
	gBS->CloseProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,ImageHandle,NULL);
	gBS->CloseProtocol(gGraphicsOutput,&gEfiGraphicsOutputProtocolGuid,ImageHandle,NULL);
	gBS->ExitBootServices(ImageHandle,MapKey);

	void (*func)(void);
	func = (void *)k_load_addr;

	asm volatile(	""
				:
				:	"rax" (MULTIBOOT2_BOOTLOADER_MAGIC),
					"rbx" (machine_info)
				:
				);

	func();

	return EFI_SUCCESS;
}

EFI_STATUS read_mb2head(IN EFI_LOADED_IMAGE	*LoadedImage)
{
	UINT64	bufsize = MULTIBOOT_SEARCH;
	PHYSICAL_ADDRESS	mb2_load_addr = 0x1000000;

	LoadFile(LoadedImage, L"kernel.bin", mb2_load_addr, bufsize);
	parse_multiboot2(mb2_load_addr);

	return EFI_SUCCESS;
}

EFI_STATUS LocateMPP(EFI_MP_SERVICES_PROTOCOL** mpp)
{
    return  gBS->LocateProtocol( &gEfiMpServiceProtocolGuid, NULL, (void**)mpp);
}

EFI_STATUS testMPPInfo(efi_machine_conf_s * machine_info)
{
	EFI_STATUS Status;
    {
        UINTN nCores = 0, nRunning = 0;
        Status = mpp -> GetNumberOfProcessors(mpp, & nCores, &nRunning);
		machine_info->efi_smp_info.core_num = nCores;
		machine_info->efi_smp_info.core_available = nRunning;
        // Print(L"System has %d cores, %d cores are running\n", nCores, nRunning);
        {
           UINTN i = 0;
           for(i =0; i< nCores; i++){
               EFI_PROCESSOR_INFORMATION mcpuInfo;
               Status = mpp -> GetProcessorInfo( mpp, i, &mcpuInfo);
			   machine_info->efi_smp_info.cpus[i].proccessor_id = mcpuInfo.ProcessorId;
			   machine_info->efi_smp_info.cpus[i].status = mcpuInfo.StatusFlag;
			   machine_info->efi_smp_info.cpus[i].pack_id = mcpuInfo.Location.Package;
			   machine_info->efi_smp_info.cpus[i].core_id = mcpuInfo.Location.Core;
			   machine_info->efi_smp_info.cpus[i].thd_id = mcpuInfo.Location.Thread;
           }
        }
    }
	return Status;
}

void get_vbe_info(efi_machine_conf_s * machine_info)
{
	int i;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info = 0;
	UINTN InfoSize = 0;

	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);

	long H_V_Resolution = gGraphicsOutput->Mode->Info->HorizontalResolution * gGraphicsOutput->Mode->Info->VerticalResolution;
	int MaxResolutionMode = gGraphicsOutput->Mode->Mode;
	// 选择最大分辨率
	for(i = 0; i < gGraphicsOutput->Mode->MaxMode; i++)
	{
		gGraphicsOutput->QueryMode(gGraphicsOutput,i,&InfoSize,&Info);
		if((Info->PixelFormat == 1) && (Info->HorizontalResolution * Info->VerticalResolution > H_V_Resolution))
		{
			H_V_Resolution = Info->HorizontalResolution * Info->VerticalResolution;
			MaxResolutionMode = i;
		}
		gBS->FreePool(Info);
	}

	gGraphicsOutput->SetMode(gGraphicsOutput,MaxResolutionMode);
	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);

	machine_info->mb_fb_common.framebuffer_width = gGraphicsOutput->Mode->Info->HorizontalResolution;
	machine_info->mb_fb_common.framebuffer_height = gGraphicsOutput->Mode->Info->VerticalResolution;
	machine_info->mb_fb_common.framebuffer_pitch = gGraphicsOutput->Mode->Info->PixelsPerScanLine;
	machine_info->mb_fb_common.framebuffer_addr = gGraphicsOutput->Mode->FrameBufferBase;
	machine_info->mb_fb_common.size = gGraphicsOutput->Mode->FrameBufferSize;
	machine_info->mb_fb_common.framebuffer_type = MaxResolutionMode;
	machine_info->mb_fb_common.type = gGraphicsOutput->Mode->Mode;
}

void get_machine_memory_info(mb_memmap_s * mb_memmap)
{
	EFI_STATUS status = EFI_SUCCESS;
	UINTN MemMapSize = 0;
	EFI_MEMORY_DESCRIPTOR* MemMap = 0;

	int i;
	int e820_nr = 0;
	unsigned long last_end = 0;
	mb_memmap_s *last_mb_mmap = NULL;
	mb_memmap_s *mb_mmap = mb_memmap;

	// 计算缓冲区大小
	gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DescriptorSize,&DesVersion);
	MemMapSize += DescriptorSize * 5;
	// 申请缓冲区
	gBS->AllocatePool(EfiRuntimeServicesData,MemMapSize,(VOID**)&MemMap);
	gBS->SetMem((void*)MemMap,MemMapSize,0);
	// 重新获取内存分布
	status = gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DesVersion);
	if(EFI_ERROR(status))
		Print(L"status:%018lx\n",status);

	for(i = 0;i < MemMapSize / DescriptorSize;i++)
	{
		int MemType = 0;
		// EFI_MEMORY_DESCRIPTOR的大小不一定等于DescriptorSize
		EFI_MEMORY_DESCRIPTOR* MMap = (EFI_MEMORY_DESCRIPTOR*) ((CHAR8*)MemMap + i * DescriptorSize);
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
				Print(L"Invalid UEFI Memory Type:%4d\n",MMap->Type);
				continue;
		}

		// check if same type
		if((last_mb_mmap != NULL) && (last_mb_mmap->type == MemType) && 
		// and address contiguous
			(MMap->PhysicalStart == last_end))
		{
			last_mb_mmap->len += MMap->NumberOfPages << 12;
			last_end += MMap->NumberOfPages << 12;
		}
		else
		{
			mb_mmap->addr = MMap->PhysicalStart;
			mb_mmap->len = MMap->NumberOfPages << 12;
			mb_mmap->type = MemType;

			last_mb_mmap = mb_mmap;
			mb_mmap++;

			last_end = MMap->PhysicalStart + (MMap->NumberOfPages << 12);
			e820_nr++;			
		}
	}

	last_mb_mmap = mb_memmap;
	int j = 0;
	for(i = 0; i< e820_nr; i++)
	{
		mb_memmap_s * e820i = last_mb_mmap + i;
		mb_memmap_s mmap_ent;
		for(j = i + 1; j < e820_nr; j++)
		{
			mb_memmap_s * e820j = last_mb_mmap + j;
			if(e820i->addr > e820j->addr)
			{
				mmap_ent = *e820i;
				*e820i = *e820j;
				*e820j = mmap_ent;
			}
		}
	}
	last_mb_mmap = mb_memmap;

	gBS->FreePool(MemMap);
}

EFI_STATUS load_elf_kernel(IN EFI_LOADED_IMAGE	*LoadedImage,
	CHAR16 *filename, PHYSICAL_ADDRESS *entry_addr)
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
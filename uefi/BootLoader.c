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
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include "Pi/PiDxeCis.h"
#include "Protocol/MpService.h"

#include "bootloader.h"
#include "multiboot2.h"

#define MACHINE_CONF_ADDR 0x60000
#define KERNEL_LOADED_ADDR	0x1000000

EFI_STATUS status = EFI_SUCCESS;
EFI_MP_SERVICES_PROTOCOL* mpp;
EFI_LOADED_IMAGE        *LoadedImage;
EFI_PHYSICAL_ADDRESS pages;
EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphicsOutput = 0;
UINTN MemMapSize = 0;
EFI_MEMORY_DESCRIPTOR* MemMap = 0;
UINTN MapKey = 0;
UINTN DescriptorSize = 0;
UINT32 DesVersion = 0;


EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	efi_machine_conf_s * machine_info = NULL;
	void (*func)(void);
//////////////////////

	pages = KERNEL_LOADED_ADDR;
	status = load_kernel_image(ImageHandle);

///////////////////

	pages = MACHINE_CONF_ADDR;
	gBS->AllocatePages(AllocateAddress,EfiLoaderData,1,&pages);
	gBS->SetMem((void*)machine_info,0x1000,0);
	machine_info = (efi_machine_conf_s *)MACHINE_CONF_ADDR;

	get_vbe_info(machine_info);

	get_machine_memory_info(machine_info);

/////////////////////

    LocateMPP(&mpp);
    testMPPInfo(machine_info);

/////////////////////

	Print(L"Call ExitBootServices And Jmp to Kernel.\n");
	gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DescriptorSize,&DesVersion);

	gBS->CloseProtocol(LoadedImage->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,ImageHandle,NULL);
	gBS->CloseProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,ImageHandle,NULL);

	gBS->CloseProtocol(gGraphicsOutput,&gEfiGraphicsOutputProtocolGuid,ImageHandle,NULL);
	status = gBS->ExitBootServices(ImageHandle,MapKey);

	if(EFI_ERROR(status))
	{
		Print(L"ExitBootServices: Failed, Memory Map has Changed.\n");
		return EFI_INVALID_PARAMETER;
	}
	func = (void *)KERNEL_LOADED_ADDR;
	func();

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
		machine_info->smp_info.core_num = nCores;
		machine_info->smp_info.core_available = nRunning;
        Print(L"System has %d cores, %d cores are running\n", nCores, nRunning);
        {
           UINTN i = 0;
           for(i =0; i< nCores; i++){
               EFI_PROCESSOR_INFORMATION mcpuInfo;
               Status = mpp -> GetProcessorInfo( mpp, i, &mcpuInfo);
               Print(L"CORE %d: ;", i);
               Print(L"  ProcessorId\t:%d ;", mcpuInfo.ProcessorId);
               Print(L"  StatusFlag\t:%x ;", mcpuInfo.StatusFlag);
               Print(L"  Location\t:(%d %d %d)\n", mcpuInfo.Location.Package, mcpuInfo.Location.Core, mcpuInfo.Location.Thread);
			   machine_info->smp_info.cpus[i].proccessor_id = mcpuInfo.ProcessorId;
			   machine_info->smp_info.cpus[i].status = mcpuInfo.StatusFlag;
			   machine_info->smp_info.cpus[i].pack_id = mcpuInfo.Location.Package;
			   machine_info->smp_info.cpus[i].core_id = mcpuInfo.Location.Core;
			   machine_info->smp_info.cpus[i].thd_id = mcpuInfo.Location.Thread;
           }
        }
    }
	return Status;
}

EFI_STATUS load_kernel_image(IN EFI_HANDLE ImageHandle)
{
	EFI_FILE_IO_INTERFACE   *Vol;
	EFI_FILE_HANDLE         RootFs;
	EFI_FILE_HANDLE         FileHandle;

	gBS->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID*)&LoadedImage);
	gBS->HandleProtocol(LoadedImage->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,(VOID*)&Vol);
	Vol->OpenVolume(Vol,&RootFs);
	status = RootFs->Open(RootFs,&FileHandle,(CHAR16*)L"kernel.bin",EFI_FILE_MODE_READ,0);
	if(EFI_ERROR(status))
	{
		Print(L"Open kernel.bin Failed.\n");
		return status;
	}

	EFI_FILE_INFO* FileInfo;
	UINTN BufferSize = 0;

	BufferSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
	gBS->AllocatePool(EfiRuntimeServicesData,BufferSize,(VOID**)&FileInfo); 
	FileHandle->GetInfo(FileHandle,&gEfiFileInfoGuid,&BufferSize,FileInfo);

	gBS->AllocatePages(AllocateAddress,EfiLoaderData,(FileInfo->FileSize + 0x1000 - 1) / 0x1000,&pages);
	BufferSize = FileInfo->FileSize;
	FileHandle->Read(FileHandle,&BufferSize,(VOID*)pages);

#ifdef DEBUG
	Print(L"\tFileName:%s\t Size:%d\t FileSize:%d\t Physical Size:%d\n",FileInfo->FileName,FileInfo->Size,FileInfo->FileSize,FileInfo->PhysicalSize);
	Print(L"Read Kernel File to Memory Address:%018lx\n",pages);
#endif

	gBS->FreePool(FileInfo);
	FileHandle->Close(FileHandle);
	RootFs->Close(RootFs);

	return EFI_SUCCESS;
}

void get_vbe_info(efi_machine_conf_s * machine_info)
{
	int i;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info = 0;
	UINTN InfoSize = 0;

	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);

	long H_V_Resolution = gGraphicsOutput->Mode->Info->HorizontalResolution * gGraphicsOutput->Mode->Info->VerticalResolution;
	int MaxResolutionMode = gGraphicsOutput->Mode->Mode;

	for(i = 0;i < gGraphicsOutput->Mode->MaxMode;i++)
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
	Print(L"Current Mode:%02d,Version:%x,Format:%d,Horizontal:%d,Vertical:%d,ScanLine:%d,FrameBufferBase:%018lx,FrameBufferSize:%018lx\n",gGraphicsOutput->Mode->Mode,gGraphicsOutput->Mode->Info->Version,gGraphicsOutput->Mode->Info->PixelFormat,gGraphicsOutput->Mode->Info->HorizontalResolution,gGraphicsOutput->Mode->Info->VerticalResolution,gGraphicsOutput->Mode->Info->PixelsPerScanLine,gGraphicsOutput->Mode->FrameBufferBase,gGraphicsOutput->Mode->FrameBufferSize);

	machine_info->Graphics_Info.HorizontalResolution = gGraphicsOutput->Mode->Info->HorizontalResolution;
	machine_info->Graphics_Info.VerticalResolution = gGraphicsOutput->Mode->Info->VerticalResolution;
	machine_info->Graphics_Info.PixelsPerScanLine = gGraphicsOutput->Mode->Info->PixelsPerScanLine;
	machine_info->Graphics_Info.FrameBufferBase = gGraphicsOutput->Mode->FrameBufferBase;
	machine_info->Graphics_Info.FrameBufferSize = gGraphicsOutput->Mode->FrameBufferSize;
}

void get_machine_memory_info(efi_machine_conf_s * machine_info)
{
	int i;
	int E820Count = 0;
	unsigned long LastEndAddr = 0;
	efi_e820entry_s *E820p = NULL;
	efi_e820entry_s *LastE820 = NULL;

	E820p = machine_info->E820_Info.E820_Entry;

	gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DescriptorSize,&DesVersion);
	MemMapSize += DescriptorSize * 5;
	gBS->AllocatePool(EfiRuntimeServicesData,MemMapSize,(VOID**)&MemMap);
	gBS->SetMem((void*)MemMap,MemMapSize,0);
	status = gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DescriptorSize,&DesVersion);
	if(EFI_ERROR(status))
		Print(L"status:%018lx\n",status);

	for(i = 0;i < MemMapSize / DescriptorSize;i++)
	{
		int MemType = 0;
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

		if((LastE820 != NULL) && (LastE820->type == MemType) && (MMap->PhysicalStart == LastEndAddr))
		{
			LastE820->length += MMap->NumberOfPages << 12;
			LastEndAddr += MMap->NumberOfPages << 12;
		}
		else
		{
			E820p->address = MMap->PhysicalStart;
			E820p->length = MMap->NumberOfPages << 12;
			E820p->type = MemType;
			LastEndAddr = MMap->PhysicalStart + (MMap->NumberOfPages << 12);
			LastE820 = E820p;
			E820p++;
			E820Count++;			
		}
	}

	machine_info->E820_Info.E820_Entry_count = E820Count;
	LastE820 = machine_info->E820_Info.E820_Entry;
	int j = 0;
	for(i = 0; i< E820Count; i++)
	{
		efi_e820entry_s * e820i = LastE820 + i;
		efi_e820entry_s MemMap;
		for(j = i + 1; j< E820Count; j++)
		{
			efi_e820entry_s * e820j = LastE820 + j;
			if(e820i->address > e820j->address)
			{
				MemMap = *e820i;
				*e820i = *e820j;
				*e820j = MemMap;
			}
		}
	}

	LastE820 = machine_info->E820_Info.E820_Entry;

#ifdef DEBUG
	Print(L"Get MemMapSize:%d,DescriptorSize:%d,count:%d\n",MemMapSize,DescriptorSize,MemMapSize/DescriptorSize);
	Print(L"Get MemMapSize:%d,DescriptorSize:%d,count:%d\n",MemMapSize,DescriptorSize,MemMapSize/DescriptorSize);
	Print(L"Get EFI_MEMORY_DESCRIPTOR Structure:%018lx\n",MemMap);
	for(i = 0;i < E820Count;i++)
	{
		Print(L"MemoryMap (%10lx<->%10lx) %4d\n",LastE820->address,LastE820->address+LastE820->length,LastE820->type);
		LastE820++;
	}
#endif

	gBS->FreePool(MemMap);
}

#include "utils.h"

EFI_STATUS LoadFile(IN EFI_LOADED_IMAGE	*LoadedImage,
		CHAR16 *filename, PHYSICAL_ADDRESS addr, UINTN size)
{
	EFI_STATUS status		= EFI_SUCCESS;
	EFI_FILE_IO_INTERFACE	*Vol;
	EFI_FILE_HANDLE			RootFs;
	EFI_FILE_HANDLE			FileHandle;


	gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID*)&Vol);
	Vol->OpenVolume(Vol,&RootFs);
	// Open File
	status = RootFs->Open(RootFs, &FileHandle, filename, EFI_FILE_MODE_READ, 0);
	if(EFI_ERROR(status))
	{
		Print(L"Open %s Failed.\n", filename);
		return status;
	}

	Print(L"Input size: 0x%lx\n", size);
	// Get File Info
	EFI_FILE_INFO* FileInfo;
	UINTN bufsize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
	gBS->AllocatePool(EfiRuntimeServicesData, bufsize, (VOID**)&FileInfo); 
	FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &bufsize, FileInfo);

	bufsize = size;
	if (size == 0)
		bufsize = FileInfo->FileSize;

	Print(L"File loaded address: 0x%llx\n", addr);
	Print(L"File loaded size: 0x%llx\n", bufsize);
	
	// Alloc File Image Memory
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(bufsize + PAGE_SIZE - 1) / PAGE_SIZE, &addr);

	FileHandle->Read(FileHandle, &bufsize, (VOID*)addr);

	gBS->FreePool(FileInfo);
	FileHandle->Close(FileHandle);
	RootFs->Close(RootFs);

	return EFI_SUCCESS;
}
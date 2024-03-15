#ifndef _UTILS_H_
#define _UTILS_H_

	#include <Uefi.h>
	#include <Library/UefiLib.h>
	#include <Library/UefiBootServicesTableLib.h>
	#include <Library/UefiRuntimeServicesTableLib.h>
	#include <Protocol/SimpleFileSystem.h>
	#include <Protocol/LoadedImage.h>
	#include <Guid/FileInfo.h>
	#include "ElfLib.h"

	#define PAGE_SIZE	0x1000


    EFI_STATUS LoadFile(IN EFI_LOADED_IMAGE	*LoadedImage,
    		CHAR16 *filename, PHYSICAL_ADDRESS addr, UINTN size);

#endif /* _UTILS_H_ */
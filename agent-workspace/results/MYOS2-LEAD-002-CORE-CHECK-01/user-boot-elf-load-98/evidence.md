---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-ELF-LOAD-98
record_type: fixed_time_single_definition_static_source_quotes
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
produced_by: GPT-6（当前可见模型家族；backend UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-17
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
read_channel: connector
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# BE98｜固定time单一定义短锚

本文件所有源码引文为本次固定 `time@a039d9803ade` 原文的单一真实函数、结构体、匿名typedef定义或宏内连续1–5行，且均经源文件范围重新核对。`VERIFIED`标签只标相应定义文本，不能据此认证实际EFI状态、ABI宽度、ELF文件对象长度、合法装入或硬件启动。所有类型定义、宏、间接服务、内部计算、消费者与外部服务是否成功分别处理；未运行仓库代码。

### BE98-E001 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	EFI_LOADED_IMAGE	*LoadedImage;
	mbi_tags_header_s	*mbi_tags_header_ptr;
	mbi_tag_s			*mb2_infotag_ptr;
	PHYSICAL_ADDRESS	k_load_addr = 0;
```
证据类别：`CALLER_LOCAL`；专题`A`；固定time第29–32行。该锚仅支持其实际源码语义。

### BE98-E002 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID*)&LoadedImage);
```
证据类别：`BOOT_SERVICE_CALL`；专题`A`；固定time第34–34行。该锚仅支持其实际源码语义。

### BE98-E003 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	set_video_mode(ImageHandle, -1);
	load_elf_kernel(LoadedImage, L"\\boot\\kernel", &k_load_addr);
```
证据类别：`DIRECT_CALL`；专题`A`；固定time第38–39行。该锚仅支持其实际源码语义。

### BE98-E004 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
EFI_STATUS LoadFile(IN EFI_LOADED_IMAGE	*LoadedImage,
		CHAR16 *filename, PHYSICAL_ADDRESS addr, UINTN size)
{
```
证据类别：`FUNCTION_DEFINITION`；专题`A`；固定time第3–5行。该锚仅支持其实际源码语义。

### BE98-E005 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	EFI_STATUS status		= EFI_SUCCESS;
	EFI_FILE_IO_INTERFACE	*Vol;
	EFI_FILE_HANDLE			RootFs;
	EFI_FILE_HANDLE			FileHandle;
```
证据类别：`LOCAL_DECLARATIONS`；专题`A`；固定time第6–9行。该锚仅支持其实际源码语义。

### BE98-E006 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID*)&Vol);
	Vol->OpenVolume(Vol,&RootFs);
	// Open File
	status = RootFs->Open(RootFs, &FileHandle, filename, EFI_FILE_MODE_READ, 0);
```
证据类别：`DIRECT_AND_INDIRECT_CALL`；专题`A`；固定time第12–15行。该锚仅支持其实际源码语义。

### BE98-E007 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	if(EFI_ERROR(status))
	{
		Print(L"Open %s Failed.\n", filename);
		return status;
	}
```
证据类别：`ERROR_BRANCH`；专题`A`；固定time第16–20行。该锚仅支持其实际源码语义。

### BE98-E008 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	EFI_FILE_INFO* FileInfo;
	UINTN bufsize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
	gBS->AllocatePool(EfiRuntimeServicesData, bufsize, (VOID**)&FileInfo); 
	FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &bufsize, FileInfo);
```
证据类别：`FILE_INFO_REQUEST`；专题`A`；固定time第24–27行。该锚仅支持其实际源码语义。

### BE98-E009 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	bufsize = size;
	if (size == 0)
		bufsize = FileInfo->FileSize;
```
证据类别：`BYTE_COUNT_SELECTED`；专题`A`；固定time第29–31行。该锚仅支持其实际源码语义。

### BE98-E010 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	Print(L"File loaded address: 0x%llx\n", addr);
	Print(L"File loaded size: 0x%llx\n", bufsize);
```
证据类别：`LOG_ONLY`；专题`A`；固定time第33–34行。该锚仅支持其实际源码语义。

### BE98-E011 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	// Alloc File Image Memory
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(bufsize + PAGE_SIZE - 1) / PAGE_SIZE, &addr);

	FileHandle->Read(FileHandle, &bufsize, (VOID*)addr);
```
证据类别：`ALLOCATION_AND_READ_REQUEST`；专题`A`；固定time第36–40行。该锚仅支持其实际源码语义。

### BE98-E012 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	gBS->FreePool(FileInfo);
	FileHandle->Close(FileHandle);
	RootFs->Close(RootFs);

	return EFI_SUCCESS;
```
证据类别：`RESOURCE_CLOSE_AND_RETURN`；专题`A`；固定time第42–46行。该锚仅支持其实际源码语义。

### BE98-E013 [VERIFIED myloader/uefi/utils.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT	12
```
证据类别：`LOADER_MACRO`；专题`A`；固定time第14–14行。该锚仅支持其实际源码语义。

### BE98-E014 [VERIFIED myloader/uefi/utils.h::PAGE_SIZE]
```c
	#define PAGE_SIZE	(1 << PAGE_SHIFT)
```
证据类别：`LOADER_MACRO`；专题`A`；固定time第15–15行。该锚仅支持其实际源码语义。

### BE98-E015 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	EFI_STATUS status = EFI_SUCCESS;
	// Check Format
	PHYSICAL_ADDRESS raw_img_addr = 0x2000000;
	LoadFile(LoadedImage, filename, raw_img_addr, 0);
	BOOLEAN isElf = IsElfFormat((UINT8 *)raw_img_addr);
```
证据类别：`RAW_FILE_CALLER`；专题`A`；固定time第334–338行。该锚仅支持其实际源码语义。

### BE98-E016 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	BOOLEAN isElf = IsElfFormat((UINT8 *)raw_img_addr);
	if (!isElf)
	{
		Print(L"PANIC: Kernel: %s is NOT Elf format.\n", filename);
		while (1);
```
证据类别：`FORMAT_GATE`；专题`A`；固定time第338–342行。该锚仅支持其实际源码语义。

### BE98-E017 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	// Read info and load
	ELF_IMAGE_CONTEXT ElfCtx;
	status = ParseElfImage((VOID *)raw_img_addr, &ElfCtx);
	if (status != EFI_SUCCESS)
```
证据类别：`PARSE_GATE`；专题`A`；固定time第345–348行。该锚仅支持其实际源码语义。

### BE98-E018 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  Elf32_Ehdr  *Elf32Hdr;
  Elf64_Ehdr  *Elf64Hdr;

  ASSERT (ImageBase != NULL);

```
证据类别：`FORMAT_PREDICATE`；专题`B`；固定time第24–28行。该锚仅支持其实际源码语义。

### BE98-E019 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  if ((Elf32Hdr->e_ident[EI_MAG0] != ELFMAG0) ||
      (Elf32Hdr->e_ident[EI_MAG1] != ELFMAG1) ||
      (Elf32Hdr->e_ident[EI_MAG1] != ELFMAG1) ||
      (Elf32Hdr->e_ident[EI_MAG2] != ELFMAG2)
      )
```
证据类别：`MAGIC_PREDICATE`；专题`B`；固定time第34–38行。该锚仅支持其实际源码语义。

### BE98-E020 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  {
    return FALSE;
  }
```
证据类别：`FORMAT_ERROR`；专题`B`；固定time第39–41行。该锚仅支持其实际源码语义。

### BE98-E021 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  if (Elf32Hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
    return FALSE;
  }
```
证据类别：`ENDIAN_GATE`；专题`B`；固定time第46–48行。该锚仅支持其实际源码语义。

### BE98-E022 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  if (Elf32Hdr->e_ident[EI_CLASS] == ELFCLASS64) {
    Elf64Hdr = (Elf64_Ehdr *)Elf32Hdr;
    Elf32Hdr = NULL;
```
证据类别：`ELF64_SELECTED`；专题`B`；固定time第53–55行。该锚仅支持其实际源码语义。

### BE98-E023 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  } else if (Elf32Hdr->e_ident[EI_CLASS] == ELFCLASS32) {
    Elf64Hdr = NULL;
  } else {
    return FALSE;
  }
```
证据类别：`CLASS_BRANCH`；专题`B`；固定time第56–60行。该锚仅支持其实际源码语义。

### BE98-E024 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
    if (Elf64Hdr->e_machine != EM_X86_64) {
      return FALSE;
    }
```
证据类别：`MACHINE_GATE`；专题`B`；固定time第66–68行。该锚仅支持其实际源码语义。

### BE98-E025 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
    if ((Elf64Hdr->e_type != ET_EXEC) && (Elf64Hdr->e_type != ET_DYN)) {
      return FALSE;
    }
```
证据类别：`TYPE_GATE`；专题`B`；固定time第73–75行。该锚仅支持其实际源码语义。

### BE98-E026 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
    if (Elf64Hdr->e_version != EV_CURRENT) {
      return FALSE;
    }
```
证据类别：`VERSION_GATE`；专题`B`；固定time第80–82行。该锚仅支持其实际源码语义。

### BE98-E027 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::IsElfFormat]
```c
  return TRUE;
}
```
证据类别：`PREDICATE_SUCCESS`；专题`B`；固定time第106–107行。该锚仅支持其实际源码语义。

### BE98-E028 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EI_MAG1]
```c
#define EI_MAG1        1  /* Magic number, byte 1. */
```
证据类别：`MACRO`；专题`B`；固定time第63–63行。该锚仅支持其实际源码语义。

### BE98-E029 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EI_MAG2]
```c
#define EI_MAG2        2  /* Magic number, byte 2. */
```
证据类别：`MACRO`；专题`B`；固定time第64–64行。该锚仅支持其实际源码语义。

### BE98-E030 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EI_MAG3]
```c
#define EI_MAG3        3  /* Magic number, byte 3. */
```
证据类别：`MACRO`；专题`B`；固定time第65–65行。该锚仅支持其实际源码语义。

### BE98-E031 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFMAG3]
```c
#define ELFMAG3  'F'
```
证据类别：`MACRO`；专题`B`；固定time第79–79行。该锚仅支持其实际源码语义。

### BE98-E032 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFDATA2LSB]
```c
#define ELFDATA2LSB  1  /* 2's complement little-endian. */
```
证据类别：`MACRO`；专题`B`；固定time第94–94行。该锚仅支持其实际源码语义。

### BE98-E033 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFCLASS64]
```c
#define ELFCLASS64    2 /* 64-bit architecture. */
```
证据类别：`MACRO`；专题`B`；固定time第90–90行。该锚仅支持其实际源码语义。

### BE98-E034 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EM_X86_64]
```c
#define EM_X86_64       62        /* Advanced Micro Devices x86-64 */
```
证据类别：`MACRO`；专题`B`；固定time第178–178行。该锚仅支持其实际源码语义。

### BE98-E035 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ET_EXEC]
```c
#define ET_EXEC    2      /* Executable. */
```
证据类别：`MACRO`；专题`B`；固定time第128–128行。该锚仅支持其实际源码语义。

### BE98-E036 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ET_DYN]
```c
#define ET_DYN     3      /* Shared object. */
```
证据类别：`MACRO`；专题`B`；固定time第129–129行。该锚仅支持其实际源码语义。

### BE98-E037 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EV_CURRENT]
```c
#define EV_CURRENT  1
```
证据类别：`MACRO`；专题`B`；固定time第85–85行。该锚仅支持其实际源码语义。

### BE98-E038 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::PT_LOAD]
```c
#define PT_LOAD           1          /* Loadable segment. */
```
证据类别：`MACRO`；专题`B`；固定time第259–259行。该锚仅支持其实际源码语义。

### BE98-E039 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Ehdr]
```c
typedef struct {
  unsigned char    e_ident[EI_NIDENT]; /* File identification. */
  Elf64_Half       e_type;             /* File type. */
  Elf64_Half       e_machine;          /* Machine architecture. */
  Elf64_Word       e_version;          /* ELF format version. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第69–73行。该锚仅支持其实际源码语义。

### BE98-E040 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Ehdr]
```c
  Elf64_Addr       e_entry;            /* Entry point. */
  Elf64_Off        e_phoff;            /* Program header file offset. */
  Elf64_Off        e_shoff;            /* Section header file offset. */
  Elf64_Word       e_flags;            /* Architecture-specific flags. */
  Elf64_Half       e_ehsize;           /* Size of ELF header in bytes. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第74–78行。该锚仅支持其实际源码语义。

### BE98-E041 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Ehdr]
```c
  Elf64_Half       e_phentsize;        /* Size of program header entry. */
  Elf64_Half       e_phnum;            /* Number of program header entries. */
  Elf64_Half       e_shentsize;        /* Size of section header entry. */
  Elf64_Half       e_shnum;            /* Number of section header entries. */
  Elf64_Half       e_shstrndx;         /* Section name strings section. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第79–83行。该锚仅支持其实际源码语义。

### BE98-E042 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Shdr]
```c
typedef struct {
  Elf64_Word     sh_name;      /* Section name (index into the
             section header string table). */
  Elf64_Word     sh_type;      /* Section type. */
  Elf64_Xword    sh_flags;     /* Section flags. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第90–94行。该锚仅支持其实际源码语义。

### BE98-E043 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Shdr]
```c
  Elf64_Addr     sh_addr;      /* Address in memory image. */
  Elf64_Off      sh_offset;    /* Offset in file. */
  Elf64_Xword    sh_size;      /* Size in bytes. */
  Elf64_Word     sh_link;      /* Index of a related section. */
  Elf64_Word     sh_info;      /* Depends on section type. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第95–99行。该锚仅支持其实际源码语义。

### BE98-E044 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Phdr]
```c
typedef struct {
  Elf64_Word     p_type;   /* Entry type. */
  Elf64_Word     p_flags;  /* Access permission flags. */
  Elf64_Off      p_offset; /* File offset of contents. */
  Elf64_Addr     p_vaddr;  /* Virtual address in memory image. */
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第108–112行。该锚仅支持其实际源码语义。

### BE98-E045 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Phdr]
```c
  Elf64_Addr     p_paddr;  /* Physical address (not used). */
  Elf64_Xword    p_filesz; /* Size of contents in file. */
  Elf64_Xword    p_memsz;  /* Size of contents in memory. */
  Elf64_Xword    p_align;  /* Alignment in memory and file. */
} Elf64_Phdr;
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第113–117行。该锚仅支持其实际源码语义。

### BE98-E046 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
typedef struct {
  RETURN_STATUS    ParseStatus;            ///< Return the status after ParseElfImage().
  UINT8            *FileBase;              ///< The source location in memory.
  UINTN            FileSize;               ///< The size including sections that don't require loading.
  UINT8            *PreferredImageAddress; ///< The preferred image to be loaded. No relocation is needed if loaded to this address.
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第19–23行。该锚仅支持其实际源码语义。

### BE98-E047 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
  BOOLEAN          ReloadRequired;         ///< The image needs a new memory location for running.
  UINT8            *ImageAddress;          ///< The destination memory address set by caller.
  UINTN            ImageSize;              ///< The memory size for loading and execution.
  UINT32           EiClass;
  UINT32           ShNum;
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第24–28行。该锚仅支持其实际源码语义。

### BE98-E048 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
  UINT32           PhNum;
  UINTN            ShStrOff;
  UINTN            ShStrLen;
  UINTN            EntryPoint;           ///< Return the actual entry point after LoadElfImage().
} ELF_IMAGE_CONTEXT;
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第29–33行。该锚仅支持其实际源码语义。

### BE98-E049 [VERIFIED myloader/uefi/ElfLib.h::SEGMENT_INFO]
```c
typedef struct {
  UINT32    PtType;
  UINTN     Offset;
  UINTN     Length;
  UINTN     MemLen;
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第35–39行。该锚仅支持其实际源码语义。

### BE98-E050 [VERIFIED myloader/uefi/ElfLib.h::SEGMENT_INFO]
```c
  UINTN     MemAddr;
  UINTN     Alignment;
} SEGMENT_INFO;
```
证据类别：`STRUCT_DEF`；专题`B`；固定time第40–42行。该锚仅支持其实际源码语义。

### BE98-E051 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::GetElf64SectionByIndex]
```c
  Ehdr = (Elf64_Ehdr *)ImageBase;
  if (Index >= Ehdr->e_shnum) {
    return NULL;
  }
```
证据类别：`INDEX_BOUND`；专题`B`；固定time第27–30行。该锚仅支持其实际源码语义。

### BE98-E052 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::GetElf64SectionByIndex]
```c
  return (Elf64_Shdr *)(ImageBase + Ehdr->e_shoff + Index * Ehdr->e_shentsize);
}
```
证据类别：`INDEX_POINTER`；专题`B`；固定time第32–33行。该锚仅支持其实际源码语义。

### BE98-E053 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::GetElf64SegmentByIndex]
```c
  Ehdr = (Elf64_Ehdr *)ImageBase;
  if (Index >= Ehdr->e_phnum) {
    return NULL;
  }
```
证据类别：`INDEX_BOUND`；专题`B`；固定time第51–54行。该锚仅支持其实际源码语义。

### BE98-E054 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::GetElf64SegmentByIndex]
```c
  return (Elf64_Phdr *)(ImageBase + Ehdr->e_phoff + Index * Ehdr->e_phentsize);
}
```
证据类别：`INDEX_POINTER`；专题`B`；固定time第56–57行。该锚仅支持其实际源码语义。

### BE98-E055 [VERIFIED myloader/uefi/ElfLib/ElfLibInternal.h::ELF_NEXT_ENTRY]
```c
#define ELF_NEXT_ENTRY(EntryType, Current, EntrySize) \
              ((EntryType *) ((UINT8 *)Current + EntrySize))
```
证据类别：`MACRO`；专题`B`；固定time第20–21行。该锚仅支持其实际源码语义。

### BE98-E056 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  if (ElfCt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (ElfCt, sizeof (ELF_IMAGE_CONTEXT));
```
证据类别：`CONTEXT_INIT`；专题`B`；固定time第244–248行。该锚仅支持其实际源码语义。

### BE98-E057 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  if (ImageBase == NULL) {
    return (ElfCt->ParseStatus = EFI_INVALID_PARAMETER);
  }

  ElfCt->FileBase = (UINT8 *)ImageBase;
```
证据类别：`PARSE_PRECHECK`；专题`B`；固定time第250–254行。该锚仅支持其实际源码语义。

### BE98-E058 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  ElfCt->FileBase = (UINT8 *)ImageBase;
  if (!IsElfFormat (ElfCt->FileBase)) {
    return (ElfCt->ParseStatus = EFI_UNSUPPORTED);
  }
```
证据类别：`FORMAT_STATUS`；专题`B`；固定time第254–257行。该锚仅支持其实际源码语义。

### BE98-E059 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  Elf32Hdr       = (Elf32_Ehdr *)ElfCt->FileBase;
  ElfCt->EiClass = Elf32Hdr->e_ident[EI_CLASS];
```
证据类别：`CLASS_SET`；专题`B`；固定time第259–260行。该锚仅支持其实际源码语义。

### BE98-E060 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    Elf64Hdr = (Elf64_Ehdr *)Elf32Hdr;
    if ((Elf64Hdr->e_type != ET_EXEC) && (Elf64Hdr->e_type != ET_DYN)) {
      return (ElfCt->ParseStatus = EFI_UNSUPPORTED);
    }
```
证据类别：`ELF64_TYPE_CHECK`；专题`B`；固定time第277–280行。该锚仅支持其实际源码语义。

### BE98-E061 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    Elf64Shdr = (Elf64_Shdr *)GetElf64SectionByIndex (ElfCt->FileBase, Elf64Hdr->e_shstrndx);
    if (Elf64Shdr == NULL) {
      return (ElfCt->ParseStatus = EFI_UNSUPPORTED);
    }
```
证据类别：`SECTION_NAME_INDEX`；专题`B`；固定time第282–285行。该锚仅支持其实际源码语义。

### BE98-E062 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    ElfCt->EntryPoint = (UINTN)Elf64Hdr->e_entry;
    ElfCt->ShNum      = Elf64Hdr->e_shnum;
    ElfCt->PhNum      = Elf64Hdr->e_phnum;
    ElfCt->ShStrLen   = (UINT32)Elf64Shdr->sh_size;
    ElfCt->ShStrOff   = (UINT32)Elf64Shdr->sh_offset;
```
证据类别：`CONTEXT_FIELD_SOURCE`；专题`B`；固定time第287–291行。该锚仅支持其实际源码语义。

### BE98-E063 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  End                   = 0;
  Base                  = MAX_UINT32;
  ElfCt->ReloadRequired = FALSE;
  for (Index = 0; Index < ElfCt->PhNum; Index++) {
    Status = GetElfSegmentInfo (ElfCt->FileBase, ElfCt->EiClass, Index, &SegInfo);
```
证据类别：`SEGMENT_RANGE_INIT`；专题`B`；固定time第297–301行。该锚仅支持其实际源码语义。

### BE98-E064 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    Status = GetElfSegmentInfo (ElfCt->FileBase, ElfCt->EiClass, Index, &SegInfo);
    ASSERT_EFI_ERROR (Status);

    if (SegInfo.PtType != PT_LOAD) {
      continue;
```
证据类别：`SEGMENT_STATUS_AND_FILTER`；专题`B`；固定time第301–305行。该锚仅支持其实际源码语义。

### BE98-E065 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    if (SegInfo.MemLen != SegInfo.Length) {
      //
      // Not enough space to execute at current location.
      //
      ElfCt->ReloadRequired = TRUE;
```
证据类别：`RELOAD_FLAG_CONDITION`；专题`B`；固定time第308–312行。该锚仅支持其实际源码语义。

### BE98-E066 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
      //
      ElfCt->ReloadRequired = TRUE;
    }
```
证据类别：`RELOAD_FLAG_WRITE`；专题`B`；固定time第311–313行。该锚仅支持其实际源码语义。

### BE98-E067 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    if (Base > (SegInfo.MemAddr & ~(EFI_PAGE_SIZE - 1))) {
      Base = SegInfo.MemAddr & ~(EFI_PAGE_SIZE - 1);
    }
```
证据类别：`IMAGE_BASE_CALC`；专题`B`；固定time第315–317行。该锚仅支持其实际源码语义。

### BE98-E068 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    if (End < ALIGN_VALUE (SegInfo.MemAddr + SegInfo.MemLen, EFI_PAGE_SIZE) - 1) {
      End = ALIGN_VALUE (SegInfo.MemAddr + SegInfo.MemLen, EFI_PAGE_SIZE) - 1;
    }
```
证据类别：`IMAGE_END_CALC`；专题`B`；固定time第319–321行。该锚仅支持其实际源码语义。

### BE98-E069 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  ElfCt->ImageSize             = End - Base + 1;
  ElfCt->PreferredImageAddress = (VOID *)Base;

  CalculateElfFileSize (ElfCt, &ElfCt->FileSize);
  return (ElfCt->ParseStatus = EFI_SUCCESS);
```
证据类别：`CONTEXT_SIZE_AND_RETURN`；专题`B`；固定time第327–331行。该锚仅支持其实际源码语义。

### BE98-E070 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::GetElfSegmentInfo]
```c
  } else if (EiClass == ELFCLASS64) {
    Elf64Phdr = GetElf64SegmentByIndex (ImageBase, Index);
    if (Elf64Phdr != NULL) {
      SegInfo->PtType    = Elf64Phdr->p_type;
      SegInfo->Offset    = (UINTN)Elf64Phdr->p_offset;
```
证据类别：`PROGRAM_HEADER_FIELD_SOURCE`；专题`B`；固定time第198–202行。该锚仅支持其实际源码语义。

### BE98-E071 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::GetElfSegmentInfo]
```c
      SegInfo->Length    = (UINTN)Elf64Phdr->p_filesz;
      SegInfo->MemLen    = (UINTN)Elf64Phdr->p_memsz;
      SegInfo->MemAddr   = (UINTN)Elf64Phdr->p_paddr;
      SegInfo->Alignment = (UINTN)Elf64Phdr->p_align;
      return EFI_SUCCESS;
```
证据类别：`PROGRAM_HEADER_FIELD_SOURCE`；专题`B`；固定time第203–207行。该锚仅支持其实际源码语义。

### BE98-E072 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  if ((ElfCt == NULL) || (FileSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
```
证据类别：`SIZE_PRECHECK`；专题`B`；固定time第133–135行。该锚仅支持其实际源码语义。

### BE98-E073 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  // Use last section as end of file
  Status = GetElfSectionPos (ElfCt, ElfCt->ShNum - 1, &Offset, &Size);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
```
证据类别：`LAST_SECTION_STATUS`；专题`B`；固定time第137–141行。该锚仅支持其实际源码语义。

### BE98-E074 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  FileSize1 = Offset + Size;
```
证据类别：`LAST_SECTION_END`；专题`B`；固定time第143–143行。该锚仅支持其实际源码语义。

### BE98-E075 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  } else if (ElfCt->EiClass == ELFCLASS64) {
    Elf64Hdr  = (Elf64_Ehdr *)ElfCt->FileBase;
    FileSize2 = ((UINTN)Elf64Hdr->e_shoff + (UINTN)(Elf64Hdr->e_shentsize * Elf64Hdr->e_shnum));
  }

```
证据类别：`SECTION_TABLE_SIZE`；专题`B`；固定time第150–154行。该锚仅支持其实际源码语义。

### BE98-E076 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  *FileSize = MAX (FileSize1, FileSize2);

  return EFI_SUCCESS;
```
证据类别：`FINAL_COMPUTED_FILESIZE`；专题`B`；固定time第155–157行。该锚仅支持其实际源码语义。

### BE98-E077 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::GetElfSectionPos]
```c
  if ((ElfCt == NULL) || (Offset == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (ElfCt->ParseStatus)) {
```
证据类别：`SECTION_POS_PRECHECK`；专题`B`；固定time第456–460行。该锚仅支持其实际源码语义。

### BE98-E078 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::GetElfSectionPos]
```c
    if (Elf64Shdr != NULL) {
      *Offset = (UINTN)Elf64Shdr->sh_offset;
      *Size   = (UINTN)Elf64Shdr->sh_size;
      return EFI_SUCCESS;
    }
```
证据类别：`SECTION_POS_VALUES`；专题`B`；固定time第473–477行。该锚仅支持其实际源码语义。

### BE98-E079 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	status = ParseElfImage((VOID *)raw_img_addr, &ElfCtx);
	if (status != EFI_SUCCESS)
	{
		Print(L"PANIC: ElfParse kernel: %s failed.\n", filename);
		while (1);
```
证据类别：`PARSE_STATUS_CONSUMER`；专题`C`；固定time第347–351行。该锚仅支持其实际源码语义。

### BE98-E080 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	Print(L"Elf file size: 0x%llx\n", ElfCtx.FileSize);
	Print(L"Elf Image size: 0x%llx\n", ElfCtx.ImageSize);
	Print(L"Elf Image loaded address: 0x%llx\n", ElfCtx.ImageAddress);
	Print(L"Elf Image preferred address: 0x%llx\n", ElfCtx.PreferredImageAddress);
	Print(L"Elf Image entry point: 0x%llx\n", ElfCtx.EntryPoint);
```
证据类别：`LOG_ONLY`；专题`C`；固定time第353–357行。该锚仅支持其实际源码语义。

### BE98-E081 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	EFI_PHYSICAL_ADDRESS loaded_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.ImageAddress;
	*entry_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.EntryPoint;
	// Alloc Image Memory
	ElfCtx.ImageAddress = ElfCtx.PreferredImageAddress;
```
证据类别：`ALLOC_TARGET_AND_ENTRY`；专题`C`；固定time第361–364行。该锚仅支持其实际源码语义。

### BE98-E082 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(ElfCtx.ImageSize + PAGE_SIZE - 1) / PAGE_SIZE, &loaded_addr);
	LoadElfImage(&ElfCtx);

	return status;
```
证据类别：`ALLOC_LOAD_RETURN`；专题`C`；固定time第365–369行。该锚仅支持其实际源码语义。

### BE98-E083 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::LoadElfImage]
```c
  if (ElfCt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (ElfCt->ParseStatus)) {
```
证据类别：`LOAD_CONTEXT_PRECHECK`；专题`C`；固定time第357–361行。该锚仅支持其实际源码语义。

### BE98-E084 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::LoadElfImage]
```c
  if (ElfCt->ImageAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
```
证据类别：`LOAD_ADDR_CHECK`；专题`C`；固定time第365–369行。该锚仅支持其实际源码语义。

### BE98-E085 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::LoadElfImage]
```c
  if (ElfCt->EiClass == ELFCLASS32) {
    Status = LoadElf32Image (ElfCt);
  } else if (ElfCt->EiClass == ELFCLASS64) {
    Status = LoadElf64Image (ElfCt);
  }
```
证据类别：`CLASS_DISPATCH`；专题`C`；固定time第370–374行。该锚仅支持其实际源码语义。

### BE98-E086 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::LoadElfImage]
```c
  return Status;
```
证据类别：`CLASS_STATUS_RETURN`；专题`C`；固定time第376–376行。该锚仅支持其实际源码语义。

### BE98-E087 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
  Elf64_Ehdr  *Ehdr;
  Elf64_Phdr  *Phdr;
  UINT16      Index;
  UINTN       Delta;

```
证据类别：`LOADER_LOCALS`；专题`C`；固定time第443–447行。该锚仅支持其实际源码语义。

### BE98-E088 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
  Ehdr = (Elf64_Ehdr *)ElfCt->FileBase;

  for ( Index = 0, Phdr = (Elf64_Phdr *)(ElfCt->FileBase + Ehdr->e_phoff)
        ; Index < Ehdr->e_phnum
        ; Index++, Phdr = ELF_NEXT_ENTRY (Elf64_Phdr, Phdr, Ehdr->e_phentsize)
```
证据类别：`SEGMENT_ITERATION`；专题`C`；固定time第453–457行。该锚仅支持其实际源码语义。

### BE98-E089 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
    if ((Phdr->p_type != PT_LOAD) ||
        (Phdr->p_memsz == 0))
    {
      continue;
    }
```
证据类别：`SEGMENT_SELECT`；专题`C`；固定time第463–467行。该锚仅支持其实际源码语义。

### BE98-E090 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
    Delta = (UINTN)Phdr->p_paddr - (UINTN)ElfCt->PreferredImageAddress;
    CopyMem (ElfCt->ImageAddress + Delta, ElfCt->FileBase + (UINTN)Phdr->p_offset, (UINTN)Phdr->p_filesz);
    ZeroMem (ElfCt->ImageAddress + Delta + (UINTN)Phdr->p_filesz, (UINTN)(Phdr->p_memsz - Phdr->p_filesz));
  }
```
证据类别：`SEGMENT_COPY_ZERO`；专题`C`；固定time第473–476行。该锚仅支持其实际源码语义。

### BE98-E091 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
  if (ElfCt->ImageAddress != ElfCt->PreferredImageAddress) {
    RelocateElf64Sections (ElfCt);
  }

  return EFI_SUCCESS;
```
证据类别：`RELOCATION_BRANCH`；专题`C`；固定time第481–485行。该锚仅支持其实际源码语义。

### BE98-E092 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::RelocateElf64Sections]
```c
  Ehdr = (Elf64_Ehdr *)ElfCt->FileBase;
  if (Ehdr->e_machine != EM_X86_64) {
    return EFI_UNSUPPORTED;
  }

```
证据类别：`RELOCATION_INTERFACE_GATE`；专题`C`；固定time第369–373行。该锚仅支持其实际源码语义。

### BE98-E093 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::RelocateElf64Sections]
```c
  Delta             = (UINTN)ElfCt->ImageAddress - (UINTN)ElfCt->PreferredImageAddress;
  ElfCt->EntryPoint = (UINTN)(Ehdr->e_entry + Delta);
```
证据类别：`RELOCATION_ENTRY_REWRITE`；专题`C`；固定time第374–375行。该锚仅支持其实际源码语义。

### BE98-E094 [VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::RelocateElf64Sections]
```c
  if (Ehdr->e_type == ET_DYN) {
    DEBUG ((DEBUG_INFO, "DYN ELF: Relocate using dynamic sections...\n"));
    Status = RelocateElf64Dynamic (ElfCt);
    ASSERT_EFI_ERROR (Status);
    return Status;
```
证据类别：`DYNAMIC_BRANCH_STATUS`；专题`C`；固定time第380–384行。该锚仅支持其实际源码语义。

### BE98-E095 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	Print(L"Allocate Pages At: 0x%llx.\n", pages);
	Print(L"Entry Point At: 0x%llx.\n", k_load_addr);
	Print(L"Call ExitBootServices.\n");
	Print(L"Jmp to Kernel.\n");
```
证据类别：`CALLER_LOGS`；专题`C`；固定time第62–65行。该锚仅支持其实际源码语义。

### BE98-E096 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	gBS->CloseProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, ImageHandle, NULL);
```
证据类别：`CALLER_CLOSE`；专题`C`；固定time第67–68行。该锚仅支持其实际源码语义。

### BE98-E097 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	void (*func)(void);
	func = (void *)k_load_addr;
```
证据类别：`ENTRY_POINTER`；专题`C`；固定time第70–71行。该锚仅支持其实际源码语义。

### BE98-E098 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	asm volatile(	"movq	%0,		%%rax		\n"
					"movq	%1,		%%rbx		\n"
					"callq	*%2					\n"
				:
```
证据类别：`ENTRY_CALL_PREP`；专题`C`；固定time第75–78行。该锚仅支持其实际源码语义。

### BE98-E099 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
				:	"i" (MULTIBOOT2_BOOTLOADER_MAGIC),
					"m" (pages),
					"m" (func)
				:
				);
```
证据类别：`ENTRY_CALL_ARGUMENTS`；专题`C`；固定time第79–83行。该锚仅支持其实际源码语义。

### BE98-E100 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::EI_MAG0]
```c
#define EI_MAG0        0  /* Magic number, byte 0. */
```
证据类别：`MACRO`；专题`B`；固定time第62–62行。该锚仅支持其实际源码语义。

### BE98-E101 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFMAG0]
```c
#define ELFMAG0  0x7f
```
证据类别：`MACRO`；专题`B`；固定time第76–76行。该锚仅支持其实际源码语义。

### BE98-E102 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFMAG1]
```c
#define ELFMAG1  'E'
```
证据类别：`MACRO`；专题`B`；固定time第77–77行。该锚仅支持其实际源码语义。

### BE98-E103 [VERIFIED myloader/uefi/ElfLib/ElfCommon.h::ELFMAG2]
```c
#define ELFMAG2  'L'
```
证据类别：`MACRO`；专题`B`；固定time第78–78行。该锚仅支持其实际源码语义。

## 使用约束

BE98短锚不构成任何结构体背后对象可读长度的证明。EFI函数调用（例如GetInfo、Read、AllocatePages）和库函数（例如CopyMem、ZeroMem）只说明源级调用，具体外部返回约定未在固定仓库验证的部分标未证。相同ELF定义中的不同字段可分段锚定，但任何独立宏均须一宏一锚。无ELF32装入分支重审、无重定位条目算法或用户态解释器调查。实际状态返回、申请内存和入口执行需独立证据。

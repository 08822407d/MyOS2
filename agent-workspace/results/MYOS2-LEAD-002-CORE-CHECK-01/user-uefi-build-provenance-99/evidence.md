---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-UEFI-BUILD-PROVENANCE-99
record_type: fixed_time_declaration_command_type_and_function_short_quote_index
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
produced_by: GPT-6（可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-18
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第99批固定time单定义/唯一声明短锚

本文件所有源码取自固定time，真实引文1–5物理行。C结构、函数、独立typedef/宏属于C证据；INF使用真实单个命名节/字段；CMake仅用唯一命令或其内部行；Shell仅引用实际函数体及唯一顶层赋值/调用。类型、宏、头文件包含与外部EDK具体实现分离。CMake/INF/Shell的文本属于声明或命令来源，不当作执行或制品身份。有效ID必须具有路径、真实定位符、逐字原文与类别。未运行仓库实现或任何安装命令。

### UB99-E001 [VERIFIED myloader/uefi/bootloader.inf::[Defines].MODULE_TYPE]
```ini
  MODULE_TYPE                    = UEFI_APPLICATION
```
类别：`INF_FIELD`；专题`A`；固定time第5–5行。仅为声明或源级定义，不认证构建。

### UB99-E002 [VERIFIED myloader/uefi/bootloader.inf::[Defines].ENTRY_POINT]
```ini
  ENTRY_POINT                    = UefiMain
```
类别：`INF_FIELD`；专题`A`；固定time第7–7行。仅为声明或源级定义，不认证构建。

### UB99-E003 [VERIFIED myloader/uefi/bootloader.inf::[Defines].BASE_NAME]
```ini
  BASE_NAME                      = bootloader
```
类别：`INF_FIELD`；专题`A`；固定time第3–3行。仅为声明或源级定义，不认证构建。

### UB99-E004 [VERIFIED myloader/uefi/bootloader.inf::[Defines].FILE_GUID]
```ini
  FILE_GUID                      = 6987936E-f301-4a63-9661-fc6030dcc860
```
类别：`INF_FIELD`；专题`A`；固定time第4–4行。仅为声明或源级定义，不认证构建。

### UB99-E005 [VERIFIED myloader/uefi/bootloader.inf::[Sources]]
```ini
  bootloader.h
  bootloader.c
  multiboot2.h
  multiboot2_utils.c
  utils.h
```
类别：`INF_SECTION`；专题`A`；固定time第10–14行。仅为声明或源级定义，不认证构建。

### UB99-E006 [VERIFIED myloader/uefi/bootloader.inf::[Sources]]
```ini
  utils.c
  ElfLib.h
  ElfLib/ElfLibInternal.h
  ElfLib/ElfCommon.h
  ElfLib/Elf32.h
```
类别：`INF_SECTION`；专题`A`；固定time第15–19行。仅为声明或源级定义，不认证构建。

### UB99-E007 [VERIFIED myloader/uefi/bootloader.inf::[Sources]]
```ini
  ElfLib/Elf64.h
  ElfLib/ElfLibInternal.h
  ElfLib/ElfLib.c
  ElfLib/Elf32Lib.c
  ElfLib/Elf64Lib.c
```
类别：`INF_SECTION`；专题`A`；固定time第20–24行。仅为声明或源级定义，不认证构建。

### UB99-E008 [VERIFIED myloader/uefi/bootloader.inf::[Packages]]
```ini
  MdePkg/MdePkg.dec
  MdeModulePkg/MdeModulePkg.dec
  PcAtChipsetPkg/PcAtChipsetPkg.dec
  UefiPayloadPkg/UefiPayloadPkg.dec
```
类别：`INF_SECTION`；专题`A`；固定time第27–30行。仅为声明或源级定义，不认证构建。

### UB99-E009 [VERIFIED myloader/uefi/bootloader.inf::[LibraryClasses]]
```ini
  UefiApplicationEntryPoint
  UefiLib
  UefiBootServicesTableLib
  DebugLib
  UefiRuntimeServicesTableLib
```
类别：`INF_SECTION`；专题`A`；固定time第33–37行。仅为声明或源级定义，不认证构建。

### UB99-E010 [VERIFIED myloader/uefi/bootloader.inf::[LibraryClasses]]
```ini
  BaseMemoryLib
  PcdLib
  MemoryAllocationLib
  HobLib
  BaseLib
```
类别：`INF_SECTION`；专题`A`；固定time第38–42行。仅为声明或源级定义，不认证构建。

### UB99-E011 [VERIFIED myloader/uefi/bootloader.inf::[Protocols]]
```ini
  gEfiGraphicsOutputProtocolGuid
  gEfiSimpleFileSystemProtocolGuid
  gEfiLoadedImageProtocolGuid
  gEfiUnicodeCollation2ProtocolGuid                       # ALWAYS_CONSUMED
  gEfiMpServiceProtocolGuid
```
类别：`INF_SECTION`；专题`A`；固定time第45–49行。仅为声明或源级定义，不认证构建。

### UB99-E012 [VERIFIED myloader/uefi/bootloader.inf::[Ppis]]
```ini
[Ppis]
  gEfiPeiLoadFilePpiGuid                 ## PRODUCES
```
类别：`INF_SECTION`；专题`A`；固定time第51–52行。仅为声明或源级定义，不认证构建。

### UB99-E013 [VERIFIED myloader/uefi/bootloader.inf::[Pcd]]
```ini
[Pcd]
  gPcAtChipsetPkgTokenSpaceGuid.PcdRtcIndexRegister
  gPcAtChipsetPkgTokenSpaceGuid.PcdRtcTargetRegister
```
类别：`INF_SECTION`；专题`A`；固定time第54–56行。仅为声明或源级定义，不认证构建。

### UB99-E014 [VERIFIED myloader/uefi/bootloader.inf::[Guids]]
```ini
[Guids]
  gEfiFileInfoGuid
  gUniversalPayloadExtraDataGuid         ## PRODUCES
```
类别：`INF_SECTION`；专题`A`；固定time第58–60行。仅为声明或源级定义，不认证构建。

### UB99-E015 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_LOADED_IMAGE	*LoadedImage;
```
类别：`C_FUNCTION`；专题`A`；固定time第27–29行。仅为声明或源级定义，不认证构建。

### UB99-E016 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID*)&LoadedImage);
```
类别：`C_FUNCTION`；专题`A`；固定time第34–34行。仅为声明或源级定义，不认证构建。

### UB99-E017 [VERIFIED myloader/CMakeLists.txt::project(MyOS2_BootLoader)]
```cmake
project(MyOS2_BootLoader ASM C)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`A`；固定time第3–3行。仅为声明或源级定义，不认证构建。

### UB99-E018 [VERIFIED myloader/CMakeLists.txt::install(PROGRAMS BOOTX64.EFI)]
```cmake
install(PROGRAMS BOOTX64.EFI DESTINATION EFI/BOOT)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`A`；固定time第5–5行。仅为声明或源级定义，不认证构建。

### UB99-E019 [VERIFIED myloader/uefi/utils.h::include(Uefi.h)]
```c
	#include <Uefi.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第4–4行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E020 [VERIFIED myloader/uefi/utils.h::include(UefiBootServicesTableLib.h)]
```c
	#include <Library/UefiBootServicesTableLib.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第6–6行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E021 [VERIFIED myloader/uefi/utils.h::include(BaseMemoryLib.h)]
```c
	#include <Library/BaseMemoryLib.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第8–8行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E022 [VERIFIED myloader/uefi/utils.h::include(SimpleFileSystem.h)]
```c
	#include <Protocol/SimpleFileSystem.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第9–9行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E023 [VERIFIED myloader/uefi/utils.h::include(LoadedImage.h)]
```c
	#include <Protocol/LoadedImage.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第10–10行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E024 [VERIFIED myloader/uefi/utils.h::include(ElfLib.h)]
```c
	#include "ElfLib.h"
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第12–12行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E025 [VERIFIED myloader/uefi/bootloader.h::include(PiDxeCis.h)]
```c
		#include "Pi/PiDxeCis.h"
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第11–11行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E026 [VERIFIED myloader/uefi/bootloader.h::include(MpService.h)]
```c
		#include "Protocol/MpService.h"
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第12–12行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E027 [VERIFIED myloader/uefi/bootloader.h::include(multiboot2.h)]
```c
	#include "multiboot2.h"
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第15–15行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E028 [VERIFIED myloader/uefi/ElfLib.h::include(PiPei.h)]
```c
#include <PiPei.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第12–12行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E029 [VERIFIED myloader/uefi/ElfLib/ElfLibInternal.h::include(BaseLib.h)]
```c
#include <Library/BaseLib.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第12–12行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E030 [VERIFIED myloader/uefi/ElfLib/ElfLibInternal.h::include(DebugLib.h)]
```c
#include <Library/DebugLib.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第13–13行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E031 [VERIFIED myloader/uefi/ElfLib/ElfLibInternal.h::include(BaseMemoryLib.h)]
```c
#include <Library/BaseMemoryLib.h>
```
类别：`HEADER_INCLUDE_DIRECTIVE`；专题`B`；固定time第14–14行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E032 [VERIFIED myloader/uefi/ElfLib/ElfLibInternal.h::ELF_NEXT_ENTRY]
```c
#define ELF_NEXT_ENTRY(EntryType, Current, EntrySize) \
              ((EntryType *) ((UINT8 *)Current + EntrySize))
```
类别：`MACRO_SINGLE`；专题`B`；固定time第20–21行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E033 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
typedef struct {
  RETURN_STATUS    ParseStatus;            ///< Return the status after ParseElfImage().
  UINT8            *FileBase;              ///< The source location in memory.
  UINTN            FileSize;               ///< The size including sections that don't require loading.
  UINT8            *PreferredImageAddress; ///< The preferred image to be loaded. No relocation is needed if loaded to this address.
```
类别：`ANONYMOUS_TYPEDEF_STRUCT`；专题`B`；固定time第19–23行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E034 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
  BOOLEAN          ReloadRequired;         ///< The image needs a new memory location for running.
  UINT8            *ImageAddress;          ///< The destination memory address set by caller.
  UINTN            ImageSize;              ///< The memory size for loading and execution.
  UINT32           EiClass;
  UINT32           ShNum;
```
类别：`ANONYMOUS_TYPEDEF_STRUCT`；专题`B`；固定time第24–28行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E035 [VERIFIED myloader/uefi/ElfLib.h::ELF_IMAGE_CONTEXT]
```c
  UINT32           PhNum;
  UINTN            ShStrOff;
  UINTN            ShStrLen;
  UINTN            EntryPoint;           ///< Return the actual entry point after LoadElfImage().
} ELF_IMAGE_CONTEXT;
```
类别：`ANONYMOUS_TYPEDEF_STRUCT`；专题`B`；固定time第29–33行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E036 [VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Half]
```c
typedef UINT16  Elf64_Half;
```
类别：`TYPEDEF`；专题`B`；固定time第44–44行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E037 [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint32_t]
```c
			typedef UINT32          multiboot_uint32_t;
```
类别：`TYPEDEF_IN_UEFI_CONDITIONAL`；专题`B`；固定time第99–99行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E038 [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint32_t]
```c
			typedef uint32_t        multiboot_uint32_t;
```
类别：`TYPEDEF_IN_NON_UEFI_CONDITIONAL`；专题`B`；固定time第104–104行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E039 [VERIFIED myloader/uefi/utils.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT	12
```
类别：`MACRO_SINGLE`；专题`B`；固定time第14–14行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E040 [VERIFIED myloader/uefi/utils.h::PAGE_SIZE]
```c
	#define PAGE_SIZE	(1 << PAGE_SHIFT)
```
类别：`MACRO_SINGLE`；专题`B`；固定time第15–15行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E041 [VERIFIED myloader/uefi/bootloader.h::load_elf_kernel]
```c
		EFI_STATUS load_elf_kernel(IN EFI_LOADED_IMAGE	*LoadedImage,
			CHAR16 *filename, PHYSICAL_ADDRESS *entry_addr);
```
类别：`FUNCTION_DECLARATION`；专题`B`；固定time第73–74行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E042 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	EFI_FILE_INFO* FileInfo;
	UINTN bufsize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
	gBS->AllocatePool(EfiRuntimeServicesData, bufsize, (VOID**)&FileInfo); 
	FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &bufsize, FileInfo);
```
类别：`C_FUNCTION`；专题`B`；固定time第24–27行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E043 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
  End                   = 0;
  Base                  = MAX_UINT32;
  ElfCt->ReloadRequired = FALSE;
  for (Index = 0; Index < ElfCt->PhNum; Index++) {
    Status = GetElfSegmentInfo (ElfCt->FileBase, ElfCt->EiClass, Index, &SegInfo);
```
类别：`C_FUNCTION`；专题`B`；固定time第297–301行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E044 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    ASSERT_EFI_ERROR (Status);
```
类别：`C_FUNCTION`；专题`B`；固定time第302–302行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E045 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    if (Base > (SegInfo.MemAddr & ~(EFI_PAGE_SIZE - 1))) {
      Base = SegInfo.MemAddr & ~(EFI_PAGE_SIZE - 1);
```
类别：`C_FUNCTION`；专题`B`；固定time第315–316行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E046 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    if (End < ALIGN_VALUE (SegInfo.MemAddr + SegInfo.MemLen, EFI_PAGE_SIZE) - 1) {
      End = ALIGN_VALUE (SegInfo.MemAddr + SegInfo.MemLen, EFI_PAGE_SIZE) - 1;
```
类别：`C_FUNCTION`；专题`B`；固定time第319–320行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E047 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```
类别：`CMAKE_UNIQUE_COMMAND_BLOCK`；专题`B`；固定time第39–41行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E048 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-ggdb \
	-mabi=sysv \
	-mno-red-zone \
```
类别：`CMAKE_UNIQUE_COMMAND_BLOCK`；专题`B`；固定time第48–50行。仅证明include、类型、宏或代码字面，外部实现/ABI未认证。

### UB99-E049 [VERIFIED make_install.sh::PROJ_ROOT]
```bash
PROJ_ROOT=$(pwd)
```
类别：`SHELL_TOP_LEVEL_ASSIGNMENT`；专题`C`；固定time第3–3行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E050 [VERIFIED make_install.sh::DEBUG_FLAG]
```bash
DEBUG_FLAG=-DCMAKE_BUILD_TYPE=Debug
```
类别：`SHELL_TOP_LEVEL_ASSIGNMENT`；专题`C`；固定time第4–4行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E051 [VERIFIED make_install.sh::TARGET_ARCH]
```bash
TARGET_ARCH=x86_64
```
类别：`SHELL_TOP_LEVEL_ASSIGNMENT`；专题`C`；固定time第5–5行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E052 [VERIFIED make_install.sh::LINK_TYPE]
```bash
LINK_TYPE=static
```
类别：`SHELL_TOP_LEVEL_ASSIGNMENT`；专题`C`；固定time第6–6行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E053 [VERIFIED make_install.sh::source(make_install_kernel.sh)]
```bash
source ./scripts/make_install_kernel.sh
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第11–11行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E054 [VERIFIED make_install.sh::source(make_install_initranfs.sh)]
```bash
source ./scripts/make_install_initranfs.sh
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第12–12行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E055 [VERIFIED make_install.sh::source(make_install_bootloader.sh)]
```bash
source ./scripts/make_install_bootloader.sh
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第13–13行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E056 [VERIFIED make_install.sh::source(phys_nvme_install.sh)]
```bash
source ./scripts/phys_nvme_install.sh
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第14–14行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E057 [VERIFIED make_install.sh::rm(build_star)]
```bash
rm -rf ./build/*
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第19–19行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E058 [VERIFIED make_install.sh::make_install_kernel]
```bash
make_install_kernel $PROJ_ROOT $DEBUG_FLAG $TARGET_ARCH
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第24–24行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E059 [VERIFIED make_install.sh::make_install_initranfs]
```bash
make_install_initranfs $PROJ_ROOT $DEBUG_FLAG $LINK_TYPE
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第25–25行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E060 [VERIFIED make_install.sh::make_install_bootloader]
```bash
make_install_bootloader $PROJ_ROOT $TARGET_ARCH
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第26–26行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E061 [VERIFIED scripts/make_install_bootloader.sh::make_install_bootloader]
```bash
function make_install_bootloader() {
	CMAKE_GEN_DIR="$1/build/bootloader"
	if [ ! -d $CMAKE_GEN_DIR ]; then
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第3–5行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E062 [VERIFIED scripts/make_install_bootloader.sh::make_install_bootloader]
```bash
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第6–8行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E063 [VERIFIED scripts/make_install_bootloader.sh::make_install_bootloader]
```bash
	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	cmake $1/myloader -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第10–11行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E064 [VERIFIED scripts/make_install_bootloader.sh::make_install_bootloader]
```bash
	sync
	sudo umount /mnt
}
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第12–14行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E065 [VERIFIED scripts/make_install_kernel.sh::make_install_kernel]
```bash
function make_install_kernel() {
	CMAKE_GEN_DIR="$1/build/mykernel"
	if [ ! -d $CMAKE_GEN_DIR ]; then
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第3–5行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E066 [VERIFIED scripts/make_install_kernel.sh::make_install_kernel]
```bash
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第6–8行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E067 [VERIFIED scripts/make_install_kernel.sh::make_install_kernel]
```bash
	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	rm /mnt/boot/kernel
	cmake $1/mykernel $2 -DARCH=$3 -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第10–12行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E068 [VERIFIED scripts/make_install_kernel.sh::make_install_kernel]
```bash
	sync
	sudo umount /mnt
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第13–14行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E069 [VERIFIED scripts/make_install_kernel.sh::make_install_kernel]
```bash
	objdump -S kernel > $CMAKE_GEN_DIR/../kern_dasm.txt

	cp kernel $CMAKE_GEN_DIR/../kernel
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第16–18行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E070 [VERIFIED scripts/make_install_initranfs.sh::make_install_initranfs]
```bash
function make_install_initranfs() {
	CMAKE_GEN_DIR="$1/build/myinitramfs"
	if [ ! -d $CMAKE_GEN_DIR ]; then
		mkdir -p $CMAKE_GEN_DIR
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第18–21行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E071 [VERIFIED scripts/make_install_initranfs.sh::make_install_initranfs]
```bash
	cd $CMAKE_GEN_DIR

	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第23–25行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E072 [VERIFIED scripts/make_install_initranfs.sh::make_install_initranfs]
```bash
	rm /mnt/boot/init
	rm /mnt/boot/sh
	cmake $1/myinitramfs $2 -DLINK_TYPE=$3 -DCMAKE_INSTALL_PREFIX=/mnt && make
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第27–29行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E073 [VERIFIED scripts/make_install_initranfs.sh::make_install_initranfs]
```bash
	make install

	sync

	sudo umount /mnt
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第37–41行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E074 [VERIFIED myloader/uefi/install_loader.sh::mount(/dev/dm-0)]
```bash
sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第3–3行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E075 [VERIFIED myloader/uefi/install_loader.sh::cp(Build-to-BOOTX64.EFI)]
```bash
cp Build/Shell/DEBUG_GCC5/X64/BootLoader/bootloader/DEBUG/bootloader.efi /mnt/EFI/BOOT/BOOTX64.EFI
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第4–4行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E076 [VERIFIED myloader/uefi/install_loader.sh::cp(Build-to-elfbinary_loader.efi)]
```bash
cp Build/Shell/DEBUG_GCC5/X64/BootLoader/bootloader/DEBUG/bootloader.efi ~/projs/MyOS2/elfbinary_loader.efi
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第5–5行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E077 [VERIFIED myloader/uefi/install_loader.sh::sync]
```bash
sync
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第6–6行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E078 [VERIFIED myloader/uefi/install_loader.sh::umount(/mnt)]
```bash
sudo umount /mnt
```
类别：`SHELL_TOP_LEVEL_CALL`；专题`C`；固定time第7–7行。仅证明shell文本与条件顺序，未运行或认证命令结果。

### UB99-E079 [VERIFIED myloader/CMakeLists.txt::cmake_minimum_required]
```cmake
cmake_minimum_required(VERSION 3.16)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`C`；固定time第1–1行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E080 [VERIFIED mykernel/scripts/target_kernel.cmake::install(TARGETS kernel)]
```cmake
install(TARGETS kernel DESTINATION boot)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`C`；固定time第32–32行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E081 [VERIFIED mykernel/CMakeLists.txt::add_executable(kernel)]
```cmake
add_executable(kernel)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`C`；固定time第13–13行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E082 [VERIFIED mykernel/CMakeLists.txt::include(target_kernel.cmake)]
```cmake
include(${SCRIPTS_PATH}/target_kernel.cmake)
```
类别：`CMAKE_UNIQUE_COMMAND`；专题`C`；固定time第35–35行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E083 [VERIFIED scripts/phys_nvme_install.sh::install_to_physdisk]
```bash
function install_to_physdisk() {
    sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
    sudo mount $1 /mnt1 -t vfat -o uid=$USER,gid=$USER
```
类别：`SHELL_FUNCTION`；专题`C`；固定time第4–6行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E084 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	bufsize = size;
	if (size == 0)
		bufsize = FileInfo->FileSize;
```
类别：`C_FUNCTION`；专题`C`；固定time第29–31行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E085 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(bufsize + PAGE_SIZE - 1) / PAGE_SIZE, &addr);

	FileHandle->Read(FileHandle, &bufsize, (VOID*)addr);
```
类别：`C_FUNCTION`；专题`C`；固定time第37–40行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E086 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	set_video_mode(ImageHandle, -1);
	load_elf_kernel(LoadedImage, L"\\boot\\kernel", &k_load_addr);
```
类别：`C_FUNCTION`；专题`C`；固定time第38–39行。只是源码或命令定义；不证明制品/装入成功。

### UB99-E087 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	// Check Format
	PHYSICAL_ADDRESS raw_img_addr = 0x2000000;
	LoadFile(LoadedImage, filename, raw_img_addr, 0);
	BOOLEAN isElf = IsElfFormat((UINT8 *)raw_img_addr);
	if (!isElf)
```
类别：`C_FUNCTION`；专题`C`；固定time第335–339行。只是源码或命令定义；不证明制品/装入成功。

## 源码证明边界

外部EDK II的真实平台DSC、解析过的INF和库实例、UINTN及EFIAPI实际ABI、工具链命令、EFI文件机器码与部署字节均不因上述声明自动成立。头文件单条include仅证明包含请求，不证明磁盘上的外部头文件版本。二进制的Git Blob元数据在facts单独记账，不是C/INF/CMake/Shell源码短锚。

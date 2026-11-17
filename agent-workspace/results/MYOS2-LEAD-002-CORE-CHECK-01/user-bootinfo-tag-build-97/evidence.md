---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOTINFO-TAG-BUILD-97
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
record_type: fixed_time_single_definition_source_quotes_and_R96_documentary_corrections
produced_by: GPT-6（可见家族，后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-17
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第97批固定源码短锚（含R96-01/02追加订正）

唯一源码 MyOS2 `time@a039d9803ade`。本执行者按固定GitHub原文抽取，每锚为单一实际宏、typedef、结构体或函数中连续1–5行。分类标注的是源码证据而非编译、ABI、外部EFI服务、真实地址范围或运行保证。独立typedef另设锚；历史96的110/110/0原样冻结。E036的跨定义错误与F05字段错置仅在本次追加纠正。

### BT97-E001 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common] | STRUCT | D00
```c
			multiboot_uint8_t framebuffer_type;
			multiboot_uint16_t reserved;
		};
```
固定time第320–322行；只支持所示定义内源码字面。

### BT97-E002 | [VERIFIED myloader/uefi/multiboot2.h::mbi_framebuffer_common_s] | TYPEDEF | D00
```c
		typedef struct multiboot_tag_framebuffer_common mbi_framebuffer_common_s;
```
固定time第323–323行；只支持所示定义内源码字面。

### BT97-E003 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common] | STRUCT_FIELD | D00
```c
			multiboot_uint8_t framebuffer_bpp;
```
固定time第316–316行；只支持所示定义内源码字面。

### BT97-E004 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info] | KERNEL_FUNCTION | D00
```c
	framebuffer.FB_size = common->framebuffer_width * common->framebuffer_height * common->framebuffer_bpp / 8;
	framebuffer.X_Resolution = common->framebuffer_width;
	framebuffer.Y_Resolution = common->framebuffer_height;
	framebuffer.PixperScanline = common->framebuffer_pitch / (common->framebuffer_bpp / 8);
```
固定time第34–37行；只支持所示定义内源码字面。

### BT97-E005 | [VERIFIED mykernel/include/obsolete/ktypes.h::framebuffer_s] | STRUCT | D00
```c
		uint32_t	PixperScanline;
		uint32_t	X_Resolution;
		uint32_t	Y_Resolution;
	} framebuffer_s;
```
固定time第28–31行；只支持所示定义内源码字面。

### BT97-E006 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info] | LOADER_FUNCTION | D00
```c
	common->framebuffer_bpp = Mode->FrameBufferSize * 8 /
			(common->framebuffer_width * common->framebuffer_height);
	common->framebuffer_pitch = Mode->Info->PixelsPerScanLine * common->framebuffer_bpp / 8;
	common->framebuffer_type = Mode->Mode;
```
固定time第152–155行；只支持所示定义内源码字面。

### BT97-E007 | [VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_FRAMEBUFFER_TYPE_RGB] | MACRO | D00
```c
		#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
```
固定time第318–318行；只支持所示定义内源码字面。

### BT97-E008 | [VERIFIED myloader/uefi/utils.h::PAGE_SHIFT] | MACRO | A
```c
	#define PAGE_SHIFT	12
```
固定time第14–14行；只支持所示定义内源码字面。

### BT97-E009 | [VERIFIED myloader/uefi/utils.h::PAGE_SIZE] | MACRO | A
```c
	#define PAGE_SIZE	(1 << PAGE_SHIFT)
```
固定time第15–15行；只支持所示定义内源码字面。

### BT97-E010 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | A
```c
	UINTN page_count = 32;
	EFI_PHYSICAL_ADDRESS pages = 0;
	gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, page_count, &pages);
	gBS->SetMem((void*)pages, page_count * PAGE_SIZE, 0);
	mbi_tags_header_ptr = (mbi_tags_header_s *)pages;
```
固定time第43–47行；只支持所示定义内源码字面。

### BT97-E011 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | A
```c
	mb2_infotag_ptr = init_info_tag_addr(pages);
```
固定time第48–48行；只支持所示定义内源码字面。

### BT97-E012 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | A
```c
	mb2_infotag_ptr = fill_framebuffer_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_mmap_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_acpi_RSDT_info(SystemTable, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_cputopo_info(mb2_infotag_ptr);
```
固定time第51–54行；只支持所示定义内源码字面。

### BT97-E013 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | A
```c
	mb2_infotag_ptr = fill_info_tag_end(mb2_infotag_ptr);
	fill_info_tag_header(mbi_tags_header_ptr, pages, mb2_infotag_ptr);
```
固定time第57–58行；只支持所示定义内源码字面。

### BT97-E014 | [VERIFIED myloader/uefi/multiboot2_utils.c::init_info_tag_addr] | LOADER_FUNCTION | A
```c
mbi_tag_s *init_info_tag_addr(PHYSICAL_ADDRESS start_addr)
{
	return (mbi_tag_s *) (start_addr + sizeof(mbi_tags_header_s));
}
```
固定time第57–60行；只支持所示定义内源码字面。

### BT97-E015 | [VERIFIED myloader/uefi/multiboot2_utils.c::next_info_tag_addr] | LOADER_FUNCTION | A
```c
mbi_tag_s *next_info_tag_addr(mbi_tag_s *prev_info_tag)
{
	return (mbi_tag_s *) ((UINT8 *) prev_info_tag +
				((prev_info_tag->size + 7) & ~7));
}
```
固定time第62–66行；只支持所示定义内源码字面。

### BT97-E016 | [VERIFIED myloader/uefi/multiboot2_utils.c::fill_info_tag_end] | LOADER_FUNCTION | A
```c
mbi_tag_s *fill_info_tag_end(mbi_tag_s *info_tag_ptr)
{
	info_tag_ptr->type = MULTIBOOT_TAG_TYPE_END;
	info_tag_ptr->size = sizeof(mbi_tag_s);
	return next_info_tag_addr(info_tag_ptr);
```
固定time第68–72行；只支持所示定义内源码字面。

### BT97-E017 | [VERIFIED myloader/uefi/multiboot2_utils.c::fill_info_tag_header] | LOADER_FUNCTION | A
```c
{
	mbi_tags_header_ptr->total_size = (PHYSICAL_ADDRESS)info_tag_end - start_addr;
	mbi_tags_header_ptr->reserved = 0;
}
```
固定time第77–80行；只支持所示定义内源码字面。

### BT97-E018 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tags_header] | STRUCT | A
```c
		struct multiboot_tags_header
		{
			multiboot_uint32_t total_size;
			multiboot_uint32_t reserved;
		};
```
固定time第219–223行；只支持所示定义内源码字面。

### BT97-E019 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag] | STRUCT | A
```c
		struct multiboot_tag
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
		};
```
固定time第226–230行；只支持所示定义内源码字面。

### BT97-E020 | [VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_ALIGN] | MACRO | A
```c
	#define MULTIBOOT_TAG_ALIGN						8
```
固定time第43–43行；只支持所示定义内源码字面。

### BT97-E021 | [VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_TYPE_END] | MACRO | A
```c
	#define MULTIBOOT_TAG_TYPE_END					0
```
固定time第44–44行；只支持所示定义内源码字面。

### BT97-E022 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info] | LOADER_FUNCTION | A
```c
	common->type = MULTIBOOT_TAG_TYPE_FRAMEBUFFER;
	common->size = sizeof(mbi_framebuffer_s);
	return next_info_tag_addr(mb2_infotag_ptr);
```
固定time第160–162行；只支持所示定义内源码字面。

### BT97-E023 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	UINT32 DesVersion = 0;
	UINTN MapKey = 0;
	UINTN MemMapSize = 0;
	UINTN DescriptorSize = 0;
	EFI_MEMORY_DESCRIPTOR* MemMap = 0;
```
固定time第169–173行；只支持所示定义内源码字面。

### BT97-E024 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	int i;
	int e820_nr = 0;
	unsigned long last_end = 0;
	mbi_mmap_s *memmap_info = (mbi_mmap_s *)mb2_infotag_ptr;
```
固定time第175–178行；只支持所示定义内源码字面。

### BT97-E025 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	mbi_mmap_ent_s *last_mb_mmap = NULL;
	mbi_mmap_ent_s *mb_mmap = memmap_info->entries;
```
固定time第179–180行；只支持所示定义内源码字面。

### BT97-E026 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DesVersion);
	MemMapSize += DescriptorSize * 5;
	// 申请缓冲区
	gBS->AllocatePool(EfiRuntimeServicesData, MemMapSize, (VOID **)&MemMap);
	gBS->SetMem((void*)MemMap, MemMapSize, 0);
```
固定time第183–187行；只支持所示定义内源码字面。

### BT97-E027 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	status = gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DesVersion);
	if(EFI_ERROR(status))
	{
		Print(L"GetMemoryMap FAIL. Status: %018lx\n",status);
		while (1);
```
固定time第189–193行；只支持所示定义内源码字面。

### BT97-E028 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	for(i = 0; i < MemMapSize / DescriptorSize; i++)
	{
		int MemType = 0;
		// EFI_MEMORY_DESCRIPTOR的大小不一定等于DescriptorSize
		EFI_MEMORY_DESCRIPTOR *MMap = (EFI_MEMORY_DESCRIPTOR*) ((CHAR8*)MemMap + i * DescriptorSize);
```
固定time第196–200行；只支持所示定义内源码字面。

### BT97-E029 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
		if(MMap->NumberOfPages == 0)
			continue;
		switch(MMap->Type)
```
固定time第201–203行；只支持所示定义内源码字面。

### BT97-E030 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			case EfiReservedMemoryType:
			case EfiMemoryMappedIO:
			case EfiMemoryMappedIOPortSpace:
			case EfiPalCode:
				MemType= 2;	//2:ROM or Reserved
```
固定time第205–209行；只支持所示定义内源码字面。

### BT97-E031 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			case EfiUnusableMemory:
				MemType= 5;	//5:Unusable
				break;
```
固定time第212–214行；只支持所示定义内源码字面。

### BT97-E032 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			case EfiACPIReclaimMemory:
				MemType= 3;	//3:ACPI Reclaim Memory
				break;
```
固定time第216–218行；只支持所示定义内源码字面。

### BT97-E033 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			case EfiRuntimeServicesData:
			case EfiConventionalMemory:
			case EfiPersistentMemory:
				MemType= 1;	//1:RAM
				break;
```
固定time第225–229行；只支持所示定义内源码字面。

### BT97-E034 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			case EfiACPIMemoryNVS:
				MemType= 4;	//4:ACPI NVS Memory
				break;
```
固定time第231–233行；只支持所示定义内源码字面。

### BT97-E035 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			default:
				Print(L"Invalid UEFI Memory Type:%4d\n", MMap->Type);
				continue;
		}
```
固定time第235–238行；只支持所示定义内源码字面。

### BT97-E036 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
		if((last_mb_mmap != NULL) &&
			(last_mb_mmap->type == MemType) && 
			(MMap->PhysicalStart == last_end))
		{
			last_mb_mmap->len += MMap->NumberOfPages << PAGE_SHIFT;
```
固定time第241–245行；只支持所示定义内源码字面。

### BT97-E037 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			last_mb_mmap->len += MMap->NumberOfPages << PAGE_SHIFT;
			last_end += MMap->NumberOfPages << PAGE_SHIFT;
```
固定time第245–246行；只支持所示定义内源码字面。

### BT97-E038 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
		else
		{
			mb_mmap->addr = MMap->PhysicalStart;
			mb_mmap->len = MMap->NumberOfPages << PAGE_SHIFT;
			mb_mmap->type = MemType;
```
固定time第248–252行；只支持所示定义内源码字面。

### BT97-E039 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
			last_mb_mmap = mb_mmap;
			mb_mmap++;

			last_end = MMap->PhysicalStart + (MMap->NumberOfPages << 12);
			e820_nr++;			
```
固定time第254–258行；只支持所示定义内源码字面。

### BT97-E040 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	last_mb_mmap = memmap_info->entries;
	int j = 0;
	for(i = 0; i< e820_nr; i++)
	{
		mbi_mmap_ent_s * e820i = last_mb_mmap + i;
```
固定time第263–267行；只支持所示定义内源码字面。

### BT97-E041 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
		for(j = i + 1; j < e820_nr; j++)
		{
			mbi_mmap_ent_s * e820j = last_mb_mmap + j;
			if(e820i->addr > e820j->addr)
			{
```
固定time第269–273行；只支持所示定义内源码字面。

### BT97-E042 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
				mmap_ent = *e820i;
				*e820i = *e820j;
				*e820j = mmap_ent;
			}
```
固定time第274–277行；只支持所示定义内源码字面。

### BT97-E043 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	gBS->FreePool(MemMap);
	gBS->ExitBootServices(ImageHandle, MapKey);

	memmap_info->type = MULTIBOOT_TAG_TYPE_MMAP;
	memmap_info->size = sizeof(mbi_mmap_s) + sizeof(mbi_mmap_ent_s) * (e820_nr + 1);
```
固定time第281–285行；只支持所示定义内源码字面。

### BT97-E044 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | B
```c
	memmap_info->type = MULTIBOOT_TAG_TYPE_MMAP;
	memmap_info->size = sizeof(mbi_mmap_s) + sizeof(mbi_mmap_ent_s) * (e820_nr + 1);
	memmap_info->entry_size = sizeof(mbi_mmap_ent_s);
	return next_info_tag_addr(mb2_infotag_ptr);
```
固定time第284–287行；只支持所示定义内源码字面。

### BT97-E045 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_mmap] | STRUCT | B
```c
		struct multiboot_tag_mmap
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t entry_size;
```
固定time第271–275行；只支持所示定义内源码字面。

### BT97-E046 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_mmap] | STRUCT | B
```c
			multiboot_uint32_t entry_version;
			mbi_mmap_ent_s entries[0];  
```
固定time第276–277行；只支持所示定义内源码字面。

### BT97-E047 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_mmap_entry] | STRUCT | B
```c
		struct multiboot_mmap_entry
		{
			multiboot_uint64_t addr;
			multiboot_uint64_t len;
```
固定time第205–208行；只支持所示定义内源码字面。

### BT97-E048 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_mmap_entry] | STRUCT | B
```c
			multiboot_uint32_t type;
			multiboot_uint32_t zero;
		};
```
固定time第214–216行；只支持所示定义内源码字面。

### BT97-E049 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag] | KERNEL_FUNCTION | B
```c
		case MULTIBOOT_TAG_TYPE_MMAP:
			mbi_mmap_s *mbi_mmap = (mbi_mmap_s *)tag;
			ram_map = (mbi_mmap_ent_s *)mbi_mmap->entries;
			break;
```
固定time第89–92行；只支持所示定义内源码字面。

### BT97-E050 | [VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup] | KERNEL_FUNCTION | B
```c
	for (mbi_mmap_ent_s *mmap_entp = ram_map;
		mmap_entp->len != 0 && mmap_entp->type != 0;
		i++, mmap_entp++)
	{
		e820_entry_s *e820_entp = &(e820_table->entries[i]);
```
固定time第103–107行；只支持所示定义内源码字面。

### BT97-E051 | [VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup] | KERNEL_FUNCTION | B
```c
		e820_entry_s *e820_entp = &(e820_table->entries[i]);
		e820_entp->addr = mmap_entp->addr;
		e820_entp->size = mmap_entp->len;
		e820_entp->type = mmap_entp->type;
```
固定time第107–110行；只支持所示定义内源码字面。

### BT97-E052 | [VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup] | KERNEL_FUNCTION | B
```c
	e820_table->nr_entries = i;
```
固定time第112–112行；只支持所示定义内源码字面。

### BT97-E053 | [VERIFIED mykernel/arch/x86_64/mm/early/e820_types.h::e820_table] | STRUCT | B
```c
	struct e820_table {
		__u32			nr_entries;
		e820_entry_s	entries[E820_MAX_ENTRIES];
	};
```
固定time第29–32行；只支持所示定义内源码字面。

### BT97-E054 | [VERIFIED mykernel/arch/x86_64/mm/early/e820_const.h::E820_MAX_ENTRIES] | MACRO | B
```c
	#define E820_MAX_ENTRIES	128
```
固定time第83–83行；只支持所示定义内源码字面。

### BT97-E055 | [VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_TYPE_MMAP] | MACRO | B
```c
	#define MULTIBOOT_TAG_TYPE_MMAP					6
```
固定time第50–50行；只支持所示定义内源码字面。

### BT97-E056 | [VERIFIED myloader/uefi/bootloader.c::fill_acpi_RSDT_info] | LOADER_FUNCTION | C
```c
mbi_tag_s *fill_acpi_RSDT_info(EFI_SYSTEM_TABLE *SystemTable, mbi_tag_s *mb2_infotag_ptr)
{
	mbi_acpi_new_s *rsdt_new = (mbi_acpi_new_s *)mb2_infotag_ptr;

	UINTN		i;
```
固定time第290–294行；只支持所示定义内源码字面。

### BT97-E057 | [VERIFIED myloader/uefi/bootloader.c::fill_acpi_RSDT_info] | LOADER_FUNCTION | C
```c
	configTab = gST->ConfigurationTable;
	for (i = 0; i < gST->NumberOfTableEntries; i++)
	{   
		//Step1. Find the table for ACPI
		if ((CompareGuid(&configTab->VendorGuid, &AcpiTableGuid) == 0) ||
```
固定time第303–307行；只支持所示定义内源码字面。

### BT97-E058 | [VERIFIED myloader/uefi/bootloader.c::fill_acpi_RSDT_info] | LOADER_FUNCTION | C
```c
		if ((CompareGuid(&configTab->VendorGuid, &AcpiTableGuid) == 0) ||
			(CompareGuid(&configTab->VendorGuid, &Acpi2TableGuid) == 0))
		{
			Root = configTab->VendorTable;
```
固定time第307–310行；只支持所示定义内源码字面。

### BT97-E059 | [VERIFIED myloader/uefi/bootloader.c::fill_acpi_RSDT_info] | LOADER_FUNCTION | C
```c
			if (Root->Signature == RSDP_SIGNATUR && Root->Revision >= EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION)
			{
				ZeroMem(&rsdt_new->rsdp, sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER));
				CopyMem(&rsdt_new->rsdp, Root, sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER));
				break;
```
固定time第314–318行；只支持所示定义内源码字面。

### BT97-E060 | [VERIFIED myloader/uefi/bootloader.c::fill_acpi_RSDT_info] | LOADER_FUNCTION | C
```c
	rsdt_new->type = MULTIBOOT_TAG_TYPE_ACPI_NEW;
	rsdt_new->size = sizeof(mbi_acpi_new_s) + sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER);
	return next_info_tag_addr(mb2_infotag_ptr);
}
```
固定time第326–329行；只支持所示定义内源码字面。

### BT97-E061 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_new_acpi] | STRUCT | C
```c
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint8_t rsdp[0];
		};
```
固定time第407–411行；只支持所示定义内源码字面。

### BT97-E062 | [VERIFIED myloader/uefi/multiboot2.h::mbi_acpi_new_s] | TYPEDEF | C
```c
		typedef struct multiboot_tag_new_acpi mbi_acpi_new_s;
```
固定time第412–412行；只支持所示定义内源码字面。

### BT97-E063 | [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info] | LOADER_FUNCTION | C
```c
	myos2_tag_smpinfo_s *smp_info = (myos2_tag_smpinfo_s *)mb2_infotag_ptr;

	UINTN i = 0;
	UINTN nCores = 0, nRunning = 0;
	EFI_MP_SERVICES_PROTOCOL *mpp = 0;
```
固定time第374–378行；只支持所示定义内源码字面。

### BT97-E064 | [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info] | LOADER_FUNCTION | C
```c
	gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mpp);
	mpp->GetNumberOfProcessors(mpp, &nCores, &nRunning);
	smp_info->core_num = nCores;
	smp_info->core_available = nRunning;
```
固定time第380–383行；只支持所示定义内源码字面。

### BT97-E065 | [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info] | LOADER_FUNCTION | C
```c
	for(i =0; i< nCores; i++){
		EFI_PROCESSOR_INFORMATION mcpuInfo;
		mpp -> GetProcessorInfo( mpp, i, &mcpuInfo);
		smp_info->cpus[i].proccessor_id	= mcpuInfo.ProcessorId;
		smp_info->cpus[i].status		= mcpuInfo.StatusFlag;
```
固定time第385–389行；只支持所示定义内源码字面。

### BT97-E066 | [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info] | LOADER_FUNCTION | C
```c
		smp_info->cpus[i].status		= mcpuInfo.StatusFlag;
		smp_info->cpus[i].pack_id		= mcpuInfo.Location.Package;
		smp_info->cpus[i].core_id		= mcpuInfo.Location.Core;
		smp_info->cpus[i].thd_id		= mcpuInfo.Location.Thread;
	}
```
固定time第389–393行；只支持所示定义内源码字面。

### BT97-E067 | [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info] | LOADER_FUNCTION | C
```c
	smp_info->type = MYOS2_TAG_TYPE_CPU_TOPO;
	smp_info->size = sizeof(myos2_tag_smpinfo_s) + nCores * sizeof(myos2_cpudesc_s);

```
固定time第395–397行；只支持所示定义内源码字面。

### BT97-E068 | [VERIFIED myloader/uefi/multiboot2.h::CPU_DESCRIPTOR] | STRUCT | C
```c
		struct CPU_DESCRIPTOR
		{
			multiboot_uint32_t	pack_id;
			multiboot_uint32_t	core_id;
			multiboot_uint32_t	thd_id;
```
固定time第457–461行；只支持所示定义内源码字面。

### BT97-E069 | [VERIFIED myloader/uefi/multiboot2.h::CPU_DESCRIPTOR] | STRUCT | C
```c
			multiboot_uint32_t	status;
			multiboot_uint64_t	proccessor_id;
		};
```
固定time第462–464行；只支持所示定义内源码字面。

### BT97-E070 | [VERIFIED myloader/uefi/multiboot2.h::myos2_tag_smpinfo] | STRUCT | C
```c
		struct myos2_tag_smpinfo
		{
			multiboot_uint32_t	type;
			multiboot_uint32_t	size;
			multiboot_uint32_t	core_num;
```
固定time第466–470行；只支持所示定义内源码字面。

### BT97-E071 | [VERIFIED myloader/uefi/multiboot2.h::myos2_tag_smpinfo] | STRUCT | C
```c
			multiboot_uint32_t	core_num;
			multiboot_uint32_t	core_available;
			myos2_cpudesc_s		cpus[0];
		};
```
固定time第470–473行；只支持所示定义内源码字面。

### BT97-E072 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_smpinfo] | KERNEL_FUNCTION | C
```c
static void get_smpinfo(myos2_tag_smpinfo_s *smp_info)
{
	nr_cpu_ids = smp_info->core_num;
}
```
固定time第24–27行；只支持所示定义内源码字面。

### BT97-E073 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag] | KERNEL_FUNCTION | C
```c
		case MYOS2_TAG_TYPE_CPU_TOPO:
			myos2_tag_smpinfo_s *smpinfo = (myos2_tag_smpinfo_s *)tag;
			get_smpinfo(smpinfo);
			break;
```
固定time第112–115行；只支持所示定义内源码字面。

### BT97-E074 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag] | KERNEL_FUNCTION | C
```c
		case MULTIBOOT_TAG_TYPE_ACPI_NEW:
			mbi_acpi_new_s *mbi_acpi_new = (mbi_acpi_new_s *)tag;
			break;
```
固定time第103–105行；只支持所示定义内源码字面。

### BT97-E075 | [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info] | LOADER_FUNCTION | C
```c
	gBS->FreePool(MemMap);
	gBS->ExitBootServices(ImageHandle, MapKey);
```
固定time第281–282行；只支持所示定义内源码字面。

### BT97-E076 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | C
```c
	Print(L"Allocate Pages At: 0x%llx.\n", pages);
	Print(L"Entry Point At: 0x%llx.\n", k_load_addr);
	Print(L"Call ExitBootServices.\n");
	Print(L"Jmp to Kernel.\n");
```
固定time第62–65行；只支持所示定义内源码字面。

### BT97-E077 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | C
```c
	gBS->CloseProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, ImageHandle, NULL);
```
固定time第67–68行；只支持所示定义内源码字面。

### BT97-E078 | [VERIFIED myloader/uefi/bootloader.c::UefiMain] | LOADER_FUNCTION | C
```c
	asm volatile(	"movq	%0,		%%rax		\n"
					"movq	%1,		%%rbx		\n"
					"callq	*%2					\n"
				:
				:	"i" (MULTIBOOT2_BOOTLOADER_MAGIC),
```
固定time第75–79行；只支持所示定义内源码字面。

### BT97-E079 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag] | KERNEL_FUNCTION | ABC
```c
	mbi_tag_header = (mbi_tags_header_s *) addr;
	size = mbi_tag_header->total_size;
	for (tag = (mbi_tag_s *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (mbi_tag_s *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
```
固定time第67–70行；只支持所示定义内源码字面。

### BT97-E080 | [VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_TYPE_ACPI_NEW] | MACRO | C
```c
	#define MULTIBOOT_TAG_TYPE_ACPI_NEW				15
```
固定time第59–59行；只支持所示定义内源码字面。

### BT97-E081 | [VERIFIED myloader/uefi/multiboot2.h::mbi_mmap_ent_s] | TYPEDEF | B
```c
		typedef struct multiboot_mmap_entry mbi_mmap_ent_s;
```
固定time第217–217行；只支持所示定义内源码字面。

### BT97-E082 | [VERIFIED myloader/uefi/multiboot2.h::mbi_tag_s] | TYPEDEF | A
```c
		typedef struct multiboot_tag mbi_tag_s;
```
固定time第231–231行；只支持所示定义内源码字面。

### BT97-E083 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint32_t] | TYPEDEF | ABC
```c
			typedef UINT32          multiboot_uint32_t;
```
固定time第99–99行；只支持所示定义内源码字面。

### BT97-E084 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint64_t] | TYPEDEF | ABC
```c
			typedef UINT64          multiboot_uint64_t;
```
固定time第100–100行；只支持所示定义内源码字面。

### BT97-E085 | [VERIFIED myloader/uefi/multiboot2.h::MYOS2_TAG_TYPE_CPU_TOPO] | MACRO | C
```c
	#define MYOS2_TAG_TYPE_CPU_TOPO					1000
```
固定time第67–67行；只支持所示定义内源码字面。

## 类别、消费者与历史链接门

本包新证据ID为 `BT97-E###`；旧的 `FM96-E###` 为第96批冻结证据，`CORE-USER-…::…` 是旧事项的qualified_id，不能互换。R96-01将旧FM96-E036拆为本E001结构体边界（至结束大括号）和E002独立typedef；嵌套结构及尾部空白不是自动错误。R96-02旧F05真实证据为旧FM96-E035/E041/E044，对照本E003/E004/E005；旧CN95-U05仅作为历史关系。所有短锚只支持相应语义，不认证EFI外部状态、指针、C ABI/sizeof、运行或完整机械P9。

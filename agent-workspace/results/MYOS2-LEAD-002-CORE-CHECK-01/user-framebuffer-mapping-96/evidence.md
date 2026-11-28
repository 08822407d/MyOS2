---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FRAMEBUFFER-MAPPING-96
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-13
record_type: single_definition_fixed_time_short_quote_evidence_with_D00
produced_by: GPT-6（可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-17
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_STATIC_EVIDENCE_PENDING_C06_PRO
runtime_evidence: NOT_RUN
---

# 第96批固定源码短锚（含D00补正）

**范围：** 唯一固定源为 MyOS2 `time@a039d9803ade`（loader亦取同一提交）。每段是所标实际定义内连续1–5物理行，不拼接邻接宏、宏前说明、extern声明或不同函数。每个锚只证明对应源码字面，不能替代全体函数的条件审查，也不代表运行/预处理/硬件事实。旧95的70条标签和历史70/70/0保持冻结，本96是另一个新证据账。

## D00及帧缓冲生产、物理入口短锚（FM96-E001–E050）

### FM96-E001 | [VERIFIED mykernel/printk/printk.c::register_console]
```c
	if (list_header_contains(&console_list_hdr, &newcon->node))
		return;
	else
		list_header_add_to_tail(&console_list_hdr, &newcon->node);
```
范围：固定time源第607–610行；专题D00。

### FM96-E002 | [VERIFIED mykernel/printk/printk.c::register_console]
```c
	// 	exclusive_console = newcon;
	// }
	console_unlock();
	// console_sysfs_notify();
```
范围：固定time源第718–721行；专题D00。

### FM96-E003 | [VERIFIED mykernel/printk/printk.c::register_console]
```c
	// if (newcon->flags & CON_PRINTBUFFER) {
	// 	/*
	// 	 * console_unlock(); will print out the buffered messages
	// 	 * for us.
	// 	 */
```
范围：固定time源第705–709行；专题D00。

### FM96-E004 | [VERIFIED mykernel/printk/printk.c::console_unlock]
```c
		// spin_lock_irqsave(&logbuf_lock, flags);
		wake_klogd |= log_start - log_end;
		if (con_start == log_end)
			break;			/* Nothing to print */
```
范围：固定time源第554–557行；专题D00。

### FM96-E005 | [VERIFIED mykernel/printk/printk.c::console_unlock]
```c
		_con_start = con_start;
		_log_end = log_end;
		con_start = log_end;		/* Flush */
```
范围：固定time源第558–560行；专题D00。

### FM96-E006 | [VERIFIED mykernel/printk/printk.c::console_unlock]
```c
		// spin_unlock(&logbuf_lock);
		// stop_critical_timings();	/* don't trace print latency */
		call_console_drivers(_con_start, _log_end);
```
范围：固定time源第561–563行；专题D00。

### FM96-E007 | [VERIFIED mykernel/printk/printk.c::__call_console_drivers]
```c
	for_each_console(con) {
		// if (exclusive_console && con != exclusive_console)
		// 	continue;
		// if ((con->flags & CON_ENABLED) && con->write &&
```
范围：固定time源第125–128行；专题D00。

### FM96-E008 | [VERIFIED mykernel/printk/printk.c::__call_console_drivers]
```c
		// 		(cpu_online(smp_processor_id()) ||
		// 		(con->flags & CON_ANYTIME)))
			con->write(con, &LOG_BUF(start), end - start);
	}
```
范围：固定time源第129–132行；专题D00。

### FM96-E009 | [VERIFIED mykernel/printk/printk.c::call_console_drivers]
```c
			cur_index++;
			if (c == '\n') {
				if (msg_level < 0) {
```
范围：固定time源第243–245行；专题D00。

### FM96-E010 | [VERIFIED mykernel/printk/printk.c::call_console_drivers]
```c
					msg_level = default_message_loglevel;
				}
				_call_console_drivers(start_print, cur_index, msg_level);
				msg_level = -1;
				start_print = cur_index;
```
范围：固定time源第252–256行；专题D00。

### FM96-E011 | [VERIFIED mykernel/printk/printk.c::console_init]
```c
	myos_init_video();
	INIT_LIST_S(&myos_tempcon.node);
	register_console(&myos_tempcon);
```
范围：固定time源第794–796行；专题D00。

### FM96-E012 | [VERIFIED mykernel/printk/printk.c::myos_tempcon]
```c
console_s myos_tempcon = {
	.name	= "Myos_Temp_Console",
	.write	= myos_tempcon_write,
	.read	= myos_tempcon_read,
};
```
范围：固定time源第835–839行；专题D00。

### FM96-E013 | [VERIFIED mykernel/include/linux/device/console.h::for_each_console]
```c
	#define for_each_console(con)						\
			list_header_for_each_container(						\
				con, (&console_list_hdr), node		\
			)
```
范围：固定time源第337–340行；专题D00。

### FM96-E014 | [VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container]
```c
	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)
```
范围：固定time源第346–347行；专题D00。

### FM96-E015 | [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
范围：固定time源第9–9行；专题D00。

### FM96-E016 | [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
范围：固定time源第10–10行；专题D00。

### FM96-E017 | [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_MASK]
```c
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```
范围：固定time源第11–11行；专题D00。

### FM96-E018 | [VERIFIED mykernel/klib/font.c::font_ascii]
```c
unsigned char font_ascii[256][16]=
{
	/*	0000	*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
```
范围：固定time源第3–6行；专题D00。

### FM96-E019 | [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	set_video_mode(ImageHandle, -1);
	load_elf_kernel(LoadedImage, L"\\boot\\kernel", &k_load_addr);
```
范围：固定time源第38–39行；专题A。

### FM96-E020 | [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, page_count, &pages);
	gBS->SetMem((void*)pages, page_count * PAGE_SIZE, 0);
	mbi_tags_header_ptr = (mbi_tags_header_s *)pages;
	mb2_infotag_ptr = init_info_tag_addr(pages);
```
范围：固定time源第45–48行；专题A。

### FM96-E021 | [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	mb2_infotag_ptr = fill_framebuffer_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_mmap_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_acpi_RSDT_info(SystemTable, mb2_infotag_ptr);
```
范围：固定time源第51–53行；专题A。

### FM96-E022 | [VERIFIED myloader/uefi/bootloader.c::set_video_mode]
```c
		long H_V_Resolution = Mode->Info->HorizontalResolution * Mode->Info->VerticalResolution;
		int MaxResolutionMode = gGraphicsOutput->Mode->Mode;
		// 选择最大分辨率
		for(i = 0; i < Mode->MaxMode; i++)
```
范围：固定time源第106–109行；专题A。

### FM96-E023 | [VERIFIED myloader/uefi/bootloader.c::set_video_mode]
```c
			gGraphicsOutput->QueryMode(gGraphicsOutput, i, &InfoSize, &Info);

			// Print(L"Mode: %02d, Version: %x, Format: %d, Horizontal: %d, Vertical: %d, ScanLine: %d\n", i, Info->Version, Info->PixelFormat, Info->HorizontalResolution, Info->VerticalResolution, Info->PixelsPerScanLine);

			if((Info->PixelFormat == 1) && (Info->HorizontalResolution * Info->VerticalResolution > H_V_Resolution))
```
范围：固定time源第111–115行；专题A。

### FM96-E024 | [VERIFIED myloader/uefi/bootloader.c::set_video_mode]
```c
		gGraphicsOutput->SetMode(gGraphicsOutput,MaxResolutionMode);
		gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);
		Mode = gGraphicsOutput->Mode;
```
范围：固定time源第123–125行；专题A。

### FM96-E025 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info]
```c
mbi_tag_s *fill_framebuffer_info(IN EFI_HANDLE ImageHandle, mbi_tag_s *mb2_infotag_ptr)
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphicsOutput = 0;
	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode = gGraphicsOutput->Mode;
```
范围：固定time源第139–143行；专题A。

### FM96-E026 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info]
```c
	mbi_framebuffer_s *framebuffer_info = (mbi_framebuffer_s *)mb2_infotag_ptr;
	mbi_framebuffer_common_s *common = &framebuffer_info->common;
```
范围：固定time源第146–147行；专题A。

### FM96-E027 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info]
```c
	common->framebuffer_addr = Mode->FrameBufferBase;
	common->framebuffer_width = Mode->Info->HorizontalResolution;
	common->framebuffer_height = Mode->Info->VerticalResolution;
```
范围：固定time源第149–151行；专题A。

### FM96-E028 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info]
```c
	common->framebuffer_bpp = Mode->FrameBufferSize * 8 /
			(common->framebuffer_width * common->framebuffer_height);
	common->framebuffer_pitch = Mode->Info->PixelsPerScanLine * common->framebuffer_bpp / 8;
	common->framebuffer_type = Mode->Mode;
```
范围：固定time源第152–155行；专题A。

### FM96-E029 | [VERIFIED myloader/uefi/bootloader.c::fill_framebuffer_info]
```c
	common->type = MULTIBOOT_TAG_TYPE_FRAMEBUFFER;
	common->size = sizeof(mbi_framebuffer_s);
	return next_info_tag_addr(mb2_infotag_ptr);
```
范围：固定time源第160–162行；专题A。

### FM96-E030 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint8_t]
```c
			typedef UINT8           multiboot_uint8_t;
```
范围：固定time源第97–97行；专题A。

### FM96-E031 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint32_t]
```c
			typedef UINT32          multiboot_uint32_t;
```
范围：固定time源第99–99行；专题A。

### FM96-E032 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_uint64_t]
```c
			typedef UINT64          multiboot_uint64_t;
```
范围：固定time源第100–100行；专题A。

### FM96-E033 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common]
```c
		struct multiboot_tag_framebuffer_common
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
```
范围：固定time源第306–309行；专题A。

### FM96-E034 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common]
```c
			multiboot_uint64_t framebuffer_addr;
			// here I save scanline in pitch field
			multiboot_uint32_t framebuffer_pitch;
			multiboot_uint32_t framebuffer_width;
			multiboot_uint32_t framebuffer_height;
```
范围：固定time源第311–315行；专题A。

### FM96-E035 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common]
```c
			multiboot_uint8_t framebuffer_bpp;
```
范围：固定time源第316–316行；专题A。

### FM96-E036 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common]
```c
			multiboot_uint8_t framebuffer_type;
			multiboot_uint16_t reserved;
		};
		typedef struct multiboot_tag_framebuffer_common mbi_framebuffer_common_s;
```
范围：固定time源第320–323行；专题A。

### FM96-E037 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer]
```c
		struct multiboot_tag_framebuffer
		{
			mbi_framebuffer_common_s common;
```
范围：固定time源第325–327行；专题A。

### FM96-E038 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer]
```c
					multiboot_uint8_t framebuffer_red_field_position;
					multiboot_uint8_t framebuffer_red_mask_size;
					multiboot_uint8_t framebuffer_green_field_position;
					multiboot_uint8_t framebuffer_green_mask_size;
```
范围：固定time源第338–341行；专题A。

### FM96-E039 | [VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer]
```c
					multiboot_uint8_t framebuffer_blue_field_position;
					multiboot_uint8_t framebuffer_blue_mask_size;
				};
			};
		};
```
范围：固定time源第342–346行；专题A。

### FM96-E040 | [VERIFIED mykernel/include/obsolete/ktypes.h::framebuffer_s]
```c
	typedef struct framebuffer
	{
		phys_addr_t	FB_phybase;
		virt_addr_t	FB_virbase;
		uint64_t	FB_size;
```
范围：固定time源第22–26行；专题A。

### FM96-E041 | [VERIFIED mykernel/include/obsolete/ktypes.h::framebuffer_s]
```c
		uint32_t	PixperScanline;
		uint32_t	X_Resolution;
		uint32_t	Y_Resolution;
	} framebuffer_s;
```
范围：固定time源第28–31行；专题A。

### FM96-E042 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::framebuffer]
```c
framebuffer_s		framebuffer;
```
范围：固定time源第18–18行；专题A/B。

### FM96-E043 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info]
```c
static void get_framebuffer_info(mbi_framebuffer_s *framebuffer_info)
{
	mbi_framebuffer_common_s *common = &framebuffer_info->common;
	framebuffer.FB_phybase = common->framebuffer_addr;
	framebuffer.FB_virbase = (virt_addr_t)phys_to_virt(framebuffer.FB_phybase);
```
范围：固定time源第29–33行；专题A/B。

### FM96-E044 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info]
```c
	framebuffer.FB_size = common->framebuffer_width * common->framebuffer_height * common->framebuffer_bpp / 8;
	framebuffer.X_Resolution = common->framebuffer_width;
	framebuffer.Y_Resolution = common->framebuffer_height;
	framebuffer.PixperScanline = common->framebuffer_pitch / (common->framebuffer_bpp / 8);
```
范围：固定time源第34–37行；专题A/B。

### FM96-E045 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::myos_early_init_system]
```c
	parse_tag(mbi_magic, (ulong)&multiboot_MBI);
```
范围：固定time源第47–47行；专题A/B。

### FM96-E046 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
		case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			mbi_framebuffer_s *mbi_fb = (mbi_framebuffer_s *)tag;
			get_framebuffer_info(mbi_fb);
			break;
```
范围：固定time源第96–99行；专题A/B。

### FM96-E047 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
```c
	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;
```
范围：固定time源第195–198行；专题A/B。

### FM96-E048 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
```c
	char linebuf[4096] = {0};
	int i;
	for (i = 0; i < Pos.XResolution / Pos.XCharSize ; i++)
		linebuf[i] = ' ';
	color_printk(BLACK, GREEN, "%s", linebuf);
```
范围：固定time源第208–212行；专题A/B。

### FM96-E049 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_VBE_mapping]
```c
void myos_init_VBE_mapping()
{
	u64 start, end;
	start = PFN_PHYS(PFN_DOWN(framebuffer.FB_phybase));
	myos_ioremap(start, PFN_PHYS(PFN_UP(framebuffer.FB_size)));
```
范围：固定time源第221–225行；专题A/B。

### FM96-E050 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_VBE_mapping]
```c
	myos_ioremap(start, PFN_PHYS(PFN_UP(framebuffer.FB_size)));
	flush_tlb_local();
}
```
范围：固定time源第225–227行；专题A/B。

## 启动映射、属性、接口返回及TLB定义短锚（FM96-E051–E110）

### FM96-E051 | [VERIFIED mykernel/arch/x86_64/mm/init.c::myos_memory_map]
```c
myos_memory_map() {
extern void myos_init_VBE_mapping(void);
	myos_init_VBE_mapping();
```
范围：固定time源第250–252行；专题B/C。

### FM96-E052 | [VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
void __init init_mem_mapping(void)
{
	ulong end;

	// pti_check_boottime_disable();
```
范围：固定time源第487–491行；专题B/C。

### FM96-E053 | [VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
		memory_map_bottom_up(kernel_end, end);
		memory_map_bottom_up(ISA_END_ADDRESS, kernel_end);
	} else {
		// memory_map_top_down(ISA_END_ADDRESS, end);
```
范围：固定time源第517–520行；专题B/C。

### FM96-E054 | [VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	myos_memory_map();
```
范围：固定time源第522–522行；专题B/C。

### FM96-E055 | [VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```
范围：固定time源第529–530行；专题B/C。

### FM96-E056 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
phys_pte_init(pte_t *pte_page, ulong paddr,
		ulong paddr_end, pgprot_t prot, bool init) {

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;
```
范围：固定time源第623–627行；专题B/C。

### FM96-E057 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
	pte = pte_page + pte_index(paddr);
	i = pte_index(paddr);

	for (; i < PTRS_PER_PTE; i++, paddr = paddr_next, pte++) {
		paddr_next = (paddr & PAGE_MASK) + PAGE_SIZE;
```
范围：固定time源第631–635行；专题B/C。

### FM96-E058 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		if (!pte_none(*pte)) {
			if (!after_bootmem)
				pages++;
			continue;
		}
```
范围：固定time源第645–649行；专题B/C。

### FM96-E059 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		pages++;
		set_pte_init(pte, pfn_pte(paddr >> PAGE_SHIFT, prot), init);
		paddr_last = (paddr & PAGE_MASK) + PAGE_SIZE;
	}
	update_page_count(PG_LEVEL_4K, pages);
```
范围：固定time源第654–658行；专题B/C。

### FM96-E060 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		if (!pmd_none(*pmd)) {
			if (!pmd_leaf(*pmd)) {
				spin_lock(&init_mm.page_table_lock);
				pte = (pte_t *)pmd_page_vaddr(*pmd);
				paddr_last = phys_pte_init(pte, paddr,
```
范围：固定time源第685–689行；专题B/C。

### FM96-E061 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
			if (page_size_mask & (1 << PG_LEVEL_2M)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
				continue;
```
范围：固定time源第707–711行；专题B/C。

### FM96-E062 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
			new_prot = pte_pgprot(pte_clrhuge(*(pte_t *)pmd));
		}

		if (page_size_mask & (1<<PG_LEVEL_2M)) {
			pages++;
```
范围：固定time源第713–717行；专题B/C。

### FM96-E063 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		pte = alloc_low_page();
		paddr_last = phys_pte_init(pte, paddr, paddr_end, new_prot, init);

		spin_lock(&init_mm.page_table_lock);
		pmd_populate_kernel_init(&init_mm, pmd, pte, init);
```
范围：固定time源第727–731行；专题B/C。

### FM96-E064 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
			if (!pud_leaf(*pud)) {
				pmd = pmd_offset(pud, 0);
				paddr_last =
					phys_pmd_init(pmd, paddr, paddr_end,
						page_size_mask, prot, init);
```
范围：固定time源第767–771行；专题B/C。

### FM96-E065 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
			if (page_size_mask & (1 << PG_LEVEL_1G)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
				continue;
```
范围：固定time源第786–790行；专题B/C。

### FM96-E066 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		if (page_size_mask & (1<<PG_LEVEL_1G)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
			set_pud_init(pud,
				pfn_pud(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
```
范围：固定time源第795–799行；专题B/C。

### FM96-E067 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		pmd = alloc_low_page();
		paddr_last = phys_pmd_init(pmd, paddr, paddr_end,
					   page_size_mask, prot, init);

		spin_lock(&init_mm.page_table_lock);
```
范围：固定time源第806–810行；专题B/C。

### FM96-E068 | [VERIFIED mykernel/arch/x86_64/mm/init.c::phys_p4d_init]
```c
#define phys_p4d_init(p4d_page, paddr, paddr_end,					\
				page_size_mask, prot, init)							\
			phys_pud_init((pud_t *)(p4d_page), (paddr),				\
					(paddr_end), (page_size_mask), (prot), (init))
```
范围：固定time源第818–821行；专题B/C。

### FM96-E069 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
static ulong __meminit
__kernel_physical_mapping_init(ulong paddr_start,
		ulong paddr_end, ulong page_size_mask,
		pgprot_t prot, bool init) {
```
范围：固定time源第823–826行；专题B/C。

### FM96-E070 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	paddr_last = paddr_end;
	vaddr = (ulong)__va(paddr_start);
	vaddr_end = (ulong)__va(paddr_end);
	vaddr_start = vaddr;
```
范围：固定time源第831–834行；专题B/C。

### FM96-E071 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	for (; vaddr < vaddr_end; vaddr = vaddr_next) {
		pgd_t *pgd = pgd_offset_k(vaddr);
		p4d_t *p4d;

		vaddr_next = (vaddr & PGDIR_MASK) + PGDIR_SIZE;
```
范围：固定time源第836–840行；专题B/C。

### FM96-E072 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		if (pgd_val(*pgd)) {
			p4d = (p4d_t *)pgd_page_vaddr(*pgd);
			paddr_last = phys_p4d_init(p4d,
					__pa(vaddr), __pa(vaddr_end),
					page_size_mask, prot, init);
```
范围：固定time源第842–846行；专题B/C。

### FM96-E073 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		p4d = alloc_low_page();
		paddr_last = phys_p4d_init(p4d, __pa(vaddr),
			__pa(vaddr_end), page_size_mask, prot, init);

```
范围：固定time源第850–853行；专题B/C。

### FM96-E074 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		spin_lock(&init_mm.page_table_lock);
		p4d_populate_init(&init_mm, p4d_offset(pgd, vaddr),
				(pud_t *) p4d, init);
		spin_unlock(&init_mm.page_table_lock);
```
范围：固定time源第854–857行；专题B/C。

### FM96-E075 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	// if (pgd_changed)
	// 	sync_global_pgds(vaddr_start, vaddr_end - 1);
	return paddr_last;
```
范围：固定time源第861–863行；专题B/C。

### FM96-E076 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__supported_pte_mask]
```c
pteval_t __supported_pte_mask __read_mostly = ~0;
```
范围：固定time源第873–873行；专题B/C。

### FM96-E077 | [VERIFIED mykernel/arch/x86_64/mm/init.c::__default_kernel_pte_mask]
```c
pteval_t __default_kernel_pte_mask __read_mostly = ~0;
```
范围：固定time源第875–875行；专题B/C。

### FM96-E078 | [VERIFIED mykernel/arch/x86_64/mm/init.c::myos_ioremap]
```c
void __iomem
*myos_ioremap(size_t paddr_start, ulong size)
{
	// return __myos_kernel_physical_mapping_init(paddr_start,
	// 			PFN_PHYS(PFN_UP(paddr_start + size)), __PAGE_KERNEL_NOCACHE);
```
范围：固定time源第877–881行；专题B/C。

### FM96-E079 | [VERIFIED mykernel/arch/x86_64/mm/init.c::myos_ioremap]
```c
	return (void *)__kernel_physical_mapping_init(paddr_start,
			PFN_PHYS(PFN_UP(paddr_start + size)),
				1 << PG_LEVEL_4K, PAGE_KERNEL_NOENC, true);
}
```
范围：固定time源第882–885行；专题B/C。

### FM96-E080 | [VERIFIED mykernel/arch/x86_64/mm/init.c::iounmap]
```c
void iounmap(volatile void __iomem *addr)
{

}
```
范围：固定time源第886–889行；专题B/C。

### FM96-E081 | [VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	init_mem_mapping();
```
范围：固定time源第249–249行；专题B/C。

### FM96-E082 | [VERIFIED mykernel/init/main.c::start_kernel]
```c
	setup_arch(NULL);
```
范围：固定time源第138–138行；专题B/C。

### FM96-E083 | [VERIFIED mykernel/init/main.c::start_kernel]
```c
	console_init();
```
范围：固定time源第174–174行；专题B/C。

### FM96-E084 | [VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_UP]
```c
	#define PFN_UP(x)			(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
```
范围：固定time源第7–7行；专题B/C。

### FM96-E085 | [VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_DOWN]
```c
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
```
范围：固定time源第8–8行；专题B/C。

### FM96-E086 | [VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_PHYS]
```c
	#define PFN_PHYS(x)			((phys_addr_t)(x) << PAGE_SHIFT)
```
范围：固定time源第9–9行；专题B/C。

### FM96-E087 | [VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
范围：固定time源第25–25行；专题B/C。

### FM96-E088 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::pg_level]
```c
			PG_LEVEL_NONE,
			PG_LEVEL_4K,
			PG_LEVEL_2M,
			PG_LEVEL_1G,
			PG_LEVEL_512G,
```
范围：固定time源第8–12行；专题B/C。

### FM96-E089 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_PWT]
```c
	#define _PAGE_PWT					(_AT(pteval_t, 1) << _PAGE_BIT_PWT)
```
范围：固定time源第53–53行；专题B/C。

### FM96-E090 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_PCD]
```c
	#define _PAGE_PCD					(_AT(pteval_t, 1) << _PAGE_BIT_PCD)
```
范围：固定time源第54–54行；专题B/C。

### FM96-E091 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_ENC]
```c
	#define _PAGE_ENC					(_AT(pteval_t, 0))
```
范围：固定time源第157–157行；专题B/C。

### FM96-E092 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_NOCACHE]
```c
	#define _PAGE_NOCACHE				(_PAGE_PWT | _PAGE_PCD)
```
范围：固定time源第161–161行；专题B/C。

### FM96-E093 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::__PAGE_KERNEL]
```c
	#define __PAGE_KERNEL				(__PP|__RW|   0|___A|__NX|___D|   0|___G)
```
范围：固定time源第189–189行；专题B/C。

### FM96-E094 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::__PAGE_KERNEL_NOCACHE]
```c
	#define __PAGE_KERNEL_NOCACHE		(__PP|__RW|   0|___A|	0|___D|   0|___G| __NC)
```
范围：固定time源第198–198行；专题B/C。

### FM96-E095 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::__pgprot_mask]
```c
	#  define __pgprot_mask(x)			__pgprot((x) & __default_kernel_pte_mask)
```
范围：固定time源第212–212行；专题B/C。

### FM96-E096 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_KERNEL_NOENC]
```c
	#  define PAGE_KERNEL_NOENC		    __pgprot_mask(__PAGE_KERNEL            |    0)
```
范围：固定time源第215–215行；专题B/C。

### FM96-E097 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_KERNEL_NOCACHE]
```c
	#  define PAGE_KERNEL_NOCACHE		__pgprot_mask(__PAGE_KERNEL_NOCACHE    | _ENC)
```
范围：固定time源第220–220行；专题B/C。

### FM96-E098 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pte]
```c
		pfn_pte(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PTE_PFN_MASK;
			return __pte(pfn | check_pgprot(pgprot));
```
范围：固定time源第650–654行；专题B/C。

### FM96-E099 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::massage_pgprot]
```c
		massage_pgprot(pgprot_t pgprot) {
			pgprotval_t protval = pgprot_val(pgprot);

			if (protval & _PAGE_PRESENT)
				protval &= __supported_pte_mask;
```
范围：固定time源第686–690行；专题B/C。

### FM96-E100 | [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_init]
```c
		set_pte_init(pte_t *pte_ptr, pte_t pte_ent, bool init) {
			if (init)
				set_pte_safe(pte_ptr, pte_ent);
			else
				set_pte(pte_ptr, pte_ent);
```
范围：固定time源第739–743行；专题B/C。

### FM96-E101 | [VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::flush_tlb_local]
```c
void flush_tlb_local(void)
{
	// /*
```
范围：固定time源第233–235行；专题B/C。

### FM96-E102 | [VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::flush_tlb_local]
```c
	// invalidate_user_asid(this_cpu_read(cpu_tlbstate.loaded_mm_asid));

	/* If current->mm == NULL then the read_cr3() "borrows" an mm */
	write_cr3(__read_cr3());
}
```
范围：固定time源第242–246行；专题B/C。

### FM96-E103 | [VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::__flush_tlb_all]
```c
	// 	 * !PGE -> !PCID (setup_pcid()), thus every flush is total.
	// 	 */
		flush_tlb_local();
	// }
```
范围：固定time源第263–266行；专题B/C。

### FM96-E104 | [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::phys_to_virt]
```c
		void
		*phys_to_virt(volatile phys_addr_t address) {
			return __va(address);
		}
```
范围：固定time源第57–60行；专题B/C。

### FM96-E105 | [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_set_mapping]
```c

static inline void hpet_set_mapping(void) {
	hpet_address = 0xfed00000;
	hpet_virt_address = myos_ioremap(hpet_address, HPET_MMAP_SIZE);
	flush_tlb_local();
```
范围：固定time源第78–82行；专题B/C。

### FM96-E106 | [VERIFIED mykernel/arch/x86_64/insns/special_insns_arch.h::write_cr3]
```c
		write_cr3(ulong val) {
			asm volatile(	"mov	%0,		%%cr3	\n\t"
						:
						:	"r"(val)
						:	"memory"
```
范围：固定time源第125–129行；专题B/C。

### FM96-E107 | [VERIFIED mykernel/arch/x86_64/insns/special_insns_arch.h::load_cr3]
```c
		load_cr3(pgd_t *pgdir) {
			write_cr3(__pa(pgdir));
		}
```
范围：固定time源第145–147行；专题B/C。

### FM96-E108 | [VERIFIED mykernel/mm/fault/pgtable.h::p4d_populate_init]
```c
		p4d_populate_init(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p, bool init) {
			if (init)
				p4d_populate_safe(mm, p4d_entp, pud_p);
			else
				p4d_populate(mm, p4d_entp, pud_p);
```
范围：固定time源第493–497行；专题B/C。

### FM96-E109 | [VERIFIED mykernel/mm/fault/pgtable.h::pud_populate_init]
```c
		pud_populate_init(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p, bool init) {
			if (init)
				pud_populate_safe(mm, pud_entp, pmd_p);
			else
				pud_populate(mm, pud_entp, pmd_p);
```
范围：固定time源第501–505行；专题B/C。

### FM96-E110 | [VERIFIED mykernel/mm/fault/pgtable.h::pmd_populate_kernel_init]
```c
		pmd_populate_kernel_init(mm_s *mm, pmd_t *pmd_entp, pte_t *pte_p, bool init) {
			if (init)
				pmd_populate_kernel_safe(mm, pmd_entp, pte_p);
			else
				pmd_populate_kernel(mm, pmd_entp, pte_p);
```
范围：固定time源第509–513行；专题B/C。

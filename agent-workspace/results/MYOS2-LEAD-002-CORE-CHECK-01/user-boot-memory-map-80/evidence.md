---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-MEMORY-MAP-80
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
record_type: fixed_time_anchored_static_evidence_batch80
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第80批证据｜D00追加、A/B/C三专题

本件的每一条编号短锚均从固定 `time@a039d9803ade` 的**同一符号定义内部**连续复制1–5物理行，行号仅供导航，引用原文不重排。未引用宏外条件指令作为宏定义。本件92条为**第80批新增标签**，原79六件95个历史标签不并入；D00的7条属于文档纠正取证，A/B/C属于启动内存图取证。本件及其他五件均没有内核运行结论。编译、预处理、Graphviz、QEMU、真实启动、完整本地P9：NOT_RUN。

## D01｜D00纠正｜纠正79/A06：只取宏定义；前置#ifndef仍是宏外条件门。

[VERIFIED mykernel/mm/early/memblock_const.h::INIT_MMBLK_RESERVED_REGIONS]
```c
	#  define INIT_MMBLK_RESERVED_REGIONS	(2 * INIT_MEMBLOCK_REGIONS)
```
固定time物理行47；单一符号定义边界47–47；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D02｜D00纠正｜纠正79/A07：只取宏定义，不再夹入#ifndef。

[VERIFIED mykernel/mm/early/memblock_const.h::INIT_MMBLK_MEMORY_REGIONS]
```c
	#  define INIT_MMBLK_MEMORY_REGIONS	INIT_MEMBLOCK_REGIONS
```
固定time物理行51；单一符号定义边界51–51；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D03｜D00纠正｜外层start首先受PAGE_SIZE下界约束；112只能作直接finder隔离例。

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);
```
固定time物理行478–480；单一符号定义边界465–488；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D04｜D00纠正｜首次0且min_addr非零时可能重试，不能首次失败直接推出NULL。

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);
```
固定time物理行545–548；单一符号定义边界533–554；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D05｜D00纠正｜请求MEMBLOCK_MIRROR才排除非镜像区域；flags0不因MIRROR本身排除。

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	/* if we want mirror memory skip non-mirror memory regions */
	if ((flags & MEMBLOCK_MIRROR) && !memblock_is_mirror(m))
		return true;
```
固定time物理行302–304；单一符号定义边界290–315；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D06｜D00纠正｜终止赋值ULLONG_MAX，为原79/C25不足的真实尾锚。

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
	/* signal end of iteration */
	*idx = ULLONG_MAX;
```
固定time物理行399–400；单一符号定义边界343–401；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## D07｜D00纠正｜size0立即返回ENOERR；外层size0候选不会形成新增保留。

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;
```
固定time物理行209–212；单一符号定义边界204–262；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A01｜G01/A｜入口将mbi_magic和本核multiboot_MBI地址交给parse_tag。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::myos_early_init_system]
```c
	parse_tag(mbi_magic, (ulong)&multiboot_MBI);
```
固定time物理行47；单一符号定义边界40–50；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A02｜G01/A｜文件作用域ram_map指针定义；本函数未每次重置。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::ram_map]
```c
mbi_mmap_ent_s		*ram_map;
```
固定time物理行17；单一符号定义边界17–17；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A03｜G01/A｜文件作用域标签头指针定义。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::mbi_tag_header]
```c
mbi_tags_header_s	*mbi_tag_header;
```
固定time物理行19；单一符号定义边界19–19；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A04｜G01/A｜magic不符提前return。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		return;
	}
```
固定time物理行59–61；单一符号定义边界52–118；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A05｜G01/A｜addr八字节对齐门。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
	if (addr & 7) {
		return;
	}
```
固定time物理行63–65；单一符号定义边界52–118；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A06｜G01/A｜total_size读到局部size，但循环按END标签和tag.size步长推进。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
	mbi_tag_header = (mbi_tags_header_s *) addr;
	size = mbi_tag_header->total_size;
	for (tag = (mbi_tag_s *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (mbi_tag_s *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
```
固定time物理行67–70；单一符号定义边界52–118；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A07｜G01/A｜MMAP case仅发布entries指针，entry_size/version未参加该消费。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
		case MULTIBOOT_TAG_TYPE_MMAP:
			mbi_mmap_s *mbi_mmap = (mbi_mmap_s *)tag;
			ram_map = (mbi_mmap_ent_s *)mbi_mmap->entries;
			break;
```
固定time物理行89–92；单一符号定义边界52–118；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A08｜G01/A｜复制目标固定32*PAGE_SIZE字节.data数组。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::multiboot_MBI]
```c
char multiboot_MBI[32 * PAGE_SIZE] __section(".data") __aligned(PAGE_SIZE);
```
固定time物理行46；单一符号定义边界46–46；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A09｜G01/A｜取得源mbi_base_p、目标mbi_dst及bp_dst。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]
```c
	u64 *mbi_base_p = rip_rel_ptr(&mbi_base);
	void *mbi_dst = rip_rel_ptr(&multiboot_MBI);
	void *bp_dst = rip_rel_ptr(&boot_params);
```
固定time物理行127–129；单一符号定义边界53–144；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A10｜G01/A｜满足magic时执行固定sizeof(multiboot_MBI)拷贝。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]
```c
	if (*rip_rel_ptr(&mbi_magic) == MULTIBOOT2_BOOTLOADER_MAGIC) {
		memcpy(mbi_dst, (const void *)(*mbi_base_p), sizeof(multiboot_MBI));
	} else {
		memcpy(bp_dst, (const void *)bp, sizeof(boot_params));
```
固定time物理行131–134；单一符号定义边界53–144；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A11｜G01/A｜所读内核C路径调用myos_early_init_system；不外推先行汇编。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
	extern void myos_early_init_system(void);

	myos_early_init_system();
```
固定time物理行169–171；单一符号定义边界155–209；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A12｜G01/A｜本树boot magic定义。

[VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT2_BOOTLOADER_MAGIC]
```c
	#define MULTIBOOT2_BOOTLOADER_MAGIC             0x36d76289
```
固定time物理行33；单一符号定义边界33–33；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A13｜G01/A｜END类型为0。

[VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_TYPE_END]
```c
	#define MULTIBOOT_TAG_TYPE_END					0
```
固定time物理行44；单一符号定义边界44–44；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A14｜G01/A｜MMAP类型为6。

[VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_TYPE_MMAP]
```c
	#define MULTIBOOT_TAG_TYPE_MMAP					6
```
固定time物理行50；单一符号定义边界50–50；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A15｜G01/A｜声明对齐值8不构成长度界限校验。

[VERIFIED myloader/uefi/multiboot2.h::MULTIBOOT_TAG_ALIGN]
```c
	#define MULTIBOOT_TAG_ALIGN						8
```
固定time物理行43；单一符号定义边界43–43；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A16｜G01/A｜total_size/reserved为uint32；消费者仅读取前者。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_tags_header]
```c
		struct multiboot_tags_header
		{
			multiboot_uint32_t total_size;
			multiboot_uint32_t reserved;
		};
```
固定time物理行219–223；单一符号定义边界219–223；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A17｜G01/A｜type与size为uint32字段。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_tag]
```c
		struct multiboot_tag
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
		};
```
固定time物理行226–230；单一符号定义边界226–230；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A18｜G01/A｜entry_size/entry_version/entries[0]布局。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_mmap]
```c
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t entry_size;
			multiboot_uint32_t entry_version;
			mbi_mmap_ent_s entries[0];  
```
固定time物理行273–277；单一符号定义边界271–278；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A19｜G01/A｜地址和长度字段是uint64。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_mmap_entry]
```c
		struct multiboot_mmap_entry
		{
			multiboot_uint64_t addr;
			multiboot_uint64_t len;
```
固定time物理行205–208；单一符号定义边界205–216；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A20｜G01/A｜type/zero字段为uint32；步进是C结构对象步长。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_mmap_entry]
```c
		#define MULTIBOOT_MEMORY_NVS                    4
		#define MULTIBOOT_MEMORY_BADRAM                 5
			multiboot_uint32_t type;
			multiboot_uint32_t zero;
```
固定time物理行212–215；单一符号定义边界205–216；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A21｜G01/A｜静态目标表对象。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820_table_init]
```c
static e820_table_s e820_table_init		__initdata;
```
固定time物理行63；单一符号定义边界63–63；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A22｜G01/A｜目标指针指向e820_table_init。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820_table]
```c
e820_table_s *e820_table __refdata			= &e820_table_init;
```
固定time物理行65；单一符号定义边界65–65；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A23｜G01/A｜nr_entries与静态entries容量的定义。

[VERIFIED mykernel/arch/x86_64/mm/early/e820_types.h::e820_table]
```c
	struct e820_table {
		__u32			nr_entries;
		e820_entry_s	entries[E820_MAX_ENTRIES];
	};
```
固定time物理行29–32；单一符号定义边界29–32；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A24｜G01/A｜目标数组固定128条，不是自动扩容。

[VERIFIED mykernel/arch/x86_64/mm/early/e820_const.h::E820_MAX_ENTRIES]
```c
	#define E820_MAX_ENTRIES	128
```
固定time物理行83；单一符号定义边界83–83；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A25｜G01/A｜循环前nr_entries先清零。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup]
```c
	int i = 0;
	e820_table->nr_entries = 0;
	char *who = "BIOS-e820";
```
固定time物理行99–101；单一符号定义边界97–116；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A26｜G01/A｜复制按len/type非0迭代，没有读tag.size或entry_size。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup]
```c
	for (mbi_mmap_ent_s *mmap_entp = ram_map;
		mmap_entp->len != 0 && mmap_entp->type != 0;
		i++, mmap_entp++)
```
固定time物理行103–105；单一符号定义边界97–116；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A27｜G01/A｜e820_table->entries[i]先形成，然后依次写addr、size和type。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup]
```c
		e820_entry_s *e820_entp = &(e820_table->entries[i]);
		e820_entp->addr = mmap_entp->addr;
		e820_entp->size = mmap_entp->len;
		e820_entp->type = mmap_entp->type;
```
固定time物理行107–110；单一符号定义边界97–116；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A28｜G01/A｜循环后才发布nr_entries并打印。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup]
```c
	e820_table->nr_entries = i;

	pr_info("BIOS-provided physical RAM map:\n");
	e820__print_table(who);
```
固定time物理行112–115；单一符号定义边界97–116；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## A29｜G01/A｜打印基于已发布nr_entries，不保证此前复制合法。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__print_table]
```c
	for (i = 0; i < e820_table->nr_entries; i++) {
		pr_info("%s: [mem %#018Lx-%#018Lx] ",
			who,
			e820_table->entries[i].addr,
			e820_table->entries[i].addr + e820_table->entries[i].size - 1);
```
固定time物理行86–90；单一符号定义边界82–95；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B01｜G02/B｜宏未整体加括号，调用实参处右移按C运算符规则求值。

[VERIFIED mykernel/arch/x86_64/mm/early/earlymem_api_arch.h::MAX_ARCH_PFN]
```c
	#define MAX_ARCH_PFN MAXMEM >> PAGE_SHIFT
```
固定time物理行6；单一符号定义边界6–6；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B02｜G02/B｜time快照中是传MAX_ARCH_PFN的包装宏，不是直接函数定义。

[VERIFIED mykernel/arch/x86_64/mm/early/earlymem_api_arch.h::e820__end_of_ram_pfn]
```c
	#define e820__end_of_ram_pfn() e820__end_ram_pfn(MAX_ARCH_PFN)
```
固定time物理行10；单一符号定义边界10–10；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B03｜G02/B｜MAXMEM是1UL左移MAX_PHYSMEM_BITS。

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::MAXMEM]
```c
	#define MAXMEM (1UL << MAX_PHYSMEM_BITS)
```
固定time物理行290；单一符号定义边界290–290；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B04｜G02/B｜本树数值为46。

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_PHYSMEM_BITS]
```c
	#define MAX_PHYSMEM_BITS 46
```
固定time物理行139；单一符号定义边界139–139；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B05｜G02/B｜每页移位12。

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
固定time物理行9；单一符号定义边界9–9；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B06｜G02/B｜页大小为UL 1左移PAGE_SHIFT。

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定time物理行10；单一符号定义边界10–10；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B07｜G02/B｜RAM/RESERVED/ACPI的本树枚举数值。

[VERIFIED mykernel/arch/x86_64/mm/early/e820_const.h::e820_type]
```c
		enum e820_type {
			E820_TYPE_RAM		= 1,
			E820_TYPE_RESERVED	= 2,
			E820_TYPE_ACPI		= 3,
```
固定time物理行14–17；单一符号定义边界14–42；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B08｜G02/B｜last_pfn=0、max_arch_pfn来自宏。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
	int i;
	ulong last_pfn = 0;
	ulong max_arch_pfn = MAX_ARCH_PFN;
```
固定time物理行121–123；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B09｜G02/B｜按目标表nr_entries遍历，取得当前条目。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
	for (i = 0; i < e820_table->nr_entries; i++) {
		e820_entry_s *entry = &(e820_table->entries[i]);
		ulong start_pfn;
		ulong end_pfn;
```
固定time物理行125–128；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B10｜G02/B｜RAM和ACPI参加上界，其它类型continue。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
		if (entry->type != E820_TYPE_RAM &&
		    entry->type != E820_TYPE_ACPI)
			continue;
```
固定time物理行130–132；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B11｜G02/B｜addr+size在右移前求和，溢出与舍入须分开。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
		start_pfn = entry->addr >> PAGE_SHIFT;
		end_pfn = (entry->addr + entry->size) >> PAGE_SHIFT;
```
固定time物理行134–135；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B12｜G02/B｜start>=limit时跳过。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
		if (start_pfn >= limit_pfn)
			continue;
```
固定time物理行137–138；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B13｜G02/B｜end越limit时last=limit并break。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
		if (end_pfn > limit_pfn) {
			last_pfn = limit_pfn;
			break;
		}
```
固定time物理行139–142；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B14｜G02/B｜否则保留最大端点。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
		if (end_pfn > last_pfn)
			last_pfn = end_pfn;
```
固定time物理行143–144；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B15｜G02/B｜返回前另用max_arch_pfn裁切。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
	if (last_pfn > max_arch_pfn)
		last_pfn = max_arch_pfn;
```
固定time物理行147–148；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B16｜G02/B｜返回last_pfn物理页号上界。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__end_ram_pfn]
```c
	pr_info("last_pfn = %#lx max_arch_pfn = %#lx\n",
			last_pfn, max_arch_pfn);
	return last_pfn;
```
固定time物理行150–152；单一符号定义边界119–153；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B17｜G02/B｜max_pfn接收宏包装结果、max_possible_pfn随后接收。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	max_pfn = e820__end_of_ram_pfn();
	max_possible_pfn = max_pfn;
```
固定time物理行220–221；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B18｜G02/B｜max_low_pfn同步并形成high_memory地址表达式。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	max_low_pfn = max_pfn;
	high_memory = (void *)__va(max_pfn * PAGE_SIZE - 1) + 1;
```
固定time物理行228–229；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B19｜G02/B｜__va是地址表达宏，不证明direct map已安装。

[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
固定time物理行25；单一符号定义边界25–25；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B20｜G02/B｜zone截止PFN用max_low_pfn计算。

[VERIFIED mykernel/mm/page_alloc/mmzone.c::zone_sizes_init]
```c
	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;
```
固定time物理行13–15；单一符号定义边界3–18；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B21｜G02/B｜直接调用free_area_init只消费上界；不研究内部。

[VERIFIED mykernel/mm/page_alloc/mmzone.c::zone_sizes_init]
```c
	free_area_init(max_zone_pfns);
```
固定time物理行17；单一符号定义边界3–18；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## B22｜G02/B｜max_pfn在所读源码定义为ulong。

[VERIFIED mykernel/mm/early/memblock.c::max_pfn]
```c
ulong		max_pfn;
```
固定time物理行78；单一符号定义边界78–78；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C01｜G03/C｜内核物理保留的基址、两个链接符号地址差实参。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::early_reserve_memory]
```c
	simple_mmblk_reserve(__pa_symbol(_text),
		(ulong)__end_of_kernel_reserve - (ulong)_text);
```
固定time物理行128–129；单一符号定义边界121–155；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C02｜G03/C｜[0,SZ_64K)的请求。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::early_reserve_memory]
```c
	simple_mmblk_reserve(0, SZ_64K);
```
固定time物理行142；单一符号定义边界121–155；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C03｜G03/C｜[0x9F000,SZ_1M)的请求。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::early_reserve_memory]
```c
		phys_addr_t bios_start = 0x9F000;
		simple_mmblk_reserve(bios_start, SZ_1M - bios_start);
```
固定time物理行151–152；单一符号定义边界121–155；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C04｜G03/C｜SZ_64K为0x10000。

[VERIFIED mykernel/include/linux/kernel/sizes.h::SZ_64K]
```c
	#define SZ_64K			0x00010000
```
固定time物理行27；单一符号定义边界27–27；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C05｜G03/C｜SZ_1M为0x100000。

[VERIFIED mykernel/include/linux/kernel/sizes.h::SZ_1M]
```c
	#define SZ_1M			0x00100000
```
固定time物理行32；单一符号定义边界32–32；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C06｜G03/C｜真实符号物理转换入口。

[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__pa_symbol]
```c
	#define __pa_symbol(x) __phys_addr_symbol((ulong)(x))
```
固定time物理行22；单一符号定义边界22–22；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C07｜G03/C｜初值为__brk_base的地址。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::_brk_start]
```c
ulong _brk_start = (ulong)__brk_base;
```
固定time物理行42；单一符号定义边界42–42；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C08｜G03/C｜初值同__brk_base。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::_brk_end]
```c
ulong _brk_end   = (ulong)__brk_base;
```
固定time物理行43；单一符号定义边界43–43；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C09｜G03/C｜size/align及先行mask=align-1。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
extend_brk(size_t size, size_t align) {
	size_t mask = align - 1;
	void *ret;
```
固定time物理行87–89；单一符号定义边界86–104；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C10｜G03/C｜检查关闭与align&mask，align0不是此检查的拒绝例。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
	BUG_ON(_brk_start == 0);
	BUG_ON(align & mask);
```
固定time物理行92–93；单一符号定义边界86–104；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C11｜G03/C｜先修改_brk_end，再进行边界指针比较。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
	_brk_end = (_brk_end + mask) & ~mask;
	BUG_ON((char *)(_brk_end + size) > __brk_limit);
```
固定time物理行95–96；单一符号定义边界86–104；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C12｜G03/C｜返回指针、增加_end、memset；后续均需合法前提。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
	ret = (void *)_brk_end;
	_brk_end += size;

	memset(ret, 0, size);
```
固定time物理行98–101；单一符号定义边界86–104；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C13｜G03/C｜仅非空区间调用simple_mmblk_reserve，未消费其返回。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::reserve_brk]
```c
	if (_brk_end > _brk_start)
		simple_mmblk_reserve(__pa_symbol(_brk_start),
				_brk_end - _brk_start);
```
固定time物理行108–110；单一符号定义边界106–115；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C14｜G03/C｜不论请求结果均清零_brk_start。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::reserve_brk]
```c
	/* Mark brk area as locked down and no longer taking any
	   new allocations */
	_brk_start = 0;
```
固定time物理行112–114；单一符号定义边界106–115；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C15｜G03/C｜early_reserve_memory先于e820__memory_setup。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	early_reserve_memory();

	// 将固件生成的e820分布提取存储到内核结构中
	e820__memory_setup();
```
固定time物理行189–192；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C16｜G03/C｜early_alloc_pgt_buf先于reserve_brk。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	// 为内核页映射预留内存空间并构建映射树
	early_alloc_pgt_buf();
```
固定time物理行231–232；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C17｜G03/C｜reserve_brk、随后directmap相关调用。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	reserve_brk();

	// cleanup_highmap();
	myos_early_map_pgt_buf_into_directmap();
```
固定time物理行239–242；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C18｜G03/C｜内存图登记入口e820__memblock_setup。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	// 用e820获取的内存分布初始化memblock分配器
	e820__memblock_setup();
```
固定time物理行244–245；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C19｜G03/C｜后续init_mem_mapping，仅标入口先后。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	// 初始化x86架构物理页映射
	init_mem_mapping();
```
固定time物理行248–249；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C20｜G03/C｜后续zone_sizes_init。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
		zone_sizes_init();
```
固定time物理行254；单一符号定义边界169–256；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C21｜G03/C｜以nr_entries为限逐项读取类型。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memblock_setup]
```c
	for (i = 0; i < e820_table->nr_entries; i++)
	{
		e820_entry_s *entry = &(e820_table->entries[i]);
		enum e820_type type = entry->type;
```
固定time物理行171–174；单一符号定义边界155–193；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C22｜G03/C｜u64先加，后用resource_size_t强转比较。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memblock_setup]
```c
		end = entry->addr + entry->size;
		if (end != (resource_size_t)end)
			continue;
```
固定time物理行176–178；单一符号定义边界155–193；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C23｜G03/C｜SOFT_RESERVED转reserve请求。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memblock_setup]
```c
		if (type == E820_TYPE_SOFT_RESERVED)
			simple_mmblk_reserve(entry->addr, entry->size);
```
固定time物理行180–181；单一符号定义边界155–193；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C24｜G03/C｜非RAM跳过add、RAM提出add。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memblock_setup]
```c
		if (type != E820_TYPE_RAM)
			continue;

		simple_mmblk_add(entry->addr, entry->size);
```
固定time物理行183–186；单一符号定义边界155–193；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C25｜G03/C｜最后memblock_trim_memory(PAGE_SIZE)。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memblock_setup]
```c
	/* Throw away partial pages: */
	memblock_trim_memory(PAGE_SIZE);
```
固定time物理行189–190；单一符号定义边界155–193；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C26｜G03/C｜reserve写入reserved类型的add_range。

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_reserve]
```c
simple_mmblk_reserve(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.reserved, base, size, 0);
```
固定time物理行282–283；单一符号定义边界282–284；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C27｜G03/C｜add写入memory类型的add_range。

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add]
```c
simple_mmblk_add(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.memory, base, size, 0);
```
固定time物理行277–278；单一符号定义边界277–279；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C28｜G03/C｜上游不消费返回且size0返回0，不能称保留成功。

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;
```
固定time物理行209–212；单一符号定义边界204–262；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C29｜G03/C｜resource_size_t同phys_addr_t的typedef。

[VERIFIED mykernel/include/linux/kernel/types.h::resource_size_t]
```c
		typedef phys_addr_t	resource_size_t;
```
固定time物理行173；单一符号定义边界173–173；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C30｜G03/C｜phys_addr_t在当前树为u64。

[VERIFIED mykernel/include/linux/kernel/types.h::phys_addr_t]
```c
		typedef u64		phys_addr_t;
```
固定time物理行170；单一符号定义边界170–170；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C31｜G03/C｜SOFT_RESERVED的本树枚举数值。

[VERIFIED mykernel/arch/x86_64/mm/early/e820_const.h::e820_type]
```c
			E820_TYPE_SOFT_RESERVED	= 0xefffffff,
```
固定time物理行41；单一符号定义边界14–42；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C32｜G03/C｜内存区间round_up/down由79的合法集合边界制约。

[VERIFIED mykernel/mm/early/memblock.c::memblock_trim_memory]
```c
	for_each_memory_region_topdown(r) {
		orig_start = r->base;
		orig_end = r->base + r->size;
		start = round_up(orig_start, align);
		end = round_down(orig_end, align);
```
固定time物理行656–660；单一符号定义边界652–674；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C33｜G03/C｜memblock.memory初始cnt0、max受宏控制。

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
	.memory.name		= "memory",
```
固定time物理行84–88；单一符号定义边界84–97；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## C34｜G03/C｜memblock.reserved独立初始cnt0与max。

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
	.reserved.regions	= mmblk_reserved_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MMBLK_RESERVED_REGIONS,
	.reserved.name		= "reserved",
```
固定time物理行90–93；单一符号定义边界84–97；条件性边界：引文本身证明局部表达式/门/调用，**不证明**外部输入有效、先行函数成功返回或实际运行可达。

## 证据解释纪律

- D01/D02为原79 A06/A07的替换锚，旧标签身份、原始95个前缀和旧评论一律冻结；宏前置#ifndef只能在锚外说明，条件编译实际选中与否保持条件。
- A26/A27必须结合A18/A23/A24理解：tag的entry_size/entry_version未用于copy，i达到128时首先形成越界的entries[128]，后续发布nr_entries不可当成可信状态。
- B01–B06在MAXMEM实例展开到该函数实参位置后讨论；(addr+size)的u64求和先于>>12。上界不是RAM数量，不证明页表映射。
- C22与C29/C30联用：resource_size_t和u64同宽的强转不能证明加法未溢出；调用顺序和返回值消费单独记账。C32只说明trim入口，MR79旧指针/变形疑点尚未独立闭合。
- 7条D00不冒充新的内核技术专题；本批候选均为条件记录，详见deferred与facts。


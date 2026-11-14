---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-ROOT-HANDOFF-84
produced_by: GPT-6（当前会话标识；后台不可独立认证）
model_per_owner: GPT-6＋Extra
reading_scope: time@a039d9803ade
base_snapshot: time
read_channel: connector
date: 2026-11-13
status: final
open_questions: [ER84-U01, ER84-U02, ER84-U03, ER84-U04, ER84-U05, ER84-U06, ER84-U07, ER84-U08]
---

# 第84批固定源码短锚

## 引用契约

每个锚点为同一固定time源码中的一个真实定义体、数据标签块或宏内**连续1–5行原文**。不拼接不同函数/标签，不把符号名存在等于实际运行，也不将固定时间点下的源行号当作单独引文身份。关键宏值、链接区段与入口先后是静态推导；最终ELF映射、实际码生成及硬件运行均未检验。

固定来源：`https://github.com/08822407d/MyOS2/tree/a039d9803ade`。本文件共49个原文短锚，支持正文与YAML矩阵证据索引；新候选/缺口各单独登记。不包含第81–83批重做。

### ER84-E01 — 根 PGD 的精确初始化槽数为511零槽加一个高地址指针槽；并非注释所称8KiB。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::early_top_pgt] — `time@a039d9803ade`，源码连续行 368–371（行号只辅助，定义范围已人工核对）。

```asm
SYM_DATA_START_PTI_ALIGNED(early_top_pgt)
	.fill	511,8,0
	.quad	level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE_NOENC
SYM_DATA_END(early_top_pgt)
```

### ER84-E02 — 早期动态表由固定数量的512项页组成。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::early_dynamic_pgts] — `time@a039d9803ade`，源码连续行 373–375（行号只辅助，定义范围已人工核对）。

```asm
SYM_DATA_START_PAGE_ALIGNED(early_dynamic_pgts)
	.fill	512*EARLY_DYNAMIC_PAGE_TABLES,8,0
SYM_DATA_END(early_dynamic_pgts)
```

### ER84-E03 — 同一PUD表第510与511槽分别指向高地址大页与fixmap下级。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::level3_kernel_pgt] — `time@a039d9803ade`，源码连续行 380–384（行号只辅助，定义范围已人工核对）。

```asm
	.fill	510,8,0
	/* (2^48-(2*1024*1024*1024)-((2^39)*511))/(2^30) = 510 */
	.quad	level2_kernel_pgt - __START_KERNEL_map + _KERNPG_TABLE_NOENC
	.quad	level2_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE_NOENC
SYM_DATA_END(level3_kernel_pgt)
```

### ER84-E04 — PMDS宏按配置参数输出高映射的连续PMD叶项。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::level2_kernel_pgt] — `time@a039d9803ade`，源码连续行 399–401（行号只辅助，定义范围已人工核对）。

```asm
	 */
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC, KERNEL_IMAGE_SIZE/PMD_SIZE)
SYM_DATA_END(level2_kernel_pgt)
```

### ER84-E05 — fixmap PMD对象显式填零512项。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::level2_fixmap_pgt] — `time@a039d9803ade`，源码连续行 403–405（行号只辅助，定义范围已人工核对）。

```asm
SYM_DATA_START_PAGE_ALIGNED(level2_fixmap_pgt)
	.fill	512,8,0
	// .fill	(512 - 4 - FIXMAP_PMD_NUM),8,0
```

### ER84-E06 — phys_base 的汇编存储初始化为零；运行期赋值需另证。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::phys_base] — `time@a039d9803ade`，源码连续行 435–438（行号只辅助，定义范围已人工核对）。

```asm
	.align 16
/* This must match the first entry in level2_kernel_pgt */
SYM_DATA(phys_base, .quad 0x0)
EXPORT_SYMBOL(phys_base)
```

### ER84-E07 — 启动C入口指针定义在汇编 REF 区，初始值为 x86_64_start_kernel。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::initial_code] — `time@a039d9803ade`，源码连续行 284–287（行号只辅助，定义范围已人工核对）。

```asm
	/* Both SMP bootup and ACPI suspend change these variables */
	__REFDATA
	.balign	8
SYM_DATA(initial_code,	.quad x86_64_start_kernel)
```

### ER84-E08 — init_top_pgt 是独立C定义的PGD数组而非extern声明。

[VERIFIED mykernel/mm/misc/init-mm.c::init_top_pgt] — `time@a039d9803ade`，源码连续行 14–14（行号只辅助，定义范围已人工核对）。

```c
pgd_t	init_top_pgt[ENT_PER_TABLE]__aligned(PAGE_SIZE);
```

### ER84-E09 — swapper_pg_dir 被预处理别名为 init_top_pgt。

[VERIFIED mykernel/mm/fault/memfault_api.h::swapper_pg_dir] — `time@a039d9803ade`，源码连续行 17–18（行号只辅助，定义范围已人工核对）。

```c
	extern pgd_t init_top_pgt[];
	#define swapper_pg_dir init_top_pgt
```

### ER84-E10 — 早期动态表页数宏为64。

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::EARLY_DYNAMIC_PAGE_TABLES] — `time@a039d9803ade`，源码连续行 334–334（行号只辅助，定义范围已人工核对）。

```c
	#define EARLY_DYNAMIC_PAGE_TABLES	64
```

### ER84-E11 — PMDS 按COUNT重复输出以2MiB步进的.quad叶项。

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMDS] — `time@a039d9803ade`，源码连续行 391–395（行号只辅助，定义范围已人工核对）。

```c
	#define PMDS(START, PERM, COUNT)							\
				i = 0 ;											\
				.rept (COUNT) ;									\
				.quad	(START) + (i << PMD_SHIFT) + (PERM) ;	\
				i = i + 1 ;										\
```

### ER84-E12 — 镜像名义上限受CONFIG_RANDOMIZE_BASE条件分支影响。

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::KERNEL_IMAGE_SIZE] — `time@a039d9803ade`，源码连续行 376–380（行号只辅助，定义范围已人工核对）。

```c
	#ifdef CONFIG_RANDOMIZE_BASE
	#  define KERNEL_IMAGE_SIZE	(1024 * 1024 * 1024)
	#else
	#  define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
	#endif
```

### ER84-E13 — PMD页大小为2的21次方、表有512项。

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT] — `time@a039d9803ade`，源码连续行 260–261（行号只辅助，定义范围已人工核对）。

```c
	#define PMD_SHIFT			21
	#define PTRS_PER_PMD		512
```

### ER84-E14 — 链接入口实物由链接表达式导出；_text在.text起始。

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS] — `time@a039d9803ade`，源码连续行 26–30（行号只辅助，定义范围已人工核对）。

```ld
	. += (__START_KERNEL_map + ((DMA_END + (_Page2M_Size - 1)) & ~(_Page2M_Size - 1)));
	phys_startup_64 = ABSOLUTE(startup_64 - __START_KERNEL_map);
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - __START_KERNEL_map)
	{
		_text = .;
```

### ER84-E15 — 主数据输入节由显式.data/.data.rel*/.data..page_aligned*收集。

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS] — `time@a039d9803ade`，源码连续行 58–62（行号只辅助，定义范围已人工核对）。

```ld
		*(.data)
		*(.data.rel*)
		*(.data..page_aligned*)
		_edata = .;
	} : data
```

### ER84-E16 — 汇编INITDATA输入映射到输出.init.data段。

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS] — `time@a039d9803ade`，源码连续行 92–94（行号只辅助，定义范围已人工核对）。

```ld
	.init.data : AT(ADDR(.init.data) - __START_KERNEL_map)
	{
		*(.init.data init.data.*) . = ALIGN(8);
```

### ER84-E17 — __bss_start与__bss_stop包住.bss*以及COMMON。

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS] — `time@a039d9803ade`，源码连续行 162–166（行号只辅助，定义范围已人工核对）。

```ld
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
	}
```

### ER84-E18 — 链接脚本1GiB断言不是512MiB配置检测。

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS] — `time@a039d9803ade`，源码连续行 186–187（行号只辅助，定义范围已人工核对）。

```ld
. = ASSERT((_end - 0xffffffff80000000 <= (1024 * 1024 * 1024)),
    "kernel image bigger than KERNEL_IMAGE_SIZE");
```

### ER84-E19 — 汇编INITDATA实际指向.init.data输入节。

[VERIFIED mykernel/include/linux/init/init.h::__INITDATA] — `time@a039d9803ade`，源码连续行 106–106（行号只辅助，定义范围已人工核对）。

```c
	#define __INITDATA		.section	".init.data","aw",%progbits
```

### ER84-E20 — 普通页移位12，即4KiB；表页采用该对齐单位。

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT] — `time@a039d9803ade`，源码连续行 7–10（行号只辅助，定义范围已人工核对）。

```c

	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

### ER84-E21 — C初始化根表的元素数取决于sizeof(pgd_t)。

[VERIFIED mykernel/mm/highmem/highmem_macro.h::ENT_PER_TABLE] — `time@a039d9803ade`，源码连续行 5–5（行号只辅助，定义范围已人工核对）。

```c
	#define ENT_PER_TABLE		(PAGE_SIZE / sizeof(pgd_t))
```

### ER84-E22 — PGD以p4d_t为别名的类型关系。

[VERIFIED mykernel/arch/x86_64/mm/mm_type_declaration_arch.h::pgd_t] — `time@a039d9803ade`，源码连续行 41–42（行号只辅助，定义范围已人工核对）。

```c
	typedef p4dval_t pgdval_t;
	typedef p4d_t pgd_t;
```

### ER84-E23 — 从common_startup_64实际RIP地址减链接地址值形成p2v_offset。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64] — `time@a039d9803ade`，源码连续行 126–127（行号只辅助，定义范围已人工核对）。

```asm
	leaq	common_startup_64(%rip),	%rdi
	subq	.Lcommon_startup_64(%rip),	%rdi
```

### ER84-E24 — 汇编以r15保留的参数作第二实参并调用__startup_64。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64] — `time@a039d9803ade`，源码连续行 134–135（行号只辅助，定义范围已人工核对）。

```asm
	movq	%r15,		%rsi
	call	__startup_64
```

### ER84-E25 — CR3构造是函数返回值加当前RIP相对根页地址。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64] — `time@a039d9803ade`，源码连续行 137–139（行号只辅助，定义范围已人工核对）。

```asm
	/* Form the CR3 value being sure to include the CR3 modifier */
	leaq	early_top_pgt(%rip),	%rcx
	addq	%rcx,		%rax
```

### ER84-E26 — CR3写入后是通过存储的链接地址间接跳向common_startup_64。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64] — `time@a039d9803ade`，源码连续行 145–149（行号只辅助，定义范围已人工核对）。

```asm
	movq	%rax,		%cr3

	/* Branch to the common startup code at its kernel virtual address */
	// ANNOTATE_RETPOLINE_SAFE
	jmp		*.Lcommon_startup_64(%rip)
```

### ER84-E27 — C函数实参及RIP相对early_pgts、_text取值。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 53–56（行号只辅助，定义范围已人工核对）。

```c
ulong __head
__startup_64(ulong p2v_offset, struct boot_params *bp) {
	pmd_t (*early_pgts)[PTRS_PER_PMD] = rip_rel_ptr(&early_dynamic_pgts[0]);
	ulong physaddr = (ulong)rip_rel_ptr(&_text);
```

### ER84-E28 — 入口物理地址高于MAX_PHYSMEM_BITS进入死循环。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 66–68（行号只辅助，定义范围已人工核对）。

```c
	/* Is the address too large? */
	if (physaddr >> MAX_PHYSMEM_BITS)
		for (;;);
```

### ER84-E29 — load_delta设置和PMD对齐检查，非零未被拒绝。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 74–78（行号只辅助，定义范围已人工核对）。

```c
	phys_base = load_delta = __START_KERNEL_map + p2v_offset;

	/* Is the address not 2M aligned? */
	if (load_delta & ~PMD_MASK)
		for (;;); // 在MyOS2中假定 load_delta == 0
```

### ER84-E30 — 只修正early根单槽和level3的末两个下级指针。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 85–89（行号只辅助，定义范围已人工核对）。

```c
	pgd = &rip_rel_ptr(&early_top_pgt[0])->val;
	pgd[pgd_index(__START_KERNEL_map)] += load_delta;

	level3_kernel_pgt[PTRS_PER_PUD - 2].val += load_delta;
	level3_kernel_pgt[PTRS_PER_PUD - 1].val += load_delta;
```

### ER84-E31 — 64页临时区的前两页分别当PUD/PMD页，计数器写2。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 101–105（行号只辅助，定义范围已人工核对）。

```c
	pud = &early_pgts[0]->val;
	pmd = &early_pgts[1]->val;
	next_early_pgt = 2;

	pgtable_flags = _KERNPG_TABLE_NOENC;
```

### ER84-E32 — PGD身份槽i及相邻i+1共用一个PUD表指针。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 107–110（行号只辅助，定义范围已人工核对）。

```c
	// if (!la57)
	i = (physaddr >> PGDIR_SHIFT) % PTRS_PER_PGD;
	pgd[i + 0] = (pgdval_t)pud + pgtable_flags;
	pgd[i + 1] = (pgdval_t)pud + pgtable_flags;
```

### ER84-E33 — PUD两入口逐项取模512且共用同一PMD表指针。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 112–114（行号只辅助，定义范围已人工核对）。

```c
	i = physaddr >> PUD_SHIFT;
	pud[(i + 0) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
	pud[(i + 1) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
```

### ER84-E34 — PMD循环按跨度计算次数，并对单页512槽取模写入。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 120–123（行号只辅助，定义范围已人工核对）。

```c
	for (i = 0; i < DIV_ROUND_UP(_end - _text, PMD_SIZE); i++) {
		int idx = i + (physaddr >> PMD_SHIFT);
		pmd[idx % PTRS_PER_PMD] = pmd_entry + i * PMD_SIZE;
	}
```

### ER84-E35 — 启动信息的二路memcpy是函数返回前的先行调用。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 131–135（行号只辅助，定义范围已人工核对）。

```c
	if (*rip_rel_ptr(&mbi_magic) == MULTIBOOT2_BOOTLOADER_MAGIC) {
		memcpy(mbi_dst, (const void *)(*mbi_base_p), sizeof(multiboot_MBI));
	} else {
		memcpy(bp_dst, (const void *)bp, sizeof(boot_params));
	}
```

### ER84-E36 — 函数字面返回值为0，非SME校验过的mask。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64] — `time@a039d9803ade`，源码连续行 143–143（行号只辅助，定义范围已人工核对）。

```c
	return 0;
```

### ER84-E37 — 非PIC路径的RIP相对地址辅助函数采用内联汇编。

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::rip_rel_ptr] — `time@a039d9803ade`，源码连续行 98–102（行号只辅助，定义范围已人工核对）。

```c
			static __always_inline __pure void
			*rip_rel_ptr(void *p) {
				asm("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
				return p;
			}
```

### ER84-E38 — 固定允许物理位上限为46位。

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_PHYSMEM_BITS] — `time@a039d9803ade`，源码连续行 138–139（行号只辅助，定义范围已人工核对）。

```c
	// #define MAX_PHYSMEM_BITS (pgtable_l5_enabled() ? 52 : 46)
	#define MAX_PHYSMEM_BITS 46
```

### ER84-E39 — 固定time配置选择mcmodel=kernel且fno-pie/fno-pic，不等于完成构建验证。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS] — `time@a039d9803ade`，源码连续行 39–42（行号只辅助，定义范围已人工核对）。

```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
```

### ER84-E40 — common_startup_64是secondary_startup_64大符号范围内部标签。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::secondary_startup_64] — `time@a039d9803ade`，源码连续行 166–168（行号只辅助，定义范围已人工核对）。

```asm
SYM_INNER_LABEL(common_startup_64, SYM_L_LOCAL)
	/*
	 * For parallel boot, the APIC ID is read from the APIC, and then
```

### ER84-E41 — common路径的间接调用经过initial_code并以ud2截断返回路径。

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::secondary_startup_64] — `time@a039d9803ade`，源码连续行 278–281（行号只辅助，定义范围已人工核对）。

```asm
.Ljump_to_C_code:
	xorl	%ebp,		%ebp	# clear frame pointer
	callq	*initial_code(%rip)
	ud2
```

### ER84-E42 — 清零范围是链接符号的半开BSS区间。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::clear_bss] — `time@a039d9803ade`，源码连续行 149–153（行号只辅助，定义范围已人工核对）。

```c
static void __init
clear_bss(void) {
	memset(__bss_start, 0,
		(unsigned long) __bss_stop - (unsigned long) __bss_start);
}
```

### ER84-E43 — C入口首先调用clear_bss。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel] — `time@a039d9803ade`，源码连续行 155–159（行号只辅助，定义范围已人工核对）。

```c
asmlinkage __visible void __init
x86_64_start_kernel(char * real_mode_data)
{
	clear_bss();

```

### ER84-E44 — C入口显式clear_page(init_top_pgt)后进行早期IDT调用。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel] — `time@a039d9803ade`，源码连续行 164–166（行号只辅助，定义范围已人工核对）。

```c
	clear_page(init_top_pgt);

	idt_setup_early_handler();
```

### ER84-E45 — 在myos_early_init_system返回后才执行第511根项值复制。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel] — `time@a039d9803ade`，源码连续行 171–174（行号只辅助，定义范围已人工核对）。

```c
	myos_early_init_system();

	/* set init_top_pgt kernel high mapping*/
	init_top_pgt[511] = early_top_pgt[511];
```

### ER84-E46 — 复制之后调用start_kernel。

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel] — `time@a039d9803ade`，源码连续行 178–179（行号只辅助，定义范围已人工核对）。

```c
		/* version is always not zero if it is copied */
		start_kernel();
```

### ER84-E47 — start_kernel的源定义第一项主体调用是setup_arch。

[VERIFIED mykernel/init/main.c::start_kernel] — `time@a039d9803ade`，源码连续行 136–139（行号只辅助，定义范围已人工核对）。

```c


	setup_arch(NULL);

```

### ER84-E48 — setup_arch执行早期页表缓冲区分配。

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch] — `time@a039d9803ade`，源码连续行 230–232（行号只辅助，定义范围已人工核对）。

```c

	// 为内核页映射预留内存空间并构建映射树
	early_alloc_pgt_buf();
```

### ER84-E49 — 后续显式切换CR3到swapper_pg_dir并刷新TLB。

[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping] — `time@a039d9803ade`，源码连续行 529–530（行号只辅助，定义范围已人工核对）。

```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```

## 未升格的边界

- `[INFERRED]` 为由源语句与明确输入的模型推导，不能当作硬件实测。
- 条件分支宏未进行真实预处理；链接映射、机器码、入口调用返回与CPU特性没有构建/运行证据。
- 第81–83批审查的限定仍适用；先行callee的副作用不可从单一调用语句排除。

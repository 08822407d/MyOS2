---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-PGTABLE-STORAGE-81
record_type: fixed_time_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第81批固定源码短锚（先行说明）

仅取固定time对应符号定义内非空连续1–5物理行；行号只辅助。链接脚本输出节按其真实section体判断定义边界。宏外#ifndef不在宏定义锚内。引文证明局部源码存在及条件计算，不证明构建选择、链接位置、指针可写、内存分配成功或硬件TLB效果。旧79/80现按正式审查限定消费，本81没有D00。

## G01/A：brk来源、容量与页号/别名

### A01｜start页号静态对象
[VERIFIED mykernel/arch/x86_64/mm/init.c::pgt_buf_start]
```c
static ulong __initdata pgt_buf_start;
```
固定time物理行47；符号范围47–47；依赖所有先行合法性前提。

### A02｜end页号游标静态对象
[VERIFIED mykernel/arch/x86_64/mm/init.c::pgt_buf_end]
```c
static ulong __initdata pgt_buf_end;
```
固定time物理行48；符号范围48–48；依赖所有先行合法性前提。

### A03｜top页号上界静态对象
[VERIFIED mykernel/arch/x86_64/mm/init.c::pgt_buf_top]
```c
static ulong __initdata pgt_buf_top;
```
固定time物理行49；符号范围49–49；依赖所有先行合法性前提。

### A04｜chunk采用PMD_SIZE
[VERIFIED mykernel/arch/x86_64/mm/init.c::EARLY_PGTBUF_CHUNK_SIZE]
```c
#define EARLY_PGTBUF_CHUNK_SIZE      PMD_SIZE
```
固定time物理行118；符号范围118–118；依赖所有先行合法性前提。

### A05｜最大chunk数为4UL
[VERIFIED mykernel/arch/x86_64/mm/init.c::EARLY_PGTBUF_MAX_CHUNKS]
```c
#define EARLY_PGTBUF_MAX_CHUNKS      4UL
```
固定time物理行119；符号范围119–119；依赖所有先行合法性前提。

### A06｜buffer乘法表达式
[VERIFIED mykernel/arch/x86_64/mm/init.c::EARLY_PGTBUF_SIZE]
```c
#define EARLY_PGTBUF_SIZE            (EARLY_PGTBUF_MAX_CHUNKS * EARLY_PGTBUF_CHUNK_SIZE)
```
固定time物理行120；符号范围120–120；依赖所有先行合法性前提。

### A07｜宏声明生成.bss..brk节数组
[VERIFIED mykernel/arch/x86_64/include/asm/setup.h::RESERVE_BRK]
```c
		#define RESERVE_BRK(name, size)							\
					__section(".bss..brk") __aligned(1) __used	\
						static char __brk_##name[size]
```
固定time物理行117–119；符号范围117–119；依赖所有先行合法性前提。

### A08｜链接.bss节的.bss*匹配
[VERIFIED mykernel/arch/x86_64/kernel.lds::.bss]
```c
	.bss ALIGN(_Page_Size) : AT(ADDR(.bss) - __START_KERNEL_map)
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
```
固定time物理行161–165；符号范围161–167；依赖所有先行合法性前提。

### A09｜brk单独段与base符号
[VERIFIED mykernel/arch/x86_64/kernel.lds::.brk]
```c
	.brk ALIGN(_Page_Size) : AT(ADDR(.brk) - __START_KERNEL_map)
	{
		__brk_base = .;
```
固定time物理行168–170；符号范围168–174；依赖所有先行合法性前提。

### A10｜brk的64KiB常量与.brk*模式和limit
[VERIFIED mykernel/arch/x86_64/kernel.lds::.brk]
```c
		. += 64 * 1024;
		*(.brk*)
		__brk_limit = .;
```
固定time物理行171–173；符号范围168–174；依赖所有先行合法性前提。

### A11｜运行时brk起点
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::_brk_start]
```c
ulong _brk_start = (ulong)__brk_base;
```
固定time物理行42；符号范围42–42；依赖所有先行合法性前提。

### A12｜运行时brk终点
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::_brk_end]
```c
ulong _brk_end   = (ulong)__brk_base;
```
固定time物理行43；符号范围43–43；依赖所有先行合法性前提。

### A13｜申请接口与mask计算
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
extend_brk(size_t size, size_t align) {
	size_t mask = align - 1;
	void *ret;
```
固定time物理行87–89；符号范围86–104；依赖所有先行合法性前提。

### A14｜先行检查和对齐上界
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
	BUG_ON(_brk_start == 0);
	BUG_ON(align & mask);

	_brk_end = (_brk_end + mask) & ~mask;
	BUG_ON((char *)(_brk_end + size) > __brk_limit);
```
固定time物理行92–96；符号范围86–104；依赖所有先行合法性前提。

### A15｜保存指针、推进end与memset
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c
	ret = (void *)_brk_end;
	_brk_end += size;

	memset(ret, 0, size);
```
固定time物理行98–101；符号范围86–104；依赖所有先行合法性前提。

### A16｜完成返回路径
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::extend_brk]
```c

	return ret;
```
固定time物理行102–103；符号范围86–104；依赖所有先行合法性前提。

### A17｜8MiB申请及__pa入口
[VERIFIED mykernel/arch/x86_64/mm/init.c::early_alloc_pgt_buf]
```c
void __init early_alloc_pgt_buf(void) {
	ulong tables = EARLY_PGTBUF_SIZE;
	phys_addr_t base = __pa(extend_brk(tables, PMD_SIZE));

```
固定time物理行126–129；符号范围126–135；依赖所有先行合法性前提。

### A18｜start、两地址别名和end/top写入
[VERIFIED mykernel/arch/x86_64/mm/init.c::early_alloc_pgt_buf]
```c
	pgt_buf_start = base >> PAGE_SHIFT;
	early_pgtbuf_pud = (void *)base + __START_KERNEL_map;
	early_pgtbuf_pmd = (void *)early_pgtbuf_pud + PAGE_SIZE;
	pgt_buf_end = pgt_buf_start + 2;
	pgt_buf_top = pgt_buf_start + (tables >> PAGE_SHIFT);
```
固定time物理行130–134；符号范围126–135；依赖所有先行合法性前提。

### A19｜PUD页别名静态指针
[VERIFIED mykernel/arch/x86_64/mm/init.c::early_pgtbuf_pud]
```c
static pud_t *early_pgtbuf_pud;
```
固定time物理行123；符号范围123–123；依赖所有先行合法性前提。

### A20｜PMD页别名静态指针
[VERIFIED mykernel/arch/x86_64/mm/init.c::early_pgtbuf_pmd]
```c
static pmd_t *early_pgtbuf_pmd;
```
固定time物理行124；符号范围124–124；依赖所有先行合法性前提。

### A21｜初始化brk分配先于保留
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	// 为内核页映射预留内存空间并构建映射树
	early_alloc_pgt_buf();
```
固定time物理行231–232；符号范围169–256；依赖所有先行合法性前提。

### A22｜关闭brk调用位置
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	 * Need to conclude brk, before e820__memblock_setup()
	 * it could use memblock_find_in_range, could overlap with
	 * brk area.
	 */
	reserve_brk();
```
固定time物理行235–239；符号范围169–256；依赖所有先行合法性前提。

### A23｜brk保留请求与返回未检查
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::reserve_brk]
```c
	if (_brk_end > _brk_start)
		simple_mmblk_reserve(__pa_symbol(_brk_start),
				_brk_end - _brk_start);
```
固定time物理行108–110；符号范围106–115；依赖所有先行合法性前提。

### A24｜brk游标归零
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::reserve_brk]
```c
	/* Mark brk area as locked down and no longer taking any
	   new allocations */
	_brk_start = 0;
```
固定time物理行112–114；符号范围106–115；依赖所有先行合法性前提。

### A25｜页移位12
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
固定time物理行9；符号范围9–9；依赖所有先行合法性前提。

### A26｜页字节数宏
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定time物理行10；符号范围10–10；依赖所有先行合法性前提。

### A27｜PMD移位21
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT]
```c
	#define PMD_SHIFT			21
```
固定time物理行260；符号范围260–260；依赖所有先行合法性前提。

### A28｜PMD_SIZE=2MiB
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SIZE]
```c
	#define PMD_SIZE			(_AC(1, UL) << PMD_SHIFT)
```
固定time物理行274；符号范围274–274；依赖所有先行合法性前提。

### A29｜镜像基址常量
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::__START_KERNEL_map]
```c
	#define __START_KERNEL_map		_AC(0xffffffff80000000, UL)
```
固定time物理行79；符号范围79–79；依赖所有先行合法性前提。

### A30｜__pa到__phys_addr的宏
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__pa]
```c
	#  define __pa(x)	__phys_addr((ulong)(x))
```
固定time物理行6；符号范围6–6；依赖所有先行合法性前提。

### A31｜物理转换算术体
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::__phys_addr_nodebug]
```c
		__phys_addr_nodebug(ulong x) {
			ulong y = x - __START_KERNEL_map;

			/* use the carry flag to determine if x was < __START_KERNEL_map */
			x = y + ((x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));
```
固定time物理行40–44；符号范围38–47；依赖所有先行合法性前提。

### A32｜direct-map别名表达式
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
固定time物理行25；符号范围25–25；依赖所有先行合法性前提。


## G02/B：提前页表表项、跨界、root与刷新

### B01｜由buffer PFN产生物理起止
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	phys_start	= pgt_buf_start << PAGE_SHIFT;
	phys_end	= pgt_buf_top   << PAGE_SHIFT;
```
固定time物理行160–161；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B02｜从phys起止形成direct-map VA/vend表达式
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	va			= (ulong)__va(phys_start);
	vend		= (ulong)__va(phys_end - 1) + 1;
```
固定time物理行166–167；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B03｜跨PGD/PUD拒绝检查位于注释而非活跃条件
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	// if (pgd_index(va) != pgd_index(vend - 1))
	// 	panic("myos_early_map_pgt_buf_into_directmap: pgt_buf crosses PGD boundary");
	// if (pud_index(va) != pud_index(vend - 1))
	// 	panic("myos_early_map_pgt_buf_into_directmap: pgt_buf crosses PUD boundary");
```
固定time物理行175–178；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B04｜选early/init两个root槽，清PUD页再populate
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	early_pgd	= &early_top_pgt[pgd_index(va)];
	init_pgd	= &init_top_pgt[pgd_index(va)];
	p4d			= (p4d_t *)early_pgd;
	memset(early_pgtbuf_pud, 0, PAGE_SIZE);
	p4d_populate(&init_mm, p4d, early_pgtbuf_pud);
```
固定time物理行186–190；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B05｜单一PUD槽指向清零后的PMD页
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	pud			= &pud_base[pud_index(va)];
	memset(early_pgtbuf_pmd, 0, PAGE_SIZE);
	pud_populate(&init_mm, pud, early_pgtbuf_pmd);
	pmd_base	= early_pgtbuf_pmd;
```
固定time物理行197–200；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B06｜由物理区间算nr_pmd及每轮pa/va/slot
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	nr_pmd	= (phys_end - phys_start) / PMD_SIZE;
	pa		= phys_start;
	for (i = 0; i < nr_pmd; i++, pa += PMD_SIZE) {
		ulong this_va	= va + i * PMD_SIZE;
		ulong pmd_slot	= pmd_index(this_va);
```
固定time物理行206–210；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B07｜构造leaf val并set_pmd写入
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
		pmd_t *pmd		= &pmd_base[pmd_slot];
		pmdval_t val	= pa | pgprot_val(PAGE_KERNEL_LARGE);
		set_pmd(pmd, __pmd(val));
	}
```
固定time物理行211–214；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B08｜先请求flush再复制early根到init根
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	__flush_tlb_all();

	// 将当前在 early_pgd 中设置的映射复制到 init_pgd 中，后者是正式启用的 direct-map 顶层页表。
	*init_pgd = *early_pgd;
```
固定time物理行220–223；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B09｜根与p4d指针局部类型
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	pgd_t *early_pgd;
	pgd_t *init_pgd;
	p4d_t *p4d;
```
固定time物理行153–155；符号范围146–224；不可用代码注释或宏名称代替硬件实际映射证明。

### B10｜pgd_index别名ent_index_in_p4d
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_index]
```c
	#define pgd_index				ent_index_in_p4d
```
固定time物理行94；符号范围94–94；不可用代码注释或宏名称代替硬件实际映射证明。

### B11｜pud_index别名
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pud_index]
```c
	#define pud_index		ent_index_in_pud
```
固定time物理行72；符号范围72–72；不可用代码注释或宏名称代替硬件实际映射证明。

### B12｜pmd_index别名
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pmd_index]
```c
	#define pmd_index		ent_index_in_pmd
```
固定time物理行73；符号范围73–73；不可用代码注释或宏名称代替硬件实际映射证明。

### B13｜pgd层真实位移及511掩码
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_p4d]
```c
		ulong
		ent_index_in_p4d(ulong vaddr) {
			return (vaddr >> P4D_SHIFT) & (PTRS_PER_P4D - 1);
		}
```
固定time物理行192–195；符号范围191–196；不可用代码注释或宏名称代替硬件实际映射证明。

### B14｜PUD索引计算
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pud]
```c
		ulong
		ent_index_in_pud(ulong vaddr) {
			return (vaddr >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
```
固定time物理行233–236；符号范围232–237；不可用代码注释或宏名称代替硬件实际映射证明。

### B15｜PMD索引计算
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pmd]
```c
		ulong
		ent_index_in_pmd(ulong vaddr) {
			return (vaddr >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
```
固定time物理行274–277；符号范围273–278；不可用代码注释或宏名称代替硬件实际映射证明。

### B16｜populated PUD页的宏别名
[VERIFIED mykernel/mm/fault/pgtable_macro.h::p4d_populate]
```c
	#define p4d_populate	fill_pud_to_p4d_ent
```
固定time物理行85；符号范围85–85；不可用代码注释或宏名称代替硬件实际映射证明。

### B17｜populated PMD页的宏别名
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pud_populate]
```c
	#define pud_populate	fill_pmd_to_pud_ent
```
固定time物理行86；符号范围86–86；不可用代码注释或宏名称代替硬件实际映射证明。

### B18｜以__pa(pud)构造PAGE_TABLE父槽
[VERIFIED mykernel/mm/fault/pgtable.h::fill_pud_to_p4d_ent]
```c
		void
		fill_pud_to_p4d_ent(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p) {
			// paravirt_alloc_pud(mm, __pa(pud_p) >> PAGE_SHIFT);
			set_p4d_ent(p4d_entp, __p4d(__pa(pud_p) | _PAGE_TABLE));
		}
```
固定time物理行203–207；符号范围202–207；不可用代码注释或宏名称代替硬件实际映射证明。

### B19｜以__pa(pmd)构造PAGE_TABLE父槽
[VERIFIED mykernel/mm/fault/pgtable.h::fill_pmd_to_pud_ent]
```c
		void
		fill_pmd_to_pud_ent(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p) {
			// paravirt_alloc_pmd(mm, __pa(pmd_p) >> PAGE_SHIFT);
			set_pud_ent(pud_entp, __pud(__pa(pmd_p) | _PAGE_TABLE));
		}
```
固定time物理行244–248；符号范围243–248；不可用代码注释或宏名称代替硬件实际映射证明。

### B20｜P4D父槽真实WRITE_ONCE写入
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_p4d_ent]
```c
		void
		set_p4d_ent(p4d_t *p4de_ptr, p4d_t p4d_ent) {
			WRITE_ONCE(*(p4de_ptr), (p4d_ent));
		}
```
固定time物理行442–445；符号范围441–446；不可用代码注释或宏名称代替硬件实际映射证明。

### B21｜PUD父槽真实WRITE_ONCE写入
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pud_ent]
```c
		void
		set_pud_ent(pud_t *pude_ptr, pud_t pud_ent) {
			WRITE_ONCE(*(pude_ptr), (pud_ent));
		}
```
固定time物理行497–500；符号范围496–501；不可用代码注释或宏名称代替硬件实际映射证明。

### B22｜PMD leaf真实WRITE_ONCE写入
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pmd_ent]
```c
		void
		set_pmd_ent(pmd_t *pmde_ptr, pmd_t pmd_ent) {
			WRITE_ONCE(*(pmde_ptr), (pmd_ent));
		}
```
固定time物理行551–554；符号范围550–555；不可用代码注释或宏名称代替硬件实际映射证明。

### B23｜set_pmd宏接set_pmd_ent
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::set_pmd]
```c
	#define set_pmd					set_pmd_ent
```
固定time物理行8；符号范围8–8；不可用代码注释或宏名称代替硬件实际映射证明。

### B24｜__pmd到arch_make_pmde
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::__pmd]
```c
	#define __pmd(x)			arch_make_pmde(x)
```
固定time物理行26；符号范围26–26；不可用代码注释或宏名称代替硬件实际映射证明。

### B25｜PGD/P4D层位移39
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::P4D_SHIFT]
```c
	#define P4D_SHIFT			39
```
固定time物理行249；符号范围249–249；不可用代码注释或宏名称代替硬件实际映射证明。

### B26｜PUD层位移30
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PUD_SHIFT]
```c
	#define PUD_SHIFT			30
```
固定time物理行254；符号范围254–254；不可用代码注释或宏名称代替硬件实际映射证明。

### B27｜PMD层位移21
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT]
```c
	#define PMD_SHIFT			21
```
固定time物理行260；符号范围260–260；不可用代码注释或宏名称代替硬件实际映射证明。

### B28｜PMD一页512槽
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PTRS_PER_PMD]
```c
	#define PTRS_PER_PMD		512
```
固定time物理行261；符号范围261–261；不可用代码注释或宏名称代替硬件实际映射证明。

### B29｜PUD一页512槽
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PTRS_PER_PUD]
```c
	#define PTRS_PER_PUD		512
```
固定time物理行255；符号范围255–255；不可用代码注释或宏名称代替硬件实际映射证明。

### B30｜大页保护位经过__pgprot_mask
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_KERNEL_LARGE]
```c
	#  define PAGE_KERNEL_LARGE		    __pgprot_mask(__PAGE_KERNEL_LARGE      | _ENC)
```
固定time物理行221；符号范围221–221；不可用代码注释或宏名称代替硬件实际映射证明。

### B31｜大页位含PSE
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::__PAGE_KERNEL_LARGE]
```c
	#define __PAGE_KERNEL_LARGE			(__PP|__RW|   0|___A|__NX|___D|_PSE|___G)
```
固定time物理行200；符号范围200–200；不可用代码注释或宏名称代替硬件实际映射证明。

### B32｜pgprot_val提取pgprot域
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::pgprot_val]
```c
	#define pgprot_val(x)				((x).pgprot)
```
固定time物理行176；符号范围176–176；不可用代码注释或宏名称代替硬件实际映射证明。

### B33｜实际init根数组定义
[VERIFIED mykernel/mm/misc/init-mm.c::init_top_pgt]
```c
pgd_t	init_top_pgt[ENT_PER_TABLE]__aligned(PAGE_SIZE);
```
固定time物理行14；符号范围14–14；不可用代码注释或宏名称代替硬件实际映射证明。

### B34｜swapper_pg_dir宏指向init_top_pgt
[VERIFIED mykernel/mm/fault/memfault_api.h::swapper_pg_dir]
```c
	#define swapper_pg_dir init_top_pgt
```
固定time物理行18；符号范围18–18；不可用代码注释或宏名称代替硬件实际映射证明。

### B35｜flush入口
[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::__flush_tlb_all]
```c
void __flush_tlb_all(void)
{
```
固定time物理行251–252；符号范围251–268；不可用代码注释或宏名称代替硬件实际映射证明。

### B36｜所读函数调用flush_tlb_local
[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::__flush_tlb_all]
```c
	// 	 * !PGE -> !PCID (setup_pcid()), thus every flush is total.
	// 	 */
		flush_tlb_local();
```
固定time物理行263–265；符号范围251–268；不可用代码注释或宏名称代替硬件实际映射证明。

### B37｜后继load_cr3加flush的独立阶段
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```
固定time物理行529–530；符号范围487–532；不可用代码注释或宏名称代替硬件实际映射证明。


## G03/C：三源分配、数量、清零与失败

### C01｜阶段标志对象
[VERIFIED mykernel/arch/x86_64/mm/init.c::after_bootmem]
```c
int after_bootmem;
```
固定time物理行226；符号范围226–226；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C02｜晚期order和__get_free_pages直接返回
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
__ref void
*alloc_low_pages(uint num) {
	if (after_bootmem) {
		uint order = get_order((ulong)num << PAGE_SHIFT);
		return (void *)__get_free_pages(GFP_ATOMIC | __GFP_ZERO, order);
```
固定time物理行62–66；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C03｜早期end+num容量门
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
	ulong pfn;
	int i;
	if ((pgt_buf_end + num) > pgt_buf_top) {
```
固定time物理行69–71；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C04｜mapped窗口门后才请求memblock物理
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
			ret = memblock_phys_alloc_range(
					PAGE_SIZE * num, PAGE_SIZE,
					min_pfn_mapped << PAGE_SHIFT,
					max_pfn_mapped << PAGE_SHIFT);
```
固定time物理行75–78；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C05｜物理返回0的panic/while失败出口
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
		if (!ret) {
			panic("alloc_low_pages: can not alloc memory");
			while (1);
		}
```
固定time物理行81–84；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C06｜回退pf​​n与缓冲cursor的分叉
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
		pfn = ret >> PAGE_SHIFT;
	} else {
		pfn = pgt_buf_end;
		pgt_buf_end += num;
	}
```
固定time物理行86–90；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C07｜早期逐页clear_page
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
	for (i = 0; i < num; i++) {
		void *adr = __va((pfn + i) << PAGE_SHIFT);
		clear_page(adr);
	}
```
固定time物理行92–95；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C08｜早期__va地址返回
[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
	return __va(pfn << PAGE_SHIFT);
```
固定time物理行97；符号范围62–98；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C09｜阶段标志写1
[VERIFIED mykernel/arch/x86_64/mm/init.c::mem_init]
```c
void __init mem_init(void)
{
	/* clear_bss() already clear the empty_zero_page */

	after_bootmem = 1;
```
固定time物理行891–895；符号范围891–912；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C10｜随后memblock_free_all调用
[VERIFIED mykernel/arch/x86_64/mm/init.c::mem_init]
```c
	/* this will put all memory onto the freelists */
	memblock_free_all();
```
固定time物理行898–899；符号范围891–912；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C11｜包装实际以num1调用
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::alloc_low_page]
```c
		PREFIX_STATIC_INLINE
		void
		*alloc_low_page(void) {
			return alloc_low_pages(1);
		}
```
固定time物理行89–93；符号范围89–93；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C12｜常量0特殊门
[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
		static __always_inline __attribute_const__
		int get_order(unsigned long size) {
			if (__builtin_constant_p(size)) {
				if (!size)
					return BITS_PER_LONG - PAGE_SHIFT;
```
固定time物理行29–33；符号范围29–44；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C13｜常量正size的阶数
[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
				if (size < (1UL << PAGE_SHIFT))
					return 0;

				return ilog2((size) - 1) - PAGE_SHIFT + 1;
```
固定time物理行35–38；符号范围29–44；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C14｜动态size路径
[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
			size--;
			size >>= PAGE_SHIFT;

			return fls64(size);
```
固定time物理行40–43；符号范围29–44；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C15｜失败0与页虚拟地址
[VERIFIED mykernel/mm/page_alloc/buddy.c::__get_free_pages]
```c
{
	page_s *page = alloc_pages(gfp_mask & ~__GFP_HIGHMEM, order);
	if (page == NULL)
		return 0;
	return (ulong)page_to_virt(page);
```
固定time物理行532–536；符号范围531–537；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C16｜ZERO原始位
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::___GFP_ZERO]
```c
	#define ___GFP_ZERO					0x100u
```
固定time物理行23；符号范围23–23；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C17｜ZERO类型宏
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::__GFP_ZERO]
```c
	#define __GFP_ZERO				((__force gfp_t)___GFP_ZERO)
```
固定time物理行226；符号范围226–226；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C18｜GFP_ATOMIC常量组合
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_ATOMIC]
```c
	#define GFP_ATOMIC				(__GFP_HIGH|__GFP_ATOMIC|__GFP_KSWAPD_RECLAIM)
```
固定time物理行308；符号范围308–308；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C19｜clear_page转memset(PAGE_SIZE)
[VERIFIED mykernel/include/linux/kernel/asm-generic/page.h::clear_page]
```c
	#	define clear_page(page)		memset((page), 0, PAGE_SIZE)
```
固定time物理行37；符号范围37–37；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C20｜页层ZERO时的实际byte地址式
[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```
固定time物理行190–192；符号范围137–196；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C21｜物理申请的参数
[VERIFIED mykernel/mm/early/memblock.c::memblock_phys_alloc_range]
```c
phys_addr_t __init
memblock_phys_alloc_range(phys_addr_t size, phys_addr_t align,
		phys_addr_t start, phys_addr_t end) {
```
固定time物理行502–504；符号范围502–509；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C22｜物理申请转入memblock_alloc_range
[VERIFIED mykernel/mm/early/memblock.c::memblock_phys_alloc_range]
```c
	return memblock_alloc_range(size, align, start, end);
```
固定time物理行508；符号范围502–509；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C23｜end哨兵处理
[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	if (end == MEMBLOCK_ALLOC_ACCESSIBLE ||
		end == MEMBLOCK_ALLOC_NOLEAKTRACE)
		end = memblock.current_limit;
```
固定time物理行474–476；符号范围465–488；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C24｜外层PAGE_SIZE下界
[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);
```
固定time物理行478–480；符号范围465–488；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C25｜finder/保留成功门
[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	found = __memblock_find_range_bottom_up(
				start, end, size, align, flags);
	if (found && !simple_mmblk_reserve(found, size))
		return found;
	else
```
固定time物理行482–486；符号范围465–488；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C26｜mapped窗口下界对象
[VERIFIED mykernel/arch/x86_64/mm/init.c::min_pfn_mapped]
```c
static ulong min_pfn_mapped;
```
固定time物理行51；符号范围51–51；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C27｜mapped窗口上界对象
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::max_pfn_mapped]
```c
ulong max_pfn_mapped;
```
固定time物理行36；符号范围36–36；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C28｜窗口上界的直接消费者
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::get_max_mapped]
```c
		PREFIX_STATIC_INLINE
		phys_addr_t
		get_max_mapped(void) {
			return (phys_addr_t)max_pfn_mapped << PAGE_SHIFT;
```
固定time物理行83–86；符号范围83–87；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C29｜被注释的登记非活动生产者
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
	// add_pfn_range_mapped(start >> PAGE_SHIFT, ret >> PAGE_SHIFT);

	return ret >> PAGE_SHIFT;
```
固定time物理行449–451；符号范围434–452；仅局部静态证据，先行地址/容量/返回门未证时停止。

### C30｜后续load_cr3与flush
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```
固定time物理行529–530；符号范围487–532；仅局部静态证据，先行地址/容量/返回门未证时停止。


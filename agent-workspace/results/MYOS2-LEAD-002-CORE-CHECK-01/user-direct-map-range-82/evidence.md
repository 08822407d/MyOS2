---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-RANGE-82
record_type: fixed_time_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第82批固定源码证据

所有编号引文均为固定time对象的单一真实符号定义内、非空连续1–5物理行；行号只辅助。每条附事实键与必含字面以供交付前正文、矩阵、模型和图的支持关系检查。尚未认证外部输入、硬件能力、内存空闲、表项、下层成功返回或真实运行。

## A01｜gb_default
[VERIFIED mykernel/arch/x86_64/mm/init.c::direct_gbpages]
```c
int direct_gbpages = 1;
```
固定time物理行227；定义范围227–227；事实键`gb_default`；支持字面`direct_gbpages`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A02｜global_mask
[VERIFIED mykernel/arch/x86_64/mm/init.c::page_size_mask]
```c
static int page_size_mask;
```
固定time物理行234；定义范围234–234；事实键`global_mask`；支持字面`page_size_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A03｜probe_2m
[VERIFIED mykernel/arch/x86_64/mm/init.c::simple_probe_page_size_mask]
```c
	if (boot_cpu_data.x86_capa_bits.PageSize_Ext)
		page_size_mask |= 1 << PG_LEVEL_2M;
	else
		direct_gbpages = 0;
```
固定time物理行268–271；定义范围261–280；事实键`probe_2m`；支持字面`PageSize_Ext`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A04｜probe_1g
[VERIFIED mykernel/arch/x86_64/mm/init.c::simple_probe_page_size_mask]
```c
	if (direct_gbpages && boot_cpu_data.x86_capa_bits.Page_1GB) {
		printk(KERN_INFO "Using GB pages for direct mapping\n");
		page_size_mask |= 1 << PG_LEVEL_1G;
	} else {
```
固定time物理行274–277；定义范围261–280；事实键`probe_1g`；支持字面`Page_1GB`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A05｜probe_1g_else
[VERIFIED mykernel/arch/x86_64/mm/init.c::simple_probe_page_size_mask]
```c
		direct_gbpages = 0;
	}
```
固定time物理行278–279；定义范围261–280；事实键`probe_1g_else`；支持字面`direct_gbpages`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A06｜probe_alias
[VERIFIED mykernel/arch/x86_64/mm/init.c::probe_page_size_mask]
```c
#define probe_page_size_mask simple_probe_page_size_mask
```
固定time物理行281；定义范围281–281；事实键`probe_alias`；支持字面`simple_probe`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A07｜level_enum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::pg_level]
```c
		enum pg_level {
			PG_LEVEL_NONE,
			PG_LEVEL_4K,
			PG_LEVEL_2M,
			PG_LEVEL_1G,
```
固定time物理行7–11；定义范围7–14；事实键`level_enum`；支持字面`PG_LEVEL_1G`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A08｜pmd_shift
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT]
```c
	#define PMD_SHIFT			21
```
固定time物理行260；定义范围260–260；事实键`pmd_shift`；支持字面`21`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A09｜pud_shift
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PUD_SHIFT]
```c
	#define PUD_SHIFT			30
```
固定time物理行254；定义范围254–254；事实键`pud_shift`；支持字面`30`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A10｜pmd_size
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SIZE]
```c
	#define PMD_SIZE			(_AC(1, UL) << PMD_SHIFT)
```
固定time物理行274；定义范围274–274；事实键`pmd_size`；支持字面`PMD_SHIFT`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A11｜pud_size
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PUD_SIZE]
```c
	#define PUD_SIZE			(_AC(1, UL) << PUD_SHIFT)
```
固定time物理行276；定义范围276–276；事实键`pud_size`；支持字面`PUD_SHIFT`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A12｜cpu_pse
[VERIFIED mykernel/arch/x86_64/include/asm/cpufeatures.h::x86_cap_bits]
```c
				PageSize_Ext			: 1,
```
固定time物理行49；定义范围42–500；事实键`cpu_pse`；支持字面`PageSize_Ext`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A13｜cpu_1g
[VERIFIED mykernel/arch/x86_64/include/asm/cpufeatures.h::x86_cap_bits]
```c
				Page_1GB				: 1,	/* 1-GByte page available */
```
固定time物理行91；定义范围42–500；事实键`cpu_1g`；支持字面`Page_1GB`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A14｜phase_flag
[VERIFIED mykernel/arch/x86_64/mm/init.c::after_bootmem]
```c
int after_bootmem;
```
固定time物理行226；定义范围226–226；事实键`phase_flag`；支持字面`after_bootmem`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A15｜promote_2m_gate
[VERIFIED mykernel/arch/x86_64/mm/init.c::adjust_range_page_size_mask]
```c
		if ((page_size_mask & (1<<PG_LEVEL_2M)) &&
			!(mr[i].page_size_mask & (1<<PG_LEVEL_2M))) {
			ulong start = round_down(mr[i].start, PMD_SIZE);
			ulong end = round_up(mr[i].end, PMD_SIZE);
```
固定time物理行309–312；定义范围304–326；事实键`promote_2m_gate`；支持字面`PG_LEVEL_2M`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A16｜promote_2m_write
[VERIFIED mykernel/arch/x86_64/mm/init.c::adjust_range_page_size_mask]
```c
			if (memblock_is_region_memory(start, end - start))
				mr[i].page_size_mask |= 1<<PG_LEVEL_2M;
```
固定time物理行314–315；定义范围304–326；事实键`promote_2m_write`；支持字面`memblock_is_region_memory`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A17｜promote_1g_gate
[VERIFIED mykernel/arch/x86_64/mm/init.c::adjust_range_page_size_mask]
```c
		if ((page_size_mask & (1<<PG_LEVEL_1G)) &&
			!(mr[i].page_size_mask & (1<<PG_LEVEL_1G))) {
			ulong start = round_down(mr[i].start, PUD_SIZE);
			ulong end = round_up(mr[i].end, PUD_SIZE);
```
固定time物理行317–320；定义范围304–326；事实键`promote_1g_gate`；支持字面`PG_LEVEL_1G`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A18｜promote_1g_write
[VERIFIED mykernel/arch/x86_64/mm/init.c::adjust_range_page_size_mask]
```c
			if (memblock_is_region_memory(start, end - start))
				mr[i].page_size_mask |= 1<<PG_LEVEL_1G;
```
固定time物理行322–323；定义范围304–326；事实键`promote_1g_write`；支持字面`memblock_is_region_memory`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A19｜single_search
[VERIFIED mykernel/mm/early/memblock.c::memblock_is_region_memory]
```c
bool __init_memblock
memblock_is_region_memory(phys_addr_t base, phys_addr_t size) {
	int idx = memblock_search(&memblock.memory, base);
	phys_addr_t end = base + memblock_cap_size(base, &size);
```
固定time物理行640–643；定义范围640–649；事实键`single_search`；支持字面`memblock_search`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A20｜single_containment
[VERIFIED mykernel/mm/early/memblock.c::memblock_is_region_memory]
```c
	if (idx == -1)
		return false;
	return (memblock.memory.regions[idx].base +
		 memblock.memory.regions[idx].size) >= end;
```
固定time物理行645–648；定义范围640–649；事实键`single_containment`；支持字面`regions[idx]`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A21｜search_enter
[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c
		int __init_memblock
		memblock_search(mmblk_type_s *type, phys_addr_t addr) {
			uint left = 0, right = type->cnt;

			do {
```
固定time物理行49–53；定义范围48–65；事实键`search_enter`；支持字面`do`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A22｜search_access
[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c

				if (addr < type->regions[mid].base)
					right = mid;
				else if (addr >= (type->regions[mid].base +
						type->regions[mid].size))
```
固定time物理行55–59；定义范围48–65；事实键`search_access`；支持字面`regions[mid]`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A23｜cap_size
[VERIFIED mykernel/mm/early/memblock.c::memblock_cap_size]
```c
static inline phys_addr_t
memblock_cap_size(phys_addr_t base, IN OUT phys_addr_t *size) {
	return *size = min(*size, PHYS_ADDR_MAX - base);
}
```
固定time物理行102–105；定义范围102–105；事实键`cap_size`；支持字面`PHYS_ADDR_MAX`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A24｜round_up
[VERIFIED mykernel/include/linux/kernel/math.h::round_up]
```c
	#define round_up(x, y)		((((x)-1) | __round_mask(x, y))+1)
```
固定time物理行24；定义范围24–24；事实键`round_up`；支持字面`__round_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A25｜round_down
[VERIFIED mykernel/include/linux/kernel/math.h::round_down]
```c
	#define round_down(x, y)	((x) & ~__round_mask(x, y))
```
固定time物理行34；定义范围34–34；事实键`round_down`；支持字面`__round_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A26｜promotion_phase
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	if (!after_bootmem)
		adjust_range_page_size_mask(mr, nr_range);
```
固定time物理行401–402；定义范围344–424；事实键`promotion_phase`；支持字面`after_bootmem`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A27｜display_preference
[VERIFIED mykernel/arch/x86_64/mm/init.c::page_size_string]
```c
	if (mr->page_size_mask & (1<<PG_LEVEL_1G))
		return str_1g;

	if (mr->page_size_mask & (1<<PG_LEVEL_2M))
		return str_2m;
```
固定time物理行335–339；定义范围328–342；事实键`display_preference`；支持字面`str_2m`。该段只支持局部源码事实，跨函数结论须合读先行门。

## A28｜display_4k
[VERIFIED mykernel/arch/x86_64/mm/init.c::page_size_string]
```c
	return str_4k;
```
固定time物理行341；定义范围328–342；事实键`display_4k`；支持字面`str_4k`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B01｜capacity
[VERIFIED mykernel/arch/x86_64/mm/init.c::NR_RANGE_MR]
```c
#define NR_RANGE_MR	5
```
固定time物理行15；定义范围15–15；事实键`capacity`；支持字面`5`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B02｜range_fields
[VERIFIED mykernel/arch/x86_64/mm/init.c::map_range]
```c
struct map_range {
	ulong	start;
	ulong	end;
	uint	page_size_mask;
};
```
固定time物理行228–232；定义范围228–232；事实键`range_fields`；支持字面`page_size_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B03｜save_args
[VERIFIED mykernel/arch/x86_64/mm/init.c::save_mr]
```c
static int __meminit
save_mr(struct map_range *mr,
	int nr_range, ulong start_pfn,
	ulong end_pfn, ulong page_size_mask) {

```
固定time物理行284–288；定义范围284–298；事实键`save_args`；支持字面`end_pfn`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B04｜save_guard
[VERIFIED mykernel/arch/x86_64/mm/init.c::save_mr]
```c
	if (start_pfn < end_pfn) {
		if (nr_range >= NR_RANGE_MR)
			panic("run out of range for init_memory_mapping\n");
		mr[nr_range].start = start_pfn<<PAGE_SHIFT;
		mr[nr_range].end   = end_pfn<<PAGE_SHIFT;
```
固定time物理行289–293；定义范围284–298；事实键`save_guard`；支持字面`NR_RANGE_MR`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B05｜save_count
[VERIFIED mykernel/arch/x86_64/mm/init.c::save_mr]
```c
		mr[nr_range].end   = end_pfn<<PAGE_SHIFT;
		mr[nr_range].page_size_mask = page_size_mask;
		nr_range++;
	}
	return nr_range;
```
固定time物理行293–297；定义范围284–298；事实键`save_count`；支持字面`nr_range++`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B06｜split_args
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
static int __meminit
split_mem_range(struct map_range *mr,
	int nr_range, ulong start, ulong end) {

	ulong start_pfn, end_pfn, limit_pfn, pfn;
```
固定time物理行344–348；定义范围344–424；事实键`split_args`；支持字面`start`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B07｜limit_floor
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	limit_pfn = PFN_DOWN(end);
```
固定time物理行351；定义范围344–424；事实键`limit_floor`；支持字面`PFN_DOWN`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B08｜head_bounds
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* head if not big page alignment ? */
	pfn = start_pfn = PFN_DOWN(start);
	end_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	if (end_pfn > limit_pfn)
		end_pfn = limit_pfn;
```
固定time物理行353–357；定义范围344–424；事实键`head_bounds`；支持字面`PFN_DOWN`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B09｜head_save
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	if (start_pfn < end_pfn) {
		nr_range = save_mr(mr, nr_range, start_pfn, end_pfn, 0);
		pfn = end_pfn;
	}
```
固定time物理行358–361；定义范围344–424；事实键`head_save`；支持字面`save_mr`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B10｜two_bounds
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* big page (2M) range */
	start_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	end_pfn = round_up(pfn, PFN_DOWN(PUD_SIZE));
	if (end_pfn > round_down(limit_pfn, PFN_DOWN(PMD_SIZE)))
		end_pfn = round_down(limit_pfn, PFN_DOWN(PMD_SIZE));
```
固定time物理行363–367；定义范围344–424；事实键`two_bounds`；支持字面`PUD_SIZE`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B11｜two_save
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	if (start_pfn < end_pfn) {
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & (1<<PG_LEVEL_2M));
		pfn = end_pfn;
```
固定time物理行369–373；定义范围344–424；事实键`two_save`；支持字面`PG_LEVEL_2M`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B12｜one_bounds
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* big page (1G) range */
	start_pfn = round_up(pfn, PFN_DOWN(PUD_SIZE));
	end_pfn = round_down(limit_pfn, PFN_DOWN(PUD_SIZE));
```
固定time物理行376–378；定义范围344–424；事实键`one_bounds`；支持字面`PUD_SIZE`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B13｜one_save
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	if (start_pfn < end_pfn) {
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & ((1<<PG_LEVEL_2M)|(1<<PG_LEVEL_1G)));
		pfn = end_pfn;
```
固定time物理行379–383；定义范围344–424；事实键`one_save`；支持字面`PG_LEVEL_1G`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B14｜tail2_bounds
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* tail is not big page (1G) alignment */
	start_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	end_pfn = round_down(limit_pfn, PFN_DOWN(PMD_SIZE));
	if (start_pfn < end_pfn) {
```
固定time物理行386–389；定义范围344–424；事实键`tail2_bounds`；支持字面`PMD_SIZE`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B15｜tail2_save
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & (1<<PG_LEVEL_2M));
		pfn = end_pfn;
	}
```
固定time物理行390–394；定义范围344–424；事实键`tail2_save`；支持字面`PG_LEVEL_2M`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B16｜tail4_save
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* tail is not big page (2M) alignment */
	start_pfn = pfn;
	end_pfn = limit_pfn;
	nr_range = save_mr(mr, nr_range, start_pfn, end_pfn, 0);
```
固定time物理行396–399；定义范围344–424；事实键`tail4_save`；支持字面`save_mr`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B17｜merge_guard
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	/* try to merge same page size and continuous */
	for (i = 0; nr_range > 1 && i < nr_range - 1; i++) {
		ulong old_start;
		if (mr[i].end != mr[i+1].start ||
			mr[i].page_size_mask != mr[i+1].page_size_mask)
```
固定time物理行404–408；定义范围344–424；事实键`merge_guard`；支持字面`mr[i+1]`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B18｜merge_move
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
			continue;
		/* move it */
		old_start = mr[i].start;
		memmove(&mr[i], &mr[i+1],
			(nr_range - 1 - i) * sizeof(struct map_range));
```
固定time物理行409–413；定义范围344–424；事实键`merge_move`；支持字面`memmove`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B19｜merge_count
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
		mr[i--].start = old_start;
		nr_range--;
	}
```
固定time物理行414–416；定义范围344–424；事实键`merge_count`；支持字面`nr_range--`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B20｜print_only
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	for (i = 0; i < nr_range; i++)
		pr_debug(" [mem %#010lx-%#010lx] page %s\n",
				mr[i].start, mr[i].end - 1,
				page_size_string(&mr[i]));
```
固定time物理行418–421；定义范围344–424；事实键`print_only`；支持字面`page_size_string`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B21｜split_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	return nr_range;
```
固定time物理行423；定义范围344–424；事实键`split_return`；支持字面`nr_range`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B22｜pfn_down
[VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_DOWN]
```c
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
```
固定time物理行8；定义范围8–8；事实键`pfn_down`；支持字面`PAGE_SHIFT`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B23｜shift_4k
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
固定time物理行9；定义范围9–9；事实键`shift_4k`；支持字面`12`。该段只支持局部源码事实，跨函数结论须合读先行门。

## B24｜size_4k
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定time物理行10；定义范围10–10；事实键`size_4k`；支持字面`PAGE_SHIFT`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C01｜local_state
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
ulong __ref
init_memory_mapping(ulong start, ulong end, pgprot_t prot) {
	struct map_range mr[NR_RANGE_MR];
	ulong ret = 0;
	int nr_range, i;
```
固定time物理行433–437；定义范围433–452；事实键`local_state`；支持字面`ret`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C02｜call_split
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
	pr_debug("init_memory_mapping: [mem %#010lx-%#010lx]\n",
			start, end - 1);

	memset(mr, 0, sizeof(mr));
	nr_range = split_mem_range(mr, 0, start, end);
```
固定time物理行439–443；定义范围433–452；事实键`call_split`；支持字面`split_mem_range`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C03｜each_request
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
	for (i = 0; i < nr_range; i++)
		ret = kernel_physical_mapping_init(mr[i].start,
				mr[i].end, mr[i].page_size_mask, prot);
```
固定time物理行445–447；定义范围433–452；事实键`each_request`；支持字面`kernel_physical_mapping_init`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C04｜last_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
	// add_pfn_range_mapped(start >> PAGE_SHIFT, ret >> PAGE_SHIFT);

	return ret >> PAGE_SHIFT;
```
固定time物理行449–451；定义范围433–452；事实键`last_return`；支持字面`return`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C05｜mapping_macro
[VERIFIED mykernel/arch/x86_64/mm/init.c::kernel_physical_mapping_init]
```c
#define kernel_physical_mapping_init(paddr_start,			\
				paddr_end, page_size_mask, prot)			\
			__kernel_physical_mapping_init(paddr_start,		\
					paddr_end, page_size_mask, prot, true)
```
固定time物理行27–30；定义范围27–30；事实键`mapping_macro`；支持字面`__kernel_physical_mapping_init`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C06｜lower_signature
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
static ulong __meminit
__kernel_physical_mapping_init(ulong paddr_start,
		ulong paddr_end, ulong page_size_mask,
		pgprot_t prot, bool init) {
```
固定time物理行823–826；定义范围823–864；事实键`lower_signature`；支持字面`page_size_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C07｜lower_initial
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	ulong vaddr, vaddr_start, vaddr_end, vaddr_next, paddr_last;

	paddr_last = paddr_end;
	vaddr = (ulong)__va(paddr_start);
	vaddr_end = (ulong)__va(paddr_end);
```
固定time物理行829–833；定义范围823–864；事实键`lower_initial`；支持字面`paddr_last`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C08｜lower_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		pgd_changed = true;
	}
	// if (pgd_changed)
	// 	sync_global_pgds(vaddr_start, vaddr_end - 1);
	return paddr_last;
```
固定time物理行859–863；定义范围823–864；事实键`lower_return`；支持字面`paddr_last`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C09｜range_initial
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_range_memory_mapping]
```c
static ulong __init
init_range_memory_mapping(ulong r_start, ulong r_end) {
	ulong start_pfn, end_pfn;
	ulong mapped_ram_size = 0;
	int i;
```
固定time物理行467–471；定义范围467–484；事实键`range_initial`；支持字面`mapped_ram_size`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C10｜range_clamp
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_range_memory_mapping]
```c
	for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
		u64 start = clamp_val(PFN_PHYS(start_pfn), r_start, r_end);
		u64 end = clamp_val(PFN_PHYS(end_pfn), r_start, r_end);
		if (start >= end)
			continue;
```
固定time物理行473–477；定义范围467–484；事实键`range_clamp`；支持字面`clamp_val`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C11｜range_account
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_range_memory_mapping]
```c
		init_memory_mapping(start, end, PAGE_KERNEL);
		mapped_ram_size += end - start;
```
固定time物理行479–480；定义范围467–484；事实键`range_account`；支持字面`mapped_ram_size`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C12｜range_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_range_memory_mapping]
```c
	return mapped_ram_size;
```
固定time物理行483；定义范围467–484；事实键`range_return`；支持字面`mapped_ram_size`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C13｜bottomup_alias
[VERIFIED mykernel/arch/x86_64/mm/init.c::memory_map_bottom_up]
```c
#define memory_map_bottom_up	init_range_memory_mapping
```
固定time物理行485；定义范围485–485；事实键`bottomup_alias`；支持字面`init_range_memory_mapping`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C14｜pfn_phys
[VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_PHYS]
```c
	#define PFN_PHYS(x)			((phys_addr_t)(x) << PAGE_SHIFT)
```
固定time物理行9；定义范围9–9；事实键`pfn_phys`；支持字面`phys_addr_t`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C15｜clamp_val
[VERIFIED mykernel/include/linux/kernel/minmax.h::clamp_val]
```c
	#define clamp_val(val, lo, hi)	clamp_t(typeof(val), val, lo, hi)
```
固定time物理行148；定义范围148–148；事实键`clamp_val`；支持字面`clamp_t`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C16｜clamp_t
[VERIFIED mykernel/include/linux/kernel/minmax.h::clamp_t]
```c
	#define clamp_t(type, val, lo, hi)	min_t(type, max_t(type, val, lo), hi)
```
固定time物理行135；定义范围135–135；事实键`clamp_t`；支持字面`max_t`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C17｜iterator_macro
[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_mem_pfn_range]
```c
	#define for_each_mem_pfn_range(i, p_start, p_end)					\
				for (i = -1, __next_mem_pfn_range(&i, p_start, p_end);	\
					i >= 0; __next_mem_pfn_range(&i, p_start, p_end))
```
固定time物理行64–66；定义范围64–66；事实键`iterator_macro`；支持字面`__next_mem_pfn_range`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C18｜iterator_entry
[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
__init_memblock void
__next_mem_pfn_range(OUT int *idx,
		OUT ulong *out_start_pfn,
		OUT ulong *out_end_pfn) {
```
固定time物理行680–683；定义范围680–701；事实键`iterator_entry`；支持字面`__next_mem_pfn_range`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C19｜iterator_scan
[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
	mmblk_type_s *type = &memblock.memory;
	mmblk_rgn_s *r;
	while (++*idx < type->cnt) {
		r = &type->regions[*idx];
		if (PFN_UP(r->base) < PFN_DOWN(r->base + r->size))
```
固定time物理行685–689；定义范围680–701；事实键`iterator_scan`；支持字面`PFN_UP`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C20｜iterator_stop
[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
	if (*idx >= type->cnt) {
		*idx = -1;
		return;
	}
```
固定time物理行692–695；定义范围680–701；事实键`iterator_stop`；支持字面`-1`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C21｜iterator_publish
[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
	if (out_start_pfn)
		*out_start_pfn = PFN_UP(r->base);
	if (out_end_pfn)
		*out_end_pfn = PFN_DOWN(r->base + r->size);
```
固定time物理行697–700；定义范围680–701；事实键`iterator_publish`；支持字面`out_end_pfn`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C22｜top_entry
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
void __init init_mem_mapping(void)
{
	ulong end;
```
固定time物理行487–489；定义范围487–532；事实键`top_entry`；支持字面`init_mem_mapping`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C23｜top_probe
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	// pti_check_boottime_disable();
	probe_page_size_mask();
	// setup_pcid();

	end = max_pfn << PAGE_SHIFT;
```
固定time物理行491–495；定义范围487–532；事实键`top_probe`；支持字面`probe_page_size_mask`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C24｜top_isa
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	/* the ISA range is always mapped regardless of memory holes */
	init_memory_mapping(0, ISA_END_ADDRESS, PAGE_KERNEL);
```
固定time物理行497–498；定义范围487–532；事实键`top_isa`；支持字面`init_memory_mapping`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C25｜direction_gate
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	if (memblock_bottom_up()) {
		ulong kernel_end = __pa_symbol(_end);
```
固定time物理行507–508；定义范围487–532；事实键`direction_gate`；支持字面`memblock_bottom_up`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C26｜two_calls
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
		memory_map_bottom_up(kernel_end, end);
		memory_map_bottom_up(ISA_END_ADDRESS, kernel_end);
	} else {
		// memory_map_top_down(ISA_END_ADDRESS, end);
```
固定time物理行517–520；定义范围487–532；事实键`two_calls`；支持字面`memory_map_bottom_up`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C27｜devices_and_low
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	myos_memory_map();

	if (max_pfn > max_low_pfn) {
		/* can we preserve max_low_pfn ?*/
		max_low_pfn = max_pfn;
```
固定time物理行522–526；定义范围487–532；事实键`devices_and_low`；支持字面`myos_memory_map`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C28｜load_root
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```
固定time物理行529–530；定义范围487–532；事实键`load_root`；支持字面`load_cr3`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C29｜bottomup_accessor
[VERIFIED mykernel/mm/early/memblock.h::memblock_bottom_up]
```c
		PREFIX_STATIC_INLINE
		__init_memblock bool
		memblock_bottom_up(void) {
			return memblock.bottom_up;
```
固定time物理行114–117；定义范围114–118；事实键`bottomup_accessor`；支持字面`memblock.bottom_up`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C30｜bottomup_initial
[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
	.bottom_up			= true,
	.current_limit		= (phys_addr_t)MEMBLOCK_ALLOC_ANYWHERE,
```
固定time物理行95–96；定义范围84–97；事实键`bottomup_initial`；支持字面`bottom_up`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C31｜isa_end
[VERIFIED mykernel/arch/x86_64/mm/early/e820_const.h::ISA_END_ADDRESS]
```c
	#define ISA_END_ADDRESS		0x00100000
```
固定time物理行51；定义范围51–51；事实键`isa_end`；支持字面`0x00100000`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C32｜pa_symbol
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__pa_symbol]
```c
	#define __pa_symbol(x) __phys_addr_symbol((ulong)(x))
```
固定time物理行22；定义范围22–22；事实键`pa_symbol`；支持字面`__phys_addr_symbol`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C33｜device_vbe
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_memory_map]
```c
static void __init
myos_memory_map() {
extern void myos_init_VBE_mapping(void);
	myos_init_VBE_mapping();
```
固定time物理行249–252；定义范围249–257；事实键`device_vbe`；支持字面`myos_init_VBE_mapping`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C34｜device_apic
[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_memory_map]
```c
extern void __init init_io_apic_mappings(void);
	init_io_apic_mappings();
```
固定time物理行255–256；定义范围249–257；事实键`device_apic`；支持字面`init_io_apic_mappings`。该段只支持局部源码事实，跨函数结论须合读先行门。

## C35｜max_pfn_upstream
[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	max_pfn = e820__end_of_ram_pfn();
	max_possible_pfn = max_pfn;
```
固定time物理行220–221；定义范围169–256；事实键`max_pfn_upstream`；支持字面`max_pfn`。该段只支持局部源码事实，跨函数结论须合读先行门。

## 解释上限

G01的page_size_mask候选位和memblock单记录包含不等于空闲或实际页大小；G02五段按PFN转换与数组计数组织，合并仅变更MR；G03下层映射请求、覆盖返回值和累计请求字节均不等于实际映射完成。R81-S01/S02/S03及上游79/80限定继续适用，本批无D00。

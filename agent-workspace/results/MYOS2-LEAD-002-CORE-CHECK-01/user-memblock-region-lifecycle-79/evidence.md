---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79
record_type: bounded_source_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
source_pin_short12: a039d9803ade
contract_pin_short12: ed8b3ac237f6
read_channel: connector
runtime_evidence: NOT_RUN
---

# 第79批｜D00与A/B/C固定源码短锚

以下95个标签逐条直接取自不可变time固定对象，同一定义内1–5行连续非空物理原文；不把说明文字、跨定义前提或旧78标签冒充本批新验证。出于静态分析边界，指针/容量/调用成功和实际构建条件均应见正文与facts再判断。

## D01｜旧78-A31的__va定义内替代引文，不混入条件指令

[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
固定time物理行25；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D02｜internal第一次调用与去掉min_addr的重试

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);
```
固定time物理行545–548；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D03｜internal在物理0时返回NULL

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	if (!alloc)
		return NULL;
```
固定time物理行550–551；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D04｜internal成功后才转换phys_to_virt

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	return (void *)phys_to_virt(alloc);
```
固定time物理行553；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D05｜先清zone边界数组

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
	/* Record where the zone boundaries are */
	memset(arch_zone_lowest_possible_pfn, 0,
			sizeof(arch_zone_lowest_possible_pfn));
	memset(arch_zone_highest_possible_pfn, 0,
			sizeof(arch_zone_highest_possible_pfn));
```
固定time物理行354–358；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D06｜后清pgdat，不等同于前置边界数组

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		pg_data_t *pgdat = NODE_DATA(0);
		memset(pgdat, 0, sizeof(pg_data_t));
```
固定time物理行370–371；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## D07｜zone统计首次写入生产者

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
			zone->zone_start_pfn = zone_start_pfn;
		else
			zone->zone_start_pfn = 0;
		zone->spanned_pages = spanned;
		zone->present_pages = real_size;
```
固定time物理行175–179；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D08｜后置zone present_pages赋零

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
		zone->name = zone_names[i];
		zone->zone_pgdat = NODE_DATA(0);
```
固定time物理行89–93；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D09｜mem_map分配先于core

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		alloc_node_mem_map(pgdat);
		free_area_init_core(pgdat);
	// }
```
固定time物理行381–383；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D10｜core之后请求memmap_init

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
	memmap_init(max_zone_pfn);
```
固定time物理行385；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D11｜另一无交集return没有初始化输出

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
	if (*zone_end_pfn < node_start_pfn || *zone_start_pfn > node_end_pfn)
		return 0;
```
固定time物理行128–129；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## D12｜未初始化pages声明

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
	ulong pages;
```
固定time物理行740；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## D13｜非空free区间先读pages自加

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
		pages += end_pfn - start_pfn;
```
固定time物理行757；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A01｜memory初始cnt0与容量

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
```
固定time物理行84–87；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A02｜reserved初始cnt0与容量

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
	.reserved.regions	= mmblk_reserved_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MMBLK_RESERVED_REGIONS,
	.reserved.name		= "reserved",
```
固定time物理行90–93；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A03｜cnt、max、total_size与regions的类型

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_type]
```c
		ulong			cnt;
		ulong			max;
		phys_addr_t		total_size;
		mmblk_rgn_s *	regions;
		char			*name;
```
固定time物理行35–39；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A04｜base、size与flags字段

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_region]
```c
	struct memblock_region {
		phys_addr_t			base;
		phys_addr_t			size;
		enum mmblk_flags	flags;
		// int nid;
```
固定time物理行19–23；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A05｜通用128初始容量

[VERIFIED mykernel/mm/early/memblock_const.h::INIT_MEMBLOCK_REGIONS]
```c
	#define INIT_MEMBLOCK_REGIONS			128
```
固定time物理行43；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A06｜reserved容量上限常量

[VERIFIED mykernel/mm/early/memblock_const.h::INIT_MMBLK_RESERVED_REGIONS]
```c
	#ifndef INIT_MMBLK_RESERVED_REGIONS
	#  define INIT_MMBLK_RESERVED_REGIONS	(2 * INIT_MEMBLOCK_REGIONS)
```
固定time物理行46–47；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A07｜memory容量上限常量

[VERIFIED mykernel/mm/early/memblock_const.h::INIT_MMBLK_MEMORY_REGIONS]
```c
	#ifndef INIT_MMBLK_MEMORY_REGIONS
	#  define INIT_MMBLK_MEMORY_REGIONS	INIT_MEMBLOCK_REGIONS
```
固定time物理行50–51；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A08｜base到PHYS_ADDR_MAX的截取

[VERIFIED mykernel/mm/early/memblock.c::memblock_cap_size]
```c
static inline phys_addr_t
memblock_cap_size(phys_addr_t base, IN OUT phys_addr_t *size) {
	return *size = min(*size, PHYS_ADDR_MAX - base);
}
```
固定time物理行102–105；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A09｜裁剪size后零长度早退

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	int idx, start_rgn, end_rgn;
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;
```
固定time物理行208–212；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A10｜memory注册入口

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add]
```c
__init_memblock int
simple_mmblk_add(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.memory, base, size, 0);
}
```
固定time物理行276–279；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A11｜reserved注册入口

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_reserve]
```c
__init_memblock int
simple_mmblk_reserve(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.reserved, base, size, 0);
}
```
固定time物理行281–284；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A12｜cnt加一和循环指针的真实宏

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_memblock_type]
```c
	#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)			\
				for (rgn_idx = 0, rgn_ptr = &(memblock_type)->regions[0];	\
					rgn_idx < (memblock_type)->cnt + 1;						\
					rgn_idx++, rgn_ptr = &(memblock_type)->regions[rgn_idx])
```
固定time物理行13–16；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A13｜首次idx0先形成rgn减一的指针

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	for_each_memblock_type(idx, type, rgn) {
		mmblk_rgn_s *prev = rgn - 1;
		if (idx > 0)
			BUG_ON(base < (prev->base + prev->size));
```
固定time物理行221–224；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A14｜读取当前槽和跳过旧区间判断

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		phys_addr_t rbase = rgn->base;
		phys_addr_t rend = rbase + rgn->size;
		size_t insert_base, insert_size;
		if ((rend <= base) && (idx != type->cnt))
			continue;
```
固定time物理行226–230；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A15｜尾槽或新段全在前的分支

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		if ((idx == type->cnt) || (rbase >= end)) {
		/* case1: Scan reach the start of empty records */
		/* case2: ner-region fully below rgn */
			insert_size = end - base;
			base = end;
```
固定time物理行236–240；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A16｜重叠分支执行rbase减base

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		else if (rbase < end) {
		/* case3: Head part of new-region may be cut by or merged to rgn */
			insert_size = rbase - base;
			base = rend;
		}
```
固定time物理行242–246；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A17｜另一个goto分支

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		else {
		/* case4: There is no part of new-region need to be inserted or merged */
			base = rend;
			goto no_update;
```
固定time物理行247–250；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A18｜插入后调用merge的索引

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		memblock_insert_region(type, idx, insert_base, insert_size, flags);
		simple_mmblk_merge_rgn(type, idx - 1, idx);
```
固定time物理行253–254；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A19｜结束时后置容量检查与返回

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		if (base >= end)	/* if @base meets @end, insertion is finished */
			break;
	}
	BUG_ON(type->cnt >= (type->max - 2));
	return ENOERR;
```
固定time物理行257–261；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A20｜最大物理地址宏

[VERIFIED mykernel/include/linux/kernel/limits.h::PHYS_ADDR_MAX]
```c
	#define PHYS_ADDR_MAX	(~(phys_addr_t)0)
```
固定time物理行9；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A21｜零错误码

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```
固定time物理行5；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A22｜标志位取值

[VERIFIED mykernel/mm/early/memblock_const.h::mmblk_flags]
```c
		MEMBLOCK_NONE			= 0x0,	/* No special request */
		MEMBLOCK_HOTPLUG		= 0x1,	/* hotpluggable region */
		MEMBLOCK_MIRROR			= 0x2,	/* mirrored region */
		MEMBLOCK_NOMAP			= 0x4,	/* don't add to kernel direct mapping */
		MEMBLOCK_DRIVER_MANAGED	= 0x8,	/* always detected via a driver */
```
固定time物理行23–27；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## A23｜reserved不是被筛选的memory类型

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	if (type != &memblock.memory)
		return false;
```
固定time物理行295–296；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B01｜先形成插入槽指针

[VERIFIED mykernel/mm/early/memblock.c::memblock_insert_region]
```c
	mmblk_rgn_s *rgn = &type->regions[idx];
```
固定time物理行176；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## B02｜memmove后仅写base和size

[VERIFIED mykernel/mm/early/memblock.c::memblock_insert_region]
```c
	BUG_ON(type->cnt >= type->max);
	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	type->cnt++;
```
固定time物理行178–182；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B03｜更新计数与total_size

[VERIFIED mykernel/mm/early/memblock.c::memblock_insert_region]
```c
	rgn->base = base;
	rgn->size = size;
	type->cnt++;
	type->total_size += size;
}
```
固定time物理行180–184；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B04｜start_rgn减一和cnt减一转换

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
	int i = 0;
	if (start_rgn)
		i = start_rgn - 1;
	end_rgn = min(end_rgn, type->cnt - 1);
	while (i < end_rgn) {
```
固定time物理行135–139；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B05｜循环先构造this与next指针

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
	while (i < end_rgn) {
		mmblk_rgn_s *this = &type->regions[i];
		mmblk_rgn_s *next = &type->regions[i + 1];
		phys_addr_t this_end = this->base + this->size;
```
固定time物理行139–142；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B06｜端点或flags不符时的门

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
		if (this_end != next->base || this->flags != next->flags) {
			BUG_ON(this_end > next->base);
			i++;
			continue;
		}
```
固定time物理行144–148；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B07｜合并先增左块再删除右块

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
		this->size += next->size;
		type->total_size += next->size;
		/* move forward from next + 1, index of which is i + 2 */
		simple_mmblk_remove_rgn(type, i + 1);
		end_rgn--;
```
固定time物理行150–154；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B08｜删除先扣total、向左移动后减cnt

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_remove_rgn]
```c
	type->total_size -= type->regions[rgn_idx].size;
	memmove(&type->regions[rgn_idx], &type->regions[rgn_idx + 1],
		(type->cnt - (rgn_idx + 1)) * sizeof(type->regions[rgn_idx]));
	type->cnt--;
```
固定time物理行111–114；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## B09｜memmove之后才cnt减一

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_remove_rgn]
```c
	type->total_size -= type->regions[rgn_idx].size;
	memmove(&type->regions[rgn_idx], &type->regions[rgn_idx + 1],
		(type->cnt - (rgn_idx + 1)) * sizeof(type->regions[rgn_idx]));
	type->cnt--;
```
固定time物理行111–114；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B10｜删除未清尾槽的源码注释

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_remove_rgn]
```c
	 * There is no need to clean array tail
	 * since we have @type->cnt
	 */
```
固定time物理行117–119；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B11｜重叠移动函数定义及方向

[VERIFIED mykernel/lib/string/string.c::memmove]
```c
	void *memmove(void *dest, const void *src, size_t count)
	{
		char *tmp;
		const char *s;
		if (dest <= src) {
```
固定time物理行719–723；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B12｜正向拷贝的请求

[VERIFIED mykernel/lib/string/string.c::memmove]
```c
			s = src;
			while (count--)
				*tmp++ = *s++;
		} else {
			tmp = dest;
```
固定time物理行725–729；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B13｜反向拷贝的请求

[VERIFIED mykernel/lib/string/string.c::memmove]
```c
			tmp += count;
			s = src;
			s += count;
			while (count--)
				*--tmp = *--s;
```
固定time物理行730–734；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B14｜容量检查放在循环后

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	BUG_ON(type->cnt >= (type->max - 2));
	return ENOERR;
```
固定time物理行260–261；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B15｜容量检查与memmove在形成rgn指针之后

[VERIFIED mykernel/mm/early/memblock.c::memblock_insert_region]
```c
	BUG_ON(type->cnt >= type->max);
	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
```
固定time物理行178–179；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## B16｜signed-int和ulong边界

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
	int i = 0;
	if (start_rgn)
		i = start_rgn - 1;
	end_rgn = min(end_rgn, type->cnt - 1);
```
固定time物理行135–138；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B17｜有效cnt和物理数组容量应分离

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_type]
```c
	struct memblock_type {
		ulong			cnt;
		ulong			max;
		phys_addr_t		total_size;
		mmblk_rgn_s *	regions;
```
固定time物理行34–38；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B18｜零长度不必经过插入

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
	int idx, start_rgn, end_rgn;
	phys_addr_t end = base + memblock_cap_size(base, &size);
	mmblk_rgn_s *rgn;
	if (!size)
		return ENOERR;
```
固定time物理行208–212；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B19｜计数加一尾槽同一根因

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_memblock_type]
```c
	#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)			\
				for (rgn_idx = 0, rgn_ptr = &(memblock_type)->regions[0];	\
					rgn_idx < (memblock_type)->cnt + 1;						\
					rgn_idx++, rgn_ptr = &(memblock_type)->regions[rgn_idx])
```
固定time物理行13–16；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B20｜启动初值未代表后续tail有效

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
```
固定time物理行84–87；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B21｜flags形参没有活动赋值

[VERIFIED mykernel/mm/early/memblock.c::memblock_insert_region]
```c
	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	type->cnt++;
	type->total_size += size;
```
固定time物理行179–183；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B22｜合并不是事务回滚

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_merge_rgn]
```c
		this->size += next->size;
		type->total_size += next->size;
		/* move forward from next + 1, index of which is i + 2 */
		simple_mmblk_remove_rgn(type, i + 1);
		end_rgn--;
```
固定time物理行150–154；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B23｜删除必须有cnt大于idx

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_remove_rgn]
```c
	type->total_size -= type->regions[rgn_idx].size;
	memmove(&type->regions[rgn_idx], &type->regions[rgn_idx + 1],
		(type->cnt - (rgn_idx + 1)) * sizeof(type->regions[rgn_idx]));
	type->cnt--;
```
固定time物理行111–114；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## B24｜idx为零时调用merge的负数实参

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_add_range]
```c
		memblock_insert_region(type, idx, insert_base, insert_size, flags);
		simple_mmblk_merge_rgn(type, idx - 1, idx);
```
固定time物理行253–254；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C01｜倒序宏cnt减一的边界

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_memory_region_topdown]
```c
	#define for_each_memory_region_topdown(region)			\
				for (region = &memblock.memory.regions[		\
								memblock.memory.cnt - 1];	\
					region >= memblock.memory.regions;		\
					region--)
```
固定time物理行82–86；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C02｜读取区间并向内对齐

[VERIFIED mykernel/mm/early/memblock.c::memblock_trim_memory]
```c
	for_each_memory_region_topdown(r) {
		orig_start = r->base;
		orig_end = r->base + r->size;
		start = round_up(orig_start, align);
		end = round_down(orig_end, align);
```
固定time物理行656–660；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C03｜保留裁齐后段但未更新total_size

[VERIFIED mykernel/mm/early/memblock.c::memblock_trim_memory]
```c
		if (start < end) {
			r->base = start;
			r->size = end - start;
```
固定time物理行665–667；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C04｜删除分支中显式r减一

[VERIFIED mykernel/mm/early/memblock.c::memblock_trim_memory]
```c
		} else {
			simple_mmblk_remove_rgn(&memblock.memory,
					r - memblock.memory.regions);
			r--;
		}
```
固定time物理行668–672；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C05｜仅2幂对齐前提下的上取整宏

[VERIFIED mykernel/include/linux/kernel/math.h::round_up]
```c
	#define round_up(x, y)		((((x)-1) | __round_mask(x, y))+1)
```
固定time物理行24；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C06｜仅2幂对齐前提下的下取整宏

[VERIFIED mykernel/include/linux/kernel/math.h::round_down]
```c
	#define round_down(x, y)	((x) & ~__round_mask(x, y))
```
固定time物理行34；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C07｜二分搜索以cnt作右界

[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c
		memblock_search(mmblk_type_s *type, phys_addr_t addr) {
			uint left = 0, right = type->cnt;
```
固定time物理行50–51；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C08｜do在cnt零时仍运行一次

[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c
			do {
				uint mid = (right + left) / 2;
```
固定time物理行53–54；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C09｜二分读取槽base和end

[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c
				if (addr < type->regions[mid].base)
					right = mid;
				else if (addr >= (type->regions[mid].base +
						type->regions[mid].size))
					left = mid + 1;
```
固定time物理行56–60；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C10｜命中立即返回否则负一

[VERIFIED mykernel/mm/early/memblock.h::memblock_search]
```c
				else
					return mid;
			} while (left < right);
			return -1;
		}
```
固定time物理行61–65；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C11｜reserved成员测试消费search结果

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_reserved]
```c
		memblock_is_reserved(phys_addr_t addr) {
			return memblock_search(&memblock.reserved, addr) != -1;
		}
```
固定time物理行69–71；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C12｜memory成员测试消费search结果

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_memory]
```c
		memblock_is_memory(phys_addr_t addr) {
			return memblock_search(&memblock.memory, addr) != -1;
		}
```
固定time物理行75–77；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C13｜NOMAP标志访问器

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_nomap]
```c
		bool
		memblock_is_nomap(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_NOMAP;
		}
```
固定time物理行89–92；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C14｜先拒绝未命中，再读NOMAP筛选

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_map_memory]
```c
			if (i == -1)
				return false;
			return !memblock_is_nomap(&memblock.memory.regions[i]);
```
固定time物理行103–105；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## C15｜基址搜索并上限裁size

[VERIFIED mykernel/mm/early/memblock.c::memblock_is_region_memory]
```c
memblock_is_region_memory(phys_addr_t base, phys_addr_t size) {
	int idx = memblock_search(&memblock.memory, base);
	phys_addr_t end = base + memblock_cap_size(base, &size);
```
固定time物理行641–643；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C16｜只用单区终点覆盖被裁区间

[VERIFIED mykernel/mm/early/memblock.c::memblock_is_region_memory]
```c
	if (idx == -1)
		return false;
	return (memblock.memory.regions[idx].base +
		 memblock.memory.regions[idx].size) >= end;
}
```
固定time物理行645–649；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C17｜筛掉HOTPLUG

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	if (!(flags & MEMBLOCK_HOTPLUG) && memblock_is_hotpluggable(m))
		return true;
```
固定time物理行299–300；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C18｜MIRROR特殊选择

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	if ((flags & MEMBLOCK_MIRROR) && !memblock_is_mirror(m))
		return true;
```
固定time物理行303–304；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C19｜NOMAP筛选

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	/* skip nomap memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_NOMAP) && memblock_is_nomap(m))
		return true;
```
固定time物理行306–308；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C20｜DRIVER_MANAGED筛选

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	/* skip driver-managed memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_DRIVER_MANAGED) && memblock_is_driver_managed(m))
		return true;
```
固定time物理行310–312；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C21｜memory减reserved的宏入口

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_free_mem_range]
```c
	#define for_each_free_mem_range(i, flags, p_start, p_end)			\
				__for_each_mem_range(i, &memblock.memory,				\
						&memblock.reserved, flags, p_start, p_end)
```
固定time物理行50–52；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C22｜双游标先读取memory槽与base

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
	for (int idx_a = *idx & 0xffffffff, idx_b = *idx >> 32;
		idx_a < within_type->cnt; idx_a++)
	{
		mmblk_rgn_s *m = &within_type->regions[idx_a];
		phys_addr_t m_start = m->base;
```
固定time物理行349–353；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。
## C23｜reserved尾槽只形成指针且分支读取

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
			r = &exclude_type->regions[idx_b];
			r_gap_start = (idx_b == 0) ? 0 : r[-1].base + r[-1].size;
			r_gap_end = (idx_b < exclude_type->cnt) ?
							r->base : (phys_addr_t)PHYS_ADDR_MAX;
```
固定time物理行367–370；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C24｜输出交集区间

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
				if (out_start)
					*out_start = max(m_start, r_gap_start);
				if (out_end)
					*out_end = min(m_end, r_gap_end);
```
固定time物理行381–384；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C25｜更新两个游标

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
				if (m_end <= r_gap_end)
					idx_a++;
				else
					idx_b++;
				*idx = (uint32_t)idx_a | (uint64_t)idx_b << 32;
```
固定time物理行389–393；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C26｜把free区间裁到调用参数内

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
	for_each_free_mem_range (i, flags, &this_start, &this_end) {
		this_start = clamp(this_start, start, end);
		this_end = clamp(this_end, start, end);
```
固定time物理行425–427；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C27｜对齐后容量判断

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
		cand = (phys_addr_t)round_up((size_t)this_start, (size_t)align);
		if (cand < this_end && this_end - cand >= size)
			return cand;
	}
```
固定time物理行429–432；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C28｜未找到返回物理0

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
	return 0;
```
固定time物理行434；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C29｜align零转SMP_CACHE_BYTES

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	enum mmblk_flags flags = MEMBLOCK_NONE;
	phys_addr_t found;
	if (align == 0)
		align = SMP_CACHE_BYTES;
```
固定time物理行468–471；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C30｜特殊end标记取current_limit

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	if (end == MEMBLOCK_ALLOC_ACCESSIBLE ||
		end == MEMBLOCK_ALLOC_NOLEAKTRACE)
		end = memblock.current_limit;
```
固定time物理行474–476；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C31｜PAGE_SIZE下界和上界钳制

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);
```
固定time物理行478–480；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C32｜候选找到且reserve成功才返回

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	found = __memblock_find_range_bottom_up(
				start, end, size, align, flags);
	if (found && !simple_mmblk_reserve(found, size))
		return found;
```
固定time物理行482–485；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C33｜失败返回物理零

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	else
		return 0;
```
固定time物理行486–487；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C34｜internal在第一次失败后可再试

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);
```
固定time物理行545–548；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

## C35｜先以物理零返回NULL；转换另见D04

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	if (!alloc)
		return NULL;
```
固定time物理行550–551；单一定义内连续原文。后续状态只在各条前置合法性条件满足且callee返回时成立；非运行证据。

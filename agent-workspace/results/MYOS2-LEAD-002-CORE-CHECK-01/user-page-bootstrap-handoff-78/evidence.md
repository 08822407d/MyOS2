# 第78批｜time固定源码短锚

新引文均由GitHub连接器在time@a039d9803ade取得，编号A/B/C各对应真实单一定义，原样摘录1–5个连续物理行。行号只是辅助；任何基于假设的数值、宏分支、调用顺序和存储合法性均在正文及facts独立说明。D00属于原77文档计数的追认纠正，不改写原77引文。本批未进行预处理、构建或内核运行，完整本地P9仍NOT_RUN。

## A01｜nodes分配与逐项NODE_DATA

[VERIFIED mykernel/mm/page_alloc/mmzone.c::zone_sizes_init]
```c
	pg_data_t *nodes = myos_memblock_alloc_normal(
			MAX_NUMNODES *sizeof(pg_data_t), SMP_CACHE_BYTES);
	for (int i = 0; i < MAX_NUMNODES; i++)
		NODE_DATA(i) = nodes + i;
```
固定time物理行5–8；须满足相应调用和对象前提。

## A02｜NODE_DATA的真实宏定义

[VERIFIED mykernel/arch/x86_64/mm/page/page_api_arch.h::NODE_DATA]
```c
	#define NODE_DATA(nid)		(node_data[nid])
```
固定time物理行16–16；须满足相应调用和对象前提。

## A03｜NODES_SHIFT的缺省零定义

[VERIFIED mykernel/include/linux/kernel/numa.h::NODES_SHIFT]
```c
	#	define NODES_SHIFT     0
```
固定time物理行10–10；须满足相应调用和对象前提。

## A04｜MAX_NUMNODES的shift来源

[VERIFIED mykernel/include/linux/kernel/numa.h::MAX_NUMNODES]
```c
	#define MAX_NUMNODES    (1 << NODES_SHIFT)
```
固定time物理行13–13；须满足相应调用和对象前提。

## A05｜对齐参数来自cache-line宏

[VERIFIED mykernel/include/linux/kernel/cache.h::SMP_CACHE_BYTES]
```c
	#  define SMP_CACHE_BYTES		L1_CACHE_BYTES
```
固定time物理行13–13；须满足相应调用和对象前提。

## A06｜上界PFN表清零及前两区赋值

[VERIFIED mykernel/mm/page_alloc/mmzone.c::zone_sizes_init]
```c
	unsigned long max_zone_pfns[MAX_NR_ZONES];
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
```
固定time物理行10–14；须满足相应调用和对象前提。

## A07｜DMA上界单位为PFN

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_DMA_PFN]
```c
	#define MAX_DMA_PFN			((16UL * 1024 * 1024) >> PAGE_SHIFT)
```
固定time物理行56–56；须满足相应调用和对象前提。

## A08｜DMA32上界PFN

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_DMA32_PFN]
```c
	#define MAX_DMA32_PFN		(1UL << (32 - PAGE_SHIFT))
```
固定time物理行59–59；须满足相应调用和对象前提。

## A09｜normal按页移位下界和成功清零门

[VERIFIED mykernel/mm/early/memblock.c::myos_memblock_alloc_normal]
```c
myos_memblock_alloc_normal(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);
```
固定time物理行607–611；须满足相应调用和对象前提。

## A10｜internal上界夹紧及首轮range调用

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	if (max_addr > memblock.current_limit)
		max_addr = memblock.current_limit;

	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
```
固定time物理行542–545；须满足相应调用和对象前提。

## A11｜物理0失败哨兵先检查再转换虚拟

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	if (!alloc)
		return NULL;

	return (void *)phys_to_virt(alloc);
```
固定time物理行550–553；须满足相应调用和对象前提。

## A12｜range接收physical size align和边界

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
memblock_alloc_range(phys_addr_t size, phys_addr_t align,
		phys_addr_t start, phys_addr_t end) {

	enum mmblk_flags flags = MEMBLOCK_NONE;
	phys_addr_t found;
```
固定time物理行465–469；须满足相应调用和对象前提。

## A13｜end哨兵替换为current_limit

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	/* pump up @end */
	if (end == MEMBLOCK_ALLOC_ACCESSIBLE ||
		end == MEMBLOCK_ALLOC_NOLEAKTRACE)
		end = memblock.current_limit;
```
固定time物理行473–476；须满足相应调用和对象前提。

## A14｜避开首个物理页并找空闲块

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	/* avoid allocating the first page */
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);

	found = __memblock_find_range_bottom_up(
```
固定time物理行478–482；须满足相应调用和对象前提。

## A15｜找到且reserve返回成功才返回地址

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	found = __memblock_find_range_bottom_up(
				start, end, size, align, flags);
	if (found && !simple_mmblk_reserve(found, size))
		return found;
```
固定time物理行482–485；须满足相应调用和对象前提。

## A16｜bottom_up寻找候选的初值和入口

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
__memblock_find_range_bottom_up(phys_addr_t start, phys_addr_t end,
		phys_addr_t size, phys_addr_t align, enum mmblk_flags flags) {

	phys_addr_t this_start, this_end, cand;
	uint64_t i = 0;
```
固定time物理行419–423；须满足相应调用和对象前提。

## A17｜区间夹紧与对齐候选

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
		this_start = clamp(this_start, start, end);
		this_end = clamp(this_end, start, end);

		cand = (phys_addr_t)round_up((size_t)this_start, (size_t)align);
		if (cand < this_end && this_end - cand >= size)
```
固定time物理行426–430；须满足相应调用和对象前提。

## A18｜实际reservation入口

[VERIFIED mykernel/mm/early/memblock.c::simple_mmblk_reserve]
```c
simple_mmblk_reserve(phys_addr_t base, phys_addr_t size) {
	return simple_mmblk_add_range(&memblock.reserved, base, size, 0);
```
固定time物理行282–283；须满足相应调用和对象前提。

## A19｜物理零哨兵的宏值

[VERIFIED mykernel/mm/early/memblock_const.h::MEMBLOCK_ALLOC_ACCESSIBLE]
```c
	#define MEMBLOCK_ALLOC_ACCESSIBLE		0
```
固定time物理行33–33；须满足相应调用和对象前提。

## A20｜memory静态数组与初始cnt

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
	.memory.name		= "memory",
```
固定time物理行84–88；须满足相应调用和对象前提。

## A21｜reserved静态数组与初始容量

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
	.reserved.regions	= mmblk_reserved_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MMBLK_RESERVED_REGIONS,
	.reserved.name		= "reserved",
```
固定time物理行90–93；须满足相应调用和对象前提。

## A22｜region物理地址尺寸flags字段

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_region]
```c
	struct memblock_region {
		phys_addr_t			base;
		phys_addr_t			size;
		enum mmblk_flags	flags;
		// int nid;
```
固定time物理行19–23；须满足相应调用和对象前提。

## A23｜type的cnt max及regions字段

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_type]
```c
	struct memblock_type {
		ulong			cnt;
		ulong			max;
		phys_addr_t		total_size;
		mmblk_rgn_s *	regions;
```
固定time物理行34–38；须满足相应调用和对象前提。

## A24｜pgdat清零先于PFN范围计算

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		pg_data_t *pgdat = NODE_DATA(0);
		memset(pgdat, 0, sizeof(pg_data_t));

		start_pfn = 0;
		end_pfn = 0;
```
固定time物理行370–374；须满足相应调用和对象前提。

## A25｜节点范围统计及map分配顺序

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		pgdat->node_start_pfn = start_pfn;
		calculate_node_totalpages(pgdat, start_pfn, end_pfn);

		alloc_node_mem_map(pgdat);
		free_area_init_core(pgdat);
```
固定time物理行378–382；须满足相应调用和对象前提。

## A26｜聚合memblock区间最小最大PFN

[VERIFIED mykernel/mm/page_alloc/buddy.c::get_pfn_range]
```c
	*start_pfn = -1UL;
	*end_pfn = 0;

	for_each_mem_pfn_range(i, &this_start_pfn, &this_end_pfn) {
		*start_pfn = min(*start_pfn, this_start_pfn);
```
固定time物理行592–596；须满足相应调用和对象前提。

## A27｜空节点早退及起点下对齐

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
	/* Skip empty nodes */
	if (!pgdat->node_spanned_pages)
		return;

	start = pgdat->node_start_pfn & ~(MAX_ORDER_NR_PAGES - 1);
```
固定time物理行292–296；须满足相应调用和对象前提。

## A28｜memmap对齐步长页数

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::MAX_ORDER_NR_PAGES]
```c
	#define MAX_ORDER_NR_PAGES	(1 << MAX_ORDER)
```
固定time物理行85–85；须满足相应调用和对象前提。

## A29｜上对齐end及sizeof长度

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		end = pgdat->node_start_pfn + pgdat->node_spanned_pages;
		end = ALIGN(end, MAX_ORDER_NR_PAGES);
		size =  (end - start) * sizeof(page_s);

		map = (void *)phys_to_virt(memblock_alloc_range(size,
```
固定time物理行307–311；须满足相应调用和对象前提。

## A30｜物理转虚拟近端定义

[VERIFIED mykernel/arch/x86_64/insns/io_arch.h::phys_to_virt]
```c
		void
		*phys_to_virt(volatile phys_addr_t address) {
			return __va(address);
```
固定time物理行57–59；须满足相应调用和对象前提。

## A31｜__va按PAGE_OFFSET计算

[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#ifndef __va
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
固定time物理行24–25；须满足相应调用和对象前提。

## A32｜PAGE_OFFSET直接宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_OFFSET]
```c
	#define PAGE_OFFSET				((ulong)__PAGE_OFFSET)
```
固定time物理行39–39；须满足相应调用和对象前提。

## A33｜内核虚拟高基址非零常量

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::__PAGE_OFFSET_BASE_L4]
```c
	#define __PAGE_OFFSET_BASE_L4	_AC(0xffff800000000000, UL)
```
固定time物理行75–75；须满足相应调用和对象前提。

## A34｜物理候选先转换后map非空检查

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		map = (void *)phys_to_virt(memblock_alloc_range(size,
							SMP_CACHE_BYTES, MAX_DMA_PFN, 0));
		if (!map) {
			while (1);
```
固定time物理行311–314；须满足相应调用和对象前提。

## A35｜map清零和map+offset发布

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		memset(map, 0, size);
		pgdat->node_mem_map = mem_map = map + offset;
```
固定time物理行319–320；须满足相应调用和对象前提。

## A36｜pfn_to_page按绝对PFN加全局mem_map

[VERIFIED mykernel/mm/highmem/highmem_macro.h::pfn_to_page]
```c
	#define pfn_to_page(pfn)	((pfn) + mem_map)
```
固定time物理行12–12；仅合法对象条件下消费。

## A37｜局部node_mem_map的i递增

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
	for (int i = 0; i < NODE_DATA(0)->node_spanned_pages; i++) {
		page_s *page = &(NODE_DATA(0)->node_mem_map[i]);
		INIT_LIST_S(&page->lru);
		INIT_LIST_S(&page->buddy_list);
```
固定time物理行327–330；须满足相应调用和对象前提。

## A38｜page到全局PFN的逆向差值

[VERIFIED mykernel/mm/highmem/highmem_macro.h::page_to_pfn]
```c
	#define page_to_pfn(page)	((ulong)((page) - mem_map))
```
固定time物理行11–11；须满足相应调用和对象前提。

## A39｜描述符长度由end-start乘sizeof

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		end = pgdat->node_start_pfn + pgdat->node_spanned_pages;
		end = ALIGN(end, MAX_ORDER_NR_PAGES);
		size =  (end - start) * sizeof(page_s);
```
固定time物理行307–309；须满足相应调用和对象前提。

## A40｜normal的有效ptr清零门

[VERIFIED mykernel/mm/early/memblock.c::myos_memblock_alloc_normal]
```c
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);
```
固定time物理行608–611；须满足相应调用和对象前提。

## A41｜以原始MAX_DMA_PFN作为物理下界

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		map = (void *)phys_to_virt(memblock_alloc_range(size,
							SMP_CACHE_BYTES, MAX_DMA_PFN, 0));
```
固定time物理行311–312；须满足相应调用和对象前提。

## A42｜内层可用零低界

[VERIFIED mykernel/mm/early/memblock_const.h::MEMBLOCK_LOW_LIMIT]
```c
	#define MEMBLOCK_LOW_LIMIT				0
```
固定time物理行37–37；须满足相应调用和对象前提。

## A43｜节点统计与node_mem_map是不同字段

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::pglist_data]
```c
		int			nr_zones;	  /* number of populated zones in this node */
		// page_s *node_mem_map;
		page_s		*node_mem_map;
```
固定time物理行165–167；须满足相应调用和对象前提。

## B01｜三种zone的最高PFN上界生产

[VERIFIED mykernel/mm/page_alloc/mmzone.c::zone_sizes_init]
```c
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;
```
固定time物理行11–15；仅在输入和调用有效时使用。

## B02｜e820 memblock输入阶段调用

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	// 用e820获取的内存分布初始化memblock分配器
	e820__memblock_setup();
```
固定time物理行244–245；仅在输入和调用有效时使用。

## B03｜setup_arch末尾请求zone_sizes_init

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c

	// x86_init.paging.pagetable_init()
	// void __init paging_init(void)
	// {
		zone_sizes_init();
```
固定time物理行250–254；仅在输入和调用有效时使用。

## B04｜五区低高PFN数组清零

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
	/* Record where the zone boundaries are */
	memset(arch_zone_lowest_possible_pfn, 0,
			sizeof(arch_zone_lowest_possible_pfn));
	memset(arch_zone_highest_possible_pfn, 0,
			sizeof(arch_zone_highest_possible_pfn));
```
固定time物理行354–358；仅在输入和调用有效时使用。

## B05｜pgdat先清零再计算范围

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		pg_data_t *pgdat = NODE_DATA(0);
		memset(pgdat, 0, sizeof(pg_data_t));

		start_pfn = 0;
		end_pfn = 0;
```
固定time物理行370–374；仅在输入和调用有效时使用。

## B06｜空节点条件提前return而未写输出

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
	ulong zone_low = arch_zone_lowest_possible_pfn[zone_type];
	ulong zone_high = arch_zone_highest_possible_pfn[zone_type];
	/* When hotadd a new node from cpu_up(), the node should be empty */
	if (!node_start_pfn && !node_end_pfn)
		return 0;
```
固定time物理行117–121；仅在输入和调用有效时使用。

## B07｜zone边界clamp与交集检查

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
	*zone_start_pfn = clamp(node_start_pfn, zone_low, zone_high);
	*zone_end_pfn = clamp(node_end_pfn, zone_low, zone_high);

	/* Check that this node has pages within the zone's required range */
	if (*zone_end_pfn < node_start_pfn || *zone_start_pfn > node_end_pfn)
```
固定time物理行124–128；仅在输入和调用有效时使用。

## B08｜缺页计数计算与内存PFN迭代

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
		*nr_absent = *zone_end_pfn - *zone_start_pfn;
		ulong start_pfn, end_pfn;
		int i;

		for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
```
固定time物理行135–139；仅在输入和调用有效时使用。

## B09｜逐区域扣除实际PFN覆盖数

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
		for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
			start_pfn = clamp(start_pfn, *zone_start_pfn, *zone_end_pfn);
			end_pfn = clamp(end_pfn, *zone_start_pfn, *zone_end_pfn);
			*nr_absent -= end_pfn - start_pfn;
		}
```
固定time物理行139–143；仅在输入和调用有效时使用。

## B10｜最终zone跨度和端点写入

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
	/* count spanned pages */
	*zone_end_pfn = min(*zone_end_pfn, node_end_pfn);
	*zone_start_pfn = max(*zone_start_pfn, node_start_pfn);
	*nr_spanned = *zone_end_pfn - *zone_start_pfn;
	return *nr_spanned;
```
固定time物理行146–150；仅在输入和调用有效时使用。

## B11｜spanned absent未预初始化的本地变量

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
	for (i = 0; i < MAX_NR_ZONES; i++) {
		zone_s *zone = pgdat->node_zones + i;
		zone->zone_pgdat = pgdat;
		ulong zone_start_pfn, zone_end_pfn;
		ulong spanned, absent;
```
固定time物理行160–164；仅在输入和调用有效时使用。

## B12｜下层返回之后直接读取span absent

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
				node_start_pfn, node_end_pfn,
				&zone_start_pfn, &zone_end_pfn,
				&absent, &spanned);

		real_size = spanned - absent;
```
固定time物理行168–172；仅在输入和调用有效时使用。

## B13｜先写zone start spanned present

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
			zone->zone_start_pfn = zone_start_pfn;
		else
			zone->zone_start_pfn = 0;
		zone->spanned_pages = spanned;
		zone->present_pages = real_size;
```
固定time物理行175–179；仅在输入和调用有效时使用。

## B14｜node nr_zones和跨度汇总

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
	pgdat->nr_zones = MAX_NR_ZONES;
	pgdat->node_spanned_pages = totalpages;
	pgdat->node_present_pages = realtotalpages;
```
固定time物理行185–187；仅在输入和调用有效时使用。

## B15｜zone.lock初始化语句是注释

[VERIFIED mykernel/mm/misc/mm_init.c::zone_init_internals]
```c
	// atomic_long_set(&zone->managed_pages, remaining_pages);
	zone->name = zone_names[idx];
	zone->zone_pgdat = NODE_DATA(0);
	// spin_lock_init(&zone->lock);
```
固定time物理行215–218；仅在输入和调用有效时使用。

## B16｜core使用先前span present

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init_core]
```c
		zone_s *zone = pgdat->node_zones + j;
		ulong size, freesize, memmap_pages;

		size = zone->spanned_pages;
		freesize = zone->present_pages;
```
固定time物理行240–244；仅在输入和调用有效时使用。

## B17｜zone_init_internals调用和空区跳过

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init_core]
```c
		zone_init_internals(zone, j, freesize);

		if (!size)
			continue;
```
固定time物理行276–279；仅在输入和调用有效时使用。

## B18｜memmap从PFN1启动跨五区

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
memmap_init(ulong *max_zone_pfn) {
	ulong start_pfn, end_pfn;
	start_pfn = 1;

	for (int i = 0; i < MAX_NR_ZONES; i++) {
```
固定time物理行81–85；仅在输入和调用有效时使用。

## B19｜后续覆写zone present为0

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
		zone->name = zone_names[i];
		zone->zone_pgdat = NODE_DATA(0);
```
固定time物理行89–93；仅在输入和调用有效时使用。

## B20｜各阶free_area链初始化并取page

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (int j = 0; j < NR_PAGE_ORDERS; j++)
			INIT_LIST_HEADER_S(&zone->free_area[j]);

		for (ulong pfn = start_pfn; pfn < end_pfn; pfn++) {
			page_s *page = pfn_to_page(pfn);
```
固定time物理行98–102；仅在输入和调用有效时使用。

## B21｜每页refcount初始化和mapcount注释

[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
__init_single_page(page_s *page, ulong pfn, ulong zone) {
	// mm_zero_struct_page(page);
	// set_page_links(page, zone, nid, pfn);
	init_page_count(page);
	// page_mapcount_reset(page);
```
固定time物理行20–24；仅在输入和调用有效时使用。

## B22｜每页lru与buddy_list的初始化

[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
	INIT_LIST_S(&page->lru);
	INIT_LIST_S(&page->buddy_list);
```
固定time物理行28–29；仅在输入和调用有效时使用。

## B23｜page flags和flag_defs union

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
	struct page {
		union {	// 为了方便debug，增加了按位定义的union
			ulong				flags;		/* Atomic flags, some possibly */
			pgflag_defs_s		flag_defs;	/* updated asynchronously */
		};
```
固定time物理行45–49；仅在输入和调用有效时使用。

## B24｜page_type真实存储字段

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
			uint				page_type;
```
固定time物理行153–153；仅在输入和调用有效时使用。

## B25｜mapcount与page_type同一union

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
			 */
			atomic_t			_mapcount;
		};
```
固定time物理行163–165；仅在输入和调用有效时使用。

## B26｜_refcount单独成员

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
		/* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
		atomic_t				_refcount;
```
固定time物理行167–168；仅在输入和调用有效时使用。

## B27｜计数初值为1

[VERIFIED mykernel/mm/page_alloc/buddy.h::init_page_count]
```c
		init_page_count(page_s *page) {
			set_page_count(page, 1);
		}
```
固定time物理行238–240；仅在输入和调用有效时使用。

## B28｜pfn_valid只检查上界

[VERIFIED mykernel/include/linux/kernel/asm-generic/page.h::pfn_valid]
```c
	#	define pfn_valid(pfn)		((pfn) < max_pfn)
```
固定time物理行93–93；仅在输入和调用有效时使用。

## B29｜保留物理字节区间转PFN和上界门

[VERIFIED mykernel/mm/misc/mm_init.c::reserve_bootmem_region]
```c
	ulong start_pfn = PFN_DOWN(start);
	ulong end_pfn = PFN_UP(end);

	for (; start_pfn < end_pfn; start_pfn++) {
		if (pfn_valid(start_pfn)) {
```
固定time物理行41–45；仅在输入和调用有效时使用。

## B30｜保留页重新初始化元数据

[VERIFIED mykernel/mm/misc/mm_init.c::reserve_bootmem_region]
```c
				__init_single_page(page, start_pfn, 0);
```
固定time物理行66–66；仅在输入和调用有效时使用。

## B31｜保留页设置PG_reserved

[VERIFIED mykernel/mm/misc/mm_init.c::reserve_bootmem_region]
```c
			__SetPageReserved(page);
```
固定time物理行74–74；仅在输入和调用有效时使用。

## B32｜Reserved真实位写入

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__SetPageReserved]
```c
		__SetPageReserved(page_s *page) {
			arch___set_bit(PG_reserved, &(page)->flags);
```
固定time物理行353–354；仅在输入和调用有效时使用。

## B33｜后继移交会清Reserved

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__ClearPageReserved]
```c
		__ClearPageReserved(page_s *page) {
			arch___clear_bit(PG_reserved, &(page)->flags);
```
固定time物理行343–344；仅在输入和调用有效时使用。

## B34｜memory PFN迭代宏

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_mem_pfn_range]
```c
	#define for_each_mem_pfn_range(i, p_start, p_end)					\
				for (i = -1, __next_mem_pfn_range(&i, p_start, p_end);	\
					i >= 0; __next_mem_pfn_range(&i, p_start, p_end))
```
固定time物理行64–66；仅在输入和调用有效时使用。

## B35｜memory只取有完整页的区域

[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
	mmblk_rgn_s *r;
	while (++*idx < type->cnt) {
		r = &type->regions[*idx];
		if (PFN_UP(r->base) < PFN_DOWN(r->base + r->size))
			break;;
```
固定time物理行686–690；仅在输入和调用有效时使用。

## B36｜到尾部写负索引结束

[VERIFIED mykernel/mm/early/memblock.c::__next_mem_pfn_range]
```c
	if (*idx >= type->cnt) {
		*idx = -1;
		return;
	}

```
固定time物理行692–696；仅在输入和调用有效时使用。

## B37｜zone_start_pfn字段

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
		ulong			zone_start_pfn;
```
固定time物理行43–44；仅在输入和调用有效时使用。

## B38｜managed span present三个区统计

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		atomic_long_t	managed_pages;
		ulong			spanned_pages;
		ulong			present_pages;
```
固定time物理行88–90；仅在输入和调用有效时使用。

## B39｜node nr_zones与mem_map字段

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::pglist_data]
```c
		int			nr_zones;	  /* number of populated zones in this node */
		// page_s *node_mem_map;
		page_s		*node_mem_map;
```
固定time物理行165–167；仅在输入和调用有效时使用。

## B40｜内核入口先setup_arch

[VERIFIED mykernel/init/main.c::start_kernel]
```c
	setup_arch(NULL);

	setup_per_cpu_areas();
```
固定time物理行138–140；仅在输入和调用有效时使用。

## B41｜后再mm_core_init

[VERIFIED mykernel/init/main.c::start_kernel]
```c
	trap_init();
	mm_core_init();
```
固定time物理行143–144；仅在输入和调用有效时使用。

## B42｜mem_init在kmem_cache_init前

[VERIFIED mykernel/mm/misc/mm_init.c::mm_core_init]
```c
	mem_init();
	// mem_init_print_info();
	kmem_cache_init();
```
固定time物理行409–411；仅在输入和调用有效时使用。

## B43｜after_bootmem先置1再请求释放

[VERIFIED mykernel/arch/x86_64/mm/init.c::mem_init]
```c
	after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	/* this will put all memory onto the freelists */
	memblock_free_all();
```
固定time物理行895–899；仅在输入和调用有效时使用。

## B44｜alloc_node_mem_map与core后memmap_init

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		alloc_node_mem_map(pgdat);
		free_area_init_core(pgdat);
	// }

	memmap_init(max_zone_pfn);
```
固定time物理行381–385；仅在输入和调用有效时使用。

## C01｜memblock全局memory表初始化及cnt

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
memblock_s memblock __initdata_memblock = {
	.memory.regions		= mmblk_memory_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MMBLK_MEMORY_REGIONS,
	.memory.name		= "memory",
```
固定time物理行84–88；只证明该对象定义本身。

## C02｜memory和reserved两套集合类型

[VERIFIED mykernel/mm/early/memblock_types.h::memblock]
```c
	struct memblock {
		bool			bottom_up;  /* is bottom up direction? */
		phys_addr_t		current_limit;
		mmblk_type_s	memory;
		mmblk_type_s	reserved;
```
固定time物理行49–53，要求合法输入和先行操作返回。

## C03｜region的物理地址尺寸与flags

[VERIFIED mykernel/mm/early/memblock_types.h::memblock_region]
```c
	struct memblock_region {
		phys_addr_t			base;
		phys_addr_t			size;
		enum mmblk_flags	flags;
		// int nid;
```
固定time物理行19–23，要求合法输入和先行操作返回。

## C04｜reserved枚举采用cnt+1上界

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_memblock_type]
```c
	#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)			\
				for (rgn_idx = 0, rgn_ptr = &(memblock_type)->regions[0];	\
					rgn_idx < (memblock_type)->cnt + 1;						\
					rgn_idx++, rgn_ptr = &(memblock_type)->regions[rgn_idx])
```
固定time物理行13–16，要求合法输入和先行操作返回。

## C05｜预留区逐区域传入reserve_bootmem_region

[VERIFIED mykernel/mm/early/memblock.c::memmap_init_reserved_pages]
```c
	/* initialize struct pages for the reserved regions */
	for_each_memblock_type(i, &memblock.reserved, region) {
		start = region->base;
		end = start + region->size;
		reserve_bootmem_region(region->base, region->base + region->size);
```
固定time物理行727–731，要求合法输入和先行操作返回。

## C06｜保留区间向外扩到整页并测试pfn_valid

[VERIFIED mykernel/mm/misc/mm_init.c::reserve_bootmem_region]
```c
	ulong start_pfn = PFN_DOWN(start);
	ulong end_pfn = PFN_UP(end);

	for (; start_pfn < end_pfn; start_pfn++) {
		if (pfn_valid(start_pfn)) {
```
固定time物理行41–45，要求合法输入和先行操作返回。

## C07｜PG_reserved设置的实际callee

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__SetPageReserved]
```c
		__SetPageReserved(page_s *page) {
			arch___set_bit(PG_reserved, &(page)->flags);
```
固定time物理行353–354，要求合法输入和先行操作返回。

## C08｜非memory类型无条件跳过过滤

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	/* we never skip regions when iterating memblock.reserved or physmem */
	if (type != &memblock.memory)
		return false;

	/* skip hotpluggable memory regions if needed */
```
固定time物理行294–298，要求合法输入和先行操作返回。

## C09｜MIRROR与NOMAP条件检测

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	if ((flags & MEMBLOCK_MIRROR) && !memblock_is_mirror(m))
		return true;

	/* skip nomap memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_NOMAP) && memblock_is_nomap(m))
```
固定time物理行303–307，要求合法输入和先行操作返回。

## C10｜DRIVER_MANAGED条件和默认不跳过

[VERIFIED mykernel/mm/early/memblock.c::should_skip_region]
```c
	/* skip driver-managed memory unless we were asked for it explicitly */
	if (!(flags & MEMBLOCK_DRIVER_MANAGED) && memblock_is_driver_managed(m))
		return true;

	return false;
```
固定time物理行310–314，要求合法输入和先行操作返回。

## C11｜NOMAP位访问来源

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_nomap]
```c
		bool
		memblock_is_nomap(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_NOMAP;
		}
```
固定time物理行89–92，要求合法输入和先行操作返回。

## C12｜memory减reserved才构成free迭代

[VERIFIED mykernel/mm/early/memblock_macro.h::for_each_free_mem_range]
```c
	#define for_each_free_mem_range(i, flags, p_start, p_end)			\
				__for_each_mem_range(i, &memblock.memory,				\
						&memblock.reserved, flags, p_start, p_end)
```
固定time物理行50–52，要求合法输入和先行操作返回。

## C13｜起始游标0并首次调用next

[VERIFIED mykernel/mm/early/memblock_macro.h::__for_each_mem_range]
```c
	#define __for_each_mem_range(i, within_type, exclude_type,			\
					flags, p_start, p_end)								\
				for (i = 0, __simple_next_mem_range(&i, flags,			\
					within_type, exclude_type, p_start, p_end);			\
```
固定time物理行30–33，要求合法输入和先行操作返回。

## C14｜当且仅当ULLONG_MAX时停止

[VERIFIED mykernel/mm/early/memblock_macro.h::__for_each_mem_range]
```c
					i != (uint64_t)ULLONG_MAX;							\
					__simple_next_mem_range(&i, flags, within_type,		\
							exclude_type, p_start, p_end))
```
固定time物理行34–36，要求合法输入和先行操作返回。

## C15｜游标分别取低高32位

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
	BUG_ON(within_type == NULL || exclude_type == NULL);

	for (int idx_a = *idx & 0xffffffff, idx_b = *idx >> 32;
		idx_a < within_type->cnt; idx_a++)
```
固定time物理行347–350，要求合法输入和先行操作返回。

## C16｜memory区域地址与skip逻辑

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
	{
		mmblk_rgn_s *m = &within_type->regions[idx_a];
		phys_addr_t m_start = m->base;
		phys_addr_t m_end = m->base + m->size;
		if (should_skip_region(within_type, m, flags))
```
固定time物理行351–355，要求合法输入和先行操作返回。

## C17｜reserved遍历计数cnt+1及尾槽指针

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
		for (; idx_b < exclude_type->cnt + 1; idx_b++)
		{
			mmblk_rgn_s *r;
			// @r_gap_start and @r_gap_end is the start and end of
			// range which between two regions of type reserved.
```
固定time物理行359–363，要求合法输入和先行操作返回。

## C18｜预留区各gap物理端点

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
			r = &exclude_type->regions[idx_b];
			r_gap_start = (idx_b == 0) ? 0 : r[-1].base + r[-1].size;
			r_gap_end = (idx_b < exclude_type->cnt) ?
							r->base : (phys_addr_t)PHYS_ADDR_MAX;
```
固定time物理行367–370，要求合法输入和先行操作返回。

## C19｜两个半开范围交集判断

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
			if (m_end <= r_gap_start)
				break;
			// here @m_end > @r_gap_start
			/* if the two regions intersect, we're done */
			if (m_start < r_gap_end)
```
固定time物理行375–379，要求合法输入和先行操作返回。

## C20｜输出max起点与min终点

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
			{
				if (out_start)
					*out_start = max(m_start, r_gap_start);
				if (out_end)
					*out_end = min(m_end, r_gap_end);
```
固定time物理行380–384，要求合法输入和先行操作返回。

## C21｜输出一段后推进双游标

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
					idx_a++;
				else
					idx_b++;
				*idx = (uint32_t)idx_a | (uint64_t)idx_b << 32;
				return;
```
固定time物理行390–394，要求合法输入和先行操作返回。

## C22｜结束时写入ULLONG_MAX

[VERIFIED mykernel/mm/early/memblock.c::__simple_next_mem_range]
```c
	/* signal end of iteration */
	*idx = ULLONG_MAX;
```
固定time物理行399–400，要求合法输入和先行操作返回。

## C23｜空闲范围请求无额外flags

[VERIFIED mykernel/mm/early/memblock_const.h::MEMBLOCK_NONE]
```c
		MEMBLOCK_NONE			= 0x0,	/* No special request */
```
固定time物理行23–23，要求合法输入和先行操作返回。

## C24｜局部累计量pages在声明处未初始化

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
memblock_free_all(void) {
	ulong pages;
	uint64_t i = 0;
	phys_addr_t start, end;

```
固定time物理行739–743，要求合法输入和先行操作返回。

## C25｜遍历空闲区间并做PFN裁剪

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
	for_each_free_mem_range (i, MEMBLOCK_NONE, &start, &end) {
		ulong start_pfn = PFN_UP(start);
		ulong end_pfn = min_t(ulong, PFN_DOWN(end), max_low_pfn);
		if (start_pfn >= end_pfn)
			continue;
```
固定time物理行751–755，要求合法输入和先行操作返回。

## C26｜pages进行自增累加读取旧值

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
		pages += end_pfn - start_pfn;
```
固定time物理行757–757，要求合法输入和先行操作返回。

## C27｜用__ffs选阶并检查末尾收缩

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
		while (start_pfn < end_pfn) {
			int order = min_t(int, MAX_ORDER, __ffs(start_pfn));
			while (start_pfn + (1UL << order) > end_pfn)
				order--;
			memblock_free_pages(pfn_to_page(start_pfn), start_pfn, order);
```
固定time物理行760–764，要求合法输入和先行操作返回。

## C28｜移交一个块并前进PFN

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
			memblock_free_pages(pfn_to_page(start_pfn), start_pfn, order);
			start_pfn += (1UL << order);
```
固定time物理行764–765，要求合法输入和先行操作返回。

## C29｜ffs实现含位扫描无本体零值分支

[VERIFIED mykernel/arch/x86_64/insns/bitops_arch.h::__ffs]
```c
		__ffs(unsigned long word) {
			asm (	"rep					\t\n"
					"bsf	%1,		%0		\t\n"
				:	"=r" (word)
				:	"rm" (word)
```
固定time物理行120–124，要求合法输入和先行操作返回。

## C30｜物理字节地址向上取PFN

[VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_UP]
```c
	#define PFN_UP(x)			(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
```
固定time物理行7–7，要求合法输入和先行操作返回。

## C31｜物理字节地址向下取PFN

[VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_DOWN]
```c
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
```
固定time物理行8–8，要求合法输入和先行操作返回。

## C32｜全局PFN经mem_map换page

[VERIFIED mykernel/mm/highmem/highmem_macro.h::pfn_to_page]
```c
	#define pfn_to_page(pfn)	((pfn) + mem_map)
```
固定time物理行12–12，要求合法输入和先行操作返回。

## C33｜每页Reserved清除和refcount置零

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	for (uint loop = 0; loop < nr_pages; loop++, p++) {
		__ClearPageReserved(p);
		set_page_count(p, 0);
	}
```
固定time物理行560–563，要求合法输入和先行操作返回。

## C34｜managed_pages先加nr_pages

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	atomic_long_add(nr_pages, &myos_page_zone(page)->managed_pages);
```
固定time物理行565–565，要求合法输入和先行操作返回。

## C35｜之后才调用__free_pages_ok

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	__free_pages_ok(page, order);
```
固定time物理行571–571，要求合法输入和先行操作返回。

## C36｜after_bootmem先置位再移交空闲页

[VERIFIED mykernel/arch/x86_64/mm/init.c::mem_init]
```c
	after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	/* this will put all memory onto the freelists */
	memblock_free_all();
```
固定time物理行895–899，要求合法输入和先行操作返回。

## C37｜mem_init先于slab初始化

[VERIFIED mykernel/mm/misc/mm_init.c::mm_core_init]
```c
	mem_init();
	// mem_init_print_info();
	kmem_cache_init();
```
固定time物理行409–411，要求合法输入和先行操作返回。

## C38｜先reserved标记后free枚举

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
	ulong pages;
	uint64_t i = 0;
	phys_addr_t start, end;

	memmap_init_reserved_pages();
```
固定time物理行740–744，要求合法输入和先行操作返回。

## C39｜每zone阶数组容量

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::NR_PAGE_ORDERS]
```c
	#define NR_PAGE_ORDERS (MAX_ORDER + 1)
```
固定time物理行87–87，要求合法输入和先行操作返回。

## C40｜对齐最大页块数

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::MAX_ORDER_NR_PAGES]
```c
	#define MAX_ORDER_NR_PAGES	(1 << MAX_ORDER)
```
固定time物理行85–85，要求合法输入和先行操作返回。

## C41｜找到候选并写reserve门

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	found = __memblock_find_range_bottom_up(
				start, end, size, align, flags);
	if (found && !simple_mmblk_reserve(found, size))
		return found;
```
固定time物理行482–485，要求合法输入和先行操作返回。

## C42｜HOTPLUG位访问器

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_hotpluggable]
```c
		bool
		memblock_is_hotpluggable(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_HOTPLUG;
		}
```
固定time物理行79–82，要求合法输入和先行操作返回。

## C43｜MIRROR位访问器

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_mirror]
```c
		bool
		memblock_is_mirror(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_MIRROR;
		}
```
固定time物理行84–87，要求合法输入和先行操作返回。

## C44｜DRIVER_MANAGED位访问器

[VERIFIED mykernel/mm/early/memblock.h::memblock_is_driver_managed]
```c
		bool
		memblock_is_driver_managed(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_DRIVER_MANAGED;
		}
```
固定time物理行94–97，要求合法输入和先行操作返回。

## C45｜reserved静态cnt与capacity

[VERIFIED mykernel/mm/early/memblock.c::memblock]
```c
	.reserved.regions	= mmblk_reserved_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MMBLK_RESERVED_REGIONS,
	.reserved.name		= "reserved",
```
固定time物理行90–93，要求合法输入和先行操作返回。

## C46｜描述符准备与memmap_init先后

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
		alloc_node_mem_map(pgdat);
		free_area_init_core(pgdat);
	// }

	memmap_init(max_zone_pfn);
```
固定time物理行381–385，要求合法输入和先行操作返回。

## C47｜取PFN和zone并执行free_prepare

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	ulong pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	free_pages_prepare(page, order);
	// if (!free_pages_prepare(page, order, fpi_flags))
```
固定time物理行401–405，要求合法输入和先行操作返回。

## C48｜锁内归还请求的开始

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	spin_lock_irqsave(&zone->lock, flags);
	// if (unlikely(has_isolate_pageblock(zone) ||
	// 	is_migrate_isolate(migratetype))) {
	// 	migratetype = get_pfnblock_migratetype(page, pfn);
```
固定time物理行410–413，要求合法输入和先行操作返回。

## C49｜调用buddy归还后解锁

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	__myos_free_one_page(page, pfn, zone, order);
	spin_unlock_irqrestore(&zone->lock, flags);
```
固定time物理行415–416，要求合法输入和先行操作返回。


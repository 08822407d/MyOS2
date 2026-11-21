# 第77批｜固定time源码单一定义短锚

每条1–5连续物理行来自time@a039d9803ade。函数、宏、结构和全局变量标签均指向各自真实定义，注释不当活动代码；数值模型及最早停止点见独立facts和正文。构建、内核运行与完整本地P9均NOT_RUN。

## A01｜外层alloc_pages转调实际页分配

[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
	page = __myos_alloc_pages(gfp, order);

	return page;
```
固定源物理行 108–110；仅在对象有效和先行调用正常返回时消费。

## A02｜order超过MAX_ORDER即返回NULL

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
	if (unlikely(order > MAX_ORDER)) {
		return NULL;
	}
```
固定源物理行 450–452；仅在对象有效和先行调用正常返回时消费。

## A03｜三区偏好数组与动态nr_zones上限

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		int prefered_zone_list[] = {ZONE_NORMAL, ZONE_DMA32, ZONE_DMA};
		int max_prefered_idx = NODE_DATA(0)->nr_zones;
		int start_prefered_idx = 0;
```
固定源物理行 459–461；仅在对象有效和先行调用正常返回时消费。

## A04｜DMA优先于DMA32起点

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		if (gfp & GFP_DMA)
			start_prefered_idx = 2;
		else if (gfp & GFP_DMA32)
			start_prefered_idx = 1;
```
固定源物理行 463–466；仅在对象有效和先行调用正常返回时消费。

## A05｜普通分配从下标0开始

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		else
			start_prefered_idx = 0;
```
固定源物理行 467–468；仅在对象有效和先行调用正常返回时消费。

## A06｜DMA与DMA32定义

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::zone_type]
```c
		ZONE_DMA,
		ZONE_DMA32,
```
固定源物理行 16–17；仅在对象有效和先行调用正常返回时消费。

## A07｜NORMAL区枚举

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::zone_type]
```c
		ZONE_NORMAL,
		/*
```
固定源物理行 23–24；仅在对象有效和先行调用正常返回时消费。

## A08｜五个zone枚举及上界成员

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::zone_type]
```c
		ZONE_MOVABLE,
		ZONE_DEVICE,
		__MAX_NR_ZONES
	};
```
固定源物理行 73–76；仅在对象有效和先行调用正常返回时消费。

## A09｜MAX_ORDER条件宏

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::MAX_ORDER]
```c
	#  define MAX_ORDER	10
```
固定源物理行 81–81；仅在对象有效和先行调用正常返回时消费。

## A10｜可遍历free_area阶总数

[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::NR_PAGE_ORDERS]
```c
	#define NR_PAGE_ORDERS (MAX_ORDER + 1)
```
固定源物理行 87–87；仅在对象有效和先行调用正常返回时消费。

## A11｜node_zones静态数组

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::pglist_data]
```c
		// struct zone node_zones[MAX_NR_ZONES];
		zone_s		node_zones[MAX_NR_ZONES];
```
固定源物理行 155–156；仅在对象有效和先行调用正常返回时消费。

## A12｜nr_zones与mem_map字段

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::pglist_data]
```c
		int			nr_zones;	  /* number of populated zones in this node */
		// page_s *node_mem_map;
		page_s		*node_mem_map;
```
固定源物理行 165–167；仅在对象有效和先行调用正常返回时消费。

## A13｜nr_zones实际赋值为MAX_NR_ZONES

[VERIFIED mykernel/mm/misc/mm_init.c::calculate_node_totalpages]
```c
	}

	pgdat->nr_zones = MAX_NR_ZONES;
	pgdat->node_spanned_pages = totalpages;
	pgdat->node_present_pages = realtotalpages;
```
固定源物理行 183–187；仅在对象有效和先行调用正常返回时消费。

## A14｜越界前读取偏好索引

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		for (i = start_prefered_idx; i < max_prefered_idx; i++) {
			zone_s *zone = &(NODE_DATA(0)->node_zones[prefered_zone_list[i]]);
```
固定源物理行 470–471；仅在对象有效和先行调用正常返回时消费。

## A15｜从free_area取页并请求prep

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
			page_s *page = __rmqueue_smallest(zone, order);
			if (likely(page)) {
				prep_new_page(page, order, gfp);
				return page;
```
固定源物理行 476–479；仅在对象有效和先行调用正常返回时消费。

## A16｜未命中继续下一偏好区

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
			else
				continue;
		// }
		}
	// }
```
固定源物理行 481–485；仅在对象有效和先行调用正常返回时消费。

## A17｜从请求阶开始取free_area并判空

[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
	/* Find a page of the appropriate size in the preferred list */
	for (current_order = order; current_order < NR_PAGE_ORDERS; ++current_order) {
		List_hdr_s *area = &zone->free_area[current_order];
		page = get_page_from_free_area(area);
		if (page == NULL)
```
固定源物理行 229–233；仅在对象有效和先行调用正常返回时消费。

## A18｜再删除已移出的页并分裂增引用

[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));
```
固定源物理行 236–239；仅在对象有效和先行调用正常返回时消费。

## A19｜成功页返回与真正遍历失败NULL

[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		return page;
	}

	return NULL;
}
```
固定源物理行 241–245；仅在对象有效和先行调用正常返回时消费。

## A20｜首先remove_head而后转page

[VERIFIED mykernel/mm/page_alloc/buddy.c::get_page_from_free_area]
```c
*get_page_from_free_area(List_hdr_s *area) {
	List_s *pg_lp = list_header_remove_head(area);
	return __list_null_or_page(pg_lp);
```
固定源物理行 102–104；仅在对象有效和先行调用正常返回时消费。

## A21｜空链转换的NULL路径

[VERIFIED mykernel/mm/page_alloc/buddy.c::__list_null_or_page]
```c
*__list_null_or_page(List_s * pg_lp) {
	if (pg_lp == NULL) return NULL;
	else return container_of(pg_lp, page_s, buddy_list);
```
固定源物理行 97–99；仅在对象有效和先行调用正常返回时消费。

## A22｜链表头移除的前置动作

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
		*list_header_remove_head(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
```
固定源物理行 652–655；仅在对象有效和先行调用正常返回时消费。

## A23｜实际count减1与空链NULL

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
				lhdr_p->count--;
				return ret_val;
			}
			return NULL;
```
固定源物理行 656–659；仅在对象有效和先行调用正常返回时消费。

## A24｜再次删除需成员命中

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```
固定源物理行 683–687；仅在对象有效和先行调用正常返回时消费。

## A25｜逐链表比较成员地址

[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```
固定源物理行 635–639；仅在对象有效和先行调用正常返回时消费。

## A26｜二次删除后独立清标志/private

[VERIFIED mykernel/mm/page_alloc/buddy.c::del_page_from_free_list]
```c
del_page_from_free_list(page_s *page, List_hdr_s *area) {
	list_header_delete_node(area, &page->buddy_list);
	__ClearPageBuddy(page);
	page->private = 0;
```
固定源物理行 90–93；仅在对象有效和先行调用正常返回时消费。

## A27｜拆分高阶块的循环初始阶段

[VERIFIED mykernel/mm/page_alloc/buddy.c::expand]
```c
expand(zone_s *zone, page_s *page, int low, int high) {
	ulong size = 1 << high;
	while (high > low) {
		high--;
		size >>= 1;
```
固定源物理行 123–127；仅在对象有效和先行调用正常返回时消费。

## A28｜分裂伙伴节点并入小阶free_area

[VERIFIED mykernel/mm/page_alloc/buddy.c::expand]
```c
		page_s *p = &page[size];
		INIT_LIST_S(&p->buddy_list);

		add_to_free_list(p, zone, high);
		set_buddy_order(p, high);
```
固定源物理行 128–132；仅在对象有效和先行调用正常返回时消费。

## A29｜把page.buddy_list链接到目标阶

[VERIFIED mykernel/mm/page_alloc/buddy.c::add_to_free_list]
```c
add_to_free_list(page_s *page, zone_s *zone, uint order) {
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_header_add_to_head(fa_lhdr, &page->buddy_list);
```
固定源物理行 77–79；仅在对象有效和先行调用正常返回时消费。

## A30｜把order写private并设PageBuddy

[VERIFIED mykernel/mm/page_alloc/buddy.c::set_buddy_order]
```c
set_buddy_order(page_s *page, uint order) {
	page->private = (ulong)order;
	__SetPageBuddy(page);
```
固定源物理行 70–72；仅在对象有效和先行调用正常返回时消费。

## A31｜对交付低阶页数逐一refcount加量

[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

		return page;
```
固定源物理行 238–241；仅在对象有效和先行调用正常返回时消费。

## A32｜真实链入后count加1

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
		list_header_add_to_head(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```
固定源物理行 646–648；仅在对象有效和先行调用正常返回时消费。

## A33｜链表头anchor及count类型

[VERIFIED mykernel/lib/list/double_list_types.h::list_hdr]
```c
	struct list_hdr {
		List_s		anchor;
		ulong		count;
```
固定源物理行 13–15；仅在对象有效和先行调用正常返回时消费。

## A34｜每个zone的free_area数组

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		// struct free_area free_area[NR_PAGE_ORDERS];
		List_hdr_s		free_area[NR_PAGE_ORDERS];
```
固定源物理行 103–104；仅在对象有效和先行调用正常返回时消费。

## A35｜最近page_ref_inc原语

[VERIFIED mykernel/mm/page_alloc/buddy.h::page_ref_inc]
```c
		page_ref_inc(page_s *page) {
			atomic_inc(&page->_refcount);
```
固定源物理行 93–94；仅在对象有效和先行调用正常返回时消费。

## A36｜GFP_DMA宏定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_DMA]
```c
	#define GFP_DMA					__GFP_DMA
```
固定源物理行 315–315；仅在对象有效和先行调用正常返回时消费。

## A37｜GFP_DMA32宏定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_DMA32]
```c
	#define GFP_DMA32				__GFP_DMA32
```
固定源物理行 316–316；仅在对象有效和先行调用正常返回时消费。

## A38｜各阶free_area清空初始化

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (int j = 0; j < NR_PAGE_ORDERS; j++)
			INIT_LIST_HEADER_S(&zone->free_area[j]);

```
固定源物理行 98–100；仅在对象有效和先行调用正常返回时消费。

## A39｜zone_start_pfn成员

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
		ulong			zone_start_pfn;
```
固定源物理行 43–44；仅在对象有效和先行调用正常返回时消费。

## A40｜zone页面跨度和数量成员

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		atomic_long_t	managed_pages;
		ulong			spanned_pages;
		ulong			present_pages;
```
固定源物理行 88–90；仅在对象有效和先行调用正常返回时消费。

## B01｜ZERO请求循环真实地址表达式

[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```
固定源物理行 190–192；仅在对象有效和先行调用正常返回时消费。

## B02｜clear_page实际PAGE_SIZE长度memset

[VERIFIED mykernel/include/linux/kernel/asm-generic/page.h::clear_page]
```c
	#	define clear_page(page)		memset((page), 0, PAGE_SIZE)
```
固定源物理行 37–37；仅在对象有效和先行调用正常返回时消费。

## B03｜page_to_virt宏

[VERIFIED mykernel/mm/highmem/highmem_macro.h::page_to_virt]
```c
	#  define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
```
固定源物理行 19–19；仅在对象有效和先行调用正常返回时消费。

## B04｜page到pfn的真实宏

[VERIFIED mykernel/mm/highmem/highmem_macro.h::page_to_pfn]
```c
	#define page_to_pfn(page)	((ulong)((page) - mem_map))
```
固定源物理行 11–11；仅在对象有效和先行调用正常返回时消费。

## B05｜页偏移12的独立宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
固定源物理行 9–9；仅在对象有效和先行调用正常返回时消费。

## B06｜PAGE_SIZE真实宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定源物理行 10–10；仅在对象有效和先行调用正常返回时消费。

## B07｜_AC C分支token-paste

[VERIFIED mykernel/include/uapi/linux/const.h::_AC]
```c
	#	define _AC(X,Y)		(X##Y)
```
固定源物理行 20–20；仅在对象有效和先行调用正常返回时消费。

## B08｜post_alloc先于compound初始化

[VERIFIED mykernel/mm/page_alloc/buddy.c::prep_new_page]
```c
prep_new_page(page_s *page, uint order, gfp_t gfp_flags) {
	post_alloc_hook(page, order, gfp_flags);

	if (order && (gfp_flags & __GFP_COMP))
		prep_compound_page(page, order);
```
固定源物理行 199–203；仅在对象有效和先行调用正常返回时消费。

## B09｜设置head后逐尾循环

[VERIFIED mykernel/mm/page_alloc/buddy.c::prep_compound_page]
```c
	int i;
	int nr_pages = 1 << order;

	__SetPageHead(page);
	for (i = 1; i < nr_pages; i++)
```
固定源物理行 59–63；仅在对象有效和先行调用正常返回时消费。

## B10｜逐尾之后再写head元数据

[VERIFIED mykernel/mm/page_alloc/buddy.c::prep_compound_page]
```c
		prep_compound_tail(page, i);

	prep_compound_head(page, order);
```
固定源物理行 64–66；仅在对象有效和先行调用正常返回时消费。

## B11｜tail链指针编码及private写入

[VERIFIED mykernel/mm/page_alloc/buddy.h::prep_compound_tail]
```c
			page_s *p = head + tail_idx;

			// p->mapping = TAIL_MAPPING;
			set_compound_head(p, head);
			p->private = 0;
```
固定源物理行 374–378；仅在对象有效和先行调用正常返回时消费。

## B12｜tail指向head+1

[VERIFIED mykernel/mm/page_alloc/page-flags.h::set_compound_head]
```c
		set_compound_head(page_s *page, page_s *head) {
			WRITE_ONCE(page->compound_head, (ulong)head + 1);
```
固定源物理行 150–151；仅在对象有效和先行调用正常返回时消费。

## B13｜真实PG_head位设置

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__SetPageHead]
```c
		__SetPageHead(page_s *page) {
			arch___set_bit(PG_head, &(page)->flags);
```
固定源物理行 401–402；仅在对象有效和先行调用正常返回时消费。

## B14｜head有效性检查可能return

[VERIFIED mykernel/mm/page_alloc/buddy.h::prep_compound_head]
```c
		// static inline void folio_set_order(struct folio *folio, unsigned int order)
		// {
			if (WARN_ON_ONCE(!order || !folio_test_head(folio)))
				return;
```
固定源物理行 359–362；仅在对象有效和先行调用正常返回时消费。

## B15｜head低8位order与页面总数

[VERIFIED mykernel/mm/page_alloc/buddy.h::prep_compound_head]
```c
			folio->_flags_1 = (folio->_flags_1 & ~0xffUL) | order;
			folio->_folio_nr_pages = 1U << order;
		// }
			atomic_set(&folio->_entire_mapcount, -1);
```
固定源物理行 364–367；仅在对象有效和先行调用正常返回时消费。

## B16｜head的计数字段初始化

[VERIFIED mykernel/mm/page_alloc/buddy.h::prep_compound_head]
```c
			atomic_set(&folio->_entire_mapcount, -1);
			atomic_set(&folio->_nr_pages_mapped, 0);
			atomic_set(&folio->_pincount, 0);
```
固定源物理行 367–369；仅在对象有效和先行调用正常返回时消费。

## B17｜读取folio的低8位order

[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_order]
```c
		folio_order(folio_s *folio) {
			if (!folio_test_large(folio))
				return 0;
			return folio->_flags_1 & 0xff;
```
固定源物理行 144–147；仅在对象有效和先行调用正常返回时消费。

## B18｜compound_order先检查head标志

[VERIFIED mykernel/mm/page_alloc/buddy.h::compound_order]
```c
		compound_order(page_s *page) {
			folio_s *folio = (folio_s *)page;
			if (!test_bit(PG_head, &folio->flags))
				return 0;
			return folio->_flags_1 & 0xff;
```
固定源物理行 127–131；仅在对象有效和先行调用正常返回时消费。

## B19｜folio_test_head真实位判定

[VERIFIED mykernel/mm/page_alloc/page-flags.h::folio_test_head]
```c
		folio_test_head(folio_s *folio) {
			return (__builtin_constant_p((PG_head)) ?
						constant_test_bit((PG_head), (folio_flags(folio, 0))) :
						variable_test_bit((PG_head), (folio_flags(folio, 0)))
```
固定源物理行 377–380；仅在对象有效和先行调用正常返回时消费。

## B20｜folio_test_large宏别名

[VERIFIED mykernel/mm/page_alloc/page-flags_macro.h::folio_test_large]
```c
	#define folio_test_large(folio)	folio_test_head(folio)
```
固定源物理行 250–250；仅在对象有效和先行调用正常返回时消费。

## B21｜folio对page的叠放与flags_1

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::folio]
```c
			page_s	page;
		};
		union {
			struct {
				ulong			_flags_1;
```
固定源物理行 240–244；仅在对象有效和先行调用正常返回时消费。

## B22｜folio页映射和pincount成员

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::folio]
```c
		/* public: */
				atomic_t		_entire_mapcount;
				atomic_t		_nr_pages_mapped;
				atomic_t		_pincount;
				uint			_folio_nr_pages;
```
固定源物理行 247–251；仅在对象有效和先行调用正常返回时消费。

## B23｜PG_head枚举成员

[VERIFIED mykernel/mm/page_alloc/page-flags_const.h::pageflags]
```c
		PG_dirty,
		PG_lru,
		PG_head,			/* Must be in bit 6 */
		PG_waiters,			/* Page has waiters, check its waitqueue. Must be bit #7 and in the same byte as "PG_locked" */
```
固定源物理行 95–98；仅在对象有效和先行调用正常返回时消费。

## B24｜tail compound_head成员

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
			struct {	/* Tail pages of compound page */
				ulong			compound_head;		/* Bit zero is set */
			};
```
固定源物理行 114–116；仅在对象有效和先行调用正常返回时消费。

## B25｜slab过滤flag并OR缓存allocflags

[VERIFIED mykernel/mm/kmalloc/slub.c::prepare_alloc_flags]
```c
prepare_alloc_flags(kmem_cache_s *s, gfp_t flags) {
	flags &= (GFP_RECLAIM_MASK | GFP_CONSTRAINT_MASK) & gfp_allowed_mask;
	flags |= s->allocflags;
```
固定源物理行 87–89；仅在对象有效和先行调用正常返回时消费。

## B26｜slab页分配额外标志处理

[VERIFIED mykernel/mm/kmalloc/slub.c::prepare_alloc_flags]
```c
	return (flags | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_NOFAIL;
```
固定源物理行 94–94；仅在对象有效和先行调用正常返回时消费。

## B27｜reclaim mask原定义排除ZERO

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_RECLAIM_MASK]
```c
	#define GFP_RECLAIM_MASK (__GFP_RECLAIM|__GFP_HIGH|__GFP_IO|__GFP_FS|\
				__GFP_NOWARN|__GFP_RETRY_MAYFAIL|__GFP_NOFAIL|\
				__GFP_NORETRY|__GFP_MEMALLOC|__GFP_NOMEMALLOC|\
				__GFP_NOLOCKDEP)
```
固定源物理行 397–400；仅在对象有效和先行调用正常返回时消费。

## B28｜constraint mask独立定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_CONSTRAINT_MASK]
```c
	#define GFP_CONSTRAINT_MASK (__GFP_HARDWALL|__GFP_THISNODE)
```
固定源物理行 406–406；仅在对象有效和先行调用正常返回时消费。

## B29｜slab缓存allocflags初值与COMP

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	s->allocflags = 0;
	if (order)
		s->allocflags |= __GFP_COMP;
```
固定源物理行 400–402；仅在对象有效和先行调用正常返回时消费。

## B30｜缓存DMA位产生者

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	if (s->flags & SLAB_CACHE_DMA)
		s->allocflags |= GFP_DMA;

	if (s->flags & SLAB_CACHE_DMA32)
		s->allocflags |= GFP_DMA32;
```
固定源物理行 404–408；仅在对象有效和先行调用正常返回时消费。

## B31｜slab通过过滤flags向alloc_pages取页

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
*alloc_slab(kmem_cache_s *s, gfp_t flags) {
	gfp_t alloc_gfp = prepare_alloc_flags(s, flags);
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
```
固定源物理行 103–105；仅在对象有效和先行调用正常返回时消费。

## B32｜large单独按原size请求memset

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_large]
```c
	page_s *page = alloc_pages(flags | __GFP_COMP, get_order(size));
	if (page) {
		ptr = (void *)page_to_virt(page);
		if (flags & __GFP_ZERO)
			memset(ptr, 0, size);
```
固定源物理行 268–272；仅在对象有效和先行调用正常返回时消费。

## B33｜大块释放按folio_order再调__free_pages

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	uint order = folio_order(folio);
	if (WARN_ON_ONCE(order == 0))
		pr_warn_once("object pointer: 0x%p\n", object);
	__free_pages(folio_page(folio, 0), order);
```
固定源物理行 280–283；仅在对象有效和先行调用正常返回时消费。

## B34｜slab清flag后请求__free_pages

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	smp_wmb();
	__folio_clear_slab(folio);
	// mm_account_reclaimed_pages(pages);
	// unaccount_slab(slab, order, s);
	__free_pages(&folio->page, order);
```
固定源物理行 247–251；仅在对象有效和先行调用正常返回时消费。

## B35｜ZERO位宏独立定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::__GFP_ZERO]
```c
	#define __GFP_ZERO				((__force gfp_t)___GFP_ZERO)
```
固定源物理行 226–226；仅在对象有效和先行调用正常返回时消费。

## B36｜COMP位宏独立定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::__GFP_COMP]
```c
	#define __GFP_COMP				((__force gfp_t)___GFP_COMP)
```
固定源物理行 225–225；仅在对象有效和先行调用正常返回时消费。

## B37｜GFP初始允许掩码

[VERIFIED mykernel/mm/page_alloc/buddy.c::gfp_allowed_mask]
```c
gfp_t gfp_allowed_mask __read_mostly = GFP_BOOT_MASK;
```
固定源物理行 21–21；仅在对象有效和先行调用正常返回时消费。

## B38｜GFP_BOOT_MASK定义

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_BOOT_MASK]
```c
	#define GFP_BOOT_MASK (__GFP_BITS_MASK & ~(__GFP_RECLAIM|__GFP_IO|__GFP_FS))
```
固定源物理行 403–403；仅在对象有效和先行调用正常返回时消费。

## C01｜__free_pages立即请求__free_pages_ok

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages]
```c
	__free_pages_ok(page, order);
```
固定源物理行 518–518；仅在对象有效和先行调用正常返回时消费。

## C02｜非零虚拟地址向page转换

[VERIFIED mykernel/mm/page_alloc/buddy.c::free_pages]
```c
void free_pages(ulong addr, uint order) {
	if (addr != 0)
		__free_pages(virt_to_page((void *)addr), order);
```
固定源物理行 521–523；仅在对象有效和先行调用正常返回时消费。

## C03｜释放前PFN、zone和prepare调用

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	ulong pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	free_pages_prepare(page, order);
```
固定源物理行 401–404；仅在对象有效和先行调用正常返回时消费。

## C04｜free_pages_prepare唯一活动位清理

[VERIFIED mykernel/mm/page_alloc/buddy.c::free_pages_prepare]
```c
	// page_cpupid_reset_last(page);
	page->flags &= ~PAGE_FLAGS_CHECK_AT_PREP;
	// reset_page_owner(page, order);
```
固定源物理行 353–355；仅在对象有效和先行调用正常返回时消费。

## C05｜prepare返回未被活动条件检查

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	free_pages_prepare(page, order);
	// if (!free_pages_prepare(page, order, fpi_flags))
	// 	return;
```
固定源物理行 404–406；仅在对象有效和先行调用正常返回时消费。

## C06｜释放区取得锁并含注释门

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	spin_lock_irqsave(&zone->lock, flags);
	// if (unlikely(has_isolate_pageblock(zone) ||
	// 	is_migrate_isolate(migratetype))) {
	// 	migratetype = get_pfnblock_migratetype(page, pfn);
```
固定源物理行 410–413；仅在对象有效和先行调用正常返回时消费。

## C07｜锁内合并入链请求与解锁

[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	__myos_free_one_page(page, pfn, zone, order);
	spin_unlock_irqrestore(&zone->lock, flags);
```
固定源物理行 415–416；仅在对象有效和先行调用正常返回时消费。

## C08｜扫描zone根据PFN选所属区

[VERIFIED mykernel/mm/page_alloc/buddy.h::myos_page_zone]
```c
		*myos_page_zone(const page_s * page) {
			ulong pfn = page_to_pfn(page);
			for (int i = 0 ; i < MAX_NR_ZONES; i ++)
			{
				zone_s * zone = &(NODE_DATA(0)->node_zones[i]);
```
固定源物理行 322–326；仅在对象有效和先行调用正常返回时消费。

## C09｜zone匹配return而尾部缺return

[VERIFIED mykernel/mm/page_alloc/buddy.h::myos_page_zone]
```c
				if (pfn >= zone->zone_start_pfn && 
					pfn < (zone->zone_start_pfn + zone->spanned_pages))
					return zone;
			}
		}
```
固定源物理行 327–331；仅在对象有效和先行调用正常返回时消费。

## C10｜真实zone起始PFN

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
		ulong			zone_start_pfn;
```
固定源物理行 43–44；仅在对象有效和先行调用正常返回时消费。

## C11｜真实zone跨度及present字段

[VERIFIED mykernel/mm/page_alloc/mmzone_types.h::zone]
```c
		atomic_long_t	managed_pages;
		ulong			spanned_pages;
		ulong			present_pages;
```
固定源物理行 88–90；仅在对象有效和先行调用正常返回时消费。

## C12｜循环检查buddy是否匹配

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
	while (order < MAX_ORDER) {
		buddy = find_buddy_page_pfn(page, pfn, order, &buddy_pfn);
		if (buddy == NULL)
			break;
```
固定源物理行 280–283；仅在对象有效和先行调用正常返回时消费。

## C13｜合并PFN地址表达式和order更新

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
		combined_pfn = buddy_pfn & pfn;
		page = page + (combined_pfn - pfn);
		pfn = combined_pfn;
		order++;
```
固定源物理行 285–288；仅在对象有效和先行调用正常返回时消费。

## C14｜退出循环设阶与挂链

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
```
固定源物理行 291–292；仅在对象有效和先行调用正常返回时消费。

## C15｜伙伴PFN按xor生成

[VERIFIED mykernel/mm/page_alloc/buddy.h::__find_buddy_pfn]
```c
		__find_buddy_pfn(ulong page_pfn, uint order) {
			return page_pfn ^ (1 << order);
```
固定源物理行 399–400；仅在对象有效和先行调用正常返回时消费。

## C16｜先计算差值再形成buddy指针

[VERIFIED mykernel/mm/page_alloc/buddy.h::find_buddy_page_pfn]
```c
			ulong __buddy_pfn = __find_buddy_pfn(pfn, order);
			page_s *buddy = page + (__buddy_pfn - pfn);
			if (buddy_pfn)
				*buddy_pfn = __buddy_pfn;
```
固定源物理行 445–448；仅在对象有效和先行调用正常返回时消费。

## C17｜指针产生后才执行buddy判定

[VERIFIED mykernel/mm/page_alloc/buddy.h::find_buddy_page_pfn]
```c
			if (page_is_buddy(page, buddy, order))
				return buddy;
			return NULL;
```
固定源物理行 450–452；仅在对象有效和先行调用正常返回时消费。

## C18｜guard与PageBuddy和order门

[VERIFIED mykernel/mm/page_alloc/buddy.h::page_is_buddy]
```c
		page_is_buddy(page_s *page, page_s *buddy, uint order) {
			if (!page_is_guard(page) && !PageBuddy(buddy))
				return false;
			if (buddy_order(buddy) != order)
				return false;
```
固定源物理行 417–421；仅在对象有效和先行调用正常返回时消费。

## C19｜同zone核对后返回true

[VERIFIED mykernel/mm/page_alloc/buddy.h::page_is_buddy]
```c
			if (myos_page_zone(page) != myos_page_zone(buddy))
				return false;
			return true;
```
固定源物理行 422–424；仅在对象有效和先行调用正常返回时消费。

## C20｜page_is_guard实际固定false

[VERIFIED mykernel/mm/page_alloc/buddy.h::page_is_guard]
```c
		page_is_guard(page_s *page) {
			return false;
		}
```
固定源物理行 222–224；仅在对象有效和先行调用正常返回时消费。

## C21｜buddy_order读page.private

[VERIFIED mykernel/mm/page_alloc/buddy.h::buddy_order]
```c
		buddy_order(page_s *page) {
			/* PageBuddy() must be checked by the caller */
			return page_private(page);
```
固定源物理行 350–352；仅在对象有效和先行调用正常返回时消费。

## C22｜set_buddy_order私有阶记录

[VERIFIED mykernel/mm/page_alloc/buddy.c::set_buddy_order]
```c
set_buddy_order(page_s *page, uint order) {
	page->private = (ulong)order;
	__SetPageBuddy(page);
```
固定源物理行 70–72；仅在对象有效和先行调用正常返回时消费。

## C23｜目标阶归还链请求

[VERIFIED mykernel/mm/page_alloc/buddy.c::add_to_free_list]
```c
add_to_free_list(page_s *page, zone_s *zone, uint order) {
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_header_add_to_head(fa_lhdr, &page->buddy_list);
```
固定源物理行 77–79；仅在对象有效和先行调用正常返回时消费。

## C24｜PageBuddy比较page_type基值

[VERIFIED mykernel/mm/page_alloc/page-flags.h::PageBuddy]
```c
		PageBuddy(page_s *page) {
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
```
固定源物理行 480–481；仅在对象有效和先行调用正常返回时消费。

## C25｜设置buddy状态实际上清特定位

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__SetPageBuddy]
```c
		__SetPageBuddy(page_s *page) {
			page->page_type &= ~PG_buddy;
```
固定源物理行 485–486；仅在对象有效和先行调用正常返回时消费。

## C26｜清除buddy状态实际上置特定位

[VERIFIED mykernel/mm/page_alloc/page-flags.h::__ClearPageBuddy]
```c
		__ClearPageBuddy(page_s *page) {
			page->page_type |= PG_buddy;
```
固定源物理行 490–491；仅在对象有效和先行调用正常返回时消费。

## C27｜PAGE_TYPE_BASE独立宏

[VERIFIED mykernel/mm/page_alloc/page-flags_const.h::PAGE_TYPE_BASE]
```c
	#define PAGE_TYPE_BASE			0xf0000000
```
固定源物理行 190–190；仅在对象有效和先行调用正常返回时消费。

## C28｜PG_buddy独立宏

[VERIFIED mykernel/mm/page_alloc/page-flags_const.h::PG_buddy]
```c
	#define PG_buddy				0x00000080
```
固定源物理行 193–193；仅在对象有效和先行调用正常返回时消费。

## C29｜page_type成员真正属于page结构

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
			uint				page_type;
```
固定源物理行 153–153；仅在对象有效和先行调用正常返回时消费。

## C30｜与page_type同一union的_mapcount

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
			atomic_t			_mapcount;
```
固定源物理行 164–164；仅在对象有效和先行调用正常返回时消费。

## C31｜引用数_refcount另独立存储

[VERIFIED mykernel/mm/page_alloc/buddy_types.h::page]
```c
		/* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
		atomic_t				_refcount;
```
固定源物理行 167–168；仅在对象有效和先行调用正常返回时消费。

## C32｜mem_map清零及指针设定

[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		memset(map, 0, size);
		pgdat->node_mem_map = mem_map = map + offset;
```
固定源物理行 319–320；仅在对象有效和先行调用正常返回时消费。

## C33｜page的mapcount_reset注释

[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
__init_single_page(page_s *page, ulong pfn, ulong zone) {
	// mm_zero_struct_page(page);
	// set_page_links(page, zone, nid, pfn);
	init_page_count(page);
	// page_mapcount_reset(page);
```
固定源物理行 20–24；仅在对象有效和先行调用正常返回时消费。

## C34｜各阶链表初始化后逐PFN

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (int j = 0; j < NR_PAGE_ORDERS; j++)
			INIT_LIST_HEADER_S(&zone->free_area[j]);

		for (ulong pfn = start_pfn; pfn < end_pfn; pfn++) {
```
固定源物理行 98–101；仅在对象有效和先行调用正常返回时消费。

## C35｜每个page初始化的调用点

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (ulong pfn = start_pfn; pfn < end_pfn; pfn++) {
			page_s *page = pfn_to_page(pfn);
			__init_single_page(page, pfn, i);
```
固定源物理行 101–103；仅在对象有效和先行调用正常返回时消费。

## C36｜引导释放前逐页refcount设零

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	for (uint loop = 0; loop < nr_pages; loop++, p++) {
		__ClearPageReserved(p);
		set_page_count(p, 0);
	}
```
固定源物理行 560–563；仅在对象有效和先行调用正常返回时消费。

## C37｜引导释放更新managed_pages

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	atomic_long_add(nr_pages, &myos_page_zone(page)->managed_pages);
```
固定源物理行 565–565；仅在对象有效和先行调用正常返回时消费。

## C38｜引导直接归还__free_pages_ok

[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	__free_pages_ok(page, order);
```
固定源物理行 571–571；仅在对象有效和先行调用正常返回时消费。

## C39｜__folio_put本体只有注释分支

[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
	// 	free_zone_device_page(&folio->page);
	// else if (unlikely(folio_test_large(folio)))
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
```
固定源物理行 635–639；仅在对象有效和先行调用正常返回时消费。

## C40｜计数减到零时调用__folio_put

[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_put]
```c
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
```
固定源物理行 179–182；仅在对象有效和先行调用正常返回时消费。

## C41｜put_page直接转folio_put

[VERIFIED mykernel/mm/page_alloc/buddy.h::put_page]
```c
			// if (put_devmap_managed_page(&folio->page))
			// 	return;
			folio_put(page_folio(page));
```
固定源物理行 197–199；仅在对象有效和先行调用正常返回时消费。

## C42｜slab释放者的页分配器直接接口

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	smp_wmb();
	__folio_clear_slab(folio);
	// mm_account_reclaimed_pages(pages);
	// unaccount_slab(slab, order, s);
	__free_pages(&folio->page, order);
```
固定源物理行 247–251；仅在对象有效和先行调用正常返回时消费。

## C43｜large释放者的页分配器直接接口

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	uint order = folio_order(folio);
	if (WARN_ON_ONCE(order == 0))
		pr_warn_once("object pointer: 0x%p\n", object);
	__free_pages(folio_page(folio, 0), order);
```
固定源物理行 280–283；仅在对象有效和先行调用正常返回时消费。


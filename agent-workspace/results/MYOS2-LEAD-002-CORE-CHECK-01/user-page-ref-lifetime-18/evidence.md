---
task_id: CORE-USER-PAGE-REF-LIFETIME-18
track_id: MYOS2-LEAD-002
record_type: fixed_source_anchor_ledger
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
date: 2026-11-01
source_branch: time
source_pin_short12: a039d9803ade
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
self_check:
  verified_claims: 84
  quotes_reconfirmed: 84
  downgraded_to_inferred: 0
---

# PL18 固定源码锚点

逐字引文均在 `time @ a039d9803ade` 的已读定义内部；`VERIFIED`仅指源文本内容的限定事实。被注释的语句只作注释存在证据，不作活动执行。每个引文为1–5行，行号仅是当前快照的辅助定位。

### PL18-A01 · lru是链表union成员
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
				union {
					List_s		lru;
					/* Or, for the Unevictable "LRU list" slot */
					struct {
```

### PL18-A02 · buddy_list与lru共享union
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
					/* Or, free page */
					List_s		buddy_list;
					List_s		pcp_list;
				};
```

### PL18-A03 · compound_head与首字union重叠
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
			struct {	/* Tail pages of compound page */
				ulong			compound_head;		/* Bit zero is set */
			};
```

### PL18-A04 · page_type与_mapcount共享union
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
			uint				page_type;

```

### PL18-A05 · _mapcount不是另一独立存储
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
			 * Note that the mapcount is always initialized to -1, so that
			 * transitions both from it and to it can be tracked, using
			 * atomic_inc_and_test() and atomic_add_negative(-1).
			 */
			atomic_t			_mapcount;
```

### PL18-A06 · _refcount独立原子字段
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
		/* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
		atomic_t				_refcount;
```

### PL18-A07 · mem_map申请及失败分支
[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		map = (void *)phys_to_virt(memblock_alloc_range(size,
							SMP_CACHE_BYTES, MAX_DMA_PFN, 0));
		if (!map) {
			while (1);
			
```

### PL18-A08 · 页描述符memset(0)
[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
		memset(map, 0, size);
		pgdat->node_mem_map = mem_map = map + offset;
```

### PL18-A09 · lru和buddy_list共同初始化
[VERIFIED mykernel/mm/misc/mm_init.c::alloc_node_mem_map]
```c
	for (int i = 0; i < NODE_DATA(0)->node_spanned_pages; i++) {
		page_s *page = &(NODE_DATA(0)->node_mem_map[i]);
		INIT_LIST_S(&page->lru);
		INIT_LIST_S(&page->buddy_list);
```

### PL18-A10 · 初始化调用init_page_count
[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
static void __meminit
__init_single_page(page_s *page, ulong pfn, ulong zone) {
	// mm_zero_struct_page(page);
	// set_page_links(page, zone, nid, pfn);
	init_page_count(page);
```

### PL18-A11 · mapcount_reset是注释
[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
	init_page_count(page);
	// page_mapcount_reset(page);
	// page_cpupid_reset_last(page);
```

### PL18-A12 · 空闲列表头初始化
[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (int j = 0; j < NR_PAGE_ORDERS; j++)
			INIT_LIST_HEADER_S(&zone->free_area[j]);

```

### PL18-A13 · 逐PFN进行初始化
[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		for (ulong pfn = start_pfn; pfn < end_pfn; pfn++) {
			page_s *page = pfn_to_page(pfn);
			__init_single_page(page, pfn, i);
		}
```

### PL18-A14 · set_page_count原子赋值
[VERIFIED mykernel/mm/page_alloc/buddy.h::set_page_count]
```c
		PREFIX_STATIC_INLINE
		void
		set_page_count(page_s *page, int v) {
			atomic_set(&page->_refcount, v);
		}
```

### PL18-A15 · 初始化引用设1
[VERIFIED mykernel/mm/page_alloc/buddy.h::init_page_count]
```c
		PREFIX_STATIC_INLINE
		void
		init_page_count(page_s *page) {
			set_page_count(page, 1);
		}
```

### PL18-A16 · 只遍历匹配空闲范围
[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
	for_each_free_mem_range (i, MEMBLOCK_NONE, &start, &end) {
		ulong start_pfn = PFN_UP(start);
		ulong end_pfn = min_t(ulong, PFN_DOWN(end), max_low_pfn);
		if (start_pfn >= end_pfn)
			continue;
```

### PL18-A17 · 选阶调用memblock_free_pages
[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
		while (start_pfn < end_pfn) {
			int order = min_t(int, MAX_ORDER, __ffs(start_pfn));
			while (start_pfn + (1UL << order) > end_pfn)
				order--;
			memblock_free_pages(pfn_to_page(start_pfn), start_pfn, order);
```

### PL18-A18 · 按order形成页数量
[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
// void __free_pages_core(page_s *page, unsigned int order)
// {
	uint nr_pages = 1 << order;
	page_s *p = page;
```

### PL18-A19 · 首次投放引用归零/清Reserved
[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	for (uint loop = 0; loop < nr_pages; loop++, p++) {
		__ClearPageReserved(p);
		set_page_count(p, 0);
	}
```

### PL18-A20 · 首次投放managed_pages记录
[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	atomic_long_add(nr_pages, &myos_page_zone(page)->managed_pages);
```

### PL18-A21 · 首投进入free_pages_ok
[VERIFIED mykernel/mm/page_alloc/buddy.c::memblock_free_pages]
```c
	 * Bypass PCP and place fresh pages right to the tail, primarily
	 * relevant for memory onlining.
	 */
	__free_pages_ok(page, order);
// }
```

### PL18-A22 · 直接free获取PFN/zone并执行准备
[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	ulong pfn = page_to_pfn(page);
	zone_s *zone = myos_page_zone(page);

	free_pages_prepare(page, order);
```

### PL18-A23 · 直接free取得zone锁
[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	spin_lock_irqsave(&zone->lock, flags);
	// if (unlikely(has_isolate_pageblock(zone) ||
```

### PL18-A24 · zone锁内入链并解锁
[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	__myos_free_one_page(page, pfn, zone, order);
	spin_unlock_irqrestore(&zone->lock, flags);
```

### PL18-A25 · 准备函数只清检查flag
[VERIFIED mykernel/mm/page_alloc/buddy.c::free_pages_prepare]
```c
	// page_cpupid_reset_last(page);
	page->flags &= ~PAGE_FLAGS_CHECK_AT_PREP;
	// reset_page_owner(page, order);
```

### PL18-A26 · 伙伴循环存在条件门
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
	while (order < MAX_ORDER) {
		buddy = find_buddy_page_pfn(page, pfn, order, &buddy_pfn);
		if (buddy == NULL)
			break;

```

### PL18-A27 · 符合伙伴时合并更新page/order
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
		combined_pfn = buddy_pfn & pfn;
		page = page + (combined_pfn - pfn);
		pfn = combined_pfn;
		order++;
	}
```

### PL18-A28 · 最终标记阶数并入链
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
```

### PL18-A29 · 伙伴PFN为异或位运算
[VERIFIED mykernel/mm/page_alloc/buddy.h::__find_buddy_pfn]
```c
		static inline ulong
		__find_buddy_pfn(ulong page_pfn, uint order) {
			return page_pfn ^ (1 << order);
		}
```

### PL18-A30 · 伙伴页描述符相对偏移
[VERIFIED mykernel/mm/page_alloc/buddy.h::find_buddy_page_pfn]
```c
			ulong __buddy_pfn = __find_buddy_pfn(pfn, order);
			page_s *buddy = page + (__buddy_pfn - pfn);
			if (buddy_pfn)
				*buddy_pfn = __buddy_pfn;

```

### PL18-A31 · 须page_is_buddy认可
[VERIFIED mykernel/mm/page_alloc/buddy.h::find_buddy_page_pfn]
```c
			if (page_is_buddy(page, buddy, order))
				return buddy;
			return NULL;
```

### PL18-A32 · PageBuddy为假则局部拒合并
[VERIFIED mykernel/mm/page_alloc/buddy.h::page_is_buddy]
```c
		page_is_buddy(page_s *page, page_s *buddy, uint order) {
			if (!page_is_guard(page) && !PageBuddy(buddy))
				return false;
			if (buddy_order(buddy) != order)
				return false;
```

### PL18-A33 · 匹配阶数/zone判定
[VERIFIED mykernel/mm/page_alloc/buddy.h::page_is_buddy]
```c
			if (buddy_order(buddy) != order)
				return false;
			if (myos_page_zone(page) != myos_page_zone(buddy))
				return false;
			return true;
```

### PL18-A34 · BASE高位/PG_buddy位常量
[VERIFIED mykernel/mm/page_alloc/page-flags_const.h::PAGE_TYPE_BASE]
```c
	#define PAGE_TYPE_BASE			0xf0000000
	/* Reserve		0x0000007f to catch underflows of page_mapcount */
	#define PAGE_MAPCOUNT_RESERVE	-128
	#define PG_buddy				0x00000080
```

### PL18-A35 · PageBuddy位掩码相等测试
[VERIFIED mykernel/mm/page_alloc/page-flags.h::PageBuddy]
```c
		int
		PageBuddy(page_s *page) {
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
		}
```

### PL18-A36 · SetBuddy仅清PG_buddy位
[VERIFIED mykernel/mm/page_alloc/page-flags.h::__SetPageBuddy]
```c
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__SetPageBuddy(page_s *page) {
			page->page_type &= ~PG_buddy;
		}
```

### PL18-A37 · ClearBuddy仅置PG_buddy位
[VERIFIED mykernel/mm/page_alloc/page-flags.h::__ClearPageBuddy]
```c
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageBuddy(page_s *page) {
			page->page_type |= PG_buddy;
```

### PL18-A38 · private设置阶数与Buddy标记
[VERIFIED mykernel/mm/page_alloc/buddy.c::set_buddy_order]
```c
static inline void
set_buddy_order(page_s *page, uint order) {
	page->private = (ulong)order;
	__SetPageBuddy(page);
}
```

### PL18-A39 · header_add_to_head实际调用
[VERIFIED mykernel/mm/page_alloc/buddy.c::add_to_free_list]
```c
static inline void
add_to_free_list(page_s *page, zone_s *zone, uint order) {
	List_hdr_s *fa_lhdr = &zone->free_area[order];
	list_header_add_to_head(fa_lhdr, &page->buddy_list);
}
```

### PL18-A40 · 空闲链头节点弹出
[VERIFIED mykernel/mm/page_alloc/buddy.c::get_page_from_free_area]
```c
static inline page_s
*get_page_from_free_area(List_hdr_s *area) {
	List_s *pg_lp = list_header_remove_head(area);
	return __list_null_or_page(pg_lp);
}
```

### PL18-A41 · 链表头节点已del_init且count--
[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
		*list_header_remove_head(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
```

### PL18-A42 · 弹出后返值或NULL
[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
			}
			return NULL;
```

### PL18-A43 · 再次调用delete_node并清标记
[VERIFIED mykernel/mm/page_alloc/buddy.c::del_page_from_free_list]
```c
del_page_from_free_list(page_s *page, List_hdr_s *area) {
	list_header_delete_node(area, &page->buddy_list);
	__ClearPageBuddy(page);
	page->private = 0;
}
```

### PL18-A44 · 再次删除需要contains门
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
```

### PL18-A45 · 只有contains才再次计数减少
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
				lhdr_p->count--;
				return l_p;
			}

			return NULL;
```

### PL18-A46 · contains依赖遍历
[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```

### PL18-A47 · 遍历header anchor链
[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_foreach]
```c
	#define list_header_foreach(pos, lhdr)	\
				list_for_each(pos, &((lhdr)->anchor))
```

### PL18-A48 · 高阶拆分与buddy node初始
[VERIFIED mykernel/mm/page_alloc/buddy.c::expand]
```c
	while (high > low) {
		high--;
		size >>= 1;
		page_s *p = &page[size];
		INIT_LIST_S(&p->buddy_list);
```

### PL18-A49 · 拆分块入链/标记
[VERIFIED mykernel/mm/page_alloc/buddy.c::expand]
```c

		add_to_free_list(p, zone, high);
		set_buddy_order(p, high);
	}
```

### PL18-A50 · 逐阶搜索并弹出
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
	for (current_order = order; current_order < NR_PAGE_ORDERS; ++current_order) {
		List_hdr_s *area = &zone->free_area[current_order];
		page = get_page_from_free_area(area);
		if (page == NULL)
			continue;
```

### PL18-A51 · 后继清标记/拆分/引用加1
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

```

### PL18-A52 · 可返回非NULL或NULL
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		return page;
	}

	return NULL;
```

### PL18-A53 · 阶数门返回NULL
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
	 */
	if (unlikely(order > MAX_ORDER)) {
		return NULL;
	}
```

### PL18-A54 · 偏好zone顺序常量
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		int prefered_zone_list[] = {ZONE_NORMAL, ZONE_DMA32, ZONE_DMA};
		int max_prefered_idx = NODE_DATA(0)->nr_zones;
		int start_prefered_idx = 0;
```

### PL18-A55 · 选择node zone
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		for (i = start_prefered_idx; i < max_prefered_idx; i++) {
			zone_s *zone = &(NODE_DATA(0)->node_zones[prefered_zone_list[i]]);
```

### PL18-A56 · 实际分配/准备/返回
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
			page_s *page = __rmqueue_smallest(zone, order);
			if (likely(page)) {
				prep_new_page(page, order, gfp);
				return page;
			}
```

### PL18-A57 · 耗尽返回NULL
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
				continue;
		// }
		}
	// }
	return NULL;
```

### PL18-A58 · 实参传递__myos_alloc_pages
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
	page = __myos_alloc_pages(gfp, order);

	return page;
```

### PL18-A59 · folio wrapper丢弃gfp/order
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::folio_alloc_noprof]
```c
folio_s *folio_alloc_noprof(gfp_t gfp, uint order)
{
	return page_folio(alloc_page(GFP_HIGHUSER_MOVABLE));
```

### PL18-A60 · alloc_page/folio_alloc宏
[VERIFIED mykernel/mm/highmem/highmem_macro.h::alloc_page]
```c
	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)

	#define folio_alloc				folio_alloc_noprof
```

### PL18-A61 · page_folio的compound_head宏
[VERIFIED mykernel/mm/page_alloc/page-flags_macro.h::page_folio]
```c
	#define page_folio(p)		(_Generic((p),							\
				const page_s *:		(const folio_s *)_compound_head(p), \
				page_s *:			(folio_s *)_compound_head(p))		\
			)
```

### PL18-A62 · compound_head读取page字段
[VERIFIED mykernel/mm/page_alloc/page-flags.h::_compound_head]
```c
		_compound_head(const page_s *page) {
			ulong head = READ_ONCE(page->compound_head);
			if (unlikely(head & 1))
				return head - 1;
			return (ulong)page;
```

### PL18-A63 · page_ref_inc引用加1
[VERIFIED mykernel/mm/page_alloc/buddy.h::page_ref_inc]
```c
		void
		page_ref_inc(page_s *page) {
			atomic_inc(&page->_refcount);
			// if (page_ref_tracepoint_active(page_ref_mod))
			// 	__page_ref_mod(page, 1);
```

### PL18-A64 · page_ref_dec无release钩子
[VERIFIED mykernel/mm/page_alloc/buddy.h::page_ref_dec]
```c
		void
		page_ref_dec(page_s *page) {
			atomic_dec(&page->_refcount);
			// if (page_ref_tracepoint_active(page_ref_mod))
			// 	__page_ref_mod(page, -1);
```

### PL18-A65 · folio_ref_inc到page_ref_inc
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_ref_inc]
```c
		PREFIX_STATIC_INLINE
		void
		folio_ref_inc(folio_s *folio) {
			page_ref_inc(&folio->page);
		}
```

### PL18-A66 · folio_get到folio_ref_inc
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_get]
```c
		void
		folio_get(folio_s *folio) {
			// VM_BUG_ON_FOLIO(folio_ref_zero_or_close_to_overflow(folio), folio);
			folio_ref_inc(folio);
		}
```

### PL18-A67 · folio_put递减归零调用__folio_put
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_put]
```c
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
		}
```

### PL18-A68 · get_page走page_folio
[VERIFIED mykernel/mm/page_alloc/buddy.h::get_page]
```c
		PREFIX_STATIC_INLINE
		void
		get_page(page_s *page) {
			folio_get(page_folio(page));
		}
```

### PL18-A69 · put_page走folio_put
[VERIFIED mykernel/mm/page_alloc/buddy.h::put_page]
```c
			// if (put_devmap_managed_page(&folio->page))
			// 	return;
			folio_put(page_folio(page));
		}
```

### PL18-A70 · atomic_dec_and_test宏绑定
[VERIFIED mykernel/lock_IPC/atomic/atomic_macro.h::atomic_dec_and_test]
```c
	#define atomic_sub_and_test arch_atomic_sub_and_test

	#define atomic_dec_and_test arch_atomic_dec_and_test

```

### PL18-A71 · arch_atomic_dec_test_zero宏绑定
[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_dec_and_test]
```c
	#define arch_atomic_sub_and_test	arch_atomic_sub_test_zero
	#define arch_atomic_inc_and_test	arch_atomic_inc_test_zero
	#define arch_atomic_dec_and_test	arch_atomic_dec_test_zero
```

### PL18-A72 · 执行递减与条件码检测
[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_dec_test_zero]
```c
		bool
		arch_atomic_dec_test_zero(atomic_t *v) {
			// return GEN_UNARY_RMWcc(LOCK_PREFIX "decl", v->counter, e);
			bool c;
			asm volatile(LOCK_PREFIX
```

### PL18-A73 · 返回条件码归零结果
[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_dec_test_zero]
```c
							"=@cc" "e"(c)
						:
						:	"memory"
						);
			return c;
```

### PL18-A74 · __folio_put的释放接口仅注释
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
void __folio_put(folio_s *folio)
{
	// if (unlikely(folio_is_zone_device(folio)))
	// 	free_zone_device_page(&folio->page);
	// else if (unlikely(folio_test_large(folio)))
```

### PL18-A75 · 末端无活动释放指令
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
	// else if (unlikely(folio_test_large(folio)))
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
}
```

### PL18-A76 · 直接free接口不以ref为门
[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages]
```c
	// if (!head)
	// 	while (order-- > 0)
	// 		free_the_page(page + (1 << order), order);

	__free_pages_ok(page, order);
```

### PL18-A77 · 直接free_pages地址转换
[VERIFIED mykernel/mm/page_alloc/buddy.c::free_pages]
```c
void free_pages(ulong addr, uint order) {
	if (addr != 0)
		__free_pages(virt_to_page((void *)addr), order);
}
```

### PL18-A78 · 分配清零按__GFP_ZERO
[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	// if (init)
	// kernel_init_pages(page, 1 << order);
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```

### PL18-A79 · 准备结果不被判断
[VERIFIED mykernel/mm/page_alloc/buddy.c::__free_pages_ok]
```c
	free_pages_prepare(page, order);
	// if (!free_pages_prepare(page, order, fpi_flags))
	// 	return;
```

### PL18-A80 · zone_init中的spin_lock_init为注释
[VERIFIED mykernel/mm/misc/mm_init.c::zone_init_internals]
```c
	// atomic_long_set(&zone->managed_pages, remaining_pages);
	zone->name = zone_names[idx];
	zone->zone_pgdat = NODE_DATA(0);
	// spin_lock_init(&zone->lock);
	// zone_seqlock_init(zone);
```

### PL18-A81 · mem_init触发memblock_free_all
[VERIFIED mykernel/arch/x86_64/mm/init.c::mem_init]
```c
	after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	/* this will put all memory onto the freelists */
	memblock_free_all();
```

### PL18-A82 · 清Reserved定义体
[VERIFIED mykernel/mm/page_alloc/page-flags.h::__ClearPageReserved]
```c
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__ClearPageReserved(page_s *page) {
			arch___clear_bit(PG_reserved, &(page)->flags);
		}
```

### PL18-A83 · 合并命中后到计算新PFN无删旧链步骤
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
		buddy = find_buddy_page_pfn(page, pfn, order, &buddy_pfn);
		if (buddy == NULL)
			break;

		combined_pfn = buddy_pfn & pfn;
```

### PL18-A84 · free_area阶数上界
[VERIFIED mykernel/mm/page_alloc/mmzone_const.h::NR_PAGE_ORDERS]
```c
	#define MAX_ORDER_NR_PAGES	(1 << MAX_ORDER)

	#define NR_PAGE_ORDERS (MAX_ORDER + 1)
```

## 限界与反证

第17批主线回执R17-S01/S02/S03为消费限定，旧04/05的MR-02已撤回“PageBuddy全局恒假”“全局永不合并”的绝对断言。本批仅讨论选定页和明确初始化条件，不穷尽page_type写入者、memblock全局可达性、所有zone/锁配置或不同order对象。未执行预处理、C/ASM、QEMU、内核/硬件，不修改任何源码。

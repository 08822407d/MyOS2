# 第76批固定源码短锚｜D00不适用

下列均直接取自time@a039d9803ade的实际单一函数、结构、全局或宏定义；每条非空，1–5连续物理行。原始引文只能证明所引局部源码及受限语义，不能替代结构有效性、活动调用时序或真实内核执行。以独立固定源回读核对；未运行预处理、构建、仓库脚本、QEMU、夹具或Graphviz。

## A01｜缓存object_size和offset字段

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		uint				object_size;		/* The size of an object without metadata */
		uint				offset;				/* Free pointer offset */
```
原始物理行 80–81，按定义与路径前提消费。

## A02｜缓存min_partial,size,object_size

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		ulong				min_partial;
		uint				size;				/* The size of an object including metadata */
		uint				object_size;		/* The size of an object without metadata */
```
原始物理行 78–80，按定义与路径前提消费。

## A03｜缓存oo来源字段

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		kmem_cache_order_obj_s	oo;
```
原始物理行 86–86，按定义与路径前提消费。

## A04｜缓存inuse是布局，align是另一字段

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		uint				inuse;				/* Offset to metadata */
		uint				align;				/* Alignment */
```
原始物理行 92–93，按定义与路径前提消费。

## A05｜slab.inuse16位/objects15位/frozen1位

[VERIFIED mykernel/mm/kmalloc/slub_types.h::slab]
```c
					uint	inuse	: 16;
					uint	objects	: 15;
					uint	frozen	: 1;
```
原始物理行 30–32，按定义与路径前提消费。

## A06｜slab空闲对象头字段

[VERIFIED mykernel/mm/kmalloc/slub_types.h::slab]
```c
			void			*freelist;		/* first free object */
```
原始物理行 26–26，按定义与路径前提消费。

## A07｜slab指向cache字段

[VERIFIED mykernel/mm/kmalloc/slub_types.h::slab]
```c
		kmem_cache_s		*slab_cache;
```
原始物理行 16–16，按定义与路径前提消费。

## A08｜oo.obj_nr/order均16位

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache_order_objects]
```c
			obj_nr	: 16,
			order	: 16;
```
原始物理行 67–68，按定义与路径前提消费。

## A09｜单node锁/partial/count统计/full

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache_node]
```c
		spinlock_t		list_lock;
		List_hdr_s		partial;
		atomic_long_t	nr_slabs;
		atomic_long_t	total_objects;
		List_hdr_s		full;
```
原始物理行 53–57，按定义与路径前提消费。

## A10｜取页容量除以对象实际步长

[VERIFIED mykernel/mm/kmalloc/slub.c::order_objects]
```c
	return ((uint)PAGE_SIZE << order) / size;
```
原始物理行 9–9，按定义与路径前提消费。

## A11｜oo_make保存order和obj_nr

[VERIFIED mykernel/mm/kmalloc/slub.c::oo_make]
```c
		.order		= order,
		.obj_nr		= order_objects(order, size),
```
原始物理行 15–16，按定义与路径前提消费。

## A12｜默认最小目标对象数4

[VERIFIED mykernel/mm/kmalloc/slub.c::slub_min_objects]
```c
static uint slub_min_objects	= 4;
```
原始物理行 344–344，按定义与路径前提消费。

## A13｜阶搜索局部及fraction64/min_objnr

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_order]
```c
		min_order = (int)slub_min_order;
	uint
		fraction = 64,
		min_objnr = slub_min_objects;
```
原始物理行 351–354，按定义与路径前提消费。

## A14｜阶搜索起点包含get_order

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_order]
```c
	for (order = max(min_order, get_order(min_objnr * size));
			order < NR_PAGE_ORDERS; order++) {
```
原始物理行 357–358，按定义与路径前提消费。

## A15｜余数门及终止条件

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_order]
```c
		uint slab_size = (uint)PAGE_SIZE << order;
		uint rem = slab_size % size;
		if (rem <= slab_size / fraction)
			break;
```
原始物理行 360–363，按定义与路径前提消费。

## A16｜初始未对齐object_size

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	uint order, size = s->object_size;
```
原始物理行 377–377，按定义与路径前提消费。

## A17｜先按指针宽度对齐

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	size = ALIGN(size, sizeof(void *));
```
原始物理行 383–383，按定义与路径前提消费。

## A18｜cache.inuse为对齐布局值

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	s->inuse = size;
```
原始物理行 388–388，按定义与路径前提消费。

## A19｜对象步长二次对齐与order

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	size = ALIGN(size, s->align);
	s->size = size;
	order = calculate_order(size);
```
原始物理行 394–396，按定义与路径前提消费。

## A20｜存oo并按obj_nr非零判接受

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_calculate_sizes]
```c
	s->oo = oo_make(order, size);

	return !!oo_objects(s->oo);
```
原始物理行 416–418，按定义与路径前提消费。

## A21｜cache open调用calculate_sizes并错误门

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_kmem_cache_open]
```c
	s->flags = flags;
	if (calculate_sizes(s) <= 0)
		goto error;
```
原始物理行 425–427，按定义与路径前提消费。

## A22｜partial下限/上限生产公式

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_kmem_cache_open]
```c
	s->min_partial = min_t(ulong, MAX_PARTIAL, ilog2(s->size) / 2);
	s->min_partial = max_t(ulong, MIN_PARTIAL, s->min_partial);
```
原始物理行 433–434，按定义与路径前提消费。

## A23｜cache partial/full头部初始化

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_kmem_cache_open]
```c
		INIT_LIST_HEADER_S(&node_cache->partial);
		INIT_LIST_HEADER_S(&node_cache->full);
```
原始物理行 439–440，按定义与路径前提消费。

## A24｜partial最小常量4

[VERIFIED mykernel/mm/kmalloc/slub_const.h::MIN_PARTIAL]
```c
	#define MIN_PARTIAL 4
```
原始物理行 57–57，按定义与路径前提消费。

## A25｜partial上限常量8

[VERIFIED mykernel/mm/kmalloc/slub_const.h::MAX_PARTIAL]
```c
	#define MAX_PARTIAL 8
```
原始物理行 63–63，按定义与路径前提消费。

## A26｜slab.objects 15位的旁证常量

[VERIFIED mykernel/mm/kmalloc/slub_const.h::MAX_OBJS_PER_PAGE]
```c
	#define MAX_OBJS_PER_PAGE	32767 /* since slab.objects is u15 */
```
原始物理行 64–64，按定义与路径前提消费。

## A27｜若有构造函数才调用ctor

[VERIFIED mykernel/mm/kmalloc/slub.c::setup_object]
```c
	if (unlikely(s->ctor)) {
		// kasan_unpoison_new_object(s, object);
		s->ctor(object);
```
原始物理行 77–79，按定义与路径前提消费。

## A28｜取链指针先检再读取object加offset

[VERIFIED mykernel/mm/kmalloc/slub.c::get_freepointer]
```c
	while (IS_INVAL_PTR((const void *)(object + s->offset)));
	return (void *)*(ulong *)(object + s->offset);
```
原始物理行 27–28，按定义与路径前提消费。

## A29｜freepointer写入object+offset

[VERIFIED mykernel/mm/kmalloc/slub.c::set_freepointer]
```c
	ulong freeptr_addr = (ulong)this_obj + s->offset;
	while (IS_INVAL_PTR((const void *)freeptr_addr));
	*(void **)freeptr_addr = next_obj;
```
原始物理行 33–35，按定义与路径前提消费。

## A30｜新slab计数及cache身份初始化

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	slab->objects = oo_objects(s->oo);
	slab->inuse = 0;
	slab->frozen = 0;
	slab->slab_cache = s;
```
原始物理行 126–129，按定义与路径前提消费。

## A31｜首槽地址与freelist头设置

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	start = (void *)page_to_virt(&((folio_s *)slab)->page);
	slab->freelist = start;
```
原始物理行 135–136，按定义与路径前提消费。

## A32｜槽迭代按实际s.size步长

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	for (idx = 0, p = start; idx < slab->objects - 1; idx++) {
		next = p + s->size;		// next_freelist_entry()
```
原始物理行 137–138，按定义与路径前提消费。

## A33｜ctor在next槽且前一槽写freelink

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
		next = setup_object(s, next);
		set_freepointer(s, p, next);
		p = next;
```
原始物理行 139–141，按定义与路径前提消费。

## A34｜末槽写next NULL

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	set_freepointer(s, p, NULL);
```
原始物理行 143–143，按定义与路径前提消费。

## A35｜cache普通生产者size/align/ctor

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_cache]
```c
	s->size = s->object_size = object_size;
	s->align = align;
	s->ctor = ctor;
```
原始物理行 47–49，按定义与路径前提消费。

## A36｜引导cache尺寸及align生产

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_boot_cache]
```c
	s->size = s->object_size = size;
	s->align = calculate_alignment(flags, ARCH_KMALLOC_MINALIGN, size);
```
原始物理行 127–128，按定义与路径前提消费。

## A37｜ALIGN宏是对齐式入口

[VERIFIED mykernel/include/linux/kernel/align.h::ALIGN]
```c
	#define ALIGN(x, a)				__ALIGN_KERNEL((x), (a))
```
原始物理行 8–8，按定义与路径前提消费。

## A38｜ALIGN最近掩码公式

[VERIFIED mykernel/include/uapi/linux/const.h::__ALIGN_KERNEL_MASK]
```c
	#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))
```
原始物理行 31–31，按定义与路径前提消费。

## A39｜ilog2常量/非常量选择

[VERIFIED mykernel/include/linux/kernel/log2.h::ilog2]
```c
	#define ilog2(n) (						\
				__builtin_constant_p(n) ?	\
				((n) < 2 ? 0 :				\
				63 - __builtin_clzll(n)) :	\
				(sizeof(n) <= 4) ?			\
```
原始物理行 149–153，按定义与路径前提消费。

## A40｜partial最小值类型宏

[VERIFIED mykernel/include/linux/kernel/minmax.h::min_t]
```c
	#define min_t(type, x, y)	__cmp((type)(x), (type)(y), <)
```
原始物理行 114–114，按定义与路径前提消费。

## A41｜partial最大值类型宏

[VERIFIED mykernel/include/linux/kernel/minmax.h::max_t]
```c
	#define max_t(type, x, y)	__cmp((type)(x), (type)(y), >)
```
原始物理行 123–123，按定义与路径前提消费。

## A42｜get_order常量门前置

[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
		int get_order(unsigned long size) {
			if (__builtin_constant_p(size)) {
				if (!size)
					return BITS_PER_LONG - PAGE_SHIFT;
```
原始物理行 30–33，按定义与路径前提消费。

## A43｜calculation order alias macro

[VERIFIED mykernel/mm/kmalloc/slub.c::calculate_order]
```c
#define calculate_order simple_calculate_order
```
原始物理行 367–367；按宏定义消费。

## A44｜sizes calculation alias macro

[VERIFIED mykernel/mm/kmalloc/slub.c::calculate_sizes]
```c
#define calculate_sizes simple_calculate_sizes
```
原始物理行 420–420；按宏定义消费。

## A45｜cache open alias macro

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_open]
```c
#define kmem_cache_open simple_kmem_cache_open
```
原始物理行 448–448；按宏定义消费。

## A46｜oo order field accessor macro

[VERIFIED mykernel/mm/kmalloc/slub.c::oo_order]
```c
#define oo_order(x)		((x).order)
```
原始物理行 21–21；按宏定义消费。

## A47｜oo object-count accessor macro

[VERIFIED mykernel/mm/kmalloc/slub.c::oo_objects]
```c
#define oo_objects(x)	((x).obj_nr)
```
原始物理行 22–22；按宏定义消费。

## B01｜新slab上线要取得node锁

[VERIFIED mykernel/mm/kmalloc/slub.c::online_new_empty_slab]
```c
	spin_lock_irqsave(&n->list_lock, irqflags);

	add_to_partial(n, slab);
```
原始物理行 153–155，按定义与路径前提消费。

## B02｜上线增nr_slabs和total_objects

[VERIFIED mykernel/mm/kmalloc/slub.c::online_new_empty_slab]
```c
	atomic_long_inc(&n->nr_slabs);
	atomic_long_add(slab->objects, &n->total_objects);
```
原始物理行 156–157，按定义与路径前提消费。

## B03｜上线后解锁

[VERIFIED mykernel/mm/kmalloc/slub.c::online_new_empty_slab]
```c
	spin_unlock_irqrestore(&n->list_lock, irqflags);
```
原始物理行 159–159，按定义与路径前提消费。

## B04｜取首对象后更新freelist和inuse

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_single_from_partial]
```c
	void *object = slab->freelist;
	slab->freelist = get_freepointer(s, object);
	slab->inuse++;
```
原始物理行 170–172，按定义与路径前提消费。

## B05｜抽取helper返回对象指针

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_single_from_partial]
```c
	return object;
```
原始物理行 173–173，按定义与路径前提消费。

## B06｜partial遍历前锁定

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
	spin_lock_irqsave(&n->list_lock, flags);
```
原始物理行 187–187，按定义与路径前提消费。

## B07｜安全迭代与抽取实参求值

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
	iter_partial_list(slab, slab_next, &n->partial, slab_list)
		if ((object = alloc_single_from_partial(s, slab)) != NULL) {
```
原始物理行 189–190，按定义与路径前提消费。

## B08｜达到满值后从partial移到full

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
			if (slab->inuse == slab->objects) {
				remove_from_partial(n, slab);
				add_to_full(n, slab);
			}
```
原始物理行 191–194，按定义与路径前提消费。

## B09｜成功抽取后跳出迭代

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
			break;
```
原始物理行 195–195，按定义与路径前提消费。

## B10｜解锁后返回对象

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
	spin_unlock_irqrestore(&n->list_lock, flags);

	return object;
```
原始物理行 198–200，按定义与路径前提消费。

## B11｜slab_alloc以cache node身份操作

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	kmem_cache_node_s *n = &s->node;
```
原始物理行 208–208，按定义与路径前提消费。

## B12｜取不到object才创建slab，创建NULL返回

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	while ((object = get_from_partial_list(s, n)) == NULL) {
		slab_s *slab = alloc_init_new_slab(s, gfpflags);
		if (unlikely(!slab))
			return NULL;
```
原始物理行 210–213，按定义与路径前提消费。

## B13｜上线后回到while重试取对象

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
		online_new_empty_slab(s, n, slab);
	}
```
原始物理行 215–216，按定义与路径前提消费。

## B14｜后置hook在成功取得对象后

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	slab_post_alloc_hook(s, gfpflags, &object);

	return object;
```
原始物理行 222–224，按定义与路径前提消费。

## B15｜新slab上游NULL返回门

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	slab_s *slab = alloc_slab(s, flags);
	if (unlikely(!slab))
		return NULL;
```
原始物理行 121–123，按定义与路径前提消费。

## B16｜真实page alloc返回及NULL门

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
	if (!folio)
		return NULL;
```
原始物理行 105–107，按定义与路径前提消费。

## B17｜add_to_full宏真正指向full链

[VERIFIED mykernel/mm/kmalloc/slub.c::add_to_full]
```c
#define add_to_full(n, slab)	\
			list_header_add_to_head(&((n)->full), &((slab)->slab_list))
```
原始物理行 45–46，按定义与路径前提消费。

## B18｜remove_from_partial宏调用delete_node

[VERIFIED mykernel/mm/kmalloc/slub.c::remove_from_partial]
```c
#define remove_from_partial(n, slab)	\
			list_header_delete_node(&((n)->partial), &((slab)->slab_list))
```
原始物理行 67–68，按定义与路径前提消费。

## B19｜add_to_partial宏真正指向partial

[VERIFIED mykernel/mm/kmalloc/slub.c::add_to_partial]
```c
#define add_to_partial(n, slab)	\
			list_header_add_to_head(&((n)->partial), &((slab)->slab_list))
```
原始物理行 61–62，按定义与路径前提消费。

## B20｜remove_from_full宏调用delete_node

[VERIFIED mykernel/mm/kmalloc/slub.c::remove_from_full]
```c
#define remove_from_full(n, slab)	\
			list_header_delete_node(&((n)->full), &((slab)->slab_list))
```
原始物理行 51–52，按定义与路径前提消费。

## B21｜list_header头插请求后count加一

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```
原始物理行 647–648，按定义与路径前提消费。

## B22｜成员存在才删除和减count

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```
原始物理行 684–687，按定义与路径前提消费。

## B23｜contains要遍历实际链接

[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
			}
```
原始物理行 637–640，按定义与路径前提消费。

## B24｜迭代别名宏指向safe实际定义

[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container_safe]
```c
	#define list_header_for_each_container_safe(pos, n, header, member)	\
				list_for_each_entry_safe(pos, n, &((header)->anchor), member)
```
原始物理行 349–350，按定义与路径前提消费。

## B25｜safe遍历保存后继节点

[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each_entry_safe]
```c
	#define list_for_each_entry_safe(pos, n, head, member)					\
				for (pos = list_first_entry(head, typeof(*pos), member),	\
					n = list_next_entry(pos, member);						\
					!list_entry_is_head(pos, head, member); 				\
					pos = n, n = list_next_entry(n, member))
```
原始物理行 260–264，按定义与路径前提消费。

## B26｜partial/full空链头部数值初值

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_HEADER_S]
```c
			WRITE_ONCE(lhdr->count, 0);
			INIT_LIST_S(&lhdr->anchor);
```
原始物理行 156–157，按定义与路径前提消费。

## B27｜List_hdr中的anchor与ulong count

[VERIFIED mykernel/lib/list/double_list_types.h::list_hdr]
```c
		List_s		anchor;
		ulong		count;
```
原始物理行 14–15，按定义与路径前提消费。

## B28｜真实节点前后指针改写

[VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
```c
			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
```
原始物理行 193–196，按定义与路径前提消费。

## B29｜摘节点后重置

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
```
原始物理行 281–282，按定义与路径前提消费。

## B30｜未找到时返回NULL而非删除成功

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
			return NULL;
```
原始物理行 690–690，按定义与路径前提消费。

## B31｜partial helper返回取出的对象

[VERIFIED mykernel/mm/kmalloc/slub.c::get_from_partial_list]
```c
	return object;
```
原始物理行 200–200，按定义与路径前提消费。

## B32｜取到下一freelist指针值

[VERIFIED mykernel/mm/kmalloc/slub.c::get_freepointer]
```c
	return (void *)*(ulong *)(object + s->offset);
```
原始物理行 28–28，按定义与路径前提消费。

## B33｜partial iterator alias macro

[VERIFIED mykernel/mm/kmalloc/slub.c::iter_partial_list]
```c
#define iter_partial_list list_header_for_each_container_safe
```
原始物理行 180–180；按宏定义消费。

## C01｜归还时inuse--并把对象压回freelist

[VERIFIED mykernel/mm/kmalloc/slub.c::free_single_to_partial]
```c
	slab->inuse--;
	set_freepointer(s, object, slab->freelist);
	slab->freelist = object;
```
原始物理行 272–274，按定义与路径前提消费。

## C02｜归还先持锁并按归还前满值迁移

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
	spin_lock_irqsave(&n->list_lock, flags);
	/* If object is in a full slab, move this slab to free list */
	if (slab->inuse == slab->objects) {
```
原始物理行 282–284，按定义与路径前提消费。

## C03｜full摘节点再partial头插

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
		remove_from_full(n, slab);
		add_to_partial(n, slab);
```
原始物理行 285–286，按定义与路径前提消费。

## C04｜freelist归还后解锁

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
	free_single_to_partial(s, slab, object);
	spin_unlock_irqrestore(&n->list_lock, flags);
```
原始物理行 288–289，按定义与路径前提消费。

## C05｜slab_free先取缓存node然后归还object

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	kmem_cache_node_s *n = &s->node;
	free_to_partial_list(s, n, slab, object);
```
原始物理行 294–295，按定义与路径前提消费。

## C06｜下线OR门与partial阈值

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	if (slab->inuse == 0 ||
		n->partial.count >= s->min_partial) {
```
原始物理行 302–303，按定义与路径前提消费。

## C07｜满足OR后请求下线及free_slab

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	    offline_exceeded_empty_slab(s, n, slab);
	    free_slab(s, slab);
```
原始物理行 305–306，按定义与路径前提消费。

## C08｜下线重新加锁并请求remove_from_partial

[VERIFIED mykernel/mm/kmalloc/slub.c::offline_exceeded_empty_slab]
```c
	spin_lock_irqsave(&n->list_lock, flags);

	remove_from_partial(n, slab);
```
原始物理行 259–261，按定义与路径前提消费。

## C09｜下线统计减量不按remove结果分支

[VERIFIED mykernel/mm/kmalloc/slub.c::offline_exceeded_empty_slab]
```c
	atomic_long_dec(&n->nr_slabs);
	atomic_long_sub(slab->objects, &n->total_objects);

```
原始物理行 262–264，按定义与路径前提消费。

## C10｜下线函数解锁

[VERIFIED mykernel/mm/kmalloc/slub.c::offline_exceeded_empty_slab]
```c
	spin_unlock_irqrestore(&n->list_lock, flags);
```
原始物理行 265–265，按定义与路径前提消费。

## C11｜空slab页头门可能不返回

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	while (oo_order(s->oo) == 0 && folio->page.flag_defs.PG_head != 0);
```
原始物理行 239–239，按定义与路径前提消费。

## C12｜下线读取folio实际order

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	int order = folio_order(folio);
	int pages = 1 << order;
```
原始物理行 241–242，按定义与路径前提消费。

## C13｜清mapping及slab标志请求

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	folio->mapping = NULL;
	/* Make the mapping reset visible before clearing the flag */
	smp_wmb();
	__folio_clear_slab(folio);
```
原始物理行 245–248，按定义与路径前提消费。

## C14｜止于__free_pages页归还接口

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	__free_pages(&folio->page, order);
```
原始物理行 251–251，按定义与路径前提消费。

## C15｜释放入口先对virt_to_slab实参求值

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]
```c
	slab_free(s, virt_to_slab(x), x);
```
原始物理行 314–314，按定义与路径前提消费。

## C16｜摘链成功才减少链表count

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
				lhdr_p->count--;
				return l_p;
```
原始物理行 686–687，按定义与路径前提消费。

## C17｜实际阈值公式生产

[VERIFIED mykernel/mm/kmalloc/slub.c::simple_kmem_cache_open]
```c
	s->min_partial = min_t(ulong, MAX_PARTIAL, ilog2(s->size) / 2);
	s->min_partial = max_t(ulong, MIN_PARTIAL, s->min_partial);
```
原始物理行 433–434，按定义与路径前提消费。

## C18｜阈值下界4

[VERIFIED mykernel/mm/kmalloc/slub_const.h::MIN_PARTIAL]
```c
	#define MIN_PARTIAL 4
```
原始物理行 57–57，按定义与路径前提消费。

## C19｜阈值上界8

[VERIFIED mykernel/mm/kmalloc/slub_const.h::MAX_PARTIAL]
```c
	#define MAX_PARTIAL 8
```
原始物理行 63–63，按定义与路径前提消费。

## C20｜只有归还前满值才迁移full->partial

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
	if (slab->inuse == slab->objects) {
		remove_from_full(n, slab);
		add_to_partial(n, slab);
	}
```
原始物理行 284–287，按定义与路径前提消费。

## C21｜锁外数量达到阈值即可进下线

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
		n->partial.count >= s->min_partial) {
```
原始物理行 303–303，按定义与路径前提消费。

## C22｜锁内归还先于锁外OR

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	free_to_partial_list(s, n, slab, object);
```
原始物理行 295–295，按定义与路径前提消费。

## C23｜正常partial头插使count增加

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			lhdr_p->count++;
```
原始物理行 648–648，按定义与路径前提消费。

## C24｜成功删除返回l_p，调用方未检查

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
				return l_p;
			}

			return NULL;
```
原始物理行 687–690，按定义与路径前提消费。

## C25｜未依据删除结果守护统计减量

[VERIFIED mykernel/mm/kmalloc/slub.c::offline_exceeded_empty_slab]
```c
	remove_from_partial(n, slab);
	atomic_long_dec(&n->nr_slabs);
	atomic_long_sub(slab->objects, &n->total_objects);
```
原始物理行 261–263，按定义与路径前提消费。

## C26｜清标志后请求page free

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	__folio_clear_slab(folio);
	// mm_account_reclaimed_pages(pages);
	// unaccount_slab(slab, order, s);
	__free_pages(&folio->page, order);
```
原始物理行 248–251，按定义与路径前提消费。

## C27｜归还锁结束先于slab_free判断

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
	spin_unlock_irqrestore(&n->list_lock, flags);
```
原始物理行 289–289，按定义与路径前提消费。


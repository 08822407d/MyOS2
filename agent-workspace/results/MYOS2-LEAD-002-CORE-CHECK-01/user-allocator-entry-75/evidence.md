# 第75批｜固定time单一定义源码证据

所有锚均为GitHub连接器从固定time@a039d9803ade读取的1–5个真实连续物理行；单一函数/宏/全局定义内有效。D01–D05仅纠正旧74 B12宏归属及尺寸链，原74其余73条未全量复核。历史源码文本、默认分支搜索和内核运行均不得替代本处固定源。

## D01｜R74 B12所引真正宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE_ORDER]
```c
	#define THREAD_SIZE_ORDER		3
```
固定源物理行13–13；依适用条件消费。

## D02｜独立THREAD_SIZE宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE]
```c
	#define THREAD_SIZE				(PAGE_SIZE << THREAD_SIZE_ORDER)
```
固定源物理行14–14；依适用条件消费。

## D03｜独立PAGE_SIZE宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定源物理行10–10；依适用条件消费。

## D04｜独立PAGE_SHIFT宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```
固定源物理行9–9；依适用条件消费。

## D05｜C分支_AC实际拼接定义

[VERIFIED mykernel/include/uapi/linux/const.h::_AC]
```c
	#	define _AC(X,Y)		(X##Y)
```
固定源物理行20–20；依适用条件消费。

## A01｜kmalloc实际双路大小判断

[VERIFIED mykernel/mm/kmalloc/slub.h::kmalloc]
```c
			if (size > KMALLOC_MAX_CACHE_SIZE)
				ret_val = kmalloc_large(size, flags);
			else
				ret_val = __kmalloc(size, flags);
```
固定源物理行128–131；依适用条件消费。

## A02｜直接__kmalloc大尺寸死循环门

[VERIFIED mykernel/mm/kmalloc/slab_common.c::__kmalloc]
```c
	while (unlikely(size > KMALLOC_MAX_CACHE_SIZE));
```
固定源物理行248–248；依适用条件消费。

## A03｜小块缓存指针NULL检查与调用

[VERIFIED mykernel/mm/kmalloc/slab_common.c::__kmalloc]
```c
	kmem_cache_s *s = kmalloc_slab(size, flags);
	if (s != NULL)
		ret = kmem_cache_alloc(s, flags);
```
固定源物理行251–253；依适用条件消费。

## A04｜零尺寸及上界拒绝

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_slab]
```c
	if (size == 0 || WARN_ON_ONCE(size > KMALLOC_MAX_CACHE_SIZE))
		return NULL;
```
固定源物理行181–182；依适用条件消费。

## A05｜192处选择小尺寸索引

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_slab]
```c
	else if (size <= 192)
		index = size_to_index(size);
	else
		index = fls(size - 1);
```
固定源物理行183–186；依适用条件消费。

## A06｜按索引返回cache指针

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_slab]
```c
	return kmalloc_caches[index];
```
固定源物理行188–188；依适用条件消费。

## A07｜索引helper自身非法参数不返回

[VERIFIED mykernel/mm/kmalloc/slab_common.c::size_to_index]
```c
	while (bytes <= 0 || bytes > 192);
```
固定源物理行160–160；依适用条件消费。

## A08｜尺寸索引小值分支

[VERIFIED mykernel/mm/kmalloc/slab_common.c::size_to_index]
```c
	switch ((bytes - 1) / 8) {
		case 0:			return 3;
		case 1:			return 4;
		case 2 ... 3:	return 5;
		case 4 ... 7:	return 6;
```
固定源物理行162–166；依适用条件消费。

## A09｜尺寸索引大值分支

[VERIFIED mykernel/mm/kmalloc/slab_common.c::size_to_index]
```c
		case 8 ... 11:	return 1;
		case 12 ... 15:	return 7;
		case 16 ... 23:	return 2;
		default:		return (u8)-1;
```
固定源物理行167–170；依适用条件消费。

## A10｜cache指针数组定义

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_caches]
```c
kmalloc_caches[KMALLOC_SHIFT_HIGH + 1] __ro_after_init =
{ /* initialization for https://bugs.llvm.org/show_bug.cgi?id=42570 */ };
```
固定源物理行154–155；依适用条件消费。

## A11｜实际96/192/8/16/32 cache表

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_info]
```c
	INIT_KMALLOC_INFO(96,		96),
	INIT_KMALLOC_INFO(192,		192),
	INIT_KMALLOC_INFO(8,		8),
	INIT_KMALLOC_INFO(16,		16),
	INIT_KMALLOC_INFO(32,		32),
```
固定源物理行203–207；依适用条件消费。

## A12｜4096和8192表槽并列不代表都可选

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_info]
```c
	INIT_KMALLOC_INFO(4096,		4k),
	INIT_KMALLOC_INFO(8192,		8k),
	INIT_KMALLOC_INFO(16384,	16k),
```
固定源物理行214–216；依适用条件消费。

## A13｜初始化按index循环填cache

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_kmalloc_caches]
```c
	for (int idx = 1; idx <= KMALLOC_SHIFT_HIGH; idx++)
		if (!kmalloc_caches[idx])
			kmalloc_caches[idx] = create_kmalloc_cache(
					kmalloc_info[idx].name, kmalloc_info[idx].size, flags);
```
固定源物理行236–239；依适用条件消费。

## A14｜cache初始化结束声明

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_kmalloc_caches]
```c
	slab_state = UP;
```
固定源物理行242–242；依适用条件消费。

## A15｜真正缓存大小上界宏

[VERIFIED mykernel/mm/kmalloc/slub_const.h::KMALLOC_MAX_CACHE_SIZE]
```c
	#define KMALLOC_MAX_CACHE_SIZE	PAGE_SIZE
```
固定源物理行79–79；依适用条件消费。

## A16｜真正缓存表高位宏

[VERIFIED mykernel/mm/kmalloc/slub_const.h::KMALLOC_SHIFT_HIGH]
```c
	#define KMALLOC_SHIFT_HIGH		(PAGE_SHIFT + 1)
```
固定源物理行71–71；依适用条件消费。

## A17｜cache初始化函数调用点

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_init]
```c
	create_kmalloc_caches(0);
```
固定源物理行508–508；依适用条件消费。

## A18｜fls取最高位位置后加一

[VERIFIED mykernel/arch/x86_64/insns/bitops_arch.h::fls]
```c
			return r + 1;
```
固定源物理行223–223；依适用条件消费。

## A19｜fls最近实现BSR整数归纳前提

[VERIFIED mykernel/arch/x86_64/insns/bitops_arch.h::fls]
```c
			asm (	"bsrl	%1,		%0		\n\t"
				:	"=r" (r)
				:	"rm" (x),
					"0" (-1)
				);
```
固定源物理行217–221；依适用条件消费。

## A20｜192上方通过fls索引

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_slab]
```c
		index = fls(size - 1);
```
固定源物理行186–186；依适用条件消费。

## A21｜非恒量分支阶次来源

[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
			size--;
			size >>= PAGE_SHIFT;

			return fls64(size);
```
固定源物理行40–43；依适用条件消费。

## A22｜fls64返回最高设置位序号

[VERIFIED mykernel/arch/x86_64/insns/bitops_arch.h::fls64]
```c
			return bitpos + 1;
```
固定源物理行249–249；依适用条件消费。

## A23｜大块页分配接口

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_large]
```c
	page_s *page = alloc_pages(flags | __GFP_COMP, get_order(size));
	if (page) {
		ptr = (void *)page_to_virt(page);
```
固定源物理行268–270；依适用条件消费。

## A24｜slab取页失败返回

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
	if (!folio)
		return NULL;
```
固定源物理行105–107；依适用条件消费。

## A25｜初始化函数起点

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_init]
```c
void __init kmem_cache_init(void)
{
	static __initdata kmem_cache_s
```
固定源物理行487–489；依适用条件消费。

## A26｜task大小为符号sizeof

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_task_struct_node]
```c
	return kzalloc(sizeof(task_s), GFP_KERNEL);
```
固定源物理行69–69；依适用条件消费。

## A27｜task栈选定尺寸输入

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	stack = kmalloc(THREAD_SIZE, GFP_KERNEL);
```
固定源物理行94–94；依适用条件消费。

## A28｜cache不在时不调用kmem_cache_alloc

[VERIFIED mykernel/mm/kmalloc/slab_common.c::__kmalloc]
```c
	if (s != NULL)
		ret = kmem_cache_alloc(s, flags);
```
固定源物理行252–253；依适用条件消费。

## A29｜192及以下真实索引入口

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_slab]
```c
		index = size_to_index(size);
```
固定源物理行184–184；依适用条件消费。

## A30｜cache的object_size成员

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		uint				object_size;		/* The size of an object without metadata */
```
固定源物理行80–80；依适用条件消费。

## A31｜实际cache创建参数来自表

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_kmalloc_caches]
```c
					kmalloc_info[idx].name, kmalloc_info[idx].size, flags);
```
固定源物理行239–239；依适用条件消费。

## B01｜kzalloc通过附加清零标志

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
			return kmalloc(size, flags | __GFP_ZERO);
```
固定源物理行171–171；依适用条件消费。

## B02｜清零位typed宏

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::__GFP_ZERO]
```c
	#define __GFP_ZERO				((__force gfp_t)___GFP_ZERO)
```
固定源物理行226–226；依适用条件消费。

## B03｜清零位原始值

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::___GFP_ZERO]
```c
	#define ___GFP_ZERO					0x100u
```
固定源物理行23–23；依适用条件消费。

## B04｜GFP_KERNEL实际宏来源

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_KERNEL]
```c
	#define GFP_KERNEL				(__GFP_RECLAIM | __GFP_IO | __GFP_FS)
```
固定源物理行309–309；依适用条件消费。

## B05｜partial到新slab及NULL失败门

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	while ((object = get_from_partial_list(s, n)) == NULL) {
		slab_s *slab = alloc_init_new_slab(s, gfpflags);
		if (unlikely(!slab))
			return NULL;
```
固定源物理行210–213；依适用条件消费。

## B06｜分配slab时NULL退出

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_init_new_slab]
```c
	slab_s *slab = alloc_slab(s, flags);
	if (unlikely(!slab))
		return NULL;
```
固定源物理行121–123；依适用条件消费。

## B07｜slab请求页时使用转换标志

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
	gfp_t alloc_gfp = prepare_alloc_flags(s, flags);
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
	if (!folio)
```
固定源物理行104–106；依适用条件消费。

## B08｜页分配标志掩码与cache allocflags

[VERIFIED mykernel/mm/kmalloc/slub.c::prepare_alloc_flags]
```c
	flags &= (GFP_RECLAIM_MASK | GFP_CONSTRAINT_MASK) & gfp_allowed_mask;
	flags |= s->allocflags;
```
固定源物理行88–89；依适用条件消费。

## B09｜slab正常取得对象后钩子与返回

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	slab_post_alloc_hook(s, gfpflags, &object);

	return object;
```
固定源物理行222–224；依适用条件消费。

## B10｜小缓存清零长度来自object_size

[VERIFIED mykernel/mm/kmalloc/slub.h::slab_post_alloc_hook]
```c
			uint zero_size = s->object_size;
```
固定源物理行202–202；依适用条件消费。

## B11｜小缓存真实memset调用门

[VERIFIED mykernel/mm/kmalloc/slub.h::slab_post_alloc_hook]
```c
			if (flags & __GFP_ZERO)
				memset(*obj_p, 0, zero_size);
```
固定源物理行246–247；依适用条件消费。

## B12｜object_size存储成员

[VERIFIED mykernel/mm/kmalloc/slub_types.h::kmem_cache]
```c
		uint				object_size;		/* The size of an object without metadata */
```
固定源物理行80–80；依适用条件消费。

## B13｜缓存建立时设置对象长度

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_boot_cache]
```c
	s->size = s->object_size = size;
```
固定源物理行127–127；依适用条件消费。

## B14｜普通cache建立时设置对象长度

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_cache]
```c
	s->size = s->object_size = object_size;
```
固定源物理行47–47；依适用条件消费。

## B15｜页分配成功时设置大块指针及清零

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_large]
```c
	if (page) {
		ptr = (void *)page_to_virt(page);
		if (flags & __GFP_ZERO)
			memset(ptr, 0, size);
	}
```
固定源物理行269–273；依适用条件消费。

## B16｜大块仅明确memset请求size字节

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_large]
```c
		if (flags & __GFP_ZERO)
			memset(ptr, 0, size);
```
固定源物理行271–272；依适用条件消费。

## B17｜复合页标志传递

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::__GFP_COMP]
```c
	#define __GFP_COMP				((__force gfp_t)___GFP_COMP)
```
固定源物理行225–225；依适用条件消费。

## B18｜cache分配入口真实callee

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_alloc]
```c
	return slab_alloc(s, gfpflags);
```
固定源物理行228–228；依适用条件消费。

## B19｜task的清零请求来自kzalloc

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_task_struct_node]
```c
	return kzalloc(sizeof(task_s), GFP_KERNEL);
```
固定源物理行69–69；依适用条件消费。

## B20｜stack kmalloc没有由此显式传ZERO

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	stack = kmalloc(THREAD_SIZE, GFP_KERNEL);
```
固定源物理行94–94；依适用条件消费。

## B21｜slab取页NULL直接向上返回

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
	if (!folio)
		return NULL;
```
固定源物理行106–107；依适用条件消费。

## B22｜__kmalloc在s==NULL时保留NULL并返回

[VERIFIED mykernel/mm/kmalloc/slab_common.c::__kmalloc]
```c
	kmem_cache_s *s = kmalloc_slab(size, flags);
	if (s != NULL)
		ret = kmem_cache_alloc(s, flags);

	// trace_kmalloc(caller, ret, size, s->size, flags, node);
```
固定源物理行251–255；依适用条件消费。

## B23｜新slab失败立即返回NULL

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
		if (unlikely(!slab))
			return NULL;
```
固定源物理行212–213；依适用条件消费。

## B24｜不同分支ret_val返回

[VERIFIED mykernel/mm/kmalloc/slub.h::kmalloc]
```c
			return ret_val;
```
固定源物理行133–133；依适用条件消费。

## B25｜对象需完成hook才返回

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	return object;
```
固定源物理行224–224；依适用条件消费。

## B26｜cache创建结束源码动作

[VERIFIED mykernel/mm/kmalloc/slab_common.c::create_kmalloc_caches]
```c
	slab_state = UP;
```
固定源物理行242–242；依适用条件消费。

## C01｜NULL直接返回不分类

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			if (unlikely(object == NULL))
				return;
```
固定源物理行188–189；依适用条件消费。

## C02｜先将虚拟地址转page再取folio

[VERIFIED mykernel/mm/page_alloc/buddy.h::virt_to_folio]
```c
			page_s *page = virt_to_page(x);
			return page_folio(page);
```
固定源物理行87–88；依适用条件消费。

## C03｜根据folio flag位分类

[VERIFIED mykernel/mm/page_alloc/page-flags.h::folio_test_slab]
```c
			return (__builtin_constant_p((PG_slab)) ?
						constant_test_bit((PG_slab), (folio_flags(folio, 0))) :
						variable_test_bit((PG_slab), (folio_flags(folio, 0)))
					);
```
固定源物理行257–260；依适用条件消费。

## C04｜folio分类并在两个callee间分派

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			folio = virt_to_folio(object);
			if (unlikely(!folio_test_slab(folio)))
				free_large_kmalloc(folio, (void *)object);
			else
				kmem_cache_free(folio_slab(folio)->slab_cache, (void *)object);
```
固定源物理行191–195；依适用条件消费。

## C05｜slab地址转换非slab可返回NULL

[VERIFIED mykernel/mm/kmalloc/slub.h::virt_to_slab]
```c
			folio_s *folio = virt_to_folio(addr);
			if (!folio_test_slab(folio))
				return NULL;
			return folio_slab(folio);
```
固定源物理行46–49；依适用条件消费。

## C06｜folio_slab宏cast

[VERIFIED mykernel/mm/kmalloc/slub_macro.h::folio_slab]
```c
	#define folio_slab(folio)		folio_to_slab(folio)
```
固定源物理行15–15；依适用条件消费。

## C07｜大块阶读取、告警及放回页入口

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	uint order = folio_order(folio);
	if (WARN_ON_ONCE(order == 0))
		pr_warn_once("object pointer: 0x%p\n", object);
	__free_pages(folio_page(folio, 0), order);
```
固定源物理行280–283；依适用条件消费。

## C08｜order0告警不跳过free

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	if (WARN_ON_ONCE(order == 0))
		pr_warn_once("object pointer: 0x%p\n", object);
	__free_pages(folio_page(folio, 0), order);
```
固定源物理行281–283；依适用条件消费。

## C09｜folio_order按large flag或元数据返回阶

[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_order]
```c
			if (!folio_test_large(folio))
				return 0;
			return folio->_flags_1 & 0xff;
```
固定源物理行145–147；依适用条件消费。

## C10｜direct kmem_cache_free没有缓存来源重新确认

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]
```c
	slab_free(s, virt_to_slab(x), x);
```
固定源物理行314–314；依适用条件消费。

## C11｜slab_free先解引用cache来源

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	kmem_cache_node_s *n = &s->node;
	free_to_partial_list(s, n, slab, object);
```
固定源物理行294–295；依适用条件消费。

## C12｜历史MC-07整slab回收条件

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	if (slab->inuse == 0 ||
		n->partial.count >= s->min_partial) {

	    offline_exceeded_empty_slab(s, n, slab);
	    free_slab(s, slab);
```
固定源物理行302–306；依适用条件消费。

## C13｜slab空链最终向__free_pages请求

[VERIFIED mykernel/mm/kmalloc/slub.c::free_slab]
```c
	__free_pages(&folio->page, order);
```
固定源物理行251–251；依适用条件消费。

## C14｜slab对象归还到链的直接callee

[VERIFIED mykernel/mm/kmalloc/slub.c::free_to_partial_list]
```c
	free_single_to_partial(s, slab, object);
```
固定源物理行288–288；依适用条件消费。

## C15｜slab头中cache指针来源

[VERIFIED mykernel/mm/kmalloc/slub_types.h::slab]
```c
		kmem_cache_s		*slab_cache;
```
固定源物理行16–16；依适用条件消费。

## C16｜task栈kfree后条件性清NULL

[VERIFIED mykernel/sched/forkexec/fork.c::release_task_stack]
```c
	kfree(tsk->stack);
	tsk->stack = NULL;
```
固定源物理行123–124；依适用条件消费。

## C17｜task结构kfree请求

[VERIFIED mykernel/sched/forkexec/fork.c::free_task_struct]
```c
	kfree(tsk);
```
固定源物理行74–74；依适用条件消费。

## C18｜large成功指针来自page_to_virt

[VERIFIED mykernel/mm/kmalloc/slab_common.c::kmalloc_large]
```c
		ptr = (void *)page_to_virt(page);
		if (flags & __GFP_ZERO)
```
固定源物理行270–271；依适用条件消费。

## C19｜大块最终调用__free_pages

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	__free_pages(folio_page(folio, 0), order);
```
固定源物理行283–283；依适用条件消费。

## C20｜kfree函数实参及局部

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
		kfree(const void *object) {
			folio_s *folio;
			page_s *slab;
```
固定源物理行183–185；依适用条件消费。

## C21｜大块释放读取阶

[VERIFIED mykernel/mm/kmalloc/slab_common.c::free_large_kmalloc]
```c
	uint order = folio_order(folio);
```
固定源物理行280–280；依适用条件消费。

## C22｜slab放回页之前先下线

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	    offline_exceeded_empty_slab(s, n, slab);
	    free_slab(s, slab);
```
固定源物理行305–306；依适用条件消费。

## C23｜非slab返回NULL而不是cache

[VERIFIED mykernel/mm/kmalloc/slub.h::virt_to_slab]
```c
			if (!folio_test_slab(folio))
				return NULL;
```
固定源物理行47–48；依适用条件消费。

## C24｜cache_from_obj来源核查是注释

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]
```c
	// s = cache_from_obj(s, x);
	// if (!s)
	// 	return;
	slab_free(s, virt_to_slab(x), x);
```
固定源物理行311–314；依适用条件消费。

## C25｜归还链入口而非物理页保证

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	free_to_partial_list(s, n, slab, object);
```
固定源物理行295–295；依适用条件消费。


---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-ALLOCATION-87
record_type: fixed_time_definition_and_macro_short_anchors
produced_by: GPT-6 (visible identity; backend not independently verified)
model_per_owner: GPT-6 + Extra
date: 2026-11-14
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
status: STATIC_AUTHOR_EVIDENCE_READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
open_questions: [PA87-U01, PA87-U02, PA87-U03]
---

# 第87批固定源码证据索引

本文件102项新短锚均来自MyOS2固定time。每项一份真实单一C函数、结构体、宏、全局数据定义或枚举内部连续1至5行。A主题27、B主题29、C主题46。引文位置可人工定位；源码中的代码、注释、数组类型或命名不等于真实编译结果、硬件执行或者工作队列运行的证据。本批D00只修旧86的清单计数，不新增虚假的旧86第78条源码锚。

### PA87-E001 — backing GFP whitelist expression

[VERIFIED mykernel/mm/misc/percpu.c::PCPU_GFP]

```c
#define PCPU_GFP(gfp)	((gfp) & (GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN))
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E002 — rounded chunk size has size_t type

[VERIFIED mykernel/mm/misc/percpu.c::chunk_size]

```c
size_t chunk_size = 0;
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E003 — first runtime base address object

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_base_addr]

```c
void *pcpu_base_addr __ro_after_init;
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E004 — source CPU offset array object

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_unit_offsets]

```c
const ulong *pcpu_unit_offsets __ro_after_init;	/* cpu -> unit offset */
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E005 — first chunk published global

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_first_chunk]

```c
pcpu_chunk_s *pcpu_first_chunk __ro_after_init;
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E006 — reserved chunk published global

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_reserved_chunk]

```c
pcpu_chunk_s *pcpu_reserved_chunk __ro_after_init;
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E007 — shared chunk list declared

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_chunk_lists]

```c
DECLARE_LIST_HDR_S(pcpu_chunk_lists); /* chunk list slots */
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E008 — int metadata and list field

[VERIFIED mykernel/include/linux/smp/percpu_chunk.h::simple_pcpu_chunk]

```c
	struct simple_pcpu_chunk {
		List_s		list;			/* linked to pcpu_slot lists */
		int			free_bytes;		/* free bytes in the chunk */
		int			free_offset;
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。

### PA87-E009 — relative base address and data pointers

[VERIFIED mykernel/include/linux/smp/percpu_chunk.h::simple_pcpu_chunk]

```c
		void		*base_addr;
		// 通过memblock或者slub-buddy申请到的内存块的虚拟地址
		void		*data;			/* chunk data */
	};
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。

### PA87-E010 — late create metadata allocation and null test

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_create_chunk]

```c
*simple_pcpu_create_chunk(gfp_t gfp) {
	gfp = PCPU_GFP(gfp);
	pcpu_chunk_s *chunk = kzalloc(sizeof(*chunk), gfp);
	if (!chunk)
		return NULL;
```

范围：固定time单一函数或宏定义内部连续5行；源码语义不等于测试。

### PA87-E011 — late create list/init/zeroing-data request

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_create_chunk]

```c
	INIT_LIST_HEAD(&chunk->list);
	chunk->free_offset = 0;
	chunk->free_bytes = chunk_size;
	chunk->data = kzalloc(nr_cpu_ids * chunk_size, gfp);
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。

### PA87-E012 — data pointer, base difference and insertion before return

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_create_chunk]

```c
	chunk->data = kzalloc(nr_cpu_ids * chunk_size, gfp);
	chunk->base_addr = chunk->data - (ulong)pcpu_base_addr;
	list_header_add_to_tail(&pcpu_chunk_lists, &chunk->list);

	return chunk;
```

范围：固定time单一函数或宏定义内部连续5行；源码语义不等于测试。

### PA87-E013 — early first/reserved chunk data/list

[VERIFIED mykernel/mm/misc/percpu.c::early_pcpu_create_chunk]

```c
	chunk->free_offset = 0;
	chunk->free_bytes = chunk_size;
	chunk->data =
		myos_memblock_alloc_DMA32(nr_cpu_ids * chunk_size, chunk_size);
	list_header_add_to_tail(&pcpu_chunk_lists, &chunk->list);
```

范围：固定time单一函数或宏定义内部连续5行；源码语义不等于测试。

### PA87-E014 — fit ALIGN and need_size formula

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_find_block_fit]

```c
static int
simple_pcpu_find_block_fit(pcpu_chunk_s *chunk, int size, size_t align) {
	int alloc_offset = ALIGN(chunk->free_offset, align);
	int need_size = alloc_offset + size - chunk->free_offset;
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。

### PA87-E015 — fit condition and return

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_find_block_fit]

```c
	int need_size = alloc_offset + size - chunk->free_offset;
	if (chunk->free_bytes < need_size)
		return -ENOMEM;
	else
		return alloc_offset;
```

范围：固定time单一函数或宏定义内部连续5行；源码语义不等于测试。

### PA87-E016 — allocation commit function starts with empty while

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_area]

```c
simple_pcpu_alloc_area(pcpu_chunk_s *chunk, int size, int start)
{
	while (start < chunk->free_offset);
```

范围：固定time单一函数或宏定义内部连续3行；源码语义不等于测试。

### PA87-E017 — commit updates free_offset/free_bytes and returns start

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_area]

```c
	chunk->free_offset = start + size;
	chunk->free_bytes -= size;
	return start;
```

范围：固定time单一函数或宏定义内部连续3行；源码语义不等于测试。

### PA87-E018 — alignment wrapper power-of-two expectation

[VERIFIED mykernel/include/linux/kernel/align.h::ALIGN]

```c
	#define ALIGN(x, a)				__ALIGN_KERNEL((x), (a))
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E019 — alignment converts mask to typeof offset

[VERIFIED mykernel/include/uapi/linux/const.h::__ALIGN_KERNEL]

```c
	#define __ALIGN_KERNEL(x, a)			__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E020 — alignment mask arithmetic

[VERIFIED mykernel/include/uapi/linux/const.h::__ALIGN_KERNEL_MASK]

```c
	#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E021 — different round_up macro named separately

[VERIFIED mykernel/include/linux/kernel/math.h::round_up]

```c
	#define round_up(x, y)		((((x)-1) | __round_mask(x, y))+1)
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E022 — negative ENOMEM error constant source

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]

```c
	#define	ENOMEM		12	/* Out of memory */
```

范围：固定time单一函数或宏定义内部连续1行；源码语义不等于测试。

### PA87-E023 — public allocation request size_t alignment/gfp

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
void __percpu *simple_pcpu_alloc_noprof(size_t size,
		size_t align, bool reserved, gfp_t gfp)
{
```

范围：固定time单一函数或宏定义内部连续3行；源码语义不等于测试。

### PA87-E024 — local int off/cpu/ret versus size_t params

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	gfp_t pcpu_gfp;
	pcpu_chunk_s *chunk;
	int off, cpu, ret;
	ulong flags;
	void __percpu *ptr;
```

范围：固定time单一函数或宏定义内部连续5行；源码语义不等于测试。

### PA87-E025 — pcpu_gfp masking in real function

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	/* whitelisted flags that can be passed to the backing allocators */
	pcpu_gfp = gfp & (GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN);
```

范围：固定time单一函数或宏定义内部连续2行；源码语义不等于测试。

### PA87-E026 — first chunk initial free fields for prior qualified setup

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	// 手工初始化 pcpu_first_chunk
	pcpu_first_chunk->free_bytes = chunk_size - size_sum;
	pcpu_first_chunk->free_offset = size_sum;
	pcpu_first_chunk->base_addr = 0;
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。

### PA87-E027 — unit offset source i times static_size

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	pcpu_unit_offsets =
		myos_memblock_alloc_normal(nr_cpu_ids * sizeof(void *), 8);
	for (int i = 0; i < nr_cpu_ids; i++)
		((ulong *)pcpu_unit_offsets)[i] = i * static_size;
```

范围：固定time单一函数或宏定义内部连续4行；源码语义不等于测试。


### PA87-E028 — reserved candidate selection and attempted fit

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	/* serve reserved allocations from the reserved chunk if available */
	if (reserved && pcpu_reserved_chunk) {
		chunk = pcpu_reserved_chunk;

		off = simple_pcpu_find_block_fit(chunk, size, align);
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E029 — reserved failed fit jumps to failure rather than normal list

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
		off = simple_pcpu_find_block_fit(chunk, size, align);
		if (off < 0)
			goto fail_unlock;

		off = simple_pcpu_alloc_area(chunk, size, off);
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E030 — reserved commit success/failure two exits

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
		off = simple_pcpu_alloc_area(chunk, size, off);
		if (off >= 0)
			goto area_found;

		goto fail_unlock;
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E031 — normal list traversal enters fit

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
restart:
	/* search through normal chunks */
	list_header_for_each_container(chunk, &pcpu_chunk_lists, list) {
		off = simple_pcpu_find_block_fit(chunk, size, align);
		if (off < 0)
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E032 — normal fit and commit routing

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
			continue;

		off = simple_pcpu_alloc_area(chunk, size, off);
		if (off >= 0)
			goto area_found;
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E033 — normal successful area and loop end

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
		off = simple_pcpu_alloc_area(chunk, size, off);
		if (off >= 0)
			goto area_found;
	}
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E034 — expansion create and metadata failure test

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	/* No space left.  Create a new chunk. */
	chunk = simple_pcpu_create_chunk(pcpu_gfp);
	if (!chunk)
		goto fail;
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E035 — new chunk goes back to restart

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// spin_lock_irqsave(&pcpu_lock, flags);

	goto restart;
```

固定time单一定义内连续3行；仅证明源操作，不证明运行。

### PA87-E036 — shared area_found is label and commented lock action

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
area_found:
	// spin_unlock_irqrestore(&pcpu_lock, flags);
```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E037 — return relative pointer and inactive perCPU clear comments

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// for_each_possible_cpu(cpu)
	// 	memset((void *)pcpu_chunk_addr(chunk, cpu, 0) + off, 0, size);

	// ptr = __addr_to_pcpu_ptr(chunk->base_addr + off);
	ptr = chunk->base_addr + off;
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E038 — raw return encoding assignment and commented conversion

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// ptr = __addr_to_pcpu_ptr(chunk->base_addr + off);
	ptr = chunk->base_addr + off;
	// kmemleak_alloc_percpu(ptr, size, gfp);
```

固定time单一定义内连续3行；仅证明源操作，不证明运行。

### PA87-E039 — area_found return pointer

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// pcpu_memcg_post_alloc_hook(objcg, chunk, off, size);

	// pcpu_alloc_tag_alloc_hook(chunk, off, size);

	return ptr;
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E040 — failure and commented unlock label

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
fail_unlock:
	// spin_unlock_irqrestore(&pcpu_lock, flags);
fail:
	// trace_percpu_alloc_percpu_fail(reserved, is_atomic, size, align);
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E041 — failed allocator returns NULL

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// pcpu_memcg_post_alloc_hook(objcg, NULL, 0, size);

	return NULL;
```

固定time单一定义内连续3行；仅证明源操作，不证明运行。

### PA87-E042 — lock acquisition is commented

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// spin_lock_irqsave(&pcpu_lock, flags);
```

固定time单一定义内连续1行；仅证明源操作，不证明运行。

### PA87-E043 — lock release around expansion is commented

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// spin_unlock_irqrestore(&pcpu_lock, flags);

```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E044 — lock reentry around restart is commented

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_noprof]

```c
	// spin_lock_irqsave(&pcpu_lock, flags);
```

固定time单一定义内连续1行；仅证明源操作，不证明运行。

### PA87-E045 — list header anchor and count initializer

[VERIFIED mykernel/lib/list/double_list_macro.h::LIST_HEADER_INIT]

```c
	#define LIST_HEADER_INIT(name) {				\
				.anchor	= LIST_INIT(name.anchor),	\
				.count	= 0,						\
			}
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E046 — list head global declaration macro

[VERIFIED mykernel/lib/list/double_list_macro.h::DECLARE_LIST_HDR_S]

```c
	#define DECLARE_LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)
```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E047 — first-entry extraction expression

[VERIFIED mykernel/lib/list/double_list_macro.h::list_first_entry]

```c
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)
```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E048 — head sentinel termination

[VERIFIED mykernel/lib/list/double_list_macro.h::list_entry_is_head]

```c
	#define list_entry_is_head(pos, head, member)				\
				(&pos->member == (head))
```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E049 — list iteration true macro

[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each_entry]

```c
	#define list_for_each_entry(pos, head, member)							\
				for (pos = list_first_entry(head, typeof(*pos), member);	\
					!list_entry_is_head(pos, head, member);					\
					pos = list_next_entry(pos, member))
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E050 — wrapper traverses anchor list

[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container]

```c
	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)
```

固定time单一定义内连续2行；仅证明源操作，不证明运行。

### PA87-E051 — list head init synonym macro

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_HEAD]

```c
		#define INIT_LIST_HEAD(list) INIT_LIST_S(list)
```

固定time单一定义内连续1行；仅证明源操作，不证明运行。

### PA87-E052 — list node self links on init

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_S]

```c
		INIT_LIST_S(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}
```

固定time单一定义内连续4行；仅证明源操作，不证明运行。

### PA87-E053 — list insertion increments header count

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]

```c
		void
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E054 — kzalloc appends __GFP_ZERO and forwards kmalloc

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]

```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```

固定time单一定义内连续5行；仅证明源操作，不证明运行。

### PA87-E055 — slab cache allocator forwards to slab_alloc

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_alloc]

```c
void *kmem_cache_alloc(kmem_cache_s *s, gfp_t gfpflags) {
	return slab_alloc(s, gfpflags);
}
```

固定time单一定义内连续3行；仅证明源操作，不证明运行。

### PA87-E056 — kernel GFP macro as a flag expression

[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_KERNEL]

```c
	#define GFP_KERNEL				(__GFP_RECLAIM | __GFP_IO | __GFP_FS)
```

固定time单一定义内连续1行；仅证明源操作，不证明运行。


### PA87-E057 — workqueue pool workqueue cache pointer

[VERIFIED mykernel/kactive/workqueue/workqueue.c::pwq_cache]

```c
static kmem_cache_s *pwq_cache;
```

固定time单一定义内部1连续原文行；与运行见证严格区分。

### PA87-E058 — global workqueue list header

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueues]

```c
static DECLARE_LIST_HDR_S(workqueues);	/* PR: list of all workqueues */
```

固定time单一定义内部1连续原文行；与运行见证严格区分。

### PA87-E059 — caller creates perCPU slots for pwq pointer type

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
simple_alloc_and_link_pwqs(workqueue_s *wq) {
	bool highpri = wq->flags & WQ_HIGHPRI;
	int cpu, ret;

	wq->cpu_pwq = alloc_percpu(pool_workqueue_s *);
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E060 — perCPU allocation failure branches to cleanup

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
	wq->cpu_pwq = alloc_percpu(pool_workqueue_s *);
	if (!wq->cpu_pwq)
		goto enomem;
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E061 — bounded versus unbound and BH pool selection

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
	if (!(wq->flags & WQ_UNBOUND)) {
		worker_pool_s __percpu *pools;
		if (wq->flags & WQ_BH)
			pools = bh_worker_pools;
		else
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E062 — BH/CPU perCPU worker pools choice

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
		if (wq->flags & WQ_BH)
			pools = bh_worker_pools;
		else
			pools = cpu_worker_pools;
```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E063 — perCPU loop and local pointer roles

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
		for_each_possible_cpu(cpu) {
			pool_workqueue_s **pwq_p;
			pool_workqueue_s *pwq;
			worker_pool_s *pool;
```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E064 — perCPU slot and pool address then slab request

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
			pool = &(per_cpu_ptr(pools, cpu)[highpri]);
			pwq_p = per_cpu_ptr(wq->cpu_pwq, cpu);

			*pwq_p = kmem_cache_alloc(pwq_cache, GFP_KERNEL);
```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E065 — slab allocation null check per slot

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
			*pwq_p = kmem_cache_alloc(pwq_cache, GFP_KERNEL);
			if (!*pwq_p)
				goto enomem;

```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E066 — PWQ pool and owning workqueue assignment

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
			pwq = *pwq_p;
			pwq->pool = pool;
			pwq->wq = wq;
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E067 — list publication and successful return

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
			// mutex_lock(&wq->mutex);
			list_header_add_to_tail(&wq->pwqs, &pwq->pwqs_node);
			// mutex_unlock(&wq->mutex);
		}
		return 0;
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E068 — success path and unbound ret read

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
		}
		return 0;
	}

	return ret;
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E069 — cleanup traverses all CPU slots and frees nonnull PWQ

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
	if (wq->cpu_pwq) {
		for_each_possible_cpu(cpu) {
			pool_workqueue_s *pwq = *per_cpu_ptr(wq->cpu_pwq, cpu);
			if (pwq)
				kmem_cache_free(pwq_cache, pwq);
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E070 — cleanup free and disabled perCPU release

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
			if (pwq)
				kmem_cache_free(pwq_cache, pwq);
		}
		// free_percpu(wq->cpu_pwq);
		wq->cpu_pwq = NULL;
```

固定time单一定义内部5连续原文行；与运行见证严格区分。

### PA87-E071 — failure resets cpu_pwq and returns ENOMEM

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs]

```c
		// free_percpu(wq->cpu_pwq);
		wq->cpu_pwq = NULL;
	}
	return -ENOMEM;
```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E072 — outer workqueue object kzalloc and NULL exit

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]

```c
	wq = kzalloc(wq_size, GFP_KERNEL);
	if (!wq)
		return NULL;
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E073 — workqueue flags/list initialization

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]

```c
	/* init wq */
	wq->flags = flags;
	INIT_LIST_HEADER_S(&wq->pwqs);
	INIT_LIST_HEAD(&wq->list);
```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E074 — caller discards alloc_and_link return status

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]

```c
	// if (alloc_and_link_pwqs(wq) < 0)
	// 	goto err_unlock_free_node_nr_active;
	simple_alloc_and_link_pwqs(wq);

```

固定time单一定义内部4连续原文行；与运行见证严格区分。

### PA87-E075 — global list publication despite ignored status

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]

```c
	// mutex_unlock(&wq->mutex);

	list_header_add_to_tail(&workqueues, &wq->list);
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E076 — outer caller returns workqueue pointer

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]

```c
	return wq;
```

固定time单一定义内部1连续原文行；与运行见证严格区分。

### PA87-E077 — source of slab cache before actual wq allocations

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	pwq_cache = KMEM_CACHE(pool_workqueue, SLAB_PANIC);
	idr_init(&worker_pool_idr);
```

固定time单一定义内部2连续原文行；与运行见证严格区分。

### PA87-E078 — first named simple workqueue call

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	system_wq =
			simple_alloc_workqueue("events", 0, 0);
```

固定time单一定义内部2连续原文行；与运行见证严格区分。

### PA87-E079 — BH named simple workqueue call

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	system_bh_wq =
			simple_alloc_workqueue("events_bh", WQ_BH, 0);
```

固定time单一定义内部2连续原文行；与运行见证严格区分。

### PA87-E080 — PWQ pool/owner fields

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::pool_workqueue]

```c
	struct pool_workqueue {
		worker_pool_s	*pool;			/* I: the associated pool */
		workqueue_s		*wq;			/* I: the owning workqueue */
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E081 — linked pwqs_node member

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::pool_workqueue]

```c
		List_s			pwqs_node;	/* WR: node on wq->pwqs */
```

固定time单一定义内部1连续原文行；与运行见证严格区分。

### PA87-E082 — workqueue two header/link fields

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::workqueue_struct]

```c
	struct workqueue_struct {
		List_hdr_s		pwqs;		/* WR: all pwqs of this wq */
		List_s			list;		/* PR: list of all workqueues */
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E083 — workqueue flags and typed perCPU cpu_pwq pointer

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::workqueue_struct]

```c
		/* hot fields used during command issue, aligned to cacheline */
		uint			flags ____cacheline_aligned; /* WQ: WQ_* flags */
		pool_workqueue_s __rcu * __percpu	*cpu_pwq; /* I: per-cpu pwqs */
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E084 — workqueue enum BH and UNBOUND bits

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_flags]

```c
	enum wq_flags {
		WQ_BH					= 1 << 0,	/* execute in bottom half (softirq) context */
		WQ_UNBOUND				= 1 << 1,	/* not bound to any cpu */
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E085 — workqueue HIGHPRI enum bit

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_flags]

```c
		WQ_FREEZABLE			= 1 << 2,	/* freeze during suspend */
		WQ_MEM_RECLAIM			= 1 << 3,	/* may be used for memory reclaim */
		WQ_HIGHPRI				= 1 << 4,	/* high priority */
```

固定time单一定义内部3连续原文行；与运行见证严格区分。

### PA87-E086 — BH compatible flags enum entry

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_flags]

```c
		__WQ_BH_ALLOWS			= WQ_BH | WQ_HIGHPRI,
```

固定time单一定义内部1连续原文行；与运行见证严格区分。


### PA87-E087 — alloc function name alias

[VERIFIED mykernel/include/linux/smp/percpu.h::pcpu_alloc_noprof]

```c
	#define pcpu_alloc_noprof simple_pcpu_alloc_noprof
```

固定time单一定义内部连续1行；不认证可访问对象与运行。

### PA87-E088 — normal allocation wrapper passes reserved false

[VERIFIED mykernel/include/linux/smp/percpu.h::__alloc_percpu]

```c
	#define __alloc_percpu(_size, _align)			\
				pcpu_alloc_noprof(_size, _align,	\
					false, GFP_KERNEL)
```

固定time单一定义内部连续3行；不认证可访问对象与运行。

### PA87-E089 — typed slot allocation uses sizeof and alignof type

[VERIFIED mykernel/include/linux/smp/percpu.h::alloc_percpu]

```c
	#define alloc_percpu(type)								\
			(typeof(type) __percpu *)__alloc_percpu(		\
				sizeof(type), __alignof__(type)				\
			)
```

固定time单一定义内部连续4行；不认证可访问对象与运行。

### PA87-E090 — reserved allocation wrapper passes reserved true

[VERIFIED mykernel/include/linux/smp/percpu.h::__alloc_reserved_percpu]

```c
	#define __alloc_reserved_percpu(_size, _align)	\
				pcpu_alloc_noprof(_size, _align,	\
					true, GFP_KERNEL)
```

固定time单一定义内部连续3行；不认证可访问对象与运行。

### PA87-E091 — index into fixed CPU offset array

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_offset]

```c
		#	define per_cpu_offset(x) (__per_cpu_offset[x])
```

固定time单一定义内部连续1行；不认证可访问对象与运行。

### PA87-E092 — percpu pointer shift via RELOC_HIDE

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::SHIFT_PERCPU_PTR]

```c
	#define SHIFT_PERCPU_PTR(__p, __offset)								\
			RELOC_HIDE(													\
				(typeof(*(__p)) __kernel __force *)(__p), (__offset)	\
			)
```

固定time单一定义内部连续4行；不认证可访问对象与运行。

### PA87-E093 — perCPU pointer instance decoding

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_ptr]

```c
	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})
```

固定time单一定义内部连续5行；不认证可访问对象与运行。

### PA87-E094 — unsigned long pointer integer add and cast

[VERIFIED mykernel/include/linux/compiler/compiler.h::RELOC_HIDE]

```c
# define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
	 __ptr = (unsigned long) (ptr);				\
	(typeof(ptr)) (__ptr + (off)); })
```

固定time单一定义内部连续4行；不认证可访问对象与运行。

### PA87-E095 — named perCPU dereference macro

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu]

```c
	#define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))
```

固定time单一定义内部连续1行；不认证可访问对象与运行。

### PA87-E096 — runtime delta and CPU offset published

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```

固定time单一定义内部连续5行；不认证可访问对象与运行。

### PA87-E097 — reserved backing chunk base encoding

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	// 创建pcpu_reserved_chunk
	pcpu_reserved_chunk = early_pcpu_create_chunk();
	pcpu_reserved_chunk->base_addr =
		pcpu_reserved_chunk->data - (ulong)pcpu_base_addr;
```

固定time单一定义内部连续4行；不认证可访问对象与运行。

### PA87-E098 — dense contiguous CPU iteration

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_cpu]

```c
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)
```

固定time单一定义内部连续2行；不认证可访问对象与运行。

### PA87-E099 — possible CPU alias dense loop

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]

```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
```

固定time单一定义内部连续1行；不认证可访问对象与运行。

### PA87-E100 — cleanup slab cache free wrapper

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]

```c
void kmem_cache_free(kmem_cache_s *s, void *x) {
	// s = cache_from_obj(s, x);
	// if (!s)
	// 	return;
	slab_free(s, virt_to_slab(x), x);
```

固定time单一定义内部连续5行；不认证可访问对象与运行。

### PA87-E101 — two standard worker pools per CPU

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::NR_STD_WORKER_POOLS]

```c
		NR_STD_WORKER_POOLS		= 2,			/* # standard pools per cpu */
```

固定time单一定义内部连续1行；不认证可访问对象与运行。

### PA87-E102 — workqueue consumer reads cpu_pwq slot to pool

[VERIFIED mykernel/kactive/workqueue/workqueue.c::__simple_queue_work]

```c
	pwq = *per_cpu_ptr(wq->cpu_pwq, cpu);
	pool = pwq->pool;
```

固定time单一定义内部连续2行；不认证可访问对象与运行。


---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；lead=agent/MYOS2-LEAD-002 c2176ad4da02"
inputs_read: "见 facts-and-dependencies.yaml 的 inputs_read"
status: final
open_questions: "见 §0 仍未决与 YAML gaps"
notation: "[VERIFIED path::symbol] 为逐字引文（机械核对）；其余陈述以 A-/Q-/E-/MC- 编号指向 YAML 中的引文与检索；推导性内容在 YAML concerns 中以 premise/path/counter_evidence/unknown 分列"
record_type: mm_static_baseline
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
parts: [M01, M02, M03, M04]
executor: "Claude Code cloud session (claude.ai/code)"
model: unknown_or_not_attestable
date: 2026-10-02
kernel: "time a039d9803ade"
data: "facts-and-dependencies.yaml（本文所有表格由其生成；ID 以 YAML 为准）"
evidence_class: "静态源码盘点 + 一次独立只读反驳式审查；没有编译、没有运行 C/ASM、内核或 QEMU"
runtime: NOT_RUN
---

# 内存四子系统静态基线：页分配、内核堆、虚拟映射、缺页处理

## 0. 当前效果与未决

**一句话：** 四个子系统的主路径都能从源码讲清楚，并且彼此确实接上了：缺页时补页表，取页靠 buddy，堆也从 buddy 取页，VMA 用链表管理。但其中有几处确定性的缺口，**不能当作可靠的运行基础**：用户内存只分配不回收，错误访问不会让进程终止，匿名页不清零，mmap 选址与区间查询有确定性错误。这些是源码事实和推导，**一项都没有运行验证**。

| 子系统 | 当前效果 | 实现证据 | 正确性 | 并发 | 主要未知 |
|---|---|---|---|---|---|
| mm.page_alloc | zoned buddy 的取块/拆分/释放/合并都有活动正文，并已被 slab、页表分配、缺页使用；但释放块永不合并（MC-01）、分配不加锁（MC-02）、put_page 不归还（MC-05）。 | connected_body（folio 释放部分为 partial_body） | static_concern | source_only | 构建类型（Debug/Release）决定头文件内联函数的链接形式；--gc-sections 后实际保留哪些函数；碎片化在实际运行中多久导致高阶分配失败 |
| mm.kmalloc | 单节点 SLUB 风格分配器：partial/full 两条链、按页分流、kmalloc 尺寸表齐全并被大量调用；释放可能丢掉仍有对象的 slab（MC-07）；空闲指针覆盖对象首 8 字节、不保留 ctor 状态（MC-08，本 pin 无可达有害后果）。 | connected_body（ctor 支持为 partial_body） | static_concern | source_only | partial 链在实际负载下能否达到 min_partial；中断上下文是否调用 kmalloc |
| mm.vm_map | VMA 链表、mmap/munmap/brk、exec 建栈都有活动文本，ELF 装载在用；但只管 VMA，不撤销页表（MC-16），选址固定（MC-11），相交查询与合并有确定性缺陷（MC-13、MC-14、MC-15），无任何 mmap 锁（MC-18）。 | connected_body（munmap、选址、生命周期为 partial_body） | static_concern | source_only | 实际用户程序是否为动态链接 PIE（MC-12）以及其 mmap 序列；多线程共享 mm 是否存在（CLONE_VM 用户） |
| mm.fault | #PF→VMA→页表补齐→匿名/文件/COW 分派一条链完整，页来自 buddy；但任何未处理结果都在内核里自旋而不发信号（MC-23），匿名写页不清零（MC-21），栈不能靠缺页增长（MC-22），fork 后不刷 TLB（MC-25）。 | connected_body（共享写为占位自旋） | static_concern | source_only | 自旋中的任务能否被时钟中断换下（DR-01）；文件缺页之下的块 I/O 等待（DR-02） |

**跨子系统的六条主要结论：**

1. **用户内存只增不减。** 经 `put_page`/`folio_put` 释放的页不回 buddy（MC-05）；munmap 只摘 VMA，不撤销页表（MC-16）；进程退出或 exec 时旧地址空间从不拆除（MC-16）；buddy 释放块永不合并（MC-01）。合在一起，长时间运行会先碎片化，再耗尽。
2. **缺页错误不终止进程。** 找不到 VMA、共享写、内核地址缺页、页表页分配失败，都会让任务在内核里自旋（MC-23）；写只读私有映射会反复缺页、每次分配一个新页（MC-24）；数据页取页失败时，取页函数会先读空指针，后续结果未知（mm.fault.anonymous 的 failure_path）。这些出口都没有信号路径（LB-05）。中断出口在 `!in_atomic()` 时会调 `schedule()`，所以出错任务大概率只是一直占用时间片，不一定拖住整个系统；但这取决于 in_atomic 的实际值（DR-01、GAP-M05）。
3. **匿名页不清零。** 写缺页与零页 COW 拿到的新页没有清零，内容是物理页原来的数据（MC-21）。
4. **mmap 在多次映射后行为不可预测。** 选址恒为 `mmap_base − len`（MC-11），相交查询会漏判（MC-13），合并时读取未初始化的指针（MC-14），拆分后文件偏移算错（MC-15）。动态链接 PIE 在 exec 时还可能卡在选址循环里（MC-12，条件性）。
5. **与调度、锁、等待的关系。** 内存子系统的活动文本不直接使用 integration-03 已登记的问题原语，包括 atomic 判负、trylock、swait、completion、超时（DR-03，负检索 Q28）；它直接依赖的是自旋锁与普通原子操作：zone 锁与 slab 链表锁用关中断版本，页表锁用不关中断的 spin_lock。但分配侧不加锁（MC-02），mmap 锁是空函数（MC-18），并发前提本身未知（DR-05）。文件映射缺页下面的块 I/O 等待没有追踪（DR-02）。fork 的 COW 是否被绕过，取决于调度切换时机（DR-06、MC-25）。
6. **可以拿来讲解的范围。** 区与空闲链表的建立、buddy 的拆分、SLUB 的 partial/full 结构与尺寸分流、VMA 链与 mmap 参数处理、四级页表补齐、匿名/文件/COW 的分派，都可以按源码讲解，但讲解时必须同时讲清本文列出的缺口（M04）。

**本批收窄或驳回的初稿判断（保留反证）：** 独立审查驳回了 2 条初稿推断（RF-02、RF-03），把 1 条收窄为潜伏（RF-01），更正了 1 条路径（RF-04），另驳回 1 条“唯一”式断言（RF-05）。主线回源确认后改写如下：

- **RF-01：** 初稿认为伙伴不摘链会导致重复分配。实际上 PageBuddy 恒假，合并分支从不执行，这个后果在本 pin 不可达。
- **RF-02：** 初稿认为 sighand 锁会自旋。实际上新分配的 sighand 对象从未被安装，所以锁不会被用到。
- **RF-03：** 初稿认为 munmap 在 prev 为 NULL 时会自旋。实际上 `find_vma_intersection` 不会返回第一个 VMA，这种情况下 munmap 什么也不做，已并入 MC-13。
- **RF-04：** 初稿认为共享匿名映射在缺页时返回 SIGBUS。实际路径是共享匿名 VMA 带着 dummy_vm_ops，进入 `do_fault` 后自旋。
- **RF-05：** 初稿认为只有 sighand_cache 带构造函数。实际上 bdev_cache 与 shmem_inode_cache 也带。

**仍未决：** 构建类型与链接保留集合（GAP-M01、GAP-M02）；中断上下文分配与 SMP（GAP-M03）；文件读之下的块 I/O（GAP-M04）；缺页时的 in_atomic（GAP-M05）；实际用户程序类型（GAP-M06）；用户 PTE 的全局位（GAP-M07）；page_type 写入者的全树检索（GAP-M08）；未锚定的阅读记录（GAP-M09）。

## 1. M01 输入索引与能力对象

子系统固定为 `mm.page_alloc`、`mm.kmalloc`、`mm.vm_map`、`mm.fault`。起点是 `mykernel/mm/mm_api.h` 包含的四个 API 头（Q01），以及各目录的实际文件（time 的 `git ls-tree`）。四个目录各自的 CMakeLists.txt 都是 0 字节（Q02），源码由顶层 `GLOB_RECURSE` 收进 kernel 目标（A-B01）；链接时带 `--gc-sections`（Q03）。因此“列入构建”不等于“被链接保留”，更不等于“运行可达”（GAP-M02）。

缺页处理除 `mm/fault/` 外，还包括异常入口 `arch/x86_64/mm/fault.c` 与文件缺页回调 `mm/vm_map/filemap.c`。这两处都是按实际调用关系找到的，没有按 Linux 的惯例去猜文件名。

**旧输入。** 从 master `de3bb1df906a` 只读取了 DR-002 `completeness.yaml` 中这四个子系统条目的 ID 与节点名，以及 DR-003 `deps.yaml` 中相交边的端点名。旧的证据路径、行号、溯源与可靠性判断都没有复制。DR-003 没有任何 `mm.fault` 节点。本批新增的细粒度节点一律标为 `proposed_id`，没有修改公共词汇表。

| 编号 | 旧 ID | 来源 | 处置 | 对应本批节点 | 说明 |
|---|---|---|---|---|---|
| OLD-01 | mm.page_alloc | DR-002 + DR-003 | subsystem_row | mm.page_alloc |  |
| OLD-02 | mm.page_alloc.zoned_buddy | DR-002 | refined | mm.page_alloc.buddy_alloc、mm.page_alloc.buddy_free |  |
| OLD-03 | mm.page_alloc.obvious_missing | DR-002 | not_covered | — | 回收、OOM、per-CPU 页表等“缺失项”本批未逐项检索，不确认也不否认。 |
| OLD-04 | mm.page_alloc.mm_core_init | DR-003（init_order 端点） | kept_as_capability | mm.page_alloc.mm_core_init |  |
| OLD-05 | mm.page_alloc.buddy_system | DR-003（functional 端点） | refined | mm.page_alloc.buddy_alloc、mm.page_alloc.buddy_free |  |
| OLD-06 | mm.kmalloc | DR-002 + DR-003 | subsystem_row | mm.kmalloc |  |
| OLD-07 | mm.kmalloc.slub_and_size_classes | DR-002 | refined | mm.kmalloc.object_allocator、mm.kmalloc.object_free、mm.kmalloc.size_classes_and_large |  |
| OLD-08 | mm.kmalloc.obvious_missing | DR-002 | not_covered | — | per-CPU 快路径、cache 销毁等未逐项检索。 |
| OLD-09 | mm.kmalloc.object_allocator | DR-003（functional 端点） | kept_as_capability | mm.kmalloc.object_allocator |  |
| OLD-10 | mm.vm_map | DR-002 + DR-003 | subsystem_row | mm.vm_map |  |
| OLD-11 | mm.vm_map.vma_and_syscalls | DR-002 | refined | mm.vm_map.vma_list_lookup、mm.vm_map.mmap_create、mm.vm_map.unmapped_area_search、mm.vm_map.munmap、mm.vm_map.brk_stack_exec |  |
| OLD-12 | mm.vm_map.obvious_missing | DR-002 | not_covered | — | mprotect/mremap、ASLR 等未检索。 |
| OLD-13 | mm.fault | DR-002（DR-003 无此节点） | subsystem_row | mm.fault | DR-003 的边中没有任何 mm.fault 端点；本批的 fault 依赖全部是新增 proposed 边。 |
| OLD-14 | mm.fault.anon_file_cow | DR-002 | refined | mm.fault.anonymous、mm.fault.file_backed、mm.fault.cow |  |
| OLD-15 | mm.fault.obvious_missing | DR-002 | not_covered | — | swap、大页等未检索。 |
| OLD-16 | DR-003 相交端点（四子系统之外） | DR-003 | endpoint_only | — | init.kernel_sources、mm.early、mm.early.setup_arch、mm.early.memblock、mm.misc.zero_pfn、sched.scheduler.sched_init、sched.scheduler.multitasking、sched.forkexec.user_process、fs.vfs.root_mount、lock.futex.wait_wake、entry.trap_init 只作端点名使用，未盘点。 |

本批没有覆盖的部分：mm.early / mm.highmem / mm.misc（含 gup、percpu、init-mm）不在本批四个子系统内；arch/x86_64/mm 的直映射建立（init_mem_mapping 等）与 pgd_alloc/pgd_free 只作为端点；各 obvious_missing 节点（OLD-03、OLD-08、OLD-12、OLD-15）；highmem 的 kmap 系列、filemap 的读写路径（generic_file_read_iter 等）。

## 2. M02 能力表：实现程度与正确性分轴

共 21 个代表能力，每个子系统 5 到 6 个。下表的证据字段只记录本批的证据状态，不折算成旧 002 的 0–4 成熟度分数。运行证据一律为 NOT_RUN。每个能力的正常路径、失败路径、源码条件、引文 ID 与未覆盖处见 YAML 的 `capabilities[]`。

| 能力 ID | ID 状态 | 用途 | 实现证据 | 正确性 | 并发 | 源码条件 | 疑点 |
|---|---|---|---|---|---|---|---|
| mm.page_alloc.zone_layout | proposed_id | 按 max_low_pfn 给 DMA/DMA32/NORMAL 三个区定上限，分配 mem_map，并为每个区建好各阶空闲链表头。 | connected_body | not_assessed | not_assessed | not_conditional | — |
| mm.page_alloc.mm_core_init | old_id_kept | 启动期把 memblock 记录的空闲内存交给 buddy，然后初始化 slab；决定“页分配器先可用、堆后可用”的顺序。 | connected_body | not_assessed | not_assessed | not_conditional | — |
| mm.page_alloc.buddy_alloc | proposed_id | 按阶从区空闲链表取连续页块，拆分多余部分放回低阶链表，并按 gfp 选区与清零。 | connected_body | static_concern | source_only | resolved | MC-02、MC-03、MC-04 |
| mm.page_alloc.buddy_free | proposed_id | 把页块还给区空闲链表，并与同阶空闲伙伴合并成更高阶块。 | connected_body | static_concern | source_only | partially_resolved | MC-01 |
| mm.page_alloc.folio_ref_release | proposed_id | 按引用计数释放页（put_page/folio_put），供缺页、COW 等路径交还页。 | partial_body | static_concern | not_assessed | partially_resolved | MC-05 |
| mm.kmalloc.boot_init | proposed_id | 启动期用静态 boot cache 自举出 kmem_cache 的 cache，再建 kmalloc 各尺寸 cache。 | connected_body | not_assessed | not_assessed | resolved | — |
| mm.kmalloc.object_allocator | old_id_kept | 从 cache 的部分空闲 slab 取对象；没有时向页分配器要新 slab 并串好空闲链。 | connected_body | static_concern | source_only | partially_resolved | MC-08 |
| mm.kmalloc.object_free | proposed_id | 把对象放回所在 slab，必要时把 slab 从 full 链移回 partial 链，或整块还给页分配器。 | connected_body | static_concern | source_only | resolved | MC-07 |
| mm.kmalloc.size_classes_and_large | proposed_id | kmalloc/kzalloc/kcalloc/kfree：小于等于一页走尺寸 cache，大于一页直接按阶要页。 | connected_body | static_concern | not_assessed | partially_resolved | MC-09 |
| mm.kmalloc.ctor_cache_state | proposed_id | 带构造函数的 cache：对象状态由 ctor 初始化，并期望在分配/释放之间保持。 | partial_body | static_concern | not_assessed | not_conditional | MC-08 |
| mm.vm_map.vma_list_lookup | proposed_id | 用按地址排序的双链表保存 VMA，并提供包含查询、相交查询、前后遍历。 | connected_body | static_concern | source_only | partially_resolved | MC-13 |
| mm.vm_map.mmap_create | proposed_id | mmap(2)：检查参数与文件权限，选地址，清掉该范围旧映射，合并或新建 VMA 并挂到链表；文件映射交给文件的 mmap 钩子设 vm_ops。 | connected_body | static_concern | source_only | not_conditional | MC-14、MC-18 |
| mm.vm_map.unmapped_area_search | proposed_id | 为非 MAP_FIXED 的映射在 mmap_base 以下找一段空闲地址。 | partial_body | static_concern | not_assessed | not_conditional | MC-11、MC-12 |
| mm.vm_map.munmap | proposed_id | munmap(2) 与 mmap 内部的清范围：拆分边界 VMA、摘掉范围内的 VMA，并（按设计）撤销页表映射。 | partial_body | static_concern | source_only | not_conditional | MC-13、MC-15、MC-16 |
| mm.vm_map.brk_stack_exec | proposed_id | brk 扩缩堆、exec 建栈 VMA 并向下扩栈、ELF 装载映射段与 bss。 | connected_body | static_concern | source_only | not_conditional | MC-22 |
| mm.vm_map.mm_lifecycle | proposed_id | fork 复制地址空间（VMA 与页表），进程退出/exec 换 mm 时释放旧地址空间。 | partial_body | static_concern | source_only | not_conditional | MC-16、MC-25 |
| mm.fault.arch_entry | proposed_id | #PF 异常入口：区分内核地址/用户地址，找 VMA，交给通用缺页处理，处理失败结果。 | connected_body | static_concern | source_only | not_conditional | MC-22、MC-23、MC-24 |
| mm.fault.pgtable_walk | proposed_id | 按缺页地址补齐 PUD/PMD/PTE 页表页（P4D 为折叠层，p4d_alloc 直接返回 pgd 项），读出原 PTE 并按状态分派。 | connected_body | static_concern | source_only | partially_resolved | MC-23 |
| mm.fault.anonymous | proposed_id | 匿名映射首次访问：读映射共享零页，写分配一个私有新页。 | connected_body | static_concern | source_only | not_conditional | MC-21、MC-23 |
| mm.fault.file_backed | proposed_id | 文件映射缺页：已缓存页直接映射，否则读文件进页缓存后映射；私有写做 COW，共享写未实现。 | connected_body | static_concern | not_assessed | not_conditional | MC-23 |
| mm.fault.cow | proposed_id | 写时复制：fork 时把父子 PTE 设为只读共享，之后写缺页复制出私有页。 | connected_body | static_concern | source_only | partially_resolved | MC-21、MC-24、MC-25 |

**源码条件的共同点。** 很多 helper 写在头文件里，并包在 `#if defined(X_DEFINATION) || !(DEBUG)` 之中（引文的 `cpp_conditions` 字段逐条记录了这一点）。Debug 构建时它们由定义宏所在的 .c 文件提供，其他单元用 extern 引用；非 Debug 构建时它们在每个单元里都是 `static inline`（Q35）。两种情况下函数正文相同，只是链接形式不同，而实际构建类型未知（GAP-M01）。zone_type 枚举没有任何 `#if`，`CONFIG_ZONE_DMA`/`CONFIG_ZONE_DMA32` 不会改变区列表。

### 2.1 关键引文（全部由 `scripts/mm_facts_check.py` 机械核对）

伙伴合并依赖的类型判定（MC-01）：

[VERIFIED mykernel/mm/page_alloc/page-flags.h::PageBuddy]
```c
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
```

`page_type` 与 `_mapcount` 共用存储，初始化里把它置为类型基值的那一行被注释掉了：

[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
	// page_mapcount_reset(page);
```

slab 释放的下线条件（MC-07）：

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_free]
```c
	if (slab->inuse == 0 ||
		n->partial.count >= s->min_partial) {

	    offline_exceeded_empty_slab(s, n, slab);
	    free_slab(s, slab);
```

选址结果被赋值覆盖（MC-11）：

[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	if (gap = ~0) {
		gap = info->high_limit - info->length;
		gap &= PAGE_MASK;
	}
```

相交查询只看一个后继（MC-13）：

[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			vma_s *vma = find_vma_prev_to_addr(mm, start);

			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
				return vma;
```

匿名页的取页函数忽略了 `need_zero`（MC-21）：

[VERIFIED mykernel/mm/fault/fault.c::folio_prealloc]
```c
	return folio_alloc(GFP_HIGHUSER_MOVABLE, 0);
```

缺页处理的结果只要非 0 就自旋（MC-23）：

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);
```

fork 之后不刷新 TLB（MC-25）：

[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap]
```c
	// flush_tlb_mm(oldmm);
```

## 3. M03 接口之间的真实依赖

共 39 条能力级的边，类型为 call、data、init_order、config、build，方向约定见 YAML 的 `edge_direction`。每条边都附有引文 ID，或附有带所在函数名的检索命中（Q 编号、路径、行号）。`#include` 关系一律不当作调用边。

### 3.1 四个问题的回答

**物理页从哪里准备并分配/归还？**

setup_arch 建立 memblock 与直映射后调 zone_sizes_init 建三区与空闲链表头（E02、A-PA01）；mm_core_init→mem_init→memblock_free_all 把空闲区间按对齐切块，经普通释放路径放进 buddy（E05、A-PA04）。分配：alloc_pages→__myos_alloc_pages 按 NORMAL→DMA32→DMA 取块并拆分（A-PA06、A-PA07）。归还：__free_pages→__free_pages_ok 在 zone->lock 内挂回链表，但伙伴从不合并（MC-01）；经 folio_put/put_page 的归还在 __folio_put 处断开（LB-01）。

相关边：E02、E03、E04、E05、E07、E08；断点：LB-01、LB-08。

**堆接口是否及如何消费页分配？**

是。slab 新建时 alloc_pages(oo_order)（E09），整 slab 释放时 __free_pages（E10）；>PAGE_SIZE 的 kmalloc 直接按阶要页、kfree 按阶还页（E12、E13）。kmem_cache_init 在 mem_init 之后（E06）。kfree 依据 page->flags 的 PG_slab 位分派（E14），该位是 flags 位而非 page_type，不受 MC-01 根因影响。

相关边：E06、E09、E10、E11、E12、E13、E14；断点：无。

**虚拟映射怎样管理地址区间与页表？**

只管地址区间：VMA 存在按地址排序的双链表中（E21、E22），mmap/brk/exec 建 VMA，VMA 结构来自 vm_area_cachep（E18、E19）；文件映射经 f_op->mmap 设 vm_ops（E20）。页表不由 vm_map 建立，而是在缺页时由 fault 补齐（E30）；vm_map 也从不拆页表——munmap 与地址空间拆除都断开（LB-02、LB-03），fork 时由 dup_mmap 调 fault 侧的 copy_page_range 复制（E26）。

相关边：E16、E17、E18、E19、E20、E21、E22、E23、E24、E25、E26；断点：LB-02、LB-03、LB-07、LB-09。

**缺页入口怎样到实际页获取/映射？**

exception_handler→exc_page_fault（E27）→do_user_addr_fault：simple_find_vma 找包含地址的 VMA（E28）→myos_handle_mm_fault 逐级补页表（页表页来自 buddy，E30）→按 PTE 状态分派：空 PTE 的匿名 VMA 走 do_anonymous_page（读映射零页，写取 folio_alloc 新页，E31、E34、E35），非匿名走 do_fault（读：页缓存或 simple_filemap_fault 取清零页并 f_op->read，E32、E36、E37；私有写：do_cow_fault），已存在只读 PTE 的写走 do_wp_page→wp_page_copy（E33、E38、E39）。所有错误出口都自旋（LB-05），栈增长断开（LB-04），文件读之下未追踪（LB-06）。

相关边：E27、E28、E29、E30、E31、E32、E33、E34、E35、E36、E37、E38、E39；断点：LB-04、LB-05、LB-06。

### 3.2 边表

| 边 | 从 | 到 | 类型 | 依据 | 说明 |
|---|---|---|---|---|---|
| E01 | mm.page_alloc（同样适用于 mm.kmalloc、mm.vm_map、mm.fault） | external:mykernel/CMakeLists.txt::KERNEL_C_SRCS | build | A-B01；Q03 scripts/options_flags.cmake:75 | 四个目录的 .c 都由顶层 GLOB_RECURSE 列入 kernel 目标（子目录 CMakeLists 为 0 字节，Q02）；链接带 --gc-sections。列入构建不等于被链接保留，更不等于运行可达。 |
| E02 | mm.page_alloc.zone_layout | external:mykernel/arch/x86_64/kernel/setup.c::setup_arch | init_order | Q04 arch/x86_64/kernel/setup.c:254 (setup_arch) | zone_sizes_init 由 setup_arch 调用（在 memblock 建立与直映射之后，阅读记录）。 |
| E03 | mm.page_alloc.mm_core_init | mm.page_alloc.zone_layout | init_order | Q04 init/main.c:138 (start_kernel)；Q04 init/main.c:144 (start_kernel) | start_kernel 先 setup_arch（第 138 行，内含 zone_sizes_init）后 mm_core_init（第 144 行）；空闲链表头必须先建好，memblock_free_all 才能往里放块。 |
| E04 | mm.page_alloc.buddy_alloc | mm.page_alloc.zone_layout | data | A-PA02；A-PA06 | 分配的区循环上界读 nr_zones（=MAX_NR_ZONES=5），偏好表只有 3 项（MC-04）。 |
| E05 | mm.page_alloc.mm_core_init | mm.page_alloc.buddy_free | call | A-PA04；Q05 mm/page_alloc/buddy.c:571 (memblock_free_pages) | 启动交接把空闲内存经 memblock_free_pages→__free_pages_ok 放进 buddy，走的就是普通释放路径。 |
| E06 | mm.kmalloc.boot_init | mm.page_alloc.mm_core_init | init_order | A-PA05；Q04 mm/misc/mm_init.c:409 (mm_core_init)；Q04 mm/misc/mm_init.c:411 (mm_core_init) | mem_init（buddy 已填充）在 kmem_cache_init 之前；slab 自举要从 buddy 取页。 |
| E07 | mm.page_alloc.buddy_free | external:mykernel/mm/page_alloc/page-flags.h::PageBuddy | data | A-PA13；A-PA17；A-PA18 | 合并判定读 page_type；page_type 从未被置成类型基值，PageBuddy 恒假（MC-01）。 |
| E08 | mm.page_alloc.buddy_alloc | mm.page_alloc.buddy_free | data | A-PA07；A-PA10 | 两边共用 zone->free_area；释放在 zone->lock 内、分配不取锁（MC-02）。 |
| E09 | mm.kmalloc.object_allocator | mm.page_alloc.buddy_alloc | call | A-KM05；Q13 mm/kmalloc/slub.c:105 (alloc_slab) | 新 slab 的页来自 alloc_pages(oo_order)。 |
| E10 | mm.kmalloc.object_free | mm.page_alloc.buddy_free | call | Q13 mm/kmalloc/slub.c:251 (free_slab) | 整 slab 释放走 __free_pages（不经 folio_put，因此不受 MC-05 影响）。 |
| E11 | mm.kmalloc.size_classes_and_large | mm.kmalloc.object_allocator | call | A-KM10；Q36 mm/kmalloc/slab_common.c:253 (__kmalloc) | ≤PAGE_SIZE 的 kmalloc 经 __kmalloc→kmalloc_slab→kmem_cache_alloc。 |
| E12 | mm.kmalloc.size_classes_and_large | mm.page_alloc.buddy_alloc | call | Q13 mm/kmalloc/slab_common.c:268 (kmalloc_large) | >PAGE_SIZE 的 kmalloc 以 __GFP_COMP 直接按阶要页。 |
| E13 | mm.kmalloc.size_classes_and_large | mm.page_alloc.buddy_free | call | Q13 mm/kmalloc/slab_common.c:283 (free_large_kmalloc) | kfree 非 slab 页时按 folio 阶 __free_pages。 |
| E14 | mm.kmalloc.size_classes_and_large | mm.kmalloc.object_allocator | data | A-KM11；Q13 mm/kmalloc/slub.c:111 (alloc_slab)；Q13 mm/kmalloc/slub.h:192 (kfree) | kfree 用 PG_slab（page->flags 位，由 alloc_slab 置位）决定归还对象；这是 flags 位，不受 page_type 未初始化影响。 |
| E15 | external:mykernel/sched/forkexec/fork.c::copy_sighand | mm.kmalloc.ctor_cache_state | call | A-KM14；Q11 sched/forkexec/fork.c:1654 (proc_caches_init) | 唯一需要 ctor 状态的使用者分配后并不安装对象（DR-04），因此 MC-08 在本 pin 没有可达的有害后果。 |
| E16 | mm.vm_map.mmap_create | mm.vm_map.unmapped_area_search | call | A-VM07；Q37 mm/vm_map/mmap.c:522 (do_mmap) | do_mmap→__get_unmapped_area→mm->get_unmapped_area（由 arch_pick_mmap_layout 设为 simple_get_unmapped_area）。 |
| E17 | mm.vm_map.mmap_create | mm.vm_map.munmap | call | A-VM03；Q16 mm/vm_map/mmap.c:149 (simple_munmap_vma_range) | 建映射前先对目标范围做 VMA 层 munmap。 |
| E18 | mm.vm_map.mmap_create | mm.kmalloc.object_allocator | call | Q36 mm/vm_map/mmap.c:33 (vm_area_alloc) | VMA 结构来自 vm_area_cachep。 |
| E19 | mm.vm_map.mmap_create | external:mykernel/sched/forkexec/fork.c::proc_caches_init | init_order | A-KM17；Q04 init/main.c:186 (start_kernel) | vm_area_cachep 在 start_kernel 调 proc_caches_init 时创建，早于任何用户映射。 |
| E20 | mm.vm_map.mmap_create | external:mykernel/mm/vm_map/filemap.c::generic_file_mmap | call | Q17 mm/vm_map/mmap.c:693 (simple_mmap_region)；Q17 mm/vm_map/filemap.c:418 (generic_file_mmap) | 间接调用 f_op->mmap；块设备与 FAT32 都指向 generic_file_mmap，它设 vm_ops=generic_file_vm_ops（缺页时的 ->fault）。 |
| E21 | mm.vm_map.mmap_create | mm.vm_map.vma_list_lookup | call | Q38 mm/vm_map/mmap.c:180 (simple_vma_link)；Q38 mm/vm_map/mmap.c:384 (simple_vma_merge) | 入链与合并前的相交查询。 |
| E22 | mm.vm_map.munmap | mm.vm_map.vma_list_lookup | call | A-VM14；Q38 mm/vm_map/mmap.c:1131 (simple_do_vma_munmap)；Q38 mm/vm_map/mmap.c:1158 (simple_do_vma_munmap) | 用 find_vma_intersection 找起点（MC-13），逐个摘链。 |
| E23 | external:mykernel/fs/vfs/binfmt_elf.c::elf_map | mm.vm_map.mmap_create | call | Q16 fs/vfs/binfmt_elf.c:343 (elf_map)；Q16 fs/vfs/binfmt_elf.c:347 (elf_map) | ELF 段映射经 vm_mmap；对应 DR-003 中 sched.forkexec.user_process→mm.vm_map 的功能边（只沿用节点名）。 |
| E24 | external:mykernel/fs/vfs/binfmt_elf.c::elf_load | mm.vm_map.brk_stack_exec | call | Q16 fs/vfs/binfmt_elf.c:403 (elf_load) | bss 零区经 vm_brk_flags。 |
| E25 | external:mykernel/sched/forkexec/exec.c::setup_arg_pages | mm.vm_map.brk_stack_exec | call | Q29 sched/forkexec/exec.c:540 (setup_arg_pages)；Q29 sched/forkexec/exec.c:127 (__bprm_mm_init) | exec 建栈 VMA（insert_vm_struct）并预先扩栈（expand_stack）；这是 expand_stack 唯一可达的调用者（MC-22）。 |
| E26 | mm.vm_map.mm_lifecycle | mm.fault.cow | call | Q27 sched/forkexec/fork.c:218 (dup_mmap) | fork 逐 VMA 调 copy_page_range 复制页表并写保护。 |
| E27 | external:mykernel/arch/x86_64/myos/interrupt.c::exception_handler | mm.fault.arch_entry | call | Q22 arch/x86_64/myos/interrupt.c:151 (exception_handler) | #PF 向量分发到 exc_page_fault；返回后 excep_hwint_context 在 !in_atomic() 时调 schedule()（Q32，DR-01）。 |
| E28 | mm.fault.arch_entry | mm.vm_map.vma_list_lookup | call | A-FT04；Q38 arch/x86_64/mm/fault.c:276 (do_user_addr_fault) | 缺页按“包含地址”查 VMA（MC-22）。 |
| E29 | mm.fault.arch_entry | mm.fault.pgtable_walk | call | A-FT08；Q22 arch/x86_64/mm/fault.c:323 (do_user_addr_fault) | 结果非 0 即自旋（MC-23）。 |
| E30 | mm.fault.pgtable_walk | mm.page_alloc.buddy_alloc | call | A-FT11 | PUD/PMD/PTE 页表页 alloc_page 并清零；页表页释放函数（pagetable_free→__free_pages）存在，其调用者本批未检索。 |
| E31 | mm.fault.pgtable_walk | mm.fault.anonymous | call | A-FT12；Q39 mm/fault/fault.c:1232 (do_pte_missing) | vm_ops 为 NULL（vma_set_anonymous）才算匿名；新 VMA 默认带 dummy_vm_ops（A-VM18）。 |
| E32 | mm.fault.pgtable_walk | mm.fault.file_backed | call | A-FT12；Q39 mm/fault/fault.c:1234 (do_pte_missing) | 非匿名 VMA（含只带 dummy_vm_ops 的共享匿名映射）走 do_fault。 |
| E33 | mm.fault.pgtable_walk | mm.fault.cow | call | A-FT13；Q27 mm/fault/fault.c:1273 (handle_pte_fault) | 写缺页且 PTE 不可写 → do_wp_page。 |
| E34 | mm.fault.anonymous | mm.page_alloc.buddy_alloc | call | A-FT15；A-FT16；A-FT17；Q23 mm/highmem/highmem_macro.h:26 | folio_alloc 是 folio_alloc_noprof 的宏，后者忽略参数，取 0 阶 GFP_HIGHUSER_MOVABLE 页（无 __GFP_ZERO）。 |
| E35 | mm.fault.anonymous | external:mykernel/init/main.c::do_pre_smp_initcalls | init_order | A-FT14；Q30 init/main.c:255 (do_pre_smp_initcalls) | 读缺页映射的零页 pfn 由 init_zero_pfn 设置，在 kernel_init 启动用户进程之前调用。 |
| E36 | mm.fault.file_backed | mm.page_alloc.buddy_alloc | call | A-FT25 | 页缓存页 alloc_page(GFP_USER\|__GFP_ZERO)，0 阶，清零有效。 |
| E37 | mm.fault.file_backed | external:<file>->f_op->read | call | A-FT26 | 间接调用，端点保留；其下的文件系统与块设备等待未追踪（DR-02、LB-06）。 |
| E38 | mm.fault.cow | mm.page_alloc.buddy_alloc | call | A-FT20 | wp_page_copy 经 folio_prealloc 取新页（与匿名写缺页同一取页函数）。 |
| E39 | mm.fault.cow | mm.page_alloc.folio_ref_release | call | Q40 mm/fault/fault.c:676 (wp_page_copy)；Q40 mm/fault/fault.c:680 (wp_page_copy) | 旧页/新页经 folio_put 释放，最终 __folio_put 为空（MC-05）。 |

### 3.3 链路断点（命中缺口就断开，不按 Linux 惯例补齐）

| 断点 | 位置 | 说明 | 依据 |
|---|---|---|---|
| LB-01 | mm.page_alloc.folio_ref_release → mm.page_alloc.buddy_free | 引用降为 0 后 __folio_put 什么也不做；经 put_page/folio_put 释放的用户页（缺页、COW、exec 参数页）不回到 buddy。 | A-PA15、Q40 |
| LB-02 | mm.vm_map.munmap → 页表 / TLB | munmap 只摘 VMA；simple_unmap_region 全部注释，没有清 PTE、释放页表或刷 TLB 的活动文本。 | A-VM15、Q20、Q26 |
| LB-03 | 进程退出 / exec 换 mm → 地址空间拆除 | __mmput 全部注释；没有 exit_mmap/unmap_vmas/free_pgtables 等活动实现。 | A-VM16、Q20 |
| LB-04 | mm.fault.arch_entry → expand_stack | VMA 查询只返回包含地址的 VMA，栈下方的缺页先到 myos_bad_area，扩栈分支不可达。 | A-FT05、A-FT06、Q29 |
| LB-05 | 缺页错误 → 信号/进程终止 | 没有 SIGSEGV/SIGBUS 投递路径；所有未处理结果都在内核内 while 自旋（是否能被换下见 DR-01）。 | A-FT02、A-FT08、A-FT09 |
| LB-06 | mm.fault.file_backed → 块设备读完成 | f_op->read 端点保留，未追到具体文件系统/块驱动与其等待机制；不能把文件缺页当作已闭合的链。 | A-FT26 |
| LB-07 | VMA → 文件反向映射（i_mmap） | vma_interval_tree_insert 只在注释中出现；文件页没有反向映射。 | Q41 |
| LB-08 | buddy 释放 → 伙伴合并 | PageBuddy 依赖 page_type 的类型基值，page_type 从不被设置成该值，合并分支永不进入。 | A-PA17、A-PA18、Q34 |
| LB-09 | fork 写保护父 PTE → TLB 失效 | flush_tlb_mm 被注释，fork/exec/mm 目录中无活动的 TLB 刷新。 | A-FT24、Q26 |

## 4. M04 对后续工作的可用性

“可读讲解”、“宿主可单独验证”、“需要真实内核”三项分开填写。存在依赖风险，不等于整个模块不可用；但也不能忽略这些风险。

| 能力 | 可读讲解 | 宿主可单独验证 | 需要真实内核 | 依赖已知待修机制 | 结论 |
|---|---|---|---|---|---|
| mm.page_alloc.zone_layout | 可 | 可（输入为人造 memblock 区间） | 否 | 无 | 结构可讲；正确性未评估 |
| mm.page_alloc.mm_core_init | 可 | 部分（切块算术可宿主验证） | 启动交接总量需要 | 无 | 结构可讲；数量未验证 |
| mm.page_alloc.buddy_alloc | 可 | 可 | 并发部分需要 | 无（DR-05 的并发前提未知） | 有实现，正确性有疑点（MC-02/03/04） |
| mm.page_alloc.buddy_free | 可（须同时讲 MC-01） | 可 | 否 | 无 | 有实现，不合并（MC-01） |
| mm.page_alloc.folio_ref_release | 可（空实现本身就是结论） | 不必 | 否 | 无 | 归还缺失（MC-05） |
| mm.kmalloc.boot_init | 可 | 部分 | 完整自举需要 | 无 | 结构可讲 |
| mm.kmalloc.object_allocator | 可 | 可（替换 alloc_pages） | 并发部分需要 | 无 | 有实现；MC-08 机制性问题无当前受害者 |
| mm.kmalloc.object_free | 可（须同时讲 MC-07） | 可 | 否 | 无 | 有实现，正确性有疑点（MC-07） |
| mm.kmalloc.size_classes_and_large | 可 | 可 | 否 | 无 | 有实现；MC-09 潜伏 |
| mm.kmalloc.ctor_cache_state | 可 | 可 | 否 | 无 | 机制性问题（MC-08），本 pin 无受害者 |
| mm.vm_map.vma_list_lookup | 可（须同时讲 MC-13） | 可（纯链表） | 否 | 无 | 有实现，查询缺陷（MC-13） |
| mm.vm_map.mmap_create | 可 | 部分（文件钩子需桩） | 端到端需要 | 无（锁为空，DR-05） | 有实现，正确性有疑点（MC-14/18） |
| mm.vm_map.unmapped_area_search | 可 | 可 | MC-12 可达性需要 | 无 | 实际行为等于固定地址（MC-11/12） |
| mm.vm_map.munmap | 可 | VMA 层可 | 页表后果需要 | 无 | 只有 VMA 层（MC-15/16） |
| mm.vm_map.brk_stack_exec | 部分（brk 正文未锚定） | 部分 | 需要 | exec 的调度/等待前置未核 | 调用链已核；正文未逐行锚定 |
| mm.vm_map.mm_lifecycle | 可 | 不适合 | 需要 | fork 切换时机（DR-06） | 复制有、拆除无（MC-16） |
| mm.fault.arch_entry | 可 | 否 | 需要 | 自旋能否被换下依赖中断出口调度（DR-01） | 主路径可讲；错误出口均为自旋（MC-23） |
| mm.fault.pgtable_walk | 可 | 部分（页表页内存可模拟） | 需要 | DR-01（失败结果） | 有实现 |
| mm.fault.anonymous | 可 | 否 | 需要 | 无 | 有实现，新页不清零（MC-21） |
| mm.fault.file_backed | 可（到 f_op->read 为止） | 否 | 需要 | 块 I/O 等待（DR-02） | 读与私有写有实现，共享写占位 |
| mm.fault.cow | 可 | 否 | 需要 | 上下文切换时机（DR-06） | 有实现，隔离不完整（MC-24/25） |

### 4.1 下一步验证候选（全部 NOT_RUN，只是规格）

| 编号 | 目的 | 最小观察量 | 触及当前待修项 | 执行前提 | 状态 |
|---|---|---|---|---|---|
| NV-1 | 确认释放块是否合并（MC-01），以及给 page_type 置基值后合并循环是否会把伙伴留在原链表 | 从一个 3 阶块取 0 阶页再释放后，各阶 free_area 的节点集合与 count；PageBuddy 返回值 | 否（纯 page_alloc） | 宿主夹具执行授权（当前 host_fixture_execution_authorized=false）；buddy.c 与所需头文件的宿主桩 | NOT_RUN |
| NV-2 | 确认 slab_free 会释放仍有对象的 slab（MC-07） | 让同一 cache 的 ≥min_partial 个已满 slab 各释放一个对象后，free_slab 被调用时该 slab 的 inuse | 否 | 宿主夹具执行授权；alloc_pages/__free_pages 宿主替身 | NOT_RUN |
| NV-3 | 确认 VMA 查询与选址缺陷（MC-11、MC-12、MC-13、MC-14、MC-15） | 人造 VMA 链上 find_vma_intersection 的返回值、topdown 选址返回值、合并后 VMA 的 start/end/pgoff；自旋用看门狗超时记录为 INCOMPLETE_EVIDENCE 而非“无问题” | 否 | 宿主夹具执行授权；MC-14 的未初始化读需要用填充值区分，记录编译器与优化级别 | NOT_RUN |
| NV-4 | 确认匿名写缺页映射的新页是否带旧数据（MC-21） | 内核先释放一批写满特征值的页，再让新进程首次写一个匿名页并读回其余字节 | 是：需要用户进程运行，依赖调度与 exec 路径（DR-01） | 完整内核构建与 QEMU 运行授权（当前均未授权）；可运行的用户程序 | NOT_RUN |
| NV-5 | 确认 fork 后父进程能否绕过 COW（MC-25） | 父进程 fork 前后连续写同一页、在任何切换之前子进程读该页的值；同时记录 CR3 切换时刻 | 是：结果取决于调度切换时机（DR-06） | 完整内核构建与 QEMU 运行授权；能控制或记录切换时机的手段 | NOT_RUN |
| NV-6 | 确认非法访问/动态 PIE exec 时任务是否自旋、其他任务是否继续运行（MC-12、MC-23、DR-01） | 触发一次非法用户访问（或 exec 一个动态 PIE）后，其他任务的进度计数与出错任务的状态；缺页时的 preempt_count | 是：依赖中断出口调度与 in_atomic（integration-03 GAP-02） | 完整内核构建与 QEMU 运行授权 | NOT_RUN |

这些候选都没有实现夹具，也都没有启动。宿主类候选需要 `host_fixture_execution_authorized`，真实内核类候选需要完整构建与 QEMU 授权；两项当前都为 false。

## 5. 静态疑点总表

每条疑点都列出了前提、路径、反证与 unknown，详见 YAML 的 `concerns[]`。“独立审查”一列记录只读审查代理的反驳式复核结果，以及主线回源复查的结论。

| 疑点 | 能力 | 内容 | 可达性 | 独立审查 |
|---|---|---|---|---|
| MC-01 | mm.page_alloc.buddy_free | 释放出的块永远不会与伙伴合并；合并循环本身还缺少“把伙伴从原链表摘下”的步骤，但该缺陷被前一个问题掩盖，在本 pin 不可达。 | 每一次运行期释放都走这条“不合并”路径；双链缺陷潜伏。 | 初稿把后果写成“同一物理页上两条链、重复分配”；审查代理指出 PageBuddy 恒假使该后果不可达，主线回源确认（page-flags.h 第 480–491 行、mm_init.c 第 24 行），已改写并把原说法登记为 RF-01。 |
| MC-02 | mm.page_alloc.buddy_alloc | 分配路径修改 zone->free_area 时不取任何锁，而释放路径在 zone->lock + 关中断内修改同一组链表，两者没有互斥。 | 取决于上述两个未知。 | 审查代理确认；补充 zone->lock 的 spin_lock_init 被注释但 pgdat 被 memset 0，0 值即解锁态。 |
| MC-03 | mm.page_alloc.buddy_alloc | __GFP_ZERO 的高阶分配只清零第 0 页（外加第 1 页开头几个字节），其余页不清零。 | 潜伏：本 pin 未找到依赖它的活动调用者。 | 审查代理确认并给出“潜伏”判断。 |
| MC-04 | mm.page_alloc.buddy_alloc | 所有偏好区都分配失败时，区循环以 nr_zones=5 为上界读取 3 元素数组 prefered_zone_list 的下标 3、4（越界读，未定义行为）。 | 内存耗尽或高阶碎片化（MC-01 会加剧）时可达；GFP_DMA 请求在 DMA 区单独失败时即可达（审查代理补充）。 | 审查代理确认并补充 GFP_DMA 情形。 |
| MC-05 | mm.page_alloc.folio_ref_release | 经 put_page/folio_put 释放的页永远不会回到 buddy。 | 每次 folio_put 降到 0 都发生。 | 审查代理确认；漏页调用点在 fault.c 与 exec.c。 |
| MC-07 | mm.kmalloc.object_free | slab_free 在 partial 链长度达到 min_partial 时，会把仍有存活对象（inuse>0）的 slab 下线并把页还给 buddy；该判断在释放 list_lock 之后进行。 | 条件性：需要同一 cache 的对象在 ≥min_partial 个已满 slab 上交错释放。 | 审查代理确认并补充可达性条件。 |
| MC-08 | mm.kmalloc.ctor_cache_state | 所有 cache 的空闲指针都写在对象偏移 0，新 slab 的首对象不调用构造函数；因此带 ctor 的 cache 无法依赖“ctor 初始化的状态在分配/释放之间保持”。本 pin 没有找到受此影响而出错的使用者。 | 机制始终存在；本 pin 无可达的有害后果（每次非 CLONE_SIGHAND 的 fork 泄漏一个 sighand 对象）。 | 初稿推出“首次锁 siglock 会自旋”；审查代理指出对象从未安装且另有两个 ctor cache，主线回源确认，原说法登记为 RF-02、RF-05。 |
| MC-09 | mm.kmalloc.size_classes_and_large | 非编译期常量参数的 kmalloc_array/kcalloc 在总尺寸超过一页时进入 __kmalloc 的无限自旋。 | 潜伏。 | 审查代理确认并给出“潜伏”判断。 |
| MC-11 | mm.vm_map.unmapped_area_search | topdown 选址把结果放在 if (gap = ~0) 中被赋值覆盖，总是返回 (mmap_base - len) 页对齐地址，与已有 VMA 无关。 | 每次非固定、无提示的 mmap。 | 审查代理确认。 |
| MC-12 | mm.vm_map.unmapped_area_search | topdown 遍历跑完（没有 break）时以 NULL 调 vma_next_vma，后者对 NULL 原地自旋。 | 条件性；对动态链接 PIE 的 exec 可能在装载解释器时触发。 | 审查代理确认，并给出比初稿更具体的触发场景。 |
| MC-13 | mm.vm_map.vma_list_lookup | find_vma_intersection 只检查“最后一个 vm_end 严格小于 start 的 VMA”的后继：没有这样的前驱时返回 NULL；前驱的后继恰好止于 start 时也返回 NULL，即使更后面的 VMA 与范围相交。 | 常见：任何以“紧贴前一 VMA 的 VMA 起点”为起点的 munmap。 | 审查代理确认；初稿另列的“prev 为 NULL 时 munmap 自旋”被其驳回（RF-03），其实际表现已并入本条。 |
| MC-14 | mm.vm_map.mmap_create | simple_vma_merge 中 adjust_target 未初始化：prev 非空但不可合并、next 可合并时读取未初始化指针并交给 __simple_vma_adjust。 | 条件性但不罕见（例如 MAP_FIXED 或有提示地址的匿名映射紧贴下一个同类 VMA）。 | 审查代理确认，并指出 merge_prev 恒为假使其比初稿更容易触发。 |
| MC-15 | mm.vm_map.munmap | __simple_vma_adjust 先改 vm_start，再用 (start - vm_pgoff) >> PAGE_SHIFT 更新 vm_pgoff：把页偏移当地址相减，结果错误。 | 条件性。 | 审查代理确认并补充受影响范围。 |
| MC-16 | mm.vm_map.munmap | munmap 不撤销页表映射也不释放页与 VMA 结构；进程退出与 exec 换 mm 时地址空间也从不拆除。 | 每次 munmap/exit/exec。 | 审查代理确认。 |
| MC-18 | mm.vm_map.mmap_create | mmap 锁 API 全部是空函数，其中 mmap_read_lock_killable / mmap_read_trylock 返回未初始化的局部变量；缺页、munmap、brk 根本不调用锁。 | 并发前提成立时。 | 审查代理确认并列出实际调用点。 |
| MC-21 | mm.fault.anonymous | 匿名映射写缺页（以及对零页的 COW）映射的新页没有清零，用户可能读到该物理页原有的数据；由于 MC-05 用户页从不回收，这些数据只可能来自内核自身释放过的页或启动后未写过的内存。 | 每次匿名写缺页。 | 审查代理确认，补充 gup.c 的 alloc_page(GFP_USER) 同样不清零。 |
| MC-22 | mm.fault.arch_entry | 缺页处理中的栈增长分支不可达：simple_find_vma 只返回包含地址的 VMA，随后的 vm_start <= address 判断恒真。 | 栈超过预扩范围时。 | 审查代理确认。 |
| MC-23 | mm.fault.arch_entry | 任何未处理的缺页都让任务在内核内自旋而不是收到信号：内核地址缺页、找不到 VMA、处理结果非 0、共享写缺页、共享映射写保护缺页、VMA 无 ->fault（包括共享匿名映射）。 | 任何用户程序的非法访问或未实现路径。 | 审查代理确认并纠正共享匿名路径（RF-04）。 |
| MC-24 | mm.fault.arch_entry | 没有访问权限检查：对没有 VM_WRITE 的私有映射写入，会反复缺页、每次复制出一个新的只读页，直到内存耗尽后进入 MC-23 的自旋。 | 写只读私有映射时。 | 审查代理确认。 |
| MC-25 | mm.fault.cow | fork 只清父 PTE 的写位、不刷 TLB，也不为共享页增加引用；父进程在下一次 CR3 重载前可能仍经旧的可写 TLB 项写共享页，子进程会看到。 | fork 后、父进程下次切换前的窗口。 | 审查代理确认。 |

**已驳回或收窄的初稿判断（保留反证）：**

| 编号 | 初稿说法 | 结论 | 理由 |
|---|---|---|---|
| RF-01 | MC-01 初稿：合并时伙伴不摘链，导致同一物理页出现在两条空闲链、被重复分配。 | 本 pin 不可达 | PageBuddy 恒假，合并循环体从不执行（A-PA17、A-PA18、Q34）。缺陷本身保留为 MC-01 中的潜伏部分。 |
| RF-02 | MC-08 初稿：fork 出的任务首次锁 siglock 时，因对象首 8 字节是空闲指针而自旋。 | 驳回 | copy_sighand 的安装语句被注释，任务共用静态 init_sighand（A-KM14、Q33）。 |
| RF-03 | MC-17 初稿：munmap 起点不晚于最低 VMA 时 prev 为 NULL，vma_next_vma(NULL) 自旋。 | 驳回 | find_vma_intersection 只会返回某个非空前驱的后继，不会返回第一个 VMA；该情形下函数直接返回 0（并入 MC-13）。 |
| RF-04 | MC-23 初稿：共享匿名映射经 do_anonymous_page 返回 VM_FAULT_SIGBUS 后由入口自旋。 | 路径更正 | vma_init 给新 VMA 设 dummy_vm_ops（A-VM18），shmem_zero_setup 不改它，共享匿名 VMA 不算匿名，进入 do_fault 的 ->fault 缺失自旋（A-FT27）；do_anonymous_page 的 VM_SHARED 分支在 mmap 路径下不可达。 |
| RF-05 | MC-08 初稿：sighand_cache 是唯一带构造函数的 cache。 | 驳回 | bdev_cache（init_once）与 shmem_inode_cache（shmem_init_inode）也带 ctor（Q11）；两者在本 pin 无害。 |

## 6. 依赖风险

| 风险 | 依赖于 | 影响 | 说明 | 证据类别 |
|---|---|---|---|---|
| DR-01 | 中断/异常出口在 !in_atomic() 时调用 schedule()（Q32）；integration-03 GAP-02：in_atomic()/preempt_count 实际值未追踪；调度器既有问题 W01/W02/DV-11/DV-13/DV-23（integration-03） | mm.fault.arch_entry、mm.fault.pgtable_walk、mm.fault.anonymous、mm.fault.file_backed、mm.fault.cow | 缺页失败的自旋（MC-23、MC-24）是否只卡住出错任务，取决于中断出口能否把它换下；能否换下又依赖 in_atomic 与调度队列行为。 | source_only |
| DR-02 | f_op->read 之下的文件系统与块设备路径（未追踪）；integration-03 已登记的等待机制问题：swait 计数（V02/V03/V10/V11）、有限超时 DV-19 | mm.fault.file_backed | 文件映射缺页同步读文件；若底层读要等待完成通知或超时，缺页可靠性就受这些等待机制影响。 | endpoint_retained_not_traced |
| DR-03 | V12 atomic 判负、V13 trylock、swait/completion/schedule_timeout/msleep 系列（integration-03 归集） | mm.page_alloc、mm.kmalloc、mm.vm_map、mm.fault | 负检索 Q28：mm 与 arch/x86_64/mm 的活动文本不直接使用这些已知有问题的原语；内存子系统直接依赖的是自旋锁（zone->lock、slab list_lock 用关中断版本；page_table_lock 与 PMD 级锁用不关中断的 spin_lock）与普通原子操作。 | negative_search |
| DR-04 | MC-08 的 ctor/空闲指针机制 | mm.kmalloc.ctor_cache_state、external:mykernel/sched/forkexec/fork.c::copy_sighand | copy_sighand 分配后不安装对象：不会触发锁问题，但每次非 CLONE_SIGHAND 的 fork 泄漏一个 sighand 对象。 | source_only |
| DR-05 | 中断上下文是否分配内存（未追踪）；AP 是否运行（未核） | mm.page_alloc.buddy_alloc、mm.page_alloc.buddy_free、mm.vm_map.vma_list_lookup、mm.vm_map.mmap_create | 无锁分配（MC-02）与无锁 VMA 操作（MC-18）只有在并发前提成立时才出错；不能因存在 per-CPU 或 NR_CPUS 名字就认为支持 SMP。 | source_only |
| DR-06 | 上下文切换中 switch_mm 写 CR3 的时机（调度器） | mm.fault.cow、mm.vm_map.mm_lifecycle | fork 后 COW 是否被绕过（MC-25）取决于父进程在下一次切换前是否写共享页；切换时机由调度器决定。 | source_only |

## 7. 范围、方法与限制

- **读了什么：** 四个子系统目录的实际 .c 与相关头文件中的主路径函数；`arch/x86_64/mm/fault.c`；`init/main.c`、`mm/misc/mm_init.c`、`arch/x86_64/mm/init.c` 中的相关初始化；`sched/forkexec/fork.c` 与 `exec.c` 的 mm 相关函数；`lock_IPC` 中的 spinlock 表示。没有读的部分列在 YAML 的 `uncovered_old_capabilities` 与各能力的 `not_covered` 中。
- **引文：** 共 80 条，每条 1 到 5 行逐字引文，按类型（in_body、in_body_comment、macro、type_member、build）严格核对，并逐条记录所在的 `#if` 条件。另有 43 个定点检索，每个都记录了命令、范围、返回码与命中，负检索的返回码也照实记录。
- **独立审查：** 两个只读代理对初稿的 21 条疑点（页分配与堆 8 条、虚拟映射与缺页 13 条）做了反驳式复核，没有运行任何代码。主线对它们给出的关键反证逐一回源确认。随后第三个只读代理对成稿做终审，提出的 11 处问题已全部改正。过程见 evidence.md §4（与本文同一 PR 的文档提交）。
- **没有做的事：** 没有编译或运行任何 C/ASM，没有构建内核，没有运行 QEMU，也没有执行仓库脚本。没有修改内核，没有出补丁，A、B 与键下限都没有采用。
- **读阅读记录时要注意：** YAML 的 `observed_not_anchored` 是读源码时顺带看到但未锚定的点，状态为 not_assessed，不构成结论。

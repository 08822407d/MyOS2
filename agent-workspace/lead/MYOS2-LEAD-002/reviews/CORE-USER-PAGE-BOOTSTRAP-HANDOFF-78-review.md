---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78-REVIEW-001
record_type: independent_bounded_review_with_documentation_return
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner在本C06明确选择）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract112_short12: ea5eb636ae89
reviewed_base_short12: 5a8c5dc1cd38
reviewed_head_short12: 03519ebb5d86
reviewed_delivery_comment: 6825390768
acceptance_verdict: RETURN
returned_scope: [R78-01]
returned_scope_kind: definition_boundary_and_evidence_navigation_documentation_only
accepted_partial_scope: D00已独立闭合77；A/B/C技术主干按R78-S01/S02/S03限定保留
R77_01: CLOSED_DOCUMENTATION_ONLY
batch77_composite_acceptance: PASS_PENDING_LOCAL
mandatory_supplements: [R78-S01, R78-S02, R78-S03, R77-S01, R77-S02, R77-S03]
original_artifacts_modified: false
original_numbered_definition_labels_readable: 136
original_raw_prefixes_readable: 136
original_mechanical_prefix_definition_or_JSON_scan: NOT_RUN
original_model_reexecution: NOT_RUN
original_DOT_parser_Graphviz: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
self_check: {verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, scope: 本回执六条短锚，按固定源可读核对}
next_followup: CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79
next_task_ref: ../114-user-memblock-region-lifecycle-contract.md
next_executor: MYOS2-A-C06a / MYOS2-LEAD-002-CHAT-EXEC-10
phase: 2
owner_decisions_requested_now: []
---

# 第78批：文档部分退回，技术主干限定保留

独立审核已完成，整包RETURN，仅R78-01文档补正；不要求修改内核或重做78。77/D00已满足原退回要求，R77-01在同轮独立闭合回执中关闭，77组合结论为PASS_PENDING_LOCAL。78的定义引文与证据导航纠正并入79/D00，之后连续三个新专题；由新执行对话10承接，旧C05c/09不再使用。

## 1. 实际交付与核查边界

PR17正式评论6825390768及直接执行ref均指向03519ebb5d86。独立compare 5a8c5dc1cd38→03519ebb5d86为一提交、ahead1/behind0，严格六件added共4156行：MANIFEST128、正文88、facts2013、DOT149、deferred342、evidence1436；没有旧文件净修改或删除。PR17仍Open/Draft，PR16与内核直接分支未因执行而推进。09停写依据其正式交付与Owner本轮“C05c无法再使用”的明确说明，不冒称监控后台。

主线完整读完六件，长件分段至末尾。A43+B44+C49共136个有编号标签均已可读检查；其余五件使用字符名称而没有把说明文字变成额外保留前缀，原始可读总数136。这个计数不等于136条都满足定义边界或支持全部引用它们的断言；下述A31和交叉索引问题分别判定。未运行机械扫描、JSON解析、DOT解析或原模型。

固定源码采用本轮新回源及本有效C06此前直接取得、pin/blob相同的不可变对象，覆盖强制结论和所读短锚的真实定义；不是继承09已读声明。26份台账中的引用来源均在这些直接读取范围内，未宣称本轮全文重读所有26份。两份超24软上限的地址转换/位扫描最近依赖有实际必要性，不因超限本身退回。完整本地P9、预处理、ABI、构建与物理效果仍NOT_RUN。

## 2. R78-01：A31定义边界及跨文件证据导航补正

原evidence的A31标识为page_macro_arch.h::__va，实际引文两行包含前置的ifndef条件指令和下一行宏定义。前者控制条件编译，不属于所标__va宏本身的定义体；P2/P9-2及112要求单一定义内连续原文。因此原包不能因字面能够命中就整包通过。不是代码编造，也不是空白行/尾换行争议，更不恢复历史58/63误判。

正确的定义内最小锚如下；条件编译前提可在锚外另述，不能混进同一宏定义引文。

[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```

此外，正文/facts/deferred中若干B编号没有指向所述写入，存在“真实引文但不足以支持该链接断言”的问题，主要映射见S01。79/D00须在新六件内追加A31的定义内替代引文、旧位置及替代消费范围，并落实S01索引纠正、S02图上下文和S03限定；原78六件和评论保持冻结。不重做136条技术分析，不把已有主线补充冒充执行者原先已交正确。

执行者只能报告DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06；是否闭合由C06下轮独立判断。新79计数按其六件全文单独计算，不能混入旧136，说明文字不得再次出现连续保留前缀。新纠正证据自身同样须回源。

## 3. R78-S01：证据索引、问题身份与统计口径

以下是原78证据的实际内容，不改旧编号。后续涉及这些断言时以本表替代错位链接；不是将无关引文继续当证据。

| 论点 | 正确的原78锚或本回执补充 |
|---|---|
| setup_arch请求zone_sizes_init | B03；不是free_area_init内部边界赋值 |
| free_area_init清边界数组 / 清pgdat | B04 / B05；边界赋值循环需读真实函数，不能仅引用B03 |
| 空节点提前return / 调用者未初始化输出 / 读取输出 | B06 / B11 / B12；B10是callee正常尾部写跨度 |
| zone start/spanned/present的首次写入 | B13；B12只到real_size计算 |
| node nr_zones与总数 | B14；不是锁初始化 |
| 显式锁初始化为注释 / core读取统计 / 调用zone_init | B15 / B16 / B17 |
| memmap_init起始PFN和循环 / present覆写0 / 链表与page遍历 | B18 / B19 / B20 |
| 单页refcount与mapcount注释 / lru和buddy_list初始化 | B21 / B22 |
| 启动入口 / mm_core_init调用 | B40 / B41；B40不是present覆写证据 |
| map分配、core初始化、memmap_init先后 | B44（亦C46） |
| PB78-F05的生产、覆盖与先后 | B13+B19+B44；原deferred的B12/B18/B40不足以替代 |
| internal失败后去掉下界重试 | 本回执下方真实锚；A10只含首轮调用，不含retry |

facts.zone_initialization_matrix的memmap_init项，以及span_hole_and_reserved_matrix的zone coverage reset项，将present覆写标为PB78-F06是编号错误；二者均应消费为PB78-F05。PB78-F06保留给memblock_free_all的未初始化pages累加，不改任一qualified_id。

[VERIFIED mykernel/mm/misc/mm_init.c::memmap_init]
```c
		zone->zone_start_pfn = start_pfn;
		zone->spanned_pages = end_pfn - start_pfn;
		zone->present_pages = 0;
```

finite_model_cases有32条记录，其中M32明写historical_D00_counts、无源码锚且NOT kernel model。应记31条源码数值/条件模型记录＋1条文档计数记录，不称32项内核模型或32个独立技术见证。主线只可读核查其内容，没有重跑原模型或图计数。

## 4. R78-S02：分开失败路径和初始化顺序

原DOT共享A_ZERO→A_VIRT→A_BAD_NULL，使从A_MEMINT到物理失败的路线也能被读作继续转换。实际memblock_alloc_internal在可能重试之后先检查物理0，失败直接返回NULL；只有alloc_node_mem_map直接组合物理分配与转换后才测试虚拟map。两条调用者路径不得拼接。

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	if (!alloc)
		return NULL;

	return (void *)phys_to_virt(alloc);
```

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]
```c
	alloc = memblock_alloc_range(size, align, min_addr, max_addr);
	/* retry allocation without lower limit */
	if (!alloc && min_addr)
		alloc = memblock_alloc_range(size, align, 0, max_addr);
```

PB78-F03的4096字节与16MiB是两个调用者的首次请求参数差异；normal包装器首轮失败可去掉min_addr，后续range再施加PAGE_SIZE下界。因此不能宣称normal在所有成功分配中都位于16MiB以上，也尚未证明mem_map必须放在那个区间。

原图A_FREE_INIT标签“清pgdat”之后再连接B_ZONEB，不能解释成真实顺序。free_area_init先清并填边界数组，再取NODE_DATA(0)和清pgdat、取PFN范围、统计、分配map、core、memmap_init。

[VERIFIED mykernel/mm/misc/mm_init.c::free_area_init]
```c
	for (int i = 0; i < MAX_NR_ZONES; i++) {
		end_pfn = max(max_zone_pfn[i], start_pfn);
		arch_zone_lowest_possible_pfn[i] = start_pfn;
		arch_zone_highest_possible_pfn[i] = end_pfn;
```

B_MEMMAP→B_RESERVED仅可表示跨阶段依赖；真实预留初始化请求由mem_init→memblock_free_all→memmap_init_reserved_pages发出。C_MEMINIT→C_SLAB须解释为mm_core_init在mem_init正常返回后的下一调用，不是mem_init直接调用kmem_cache_init。数据/算术/锁节点不能被当作真实函数名，CALLEE和CALLER_SEQUENCE分开；所有后续写入均有前序合法返回门，没有自动回滚。

## 5. R78-S03：地址、边界和初始化候选的条件

PB78-F01保留条件性原点/容量风险。start1024、span16例得到尾后一地址map+1024：合法数组的尾后一指针形成，与把它当page对象解引用不是同一操作。start1025例的map+1026已超出该声明extent，另作地址形成门。不能仅因local/global下标含义不同就宣布实际物理页错配，需继续区分真实生产者、分配容量、消费者和编译模型。

同理，for_each_memblock_type在cnt==max时形成尾后一指针并在循环体读取base/size，问题不能简写成“形成尾后一指针必错”。__simple_next_mem_range尾gap则可能只用r[-1]并在条件分支避免读取r->base；在正确数组和索引前提下，不应强加所有迭代器都必须额外存一条尾记录的结论。循环增量、cnt+1溢出及未知生产者仍需分别限定。

PB78-F02使用的是源宏中先强制转换为void*再加PAGE_OFFSET的表达式；高端非NULL结果只在声明的目标/GNU算术解释下保留，不改写成已证明的可移植整数地址定理，更不证明页表映射可写。__ffs(0)依所读函数自身的接口注释缺少有效输入保证，不由此判定某一CPU/编码的唯一机器返回值。

PB78-F04的空节点0/0早退并非唯一未写nr_absent/nr_spanned的出口，所读函数还有下面的无交集早退。它补充同一条件候选，不新编根因，也不证明真实启动一定走该分支。

[VERIFIED mykernel/mm/misc/mm_init.c::__myos_calculate_node_pages]
```c
	if (*zone_end_pfn < node_start_pfn || *zone_start_pfn > node_end_pfn)
		return 0;
```

PB78-F06未初始化pages读取的断点保留；其前还有预留遍历、合法PFN和返回前提，之后chunk仅为隔离的条件模型。统计managed_pages先增、after_bootmem先置1均不能证明后续buddy归还成功。F01–F06与U01–U08继续needs_evidence，旧MC、GAP-M08、MR-02、AE75-U07、SL76-U09及PA77-F01/F02不因初始化来源被读到而自动闭合。

## 6. 独立性、读取与接续

本轮新回源：page_api_arch.h、numa.h、mmzone_const_arch.h、memblock_const.h、page_macro_arch.h全文；cache.h1–55、io_arch.h1–75、page_const_arch.h68–末尾、bitops_arch.h100–138、memblock.h1–105、setup.c全文、main.c105–160、page-flags.h323–362、mm_init.c110–190、memblock.c101–285。其余所需固定memblock.c/mm_init.c/buddy.c/buddy.h/mmzone.c及相关宏/结构的正文，复用本有效C06前轮直接完整或定点取得的同一不可变Git对象；不冒称本轮全部重新打开。

本回执六条短锚与固定源逐字可读核对；对原136标签的字面/定义与支持关系审查不等于自动扫描或全部程序正确性证明。D00仅关闭77文档，78原包仍RETURN。完整本地P9、JSON/图解析、原模型重放和内核运行NOT_RUN；同族独立会话验收，不是异族复核。

后继由113交接包和114合同共同限定：Owner已停用旧C05c，C06制定新10的条件写授权；新接收者先Mnemosyne接收报告，再独立完整读取九份MyOS2规则和110，然后单独加载适用Mnemosyne指导，G00通过后连续79/D00、G01–G04。发布不表示新聊天已创建或开始。原件冻结、63安全重试、79合包和110以后交接必读继续有效。

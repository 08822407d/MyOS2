---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EARLY-PGTABLE-STORAGE-81-REVIEW-001
record_type: independent_bounded_static_acceptance_with_append_only_corrections
evidence_class: 六件全文、固定定义、参数传递与远端净差异的可读审核；非运行证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner交本主线独立审核）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract116_short12: df960bf5c5e9
reviewed_base_short12: 0e9e9e89c4be
reviewed_head_short12: 05fe580c12a3
reviewed_delivery_comment: 6842499415
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/116-user-early-pgtable-storage-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
  - agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-review.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/early-pgtable-storage.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/early-pgtable-storage-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/evidence.md
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R81-S01, R81-S02, R81-S03, R80-S01, R80-S02, R80-S03]
returned_items: []
new_D00_required: false
original_artifacts_modified: false
original_readable_definition_labels: 99
original_readable_raw_prefixes: 99
original_mechanical_scan_JSON_DOT_or_model_replay: NOT_RUN
self_check: {verified_claims: 9, quotes_reconfirmed: 9, downgraded_to_inferred: 0, scope: 本回执九条固定定义短锚；可读回源，不是原99条机械闸门}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-DIRECT-MAP-RANGE-82
next_contract: ../117-user-direct-map-range-contract.md
executor: MYOS2-A-C06a / MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第81批按补充限定接收；不新增执行返工

结论为PASS_PENDING_LOCAL，必须将原六件与本回执R81-S01/S02/S03组合使用。主线直接补齐证据索引、路径条件和解释，不改作者原件或旧评论，不把错误索引追认为原来正确。没有新增RETURN，也不安排D00；第82批继续三个新专题。79文档闭合及80既有接收不重开，代码候选不因报告接收而被关闭。

## 1. 实际交付及可读核查

直接执行ref、PR17与正式评论6842499415均指向05fe580c12a3。compare 0e9e9e89c4be至05fe580c12a3为ahead1/behind0、一个提交，严格六件added共3132行：MANIFEST164、正文66、facts1567、DOT168、deferred295、evidence872；旧文件零净修改删除。PR16/17保持Open/unmerged，17仍Draft。STOP_WRITING依据正式交付和Owner交回，不冒称观察另一聊天后台。

六件及正式评论已独立完整读取。源码标签按A32+B37+C30可读计数为99，其余五件未见额外原始保留前缀，MANIFEST三数99/99/0一致；14个原字段、公约整句、time/master原文金丝雀、read_channel均存在。短引文采用本轮新回源与本有效C06此前直接取得的相同固定对象核对，不能用作者99/99自报替代本次判断。未发现40位十六进制串或新宏外条件指令混入；空白物理行不自动构成失败，链接脚本按实际输出节定义范围消费。

原证据标签字面/定义范围与正文、矩阵对它们的交叉引用是两个检查面。后者确有A组错位及C28/C29混淆，不能笼统称所有原支持关系无误。本回执提供完整的事实定位替代、关键源码和消费范围，满足组合接收；未把这些错误隐入普通源码缺口或转给Owner处理。

可读P9的1/2/3/4/5/7/8/11/13依本回执限定接收；外部规范未用于补源码事实，6无新增外部证明；9/10/12无代码、补丁或降级交付，相关项N/A。机械全扫描、JSON/DOT解析、编译、原模型重放及完整本地P9均NOT_RUN，不能升级PASS。

交付统计为12矩阵78行（18/26/34）、32隔离条件样本（8/9/15）、8候选记录及10缺口。22项source ledger中20份有短锚，另两份是最近定义。作者图63节点/90边/0悬空、99次二读是作者自检账，本轮未运行其计数器。矩阵、样本、引文互有重叠，不合计为更多独立技术见证。

## 2. R81-S01：以事实定位修正A组及C组索引

A16实际是extend_brk的return，而正文/facts等仍多处把A16当early_alloc_pgt_buf申请、把A17当状态写入。不能机械地给所有A编号加一，也不能修改原99个标签。后继统一按下表定位原evidence中的真实内容：

| 待支持事实 | 原81正确标签 |
|---|---|
| 三个buffer游标对象；尺寸三个宏 | A01/A02/A03；A04/A05/A06 |
| RESERVE_BRK定义、.bss与.brk模式 | A07；A08/A09/A10 |
| _brk_start/_brk_end初值 | A11/A12 |
| extend_brk参数、检查/对齐、推进/清零、返回 | A13；A14；A15；A16 |
| early_alloc_pgt_buf的tables、extend_brk实参和__pa | A17，不是A16 |
| start、PUD/PMD别名、end、top五次写入 | A18，不是A17 |
| PUD与PMD静态指针 | A19/A20 |
| setup_arch中申请与reserve_brk调用 | A21/A22 |
| reserve_brk请求与_start归零 | A23/A24 |
| PAGE_SHIFT/PAGE_SIZE | A25/A26 |
| PMD_SHIFT/PMD_SIZE | A27/A28 |
| __START_KERNEL_map、__pa、__phys_addr_nodebug、__va | A29/A30/A31/A32 |
| get_max_mapped读取上界 | C28 |
| 被注释的add_pfn_range_mapped登记 | C29，不是C28 |

此表替代正文G01、facts.G01及M01–M08、C-X01/C-X03、deferred F01/F02和DOT的A组证据导航；对每项事实取实际支持它的标签组合，不把对象定义当状态更新。C29替代正文G03、facts.C-M02和F06中用于证明登记被注释的C28；C-M03关于get_max_mapped仍使用C28。

[VERIFIED mykernel/arch/x86_64/mm/init.c::early_alloc_pgt_buf]
```c
void __init early_alloc_pgt_buf(void) {
	ulong tables = EARLY_PGTBUF_SIZE;
	phys_addr_t base = __pa(extend_brk(tables, PMD_SIZE));
```

[VERIFIED mykernel/arch/x86_64/mm/init.c::early_alloc_pgt_buf]
```c
	pgt_buf_start = base >> PAGE_SHIFT;
	early_pgtbuf_pud = (void *)base + __START_KERNEL_map;
	early_pgtbuf_pmd = (void *)early_pgtbuf_pud + PAGE_SIZE;
	pgt_buf_end = pgt_buf_start + 2;
	pgt_buf_top = pgt_buf_start + (tables >> PAGE_SHIFT);
```

facts.A-P05所说reserve_brk与提前映射之间还有另一映射hook不成立；实际紧接的是本批myos_early_map_pgt_buf_into_directmap，cleanup_highmap仅注释。后面才是e820登记和init_mem_mapping。按真实调用者顺序消费：

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	reserve_brk();

	// cleanup_highmap();
	myos_early_map_pgt_buf_into_directmap();
```

原C04只引了回退请求参数，没有引ret初值与mapped窗口条件。以下补齐该支持，不从两个全局变量定义推出窗口有效：

[VERIFIED mykernel/arch/x86_64/mm/init.c::alloc_low_pages]
```c
		ulong ret = 0;

		if (min_pfn_mapped < max_pfn_mapped) {
```

B-W05关于保护位过滤应合读pgtable_const_arch.h::__pgprot_mask的真实定义，其计算为传入位与__default_kernel_pte_mask按位与后构成pgprot；B30–B32仅列调用宏和原始位，不单独证明该实现或动态掩码值。该定义本轮已从固定源码直接取得。

facts.methods.source_quote_check仍写prepublication reopen pending，而MANIFEST及正式评论说二读完成。前者作为未同步的准备期字段保留；本回执不倒填作者历史检查日志。本轮独立可读核对与作者二读声明分别成立，不以此字段推断执行者仍在写入，也不声称原日志已恢复。

## 3. R81-S02：直接页分配不是slab过滤路径

R77-S01针对slub.c::alloc_slab中的prepare_alloc_flags与对象层hook，不能推广为所有页分配都先去掉ZERO。81晚期路径直接调用__get_free_pages，其直接参数链如下。

[VERIFIED mykernel/mm/page_alloc/buddy.c::__get_free_pages]
```c
{
	page_s *page = alloc_pages(gfp_mask & ~__GFP_HIGHMEM, order);
	if (page == NULL)
		return 0;
	return (ulong)page_to_virt(page);
```

[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
	page = __myos_alloc_pages(gfp, order);

	return page;
```

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
			if (likely(page)) {
				prep_new_page(page, order, gfp);
				return page;
			}
```

[VERIFIED mykernel/mm/page_alloc/buddy.c::prep_new_page]
```c
static void
prep_new_page(page_s *page, uint order, gfp_t gfp_flags) {
	post_alloc_hook(page, order, gfp_flags);
```

本树buddy_const.h的___GFP_HIGHMEM为0x02u、___GFP_ZERO为0x100u，类型宏分别引用相应位。对81晚期给出的GFP_ATOMIC|__GFP_ZERO，去HIGHMEM不去ZERO；上述中间路径没有slab的prepare_alloc_flags。若合法order、成功取得页且前序均合法返回，则该路径向页hook传递仍含ZERO的参数。不能把“受R77限制”读成此处ZERO已被slab过滤；也不能由参数位推断分配一定成功。

[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```

配合本树clear_page到memset(PAGE_SIZE)的宏，在声明GNU字节步进、PAGE_SIZE=4096、order=2、地址有效且每次调用合法返回的隔离模型中，四次请求并集为[base,base+4099)，不是四页16384字节。它只描述请求覆盖，不证明剩余字节实际非零或真实内核曾运行该例。原77已记录同一hook的order1/3区间；PT81-F07保留原ID，标为该已知行为的新直接消费者证据，不宣传为独立新根因，不重开77。

## 4. R81-S03：地址回译、模型分支和图示限界

“镜像别名”不能无条件等同原brk对象。令K为__START_KERNEL_map，在所选__phys_addr_nodebug镜像分支内__pa(v)=v-K+phys_base；随后代码用base+K构造地址，代数上为v+phys_base。是否与原v指向同一可写对象还需phys_base及映射前提，不能仅因虚拟v按PMD_SIZE对齐便断言物理base或重构别名正确。这里只取固定表达式作条件推导，不认定真实phys_base非零或已发生错映射。

B-S08把phys_end<=phys_start归入同一异常范围过宽。正的相等端点给出nr_pmd=0，若其它前提合法仍可能执行父页清零/populate、刷新与单根项复制，正如M13；反向端点的无符号差与phys_end=0时的减一是不同模型。分析停止不是源码中的拒绝分支，模运算本身不等于CPU自动异常。

M24的早期memblock返回0例必须追加“容量不足”的门，例如after_bootmem=0、num=2、remaining=0且有效mapped窗口；否则函数可能走buffer分支，根本不消费该假定返回值。窗口无效时ret保持0、没有memblock调用也可进入同一失败语句；DOT只画回退callee返回不能覆盖这一分支。

get_order文档的size0结果未定义是接口语义限制，不可直接写成C语言必然UB或陷阱。固定实现常量0分支明确返回BITS_PER_LONG-PAGE_SHIFT，动态路径另有无符号减一、移位和fls64。选择哪个编译路径与有效阶数/成功分配分别判断；不得把零页请求认证为普通成功。M29的ULONG_MAX游标与M30极大num是隔离数值前提，不代表合法启动PFN状态可达。

图中setup到extend_brk压缩了early_alloc_pgt_buf这一中间调用者，按说明作关系路径而非直接CALL或完整CFG；共享reserve、clear、flush节点不能拼接出跨调用者控制流。原索引标签按S01消费。1GiB/512GiB越界模型还要求真实地址域、对齐与父项有效，8MiB容量本身不能保证不跨界。根项复制、刷新请求和当前活动CR3是不同事件，不证明全CPU即时可见。

PT81-F01–F08及U01–U10继续needs_evidence；8是候选记录数，不是8个已复现故障或8个已证明独立根因。AE75、SL76、PA77、PB78、MR79、BM80、原MC/LB/GAP、MR-02收窄闭合及58/63撤回均保持原身份和限界。

## 5. 方法、独立性和后继

本轮新回源覆盖setup.c全文、kernel.lds145至末尾、page_arch/page_macro_arch全文、pgtable_const150–290、pgtable.h180–290、pgtable_arch430–565、pgtable_macro_arch1–55、getorder全文、buddy.c125–210与420–545、mempolicy全文、buddy_const相关位定义段、asm-generic/page全文、mmu_arch240至末尾、init-mm全文、memfault_api1–45、page_const65–90及highmem_macro全文。buddy.h1–150和buddy_macro全文只作定位依赖；一次默认分支搜索仅定位mempolicy，结论回固定time。

init.c相关81定义、asm/setup.h、PAGE_SHIFT/PAGE_SIZE、mm/fault/pgtable_macro、memblock相关定义、公约和time/master金丝雀使用本有效C06前轮已直接取得的相同固定对象。82设计另重读init.c225–535。没有继承执行者已读声明；完整逐文件范围与权限定界在本轮检查点，未声称全仓或每个来源全文复审。

九条本回执短锚与固定源码逐字可读核对。原99条、图和32模型未执行机械闸门/解析/重放；同族独立会话审核不等于异族复核。117号安排现C06a/10执行第82批，先G00后连续G01–G04，无D00；发布不等于该聊天已经收到或启动。110号后续主线交接固定必读不变。

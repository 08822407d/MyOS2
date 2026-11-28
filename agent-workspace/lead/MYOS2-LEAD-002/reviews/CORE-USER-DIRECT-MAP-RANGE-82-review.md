---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-DIRECT-MAP-RANGE-82-REVIEW-001
record_type: independent_bounded_static_acceptance_with_consumption_corrections
evidence_class: 六件全文、固定源码与远端净差异的可读审核；非内核运行证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner本轮明确要求独立验收）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract117_short12: e8f950fa4ad8
reviewed_base_short12: 05fe580c12a3
reviewed_head_short12: 3f41eb242040
reviewed_delivery_comment: 6843923007
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/117-user-direct-map-range-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md
  - agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
  - agent-workspace/tasks/00-gpt-task-protocol-v2.md
  - agent-workspace/conventions.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/direct-map-range.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/direct-map-range-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/evidence.md
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R82-S01, R82-S02, R82-S03, R81-S01, R81-S02, R81-S03]
returned_items: []
new_D00_required: false
original_artifacts_modified: false
original_readable_definition_labels: 87
original_readable_raw_prefixes: 87
original_mechanical_scan_JSON_DOT_or_model_replay: NOT_RUN
self_check: {verified_claims: 8, quotes_reconfirmed: 8, downgraded_to_inferred: 0, scope: 本回执八条短锚的逐字可读核对；不是原87条机械检查}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-DIRECT-MAP-TABLE-INSTALL-83
next_contract: ../118-user-direct-map-table-install-contract.md
executor: MYOS2-A-C06a / MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第82批按补充限定接收；没有新增执行返工

结论为PASS_PENDING_LOCAL，必须把原六件与R82-S01/S02/S03组合使用。主线直接补充样例阶段、算术单位、分类及图示消费边界，不改作者原件或旧评论，不追认原样例前提已经齐全。没有新增RETURN、没有D00；第83批继续三个有界新专题。79/80文档闭合、81接收及更早限定不重开。

## 1. 正式交付与可读门

执行direct ref、PR17及评论6843923007均指向3f41eb242040。compare 05fe580c12a3至新头为ahead1/behind0、一个提交，严格六件added共4338行：MANIFEST169、正文70、facts2636、DOT124、deferred530、evidence809；旧文件零净修改删除。PR16/17 Open/unmerged，17仍Draft。停写依据Owner交回与正式交付声明，不冒称监控另一聊天后台。

六件及正式评论独立完整读取；facts长响应的截断以重叠分段补齐。87个源码标签为A28+B24+C35，其他五件可读检查未见额外原始保留前缀；MANIFEST三数87/87/0、14个原字段、公约原句、time/master逐字金丝雀和read_channel齐备。没有把source_fact_index、deferred中的重复引文再算成新增标签。

全部87个短锚按本轮新回源及本有效C06此前直接取得的同一固定对象作字面和定义范围可读比对；未见宏外条件混入或真实路径不存在。source_fact_index的事实键与新证据ID对应，未复现81的整体编号错位；但“字面在引文里出现”仍不是充分证明，下面的样例条件必须追加。作者87次二读、内容检查及图46节点/63边/0悬空是作者自检账，本主线未机械重跑。

可读P9的1/2/3/4/5/7/8/11/13按本回执范围接收；六件通读未见40位十六进制串或运行认证总括。外部硬件规范未作结论依据；6无新增外部认证；9/10/12无代码、补丁或降级件，相关项N/A。原件全文机械扫描、JSON/DOT解析、原30模型执行及完整本地P9均NOT_RUN，结论不能升级PASS。

实际技术记录为9矩阵57行（A16/B17/C24）、30隔离样本（A9/B12/C9），13项直接源码台账、8个条件记录和10个缺口。矩阵、样本和引文有重叠，不相加作为独立技术见证。主线未运行内核或编译来认证任何样例。

## 2. R82-S01：先拆分、再提升、后合并，样例必须带阶段

原M10的mask0、M12的mask4、M14的五段mask0/4/12/4/0，以及正文“正常情况下仍有五段”，只能无条件描述提升前状态。完整split在合并前还有下面的真实门。

[VERIFIED mykernel/arch/x86_64/mm/init.c::split_mem_range]
```c
	if (!after_bootmem)
		adjust_range_page_size_mask(mr, nr_range);
```

[VERIFIED mykernel/arch/x86_64/mm/init.c::adjust_range_page_size_mask]
```c
			if (memblock_is_region_memory(start, end - start))
				mr[i].page_size_mask |= 1<<PG_LEVEL_2M;
```

替代消费：这些样例要么明确停在提升前，要么补after_bootmem非0、或相应包含查询均不提升的前提，才能把给定mask称为最终结果。M10的字节区间[4096,8192)本身不因提升而变，但候选位可变；M12的[0,2MiB)也可能追加1G候选位。1G添加还须合读原A17/A18，不由2M查询成功推出。

补充一个未执行的反例模型：M14的start=4096、end=2149584896、global mask=12；after_bootmem=0，memory为一条合法、稳定、可查询的[0,3GiB)记录，全部先行整数和存储门成立。拆分先得五段；各段按PMD/PUD向外取整均被这条记录包含，提升后五段mask都成为12，合并后是一条[4096,2149584896)。这里的3GiB只是声明输入，不是实际固件内存；单条MR也不证明用1G叶完成了映射。

M15的global mask0使这两个提升分支都不进入，五个同mask片段合并为一段的结论可保留。M17只表示“进入合并比较时mask4与mask8不同”；不能把提升前的两种mask视为提升后仍必不同。M06须补MR原来缺2M位、全局允许该位；若通过split到达，还须after_bootmem=0。其余能力模型的初始/重复状态必须按各自输入分开，重复调用并非真实启动观测。

以上限定覆盖正文G01/G02、facts相应模型及BM04、相关图边的最终mask消费。原87标签与30模型编号不改，不把主线反例倒填成作者第31个已执行实验。

## 3. R82-S02：字节取整、PFN取整与反向clamp分开

原M09的字节上端向外取整与M20提到的split高端回绕不能混作同一运算。split先将字节值转成PFN，而后在PFN单位按PMD/PUD页数取整。

[VERIFIED mykernel/mm/highmem/highmem_macro.h::PFN_DOWN]
```c
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
```

[VERIFIED mykernel/include/linux/kernel/math.h::__round_mask]
```c
	#define __round_mask(x, y)	((__typeof__(x))((y)-1))
```

在明确的64位ulong、PAGE_SHIFT=12数值模型中，原字节值不超过2^64-1，PFN_DOWN结果不超过2^52-1；再向512或262144页对齐，最高到2^52，不会仅因原字节端点很高就跨越64位上界。零输入时round_up的中间减一按无符号模运算另记，最终可得0，不是CPU自动异常。M20因此保留为“需指明实际操作数、单位和转换”的条件记录，不能宣称该split路径已有取整溢出反例。adjust对字节end直接向上取整的M09是另一条可构造回绕的路径；物理区间解释失效也不等于语言算术自动陷阱。

反向请求的源码行为同样比“分析停止”更具体：

[VERIFIED mykernel/include/linux/kernel/minmax.h::clamp_val]
```c
	#define clamp_val(val, lo, hi)	clamp_t(typeof(val), val, lo, hi)
```

[VERIFIED mykernel/include/linux/kernel/minmax.h::clamp_t]
```c
	#define clamp_t(type, val, lo, hi)	min_t(type, max_t(type, val, lo), hi)
```

本轮还读取了active min_t/max_t及__cmp定义。对本调用的稳定、无副作用、按同一选中类型可表示的L>H，min(max(v,L),H)恒为H；两端都会变成H，因此start>=end，当前区间被跳过。若memory迭代本身合法终止，init_range_memory_mapping可返回请求累计0；不是源码在发现反向范围时自动panic或停机。M28与CB08只能停止“合法物理映射已建立”的证明，不能据此断言下一次顶层请求必不可达。转换不满足这些前提时不套此代数结论，也不把clamp当通用输入验证器。

请求累计、最后返回PFN和硬件成功仍三账分离。M23/M29给定的callee返回是隔离输入，不证明真实下层能产生该值；多个memory记录若重叠，累计亦不自动等于去重后的并集长度。正常洞模型的16384字节只在其声明的不重叠记录和callee正常返回前提下成立。

## 4. R82-S03：容量、候选类别和图示限界

F05/M18的nr_range=5是假定输入，不能忽略实际init_memory_mapping以0调用split、五段各最多保存一次这一控制流上界。所读正常调用没有第六次保存；外部错误nr或先行损坏必须另证，不以“没有数组检查”概括已有panic门。

[VERIFIED mykernel/arch/x86_64/mm/init.c::save_mr]
```c
	if (start_pfn < end_pfn) {
		if (nr_range >= NR_RANGE_MR)
			panic("run out of range for init_memory_mapping\n");
		mr[nr_range].start = start_pfn<<PAGE_SHIFT;
		mr[nr_range].end   = end_pfn<<PAGE_SHIFT;
```

本轮为核该边界完整读了固定panic.c；它尾部的等待循环处在注释中，而不是以函数名或文件注释为不返回证据。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	// /* Do not scroll important messages printed above */
	// suppress_printk = 1;
	// local_irq_enable();
	// for (i = 0; ; i += PANIC_TIMER_STEP) {
	// 	touch_softlockup_watchdog();
```

这只是新增函数文本证据。声明属性、打印与禁中断等callee、编译选择和实际机器路径未作完整证明；不宣布真实panic一定返回或一定停机。F05/U06保留原身份及条件，不把以上读取冒作修复或运行闭合。

AP07/F03/U03引用memblock_search的空cnt风险时，须区分逻辑记录数与底层数组容量。cnt0的do-while仍读slot0，但slot0是否构成C越界取决于实际backing；在有效、零初始化slot0且稳定字段模型中，base=0,size=0可走到-1返回，不能仅由cnt0认定非法访问。原MR79其它计数/指针/变形限制不自动关闭。PFN迭代器的cnt0路径则先跳过while、置idx=-1，不混成同一个函数。

F02首先是memory查询不等于free查询的证明边界；映射被reserved占用的RAM与重新分配该RAM不是一件事。没有额外策略依据，不能把“不减reserved”直接升级成内核缺陷；NOMAP义务单独保留缺口。F07/F08的返回接口和请求累计也不是单凭名字即可裁定的错误。八个候选记录保持needs_evidence，不能称八个独立已证根因或实际故障。

DOT保持静态关系图：B_head等是split里的阶段，不是另五个函数；它们到save_mr的真实调用锚应分别合读B09/B11/B13/B15/B16，不能只靠callee签名。C_top→C_isa→C_inner是同一个ISA调用的分层表示，不是两次调用。C_inner→C_sum只在init_range调用者返回后成立，不能给ISA路径增加不存在的累计。after_bootmem非0可绕过提升仍进入合并；bottom_up为真时先有两次请求返回，为假时跳过它们。ANALYSIS_STOP是证明边界，不是内核中的同名控制转移。

## 5. 读取方法、独立性和接续

本轮完整读取82六件、评论、117、63、79、协议v2、公约；00/18/24/28/110和81/80/79等适用回执按本有效C06此前实际取得的固定对象复用，不借执行者读取资格。没有声称新一轮全量Mnemosyne刷新或恢复历史临时报告。

新回源包括init.c225–865（后半同时为83预读）、math.h1–80、minmax.h1–160、memblock.h全文、memblock.c75–110及630–710、memblock_macro全文、cpufeatures35–112、pgtable_const1–20、e820_const全文、time options全文和panic全文，以及master两条金丝雀所在段。尺寸、setup、地址转换和高端PFN宏等其余强制锚复用本有效C06同一固定对象；详细范围见本轮检查点。

八条本回执短锚逐字可读核对；原87条没有机械扫描、语法解析或执行原模型。上文新增反例和代数推导均为未执行的条件推理，不冒称宿主测试。同族独立会话审核不是异族复核；完整本地P9、预处理/构建、Graphviz、内核/QEMU/实机和硬件验证全部NOT_RUN。

第83批按118号在现C06a/10执行，承接下层建表边界，先G00后连续G01–G04，无D00。旧PA77/PB78/MR79/BM80/PT81、MC/LB/GAP、MR-02收窄及58/63撤回继续原身份和限界；DM82不因被下一包引用而自动关闭。110号以后每次主线交接固定必读不变。任务发布不等于执行聊天已收到或已启动。

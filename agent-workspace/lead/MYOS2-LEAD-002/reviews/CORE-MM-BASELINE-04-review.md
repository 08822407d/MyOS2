---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-MM-BASELINE-04-REVIEW-001
record_type: bounded_memory_baseline_review
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
evidence_class: "远端文本、程序和记录的有限审阅与代表源码回核；无主线命令执行"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: de7c96ede546
results_frozen_short12: eb75b3100601
kernel_short12: a039d9803ade
status: BOUNDED_BASELINE_RECEIVED_DOCUMENTATION_CORRECTIONS_QUEUED
acceptance_verdict: RETURN
returned_items: [MR-01, MR-02]
returned_scope: "仅两处报告总括/条件表述；由下一任务U00追加校正，不退回内核、不要求重做内存盘点。"
accepted_partial_scope: "四个子系统21个代表能力、39条依赖的静态资料形态和已读范围；M00原IR-01/IR-02的指定矛盾已处理；新疑点仍为静态待核。"
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_acceptance_verdict: NOT_ISSUED
prior_IR_01_disposition: CLOSED_AS_SPEC_CORRECTION_NOT_RUNTIME
prior_IR_02_disposition: CLOSED_EXPLICIT_KEEPS_WITH_MR_01_NEW_QUALIFIER
reviewer_execution: false
reviewer_hash_or_parser_run: false
executor_model: unknown_or_not_attestable
kernel_change_authorized: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_decisions_requested_now: []
findings_policy: agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
next_followup: CORE-USER-VFS-BASELINE-05
next_taskbook: agent-workspace/lead/MYOS2-LEAD-002/19-user-vfs-baseline-contract.md
phase3_entered: false
branch_release: false
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-INTEGRATION-03-review.md
inputs_read:
  - "de7c96ede546:mm-baseline-04/MANIFEST.md、integration-03-disposition.md、memory-baseline.md、evidence.md分段读至末尾，长响应缺段另补。"
  - "同目录scripts/mm_facts_check.py、scripts/mm_scope.py全文；records/facts_check.json、records/checker_probes.json全文。"
  - "同目录facts-and-dependencies.yaml的1–145、2820–2935区段；未通读4269行。其能力与边、疑点全表另见主报告，非全YAML语义逐条认证。"
  - "time:options_flags.cmake全文；mm_init.c的1–55、385–430；slub.c的95–155；mm/fault/fault.c的1200–1385；page-flags.h的445–510；buddy.c的550–655。"
  - "time:vm_map/filemap.c的1–145、280–430；init/main.c的235–430，含完整kernel_init与kernel_init_freeable。"
  - "本会话17号任务与INTEGRATION-03回执；PR17元数据；ee9e至de7差异；c217至主线、a039至time比较；执行头combined statuses。"
self_check:
  scope: this_file_only
  verified_claims: 9
  quotes_reconfirmed: 9
  downgraded_to_inferred: 0
  method: "对照本轮所读文本并人工式计数；非执行面机械复核。"
local_validation: "待执行面；主线未运行脚本、解析器、哈希、编译或测试。本次不要求重复运行旧检查。"
open_questions:
  - "MR-01/02为AI文档质量事项，不能登记成Owner需要现在修的代码bug。"
  - "实际用户/文件路径、文件缺页读回调下的完成条件及资源生命周期待下一静态任务核查；真实运行层仍未测。"
---

# 内存代表基线已取得，问题收存不阻塞下一条静态主链

**当前不要求Owner处理代码问题或选择方案。** 按18号新约定，问题默认记录入库、留给未来新专项。已交内存正文可在本回执限定下使用；整包验收仅因MR-01/02两处表述暂保留RETURN。下一云任务开头追加短小处置，随后直接完成用户程序与文件访问基线，不为文档修正单独停一轮。

## 1. 本次实际收到了什么

[VRF] 相对ee9e6a738224，de7c96ede546仅增加20件文件，均在mm-baseline-04，无旧文件净修改。四份Markdown正文有实质内容，另有结构化数据、两个检查脚本、检索/核对/回读记录，不是仅MANIFEST。未根据PR顶部摘要直接判通过。

四个子系统为mm.page_alloc、mm.kmalloc、mm.vm_map、mm.fault；能力分布5/5/6/5，总21；依赖39条。16个connected_body和5个partial_body是实现证据状态，不能折算成正确性分数或运行通过。正文把调用、初始化、数据、构建等边区分，include未被充作调用；未覆盖旧节点单列，未假装完成原002R/003R的全量工作。

records/facts_check.json与实际检查程序相互对应：80条引文、12个Markdown标签、43个查询、数量与引用核对、M00值表算术均有记录。71条in_body、6条in_body_comment和其余宏/类型/构建引文不能合成“80条已证明机制正确”。p0对照与p1–p10错误副本的返回记录已读；副本原件未交，主线未重跑。evidence前部“9个探针”是第一阶段说明，后文明确追加p10，以实际完整记录为10个负面探针，不另开检查器返工。

执行包中的“主线回源”指其云端编排者的自查过程，不等于本LEAD-002已逐条复核所有80条或20项疑点。多代理过程不替代原始来源。

## 2. M00：原两项规格纠正可以关闭

IR-01：六步表由上一步返回任务成为下一步current，idle键0分支为N2=50、N1=40、idle区间10；最大键分支60/40/0。计费加idle区间才覆盖100。表、原规则、算术检查记录相符；A/B仍只是预期，没有新运行。

IR-02：B-K1…B-K10已经明确列出，排除了普通唤醒头插，原“保持A全部行为”矛盾已处理。沉默不授权、显式唤醒返回值读取者、DV-19立即返回/忙等分支、DV-11源码与机器执行的区别也得到登记。下述MR-01是新总括句的漏前提，不重新打开整个调度设计。

## 3. 两项局部文档校正（AI内部完成）

### MR-01：键下限不使所有唤醒任务到队首

位置：integration-03-disposition.md §4“设下限”段，先写max(自身键,队首非idle键)，随后总括“它们插在队首之前，因此仍会先运行一次”。该结论漏掉了自身键原本更大的分支。

[INFERRED，规则算术，未运行] 队首10、待入队35，max(35,10)=35，在B的有序插入规则下不会排到10前。自身键5时才变成10，依并列规则排在原10前。新任务原0键在有非idle队首时可归入后一种；无非idle队首时另按原规则处理。

所需处置：追加精确的分支说明，将“仍先运行一次”限定为钳制后与队首相等且没有后续插入/其他选择条件改变的声明状态。保持B、OD-2未采用，不改原文件、不向Owner提问、不运行C。

### MR-02：局部初始化与词法查询不足以给全系统不可达结论

位置：memory-baseline.md §0、MC-01/RF-01、LB-08及YAML对应总括；它们写“永不合并”“本pin不可达”，但同包GAP-M08仍保留page_type写入者全树检索未做。已取得的__init_single_page注释与PageBuddy按位判定支持局部疑点，不足以排除其他初始化、别名、共享存储及运行中的字段演变。

所需处置：保留原引文、局部根因候选和风险，不反向宣布合并正常；将“PageBuddy恒假、所有释放永不合并、另一缺陷全局不可达”收窄为明确初值/已检查写入路径前提下的推导，未闭合的部分继续needs_evidence。RF-01只能否定初稿已充分证明全局重复分配，不能凭有限检索证明全局不可达。

同一处置中把“先碎片化再耗尽”“出错任务大概率只占自己的时间片”“用户内存只增不减”等跨路径时序/概率总括改为所读路径中的风险与断点，不增加运行结论。可以引用已有不足而收窄，不要求为了闭合这些绝对句另做全树研究。

MR-01/02不影响已核的正文存在、局部接口接合与代表能力盘点；但后继不能直接采用有问题的原句。这是报告质量修正，与暂缓内核修复是两件事。

## 4. 同轮继续推进：确定下一块可查的接合面

主线本轮重新读到：内存启动入口先mem_init后kmem_cache_init；slab实际调用alloc_pages；文件mmap设置vm_ops；文件缺页通过f_op->read读入页。kernel_init实际先调用myos_switch_to_root_disk，再调用kjmp_to_doexecve。这把下一步收敛成“用户程序与文件访问怎样用上内存”，不需要现在修调度或启动QEMU。

这里没有证明具体read回调、文件系统后端、首个用户程序路径或完整启动可达性。它们恰是下一任务应以实际定义和对象赋值追踪的内容，不能用典型Linux知识填空。

### 本轮九条最小源码锚点

K01 配置文本，不代表运行正确。
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
```

K02 内存初始化的直接顺序。
[VERIFIED mykernel/mm/misc/mm_init.c::mm_core_init]
```c
	mem_init();
	// mem_init_print_info();
	kmem_cache_init();
```

K03 slab取页及局部失败返回。
[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
	if (!folio)
		return NULL;
```

K04 文件mmap安装回调表。
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap]
```c
	vma->vm_ops = &generic_file_vm_ops;
```

K05 文件缺页的间接读端点；具体实现待追踪。
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_fault]
```c
	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
```

K06 根文件系统切换调用。
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	myos_switch_to_root_disk();
```

K07 进入程序装载相关入口；本轮未读其定义。
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	kjmp_to_doexecve();
```

K08 PageBuddy的实际判定。
[VERIFIED mykernel/mm/page_alloc/page-flags.h::PageBuddy]
```c
			return ( (page->page_type & (PAGE_TYPE_BASE | PG_buddy)) == PAGE_TYPE_BASE );
```

K09 局部初始化中这一行是注释，不等于全树没有别的写入。
[VERIFIED mykernel/mm/misc/mm_init.c::__init_single_page]
```c
	// page_mapcount_reset(page);
```

## 5. 不再重复扩大验证器任务

mm_facts_check.py全文已读。其范围检查、逐类引文、双向边引用、计数与M00算术均有对应程序；词法器仍不是预处理器、链接器或形式化证明器。负检索只覆盖实际词和目录，不覆盖所有间接调用。M00检查不求队列语义，队列仍由源码/值表推导而非程序实跑。

本轮未做任意输入的鲁棒性认证，不以这些有限检查承诺脚本长期通用可靠；也不为了未发现的通用缺口再建一套验证框架。scope脚本把master固定为旧头，未来若合并推进要按新任务的原件比对处理，不能盲目重跑旧绑定并要求Owner撤回合并。

## 6. 后续及Owner注意力边界

按18号约定，20项MC、6项DR、9项LB及其他缺口进入可追溯记录；不自动改码，不以发现数量向Owner列问卷。下一任务CORE-USER-VFS-BASELINE-05：U00处理上述两处文字；U01–U04连续补用户启动/创建/exec/exit与文件打开/读/映射/关闭的静态链，建立带ID的延期问题增量，维持原未运行层。

PR17继续Draft，MR-01/02由下一批新增文档回应；不必先合并16/17。旧IR修正和W/V有限接收不撤销，整项内存正确性没有验收。分支保留，主线不合并、不删除。文件读取、写后核对和PR状态在本轮后续连接器中确认。

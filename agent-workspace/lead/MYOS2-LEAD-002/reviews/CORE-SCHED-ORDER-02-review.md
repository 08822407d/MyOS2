---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-SCHED-ORDER-02-REVIEW-001
record_type: scheduler_witness_review_and_source_followup
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "workspace=master；kernel=time；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
evidence_class: "远端夹具、记录及来源的可读复核；另有明确标注的源码推导，主线未运行命令"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-ORDER-02
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: f36b89b8a53a
results_frozen_short12: 5078686e8267
previous_execution_short12: 0d62c4d19711
kernel_short12: a039d9803ade
status: BOUNDED_WITNESS_REVIEW_COMPLETE
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: "15号合同的W01-W08宿主原函数见证与三项结果控制；不是调度器或内核正确性验收"
remaining_return_items_in_contract15: []
kernel_acceptance_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_hash_or_parser_run: false
executor_model: unknown_or_not_attestable
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-recheck-02-review.md
pr17_merge_recommendation: READY_AS_SCOPED_RECORDS_AT_REVIEWED_HEAD
branch_release: false
next_followup: CORE-SCHED-INTEGRATION-03
next_taskbook: agent-workspace/lead/MYOS2-LEAD-002/16-scheduler-integration-contract.md
phase3_entered: false
inputs_read:
  - "15号任务书全文；长响应末段另读至末尾。"
  - "f36b89b8a53a:scheduler-order-02/MANIFEST.md、evidence.md全文；截断处补读。"
  - "同目录fixtures：fx_order.c、evaluate_order.py、run_order.py、controls4.py、make_results4.py、guard4.py、frozen_0d62.py、common4.py、final_check4.py全文。"
  - "同目录observations：controls.json全文；runs.json抽取清单后部、编译记录、W01/W02/W07两次原始输出及相邻W03/W06记录；fx_order.expanded.c类型、宏和完整pick_next_task_myos区段。"
  - "同目录results.yaml末段的W08结束、游标解释、控制和全部summary。未通读全部1712行。"
  - "time:mykernel/scripts/options_flags.cmake、mykernel/sched/scheduler/myos_rt.c全文；panic.c开头至85行；double_list.h的list_header接口区段。"
  - "time:scheduler_core.c的1-225、240-820、1030-1245、1330-1643区段；包含set_task_cpu、try_to_wake_up、__sched_fork、sched_fork、wake_up_new_task、init_idle等完整定义。不是全树扫描。"
  - "PR16/17元数据；0d62至f36、5078至f36的远端比较；time基线比较；执行头combined statuses和PR触发workflow查询。"
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "lead目录写权来自本轨道工作令和Owner连续推进授权，不修改公共公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c_panic: "\tthis_cpu = smp_processor_id();"
branch_check_scope: "本轮读取time两条原文且远端比较time与a039d9803ade identical；不申报重新执行time/master机械金丝雀检查。"
self_check:
  scope: this_file_only
  verified_claims: 7
  quotes_reconfirmed: 7
  downgraded_to_inferred: 0
  method: "本轮逐条对照已读源码并人工式计数；机械核对待执行面，不并入原47条分母。"
local_validation: "待执行面；本主线未编译、运行、解析或计算哈希。无需为本次接收重跑同一批宿主实验。"
open_questions:
  - "全局队列政策尚未采用：唤醒头插优先与始终按vruntime排序不能混成同一已实现契约。"
  - "本轮新增的唤醒头插状态见证是源码推导，不是新增宿主运行或全局可达性证明。"
  - "真实ELF、IRQ、SMP、上下文切换、CA-02全局可达性继续开放。"
---

# 八组见证可以接收；下一步形成能实施的调度改进边界

**本批按15号合同收口，结论为 PASS_PENDING_LOCAL，不要求重新修通用验证器或重跑W01-W08。** 被审PR17头f36b89b8a53a可以作为有限实验与历史修订记录合入；不表示内核已经修好。合并不是后续工作的前置，分支保留要求不解除。

这次真正得到的是两个相互独立的回插失序见证，并观察到了失序影响下一次选择。主线随后已继续检查入队与初始化源码：只改选择器不能自动建立全局排序保证。下一项收敛到调度生命周期的局部事实表和最小修复规格，不再扩展单函数见证或验证平台。

## 1. 回收结论

[VRF] 下表“运行”指执行者交付了原函数夹具、命令、原始输出与终态；不是主线再次运行，也不是独立CI认证。

| 核查面 | 已检查到什么 | 裁定范围 |
|---|---|---|
| 原函数保真 | 展开文件中的完整pick_next_task_myos与本轮time原定义逐段对照一致；模板在调用前后观察，不插内部探针。抽取清单列出来源、字节及分段摘要。 | 接受目标函数的来源与有限夹具边界；未由主线逐字节重算全部抽取项。 |
| 八组输入 | 模板明确设定任务、状态、时钟、队列及idle；last_jiffies只初始化一次。W07/W08的current替换明确是序列模型。 | 没有用删除idle或伪造空anchor作为这八组的初态。 |
| 两次输出 | run_order对每个场景启动两个独立进程并保存完整stdout/stderr/退出及终态；所抽查W01/W02/W07两份原始输出与正文相符，不只依赖repeat布尔值。 | 两次记录与程序相互支持；未对全部runs.json逐字节重新计算。 |
| 独立判据 | evaluate_order用值列表计算计费、成员及候选排序，不复制原函数指针遍历；排序、计费、选择与结构分栏。 | 适用于这八组声明输入，不作为任意损坏输入的通用认证器。 |
| 三项控制 | controls代码与controls.json分别支持完整W03有效、缺after快照无排序判定、完整重排样本有效但反驳预测。 | 三项合同控制接收；变造样本不是实际内核输出。 |
| 冻结与范围 | 远端比较0d62至f36仅22件新增且全在scheduler-order-02；5078至f36只新增六件文档/回读材料，results未变。 | 保留旧pilot/core/consumer全部原件，不把新增实验继承成旧验收。 |

编译记录为指定GCC及-O0等参数下退出0、stderr为空。该参数集还关闭了几类unused警告，因此不表述为“所有编译诊断均无问题”，也不证明Release、优化或实际内核构建通过。

## 2. 哪些内核认识得到推进

**W01：比较对象与遍历游标不一致。** 记录给出C20、D30、idle0、A25；既破坏非idle按键排序，也把idle留在普通任务前。这一结果与time中tmp_list更新而tmp_rt不更新相符。

**W02：先回插后计费，独立造成失序。** A先用15定位在C20前，之后变成25。首次比较即不满足，故没有进入游标循环；只改循环内部不能改变这个状态序列。

**W07：不仅是列表看起来不整齐。** 第二次选择拿走队首A25，C20仍在队列中。此处已观察到排序与后续选择的联系，但不据此宣称真实系统已出现某种饥饿概率或延迟数值。

W03零增量和W04相等键为正常对照；W05阻塞任务不回队但累计已用时间；W06 idle不计费；W07没有重复计费；W08把未切出期间累计的5留到后一次选择时计入。这些正常行为应在后续改进中保留，不能只围绕两个反例改码。

**三处消费解释须固定：**

- `all_selection_matches_oracle=true`只说明遵守本实验的“取队首”行为模型；不说明选到了最小vruntime或证明公平调度正确。W07已经同时记录selected_key_is_queue_minimum=false。
- `structure_ok`覆盖所列前向遍历、count、重复节点、idle出现次数及返回任务不在队列等检查；不是所有双向链接、并发访问或生命周期性质的证明。
- 原函数返回之后、模型切换之前，夹具中的current仍指向刚回插的旧任务；此时current_occurrences=1不违反“调用前current未入队”的输入前提。不能把这个过渡状态误判为重复入队。

新增的“只改游标可能先解引用anchor容器”仍是源码推导；本次没有执行任何修改后的函数。内核修复代码、全局可达性、真实时钟/IRQ/SMP/上下文切换均未获得新验收。

## 3. 同轮继续核查：排序是队列所有写入者共同承担的约束

以下G01-G07是本轮直接读取的源码事实；后面的影响判断另标推导。它们不扩充刚完成实验的声称覆盖。

G01 配置面已读：

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_BUG \
```

G02 set_task_cpu把目标变成可运行，在目标队列尚无该节点时头插：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::set_task_cpu]
```c
	rq_s *target_rq = &(per_cpu(runqueues, new_cpu));
	p->__state = TASK_RUNNING;
	if (!list_header_contains(&target_rq->myos.running_lhdr, &p->rt.run_list))
		list_header_add_to_head(&target_rq->myos.running_lhdr, &p->rt.run_list);
```

G03 首次唤醒也使用头插：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);
```

G04 初始化新调度实体把vruntime置零：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	p->se.vruntime			= 0;
```

G05 idle初始化将其设为当前任务并初始化空运行链表：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::init_idle]
```c
	rq->curr = idle;
	INIT_LIST_HEADER_S(&rq->myos.running_lhdr);
```

G06 头插原语修改链接并增加计数：

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

G07 选择器的计费位于回插之后：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

[INFERRED] 假设合法目标队列原为B10/C20/idle，另有未入队的睡眠任务X35，调用G02对应的入队动作会得到X35/B10/C20/idle。即使选择器局部修好，按现有入队规则仍不能推出“队列始终按vruntime排列”。这只是在明确状态前提下按函数推导，不是主线实跑，也不是从开机可达性的证明。

**不能直接把头插判成新的bug。** 它可能承担唤醒/新任务优先的政策；目前的源码事实只说明它与“全局始终升序”不是同一个契约。相反，G04的新任务零键在非负且已排序的普通队列中，头插本来可能符合顺序；不能把每次头插都当成失序。idle初始化与回插也有阶段差异，不能把“idle通常在尾部”当成从初始化到回调返回每一瞬间都相同的形态。

由此，下一步应一次理清初始化、首次入队、睡眠后再入队、运行后回插和选取五类边界，并给出修改哪些入口、保留什么语义的具体规格。不是只给pick函数补两行后宣告调度完成，也不是强行重写成Linux调度器。

## 4. 下一项的收敛方式

任务见同目录 `16-scheduler-integration-contract.md`。它完成字段/写入者的有界扫描、活动调用证据、计费责任和候选修复影响面；**不再编译运行W/M/N，不改内核、不出补丁、不搭框架**。已有见证直接引用，不重新包装成新的运行。

输出要能回答：哪些是已核事实；哪些是政策选择；最小兼容修复能保证什么；统一排序会影响哪些调用者；已有回归该保留哪些、真正新缺口是什么。建议方案由AI先给，重要行为改变再显式提交Owner；不能把内部扫描和文档安排变成Owner问卷。

PR17可转Ready保存当前被审头；若继续下一项则按新任务转回Draft复用，已经合并时按先验证旧记录已入master再开唯一新Draft的规则处理。两条分支均保留，主线不合并、不删分支。

## 5. 覆盖与未做

本轮全文读取两份交付正文、八个Python文件及C模板；build_evidence4.py未全文读。runs.json、展开C及results.yaml按inputs_read列明区段读取，不宣称主线机械审完全部22件、重跑16次或重算摘要。执行头combined statuses和PR触发workflow查询为空，不作为CI通过。执行者模型未知，Owner合并不等于人工全文审查。

同轮源码继续核查是局部，不是完成002R/003R全量矩阵，也没有进行阶段3路线综合。旧core核验、两次consumer补修的既有收口保持有效；此次接收不恢复撤回的无调用者断言，不升级真实内核运行层。

来源入口：
- https://github.com/08822407d/MyOS2/blob/f36b89b8a53a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/MANIFEST.md
- https://github.com/08822407d/MyOS2/blob/f36b89b8a53a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/observations/runs.json
- https://github.com/08822407d/MyOS2/blob/f36b89b8a53a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/fixtures/evaluate_order.py
- https://github.com/08822407d/MyOS2/blob/time/mykernel/sched/scheduler/scheduler_core.c
- https://github.com/08822407d/MyOS2/blob/time/mykernel/sched/scheduler/myos_rt.c

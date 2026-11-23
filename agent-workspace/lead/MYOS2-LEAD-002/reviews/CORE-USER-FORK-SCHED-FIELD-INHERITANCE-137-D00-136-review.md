---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话标识）"
model_per_owner: "Pro（主线独立验收）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "0b6cffc26e2d"
execution_pin_short12: "4ae5e3ea1af0"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-new-task136-reviewed-fork-fields137.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md","mykernel/lib/list/double_list.h","mykernel/sched/scheduler/myos_rt.c","mykernel/scripts/options_flags.cmake","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/debug/panic.c"]
status: "RETURN_PARTIAL_DOCUMENTATION_CLOSURE"
closed: ["R136-01","R136-02","R136-03"]
open: ["R136-04"]
combined_136: "RETURN"
combined_135: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":1,"quotes_reconfirmed":1,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 136 批四项补正的独立裁定

冻结作者件 95b6af84d8f6、136 退回回执 #7096722711、189 号修订约定与增量 4ae5e3ea1af0 分开读取。136 和 137 的作者均为 C09a／CHAT-EXEC-21。三项达到原关闭条件；R136-04 留 OPEN，136 原件及组合均为 RETURN。135 组合继续 PASS_PENDING_LOCAL。[136 原裁定](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md) · [189 号原验收条件](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md#L49) · [136 主线回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7096722711) · [137 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7097528986)

| 原编号 | 裁定 | 可消费的结果 |
|---|---|---|
| R136-01 | CLOSED_DOCUMENTATION_ONLY | 当次补录的启动句、金丝雀、任务范围和作者身份均可追溯 |
| R136-02 | CLOSED_DOCUMENTATION_ONLY | 旧 38 窗按原始 ID 逐一映射，旧 014/022/023 和配置定位处置成立 |
| R136-03 | CLOSED_DOCUMENTATION_ONLY | 四组合法输入、24 项谓词、16 次写入及 B 未给定的重排标志已补全 |
| R136-04 | OPEN_PARTIAL | C1/C2 的数值与三时点状态已补齐，纠偏选择器仍引到不存在的类型 |

## R136-01 · 启动与接收

旧 MANIFEST 未含的 startup_selfcheck_quote、branch_canary_quotes 在 137 新清单明确存在。C09 直接重读 master 公约 §1.2 全句，匹配第 6 行；两个金丝雀在 time 的 options_flags.cmake 第 41 行和 panic.c 第 79 行出现，在直接 master 均不出现对应原句。另读 188/00 与 137 的任务指导说明，task_local_only=true 有来源。135 冻结原作者 C08c/EXEC-20 与 136/137 作者 C09a/EXEC-21 分列。旧接收与刷新记录保持历史身份，当前补正日期明确。[136 原清单](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md#L129) · [137 自检](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md#L14) · [本地任务适用范围](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md#L89) · [188 接收约定](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md#L79)

## R136-02 · 38 个冻结窗的修订

将 136 原清单与 137 facts.D00 的 38 个旧 ID 对齐。原编号、路径、符号、起行、标题和原文 38/38 相符。35 个按活动窗对应，旧 014 的活动初始化重锚 E137-014、注释移至 L137-COMMENT-14；旧 022/023 分别移至 L137-COMMENT-22/23。HAVE_ARCH_BUG_ON 内门位于 bug.h:65，CONFIG_BUG 外门位于 :24，活动宏在 :66；配置选项的固定证据是 E137-073。列表头的条件在 double_list.h:138，定义入口在 double_list.c:1–2。这里只据源码判条件，不认证实际预处理结果。子帧是新 E011，两状态宏是 E018/E019，四指针写是 E025，回插与返回、摘后自连已有相应新编号。原批要求的勘误被核收。137 关于自身 73 个新窗的覆盖声明另立 R137-02。[冻结证据](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md#L155) · [旧窗映射](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L772) · [条件定位](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L762) · [机器证据组](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L465) · [BUG_ON 定义条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bug.h#L65)

## R136-03 · B 的合法插入与输入范围

对四次插入按源码顺序重放六个无效谓词：prev 空，next 空，next.prev 与 prev 不同，prev.next 与 next 不同，new 等于 prev，new 等于 next。24 项均求值且均假，while 不进入，validator 返回真，外层 if(!valid) 为假。四组顺序写指针之后才增加 count。

| new/prev/next | 四写 | 正向（含 H） | 计数 |
|---|---|---|---|
| K/H/Q | Q.prev=K，K.next=Q，K.prev=H，H.next=K | H K Q R H | 2→3 |
| J/H/K | K.prev=J，J.next=K，J.prev=H，H.next=J | H J K Q R H | 3→4 |
| J/H/Q | Q.prev=J，J.next=Q，J.prev=H，H.next=J | H J Q R H | 2→3 |
| K/H/J | J.prev=K，K.next=J，K.prev=H，H.next=K | H K J Q R H | 3→4 |

四组反向链均为对应正向链的逆序，尾均为 R。旧 B.base 与四个后态的 need_resched 均从“false”修订为初值未给定、wake 片段未写、沿用输入。136 的 C 案例单独给定 false。[旧 B 五处字段](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1032) · [新四组证明](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L505) · [校验体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L162)

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

## R136-04 · 数值核收，图选择器保留退回

两例初态都是 H Q R H、count=2、K=TASK_NEW 自连，P 键 3、last=100、rq.curr=P。唤醒后是 H K Q R H、count=3、K=TASK_RUNNING。103 样本 3≥10 假，最后一个 OR 操作数“状态不为 RUNNING”仍求值且假，整个 OR 假才跳过 count。返回 P，队列、P 键、last、rq.curr 维持，K 仍 RUNNING。110 样本 10≥10 真、跳过状态项、读 count，取 K，自连；3>20 假、右侧 anchor 项未求值，回插 P 后键 13、last=110、返回 K 且 rq.curr=K，队列 P Q R。[两样本的三个时点](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L893) · [四项 OR 与 count](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L14)

| 实物 | ID、类型与标签 |
|---|---|
| 旧冻结图 | C136_103；condition；used3<time_slice10 OR false |
| 137 的 D00.new_graph_node | D137_R13604_C1_GE_TEST；PREDICATE_GE_FALSE；used_jiffies=3 >= rt.time_slice=10 => false；并列出 FALSE_SKIP_COUNT_AND_PICKER_BODY |
| 实际 137 facts.graph 与 DOT 节点 | D137_R13604_C1_GE_TEST；condition；3 >= 10 false (not 3<10) |
| 实际 137 图第一条边 | D137_R13604_C1_GE_TEST → D137_R13604_C1_SKIP，kind=FALSE_3_GE_10，label 为空串 |
| 实际 137 图第二条边 | D137_R13604_C1_SKIP → D137_R13604_C1_OUT，kind=SHORT_CIRCUIT_COUNT，label 为空串 |

D00 声称的边类型在实际图中零命中。接续时在 138 新件列出冻结实物的真实节点和两条边，再列修订投影的实际节点、端点、kind、label；在比较为假和整个 OR 为假之间明确标出状态项已求值。两份图均从实物核匹配。不得把旧摘要里假设的别名作为旧图选择器。无需重复重做已核收的 136 数值与其余三个历史问题。[136 图节点](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot#L108) · [137 D00 自述](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L888) · [137 实际节点](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1880) · [137 实际边](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L2341) · [137 DOT 两条边](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot#L101)

阶段 2。完整本地 P9、预处理、编译链接、Graphviz、QEMU、实机与真实操作均为 NOT_RUN。

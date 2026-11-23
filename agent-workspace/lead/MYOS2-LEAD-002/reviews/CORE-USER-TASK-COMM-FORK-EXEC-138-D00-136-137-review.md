---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_historical_correction_review"
followup_id: "CORE-USER-TASK-COMM-FORK-EXEC-138"
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
lead_before_publication_short12: "4777e07269dd"
execution_pin_short12: "38a5184142d7"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-D00-136-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-fork-fields137-reviewed-task-comm138.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/190-user-task-comm-fork-exec-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/deferred-findings.yaml","mykernel/sched/task/task_const.h","mykernel/sched/task/task_types.h","mykernel/sched/forkexec/fork.c","mykernel/sched/syscall.c","mykernel/lib/string/string.c","mykernel/lib/string/string_helpers.c","mykernel/sched/forkexec/exec.c","mykernel/sched/task/task.h","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/lib/list/double_list.h","mykernel/sched/task/pid.c","mykernel/lock_IPC/signal/signal.h","mykernel/arch/x86_64/sched/context/process_arch.c","mykernel/sched/scheduler/scheduler.h","mykernel/lib/list/rcu_list_macro.h","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/scripts/options_flags.cmake","mykernel/include/linux/lib/string.h","mykernel/debug/panic.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/lib/string/strnlen_user.c","mykernel/include/uapi/linux/resource.h","mykernel/include/uapi/linux/limits.h","mykernel/include/uapi/linux/auxvec.h","mykernel/fs/vfs/binfmt_elf.c","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ARG-PAGES-24-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-STACK-23-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md"]
status: "CLOSED_DOCUMENTATION_ONLY"
closed: ["R136-04","R137-01","R137-02"]
combined_136: "PASS_PENDING_LOCAL"
combined_137: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 136 与 137 的增量补正裁定

按 190 号 D00 独立检查冻结的 137 六件、137 原裁定与 138 的增量记录。原记录保留原判 RETURN。合并此前已关闭的 R136-01/02/03 和本次三项文档关闭后，136、137 两批的组合状态均为 PASS_PENDING_LOCAL。138 本批四项新发现单独登记。[190 号关闭条件](https://github.com/08822407d/MyOS2/blob/4777e07269dd/agent-workspace/lead/MYOS2-LEAD-002/190-user-task-comm-fork-exec-contract.md#L50) · [前次历史裁定](https://github.com/08822407d/MyOS2/blob/4777e07269dd/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-D00-136-review.md) · [137 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7097528986) · [138 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7099213925) · [138 独立裁定](CORE-USER-TASK-COMM-FORK-EXEC-138-review.md)

| 编号 | 原作者／增补者 | 文档裁定 | 原要求核对结果 |
|---|---|---|---|
| R136-04 | C09a／CHAT-EXEC-21 | CLOSED_DOCUMENTATION_ONLY | 已还原错误选择器、真实节点和两条边；新五节点链与两组三时点吻合 |
| R137-01 | 同上 | CLOSED_DOCUMENTATION_ONLY | 已区分调用、采样、时钟写点、数值是否变化和调用者 flags 的两次写入 |
| R137-02 | 同上 | CLOSED_DOCUMENTATION_ONLY | 旧 73 窗、原机器件 52 项、21 个缺项、分拆活动行、样本绑定和回执时点均可定位 |

## R136-04 · 跟踪实际谓词与目标节点

原自述对 D137_R13604_C1_GE_TEST 的 kind 记 PREDICATE_GE_FALSE，false 边记 FALSE_SKIP_COUNT_AND_PICKER_BODY。旧 facts.graph 和 DOT 中该节点实际为 condition，label 原文为 `3 >= 10 false (not 3<10)`。从 GE_TEST 到 D137_R13604_C1_SKIP 的 kind 是 FALSE_3_GE_10，从 SKIP 到 D137_R13604_C1_OUT 是 SHORT_CIRCUIT_COUNT，两条 label 均为空串。自述中的旧类型均未在该选择器处匹配。[原自述](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L888) · [原节点](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot#L10) · [原边](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot#L101)

138 新图从 D138_GE 经 STATE、OR、SKIP 到 P，四条实际有向边均能在机器图按端点及类型定位。3≥10 为假，随后 P 状态操作数确实求值且为假，整个 OR 为假，右侧 count 未求值，返回 P。两组三时点机器对象与冻结 137 的 D00 样本逐字段相等。C1 唤醒后队列 K,Q,R，选择后仍为 K,Q,R，K 保持已链入、last=100、P.vruntime=3。C2 唤醒后同队列，随后 K 摘出并自连，队列 P,Q,R，P.vruntime=13、last=110、rq.curr 和返回值均为 K。[实际新边](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec-paths.dot#L61) · [冻结快照副本](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L83) · [选择条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L15)

## R137-01 · 静态分开两种时钟

固定 rq.clock=1000 的三个独立试算分别得到：

| local_clock | 差值 | 写 rq.clock | 末值 | 数值变化 |
|---:|---:|---|---:|---|
| 999 | −1 | 函数早返 | 1000 | 否 |
| 1000 | 0 | 执行加零 | 1000 | 否 |
| 1001 | +1 | 执行加一 | 1001 | 是 |

在所给路径，调用者先左移 flags，调用 update_rq_clock，随后写 RQCF_UPDATED。原 137 的采样值未给，不能用三个对照样本反填。原 picker 仍单独使用 106−100=6；K 的片为 4，选择 Q，J 的片为 9，继续 J。旧 facts 与作者回执的无条件时钟结论已按 190 号要求收窄。138 新字段将调用后已知的 flags 一并记 UNKNOWN，另开 R138-04。[旧结论](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1843) · [三例](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L216) · [两处写点及新字段](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L251) · [采样与条件写](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L24) · [最终 flags](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_const.h#L48)

## R137-02 · 旧覆盖差额与通用函数的样本身份

主线逐条将 73 个旧 ID 的标题、path、symbol、起止、引文正文与固定源及新表核对，再从整个原 facts 独立取唯一 ID 得 52 项。以下 21 个尾号与新表标出的差项完全相同。实际新键为 `D00["R137-02"].all_73_old_windows`。它是旧窗覆盖表；新 47 个证据编号另在同级 `new_47_window_index`。正文和证据引用了另一个拼写，归本批 R138-03。[冻结 137 证据](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L19) · [真实 73 项表](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L300) · [本批索引](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L1266)

`038、039、041、042、043、044、045、046、047、049、050、051、055、058、059、065、066、067、068、069、070`

E137-048 的两个成员已分取 scheduler_types.h 第 43、46 行；E137-049 已收窄为 runqueue_types.h 第 195–196 行，并保留活动行内文字。旧 E028、E031、E035 分别还原为 136 C2 摘 K、插 P 于 Q 前，137 C1 摘 Q、插 K 于 R 前，137 C2 跳过三步。E033 是选中对象赋值及 last 写，E034 的实际 return 另列。原作者回执中的“提交前一致”按分支快进后才可观察的证据时点勘误。闭合依据是可定位的原实物及已达标的增量。[行与样本绑定](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L1692) · [E044 与 E045](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md#L495) · [时点说明](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L1736)

阶段 2。文档关闭不授予任何运行或内核写入权限，完整本地 P9 及全部运行项均 NOT_RUN。

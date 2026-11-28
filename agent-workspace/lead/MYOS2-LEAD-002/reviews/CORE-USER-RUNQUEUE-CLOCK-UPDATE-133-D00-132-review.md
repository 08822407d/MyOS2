---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-RUNQUEUE-CLOCK-UPDATE-133"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（Owner 指定的主线独审档）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "f73f55546245"
execution_pin_short12: "5e979654dc92"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/185-user-runqueue-clock-update-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler.h","mykernel/sched/scheduler/myos_rt.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
status: "CLOSED_DOCUMENTATION_ONLY"
R132_01: "CLOSED_DOCUMENTATION_ONLY"
R132_02: "CLOSED_DOCUMENTATION_ONLY"
R132_03: "CLOSED_DOCUMENTATION_ONLY"
combined_132: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 133 批 D00 · 第 132 批三项补正的独立裁定

**R132-01、R132-02、R132-03 均为 CLOSED_DOCUMENTATION_ONLY。** 132 固定原件与 133 的有效差量合读为 PASS_PENDING_LOCAL。本裁定给出实际新图关系的定位；133 对新旧 ID 的叙述和可机读映射仍有自己的 R133-01。旧件及原 RETURN 均保留。 [132 原审](CORE-USER-YIELD-TASK-SELECTION-132-review.md) · [133 主审](CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-review.md) · [185 合同](../185-user-runqueue-clock-update-contract.md)

## R132-01 · 四组轨迹逐项重放

按案例、旧序号、源位置定位 132 的 70 个事件。133 四组共 78 条、新序号在各案例内从 1 连续递增。每例增补一个第 1078 行的 prev_state 读取；原先合并的 1116 行各拆成一次函数调用发起、一次接收返回。源码调用者先在 1034 保存 prev，再经过 1065–1067 的时钟片段，1069 绑定计数地址，1078 读状态，1116 才进入 picker。清标志在接收 next 之后，prev!=next 门在这两次清标志之后。[先绑定](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1069) · [调用及清标志](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1116) · [修订轨迹](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L26)

| 样本 | 旧→新事件数 | 绑定计数地址的旧→新号 | 1116 旧号 → 新发起 / 新接收 | count>0 的实际求值 | 选中及三计数 |
|---|---:|---|---|---|---|
| B1 | 13→15 | 7→2 | 8→4 / 10 | 是，假 | P；8/5/4 |
| B2 | 13→15 | 7→2 | 8→4 / 10 | 否；输入 count 为 2 | P；8/5/4 |
| B3 | 22→24 | 13→2 | 14→4 / 16 | 是，真 | Q；9/6/4 |
| B4 | 22→24 | 13→2 | 14→4 / 16 | 是，真 | Q；9/6/4 |

四组的十个原输出字段逐键比较，40/40 相同。运行队列、选中指针、P 的计费及 last_jiffies 可复现。B1/B2 保留在 P；在正常回到循环底、无新置标的条件下不再迭代。B3/B4 的一次选择不能确定哪段执行上下文何时在循环底重测哪一个标志。deferred.POST_LOOP_CONTEXT 对未知条件的记录符合这一结论。[循环底](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L281) · [实际样本图](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L19) · [延后项](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/deferred-findings.yaml)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	switch_count = &prev->nivcsw;
```

四个 132 冻结节点 S132_B1_loop 至 S132_B4_loop 均为有效的旧选择器。133 实际节点逐例为 D133_B1_loop、D133_B2_loop、D133_B3_loop、D133_B4_loop；均在 facts.graph 和 DOT 各出现一次。以此实测映射复核即可读得正确结论。133 正文称“相同 ID”，需要随 134 更正；不得把旧名在新图中零命中记为一次原名命中。

## R132-02 · 类表项和间接调用者

132 原图 S132_class_member → S132_picker 唯一一条 INDIRECT_CALL_INSTANCE，在原 facts 与 DOT 中均能命中。固定 scheduler.h:264 由 __pick_next_task 通过 class->pick_next_task(rq, prev) 发起间接调用；myos_rt.c:67 的表项只提供函数目标。133 实际图用三步表达这一区别。下表的节点均带 D133_CLASS_ 前缀。[旧图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L375) · [新图](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L188) · [真实调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L264)

| 起点→终点 | 关系 | 对应源码含义 |
|---|---|---|
| member→callee | STATIC_INITIALIZER_TARGET_ONLY | myos_rt 的静态表项指定 pick_next_task_myos。 |
| owner→call | INDIRECT_CALL_INSTANCE | 调用拥有者 __pick_next_task 在类遍历中执行间接调用。 |
| call→callee | TARGET_RESOLUTION_IF_MYOS_RT | 当遍历到给定类，现场使用该目标。 |

另有 member→call 的指针值边、argument→call 的 rq/prev 数据边。每次非空立即返回；一次 NULL 继续下一类，只有穷尽才到末尾 BUG 语句。此前 yield_task 与此选择现场分别有独立拥有者。133 的 D00.correct 使用旧节点拼写描述原图上的修正，实际投影在上表；二者都能由固定文本核清。其未显式区分两套命名属于 133 新记录的问题。[虚拟差量](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1343) · [耗尽出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L276)

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
```

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
	.pick_next_task		= pick_next_task_myos,
```

## R132-03 · 五簇与七簇的成员范围

用 130 冻结 DOT 和 131、132 的冻结 facts/DOT，按 CD_LOST、LS_DOT、CAT_NOTE、RMDIR_TMPD、RM_LOG 五个语义案例选择节点及类型边。旧图采用相应 ERR 簇，新图使用 DIAG 簇。

| 被选集合 | 节点 / 边 | 重算方法 |
|---|---:|---|
| 130 五簇 | 40 / 35 | 各 8 / 7 |
| 131 五簇 | 70 / 80 | 各 14 / 16 |
| 131 五簇＋A_INIT＋C_YIELD | 91 / 99 | 在 70 / 80 上加 7 / 6 和 14 / 13 |
| 132 补后五簇 | 85 / 115 | 各 17 / 23 |

133 新清单列出了 130 固定图并给出来源，旧执行者的内部阅读状态标为 UNKNOWN。132 中混用五簇与七簇的统计问题至此闭合。 [130 固定图](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot#L192) · [131 固定图](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L186) · [132 固定图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L36)

文档关闭的依据限上述源码、给定样本与历史投影；130/131 的既有组合裁定沿用。完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机及真实时钟、文件、进程、调度操作均为 NOT_RUN。

---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（主线独审档）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "3251b8f68697"
execution_pin_short12: "f0d46c448367"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/186-user-process-id-namespace-view-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-D00-132-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
status: "PARTIAL_RETURN"
R133_01: "RETURN"
R133_02: "CLOSED_DOCUMENTATION_ONLY"
R133_03: "CLOSED_DOCUMENTATION_ONLY"
combined_133: "RETURN"
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 134 批 D00 · 第 133 批三项补正裁定

| 项目 | 本次裁定 | 剩余范围 |
|---|---|---|
| R133-01 | RETURN（部分核收） | 四组循环节点、旧静态差量和五条新边均核收。仅余一条虚拟间接调用的来源选择器至两个实际新边的对应。 |
| R133-02 | CLOSED_DOCUMENTATION_ONLY | 中立判断、两条分支和三个样本的字段写入均已修对。134 自己的替代图命名另列 R134-01。 |
| R133-03 | CLOSED_DOCUMENTATION_ONLY | 三个宏各有自身单行定义，冻结账、替代视图和 134 新账分清。 |

**133 原件与本次补正合读仍为 RETURN。** 132 组合已为 PASS_PENDING_LOCAL。134 新问题及 135 追加范围见[本批主审](CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md)与[187 号合同](../187-user-fork-return-identities-contract.md)。[PR17 #7094240400](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094240400)

## R133-01 · 冻结图、旧拼写差量与实际新图

逐项读 132、133 的 facts.graph、DOT 和 133 的 D00.correct，再与 134 列的物理行和完整 ID 对照。下表每对位置按 facts / DOT 排列，四组旧图各 1/1、新图各 1/1，16 个行号均命中。旧名在 133 的两个实际图中均为 0。[132 旧图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L404) · [133 实际图](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L19) · [134 对照表](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md#L35)

| 样本 | 132 冻结节点及位置 | 133 实际节点及位置 |
|---|---|---|
| B1 | S132_B1_loop，2595 / 404 | D133_B1_loop，2253 / 19 |
| B2 | S132_B2_loop，2650 / 428 | D133_B2_loop，2338 / 54 |
| B3 | S132_B3_loop，2705 / 452 | D133_B3_loop，2468 / 98 |
| B4 | S132_B4_loop，2760 / 476 | D133_B4_loop，2598 / 151 |

132 冻结错边 S132_class_member→S132_picker / INDIRECT_CALL_INSTANCE 唯一；133 虚拟静态边沿用这两个旧名，类型改为 STATIC_INITIALIZER_TARGET_ONLY。实际 133 两载体中以下五条三元组各唯一，指针值及 rq/prev 数据保留。[旧调用边](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L375) · [两条虚拟差量](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1342) · [新五边](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L188)

| 133 真实起点 → 终点 | 关系 |
|---|---|
| D133_CLASS_member → D133_CLASS_callee | STATIC_INITIALIZER_TARGET_ONLY |
| D133_CLASS_owner → D133_CLASS_call | INDIRECT_CALL_INSTANCE |
| D133_CLASS_call → D133_CLASS_callee | TARGET_RESOLUTION_IF_MYOS_RT |
| D133_CLASS_member → D133_CLASS_call | FUNCTION_POINTER_VALUE |
| D133_CLASS_argument → D133_CLASS_call | RQ_AND_PREV_DATA |

余项可精确复现。133 的 correct.indirect_call 给出 **S132_class_iter → S132_picker / INDIRECT_CALL_INSTANCE**，owner 为 __pick_next_task。134 六件全文对 S132_class_iter 零命中。补正须把这一旧虚拟选择器显式对应到上表 owner→call 的调用及 call→callee 的条件解析，并分别列两个实际提供方。C09 在这里定位了对应关系，作者仍须把源端写入可重放的追加件。[唯一旧虚拟源](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1348) · [此次映射所列源端](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L205)

## R133-02 · 实际分支与字段写入

133 的已早返节点仍有 CONDITION_NONNEGATIVE 指向加赋值。134 的实际图改为 PID134_CLOCK_guard 分别连 early、add；两路都归入 caller 的 flags 覆盖。三个样本给定终值与 133 的 clock.control、source_write_owners 对得上。C1/C2 经过加赋值，C3 在加赋值前返回。此历史要求已经满足。134 facts.D00.replacement_graph 和实际图的不同命名及汇合边类型，由本批 R134-01 订正。[冻结旧边](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L231) · [新图四条实际边](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L38) · [分例写点](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L350) · [调用者三句](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1065)

| 样本 | __schedule 左移 flags | update_rq_clock 写 clock | __schedule 覆盖 flags | clock 终值 |
|---|---|---|---|---:|
| C1 | 是 | 是，+12 | 是，4 | 1012 |
| C2 | 是 | 是，+12 | 是，4 | 1012 |
| C3 | 是 | 否，−4 早返 | 是，4 | 1000 |

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

## R133-03 · 逐宏证据和计数

冻结 E133-015 的标题仅标 RQCF_REQ_SKIP，原窗跨越三个宏。134 的 E134-001、002、003 与固定 time 第 46、47、48 行逐字吻合，分别证明值 1、2、4。旧六件实测 35 裸／35 正式／0 降级与 7 定位。将一窗替换为三窗的完整旧账才为 37。134 自己冻结 47 裸／47 正式／0 降级与 16 定位，其中这三条各计一次。134 的 E134-021 边界单列 R134-04。[三条独立短窗](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md#L18) · [宏定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_const.h#L46)

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_REQ_SKIP]
```c
	#define RQCF_REQ_SKIP		0x01
```

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_ACT_SKIP]
```c
	#define RQCF_ACT_SKIP		0x02
```

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_UPDATED]
```c
	#define RQCF_UPDATED		0x04
```

完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机及真实 PID、命名空间、进程、调度、时钟、文件操作均为 NOT_RUN。

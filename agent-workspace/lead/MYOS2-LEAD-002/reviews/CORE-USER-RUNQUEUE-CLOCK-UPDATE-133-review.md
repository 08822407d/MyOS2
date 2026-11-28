---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/185-user-runqueue-clock-update-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-D00-130-131-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/include/linux/sched/clock.h","mykernel/arch/x86_64/kernel/tsc.c","mykernel/arch/x86_64/include/asm/timer.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
status: "RETURN"
open_returns: ["R133-01","R133-02","R133-03"]
combined_132: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 133 批独立验收 · 运行队列时钟

**C09 裁定 RETURN。** 合同内的时钟和选任务样本全部核收。两类输入控制不同字段；负纳秒差可以让时钟更新函数早返，调用者仍能进入 picker 并选出 Q。文档退回限三项：新旧图 ID 的对应、负差控制流和写入字段、三个活动宏的独立引文。第 132 批的三个实质补正另已裁定文档关闭。[185 合同](../185-user-runqueue-clock-update-contract.md) · [D00 裁定](CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-D00-132-review.md) · [186 合同](../186-user-process-id-namespace-view-contract.md) · [PR17 #7093437139](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7093437139)

## 固定交付与范围

原 C08c／CHAT-EXEC-20 的 5e979654dc92 唯一父为 1021b4b7740c。GitHub compare 领先 1、落后 0，六个纯新增文件，共 5,181 行、177,567 UTF-8 字节。固定提交与活动 exec 分别读取全文，独立按 Git Blob 头与 UTF-8 正文重算 SHA-1，六项逐件一致。正式作者评论按 ID 两读，再与无 since 的 100/100/55/0 页比对，255 个不同评论 ID 中该正式标记唯一。[执行提交比较](https://github.com/08822407d/MyOS2/compare/1021b4b7740c...5e979654dc92) · [PR17 #7093437139](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7093437139)

| 文件 | 新增行 | UTF-8 字节 | Blob 前 12 位 |
|---|---:|---:|---|
| MANIFEST.md | 115 | 11,499 | f89f871e6ce1 |
| runqueue-clock-update.md | 89 | 12,013 | 58a078f96e91 |
| facts-and-boundaries.yaml | 4075 | 115,117 | 7da6cc1e4698 |
| runqueue-clock-update-paths.dot | 342 | 21,198 | dd6e20c9d579 |
| deferred-findings.yaml | 121 | 5,252 | 8f5f3f606157 |
| evidence.md | 439 | 12,488 | a536ca7e8ea1 |

两份 YAML 可按 JSON 解析。DOT 160 个节点、151 条关系的端点、类型、分组、标签与机器件一致，重复及缺端点均为 0。清单中 27 条输入路径实际存在，其中主线 6、直接 master 公约 1、历史执行件 10、固定 time 10。两项 time 金丝雀与直接 master 能区分，公约启动句匹配。六件未检出 40 位十六进制 SHA、P9-11 四个禁语或内部引用句柄。本轮的源代码与图检查均限文本。

## A · 时钟的生产、采样和写入

__schedule 把 clock_update_flags 左移一位，调用 update_rq_clock，再直接赋 RQCF_UPDATED。三个标志值为 1、2、4。固定 update_rq_clock 函数体每次调用一次 local_clock；包装函数返回 native_sched_clock 的值。给定 __use_tsc 为真，则取计数并用 cycles_2_ns 换算；否则以 jiffies_64 与 INITIAL_JIFFIES 的差乘整数商 1000000000/HZ。实际分支与读数未知。rq.clock 为 u64，减法在无符号域进行，再赋给 s64 delta；仅依合同给定的低 64 位二补码转换解释负差。clock_task 和 clock_pelt 在所读片段无写点。[调用者](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1065) · [被调函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L20) · [时钟字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/runqueue/runqueue_types.h#L263) · [时钟两分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L221)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

## B · 连续采样与独立对照

每个值均自给定前态重算。flags 四栏按进入、左移后、被调函数返回后、调用者覆盖后的顺序。每行都取样一次，clock_task/clock_pelt 保持 700/600。[时钟报告](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md#L62) · [实际判断与写点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L24)

| 样本 | 进入 clock / 采样（ns） | flags 四态 | 无符号差 → s64 | 到达源码 += | clock 终值（ns） |
|---|---|---|---|---|---:|
| B1 | 1000 / 1012 | 1→2→2→4 | 12 → +12 | 是 | 1012 |
| B2 | 1012 / 1012 | 4→8→8→4 | 0 → 0 | 是，+=0 | 1012 |
| B3 | 1012 / 1008 | 4→8→8→4 | 18446744073709551612 → −4 | 否，负差早返 | 1012 |
| B0，独立对照 | 1000 / 1012 | 0→0→0→4 | 12 → +12 | 是 | 1012 |

相同后值对应两条不同的路径。B2 经过加赋值，B3 在它前面返回；两例外层都执行 flags=4。B0 与 B1 的起始标志不同，二者都采样并增加 12 ns。实际编译后是否发出 +=0 对应的存储指令仍 UNKNOWN。

## C · 三个独立对照

每例重置 clock=1000、flags=1、last_jiffies=100，使用 185 给定的 P、Q、R 与计数初态。时钟片段每例都在 picker 之前返回。七个给定时钟调用各比对 16 个数值/条件/写点字段；C 三组各另比对 14 个选择和计数字段，合计 154 项匹配。该计数只覆盖本段检验的字段；下节另列有冲突的冗余字段。[picker 的 tick 取值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L12) · [计费与 last_jiffies](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L49) · [条件计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1124)

| 样本 | ns / tick | s64 差 / clock后态 / flags后态 | used / next / 终队列 | P 键 / last | nr_switches / P.nivcsw / P.nvcsw |
|---|---|---|---|---|---|
| C1 | 1012 / 103 | +12 / 1012 / 4 | 3 / P / Q,R | 10 / 100 | 8 / 5 / 4 |
| C2 | 1012 / 110 | +12 / 1012 / 4 | 10 / Q / P,R | 20 / 110 | 9 / 6 / 4 |
| C3 | 996 / 110 | −4 / 1000 / 4 | 10 / Q / P,R | 20 / 110 | 9 / 6 / 4 |

C1 的选择门短路；C2/C3 恰到时间片，两例均摘 Q、按 P 的旧键将 P 插到 R 前，队列计数 2→1→2。C1/C2 隔离 tick，C2/C3 隔离纳秒。132 留下的 COUNTER_UNKNOWN_CLOCK，在上述给定前态和源码语句范围内可记局部数值已重建。物理时钟的关联、真实切换、恢复时刻及公平性均未知。

## 三项定点退回

| 项 | 冻结证据、矛盾及修订标准 |
|---|---|
| R133-01 · 旧选择器与新图端点 | 正文第 35 行称四个 S132_B*_loop 以同一 ID 更正。四条 loop_bottom.id 使用旧名，实际两个新载体各只声明 D133_B1_loop 至 D133_B4_loop。类表修正的 correct.static_edge / indirect_call 也以 S132 拼写表达，实际图为 D133_CLASS_member→callee 的静态边，以及 owner→call 的间接调用、call→callee 的条件目标解析。注明旧 ID 是旧图选择器或虚拟差量，逐项给出实际新 ID 和边，支持分解关系的重放。[原文](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md#L35) · [correct 字段](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1343) · [新类图](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L188) |
| R133-02 · 控制流与写入 | A_CLOCK 的 D133_A_negative 标签已包含“delta<0 early return”，该节点却以 CONDITION_NONNEGATIVE 连至 D133_A_add。应先判断，再由真正的负支返回、另一支到 +=，两条都回调用者写 flags。C.counters.first_clock_write 三项均填同一个含两个函数名的字符串；C3 中 clock.control 和 source_write_owners 正确记为未执行 +=，汇总却仍称有时钟写。分别列 flags 两个写点的拥有者、rq.clock 唯一条件写点及各例是否到达。[负差图](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L217) · [C3 冗余字段](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L2157) |
| R133-03 · 强制宏引文 | E133-015 只标 RQCF_REQ_SKIP，连续引用 scheduler_const.h:46–48。三行分别是三个已在行末结束的宏定义，第二、三行在所标宏定义外。三个数值正确。按各自真实符号给三个单行正式窗。冻结六件的裸/正式数是 35/35，42 个物理窗口全文匹配，另 7 个定位不计入 P6；只在完整替代视图中为 35−1＋3＝37。[E133-015](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/evidence.md#L154) · [三个独立定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_const.h#L46) |

其余正式短窗按符号归属通过限定复核。固定 CMake 选项作为配置层依据，实际执行效果需独立证据。完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机及真实时钟、文件、进程、调度操作均为 NOT_RUN。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

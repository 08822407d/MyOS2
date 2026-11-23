---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
followup_id: "CORE-USER-YIELD-TASK-SELECTION-132"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（沿用 Owner 对主线独审的选择）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "f07e60a5e8c0"
execution_pin_short12: "1021b4b7740c"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/184-user-yield-task-selection-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler.h","mykernel/sched/scheduler/scheduler_macro.h","mykernel/sched/scheduler/scheduler_const.h","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/scheduler/myos_rt.c","mykernel/lib/list/double_list.h","myinitramfs/myinit/initd.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/myshell/myshell.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","mykernel/sched/sched_api.h","mykernel/sched/scheduler/scheduler_api.h","mykernel/arch/x86_64/sched/context/switch_to_arch.c","mykernel/arch/x86_64/sched/context/switch_to_arch.h","mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h","mykernel/arch/x86_64/sched/context/process64_arch.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
status: "RETURN"
open_returns: ["R132-01","R132-02","R132-03"]
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 132 批 · 一次 yield 后选中谁

**C09 独立裁定 RETURN，退回范围是文档。** 四个样本的 40 个最终字段全部核收。B1、B2 选 P，B3、B4 选 Q，后两例才进入切换分支。机器事件表的顺序和循环重测对象、类表间接调用的归属，以及旧图计数的范围仍需补正。三个历史退回项均已文档闭合。 [184 号合同](../184-user-yield-task-selection-contract.md) · [历史三项裁定](CORE-USER-YIELD-TASK-SELECTION-132-D00-130-131-review.md) · [185 号合同](../185-user-runqueue-clock-update-contract.md) · [PR17 #7091027479](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7091027479)

## 六件原件与证据质量

作者为原 C08c／CHAT-EXEC-20。执行提交 1021b4b7740c 的唯一父为 2089bfff6f4a。比较领先 1、落后 0，共一个提交、六件纯新增。按固定提交与活动原 exec 分别取全文，逐件对照内容、UTF-8 字节及重算的 Git Blob，结果 6/6 相同。共 5,910 行、208,467 字节。给定作者评论已按 ID 两读，并与无 since 全分页正文核对；发布前 100/100/53/0 共 253 条，作者完整正式标记唯一。[作者原件比较](https://github.com/08822407d/MyOS2/compare/2089bfff6f4a...1021b4b7740c) · [PR17 #7091027479](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7091027479)

| 文件 | 新增行 | UTF-8 字节 | Git Blob 前 12 位 |
|---|---:|---:|---|
| MANIFEST.md | 140 | 12,382 | 22ad28d802e5 |
| yield-task-selection.md | 104 | 14,131 | 28bb143f1f89 |
| facts-and-boundaries.yaml | 4,402 | 126,656 | cc511e7e64d6 |
| yield-task-selection-paths.dot | 488 | 31,056 | 22beb0ccd9ab |
| deferred-findings.yaml | 115 | 5,248 | e47a542f5135 |
| evidence.md | 661 | 18,994 | 5ef6589dfd1f |

作者回执的 evidence.md 一行写了十位前缀 5ef6589dfd，十二位为 **5ef6589dfd1f**。二者指向上表同一份回读内容。两份 YAML 可按 JSON 解析。206 个图节点及 239 条关系的端点、类型、节点标签、边说明与图数据相合，缺端点 0。

正式短窗 58、另行定位 7，来自 15 个固定 time 文件；65/65 对应 1–5 个连续物理行，逐字含 CR 和制表符。各窗均依归属核过，强制调用、宏、表项和配置支撑具备。本批的六件裸前缀共 58，均在 evidence；P6 为 58/58/0。清单 45 条路径存在（lead 17、exec 10、time 18）。两条金丝雀在 time 命中、在直接 master 不命中，公约整句与 master 一致。六件中的长十六进制 SHA、P9-11 四个禁语、外部标记及内部引文句柄均为零。以上为连接器和源码文本所能核验的部分。[引文账](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md) · [作者清单](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/MANIFEST.md)

## A · 调用关系与 R132-02

固定 sys_sched_yield 直接调用 schedule。P 为 TASK_RUNNING 时跳过 sched_submit_work，随后调用 __schedule_loop(SM_NONE)。do/while 至少进入一次，依次出现禁抢占、__schedule、恢复抢占及底部重测。__schedule 先定位 rq、保存 prev，再经 pick_next_task 宏别名进入 __pick_next_task。遍历到给定 myos_rt 类时，后者从成员读取函数指针并调用，首个非空结果返回。系统调用早先使用的 yield_task 与此处遍历类时使用的 pick_next_task 是两个不同成员；真实链接顺序和实际选用类未知。[schedule](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1209) · [循环与遍历](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L258)

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

**R132-02 OPEN。** A.source_chain 把 myos_rt_sched_class 写进 caller 栏。图中 S132_class_member → S132_picker 的 INDIRECT_CALL_INSTANCE 把静态表项画成了调用发起者。表项提供目标，间接调用应从 __pick_next_task 的具体调用现场发出，rq、prev 是该现场传来的实参。保留类遍历、首个非空返回和宏别名；空返回时仍有后继类可遍历，仅在遍历耗尽时到达末尾 BUG 位置。[机器调用链](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L678) · [选任务图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L374) · [类表初始化器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L67)

## B/C · 四次独立的纸面演算

共同输入为单 CPU、有效队列、P 普通且可运行、P 旧键 10、R 键 30、时间片 10、last_jiffies=100，无并发改写。下表的 8/5/4 依次是 rq.nr_switches、P.nivcsw、P.nvcsw 的初态。对每例从源码独立重建全部十个 output 字段，再与机器件按键逐项核对。[picker](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L9) · [摘首与计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L652) · [切换门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1116)

| 输入 | used | OR 四项实际求值（need/idle/到期/非运行） | 外层 count>0 | next；队列/count | P 键/last | 三计数 |
|---|---:|---|---|---|---|---|
| B1 空、need=true、j=103 | 3 | 真、跳过、跳过、跳过 | 读，假 | P；空/0 | 10/100 | 8/5/4 |
| B2 Q,R、need=false、j=103 | 3 | 假、假、假、假 | 短路跳过 | P；Q,R/2 | 10/100 | 8/5/4 |
| B3 Q,R、need=true、j=103 | 3 | 真、跳过、跳过、跳过 | 读，真 | Q；P,R/2 | 13/103 | 9/6/4 |
| B4 Q,R、need=false、j=110 | 10 | 假、假、真、跳过 | 读，真 | Q；P,R/2 | 20/110 | 9/6/4 |

B3/B4 的 helper 摘除 Q 并把 count 从 2 减到 1，P 用计费前的键 10 与 R 的 30 比较，比较已为假，while 右侧及循环体跳过。插在 R 之前后，picker 自己把 count 加回 2，再把 used 加到 P 的键，写 last 和 rq->curr 并返回 Q。四例 R 键都为 30。四例各清两个标志；只有 prev 与 next 不同的两例才计 nr_switches、发布 rq->curr、计入 P.nivcsw 并调用 context_switch。P.nvcsw 的替代绑定只在注释内，四例仍为 4。真实切换与恢复时间未知。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	switch_count = &prev->nivcsw;
```

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	next = pick_next_task(rq, prev, &rf);
picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
```

**R132-01 OPEN。** 四份有序事件都把上述第 1069 行移到了 picker 返回后。B1/B2 在 seq 7，B3/B4 在 seq 13。实际顺序为保存 prev（1034）、绑定计数地址（1069）、读 prev_state（1078）、发起 pick（1116）、执行 picker 写值/返回、在 1116 接收 next，然后 1118–1119 清标志。绑定与随后自增是不同事件。B2 的静态队列确有两项，但 OR 已为假，外层 count 比较没有实际求值。其 gate 应分别登记已知输入和未发生的读。[B1 事件](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L855) · [B2 门](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L953) · [B3 事件](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L1255) · [B4 事件](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L1507)

C.cases 与四个样本图的重测标签统一提到 Q。B1/B2 仍在 P，若正常回到循环底且没有新置标，重测不要求下一轮。B3/B4 的选中指针 Q 及一次调用，不能确定哪一个调用栈会在何时到达循环底，届时的标志也未给出。deferred 的 POST_LOOP_Q_NEED 把“给一个 Q 标志”列为恢复依据，仍欠此次重测的执行上下文。185 要求同步修订 B、C、样本图、延后项与叙述，重算原 40 字段确认数值保留。[C 四例](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L1648) · [样本图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L404) · [循环重测](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L281)

## R132-03 · 诊断图的计数范围

D00.R130_02.old_file 声明选取 131 的五个诊断簇，其 old_graph_baseline 却写 91 节点/99 边。五簇各 14/16，共 **70/80**；加上 A_INIT 的 7/6 及 C_YIELD 的 14/13 才是 **91/99**。二者均可重算，须明确字段统计的集合。旧 130 图在本批 inputs_read 中未单列，本轮 C09 直接读取该固定图与 131 两份投影完成独立复核；作者过去的内部读取状态不能由清单的缺项判断。185 仅需追加带来源的两组范围、计数和固定读件记录。历史 R130-02 的调用及返回修正已生效。[基线字段](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L28) · [D00 复审](CORE-USER-YIELD-TASK-SELECTION-132-D00-130-131-review.md)

本次范围限定在所读源码。固定配置体的架构与包装器选项见下窗。完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机与真实进程、调度、文件操作均为 NOT_RUN。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

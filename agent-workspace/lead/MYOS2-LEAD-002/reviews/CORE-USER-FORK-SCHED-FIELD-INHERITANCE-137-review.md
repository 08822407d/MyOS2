---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
followup_id: "CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-new-task136-reviewed-fork-fields137.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md","mykernel/sched/task/pid.c","mykernel/sched/forkexec/fork.c","mykernel/lock_IPC/signal/signal.h","mykernel/arch/x86_64/sched/context/process_arch.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/lib/list/double_list.h","mykernel/sched/task/task_const.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/task/task_types.h","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/sched/scheduler/scheduler.h","mykernel/lib/list/rcu_list_macro.h","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/scripts/options_flags.cmake","mykernel/lib/list/double_list.c","mykernel/debug/panic.c"]
status: "RETURN"
new_findings: ["R137-01","R137-02"]
historical_remaining: ["R136-04"]
author_comment_id: 7097528986
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 137 批独立验收 · RETURN

四项历史补正中，R136-01、02、03 文档关闭，R136-04 的数值和三时点状态核收，图选择器仍需补正。第 136 批组合状态保持 RETURN。第 137 批的 A/B/C 字段与两个选任务结果成立，本批另外退回 R137-01、R137-02。三个待办并入 190 号合同。[189 号合同](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md) · [四项历史裁定](CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-D00-136-review.md) · [137 作者回执 #7097528986](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7097528986)

## 六件与远端来源

作者 C09a／CHAT-EXEC-21 在原执行分支提交 4ae5e3ea1af0。唯一父为 95b6af84d8f6，ahead 1、behind 0，仅六新增。主线逐份从固定提交及活动分支取全文，比较 UTF-8 字节并重算带类型和长度头的 Git Blob。

| 文件 | 行数 | 字节 | Git Blob 前十二位 |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md) | 189 | 10178 | dac0800ff288 |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/deferred-findings.yaml) | 119 | 4975 | 38c37ce7dba1 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md) | 813 | 28776 | f05cb04e58d3 |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml) | 2900 | 76550 | 72170834357b |
| [fork-sched-field-inheritance-paths.dot](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot) | 193 | 15182 | e11c2ccca42e |
| [fork-sched-field-inheritance.md](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md) | 102 | 10307 | d81928c1eb62 |
| 合计 | 4,316 | 145,968 | 6/6 |

作者回执把固定与活动的一致性写成“提交前”。被核的执行分支在单父快进后才含此六件。主线确认的是提交后的固定／活动一致性，旧评论保持原文。Owner 转交的聊天报告发评后 G04 已通过。主线另按 ID 两读 #7097528986，一致；发布前 PR17 无 since 分页为 100/100/63/0，263 个不同 ID，完整作者标记唯一。两 PR 及四条直接 refs 已核。[固定执行提交](https://github.com/08822407d/MyOS2/commit/4ae5e3ea1af0) · [执行清单](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md#L29)

## A/B · 字段从哪里来

按 A 的 11 个字段重读：普通成功 fork 在独立子任务存储上执行 *tsk = *orig，嵌入标量取副本。P 的 rt.run_list 自连时，两个指针成员在复制瞬间仍指向 P 节点。copy_process 清零 utime/stime/gtime；__sched_fork 清零 vruntime、令子链自连；sched_fork 写 TASK_NEW、on_cpu=0；wake_up_new_task 写 TASK_RUNNING 并插头。该指定路径保留复制时的 rt.time_slice、nvcsw、nivcsw。全部系统写者仍未核定。[结构复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L288) · [CPU 时间](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1026) · [节点初始化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L599) · [入队](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L721) · [字段表](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1039)

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```

| B 的时点 | P/K/J 切片 | K 与 J 的链节点 | 待选队列 |
|---|---|---|---|
| B0 父初态 | 4／—／— | 两子未创建 | Q R |
| B1、B2、B3 | 4／4／— | K 从指向 P 到自连，再入头 | Q R → K Q R |
| B4、B5、B6 | 9／4／未创建后为 9 | K 仍在队列；J 复制后指向 P，再自连 | K Q R |
| B7、B8 | 9 后为 12／4／9 | J.prev=H、J.next=K；K.prev=J、K.next=Q | J K Q R |

按 B0–B8 独立重建 21 个存在的任务快照，每个 12 个字段，252/252 相符，另 6 个未创建的位置为 null。九次正反链、count 与 rq.curr 全部相符。两子最终各自的三项 CPU 时间与 vruntime 均为 0，nvcsw/nivcsw 均为 7/11，父的最后改值只影响父切片。[九阶段完整表](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1134)

## C · 相同 6 ticks 的两个独立样本

| 核查项 | current=K，片 4 | current=J，片 9 |
|---|---|---|
| 外门 | need_resched 与 idle 均假；6≥4 真，状态项跳过，count>0 真 | 前两项假，6≥9 假，状态项求值为假，count 跳过 |
| 队列 | 取 Q 后余 R，K 旧键 0>40 假，anchor 项跳过；把 K 插到 R 前，count 回到 2 | Q R，count=2 |
| 计费与当前任务写点 | K 键 0→6，last=106；picker 写 rq.curr=Q，返回 Q | J 键 0、last=100；picker 仍写 rq.curr=J，返回 J |
| 外层 __schedule | K≠Q，nr_switches 30→31，RCU 写 Q，K.nivcsw 11→12、nvcsw=7，抵达 context_switch 调用点 | J=J，跳过上述分支；nr_switches=30，J 的 nivcsw/nvcsw=11/7 |
  
B 的入队与 C 的 current 身份分别取自各自题设。源级抵达切换调用点以外，实际运行、任务恢复和用户返回均 UNKNOWN。[C1](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1698) · [C2](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1779) · [当前实体及选择门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L9) · [回插和计费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L37) · [切换计数和边界](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1124)

## 本批两个退回项

**R137-01 · 给时钟差留出早返分支。** E137-055 和报告第 79 行记录的是从 local_clock() 计算 delta；机器件 C.clk_vs_jiffies 与作者回执却断言 rq.clock 已更新。两个样本都未给 local_clock 的采样值及原 rq.clock。固定源码在有符号差为负时提前返回；差为零时到达加法写点、值不变；差为正时到达写点、值增加。三个结果都不改变题设用于 picker 的 106−100=6。190 新件须建立分支表，分列“调用、采样、写点、数值变化”；__schedule 自身在调用前后写 clock_update_flags 的语句另列。保持 K/J 原结果，冻结评论由勘误说明其适用范围。[原时钟字段](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1843) · [E137-055](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L569) · [更新函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L20) · [调用方](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1065)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

**R137-02 · 对齐引文清单、证据性质和样本对象。** 73/73 个正式窗在固定 time 的所列路径和物理行逐字命中，P6 的 73/73/0 算术成立。正文第 24 行说机器件列全编号，实际整份 facts 只出现 52 个不同 E137 编号。缺 038–039、041–047、049–051、055、058–059、065–070，共 21 个。evidence 中 73 个编号齐全。E048 与 E049 引到真实结构成员，同时夹有独立注释行；“全部为活动窗”应按实物细分。E028/E031/E035 的“K、P、Q”来自旧 136 的 C2。源码语句支持通用操作，具体操作数要绑定各自样本。137 的 C1 摘 Q、回插 K、插在 R 前，C2 跳过。E033 支持赋返回变量及 last，实际 return 在 E034。190 应给出旧 73 个 ID 的可核索引、逐项补实 21 个缺项或清楚限定原机器索引，重分类两窗；用通用语句与两份样本映射取代无条件专名。原 73 个物理命中保留。[完整索引声明](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md#L24) · [原机器证据组](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L465) · [混合结构窗](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L494) · [原对象标题](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L365) · [摘首](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L24) · [回插](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L44)

## 检查与使用边界

facts 与 DOT 的 93 个节点按 ID/kind/label 相符，96 条唯一边按起点、终点、kind、label 相符，无悬空端点。这个集合对照覆盖 facts.graph，自述纠偏索引的失配另见 R136-04。两个 JSON 形式的 YAML 可解析。P9 可达检查中，全部六件长 SHA、禁用运行断言及内部检索句柄零命中；两个金丝雀在固定 time 命中、在直接 master 不命中，启动原句在直接 master 公约第 6 行命中。源窗存在性与窗是否支持每个标题分别核算。固定 CMake 配置中有以下选项。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

完整本地 P9、预处理、编译链接、仓库脚本或程序、Graphviz、QEMU、实机及真实文件、进程、PID、调度、时钟操作均 NOT_RUN。

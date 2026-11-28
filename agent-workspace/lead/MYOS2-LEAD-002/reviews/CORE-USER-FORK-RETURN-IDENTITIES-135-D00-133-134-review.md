---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-FORK-RETURN-IDENTITIES-135"
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
lead_before_publication_short12: "676e4169f14c"
execution_pin_short12: "d01efe19ccec"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-pid-view134-reviewed-fork-returns135.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/first-run-frame.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/clone-group.md","mykernel/sched/task/pid.c","mykernel/sched/task/pid.h","mykernel/sched/syscall.c","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/sched/task/pid_types.h","mykernel/sched/task/pid_const.h","mykernel/lock_IPC/signal/signal_types.h"]
status: "CLOSED_DOCUMENTATION_ONLY"
R133_01: "CLOSED_DOCUMENTATION_ONLY"
R134_01: "CLOSED_DOCUMENTATION_ONLY"
R134_02: "CLOSED_DOCUMENTATION_ONLY"
R134_03: "CLOSED_DOCUMENTATION_ONLY"
R134_04: "CLOSED_DOCUMENTATION_ONLY"
combined_133: "PASS_PENDING_LOCAL"
combined_134: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 135 批 D00 · 一项 133 余项和四项 134 补正的独立裁定

**五项全部 CLOSED_DOCUMENTATION_ONLY。** 与已接受的补正合读，133、134 两批各为 PASS_PENDING_LOCAL。各批原件与当时的 RETURN 保留。135 新增字段中的矛盾另见[135 独审](CORE-USER-FORK-RETURN-IDENTITIES-135-review.md)。[187 合同](https://github.com/08822407d/MyOS2/blob/676e4169f14c/agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md) · [135 正式作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852)

## R133-01 · 补齐旧虚拟源

133 facts 的 correct.indirect_call 唯一给出 S132_class_iter → S132_picker / INDIRECT_CALL_INSTANCE，owner 为 __pick_next_task。该三元组在 133 实图 facts/DOT 各为 0。135 现在明确映射至 D133_CLASS_owner → D133_CLASS_call / INDIRECT_CALL_INSTANCE 与 D133_CLASS_call → D133_CLASS_callee / TARGET_RESOLUTION_IF_MYOS_RT。两条目标在冻结的两份实图分别各 1。调用现场、条件目标、静态表项、参数关系和先前核收的四个循环节点可以一起重放。[虚拟源](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1348) · [133 实图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot#L188) · [本批补正](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L24)

## R134-01 · 四个时钟节点和四条类型边

旧虚拟节点和边都只存在于替代说明中，在实际图中的计数为 0。下表四个目标在 134 facts.graph 与 DOT 各命中 1。[原虚拟图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L322) · [原实际图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L33) · [对应记录](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L58)

| 原虚拟节点 | 实际节点 |
|---|---|
| D134_CLOCK_GUARD | PID134_CLOCK_guard |
| D134_CLOCK_RETURN | PID134_CLOCK_early |
| D134_CLOCK_ADD | PID134_CLOCK_add |
| D134_CLOCK_OVERWRITE | PID134_CLOCK_caller |

判断的真、假边各到 early 和 add。两条回调用者的实际类型为 RETURN_TO_CALLER_THEN_FLAGS_OVERWRITE 和 NORMAL_RETURN_THEN_FLAGS_OVERWRITE，均归入 caller。四条完整实际三元组都在两载体各 1。与 C1/C2 写 clock、C3 跳过且三者都覆盖 flags 的历史裁定一致。

## R134-02 · 十个查询重放

先按冻结的目标、观察者与给定对象核对十次求值，再逐键比 135 的 preserved_twenty_fields 与 134 对应行，200/200 相同。报告十个简称到完整旧主键、新图前缀都有逐行对应。旧 target→observer 的 EVALUATION_ORDER 在十例的原始两载体各 1。新投影将实参、ns 条件、取槽、层级与指针门分开。缺省 ns 由 P.thread_pid=T，再由 T.numbers[T.level].ns 求得 N1；显式 ns 跳过这一赋值。原 134 顺序、字段归属、查询 ID 三项实质要求已经落实。135 为该过程新加的身份布尔值与边类型另列 R135-01。[十行别名对照](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md#L63) · [十例机器记录](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L171) · [源语句先后](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L473)

| 组 | 依次返回 | 判定依据 |
|---|---|---|
| C1 | 20 / 7 / 22 | 同一 N1 查询 G、GP、T。 |
| C2 | 5100 / 20 | 同一 G，显式查询 N0、N1。 |
| C3 | 0 / 4100 / 20 | 一次层级不允许访问数组，显式 N0 可取父号。 |
| C4 | 0 / 20 | 同层查询，只有 ns 指针相等才读 nr。 |

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

## R134-03 · 两条系统调用分派链

登记头在 syscall_64.c 第 11、21、30 行分别供声明、静态数组、switch 使用。对编号 39/110，switch 在相应条件下调用 __x64_sys_getpid/getppid，__x64 层将 regs 转给 __se 层，__se 层请求 __do 层。零参数映射、本地构建配置和直接目标均有固定源支持。__do 的定义头由宏末行产生，后随的源码花括号承载查询体。135 报告第 91 行把宏末行简称为“定义体”，在此校为“定义头”；作者的 E135-059 和实际层级已可直接辨明。实际预处理及链接 UNKNOWN。[分派补正](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L1075) · [两条实图](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L351) · [包装定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241) · [查询体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L45)

## R134-04 · 冻结窗与一对一替代

135 为冻结的 E134-001 至 047 逐 ID 列路径、符号、物理区间和窄断言。47 个索引逐一对应原件，复取固定 time 正文均吻合。E134-021 的旧两行含签名，现通过 E135-001 只引用 pid.h:125 体内返回。冻结 134 的 47 裸／47 正式／0 降级加 16 定位保留，一对一替代视图仍为 47。135 自己的 66 裸／66 正式／0 降级与 15 定位另计。[旧窗](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md#L209) · [断言索引](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L1194) · [替代窗](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md#L21)

[VERIFIED mykernel/sched/task/pid.h::task_pid]
```c
			return task->thread_pid;
```

完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机与真实进程、PID、调度、时钟、文件操作均为 NOT_RUN。

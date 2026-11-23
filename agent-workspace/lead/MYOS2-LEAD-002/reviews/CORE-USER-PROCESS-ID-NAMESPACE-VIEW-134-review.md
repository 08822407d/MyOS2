---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/186-user-process-id-namespace-view-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-D00-132-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-rq-clock133-reviewed-process-id134.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/185-user-runqueue-clock-update-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-id-allocation-lifecycle-90/MANIFEST.md","mykernel/sched/task/pid_namespace.h","myinitramfs/myinit/initd.c","mykernel/sched/syscall.c","mykernel/sched/task/pid.c","mykernel/sched/task/pid.h","mykernel/sched/task/pid_api.h","mykernel/sched/task/pid_types.h","mykernel/sched/task/pid_const.h","mykernel/sched/task/task_types.h","mykernel/lock_IPC/signal/signal_types.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/include/linux/kernel/syscalls.h"]
status: "RETURN"
open_returns: ["R134-01","R134-02","R134-03","R134-04"]
carry_over: ["R133-01"]
combined_133: "RETURN"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 134 批独立验收 · 公开进程编号的命名空间视图

**裁定 RETURN。** 十次给定查询的返回数和中间判断全部核收。四项退回限于交付文档的投影、查询步骤、分派调用边及引文。133 的两项补正文档关闭，余一条旧图来源映射。[正式作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094240400) · [186 合同](../186-user-process-id-namespace-view-contract.md) · [历史补正裁定](CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md) · [187 合同](../187-user-fork-return-identities-contract.md)

## 六件与远端基线

执行提交 f0d46c448367 单父 5e979654dc92。compare 领先 1、落后 0，限定目录六件纯新增、4,480 行、170,040 UTF-8 字节。固定提交与活动原 exec 各读全文，按实际 UTF-8 字节和 Git Blob 头重算 SHA-1，结果逐件一致。PR17 顶层评论在主线发布前无 since 分页 100/100/57/0，257 个不同 ID。作者标记唯一，按 ID 两读与分页正文相符。[PR17 #7094240400](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094240400) · [父子比较](https://github.com/08822407d/MyOS2/compare/5e979654dc92...f0d46c448367)

| 文件 | 新增行 | 字节 | Blob 前十二位 |
|---|---:|---:|---|
| MANIFEST.md | 142 | 13799 | bc6f065b170b |
| process-id-namespace-view.md | 130 | 15628 | f0d3265e492b |
| facts-and-boundaries.yaml | 3113 | 92801 | 9daef783ece5 |
| process-id-namespace-view-paths.dot | 353 | 25731 | 25ff3b0169af |
| deferred-findings.yaml | 117 | 5224 | d43855b0dc1f |
| evidence.md | 625 | 16857 | 66dfb0a00a20 |

清单的 42 条输入路径均已定位。直接 master 公约启动句、time/master 的 CMake 与 panic 两处金丝雀正反比对通过。两份 YAML 可按 JSON 读取。DOT 与 facts.graph 在 14 个分组里有 158 个唯一节点、164 条唯一类型边；方向、端点、类型、标签及分组逐项一致，未声明端点为 0。63 个源码窗口均与所列固定物理行逐字相符。六件未命中 40 位十六进制串、P9-11 四项禁语或内部检索句柄。源码引文的归属及图边含义另见下文。[清单](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md) · [固定图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot) · [证据账](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md)

## A 与 B · 调用点、目标、观察者

init 的 normal_boot 在 fork 返回 0 和非零两个分支各写一次 getpid，结果存分支局部 pid，所读段内无活动使用。对应的后继分别为 execve 和 sched_yield 循环。该文件对 getppid 的调用次数为 0。getpid 为 39，getppid 为 110。作者正确补充了 186 合同只列举两种用途时遗漏的第三种源码语境：同一登记头用于 extern 声明、表初始化及 switch 分派。分派的直接符号和查询体之间仍有封装层。[用户分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L30) · [三次纳入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L10) · [getpid 查询](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L45) · [getppid 查询](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L115)

getpid 查 current 的线程组编号，getppid 查 current->real_parent 的线程组编号。两者以 NULL 传入 ns，在 __task_pid_nr_ns 中先取 **current 的线程 PID 对象**来决定默认观察者。传入目标为父任务时，观察者仍属于调用者。随后才按目标和类型解析 PID 槽并调用 pid_nr_ns。PID 槽为 task->thread_pid，TGID 槽为 task->signal->pids[PIDTYPE_TGID]。活动 rcu_dereference 与相邻的注释读锁已分别核实；并发保护与实际返回状态均未知。[默认观察者宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid_api.h#L12) · [PID 槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.h#L125) · [命名空间成员](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.h#L84) · [类型选槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L315)

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

## C · 十次独立求值

按合同重新构造 P 的线程对象 T、组对象 G、父组对象 GP 或 GP0，以及显式或缺省 N0/N1/M1。每行独立重算 20 个字段：入口种类、current、目标、类型、槽来源、PID 对象、对象层级、所查 ns、ns 来源、对象非空、ns 层级、层级门是否求值及结果、实际下标、给定槽内容、指针门是否求值及结果、nr 读取、返回、零原因。200/200 一致。下表列完整机器主键，为复审提供跨载体对照。给定槽内的静态数值与语句是否实际读取该数值分别处理。[作者查询表](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md#L111) · [机器查询](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L800) · [层级门与指针门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L447)

| 查询主键 | 目标 / 对象 | 观察者 | 层级门 | 数组下标 | ns 指针比较 | 读 nr | 返回 |
|---|---|---|---|---:|---|---|---:|
| PID134_C1_GETPID | P / G | N1 缺省 | 通过 | 1 | 相等 | 是 | 20 |
| PID134_C1_GETPPID | RealParent / GP | N1 缺省 | 通过 | 1 | 相等 | 是 | 7 |
| PID134_C1_PIDVNR | P / T | N1 缺省 | 通过 | 1 | 相等 | 是 | 22 |
| PID134_C2_N0_TGID | P / G | N0 显式 | 通过 | 0 | 相等 | 是 | 5100 |
| PID134_C2_N1_TGID | P / G | N1 显式 | 通过 | 1 | 相等 | 是 | 20 |
| PID134_C3_GETPPID_PARENT0 | RealParent0 / GP0 | N1 缺省 | 未通过 | 跳过 | 跳过 | 否 | 0 |
| PID134_C3_N0_GP0 | RealParent0 / GP0 | N0 显式 | 通过 | 0 | 相等 | 是 | 4100 |
| PID134_C3_GETPID | P / G | N1 缺省 | 通过 | 1 | 相等 | 是 | 20 |
| PID134_C4_M1_G | P / G | M1 显式 | 通过 | 1 | 不同 | 否 | 0 |
| PID134_C4_N1_G | P / G | N1 显式 | 通过 | 1 | 相等 | 是 | 20 |

9 次达到下标访问，8 次读取 nr。C3-GETPPID 的观察者在第 1 层，GP0 最高第 0 层，返回初值 0。C4-M1 通过层级门并取 G[1]，其中的 N1 与所求 M1 指针不同，亦保留初值 0。真实 PID、实际 libc 桥接、命名空间创建及系统调用运行均不在此次数值证明内。

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
```

## 本批四项定点退回

| 退回项 | 固定证据和补正终点 |
|---|---|
| R134-01 · 时钟替代图的标识 | 报告第 60 行及 D00.R133_02.replacement_graph 使用 D134_CLOCK_GUARD、RETURN、ADD、OVERWRITE。四个完整 ID 和声明的四个完整边三元组在两份实际图中各 0 命中。实图是 PID134_CLOCK_guard、early、add、caller，两个汇合边的 kind 也不同。提交四节点、四边逐项映射或统一用实际基准 ID，说明每条汇合边真正的类型。历史早返和写点裁定有效。[替代字段](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L322) · [实际图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L33) |
| R134-02 · 查询顺序、字段拥有者及查询 ID | 十个 target 节点的标签已包括实际编号槽及目标 PID 对象，却都以 EVALUATION_ORDER 连向 observer。默认 ns 的决定在 pid.c:473–474，目标槽读取在 475。五个缺省 viewer_chain 又将 thread_pid 写在 task_pid 返回的对象上。应分开入口目标、观察者和后续取槽，对显式 ns 标实参数据；按 P→P.thread_pid=T→T.numbers[T.level].ns=N1 记录返回身份。报告表十行使用简称，逐行给出完整查询主键及其图中阶段节点。保留十组中间值及数值。[错误次序](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L201) · [缺省字段](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L813) · [表内别名](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md#L113) |
| R134-03 · switch 直接调用与最终查询体 | 两条 CONDITIONAL_DISPATCH_39/110 从 switch 连到具有最终查询内容的 body，未标合成路径。直接目标是 __x64_sys_getpid 或 __x64_sys_getppid。所读 x86 包装配置下，宏的源码形式从 __x64_sys_* 转给 __se_sys_*，后者调 __do_sys_*，由最后一层承载查询体。需明确直接调用、封装层和条件到达；extern 和表初始化另列。[两条分派边](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L109) · [switch 宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26) · [架构入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104) · [实际查询体宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241) |
| R134-04 · 引文边界及断言 | E134-021 把 pid.h:124 函数签名与第 125 行体内返回合成正式窗。按协议 P2 缩为第 125 行即可。47 个正式窗均使用同一条通用归属说明；三宏另有明确断言，多数其他窗缺少按编号可查的具体断言。给剩余窗逐项补一句窄断言或准确的现有事实选择器，例如 E134-012 仅支持局部变量声明。冻结 47/47/0 加 16 定位；一换一不增加正式窗数。[E134-021](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md#L209) · [体内一行](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.h#L125) · [协议 v2](https://github.com/08822407d/MyOS2/blob/3251b8f68697/agent-workspace/tasks/00-gpt-task-protocol-v2.md) |

复核分派的配置依据来自固定 CMake 选项及 syscalls.h 中的条件纳入。以上为源码分析，真实预处理结果、链接符号与库桥接均未知。[配置宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L65) · [架构头条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L93) · [零参数宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L1383)

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机及真实 PID、命名空间、进程、调度、时钟、文件操作均为 NOT_RUN。

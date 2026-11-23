---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-FORK-RETURN-IDENTITIES-135"
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
lead_before_publication_short12: "9d333c5959a8"
execution_pin_short12: "95b6af84d8f6"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/01-MYOS2-A-C09a-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md","mykernel/sched/task/pid.c","mykernel/sched/forkexec/fork.c","mykernel/sched/task/pid.h","mykernel/arch/x86_64/sched/context/process_arch.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot"]
status: "CLOSED_DOCUMENTATION_ONLY"
prior_original_review: "RETURN"
combined_135: "PASS_PENDING_LOCAL"
closed: ["R135-01","R135-02","R135-03"]
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 135 冻结原件与 136 三项 D00 的组合裁定

R135-01、R135-02、R135-03 均裁为 CLOSED_DOCUMENTATION_ONLY。135 冻结原判 RETURN 留档，原件合读本次增量后的状态为 PASS_PENDING_LOCAL。旧作者为 C08c／MYOS2-LEAD-002-CHAT-EXEC-20，新作者为 C09a／MYOS2-LEAD-002-CHAT-EXEC-21，裁定人为 C09。135 与 136 的执行提交分别为 d01efe19ccec 与 95b6af84d8f6。阶段 2。[135 原始退回标准](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md) · [188 验收约定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md#L49) · [135 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852) · [136 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7096187292)

## R135-01 · 查询对象、命名空间和条件顺序

对 135 的每个 preserved_twenty_fields 与 136 的 old_preserved_twenty_fields 作结构化逐键值比较，含嵌套 upid，十例 200/200 相同。十个返回分别复核。下表的“相同”表示目标与 current=P 为同一任务。五个默认查询均经 P.thread_pid=T → T.numbers[T.level=1].ns=N1。五个显式查询均实际判断 473 为假、略过 474、在 475 读目标槽。[135 原十例](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L257) · [136 修订表](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L38) · [固定条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L473)

| 查询 | 目标 | 相同 | 473 判值／474 | 返回 |
|---|---|---|---|---:|
| C1_GETPID | P | 是 | 真／到达 | 20 |
| C1_GETPPID | RealParent | 否 | 真／到达 | 7 |
| C1_PIDVNR | P | 是 | 真／到达 | 22 |
| C2_N0_TGID | P | 是 | 假／跳过 | 5100 |
| C2_N1_TGID | P | 是 | 假／跳过 | 20 |
| C3_GETPPID_PARENT0 | RealParent0 | 否 | 真／到达 | 0 |
| C3_N0_GP0 | RealParent0 | 否 | 假／跳过 | 4100 |
| C3_GETPID | P | 是 | 真／到达 | 20 |
| C4_M1_G | P | 是 | 假／跳过 | 0 |
| C4_N1_G | P | 是 | 假／跳过 | 20 |

五条旧显式边在冻结 135 facts 各唯一，旧 DOT 的行号依次为 242、258、290、324、340。136 对应每一个 Q136 前缀的 if473→branch 及 branch→slot475 两段分别采用 IF_NS_NONNULL_SKIP_474 和 SEQUENCE_TO_475。新件中的旧 DOT 选择器是按端点、类型检索的简写，按三元组查认。[旧图首个显式样本](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L242) · [新图](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot#L129)

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

## R135-02 · 每个槽的门和原始返回指针

旧 A.copy_process.slots 和 C1/C2 的第 12 步均与新 R135_02 对齐。外门为 p->pid 非零，PIDTYPE_PID 写 K.thread_pid；随后组长内门为真，PIDTYPE_TGID 才写 K.signal->pids[PIDTYPE_TGID]。成功两例的组长条件来自样本给定，两个槽都保存 KPID 对象。kernel_clone 的 IS_ERR 使用 copy_process 返回的 p。两成功样本 p=K、判假、临时引用计数 2→3→2；指定失败样本 p=ERR_PTR(-ENOMEM)、判真，内核经 PTR_ERR 返回 −ENOMEM，两个槽均未到达。子任务标量 PID/TGID 与临时 PID 引用分别记账。[旧槽说明](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L1991) · [新分层门及三例](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L710) · [按类型写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L848) · [原始 p 的错误出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1488)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		init_task_pid(p, PIDTYPE_PID, pid_struct);
		if (thread_group_leader(p)) {
			init_task_pid(p, PIDTYPE_TGID, pid_struct);
```

## R135-03 · 谁返回、谁观察、谁继续执行

C1/C2 的 kernel_clone 各自返回 5200/30，源级结果无需 libc 前提。用户观察另需 ABI 与用户路径成立。五条旧箭头各在 135 facts 和 DOT 找到唯一实例，源为 C135_C1_19 或 C135_C2_19，终点分别为两例的子 getpid/getppid 和 C2 显式 helper，对应原 DOT 626、632、714、720、732。新图保留父方返回→父方用户观察→父自身后续查询；子方由保存帧、入队、未来被选、实际执行和返回用户态等独立条件支撑。C2 的 N0 helper 直接从给定 K/TGID/N0 求得 5200。父子相对时序 UNKNOWN。[旧图父方边](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L626) · [五条旧选择器和新含义](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L763) · [新图关系](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L2159)

| 原样本 | 冻结字段逐项比对 | 主线重核的结果 |
|---|---|---|
| C1 | 26/26 | 父内核返回 5200，子保存 ax 57→0；若查询到达，子 PID 5200、父 PID 5100，父方查自身 5100 |
| C2 | 27/27 | 父内核返回 30，子保存 ax 57→0；若查询到达，子 PID 30、父 PID 20，父方查自身 20；独立 N0 对照 5200 |
| C3 | 6/6 个事件、11/11 组未达完全相同 | dup_task_struct 返回 NULL，成功路径未到。内核返回 −ENOMEM；另给用户 rv=−1 时才可推导相应用户分支 |

冻结 53 个字段按名称和值核对，C3 的事件和未达数组逐项核对。完整本地 P9、构建及运行均为 NOT_RUN。上述裁定限已申请的三项历史勘误，第 136 批本身另有四项定点退回。[原值记录](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L849) · [136 独审](CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md)

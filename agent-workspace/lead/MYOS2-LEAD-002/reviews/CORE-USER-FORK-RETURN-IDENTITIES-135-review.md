---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-pid-view134-reviewed-fork-returns135.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/186-user-process-id-namespace-view-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/first-run-frame.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/clone-group.md","mykernel/sched/forkexec/fork.c","mykernel/arch/x86_64/sched/context/process_arch.c","myinitramfs/myinit/initd.c","mykernel/sched/task/pid.h","mykernel/sched/task/pid.c","mykernel/sched/task/pid_api.h","mykernel/sched/task/task.h","mykernel/sched/task/task_types.h","mykernel/sched/syscall.c","mykernel/lock_IPC/signal/signal.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/include/linux/kernel/syscalls.h","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md"]
status: "RETURN"
open_returns: ["R135-01","R135-02","R135-03"]
combined_133: "PASS_PENDING_LOCAL"
combined_134: "PASS_PENDING_LOCAL"
former_executor_post_comment_read_sequence: "UNKNOWN"
remote_delivery_independently_reconstructed: true
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 135 批独立验收 · 普通 fork 的父方编号、子帧零值与远端终态

**裁定 RETURN，三个样本的数值结论全部核收。** 第一个成功样本中父方内核返回 5200，第二个返回 30。子方保存帧的 ax 两次都从 57 变为 0。这个零由 copy_thread 写入，子任务随后查询的进程编号另由其线程组对象及观察者的命名空间决定。三项退回集中在查询图的新字段、PID 槽的写入条件，以及图对父子执行顺序的表达。此前五项 D00 均已文档关闭，133、134 合并补正后的裁定均为 PASS_PENDING_LOCAL。[历史裁定](CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md) · [187 合同](../187-user-fork-return-identities-contract.md) · [135 正式作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852)

## C08c 耗尽后独立重建的现场

Owner 报告原执行对话在输出终检摘要后提示上下文耗尽。C09 从固定执行提交 d01efe19ccec 及活动原 exec 独立各读六件，按完整 UTF-8 字节及 Git Blob 头重算 SHA-1。该提交唯一父为 f0d46c448367，compare 领先 1、落后 0，仅指定六件新增，合计 9,784 行、386,205 字节。下表给出 C09 复算值。[固定清单](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/MANIFEST.md) · [父子比较](https://github.com/08822407d/MyOS2/compare/f0d46c448367...d01efe19ccec)

| 文件 | 新增行 | UTF-8 字节 | Blob 前十二位 |
|---|---:|---:|---|
| MANIFEST.md | 170 | 16543 | b45ff7aaed13 |
| fork-return-identities.md | 151 | 19043 | 4f61df9adb2c |
| facts-and-boundaries.yaml | 7540 | 245720 | ffb48d7bc0f2 |
| fork-return-identities-paths.dot | 751 | 68156 | d51d79748e84 |
| deferred-findings.yaml | 142 | 6791 | 23e30f26aa19 |
| evidence.md | 1030 | 29952 | ff5bb16ef28f |

远端正式作者 #7095153852 按 ID 两次回读，正文与无 since 分页 100/100/59/0 的记录相符，发布前有 259 个不同评论 ID，完整作者标记唯一。其表中 DOT 的“Blob短12”实际为 10 位 d51d79748e；正确前十二位是 **d51d79748e84**。这是评论中的截短，固定与活动的完整 Blob 相符。本裁定与 C09 回执即为校正记录，旧作者回执冻结。

直接 refs 为 lead 676e4169f14c、exec d01efe19ccec、time a039d9803ade、direct master e8e60cd19540。开放列表只有原 PR16、17，二者对 master 且未合并，分别 Open/Ready 与 Open/Draft。作者评论末尾将发出后的 ID 双读、分页、refs 和六件回读列为后续步骤；Owner 转来的是作者“全部通过”的终检摘要。C09 能独立证实这些步骤在本次回读时均满足；仅凭可见远端对象，原 C08c 发评后实际做过哪些只读步骤、何时完成及内部顺序均 UNKNOWN。当前无须旧会话补件、补签或重发。[PR17 #7095153852](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852) · [187 正式发令](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094597734)

48 条声明输入存在。直接 master 公约的启动原句、固定 time 与直接 master 的两处反向金丝雀均核实。两份 YAML 可按 JSON 读。DOT 与 facts 的 10 组、348 个唯一节点、379 条唯一类型边的分组、方向、标签、关系类型及说明逐项一致，缺端点 0。81 条连续源码窗逐条按原始物理行和字节匹配，66 为正式窗，另 15 个独立定位。六件未命中 P1 长串、P9-11 四个禁语或内部引用句柄。以上为文本与有界纸面验收。[证据](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md) · [图](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot)

## A/B/C · 从源码重算

普通 fork 的参数仅显式指定 SIGCHLD，其余给定 flags、fn、kthread、stack 均为零。成功路径中，copy_thread 先复制父保存帧，再写子帧。copy_process 把 numbers[0].nr 写入 K.pid，普通非 CLONE_THREAD 分支将 K.tgid 设为 K.pid，普通非 CLONE_PARENT/THREAD 分支使 K.real_parent=P。组长谓词在两个成功样本中另行给定为真。kernel_clone 在当前 P 的命名空间取出 K 的 PID 数字，随后调用 wake_up_new_task，归还 PID 引用，最后返回早已存入 nr 的整数。[子帧](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/process_arch.c#L134) · [标量与线程组号](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1206) · [两个对象槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1311) · [取号和唤醒](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1497)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
```

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

| 重算项目 | C1，P/K 同在 N0 | C2，P/K 同在 N1 | C3，指定 dup_task_struct 返回 NULL |
|---|---|---|---|
| K.pid / K.tgid；两个槽 | 5200 / 5200，均指 KPID | 5200 / 5200，均指 KPID | 无新 K |
| 父方内核返回 | 5200 | 30 | −ENOMEM |
| 子保存帧 ax | 复制得 57，随后写 0 | 复制得 57，随后写 0 | 未到达 copy_thread |
| 子 getpid / 子 getppid / 父 getpid，若到达 | 5200 / 5100 / 5100 | 30 / 20 / 20 | 用户库映射未知。若另给 rv=−1，则 P 走 else、查询得 20 |
| 显式 N0 查 K 的 TGID | 合同未另设此对照 | 5200 | 无 K |
| KPID 临时引用；唤醒调用 | 2→3→2；到达 | 2→3→2；到达 | 均未到达 |

C1/C2 各有 19 个作者所列事件，独立逐字段重算结果为 26/26 和 27/27。C3 的 6 个事件与 11 组未到达操作吻合：copy_process 在 dup 前设置 −ENOMEM，NULL 时经 fork_out 的锁和解锁返回 ERR_PTR；kernel_clone 的 IS_ERR(p) 分支直接传播 PTR_ERR，sys_fork 随即返回。用户可见 −1 仅在另给前提下用于判断 init 分支。实际父子何时返回用户态、何时执行下一查询均未知。[三组作者样本](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md#L119) · [指定失败](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L958) · [错误出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1428) · [传播](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1488)

## 本批三项追加勘误

| 编号 | 冻结证据、判定与验收标准 |
|---|---|
| R135-01 · 观察者身份和实际行序 | 在五个缺省查询中，target_is_observer 均填 false，其中 C1_GETPID、C1_PIDVNR、C3_GETPID 的目标任务和 current 同为 P。两条父目标查询才为 false。应区分“目标等于 current”和“缺省 ns 始终来自 current”。另有五个显式查询仍用 SOURCE_STATEMENT_ORDER_PID_C_474_BEFORE_475，显式 ns 实际只经过 473 的假判断并跳过 474。逐例对齐机器字段和图类型。200 个原字段及返回值已核收。[布尔字段](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L257) · [显式查询边](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L242) · [条件与取槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L473) |
| R135-02 · 两个写槽门和错误指针判定 | A.copy_process.slots.require_group_leader 把组长条件用于两个槽。PID 非零已足以到达 PIDTYPE_PID 初始化；位于其后的 TGID 槽另需 thread_group_leader(p) 为真。C1/C2 虽同时满足这两门，仍须按槽标所属条件。A135_iserr 节点写 IS_ERR(K)，且接收成功和错误两路。应标 IS_ERR(p)，在成功分支才确认 p=K，失败分支为错误指针。[共用字段](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L1991) · [图节点](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L495) · [分层写槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1311) · [错误判断](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1488) |
| R135-03 · 父方源码返回、子方后续查询和独立 helper | C1/C2 的 FORK_PARENT 明示 kernel_clone 源码返回，却将 success_conditional_on_libc 填 true。每例两条边从父方最终 return 指向子方随后可能的 getpid、getppid，共四条，缺少这种跨任务先后证据。C2 显式 N0 的 helper 对照也挂在父方 return 上。分别建父方内核结果与用户观察、子帧已置零与子方条件执行、独立对照三组关系；两位执行者的相对时序记 UNKNOWN。[父方字段](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L2309) · [C1 两条边](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L626) · [C2 后续边](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L714) · [独立 helper](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L732) |

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

构建配置的否定性判断限固定文件及所读函数。实际编译后的代码形态与运行时效果未观察。[固定配置选项](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L62)

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机与真实进程、PID、调度、时钟、文件操作均为 NOT_RUN。

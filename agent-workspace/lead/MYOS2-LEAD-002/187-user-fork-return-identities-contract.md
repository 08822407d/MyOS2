---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
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
lead_before_publication_short12: "3251b8f68697"
execution_pin_short12: "f0d46c448367"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/186-user-process-id-namespace-view-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-D00-132-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-rq-clock133-reviewed-process-id134.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/first-run-frame.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/clone-group.md","mykernel/sched/forkexec/fork.c","mykernel/arch/x86_64/sched/context/process_arch.c","myinitramfs/myinit/initd.c","mykernel/sched/task/pid.h","mykernel/sched/task/pid.c","mykernel/sched/task/pid_api.h","mykernel/sched/syscall.c","mykernel/lock_IPC/signal/signal.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/include/linux/kernel/syscalls.h","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
status: "PUBLISHED_PENDING_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_display_name: "MYOS2-A-C08c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-20"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/"
allowed_files: ["MANIFEST.md","fork-return-identities.md","facts-and-boundaries.yaml","fork-return-identities-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R133-01","R134-01","R134-02","R134-03","R134-04"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 187 号合同 · 第 135 批 · 一次普通 fork 中，父方返回哪个编号，子方的零从哪里来

原「MYOS2-A-C08c 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-20，GPT-6＋Extra。Owner 转发固定合同及 C09 正式发布后，在原分支连续完成 G00、D00、A/B/C 和 G04。134 的给定查询已分清线程编号、线程组编号及观察者。本批把这些结果带回 init 的 fork 分支，连接父方 kernel_clone 返回、子方寄存器帧及后续两个 getpid 调用。只研究普通用户 fork 的有界源码和给定对象。[134 已核的查询](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md#L111) · [67 首帧研究](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/first-run-frame.md) · [72 组与父关系](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/clone-group.md) · [11 PID 对象研究](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml)

## G00 · 冻结基点与启动

1. 读本合同、同提交[134 独审](reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md)、[历史补正裁定](reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md)和[检查点](checkpoints/2026-11-23-pid-view134-reviewed-fork-returns135.md)，读[186 合同](186-user-process-id-namespace-view-contract.md)、134 固定六件、134 作者评论、C09 本轮双标记评论及上一轮发布评论。按 182/03 和 #7088796995 核 C09 接管。工作令、公约、工作区 README、协议 v2、18/24/28/63/79/110/124 继续适用。声明原 C08c 指导复用的固定来源及范围，另列本批实际新读件。[PR17 #7094240400](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094240400) · [PR17 #7093714156](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7093714156) · [PR17 #7088796995](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088796995)
2. 直接读四个 refs/heads，正常首次启动 lead 指本合同正式发布，exec 为 f0d46c448367，time 为 a039d9803ade，direct master 为 e8e60cd19540。开放 PR 翻至空，原 PR16 Open/Ready、原题 PR17 Open/Draft，均至 master、未合并。PR17 顶层评论无 since 分页至空，134 作者和 134 主审／187 发布各唯一；后两个标记位于同一条 C09 评论。按 ID 核正文。直接 master 公约启动句及固定 time/master 两处金丝雀作正反核对。
3. 134 执行固定提交单父 5e979654dc92，compare 六件纯新增、4,480 行、170,040 字节。固定与活动原 exec 分别取全文，重算六个 Git Blob，evidence.md 为 66dfb0a00a20。检查新 135 目录和完整作者标记是否已存在；已知预期效果按 63 只续缺步，陌生并发状态保全现场。记录实际通道和可见身份，后台型号与档位无法认证的填 UNATTESTED。[63 号](63-github-write-retry-and-readback.md) · [134 固定清单](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md)

## D00 · 在新六件追加五项有限勘误

| 编号 | 冻结选择器与验收动作 |
|---|---|
| R133-01（仅余一条） | 读 133 facts 的 D00 中 correct.indirect_call。完整保留旧虚拟三元组 S132_class_iter→S132_picker / INDIRECT_CALL_INSTANCE，owner=__pick_next_task；把该旧源显式映射到 133 实图 D133_CLASS_owner→D133_CLASS_call / INDIRECT_CALL_INSTANCE 及 D133_CLASS_call→D133_CLASS_callee / TARGET_RESOLUTION_IF_MYOS_RT。给两载体命中数，区分旧概念空间和实际新图。134 已核四 loop、静态边、指针和数据边仅引用一次。 |
| R134-01 | 逐个列 134 D00.R133_02.replacement_graph 四个 D134_CLOCK 大写 ID 与实际 PID134_CLOCK_guard、early、add、caller 的一对一映射；逐条列四个原/实边三元组、真实 kind、在 facts.graph 与 DOT 中的命中数。两条汇合边实际为 RETURN_TO_CALLER_THEN_FLAGS_OVERWRITE、NORMAL_RETURN_THEN_FLAGS_OVERWRITE。注明报告/清单里的旧拼写。历史 133 的控制流和三例写点已关闭。 |
| R134-02 | 将 134 十条 target→observer / EVALUATION_ORDER 定位到原两个载体，按源码分开入口选择目标 task、传入 type/ns、处理默认 ns、获取目标 slot 与 pid_s、pid_nr_ns 的条件门。显式 ns 记录为实参数据，给出 ns 条件的求值，避免对子表达式实参加设 C 求值顺序。五条默认 viewer_chain 写明 task_pid(P) 返回 T，ns_of_pid(T) 返回 T.numbers[T.level].ns。将报告十个简称、完整 C.queries.id 和实际图节点或对应子图逐行对齐；核原 20 字段×10 和两个短路零值，正文、机器表和图同步。 |
| R134-03 | 冻结 A 的两条 CONDITIONAL_DISPATCH_39/110，对 getpid、getppid 分别列 switch 直接调用宏生成的 __x64_sys_*、__se_sys_*、承载查询体的 __do_sys_* 及其调用者。区分直接调用、参数、复合可达关系，并列同一登记头在 11/21/30 三处的声明、静态表与 switch 语境。源码宏条件及活动配置另有固定短窗。实际预处理、符号链接、libc 桥接保持 UNKNOWN。 |
| R134-04 | 以 pid.h:125 单独的体内返回替代 E134-021 的签名＋返回。给 E134-001 至 047 逐 ID 补具体断言或精确事实选择器，不重复抄 47 个源码窗。声明、配置、取槽、值的读取和最终返回各按窄断言归属。冻结 47 裸／47 正式／0 降级加 16 定位；一换一的旧批完整替代视图仍为 47。135 新窗单独计数。 |

作者对五项统一标 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW。133 组合仍为 RETURN，R133-02、R133-03 文档关闭；132 组合仍为 PASS_PENDING_LOCAL。引用旧件，在 135 生成可逐项重放的差量。[133 虚拟源](https://github.com/08822407d/MyOS2/blob/5e979654dc92/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml#L1348) · [134 时钟字段](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml#L322) · [134 查询图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L201) · [分派图](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot#L109) · [引文原件](https://github.com/08822407d/MyOS2/blob/f0d46c448367/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md#L209) · [包装器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241)

## A · 父方如何得到 fork 的内核返回值

以固定 normal_boot 与 sched/syscall.c 的普通 fork 为起点。说明 rv=0 与 rv!=0 两分支、这两个用户侧局部 getpid 取值是否参与分支、fork 参数结构的实际初始化及向 kernel_clone 的返回传递。普通路径给定 flags=0、exit_signal=SIGCHLD、fn=NULL、kthread=0、stack=0。fork 中的计数循环只作存在性定位。读 copy_process 中 PID 与 TGID 的标量赋值、real_parent 以及 PID/TGID 两个对象槽的设置条件；核 thread_group_leader(p) 的实际谓词，把 p->exit_signal 与 args->exit_signal 区别开来。非零 pid、组长谓词真列为成功样本的明确前提。[用户调用点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L30) · [普通 fork](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L51) · [标量字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1206) · [父任务字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1267) · [两个槽](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1314) · [组长条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L204)

沿 kernel_clone 的 copy_process、IS_ERR、get_task_pid(p,PIDTYPE_PID)、pid_vnr、wake_up_new_task、put_pid、return nr，说明谁提供目标任务、谁提供默认 ns、PID 对象引用与已复制的整数在何处区分。只需梳理 get_pid 活动加引用和 put_pid 条件减引用的位置，在普通有效且另有引用的前提下推演。把源码顺序和外部唤醒产生的可能运行分开。[内核返回路径](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1485) · [唤醒及归还](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1516) · [get_task_pid](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L407) · [pid_vnr](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L461) · [get_pid](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.h#L61) · [put_pid](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L111)

## B · 子方零值与下一次编号查询

仅选非 PF_KTHREAD、fn=NULL、stack=0 的普通用户分支。读 copy_process 到 copy_thread、childregs 的取得、复制当前寄存器帧、childregs->ax=0 和本分支的条件写点。指出这个零属于哪个任务、哪个字段、在何时写入。与 ret_from_fork 中 fn 非空才出现的另一个 regs->ax=0 明确区分。参考 67 已定位的首帧与返回条件，无须重算整帧偏移或展开汇编。用户侧在返回工作、调度、ABI 与库桥接都按预期成立时才能观察到给定值。父子实际先后顺序未知。[建立子帧](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/process_arch.c#L92) · [普通用户帧](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/process_arch.c#L133) · [另一个条件写点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/process_arch.c#L66) · [调用 copy_thread](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1143)

对照下一次 getpid(child)、getppid(child)、getpid(parent)，逐次写 current、目标任务、PID 或 TGID 槽、默认观察者的来源及公开编号。另列 pid_nr 对 numbers[0] 的取值，说明同一子 PID 对象的标量字段和公开编号是否相等。只推给定对象，不扩展到命名空间创建、进程退出或真实并发。[全局标量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.h#L115) · [子与父的公开组号](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L45) · [父对象查询](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L115) · [逐层公开编号](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/pid.c#L447)

## C · 两个普通成功前态与一个提前返回对照

所有输入都是静态条件。P 是父方调用任务，K 是新子任务，两个成功样本中 K.real_parent=P，K 具备充分的 numbers 槽且以同一给定子 PID 对象初始化 PID/TGID 槽。P 的线程 PID 和 TGID 使用表中各自对象。给定普通源级返回路径正常、子帧复制前父帧 ax=57；P 和 K 在每个成功样本中具有相同活动命名空间。成功样本中 get_task_pid 前子 PID 的引用计数为 2，本片段除该临时取得与归还外无其他增减。两例独立，不模拟命名空间创建。

| 输入 | P 的线程对象 | P 的组对象 | K 的 PID / TGID 共用对象 | 查询条件 |
|---|---|---|---|---|
| C1 · 同在 N0 | [(N0,5102)] | [(N0,5100)] | [(N0,5200)] | 三个对象 level=0，P、K 的观察者均为 N0。 |
| C2 · 同在 N1 | [(N0,5102),(N1,22)] | [(N0,5100),(N1,20)] | [(N0,5200),(N1,30)] | N0/N1 分别为第 0/1 层，三个对象 level=1；另用显式 N0 对同一 K 的 TGID 作独立 helper 对照。 |
| C3 · 提前返回 | P 沿用 C2 | P 沿用 C2 | dup_task_struct 在指定位置返回 NULL | 沿 copy_process 的现有 -ENOMEM、fork_out、ERR_PTR、kernel_clone 的 IS_ERR/PTR_ERR 重建先后。仅为一个指定结果的静态分支，不执行分配或诱发错误。 |

C1/C2 逐项列 K.pid / K.tgid、K 两个槽的对象身份、K.real_parent、父方取得及归还前后的临时引用、pid_vnr 取哪一层与返回 nr、唤醒语句是否到达、子帧复制前后 ax、子方的条件 fork 返回、双方下一次 getpid 和子方 getppid。明确子方 fork 的零与子方 getpid 的数值各自由谁产生，父方 fork 取子线程 PID，父方自己的 getpid 取父线程组 TGID。C3 列内核返回、未到达的子帧/取号/唤醒/归还/成功 nr 返回，以及用户库映射是否有固定证据。若另给“用户侧已得到 −1”，只判断 init 会走哪支；本例不产生一个走子分支的新任务。[指定提前出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L958) · [返回错误指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1428) · [调用者传播](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1488)

## 六件、自检及远端交付

仅在原 claude/dazzling-cori-q0dnyt 的 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/ 新增 MANIFEST.md、fork-return-identities.md、facts-and-boundaries.yaml、fork-return-identities-paths.dot、deferred-findings.yaml、evidence.md。MANIFEST 列新增件、分支、输入路径和启动自检；中文报告覆盖 D00/A/B/C；facts 逐例记录输入、条件、状态和输出；DOT 用同一组可逐字查找的稳定 ID 和实际类型边，标清条件返回、调用与数据关系；deferred 列用户桥接、实际先后、运行及其他未知边界；evidence 单独核新引文和定位。

协议 v2 十三条适用。P2 每条正式引文取同一符号定义体内 1–5 连续行，附近列可核的窄断言。强制覆盖 D00 的缺省 ns 与目标槽顺序、包装器两个转发层、task_pid 体内返回；新专题覆盖 fork 入参和返回、组长及两个槽、父关系、全局/可见编号、取得和归还引用、普通子帧 ax=0、两个错误返回层、A/B 的配置条件。P6 按本六件新正式标签计数。登记 inputs_read、read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check 三整数；P9-1/2/3/4/5/6/7/8/11/13 作连接器可达的逐项检查，9/10/12 按实际代码件、补丁、降级范围据实登记。完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机及真实 PID、命名空间、进程、调度、时钟、文件操作均为 NOT_RUN。

G04 在原 exec 用当前 expected_sha、force=false，仅单父正常快进。compare 六新增、零删改，按固定提交和活动 ref 对六件全文、UTF-8 字节与重算 Git Blob 做 6/6 验证。PR17 顶层无 since 全分页查重，仅发一条 `MYOS2-LEAD-002-CHAT-EXEC-20 / CORE-USER-FORK-RETURN-IDENTITIES-135 / FORMAL_DELIVERY`，附固定提交、六件、D00 待主线复核、实际自检及未知项。按评论 ID 双读、全分页查唯一，再核四 refs、原两 PR 及六件。异常按 63 先查远端效果，再按 Retry-After 或约 10/20/40 秒有界恢复。终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING。交回原 C09 用 Pro 独审并同轮发布下一批。[79 号](79-work-package-sizing-and-pro-review.md) · [110 号](110-owner-communication-and-delivery-continuity.md) · [124 号](124-model-routing-and-pro-message-budget.md)

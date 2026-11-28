---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
followup_id: "CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-pid-view134-reviewed-fork-returns135.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/181-executor-continuation/00-C08b-to-C08c-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/181-executor-continuation/01-MYOS2-A-C08c-receiver-startup-prompt.md","mykernel/sched/forkexec/fork.c","mykernel/arch/x86_64/sched/context/process_arch.c","mykernel/sched/task/pid.c","mykernel/sched/task/pid.h","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/myos_rt.c","mykernel/lib/list/double_list.h","mykernel/lib/list/double_list_types.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/sched/task/task_const.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection.md"]
status: "PUBLISHED_PENDING_NEW_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
executor_display_name: "MYOS2-A-C09a 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/"
allowed_files: ["MANIFEST.md","new-task-runqueue-entry.md","facts-and-boundaries.yaml","new-task-runqueue-entry-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R135-01","R135-02","R135-03"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 188 号合同 · 第 136 批 · 新子任务如何进入 CPU 0 就绪队列

**接收及执行模型：次一档，GPT-6＋Extra。** 本合同只给全新项目普通 ChatGPT 对话「MYOS2-A-C09a 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-21。Owner 在新对话转发本次固定启动词和正式发布回执后，一轮连续完成接收、指导刷新、G00、D00、A/B/C、G04。135 止于父方调用 wake_up_new_task 的证据。本批继续查该调用如何改子任务状态、链表和计数，以及一次单独给定的选任务调用何时才取走队首。[00 交接包](188-executor-continuation/00-C08c-to-C09a-handoff-package.md) · [01 新对话启动词](188-executor-continuation/01-MYOS2-A-C09a-receiver-startup-prompt.md) · [135 裁定](reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md) · [历史裁定](reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md) · [检查点](checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md)

## 接收与 G00

1. 依 00/01 核包、形成 mnemosyne_handoff_receive，独立全文读九份 MyOS2 原约束及 110/124，再依登记单独形成 mnemosyne_guidance_refresh，实际读件与可见档位入 136 MANIFEST。新身份从 136 接手原执行分支和 PR17。旧 C08c 的内部终检顺序为 UNKNOWN，C09 已重建可见远端交付，以本轮裁定为准。
2. 读同提交六份主线材料、187 合同、135 固定六件、135 唯一作者评论、C09 本轮发布与前一轮发布。核 182/03 主线原接管签认。核原两 PR 与四 refs/heads，开放 PR 和 PR17 顶层评论无 since 各自翻至空，再按 ID 读取。正常首次启动时 lead 为本发布提交，exec d01efe19ccec、time a039d9803ade、直接 master e8e60cd19540。PR16 Open/Ready、原题 PR17 Open/Draft，均至 master、未合并。135 唯一作者为 #7095153852。135 主审、188 发布与新执行席位交接准备三枚标记应位于同一唯一 C09 评论。[PR17 #7095153852](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852) · [PR17 #7094597734](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094597734) · [PR17 #7088796995](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088796995)
3. 核 135 单父 f0d46c448367、一个提交六个纯新增、9,784 行／386,205 字节。固定／活动六件读全文并按 UTF-8 重算 Git Blob。DOT 前十二位以实物 d51d79748e84 为准，作者评论表里该项只列十位。直接 master 公约 §1 第 2 条与 time/master 金丝雀正反核。首次写前核 136 新目录与本人完整作者标记，核本批 expected_sha。若已有本授权所描述的部分远端效果，先按 63 查实并只续缺步；遇到不明并行变化如实报告。[63 号](63-github-write-retry-and-readback.md)

## D00 · 三项追加勘误

| 编号 | 冻结输入与验收动作 |
|---|---|
| R135-01 | 对 135 D00.R134_02 五个缺省样本分别列 current、目标任务、两者是否同一、提供 ns 的对象链。顺序为真、假、真、假、真（C1_GETPID、C1_GETPPID、C1_PIDVNR、C3_GETPPID_PARENT0、C3_GETPID）。五个显式样本准确标记 473 的假判断、474 跳过、475 取槽，修正旧 SOURCE_STATEMENT_ORDER_PID_C_474_BEFORE_475；为旧边列原两载体位置与新边的实际 ID/kind，保留十例 200 个既有字段及十个返回。 |
| R135-02 | A.copy_process.slots 及两个成功样本第 12 步分别列 PIDTYPE_PID / TGID 的写点和真实所需门。p->pid 非零时先写 PID 槽，此后组长谓词真再写 TGID 槽。A135_iserr 分开 copy_process 的原始 p、成功任务 K 和失败错误指针，判定使用 IS_ERR(p)。两成功一失败的门结果、函数拥有者及标量/引用结果逐项与旧件对齐。 |
| R135-03 | C1/C2 的 FORK_PARENT 区分内核源码返回和用户经库观察。父方第 19 步通向子方 getpid/getppid 的四条边、通向 C2 独立显式 helper 的一条边均列原选择器；子方后续查询只能依赖子方准备、获调度和用户返回等独立前提。显式 helper 作给定对象对照。保留父方自身用户后继的条件关系。逐项核 C1/C2 的 53 个结果、C3 的六步及 11 组未达，父子相对时序为 UNKNOWN。 |

各项由新作者标 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW，指明冻结原作者、旧批、旧 ID 和本批新增差量。135 回执 DOT 短号和报告“__do 定义头”已由主线校明，引用裁定即可。133、134 组合均为 PASS_PENDING_LOCAL。[身份与查询](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L257) · [两层写槽](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L1991) · [父方结果的前提](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml#L2309) · [子方和对照边](https://github.com/08822407d/MyOS2/blob/d01efe19ccec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot#L626)

## A · 唤醒前由谁准备子任务

沿普通成功 fork 的 copy_process→sched_fork→__sched_fork，读固定源码的 vruntime、rt.run_list、__state、on_cpu 初始化。列 INIT_LIST_S 对子节点的两个写入、TASK_NEW 与 TASK_RUNNING 的活动定义，区分注释中涉及的 on_rq / time_slice 与实际赋值。定位 kernel_clone 在获得子方公开编号后调用 wake_up_new_task，再归还临时 PID 引用。只计所列字段和函数体内的读写，不推定整机默认值。[调度准备调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1104) · [初始化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L574) · [新建态与 on_cpu](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L619) · [链节点初始化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L149) · [任务状态宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_const.h#L26) · [唤醒请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1516)

## B · 直接队首插入怎样改变链与计数

从 wake_up_new_task 的活动代码列 CPU 与 rq 选择、WRITE_ONCE 设置状态、传给 list_header_add_to_head 的两个不同对象。沿 list_add_to_next、__list_add_between、返回后 count++，按固定语句给出四次指针更新、字段拥有者与最终前后驱。给定每个插入点均合法、非空指针、双链互反、插入节点未在队列中，判断过程正常通过。只研究正常插入；锁、CPU 迁移、时钟更新、通用 activate_task 和抢占检查在该函数内按活动语句/注释区分。唤醒结束时分别核子 __state、on_cpu、rq->curr 及 need_resched 有无本片段写点。[固定 CPU 及状态](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L684) · [插头](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L721) · [头与计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L646) · [四次指针写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L189) · [队列字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/runqueue/runqueue_types.h#L195)

有效单 CPU 0，H 为链表 anchor，初始队列 H↔Q↔R↔H，count=2，rq->curr=P，P 不在队列。K、J 是已各自完成 A 的普通新子任务，两个 rt.run_list 分别自连。先给定依次对 K 和 J 调用 wake_up_new_task，中间无其他变动；另由同一初态作 J 后 K 的独立对照。逐次列状态、插入前 head/first、实际节点对象、前后驱、尾节点、计数和完整正反序。每次入队与 CPU 实际执行分别记账；代码不提供两次真实 fork 的调度时间。无需模拟不一致链表或产生异常输入。

## C · 已就绪的子任务何时才被一次给定的 picker 选中

两个独立样本均从 B 的原始 Q,R 队列重新开始，先完成 K 一次插头，得到待选队列。给定 current=P、rq->curr=P、P 普通非 idle 且 TASK_RUNNING、need_resched=false、P 原 vruntime=3、time_slice=10、last_jiffies=100、Q/R 的 vruntime=20/40、K 的 vruntime=0。此后单独假设一次 pick_next_task_myos 调用；单个样本中该函数各次读 jiffies 均为表中同一数值，无并发、无其它改动。

| 样本 | 给定 jiffies | 需区分的效果 |
|---|---:|---|
| C1 | 103 | K 已入队，核门表达式是否到达队列 count，是否取队首，谁留在 rq->curr。 |
| C2 | 110 | 时间片刚到等号，核是否读 count、取哪个队首、按 P 旧键如何回插、何时增加 vruntime、写 last_jiffies 及 rq->curr。 |

列 K 入队前/后与 picker 完成后的队列、计数、K.__state、P 的键、used_jiffies、last_jiffies、返回任务和 rq->curr。指明每种条件是否实际求值，区分 K 的“可运行”、被选中、真正开始执行、返回用户态四件事。仅此普通样本，C2 移走 K 后留下 Q、R 且 P 的旧键小于 Q；按实际语句复核到足以确定样本结果为止。已核 132 的通用选任务分析只用作定位。[picker 判断及摘头](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L12) · [回插、计费和返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L37) · [摘头与计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L652)

## 六件与 G04

原分支 claude/dazzling-cori-q0dnyt、原 Draft PR17，唯一写区 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/。只新增 MANIFEST.md、new-task-runqueue-entry.md、facts-and-boundaries.yaml、new-task-runqueue-entry-paths.dot、deferred-findings.yaml、evidence.md。MANIFEST 记录实际接收、指导刷新、G00 和证据数；中文报告和机器件含 D00、有前态和后态的 A/B/C、实际写入者、条件是否求值、样本观测边界。图与机器件按实际节点 ID、方向、类型、标签和端点逐项相符。deferred 写可核结论、实际调度和物理时序等未知项及恢复所需证据。evidence 为本批新增窗分别编号和列窄断言。

协议 v2 十三条适用。正式窗只取固定源码同符号定义体内的 1–5 行；必须覆盖三项 D00 的活动分支、PID/TGID 槽、父方返回、子帧零值，新专题的字段初始化、两种任务状态、选定 CPU 和链头、四次指针写、计数、picker 进门/不进门、取队首、回插、计费和返回，另列活动配置依据。注释与独立声明分开作定位；按自身六件实数填写 P6 三整数，P9-1/2/3/4/5/6/7/8/11/13 核连接器可达范围，9/10/12 按实物登记。完整本地 P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机与真实进程、PID、调度、时钟、文件操作均为 NOT_RUN。

首次发布需当时原 exec 的 expected_sha，force=false，单父正常快进一次。compare 仅六新增，固定及活动六件全文、字节与重算 Git Blob 6/6 对齐。PR17 无 since 全分页查重后仅发一条 `MYOS2-LEAD-002-CHAT-EXEC-21 / CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136 / FORMAL_DELIVERY`。作者评论列真实固定位点、六件的准确行数、字节和完整短号、新身份及实际自检，按 ID 双读、全分页核唯一，再核四 refs、两个 PR 和固定/活动六件。在聊天结尾记实际最后读回结果。临时错误按 63 先重查已生效步骤，按 Retry-After 或约 10/20/40 秒有界恢复。终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，交回原 C09 用 Pro 独审并同轮安排下一批。[110 号](110-owner-communication-and-delivery-continuity.md) · [124 号](124-model-routing-and-pro-message-budget.md)

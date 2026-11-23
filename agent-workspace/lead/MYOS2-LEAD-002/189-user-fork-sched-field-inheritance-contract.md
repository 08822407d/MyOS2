---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
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
lead_before_publication_short12: "9d333c5959a8"
execution_pin_short12: "95b6af84d8f6"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/01-MYOS2-A-C09a-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md","mykernel/sched/forkexec/fork.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/myos_rt.c","mykernel/lib/list/double_list.h","mykernel/scripts/options_flags.cmake","mykernel/sched/task/task_types.h","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/scheduler/scheduler.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/lib/list/rcu_list_macro.h","mykernel/lib/list/double_list.c","mykernel/include/linux/kernel/asm-generic/bug.h","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-review.md"]
status: "PUBLISHED_PENDING_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
executor_display_name: "MYOS2-A-C09a 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/"
allowed_files: ["MANIFEST.md","fork-sched-field-inheritance.md","facts-and-boundaries.yaml","fork-sched-field-inheritance-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R136-01","R136-02","R136-03","R136-04"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 189 号合同 · 第 137 批 · 普通 fork 之后，谁的时间片、谁的调度计数

**执行席位为现有「MYOS2-A-C09a 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-21，次一档 GPT-6＋Extra。** 本合同发布时 137 尚未启动。接收 136 时的两个操作已分别登记，继续在本对话从本批 fresh G00 开始。原 136 六件冻结在 95b6af84d8f6。本批承接四项定点文档补正，再沿普通成功 fork 追踪时间片、CPU 时间和切换计数。主线 C09 用 Pro 独审且同轮发布后继。[136 裁定](reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md) · [135 组合裁定](reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md) · [检查点](checkpoints/2026-11-23-new-task136-reviewed-fork-fields137.md) · [136 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7096187292)

## 固定输入与 G00

1. 读本提交四件、188 合同及 188/00 的本地任务约束、136 固定六件和唯一作者回执、本轮 C09 发布回执、前次 C09 回执 #7095602435。使用原 00、18、24、28、63、79、110、124 号、公约、README、协议 v2 全部十三条；74 号存储分析和 132 的调度类选择仅用作既有定位。核实际来源、作者和可见档位，后端未可认证如实记。[188 原合同](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md) · [74 存储裁定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-review.md) · [132 选择器裁定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-review.md) · [前次 C09 回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095602435)
2. 以本轮正式评论的固定发布提交核 lead，首次启动时应为该提交；exec=95b6af84d8f6、time=a039d9803ade、直接 master=e8e60cd19540。单查四 refs/heads。开放 PR 及原 PR17 顶层评论各无 since 翻至空页，查 #7096187292 两读且完整标记唯一，同一条 C09 评论中本次独审／发布标记各唯一。PR16 Open/Ready、原题 PR17 Open/Draft，均到 master，未合并。核 136 单父 d01efe19ccec、六新增、3,506 行／116,476 字节及固定／活动全文和 Git Blob。按实际读点登记完整预检。首次写前核准自己的完整作者标记、137 新目录、exec expected_sha；若出现已生效的部分操作，按 63 先核后续。[136 清单](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md) · [安全重试](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)
3. MANIFEST 必有 read_channel、startup_selfcheck_quote（直接 master 公约 §1 第 2 条整句）、branch_canary_quotes（两条 time 原行，在 master 反验）、self_check 三个整数及输入路径。清楚标记原会话的既有接收记录、本批追加声明和本批新核证据。在本批范围内使用指导，不追认未知的旧内部操作。

## D00 · 四项追加补正，全部落在 137 新件

| 编号 | 交付到可独立复核的粒度 |
|---|---|
| R136-01 | 对应 136 MANIFEST 129、162–165。新增 136 勘误段写明冻结原件没有的字段，并给出本轮真实重读的 time/master 金丝雀逐行正反结果、直接 master 公约原句、task_local_only 的适用范围及来源。135 原作者 C08c/EXEC-20、136 补正及本次作者 C09a/EXEC-21 分列。137 本身按 P3/P6 登记。 |
| R136-02 | 按 E136-001…038 建紧凑映射，注明旧 ID、原符号/行、处置和新位置。E014 留活动语句，注释另作定位；E022/023 转纯注释定位。D00 子帧、A 两状态、B 四写、C 的回插/选中/返回及摘后自连各链接实际证据。补 options_flags.cmake 的生效选项、CONFIG_BUG 的条件/活动定义及列表函数体的定义开关，限文本条件。新正式窗只用对应定义体内活动依据；独立声明、注释和条件行可独立定位，完整实物按 P6 重计。 |
| R136-03 | 回到 136 四次插入：K/H/Q、J/H/K、J/H/Q、K/H/J，次序为 new/prev/next。逐次列六个无效谓词是否求值、真假，校验函数循环是否进入、返回值及 __list_add_between 外部拒绝分支，再与原四写、正反链、尾 R、count 相连。B 五处 need_resched 改为初值未给、函数无写、后值沿用；C 保留 false。此处只核四组合法输入。 |
| R136-04 | 两载体以真实 ID C136_103 定位，按源比较 used_jiffies >= time_slice，3>=10 为假。两例均给出 K 入队前、入队后、picker 后的队列／计数、K 状态、P 键、last、rq->curr 及返回、求值边界。C1 的 K 后态、外 count 的短路，C2 的 (3>20) 假与 anchor 比较跳过均显式落表。保留原数值、节点来源、条件方向与类型，子实际开始运行／回用户态仍 UNKNOWN。 |

四项均由作者标 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW，引用各冻结 ID／事实／图边位置和新增变更。135 三项已经文档关闭。[四次插入原表](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1011) · [两个原样本](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1205) · [原证据清单](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md#L155) · [列表校验](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L162) · [BUG 配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L66) · [列表定义入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.c#L1) · [BUG_ON 定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bug.h#L66)

## A · 从父任务结构复制到子任务局部覆盖

给定 node=NUMA_NO_NODE、普通用户父 P、子任务存储有效且独立、新栈及普通 fork 的所有必要步骤成功。无并发或额外的字段写入。固定证据从 copy_process 中的 dup_task_struct(current,node)、实际结构赋值到 copy_process 的三项 CPU 时间清零、sched_fork 的调度初始化以及 wake_up_new_task 的状态与入队。对 rt.time_slice、se.vruntime、utime/stime/gtime、nvcsw/nivcsw、__state、on_cpu、rt.run_list 的两个指针分别列所属结构、复制瞬间的来源、后续活动覆盖点和唤醒后的值或条件。结构赋值会复制指针值，别把复制后的 rt.run_list 瞬间写成子节点已自连。初始化后子节点自连，入队再换为相邻节点。限定所读字段、路径和假设，全部系统的写者调查仍开放。[普通复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L273) · [调用方](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L959) · [三个 CPU 时间](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1026) · [新子任务](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L574) · [内嵌调度实体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L60) · [时间片和链节点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_types.h#L42)

## B · 两次 fork 读取的是哪一刻的父值

初始 rq0.curr=P，P 普通、TASK_RUNNING、on_cpu=1，不在给定队列，P.rt.run_list 两指针初值均指向 P 自身节点。队列 H↔Q↔R↔H、count=2。P 的 rt.time_slice=4、vruntime=23、utime/stime/gtime=17/8/2、nvcsw/nivcsw=7/11。先完成 K 的普通成功创建和唤醒，然后作为题设输入把 P.time_slice 改为 9，完成 J 的普通成功创建和唤醒，最后同样将 P.time_slice 改为 12。此处 P.time_slice 为 P.rt.time_slice 的简写。两次改值是给定外部输入，不推导实际系统调用或调度行为。

用同一组列追踪 P、K、J，在各自复制后、覆盖后、入队后与父最后改值后列选定字段。至少说明 K、J 两个独立嵌入标量是否随父一同改变，哪些父值明确归零，哪些子值仍等于复制时的父值；两次入队后给出头、尾、计数和 rq->curr。区分 copied-pointer / self-linked / linked 三种链节点状态。唤醒函数不产生真实 CPU 执行证据。[结构赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L288) · [时钟计数字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L267) · [切换次数字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L283) · [进入队首](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L721)

## C · 同样经过 6 ticks，继承来的两个时间片怎样影响选择与外层记账

C1/C2 各自从独立给定的调度现场开始。C1 指定 current=prev=rq0.curr=K，C2 指定为 J；当前任务普通非 idle、TASK_RUNNING、need_resched=false、vruntime=0、nvcsw/nivcsw=7/11。调用本次 __schedule(SM_NONE)，设执行到选择器时 CPU0 队列 H↔Q↔R↔H/count=2、当前不在队列，Q/R 键 20/40，next 由本次遍历实际到达的 myos_rt 表项给出；函数内每次读 jiffies=106，last_jiffies=100，rq->nr_switches=30。rq clock 与这两个 jiffies 来源分别记账。无其他变动，用 B 中对应孩子继承的切片。C 的当前身份是独立前提，不由 B 的唤醒推出。

两例分别列 used、读取哪个对象的切片及其值、外 OR 各项和 count 的求值状态、摘首／合法回插、摘头后所比较对象、计费前后、last、返回与队列。在 __schedule 的 prev!=next 处再区分 picker 的 rq->curr 写入和 RCU_INIT_POINTER 写点、nr_switches 与 prev 的哪个切换计数递增，另一个计数、父任务和另一个子任务是否被片段写入。给出两例各自是否到达 context_switch 的调用点并止于此边界。固定时间片 4 和 9 相对于同一 6 的两个结果须重新从源码复演；只追本例经过的合法列表节点。无需展开底层实体切换、实际任务恢复或全局写者。[间接调用现场](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L264) · [使用当前任务](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L9) · [回插与计费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L37) · [prev 的来源](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1032) · [switch_count 所指](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1069) · [外层条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1124) · [计数及调用边界](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1153) · [RCU 宏的写](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/rcu_list_macro.h#L479)

## 六件、证据与远端终检

唯一写区为 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/，仅新增 MANIFEST.md、fork-sched-field-inheritance.md、facts-and-boundaries.yaml、fork-sched-field-inheritance-paths.dot、deferred-findings.yaml、evidence.md。原分支 claude/dazzling-cori-q0dnyt，原 Draft PR17。MANIFEST 登记 G00、实际来源／模型、三整数及 P9 逐项结果。报告、机器件、图按同一源的 ID、端点、方向、kind、label 和样本前后态核对；deferred 明记 R136-G04 仅有界回答本次普通 fork 的时间片来源、全局初始化／全部写者仍待证。evidence 有本批自身窄断言和定位索引。阶段 2，完整本地 P9、预处理、编译链接、仓库程序／脚本、Graphviz、QEMU、实机及实际进程、PID、文件、调度、时钟操作均 NOT_RUN。

协议 v2 全部十三条适用。正式锚点覆盖 D00 的活动门、四写及计数、两例 picker 源谓词与末状态，A/B 的结构赋值、CPU 时间覆盖、调度键／子链初始化、状态变化，C 的当前对象、使用切片、摘首、用旧键回插、计费、末尾赋值／返回、外层选计数器和两条分支；另列固定配置依据。定义/注释与活动写点分列，物理引文逐字且每正式窗 1–5 行。P9-1/2/3/4/5/6/7/8/11/13 作连接器可达对照，9/10/12 按本批实物判适用。无引文数量或行数配额。

写前新读 expected_sha，force=false 单父快进。compare 只含六新增，逐件固定／活动全文、行数、UTF-8 字节和 Git Blob 重算。PR17 无 since 顶层评论全分页查重后，发唯一 `MYOS2-LEAD-002-CHAT-EXEC-21 / CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137 / FORMAL_DELIVERY` 作者评论，列真实固定提交、六件准确数字和每个 Blob 前十二位。发评后按实际 ID 双读，全分页到空核唯一，重读四 refs、PR16/17、固定／活动六件，聊天写明实际终检。临时异常按 63 有界恢复。终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，交回原 C09 用 Pro 独审并同轮布置下一批。[110 号沟通约定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124 号模型约定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

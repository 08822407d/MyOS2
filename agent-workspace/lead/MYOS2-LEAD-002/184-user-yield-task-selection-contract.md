---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
followup_id: "CORE-USER-YIELD-TASK-SELECTION-132"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（沿用 Owner 对 C09 正式验收的选择）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "5a22d13876bc"
execution_pin_short12: "2089bfff6f4a"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/183-user-init-shell-launch-yield-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler.h","mykernel/sched/scheduler/scheduler_const.h","mykernel/sched/scheduler/myos_rt.c","mykernel/lib/list/double_list.h","myinitramfs/myinit/initd.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/integration.md"]
status: "PUBLISHED_PENDING_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_display_name: "MYOS2-A-C08c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-20"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/"
allowed_files: ["MANIFEST.md","yield-task-selection.md","facts-and-boundaries.yaml","yield-task-selection-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R130-02","R131-01","R131-02"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 184 号 · 第 132 批 · yield 之后，一次调度选择了谁

**沿用原「MYOS2-A-C08c 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-20，执行档 GPT-6＋Extra。** 131 已核到系统调用体内的 schedule()。本批将该调用与一次普通、给定有效队列上的选择和计数接起来，回答“调用了调度器”之后，什么条件才会进入换任务分支。Owner 转发本固定任务书和 C09 的正式发布评论后，在原会话连续完成 G00、D00、A/B/C、G04。 [131 主审](reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md) · [历史三项裁定](reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-D00-127-130-review.md) · [检查点](checkpoints/2026-11-23-init-shell131-reviewed-yield-selection132.md)

## G00 · 原会话、固定源与唯一写入区

1. 独读本合同、同提交的上述三件、183 合同、C09 本轮正式发布、131 固定六件及作者 #7090247003。复用同一执行对话已完成的接收与指导时给出固定来源和具体范围；核 182/03 与确认评论 #7088796995。工作令、公约、工作区 README、协议 v2、18/24/28/63/79/110/124 的适用约束继续有效。记录当前可见模型身份、Owner 所选档次和实际读取通道，无法认证的后台信息填 UNATTESTED。[PR17 #7090247003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7090247003) · [PR17 #7088796995](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088796995) · [110 号](110-owner-communication-and-delivery-continuity.md) · [124 号](124-model-routing-and-pro-message-budget.md)
2. 直接读四条 refs/heads。原 lead 为包含 184 的 C09 正式提交，原 exec 写前为 2089bfff6f4a，time 为 a039d9803ade，直接 master 为 e8e60cd19540。开放 PR 列表翻至空，PR16 Open/Ready、原题 PR17 Open/Draft，核 head/base/未合并。PR17 顶层评论无 since 到空，131 作者、本轮主审与 184 发布三标记按 ID 核对各唯一。回读 131 唯一父 7e52edee26b4、比较一提交六纯新增 5,221 行、185,935 字节，固定和活动六件逐字比 Git Blob。若原 exec 已前移，先按 63 查已成功的本合同步骤及并行归属。
3. 核 132 新目录及下列全身份正式作者标记的现状。直接读 master 公约 §1 第 2 条，以及固定 time / 直接 master 的 options_flags.cmake 与 panic.c 两处正反金丝雀。源码只使用固定 time。写区限原 exec 的 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/ 和本人一次 PR17 顶层作者评论。发布前检查父、既有路径与远端效果；已成功的步骤保留，未知并行写入不覆盖。[63 号](63-github-write-retry-and-readback.md)

## D00 · 仅追加三项仍开放的文档差量

| 项目 / 冻结选择器 | 第 132 批交付与判据 |
|---|---|
| R130-02 · 131 五个诊断簇的 printf→s0（VARARG_FETCH0），以及各自 s0/s1、rv、request、ignored_write、printf_return | 从 130 旧图合读 131 的有效差量。对每个案例给唯一的旧端点/旧类型及替代、添加关系。画清命令→printf 的调用；printf 建立并传递 va_list，直接调用 vsnprintf；两次 %s 的 va_arg 各归 vsnprintf，其先后按格式消费；printf 直接发起 write。把 rv 的两种用途、write/命令两级弃用和命令返回各自保留。文、facts、DOT 的类型和端点一致。用 25/2 及 argc≠2 控制例复核五组，无须重写旧源码。 |
| R131-01 · E131-044、E131-053 及 facts.D00.R130_01.virtual_130_manifest_correction.inputs_read | E044 的正式替代只含 myos_rt.c:66 表项；定义头另记定位。读取固定 CMAKE_C_FLAGS 相关活动配置项，给独立 P2 短窗支撑 P4 的有限源码断言，区分金丝雀、配置项及程序体。对旧 130 的 44 个标签提取对应 11 种路径，以旧件为依据逐项列出 7 个相合和 4 个遗漏；将缺乏旧证据的 initd.c 从该旧归属移出。区分 130 原样的裸45/正式44、仅改标题44/44、若整合五替换一追加则45/45，以及 131/132 自己的实测。每项注明其适用对象。 |
| R131-02 · facts.B、facts.C / graph 与原 DOT 的 B 缺图、C_YIELD_rq→callback、C_YIELD_preempt→schedule | 从 execve 调用建 filename、args、envs 三根，画八槽、两个末槽空指针、六个目标串及其终止，filename 与 args[0] 汇入同一 prog_name；重算 86/93 和 78/84。对 C 图显式写出系统调用体→间接 yield_task(rq)、rq 实参的来源、给定 myos_rt 的表项→函数、回调→resched_curr、系统调用体对两处抢占操作和 schedule 的各自调用；另画调用现场的先后、条件与 return 0。保留 set/clear、同/异 CPU 边界。A 用 main→normal_boot→fork 两个拥有者消除复合节点的歧义。两机器载体可独立重建。 |

各项给出旧批、文件、选择器、命中数和补后图；B 原缺簇按旧命中 0 和新对象建模。不得以扩大图规模替代调用点或对象的证据。作者暂记 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW。127/128/129 已核历史结论和 R130-01 的裁定沿用，旧制品冻结。[131 原图](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L186) · [130 原引文](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md) · [真实取参点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783)

## A · 从 schedule 到一次 pick

从固定 syscall.c 的 schedule 调用开始。读取 scheduler_core.c 的 schedule、__schedule、pick_next_task 宏，以及 scheduler.h 的 __schedule_loop、__pick_next_task 和 scheduler_const.h 的 SM_NONE。区分 task_is_running 守卫、do/while 的至少一轮和再次迭代条件、prev 被保存的时间、按类遍历的函数指针调用和非空返回。给定所达类为 myos_rt、其选择器提供非空结果，从对应表项进入 pick_next_task_myos；分别解释系统调用的当前类 yield 回调与选任务处的类遍历。实编译、类的实际链接顺序、真实运行次数、锁或抢占效果仍留待其证据。图上区分直接调用、宏别名、间接调用、表项、条件、实参和返回。[schedule](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1201) · [类遍历](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L258) · [调度循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler.h#L281) · [表项](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L67)

## B · 对同一选择门做四次纸面演算

仅限单 CPU、结构有效、普通非 idle 且 TASK_RUNNING 的 current=P=先前 rq->curr，互异的普通候选 Q、R；无并行修改、无计数回绕，读取 jiffies 的值在一例内固定。P.vruntime=10、R.vruntime=30，time_slice=10、last_jiffies=100。表中 need 是进入 pick 时实际读到的给定值。两个 need=false 对照是独立输入，不能说成从 131 置标路径必然得来。

| 案例 | 队首到队尾 | need | jiffies | 要区分的条件 |
|---|---|---:|---:|---|
| B1 | 空 | true | 103 | 请求已设，count 为零 |
| B2 | Q、R | false | 103 | 有候选，时间片未到 |
| B3 | Q、R | true | 103 | 相同时间与队列，改变请求 |
| B4 | Q、R | false | 110 | 刚好到时间片等号边界 |

每例列出 used_jiffies、各门与短路、是否摘首、P 是否及如何回插、队列/计数前后、P.vruntime、last_jiffies、rq->curr 及返回指针；给出每个写入的拥有者。B3/B4 摘 Q 后还有有效普通节点 R，P 取原键与 R 比较时不需遍历。给出 list_header_remove_head 和回插处实际 count 的写点，勿重复计数。仅对这四组合法输入作纸面推导。[02 既有顺序研究](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/MANIFEST.md)、[03 既有综合报告](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/integration.md) 的排序见证、全局策略与候选修改保留原范围。[选择与记账](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L9) · [链表摘首](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L652)

## C · 选择结果如何影响可见计数

对 B1–B4 用同一个给定前态 rq.nr_switches=8、P.nivcsw=5、P.nvcsw=4。沿 __schedule 的 prev 保存、pick 返回、两处清标志、prev!=next 门、nr_switches、rq->curr 的发布、switch_count 指针所指字段、context_switch 调用以及函数尾的 local_irq_enable 列出每例的条件路径。区分 pick 中 rq->curr 赋值与 __schedule 分支里的 RCU_INIT_POINTER；区分 prev 的地址与之后 rq->curr 的值。nvcsw 的另一项赋值在注释中，单列定位。说明同任务与不同任务两组哪个计数改变、为什么，循环底部重测 need_resched 的次数要受给定状态约束。远端报告只核源码调用与给定演算；真实切换成功、回到哪个进程/时刻、子进程是否运行或公平性均 UNKNOWN。[prev 及计数器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1034) · [清标志与分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1116) · [计数与调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1153)

## 六件、证据与停写条件

原执行目录只新增 MANIFEST.md、yield-task-selection.md、facts-and-boundaries.yaml、yield-task-selection-paths.dot、deferred-findings.yaml、evidence.md。报告用通俗中文回答三个问题。机器件含 D00 精确差量、B 四个前后态、C 计数与源码拥有者，DOT 的节点和关系与之对得上。evidence 只计本批新增，deferred 按事实、给定、未知与恢复条件分列。输入限本合同点名的源码函数/配置、131 六件、130 引文与图和已指定历史范围、02 清单与 03 综合报告相应段落。不得运行旧夹具。

本任务适用 agent-workspace/tasks/00-gpt-task-protocol-v2.md 全部十三条。强制锚点为 D00 的 formatter 取参/printf 两个调用及返回、init 三实参、系统调用间接调用和 schedule、myos_rt 两个各自的表项、活动配置；A 中循环、类选择与返回；B 中初值、差值、门、摘首、回插/计数、计费、last_jiffies 与返回；C 中 prev、switch_count、两次清标志、计数、发布及 context_switch。短窗 1–5 行，函数体/表项/活动宏或配置须可归属；定义头、extern、注释作定位。P1–P13 按项申报；P9-1/2/3/4/5/6/7/8/11/13 作能由固定文本核验的检查，9/10/12 按有无代码、补丁或降级记适用性。MANIFEST 必带 inputs_read、read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check 三整数；对全六件核 P6 裸数和新正式账。完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机及真实进程、调度、文件操作均为 NOT_RUN。

G04 在原 exec 使用 expected_sha 与 force=false 单父快进，compare 仅合同六件 A、旧件零改删；固定/活动六件全文、UTF-8 字节及独立 Git Blob 二读。PR17 写前全分页查重，唯一顶层标记为 `MYOS2-LEAD-002-CHAT-EXEC-20 / CORE-USER-YIELD-TASK-SELECTION-132 / FORMAL_DELIVERY`。评论含真实固定提交、结果、补正状态与局限。按 ID 双读、无 since 全页至空、再核四 ref、两原 PR 与六件。暂态依 63 先读远端效果，再按 Retry-After 或约 10/20/40 秒有界恢复并记录每次结果。原 PR 元数据由主线维护。

作者终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING。带回原 C09，使用 Pro 独立验收并同轮安排后继。[79 号](79-work-package-sizing-and-pro-review.md) · [124 号](124-model-routing-and-pro-message-budget.md)

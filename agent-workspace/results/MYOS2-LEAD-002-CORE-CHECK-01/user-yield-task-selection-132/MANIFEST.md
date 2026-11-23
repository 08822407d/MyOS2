---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_executor_manifest
followup_id: CORE-USER-YIELD-TASK-SELECTION-132
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: "GPT-6 self-reported; actual backend model and effort UNATTESTED"
model_per_owner: "GPT-6＋Extra"
actual_backend_model: UNATTESTED
actual_backend_effort: UNATTESTED
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_publication_short12: f07e60a5e8c0
execution_parent_short12: 2089bfff6f4a
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  - "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  - "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 58
  quotes_reconfirmed: 58
  downgraded_to_inferred: 0
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/184-user-yield-task-selection-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-D00-127-130-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-init-shell131-reviewed-yield-selection132.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/183-user-init-shell-launch-yield-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/README.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/integration.md"
  - "mykernel/sched/syscall.c"
  - "mykernel/sched/scheduler/scheduler_core.c"
  - "mykernel/sched/scheduler/scheduler.h"
  - "mykernel/sched/scheduler/scheduler_macro.h"
  - "mykernel/sched/scheduler/scheduler_const.h"
  - "mykernel/sched/scheduler/scheduler_types.h"
  - "mykernel/sched/scheduler/myos_rt.c"
  - "mykernel/lib/list/double_list.h"
  - "myinitramfs/myinit/initd.c"
  - "myinitramfs/mylib/myglibc/stdio/printf.c"
  - "myinitramfs/mylib/myglibc/stdio/vsnprintf.c"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/arch/x86_64/include/asm/unistd_64.h"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
  - "mykernel/sched/sched_api.h"
  - "mykernel/sched/scheduler/scheduler_api.h"
reuse_policy:
  previous_executor_handoff: "REUSED_IN_ORIGINAL_C08c_CONVERSATION_FROM_BATCH130"
  mnemosyne_guidance: "REUSED_WITH_PREVIOUS_APPROVED_FIXED_READING; NOT_A_NEW_B132_READ"
  MyOS2_work_order: "REUSED_PREVIOUS_READINGS_WITH_FRESH_184_C09_AND_CANARIES"
D00:
  R130_02: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R131_01: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R131_02: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R130_01: CLOSED_DOCUMENTATION_ONLY_BY_C09
  R127_02: CLOSED_DOCUMENTATION_ONLY_BY_C09
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
open_questions: []
---
# 第132批 · 六件固定交付MANIFEST

**本批作者：原CHAT-EXEC-20。** Owner通过原对话转发C09的184号固定合同与PR17正式回执#7090769412；此次授权不同于此前已完成的130交接接收，也不授予主线PR16导航/验收写权。项目工作令/约束指导、Mnemosyne已批准行为指导沿用；依新184和C09独裁重新确定已闭合与开放的项目。模型自述GPT-6，任务要求GPT-6＋Extra，服务器端真实型号与档位无可独立认证证据，保持UNATTESTED。

## 六件与读取入口
| 新增文件 | 内容/消费方式 |
|---|---|
| MANIFEST.md | G00、来源、P2/P6/P9、写入许可和终检 |
| yield-task-selection.md | D00三项定点补正 + A/B/C中文解释、4例表格与运行边界 |
| facts-and-boundaries.yaml | JSON兼容YAML；源owner、旧端点选择器、11旧证据路径、execve图对象、4例逐写事件、3项计数和全部有类型图 |
| yield-task-selection-paths.dot | 从同一机器图逐group投影206节点/239有类型边；Graphviz未运行 |
| deferred-findings.yaml | 3项OPEN补正待C09独判、其他运行/链接/循环边界 |
| evidence.md | 新增58条正式短窗、7条独立定位，旧130的44及旧131的48不重计 |

上述文件仅新增在原exec分支的`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/`，必须同一单父提交，不修改旧制品、源代码、lead、任务书、PR正文或PR状态。原Open/Draft PR17保留旧标题；PR16 Open/Ready。

## Fresh G00 基线与重启证据

直接GitHub refs初核：lead f07e60a5e8c0、exec 2089bfff6f4a、固定time a039d9803ade、直接master e8e60cd19540。C09本批合同提交f07e60a5e8c0唯一父5a22d13876bc，四份主线文档纯新增，正式评论#7090769412已经按ID回读；C08→C09签认#7088796995可追溯。131作者#7090247003已经按ID回读。原PR开放列表有2项、后继分页空；PR16 Open/Ready、原PR17 Open/Draft且未合并，base均master。PR17全部顶层无since分页100/100/52/0到空共252，131作者标记及本轮C09发布各唯一，132本人的FORMAL_DELIVERY标记为0；132目录写前404。

父2089bfff6f4a唯一父7e52edee26b4；官方比较领先1/落后0，仅六件纯新增5,221行/185,935字节。本批直接读固定及活动131六件，完整文本、SHA和UTF-8字节全部6/6相同；没有假装对旧131重新实施独立主线验收。两条反向金丝雀固定time为`-m64 -mcmodel=kernel -fno-pie -fno-pic`与`this_cpu = smp_processor_id();`，直接master分别为`-m64 -mcmodel=large -fPIE`与`this_cpu = raw_smp_processor_id();`，未混用master源。直接master公约§1第2条精确记录在头部。

C09本轮最新裁定：R127-02与R130-01均CLOSED_DOCUMENTATION_ONLY，127组合PASS_PENDING_LOCAL；R129-01等历史闭合继续有效。**仍OPEN：R130-02、R131-01、R131-02**；130/131原批RETURN均由主线保持，不能被作者本轮自签关闭。

## D00/A/B/C精确覆盖及机器检查

- R130-02：五组131有效诊断图各3条旧错误端点精确选择器（每条命中1）；各补printf创建va_list、直接调用vsnprintf和write；两次%s的va_arg都由vsnprintf执行；保留vsnprintf rv→write请求和printf返回、write与command返回的两级弃用。五个请求分别37、25、37、35、28，ls给定write只返回2仍printf=25；rmdir/rm argc不等于2则直接-1、不写。
- R131-01：131旧E044正式替代仅myos_rt.c:66表项，类头65作定位。旧E053是time编译金丝雀，不是P4活动配置依据；新增options_flags.cmake:62–65四行有效配置。旧130共44正式窗口去重11路径，131虚拟8路径中7相合、漏4、initd额外1；明确拆分证据路径与其他来源，未证阅读史UNKNOWN。130冻结裸45/正式44、仅更正标题44/44、整合五替换加一追加45/45，旧131原件48/48；本132自己58个新正式窗与7个定位。
- R131-02：新B_EXECVE图具三实参根、8指针槽、2 NULL指针、6数据NUL，同一个prog_name对象，字节按引用86/93、按目标78/84。A图为main→normal_boot→fork；C图区分sys_sched_yield发起的间接yield_task与各自preempt/schedule直接调用、rq数据、myos_rt表项，以及选任务的pick_next_task成员。
- A：schedule→__schedule_loop(SM_NONE)→__schedule保存prev→pick_next_task宏别名→__pick_next_task逐类→myos_rt表项及非空返回，循环至少一轮，类全局顺序/编译效果UNKNOWN。
- B：独立四例分别返回P、P、Q、Q；count=0和未触发门均无摘首/回插/计费；B3/B4 count先2→1，再1→2，P旧键10与R30比较后回插、然后分别加3/10，last变103/110。
- C：四例同前态rq.nr_switches8、P.nivcsw5、P.nvcsw4；仅B3/B4因为prev=P、next=Q而进入context_switch调用分支，分别成为9/6/4，B1/B2保持8/5/4。两次清标志都经过；实际是否发生处理器任务切换与Q的后续need状态UNKNOWN。

图件与facts由相同节点/边数据生成，205以上节点有合法ID、每条边端点有声明，单簇和总图可逐节点比。历史早期02 W01–W08与03综合报告只是范围限定，既不执行旧夹具也不采纳其候选修复为现行策略。

## 十三条协议与执行禁令

- P1：本批真实源对象及短SHA写入，六件扫描无40位十六进制完整SHA或引用泄露；不臆造后端型号。
- P2：全部新增正式源码窗来自固定time的函数体/活动宏/类表项/CMake配置，连续1–5物理行，保持制表符与CRLF，定义头、注释、外部声明及分派项另作定位。
- P3：结构字段`inputs_read`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`齐全；双向基线金丝雀不混。
- P4：不把特定源码片段的缺失扩大为全仓断言；活动配置只证明文本，不证明真实预处理或链接。
- P5–P8：仅内部有证据对象，独立/给定/UNKNOWN分账，保留原件，不自行关闭主线RETURN。
- P9：1/2/3/4/5/6/7/8/11/13以文本和GitHub读回可做的项目逐条自检，9/10/12涉及本地全树、补丁、降级按NOT_RUN或N/A；完整本地P9依旧NOT_RUN。
- P10–P13：无源码补丁/修复、无生成可执行程序、无跨对话替换身份、不修改既有评审或PR；当前回执只由本作者发一次，C09另审。

**严守阶段2：完整本地P9、预处理、编译链接、仓库程序/脚本、Graphviz、QEMU、实机及真实进程、调度、文件操作全部NOT_RUN。** 自建GitHub文本比对及在给定整数上的有限纸面演算不视为实机运行。

write_retry_log（分支写前）：自建事实模型第一个草稿的局部变量名字碰撞、引文生成第一次的注释空白字面匹配不一致，均在更新执行分支前被文本校验拒绝。仅产生了独立未挂载Git Blob及修正文本生成，**没有动原执行ref**，也没有发出132正式作者评论。正式六件提交采用旧父expected_sha/force=false单父快进；若并发先读效果，不覆盖他人写入。提交后按父链、compare恰六个added、零旧改删、固定/活动全文、重算Git Blob、按ID双读唯一评论、无since全评论到空、四refs/两PR终检后STOP_WRITING；交回原C09使用Pro独判并同轮发下批。

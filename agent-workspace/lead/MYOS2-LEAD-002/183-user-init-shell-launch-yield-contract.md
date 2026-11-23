---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（Owner 本条发令）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "bbbe03f75da1"
execution_pin_short12: "7e52edee26b4"
read_channel: "connector"
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
record_type: "phase2_executor_contract"
followup_id: "CORE-USER-INIT-SHELL-LAUNCH-YIELD-131"
status: "PUBLISHED_PENDING_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_display_name: "MYOS2-A-C08c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-20"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/"
allowed_files: ["MANIFEST.md","init-shell-launch-yield.md","facts-and-boundaries.yaml","init-shell-launch-yield-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R127-02","R130-01","R130-02"]
inputs_read: ["myinitramfs/myinit/initd.c","myinitramfs/CMakeLists.txt","myinitramfs/myinit/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/myshell/myshell.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler_core.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/181-user-shell-error-diagnostics-contract.md","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
---

# 183 号 · 第 131 批 · init 怎样启动 shell，另一条分支怎样让出 CPU

继续使用现有「MYOS2-A-C08c 代码分析执行」／CHAT-EXEC-20。Owner 转发本固定合同和 C09 的正式发布评论后，按 G00 → D00 → A/B/C → G04 连续完成。第 130 批已还原用户在 shell 中看到的五条诊断。这一批向前追到启动该 shell 的 init，核清谁调用 execve、传入哪些字符串，以及调用者的另一条分支怎样请求重新调度。约三个相关专题只作一次交付。[130 主审](reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md) · [历史裁定](reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md) · [检查点](checkpoints/2026-11-23-diagnostics130-reviewed-init-shell131.md)

## G00 · 原席位与冻结输入

1. 读本任务书、上述两份裁定和检查点、本轮 PR17 正式发布、181 合同、第 130 批固定六件及作者评论。核 C09 原席位已接管，182/03 与 #7088796995 可追溯；执行仍是原 C08c。复用同对话已读材料时列出 pin、路径及复用范围，按变更重新读取。主线工作令、公约、工作区 README、协议 v2、18/24/28/63/79、110/124 继续适用。此前完成的接收、指导刷新与本批执行授权分别登记；本批沿用原会话。 [130 作者回执 #7088362162](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088362162) · [63 号](63-github-write-retry-and-readback.md) · [110 号](110-owner-communication-and-delivery-continuity.md) · [124 号](124-model-routing-and-pro-message-budget.md)
2. 直接查四个 refs/heads，原 lead 对应本轮 183 发布，exec 写前为 7e52edee26b4，time=a039d9803ade，直接 master=e8e60cd19540。开放 PR 列表翻至空，原 PR16 Open/Ready、原题 PR17 Open/Draft，分别核 head/base 与未合并。PR17 全部顶层评论无 since 翻至空，主线发布与 130 正式作者标记各唯一，按 ID 核正文。核 130 单父 824ce6b5607a、六新增 3,947 行、121,585 字节并按固定/活动双读。对 131 新目录及自己全身份正式标记查重；若已有本合同预期单父六件或评论，核实归属后只续缺步。遇未知竞争，保全已完成部分。
3. 从固定 time 与直接 master 独立回读 options_flags.cmake 与 panic.c 两处金丝雀、直接 master 的 conventions.md §1 第 2 条。源码仅从 time 固定对象读取。执行头部写当前显示身份、模型选项和可见性来源；后台型号与档位无请求级证明时填 UNATTESTED。短 SHA 取真实连接器输出前 12 位。

## D00 · 三个开放项的最小追加

| 编号 | 固定输入与选择器 | 必须交付的差量及演算 |
|---|---|---|
| R127-02 | 128 的 facts 与 DOT 六簇，先套 129 facts 的 D00.R127_02.additional_node_labels 及同批 DOT，再套 130 的选择器。 | 对三组 B 的 iteration/footer 六条给出在 129 后实际生效的旧标签、精确唯一匹配和替代标签。另核三组 B 的 switch，以及 C127_M16_U3 的 switch/goto/body，六个原已纠正值不得回退。两种载体都要按相同顺序回放并逐节点对照。保留三个 do0、已核的 91 边和 43 对来源；一字样本直达循环外 do0。 |
| R130-01 | 固定 130 evidence E003、014–017、031，MANIFEST 头及第 24/34/38 行。协议 P2/3/6/9。 | 按旧 ID 给出活动宏、函数体、数组元素以及 printf 返回的合格替代/追加。重列旧六件裸前缀总数 45 与正式引文 44，说明原声明 44/44/0 的适用范围和失配；给出旧 MANIFEST 缺少的三个结构化字段的定点补记，并为本 131 批独立填真实字段及计数。让逐条源码匹配、定义体归属和支持的语义分别可核。 |
| R130-02 | 五个 ERR 簇的 s1/write/ret/ignore；130 facts.A.cases、B.valid_given_values、B.boundary、C.cases。 | 在 facts 和 DOT 逐例记录调用拥有者、两次取参、格式化长度、写请求、弃用 write 返回、命令返回来源及 run_cmd 弃用。补 rmdir/rm 参数数目门、常量与 err 之别、0 槽字面选择器及两处活动 extern 和一处注释的定位；保留真实 strerror 桥接 UNKNOWN。用 ls 请求 25、返回 2 及一个 argc≠2 的例子区分来源。 |

合读 [130 固定图](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot)、[129 旧标签](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L679)、[128 原图](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L354)。R129-01 已由主线文档闭合；无需重做四个日期案例。所有补正由 131 新目录承载，作者状态填 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW。保持原主线裁定直到 C09 再审。

## A · init 决定谁走哪条分支

从 myinit 配置的 init 目标、myshell 配置的 sh 目标、顶层 boot 安装目的地进入 initd.c 的 main/normal_boot。列三次 freopen 的路径、模式、流和返回值消费，欢迎语句与 normal_boot 的先后。随后列 fork 的调用次数、分支选择及每个 getpid 结果去向。给定 rv=0、正 PID、−1 三种独立返回，各自重建下一步。子分支区分给定成功替换与 execve 返回到调用现场后落到函数结尾，注明非 void 函数的返回值被 main 使用时的边界。父/失败分支逐项核 getpid、循环、yield、重试 fork、wait 和重启子进程的源码情况。实机能否看到 shell 留待运行证据。旧 07/08 的 console 与系统调用基线仅作限定引用。 [init 主体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L9) · [安装位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L46)

## B · 第一次 execve 的具体参数

限定 initd.c:24–35 的普通有效对象。分别画 filename、args、envs 三个实参；列出两张指针表的槽序、每个字符串的原样 ASCII 和含/不含终止符的长度、两个末尾空指针。验证 filename 与 args[0] 指向同一 prog_name 对象。比较 main 传给 normal_boot 的外层 argc/argv 与实际传入 execve 的局部 args/envs。给出字符串内容字节总和、按引用逐次统计与按不同目标对象统计的口径（路径对象只分配一次），指针空槽与字符串结尾分别表示。可将其与成功替换后 sh 源码的 main 入口对照，实际栈地址、动态链接和成功进入 main 均需另证。旧 23/25/69–71 的栈、ELF 与内核 exec 专题按已有边界引用。 [execve 的实际指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L24) · [sh 入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L66)

## C · 一次 sched_yield 请求

限定父/失败分支中的一次循环体。用仓内 x86-64 活动号定义、syscalls_64.h 的 sys_sched_yield 实例以及 syscall_64.c 活动 switch 表明候选入口；用户态 musl 调用到内核号之间留出真实链接边界。在 MYOS_SYSCALL_DEFINE0(sched_yield) 内逐步核调用拥有者、rq 来源、类回调、preempt 相关语句、schedule 与正常返回。把 rq_unlock_irq 注释单列，锁和抢占 API 的实际效果只报告本包有证据的部分。若给定当前调度类为 myos_rt，核表项 → yield_task_fair_myos → resched_curr。分别比较标志已设与未设的同 CPU 样本；远 CPU 分支只作源码界限。先给直接/间接调用与数据边，再用一句话解释能否据此断言一定换到另一个进程、子进程被唤醒或公平性。无需展开上下文切换或真实进度。 [号与分派](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/unistd_64.h#L32) · [系统调用体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L22) · [类回调](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L60) · [重调度标志](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L732)

## 六件与可核收的终点

写区为原 exec 分支下 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/`，只新增头部所列六件。主报告用中文解释三个专题与 D00；facts 为可解析的 YAML 或 JSON，含选择器、指针表与分支样本；DOT 与机器件按真实类型和端点对应；deferred 区分源码事实、条件、缺口及恢复依据；evidence 只计本批新窗；MANIFEST 记输入、旧批差量、覆盖、计数、写入恢复和 P9 按项边界。输入限本合同指名的固定路径/节及完成三个补正所需的前序对应片段。材料超出容量时按准确性保全，禁止以旧数字填补覆盖。

本任务适用 agent-workspace/tasks/00-gpt-task-protocol-v2.md 全部十三条。强制锚点包括三个活动 errno 宏、run_cmd 体内调用、数组 0 槽、printf 请求及返回、两个参数门；init 的三流/欢迎语/主调、fork 守卫、两个 getpid、execve 参数与 yield 循环；本包给定分派的活动号、类表项、回调体、resched_curr 门和正常返回。独立宏、数组初值、配置项分别锚定；声明、函数头、include 与注释另记定位。P9-1/2/3/4/5/6/7/8/11/13 在文本可行范围逐项核；9/10/12 按有无代码件、补丁或降级交付如实记适用性。机器字段须含 inputs_read、read_channel、startup_selfcheck_quote、branch_canary_quotes 和 self_check 三整数。完整本地 P9、预处理、编译链接、Graphviz、仓库程序/脚本、QEMU、实机、真实进程/调度/文件操作均 NOT_RUN。模型仅限自建文本解析与有限纸面演算。

G04 使用一次单父正常快进、expected_sha、force=false；提交后核父链、compare 仅合同六件 A、旧件零改删。六件固定/活动逐字与独立 Git Blob 回读。对原 PR17 顶层评论查重后只发一次 `MYOS2-LEAD-002-CHAT-EXEC-20 / CORE-USER-INIT-SHELL-LAUNCH-YIELD-131 / FORMAL_DELIVERY`，含真实提交、作者状态、可复核成果与局限；按 ID 双读，再无 since 全分页查重，核四 ref、两原 PR、六件。GitHub 暂态异常按 63 号先查真实远端效果，再按 Retry-After 或约 10/20/40 秒有界重试；命中已成功效果只补缺项，记录实际 attempt、head、effect、间隔及回读。未知并行写入不覆盖。

作者终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，交回 C09 用 Pro 独审并同轮安排后继。执行者不维护 PR 元数据。阶段 2 的源码、旧制品、旧评论、既有分支与 PR 边界继续适用。

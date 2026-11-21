---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-INPUT-DISPATCH-119"
record_type: "phase2_executor_contract"
status: "PUBLISHED_PENDING_NEW_RECEIVER"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "2b170b8072ed"
execution_pin_short12: "cb96288abb8b"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118: "RETURN"
R115_03: "CLOSED_DOCUMENTATION_ONLY"
R116_01: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "CLOSED_DOCUMENTATION_ONLY"
R117_02: "OPEN"
R117_03: "CLOSED_DOCUMENTATION_ONLY"
R118_01: "OPEN"
R118_02: "OPEN"
package_id: "MYOS2-C07c-EXEC17-TO-C08a-EXEC18-BATCH119"
package_status: "non_execution_source_transfer_artifact"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"yes","task_local_only":true,"refresh_completed":"pending_executor"}
retired_executor: "MYOS2-LEAD-002-CHAT-EXEC-17"
old_executor_119_write_authority: "NONE"
executor_display_name: "MYOS2-A-C08a 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-18"
receiving_model_tier: "next_tier_default"
receiving_model_tier_reason: "普通有界执行包，Owner向新对话发令时选择 GPT-6＋Extra，C08用Pro独立验收。"
new_receiver_started_at_release: false
execution_status: "NOT_STARTED"
activation_gate: "OWNER_FORWARDS_FIXED_168_01_AND_C08_RECEIPT_TO_NEW_CHAT_THEN_RECEIVE_REFRESH_FRESH_G00"
write_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/"
allowed_new_files: ["MANIFEST.md","shell-input-dispatch.md","facts-and-boundaries.yaml","shell-input-dispatch-paths.dot","deferred-findings.yaml","evidence.md"]
sole_parent_execution_short12: "cb96288abb8b"
author_marker: "MYOS2-LEAD-002-CHAT-EXEC-18 / CORE-USER-SHELL-INPUT-DISPATCH-119 / FORMAL_DELIVERY"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/strcmp.c","mykernel/fs/vfs/d_path.c","mykernel/fs/vfs/open.c","mykernel/scripts/options_flags.cmake"]
---

# 169号 · 第119批 · shell 一行输入如何变成内建命令调用

**交给 MYOS2-A-C08a 代码分析执行（MYOS2-LEAD-002-CHAT-EXEC-18），GPT-6＋Extra。** 新对话由Owner转发固定168/01及C08正式评论后接收。按168/00与01独立完成接收、指引刷新和 fresh G00，然后一次执行 D00、A、B、C、交付与回读。原C07c已退休。C08主线继续用Pro独立验收。[168/01 新对话启动词](168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md) · [168/00 接续包](168-executor-continuation/00-C07c-to-C08a-handoff-package.md) · [118验收](reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md) · [历史五项裁定](reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md)

## G00 · 先核当前现场

独立读取固定发布提交中的本合同、168两件、检查点、两份验收及Owner转来的PR17 C08正式评论；逐项核发布和执行姓名、分支及门槛。直接读 lead、exec、time、master。lead 应等于正式评论中的发布提交，exec 应为 `cb96288abb8b`，time `a039d9803ade`，master `e8e60cd19540`。用 API 实得完整值作比较，入件只写短12。Open PR 清单应恰含 PR16（lead→master，Open/Ready）、PR17（原执行分支→master，原题Open/Draft），均未合并。以固定提交和活动分支回读118六件及 Blob，确认119目录和六个文件全未占用。[118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701) · [118固定六件](https://github.com/08822407d/MyOS2/tree/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118)

无 since 分页读 PR17 全部顶层评论到空页，检查身份、精确标记和正文：167发布 #7057197079、118作者 #7057835701、本次118验收与119发布各一次，119作者标记在首次启动时为零。已有本身份相同交付则只做缺失回读；目标出现未知文件、身份或并发写入时保全现场。独立核 P1–P13、direct master 公约§1第2条、两分支 CMake/panic 金丝雀和 time 活动 C 配置；所有值及实际读取来源记录进 MANIFEST。

## D00 · 三个开放项，各有关闭条件

| 编号 | 在119六件追加的纠正 | C08复核条件 |
|---|---|---|
| R117-02 | 分列117的88窗/89前缀和118的91窗/92前缀，给具体文件、行号和每文件的正式计数；核新119真实最终六件，说明文字不含可误计的前缀。 | 从最终字节独立计数；新引文逐条确认符号、命题和1–5行，P6三数一致可重算。旧账不混入新账。 |
| R118-01 | E051/E052 降为普通位置；main、cd、pwd消费另取活动体；核 lookup_flags、pwd/root、分配/拼接和复制返回的各自锚及 R60-S01 第3项与 S02。补 time 活动 C flags。 | 给旧ID→新ID或普通位置→消费者的实表，同时改正报告替代段、facts、evidence及相关deferred的当前读法；重读固定源和第60批原裁定。 |
| R118-02 | 在新DOT实际画出替代的 A/B/C 相关子图。main、cd_command、内核getcwd指向各自的实际调用；分配大小、pwd/root数据、条件、顺序和返回分别标。 | 提取实际端点与标签，复核原图适用的启动、cd成功/失败/点名、复制/未写、缓存输出路径；给出旧/新节点与路径对应表，旧图错误边在替代图中消除。 |

119只追加纠正，旧文件与作者原评论保留。保留已闭合四项的条件；CAT图注中 separator 的活动调用按 strcat、cat_command 入口按真实处读取。逐条标 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，作者维持三项 `OPEN`。[118详细退回](reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md) · [历史裁定及CAT补记](reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md)

## A · 编辑一行

沿 main 的清零、read_line、独立 LF、parse_command 跟踪。用成功的单字节读取、有效 console_fd 和事先清零的4096字节 buf，普通输入限制在32字节内。逐个列 key、count 前后、写入/抹除的下标、请求给 console_fd 的字节、返回/继续及最后 buf。至少覆盖 `p,x,BS,w,d,LF` 得 pwd、空行、count=0的退格、NUL继续；另用已读 `pwd` 后读到字节4或下一次 read 返回0作停止对照。区分循环条件、体首退格、分支内第二次退格、尾部下划线，LF 命中时没有尾部下划线。区分 LF 的显式 return count、循环条件不满足后函数体无显式返回、main 未使用 read_line 返回值。字节请求与屏幕表现分栏。[主循环和read_line](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L83-L137)

## B · 从 buf 到 argc 和 argv

只按 ASCII 空格分词。在成功分配且短字符串 NUL 后仍有足量已清零空间的模型，列扫描起点、词尾计数、分配表达式 `sizeof(char **) * (*argc + 1)`、清零、每个指针的 buf 下标、写 NUL 的位置及末槽。示例 `  cd  docs  `（两个词从2、6开始），`pwd`、`pwd   `、全空格和一个未知普通词。记录原终止和分隔符是否被重写；剩余空格哪些只是跳过。词指针借用同一 buf、指针向量单独分配；下一轮 main 重用 buf。零字节末槽解释成 NULL 的目标表示及按8字节算容量需写为模型前提。限定具体已读函数内可见的释放证据。[parse_command](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L154-L186)

## C · 查表、派发和错误出口

从 builtincmd_s 和十项活动表到 find_cmd 的线性逐项 strcmp、parse 返回、main 正负索引分支、run_cmd 的函数指针调用。逐例列 argc、argv[0]、比较项数、命中索引、实际直接调用者/绑定目标和请求输出。`cd docs` 比较1次索引0，`pwd` 比较3次索引2，`nope` 比较10次返−1；空行/全空格在分配、查表之前返−1。错误路径由 main 请求一条带LF的消息，和读行后先发的一次 LF 分开。void run_cmd 忽略内建函数返回，`pwd`调用只连接118已核缓存输出。输出按请求、比较按标识符真实大小写；其余内建只列静态表项。[表定义及绑定](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L22-L56) · [主循环、派发及查找](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L94-L151) · [strcmp比较活动体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcmp.c#L29-L44)

## 唯一写区与交付

执行分支 `claude/dazzling-cori-q0dnyt`，既有 Draft PR17，写区 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/`。恰好六件新增：`MANIFEST.md`、`shell-input-dispatch.md`、`facts-and-boundaries.yaml`、`shell-input-dispatch-paths.dot`、`deferred-findings.yaml`、`evidence.md`。单父为启动核实的118执行提交；不改旧件及源码。受理限阶段2有界源码分析；普通源码疑点依18延期，不扩展到硬件输入、长输入或内核修改。

MANIFEST 存真实身份、接收/刷新、固定输入、G00、公约原句、金丝雀、P6及实际write_retry_log。报告 D00/A/B/C 有可逐格复算的案例表；facts 存同ID结构化数据；DOT 覆盖D00替代段及A/B/C实际图；deferred 记录不确定前提、恢复条件及定位；evidence 分开新P2、历史引用、普通定义位置和推论。全部链接用已实读短12固定源；每个新P2在同一活动定义内连续1–5行，逐字、逐命题核实；注释/声明和完整体的否定结论另设普通范围。

文本P9依协议逐项记实：40位串；引文与符号；链接和行界；ID消费；六件字面前缀与MANIFEST三数；外部出处；两对金丝雀；公约和身份；六件格式与写区；实际运行状态；若降级则核依据；强制锚。机器表可解析，模型逐字段复算，DOT提取唯一节点/真实端点/类型和必经路径。检查最终六个待发正文，尤其是说明文字。编译、QEMU/实机、仓库程序、Graphviz及完整本地P9均 `NOT_RUN`。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。优先 Retry-After，否则10/20/40秒。逐级核树/提交/直接ref及只增六件；按固定提交和活动分支回读全文、字节数和 Git Blob。评论前查重，只在原PR17发表一次本人正式交付标记 `MYOS2-LEAD-002-CHAT-EXEC-18 / CORE-USER-SHELL-INPUT-DISPATCH-119 / FORMAL_DELIVERY`。按返回ID双读、全分页核唯一，复核四ref、PR状态和六件。实际回读只能事后报告；中断后先查已写对象，只补缺步。

最后交回C08：`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，三项D00仍待C08，`STOP_WRITING`。不得自行签发下一批或向旧C07c追索记忆。[28-review-and-next-batch-same-turn.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63-github-write-retry-and-readback.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79-work-package-sizing-and-pro-review.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md) · [110-owner-communication-and-delivery-continuity.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124-model-routing-and-pro-message-budget.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

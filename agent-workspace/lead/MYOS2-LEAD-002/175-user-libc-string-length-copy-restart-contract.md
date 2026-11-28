---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-LIBC-STRING-LENGTH-COPY-124"
phase: 2
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
issued_by: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
read_channel: "connector"
base_snapshot: "time"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
execution_parent_short12: "16398b8b96c6"
lead_before_publication_short12: "cb634a5d3e02"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
retired_executor: "MYOS2-LEAD-002-CHAT-EXEC-18"
retired_executor_124_write_authority: "NONE"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_pr: 17
new_receiver_started_at_release: false
owner_decisions_requested_now: []
open_questions: []
record_type: "phase2_reissued_executor_contract"
status: "PUBLISHED_AWAITING_NEW_CHAT"
supersedes: "agent-workspace/lead/MYOS2-LEAD-002/174-user-libc-string-length-copy-contract.md"
supersedes_at_short12: "cb634a5d3e02"
package_id: "MYOS2-C08a-EXEC18-TO-C08b-EXEC19-BATCH124"
receiving_model_tier: "next_tier_default"
execution_status_at_publication: "RESTART_PENDING_NEW_RECEIVER"
previous_attempt: "INTERRUPTED_BY_CONTEXT_EXHAUSTION_OWNER_REPORTED"
visible_124_result_files_on_execution_branch_at_reissue: 0
prior_executor_124_formal_deliveries_at_reissue: 0
batch122_composite: "PASS_PENDING_LOCAL"
batch123: "RETURN"
open_return: "R123-01"
write_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/"
allowed_new_files: ["MANIFEST.md","libc-string-length-copy.md","facts-and-boundaries.yaml","libc-string-length-copy-paths.dot","deferred-findings.yaml","evidence.md"]
formal_author_marker: "MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-STRING-LENGTH-COPY-124 / FORMAL_DELIVERY"
C08_reissue_marker: "MYOS2-LEAD-002 / CORE-USER-LIBC-STRING-LENGTH-COPY-124 / EXECUTOR_RESTART_175_BY_C08"
receiver_package: "agent-workspace/lead/MYOS2-LEAD-002/175-executor-continuation/00-C08a-to-C08b-handoff-package.md"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/174-user-libc-string-length-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-string-compare123-reviewed-length-copy124.md","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"]
receiver_source_inputs: ["myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/myshell/myshell.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/mylib/myglibc/string/strnlen.c","myinitramfs/mylib/myglibc/string/strncpy.c","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/string/memset.c"]
---

# 175号修订任务书 · 在新对话重新执行第124批

**新对话「MYOS2-A-C08b 代码分析执行」是本批唯一执行者，编号 `MYOS2-LEAD-002-CHAT-EXEC-19`。** 第124批任务号 `CORE-USER-LIBC-STRING-LENGTH-COPY-124`。此版完整承接174号的D00、A/B/C、写区与验收条件。Owner转发本合同与换发回执后，C08b即以GPT-6＋Extra接收并连续执行至交付。本合同亦为新会话的启动入口。[174原版](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/174-user-libc-string-length-copy-contract.md) · [PR17 #7073310725](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7073310725)

## 先接收，再独立检查远端

1. 读取同提交的[175换执行人交接包](175-executor-continuation/00-C08a-to-C08b-handoff-package.md)，完成包内新会话所需的项目约束、Mnemosyne接收和局部指导加载。只记录自己实读的固定材料。原C08a的119–123成果按原身份保留，原174中沿用C08a及复用已读记录的入口已经撤销。新执行者无需旧聊天、旧草稿或第二次派令。
2. 读第123批的审查、历史项裁定、检查点、173合同、作者回执和固定六件，取得D00所需的原始case。123为 RETURN，R123-01 OPEN；R122-01只作已关闭的历史项。[123独立审查](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md) · [R122-01裁定](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md) · [123检查点](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-string-compare123-reviewed-length-copy124.md) · [173固定合同](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md) · [PR17 #7067185003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7067185003) · [123固定六件](https://github.com/08822407d/MyOS2/tree/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123)
3. 做fresh G00。四个直接ref用GitHub完整对象核对，持久文件用真实短12。lead取 **175换发回执**指向的新头，执行父 `16398b8b96c6`，time `a039d9803ade`，直接master `e8e60cd19540`。开放列表分页至空，核 PR16 为 `agent/MYOS2-LEAD-002 → master`、Open/Ready，PR17 为 `claude/dazzling-cori-q0dnyt → master`、原题Open/Draft，两者均未合并。核123单父与六件净差异，逐件对固定/活动正文和Git Blob。重取直接master公约及time/master双金丝雀。
4. 读完整执行树及PR17顶层评论（无since，分页到空）。核123作者、原C08第123批审查兼124首发、本轮175换发各自唯一；区分两个发行回执，不复用原发行标记。对124同时按任务号和任何执行身份查正式交付。正常重启起点是执行头仍为 `16398b8b96c6`，124目录、124作者正式回执均为零。234是换发前观测值；自己的新计数照实记录。
5. 若读到已写成的提交、文件或评论，先查提交父、路径、作者身份、正文和Blob。有本身份的完整交付就核验并只补缺少的终检；若为本身份同一提交而缺正式回执，可在全量查重后补发一次。前任或来源未明的124材料、父头竞争、部分写入及任何与上述预期冲突的情况，保留现场，按63调查并向C08报告尚待裁定的写入；防止重复提交、覆盖或代签。仅有旧聊天中断报告不产生额外人工门。[63号安全写入与恢复](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)

为节省新会话上下文，将读取分成接收与G00、D00、A/B/C、交付四段。可在同一响应内部用精简表保存真实路径、行号、case_id、必经条件和待核项，随后组合成最终六件。大文件分段核对；能通过对象比对或计数完成的核验只保留结果与可回查的定位。每段继续下一段，无需Owner中途转发。

## D00 · R123-01 五个条件现场和六个返回现场

只给小型替代图及同 case_id 的机器表。D01/D02、L01/L02/L03 各有自己的函数实例。案例给定函数入口；调用边从函数实例指向真正被调的 strcmp、chdir、名称 printf 或末尾换行 printf；比较的整数用 RETURN_DATA，影响后续语句的判断用 CONTROL；同一函数中的先后用 SEQUENCE。为每例列实际发生的调用及源码行，未调用字段置空。D02由 cd 调 chdir；L01第二次比较及文件名输出均不发生，L02无名字输出，L03由 ls 分别发起两次输出。保留240行须在循环退出后才能执行的条件。[cd 调用者](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L194-L211) · [ls 调用者与循环退出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L231-L242) · [原图现场](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L497-L513)

F01–F06只补末次 strcmp 返回值、find_cmd 的判断和自身返回值，案例入口/所有者分开，不重画38次比较。对每条 CALL 或带 CALL_INSTANCE 的边按真实所有者做正反检查，并从每个案例入口实际检查能到和不能到的调用，防止仅查静态函数子图漏掉实例层。未调用的实例保留空值，禁止路径检查另列起点、目标与实测可达集合，不能拿全图不存在的虚构节点充作排他性证据。D00作者报纠正已提交，R123-01是否关闭交回C08。[find_cmd 正常返回和耗尽返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L144-L152) · [两个有代表性的终点](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L415-L442)

## A · strlen 的首个 NUL 和长度消费者

读默认 STRLEN 宏、完整函数、声明、库和 shell 的CMake。给定已知字宽（选4或8并记录）、普通内存、有效且长度可表示的字符串，首地址及所有实际整字读取都在预先给定的充分可读存储内。逐阶段列首地址对齐情况、按字节推进、整字取值和候选字节确认、首个NUL下标和返回长度。覆盖空字符串、前导逐字节阶段遇NUL、对齐后在完整字中遇NUL，以及至少跳过一个无NUL整字的正常例。区别实际候选表达式中的三个操作数；候选命中后按哪个字节首次为零决定结果。若给整字数值，声明字节序和无符号字宽。[别名与前导扫描](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strlen.c#L21-L46) · [掩码和整字筛选](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strlen.c#L57-L81) · [候选的字节确认](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strlen.c#L81-L104)

用一条已知短 cwd 和参数，把两个长度分别送入 cat_command 的 len、i、i+2，给出两个 strlen 的实参及请求分配的字节数。止于请求值，真实 sh 使用哪个 libc 仍为 UNKNOWN。保留“长度不含首个NUL”的语义，不重做117的路径、文件打开或显示。[cat_command](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L252-L265) · [strlen 声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L402-L405) · [initramfs 构建](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L38-L51) · [sh 目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)

## B · __strnlen 的预算与返回

在相同普通、完整存储模型内阅读完整函数。区分源名 __strnlen、受 STRNLEN 控制的宏改名、包装头的内部声明与公共 strnlen 声明。源码别名与真实二进制符号需分别记证。逐例给 str、maxlen、已知首NUL下标、源码走过的分支、端点与返回值。覆盖maxlen=0（合法指针、零解引用）、首NUL在预算内、恰在预算处和预算后。对同一数组改变maxlen；用适当对齐的普通输入对照 A/B 的筛选表达式和末尾夹取。区分返回上限和源码中一次整字取值的宽度；按实际入口和分支写，不扩大到任意对象。所有给定地址运算及访问在同一足够大的对象内。[别名、端点及零预算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c#L28-L54) · [整字循环门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c#L82-L88) · [候选判定](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c#L116-L130) · [最终夹取](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c#L145-L159) · [内部声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/string.h#L1-L15) · [公共声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L407-L413)

## C · strncpy 的复制区间、补零和返回指针

固定短、小、互不重叠的有效源和目的区域，目的先给非零已知内容并预留容量。对同一源检查 n=0、n小于源长、n等于源长、n大于源长，再给正常空源例，可共用输入。每例列 n、__strnlen(s2,n) 的 size、memset 是否调用及起始/长度、memcpy 的两个指针和长度、两步先后、最终前 n 字节、界外示意字节是否保持、返回指针身份。size==n 未补零；n=0 即使复制长度为零，源语句仍有 memcpy 调用。目的何处有保证的终止零须从各例给定字节得出。无需深入通用 memcpy 宏体系，用固定调用点和函数返回证据标明所采 helper 语义。图中 __strnlen / memset / memcpy 的调用者都是 STRNCPY，辅助函数之间只有数据或先后关系。[strncpy 完整函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncpy.c#L20-L33) · [memcpy 末段与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L54-L60) · [memset 短尾与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L75-L84) · [strncpy 声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L141-L145)

## 六件交付与停止条件

- 执行分支 `claude/dazzling-cori-q0dnyt`，原Draft PR17，新写区 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/`。只新增 `MANIFEST.md`、`libc-string-length-copy.md`、`facts-and-boundaries.yaml`、`libc-string-length-copy-paths.dot`、`deferred-findings.yaml`、`evidence.md`。单父快进一次。正文、facts 与 DOT 使用同一 case_id；返回值、读/写区间、条件和未调用字段可交叉核对。deferred 给最早未知与恢复所需材料。
- 本任务适用协议v2全部 P1–P13。P9第1–5、13项检查短12、证据/路径、行界、P6计数及强制锚。evidence 取真实单一函数/宏定义或活动命令内连续1–5行；强制覆盖D00的源调用/返回者、strlen 字节与整字两门、__strnlen 零预算/候选/夹取、strncpy 三个调用、相关 helper 返回和 time 活动内核 C 配置。头文件声明、include 和库目标来源另列。第6–8项检查引用、公约、分支及读取渠道，第9/10/12按实际交付记适用性，第11项保留未运行状态。MANIFEST列实际模型与身份、G00、指导刷新、双金丝雀、真实输入、P6三数、正确执行父、写前/写后边界及重试。[任务协议](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [活动内核配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L70) · [库目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L1-L21) · [字符串源清单](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/CMakeLists.txt#L1-L3)
- 仅静态文本与普通有效输入纸面核算。完整本地P9、预处理、仓库程序、编译链接、Graphviz、QEMU、实机及真实文件操作均 `NOT_RUN`。预写检查、提交父、compare单提交六件纯新增；expected-head为 `16398b8b96c6` 对应完整对象且 force=false。暂态按63先查远端效果，遵循Retry-After或10/20/40秒安全重试。核固定/活动六件全文、UTF-8长度及独立重算Git Blob，再查重，原PR17只发一次 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-STRING-LENGTH-COPY-124 / FORMAL_DELIVERY`。评论按ID双读，顶层无since分页至空，再核四ref、两PR和六件。作者终态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`、`STOP_WRITING`，交本C08用Pro独立验收。

## 交回位置

正式作者身份只用 `MYOS2-LEAD-002-CHAT-EXEC-19`。按63号记录实际 write_retry_log，通常首次加三次。工具发现和实际写请求分别记账。第124批作者交回**本主线「MYOS2-A-C08 内核分析主线续建」用Pro独立验收**。新对话到 `STOP_WRITING` 为止。

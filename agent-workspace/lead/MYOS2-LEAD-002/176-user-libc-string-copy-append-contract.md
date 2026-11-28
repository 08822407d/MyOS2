---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
status: "PUBLISHED_PENDING_EXECUTOR_START"
phase: 2
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro（后台和effort未独立认证）"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "88968361c2fa"
execution_pin_short12: "00c0228c52b8"
batch123_composite: "PASS_PENDING_LOCAL"
batch124_with_C08_supplement: "PASS_PENDING_LOCAL"
closed_documentation_only_this_round: ["R123-01"]
new_open_returns_this_round: []
followup_id: "CORE-USER-LIBC-STRING-COPY-APPEND-125"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_pr: 17
execution_status_at_publication: "NOT_STARTED"
D00: "NO_OPEN_CORRECTIONS_ASSIGNED"
write_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/"
allowed_new_files: ["MANIFEST.md","libc-string-copy-append.md","facts-and-boundaries.yaml","libc-string-copy-append-paths.dot","deferred-findings.yaml","evidence.md"]
formal_author_marker: "MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-STRING-COPY-APPEND-125 / FORMAL_DELIVERY"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/175-user-libc-string-length-copy-restart-contract.md","agent-workspace/lead/MYOS2-LEAD-002/175-executor-continuation/00-C08a-to-C08b-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/mylib/myglibc/string/strncpy.c","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/myshell/myshell.c","myinitramfs/myshell/CMakeLists.txt","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/string/strcat.c"]
---

# 176号 · 第125批 · 字符串复制、追加与 shell 路径缓冲区

唯一执行者是**现有「MYOS2-A-C08b 代码分析执行」**／CHAT-EXEC-19，使用GPT-6＋Extra。执行父 `00c0228c52b8`，源码 time `a039d9803ade`。此次从124的“长度怎样决定复制量”推进到“终止零写在哪里、哪个指针返回给调用者”。117已核 cat 的成品路径，本批新增各层参数、返回的指针和每一步缓冲区内容。三项共用一套六件。[117既有路径结论](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md) · [shell 实际调用处](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L270)

## 接收和fresh G00

在原C08b接收本固定合同及本轮C08正式发布回执，读同提交的[124独立验收](reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md)、[R123-01裁定](reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-D00-123-review.md)、[本轮检查点](checkpoints/2026-11-22-length-copy124-reviewed-copy-append125.md)，并读175、124作者回执及固定六件。R123-01已按文档关闭，125没有历史返修项。按175已完成的首次接收记录复用同会话确实读过的九份MyOS2约束，复读变化项及63、110、124，局部刷新适用的Mnemosyne指导。MANIFEST说明实际读取的版本、显示名、执行身份、模型及未知项。[175换发合同](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/175-user-libc-string-length-copy-restart-contract.md) · [C08b接收包](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/175-executor-continuation/00-C08a-to-C08b-handoff-package.md) · [PR17 #7073604860](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7073604860) · [PR17 #7073841635](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7073841635) · [124固定六件](https://github.com/08822407d/MyOS2/tree/00c0228c52b8/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124) · [110号](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md) · [28号](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)

- 核完整四个直接ref。lead应为本轮正式发布评论指定头，exec=00c0228c52b8，time=a039d9803ade，直接master=e8e60cd19540。PR16 Open/Ready、PR17原题Open/Draft，原head→master、未合并。核124固定提交的唯一父 `16398b8b96c6`、compare及六件固定/活动正文和Blob；首次启动125新目录应未占用。直接master公约原句、time/master两份分支金丝雀独立回源。
- PR17顶层评论无since分页到空，核175换发、124作者、本次124主线审查及125发布标记各一次；124旧身份作者为零。125首次写入前查所有身份的本批正式交付，自己为零。若已部分或完整发布，按63核提交归属、正文和远端效果，只补缺项。

## A · strcpy 到底复制几个字节，返回哪个地址

阅读 strcpy.c 的默认 STRCPY 名称和完整函数、strlen 的返回边界、memcpy 的原始目的指针返回；核公开声明及构建目标来源。对正常空源、普通短源和**目的起点位于一个较大数组内部**的情况，列源字节（含首个NUL）、源长度、请求复制长度、目的半开写区间、返回指针与剩余字节。用有余量的非零哨兵观察区间前后，明确 strlen(src)+1 的“1”复制哪个字节。比较124同一短串以n等于其长度进行 strncpy 的结果，沿用已核结论。把 STRCPY 对 strlen 与 memcpy 的两个直接调用，以及长度值的数据传递分别标出。[strcpy 默认名及函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L21-L32) · [memcpy 返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L54-L60) · [公开声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L137-L149) · [strncpy 对照](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncpy.c#L27-L33)

## B · strcat 把复制从哪里开始

从 strcat.c 的实参追到 strcpy.c，覆盖“空目的+非空源、非空目的+空源、两串均非空”。非空目的先放已知终止零及可见哨兵。逐例列原目的长度d、源长s、旧NUL的位置、传给内层strcpy的目的指针、写入[d,d+s+1)、新NUL位置、内层返回及外层返回。外层在调用后返回原 dest；当 d>0 时与内层指针的区别有实际数值，当 d=0 时两指针可相同。继续区分外层调用 strlen(dest)、strcpy，和内层调用 strlen(src)、memcpy；只在选择这组仓内定义时统计嵌套调用。仅用有效、已终止、容量充足且源/目的不重叠的对象。[strcat 函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcat.c#L20-L32) · [内层复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L27-L32) · [声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L137-L149)

## C · cat_command 的分配空间怎样逐步变成 open 的实参

取三例有效输入：根目录 / 配 readme，普通目录 /bin 配 hi，/bin 配 /hi。给定 argc=2、有效指针、长度在 int 的可表示范围内、分配成功。用可读的普通短字符串；凡追到仓内整字读取，都给定适当对齐及足够的同一对象存储。对 malloc 得到的 filename 只在所请求的容量内推导；源侧示意存储与目的侧容量分别记录。案例用已知输入及源码请求计算，全部为静态纸面分析。[两个长度、分配与清零](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L252-L264) · [分隔符、追加与打开请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L264-L270)

1. 逐例推导两次显式 strlen、len、i、i+2，画出清零后、第一次 strcpy 后、可选斜杠写入后以及 strcat 返回后的目的字节表。记录每步首个NUL下标、最终长度及容量内剩余字节。len>1 分支会覆盖旧终止零；指出下一次寻找目的尾端时，哪一个已清零的字节提供终止零。根目录例说明为何跳过该分支。
2. 将 shell 源码的直接调用、仓内函数之间的嵌套调用、指针与长度数据各自对应到真实拥有者。同一个 strlen 若出现在不同调用层，账上分开。用至少一个案例分别列外层两次显式求长以及两层库辅助调用的次数、实参、具体返回和使用者；不得把内部调用叠加成 shell 的直接调用次数。
3. 止于270行 open 的实参请求，明确来自 argv 的前导斜杠怎样进入最终缓冲区。实际文件名解析、打开结果、实际链接的 libc 和真实输出仍未知。对照117只需说明新增的字节和指针证据，无需重复既有文件读取及显示。[实际open调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L264-L271) · [用户库构建](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L1-L21) · [initramfs 工具及目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L38-L51) · [sh 目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)

## 六件、证据与停止条件

- 原执行分支 `claude/dazzling-cori-q0dnyt`、原Draft PR17。仅在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/` 新增 `MANIFEST.md`、`libc-string-copy-append.md`、`facts-and-boundaries.yaml`、`libc-string-copy-append-paths.dot`、`deferred-findings.yaml`、`evidence.md`。正文、facts、DOT显式使用相同case_id，逐案核输入字节、调用者和返回指针；未知项单列。用同一组输入跨专题对照，保留有信息量的中间状态，避免复制整份前批表。
- 本任务适用协议v2全部 P1–P13。P9第1–5、13项查源短12、定义内连续1–5行、行界、输入路径、正式标签/P6及强制锚。强制锚为两个函数的真实调用和返回、memcpy返回原始目的地址、cat的两处长度/清零/复制/条件写斜杠/追加/open请求及time活动内核C配置；头文件声明、用户库构建来源另列普通定位。第6–8项核外部引文、公约、金丝雀、读取通道，第9/10/12项按交付类型判适用性，第11项如实记未运行。MANIFEST列G00、真实输入、P6、指导刷新、写前/写后证据和实际重试。完整本地P9、预处理、仓库程序、编译链接、Graphviz、QEMU、实机及真实文件操作均 `NOT_RUN`。[协议v2](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [内核C配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L70)
- GitHub暂态异常先查远端是否已写成，再按63的Retry-After或10/20/40秒安全重试，通常首次加三次。保持expected-head及force=false，记录实际请求及回读；权限、工具发现与竞争写入分别处理。唯一执行父为 `00c0228c52b8` 对应的完整Git对象，核compare仅1提交、六件纯新增。核固定/活动六件正文、UTF-8长度及独立重算Git Blob。原PR17查重后只发一次 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-STRING-COPY-APPEND-125 / FORMAL_DELIVERY`，按ID双读、无since分页至空，再核四ref/两PR和六件。作者终态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`STOP_WRITING`，交现有C08主线用Pro独立验收。

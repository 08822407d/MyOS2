---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
status: "PUBLISHED_PENDING_EXECUTOR_START"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "72abc20e7a70"
execution_pin_short12: "7d0e810ec4ff"
batch120_composite: "PASS_PENDING_LOCAL"
batch121_composite: "PASS_PENDING_LOCAL"
batch122: "RETURN"
closed_documentation_only_this_round: ["R120-02","R121-01","R121-02"]
open_returns_this_round: ["R122-01"]
followup_id: "CORE-USER-LIBC-STRING-COMPARE-123"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-18"
executor_display_name: "MYOS2-A-C08a 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_pr: 17
execution_status_at_publication: "NOT_STARTED"
write_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/"
allowed_new_files: ["MANIFEST.md","libc-string-compare.md","facts-and-boundaries.yaml","libc-string-compare-paths.dot","deferred-findings.yaml","evidence.md"]
formal_author_marker: "MYOS2-LEAD-002-CHAT-EXEC-18 / CORE-USER-LIBC-STRING-COMPARE-123 / FORMAL_DELIVERY"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md","agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/evidence.md","myinitramfs/mylib/myglibc/string/strcmp.c","myinitramfs/mylib/myglibc/string/strncmp.c","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","mykernel/scripts/options_flags.cmake"]
---

# 173号 · 第123批 · shell怎样用字符串比较识别命令和目录项

**唯一执行人**为原「MYOS2-A-C08a 代码分析执行」/ CHAT-EXEC-18，使用GPT-6＋Extra。固定time `a039d9803ade`，执行父 `7d0e810ec4ff`。前批区分了两套库头与最终sh的链接来源；本批保留这一证据边界，转向shell实际多处请求的字符串比较。此前119已核命令表、116已核ls、118已核cd。本次增加逐字节的返回值、受限比较器的读数，以及调用者怎样使用结果，三项共用一套六件。[119命令表](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md) · [118目录处理](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md) · [116目录项显示](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md)

## 接收与fresh G00

Owner转发本轮C08正式回执及本固定合同后，在原C08a启动123。读同一提交的[122独立验收](reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md)、[三项历史裁定](reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md)、[检查点](checkpoints/2026-11-22-open-flag-abi122-reviewed-string-compare123.md)，读172、122固定六件和作者评论；按168身份及同会话实读记录复用九项项目约束，复读有变动的约束及63/110/124，按任务局部刷新适用的Mnemosyne指导。MANIFEST记录真实身份、来源、接收和刷新。[168执行身份](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md) · [172固定合同](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md) · [PR17 #7066063910](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7066063910) · [PR17 #7066384317](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7066384317) · [122固定六件](https://github.com/08822407d/MyOS2/tree/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122)

独立读取四条直接ref：lead为本轮发布评论给定的新头，exec为本合同执行父，time为上列固定头，直接master为 `e8e60cd19540`。核完整对象，持久成果写实际短12。核PR16 Open/Ready、PR17原题Open/Draft，原head→master、均未合并。逐一核122固定/活动六件正文和Git Blob；首次启动时123目录与六件应未占用，PR17顶层评论无since分页至空，核122发布、122作者、本轮审查/123发布各一次和123作者标记为零。直接master公约原句和两分支金丝雀回源。若恢复到部分写成状态，按63号查明来源、核已完成部分后只补缺项。

## D00 · R122-01的唯一补正

用小型替代图及机器表给出“实际调用者、调用点、被调用者、边类型”，另列同函数内的先后次序及参数数据来源。不重画122的217节点、也不重算八个how。

1. do_sys_open分别调用build_open_how和do_sys_openat2；how返回及取址传递另列。从build_open_how出发不能有到do_sys_openat2的DIRECT_CALL。
2. do_creatat/do_mkdirat通过i_op间接调用对应方法，随后在本体调用done_path_create和putname。成功案例的回调、done和putname之间用顺序边。保留方法缺失案例到内建0且不能到回调或清理的条件；有效i_op与缺某个方法分开。
3. rm_command/rmdir_command各自的strerror与printf、字符串结果与路径参数分别注明。C121模型入口到包装或直接内层采用模型边；C121-01负fd的三层返回按源码列全，C121-04/05按C08裁定注明未参与层。引用B/C原始整数时，按122审查中的低两位访问模式补记读取。

核真实端点、类型和调用者。至少机械查“应有”及“禁止”的调用对，报告结果与追加件比对；不要只以一般可达性代替直接调用核查。R120-02、R121-01、R121-02均已文档关闭。D00只报R122-01纠正已提交，C08决定关闭。[open调度](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/open.c#L266-L270) · [创建分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L916-L929) · [建目录分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L983-L996) · [两个命令及参数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L349-L375) · [原图中的调用标签](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot#L421-L429)

## A · strcmp怎样决定相等、大小与停止位置

读string/strcmp.c的默认STRCMP别名及完整函数，分别标出输入char指针、转为unsigned char指针、每轮读入的两个字节、指针递增、首串NUL和不同字节的退出条件，给出本实现真正返回的整数。对照只使用返回值符号/等零的上层代码。使用合法的已知NUL终止字符串，例如同名、首字节相异、一个是另一个的前缀及一个含高位字节的短字节数组；输出表列实际读取的字节对和首次决定结果的下标。高位例按给定字节表示，不把它当成Unicode排序。区分默认宏、函数定义、外部声明、目标中源文件及真实sh链接是否已证。[strcmp别名与函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcmp.c#L20-L44) · [声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L155-L162) · [包装头](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/string.h#L60-L64) · [库目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L1-L21) · [字符串源清单](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/CMakeLists.txt#L1-L3) · [shell的头](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L1-L9)

## B · strncmp怎样用完比较预算

读string/strncmp.c完整函数。区分n<4的余数循环、n>=4的n4=n>>2与四次依次检验、整组完成后的n&=3，以及命中NUL或首个差异时的立即返回。记录给定n、实际读多少对、最后比较位置、停止原因、返回值。用同一对合法字符串对照n=0、n刚好止于差异前、n包含差异；用n=4和5对照完整组和尾部，再加一个在四次检查中提前停止的短串。可令一组输入兼任多例。n=0用有效指针，观察无解引用的返回；所有模型都在有效对象内，按真实语句说明，不运行程序。比较A的完整相等和B仅前n字节相同：两者为零可以回答不同的问题。[strncmp完整定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncmp.c#L31-L71)

## C · 三个shell调用现场如何消费返回值

| 所在函数 | 有界现场 | 结果字段 |
|---|---|---|
| find_cmd | 在已有十项表中比较一个真实全名、一个只是某条目开头的短词，另含“rm”和“rmdir”共享开头的一例。用现有表序，不重做分词。 | 每次比较的表项和零/非零，实际次数，索引或−1。把函数调用次数与每个比较器的字节对数分开。 |
| cd_command | 给定argc=2、有效path，比较“.”与“./”两例。 | 实际实参顺序、返回符号、是否到chdir调用点；在调用请求处停止，不假定目录改变。 |
| ls_command | 假设已取得名字可读且NUL终止的一条记录，分别给“.”、“..”和“.config”。 | 第一次strcmp的结果、第二次是否实际调用及其结果，C的短路顺序和每项文件名printf请求次数。末尾换行单列，不当成一次名字输出。 |

真实shell在这些位置使用strcmp。B的strncmp只是独立的仓内函数对照；不要替换调用者的比较规则。实际sh所用libc为UNKNOWN；涉及具体差值时写明选用仓内函数的条件，shell等零与短路结论可另依调用源码表达。图将case入口、条件、直接调用和返回值流分别标注；第二次strcmp未发生时不能填写已算出的返回。[十项命令表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L56) · [查找函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L144-L152) · [cd点目录](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188-L211) · [ls筛选和换行](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L231-L242) · [sh目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)

## 交付和验收

- 仅在原执行分支 `claude/dazzling-cori-q0dnyt`、原Draft PR17向 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/` 新增 `MANIFEST.md`、`libc-string-compare.md`、`facts-and-boundaries.yaml`、`libc-string-compare-paths.dot`、`deferred-findings.yaml`、`evidence.md`。共六件，单父快进。
- 适用P1–P13。MANIFEST记录G00/实际模型/局部刷新/公约与双金丝雀/来源短12/P6/写入重试；正文、facts、图用相同case_id标出输入、字节数、出口、返回及最早未知。deferred只列未证前提与恢复入口。新evidence强制锚包括D00真实调用者和负fd的逐层返回、A两出口、B四连检查与余数/零预算、C三个调用者的判断、time活动内核C配置。定义或活动命令内1–5行逐字取证，普通声明、全局表和include另列。可行P9按最终文本复核路径、前缀数、P6、边类型、未调用字段、表行和图的正反条件。完整本地P9、预处理、仓库程序、编译链接、Graphviz、QEMU、实机和真实文件操作均NOT_RUN。[任务协议](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [28号](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md) · [110号](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)
- expected-head为本合同执行父，force=false；发布前核compare和六件写区。按63先核远端是否已成功，再按Retry-After或10/20/40秒处理瞬态错误。固定/活动六件、字节和Git Blob独立比对；查重后沿原PR17只发一次 `MYOS2-LEAD-002-CHAT-EXEC-18 / CORE-USER-LIBC-STRING-COMPARE-123 / FORMAL_DELIVERY`。按ID双读、顶层无since分页至空、再核四ref、两PR和六件。最后作者报 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、R122-01为`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`、`STOP_WRITING`，交回本C08用Pro独立验收。

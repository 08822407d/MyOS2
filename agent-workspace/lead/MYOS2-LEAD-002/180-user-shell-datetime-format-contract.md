---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "a0687ad53a3b"
execution_pin_short12: "4a8952c6c956"
read_channel: "connector"
followup_id: "CORE-USER-SHELL-DATETIME-FORMAT-129"
status: "PUBLISHED_PENDING_EXECUTOR_START"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
execution_status: "NOT_STARTED"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
execution_parent_short12: "4a8952c6c956"
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/"
allowed_files: ["MANIFEST.md","shell-datetime-format.md","facts-and-boundaries.yaml","shell-datetime-format-paths.dot","deferred-findings.yaml","evidence.md"]
batch127_composite: "RETURN"
batch128_review: "RETURN"
D00: ["R127-02","R128-01"]
closed_documentation_only: ["R127-01"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/179-user-libc-dest-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-D00-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-aligned-word-copy127-reviewed-dest-aligned-word-copy128.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/myshell/myshell.c","myinitramfs/myshell/tests.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","agent-workspace/lead/MYOS2-LEAD-002/22-user-syscall-stdio-contract.md","agent-workspace/lead/MYOS2-LEAD-002/153-user-realtime-readout-abi-contract.md","agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md"]
---

# 180号 · 第129批 · shell启动时怎样生成日期时间行

现有「MYOS2-A-C08b 代码分析执行」／CHAT-EXEC-19，Owner指定GPT-6＋Extra。阶段2。有界跟踪真实调用 `printf("DateTime Now: %s.%06ld\n", buffer, tv.tv_usec)`，将日期字符串、微秒整数、终止符、写请求和两个函数的返回值逐项对应。既有批次已追过内核时间接口、cat的字符串输出和shell提示符。本批增加混合格式的解析、十进制数字生成和整行组装。[shell启动入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L66-L76) · [实际日期时间调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c#L104-L126) · [107时间接口](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/153-user-realtime-readout-abi-contract.md) · [117字符串输出](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md)

## G00 · 身份、输入与远端预检

读取本固定合同、同提交的[128独立裁定](reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-review.md)、[两项历史裁定](reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-D00-127-review.md)、[检查点](checkpoints/2026-11-22-dest-aligned-word-copy128-reviewed-shell-datetime-format129.md)及本轮C08正式发布评论。对照179号、128作者评论与固定六件；127原表/图/facts按D00定位。现有C08b沿用同一会话确已读过的基础约束，复读变化部分、63号全文及110、124号，刷新本任务适用指导。清单分记显示身份、Owner指定模型、后台型号和推理档可认证情况及实际读取条目。[179号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/179-user-libc-dest-aligned-word-copy-contract.md) · [128作者 #7078730513](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078730513) · [179发布](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078167370) · [128清单](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/MANIFEST.md)

1. 直接读取四ref。lead等于本轮C08正式发布评论所指完整头，exec等于 `4a8952c6c956` 的完整对象，time `a039d9803ade`，直接master `e8e60cd19540`。查开放PR至空：原PR16 Open/Ready、原PR17原题Open/Draft，原head至master，均未合并。直接master公约原句、两分支配置与panic金丝雀逐一核。
2. 128唯一父 `77a1e903d9af`。compare一提交、六件纯新增，旧文件改删为零。核固定与活动六件的全文、UTF-8字节及Git Blob。首次启动129新目录未占用；PR17无since分页至空，128作者、128独审与129发布标记各唯一。首次启动前全身份129正式作者标记为零。恢复时核已写成的父链、目录、文件及标记，从真实缺口继续。
3. 本批源码为固定time的 `myinitramfs/myshell/myshell.c` 第66–76行、`myinitramfs/myshell/tests.c` 第104–127行、`myinitramfs/mylib/myglibc/stdio/printf.c` 第9–26行、`myinitramfs/mylib/myglibc/stdio/vsnprintf.c` 的flags/ranks、EMIT、format_int、vsnprintf中实际涉及的状态与收尾。D00另读179中列出的wordcopy、memcopy头、MEMCPY相关定义与两份金丝雀。仅定位所需文件/节。[协议v2](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [110号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md) · [18号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md) · [24号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md) · [28号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)

## D00 · 只追加两项差量

**R127-02。** 替代目标为128的DOT中A/B/C127簇，以及同件facts的D00.R127_02.graph。对43条DATA_REGISTER逐条以原127对应WRITE.from_reg、源READ及固定源码三处核寄存变量；给目标写节点和数据边都补上同一真实变量。可用“完整case ID＋读写序号＋端点＋变量”的紧凑映射复原两件。20对a0、23对a1；最终生成的历史活动标签各自无未展开值，旧引述另算。A入口与8字展开中已正确的顺序保留。修正B三个switch的case语义与循环体摘要，C单字例写明直接到末写，并保留三个C例的实际调用拥有者、宏位置、原始dstpp返回来源。验收按完整ID复建后查1、2、16字与三个C摘要，查剩余86个字段，检查历史facts与DOT一致。[历史寄存来源样例](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L17-L67) · [历史B/C摘要](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L354-L521) · [已核原始来源](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L534-L740) · [原C组边](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L797-L895)

**R128-01。** 目标为128的三个 `C_outer_memcpy.cases[].byte_copy_trace` 和同批三个C128图簇。镜像旧条目及替代行用选择器即可。每次请求只有一次helper调用，整字读/写可用现成子表；同层写宽求和应为16、16、41，写覆盖恰好一次。保留第二例所有真实前导和尾字节。图中每例4个外层宏位置、1个由MEMCPY拥有的直接helper调用。宏之间的先后按顺序标，helper返回void后外层按自己的游标和字节数推进，尾部完成后MEMCPY返回原dstpp。把数据来源与操作顺序落实到不同类型的边；完整端点、守卫及计数核回原件。[旧C例1](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L7650-L7697) · [旧C例3](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L8361-L8455) · [原三例图](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L904-L1085)

两个退回项作者交回状态均为 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`。R127-01已由C08关闭。历史原件冻结。优先交有旧件路径、旧提交、选择器、替代值与独立核对方式的可应用差量，无需复制整个历史图。主报告、机器差量、DOT的消费方式逐件写明。

## A · 从启动调用分清时间数据与格式骨架

核main何时调用test_PrintDateTime、是否接收返回；区分gettimeofday失败时的perror和return 1，与正常路径localtime、strftime、printf、return 0。记录缓冲容量、两个格式字符串各自归属、tv_sec与tv_usec各由哪一层消费。只给定正常localtime结果和strftime成功提供的19字节文本及NUL，固定文本用 `2026-11-22 09:08:07`。这是案例输入，无需推导时区或真实日期。说明调用方对各返回值的消费与已知边界。失败路径只追到perror请求和本函数返回。[main与调用次序](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L66-L76) · [两个格式与参数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c#L104-L126)

## B · 同一格式里的%s和%06ld如何消费参数

按仓内vsnprintf的实际状态跟踪格式指针p、输出字符数o、目标游标q和变参类型。固定格式、已终止字符串、兼容signed long的非负微秒、充足且合法的输出对象为模型前提。列出百分号、0、6、l、d各自被哪个状态处理；在p回退时说明同一格式字符何时重新处理，新的格式项怎样重置flags/rank/width/prec。对照%s取字符串、strlen及复制，与d选base=10、signed long及format_int的调用位置。字面句点与换行分别计数。

取微秒0、7、12345、999999。按实际format_int定义分别列初始有效数字数、零值处理后的数字数、宽度补零数、从右到左每次写入的目的偏移（可用逆序区间）及最终六个ASCII字符。至少一例把填充的零与数字0本身区分开。以十进制字符独立重算结果；所有目标缓冲远大于输出。[状态与宽度](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L557-L628) · [长度修饰及有符号取参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L648-L731) · [%s分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783-L818) · [数字计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L60-L116) · [补零及逆向写数字](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L153-L201)

## C · 将整行接回printf和write

对四个微秒样本列整行的精确字节（或可无损还原的短文本）、五个区间（前缀、时间文本、句点、小数位、换行），再单列终止NUL偏移。归属关系分别追到EMIT、format_int返回sz、外层vsnprintf的q/o、收尾写NUL，以及printf局部4096字节数组。核一次实际write源码位置、fd来源、请求长度、NUL是否在请求内、printf返回和test_PrintDateTime返回。可在7微秒例对照write返回请求全长或一个较小非负值两种给定结果，核上层返回值及实际写入量的证据边界。四个样本都是独立纸面推演。停止于本次用户侧写请求，真实sh链接、时区数据、实际屏显留待独立证据。[EMIT](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L39) · [format_int返回后的更新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L772-L776) · [NUL及返回值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L868-L877) · [printf的write及返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L11-L26)

## 六件、验收及停止

原执行分支 `claude/dazzling-cori-q0dnyt`，原Draft PR17。唯一写区 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/`。仅新增 `MANIFEST.md`、`shell-datetime-format.md`、`facts-and-boundaries.yaml`、`shell-datetime-format-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST记G00、公约、两处分支金丝雀、实读输入、逐件引文计数及写入恢复日志。主报告含D00/A/B/C；facts含可重建差量、输入、字节、状态、事件和返回；DOT给源级调用、宏位置、控制与数据来源；deferred给最早未知及恢复条件；evidence给固定定义内的新增短窗。对齐三件最终序列化文本，同层事件按完整case ID求和并核对，历史替代图和新增图分别统计。

适用协议v2全部P1–P13。强制锚覆盖shell调用/两个结果分支、两个格式字符串、状态重置与回读字符、long修饰及提取、%s取参/串复制、EMIT、零值和数字计数、零填充、逆向写数字、NUL及o返回、printf的write与rv返回、time活动内核C配置。短窗在所标实际函数、宏定义或活动配置中逐字连续1–5物理行；保留原制表符和正文，对含CRLF的文件按原行核对，不拼接远处片段。P9第1–5、13项核长哈希禁用、路径、定义归属、行界、逐件字面计数及锚点；第6–8项核来源、分支及公约；第9/10/12项依交付形态登记，第11项及运行状态据实。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机与真实文件操作为 `NOT_RUN`。

执行父 `4a8952c6c956` 对应的完整Git对象，单父快进，expected-head/force=false。按63号，GitHub暂态异常先核远端效果再间隔重试，通常首次加三次，默认约10/20/40秒，服务给Retry-After时从其要求。已写成时直接回读，未知并行写入按63处置。MANIFEST或正式评论按实际记录工具发现、请求、错误、写前head、已见效果、间隔及回读。核compare一提交六件纯新增，再核固定/活动全文、UTF-8字节及独立Git Blob。PR17无since分页查重后只发布一次 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-SHELL-DATETIME-FORMAT-129 / FORMAL_DELIVERY`，按ID双读；终检全页、四ref、两PR和六件。交回 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`STOP_WRITING`。原C08主线用Pro裁定。

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
model_per_owner: "Pro"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "cbbb416dae74"
batch125_author_short12: "bd1372961eaa"
batch125_mainline: "RETURN"
batch125_D00_at_author_delivery: "NONE"
open_returns: ["R125-01","R125-02"]
followup_id: "CORE-USER-LIBC-MEMORY-COPY-FILL-126"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
execution_status: "NOT_STARTED"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_new_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/"
allowed_files: ["MANIFEST.md","libc-memory-copy-fill.md","facts-and-boundaries.yaml","libc-memory-copy-fill-paths.dot","deferred-findings.yaml","evidence.md"]
execution_parent_short12: "bd1372961eaa"
D00: ["R125-01","R125-02"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strncpy.c","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"]
---

# 177号 · 第126批 · 内存复制、填充与路径缓冲区

**执行者为现有「MYOS2-A-C08b 代码分析执行」／CHAT-EXEC-19，模型选GPT-6＋Extra。** 本轮将125的两处文档补正与三个新专题合在一批。125已算出高层复制请求，本批追到固定库源码的实际循环、游标和写入宽度。用合法、容量充足的普通对象作静态推导。基线time `a039d9803ade`，执行父 `bd1372961eaa`。[上批176号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md) · [125独立验收](reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md) · [本轮检查点](checkpoints/2026-11-22-copy-append125-reviewed-memory-copy-fill126.md) · [PR17 #7074685583](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7074685583)

## G00 · 接收与写前核验

在原C08b接收本固定合同与C08在PR17的正式发布评论。读本次125独立回执、检查点、176、125作者评论和固定六件。沿用同一执行会话实际读过的MyOS2基础约束，独立复读适用的授权和规则，按变化局部刷新Mnemosyne指导。MANIFEST记录身份、可见型号、Owner指定模型、实际未知项和本次读取范围。完整读取63号及当前110、124号续记。[110号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md) · [18号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md) · [24号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md) · [28号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)

1. 核四个直接ref和两PR。lead取本次C08发布评论的固定头，exec `bd1372961eaa`、time `a039d9803ade`、直接master `e8e60cd19540`。PR16 Open/Ready，PR17原题Open/Draft，两条原head各至master、未合并。读开放PR列表与本合同的目标写区，查125父 `00c0228c52b8`、一提交六新增，逐件比对固定/活动文本与Blob。
2. 无since分页PR17顶层评论到空。125作者、125独立审查及126合同发布标记各唯一；首次启动时，本批全部身份正式交付标记应为零，新目录应未占用。动作有远端遗留时先核完整父、路径、正文、Blob及评论身份，仅补缺项。直接master公约和time/master两处金丝雀分别回源。
3. 全程固定原执行分支 `claude/dazzling-cori-q0dnyt`、原Draft PR17及下面六件。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机、真实文件操作继续记 `NOT_RUN`。仅可使用现成的纯文本和有限算术工具；阶段2、原件冻结及单写者规则继续适用。

## D00 · 两项125追加勘误

- **R125-01，证据计数与分类。** 按原始六件的最终字节分别统计协议P6所用的字面前缀、结构完整的引文和普通定位。保存原件35次、其中34个结构化引文的独立证据。给出固定父、相对路径、唯一行/证据ID选择器和更正值，使人可以用原文件重建补正后的阅读版本：改写原MANIFEST第105行说明，移除误命中的字面前缀；E022–029、E032–034的11个用户侧构建窗口按176降为普通定位，保留路径、原文与准确用途。E025只设置工具根目录，E034才设置编译器。21个C函数短窗及2个内核配置窗重新回源。在该替代版本同步清单总数、逐件分布、各处对应的自报及证据说明，预计正式引文为23/23/0；列出所有被替代的旧声明。原件、组合阅读版本、126新六件的计数分别列出。替代视图可在临时文本中重建；正式125文件和旧评论冻结。[原MANIFEST](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md#L34-L37) · [出现额外前缀的说明](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md#L103-L106) · [原证据](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md#L279-L405) · [协议v2 P6](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/tasks/00-gpt-task-protocol-v2.md#L55-L67)
- **R125-02，C125-01斜杠判断后F[1]的来源。** 在可解析的追加表引用原case、阶段和字段。明确“初始化写者／最近写者／本步写集／本步出现的新首零”，并给出原字段的可重建替换。三例分别核，根目录分支没有写入，F[1]最近由第一次strcpy的memcpy写零；另两例F[5]在追加前来自清零。保留正确的最终字节、返回地址及open实参。用同一组case_id对照原正文、facts和图。作者只登记 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，两项暂留OPEN。[C125-01机器件](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml#L476-L495) · [原报告](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md#L73-L96) · [四个相关语句](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L263-L267)

勘误放在126主报告的独立D00节及facts对应记录，列明所替代的125原件路径/字段、保留范围、复算与最早未知；六件内完成。新清单自己按六件最终序列化文本做一次字面扫描。说明文字用自然语言描述前缀，避免示范文本造成额外命中。

## A · memcpy怎样推进两个游标

固定读 `memcpy.c` 的阈值分支、短尾宏、原始目的返回和 `sysdeps/generic/memcopy.h` 的byte、OPSIZ、OP_T_THRES、BYTE_COPY_FWD、WORD_COPY_FWD、PAGE_COPY_FWD_MAYBE。声明“选定仓内通用头、8字节unsigned long、页面复制宏为空”的条件。用两个独立有效数组，非零哨兵包住目的写区；先分析长度0、3、15的短复制，至少一例从目的数组内部开始。列每步读写下标、字节数、两个局部游标结束位置、返回实参。长度15允许用区间表代替15行重复记录。[memcpy阈值及尾部](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L28-L60) · [字节宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L76-L90) · [布局与阈值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L192-L199)

另取阈值上的16字节、两端按8字节对齐。说明是否进入长分支，前导字节数、传给字复制helper的**字数**、调用后外层宏更新的游标及尾字节数。可以把已选helper当作有条件的调用边界并注明正常返回假设。分开记录C函数调用和宏展开，实际机器指令、优化及编译时解析留为未知。[通用字复制宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L119-L131) · [op_t与OPSIZ](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L60-L67)

## B · memset怎样把一个值写满指定区间

以固定 `memset.c` 为准，分清参数c、转换后的单字节、重复字模式、写入宽度和返回的原目的。给定8字节op_t、有足够存储的独立对象。在目标起点对齐时分别取长度0、7、8、9、64，对齐前导另取长度9、起点地址模8为3。选一个非零的int填充值（例如0x123），用其低字节核对字节与整字表示；另外保留cat的零填充。逐例列前导字节数、每次8词的循环次数、单词次数、尾字节数、写区、最后局部游标和函数返回。把总写宽与len相等作为自检；64字节例用整组写区说明即可。[填充值与前导](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L22-L45) · [整组与单词循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L47-L72) · [尾字节与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L75-L84)

## C · 三个cat请求实际触及哪些字节

沿用125三个已核输入、对齐、分配成功及容量9/8/9的前提。对每例按源顺序，把一次memset、两次strcpy所用的两次memcpy和可选的单字节分隔符赋值展开到A/B所选源码分支。给出每次请求的实参、写区和字节数，各次调用内的写宽分类、关键F下标最后写者、生成的open请求路径。区分初始化清零、写入相同值、守卫跳过和追加覆盖。至少对一个普通目录和根目录算出“各步骤实际写宽之和”及“写过的不同目的字节数”，解释两个数字的含义。终点仍为 `open(F,O_RDONLY,0)` 的参数。125的高层路径表作引用，新增表只呈现helper和写入来源。[cat调用顺序](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L270) · [strcpy](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L29-L32) · [strcat](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcat.c#L28-L32)

## 六件、自检与回读

唯一新增目录 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/`。六件为 `MANIFEST.md`、`libc-memory-copy-fill.md`、`facts-and-boundaries.yaml`、`libc-memory-copy-fill-paths.dot`、`deferred-findings.yaml`、`evidence.md`。报告保留D00及三个专题，facts记录可复算的案例和勘误，DOT区分源函数调用、宏选择/展开、读写范围、返回数据及分支。正文、机器表、图复用case_id。deferred列编译选择、真实sh链接、运行条件及超出范围的helper。适用协议v2全部P1–P13。[协议v2](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/tasks/00-gpt-task-protocol-v2.md)

- 强制短锚：memcpy进入阈值及尾宏、原目的返回；BYTE_COPY_FWD的读取、写入、局部游标；阈值和所用字复制/页复制分支；memset的重复字构成、前导、整组、单词、尾及返回；cat清零、第一次复制、可选赋值、追加和open；time活动内核C配置。按单一定义/活动内核配置取1–5行且支持相应断言。用户侧构建、包含链及公开声明列为普通定位。选择相关路径到文件/符号级，避免全仓普查或扩展到无效对象。[内核C配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L70) · [用户库包含路径](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L3-L16) · [sh目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)
- MANIFEST记录G00/输入、分支金丝雀、公约句、证据自检及未知项。P9第1–5、13项检查禁用长哈希、逐字命中和定义、真实路径、行界、六件计数和必需锚；第6–8项核来源和分支；第9/10/12项按交付类型判适用性，第11项保留未运行状态。125勘误的原状、替代视图和126新引文分别核数。
- 写前核预期执行父及空目录，单父快进、六件纯新增，使用expected-head与force=false。按63号先回读不确定写入的远端效果，再按Retry-After或10/20/40秒安全重试，通常首次加三次。记录实际逻辑操作、次数、错误类别、等待/介入工作、重试前head、远端效果和最终回读；工具发现与写请求分别记载。
- 固定提交与活动分支逐件核正文、UTF-8字节数和重算Git Blob；原PR17查重后仅发布一次 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-MEMORY-COPY-FILL-126 / FORMAL_DELIVERY`。按ID双读、顶层无since分页到空，终检四ref、两PR与六件，作者状态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`STOP_WRITING`。Owner交回现有C08主线用Pro作独立裁定。

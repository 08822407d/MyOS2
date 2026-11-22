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
lead_before_publication_short12: "18ab2e5dfb0f"
execution_pin_short12: "345d0f32374c"
batch125_composite: "PASS_PENDING_LOCAL"
batch126_review: "RETURN"
closed_documentation_only: ["R125-01","R125-02"]
open_returns: ["R126-01"]
followup_id: "CORE-USER-LIBC-ALIGNED-WORD-COPY-127"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
execution_status: "NOT_STARTED"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
execution_parent_short12: "345d0f32374c"
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/"
allowed_files: ["MANIFEST.md","libc-aligned-word-copy.md","facts-and-boundaries.yaml","libc-aligned-word-copy-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R126-01"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/myshell/myshell.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/wordcopy.c"]
---

# 178号 · 第127批 · 对齐整字复制如何完成最后一个字

**现有「MYOS2-A-C08b 代码分析执行」／CHAT-EXEC-19，使用GPT-6＋Extra。** 第126批把16字节双对齐复制的两字helper作为正常返回前提。本批从该调用进入仓内 `WORDCOPY_FWD_ALIGNED` 定义，解释不同入口、交替暂存及最后一次写回，并把helper的实际源级操作接回外层memcpy。阶段2，仅做固定源码与合法模型的静态取证。[上批177号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md) · [126独立验收](reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md) · [125两项关闭裁定](reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md) · [检查点](checkpoints/2026-11-22-memory-copy-fill126-reviewed-aligned-word-copy127.md)

## G00 · 接收与预检

接收本固定合同及本次C08在PR17的正式发布回执。核同提交两份独立裁定、检查点、177合同、126作者正式评论与固定六件。沿用同一执行会话已经实际读取的基础约束，复读变更项，完整读取63号及110、124号，按任务范围刷新适用行为指导。MANIFEST分别记显示身份和型号、Owner选择、不可认证的后台/推理档、实际读取的输入。[PR17 #7075488931](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7075488931) · [110号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md) · [18号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md) · [24号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md) · [28号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)

- 四个直接ref：lead取本次正式发布评论指定完整头，exec应为 `345d0f32374c`，time `a039d9803ade`，直接master `e8e60cd19540`。核原PR16 Open/Ready、原PR17原题Open/Draft，原head至master且未合并，开放PR列表分页到空。执行父 `345d0f32374c` 仅有父 `bd1372961eaa`，126固定六件对照活动分支、差异清单、正文和Blob。首次启动时127写区空。重读直接master公约原句、time/master两处金丝雀。
- PR17顶层无since分页至空，核126作者、126独立审查及127合同发布标记各唯一；首次启动前127全部身份的正式作者交付均为零。发现远端可能已经写成，核完整父、归属、正文与Blob，补缺即可。未知竞争变动按63处理。

## D00 · R126-01 只补指定图件与读法

在127主报告的D00节列 `supersedes` 的固定126路径、节点或边选择器、更正值、保留范围；facts给可解析的替代映射，DOT中给对应的历史补正子图或等价可重建视图。原件的184个唯一节点、194次声明、211条边、40条直接调用和12条已画宏边作为历史计数，追加视图另算。无需重写126的13个字节案例。[原图](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot) · [原图自检](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml#L2196-L2208)

1. M16把第40行的前导BYTE位置和第57行尾BYTE分开：两处零轮，中间第45行空PAGE、第51行WORD及2字helper。M00的零轮路径没有实际READ、WRITE或游标递增事件；可保留空区间作数据注释。按每例选到的这几类源级复制宏位置计（不含OPSIZ等类型/常量宏），修正后应能核归属13个位置实例；选择分图时注明分子、分母和过滤范围。
2. 六例memset的返回值均来自入口dstpp。把末游标到返回处改为顺序关系，消除把数值相同误判为来源相同的路径。以完整case_id划分S09与S09-U3，移除误归的十次重复声明。分别核D+3、D+12及零长例的语义。
3. 三个cat案例的清零、首次复制、末次复制九个范围全部用半开区间，逐项核“右端减左端等于写宽”；第二次复制所需长度来自argv[1]。正文“8整字”对应一次8字节整字写，E023引用含下标1–5五次赋值。图的修正用原源码、原facts及本次C08回执作交叉依据。127作者交回时R126-01为 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，由C08决定关闭。R125-01/02已结束。

## 三个源码专题

使用固定 `time` 的 `string/wordcopy.c` 中默认名称及 `WORDCOPY_FWD_ALIGNED` 整个函数，`sysdeps/generic/memcopy.h` 中OP_T_THRES、OPSIZ、字节宏、已选WORD和空PAGE分支，以及 `string/memcpy.c` 的阈值、前导、尾和返回。显式给定通用头、op_t宽8、两个独立的普通有效数组、足够容量和正常返回。可给S、D各384字节、基址8字节对齐，从内部偏移128附近开始，保留足够的前后空间，使案例中的所有游标算术与实际取数地址都能核清。用能区分源字序号的八字节值和可识别的目的哨兵。区分字节计数、op_t计数、局部整数游标与计算出的有效地址。[通用宏调度](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L119-L131) · [helper默认名及入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L24-L43) · [memcpy外层](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L28-L60)

**A · 入口分派与首个有效读。** 给出len的单位、switch(len%8)、入口预取、进入的do标签、游标及剩余len的调整。跟踪0、1、2、7、8、9字六种正常输入；其中0、1字要从条件本身解释直接返回或跳到末次存储，区分helper直接输入和下述真实外层调用模型。对0字说明是否解引用；对1、2字列首读和最终写入，不把整理局部地址的算术算作额外访存。[入口2至7](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L32-L73) · [入口0和1](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L75-L90)

**B · 两个暂存字与循环末尾。** 取2字、8字及16字。用“当前标签、从哪个有效源下标读入a0/a1、写出的值来自哪一次先前读、目的有效下标、len变化”跟踪。2字完整列出；较长案例以一个完整循环加边界表呈现。说明何时回到do8、do0为何在循环外、哪个值在最后一次写回、逻辑读写多少整字。核对每个目的字恰好得到相应的源字，区间外哨兵与源对象保持给定状态。goto是同一函数内的控制流；helper声明返回void。[展开循环与交替寄存变量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L92-L117) · [回边与最终写回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L115-L128)

**C · 接回memcpy的三个调用。** 选择两端从S/D+128起的16字节、两端从S/D+131起的16字节、两端从S/D+128起的129字节。逐例重算前导字节、对齐后的两端、传入helper的字数、helper入口及首末写、外层宏推进字节和余数、尾BYTE实际读写、memcpy返回的原始指针。第二例的起始地址模8均为3，前导后须重判两端对齐；第三例把16字的循环和1字节尾串在同一张账里。分开记录helper内部按值传入的游标和调用者的游标。只统计三个给定正常请求，止于复制完成；真实sh选择、预处理和机器指令仍未知。[前导及分页位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L35-L45) · [WORD与尾部](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L47-L60) · [外层宏的更新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L121-L131)

## 六件、核查与停写

原分支 `claude/dazzling-cori-q0dnyt`，原Draft PR17。唯一新增写区 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/`，六件为 `MANIFEST.md`、`libc-aligned-word-copy.md`、`facts-and-boundaries.yaml`、`libc-aligned-word-copy-paths.dot`、`deferred-findings.yaml`、`evidence.md`。主报告载D00与A/B/C；facts列可复算输入、读写、游标、条件、返回与最早未知；DOT使调用、宏、goto/顺序、数据及实际访存各自有明确定义。以完整ID对照三件；至少核0字无访问、1字末写、2字及长例每个字的来源与次数。deferred列有效宏、链接和执行状态。只读本包所列前向对齐定义，逆向helper和其他实现留在范围外。

适用协议v2全部P1–P13。强制短锚包含helper入口余数、0/1字特殊分支、2字的预取和跳转、选取的配对读写、循环推进/回边、do0末写；以及WORD选择helper/外层更新、memcpy阈值/前导/尾/原指针返回及time活动内核C配置。每个新正式标签绑定一个真实定义内的连续1–5行，普通声明、用户构建和默认名导航分别标明。MANIFEST保留实际G00、读取、P6、约束刷新、写入及回读记录。P9第1–5、13项核禁用长哈希、定义内引文、路径、行界、逐文件字面计数与强制锚；第6–8项核外部来源、公约及分支，第9/10/12项按交付形态核适用性，第11项如实记未运行。历史补正与127新引文分账。完整本地P9、预处理、仓库程序、编译链接、Graphviz、QEMU、实机及真实文件操作保持 `NOT_RUN`；仅做纯文本与有界算术。[协议v2](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [time内核C配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L70)

固定执行父 `345d0f32374c` 对应的完整Git对象，单父快进；用expected-head及force=false，核compare一提交、六件纯新增。遇GitHub暂态异常按63先查远端效果；服务无Retry-After时通常首次加三次，间隔约10/20/40秒。记录实际请求、写前head、错误、已见效果、等待、恢复和回读。固定提交及活动分支逐件比对全文、UTF-8字节、独立重算Git Blob；查重后在原PR17只发一次 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-ALIGNED-WORD-COPY-127 / FORMAL_DELIVERY`。评论按ID双读、无since分页至空，终检四ref、两PR、六件。作者终态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`STOP_WRITING`，交现有C08主线用Pro独立验收。

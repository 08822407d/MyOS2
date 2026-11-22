---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
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
lead_before_publication_short12: "8ad13b4fdfc7"
execution_pin_short12: "77a1e903d9af"
followup_id: "CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128"
status: "PUBLISHED_PENDING_EXECUTOR_START"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
execution_status: "NOT_STARTED"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
execution_parent_short12: "77a1e903d9af"
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/"
allowed_files: ["MANIFEST.md","libc-dest-aligned-word-copy.md","facts-and-boundaries.yaml","libc-dest-aligned-word-copy-paths.dot","deferred-findings.yaml","evidence.md"]
batch126_composite: "PASS_PENDING_LOCAL"
batch127_review: "RETURN"
D00: ["R127-01","R127-02"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-memory-copy-fill126-reviewed-aligned-word-copy127.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/myshell/myshell.c"]
---

# 179号 · 第128批 · 源地址未按整字对齐时，memcpy如何拼出目的字

执行人为现有「MYOS2-A-C08b 代码分析执行」／CHAT-EXEC-19，Owner选择GPT-6＋Extra。阶段2。在同一组合法、有充足前后空间的普通对象上，解释源地址对齐、相邻源字的选取、字节合成，以及外层复制的覆盖范围。先补127两项，再做三个关联专题。[178号前序合同](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md) · [127独立裁定](reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md) · [R126-01关闭裁定](reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-D00-126-review.md) · [检查点](checkpoints/2026-11-22-aligned-word-copy127-reviewed-dest-aligned-word-copy128.md)

## G00 · 接收、查重与固定输入

完整读取本固定合同、本次C08在PR17的正式发布评论及同提交三件主线记录；比对178号、127固定六件及作者评论。现有C08b可沿用同一会话确已读过的基本约束，复读变化内容、63号全文、110及124号，按适用范围刷新行为指导。使用界面所示身份和Owner指定值，后台型号和实际推理档如实记为不可认证。[127作者 #7078015466](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078015466) · [178发布 #7077713021](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7077713021) · [127六件清单](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md)

- 直读四个完整ref。lead对本次正式评论的固定提交，exec对 `77a1e903d9af`，time对 `a039d9803ade`，直接master对 `e8e60cd19540`。两PR均以master为base、未合并；PR16原lead Open/Ready，PR17原执行分支原题 Open/Draft。开放PR逐页至空。
- 确认127唯一父 `345d0f32374c`，旧父至执行头一提交、仅六件新增。对固定和活动六件核全文、字节和Git Blob。首次启动128新写区须空。直接读取master公约原句和time/master两处金丝雀。PR17所有顶层评论无since分页至空，127作者、127独审及128发布标记各一条；首次启动前全部身份128正式交付为零。遇已经形成的写入，按父、归属、正文、完整树及回执复核后从缺口继续。
- 固定源码仅需time的 `myinitramfs/mylib/myglibc/string/wordcopy.c` 第130–214行及127回读所需的32–128行、`myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h` 的字宽、小端MERGE、前导字节宏、WORD分派和空页宏/阈值、`myinitramfs/mylib/myglibc/string/memcpy.c` 的MEMCPY定义，另读固定内核C配置与panic金丝雀。写前记录各项实际读取及状态。[协议v2](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [110号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md) · [18号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md) · [24号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md) · [28号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)

## D00 · 两项定点补正

历史原件固定于 `77a1e903d9af`。主报告D00给旧件路径、案例或选择器、替代字段及保留范围；facts提供机器可重建映射；图件给更正阅读子图或带精确端点和类型的等效变换。只需修正涉及字段及边，可引用未变部分。历史和128新分析的节点、边和P6分别记账。

1. **R127-01。** 覆盖旧主报告第75–81、95–97行的10行。用facts的remainder、entry_prefetch、read_words、written_words及last_write.dst_word重建，区分零字无预读和非零S0→a0/a1。验收时逐格对照源码；替代表中17个未展开值均有真值，六条非零预读和三个末写目的下标完整。整字数值采用小端显示，列明字节序。[旧表](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md#L71-L97) · [机器字段](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L494-L621)
2. **R127-02。** 对六个非零A例重建“switch选支→预读→局部游标和字数调整→源码goto→目标标签”。A00直接返回。可紧凑表示调整与守卫，保留43条源字到对应目标字的数据关系。跨越非访存语句的摘要边注明“下一次所示访问”；16字回边须经过底部更新和len判断，避免平行的无条件旁路。B组三例分别标清入口分派与退出循环后顺序到do0，C组三个函数入口摘要与实际预读、首个goto的先后相符。将旧facts的循环GOTO分类注明为源码do/while回边。至少用1、2、16字逐边对照，列修正后子图统计及端点核查。[入口边](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L473-L507) · [循环与外层摘要](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L742-L827)

两项作者状态均为 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，由C08独立决定是否关闭。R126-01已关闭。

## A · 从两个相邻源字取出八个连续字节

模型选通用头、小端、unsigned long宽64位、OPSIZ=8、OP_T_THRES=16及空PAGE分支，所有地址与整数游标可表示。S、D各有独立的384字节可用存储，基址模8为零，整字访问所需的对齐、存储类型及对象有效性为模型前提。设S[i]=i mod 251，D初始0xCC，每例重置。对 S+129、S+131、S+135 各向 D+128 直接复制一个字。用对象内偏移说明向下取整地址、两次预读、sh_1与sh_2、选中MERGE及输出八字节。分别列出两个完整读取区间、贡献的子区间及目标写区，明确有充足填充空间。将函数、编译宏和实际存储分列。[地址及位移](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L139-L154) · [单字入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L179-L187) · [末次合成](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L205-L214) · [unsigned整字与MERGE](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L60-L74)

## B · 四字展开与末次合成

固定S+131、D+128；分析直接helper输入0、1、2、3、4、5、8个整字。覆盖四种余数、0字条件、1字直达末写及8字回边。表中给预读寄存变量、局部游标与len调整、首个跳转目标、每轮底部剩余字数和do0。对2字与8字逐个目的字列出构成它的两个有效源字、当时的寄存变量、读取和写入偏移；其余可复用相同映射。分别计读几个完整源字、写几个目的字以及复制多少请求字节。区分显式goto、循环回边与循环退出后落到do0。以字节直接切片为第二种校验，核源及目的区间外哨兵。[四种入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L154-L188) · [MERGE操作与最终写回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L190-L214)

## C · 接回三个正常的memcpy请求

以下三元组为源偏移、目的偏移、字节数，分别为 **(129,128,16)、(130,131,16)、(131,128,41)**。先按目的指针算前导，再用推进后的源指针选helper。每例给前导、PAGE、WORD和尾BYTE的源码位置；核对helper的源余数、整字实参、实际读取的整字区间、首末目的字、外层推进和原始dstpp返回。给出每例的连续目标区间，逐字节对照输入，区分请求源区间、已分配源对象、helper的完整源字加载区间。例如第二例前导之后目的游标为136，源游标为135；以这两个不同值各自继续算。只分析上述合法对象、正常调用与给定定义，实际sh链接及有效预处理选型登记为未知。[外层请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L28-L59) · [两个helper的选择与外层更新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L119-L131)

## 发布、自检与停止

原执行分支 `claude/dazzling-cori-q0dnyt`，原Draft PR17；唯一新增目录 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/`。恰六件，按清单顺序为 `MANIFEST.md`、`libc-dest-aligned-word-copy.md`、`facts-and-boundaries.yaml`、`libc-dest-aligned-word-copy-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST记录身份、实读、G00、公约原句、分支金丝雀、逐文件P6及写入恢复；主报告含D00/A/B/C；facts可解析且可重算，DOT限定边类型并逐例与表比对，deferred注明最早未知及恢复条件，evidence给新增短窗。交付前读回最终序列化文本，逐行检查两张替代表和新表，遍历图边并检查遗漏、未展开数值及首次预读顺序。

适用协议v2全部P1–P13。强制锚覆盖对齐与位移、四余数/0或1特例、至少两处不同MERGE配对、底部与do0、选中小端MERGE宏、外层helper分派与推进、前导/尾部/返回和time活动内核C配置。每个新正式标签紧跟真实定义内连续1–5行。P9第1–5、13项核禁用长哈希、引文、路径、行界、各件字面计数及锚点；第6–8项核来源、两处金丝雀与公约；第9/10/12项按交付形态记录；第11项及所有运行声明据实。仅用固定文本与有界算术，完整本地P9、预处理、编译链接、Graphviz、QEMU、实机、真实文件操作均记 `NOT_RUN`。

执行父 `77a1e903d9af`，单父快进，expected-head及force=false，核compare一提交、六件纯新增。63号暂态恢复先查远端效果，服务无Retry-After时通常首次加三次，间隔约10/20/40秒，只补缺项。固定及活动六件逐件核全文、UTF-8字节及独立重算Git Blob。全页查重后在原PR17只发一次正式标记 `MYOS2-LEAD-002-CHAT-EXEC-19 / CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128 / FORMAL_DELIVERY`。按评论ID双读、无since分页到空，终检四ref、两PR及六件。交回状态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`STOP_WRITING`。原C08以Pro独立验收。

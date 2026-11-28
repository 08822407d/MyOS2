---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_executor_contract"
followup_id: "CORE-USER-TASK-COMM-FORK-EXEC-138"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话标识）"
model_per_owner: "Pro（主线独立验收）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "0b6cffc26e2d"
execution_pin_short12: "4ae5e3ea1af0"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-new-task136-reviewed-fork-fields137.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/forkexec/exec.c","mykernel/sched/forkexec/fork.c","mykernel/sched/syscall.c","mykernel/sched/task/task_types.h","mykernel/sched/task/task_const.h","mykernel/sched/task/task.h","mykernel/lib/string/string.c","mykernel/lib/string/string_helpers.c","mykernel/include/linux/lib/string.h","mykernel/scripts/options_flags.cmake","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-COMMIT-GATES-71-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md"]
status: "PUBLISHED_PENDING_EXECUTOR_START"
execution_status: "NOT_STARTED"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
executor_display_name: "MYOS2-A-C09a 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_PR: 17
allowed_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/"
allowed_files: ["MANIFEST.md","task-comm-fork-exec.md","facts-and-boundaries.yaml","task-comm-fork-exec-paths.dot","deferred-findings.yaml","evidence.md"]
D00: ["R136-04","R137-01","R137-02"]
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 190 号合同 · 第 138 批 · fork 与 exec 怎样留下任务名

现有「MYOS2-A-C09a 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-21 使用 GPT-6＋Extra，一轮完成 fresh G00、三项 D00、A/B/C、六件和 G04。发布时 138 尚未启动。137 证明结构赋值会复制嵌入字段；本批跟进常见的 fork、execve 和有界读名，说明 char comm[TASK_COMM_LEN] 的 32 个字节如何复制、何时被改写、读取者得到什么。71 批已把 __set_task_comm 记为 exec 的调用点，并明确保留具体字符串结果；124 批处理的是用户库字符串。这里逐层核对内核拷贝函数、任务内数组及调用者输入。[71 批裁定](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-COMMIT-GATES-71-review.md) · [124 批裁定](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md) · [137 独审](reviews/CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-review.md) · [136 历史裁定](reviews/CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137-D00-136-review.md) · [检查点](checkpoints/2026-11-23-fork-fields137-reviewed-task-comm138.md)

## G00 · 确认本轮授权和远端实物

读本提交四件、189 合同、137 固定六件、#7097528986、#7096722711、本轮 C09 正式裁定与发布回执；沿用 00/18/24/28/63/79/110/124、182/03、公约、README 及协议 v2。直接读取四个 refs/heads、两个原 PR；开放 PR 与原 PR17 顶层评论分别无 since 翻至空页。作者评论按 ID 双读、全文标记唯一；本轮 C09 独审／发布两标记须各自唯一且同在一条评论。首次启动处的 lead 应为本次发令固定提交、exec=4ae5e3ea1af0、time=a039d9803ade、直接 master=e8e60cd19540，PR16 Open/Ready，原题 PR17 Open/Draft，均到 master 且未合并。核 137 唯一父 95b6af84d8f6，固定及活动六件全文、行数、字节和 Git Blob，总计 4,316 行／145,968 字节。原稿、旧评论冻结。写前核新目录、本人 138 完整作者标记、exec expected_sha。已产生的已授权步骤按 63 查实后只续缺项。[189 合同](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md) · [137 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7097528986) · [前次发布](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7096722711)

本批 MANIFEST 写真实启动句、两条 time 金丝雀原文及直接 master 反验、read_channel、P6 三整数、实际输入和适用的 P9 项。沿用现有席位及已接收范围。技术复核使用固定 time 源及明确给定的有效对象。所有场景为互不干扰的静态模型。

## D00 · 冻结 137，新件完成三个明确补正

| 编号 | 独立验收条件 |
|---|---|
| R136-04 余项 | 按 137 facts.D00.R136-04.new_graph_node、facts.graph、DOT 逐个列原自述和真实 ID/kind/label；核实际两边 from/to/kind/label 并明记未匹配的旧类型。新图显式区分 3≥10 假、最后的状态 OR 操作数已求值且假、整个 OR 假、count 未求值，再到返回 P。修订选择器须从新图实物复核，136 两例的三时点、返回 P/K 和 K 的后态沿用已核结果。[旧自述](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L888) · [实际节点](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot#L10) · [实际两边](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance-paths.dot#L101) |
| R137-01 | 对 C1、C2 区分无条件调用、采样、RQ 时钟写入、数值变化和调用者的 flags 写点。以原 rq.clock=1000、local_clock 的三个独立样本 999/1000/1001 演示负、零、正差，不得给原 137 样本补造采样值。核已给 jiffies 106 与 last 100、两种片值和原选择结果不受影响。实录原 facts:1843 及 #7097528986 的过强表述，给出限定版本。[时钟字段](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/facts-and-boundaries.yaml#L1843) · [写点与早返](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L24) |
| R137-02 | 对 137 的 73 个物理窗与 facts 仅列出的 52 个不同 ID 做完整可读清单，标对全部 21 个差项；按实物说明原 evidence 完整性与机器索引的实际覆盖。E048 的成员分取活动行 43 与 46，E049 收窄到 195–196；保留原行内文字，独立注释另作定位。E028/031/035 的通用行为分别绑定“136 C2 摘 K／插 P／在 Q 前”和“137 C1 摘 Q／插 K／在 R 前”，注明 137 C2 跳过。E033 的返回对象赋值、E034 实际 return 分别标注。复算本批自身引文与机器索引，按所声称范围逐个核对。137 作者回执的“提交前一致”作为时点勘误注明，远端一致性根据快进后的读取登记。[原覆盖声明](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/fork-sched-field-inheritance.md#L24) · [结构窗](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L494) · [原对象标题](https://github.com/08822407d/MyOS2/blob/4ae5e3ea1af0/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/evidence.md#L365) |

三项均由作者标 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW。R136-01/02/03 已文档关闭。每个补正注明原批、作者、冻结件位置、旧值或旧选择器、新值及实际证据。主线独立判闭。

## A · 内核 strscpy_pad 到底写了哪些字节

只读内核 string_helpers.c 的 strscpy_pad、string.c 的 strscpy，对应声明和实际选项。按源顺序核 count=0、逐字节读写、遇 NUL 返回、容量耗尽后的末字节改写，以及外层是否走到 memset。正文以 0 为首下标，所有字符串均 ASCII、源 NUL 终止、缓冲区有效且互不重叠；每例独立使用 8 字节输出区，全区预置 0x5A，count 只限制本次访问。列源读下标、内层写范围、是否重写最后字节、外层补零范围、两层返回与 8 字节最终内容。所有调用都从给定源函数体建模，宏／预处理分支只作有界定位。

| 样本 | 源串 | count | 需区分 |
|---|---|---:|---|
| A0 | ab | 0 | 源和目标有无本次访问 |
| A1 | ab | 1 | 首次写入与末字节回写 |
| A2 | ab | 3 | 恰好容纳两个字符和 NUL |
| A3 | ab | 5 | 首个 NUL 及其后独立补的零 |
| A4 | abcdef | 4 | 读到哪个字符、最终保留几个字符 |

[入口及包装返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string_helpers.c#L831) · [零容量门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string.c#L178) · [实际字节循环及截断](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string.c#L213) · [声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/lib/string.h#L31)

## B · 两个子任务各自拥有怎样的 comm 数组

父 P.comm 的 32 字节给定为 ASCII init、NUL、其后 27 个 0x5A。P 保持原值，两份独立子存储均由同一 P 成功复制。B1 取普通 fork 的实参，按聚合初始化查 args.name；B2 对照 kernel_thread 给定 name 指向 ASCII io，并假设到达 copy_process 的同一命名写点。只到该门处理完毕。建立 K（B1）和 J（B2）的字段表，分列结构赋值瞬间、args.name 门判后、原数组与两子数组的存储身份，判断 NUL 后的字节是否随结构复制、是否被命名调用清零。明确 copy_process 的直调与命名 setter 的区别。再对 K 作一次 __get_task_comm(buf,8,K)，buf 初值 8 个 0x5A，给出 8 个结果字节、内层返回值、外层返回对象及任务内原数组的后态。调度、真实线程创建不在本样本内。[32 字节常量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_const.h#L11) · [嵌入任务名](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L326) · [普通 fork 实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L60) · [结构赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L288) · [命名条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L976) · [命名对照入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1538) · [读取器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L820)

## C · exec 给任务写入什么名字，读出时还有多长

三个独立样本均从 B1 的 K.comm 原始 32 字节开始，P、J 无额外写入。C1/C2 给定普通 execve 经 do_execve、AT_FDCWD 到 alloc_bprm，分配与后续必要条件成功，直至同一 K 在 begin_new_exec 到达 __set_task_comm(me,bprm->filename) 并从该 setter 返回。给定到该点 bprm->filename 稳定保留本例字符串，源与目标不重叠。定位 filename->name、bprm->filename、bprm->interp 初始指向；另列当前任务、内嵌 comm 与读者 buf 的对象关系。独立的 argv[0] 给定为 display-name。判断实际传入 setter 的字符串，辨别源码注释与活动参数，限定到达的写点。

| 样本 | 给定文件名或入口 | 后续独立观察 |
|---|---|---|
| C1 | /opt/demo/worker | 对 setter 后的 K 调 __get_task_comm(buf,8,K) |
| C2 | /opt/demo/abcdefghijklmnopqrstuvwxyz012345 | 对 setter 后的 K 调 __get_task_comm(buf,32,K) |
| C3 | do_execveat_common 入口已给 filename=ERR_PTR(-ENOENT) | 定位早返，随后单独以 8 字节读取原 K |

每例列任务名 32 字节的确切字符区、NUL 位置和尾部；可用连续范围压缩。读者缓冲区预置 0x5A。区分 strscpy_pad 的长度或 -E2BIG、void setter、返回原 buf 指针的 getter。给出获取和释放哪个任务的锁及其围住的复制操作；限已读源语句，不推出并发保证、后续完整 exec 成功、真实文件或用户态观察。不得从函数名或注释自行补入 basename 操作。[普通 execve 入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1460) · [AT_FDCWD 传递](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1438) · [文件名赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1117) · [同一初始解释器指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1142) · [me 的来源](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L851) · [重命名实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L979) · [两种复制方向](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L820) · [入口早返](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1297) · [任务锁字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task.h#L93)

## 固定六件与 G04

原分支 claude/dazzling-cori-q0dnyt，原 Draft PR17，唯一写区 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/。限定新增 MANIFEST.md、task-comm-fork-exec.md、facts-and-boundaries.yaml、task-comm-fork-exec-paths.dot、deferred-findings.yaml、evidence.md。MANIFEST 登记 G00、实际输入和 P9 逐项范围；中文报告、机器件及 DOT 给同一案例 ID、实物节点／边、条件是否求值、字节结果与返回值来源；deferred 记实际运行、全局写者等未证边界。evidence 的正式窗为本批定义体内 1–5 行的窄断言。强制覆盖 D00、数组大小／声明、结构复制和命名门、内外层字符串拷贝与补零、两个 exec 成功样本到达的文件名选择／setter 和指定早返，以及 getter 的锁、复制及返回。独立声明／注释／配置条件各按用途定位；样本身份不写成通用 helper 自身的断言。按实际 P6 标记数复算，P9-1/2/3/4/5/6/7/8/11/13 核连接器可达部分，9/10/12 按实物判适用。

原 exec expected_sha、force=false 单父快进，比较仅六新增；快进后再按固定及活动 ref 逐件读全文、计行、计 UTF-8 字节和重算 Blob。发布前无 since 全分页查重，原 PR17 仅一条 `MYOS2-LEAD-002-CHAT-EXEC-21 / CORE-USER-TASK-COMM-FORK-EXEC-138 / FORMAL_DELIVERY` 正式作者评论，列真实 ID 所对应的固定提交与六件准确行／字节／Blob 前十二位。发评后按实际评论 ID 双读、全分页至空页查唯一、重核四 refs、原 PR16/17、固定及活动六件；在聊天如实填实际终检结果。临时失败按 63 查重后有界恢复。阶段 2，完整本地 P9、预处理、编译链接、仓库程序／脚本、Graphviz、QEMU、实机、真实文件、进程、PID、调度、时钟操作均 NOT_RUN。终点 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，交回原 C09 用 Pro 独立验收并同轮发布后继。[63 号恢复](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [110 号沟通](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124 号模型分工](https://github.com/08822407d/MyOS2/blob/0b6cffc26e2d/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMESPEC-ABI-BOUNDARIES-113"
record_type: "bounded_timespec_abi_contract"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
date: "2026-11-20"
date_timezone: "Asia/Singapore"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "92d7b5af084f"
execution_pin_short12: "f909700f17ab"
status: "READY_FOR_OWNER_FORWARDING_NOT_STARTED"
R109_01: "CLOSED_DOCUMENTATION_ONLY"
R109_02: "CLOSED_DOCUMENTATION_ONLY"
batch109_composite: "PASS_PENDING_LOCAL"
R110_01: "CLOSED_DOCUMENTATION_ONLY"
R110_02: "CLOSED_DOCUMENTATION_ONLY"
batch110_composite: "PASS_PENDING_LOCAL"
R111_01: "CLOSED_DOCUMENTATION_ONLY"
R111_02: "OPEN"
batch111: "RETURN"
R112_01: "OPEN"
R112_02: "OPEN"
batch112: "RETURN"
full_local_P9: "NOT_RUN"
runtime: "NOT_RUN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/160-user-ktime-deadline-arithmetic-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-D00-109-110-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-20-timer-arm111-reviewed-ktime112.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","mykernel/time/misc/time_misc.c","mykernel/time/misc/time_misc.h","mykernel/time/misc/time_misc_const.h","mykernel/time/misc/time_misc_types.h","mykernel/time/time_type_declaration.h","mykernel/include/uapi/linux/time_types.h","mykernel/include/uapi/asm-generic/posix_types.h","mykernel/time/syscall.c"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本件C07独立源码短引文"}
executor_display_name: "MYOS2-A-C07c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-17"
mainline_owner: "MYOS2-A-C07"
receiving_model_tier: "next_tier_default"
receiving_model_tier_reason: "已定界的第113批由原执行对话以GPT-6＋Extra完成；C07用Pro独立验收。"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"yes","task_local_only":true,"refresh_completed":"pending"}
execution_status: "NOT_STARTED"
activation_gate: "OWNER_FORWARDS_161_AND_C07_RECEIPT_FRESH_G00"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/"
allowed_files: ["MANIFEST.md","timespec-abi-boundaries.md","facts-and-boundaries.yaml","timespec-abi-boundaries-paths.dot","deferred-findings.yaml","evidence.md"]
mainline_acceptance: "PENDING"
write_retry_rule: "63"
---

# 第113批任务书　用户态时间结构的复制、校验和比较

交给现有「MYOS2-A-C07c 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-17，执行档 GPT-6＋Extra。Owner 转发本161及 C07 的 PR17 正式发布回执后接收。阶段2，读取固定 time 源码，按 D00、A、B、C 交一批。C07 独立验收。

## 接收与 G00

1. 全文读[本轮历史纠正裁定](reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-D00-110-111-review.md)、[112验收](reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-review.md)、[检查点](checkpoints/2026-11-20-ktime112-reviewed-timespec113.md)、Owner 转发的本次 C07 发布回执、[112作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7029831144)及[112固定六件](https://github.com/08822407d/MyOS2/tree/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112)。按需定点取[111冻结原表](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml)及其 evidence。以[159执行者身份与接收入口](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md)复核既有身份和写权，登记项目规则及适用的 Mnemosyne 指导实际读取/刷新情况，已接收的同一对话延续原记录。按[任务协议 P1–P13](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/tasks/00-gpt-task-protocol-v2.md)及 [28号](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)、[63号](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)、[79号](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)、[110号](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)、[124号](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)执行。
2. 分别读取四条直接 heads。lead 应为本161发布回执所列实际单父提交，exec 为 f909700f17ab、time 为 a039d9803ade、direct master 为 e8e60cd19540。开放 PR 仅16/17，均 base master、未合并，PR16 Ready、PR17 原题 Draft。回读112固定与执行活动六件的全文和 Git blob；取 direct master 公约§1第2条原句，重取 time/master 的 options_flags.cmake 与 panic.c 两组金丝雀及固定 time 的有效 C 配置。记录真实来源、对象和12位短值。
3. 看完整执行树及目标目录的六个路径。初次应全部缺席。按 PR17 顶层评论无 since 分页至空，按正文分别核112作者、旧158原发布、159改派及161主线发布标记各一次；按身份枚举113批所有正式作者标记，初次应为零。记录现场页数/总数及正式评论ID。遇中断，核本人已有单父提交、六件、评论与固定/活动一致性，复用已完成步骤。未知头、名称冲突或其他作者同批交付，报具体对象给C07判断写权。
4. 全文较长的旧原表分块记录行键、位置、引文和已读边界；D00定点沿旧资料反查。一个批次使用一套证据索引，一次提交六件。原111/112六件及对应引文、模型、原图和原评论冻结。

## D00　三项 OPEN 的追加纠正

| 项目 | 提交的最小可复核差量 |
|---|---|
| R111-02 | 在111冻结 facts 按“矩阵／实际 id”定位 B08、C07、C08、C10、X06、X07、X08。模型按含 AT111- 前缀的实际 ID 定位。为 M08 的三个 void 包装连 P02/P03/P04，为 M10、M11 各连 P05。M09分开 CPU 位写回 E044 或 KD112-E016、桶号写回 E053 或 KD112-E023；移除误归的 schedule_timeout 引文 E052，点明中间登记路线。保留 0x00100007 与 0x11900007 各自成立的时点。 |
| R112-01 | 逐个列 KD112-E026 的4个、E027的2个、E034的4个有效宏名及独立定义。允许复用已核单宏旧锚，给出可解引用的批号、路径、行界、ID和用途。将选层门、步长与位域编码分开；补 time64_t 类型第10行。保留112的53个标签、53个字面命中、50个单定义合格。在113追加标明属于112的 inputs_read、open_questions、branch_canary_quotes 三个可解析字段，恢复真实读过的输入和两对原句；113清单另填自身现场。 |
| R112-02 | 对C01–C06逐例补两列，给出经 timespec64_to_ktime 和 timespec64_to_ns 回到的值。零分支直接返回。负路列有符号内层、外层 u64 商取负再减一（q=0、1）、存入有符号 tv_sec 的目标约定，并单列 s32* 与 u32* 余数形参。各字段回到对应体内锚（含E051–E053）和普通类型位置。补上A的结构按值入参与两个标量传参。按112验收所列图82、89、102–103、107、112–113、115逐边给原端点、改正端点、边类型和固定出处。分清宏、调用者、实际写入者、读值及限定范围内无调用观察。 |

R110-01、R110-02、R111-01 已由 C07 文档闭合。三项 OPEN 由执行者提交修订，状态留给 C07。可直接使用回执中的逐项定位及已复算值，以固定旧件和源码交叉验证。

## A　用户态时间结构的输入和输出

以 get_timespec64 为主，跟随一次 copy_from_user、保存的 ret、提前返回以及对 ts->tv_sec、ts->tv_nsec 的先后赋值。列出本地 kts 的声明和 sizeof 的操作数。说明第285–287行 compat 掩码的注释状态。把 put_timespec64 的本地两字段初始化和一次 copy_to_user 作为对照，用 clock_gettime 中的真实短路调用点说明何时请求输出。只在普通有效对象和明确目标类型前提下分析字段。把复制函数返回抽象成0和非0，列本层返回值与已执行的赋值；多个字节是否被复制、填零、故障处理或硬件行为留作待证。核对两套结构与别名的字段类型，在需要时列 LP64／可表示域前提。用实际 sizeof(kts) 表示请求字节数，固定布局、字节偏移须另有依据。107的墙钟、时钟分派只引用相交调用点。

[输入复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L273-L293) · [输出复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L304-L313) · [真实调用者](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L33-L50) · [用户ABI结构](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/time_types.h#L7-L10) · [用户秒类型](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/posix_types.h#L94) · [内核结构](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_types.h#L7-L10) · [内核秒类型](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/time_type_declaration.h#L9-L15)

## B　三种有效性判断各接受哪些输入

对 timespec64_valid、timespec64_valid_strict、timespec64_valid_settod 作一张共用真值表。说明负秒门、纳秒的无符号强转与比较，两种附加秒上限及比较是否含等号。由各自宏独立算 KTIME_SEC_MAX、TIME_UPTIME_SEC_MAX、TIME_SETTOD_SEC_MAX，给计算式和秒的单位。用秒上限前一秒／等于／后一秒、纳秒 −1／0／999999999／1000000000 以及 (−1,999999999) 覆盖各门，允许在同一行满足多个条件。列基本判断返回后，后续门是否可达。与A逐体核对调用关系，另列 strict 和 settod 对 valid 的真实调用。文档中的成立域能由冻结配置及整数类型复算。

[三个判断的完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L89-L121) · [纳秒单位](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_const.h#L32) · [标量秒上限](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_const.h#L44-L47) · [预留运行时长与设时时限](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_const.h#L57-L58)

## C　字段相等、排序与规范化

核 timespec64_equal 的两个字段与短路，timespec64_compare 的两条秒门，以及同秒时返回纳秒差转 int 的实际范围。给同秒相差7纳秒的例子，并说明不同秒分支的具体返回值。在每个纳秒字段均规范、字段差和结果可表示的约束下解释符号。定点复核 set_normalized_timespec64 对进位、借位及最终赋值的顺序。用 (0,1000000000) 与 (1,0) 对照原字段比较和规范后比较；另列 (0,−1) 借位后与B的 valid 关系。可引用 add/sub 两个实际直接调用者作图例；103曾分析的一般规范化过程不重复铺开。按 A 的复制、B 的谓词和 C 的比较分别给能成立的命题和前提；只有确有固定调用点才画直接调用边。

[相等和排序](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L44-L63) · [规范化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L22-L41) · [实际调用点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L65-L84)

## 六件、复核与停写

唯一执行写入为原 PR17 分支 `claude/dazzling-cori-q0dnyt` 的 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/`。严格六件为 `MANIFEST.md`、`timespec-abi-boundaries.md`、`facts-and-boundaries.yaml`、`timespec-abi-boundaries-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST 列身份、模型、task_id／track_id／followup_id、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check、write_retry_log 及接收和 G00 实测。报告覆盖 D00/A/B/C；facts 收录旧行键、所用定义、对象/字段/分支、有限模型和成立域；图标清调用、控制、数据及注记；deferred 列前提、首个未证事实及恢复入口；evidence 单列新引文、历史引用和普通定位。

同一新P2标签对应一个定义体内1–5连续原行；宏各自归属，签名、类型声明、预处理位置及整函数无调用检查可单列普通位置。对最终六件按 P1–P13 做对应预检，复算所用样例，遍历新 facts 的全部源码路径、首末整数行、证据ID和消费者，核清新标签数、逐字命中数、单定义合格数、自检三数。每段图边在原源码可定位；历史资料和外部说明如实标明。文档内只用取自真实连接器的12位短值；Git比较与写入用实际完整对象。建议新源码≤16份、定点检索≤8组、目录定位≤4次、新引文≤60、有限模型≤12、候选与缺口各≤8，均为软上限，无最低数。用表合并边界样例。本包限定阶段2源码与有限整数核对，完整本地P9、预处理/编译/链接、运行仓库程序或脚本、Graphviz、QEMU/实机、额外模型/代理及付费服务在范围外。对未执行项登记 NOT_RUN。

发布前和移动 ref 前分别核头。以 f909700f17ab 的实际完整对象为唯一父、原树为基础，建严格六新增的单个提交，以 expected_sha=真实执行父、force=false 正常前移。本人在PR17发一次顶层交付，正文中以下标记恰好一次。

MYOS2-LEAD-002-CHAT-EXEC-17 / CORE-USER-TIMESPEC-ABI-BOUNDARIES-113 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。按10/20/40秒或服务端 Retry-After。清单或正式评论留实际操作、次数、错误、重试前头、效果、间隔与最终回读。无故障记 not_needed。

交回前独立复读单父差异、六新增/旧改删0/0、固定及活动六件全文与 blob、四条直接 heads、两 PR 状态和导航。本人评论按ID两读及顶层无 since 全分页至空，与原正文逐字相等，113全批作者标记合计一次，属于 EXEC-17。回报 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW 和三项 OPEN，随后 STOP_WRITING。交回「MYOS2-A-C07 内核分析主线续建」用 Pro 独立验收并依28号安排后继。

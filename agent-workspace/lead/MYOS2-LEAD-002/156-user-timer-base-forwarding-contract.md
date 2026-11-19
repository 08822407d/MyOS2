---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-BASE-FORWARDING-110"
record_type: "bounded_timer_base_forwarding_contract"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "66576d5b78eb"
execution_pin_short12: "3b2efdee5d85"
status: "READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R108_01: "CLOSED_DOCUMENTATION_ONLY"
R108_02: "OPEN"
R109_01: "OPEN"
R109_02: "OPEN"
batch108: "RETURN"
batch109: "RETURN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/155-user-timer-wheel-placement-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-D00-106-107-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","mykernel/time/timer/timer.c","mykernel/time/timer/timer.h","mykernel/time/timer/timer_const.h","mykernel/time/timer/timer_types.h","mykernel/time/timer/timer_api.h","mykernel/include/linux/kernel/asm-generic/bug.h"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本文件固定源码短引文"}
executor_display_name: "MYOS2-A-C07b 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-16"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/"
allowed_files: ["MANIFEST.md","timer-base-forwarding.md","facts-and-boundaries.yaml","timer-base-forwarding-paths.dot","deferred-findings.yaml","evidence.md"]
activation_gate: "OWNER_FORWARD_ACTUAL_156_RELEASE_AND_FRESH_G00"
---

# 第110批任务书　定时器怎样选择基准、推进时钟和复用桶号

**待 Owner 转发；执行尚未启动。** 执行者为现「MYOS2-A-C07b 代码分析执行」MYOS2-LEAD-002-CHAT-EXEC-16，模型次一档 GPT-6＋Extra。第109批已算清给定基准时钟时落入哪个桶；本批以109的 TW109-U01 为入口，研究该基准从哪里取得、游标何时推进，以及修改已登记定时器时旧计算在何种条件下仍可用。结论只到固定源码、普通有效对象和有限整数模型。主线由 C07（Pro）独立验收。

## 一　固定输入和权限

1. 完整读取本156号合同、同提交的[109审核](reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-review.md)、[D00裁定](reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-D00-108-review.md)、[检查点](checkpoints/2026-11-19-timer-wheel109-reviewed-timer-base110.md)、本次 C07 发布评论、[109作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7009620914) 与[109固定六件](https://github.com/08822407d/MyOS2/tree/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109)。旧155在[66576d5b78eb](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/155-user-timer-wheel-placement-contract.md)，旧108/107消费者按 D00 回执中的固定文件和项号定点读。
2. 本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条。现C07b只可复用本人已完整读过且固定版相同的[151接收记录](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/151-executor-continuation/01-MYOS2-A-C07b-receiver-startup-prompt.md)、九份原 MyOS2 约束及适用的 Mnemosyne 行为指导。完整重读[63写入及回读](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)。依[28同轮](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)、[79合包](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)、[110沟通](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)、[124模型](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)保持执行/验收分工。同一有效会话按固定记录复用；若有独立接收的需要，先按151加载约束，再接本包。
3. 固定 time@a039d9803ade，master@e8e60cd19540，执行父3b2efdee5d85。lead 为本156实际发布的 agent/MYOS2-LEAD-002 提交。唯一执行写区为原 claude/dazzling-cori-q0dnyt / PR17 的 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/`，仅文末指定六件及本人一次顶层交付评论。C07持主线和两PR导航唯一写权。

## 二　fresh G00

Owner 转发实际固定合同和正式回执后，重新读四条 direct refs；确认原执行父、真正156发布头及上述 time、master。核对开放PR仅16/17、均base master且未合并，16 Ready、17保持原题 Draft。109六件在固定提交和活动分支的全文、blob应逐一相同。110目录和六目标在执行父树均应缺席。PR17评论无 since 分页至末，110主线发布标记一次，110作者标记零次。独立读 direct master 公约§1(2)整句和 time/master 的 options_flags.cmake、panic.c 双金丝雀。使用真实完整 SHA 作租约，成果内短值只抄连接器返回的前12位。

若见本人已发布的对象或评论，核其归属、父、内容和标记，复用并补实际缺项。若未知并行写入，仅暂停依赖变动的写操作。G00通过后连续完成下列四部分。

## 三　D00　只补三个 OPEN 项

| 项目 | 110新增材料的核收条件 |
|---|---|
| R108-02 | 在110实际提交的 DOT 写出第二次 put_user 的条件。可使用不含竖线或引号的文字标签，例如“第一次 put_user 正常返回0才求值第二项 [TW109-E059,DT108-E016,DT108-E010]”。同时限定 tv 分支。以真实固定 blob 全文读回这些字符和三个 ID。核 wrapper 证据 E001 与 syscalls.h 的局部门，列出107 LOC-R107-02-12每条边的真实端点。已核商余和 delta 可直接引用D00裁定。 |
| R109-01 | 按109审核表补 A08/X08、N06/N07、G03/G07、X04/M01–07、C03/N08 和109补正F行。将源位置、条件、数值与引用对应；允许用 RV109-Q1–Q6、旧 P06/P08/P21 及 DT108-P11/P12，标明冻结来源。两个签名与两个返回体分别登记。 |
| R109-02 | 用真实调用者分别连接锁基准、推进及有条件的 trigger 请求。源码次序、函数体注释、条件、直接调用分开标。hz→depth 补 HZ>100 的独立位置。实际读回 DOT 边的两端、完整标签及证据。 |

固定109、108、107、作者评论及旧编号全部保全。已闭合的 R108-01 与更早事项沿用主线裁定。每个差量列旧文件、旧行或ID、新断言、固定证据、可消费范围和首个待证步骤。可以用一张差量表和一幅图中的独立小段完成。

## 四　A　从 flags 到候选 timer base

主读 timer.h 的 get_timer_base、get_timer_cpu_base、get_timer_this_cpu_base 与 timer.c 的 lock_timer_base、__mod_timer 基准选择。列明来源为 timer->flags 的快照、显式 cpu 形参和当前 CPU 三种情况；分别解释 TIMER_CPUMASK、TIMER_PINNED、TIMER_MIGRATING 与两个基准选择表达式。查 timer_const.h、timer_api.h 中实际宏及 timer_bases 声明，区分对象声明、定义、索引和最终运行地址。命名 LOCAL/GLOBAL 仅按固定表达式取义。若所读范围内某枚举、每CPU存储或链接绑定无法证明，保留符号化表示与明确缺口。区分活动代码和被注释的 DEF 选择。

用一个普通标志值及其加 PINNED 后的值，保持有效 CPU 号不变，说明掩码结果与候选对象变化。核 lock_timer_base 所读 MIGRATING 门、锁请求、完整 flags 相等复核与返回主体。仅把稳定标志、有效 CPU 与有效对象当作有限单线程示例的前提。止于源码请求和对象身份，运行地址、锁的实际时序延期。[基准选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L114-L146) · [取样和返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L184-L210)

## 五　B　forward_timer_base 如何决定新的 clk

从 forward_timer_base 的 READ_ONCE(jiffies) 到 __forward_timer_base，分别取 time_before_eq、time_after、time_before 及其真实底层比较。查 asm-generic/bug.h 中 WARN_ON_ONCE 的实际宏、外层 CONFIG_BUG 门与 options_flags.cmake 配置来源。把函数签名与宏条件分列。输入用普通不回绕、满足比较宽度条件的旧 clk、样本 basej、旧 next_expiry，输出列本体是否写 clk、写入何值。区分字段读取、条件提前返回、写同值与无写入。

用旧 clk=1000 及少量输入覆盖：basej 在1000之前或相等；basej=1010 时 next_expiry 早于1000、等于1000、位于1000与1010之间、等于1010、晚于1010。相同输出若落在不同分支仍应标出依据。什么情况下可以描述为取两个值之小，什么情况下前置门优先，按原条件证明。限定其与 schedule_timeout 中另一次 jiffies 采样的关系。 [推进函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L148-L175) · [告警宏位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bug.h#L105-L113)

## 六　C　已登记定时器的同桶更新、复用与重算

主读 timer.c 的 mod_timer、mod_timer_pending、timer_reduce 三个公开包装；对应 __mod_timer 的 options、返回路径、timer_pending、timer_get_idx 与必要的 detach_if_pending。重点做有效同一 CPU、正常非空回调、稳定基准的普通模型。分别追同 expires 早返、REDUCE 的方向门、同桶更新、异桶后复用 idx 和 bucket_expiry、clk 不等时重算，以及 PENDING_ONLY 面对未登记对象的出口。标明哪些分支只改 expires，哪些请求摘链或重新入桶，哪些提前返回；返回0/1须联到调用处与原 pending 前提。

以109已核的轮盘为输入，例如在 clk=1000，比较绝对期限1064与1065（同一桶）、1063与1064（相邻桶）。并列旧/新期限、差、桶号、options、返回、写字段与是否请求重新入桶。另用符号条件比较初算 clk 与最终基准 clk 相等或不同；固定源码只比较数值时，不能由其推出基准指针恒等。迁移部分只读选择门和可能使基准改变的赋值，跨CPU交错及真实运行全延期。[初算和同桶判定](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L227-L278) · [重选与消费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L292-L337)

## 七　成果与验收

六件仅为 `MANIFEST.md`、`timer-base-forwarding.md`、`facts-and-boundaries.yaml`、`timer-base-forwarding-paths.dot`、`deferred-findings.yaml`、`evidence.md`。正文含 D00 与 A/B/C；事实表给出基准选择、游标分支、pending/option/桶更新与贯通样例。每条新事实给实际文件、符号、适用域和首个待证步骤。图以真实调用主体、条件、字段、返回、源码次序标边；将标签当作文本逐边对读，不能仅以计数验证。候选与缺口分别记载，复用109 TW109-U01等历史根号，不把已核普通模型当成真实运行结果。

强制短锚覆盖所用固定配置、A 的位域门和基准选择/锁后复核、B 的采样与每一写入/返回条件、C 的真实包装参数、早退、同桶更新、clk 复用及重算条件。每条为固定 time 真实符号定义内连续1–5行；函数签名、预处理门、include实例、注释域另记普通定位；同名宏的不同定义分别定位。已有合格锚可引用到实际固定件。主辅源码软上限20份、定点搜索8组、目录定位4次、新短锚75、纸面模型12、条件候选/待证缺口各8。无条数下限，优先D00、三专题的实际决策点、可复核的有界模型；其余按18号延期。

MANIFEST 必含 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check 和实际 write_retry_log。按 P9 编号核对：1 六件40hex零；2 每条引文逐字且在正确符号内、能推出所引断言；3 路径/跨包消费者/图边真实；4 辅助行有效并与窗口对应；5 全包原始词头、三整数、不同窗口与普通位置分账；6 外部断言有一手定位；7 双金丝雀；8 公约整句和必填项；9/10 纯文档；11 如实记已做与未做；12 只记实际降级及所用通道；13 强制项逐一核对。阶段2只读固定源码与文本/有限整数计算。完整本地P9、编译链接、仓库脚本和程序、Graphviz、内核、QEMU、并发和实机均 NOT_RUN。执行沿用既有模型安排及无额外代理/付费API的约束。

## 八　发布、回读、交回

写前重新核对完整执行父、目录及 marker。原执行父建单父提交，复用原树，严格新增六件，以 expected_sha=真实父、force=false 正常前移。本人只发一次 PR17 顶层交付评论，正式标记为

MYOS2-LEAD-002-CHAT-EXEC-16 / CORE-USER-TIMER-BASE-FORWARDING-110 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。按约10/20/40秒及服务端 Retry-After，已有对象复用，结果未知先回读。无故障写 not_needed。

回读唯一父、仅六新增和旧修改/删除0/0、固定与活动六件全文/blob、四条直接refs、两PR状态、按ID和无since全分页的正式评论全文及唯一标记。若本人已发布则只补核验。作者交付态 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW；R108-02、R109-01/02 由C07独立裁定。远端回读完成后 STOP_WRITING，交回「MYOS2-A-C07 内核分析主线续建」使用 Pro 验收及同轮安排后继。

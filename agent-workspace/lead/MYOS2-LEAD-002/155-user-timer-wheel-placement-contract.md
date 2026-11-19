---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-WHEEL-PLACEMENT-109"
record_type: "bounded_timer_wheel_placement_contract"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "b089a6e48c7b"
execution_pin_short12: "a8436da2c2b3"
status: "READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "CLOSED_DOCUMENTATION_ONLY"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
R106_01: "CLOSED_DOCUMENTATION_ONLY"
R106_02: "CLOSED_DOCUMENTATION_ONLY"
R107_01: "CLOSED_DOCUMENTATION_ONLY"
R107_02: "CLOSED_DOCUMENTATION_ONLY"
batch106_composite: "PASS_PENDING_LOCAL"
batch107_composite: "PASS_PENDING_LOCAL"
batch108: "RETURN"
R108_01: "OPEN"
R108_02: "OPEN"
executor_display_name: "MYOS2-A-C07b 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-16"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/"
allowed_files: ["MANIFEST.md","timer-wheel-placement.md","facts-and-boundaries.yaml","timer-wheel-placement-paths.dot","deferred-findings.yaml","evidence.md"]
activation_gate: "OWNER_FORWARD_ACTUAL_155_RELEASE_AND_FRESH_G00"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/154-user-duration-tick-conversion-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/facts-and-boundaries.yaml","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-D00-106-107-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-19-duration108-reviewed-timer-wheel109.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SCHEDULE-TIMEOUT-50-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WAIT-WAKE-CONSISTENCY-52-review.md"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本件固定源码引文"}
---

# 第109批任务书　一个有限超时怎样进入内核时间轮

**READY_FOR_OWNER_FORWARDING / EXECUTION_NOT_STARTED。** Owner 将本155号固定链接和 C07 本轮 PR17 正式回执转发到现「MYOS2-A-C07b 代码分析执行」后，由 MYOS2-LEAD-002-CHAT-EXEC-16 使用次一档 GPT-6＋Extra 执行。fresh G00 通过后，连续完成 D00 和 A/B/C，按原 PR17 一次交付。C07 使用 Pro 独立验收。

第108批已确定普通时长对应多少个名义节拍。第50批追踪了有限 timeout 的登记请求，第52批走到 internal_add_timer/enqueue_timer，将完整轮盘推迟。本批补上中间的数值与对象关系：给定一个正的有限超时，绝对到期节拍如何产生、如何选中一个桶、入桶会写哪些字段。终点是源码中的登记与有条件的最近到期记录。是否实际睡眠、某个中断到达和回调何时运行仍依赖独立运行证据。

## 一　固定材料、身份和写区

1. 完整读取本合同、[108审核](reviews/CORE-USER-DURATION-TICK-CONVERSION-108-review.md)、[历史D00裁定](reviews/CORE-USER-DURATION-TICK-CONVERSION-108-D00-106-107-review.md)、[主线检查点](checkpoints/2026-11-19-duration108-reviewed-timer-wheel109.md)、本轮 C07 正式回执、[108作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7006324048)及[108固定六件](https://github.com/08822407d/MyOS2/tree/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108)。以108执行头 a8436da2c2b3 为唯一执行父。[154旧合同](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/154-user-duration-tick-conversion-contract.md)可定点查旧消费者。当次所需的107旧图、表和证据从108审核中的真实固定链接读取。
2. 只消费[第50批有限超时审核](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SCHEDULE-TIMEOUT-50-review.md)的期限形成与登记边界，及[第52批登记审核](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WAIT-WAKE-CONSISTENCY-52-review.md)和[文档闭合](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WAIT-WAKE-CONSISTENCY-52-DOC-CLOSURE-review.md)中与 normal NOTPENDING、internal_add_timer 有关的原结论。以实际源代码追到选桶、写入和比较。本批无须重做等待队列、取消、迁移和回调全程。
3. 本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条。同一 C07b 对话可按[151接收记录](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/151-executor-continuation/01-MYOS2-A-C07b-receiver-startup-prompt.md)复用本人亲读且固定版本相同的九份 MyOS2 约束和适用的 Mnemosyne 行为指导，含项目 README、公约、协议、00工作令、18延期、24分工、28同轮、63写入、79合包，另有[110沟通](110-owner-communication-and-delivery-continuity.md)、[124模型](124-model-routing-and-pro-message-budget.md)。本轮完整重读[63写入规则](63-github-write-retry-and-readback.md)和新裁定。
4. 固定源码 time@a039d9803ade，direct master@e8e60cd19540。lead 是本155实际发布的 agent/MYOS2-LEAD-002 / PR16 头。执行身份仅能在原 `claude/dazzling-cori-q0dnyt` / PR17 新增 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/` 的六件，另发一次本人顶层正式评论。C07 保有主线与 PR 导航唯一写权。

## 二　fresh G00 和已有工作恢复

独立读取四条 direct refs。核对 lead 为真正155发布头、exec 为 a8436da2c2b3、time/master 如上。核对仅 PR16/17 开放，均 base master、未合并，16 Ready、17 原标题且 Draft。逐件核108固定与活动六件的全文和 blob。109目标目录和六个文件在原执行父应缺席。PR17 顶层评论无 since 全分页至末，109主线发布标记应恰好1次、作者正式标记应为0。读取 direct master 公约§1第2条原句，以及两分支 options_flags.cmake 和 panic.c 的两对金丝雀。

若本人已完成部分动作，先核父提交、归属、目标内容、评论全文再复用，只补实际缺项。意外并行推进暂停冲突写入。租约用真实完整 SHA，文档仅写连接器返回的12位短值。

## 三　D00 仅补 R108-01、R108-02

| 原问题 | 109新增差量 | 核收条件 |
|---|---|---|
| R108-01 | 按108审核的三行逐项纠正两个不同的预处理条件、M01源窗口、单位、普通毫秒域、反向引用、P25页账。 | 逐项绑定旧文件和 ID。可引用 RV108-Q1–Q4，列完整探测值与掩码结果、带偏置除法的商余。冻结57条原始引文及12组模型。 |
| R108-02 | 给108图中已点名的边与节点提供正确的条件、端点、类型和依据；把跨107文件的旧消费者分别指回真实 evidence、MANIFEST、facts、DOT。 | 重点检查 putsec→putus 有“第一项返回0”的完整标签及依据；区分架构包装头和清单宏；核回 delta、辅助函数、商的返回及发生位置。可引用 RV108-Q5/Q6、107/108已有编号。 |

将差量与109新主链分开记。每项记录原固定文件与标识、补正、直接证据与尚缺事实。允许直接引用 C07 已发布的独立证据，写明真实固定回执路径与 ID。R106-02、R107-01/02 已由 C07 闭合。

## 四　A　从相对节拍到绝对到期值与轮级

主读 mykernel/time/timer/timer.c 中 schedule_timeout 的正有限分支和 __mod_timer 的 NOTPENDING 路径、internal_add_timer、calc_wheel_index。给出相对 timeout、采样的 jiffies、timer->expires、base->clk 和无符号 delta 的来源；jiffies 样本与 base->clk 在模型中若取相同值须明说。区分进入 normal insertion、预计算 idx 的条件、UINT_MAX 初值及重算。仅采用有效 timer/base、前置条件成立的登记路径。

主读 mykernel/time/timer/timer_const.h 的 LVL_CLK_SHIFT、LVL_SHIFT、LVL_GRAN、LVL_BITS/SIZE/MASK/OFFS、LVL_START、LVL_DEPTH、WHEEL_SIZE 与 mykernel/include/linux/kernel/asm-generic/param.h 的 HZ。各独立定义/选支分别登记。推导层数、每层桶数和各层差值门，亲查小于号及首个跨层值。对负距离解释、截断及上限只列适用前提和源内分支，数值集中普通无回绕小值。

## 五　B　从绝对 expires 得到 bucket_expiry 和数组下标

主读 mykernel/time/timer/timer.h 的 calc_index，结合 timer.c 的实际调用者与 timer_const.h 定义。按原运算先右移、再加1，再分别得到有效到期节拍和桶号。将“按 delta 选层”与“用绝对 expires 求该层中的位置”分别核证。区分层内槽和全轮数组下标。给出合法移位、无回绕、相对距离和数组域。

用同一份小样本表覆盖首个层级边界前、边界上、边界后，并含一个 expires 恰好对齐粒度的例子。可设 base->clk=1000、jiffies 同为1000，比较相对62、63、64节拍；另外以相同 delta 改变基准，解释桶号是否相同。表列 expires、delta、level、粒度、bucket_expiry、slot、idx 和有效节拍距请求值的差。数值必须从同一固定树算得。模型只做普通整数推导。

## 六　C　选中的桶、标志位和最近到期值

主读 timer.c 的 internal_add_timer、enqueue_timer，timer.h 的 timer_set_idx、timer_get_idx，timer_types.h 的 timer_base.vectors、clk、next_expiry、timers_pending、next_expiry_recalc、timer_list.entry/expires/flags；timer_const.h 的 TIMER_ARRAYSHIFT/ARRAYMASK。读 mykernel/time/systick/systick_macro.h 的 time_before/time_after，按需取 mykernel/lib/list/hash_list.h 的 hlist_add_head 别名与 hlist_add_to_head 体。核对链表节点、桶头的身份和写入顺序。用同一个合法 idx 说明存入与解码的位域和保留位，标明实际类型假设。

比较 bucket_expiry 早于、等于、晚于既有 next_expiry 的三种普通情况，各列无条件入链/记录 idx，以及条件成立才更新的三个 base 字段。active 写入、注释 bitmap、trigger_dyntick_cpu 的调用与其所读函数体分别定位。停止在源码可见效果，实际锁、异步处理、设备和唤醒延期。

## 七　六件、证据和验收上限

唯一六件为 `MANIFEST.md`、`timer-wheel-placement.md`、`facts-and-boundaries.yaml`、`timer-wheel-placement-paths.dot`、`deferred-findings.yaml`、`evidence.md`。正文含 D00 与 A/B/C；事实表含旧项差量、期限来源与门、轮级几何、桶号推导、有效到期值、入桶与位域、最近期限的条件更新、贯通数值样本。按问题组织表行，无条数配额。每行列精确源、适用前提、可用结论和首个待证步骤。图按实际函数、条件、宏、数据、时序和返回标边；逐边核可读标签、证据 ID、真实位置与端点。既有50/52/108根号按实际消费者引用；新候选与缺口单列。

强制短锚覆盖相关固定配置与 HZ、A 的期限赋值/调用/选层条件、B 的独立宏和两种返回载体、C 的入链/索引编码与解码、比较与条件写入。每条取固定 time、真实符号、定义内连续1–5行。函数签名、include、注释及预处理条件的定位单列；不同宏或命令按独立单元取证。已合格的旧锚允许引用。主辅源码软上限22份，定点搜索8组，目录定位4次，新短锚80条，纸面模型12个，候选/缺口各8。优先级为 D00、三专题的实际数据链和边界算例，外围源码按18号延期。

MANIFEST 含 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check 和实际 write_retry_log。按 P9 编号核对：1 全六件40hex为零；2 逐字、单元及支持关系；3 路径、跨包ID、每个消费者文件及图边；4 辅助行合法且与索引一致；5 全包词头、三整数、位置和窗口分列；6 外部断言一手可定位；7 双金丝雀；8 公约和必填字段；9/10 纯文档；11 真实执行边界；12 有降级才登记；13 所有强制项逐一核对。阶段2只读源码及文本/整数核查。完整本地 P9、构建、仓库程序与脚本、Graphviz、内核、QEMU、硬件运行均 NOT_RUN。沿用无额外模型、子代理或付费 API 的安排。

## 八　发布、回读与交回

写前刷新完整执行 HEAD、六目标及标记。以 a8436da2c2b3 对应的真实完整执行提交为唯一父，复用原树，一次严格六新增，以 expected_sha=真实父、force=false 正常前移。本人一次 PR17 顶层交付评论的标记为

MYOS2-LEAD-002-CHAT-EXEC-16 / CORE-USER-TIMER-WHEEL-PLACEMENT-109 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。按10/20/40秒及服务端 Retry-After；已有对象复用，评论响应丢失先查重。无故障记录 not_needed。

回读唯一父、compare 六件全新增和旧修改/删除0/0、固定/活动六件全文及 blob、四条 direct refs、两 PR 形态，以及按 ID 和无 since 全分页的评论正文和唯一标记。作者终态 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW、R108-01/02 OPEN。全部完成后 STOP_WRITING，交回「MYOS2-A-C07 内核分析主线续建」使用 Pro 独立验收及同轮安排后继。

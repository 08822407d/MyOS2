---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-CORRECTION-CLOSURE-001
record_type: independent_append_only_evidence_and_local_analysis_correction_closure
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用正式验收安排）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
original89_short12: 42897a06feb1
original89_comment: 6893704757
original_review_short12: 07ed0fed054c
correction90_short12: 034cd9c52dbf
correction90_comment: 6896688417
status: CLOSED_BOUNDED_CORRECTIONS_NO_KERNEL_FIX
R89_01: CLOSED_EVIDENCE_DOCUMENTATION
R89_02: CLOSED_BOUNDED_LOCAL_ANALYSIS_CORRECTION
batch89_composite_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R89-S01, R89-S02, R89-S03, R88-S01, R88-S02, R88-S03]
original_records_modified: false
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0, scope: 本件以固定对象与锚ID记录闭合，不另复制源码标签}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [127合同及128换接令, 原89回执及原固定材料的同一C06已读范围, 新90六件及正式评论全文, 本轮新读固定cpumask与runqueue全文和create_worker及err定义]
owner_decisions_requested_now: []
open_questions: []
---

# 第89批两项追加纠正闭合，组合PASS_PENDING_LOCAL

本件关闭R89-01/02，既不宣布内核问题修复，也不替第90批签收。90本身另有R90-01/02文档退回，见同轮90-review；新批退回不反向否定已经满足的旧项纠正。

## 1. 对象与独立核对

原件为MyOS2@42897a06feb1的agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-pool-bootstrap-89/六件；原89回执固定07ed0fed054c。追加件固定034cd9c52dbf的相邻user-id-allocation-lifecycle-90/六件，作者为C06c/CHAT-EXEC-12，不冒用旧C06b/11。

本轮完整读取90六件及评论6896688417，并重新取得time@a039d9803ade的cpumask.h、runqueue.c、err.h全文和workqueue.c完整create_worker。原89已读材料按本有效C06相同固定对象复用，不继承执行者的加载证明，不声称本轮重跑原120锚机械检查。

| 旧项 | 新90对应 | 闭合依据 |
|---|---|---|
| R89-01 / WQP89-E010跨宏 | IDA90-E001/E002/E003；主文D00、facts.D00.R89_01及消费者列表 | 下级for_each_cpu、possible、online分别锚定；旧锚、矩阵与M01/M02/M03/M08有映射；撤回旧120条均在单一定义内的总括，原计数和原件冻结。 |
| R89-02 / NULL任务错误停证点 | IDA90-E004至E010、E059、M30；facts.D00.R89_02，F05/U08消费说明 | 实际set_user_nice已完整读取，只有未使用局部声明，对任务的操作为注释；撤回在该处凭NULL参数认定解引用失败的读法，给出有条件接续。 |

## 2. 闭合后的有效读法

R89-01仅证明这几个宏身份和对应关系已纠正。possible与online都转发稠密整数循环，不因名字推导真实在线CPU集合；完整展开、配置和索引有效性仍是前提。新90的另一处跨宏E013不是这三条D00锚，由90独立处置。

R89-02中，调用实参pool->attrs->nice必须合法求值。只有明确假设非BH路径中线程创建包装返回NULL、错误指针判定按所读目标模型为假、worker/pool/attrs/list/lock有效且其他调用正常返回，才可继续挂接、增加计数、进入idle、跳过wake并返回worker。这不是实际kthread能返回NULL的证明，也不是线程已创建或运行的证明。

M30的0→1数字另需初始计数为0且无其他写者；一般有效结论是原计数加1，不能无条件固定为1。该消费限定由主线直接补足，不要求重做89。

WQP89-F05保留原qualified_id，收窄为NULL结果来源和调用方契约尚待证明的候选。WQP89-U08中的nice效果不再笼统未知：本固定set_user_nice没有活动的优先级更新语句；CPU绑定及真实调度效果仍未证。名称中使用nice值不等于调度优先级已生效。

## 3. 冻结、状态与后续

compare 42897a06feb1→034cd9c52dbf为ahead1/behind0，一个提交，仅90六件added3307行，旧件零净修改删除。作者只申报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，没有自签闭合。R89-01/02在上述范围满足127要求，独立关闭；89组合结论为PASS_PENDING_LOCAL，必须合读原89回执R89-S01/S02/S03及本件和90/D00。

原120/120/0、旧回执、原六件及评论不改。WQP89、PID11、PA87、PC86等源码问题及缺口不因本件自动关闭；R88-01、R86-01与既有闭合不重开。完整本地P9、仓库程序、构建、内核、QEMU、硬件和真实并发均NOT_RUN。

第91批仅纠正90新文档项并开展新专题，不再派89闭合专用回合，不要求Owner逐项核数、审源码或决定普通修复。

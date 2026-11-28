---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95-DOC-CLOSURE-001
record_type: independent_append_only_report_correction_closure
evidence_class: 固定95原件、96追加纠正与固定源码的独立可读复核；非运行认证
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台未独立认证）
model_per_owner: Pro（沿用Owner正式独立验收安排）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
original95_short12: be83c6095eb1
original95_comment: 6937736735
original_review_short12: fda0f3d1cb60
correction96_short12: 6a1fa78d0356
correction96_comment: 6941872312
status: CLOSED_REPORT_CORRECTIONS_ONLY
R95_01: CLOSED_REPORT_CORRECTION_ONLY
R95_02: CLOSED_REPORT_CORRECTION_ONLY
batch95_composite_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R95-S01, R95-S02, R95-S03, R94-S01, R94-S02, R94-S03]
original_records_modified: false
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0, scope: 本件引用96的固定D00证据与原95回执，不复制源码标签}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [95原审查及固定原件相关段, 96六件与评论全文, 固定printk和宏及字体定义]
owner_decisions_requested_now: []
open_questions: []
---

# R95-01/02闭合；95组合结论PASS_PENDING_LOCAL

关闭的是分析报告中的实质调用顺序错误和证据锚定错误，不是内核修复，也不是仅修排版。95原六件、70/70/0、评论和当时RETURN全部保留；组合使用原95、原审查补充、本件及96/D00。96自身的R96-01/02另行裁定，不能用本闭合件替代96验收。

## 1. R95-01逐项复核

96的framebuffer-mapping.md之D00、facts的D00_original95_correction_map与D00矩阵、M01–M05、图reg/console_unlock/ring_scan/dispatch/callback，使用FM96-E001–E014恢复真实调用链。固定源码仍为time@a039d9803ade。

| 原错误或消费者 | 96追加纠正及主线判断 |
|---|---|
| register只入链；console_unlock被误说成注释 | E001给重复节点提前return，E002给新节点尾插之后的活动console_unlock。两条分支分开，纠正成立。 |
| 首次可能输出必须等注册后的pr_info | D95-R01-BOOT及M02说明：若已有合法含LF积压且相关门成立，可在注册返回前触达输出；不是必然显示。 |
| 无积压、无LF、重复节点、假设NULL回调 | M01–M05分别覆盖；空积压退出flush；无LF尾段不调用write且con_start已前移；重复节点不进入本次flush；NULL只是假设的其它对象，内置write仍非空。 |
| A-REG/C-BOOT/ABC、M01/M03/M04、U02/U07和图 | 新旧消费者映射显式覆盖，旧M02重复节点结论保留；图加入register直接调用console_unlock，间接设备调用仍归__call_console_drivers。 |
| 活动排空与旧回放策略混淆 | 当前con_start至log_end的排空，不恢复注释的CON_PRINTBUFFER、重置到log_start、exclusive或setup/flags策略。 |

主线另完整回读printk.c的__call、_call、call_console_drivers；无LF结论须合其末尾被注释的_call_console_drivers(start_print,end,msg_level)，不能只靠E009/E010的LF片段证明。此为闭合消费限定，不声称95原报告本来正确。

## 2. R95-02逐项复核

| 原95标签 | 96替代 | 独立结果 |
|---|---|---|
| CN95-E045 | FM96-E015/E016/E017 | PAGE_SHIFT、PAGE_SIZE、PAGE_MASK各为一个独立宏，不夹前置说明或相邻宏。 |
| CN95-E064 | FM96-E013/E014 | 实际for_each_console及其下级宏支持遍历；宏前说明不再充当活动锁保证。 |
| CN95-E050 | FM96-E018，合原CN95-E051 | font.c真实数组存储定义与font.h extern声明分账；本消费者只读不等于对象const或页权限只读。 |

D00映射覆盖B-FMT02–05、旧M14/M15、F04/U04，A-LIST04/07、ABC-02、U03，B-BYTE02/C-GLYPH01/M07/U06等实际消费者；原source_quote_index及全通过语义由新记录明确撤回和替代。原70/70/0作为历史作者计数保留，不反向伪造当时已经通过全部定义边界核验。

E023尾空白不作为错误；53历史空行误判继续撤回。96新E036跨独立typedef与FM96-F05字段错位不在上述D00证据子集内，故不阻断95这两项独立关闭。

## 3. 保留边界

R95-S01/S02/S03及R94、91–93适用停止门继续有效；CN95-F01–F06/U01–U08等源码候选、对象/映射/并发和实际显示缺口不因报告纠正而关闭。96的FM96-U09仅“等C06签95纠正”这一流程依赖由本件解除，不将它计成修复一个内核缺陷。

角色独立是本C06对执行C06d/13的复核，不是人类逐字、异族或后台模型认证。没有运行原作者检查器、全套P9、仓库程序、编译或硬件测试。97不重开95或重复这组纠正；新任务只承接96定点补正及新的引导信息专题。

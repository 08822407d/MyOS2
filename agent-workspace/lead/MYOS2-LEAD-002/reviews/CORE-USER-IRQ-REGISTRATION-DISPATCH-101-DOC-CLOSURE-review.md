---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-IRQ-REGISTRATION-DISPATCH-101-DOC-CLOSURE-001
record_type: independent_append_only_documentation_closure
conversation_display_name: MYOS2-A-C07 内核分析主线续建
produced_by: GPT-6 Astra Pro（当前会话模型标识；后台不可独立认证）
model_per_owner: Pro（沿用Owner主线选择）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
original101_short12: 14e8f1580f5a
correction102_short12: 1a92f9839e53
correction_delivery_comment: 6978933773
review_basis_short12: 75338cd7d005
status: CLOSED_DOCUMENTATION_ONLY
R101_01: CLOSED_DOCUMENTATION_ONLY
batch101_composite: PASS_PENDING_LOCAL
original101_verdict: RETURN_PRESERVED_AS_HISTORICAL_ORIGINAL
R100_01: CLOSED_DOCUMENTATION_ONLY
batch100_composite: PASS_PENDING_LOCAL
closure_scope: 20原锚替代、向量宏链、真实消费者映射与S01/S02/S03限定
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0, scope: 本件引用已独立回源的102证据ID，不另复制源码标签}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [75338cd7d005的101-review全文；1a92f9839e53的102六件全文和6978933773；14e8f1580f5a的101证据及facts/deferred真实消费者；time固定源码对应定义与宏]
owner_decisions_requested_now: []
open_questions: []
---

# 关闭第101批R101-01，仅文档纠正范围

C07独立核收第102批D00后，R101-01=CLOSED_DOCUMENTATION_ONLY。第101批原件、101-review、本闭合件和既有S01/S02/S03合读，组合状态为PASS_PENDING_LOCAL。第102批自身的三项文档退回由另一回执处理，不撤销本次已成立的101纠正，也不重开100、96或65的文档闭合。

本件不把旧105条重新认证成原始交付时全部合格；原105/105/0与旧RETURN原样保存。关闭不是源码修复、IRQ实际运行认证或完整本地P9通过。

## 1. 固定证据和核收方法

- 原件：MyOS2@14e8f1580f5a:agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-irq-registration-dispatch-101/。
- 追加纠正：MyOS2@1a92f9839e53:agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/，facts.D00、正文D00、evidence的HC102-E001..E028及相关消费者。
- 裁定依据：[101-review](CORE-USER-IRQ-REGISTRATION-DISPATCH-101-review.md)的R101-01和S01/S02/S03；原合同146。
- 两批六件Git blob身份、102单父追加及作者正式评论均已核定，详细远端账见同轮[102-review](CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-review.md)。

新20条已逐条重新定位到实际同名函数体内部，连续1–5行，逐字、行范围、符号与命题逐项核查。没有把“旧引文原样可搜索”当作函数体资格。原E017的参数头和其余19条混入函数头的缺陷由新锚替代消费，原文件未改。

## 2. 二十条替代账

下表消费者数是旧101 facts和deferred内明确evidence_ids引用的并集；同一ID在不同容器重复只计一次。主线逐条提取实际引用，再与102 D00.legacy_consumers比较，20组无遗漏、无多列。正文和DOT的概括由D00规则及下述宏链限定共同消费，不伪称所有自然语言均可由字符串扫描穷尽。

| 原证据ID | 原真实符号 | 新体内证据 | 明确消费者数 |
|---|---|---|---:|
| IR101-E017 | register_irq | HC102-E001 | 8 |
| IR101-E022 | unregister_irq | HC102-E002 | 7 |
| IR101-E026 | hwint_irq_handler | HC102-E003 | 15 |
| IR101-E032 | IOAPIC_enable | HC102-E004 | 16 |
| IR101-E034 | IOAPIC_disable | HC102-E005 | 5 |
| IR101-E036 | IOAPIC_install | HC102-E006 | 15 |
| IR101-E037 | IOAPIC_uninstall | HC102-E007 | 1 |
| IR101-E038 | IOAPIC_level_ack | HC102-E008 | 5 |
| IR101-E040 | IOAPIC_edge_ack | HC102-E009 | 5 |
| IR101-E041 | ioapic_rte_read | HC102-E010 | 6 |
| IR101-E044 | apic_msr_write | HC102-E011 | 2 |
| IR101-E045 | ioapic_rte_write | HC102-E012 | 5 |
| IR101-E048 | IOAPIC_init | HC102-E013 | 3 |
| IR101-E051 | myos_init_bsp_intr | HC102-E014 | 3 |
| IR101-E061 | myos_init_percpu_intr | HC102-E015 | 1 |
| IR101-E065 | HPET_handler | HC102-E016 | 3 |
| IR101-E067 | myos_HPET_init | HC102-E017 | 9 |
| IR101-E076 | LVT_timer_handler | HC102-E018 | 3 |
| IR101-E078 | LVT_timer_init | HC102-E019 | 4 |
| IR101-E081 | LVT_timer_ack | HC102-E020 | 4 |

每条original_source_ref均指向真实固定101 evidence路径及原ID，supersedes使用对应限定身份。新旧关联无需回写旧矩阵、模型、图、F/U或历史评论。

## 3. 向量链、撤回总括与限定

HC102-E024是irq_vectors.h中LOCAL_TIMER_VECTOR的独立定义；E025/E026/E027分别提供FIRST_SYSTEM_VECTOR、VECTOR_IPI、LAPIC_LVT_TIMER_IRQ。按相应编译条件，0xec加2得到0xee。D00明确连接B-V03、C-S07、IR101-M19及原正文/DOT有关数值。只认证宏和条件算术，不认证IDT内容、设备投递或实际IRQ。

102 D00.original_overbroad_claim及MANIFEST§3已经撤回原“105条全合格、强制宏齐全”的过宽总括；旧字面命中和计数保留历史身份。新102原始计数自身有错误，已另立R102-01，不用它推翻这条有明确范围的撤回。

以下要求均已在D00明确落实：

1. S01：irq=256的one-past与大于256时更早的指针运算边界分开；install/disable或handler正常返回后，仍须当前controller有效、非空和字段可读。HC102-E021/E022/E023给出实际后续读取位置。
2. S02：index+1先作整数提升，再在uint8_t目标处窄化；HC102-E028独立给IOAPIC_map的三个指针字段，EOI为uint32_t指针。表示、地址合法性、访问请求次序、屏障、真实MMIO与设备接受继续分层。
3. S03：旧根为MYOS2-LEAD-002::CA-05和MYOS2-LEAD-002-CORE-CHECK-01::V14；不混成CORE-SCHED-INTEGRATION-03::IR-01。真实链接别名、入口/返回、IRQ和时基未证，不运行旧模型，不新增旧根因计数。

## 4. 后置状态消费

| 对象 | 有效状态 |
|---|---|
| 101原六件、原105/105/0及旧作者评论 | 冻结，不编辑、不删除 |
| 101原RETURN回执 | 历史原件保留，后置读取本闭合件 |
| R101-01 | CLOSED_DOCUMENTATION_ONLY |
| 101组合状态 | PASS_PENDING_LOCAL |
| 100、96、65既有文档闭合 | 保持，不重开 |
| 102原件 | 按102-review另行RETURN，R102-01/02/03 OPEN |
| 103 | 只按147实际发布与Owner转发后的现场门执行；不得自签102关闭 |

完整本地P9、预处理/编译/链接、仓库代码/脚本、Graphviz、QEMU/实机和真实并发均NOT_RUN。关闭限于主线已实际回源核实的文档项，不扩大成全105条的新一轮认证。

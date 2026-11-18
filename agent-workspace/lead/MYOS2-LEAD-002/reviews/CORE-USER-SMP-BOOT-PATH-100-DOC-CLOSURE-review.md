---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SMP-BOOT-PATH-100-DOC-CLOSURE-001
record_type: independent_append_only_documentary_correction_closure
evidence_class: 原100错误条目、101固定纠正与固定源码独立对照
conversation_display_name: MYOS2-A-C07 内核分析主线续建
produced_by: GPT-6 Astra Pro（当前会话模型标识；后台不可独立认证）
model_per_owner: Pro（沿用Owner本主线选择）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
original100_short12: dd07c3b19fa1
original100_comment: 6971992040
original_review_observed_at_short12: 0cd745b92280
correction101_short12: 14e8f1580f5a
correction101_comment: 6976123017
status: CLOSED_DOCUMENTATION_ONLY
R100_01: CLOSED_DOCUMENTATION_ONLY
batch100_composite_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R100-S01, R100-S02, R100-S03]
original_records_modified: false
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0, scope: 本件引用101固定纠正与已回源的证据ID，不重复源码标签}
inputs_read: [100原回执全文、原六个错误证据及消费者，101严格六件与正式评论全文，101中D00所需固定源码与读取账]
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第100批R100-01文档纠正闭合

本C07已独立核收第101批D00，关闭R100-01，范围仅为AI报告的六项证据资格及其消费映射。第100批原件、原RETURN回执、旧评论及101/101/0保持冻结；原件与本闭合件、R100-S01/S02/S03合读，组合状态为PASS_PENDING_LOCAL。第101批自身的R101-01另案处理，不把已经核收的100纠正重新退回。

## 1. 固定证据与实际核查

原件在agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-smp-boot-path-100/，固定dd07c3b19fa1；纠正在agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-irq-registration-dispatch-101/，固定14e8f1580f5a。原100六件在dd07固定与14e8执行头的目录元数据逐件核对，六个Git blob身份一致。

独立全文读取101六件，重新定位100 evidence的六个旧ID及facts中的真实消费者；再读取time@a039d9803ade的CMake命令、链接器.text、setup_per_cpu_areas相关完整函数体、fill_cputopo_info完整函数体等。101新增E001–E010逐条与原文、定义/命令边界及所支持命题核对。没有将作者的“已读”声明当作主线自己完成的读取。

| 100旧ID | 101替代锚 | 闭合判断与受影响消费者 |
|---|---|---|
| SP100-E032 | IR101-E005 | 回到options_flags.cmake第二条set(CMAKE_C_FLAGS ...)配置追加命令；旧CONFIG伪选择符撤回，A-C03/B-L06对齐 |
| SP100-E061 | IR101-E003 | 回到file(GLOB_RECURSE KERNEL_C_SRCS ...)真实命令；B-L01对齐 |
| SP100-E062 | IR101-E004 | 回到file(GLOB_RECURSE KERNEL_ASM_SRCS ...)真实命令；B-L01对齐 |
| SP100-E063 | IR101-E001/E002 | 两条list(REMOVE_ITEM ...)分成两个独立单命令锚；原单证据单元资格撤回，B-L05对齐 |
| SP100-E069 | IR101-E006 | 源码输出节为.text；不再把说明别名.text_output当真实节名，B-P06/B-L04对齐 |
| SP100-E070 | IR101-E007 | 同一真实.text节内另一连续短段；B-P06/B-L04对齐 |

101 facts.D00的六条记录均具有旧qualified_id、真实源路径、处置、替代新ID、消费者和未证前提。supersedes_path指向确实存在的100 evidence原件，范围只覆盖上述错误；original_all_101_definition_single_command_qualified_summary明确为WITHDRAWN。正文、MANIFEST和D00状态均保留纠正待主线审核，由本回执现在关闭。

## 2. R100-S01/S02/S03继续有效

R100-S01所需支持由IR101-E008/E009/E010分别给出target_include_directories(kernel)、setup_per_cpu_areas体内load_direct_gdt调用、fill_cputopo_info末尾游标返回。主线重新核这些命令/函数，并完整读取percpu_area.c及asm/apic.h实际接口与apicid_to_cpunr extern上下文。补充读取发生在本次/101阶段，不倒写为100原交付时已经完成；extern仍只是接口上下文，不能充当定义锚。

R100-S02仍要求先证明计数、分配、合法对象与索引，再分析AP表项；第二次表项读取的稳定性、地址与值分别保留。最早无效访问后停止正常执行推导，源码MSR请求不等于CPU上线或硬件发令成功。

R100-S03已在101 D00中纠正压缩调用箭头：x86_64_start_kernel分别调用myos_early_init_system与start_kernel，不能读成前者调用后者。图结构、模型和文本计数不是实际启动或执行观察。旧PC86/BT97等根身份及其未证边界不重编号。

## 3. 闭合的准确含义

本次并未重新认证原100全部101条为原交付时合格，也未将旧101/101/0机械改数。只核收六项追加纠正、对应消费者及必要支持；101自己的105条另账。

关闭文档退回项不修复任何内核问题，不确认实际构建选入、ABI、ELF、perCPU映射、AP启动或真实硬件结果。R99-01和R96-01/02的既有文档闭合不重开。

本裁定由不同对话角色的GPT主线审查GPT作者成果，不构成异族独立复核或后台模型认证。完整本地P9、预处理/编译/链接、仓库程序/脚本、Graphviz、QEMU/实机及真实并发均未运行。第101批正式裁定见同目录CORE-USER-IRQ-REGISTRATION-DISPATCH-101-review.md。

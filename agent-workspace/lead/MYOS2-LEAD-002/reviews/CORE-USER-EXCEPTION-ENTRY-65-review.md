---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EXCEPTION-ENTRY-65-REVIEW-001
record_type: bounded_static_review_with_combined_documentation_return
evidence_class: 固定六件全文、真实源码定义及GitHub净差异核验；非运行验收
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract95_short12: 16d202bbb484
reviewed_base_short12: 16817a5be4d1
reviewed_head_short12: 4b7b0b9642c3
reviewed_delivery_comment: 6743139361
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R65-01]
subjects: {D00: ACCEPTED_AND_R64_01_CLOSED_SEPARATELY, A: CORE_RETAINED_PENDING_DOCUMENTATION_GATE, B: CORE_RETAINED_PENDING_DOCUMENTATION_GATE, C: CORE_RETAINED_WITH_LIMITS}
mandatory_supplements: [R65-S01, R65-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original63_full_automatic_byte_definition_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
graph_blob_short12: 005f30434c12
graph_bytes: 3384
graph_nodes: 36
graph_edges: 43
graph_undefined_endpoints: 0
graph_full_object_identity_match: true
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段示例，非原63条全量认证}
next_followup: CORE-USER-ENTRY-STACK-BINDING-66
next_task_ref: ../97-user-entry-stack-binding-contract.md
executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
inputs_read: [65完整六件及正式评论, 固定95任务及64回执, 28号全文, 79号全文, 协议v2全文, 本文列出的固定源及既有同pin原文]
open_questions: []
---

# 第65批文档退回；R64-01另行闭合

## 1. 真实交付与分别裁定

固定执行4b7b0b9642c3、评论6743139361均已读取。compare自16817a5be4d1为ahead1/behind0、单提交，严格六件A新增1866行：MANIFEST80、正文73、facts760、evidence656、DOT84、deferred213；旧件零净修改删除。直接lead/PR16=16d202bbb484，执行/PR17=4b7b0b9642c3；两PR Open/unmerged，17 Draft，开放第三项为空。direct time=a039d9803ade，master=e8e60cd19540；缓存base_sha不替代direct master。
65 D00已经独立满足64归属纠正，另附64-DOC-CLOSURE回执裁定R64-01=CLOSED_DOCUMENTATION_ONLY、64组合材料PASS_PENDING_LOCAL。65自己则有明确文档闸门失败，裁定RETURN_DOCUMENTATION，合并为R65-01；不以技术主干大致正确掩盖P2/P3/P6失败，不要求整批重做或修改内核。
执行者已声明STOP_WRITING；这是交付状态证据，不是对其隐藏模型、临时文件和全部历史工具调用的认证。C05b/08继续承担后继，旧07/06不恢复。

## 2. R65-01甲：十三个标签须定点重锚

以下是定义边界/真实符号问题，不是空行指控。所列非空文本大体真实存在，但“同一文件有这些字”不等于“都在所标的单一定义内”。不重新使用已撤回的63或58空白指控。

| 原标签 | 实际问题 | 新包最小处置 |
|---|---|---|
| A01 | idt_table与idt_descr是两个独立定义，组合标签idt_table/idt_descr不构成一个定义 | 分别锚定数组定义和描述符初始化器；也可将数组作为独立source_context，但保留描述符真实锚 |
| A16 | 首行是load_idt定义之前的native_load_idt注释 | 将该注释移至上下文，摘录真实load_idt定义内片段 |
| A17 | 把HAS_KERNEL_IBT和IDT_ALIGN两个独立宏放在IDT_ALIGN标签下 | 两个单宏短锚，数值推导另述 |
| A18 | IDT_ENTRIES/NUM_EXCEPTION_VECTORS两个宏组合 | 拆成两个实际宏标签 |
| A20 | X86_TRAP_GP/X86_TRAP_PF两个宏组合 | 分别锚定 |
| A21 | GEN_PROT_VEC/PAGE_FAULT_VEC两个宏组合 | 分别锚定 |
| A22 | FIRST_EXTERNAL_VECTOR之前的三行注释不在宏定义内 | 宏本身一行作锚；注释独立上下文 |
| A23 | 注释与.align在idt_handler_array的SYM_CODE_START之前 | 唯一标签区间从真实起始标识开始；前置对齐另列上下文 |
| B11 | reg_t typedef前后的注释不是typedef定义体 | 仅保留真实typedef行，条件历史另列 |
| B12 | 五个独立偏移宏用斜线标签合并 | 五个单宏标签，保留原值而非伪造联合定义 |
| B13 | RSP/SS两个独立宏合并 | 分别锚定 |
| B14 | OFFSET macro expansions不是此文件中的定义名；这些调用位于main内 | 使用asm-offsets_64.c::main；若讨论OFFSET实现另行定位定义 |
| C04 | 驱动警告注释在in_atomic宏定义之前 | 只用三行宏定义，注释作为上下文 |

三个实际修正示例，接收方仍须独立回源：

[VERIFIED mykernel/arch/x86_64/include/asm/idtentry.h::IDT_ALIGN]
```c
	#define IDT_ALIGN	(8 * (2 + HAS_KERNEL_IBT))
```

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::main]
```c
	OFFSET(IRQ_NR, pt_regs, irq_nr);
	OFFSET(ORIG_RAX, pt_regs, orig_ax);
	OFFSET(RIP, pt_regs, ip);
	OFFSET(CS, pt_regs, cs);
	OFFSET(EFLAGS, pt_regs, flags);
```

[VERIFIED mykernel/sched/scheduler/preempt_macro.h::in_atomic]
```c
	#define in_atomic()	(				\
				preempt_count() != 0	\
			)
```

13是需处理的旧标签集合，不是13项内核漏洞；其它50条不因未列入而自动取得主线全量通过。原作者63/63声明不得继续被解释成全部定义闸门合格。新D00给旧标签到新锚的映射，P6仅统计新包实际短锚，不修改原evidence。

## 3. R65-01乙：证据索引必须与命题一致

facts.idt_binding_matrix存在实质错配：gate_data指A15/A16（实际为write_idt_entry/load_idt），gate_pack指A17/A18（实际为常量），table_write指A19（错误码mask），load_request指A20（trap常量），stubs引用A27/A30而本批A只到A26。正文A节的A15–A20、A23–A25及A26–A30也沿用了错位编号。
新包须追加精确替代矩阵：gate_data对应真实init_idt_data锚（原A11/A12并补present位），gate_pack对应idt_init_desc（原A13/A14并补offset_low），table_write对应原A15，load_request对应修正后load_idt锚，stubs对应真实IDT_ALIGN/入口数组和A24–A26的合法原片段或新锚。不得只将缺失编号换成任意存在编号；每条必须支持其命题。
同时修正正文引用范围，登记A27/A30未在原证据文件定义；不编造它们是遗漏上传的内容。图中的宏调用语义按R65-S02追加正确读法。原63条全部标签虽存在且数目相加为63，标签计数正确不抵消引用关系错误。

## 4. R65-01丙：MANIFEST必填字段

固定MANIFEST路径和独立blob入口均已全文核对：缺read_channel、startup_selfcheck_quote、branch_canary_quotes三个字段。关于“已经读完规则”和“金丝雀命中”的叙述不能替代P3/P6明确要求的字段和逐字原文，尤其公约§1第2条整句并未给出。
在新66 MANIFEST明确填写这些字段，并以prior_manifest_disposition追加记录65缺项。金丝雀必须保留整行制表符、结尾反斜线/分号并分别对照固定time与direct master；公约原句须实际回读，不凭模板记忆。新的核对只能证明本次读取，不倒填65旧时工具过程。现有self_check三数字和produced_by字段并不缺失，无须制造这两项错误。
旧正式评论6743139361还给出了完整40位提交号；新补充评论只用真实短号，并说明旧评论保留审计。本轮不把评论中的长号混计为六文件P1命中，亦不编辑旧评论。
R65-01全部处置均并入66六件的D00，执行者只提交documentation_disposition_submitted，关闭权在C05。

## 5. R65-S01：保留技术主干及收窄条件

已回源支持：IDT地址选择、门构造/写入/加载请求分层；源mask的13/14位均为1；软件push顺序与pt_regs字段、现存generated偏移在显式8字节槽模型中对应；irq_nr用于vector而orig_ax用于错误码；公共入口sti在C分发及深层read_cr2之前；GP与PF各分支的正常返回前提不同。所有CPU帧、IDTR、实际指令长度/ABI与运行均未认证。
1. 帧中CS值改变不必然改变两次swapgs的选择；取决于两次(CS & 3)!=0的布尔值是否改变。布尔值相同只说明这两处的源级选择相同，不能单独证明初始GS、被调函数/嵌套行为及最终GS正确。不得把RPL1/2模型当真实用户态配置。
2. CR2风险还需要实际相关嵌套写入且外层原地址未在读取前恢复/保全。sti先于read_cr2只是源码顺序，不是已证明的事件原因、硬件窗口或CR2损坏；未核手册时保持显式假设。
3. in_atomic()==false只是当前所选preempt_count门；不是“必发生调度切换”或“恢复成别的栈”。EE65-F03主要是opaque调用和证据边界。旧CORE-SCHED-INTEGRATION-03的integration.md §1 C5已讨论中断后schedule和GAP-02；本轮只读该导航，不重签其旧状态或重新计作新根因。
4. MM零返回仍需经过后继本地irq操作、C返回、可能schedule及完整汇编尾部；不能从零直接跳到CPU接受iret或原指令成功。EE65-F05沿原MF12身份消费。七个返回帧字段/176字节是所选模型和现存头的一致性，不是重新编译sizeof/offsetof或物理压栈证明。
5. 有界源未见恢复/patcher不等于全仓没有；六项EE65是条件记录而非六项已证新漏洞，八项缺口及既有UC63/UR64/MF12状态均保持。

## 6. R65-S02：图及方法

原DOT全文转录后3384字节、84行，完整Git blob身份与005f30434c12所对应的远端完整对象相同；独立正则计36唯一节点、43边、0未定义端点。没有执行Graphviz。结构检查不认证边语义：common→push的CALL应按宏展开/局部控制理解，不是调用PUSH_REGS函数；idt→gate、gate→write是聚合摘要，真实setup经set_intr_gate调用helper，不能把聚合节点当直接函数调用。条件swapgs的跳过路径及RETURN门未穷尽，图不是完整CFG。ANALYSIS_STOP表示证据边界，不自动表示CPU停止。
本轮临时Python仅做自建数学/文本核查：一个22槽布局模型、7个mask例、16组CS布尔组合、4组调度门、6个地址分片重组例，以及A27/A30缺失集合和DOT文本身份/结构核验。不是作者22例原程序重放，不运行仓库代码。临时原始核查记录实际保存在/mnt/data/myos2_review65/independent_checks.json，仅本会话工作副本，未上传仓库。
六件全文均已读：facts按1–260、261–520、521–EOF760，evidence按1–330、331–EOF656，其余全文；MANIFEST另独立blob回读。原facts/deferred全文机械JSON解析、原63全量逐字/定义自动扫描、完整本地P9、预处理/编译/链接、offsets生成、仓库C/ASM/脚本/旧夹具、Graphviz、QEMU/实机均NOT_RUN。
新读固定源：common.c590至EOF；desc_arch.h全文；asm-offsets_64.c全文；generated/asm-offsets.h全文；idtentry.h1–25；desc_const_arch.h140至EOF；trapnr.h全文；myos_irq_vectors.h1–45；preempt_macro.h40–80；types.h1–25；irq_vectors.h26–44。idt.c、entry_64.S、interrupt.c、ptrace.h有关定义、fault.c、bugs.c及配置上下文复用本有效C05已读同pin原文，非执行者摘录。
为66划界另读switch_to_arch.h、switch_to_macro_arch.h、percpu_area.c全文，processor_arch.h1–240及process64_arch.c1–500（覆盖__switch_to完整体）；旧03 integration只读部分导航与C5，不称整批重验。public raw可选下载一次DNS失败，连接器仍正常；这是读取通道问题，不是仓库写权限失败。

## 7. 可读门与后继

P9-2定义归属及证据支持失败；P3/P6和P9-7/8所需字段不齐。其余真实路径、六件写区/净差异、实际标签数和明确NOT_RUN可保留；未完成项不得升级为通过。故65整包RETURN_DOCUMENTATION，A/B/C主干只能与本回执限定合读，不单独作为完整合格证据库。
按28/79同轮发布97号：D00处置R65-01；三个新专题为栈顶地址计算、TSS/每CPU发布、切换与入口消费。继续C05b/08，不轮换、不恢复旧C05a。只授权新66目录，旧成果及旧评论冻结，主线不代执行。

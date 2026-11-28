---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FIRST-RUN-FRAME-67-REVIEW-001
record_type: bounded_static_review_with_P6_accounting_return
evidence_class: 固定六件、直接源码与净差异核对；非内核运行验收
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；非后台认证）
model_per_owner: gpt pro（沿用Owner此前明确选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract98_short12: d1a8cee95e42
reviewed_base_short12: 6cffc97057c7
reviewed_head_short12: 157bf5604754
reviewed_delivery_comment: 6757793690
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R67-01]
subjects: {D00: ACCEPTED_AND_R66_01_CLOSED_SEPARATELY, A: CORE_RETAINED_WITH_LIMITS, B: CORE_RETAINED_WITH_LIMITS, C: CORE_RETAINED_WITH_LIMITS}
mandatory_supplements: [R67-S01, R67-S02, R66C-S01, R66-S01, R66-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original82_full_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
source_anchor_labels_enumerated: 82
non_source_prefix_hits_observed: 1
all_file_prefix_hits_established_lower_bound: 83
self_check: {verified_claims: 2, quotes_reconfirmed: 2, downgraded_to_inferred: 0, scope: 本回执两条调用点源码锚}
next_followup: CORE-USER-KTHREAD-ENTRY-68
next_task_ref: ../99-user-kthread-entry-contract.md
executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
inputs_read: [67完整六件及正式评论, 98合同及66回执, 固定入口汇编/直接返回工作/调用者/类型, 本有效C05同pin已读原文, 63号全文与协议v2计数要求]
open_questions: []
---

# 第67批技术主干保留；仅退P6自检计数

## 1. 真实交付和分别裁定

GitHub compare自6cffc97057c7至157bf5604754为ahead1/behind0、一个提交，仅六件added，共2169行：MANIFEST110、正文88、facts961、evidence742、DOT116、deferred152。旧文件零净修改/删除。直接lead与PR16为d1a8cee95e42，exec与PR17为157bf5604754，time=a039d9803ade，master=e8e60cd19540；两PR Open/unmerged，17 Draft，开放第三项为空。PR缓存base_sha不替代直接master。
六件及6757793690已完整读取，长件按段补齐。R66-01的D01及六处传播独立成立，另发66-DOC-CLOSURE回执，66组合材料PASS_PENDING_LOCAL。67自身有P6计数不一致，裁定RETURN_DOCUMENTATION、唯一R67-01；A/B/C技术主干保留，不整批重做、不修内核。执行者的STOP_WRITING是正式声明，不是对另一聊天进程的后台监控证明。

## 2. R67-01：82个源码标签不等于全文计数82

原evidence可逐组枚举D01一条、A01–A33共33条、B01–B25共25条、C01–C23共23条，总计82个源码证据标签。除此之外，固定facts-and-boundaries.yaml对象ffd2aade28c1的P9_text_subchecks键5的说明字符串，还原样含有左方括号紧接VERIFIED再接右方括号的独立示例文本。它在该句的“all new”之后、“occurrence 82”之前，不是路径加符号的源码标签。
协议v2 P6的指定grep前缀没有要求后接空格或路径，因此这个说明文本也会产生一次命中；P9-5同样要求MANIFEST数等于全文计数。82个已列证据标签加这一个已确认额外命中，已证明六文件总命中至少83，不可能仍为申报82。无需假装已经运行完整本地扫描才能指出这个反例；本轮未做原六件自动全文计数，不把下界冒充该程序输出。
这不是第83条源码证据，不是空引文，不是伪造了某个内核函数。不能将quotes_reconfirmed机械改为83，或把说明文字算一条INFERRED来凑数。原82条标签存在性与其逐条字节/定义/命题质量是另一层；本轮不因发现计数问题而宣布全部源码锚错误，也不认证82条全部通过。

最小追加处置放68的D00：
- 定点读取原facts键5、MANIFEST.self_check、原正式评论6757793690及协议P6/P9-5，登记82个源码标签、额外说明命中以及实际原始全文计数；若未运行完整计数，只能如实写已证下界，不编造精确工具结果。
- 新包的说明文字不要再次写出会匹配P6的保留前缀；仅真实源码标签使用该形式。在发布前按协议对新六件全文计数，与真正证据标签清单及MANIFEST三数分别核对，不能只数evidence标题或跳过facts中的说明。
- 用facts.prior_return_disposition及正文追加限制原“82等于全文计数、P6已过”的说法；旧67六件和评论不改。D00只提交documentation_disposition_submitted，由C05关闭R67-01。
仅此计数/自检记录需返工，68无需重新取证整个67，也不为凑配额增加引文。该退回不撤销66文档闭合。

## 3. 有界技术核对

A：copy_thread通过task_pt_regs和container_of定位fork_frame，统一写bp、ret_addr和thread.sp；PF_KTHREAD清零childregs并设函数/参数，非内核分支复制当前帧、置ax为0并可覆盖sp，args.fn非空再置ip/sp为0并设bx/r12。调用者copy_process先清PF_KTHREAD再按args.kthread设置，其后资源复制失败可阻止到达copy_thread。16份源台账中的类型别名、container_of与current_pt_regs已定点回源；未写槽不推为必零或必垃圾。
B：六次push/pop、prev/next thread.sp的保存与装入、尾jmp __switch_to及其条件正常return，与新帧ret_addr来源相接。56/176/232字节和ret_addr偏移48是在8字节槽、无额外填充、合法对象与ABI条件下的模型，不是编译布局。进入ret_from_fork_asm后rax/rsp/rbx/r12重排为四形参；不能由字段命名证明实际首次运行。
C：两处调用点确实位于不同层。C层：

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
	if (unlikely(fn)) {
		fn(fn_arg);
```

汇编层：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	callq		*%rbx
```

完整ret_from_fork及汇编标签表明，后一次调用在C整体正常返回后、非空RBX分支内。直接读取schedule_tail确认其活动preempt_enable及被注释的finish_task_switch；直接读取entry/common.c确认退出工作会调用arch_do_signal_or_restart。上述调用可能改变帧或不正常返回，必须在最早无效对象或不返回callee处停止，不跨过它们宣称第二次调用发生。

## 4. R67-S01：调用目标、参数与帧的分层

1. FR67-F01是条件调用重叠，不是“所有内核线程都执行两次”。还须区分首帧中的fn和包装器内部的业务threadfn；真实构造者尚未由67完整追踪，68补这条边，不预签新结果。
2. 同一目标函数要求第一次C调用后活跃RBX仍为原fn；同一参数还要求活跃R12保持原fn_arg。共同ABI/栈前提不能只缩写为“保存帧里的bx仍非零”。用户返回工作改写regs中的bx/ip等，不自动等同改写当前活跃RBX/R12；反之，帧有效也不单独证明活跃寄存器被保留。
3. PF_KTHREAD选择copy_thread的构造分支，后续汇编测试的是RBX，不重新读取task.flags。函数可能修改任务或寄存器帧，不能用后来的flags值替代真实分支条件。FR67-F03仍需实际构造者允许NULL入口，以及返回工作没有建立有效用户帧等前提。
4. bp在整数cast之后加1，不能解释成指针加一个pt_regs；但数值单位不同本身并不证明错误，可能是编码/哨兵或无消费者。FR67-F02保留needs_evidence，不将其定性为已证栈破坏。
5. C15标题留有“六行需缩至...”编辑说明，真实围栏是5行、附带区间284–288，下一条C16才是间接call。按真实片段与C16合读，不指控超过5行或删除源码空行，不为标题另派返工。
FR67三项及八个缺口、SB66/EE65/09原状态均保持。首帧构造、C返回、汇编分流、CPU接受返回状态和用户指令执行仍是不同证据层次。

## 5. R67-S02：图、模型与覆盖限制

原DOT全文已阅读；46节点65边为作者申报，本轮不冒称重新运行计数/Graphviz或验证完整CFG。schedule_tail→firstfn、firstfn→exitwork等RETURN边只能理解为调用者中的条件后续顺序，不是这些callee彼此直接调用；聚合dup节点不证明完整分配生命周期。ANALYSIS_STOP表示证据边界而非CPU停机。
本轮自建Python仅核四个地址投影、八组K/F/S分支投影、八组第一次返回/退出工作返回/RBX保持条件，以及从固定facts复制的一条说明句的额外前缀命中。记录实际保存于/mnt/data/myos2_review67/independent_checks.json，未入库；不是作者JS程序重跑、原六件自动扫描或内核运行。
六件覆盖：evidence按1–270/271–530/531至EOF742；facts按1–330/331–660/661至EOF961；正文末段70至EOF补齐；MANIFEST/deferred/DOT全文。82条短锚逐组阅读，关键定义/命题定点核，未执行全82自动逐字/定义扫描或原facts/deferred全文JSON解析。
本轮新读：entry_64.S全393行、entry/common.c全文、scheduler_core.c860–912（含完整schedule_tail）、fork.c916–1166及1390–1690（含kernel_clone/kernel_thread完整体）、sched_type_declaration_arch.h/stddef.h/sched_misc_types.h全文、ptrace.h375–390、options_flags.cmake全文。process_arch.c完整copy_thread/ret_from_fork、switch_to类型与宏、__switch_to完整体、task_struct和生成头等复用本有效C05可见同pin原文，不借执行者已读声明。
为68划界另读固定kthread.c全文及init/main.c全文；默认分支搜索仅用于定位，结论回到固定time。可选容器raw读取一次DNS失败，未取得字节；连接器正常。这不是GitHub写入错误。
规则复用98、28/79及既有项目权限，本轮刷新63全文和协议计数要求；不宣称再次全量加载Mnemosyne、不接其维护任务。完整P9、仓库C/ASM/脚本/夹具、预处理/编译/链接、偏移生成、Graphviz、QEMU/实机均NOT_RUN。按28/79同轮发布99号，保持C05b/08执行、C05验收。

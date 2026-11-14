---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EXIT-REAP-73-REVIEW-001
record_type: independent_bounded_review_with_documentation_return
evidence_class: 固定六件全文、净差异与关键源码独立回读；非运行证明
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract104_short12: e482eb76bbba
reviewed_base_short12: 4cd92f424986
reviewed_head_short12: 8bc7583e45c9
reviewed_delivery_comment: 6810987102
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R73-01]
subjects: {A: TECHNICAL_CORE_RETAINED_WITH_LIMITS, B: TECHNICAL_CORE_RETAINED_WITH_LIMITS, C: TECHNICAL_CORE_RETAINED_WITH_LIMITS}
mandatory_supplements: [R73-S01, R73-S02, R73-S03]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original56_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
Graphviz: NOT_RUN
reviewer_models_executed: false
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四条直接短引文，人工回源，不是原56条机械重验}
next_followup: CORE-USER-TASK-STORAGE-LIFETIME-74
next_task_ref: ../106-user-task-storage-lifetime-contract.md
executor: MYOS2-A-C05c / MYOS2-LEAD-002-CHAT-EXEC-09
executor_rotation: false
owner_decisions_requested_now: []
phase: 2
inputs_read: [第73批固定六件及正式评论, 104合同及105接收边界, 固定源码与本文列明的历史材料]
open_questions: []
publication_note: 真正发布短提交及导航、唯一评论的后验在正式主线评论中记录
---

# 第73批独立审查：技术主体保留，一组文档纠正

裁定RETURN_DOCUMENTATION，唯一退回组R73-01。不是六件漏交、不是内核修复要求，也不要求重做73。三个技术专题按R73-S01/S02/S03保留；文档纠正并入74的D00，完成后仍由C05独立裁定关闭。72及更早的文档闭合、58/63误判撤回均维持原状。

## 1. 交付身份及实际方法

独立compare 4cd92f424986→8bc7583e45c9：ahead1/behind0，一个提交，严格六件added，共1741行；旧文件零净修改/删除。六件为MANIFEST144、正文77、facts700、evidence522、DOT100、deferred198行，均已阅读全文，长件连续分段补齐。正式评论6810987102与09身份、固定提交对应，执行者申报STOP_WRITING；这不是另一聊天物理进程的监控结论。

原A16+B15+C25共56个引文标签已读；主线独立核对退出主干、列表/namespace、包装返回类型和死亡交接等强制结论，未用56/56自报替代核查。原56逐字/定义全量自动扫描、六件机械前缀扫描、原facts/deferred完整JSON解析和完整本地P9均未重跑。DOT按可读结构消费作者46节点/47边申报，不宣称本轮计算了原图Git哈希或重放了28个模型。

## 2. R73-01：追加纠正，不改原件

### 2.1 三个成员引文的定义身份

原evidence的A16、B13、B14都标为mykernel/sched/task/task_types.h::task_s。成员文本确实存在，但该文件真正的定义是task_struct；task_s只是另一个文件中的类型别名。定位依据：

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
	struct task_struct {
```

[VERIFIED mykernel/sched/sched_type_declaration.h::task_s]
```c
	typedef struct task_struct task_s;
```

P2要求引文位于标签所指真实定义内部，不能用可等价使用的类型别名代替错误的定义位置。74 D00应将三个原片段分别重锚到task_types.h::task_struct，逐一列旧ID→新ID。不是虚构成员或空行问题；不要求改原引文的语句、不重做其它53个标签，也不宣称其它53条已全部重新认证。

### 2.2 MANIFEST结构化字段

原MANIFEST的G00正文有两条金丝雀原行，但完整144行未提供协议P3要求的branch_canary_quotes字段。该缺陷是记录结构缺项，不是没有给出金丝雀文本，也不是已发现读取错误分支。
74自己的MANIFEST必须提供该字段，并在prior_return_disposition中追加73的对应字段、实际回读版本、两条原文及time/master差异。不得向旧73文件补字段。read_channel和startup_selfcheck_quote原本已存在，不误报为缺失。

### 2.3 状态矩阵的实参文字错误

facts.exit_entry_status_matrix中sys_exit_group的operation写了“then do_exit(current)”。固定实现实际传递exit_code，不是task指针：

[VERIFIED mykernel/sched/forkexec/exit.c::do_group_exit]
```c
	do_exit(exit_code);
	/* NOTREACHED */
```

正文对这一点原本正确；D00只需给出替代表项和对应源码锚，明确“作用于当前任务”与“实参是current”不同。不得据此新报内核传错实参。

### 2.4 传播、自检及关闭门

D00同时覆盖原正文/事实表对这三个证据ID的消费、MANIFEST与facts中的全部定义检查合格声明、正式评论6810987102的56/56总括说法。56仍是原标签数，不再当作56条均通过定义身份门；新引文数量另计，不能把新锚回填成历史原件已正确。原评论保持不变，由74唯一正式交付评论追加说明。
D00纳入新74六件的正文、facts、MANIFEST和evidence，不增第七件。只提交documentation_disposition_submitted，不自关R73-01。主线在74返回时独立核收；截至本回执该项仍OPEN。

## 3. R73-S01：状态码、未初始化值和返回消费

exit与exit_group的局部编码差异真实；前者int经long形参再写int成员，后者先低8位左移。针对所选int实参的有限算例与任意long调用者的窄化须区分；负数算例应标明目标整数模型。ER73-F01仅是待证接口契约候选，不因两个值不同就认证POSIX违规、运行回归或已知父进程读到了错误状态。
wait4为空非void主体，包装层实际取得其返回值后继续返回；不能给出稳定的0、errno或PID。未实际调用该接口时，也不能凭空把整个系统认定为已触发这一行为。
group_dead的活动赋值缺失，实参求值是可证性停止点；被调函数目前不使用该形参不能消除调用处的问题。条件性源码前缀说明不等于未定义行为之后甚至之前的机器副作用都被保证保留。UV-15/PID11已有根因不另编号。

## 4. R73-S02：父子关系与对象边界

exit_notify的真实外层条件是children.count != 0；图中“>0”只作为合法正计数场景的摘要，不替换源码。remove_head返回有效节点需要列表、count及调用过程有效，空节点自旋是源码控制意图，不认证编译后恒定行为或硬件持续自旋。
PIDTYPE_PID是值0的枚举常量；其作为整数常量零可形成空指针，不能把这里描述成必然需要编译诊断的类型错误。核心问题是没有提供有效namespace对象，后续ns->idr访问不因转换可接受而安全。应在最早无效对象处停止，不继续补出正常NULL、-ESRCH或真实改挂成功。
仅在假设目标任务和列表有效时可分析节点移动；它不自动写child.parent/real_parent，更不证明通知、ZOMBIE或reap完整。原UV-15关于必然改挂和每次退出必然留下全部资源的总括说法，继续按本批限定收窄，不覆盖原记录或关闭状态。
files.refcount、fs.users、file.f_count是不同对象的账；正常正计数、无其它写者和所有先行callee正常返回是各自局部减量模型的前提。清指针/请求free不等于物理回收，R44/R45/R15既有限定继续适用。

## 5. R73-S03：死亡交接、图和补充终止路径

do_task_dead在__schedule返回后不仅有BUG，还存在不依赖BUG终止效果的源码后备循环：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	for (;;)
		cpu_relax();
```

因此不能把BUG可能为NOP解释为该函数随后正常返回；这仍不证明调度器永远不会重选该任务、真实上下文切换成功或回收完成。PF_NOFREEZE写入也是所读体的活动动作，不与TASK_DEAD或exit_state混同。
context_switch→finish_task_switch只在对应恢复后的C路径成立；schedule_tail中的同名调用和finish的死亡回收块均是注释，不由函数名补回。rq.prev_mm=NULL不能代替MM引用归还。
图S10是未初始化局部的描述，实际求值停止点在传参，不是声明本身必然触发。S27→S28是压缩的namespace数据/调用关系，真实函数链含find_task_by_pid_ns→find_pid_ns→idr_find；不得当成参数对象主动发起CALL。分析停止、注释及假设有效目标分支不是CPU CFG。原图数值和作者28例模型不升级为运行证明。

## 6. 实际来源、复用及下一批

本轮直接读取固定time：exit.c全文；syscall.c62–145；syscall_wrapper.h227–末尾；syscalls.h1378–1395；syscalls_64.h全文；task_types.h1–85与140–245；sched_type_declaration.h全文；pid.c298–390；pid_const.h1–22；double_list.h600–末尾；fs_struct.c85–110；process_arch.c25–55；scheduler_core.c780–900及1168–1190；scheduler.h150–220。file.c及x64_sys_call、同pin金丝雀与既有R44/R45/R15限制复用本有效主线此前实际读取内容，不冒称本轮再次全读。
历史05的UV-14/15/16原deferred620–820本轮回读；PID11和72/71回执及104/105完整固定正文沿本有效上下文消费。28号、79号本轮完整刷新；00/18/24/63及协议v2、适用Mnemosyne指导沿已有同pin有效加载记录，不导入其它项目维护。新74不是新执行对话，不重放105接收仪式。
为74划界另读fork.c1–370和pid.c389–475，默认分支搜索仅定位。74聚焦任务结构存储、内核栈释放条件、借用指针与持有引用及实际销毁入口，结合R73-01 D00；不重做整个exit、PID、MM或调度器。ER73一候选九缺口和UV/CG72等历史状态维持。
本轮没有运行原图、原模型、仓库脚本、预处理/编译/链接、QEMU/实机或额外付费模型；没有内核/旧成果修改、合并、新分支PR或阶段3授权。仅新增本主线回执、106合同和检查点，并维护自身导航/正式评论。

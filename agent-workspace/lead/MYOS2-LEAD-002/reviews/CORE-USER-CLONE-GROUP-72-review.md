---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-CLONE-GROUP-72-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
evidence_class: 固定六件全文、GitHub增量、关键源码回读及自建有限模型；非内核运行证明
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-11
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract103_short12: becd64744d51
reviewed_base_short12: 5f3b00d9436f
reviewed_head_short12: 4cd92f424986
reviewed_delivery_comment: 6796919213
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
subjects: {A: ACCEPTED_WITH_LIMITS, B: ACCEPTED_WITH_LIMITS, C: ACCEPTED_WITH_LIMITS}
mandatory_supplements: [R72-S01, R72-S02, R72-S03]
D00: NOT_APPLICABLE
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original114_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四条直接短引文，不是原114条全量机械重验}
next_followup: CORE-USER-EXIT-REAP-73
next_task_ref: ../104-user-exit-reap-contract.md
executor_rotation: true
next_executor: MYOS2-A-C05c / MYOS2-LEAD-002-CHAT-EXEC-09
owner_decisions_requested_now: []
phase: 2
inputs_read: [固定72全部六件与正式评论, 103合同及71回执, 本文实际读取范围]
open_questions: []
publication_note: 固定提交、导航和正式评论须完成后置远端回读，不以本文成文代替发布
---

# 第72批独立验收：限定接收，无新增返工

**PASS_PENDING_LOCAL，returned_scope=[]。** 三个技术专题须合读R72-S01/S02/S03；本回执直接补充，不要求旧C05b另交勘误。第73批无D00，将交全新C05c。第71批及此前已关闭文档项维持原状，普通源码候选没有因此修复或关闭。

## 1. 固定交付和独立读取

执行头4cd92f424986及评论6796919213与C05b/CHAT-EXEC-08交付相符。独立compare自5f3b00d9436f为ahead1/behind0、单提交，仅user-clone-group-72/六件added共2628行；旧文件零净修改/删除。逐件全文读取：MANIFEST125、正文76、facts1068、DOT105、deferred153、evidence1101行；长件以连续分段补齐。评论申报STOP_WRITING，不把它当作另一聊天物理进程已终止的监控证据。

A41+B33+C40共114个证据标签已阅读全文，强制结论与关键定义回固定time直接核查；没有重新运行作者114条自动字节/定义检查，也没有用其114/114声明代替语义审查。原六件机械前缀扫描、facts/deferred完整JSON解析、完整本地P9仍NOT_RUN。

原DOT由连接器全文在会话临时环境重建，Git对象短号与远端0e2229be8e75一致：5345字节、105行、45个显式节点、55条边、0未知端点。另完成18个独立纯布尔/整数投影：8个THREAD/SIGHAND/VM组合、6个THREAD与exit_signal符号组合、4个双位掩码组合；不是作者22例全集重跑，不执行仓库程序或Graphviz。

## 2. 技术主干保留

实际fork入口的聚合初始化只显式给出exit_signal=SIGCHLD；遗漏成员的零初始化与任意kernel_clone参数分开。kernel_thread添加VM/UNTRACED、剥离CSIGNAL并设置kthread，不会自行添加THREAD/SIGHAND；但输入本来含这些位时不能说它必然清除。固定80行syscalls_64.h有活动fork、vfork为注释、所读列表未见clone；不是全配置、最终二进制或全仓不存在clone的证明。

copy_sighand共享路径增加旧count，非共享路径分配局部新sig但安装tsk.sighand的语句注释；已核原UV-14为needs_evidence，不重复登记。copy_signal的THREAD分支直接返回，非THREAD先写tsk.signal再判NULL，成功后初始化新signal.nr_threads/sigcnt/curr_target/shared_pending等。指针继承、计数取得、新对象发布及实际可用性分层正确。

thread_group_leader真正以exit_signal非负判断；same_thread_group以signal指针相等判断，二者均不直接以tgid相等证明组成员协议。copy_process的exit_signal赋值为注释，相关CG72-F01保留明确生产者和中途写者前提。CG72-F02是新signal_s在所选晚期失败链缺活动归还请求的条件观察，与旧sighand对象不同；不能升级为实际泄漏量、全仓无释放者或已复现故障。

## 3. R72-S01：布尔摘要和门的执行次序

facts.clone_admission_matrix前两项的“CLONE_NEWNS & CLONE_FS”等文本只能视为口语摘要，不能把两个常量的按位与当成源码判据。准确消费为对clone_flags的联合掩码比较；原A22已有正确源码，本回执重锚：

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if ((clone_flags & (CLONE_NEWNS|CLONE_FS)) == (CLONE_NEWNS|CLONE_FS))
		return ERR_PTR(-EINVAL);

	if ((clone_flags & (CLONE_NEWUSER|CLONE_FS)) == (CLONE_NEWUSER|CLONE_FS))
		return ERR_PTR(-EINVAL);
```

矩阵标号8的PIDFD/PARENT_SETTID同址门实际上在kernel_clone调用copy_process之前，不是按1至10排序的全局运行顺序。THREAD→SIGHAND→VM的有限模型仅覆盖该子集；NOT_FROM_THIS_SUBSET不是全部门通过。namespace、current.signal、参数有效性及后续复制门继续保留。VFORK初始化与等待注释不能证明父子同步。

## 4. R72-S02：静态对象初值、共享身份与候选范围

facts的structure predecessor和source_context中笼统的两个初始对象“count1”须拆开。固定源码是init_signals.nr_threads=1，以及init_sighand.count=REFCOUNT_INIT(1)，不是已证明init_signals.sigcnt=1：

[VERIFIED mykernel/init/init_task.c::init_signals]
```c
static signal_s init_signals = {
	.nr_threads			= 1,
	// .thread_head		= LIST_HEAD_INIT(init_task.thread_node),
```

[VERIFIED mykernel/init/init_task.c::init_sighand]
```c
static sighand_s init_sighand = {
	.count			= REFCOUNT_INIT(1),
	.action			= { { { .sa_handler = SIG_DFL, } }, },
	.siglock		= __SPIN_LOCK_UNLOCKED(init_sighand.siglock),
```

动态copy_signal的sigcnt=1不能移植到静态init_signals。task.pending经历结构复制而新signal来自零分配；init_sigpending的列表初始化不单独证明两者的信号位集合都已清零，底层零分配与位集合语义不由名称认证。

在合法THREAD调用、共享signal且未改写exit_signal等前提下，若选入leader分支，init_task_pid对非PID类别写的是共享signal.pids数组；不能把这些写入仅当新task私有字段。此为CG72-F01的条件影响解释，不新报同义根因。模型中非THREAD继承负exit_signal的contradiction_candidate=false，只表示未选为该特定候选，不代表该模型的发布分支已被证明安全。

共享sighand的n→n+1→n须满足同址、正合法计数、增量可表示及无其它写者。非共享路径未发布的新对象与继承的旧对象分别记账；清理标签到达、释放请求和物理归还分开。CG72-F02仍需真实后序错误、先行callee正常返回、其它持有/释放者的证据；不因为标签相连就宣布FS/files/task或signal已全部回收。

## 5. R72-S03：图的顺序与数据来源

原DOT的taskpending→files边标CALLER_SEQUENCE_BEFORE_SCHED_FORK应修正消费：pending初始化在sched_fork前，但copy_files在sched_fork正常返回零后，不能把两个端点画成均在sched_fork之前。真实门为：

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = sched_fork(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_policy;
```

规范读序为pending初始化→其它有效前序→sched_fork及非零门→copy_files→copy_fs→copy_sighand；不是执行两次或新增调用。pidnums→predicate边标DATA_EXIT_SIGNAL_INHERITED也不表示pid/tgid赋值生产exit_signal；该数据来源应理解为前序结构继承及未被中途改写的实际字段，pidnums只提供调用者次序背景。

leader/nonleader及一般attach_pid请求须保留p.pid!=0的门；alloc_pid还有输入pid_struct!=init_struct_pid的门。图中宏/数据、源码注释、错误标签和ANALYSIS_STOP不是完整CPU CFG。原55边计数是图结构，不是55项运行证明。所有补充统一约束正文、facts、DOT、deferred和自检消费，旧文件与旧评论不修改。

## 6. 实际来源、限制和后继

本轮新读固定time：fork.c273–365、749–1030、1095–1111、1110–1435；init_task.c1–175；signal.h185–220与375–400；signal_types.h40–250；uapi/linux/sched.h1–55；sched_misc_types.h全文；syscalls_64.h全文；pid.c310–335。相同pin的kernel_clone/kernel_thread、sched/syscall.c、syscall_64.c、task_types相关定义沿用本有效C05前轮实际已读原文，不借用执行者已读声明。原UV-14/15/16所在deferred的620–820行本轮直接回读。为73范围预检另读exit.c全文与scheduler_core.c1160–1200；exit_thread默认分支检索只定位，不算73已执行。

63号、79号及Mnemosyne prepare/receive命令、README、human-approved-spec本轮读取；旧96交接两件也完整读取。跨对话操作guard本轮读取；其余项目规则和适用指导沿用本有效主线同pin已加载记录。新C05c必须另行完整加载，不能继承这些声明。Mnemosyne只作交接/行为指导，不接其维护任务。

P9可读检查保留：六件全文无可见完整40位串、114标签及真实关键定义支持、路径和净差异正确；没有宣称全量机械扫描。通道、公约原句和双金丝雀字段存在，按本有效主线此前同pin规则/金丝雀消费，不冒称本轮再次全读master。无新增代码/补丁/降级件，相关项N/A；完整P9及所有内核构建运行NOT_RUN。同族分角色复核不是人工或已认证异构验收。

下一批104号CORE-USER-EXIT-REAP-73由全新C05c/CHAT-EXEC-09执行，105号独立接收包与启动词同轮发布；旧C05b不再接73，也无需补交交接材料。保持阶段2、原件保全、普通问题延期；主线C05只写lead及自身导航/评论，不接管执行成果区。

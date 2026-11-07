---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SYSCALL-STDIO-08-REVIEW-001
record_type: bounded_static_analysis_review
conversation_display_name: "MYOS2-A-C03 内核分析主线续建"
produced_by: "ChatGPT 普通对话（Owner报告当前选择：gpt pro）"
produced_by_source: "Owner本轮明确说已切换到gpt pro；不是直接观察界面或后端认证。"
model_per_owner: "gpt pro"
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
human_technical_review_attested: false
date: 2026-10-28
base_snapshot: "source=time；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002"
read_channel: connector
evidence_class: "GitHub固定对象读取、代表源码语义复核及已交机械记录的有限消费"
authorization_ref: "Owner确认旧对话已完成交接，并要求查验08、设计下一步；原工作令及18号约定。"
handoff_confirmation_ref: agent-workspace/lead/MYOS2-LEAD-002/09-continuation/03-C02-to-C03-handoff-CONFIRMED.md
handoff_confirmation_short12: 88333713a4a0
reviewed_pr: 17
reviewed_head_short12: 8983a9235c8a
reviewed_task_base_short12: ab3b4b86662c
kernel_source_short12: a039d9803ade
current_default_ref_short12: e8e60cd19540
status: BOUNDED_REVIEW_WITH_LOCAL_DOCUMENTATION_RETURN
acceptance_verdict: RETURN
returned_scope: [SR08-01, SR08-02]
retained_scope: "08实际交付及代表静态链路；须按本回执的消费限定使用，不整包作废。"
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
reviewer_execution: false
kernel_change_made: false
patch_or_diff_produced: false
external_execution_started: false
phase3_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-RETURN-SIGNAL-09
next_taskbook: agent-workspace/lead/MYOS2-LEAD-002/23-user-return-signal-contract.md
next_gate: "09先完成D00两项文字处置，再在同轮继续D01-D04；不重跑08，不修内核。"
self_check: {scope: this_review_only, verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, method: manual_connector_readback_not_parser_execution}
inputs_read:
  - "接收轮已完整读取的工作令（含§12）、公约、协议v2、18号、07回执、22号任务书、08 MANIFEST/evidence及交接材料；本轮核执行头未改变。"
  - "本轮GitHub PR16/17元数据、主线确认提交、开放PR枚举、两PR全部变更路径、ab3b至8983比较、master/time/lead直接ref。"
  - "08主说明§0-9及§10开头；facts-and-boundaries.yaml第1-80行；deferred-findings.yaml中的US-01、US-02、US-03主体；未声称读取被工具截断的尾部。"
  - "08 records/run.json全文；records/ss_check.json第1-90、420-501行；scripts/ss_check.py第580-709行。"
  - "time固定源码：options_flags.cmake、myinitramfs/CMakeLists.txt、fs/syscall.c、fs/vfs/fcntl.c、include/linux/fs/file.h、fs/vfs/file.c、arch/x86_64/entry/common.c、arch/x86_64/entry/syscall_64.c、arch/x86_64/include/asm/syscalls_64.h、entry/common.c全文。"
  - "time固定源码：fs/vfs/read_write.c第312-407行、fs/vfs/file_table.c第30-135行、arch/x86_64/entry/entry_64.S第144-230行、arch/x86_64/lock_IPC/signal/signal.c第1-330行、signal_64.c第160-300行。"
review_limits:
  - "没有逐条语义审计全部70锚点、38检索及3085行事实YAML，没有重算41项表的每个定义与计数。"
  - "检查器只定点读其汇总与边界逻辑；没有完整审计或重跑，没有运行哈希、解析器、C/ASM、构建、内核、QEMU。"
  - "默认分支搜索只作路径导航；源码判断重新从time固定对象读取，不把默认分支搜索片段当time事实。"
  - "未读取仓库外libc、构建产物、镜像或用户开发工作树；未证明实际系统调用序列、账户额度或执行者后端身份。"
open_questions:
  - "GAP-U10继续open：构建文本指向仓库外libc，实际采用的工具链、库版本和二进制未核。"
  - "返回尾段的信号选择、建帧、恢复和重启组合尚未完整审查，交给09限定静态补证。"
  - "SR08-01/02的执行方文字处置尚未交付；不能把本回执记成08整包PASS_PENDING_LOCAL。"
---

# 08主要证据保留；只退回两处报告表述，接续返回尾段

## 1. 结论与下一动作

08确已交付。主线已核对其代表源码链路、机械结果记录和GitHub变更边界。**不是要求重做08，也不是内核修复任务。** 保留C01的仓库内/外边界、C02活动分发入口、C03文件表和引用路径的有限静态证据；两处过度概括按SR08-01/02退回作小型文字处置。处置前只能按本回执的收窄版本消费，不采用原来的无条件概括。

下一项为CORE-USER-RETURN-SIGNAL-09：D00先追加08文字处置，再继续返回用户态前的信号、寄存器帧与重启静态补证。08旧十件及05/06/07均保持原样。普通问题继续入库，Owner当前无需决定修复方案。

## 2. 实际GitHub状态与证据强度

[VRF] PR17仍open、Draft，执行头8983a9235c8a。GitHub compare显示ab3b4b86662c至该头仅一个提交、10个新增文件，全部位于user-syscall-stdio-08/，没有旧文件净修改或删除。这个远端比较是本轮写区结论的主要依据，不是只相信执行者的boundary记录。

[VRF] PR16当前接收起点88333713a4a0含C02的CONFIRMED文件；Owner又在本轮确认交接完成。C03继承既有lead单写者，CC02继续独占执行分支。开放PR枚举与逐个元数据一致为16/17；按创建时间倒序、每页一项的第3页为空。两PR变更路径分别在lead/和results/，本轮新增记录与PR17结果文件无路径交集；没有新建竞争分支/PR。

直接ref核得master=e8e60cd19540、time=a039d9803ade。PR元数据中的base_sha返回了旧值，本轮不把该字段当最新master；继续原PR谱系，不重置或重建分支。所有短SHA均从连接器输出复制。

机械记录的消费：run.json申报returncode=0；已读ss_check摘要记录70/70锚点、34/34正文引文、38项检索、23/23结构检查、两张值表通过，all_ok=true。这些数字属于执行方已入库运行记录，不是主线独立运行结果；正则匹配及数值一致不能证明条件完整或运行正确。

## 3. SR08-01：文件引用后果须保留有效对象和成功分支条件

**受影响处：** user-syscall-stdio.md §0、§4.3/4.6、§5.2、§9及US-01标题/影响概括；事实YAML相应摘要由执行方定点查找。US-01详细claim/premises已经写到成功F_DUPFD/F_DUPFD_CLOEXEC，不能据此说整条发现错误；问题在摘要并未一致保留条件。

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
```

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	if (filp != NULL) {
```

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
	if (!f.file)
		goto out;
```

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(argi, filp, O_CLOEXEC);
		break;
```

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
	return err;
```

[INFERRED] 正确消费口径：在合法表内fd指向可用的非空file、控制流实际到达清理点的前提下，write/writev的fdget不增加引用，而fdput减少引用；原计数为1时到达fput中的释放调用。空槽返回-EBADF的路径不执行该fput。fcntl成功F_DUPFD和成功F_DUPFD_CLOEXEC均有get_file与fdput相抵；不能只排除前一种。清理未正常到达、失效指针或越界行为不能套入确定的净引用公式。两槽共享后的引用不足以及CLOEXEC标记不实现是另外的既有事实，不因这个文字更正而消失。

**D00完成标准：** 给出非空有效file、空槽、成功两种dup命令、失败/其他命令四类条件；定点列出08需被新说明替代的位置。只新增处置，不改08旧文件、不重跑08。

## 4. SR08-02：分发阶段的ax不是无条件的最终用户返回值

**受影响处：** user-syscall-stdio.md §3.1末段、§3.2“用户看到的rax”表、§5调用图与相关事实项。08已经把信号内部列为未覆盖；因此其外层表格也必须保留这层条件。

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]
```c
			arch_do_signal_or_restart(regs);
```

本轮已读该函数全文：外层循环及信号标志条件被注释，但上面这条调用是活动语句。do_syscall_64在分发后调用syscall_exit_to_user_mode，并非把ax直接送出。

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]
```c
		case -ERESTART_RESTARTBLOCK:
			regs->ax = get_nr_restart_syscall(regs);
			regs->ip -= 2;
			break;
```

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]
```c
	regs->ax = 0;
```

[INFERRED] 源码已足以反驳“处理函数返回值无条件等于用户最终观察值”。应区分分发前保存值、分发完成后的regs->ax、返回工作处理后的帧以及真正返回用户态的观察。无信号交付、无重启改写且正常走到返回时，才能连接为普通返回路径；信号处理可能改ax/ip/sp，其他分支也可能不返回。这里没有声称已在内核触发这些路径。

另须说明号检查使用entry_SYSCALL_64中的movslq %eax及C层uint转换后的号；不能把检查范围泛化到任意原始64位rax。非法号时“ax未由分发写入”是局部事实，不是所有返回尾段均保持原值的证明。

**D00完成标准：** 将上述表格改成“分发局部结果＋最终返回前提”的追加说明；未覆盖的信号链交D01-D03，不把旧GAP-U04或GAP-U10自动关闭。

## 5. RC08-01：检查器边界记录的消费限定（不是第三项返工）

定点阅读发现：boundary中的树对象比较只保留短前缀，且变更越界/旧件变化不会直接决定非零退出码。

[VERIFIED agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-syscall-stdio-08/scripts/ss_check.py::tree_of]
```python
            rows[path] = (mode, obj[:12])
```

[VERIFIED agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-syscall-stdio-08/scripts/ss_check.py::boundary]
```python
    return 0
```

边界模式还可能因Git命令失败或HEX40输出检查失败而退出；这里不说它任何输入都返回0，而是说**不能用其退出码0单独证明写区安全或完整对象身份一致**。本轮已用独立GitHub compare核对08实际十件新增，所以没有由此否定当前交付边界。记录中的“全部对象相同”最多按该脚本的比较精度解释。

不另起通用验证器修复。09的写入边界用实际diff、完整对象内部比较或明确的布尔检查；发现越界就停止提交。持久公开文档仍只写允许的短提交，不把短号展示限制误用于内部完整对象比较。

## 6. 继续保留的边界

C01：CMake文本确实先project再设编译器，且只加入myinit/myshell。这支持构建意图和仓库外边界，不能独立确认构建实际采用的libc；三栏表里直接写“实现在musl”应按其详细说明理解为构建文本指向，而不是二进制认证。GAP-U10保持open，不为此再次扫仓库或偷偷读取外部libc。

C02：已回源核活动switch、表头、默认未实现分支和汇编衔接；41项定义唯一性与全部返回计数仍是执行者检查记录，没有主线全表独立重算。C03已回源核fdget/fdput、f_dupfd、文件表复制与释放的关键函数；其余条目按已交证据保留，不在有限审查中提升为全部语义已证。

只记录普通内核问题，不提出当前修复问卷。无内核修改、无补丁、无主线命令运行、无合并/关闭/删除分支。PR17保持Draft，主线PR16继续使用。

## 7. 为什么下一项是返回与信号，而不是重跑08

08已把活动syscall分发接到文件I/O，仍明确没有展开syscall_exit_to_user_mode内部。本轮发现它实际会调用信号/重启处理，直接影响如何解释已报告的返回值。因此选择同一固定源码上的有限补证，而非运行测试、外部libc追踪、整库重新盘点或阶段3综合。

09按冻结问题逐条追踪可由次一档执行；涉及未给定判据的开放冲突只报告候选，交当前Owner已选择gpt pro的主线裁定。不自动调用额外付费模型、API或外部研究。Deep Research和额外并行评审当前不需要。

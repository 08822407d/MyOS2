---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-RETURN-SIGNAL-09-REVIEW-001
record_type: bounded_static_analysis_review
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话，Owner报告本轮已选择GPT Pro
produced_by_source: Owner本轮指令，不是后台身份认证
model_per_owner: GPT Pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
human_technical_review_attested: false
date: 2026-10-30
base_snapshot: source=time；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
read_channel: connector
evidence_class: 固定GitHub对象、代表源码语义核对、执行方机械记录的限定消费
authorization_ref: Owner要求先准备独立ChatGPT执行对话启动提示词，再由本主线验收09；00工作令、18号与24号澄清
reviewed_pr: 17
reviewed_head_short12: 67b2458b4a6a
reviewed_task_base_short12: 8983a9235c8a
kernel_source_short12: a039d9803ade
master_ref_observed_short12: e8e60cd19540
lead_head_before_review_write_short12: 961b8bf2635b
status: BOUNDED_REVIEW_COMPLETE_WITH_LOCAL_DOCUMENTATION_RETURN
acceptance_verdict: RETURN
returned_scope: [SR09-01]
retained_scope: D00核心处置、D01-D03代表静态链路及D04交付边界；受本回执消费限制约束
prior_return_disposition:
  SR08-01: RESOLVED_IN_BOUNDED_REVIEW
  SR08-02: RESOLVED_IN_BOUNDED_REVIEW
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
reviewer_execution: false
kernel_change_made: false
patch_or_diff_produced: false
phase3_authorized: false
owner_decisions_requested_now: []
execution_receiver_package: agent-workspace/lead/MYOS2-LEAD-002/25-chatgpt-executor-startup.md
execution_receiver_display_name: MYOS2-A-C03a 代码分析执行
execution_receiver_status: STARTUP_PREPARED_NOT_OBSERVED_RECEIVED
execution_writer_transfer: NOT_GRANTED
next_safe_action: 新执行对话先只读接收；下一执行项为SR09-01的定点文档处置，具体写入入口另行绑定，不重做09
self_check:
  scope: this_review_only
  verified_claims: 9
  quotes_reconfirmed: 9
  downgraded_to_inferred: 0
  method: connector_source_readback_and_manual_count_no_parser_execution
inputs_read:
  - PR16/17元数据；全heads引用；开放PR第3页为空；lead目录；8983a9235c8a至67b2458b4a6a的GitHub比较。
  - 09 MANIFEST分段；user-return-signal.md的D01-D03及L1-L8大部分分段内容；stdio-08-disposition.md的结论、条件表、受影响位置与§3.1-3.5重点段落。工具截断的尾部不申报已读，不声称逐字读完每个表格单元。
  - 09 evidence.md第150-273行；facts-and-boundaries.yaml头部/anchor_policy/mainline_open_points以及第8000-8090行的结构检查；deferred-findings.yaml头部与RS-01可见段落；return-paths.dot全文。
  - 09 records/run.json全文；records/boundary_staged.json第70-99行；scripts/rs_check.py第670-794行。未重跑这些脚本。
  - time源码：options_flags.cmake、entry/common.c、arch/x86_64/entry/common.c、asm/syscalls_64.h全文。
  - time源码：arch/x86_64/lock_IPC/signal/signal.c第25-110、140-290行；signal_64.c第1-279行；asm/syscall.h第1-110行；entry_64.S第144-200行。
  - time源码：asm/uaccess.h第1-240行；asm/uaccess_64.h第1-160行；include/linux/kernel/uaccess.h第120-245行；lock_IPC/signal/signal.c第800-1078行；signal_const.h全文；signal.h第1-35行。
  - time源码：include/linux/fs/file.h第26-86行；fs/syscall.c第140-170行；fs/vfs/fcntl.c第1-36行；fs/vfs/file.c第225-250行；read_write.c第319-407行；sched/forkexec/fork.c第749-798行。
review_limits:
  - 未逐条复核8229行facts、1101行deferred、10661行queries、全部75锚点/262查询/353命中/110结构检查，也未独立重算其数量。
  - 未逐项核完D00所列35加20处替代位置；接受的是原退回要求的核心条件及代表回链，不是全部引用正确性认证。
  - 未运行命令、哈希、解析器、预处理、编译、C/ASM、内核或QEMU；未检查Owner电脑、账户额度或原子代理转录。
  - 未穷尽系统调用返回值产生者，未核copy_user_generic汇编后端、全部异常表消费者、并发/中断可达性或ISA一手规范。
open_questions:
  - SR09-01尚待后续追加处置；09整体不标PASS。
  - RS-01保持needs_evidence；GAP-U04运行/硬件部分与GAP-U10继续开放。
  - 新执行对话实际接收状态及唯一结果写区尚未绑定；发布启动包不等于执行或写权移交。
---

# 09主体证据保留；08两项退回限定关闭；仅退回一处摘要概括

## 1. 本次结论

**09已经完成本主线的有限复核，不再是“待首次审查”。** 12件交付、静态返回链和08处置具有可继续使用的证据。SR08-01、SR08-02的核心要求已得到处置，按本回执限定关闭，不再作为重复执行08的理由。旧08回执保留，本文件是其后继处置，不反写历史。

09有一处仍需纠正的报告概括，编号SR09-01：把按名字/字面量检索的“未发现重启码产生者”，过度总结成活动处理函数必落入非重启分支。按项目闸门，09当前结论是**局部RETURN，仅SR09-01**。它不是已证实的新内核bug，也不是整批成果作废；不要求重新进行09的批量分析、262次检索或多代理审查。

普通内核问题仍延期入库，当前无需Owner逐项决定。主线完成验收；新的独立ChatGPT对话只接替后续执行，不接替本主线验收。

## 2. 实际交付与机械证据

[VRF] GitHub compare核实8983a9235c8a至67b2458b4a6a为一个提交，只新增user-return-signal-09/下12件文件，无旧件净修改或删除。PR17仍open、Draft。这里的写区结论来自本轮远端比较，不只接受执行者的自报。

[VRF] records/run.json记录退出码0、all_ok=true、75/75锚点、16/16正文引文、262查询、353命中引用、110/110结构检查及值表一致。这是**执行方已入库的运行记录**，不是本主线重跑的结果；本轮没有独立验证这些计数或原始运行环境。

RC08-01指出的两项边界实现问题已在09交付脚本中作出针对性改变：

[VERIFIED agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/scripts/rs_check.py::tree_of]
```python
            rows[path] = (mode, obj)            # full object id; compared in memory, never written out
```

[VERIFIED agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/scripts/rs_check.py::boundary]
```python
    return 0 if ok else 2
```

本轮已读其checks集合及ok汇总：旧件完整对象、仅指定前缀新增、无未暂存/未跟踪内容、相关远端头都纳入判断。接受这两个已指定问题的静态处置；不把它扩展为检查器所有输入、安全失败和错误处理已经完备认证。

anchor_policy承认75条中存在包含/重叠、额外QD-L整行定位以及分配优先次序偏离。**75/75不是75项独立语义证明**；QD-L的expect只支持命中行含指定文字，不保证所属符号和推论。保留现有材料，不为计数美化重排ID或重造验证器；未回源的行仍不升级为主线已核。

## 3. D00：两项08退回的处置

### SR08-01：限定关闭

对照stdio-08-disposition.md §2.3、§2.4，已核：有效非空file且实际到清理点的路径；NULL槽跳过清理；成功F_DUPFD和F_DUPFD_CLOEXEC；明确失败及越界/未赋值/未到清理的不确定情形。表中还将净计数口径限于fdget/fdput及f_dupfd的get_file贡献，不擅自穷尽回调副作用。

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
	if (!f.file)
		goto out;
```

本轮已读out1/out的位置，out位于fdput之后；也读了ksys_write/do_writev的非空分支与fdput_pos位置。

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
```

这满足08回执要求，不需要重跑08。处置中新展开的panic/BUG行为、全局FMODE_PATH生产者和所有35处引用不在本轮逐项认证范围内；这些补充不能反向扩大本次关闭范围。

### SR08-02：限定关闭

处置正确区分入口保存、分发局部ax、返回工作后的帧和汇编装载值；保留eax/int/uint与完整64位rax的区别。无条件相等的旧说法已经被追加限定，且给出了四种重启码交互。没有把真实硬件结果冒充已运行。

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]
```c
			arch_do_signal_or_restart(regs);
```

本轮全文看到外层while及TIF判断被注释，调用本身活动。arch_do_signal_or_restart和handle_signal的两个switch已逐分支回源；它们不是由报告名字推测的关系。

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]
```c
			fallthrough;
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;
```

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]
```c
	regs->di = ksig->sig;
	/* In case the signal handler was declared without prototypes */
	regs->ax = 0;
```

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::restore_sigcontext]
```c
	/* disable syscall checks */
	regs->orig_ax = -1;
```

因此SR08-02的原问题已经处置。D00 §3.3和主说明L2将“数值相等”与“没有字段写入”分开，并明确排除了RS-08地址重叠的一种反例；本轮不把这些带假设的源码推演升级成任意并发、非法指针或编译后行为下的充要定理。09摘要另有SR09-01，不因这处新错误而撤销已成立的D00核心修正。

## 4. D01-D03可保留的代表结论

- **返回工作与出口：** C层确在分发后进入返回工作；汇编call do_syscall_64后顺序进入syscall_return_via_sysret，没有消费该bool的分流。固定源中get_signal的默认终止/停止处理对应代码被注释，活动fatal分支只置PF_SIGNALED。只认证所读函数内控制流，不认证所有任务的运行结果。
- **信号帧：** x64_setup_rt_frame的SA_RESTORER门、用户帧写入先于寄存器改写、失败后的处理缺口、成功时字段改写已回源。restore_sigcontext先复制，再写回通用字段、受限flags及orig_ax；rt_sigreturn先恢复掩码再恢复寄存器，之后再次经过返回工作。没有把未初始化uc_flags后面的源码描述当成编译行为保证。
- **访问边界：** get_user/put_user在所读实现中是直接访问，copy_from_user外层access_ok被注释，而copy_to_user仍保留检查；__access_ok的两个分支也已读。没有穷尽汇编复制后端或全部异常恢复路径，所以“只可能返回true”“一切失败分支都不可达”之类派生概括只可按报告的限定推断消费，不当主线全局认证。
- **重启条件：** 两个switch、SA_RESTART差异、orig_ax=-1门限和重启后再处理的关系有源码依据；活动syscalls_64.h没有__NR_restart_syscall项。具体重启码/219数值沿用交付值表及前序固定源码证据，本轮没有再重算完整常量表或全系统调用表。
- **共享sighand：** 本轮读取copy_sighand，分配后把tsk->sighand绑定到新对象的语句确被注释。RS-12与旧UV-14的关联可保留；exec、全部写入者及全局并发后果没有借此一并认证。

## 5. 唯一退回项 SR09-01：摘要不得把负向检索当成返回值穷尽证明

**类别：** documentation_erratum / evidence_scope_overstatement。范围仅09主说明“衔接”L1中“08 §3.2各行接上③之后”的表格第一行，及若存在同义摘要的定点引用。不是要求重新研究全部系统调用。

[VRF] 该行从“按名字和字面量检索，没有活动处理函数返回重启码”接到“落在RR-07或RR-14”，只显式排除了无活动return的处理函数。但同一报告D03.5的“按计算得出的返回值”一行明确写“没有穷尽。上面的检索只按名字和字面量”。这两种证据强度不一致。

[INFERRED] 负向文本检索不能排除表达式、组合返回值或未展开回调最后给出相同的数值；本轮**没有证明存在新的实际产生者**，退回原因是摘要比其现有证据说得更强。源码switch按实际syscall_get_error结果分派，不按返回语句是否含某个名字分派。因此不能把“RR-07/RR-14”当作整个在表内返回分支的确定分类。

**本轮起安全读法：** 根据实际orig_ax及ax取值判定RR各行。orig_ax按接口判为-1时仅跳过重启switch，信号建帧仍可能改帧；否则ax命中四个重启值时进入相应RR重启行，不命中时才是RR-07或RR-14。随后还要分别处理建帧成功、建帧失败及未到返回段。没有活动return与未穷尽的计算返回值都保持unknown。这里不主张具体程序已经触发任何分支。

**最小完成标准：** 后续执行只新增一份定点处置，列出上述原位置、与D03.5的关系、修正后的该行与受影响ID；检查同义摘要有无同样遗漏。无需复制整份09、改旧文件、重跑原检查器或对全部返回值作新穷尽分析。主线再核这一个处置即可；旧成果按本回执限定继续使用。

## 6. 五项交回主线的开放点：本轮裁定

1. **RS-12与UV-14：** 保留两个既有qualified_id，RS-12作为共享sighand根因的信号/exec后果补充关联UV-14；不删旧项、不重编号，也不把两个ID计为两个已证独立根因。
2. **07 UB-09：** 采用ER-03及两条证据追加的现有组织方式，不另造重复编号。原句只按分发局部阶段理解；真实返回仍受尾段与硬件边界限制。
3. **FN-01/FN-02：** 继续作事实/条件记录，不仅因与完整Linux不同就新增bug。未来有实际需求、活动可达性和后果证据时再立专项条目，不把本轮当作全局不存在性证明。
4. **RS-01：** 保持needs_evidence。已读头文件条件与signal.h顺序支持继续调查，但没有预处理/构建产物，不升级closed_static，也不据它断言所有建帧风险在真实运行必然被遮蔽。预处理和构建仍未授权。
5. **FPU调度侧：** 当前保留RS-10的跨域关联和延期线索，不从旧DR-005速查报告或单个函数缺少名称自动新签全调度结论。后续单独核实际保存/恢复链时再决定是否分立条目；本次不扩大到FPU/SIMD专项。

这些是本主线对证据组织和消费边界的裁定，不是向Owner提出新的修复选择题，也不取消普通问题延期规则。

## 7. 执行方式、来源与未做事项

130个完成子代理、五视角/三轮修正/终审、Owner在原本地会话的模型和额度指示，均属于执行者报告；原代理转录未随包交付，本轮没有独立认证其数量、授权细节、模型、账单或审查独立性。它们不参与证明某一技术断言正确。本次主线评审由ChatGPT生成，不冒称人工逐行验收。

没有命令运行、测试、哈希重算、内核修改、补丁、构建、QEMU、外部libc/镜像读取、阶段3或合并。PR16读取时mergeable=false，这只是当前合并可行性字段，原因未诊断；本轮没有合并任务，不自行rebase或修冲突。PR17仍保留Draft及原分支。

25号新执行对话启动包已先于本回执交付，Owner指定GPT-6/Extra。它只授权接收与只读预检，不自动转移PR17写权，也不重做09。SR09-01是适合随后绑定给新执行对话的有限文档处置；就位前由主线保留待办，不要求Owner再解释已有规则或转发长报告。

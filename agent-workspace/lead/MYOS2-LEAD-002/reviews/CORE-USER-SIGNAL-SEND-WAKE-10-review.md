---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SIGNAL-SEND-WAKE-10-REVIEW-001
record_type: bounded_static_analysis_review
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
human_technical_review_attested: false
date: 2026-10-31
read_channel: connector
authorization_ref: Owner报告第10批完成并要求验收，同时要求自本轮起验收后同轮给出下一工作；27号任务、28号补充与既有阶段2权限
evidence_class: 六件固定文本、定点源码人工比对及GitHub增量比较；不是运行验证
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 848e2acdf7e1
reviewed_base_short12: 037b880e792b
source_pin_short12: a039d9803ade
instruction_short12: 065abf7e8677
lead_before_this_turn_short12: 9af19669b2e8
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_REVIEW_COMPLETE_WITH_LOCAL_DOCUMENTATION_RETURN
acceptance_verdict: RETURN
returned_scope: [SR10-01]
retained_scope: 发送链、输入条件、pending与队列、唤醒断点及延期记录的有界静态证据；不包括图的无条件直接调用消费
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
execution_branch_modified_by_mainline: false
kernel_change_made: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-PID-LIFETIME-11
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/29-user-pid-lifetime-contract.md
next_task_status_at_review_creation: TO_BE_PUBLISHED_THIS_TURN
inputs_read:
  - PR17最新C03a交付评论；PR16/17元数据、heads直接引用；开放PR第三项为空。
  - 第10批六件全文：MANIFEST 76行、主说明298行、facts 686行、图58行、deferred 164行、evidence 105行；分段补足工具截断尾部。
  - compare 037b880e792b至848e2acdf7e1，六个提交、六件A新增；沿用同9af19669b2e8对象的主线路径清单。
  - time的options_flags.cmake全文；lock_IPC/syscall.c第82-101行；syscalls_64.h第1-45行；syscall_wrapper.h第1-145行；include/linux/kernel/syscalls.h第1274行至末尾。
  - time的lock_IPC/signal/signal.c第1-255、350-610、623-835、910-950、1068-1087、1230行至末尾；signal.h第140-280、373-408行；signal_macro.h第185-205行；uapi/asm-generic/signal.h第1-65行。
  - time的sched/task/pid.c第300-465行；pid_api.h第1-35行；scheduler_core.c第193-205、270-485、553-566行；init/main.c第165-205行；fork.c第749-835行；entry/common.c第9-38行。
  - 为下一任务设计另外读取05的MANIFEST、主说明第1-175行可见内容及固定pid.c第1-300行、sched/syscall.c全文、forkexec/exit.c第1-220行；这部分不作为第11批已完成。
quote_check:
  submitted_fact_anchor_records: 52
  manual_source_text_matches: 52
  additional_evidence_blocks_readback: 4
  method: manual_connector_readback_no_script_execution
  caveat: A51重复A25；文字命中不证明引用它的边或推论正确；不是52项独立证明
review_limits:
  - 未重跑执行方JS核对、YAML/Graphviz解析、旧检查器、命令或哈希；工具运行和数量自报不作独立运行认证。
  - 未全树穷尽间接调用、PID生命周期、锁/RCU、slab、调度器或其他信号生产者；当前链的否定结论不得扩大全仓。
  - 未预处理、编译、运行C/ASM/内核/QEMU；未核外部libc、硬件或Owner本机。
  - 旧09条目仅消费既有回执限定；本轮不重新签发其全量事实结论。
open_questions:
  - SR10-01随下一批P00追加处置；第10批整包尚不标PASS。
  - 运行层、RS-01、GAP-U04硬件/运行部分及GAP-U10保持原边界。
---

# 第10批：主体静态证据保留，图与证据关联定点退回

## 1. 结论

**完成本次有界验收。保留第10批主体成果，局部RETURN仅SR10-01：结构化调用图与引文关联需要修正。** 不是重新执行第10批，也不是要求修复内核。报告已正确区分局部return、pending位、siginfo节点、通知/唤醒和handler执行；未初始化参数和非法编号的后续行为没有冒充运行成功。

按28号Owner新约定，本轮同时发布第11批：先在新目录追加一份SR10-01处置，再继续独立的PID对象发布、查找与退出生命周期静态取证。修正不单独增加一次派发回合；执行者不得据任务发布或自己的accept处置自动关闭本RETURN。

## 2. 交付身份、范围与自查记录

[VRF] PR17最新交付评论指向848e2acdf7e1，并明确READY_FOR_MAINLINE_REVIEW。GitHub compare相对037b880e792b为六个ahead提交、六件A新增，仅user-signal-send-wake-10/约定六件，共1387行；没有旧件净修改或删除。执行分支与本次固定头一致，PR17仍open/Draft、未合并。

六件已经完整读取。52条facts源码引文的文字在本轮读取的固定源段中逐条人工核对，未发现伪造引文；evidence额外四条也回源。A51与A25相同，不作独立证据计数。正文20处引用这些锚点，不等于另外20项独立证明。发现的退回问题恰好说明：引文本身真实，仍可能不能支持所引用的调用边。

图与facts均列25节点、26边，序号和端点以人工可读核对；18类矩阵及六项新增、五项旧证据追加均实际存在。没有运行解析器或复现执行方JS记录，不把数量或集合自查当作语义认证。

## 3. 可以保留的关键结论

### 输入、对象与局部返回

已核kill入口实际准备info后调用kill_something_info，调试宏的默认0仅控制打印。正pid经find_vpid、pid_task查询；使用PIDTYPE_PID查任务与使用PIDTYPE_TGID选择shared_pending是两件事。NULL目标和非正pid的源码分支不同：前者局部-ESRCH，后者活动while(1)，不得照Linux常识解释进程组/广播成功。

发送链的权限检查、0号保护与显式siglock获取被注释；sigaddset源码没有输入防护。按固定64位配置解释0、负值或超过64的位移前提时必须保留语言层未知，不能保证具体寄存器值或崩溃。宏/外部编译选项未预处理确认，所有数值结论限相应配置条件。

### 队列与pending不是同一结果

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]
```c
	q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);
```

已读完整声明、被注释的赋值分支及分配函数：override_rlimit未赋值却作为实参读出。不能因被调函数右侧比较可能为真就忽略该读取。非SIGKILL/非PF_KTHREAD路径的后续只能作带前提的源码控制流说明；不认证其定义良好的运行。特殊路径绕过分配，与q非空/为空的路径分别记录是正确的。

已核先入链再写q->info、sigaddset与return ret的位置；prepare_kill_siginfo经clear_siginfo清零后填字段，因此此入口的si_uid为零来源与未初始化值不同。collect_signal的同号多节点、保留位、删除/释放节点及无节点合成info路径有依据；不因此保证发送同步或队列无竞态。

### 唤醒断点与旧项关联

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]
```c
	// complete_signal(sig, t, type);
ret:
	// trace_signal_generate(sig, info, t, type != PIDTYPE_PID, result);
	return ret;
```

当前kill直接链没有这条活动调用边；complete_signal内未赋值的t和唤醒接口只能作为潜伏条件登记。另已读try_to_wake_up全部相关函数体：success初始化为0，匹配检查和入队被注释，但仍有状态写入等副作用，不能把返回0解作完全无副作用。第10批没有作这种扩张，本回执明确保留该界限。

09返回尾仍可在其既定入口扫描pending；缺少发送唤醒不推出正在运行任务绝无处理机会，也不保证睡眠目标会及时执行。五项RS证据追加保留目标ID和旧状态，不把共享pending写入误当私有同步信号或fork继承的现行生产者。

## 4. 唯一退回项SR10-01：图、边类型与证据关联

类别为documentation_erratum / evidence_link_and_graph_semantics。范围只限以下位置及其同义消费，不重做52锚点或发送侧分析。

| 位置 | 本轮核实的问题 | 最小处置 |
|---|---|---|
| facts的SW10-E01 | sys_kill→prepare_kill_siginfo引用A03；A03只有入口、调试宏和局部声明，没有这条调用 | 引用已存在的A03b；保留真实函数关系 |
| SW10-E12 | __send_signal_locked→__sigqueue_alloc只引A18；它只支持SIGKILL/PF_KTHREAD的绕过条件，不包含分配调用 | 条件证据保留A18，补实际调用A19及必要上下文 |
| SW10-E16 | 图端点为signal_wake_up_state，当前只引包含signal_wake_up宏调用的A34 | 补A35宏绑定，注明通过宏展开而非凭名字跳到另一函数 |
| SW10-E21及DOT同ID | dequeue_signal→collect_signal被画为call/conditional，没有写明中间__dequeue_signal；A29只展示对__dequeue_signal的调用 | 恢复两段真实调用或明确且独立标注为经中间函数的路径，给中间函数和sig!=0条件证据；不得继续当直接调用 |
| facts.edge_direction、DOT头部、SW10-E24 | 自行引入“except explicit sender writes”；E24用data表达插入副作用，与27号统一的消费方→提供方口径混杂 | 去掉无授权例外；保留data作为依赖关系，实际写入在明确的effects/说明中表达；图和记录统一 |

[VERIFIED mykernel/lock_IPC/signal/signal.c::__dequeue_signal]
```c
	int sig = next_signal(pending, mask);
	if (sig != 0)
		collect_signal(sig, pending, info, resched_timer);
	return sig;
```

[VERIFIED mykernel/lock_IPC/signal/signal.c::dequeue_signal]
```c
	*type = PIDTYPE_PID;
	signr = __dequeue_signal(&tsk->pending, mask, info, &resched_timer);
	if (!signr) {
```

E21的问题不是“collect_signal不可达”，也不是C函数指针间接调用；它是省略中间函数的可达路径没有被标明。节点/边ID相等不足以证明图的调用层次正确。A01等真实引文以及发送侧的核心事实无需因此重做。

下一批P00新增signal10-graph-disposition.md，写明原位置、接受或有证据异议、替代关联、局部YAML/DOT或等价明确说明、相关ID和未知；顺带有界核查26条边是否有同类问题。旧六件冻结，不重编号或覆盖。修正前本轮图只作为带上述附注的概览，不能用于未经说明的直接调用推理。

## 5. 延期与后续

六项新发现仍按各自源码/条件/needs_evidence类别消费，不是六个运行复现。一般内核问题继续按18号延期；没有Owner修复问卷。09的SR09-01及08的两项限定关闭保持，不以新图修正反向撤销旧裁定。RS-01、GAP-U04硬件/运行和GAP-U10均不在本批关闭范围。

第11批选择PID生命周期，是为回答本批尚未核实的目标身份何时发布、查询是否持有引用、退出后是否从可查表移除等问题；沿05已有fork/exit事实作增量，不重新盘点mm/VFS或证明全局并发。任务设计依据和精确范围在29号，不以预检代替执行成果。

本轮只写lead记录及PR导航/评论，执行头保留848e2acdf7e1。没有命令、解析器、哈希、内核修改、补丁、编译/QEMU或合并；继续阶段2。主线依28号在本轮同时完成下一任务发布，C03a仍须收到入口后按新工作包执行，不能自行挑选12或签发主线PASS。

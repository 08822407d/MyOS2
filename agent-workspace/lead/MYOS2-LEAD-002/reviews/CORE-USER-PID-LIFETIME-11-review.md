---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PID-LIFETIME-11-REVIEW-001
record_type: bounded_static_analysis_review
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
human_technical_review_attested: false
date: 2026-10-31
read_channel: connector
authorization_ref: Owner报告第11批完成并要求继续；28号要求验收与下一工作包同轮发布
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 2c410132e42d
reviewed_base_short12: 848e2acdf7e1
source_pin_short12: a039d9803ade
contract_short12: 04fc4f26ba3d
lead_before_this_turn_short12: 130a4992d547
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_REVIEW_COMPLETE_WITH_LOCAL_DOCUMENTATION_RETURN
acceptance_verdict_11: RETURN
returned_scope: [SR11-01]
retained_scope: PID占号与发布、任务挂接、查询引用、退出断点及有条件延期证据；接口表按本回执限定
prior_item_disposition: {SR10-01: CLOSED_IN_BOUNDED_REVIEW_WITH_SUPPLEMENT}
effective_signal10_disposition: PASS_PENDING_LOCAL_WITH_PRIOR_REVIEW_LIMITS
candidate_disposition: {PID11-ER01: C6_QUALIFICATION_ACCEPTED_OTHER_LOCATIONS_REQUIRE_READBACK}
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
executor_claimed_quote_count: 125
reviewer_reproduced_all_125_quote_checks: false
review_method: 七件全文读取、关键路径与返回类型定点回源、可读表图复核及GitHub增量比较
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
next_followup: CORE-USER-MM-FAULT-12
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/30-user-mm-fault-contract.md
owner_decisions_requested_now: []
inputs_read:
  - PR17最新C03a交付评论、PR16/17状态、heads直接引用、开放PR第三项为空及PR16完整路径清单。
  - 固定第11批七件全文，工具截断部分分段补齐；compare 848e2acdf7e1至2c410132e42d。
  - 固定time：options_flags.cmake全文；pid.c第110-530行及attach_pid精确片段；pid.h全文、pid_const.h第1-25行、pid_namespace.h第1-35行、pid_namespace.c第105-126行。
  - 固定time：fork.c第1110-1538行；exit.c全文；scheduler_core.c第1172-1187行；idr.c第1-225行、idr.h第43-75行、idr_const.h全文。
  - 固定time：signal.c第211-241、711-750行；P00其余同一固定对象的入口、宏绑定与发送段沿用此前主线已核材料，未冒称本轮全量重跑旧10。
  - 05主说明C6第350-385行原文；05的UV-15.claim本轮未独立定位全文。
  - 为12设计另读固定time的arch/x86_64/mm/fault.c第1-460行、mm/fault/fault.c第1-240行；只作设计预检，非第12批已完成。
review_limits:
  - 七件全文阅读不等于125锚点、43正文引文及全部图边均已独立自动认证；执行方计数和JS结果仍是其报告。
  - 本轮未运行命令、JS/Python、YAML/Graphviz解析、旧检查器、哈希、预处理、编译、C/ASM、内核或QEMU。
  - 不证明全PID并发、引用原语、slab、调度器、其他回收入口或实际二进制行为；普通问题仍按18号延期。
  - RS-01、GAP-U04硬件/运行部分及GAP-U10保持既有边界。
---

# 第11批验收：关闭旧图退回，保留PID主体，一项接口记录定点退回

## 1. 总结与交付身份

接受P00对SR10-01的核心处置，旧10改按PASS_PENDING_LOCAL及原回执限制消费。第11批主体静态材料保留；唯一退回组SR11-01是kill_proc_info接口返回类型及同一接口调用证据的记录错误，不是要求重做PID分析或修内核。

被审头为2c410132e42d。GitHub compare相对848e2acdf7e1为8个提交、7件A新增，共2663行，路径全部位于约定user-pid-lifetime-11/；旧件无净修改/删除。PR17实际头与被审头一致，仍open/Draft。执行者未自行关闭SR10-01或签主线PASS。

七件内容已完整读取：MANIFEST 88行、P00处置195行、主说明553行、facts 1445行、DOT 110行、延期178行、evidence 94行。事实表/图的47节点、54边、16状态、11所有权项、7 effects及延期六新项/三追加/一候选均作为可读交付检查；未用解析器复现其集合统计。主线以关键源码语义和实际证据关联验收，不以125/125自报替代独立判断。

## 2. SR10-01限定关闭

P00已将E01的调用证据补为A03b，E12补实际分配调用A19，E16补A35宏绑定；E21以SW11-GN01/GE01/GE02明确恢复中间函数，E24及头部取消writer方向例外，写入副作用另放effects。旧26条边有逐项消费说明，旧文件与编号未被覆盖。

[VERIFIED mykernel/lock_IPC/signal/signal.c::__dequeue_signal]
```c
	int sig = next_signal(pending, mask);
	if (sig != 0)
		collect_signal(sig, pending, info, resched_timer);
	return sig;
```

本轮回读dequeue_signal的两次调用及上段，确认它不是直接调用collect_signal；第二次受私有pending无结果的条件约束。P00新增路径读法成立。其余入口和宏关系与此前已核同一time对象相符，关闭范围只涵盖原SR10-01，不升级为所有旧图语义的全局认证。

**主线补足读法：** P00把旧SW10-A20描述为“未初始化实参前提”的证据不够准确：A20只展示分配器形参和局部变量。未初始化条件应连读旧A15的声明、赋值被注释的完整发送段及A19调用；A18/A19才分别支持本次E12的绕过条件与实际调用。本回执明确补足，保留已知语言层风险，不为该附加引用说明重开原图退回。

## 3. PID主体可保留的范围

### 创建和可查性

已定点回读alloc_pid、idr_alloc/idr_alloc_cyclic/idr_alloc_u32、copy_process的发布与挂接段、kernel_clone后续。数字扫描、idr_replace发布pid_s、attach_pid关联任务、wake_up_new_task尝试使任务运行是不同阶段。NULL写入仍为空槽、循环游标会推进，两者必须一起读：有条件的占号冲突风险成立为源码候选，但不能说每次fork必冲突。

数组长度由MYOS_IDR_BUF_SIZE=32768及idr_init_base分配式核实；idr_find直接索引而未按该容量检查。对用户给出的大正pid不能无条件承诺NULL或-ESRCH。报告已保留非法访问与实际运行结果的未知，未将此当动态复现。

### 引用与对象类型

已回读find/get/put实际函数。get_pid增加pid对象计数；get_pid_task与find_get_task_by_vpid的get_task_struct为注释。持pid引用不能替代task引用。find_vpid/pid_task裸查找及kill内部对指针的使用，尚不证明对象并发生存安全；也不能因当前exit缺少释放连接就声称永远安全。

### 退出与回收

已回读exit.c全文、do_task_dead以及detach_pid/__change_pid/free_pid/put_pid。当前所读exit直接链没有连到PID摘除；释放函数存在不是它实际被调用的证据。group_dead未初始化传参、非空children分支使用错误namespace来源，均不支持后续必然完成。

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
		task_s *task_initd = find_task_by_pid_ns(1, PIDTYPE_PID);
		list_header_add_to_tail(&task_initd->children, child_lp);
```

[VERIFIED mykernel/sched/task/pid.c::find_task_by_pid_ns]
```c
task_s *find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)
```

PIDTYPE_PID为枚举首值0；本回执把问题规范为“没有提供有效namespace对象，按该值传递形成空namespace风险”，不签发“编译器必然拒绝/必有诊断”的结论。无孩子不进入此查询；上游group_dead的语言层前提仍单列。非init namespace的get/put问题保留为潜伏，不能套到所有flags为0的普通fork。

## 4. SR11-01：返回值与内部借用对象不得混为一个接口字段

### 必须更正的两个位置

| 位置 | 原记录 | 源码与最小处置 |
|---|---|---|
| facts.ownership[PID11-OWN11].return | raw task_s* via pid_task | kill_proc_info实际返回int状态。将函数返回与内部借用pid/task指针分字段记录；保留没有引用增量的判断。 |
| facts.edges[PID11-E38].anchors | 仅PID11-A93 | A93只有type、error、p声明及for入口，不包含find_vpid调用。改用已有A93b/A94；A93仅可作上下文。 |

[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]
```c
static int
kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid) {
```

[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]
```c
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p)
			error = group_send_sig_info(sig, info, p, type);
```

[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]
```c
	return error;
```

OWN11应说明：API返回整数error；内部取得裸pid_s及task_s* p，不以返回值向调用者转移task指针或引用。目标为NULL时本函数的局部初值为-ESRCH；目标存在时使用group_send_sig_info返回的状态，必须继承发送路径语言层/对象生存前提，不能承诺所有输入都稳定返回0。

下一批M00只新增pid11-disposition.md给替代表项、E38补证及同一接口相关位置的一次核查，不修改11旧件、不重做125锚点。顺带为ST14的“do_exit到do_task_dead”消费补上已有A84b：A84只到preempt_disable等行，A86/A87支持被调函数体而不独立证明调用点。这个定位补足与接口修正同轮完成，不另建退回任务。

## 5. PID11-ER01及普通问题处置

已回读05主说明C6原句“exit_notify把子任务挂到1号任务名下；最后do_task_dead”。**接受该位置的限定勘误**：只能说存在拟重链接代码；有效namespace、有效目标、列表状态以及上游定义良好的控制流没有全部成立的证明，不能把成功改挂或必达死亡边界写成已实现保证。

05::UV-15.claim本轮未重新定位全文，因此不冒称候选列出的全部位置已经验证。M00读取其准确文字后只限定确有同义过度概括的部分，原有“等待/回收缺口”不反向撤销，不重新编号UV-15。该候选是历史材料消费纠偏，不是对11正确指出问题的惩罚。

六项新发现和三项旧证据追加按源码/条件/needs_evidence分级保留；没有六个运行bug复现的结论。一般问题按18号延期，不向Owner请求立即修复。08、09及SR09-01状态不变，RS-01、GAP-U04硬件/运行和GAP-U10不在本轮关闭范围。

## 6. 同轮下一步与停止边界

依28号，同轮发布CORE-USER-MM-FAULT-12：M00完成上述定点文档处置，然后分析此前09明确未覆盖的用户地址缺页控制流、VMA/PTE分派与返回结果消费。任务设计仅预读架构fault入口和mm fault文件前部，不等于12已执行。

本轮主线仅新增本回执、30号任务和检查点，并更新PR导航/审查评论。执行头保持2c410132e42d，原11和10全部冻结；不合并、关闭或删除PR/分支，不强推、不改内核、不构建运行、不进入阶段3。C03a只有收到新任务并核对写前条件后才在新写区开工，完成后仍交回C03验收。

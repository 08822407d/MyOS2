---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MM-FAULT-12
record_type: scoped_sr11_01_and_historical_statement_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade; old results=2c410132e42d; instruction=e7ca3dbbcc06"
required_review: CORE-USER-PID-LIFETIME-11-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
prior_mainline_verdict_11: RETURN
sr11_01_local_disposition: accept
historical_pid11_er01: bounded_historical_statement_qualification
prior_sr10_01: CLOSED_IN_BOUNDED_REVIEW_WITH_SUPPLEMENT
runtime_evidence: NOT_RUN
supersedes_locations:
  - "user-pid-lifetime-11/facts-and-boundaries.yaml::ownership[PID11-OWN11]"
  - "user-pid-lifetime-11/facts-and-boundaries.yaml::edges[PID11-E38]"
  - "user-pid-lifetime-11/facts-and-boundaries.yaml::state_matrix[PID11-ST14] anchor consumption only"
  - "user-pid-lifetime-11/pid-lifetime.md::P02 API所有权表与kill桥接段的合读限定"
  - "user-pid-lifetime-11/signal10-graph-disposition.md::SW10-A20证据消费"
  - "user-vfs-baseline-05/user-vfs-baseline.md::C6 365行描述与deferred-findings.yaml::UV-15.claim 731–732行相关概括"
affected_ids: [PID11-OWN11, PID11-E38, PID11-E39, PID11-ST14, PID11-A93, PID11-A93b, PID11-A94, PID11-A84, PID11-A84b, SW10-A15, SW10-A19, SW10-A20, "CORE-USER-VFS-BASELINE-05::UV-15", PID11-ER01]
open_questions:
  - "SR11-01关闭及05旧文字消费最终裁定由C03决定"
  - "未对旧11的125条引文作全批重跑，正文/图只定点合读"
---

# M00：SR11-01与05历史措辞的有界追加修正

**执行方处置：accept**。这是对记录、接口所有权及引文关系的勘误，不是内核补丁、代码重验或对主线RETURN的自行关闭。旧10/11/05文件和编号均不改；以下表格及短引文提供本批追加、可直接消费的替代读法。

## 1. SR11-01：OWN11的返回值和借用对象拆分

**原址**：旧 `facts-and-boundaries.yaml::ownership[PID11-OWN11]` 原字段 `return: "raw task_s* via pid_task"`，实际上混合了函数内部局部指针和对调用者返回的API值。

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
static int
kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid) {
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
	enum pid_type type = PIDTYPE_TGID;
	int error = -ESRCH;
	task_s *p;
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p)
			error = group_send_sig_info(sig, info, p, type);
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
		if (likely(!p || error != -ESRCH))
			// return error;
			break;
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
	return error;
```

- **返回类型与结果**：API定义 `static int kill_proc_info(...)`，返回整数 `error`。`error`初值 `-ESRCH`；当 `p==NULL`，此局部路径保留该值并退出循环；`p!=NULL`则更新为 `group_send_sig_info`的返回值，若仍是 `-ESRCH`可再次查询。后续路径受发送方此前的`override_rlimit`未初始化、无活动锁、合法信号等前提限制；不能给所有输入固定成功码。
- **内部借用**：表达式 `find_vpid(pid)` 得到 `pid_s*`，`pid_task(..., PIDTYPE_PID)` 得到局部 `task_s* p`；此处未调用`get_pid`或`get_task_struct`、没有局部RCU锁。调用者**不会**通过该函数返回拿到 `task_s*`，也未从此API获得对象引用。
- **边界**：PIDTYPE_PID用于查找单个任务，`type=PIDTYPE_TGID`用于发送方共享pending选择；不把PID对象引用和任务对象引用混淆。对并发任务摘链、释放和对象生存的保证为unknown。

可直接替代的结构化记录：

```yaml
target: PID11-OWN11
status: corrected_consumption_only
api: "kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid)"
api_return_type: int
api_return_value: "error; initially -ESRCH, updated by group_send_sig_info when p exists; retry possible"
internal_borrowed_pid: "find_vpid(pid) -> pid_s*, unpinned"
internal_borrowed_task: "pid_task(..., PIDTYPE_PID) -> task_s* p or NULL, unpinned"
pid_reference_increment: false
task_reference_increment: false
locking_in_this_direct_call: "rcu_read_lock/unlock commented"
release_counterpart: "none for unpinned borrow"
anchors: [PID11-A93, PID11-A93b, PID11-A94, "time:signal.c::kill_proc_info:747"]
runtime_evidence: NOT_RUN
```

## 2. SR11-01：E38的实际调用证据及同类检查

**原址** `facts-and-boundaries.yaml::edges[PID11-E38]`：`kill_proc_info → find_vpid`虽是真实的嵌套函数调用，原 `anchors: [PID11-A93]`只给出type、error、p声明及for入口，没有调用本身。应以**PID11-A93b和PID11-A94**为实际调用证据，A93仅作上下文；不更换旧边ID。与其相邻的 `PID11-E39`（`pid_task`调用）已有A94可证明调用，未发现需要撤销；但这两条call都不能从函数内局部借用跨成“API返回task指针”的data边。

**[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]**
```c
		// rcu_read_lock();
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
```

替代局部记录：

```yaml
corrections:
  - target: PID11-E38
    kind: call
    from: PID11-N46
    to: PID11-N14
    status: conditional
    anchors: [PID11-A93b, PID11-A94]
    context_only: [PID11-A93]
    note: "nested find_vpid invocation; current kill reaches only after positive-pid branch"
  - target: PID11-E39
    kind: call
    from: PID11-N46
    to: PID11-N17
    status: conditional
    anchors: [PID11-A94]
    action: retain
  - target: PID11-OWN11
    action: replace_return_field_and_add_borrow_fields
```

本处同步核过旧 `pid-lifetime.md` P02“10 kill_proc_info”行与“与10的桥接”段：文字称内部裸查找、没有task引用，本身**不直接说API返回指针**；其消费必须附上本文件新增的`api_return_type=int`，不虚构这两段已经文本更新。旧OWN11表的混合返回字段才是必须替换的实体。

## 3. ST14及SR10-01的补充锚点

**ST14定位补足**：旧 `PID11-ST14` 主要锚点A84覆盖`preempt_disable`等调用前置行，A86/A87仅支持`do_task_dead`函数本体，不支持“调用者已调用它”。真实调用点在已存在的**PID11-A84b**；将它与A84、A86/A87合读。

**[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]**
```c
	do_task_dead();
```

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]**
```c
	set_special_state(TASK_DEAD);
```

**SR10-01证据限定**：旧P00将SW10-A20称为“未初始化实参前提”并不充分。A20仅含`__sigqueue_alloc`形参、`sigpending=0`局部；须同时读**SW10-A15（声明`override_rlimit`）、发送段523–526行全为注释、SW10-A19（实参使用）**。SW10-A18支持SIGKILL/PF_KTHREAD绕过条件；SW10-A19支持真实分配调用；SW10-A21的`if (override_rlimit || ...)`支持分配器读形参。SR10-01已由C03限定关闭，新增这条引用说明**不重新开启**原退回。

**[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]**
```c
	int override_rlimit;
	int ret = 0, result;
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]**
```c
	// if (sig < SIGRTMIN)
	// 	override_rlimit = (is_si_special(info) || info->si_code >= 0);
	// else
	// 	override_rlimit = 0;
```

**[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]**
```c
	q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);
```

此处只给实参的源文本不满足已赋值前提，不宣告具体运行后果。

## 4. PID11-ER01：05历史两处措辞的准确限定

逐字读取了旧05两处：

- `user-vfs-baseline-05/user-vfs-baseline.md::C6`第365行原文为“**exit_notify 把子任务挂到 1 号任务名下；最后 do_task_dead**”。这确实把可能路径描述成无条件完成。
- `user-vfs-baseline-05/deferred-findings.yaml::UV-15.claim`第731–732行实际含“**只把子任务挂到 1 号任务名下，摘到空节点时原地自旋**”，也存在关于“改挂已完成”的类似简化；**需要加入前提限定，不是无须更改**。同一条对`wait4`空体、`TASK_DEAD`清理缺口等历史记录保留，UV-15状态不动。

**[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]**
```c
	while (tsk->children.count != 0)
	{
		List_s * child_lp = list_header_remove_head(&tsk->children);
		while (child_lp == 0);
```

**[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]**
```c
		task_s *task_initd = find_task_by_pid_ns(1, PIDTYPE_PID);
		list_header_add_to_tail(&task_initd->children, child_lp);
```

**[VERIFIED mykernel/sched/task/pid.c::find_task_by_pid_ns]**
```c
task_s *find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)
```

`PIDTYPE_PID`在`pid_const.h::enum pid_type`是首个值0，它不能作为有效的`pid_ns_s*`对象来源。C源码实参形成**空namespace指针前提**（具体编译接受性、运行崩溃均NOT_RUN），不能从函数名推导成功找到了1号任务。有孩子、链头移除返回非空、有效namespace、有效pid1 task和目标列表状态是能完成改挂的必要条件；上游`do_exit`还将未赋值`group_dead`传给`exit_notify`，进一步限制“最后必达do_task_dead”。

**可直接替代C6与UV-15该子句的文字（不改变其它UV-15内容）：**

> `do_exit`的活动源码包含`exit_notify(tsk, group_dead)`以及后续`do_task_dead()`调用点，但传入的`group_dead`在本函数内没有活动赋值。仅当`exit_notify`到达子任务非空分支时，代码尝试从子任务列表取出头节点并用`find_task_by_pid_ns(1, PIDTYPE_PID)`查询接收者；该调用把enum值0传给要求`pid_ns_s*`的第二参数，没有可核实的有效namespace，所以既不能保证找到1号任务也不能保证完成子节点重挂。无孩子时不会走该查询。其它关于等待、ZOMBIE、后续PID/task回收的既有缺口保持独立成立并受前提限制。

**处置范围**：本补充直接关联`PID11-ER01`、`CORE-USER-VFS-BASELINE-05::UV-15`，05旧文件冻结；执行者不把候选升级为主线正式ERRATUM关闭，也不签发内核已修复。

## 5. 同义摘要与验证界限

只定点核查旧OWN11、E38/E39、ST14、P02正文桥接、P00 A20解释、05 C6/UV-15.claim这些位置，相关引文从`time @ a039d9803ade`真实函数回源。**没有全量重做11的125条锚点或10全部52条锚点**。对需要“执行方accept”的文档修正已提供覆盖读法，但SR11-01最终仍等C03主线验收。没有运行C/ASM/编译/内核/QEMU、外部libc、原检查器，亦没有修改任何旧件。

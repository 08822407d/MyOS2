---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SIGNAL-SEND-WAKE-10
record_type: readable_signal_sender_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话（Owner选择 GPT-6，Extra；后台未独立认证）"
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；results_base=037b880e792b（工具复制的短提交）"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
inputs_read:
  - "27号合同固定版本065abf7e8677全文及C03的DOC-FIX-01-REVIEW-001"
  - "time固定对象：lock_IPC/syscall.c、signal/signal.c、signal.h、signal_macro.h、signal_types.h"
  - "time固定对象：x86-64系统调用表/号/包装宏；sched/task/pid.c、pid.h、pid_api.h；scheduler/scheduler_core.c直接相关函数"
  - "time固定对象：init/main.c、sched/forkexec/fork.c中有限初始化路径、asm-generic/signal.h、scripts/options_flags.cmake"
  - "09固定文档的发送边界、D02-b、RS-03/04/12/13/14；DOC-FIX-01限定；07/08引用时只复用受限制的既有ID"
open_questions:
  - "未初始化参数及非法信号编号的C语言前提下，实际返回/位图/队列状态未知"
  - "发送端没有活动锁路径；并发及PID生存未验证"
  - "唤醒链在当前kill路径中断开；真实调度、运行及用户handler不可保证"
---

# 10 · 信号发送、pending 与唤醒：当前可证明的边界

## 0. 结论与消费限制

**本批能证明有一条用于发送的静态调用链，但不能证明“kill返回0＝目标已经接到信号/被唤醒/执行handler”。** 固定源码中的正PID分支查找目标后，确实进入共享pending的队列/位图写入段；然而其权限、加锁、重复信号分流及唤醒通知多处是注释；更重要的是，非SIGKILL、非内核线程路径把未赋值的 `override_rlimit` 传给分配器，非法信号号也没有活动检查。涉及这些条件时只能描述控制流、不能给定实际运行结果。源码层面含一个不可忽略的断点：`complete_signal` 调用被注释，不能从其函数体、`wake_up_state` 存在就画出当前kill的活动唤醒链（SW10-E15）。

阶段2范围仅为 `time @ a039d9803ade`。09目前因SR09-01关闭而处于带前次限制的 `PASS_PENDING_LOCAL`，但那不是内核运行PASS。本批新增的是上游发送/唤醒事实，并不重核09的返回帧、信号建帧或08系统调用语义。所有主张均须读为源码条件，实际内核、编译、QEMU、硬件与外部libc均NOT_RUN。

## 1. S01：62号入口、参数和正PID目标

活动表 `mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_kill` 给出62，`syscalls_64.h` 的 `__SYSCALL(__NR_kill, sys_kill)` 有活动表项（SW10-A01）。`MYOS_SYSCALL_DEFINE2` 与架构包装器将 `pt_regs` 的di/si映射为 `pid_t pid` 和 `int sig`，实现在 `lock_IPC/syscall.c`。宏 `IF_ALERT_DUMMY_SYSCALL` 为字面量0，`ALERT_DUMMY_SYSCALL` 仅包含被条件保护的 `pr_alert`，不是挂起或提前退出；即使开启显示，其后续发送语句仍在函数体内。保留包装器的返回值传递，而非只因“DUMMY”一词把kill当作空实现（SW10-A02～A05）。

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE2(kill)]（SW10-A03b，本轮从time固定源码回读）

~~~c

	prepare_kill_siginfo(sig, &info, PIDTYPE_TGID);

	return kill_something_info(sig, &info, pid);
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_something_info]（SW10-A07，本轮从time固定源码回读）

~~~c
	if (pid > 0)
		return kill_proc_info(sig, info, pid);
	// 目前不支持pid <= 0的情形
	else
		while (1);
~~~


`prepare_kill_siginfo(sig,&info,PIDTYPE_TGID)` 总在pid分流前执行。内部 `clear_siginfo` 以 `memset(info,0,sizeof(*info))` 清空结构，再设置 `si_signo=sig`、`si_errno=0`、`si_code=SI_USER`（传入TGID，非PID）、`si_pid=task_tgid_vnr(current)`；`si_uid` 的设置行被注释，**本次由清零得0而不是未经初始化**。`si_pid` 来源是调用者当前视图，不是查找出的目标PID。参见SW10-A05/A06及`sched/task/pid.h::task_tgid_vnr`。

正pid条件进入 `kill_proc_info`：先以 `find_vpid(pid)` 从 `task_active_pid_ns(current)` 的IDR取 `pid_s*`，再以 `pid_task(...,PIDTYPE_PID)` 取任务指针；若为NULL，局部 `error` 从初值 `-ESRCH` 返回。若非NULL，以固定 `type=PIDTYPE_TGID` 调 `group_send_sig_info`，所以**查询任务的PIDTYPE_PID** 与 **选择共享pending的PIDTYPE_TGID** 不应混作一种语义。发送目标并不因为函数名中含group就自动枚举所有线程。`pid_task` 自身会检查pid指针是否NULL，但当前调用链中 `rcu_read_lock`、`tasklist_lock` 等保护语句被注释，远端生命周期、任务摘链竞态及跨namespace权限不作已验证声明（SW10-A08～A14/A45）。

[VERIFIED mykernel/lock_IPC/signal/signal.c::kill_proc_info]（SW10-A08，本轮从time固定源码回读）

~~~c
	for (;;) {
		// rcu_read_lock();
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p)
			error = group_send_sig_info(sig, info, p, type);
~~~

[VERIFIED mykernel/sched/task/pid.c::find_vpid]（SW10-A10，本轮从time固定源码回读）

~~~c
pid_s *find_vpid(int nr) {
	return find_pid_ns(nr, task_active_pid_ns(current));
}
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::group_send_sig_info]（SW10-A13，本轮从time固定源码回读）

~~~c
	// if (!ret && sig) {
		// ret = do_send_sig_info(sig, info, p, type);
		// if (lock_task_sighand(p, &flags)) {
			ret = send_signal_locked(sig, info, p, type);
		// 	unlock_task_sighand(p, &flags);
~~~


没有活动权限检查：`group_send_sig_info` 中的 `check_kill_permission`、`lock_task_sighand` 及相应解锁语句均是注释；`send_signal_locked` 虽有“locked”名称却只是令 `force=false` 并调用 `__send_signal_locked`，后者的 `lockdep_assert_held` 也被注释（SW10-A12～A16）。09接收端 `get_signal` 会主动取得 `sighand->siglock`，**但不等于发送端也取得同锁**。上述仅对直接可读的kill链成立，没有对所有调用者、RCU或全局并发给出不存在性证明。

## 2. S01：输入与结果矩阵（参考结构化SW10-M01～M18）

| 组合 | 实际分流、返回与副作用（源码条件） |
|---|---|
| 正pid存在；可能恰好为自己 | `pid_task` 非NULL则到 `group_send_sig_info`；后续若定义良好地返回，`__send_signal_locked` 局部 `ret=0`，没有同步通知/唤醒。自身无专门分支（M01/M03） |
| 正pid不存在 | 目标NULL，不调用发送函数，`kill_proc_info` 的局部返回 `-ESRCH`（M02） |
| pid=0、pid=-1、pid小于-1 | `kill_something_info` 的非正pid支路是活动 `while(1)`，进不了查找、排队或返回；进程组与广播代码已注释（M04～06） |
| sig=0 | 当前发送侧无0号特判；若正pid目标存在，会走入 `sigaddset` 的 `sig-1` 移位，语言层前提失效，不能报告“仅探测权限”或固定返回（M07） |
| sig负值、sig≥65 | 既无活动 `valid_signal` 检查，也无可靠越界防护；负值可能引起超范围移位，极小负值涉及有符号减法；≥65的移位计数超过当前64位单词位数，不能预测最终位图或返回（M08/M11） |
| sig=1、sig=64 | `_NSIG=64`、64位配置对应位号0与63；实际仍受节点分配的未初始化参数、锁/对象有效性等前提限制（M09/10） |
| SIGKILL=9 | 在 `__send_signal_locked` 直接 `goto out_set`，此次发送不新建siginfo节点；仍尝试置pending位，未通知唤醒（M12） |
| 普通信号SIGTERM=15、实时SIGRTMIN=32 | 都进入相同分配路径；`legacy_queue`、按SIGRTMIN调整`override_rlimit`、溢出时返回EAGAIN等逻辑均为注释。若分配成功可产生重复同号节点；不能宣布“只允许一个普通信号节点”或“实时信号已有完整排队保证”（M13/M14） |
| 目标为PF_KTHREAD | 与SIGKILL一样绕过此次节点分配，直达pending位更新；能否被消费/唤醒另需条件（M15） |
| 分配失败；分配成功 | 失败仍可走 `out_set` 置位却没有此次的节点；成功则先插入链表再写info。均不能将局部return0当作端到端成功（M16/17） |
| `SEND_SIG_NOINFO`、`SEND_SIG_PRIV` | 这是`__send_signal_locked` 的特殊指针分支；本`sys_kill` 传的是栈上真实 `&info`，进入default的复制路径，而不是这两条特殊分支（M18） |

矩阵中的“若正常返回”不是替未定义行为证明会正常返回；号合法、目标有效也无法抹去 `override_rlimit` 未初始化这个独立问题。

## 3. S02：节点、位图、返回值是三种独立结果

`__send_signal_locked` 根据参数 `type != PIDTYPE_PID` 选择 `t->signal->shared_pending`，否则用 `t->pending`。kill当前传TGID，所以在有效指针前提下写**共享pending**；不存在“因为pid_task用了PIDTYPE_PID，所以写私有pending”的结论（SW10-A16）。

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A16，本轮从time固定源码回读）

~~~c
	// result = TRACE_SIGNAL_IGNORED;
	// if (!prepare_signal(sig, t, force))
	// 	goto ret;

	pending = (type != PIDTYPE_PID) ? &t->signal->shared_pending : &t->pending;
~~~


发送函数声明 `int override_rlimit;` 却把其用于赋值的按普通/实时信号分支整段注释；随后直接传给 `__sigqueue_alloc(...,override_rlimit,0)`。分配函数虽将 `long sigpending=0`，却仍在 `if (override_rlimit || likely(sigpending<=task_rlimit(...)))` 读取该未初始化局部参数。该C语言层问题不能被“另一边条件通常为真”或“分配成功记录”掩盖；以下关于q的行为均明确以分配调用在适当定义良好前提下继续为条件。

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A15，本轮从time固定源码回读）

~~~c
	sigpending_s *pending;
	sigqueue_s *q;
	int override_rlimit;
	int ret = 0, result;
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A19，本轮从time固定源码回读）

~~~c
	q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::__sigqueue_alloc]（SW10-A21，本轮从time固定源码回读）

~~~c
	if (override_rlimit || likely(sigpending <= task_rlimit(t, RLIMIT_SIGPENDING))) {
		q = kmem_cache_alloc(sigqueue_cachep, gfp_flags);
~~~


进入 `q!=NULL` 分支时，`__sigqueue_alloc` 先初始化链表节点与flags，调用方之后 `list_header_add_to_tail(&pending->list,&q->list)`，**随后**才根据info指针分支填 `q->info`。对本kill真实 `&info` 指针取default并 `copy_siginfo`，复制由 `prepare_kill_siginfo` 准备的 `si_signo、si_code、si_pid、si_uid`（UID在准备阶段清零）；特殊指针分支不由本入口命中。发送方实际锁不存在使“先插链再填信息”带来并发前提风险；并不能声称已观测到竞态。

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A23，本轮从time固定源码回读）

~~~c
	if (q) {
		// list_add_tail(&q->list, &pending->list);
		list_header_add_to_tail(&pending->list, &q->list);
		switch ((ulong) info) {
		case (ulong) SEND_SIG_NOINFO:
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A24，本轮从time固定源码回读）

~~~c
		default:
			copy_siginfo(&q->info, info);
			break;
~~~


无论是因SIGKILL/PF_KTHREAD跳过分配、q为空，还是q被加入链表，源码随后都执行 `sigaddset(&pending->signal,sig)`（在参数有效且控制到达时）。`sigaddset` 的单词版是对 `1UL << (sig-1)` 的无保护移位；它不做0/负数/超过范围检查。`valid_signal(ulong)` 虽存在，但这条发送链没有调用。`_NSIG` 为64，`SIGRTMIN` 为32；没有可用的“非法值自动归一化”证据（SW10-A25/A27/A28）。末尾 `ret` 初始化为0，且发送函数中改变ret为EAGAIN的分支被注释；“局部返回0”在未定义参数和并发风险之前不具备任务已收到/已被唤醒的证明力。

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A25，本轮从time固定源码回读）

~~~c
out_set:
	// signalfd_notify(t, sig);
	sigaddset(&pending->signal, sig);
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::__send_signal_locked]（SW10-A26，本轮从time固定源码回读）

~~~c

	// complete_signal(sig, t, type);
ret:
	// trace_signal_generate(sig, info, t, type != PIDTYPE_PID, result);
	return ret;
~~~


**缓存初始化边界。** `signals_init` 用 `KMEM_CACHE(sigqueue,SLAB_PANIC|SLAB_ACCOUNT)` 创建 `sigqueue_cachep`，从所读 `start_kernel` 的活动调用顺序看它在 `proc_caches_init、vfs_caches_init` 后执行（SW10-A41/A42）。这确立源码调用关系，不是检查过slab成功返回、任务实际执行时序或宿主启动记录。`copy_signal` 对新共享signal对象采用 `kmem_cache_zalloc` 并调用 `init_sigpending`；后者只初始化链表、清位图语句被注释。fork私有pending位可能继承父值的问题继续沿用 `CORE-USER-RETURN-SIGNAL-09::RS-13`，不从本批签发独立新根因。

## 4. S02：接收端只取必要接口，不重做09

当前目标接收端 `get_signal` 先尝试私有同步信号，然后调用 `dequeue_signal(current,&current->blocked,...)`；`dequeue_signal` 先用 `tsk->pending`，没有可提取的信号再用 `tsk->signal->shared_pending`。`next_signal` 对pending位和blocked掩码做运算；对本kill共享pending来说，只有当前进程后续到达返回端、没有更早选中的私有信号且相关位未屏蔽，才可能被这条消费路径选择。未被调度、正在睡眠、仍屏蔽的目标不会因“pending置位”就自动完成投递。

[VERIFIED mykernel/lock_IPC/signal/signal.c::dequeue_signal]（SW10-A29，本轮从time固定源码回读）

~~~c
	*type = PIDTYPE_PID;
	signr = __dequeue_signal(&tsk->pending, mask, info, &resched_timer);
	if (!signr) {
		*type = PIDTYPE_TGID;
		signr = __dequeue_signal(&tsk->signal->shared_pending,
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::get_signal]（SW10-A33，本轮从time固定源码回读）

~~~c
		type = PIDTYPE_PID;
		signr = dequeue_synchronous_signal(&ksig->info);
		if (signr == 0)
			signr = dequeue_signal(curr, &curr->blocked,
							&ksig->info, &type);
~~~


`collect_signal` 沿pending链按 `si_signo` 找节点；有同号多节点时在第二个节点处跳到 `still_pending`，保留该位，摘出第一个匹配节点并复制info、调用 `__sigqueue_free`。如果没有节点，则清info后合成 `si_signo=sig, si_code=SI_USER, si_pid=0, si_uid=0`；所以**位存在不等于info节点存在，节点不存在也不等于dequeue绝不返回该信号号**（SW10-A30～A32）。此处是有界调用与数据后置关系，未重算09所有查询/锚点。

[VERIFIED mykernel/lock_IPC/signal/signal.c::collect_signal]（SW10-A32，本轮从time固定源码回读）

~~~c
		info->si_signo = sig;
		info->si_errno = 0;
		info->si_code = SI_USER;
		info->si_pid = 0;
		info->si_uid = 0;
~~~


接收端本身还受09已经记录的限制：`CORE-USER-RETURN-SIGNAL-09::RS-03` 的默认动作并未完整实现终止/停止；`RS-04` 的返回尾信号门被注释，`get_signal` 仍在经过指定 `syscall_exit_to_user_mode` 的出口被调用；`RS-12` 的共享sighand/锁有跨任务前提；`RS-13/14` 是分别附带私有pending继承/同步出队前提的潜伏问题。09/26号既有消费限制继续生效，不把任意中断或异常出口、所有sleep状态或SIGKILL硬件表现当作已核。

## 5. S03：唤醒链在哪里断开

`__send_signal_locked` 中的 `complete_signal(sig,t,type)` 调用是注释，因此当前kill路径**没有活动的边**到该函数（SW10-E15标注commented）。函数体末尾虽有 `signal_wake_up(t,sig==SIGKILL)`，但此前选择 `t` 的主体整段也是注释，`task_s *t` 在当前函数体没有活动赋值；只能作为**潜伏风险**登记，不应伪画成已执行的call，也不能把它的本体错误说成此kill必经错误。

[VERIFIED mykernel/lock_IPC/signal/signal.c::complete_signal]（SW10-A34，本轮从time固定源码回读）

~~~c
	/*
	 * The signal is already in the shared-pending queue.
	 * Tell the chosen thread to wake up and dequeue it.
	 */
	signal_wake_up(t, sig == SIGKILL);
~~~


`signal_wake_up` 宏映射到 `signal_wake_up_state`。后者先 `set_tsk_thread_flag(t,TIF_SIGPENDING)`，再调用 `wake_up_state(t,state|TASK_INTERRUPTIBLE)`；当它返回0时调用 `kick_process(t)`。`wake_up_state` 当前包装器实际进入 `try_to_wake_up`；所读实现里 `success` 初始化为0而返回前没有活动成功赋值、对runqueue的 `ttwu_queue` 调用被注释；`kick_process` 的 `smp_send_reschedule` 也被注释。这些属于尚未接入本kill链的**潜伏唤醒接口局部事实**，不报告任何真实任务已经被它唤醒、跨CPU收到IPI或得到实时调度保证。

[VERIFIED mykernel/lock_IPC/signal/signal.c::signal_wake_up_state]（SW10-A37，本轮从time固定源码回读）

~~~c
	if (!wake_up_state(t, state | TASK_INTERRUPTIBLE))
		kick_process(t);
}
~~~

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_state]（SW10-A38，本轮从time固定源码回读）

~~~c
int wake_up_state(task_s *p, uint state)
{
	return try_to_wake_up(p, state, 0);
}
~~~

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::kick_process]（SW10-A39，本轮从time固定源码回读）

~~~c
	int cpu = task_cpu(p);

	// if ((cpu != smp_processor_id()) && task_curr(p))
	// 	smp_send_reschedule(cpu);
}
~~~


对四类目标必须区分：
- **正在执行/将自然再次运行：** pending未被屏蔽且安全可读时，可能在目标下次经过已核的返回尾被取走；不是“发送方立即推送handler”。
- **睡眠任务：** 本次kill不调用complete_signal，所以没有这条路径提供的活动 `wake_up_state`；未来是否通过其他独立事件唤醒、何时返回，未追。
- **被屏蔽的信号：** 位图/节点可留下，需后续解除屏蔽且经过接收路径才有处理机会；解除屏蔽本身没有在此批证明触发唤醒。
- **向自身发送：** 正pid也按同一发送链入共享pending，当前syscall若到达正常返回尾可发生随后消费；不能倒推准备阶段同步执行了handler。

## 6. 旧项关联、范围和结论强度

| 已登记旧项 | 本次增量 | 当前状态处理 |
|---|---|---|
| `CORE-USER-RETURN-SIGNAL-09::RS-03` | 发送方SIGKILL仅置pending，不负责默认终止；接收方默认动作的原缺口仍是独立接收端条件 | 只加发送侧证据，旧状态保留 |
| `CORE-USER-RETURN-SIGNAL-09::RS-04` | `complete_signal` 调用注释，`signal_wake_up_state` 虽有TIF置位实现而当前不达；返回端仍因门被注释而可扫描pending | 补充“置位端不可由此kill到达”的限定，非全树不存在性证明 |
| `CORE-USER-RETURN-SIGNAL-09::RS-12` | 发送方读取目标signal/sighand、未持锁且PID对象寿命未审，和共享sighand问题相互关联 | 只作evidence append，不把同源风险重复计为独立根因 |
| `CORE-USER-RETURN-SIGNAL-09::RS-13` | 当前sys_kill传TGID写共享pending，不是产生私有pending继承前提的发送者 | 原潜伏条件仍在，不升级为现行kill触发 |
| `CORE-USER-RETURN-SIGNAL-09::RS-14` | 当前kill写共享pending且 `si_code=SI_USER`，并不满足私有同步队列 `si_code>SI_USER` 的触发组合 | 保留潜伏；未核全部其他生产者 |

本批新发现与延期证据见 `deferred-findings.yaml`，其性质含文档式风险界限、未定义前提及潜伏接口。均不要求Owner现在修内核；不能将延期当作已经解决。`CORE-USER-RETURN-SIGNAL-09::RS-01` 仍needs_evidence，`CORE-USER-VFS-BASELINE-05::GAP-U04` 的硬件/运行部分与 `GAP-U10` 仍开放，09的SR09-01既已限定关闭，本批不重开。

**消费优先级：** 此说明给阅读路径；有界矩阵、可追踪边、源码引文在 `facts-and-boundaries.yaml`；图在 `send-paths.dot`。带 `conditional`、`commented`、`latent` 的边不得作为已执行链路证明；全局调度、权限/锁和未定义路径需未来独立授权后再核。

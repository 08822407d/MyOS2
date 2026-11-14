---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PID-LIFETIME-11
record_type: bounded_pid_lifecycle_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话（Owner选择 GPT-6 / Extra；后台未独立认证）"
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；execution_base=848e2acdf7e1；instruction_base=69523af0507e"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
inputs_read:
  - "29号合同与10主线回执；P00新增signal10-graph-disposition.md"
  - "time固定源码：sched/syscall.c、forkexec/fork.c/exit.c、task/pid.c及直接pid头、PID namespace/IDR直接实现、init/main.c和init/init_task.c、调度死亡函数"
  - "05 C2/C6及UV-14/UV-15，10的正PID查询链；不重做MM/VFS/08–10"
open_questions:
  - "未运行编译/预处理、PID并发、异常/调度、外部libc"
  - "exit_notify的实参/未初始化值所致实际执行结果不确定"
  - "主线SR10-01是否关闭及05历史陈述的修订由C03裁定"
---

# 第11批：PID创建、发布、查询引用与退出摘除

## 0. 本批结论

**分得数字、IDR发布PID对象、将任务挂到PID对象，以及让任务尝试运行，是四个不同的源码时点**。在固定的 `time @ a039d9803ade`，普通用户 `sys_fork→kernel_clone→copy_process→alloc_pid` 先分配ID数字，再把 `pid_s*` 赋入IDR；`attach_pid` 以后 `pid_task` 才有可读的任务链节点。固定IDR采用简单指针数组，`NULL` 空槽当作预留标记却没有占位效果，不能将“已申请数字”提升为“已互斥占号”。

**查找不等于引用。** `find_vpid`/ `pid_task` 返回裸指针；`get_pid` 对PID对象加引用，不能推成task引用。第10批 `kill` 用的两个裸指针，在已读发送链中没有活动RCU锁或对象引用证明；安全和具体竞态结果都留作未知。

**退出未与摘链闭合。** 相关 `do_exit` 直接链里没有 `detach_pid/free_pid/idr_remove` 的活动连接，且 `group_dead` 未初始化即被传参，`exit_notify` 的 `find_task_by_pid_ns` 第二实参类型也不符。不能把旧05概述的“退出后已改挂给1号”或Linux回收习惯当作此版本无条件事实；不宣称实际运行一定崩溃或所有PID永不重用。

本报告只作阶段2静态取证，P00 的 SR10-01 图关联处置另见 `signal10-graph-disposition.md`。本批 `accept` 并不替C03关闭第10批原RETURN。

## 1. P01：fork入口与PID三时点

### 1.1 普通用户fork，而非任意内部clone参数

`mykernel/sched/syscall.c::sys_fork` 先空转，再以 `exit_signal=SIGCHLD`、其余零初始化的 `kclone_args_s` 调用 `kernel_clone`；与内部 `kernel_thread` 构造不同。 `kernel_clone` 在检查部分clone标志后调 `copy_process`，成功后按PID引用求取调用者可见编号，再调用 `wake_up_new_task`。这与“fork返回正pid”在源码级是相邻调用，但不能代替内核运行验证。

**[VERIFIED `mykernel/sched/syscall.c::sys_fork` · PID11-A02，60–64行]**

```c
	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

**[VERIFIED `mykernel/sched/forkexec/fork.c::kernel_clone` · PID11-A62，1485–1489行]**

```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
	// add_latent_entropy();

	if (IS_ERR(p))
		return PTR_ERR(p);
```


`copy_process` 完成直接相关资源复制/ `copy_thread` 后，对非特殊 `init_struct_pid` 路径调用 `alloc_pid(p->nsproxy->pid_ns_for_children,args->set_tid,args->set_tid_size)`，遇ERR_PTR则从 `PTR_ERR` 取得错误并跳清理。 `sys_fork` 没有设置set_tid，因此通常走 `idr_alloc_cyclic(...,NULL,...)`；特定set_tid则是 `idr_alloc`。

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A52，1149–1153行]**

```c
	if (pid_struct != &init_struct_pid) {
		pid_struct = alloc_pid(p->nsproxy->pid_ns_for_children,args->set_tid, args->set_tid_size);
		if (IS_ERR(pid_struct)) {
			retval = PTR_ERR(pid_struct);
			goto bad_fork_cleanup_thread;
```

**[VERIFIED `mykernel/sched/task/pid.c::alloc_pid` · PID11-A13，230–234行]**

```c
			/*
			 * Store a null pointer so find_pid_ns does not find
			 * a partially initialized PID (see below).
			 */
			nr = idr_alloc_cyclic(&tmp->idr, NULL,
```


### 1.2 三个可见性阶段

**T1：数字扫描 / NULL预留。** `alloc_pid` 逐namespace选数并写 `pid->numbers[i].nr/ns`。它向IDR传 `NULL`，简化 `idr_alloc_u32` 把查到的空槽再写成NULL，仅通过 `idr_next` 等局部状态记录返回数字。真实数组大小是32768，内部扫描比较 `MYOS_IDR_BUF_SIZE` 而没有按传入max做区间截断。由此不能保证重复/并发分配不会抢到同一个尚未发布的槽；但循环游标的具体过程又不能让我们断言每两次调用必冲突。

**[VERIFIED `mykernel/lib/idr/idr.c::idr_alloc_u32` · PID11-A66，53–57行]**

```c
	for (int slot_idx = id; slot_idx < MYOS_IDR_BUF_SIZE; slot_idx++)
	{
		void **tmp_slot = &idr->idr_rt[slot_idx];
		if (*tmp_slot == NULL)
		{
```

**[VERIFIED `mykernel/lib/idr/idr.c::idr_alloc_u32` · PID11-A67，58–61行]**

```c
			*tmp_slot = ptr;
			*nextid = slot_idx;
			return 0;
		}
```


**T2：PID对象IDR可查。** `alloc_pid` 初始化 `pid->count=1` 与 `pid->tasks[type]` 空表，检查 `PIDNS_ADDING` 后在各级IDR使用 `idr_replace` 把 `pid_s*` 放进槽并增加计数。因此在IDR槽有效且未被其他写入覆盖时，`find_pid_ns` 已可返回对象，而此刻它的任务链依然可能为空：**对象可查并不保证 `pid_task` 非NULL**。 `idr_replace` 本树只是无验证地直接赋值，不能假设标准Linux IDR的占位契约成立。

**[VERIFIED `mykernel/sched/task/pid.c::alloc_pid` · PID11-A15，260–264行]**

```c
	get_pid_ns(ns);
	refcount_set(&pid->count, 1);
	spin_lock_init(&pid->lock);
	for (type = 0; type < PIDTYPE_MAX; ++type)
		INIT_HLIST_HEAD(&pid->tasks[type]);
```

**[VERIFIED `mykernel/sched/task/pid.c::alloc_pid` · PID11-A17，274–278行]**

```c
	pid->ino = ++pidfs_ino;
	for ( ; upid >= pid->numbers; --upid) {
		/* Make the PID visible to find_pid_ns. */
		idr_replace(&upid->ns->idr, pid, upid->nr);
		upid->ns->pid_allocated++;
```

**[VERIFIED `mykernel/lib/idr/idr.c::idr_replace` · PID11-A70，318–322行]**

```c
	// __radix_tree_replace(&idr->idr_rt, node, slot, ptr);

	entry = idr->idr_rt[id];
	idr->idr_rt[id] = ptr;

```


**T3：任务关联。** `copy_process` 用 `pid_nr(pid_struct)` 设置 `p->pid`，在 `p->pid!=0` 门下初始化 `pid_links` 并赋 `thread_pid`；只有 `thread_group_leader(p)` 成立才设置TGID/PGID/SID指针并挂对应链，最后挂PIDTYPE_PID链。 `p->exit_signal=args->exit_signal` 为注释，不能无条件把所有内部clone分支视作leader。外层 `write_lock_irq(&tasklist_lock)` 在此处也为注释；任务列表的源码可见性与多CPU并发安全仍是两件事。

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A53，1205–1209行]**

```c
	/* ok, now we should be set up.. */
	p->pid = pid_nr(pid_struct);
	if (clone_flags & CLONE_THREAD) {
		// p->group_leader = current->group_leader;
		p->tgid = current->tgid;
```

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A56，1310–1314行]**

```c
	init_task_pid_links(p);
	if (likely(p->pid != 0)) {
		ptrace_init_task(p, (clone_flags & CLONE_PTRACE) || trace);

		init_task_pid(p, PIDTYPE_PID, pid_struct);
```

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A58，1333–1337行]**

```c
			list_header_add_to_tail(&p->real_parent->children, &p->sibling);
			// list_add_tail_rcu(&p->tasks, &init_task.tasks);
			list_header_add_to_tail(&all_task_lhdr, &p->tasks);
			attach_pid(p, PIDTYPE_TGID);
			attach_pid(p, PIDTYPE_PGID);
```

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A59，1347–1351行]**

```c
			// list_add_tail_rcu(&p->thread_node,
			// 		  &p->signal->thread_head);
		}
		attach_pid(p, PIDTYPE_PID);
		nr_threads++;
```


`kernel_clone` 随后使用 `get_task_pid(p,PIDTYPE_PID)` 为PID对象临时增加引用、`pid_vnr` 求当前namespace编号，调用 `wake_up_new_task`，再 `put_pid`。这不是给task永久取得引用；新任务是否真实运行并不在本批验证范围。

**[VERIFIED `mykernel/sched/forkexec/fork.c::kernel_clone` · PID11-A63，1497–1499行]**

```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);

```

**[VERIFIED `mykernel/sched/forkexec/fork.c::kernel_clone` · PID11-A64，1515–1517行]**

```c

	wake_up_new_task(p);

```

**[VERIFIED `mykernel/sched/forkexec/fork.c::kernel_clone` · PID11-A65，1527–1529行]**

```c
	put_pid(pid_struct);

	return nr;
```


**初始PID是特殊路径。** `init_task.thread_pid=&init_struct_pid`，`init_signals.pids` 也引用静态PID；`pid_idr_init` 在 `init_pid_ns.idr` 的0槽手工写 `init_struct_pid`。它的 `tasks[type]` 初值是空表，故“IDR能查到0号PID对象”不等于 `pid_task(...,PIDTYPE_PID)` 已能查到任务。 `start_kernel` 的PID初始化调用确实先于 `rest_init` 出现，但运行初始化是否成功仍NOT_RUN。

**[VERIFIED `mykernel/sched/task/pid.c::pid_idr_init` · PID11-A42，508–511行]**

```c
	/* MyOS2 init_pid_ns initiate simple-idr by hand */
	idr_s *init_pid_ns_idr = &init_pid_ns.idr;
	init_pid_ns_idr->idr_rt[0] = &init_struct_pid;
	init_pid_ns_idr->idr_next++;
```


### 1.3 创建失败和返回值

| 状态（facts中的ST） | 可确认的文本行为 | 尚未证明 |
|---|---|---|
| ST01–02 初始分配及NULL槽 | 入口异常可ERR_PTR；成功扫描先得到数字 | 没有PID对象发布，也不具排他占号保证 |
| ST03–04 跨namespace部分成功、PID对象初始化 | `alloc_pid::out_free` 移除之前阶段分配的槽；后阶段初始化引用和各任务链表头 | 不能将返回errno一概固定，亦不能将空任务链当已有task |
| ST05 发布PID对象 | `idr_replace` 后IDR可返回pid_s | task链接尚未建立，`pid_task` 仍可NULL |
| ST06–08 任务挂接与fork后续 | 条件满足时挂PID与leader相关链，kernel_clone尝试唤醒并返回编号 | 不保证每种clone条件下都可查，未执行调度 |
| ST09–10 alloc_pid失败 | 其内部 `out_free` 做逆序清理，再由copy_process走失败标签 | 不意味着存在成功发布后再调用 `free_pid` 的活动回滚 |
| ST11 成功发布之后的失败标签 | `bad_fork_free_pid` 在源码中存在，但相关PIDFD/后置失败跳转为注释 | 不得把不连通的标签当当前用户fork正常清理证明 |

**[VERIFIED `mykernel/sched/task/pid.c::alloc_pid` · PID11-A18，288–292行]**

```c
out_free:
	spin_lock_irq(&pidmap_lock);
	while (++i <= ns->level) {
		upid = pid->numbers + i;
		idr_remove(&upid->ns->idr, upid->nr);
```

**[VERIFIED `mykernel/sched/forkexec/fork.c::copy_process` · PID11-A60，1385–1389行]**

```c
bad_fork_free_pid:
	if (pid_struct != &init_struct_pid)
		free_pid(pid_struct);
bad_fork_cleanup_thread:
	exit_thread(p);
```


尤其不要把 `alloc_pid` 自己的错误清理与 `copy_process::bad_fork_free_pid` 视为同一活动分支。05::UV-14已登记失败时共享sighand引用的另一缺口，本批继续引用，不重签同源MM/VFS根因。

## 2. P02：namespace、裸查找与引用所有权

`task_active_pid_ns(tsk)` 宏展开为 `ns_of_pid(task_pid(tsk))`。 `find_vpid(nr)` **以current的namespace** 调 `find_pid_ns`，后者直接读 `idr_find(&ns->idr,nr)`。 `pid_nr_ns` 则须 `pid` 非NULL且namespace level与 `upid.ns` 精确匹配才返回对应数字，否则返回0；无任务引用增量。

`task_pid_ptr` 对 `PIDTYPE_PID` 取 `thread_pid`，对TGID/PGID/SID取 `task->signal->pids[type]`；`pid_task` 从 `pid->tasks[type]` 的第一个链节点计算 `task_s*`。查找用哪个pid_type、拿到哪个对象，必须按该调用点而非字段名判断。

**[VERIFIED `mykernel/sched/task/pid.c::find_vpid` · PID11-A25，310–312行]**

```c
pid_s *find_vpid(int nr) {
	return find_pid_ns(nr, task_active_pid_ns(current));
}
```

**[VERIFIED `mykernel/sched/task/pid.c::task_pid_ptr` · PID11-A26，315–319行]**

```c
static pid_s **task_pid_ptr(task_s *task, enum pid_type type) {
	if (type == PIDTYPE_PID)
		return &task->thread_pid;
	else
		return &task->signal->pids[type];
```

**[VERIFIED `mykernel/sched/task/pid.c::pid_task` · PID11-A32，371–375行]**

```c
		first = hlist_first_rcu(&pid->tasks[type]);
		if (first)
			result = hlist_entry(first, task_s, pid_links[(type)]);
	}
	return result;
```

**[VERIFIED `mykernel/sched/task/pid.c::pid_nr_ns` · PID11-A38，450–454行]**

```c
	pid_t nr = 0;

	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
```


### 所有权矩阵（与facts OWN01–OWN11一致）

| API | 实际增加的引用 | 失效与释放边界 |
|---|---|---|
| `find_pid_ns/find_vpid` | 无，返回裸pid_s或NULL | 调用本身无RCU加锁；需外部对象生存前提 |
| `pid_task` | 无，返回裸task_s或NULL | 只读目标pid类型对应HList，无task引用 |
| `get_pid` | **pid_s.count++**（非NULL） | `put_pid` 对应减计数，不保活task_s |
| `get_task_pid` | 内部get_pid增加PID引用 | 当前函数的RCU保护为注释；kernel_clone后有put_pid |
| `find_get_pid` | find_vpid后get_pid增加PID引用 | RCU注释，不能证明从裸查找到加引用的竞态被消除 |
| `get_pid_task` | **没有task引用增量** | `get_task_struct` 为注释；返回裸task |
| `find_get_task_by_vpid` | **没有task引用增量** | get_task_struct及RCU同为注释 |
| `find_task_by_pid_ns` | 无，返回裸task（有效namespace前提） | 第二参数必须是真正pid_ns_s*，不能传pid_type常量 |
| `task_tgid_vnr/ pid_nr_ns` | 无，仅返回数字 | 需有效namespace及稳定PID指针，不能解释为保活 |
| 10 `kill_proc_info` | 两次裸查找均无PID/task引用增量 | 目标在查找和写shared_pending之间的安全性尚未验证 |

**[VERIFIED `mykernel/sched/task/pid.h::get_pid` · PID11-A43，59–63行]**

```c
		PREFIX_STATIC_INLINE
		pid_s
		*get_pid(pid_s *pid) {
			if (pid) refcount_inc(&pid->count);
			return pid;
```

**[VERIFIED `mykernel/sched/task/pid.c::get_task_pid` · PID11-A35，409–413行]**

```c
	pid_s *pid;
	// rcu_read_lock();
	pid = get_pid(rcu_dereference(*task_pid_ptr(task, type)));
	// rcu_read_unlock();
	return pid;
```

**[VERIFIED `mykernel/sched/task/pid.c::get_pid_task` · PID11-A36，420–424行]**

```c
	// rcu_read_lock();
	result = pid_task(pid, type);
	// if (result)
	// 	get_task_struct(result);
	// rcu_read_unlock();
```

**[VERIFIED `mykernel/sched/task/pid.c::find_get_task_by_vpid` · PID11-A34，398–402行]**

```c
	// rcu_read_lock();
	task = find_task_by_vpid(nr);
	// if (task)
	// 	get_task_struct(task);
	// rcu_read_unlock();
```

**[VERIFIED `mykernel/sched/task/pid.c::find_get_pid` · PID11-A37，433–437行]**

```c
	// rcu_read_lock();
	pid = get_pid(find_vpid(nr));
	// rcu_read_unlock();

	return pid;
```


`put_pid` 在计数归零时释放pid cache对象并调用 `put_pid_ns`。固定源码中 `get_pid_ns` 对非初始namespace的增计数注释掉了，而 `put_pid_ns` 对非init namespace的循环体无有效更新或退出；本批只将这种**潜伏/有前提**的namespace引用问题记录，不扩大为完整PID namespace实现审计。

**与10的桥接。** `kill_proc_info` 先 `pid_task(find_vpid(pid),PIDTYPE_PID)`，只在非NULL时转入共享pending发送链。它没有选择 `find_get_pid` 或 `get_pid_task` 的替代引用方案；既不能保证目标task在并发退出下始终活着，也不能因未直接看到释放而保证其一直安全。若PID对象已发布但任务未attach，裸查找可能得NULL；若已attach而未按运行路径摘链，也不能直接对后续可达性签发运行结论。

`sys_getpid` 用 `task_tgid_vnr(current)` 返回当前namespace视图的编号；`sys_getppid` 使用 `current->real_parent`，其中RCU加解锁被注释。本批只交叉核对编号来源。

## 3. P03：exit的活动段、未连通摘链和重用边界

入口文本为 `sys_exit→do_exit`、`sys_exit_group→do_group_exit→do_exit`；`sys_wait4` 是空函数体，未实现可依赖的等待/回收返回。 `do_exit` 设置exit_code、调用部分资源清理，再调用 `exit_notify(tsk,group_dead)`，后部才出现 `do_task_dead`。05::UV-15保留等待/资源回收缺口，本批只增量核对PID列表与引用。

**[VERIFIED `mykernel/sched/syscall.c::sys_exit` · PID11-A03，68–72行]**

```c
MYOS_SYSCALL_DEFINE1(exit, int, error_code)
{
	ALERT_DUMMY_SYSCALL(exit, IF_ALERT_DUMMY_SYSCALL);

	do_exit(error_code);
```

**[VERIFIED `mykernel/sched/syscall.c::sys_exit_group` · PID11-A04，133–137行]**

```c
MYOS_SYSCALL_DEFINE1(exit_group, int, error_code)
{
	ALERT_DUMMY_SYSCALL(exit_group, IF_ALERT_DUMMY_SYSCALL);

	do_group_exit((error_code & 0xff) << 8);
```

**[VERIFIED `mykernel/sched/syscall.c::sys_wait4` · PID11-A05，76–80行]**

```c
MYOS_SYSCALL_DEFINE4(wait4, pid_t, pid, int *, start_addr,
		int, options, void *, rusage)
{

}
```

**[VERIFIED `mykernel/sched/forkexec/exit.c::do_exit` · PID11-A79，68–72行]**

```c
void __noreturn do_exit(long code)
{
	task_s *tsk = current;
	int group_dead;

```

**[VERIFIED `mykernel/sched/forkexec/exit.c::do_exit` · PID11-A83，152–154行]**

```c
	// exit_tasks_rcu_start();
	exit_notify(tsk, group_dead);
	// proc_exit_connector(tsk);
```


**两项必须显式保留的风险：** (1) `int group_dead` 只有声明，原计算逻辑被注释，却在exit_notify调用处作为实参使用；这属于C未初始化值前提，后续步骤不能无条件列为确定可达。(2) `exit_notify` 在 `tsk->children.count !=0` 时调用 `find_task_by_pid_ns(1,PIDTYPE_PID)`；该函数第二参数是 `pid_ns_s*`，不是pid_type。 `PIDTYPE_PID` 是enum的第一个常量0；如果该调用被编译接受并真的到达，其后的 `find_pid_ns` 要用 `ns->idr`，可能出现无效namespace指针。此处不是实际崩溃见证，且子任务链为空时不会走这个查询。05 C6那句“已改挂给1号”必须加此前提才能消费；历史文件冻结，是否签发文档勘误交C03。

**[VERIFIED `mykernel/sched/forkexec/exit.c::exit_notify` · PID11-A75，15–19行]**

```c
static void exit_notify(task_s *tsk, int group_dead)
{
	while (tsk->children.count != 0)
	{
		List_s * child_lp = list_header_remove_head(&tsk->children);
```

**[VERIFIED `mykernel/sched/forkexec/exit.c::exit_notify` · PID11-A76，19–23行]**

```c
		List_s * child_lp = list_header_remove_head(&tsk->children);
		while (child_lp == 0);

		task_s *task_initd = find_task_by_pid_ns(1, PIDTYPE_PID);
		list_header_add_to_tail(&task_initd->children, child_lp);
```

**[VERIFIED `mykernel/sched/task/pid.c::find_task_by_pid_ns` · PID11-A33，382–386行]**

```c
task_s *find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)
{
	// RCU_LOCKDEP_WARN(!rcu_read_lock_held(),
	// 		 "find_task_by_pid_ns() needs rcu_read_lock() protection");
	return pid_task(find_pid_ns(nr, ns), PIDTYPE_PID);
```

**[VERIFIED `mykernel/sched/task/pid_const.h::pid_type` · PID11-A49，6–10行]**

```c
	enum pid_type {
		PIDTYPE_PID,
		PIDTYPE_TGID,
		PIDTYPE_PGID,
		PIDTYPE_SID,
```


**定义存在与当前退出执行必须分开。** `detach_pid` 内调 `__change_pid`，后者摘 `pid_links[type]` 并在所有类型都没有任务时才调 `free_pid`；`free_pid` 可对各层IDR调用 `idr_remove`，最后调用 `put_pid`。不过在本批实际读取的 `exit.c` 的 `do_exit/exit_notify`、所读 `do_task_dead` 函数体没有活动的detach/free连接。 `release_task` 只在所读API中声明、在exit_notify中作为注释调用，不能把它画成当前退出的执行边。 `pid_s.count` 是否最终归零、IDR槽什么时候释放、编号何时重用均不可确定为实际完成。

**[VERIFIED `mykernel/sched/task/pid.c::detach_pid` · PID11-A30，353–355行]**

```c
void detach_pid(task_s *task, enum pid_type type) {
	__change_pid(task, type, NULL);
}
```

**[VERIFIED `mykernel/sched/task/pid.c::__change_pid` · PID11-A29，346–350行]**

```c
	for (tmp = PIDTYPE_MAX; --tmp >= 0; )
		if (pid_has_task(pid, tmp))
			return;

	free_pid(pid);
```

**[VERIFIED `mykernel/sched/task/pid.c::free_pid` · PID11-A22，150–154行]**

```c
		}

		idr_remove(&ns->idr, upid->nr);
	}
	spin_unlock_irqrestore(&pidmap_lock, flags);
```

**[VERIFIED `mykernel/lib/idr/idr.c::idr_remove` · PID11-A71，164–168行]**

```c
void *idr_remove(idr_s *idr, ulong id)
{
	// return radix_tree_delete_item(&idr->idr_rt, id - idr->idr_base, NULL);
	void *ptr = idr->idr_rt[id];
	idr->idr_rt[id] = NULL;
```


`do_task_dead` 的主体设置TASK_DEAD后进入 `__schedule`，末尾还有BUG及无限放松循环；它不包含PID摘链。又因为上游退出路径可能遇未定义前提，甚至不能无条件说每个exit均到达这里，更不能从该函数名推导task_s内存已释放。

**[VERIFIED `mykernel/sched/scheduler/scheduler_core.c::do_task_dead` · PID11-A86，1172–1176行]**

```c
void __noreturn do_task_dead(void)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	set_special_state(TASK_DEAD);

```

**[VERIFIED `mykernel/sched/scheduler/scheduler_core.c::do_task_dead` · PID11-A87，1180–1184行]**

```c
	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	for (;;)
```


### 退出后的查询状态

| 状态 | 已证的变化 | 对kill和重用的限制 |
|---|---|---|
| 尚未发布（ST02–04） | IDR可能只有NULL槽/未发布PID对象 | `find_vpid`可能NULL，不能证明独占数字 |
| PID已发布但无task链（ST05） | 数字映射可返回PID对象，task链初始为空 | `pid_task`可能NULL，kill可能拿不到task |
| 关联完成（ST07） | hlist可提供裸task指针 | 没有引用保护或调度运行保证 |
| 创建失败（ST09–11） | alloc_pid有内部out_free，postpublish标签有但未见活动入口 | 不能统一断言所有中途失败均完成同一套回收 |
| 退出中/死亡边界（ST12–14） | 文本有状态与exit_notify、do_task_dead调用 | group_dead未初始化/类型错误、缺摘链；没有固定“退出后kill返回ESRCH” |
| 假设完成detach/free（ST15–16） | 若经相关API实际更新，链空会导致IDR删除、引用减少 | 不是当前exit已证活动链；重用时点仍未知 |

## 4. 问题归档、反证与停止条件

`deferred-findings.yaml` 只新增本批风险与对旧05::UV-14/UV-15、10相关问题的证据增量。源码层潜伏问题不能因定义存在就标为“已触发”，旧05的fork失败和退出资源清理不重建独立编号。对于05关于exit_notify的宽泛描述，仅登记文档范围冲突候选，不编辑历史原件。

主动核对以下反命题：**NULL数字槽不是排他占号；pid对象可查不必有task；find/get并非一律持task引用；exit不等于PID被detach；free_pid有定义不等于现行exit调用；未初始化值或空wait4不能支持固定结果。** 证据取样范围、引用/图校验及未运行项见 `evidence.md`。第10批SR10-01的本批处置并不意味着主线退回自动关闭。

不做全PID namespace、完整并发/IDR内核行为、MM/VFS、全调度、外部libc、FPU/ISA、CMake预处理、内核/QEMU或用户程序运行。对真实系统可靠性不签PASS；这七件只供C03按 `PASS_PENDING_LOCAL` 上限作有限静态验收。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FORK-RETURN-IDENTITIES-135
record_type: phase2_new_fixed_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
model_per_owner: "GPT-6＋Extra"
actual_backend_model: UNATTESTED
actual_backend_effort: UNATTESTED
produced_by: GPT-6 self-reported
date: 2026-11-23
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
self_check: {verified_claims: 66, quotes_reconfirmed: 66, downgraded_to_inferred: 0}
---
# 第135批独立源码引文与窄断言

本批新增正式短窗 66 个、独立定位 15 个，均按time固定对象直接逐行复核，保留原有tab和CRLF。E135-001单独锚定task_pid体内第125行，支持134旧E134-021的虚拟一对一替代；不修改134旧47正式/16定位及任何历史制品。每个新增正式窗都附独立窄断言。条件编译和连接、真实系统调用桥接及实机均NOT_RUN。

## E135-001 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::task_pid]
```c
			return task->thread_pid;
```

**窄断言**：task_pid函数体返回task的thread_pid指针；纠正旧E134-021签名混入。

固定time物理行 125–125；同一有效函数或宏定义范围连续1–5行。

## E135-002 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

**窄断言**：只有ns为空才先取当前任务命名空间，然后进入目标slot的rcu_dereference表达式。

固定time物理行 473–475；同一有效函数或宏定义范围连续1–5行。

## E135-003 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::task_pid_ptr]
```c
	if (type == PIDTYPE_PID)
		return &task->thread_pid;
	else
		return &task->signal->pids[type];
```

**窄断言**：按type选择thread_pid指针槽或signal.pids[type]指针槽。

固定time物理行 316–319；同一有效函数或宏定义范围连续1–5行。

## E135-004 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid_api.h::task_active_pid_ns]
```c
	#define task_active_pid_ns(tsk)	ns_of_pid(task_pid(tsk))
```

**窄断言**：缺省观察者由task_pid(tsk)的结果传给ns_of_pid。

固定time物理行 12–12；同一有效函数或宏定义范围连续1–5行。

## E135-005 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::ns_of_pid]
```c
			pid_ns_s *ns = NULL;
			if (pid)
				ns = pid->numbers[pid->level].ns;
			return ns;
```

**窄断言**：非空PID对象的最高层numbers[]命名空间是ns_of_pid返回值。

固定time物理行 85–88；同一有效函数或宏定义范围连续1–5行。

## E135-006 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::task_pid_vnr]
```c
			return __task_pid_nr_ns(tsk, PIDTYPE_PID, NULL);
```

**窄断言**：PID vnr helper传PIDTYPE_PID和NULL命名空间。

固定time物理行 144–144；同一有效函数或宏定义范围连续1–5行。

## E135-007 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::task_tgid_vnr]
```c
			return __task_pid_nr_ns(tsk, PIDTYPE_TGID, NULL);
```

**窄断言**：TGID vnr helper传PIDTYPE_TGID和NULL命名空间。

固定time物理行 173–173；同一有效函数或宏定义范围连续1–5行。

## E135-008 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
```

**窄断言**：先判PID非空且层级允许，再索引并比较ns指针，只有相等才读nr。

固定time物理行 452–455；同一有效函数或宏定义范围连续1–5行。

## E135-009 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	pid_t nr = 0;
```

**窄断言**：pid_nr_ns默认返回变量初始化为0。

固定time物理行 450–450；同一有效函数或宏定义范围连续1–5行。

## E135-010 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	return nr;
```

**窄断言**：pid_nr_ns返回已决定或仍为零的nr。

固定time物理行 457–457；同一有效函数或宏定义范围连续1–5行。

## E135-011 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::pid_vnr]
```c
	return pid_nr_ns(pid, task_active_pid_ns(current));
```

**窄断言**：pid_vnr按调用者current命名空间对PID对象求公开编号。

固定time物理行 463–463；同一有效函数或宏定义范围连续1–5行。

## E135-012 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::get_task_pid]
```c
	pid = get_pid(rcu_dereference(*task_pid_ptr(task, type)));
```

**窄断言**：get_task_pid从所选PID槽经RCU读和get_pid请求取得临时引用。

固定time物理行 411–411；同一有效函数或宏定义范围连续1–5行。

## E135-013 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::get_task_pid]
```c
	return pid;
```

**窄断言**：get_task_pid返回所取得的PID对象指针。

固定time物理行 413–413；同一有效函数或宏定义范围连续1–5行。

## E135-014 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::get_pid]
```c
			if (pid) refcount_inc(&pid->count);
			return pid;
```

**窄断言**：get_pid对非空PID对象的count加一次引用并返回指针。

固定time物理行 62–63；同一有效函数或宏定义范围连续1–5行。

## E135-015 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::put_pid]
```c
	if (!pid)
		return;

	ns = pid->numbers[pid->level].ns;
	if (refcount_dec_and_test(&pid->count)) {
```

**窄断言**：put_pid对非空PID对象递减并测试是否为最后引用。

固定time物理行 115–119；同一有效函数或宏定义范围连续1–5行。

## E135-016 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.c::put_pid]
```c
		kmem_cache_free(ns->pid_cachep, pid);
		put_pid_ns(ns);
	}
```

**窄断言**：仅在递减测试为零时释放缓存并归还命名空间引用。

固定time物理行 120–122；同一有效函数或宏定义范围连续1–5行。

## E135-017 · 正式定义体短窗

[VERIFIED mykernel/sched/task/pid.h::pid_nr]
```c
			pid_t nr = 0;
			if (pid)
				nr = pid->numbers[0].nr;
			return nr;
```

**窄断言**：pid_nr读取PID对象numbers[0]标量编号，区别于pid_vnr。

固定time物理行 116–119；同一有效函数或宏定义范围连续1–5行。

## E135-018 · 正式定义体短窗

[VERIFIED mykernel/sched/syscall.c::fork]
```c
	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

**窄断言**：普通fork仅显式设置exit_signal=SIGCHLD、其余聚合成员零，返回kernel_clone的结果。

固定time物理行 60–64；同一有效函数或宏定义范围连续1–5行。

## E135-019 · 正式定义体短窗

[VERIFIED mykernel/sched/syscall.c::fork]
```c
	barrier();
	ulong counter = 0;
	for (long i = 0; i < 0x1000; i++)
		for (long j = 0; j < 0x1000; j++)
			counter++;
```

**窄断言**：普通fork有活动延时计数循环，此窗不产生fork返回PID。

固定time物理行 53–57；同一有效函数或宏定义范围连续1–5行。

## E135-020 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

**窄断言**：在dup_task_struct之前retval=-ENOMEM；得到NULL直接goto fork_out。

固定time物理行 958–961；同一有效函数或宏定义范围连续1–5行。

## E135-021 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```

**窄断言**：先清除PF_KTHREAD、仅args.kthread真时再置位。

固定time物理行 962–964；同一有效函数或宏定义范围连续1–5行。

## E135-022 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_thread(p, args);
	if (retval)
		goto bad_fork_cleanup_io;
```

**窄断言**：copy_process先请求copy_thread，失败进特定清理标签。

固定time物理行 1143–1145；同一有效函数或宏定义范围连续1–5行。

## E135-023 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (pid_struct != &init_struct_pid) {
		pid_struct = alloc_pid(p->nsproxy->pid_ns_for_children,args->set_tid, args->set_tid_size);
		if (IS_ERR(pid_struct)) {
			retval = PTR_ERR(pid_struct);
			goto bad_fork_cleanup_thread;
```

**窄断言**：在copy_thread之后普通场景请求alloc_pid及检测错误对象。

固定time物理行 1149–1153；同一有效函数或宏定义范围连续1–5行。

## E135-024 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->pid = pid_nr(pid_struct);
	if (clone_flags & CLONE_THREAD) {
		// p->group_leader = current->group_leader;
		p->tgid = current->tgid;
```

**窄断言**：从pid_nr赋K.pid；只有CLONE_THREAD时继承parent.tgid。

固定time物理行 1206–1209；同一有效函数或宏定义范围连续1–5行。

## E135-025 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	} else {
		// p->group_leader = p;
		p->tgid = p->pid;
	}
```

**窄断言**：普通非CLONE_THREAD将K.tgid赋成K.pid。

固定time物理行 1210–1213；同一有效函数或宏定义范围连续1–5行。

## E135-026 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (clone_flags & (CLONE_PARENT|CLONE_THREAD)) {
		p->real_parent = current->real_parent;
		// p->parent_exec_id = current->parent_exec_id;
```

**窄断言**：仅CLONE_PARENT或CLONE_THREAD时新任务real_parent继承原父。

固定time物理行 1267–1269；同一有效函数或宏定义范围连续1–5行。

## E135-027 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	} else {
		p->real_parent = current;
		// p->parent_exec_id = current->self_exec_id;
		// p->exit_signal = args->exit_signal;
```

**窄断言**：普通分支real_parent=current；exit_signal来自args的赋值仅注释。

固定time物理行 1274–1277；同一有效函数或宏定义范围连续1–5行。

## E135-028 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid]
```c
	if (type == PIDTYPE_PID)
		task->thread_pid = pid_struct;
	else
		task->signal->pids[type] = pid_struct;
```

**窄断言**：init_task_pid在PID分支写thread_pid，其他类型写signal.pids[type]。

固定time物理行 848–851；同一有效函数或宏定义范围连续1–5行。

## E135-029 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid_links]
```c
	for (type = PIDTYPE_PID; type < PIDTYPE_MAX; ++type)
		INIT_HLIST_NODE(&task->pid_links[type]);
```

**窄断言**：为每种PIDTYPE初始化任务链节点。

固定time物理行 842–843；同一有效函数或宏定义范围连续1–5行。

## E135-030 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	init_task_pid_links(p);
	if (likely(p->pid != 0)) {
		ptrace_init_task(p, (clone_flags & CLONE_PTRACE) || trace);

		init_task_pid(p, PIDTYPE_PID, pid_struct);
```

**窄断言**：先初始化链，若K.pid非零则设置PIDTYPE_PID槽。

固定time物理行 1310–1314；同一有效函数或宏定义范围连续1–5行。

## E135-031 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		if (thread_group_leader(p)) {
			init_task_pid(p, PIDTYPE_TGID, pid_struct);
			init_task_pid(p, PIDTYPE_PGID, task_pgrp(current));
			init_task_pid(p, PIDTYPE_SID, task_session(current));
```

**窄断言**：thread_group_leader为真时才设置TGID并请求PGID/SID槽。

固定time物理行 1315–1318；同一有效函数或宏定义范围连续1–5行。

## E135-032 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	return p;
```

**窄断言**：copy_process成功分支返回新任务指针p。

固定time物理行 1372–1372；同一有效函数或宏定义范围连续1–5行。

## E135-033 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
fork_out:
	spin_lock_irq(&current->sighand->siglock);
	// hlist_del_init(&delayed.node);
	spin_unlock_irq(&current->sighand->siglock);
	return ERR_PTR(retval);
```

**窄断言**：fork_out执行所示lock/unlock源码后返回ERR_PTR(retval)。

固定time物理行 1428–1432；同一有效函数或宏定义范围连续1–5行。

## E135-034 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
```

**窄断言**：kernel_clone向copy_process传NULL初始pid对象并取得任务或错误指针。

固定time物理行 1485–1485；同一有效函数或宏定义范围连续1–5行。

## E135-035 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (IS_ERR(p))
		return PTR_ERR(p);
```

**窄断言**：kernel_clone在IS_ERR(p)真时立即传播PTR_ERR(p)，不执行成功取号。

固定time物理行 1488–1489；同一有效函数或宏定义范围连续1–5行。

## E135-036 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

**窄断言**：成功后先借子PIDTYPE_PID对象再在父current视图取得nr。

固定time物理行 1497–1498；同一有效函数或宏定义范围连续1–5行。

## E135-037 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

**窄断言**：kernel_clone在取完nr后直接请求唤醒新任务，非实机观察。

固定time物理行 1516–1516；同一有效函数或宏定义范围连续1–5行。

## E135-038 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
```

**窄断言**：归还临时PID引用后返回此前复制的编号nr。

固定time物理行 1527–1529；同一有效函数或宏定义范围连续1–5行。

## E135-039 · 正式定义体短窗

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (!(clone_flags & CLONE_UNTRACED)) {
		if (clone_flags & CLONE_VFORK)
			trace = PTRACE_EVENT_VFORK;
		else if (args->exit_signal != SIGCHLD)
			trace = PTRACE_EVENT_CLONE;
```

**窄断言**：在非CLONE_UNTRACED条件下根据flags及args.exit_signal设置trace候选。

固定time物理行 1473–1477；同一有效函数或宏定义范围连续1–5行。

## E135-040 · 正式定义体短窗

[VERIFIED mykernel/lock_IPC/signal/signal.h::thread_group_leader]
```c
			return p->exit_signal >= 0;
```

**窄断言**：线程组长谓词读取p.exit_signal，不能由args.exit_signal直接推定。

固定time物理行 205–205；同一有效函数或宏定义范围连续1–5行。

## E135-041 · 正式定义体短窗

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		int				exit_signal;
```

**窄断言**：task_s有独立的exit_signal成员，被组长谓词读取。

固定time物理行 148–148；同一有效函数或宏定义范围连续1–5行。

## E135-042 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, fork_frame_s, regs);
	frame = &fork_frame->frame;
```

**窄断言**：copy_thread取得新任务的childregs、fork_frame和frame指针。

固定time物理行 92–94；同一有效函数或宏定义范围连续1–5行。

## E135-043 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bp = (reg_t)childregs + 1;
	frame->ret_addr = (reg_t)ret_from_fork_asm;
	p->thread.sp = (reg_t)fork_frame;
```

**窄断言**：copy_thread写初始返回入口和新线程的保存栈指针。

固定time物理行 97–99；同一有效函数或宏定义范围连续1–5行。

## E135-044 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
```

**窄断言**：只有PF_KTHREAD分支才清空childregs并按kthread初始化。

固定time物理行 120–124；同一有效函数或宏定义范围连续1–5行。

## E135-045 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;
```

**窄断言**：普通用户分支先复制当前寄存器帧，再将子帧ax单独置零；sp非零才覆盖sp。

固定time物理行 133–137；同一有效函数或宏定义范围连续1–5行。

## E135-046 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(args->fn)) {
```

**窄断言**：args.fn非空触发另一条栈/IP初始化分支，普通NULL不进入。

固定time物理行 139–139；同一有效函数或宏定义范围连续1–5行。

## E135-047 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
		childregs->sp = 0;
		childregs->ip = 0;
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
```

**窄断言**：只有上述fn非空分支才清sp/ip并调用kthread_frame_init。

固定time物理行 150–153；同一有效函数或宏定义范围连续1–5行。

## E135-048 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	return ret;
```

**窄断言**：普通copy_thread从有效路径返回局部ret。

固定time物理行 163–163；同一有效函数或宏定义范围连续1–5行。

## E135-049 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
	if (unlikely(fn)) {
		fn(fn_arg);
```

**窄断言**：ret_from_fork的fn非空分支才执行fn(fn_arg)。

固定time物理行 69–70；同一有效函数或宏定义范围连续1–5行。

## E135-050 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
		regs->ax = 0;
```

**窄断言**：另一个ax置零位于ret_from_fork的fn非空条件体内，不是普通copy_thread零。

固定time物理行 76–76；同一有效函数或宏定义范围连续1–5行。

## E135-051 · 正式定义体短窗

[VERIFIED mykernel/sched/syscall.c::getpid]
```c
	return task_tgid_vnr(current);
```

**窄断言**：getpid读取当前任务的线程组公开编号。

固定time物理行 47–47；同一有效函数或宏定义范围连续1–5行。

## E135-052 · 正式定义体短窗

[VERIFIED mykernel/sched/syscall.c::getppid]
```c
	pid = task_tgid_vnr(current->real_parent);
```

**窄断言**：getppid取当前任务real_parent为目标，观察者仍为执行current。

固定time物理行 120–120；同一有效函数或宏定义范围连续1–5行。

## E135-053 · 正式定义体短窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	int rv = fork();
	
	if (rv == 0) {
```

**窄断言**：init先fork并以rv==0分子分支。

固定time物理行 30–32；同一有效函数或宏定义范围连续1–5行。

## E135-054 · 正式定义体短窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		pid_t pid = getpid();
		// printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
```

**窄断言**：子分支getpid值存局部pid后请求execve，注释打印不是活动消费者。

固定time物理行 33–35；同一有效函数或宏定义范围连续1–5行。

## E135-055 · 正式定义体短窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	else {
		pid_t pid = getpid();
		// printf("parent task, %d\n", rv);
		while (1) {
			sched_yield();
```

**窄断言**：非零rv走else getpid保存局部并循环请求sched_yield。

固定time物理行 37–41；同一有效函数或宏定义范围连续1–5行。

## E135-056 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

**窄断言**：x64/其他ABI存根函数体将regs直接传给__se包装层。

固定time物理行 104–108；同一有效函数或宏定义范围连续1–5行。

## E135-057 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

**窄断言**：复合定义宏声明__se和__do并借__SYS_STUBx生成x64封装。

固定time物理行 241–245；同一有效函数或宏定义范围连续1–5行。

## E135-058 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

**窄断言**：__se包装层调用__do并返回ret，参数由__ARCH_MAP映射。

固定time物理行 246–249；同一有效函数或宏定义范围连续1–5行。

## E135-059 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
```

**窄断言**：复合宏末尾声明/引入真实__do查询体定义头。

固定time物理行 251–251；同一有效函数或宏定义范围连续1–5行。

## E135-060 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__X86_64_MAP0]
```c
	#define __X86_64_MAP0(m, ...)
```

**窄断言**：零参数的x86寄存器参数映射为空。

固定time物理行 14–14；同一有效函数或宏定义范围连续1–5行。

## E135-061 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__ARCH_MAP]
```c
	#define __ARCH_MAP(n, ...)	__X86_64_MAP##n(__VA_ARGS__)
```

**窄断言**：__ARCH_MAP按参数个数选择X86_64_MAPn。

固定time物理行 30–30；同一有效函数或宏定义范围连续1–5行。

## E135-062 · 正式定义体短窗

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP0]
```c
	#define __MAP0(m, ...)
```

**窄断言**：通用__MAP0为空，支持零参数__do声明。

固定time物理行 113–113；同一有效函数或宏定义范围连续1–5行。

## E135-063 · 正式定义体短窗

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE0]
```c
	#define MYOS_SYSCALL_DEFINE0(name, ...)	__MYOS_SYSCALL_DEFINEx(0, _##name, __VA_ARGS__)
```

**窄断言**：MYOS_SYSCALL_DEFINE0用零参数调用底层复合宏。

固定time物理行 1383–1383；同一有效函数或宏定义范围连续1–5行。

## E135-064 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

**窄断言**：x64_sys_call的登记宏生成按nr匹配的直接__x64调用。

固定time物理行 26–26；同一有效函数或宏定义范围连续1–5行。

## E135-065 · 正式定义体短窗

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

**窄断言**：x64_sys_call在switch中包含映射表，未匹配分默认路径。

固定time物理行 29–31；同一有效函数或宏定义范围连续1–5行。

## E135-066 · 正式定义体短窗

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

**窄断言**：固定time CMake正文定义架构包装器开关等，不认证真实编译。

固定time物理行 62–65；同一有效函数或宏定义范围连续1–5行。

## E135-067 · 独立定位

[SOURCE_LOCATION mykernel/include/linux/kernel/syscalls.h::wrapper_condition_header]
```c
	#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
	/*
```

**窄断言**：宏条件头，不计作函数体定义。

固定time物理行 93–94；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-068 · 独立定位

[SOURCE_LOCATION mykernel/include/linux/kernel/syscalls.h::wrapper_include]
```c
	#	include <asm/syscall_wrapper.h>
```

**窄断言**：条件包含指令定位。

固定time物理行 101–101；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-069 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::fork_header]
```c
MYOS_SYSCALL_DEFINE0(fork)
```

**窄断言**：函数入口宏调用而非内部赋值。

固定time物理行 51–51；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-070 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::fork_dispatch_mapping]
```c
__SYSCALL(__NR_fork, sys_fork)
```

**窄断言**：单独登记表项不是函数体调用。

固定time物理行 31–31；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-071 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::getpid_dispatch_mapping]
```c
__SYSCALL(__NR_getpid, sys_getpid)
```

**窄断言**：单独登记表项不是内核查询体。

固定time物理行 29–29；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-072 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::getppid_dispatch_mapping]
```c
__SYSCALL(__NR_getppid, sys_getppid)
```

**窄断言**：单独登记表项不是内核查询体。

固定time物理行 57–57；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-073 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::extern_context]
```c
#define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
#	include <asm/syscalls_64.h>
```

**窄断言**：文件范围原型及登记头纳入。

固定time物理行 10–11；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-074 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::table_context]
```c
#define __SYSCALL(nr, sym)	[nr] = __x64_##sym,
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
```

**窄断言**：静态表初始化不是运行调用。

固定time物理行 19–21；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-075 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::switch_context]
```c
	#include <asm/syscalls_64.h>
```

**窄断言**：switch内登记头纳入位置。

固定time物理行 30–30；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-076 · 独立定位

[SOURCE_LOCATION mykernel/sched/forkexec/fork.c::comment_exit_signal]
```c
		// p->exit_signal = args->exit_signal;
```

**窄断言**：注释不能推断已复制到任务字段。

固定time物理行 1277–1277；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-077 · 独立定位

[SOURCE_LOCATION mykernel/sched/forkexec/fork.c::copy_process_header]
```c
static __latent_entropy task_s
*copy_process(pid_s *pid_struct, int trace, int node, kclone_args_s *args)
```

**窄断言**：函数签名仅作定位。

固定time物理行 867–868；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-078 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread_header]
```c
int copy_thread(task_s *p, const kclone_args_s *args)
```

**窄断言**：函数签名仅作定位。

固定time物理行 82–82；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-079 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork_header]
```c
ret_from_fork(task_s *prev, pt_regs_s *regs,
		int (*fn)(void *), void *fn_arg)
```

**窄断言**：另一个ret_from_fork头仅作定位。

固定time物理行 63–64；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-080 · 独立定位

[SOURCE_LOCATION mykernel/scripts/options_flags.cmake::time_canary]
```c
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

**窄断言**：固定time基线金丝雀。

固定time物理行 41–41；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

## E135-081 · 独立定位

[SOURCE_LOCATION mykernel/debug/panic.c::time_canary]
```c
	this_cpu = smp_processor_id();
```

**窄断言**：固定time panic金丝雀。

固定time物理行 79–79；独立声明/函数头/注释/登记表项/配置金丝雀，不计正式源窗。

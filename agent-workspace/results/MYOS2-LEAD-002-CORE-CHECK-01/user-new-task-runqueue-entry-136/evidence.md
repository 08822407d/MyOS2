---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: source_window_evidence
date: 2026-11-23
produced_by: GPT-6（后端未认证）
model_per_owner: GPT-6＋Extra（Owner指定）
base_snapshot: time（分支名）
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---


# 第136批固定 time 源码引文

每条均取 time@a039d9803ade 同名符号定义体内的1–5行连续物理源码；行号只作辅助，正式锚定以 path::symbol 和逐字引文为准。没有实际运行。

## E136-001 default or explicit namespace and target slot

源码：`mykernel/sched/task/pid.c:473`。

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

## E136-002 indexed PID namespace value

源码：`mykernel/sched/task/pid.c:452`。

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
```

## E136-003 PIDTYPE writes different slots

源码：`mykernel/sched/forkexec/fork.c:848`。

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid]
```c
	if (type == PIDTYPE_PID)
		task->thread_pid = pid_struct;
	else
		task->signal->pids[type] = pid_struct;
```

## E136-004 outer nonzero PID gate

源码：`mykernel/sched/forkexec/fork.c:1311`。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (likely(p->pid != 0)) {
```

## E136-005 PID gate precedes independent leader gate

源码：`mykernel/sched/forkexec/fork.c:1314`。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		init_task_pid(p, PIDTYPE_PID, pid_struct);
		if (thread_group_leader(p)) {
			init_task_pid(p, PIDTYPE_TGID, pid_struct);
```

## E136-006 leader predicate

源码：`mykernel/lock_IPC/signal/signal.h:205`。

[VERIFIED mykernel/lock_IPC/signal/signal.h::thread_group_leader]
```c
			return p->exit_signal >= 0;
```

## E136-007 raw p checked not K

源码：`mykernel/sched/forkexec/fork.c:1488`。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (IS_ERR(p))
		return PTR_ERR(p);
```

## E136-008 read visible PID before wake

源码：`mykernel/sched/forkexec/fork.c:1497`。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

## E136-009 explicit wake call

源码：`mykernel/sched/forkexec/fork.c:1516`。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

## E136-010 return saved source nr

源码：`mykernel/sched/forkexec/fork.c:1527`。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
```

## E136-011 child ax assigned 0

源码：`mykernel/arch/x86_64/sched/context/process_arch.c:133`。

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
```

## E136-012 sched_fork called in copy

源码：`mykernel/sched/forkexec/fork.c:1104`。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = sched_fork(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_policy;
```

## E136-013 vruntime zero

源码：`mykernel/sched/scheduler/scheduler_core.c:582`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	p->se.vruntime			= 0;
```

## E136-014 live self-list init; time_slice comment only

源码：`mykernel/sched/scheduler/scheduler_core.c:599`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	// INIT_LIST_HEAD(&p->rt.run_list);
	INIT_LIST_S(&p->rt.run_list);
	// p->rt.timeout		= 0;
	// p->rt.time_slice	= sched_rr_timeslice;
```

## E136-015 TASK_NEW written

源码：`mykernel/sched/scheduler/scheduler_core.c:627`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::sched_fork]
```c
	p->__state = TASK_NEW;
```

## E136-016 on_cpu zero

源码：`mykernel/sched/scheduler/scheduler_core.c:670`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::sched_fork]
```c
	p->on_cpu = 0;
```

## E136-017 self-link new list node

源码：`mykernel/lib/list/double_list.h:150`。

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_S]
```c
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
```

## E136-018 TASK_NEW value

源码：`mykernel/sched/task/task_const.h:41`。

[VERIFIED mykernel/sched/task/task_const.h::TASK_NEW]
```c
	#define TASK_NEW				0x00000800
```

## E136-019 TASK_RUNNING value

源码：`mykernel/sched/task/task_const.h:26`。

[VERIFIED mykernel/sched/task/task_const.h::TASK_RUNNING]
```c
	#define TASK_RUNNING			0x00000000
```

## E136-020 fixed CPU0 rq

源码：`mykernel/sched/scheduler/scheduler_core.c:688`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	rq_s *rq = &(per_cpu(runqueues, 0));
```

## E136-021 new task state running

源码：`mykernel/sched/scheduler/scheduler_core.c:691`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	WRITE_ONCE(p->__state, TASK_RUNNING);
```

## E136-022 migration lock clock lines commented

源码：`mykernel/sched/scheduler/scheduler_core.c:702`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	// __set_task_cpu(p, select_task_rq(p, task_cpu(p), WF_FORK));
	// rq = __task_rq_lock(p, &rf);
	// update_rq_clock(rq);
```

## E136-023 general activation and preemption commented

源码：`mykernel/sched/scheduler/scheduler_core.c:707`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	// activate_task(rq, p, ENQUEUE_NOCLOCK);
	// trace_sched_wakeup_new(p);
	// check_preempt_curr(rq, p, WF_FORK);
```

## E136-024 distinct list header and node

源码：`mykernel/sched/scheduler/scheduler_core.c:721`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);
```

## E136-025 head insertion and count increment

源码：`mykernel/lib/list/double_list.h:647`。

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

## E136-026 delegate to two-neighbor insertion

源码：`mykernel/lib/list/double_list.h:209`。

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_next]
```c
			__list_add_between(new, head, head->next);
```

## E136-027 four ordered links

源码：`mykernel/lib/list/double_list.h:193`。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
```c
			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
```

## E136-028 elapsed jiffies

源码：`mykernel/sched/scheduler/myos_rt.c:12`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

## E136-029 picker short-circuit gate

源码：`mykernel/sched/scheduler/myos_rt.c:14`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

## E136-030 pop head K

源码：`mykernel/sched/scheduler/myos_rt.c:24`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
```

## E136-031 remove head then count decrement

源码：`mykernel/lib/list/double_list.h:653`。

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
```

## E136-032 old vruntime position comparison

源码：`mykernel/sched/scheduler/myos_rt.c:37`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

## E136-033 reinsert P and increment count

源码：`mykernel/sched/scheduler/myos_rt.c:44`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

## E136-034 charge vruntime after reinsert

源码：`mykernel/sched/scheduler/myos_rt.c:49`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

## E136-035 return task and last-jiffies update

源码：`mykernel/sched/scheduler/myos_rt.c:52`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
```

## E136-036 record selected current and return

源码：`mykernel/sched/scheduler/myos_rt.c:56`。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

## E136-037 insert before Q

源码：`mykernel/lib/list/double_list.h:221`。

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
```

## E136-038 removed node reinitialized

源码：`mykernel/lib/list/double_list.h:280`。

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
```

## 旧批固定实际载体（区别于内核代码窗）

135 facts 的 D00.R134_02.queries、A.copy_process.slots、C.C1/C2/C3 和 graph；135 DOT 的源行 626、632、714、720、732；以上旧件不改，使用新机件列出旧 ID/kind 与纠偏映射。

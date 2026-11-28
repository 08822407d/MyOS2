---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
record_type: "formal_source_windows"
date: "2026-11-23"
produced_by: "GPT-6（文本模型族；后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner要求）"
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# E137 源码证据窗口与 E136 完整纠偏索引

本批取自固定 time 的对应定义体，按物理行完整复制，每条正式窗口1–5行。原136旧窗物理文本38/38命中，不能据此称38条都构成有效活动代码；旧E136-022/023在新证据中仅为注释定位，旧E136-014拆出活动INIT_LIST_S。另列条件行而不把它们冒充实际预处理结果。

## E137-001 · default or explicit namespace and target slot

路径/符号：`mykernel/sched/task/pid.c::__task_pid_nr_ns`；物理行473–475；旧ID：E136-001。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

## E137-002 · indexed PID namespace value

路径/符号：`mykernel/sched/task/pid.c::pid_nr_ns`；物理行452–455；旧ID：E136-002。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
```

## E137-003 · PIDTYPE writes different slots

路径/符号：`mykernel/sched/forkexec/fork.c::init_task_pid`；物理行848–851；旧ID：E136-003。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid]
```c
	if (type == PIDTYPE_PID)
		task->thread_pid = pid_struct;
	else
		task->signal->pids[type] = pid_struct;
```

## E137-004 · outer nonzero PID gate

路径/符号：`mykernel/sched/forkexec/fork.c::copy_process`；物理行1311–1311；旧ID：E136-004。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (likely(p->pid != 0)) {
```

## E137-005 · PID gate precedes independent leader gate

路径/符号：`mykernel/sched/forkexec/fork.c::copy_process`；物理行1314–1316；旧ID：E136-005。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		init_task_pid(p, PIDTYPE_PID, pid_struct);
		if (thread_group_leader(p)) {
			init_task_pid(p, PIDTYPE_TGID, pid_struct);
```

## E137-006 · leader predicate

路径/符号：`mykernel/lock_IPC/signal/signal.h::thread_group_leader`；物理行205–205；旧ID：E136-006。以下只支持标题的窄断言。

[VERIFIED mykernel/lock_IPC/signal/signal.h::thread_group_leader]
```c
			return p->exit_signal >= 0;
```

## E137-007 · raw p checked not K

路径/符号：`mykernel/sched/forkexec/fork.c::kernel_clone`；物理行1488–1489；旧ID：E136-007。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (IS_ERR(p))
		return PTR_ERR(p);
```

## E137-008 · read visible PID before wake

路径/符号：`mykernel/sched/forkexec/fork.c::kernel_clone`；物理行1497–1498；旧ID：E136-008。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

## E137-009 · explicit wake call

路径/符号：`mykernel/sched/forkexec/fork.c::kernel_clone`；物理行1516–1516；旧ID：E136-009。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

## E137-010 · return saved source nr

路径/符号：`mykernel/sched/forkexec/fork.c::kernel_clone`；物理行1527–1529；旧ID：E136-010。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
```

## E137-011 · child ax assigned 0

路径/符号：`mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread`；物理行133–135；旧ID：E136-011。以下只支持标题的窄断言。

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
```

## E137-012 · sched_fork called in copy

路径/符号：`mykernel/sched/forkexec/fork.c::copy_process`；物理行1104–1106；旧ID：E136-012。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = sched_fork(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_policy;
```

## E137-013 · vruntime zero

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__sched_fork`；物理行582–582；旧ID：E136-013。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	p->se.vruntime			= 0;
```

## E137-014 · active INIT_LIST_S only; old comment is locator

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__sched_fork`；物理行600–600；旧ID：E136-014。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	INIT_LIST_S(&p->rt.run_list);
```

## E137-015 · TASK_NEW written

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::sched_fork`；物理行627–627；旧ID：E136-015。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::sched_fork]
```c
	p->__state = TASK_NEW;
```

## E137-016 · on_cpu zero

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::sched_fork`；物理行670–670；旧ID：E136-016。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::sched_fork]
```c
	p->on_cpu = 0;
```

## E137-017 · self-link new list node

路径/符号：`mykernel/lib/list/double_list.h::INIT_LIST_S`；物理行150–151；旧ID：E136-017。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_S]
```c
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
```

## E137-018 · TASK_NEW value

路径/符号：`mykernel/sched/task/task_const.h::TASK_NEW`；物理行41–41；旧ID：E136-018。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_const.h::TASK_NEW]
```c
	#define TASK_NEW				0x00000800
```

## E137-019 · TASK_RUNNING value

路径/符号：`mykernel/sched/task/task_const.h::TASK_RUNNING`；物理行26–26；旧ID：E136-019。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_const.h::TASK_RUNNING]
```c
	#define TASK_RUNNING			0x00000000
```

## E137-020 · fixed CPU0 rq

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task`；物理行688–688；旧ID：E136-020。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	rq_s *rq = &(per_cpu(runqueues, 0));
```

## E137-021 · new task state running

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task`；物理行691–691；旧ID：E136-021。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	WRITE_ONCE(p->__state, TASK_RUNNING);
```

## E137-022 · distinct list header and node

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task`；物理行721–721；旧ID：E136-024。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);
```

## E137-023 · head insertion and count increment

路径/符号：`mykernel/lib/list/double_list.h::list_header_add_to_head`；物理行647–648；旧ID：E136-025。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

## E137-024 · delegate to two-neighbor insertion

路径/符号：`mykernel/lib/list/double_list.h::list_add_to_next`；物理行209–209；旧ID：E136-026。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_next]
```c
			__list_add_between(new, head, head->next);
```

## E137-025 · four ordered links

路径/符号：`mykernel/lib/list/double_list.h::__list_add_between`；物理行193–196；旧ID：E136-027。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
```c
			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
```

## E137-026 · elapsed jiffies

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行12–12；旧ID：E136-028。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

## E137-027 · picker short-circuit gate

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行14–18；旧ID：E136-029。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

## E137-028 · pop head K

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行24–24；旧ID：E136-030。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
```

## E137-029 · remove head then count decrement

路径/符号：`mykernel/lib/list/double_list.h::list_header_remove_head`；物理行653–657；旧ID：E136-031。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
```

## E137-030 · old vruntime position comparison

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行37–40；旧ID：E136-032。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

## E137-031 · reinsert P and increment count

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行44–45；旧ID：E136-033。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

## E137-032 · charge vruntime after reinsert

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行49–50；旧ID：E136-034。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

## E137-033 · return task and last-jiffies update

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行52–54；旧ID：E136-035。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
```

## E137-034 · record selected current and return

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行56–57；旧ID：E136-036。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

## E137-035 · insert before Q

路径/符号：`mykernel/lib/list/double_list.h::list_add_to_prev`；物理行221–222；旧ID：E136-037。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
```

## E137-036 · removed node reinitialized

路径/符号：`mykernel/lib/list/double_list.h::list_del_init`；物理行280–282；旧ID：E136-038。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
```

## E137-037 · whole task structure value-copy

路径/符号：`mykernel/sched/forkexec/fork.c::dup_task_struct`；物理行288–288；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
```

## E137-038 · task allocation before copy

路径/符号：`mykernel/sched/forkexec/fork.c::dup_task_struct`；物理行280–282；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;
```

## E137-039 · copy_process calls dup_task_struct

路径/符号：`mykernel/sched/forkexec/fork.c::copy_process`；物理行958–961；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

## E137-040 · child three CPU times reset

路径/符号：`mykernel/sched/forkexec/fork.c::copy_process`；物理行1026–1026；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->utime = p->stime = p->gtime = 0;
```

## E137-041 · task state member

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行18–19；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		thread_info_s	thread_info;
		uint			__state;
```

## E137-042 · on_cpu member

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行38–38；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		int				on_cpu;
```

## E137-043 · embedded scheduler entities

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行60–61；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		sched_entity_s		se;
		sched_rt_entity_s	rt;
```

## E137-044 · utime stime members

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行267–268；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		u64				utime;
		u64				stime;
```

## E137-045 · gtime member

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行273–273；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		u64				gtime;
```

## E137-046 · nvcsw nivcsw fields

路径/符号：`mykernel/sched/task/task_types.h::task_struct`；物理行283–284；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		ulong			nvcsw;
		ulong			nivcsw;
```

## E137-047 · vruntime field

路径/符号：`mykernel/sched/scheduler/scheduler_types.h::sched_entity`；物理行17–17；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_types.h::sched_entity]
```c
		u64				vruntime;
```

## E137-048 · rt run_list and time_slice fields

路径/符号：`mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity`；物理行42–46；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity]
```c
	struct sched_rt_entity {
		List_s		run_list;
		// unsigned long timeout;
		// unsigned long watchdog_stamp;
		uint		time_slice;
```

## E137-049 · running header and last jiffies

路径/符号：`mykernel/sched/runqueue/runqueue_types.h::myos_rq`；物理行193–197；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::myos_rq]
```c
	struct myos_rq {
		// MyOS2 variables
		List_hdr_s		running_lhdr;
		ulong			last_jiffies;	// abs jiffies when curr-task loaded
	};
```

## E137-050 · rq nr_switches field

路径/符号：`mykernel/sched/runqueue/runqueue_types.h::runqueue`；物理行229–229；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::runqueue]
```c
		u64				nr_switches;
```

## E137-051 · rq curr and idle fields

路径/符号：`mykernel/sched/runqueue/runqueue_types.h::runqueue`；物理行257–258；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::runqueue]
```c
		task_s __rcu	*curr;
		task_s			*idle;
```

## E137-052 · six invalid predicates in while

路径/符号：`mykernel/lib/list/double_list.h::__list_add_valid`；物理行163–165；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

## E137-053 · validator true return

路径/符号：`mykernel/lib/list/double_list.h::__list_add_valid`；物理行167–167；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			return true;
```

## E137-054 · outer if not valid early return

路径/符号：`mykernel/lib/list/double_list.h::__list_add_between`；物理行190–191；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
```c
			if (!__list_add_valid(new, prev, next))
				return;
```

## E137-055 · rq clock uses local_clock delta

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::update_rq_clock`；物理行24–27；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

## E137-056 · prev from rq curr

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1032–1034；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;
```

## E137-057 · rq clock and prev nivcsw pointer

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1065–1069；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_count = &prev->nivcsw;
```

## E137-058 · SM_NONE comparison for preempt

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1072–1072；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	preempt = sched_mode == SM_PREEMPT;
```

## E137-059 · read prev state

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1078–1078；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	prev_state = READ_ONCE(prev->__state);
```

## E137-060 · pick_next_task and clear flags

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1116–1119；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	next = pick_next_task(rq, prev, &rf);
picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
```

## E137-061 · prev != next and nr_switches

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1124–1125；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	if (likely(prev != next)) {
		rq->nr_switches++;
```

## E137-062 · RCU pointer write call

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1130–1130；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		RCU_INIT_POINTER(rq->curr, next);
```

## E137-063 · increment selected prev count

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1153–1153；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		++*switch_count;
```

## E137-064 · context_switch call boundary

路径/符号：`mykernel/sched/scheduler/scheduler_core.c::__schedule`；物理行1162–1162；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		rq = context_switch(rq, prev, next, &rf);
```

## E137-065 · scheduler class traversal

路径/符号：`mykernel/sched/scheduler/scheduler.h::__pick_next_task`；物理行258–258；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
			for_each_class(class) {
```

## E137-066 · indirect class picker and return

路径/符号：`mykernel/sched/scheduler/scheduler.h::__pick_next_task`；物理行264–266；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
```

## E137-067 · picker uses current rt

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行9–10；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	retval = curr_task = current;
	sched_rt_entity_s *rt = &curr_task->rt;
```

## E137-068 · BUG_ON guard expression

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行20–21；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		BUG_ON(curr_task->__state != TASK_RUNNING &&
				myos_rq->running_lhdr.count <= 0);
```

## E137-069 · running requeue gate

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行28–28；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task->__state == TASK_RUNNING)
```

## E137-070 · idle check

路径/符号：`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`；物理行30–30；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
			if (curr_task == rq->idle)
```

## E137-071 · RCU_INIT_POINTER WRITE_ONCE macro

路径/符号：`mykernel/lib/list/rcu_list_macro.h::RCU_INIT_POINTER`；物理行479–482；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/lib/list/rcu_list_macro.h::RCU_INIT_POINTER]
```c
	#define RCU_INIT_POINTER(p, v)		\
			do {						\
				WRITE_ONCE(p, v);		\
			} while (0)
```

## E137-072 · BUG_ON macro guarded by CONFIG_BUG

路径/符号：`mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON`；物理行66–66；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

## E137-073 · CMake -DCONFIG_BUG setting

路径/符号：`mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS`；物理行64–66；旧ID：本批新增。以下只支持标题的窄断言。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

## 独立注释/条件位置（不计正式锚）

- `L137-COMMENT-14`：`mykernel/sched/scheduler/scheduler_core.c` 599;601–604，comment_only；active INIT_LIST_S is separate from commented time_slice/on_rq
- `L137-COMMENT-22`：`mykernel/sched/scheduler/scheduler_core.c` 702–704，comment_only；CPU selection, lock, clock comments only
- `L137-COMMENT-23`：`mykernel/sched/scheduler/scheduler_core.c` 707–709，comment_only；activate_task/check_preempt_curr comments only
- `L137-LIST-CONDITION`：`mykernel/lib/list/double_list.h` 138，preprocessor_condition_not_runtime；文本：	#if defined(LIST_DEFINATION) || !(DEBUG)
- `L137-LIST-DEFINITION`：`mykernel/lib/list/double_list.c` 1–2，independent_source_location；文本：#define LIST_DEFINATION\n#include "double_list.h"
- `L137-BUG-CONDITION`：`mykernel/include/linux/kernel/asm-generic/bug.h` 24，preprocessor_condition_not_runtime；文本：	#	ifdef CONFIG_BUG
- `L137-SCHED-COMMENT`：`mykernel/sched/scheduler/scheduler_core.c` 601–603，comment_only；文本：	// p->rt.timeout		= 0; | 	// p->rt.time_slice	= sched_rr_timeslice; | 	// p->rt.on_rq		= 0;

## E136 全部38窗口一对一对照

| 旧 ID | 原符号 / 行 | 新ID或定位 | 处置 |
|---|---|---|---|
| E136-001 | __task_pid_nr_ns:473 | E137-001 | REANCHOR_ACTIVE |
| E136-002 | pid_nr_ns:452 | E137-002 | REANCHOR_ACTIVE |
| E136-003 | init_task_pid:848 | E137-003 | REANCHOR_ACTIVE |
| E136-004 | copy_process:1311 | E137-004 | REANCHOR_ACTIVE |
| E136-005 | copy_process:1314 | E137-005 | REANCHOR_ACTIVE |
| E136-006 | thread_group_leader:205 | E137-006 | REANCHOR_ACTIVE |
| E136-007 | kernel_clone:1488 | E137-007 | REANCHOR_ACTIVE |
| E136-008 | kernel_clone:1497 | E137-008 | REANCHOR_ACTIVE |
| E136-009 | kernel_clone:1516 | E137-009 | REANCHOR_ACTIVE |
| E136-010 | kernel_clone:1527 | E137-010 | REANCHOR_ACTIVE |
| E136-011 | copy_thread:133 | E137-011 | REANCHOR_ACTIVE |
| E136-012 | copy_process:1104 | E137-012 | REANCHOR_ACTIVE |
| E136-013 | __sched_fork:582 | E137-013 | REANCHOR_ACTIVE |
| E136-014 | __sched_fork:599 | E137-014 | SPLIT_ACTIVE_COMMENT |
| E136-015 | sched_fork:627 | E137-015 | REANCHOR_ACTIVE |
| E136-016 | sched_fork:670 | E137-016 | REANCHOR_ACTIVE |
| E136-017 | INIT_LIST_S:150 | E137-017 | REANCHOR_ACTIVE |
| E136-018 | TASK_NEW:41 | E137-018 | REANCHOR_ACTIVE |
| E136-019 | TASK_RUNNING:26 | E137-019 | REANCHOR_ACTIVE |
| E136-020 | wake_up_new_task:688 | E137-020 | REANCHOR_ACTIVE |
| E136-021 | wake_up_new_task:691 | E137-021 | REANCHOR_ACTIVE |
| E136-022 | wake_up_new_task:702 | L137-COMMENT-22 | COMMENT_ONLY_NOT_FORMAL |
| E136-023 | wake_up_new_task:707 | L137-COMMENT-23 | COMMENT_ONLY_NOT_FORMAL |
| E136-024 | wake_up_new_task:721 | E137-022 | REANCHOR_ACTIVE |
| E136-025 | list_header_add_to_head:647 | E137-023 | REANCHOR_ACTIVE |
| E136-026 | list_add_to_next:209 | E137-024 | REANCHOR_ACTIVE |
| E136-027 | __list_add_between:193 | E137-025 | REANCHOR_ACTIVE |
| E136-028 | pick_next_task_myos:12 | E137-026 | REANCHOR_ACTIVE |
| E136-029 | pick_next_task_myos:14 | E137-027 | REANCHOR_ACTIVE |
| E136-030 | pick_next_task_myos:24 | E137-028 | REANCHOR_ACTIVE |
| E136-031 | list_header_remove_head:653 | E137-029 | REANCHOR_ACTIVE |
| E136-032 | pick_next_task_myos:37 | E137-030 | REANCHOR_ACTIVE |
| E136-033 | pick_next_task_myos:44 | E137-031 | REANCHOR_ACTIVE |
| E136-034 | pick_next_task_myos:49 | E137-032 | REANCHOR_ACTIVE |
| E136-035 | pick_next_task_myos:52 | E137-033 | REANCHOR_ACTIVE |
| E136-036 | pick_next_task_myos:56 | E137-034 | REANCHOR_ACTIVE |
| E136-037 | list_add_to_prev:221 | E137-035 | REANCHOR_ACTIVE |
| E136-038 | list_del_init:280 | E137-036 | REANCHOR_ACTIVE |

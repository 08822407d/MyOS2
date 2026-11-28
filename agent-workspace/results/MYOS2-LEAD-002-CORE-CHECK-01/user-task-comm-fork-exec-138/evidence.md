---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-TASK-COMM-FORK-EXEC-138
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: source_evidence
date: 2026-11-23
produced_by: "GPT-6（模型族可识别，后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第138批 · 新增正式源码窗及定位

正式窗逐字来自固定time符号定义体内部，1–5行。冻结E137的73/73物理文本在本批独立重读，完整差异索引置于facts.D00.R137_02；旧73不当作本批新的正式引文计数。

## E138-001 · TASK_COMM_LEN=32

`mykernel/sched/task/task_const.h::TASK_COMM_LEN`，物理行11–11。

[VERIFIED mykernel/sched/task/task_const.h::TASK_COMM_LEN]
```c
		TASK_COMM_LEN = 32,
```

## E138-002 · comm inline char array

`mykernel/sched/task/task_types.h::task_struct`，物理行326–326。

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		char			comm[TASK_COMM_LEN];
```

## E138-003 · whole task value copied

`mykernel/sched/forkexec/fork.c::dup_task_struct`，物理行288–289。

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```

## E138-004 · calls dup_task_struct(current,node)

`mykernel/sched/forkexec/fork.c::copy_process`，物理行958–961。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

## E138-005 · args.name name override gate

`mykernel/sched/forkexec/fork.c::copy_process`，物理行976–977。

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (args->name)
		strscpy_pad(p->comm, args->name, sizeof(p->comm));
```

## E138-006 · ordinary fork args exitsignal

`mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)`，物理行60–64。

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)]
```c
	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

## E138-007 · kernel_thread argument aggregate begins

`mykernel/sched/forkexec/fork.c::kernel_thread`，物理行1538–1539。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
	kclone_args_s args = {
		.flags			= ((lower_32_bits(flags) | CLONE_VM |
```

## E138-008 · kernel_thread name pointer passed

`mykernel/sched/forkexec/fork.c::kernel_thread`，物理行1542–1545。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.fn				= fn,
		.fn_arg			= arg,
		.name			= name,
		.kthread		= 1,
```

## E138-009 · kernel_thread calls kernel_clone

`mykernel/sched/forkexec/fork.c::kernel_thread`，物理行1548–1550。

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
	};

	return kernel_clone(&args);
```

## E138-010 · strscpy res and count initialization

`mykernel/lib/string/string.c::strscpy`，物理行172–176。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
	ssize_t strscpy(char *dest, const char *src, size_t count)
	{
		// const struct word_at_a_time constants = WORD_AT_A_TIME_CONSTANTS;
		size_t max = count;
		long res = 0;
```

## E138-011 · strscpy rejects zero count

`mykernel/lib/string/string.c::strscpy`，物理行178–179。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		if (count == 0 || WARN_ON_ONCE(count > INT_MAX))
			return -E2BIG;
```

## E138-012 · strscpy copies one byte

`mykernel/lib/string/string.c::strscpy`，物理行213–217。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		while (count) {
			char c;

			c = src[res];
			dest[res] = c;
```

## E138-013 · strscpy detects NUL or increments

`mykernel/lib/string/string.c::strscpy`，物理行218–222。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
			if (!c)
				return res;
			res++;
			count--;
		}
```

## E138-014 · strscpy overwrites last byte when truncated

`mykernel/lib/string/string.c::strscpy`，物理行224–228。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		/* Hit buffer length without finding a NUL; force NUL-termination. */
		if (res)
			dest[res-1] = '\0';

		return -E2BIG;
```

## E138-015 · pad wrapper calls inner strscpy

`mykernel/lib/string/string_helpers.c::strscpy_pad`，物理行831–835。

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
ssize_t strscpy_pad(char *dest, const char *src, size_t count)
{
	ssize_t written;

	written = strscpy(dest, src, count);
```

## E138-016 · pad wrapper early return for negative/full

`mykernel/lib/string/string_helpers.c::strscpy_pad`，物理行835–837。

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	written = strscpy(dest, src, count);
	if (written < 0 || written == count - 1)
		return written;
```

## E138-017 · pad wrapper zeros trailing bytes

`mykernel/lib/string/string_helpers.c::strscpy_pad`，物理行839–841。

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	memset(dest + written + 1, 0, count - written - 1);

	return written;
```

## E138-018 · get_task_comm locks tsk and copies into buf

`mykernel/sched/forkexec/exec.c::__get_task_comm`，物理行820–824。

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
char *__get_task_comm(char *buf, size_t buf_size, task_s *tsk)
{
	task_lock(tsk);
	/* Always NUL terminated and zero-padded */
	strscpy_pad(buf, tsk->comm, buf_size);
```

## E138-019 · getter unlocks tsk and returns buf

`mykernel/sched/forkexec/exec.c::__get_task_comm`，物理行824–826。

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
	strscpy_pad(buf, tsk->comm, buf_size);
	task_unlock(tsk);
	return buf;
```

## E138-020 · setter void and task lock

`mykernel/sched/forkexec/exec.c::__set_task_comm`，物理行836–838。

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
void __set_task_comm(task_s *tsk, const char *buf)
{
	task_lock(tsk);
```

## E138-021 · setter copies to task inline comm and unlocks

`mykernel/sched/forkexec/exec.c::__set_task_comm`，物理行840–841。

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
```

## E138-022 · begin_new_exec me=current

`mykernel/sched/forkexec/exec.c::begin_new_exec`，物理行851–854。

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
int begin_new_exec(linux_bprm_s * bprm)
{
	task_s *me = current;
	int retval;
```

## E138-023 · setter receives bprm filename

`mykernel/sched/forkexec/exec.c::begin_new_exec`，物理行979–979。

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	__set_task_comm(me, bprm->filename);
```

## E138-024 · fd AT_FDCWD chooses filename name

`mykernel/sched/forkexec/exec.c::alloc_bprm`，物理行1117–1119。

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```

## E138-025 · interp initially aliases filename pointer

`mykernel/sched/forkexec/exec.c::alloc_bprm`，物理行1140–1142。

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		bprm->filename = bprm->fdpath;
	}
	bprm->interp = bprm->filename;
```

## E138-026 · do_execve calls do_execveat_common AT_FDCWD

`mykernel/sched/forkexec/exec.c::do_execve`，物理行1438–1442。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execve]
```c
do_execve(filename_s *filename,
		const char *const *__argv,
		const char *const *__envp) {
	return do_execveat_common(AT_FDCWD,
			filename, __argv, __envp, 0);
```

## E138-027 · execve calls do_execve getname

`mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve,`，物理行1460–1463。

[VERIFIED mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve,]
```c
MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
```

## E138-028 · error name pointer early PTR_ERR return

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1297–1298。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (IS_ERR(filename))
		return PTR_ERR(filename);
```

## E138-029 · valid name reaches alloc_bprm

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1314–1316。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	current->flags &= ~PF_NPROC_EXCEEDED;

	bprm = alloc_bprm(fd, filename, flags);
```

## E138-030 · task_lock holds p.alloc_lock

`mykernel/sched/task/task.h::task_lock`，物理行93–95。

[VERIFIED mykernel/sched/task/task.h::task_lock]
```c
		task_lock(task_s *p) {
			spin_lock(&p->alloc_lock);
		}
```

## E138-031 · task_unlock releases p.alloc_lock

`mykernel/sched/task/task.h::task_unlock`，物理行98–100。

[VERIFIED mykernel/sched/task/task.h::task_unlock]
```c
		task_unlock(task_s *p) {
			spin_unlock(&p->alloc_lock);
		}
```

## E138-032 · rq clock local_clock sample

`mykernel/sched/scheduler/scheduler_core.c::update_rq_clock`，物理行20–24。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
void update_rq_clock(rq_s *rq)
{
	s64 delta;

	delta = local_clock() - rq->clock;
```

## E138-033 · clock negative delta skips additive write

`mykernel/sched/scheduler/scheduler_core.c::update_rq_clock`，物理行24–27。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

## E138-034 · clock_update_flags written around call

`mykernel/sched/scheduler/scheduler_core.c::__schedule`，物理行1064–1067。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

## E138-035 · used jiffies separate arithmetic

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行12–12。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

## E138-036 · picker full OR including task state test

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行14–18。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

## E138-037 · picker curr and return

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行56–57。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

## E138-038 · run_list active member E137-048 split

`mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity`，物理行43–43。

[VERIFIED mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity]
```c
		List_s		run_list;
```

## E138-039 · time_slice active member E137-048 split

`mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity`，物理行46–46。

[VERIFIED mykernel/sched/scheduler/scheduler_types.h::sched_rt_entity]
```c
		uint		time_slice;
```

## E138-040 · running_lhdr and last_jiffies active E137-049 split

`mykernel/sched/runqueue/runqueue_types.h::myos_rq`，物理行195–196。

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::myos_rq]
```c
		List_hdr_s		running_lhdr;
		ulong			last_jiffies;	// abs jiffies when curr-task loaded
```

## E138-041 · generic head removal can pick K or Q

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行24–25。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);
```

## E138-042 · generic reinsert is case-specific current

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行44–45。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

## E138-043 · generic before-node insertion

`mykernel/lib/list/double_list.h::list_add_to_prev`，物理行221–222。

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
```

## E138-044 · retval assignment and last write not return statement

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行52–54。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
```

## E138-045 · actual return statement

`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，物理行56–57。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

## E138-046 · head removal calls list_del_init and decreases count

`mykernel/lib/list/double_list.h::list_header_remove_head`，物理行653–657。

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
```

## E138-047 · removed node selflinks

`mykernel/lib/list/double_list.h::list_del_init`，物理行280–282。

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
```

## 非正式源码定位：声明、注释与条件

- `L138-STRSCPY-DECL`：declaration，`mykernel/include/linux/lib/string.h:31`；`		ssize_t strscpy(char *, const char *, size_t);`。
- `L138-PAD-DECL`：declaration，`mykernel/include/linux/lib/string.h:35`；`	ssize_t strscpy_pad(char *dest, const char *src, size_t count);`。
- `L138-STRSCPY-BRANCH`：preprocessor，`mykernel/lib/string/string.c:181`；`	#ifdef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS`。
- `L138-COMM-COMMENT`：comment-not-activity，`mykernel/sched/task/task_types.h:320`；`		 * executable name, excluding path. / 		 * / 		 * - normally initialized setup_new_exec() / 		 * - access it with [gs]et_task_comm() / 		 * - lock it with task_lock()`。
- `L138-E048-COMMENT`：comment-only，`mykernel/sched/scheduler/scheduler_types.h:44`；`		// unsigned long timeout; / 		// unsigned long watchdog_stamp;`。
- `L138-E049-COMMENT`：comment-only，`mykernel/sched/runqueue/runqueue_types.h:194`；`		// MyOS2 variables`。
- `L138-CMAKE`：config-only，`mykernel/scripts/options_flags.cmake:64`；`	-DCONFIG_SLUB \ / 	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \ / 	-DCONFIG_BUG \`。

## 原 E137 特别重锚（本文编号可独立使用）

E137-048 已拆成E138-038与E138-039，旧注释留L138-E048-COMMENT；E137-049仅取E138-040活动成员，旧注释留L138-E049-COMMENT。旧E137-028/031/035需按具体样本分别为136的K/P/Q与137的Q/K/R代入，参E138-041、E138-042、E138-043。旧E137-033是赋值及last写（E138-044），实际return由E138-045给出。事实文件的old_window_crosswalk覆盖全部73条，标记旧事实机件遗漏的21个ID。
